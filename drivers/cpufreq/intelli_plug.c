/*
 * Author: Paul Reioux aka Faux123 <reioux@gmail.com>
 *         Park Ju Hyung aka arter97 <qkrwngud825@gmail.com>
 *
 * Copyright 2012~2014 Paul Reioux
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/workqueue.h>
#include <linux/cpu.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/cpufreq.h>
#include <linux/fb.h>

//#define DEBUG_INTELLI_PLUG
#undef DEBUG_INTELLI_PLUG

#define INTELLI_PLUG_MAJOR_VERSION	4
#define INTELLI_PLUG_MINOR_VERSION	0

#define DEF_SAMPLING_MS			(268)

#define DUAL_PERSISTENCE		(2500 / DEF_SAMPLING_MS)
#define TRI_PERSISTENCE			(1700 / DEF_SAMPLING_MS)
#define QUAD_PERSISTENCE		(1000 / DEF_SAMPLING_MS)

#define BUSY_PERSISTENCE		(3500 / DEF_SAMPLING_MS)

static DEFINE_MUTEX(intelli_plug_mutex);

static struct delayed_work intelli_plug_work;
static struct workqueue_struct *intelli_plug_wq;
static struct workqueue_struct *intelli_plug_cac_wq;

static unsigned int __read_mostly intelli_plug_active = 0;
static unsigned int __read_mostly nr_run_profile_sel = 3;

//default to something sane rather than zero
static unsigned int __read_mostly sampling_time = DEF_SAMPLING_MS;

static int persist_count = 0;

static bool __read_mostly cac_bool = true;

struct ip_cpu_info {
	unsigned int sys_max;
	unsigned int cur_max;
	unsigned long cpu_nr_running;
};

static DEFINE_PER_CPU(struct ip_cpu_info, ip_info);

#define CAPACITY_RESERVE	50
#define THREAD_CAPACITY		(265 - CAPACITY_RESERVE) // Exynos 7420 A53

#define MULT_FACTOR	4
#define DIV_FACTOR	100000
#define NR_FSHIFT	3

static unsigned int nr_fshift = NR_FSHIFT;

static unsigned int __read_mostly nr_run_thresholds_balance[] = {
	(THREAD_CAPACITY * 625 * MULT_FACTOR) / DIV_FACTOR,
	(THREAD_CAPACITY * 875 * MULT_FACTOR) / DIV_FACTOR,
	(THREAD_CAPACITY * 1125 * MULT_FACTOR) / DIV_FACTOR,
	UINT_MAX
};

static unsigned int __read_mostly nr_run_thresholds_performance[] = {
	(THREAD_CAPACITY * 380 * MULT_FACTOR) / DIV_FACTOR,
	(THREAD_CAPACITY * 625 * MULT_FACTOR) / DIV_FACTOR,
	(THREAD_CAPACITY * 875 * MULT_FACTOR) / DIV_FACTOR,
	UINT_MAX
};

static unsigned int __read_mostly nr_run_thresholds_conservative[] = {
	(THREAD_CAPACITY * 875 * MULT_FACTOR) / DIV_FACTOR,
	(THREAD_CAPACITY * 1625 * MULT_FACTOR) / DIV_FACTOR,
	(THREAD_CAPACITY * 2125 * MULT_FACTOR) / DIV_FACTOR,
	UINT_MAX
};

static unsigned int __read_mostly nr_run_thresholds_eco[] = {
        (THREAD_CAPACITY * 380 * MULT_FACTOR) / DIV_FACTOR,
	UINT_MAX
};

static unsigned int __read_mostly nr_run_thresholds_eco_extreme[] = {
        (THREAD_CAPACITY * 750 * MULT_FACTOR) / DIV_FACTOR,
	UINT_MAX
};

static unsigned int __read_mostly nr_run_thresholds_disable[] = {
	0,  0,  0,  UINT_MAX
};

static unsigned int __read_mostly *nr_run_profiles[] = {
	nr_run_thresholds_balance,
	nr_run_thresholds_performance,
	nr_run_thresholds_conservative,
	nr_run_thresholds_eco,
	nr_run_thresholds_eco_extreme,
	nr_run_thresholds_disable,
};

#define NR_RUN_ECO_MODE_PROFILE	3
#define NR_RUN_HYSTERESIS_QUAD	8
#define NR_RUN_HYSTERESIS_DUAL	4

#define CPU_NR_THRESHOLD	((THREAD_CAPACITY << 1) + (THREAD_CAPACITY / 2))

static unsigned int __read_mostly cpu_nr_run_threshold = CPU_NR_THRESHOLD;
static unsigned int __read_mostly nr_run_hysteresis = NR_RUN_HYSTERESIS_DUAL;

static unsigned int nr_run_last;

extern unsigned long avg_nr_running(void);
extern unsigned long avg_cpu_nr_running(unsigned int cpu);

static unsigned int calculate_thread_stats(void)
{
	unsigned int avg_nr_run = avg_nr_running();
	unsigned int nr_run;
	unsigned int threshold_size;
	unsigned int *current_profile;

	current_profile = nr_run_profiles[nr_run_profile_sel];
	threshold_size = ARRAY_SIZE(nr_run_thresholds_eco);

	nr_fshift = 1;

	for (nr_run = 1; nr_run < threshold_size; nr_run++) {
		unsigned int nr_threshold;
		nr_threshold = current_profile[nr_run - 1];

		if (nr_run_last <= nr_run)
			nr_threshold += nr_run_hysteresis;
		if (avg_nr_run <= (nr_threshold << (FSHIFT - nr_fshift)))
			break;
	}
	nr_run_last = nr_run;

	return nr_run;
}

/*
static int cmp_nr_running(const void *a, const void *b)
{
	return *(unsigned long *)a - *(unsigned long *)b;
}
*/

