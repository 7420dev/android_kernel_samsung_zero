#!/res/busybox sh

exec 2>&1 > /dev/kmsg

export PATH=/res/asset:$PATH

if [[ $(cat /data/media/0/arter97/adaptive_lmk) == "0" ]]; then
	echo "Disabling Adaptive LMK ..."
	echo "0" > /sys/module/lowmemorykiller/parameters/enable_adaptive_lmk
fi

if [[ $(cat /data/media/0/arter97/fade_led) == "0" ]]; then
	echo "Disabling LED fade ..."
	echo "0" > /sys/devices/virtual/sec/led/led_notification_ramp_control
fi

if [[ $(cat /data/media/0/arter97/dualcore_blank) == "1" ]]; then
	echo "Enabling dualcore_blank mode ..."
	echo "2" > /sys/power/enable_dm_hotplug
fi
if [[ $(cat /data/media/0/arter97/dualcore_blank) == "2" ]]; then
	echo "Allowing all CPUs to be turned on during screen-off ..."
	echo "3" > /sys/power/enable_dm_hotplug
	sleep 0.5
	echo "5" > /sys/power/enable_dm_hotplug
fi