static void update_per_cpu_stat(void)
{
	unsigned int cpu;
	struct ip_cpu_info *l_ip_info;

	for_each_online_cpu(cpu) {
		if (cpu >= 2)
			continue;

		l_ip_info = &per_cpu(ip_info, cpu);
		l_ip_info->cpu_nr_running = avg_cpu_nr_running(cpu);
#ifdef DEBUG_INTELLI_PLUG
		pr_info("cpu %u nr_running => %lu\n", cpu,
			l_ip_info->cpu_nr_running);
#endif
	}
}

static int num_online_cpus_little(void)
{
	unsigned int cpu;
	int ret = 1;

	for_each_online_cpu(cpu) {
		if (cpu >= 2 || cpu == 0)
			continue;
		ret++;
	}
	return ret;
}

static void unplug_cpu(int min_active_cpu)
{
	unsigned int cpu;
	struct ip_cpu_info *l_ip_info;
	int l_nr_threshold;

	for_each_online_cpu(cpu) {
		if (cpu >= 2 || cpu == 0)
			continue;

		l_nr_threshold =
			cpu_nr_run_threshold << 1 / (num_online_cpus_little());
		l_ip_info = &per_cpu(ip_info, cpu);
		if (cpu > min_active_cpu)
			if (l_ip_info->cpu_nr_running < l_nr_threshold)
				cpu_down(cpu);
	}
}

static void __ref intelli_plug_work_fn(struct work_struct *work)
{
	unsigned int nr_run_stat;
	unsigned int cpu_count = 0;
	unsigned int nr_cpus = 0;

	if (intelli_plug_active) {
		nr_run_stat = calculate_thread_stats();
		update_per_cpu_stat();
#ifdef DEBUG_INTELLI_PLUG
		pr_info("nr_run_stat: %u\n", nr_run_stat);
#endif
		cpu_count = nr_run_stat;
		nr_cpus = num_online_cpus_little();

		if (persist_count > 0)
			persist_count--;

		if (cpu_count == 1) {
			if (persist_count == 0) {
				//take down everyone
				unplug_cpu(0);
			}
#ifdef DEBUG_INTELLI_PLUG
			pr_info("case 1: %u\n", persist_count);
#endif
		} else if (cpu_count >= 2) {
			if (persist_count == 0)
				persist_count = DUAL_PERSISTENCE;
			if (nr_cpus < 2) {
				cpu_up(1);
			} else {
				unplug_cpu(1);
			}
#ifdef DEBUG_INTELLI_PLUG
			pr_info("case 2: %u\n", persist_count);
#endif
		} else {
			pr_err("Run Stat Error: Bad value %u\n", nr_run_stat);
		}
		queue_delayed_work_on(0, intelli_plug_wq, &intelli_plug_work,
			msecs_to_jiffies(sampling_time));
	}
}

static void __ref cpu_all_ctrl(bool online)
{
	unsigned int i, j;

	if (online) {
		if (intelli_plug_active) {
			mutex_lock(&intelli_plug_mutex);
			flush_workqueue(intelli_plug_wq);
			intelli_plug_active = false;

			for (i = 4; i < 11; i++) {
				j = i;
				if (j >= 8) j = 11 - j;

				if (!cpu_online(j)) {
					cpu_up(j);
				}
			}
			mutex_unlock(&intelli_plug_mutex);
		}
	} else {
		if (!intelli_plug_active) {
			mutex_lock(&intelli_plug_mutex);
			for (i = setup_max_cpus - 1; i > 1; i--) {
				if (cpu_online(i)) {
					cpu_down(i);
				}
			}

			intelli_plug_active = true;
			queue_delayed_work_on(0, intelli_plug_wq, &intelli_plug_work,
				msecs_to_jiffies(10));
			mutex_unlock(&intelli_plug_mutex);
		}
	}
}

static void intelli_plug_cac_fn(struct work_struct *work)
{
	cpu_all_ctrl(cac_bool);
}
static DECLARE_WORK(intelli_plug_cac, intelli_plug_cac_fn);

void call_cac(bool online)
{
	cac_bool = online;
	queue_work_on(0, intelli_plug_cac_wq, &intelli_plug_cac);
}

static int __ref fb_state_change(struct notifier_block *nb,
		unsigned long val, void *data)
{
	struct fb_event *evdata = data;
	struct fb_info *info = evdata->info;
	unsigned int blank;

	if (val != FB_EVENT_BLANK &&
		val != FB_R_EARLY_EVENT_BLANK)
		return 0;
	/*
	 * If FBNODE is not zero, it is not primary display(LCD)
	 * and don't need to process these scheduling.
	 */
	if (info->node)
		return NOTIFY_OK;

	blank = *(int *)evdata->data;

	switch (blank) {
	case FB_BLANK_POWERDOWN:
		call_cac(false);
		break;
	case FB_BLANK_UNBLANK:
		call_cac(true);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block fb_block = {
	.notifier_call = fb_state_change,
};

int __init intelli_plug_init(void)
{
	pr_info("intelli_plug by faux123, modified by arter97 based on version %d.%d\n",
		 INTELLI_PLUG_MAJOR_VERSION,
		 INTELLI_PLUG_MINOR_VERSION);

	fb_register_client(&fb_block);

	intelli_plug_wq = alloc_workqueue("intelliplug", WQ_HIGHPRI, 1);
	intelli_plug_cac_wq = alloc_workqueue("iplug_cac", WQ_HIGHPRI, 1);
	INIT_DELAYED_WORK(&intelli_plug_work, intelli_plug_work_fn);

	return 0;
}

MODULE_AUTHOR("Paul Reioux <reioux@gmail.com>");
MODULE_DESCRIPTION("'intell_plug' - An intelligent cpu hotplug driver for "
	"Low Latency Frequency Transition capable processors");
MODULE_LICENSE("GPL");

late_initcall(intelli_plug_init);
