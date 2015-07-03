#ifndef __S6E3HA3_WQHD_PARAM_H__
#define __S6E3HA3_WQHD_PARAM_H__

#include <linux/types.h>
#include <linux/kernel.h>

struct lcd_seq_info
{
        unsigned char *cmd;
        unsigned int len;
        unsigned int sleep;
};

#define LCD_TYPE_MASK 0x0030C0
#define LCD_TYPE_ID_HA2 0x0
#define LCD_TYPE_ID_HA3 0x001040
#define LCD_TYPE_ID_HF3 0x002080

#define LCD_DCDC_MAX77838	0
#define LCD_DCDC_TPS65633B	1

#define LCD_TYPE_S6E3HA2_WQHD		0
#define LCD_TYPE_S6E3HA3_WQHD		1
#define LCD_TYPE_S6E3HF3_WQHD		2

#define LCDTYPE_MASK				0x0030C0

#define POWER_IS_ON(pwr)			(pwr <= FB_BLANK_NORMAL)
#define LEVEL_IS_HBM(level)			(level >= 6)
#define UNDER_MINUS_20(temperature)	(temperature <= -20)
#define UNDER_0(temperature)	(temperature <= 0)

#define ACL_IS_ON(nit) 				(nit < 360)
#define CAPS_IS_ON(level)	(level >= 41)

#define NORMAL_TEMPERATURE			25      /* 25 degrees Celsius */
#define UI_MAX_BRIGHTNESS 			255
#define UI_MIN_BRIGHTNESS 			0
#define UI_DEFAULT_BRIGHTNESS 		134

#ifdef CONFIG_LCD_HMT
#define DEFAULT_HMT_BRIGHTNESS			162
#define	HMT_OFF							0
#define HMT_ON							1
#endif

enum
{
        HBM_STATUS_OFF,
        HBM_STATUS_ON,
        HBM_STATUS_MAX,
};

enum
{
        ACL_STATUS_0P,
        ACL_STATUS_8P,
        ACL_STATUS_MAX
};

enum
{
        ACL_OPR_16_FRAME,
        ACL_OPR_32_FRAME,
        ACL_OPR_MAX
};

struct aid_dimming_dynamic_var
{
	unsigned char elvss_len;
	unsigned char elvss_reg;
	unsigned char elvss_cmd_cnt;
	unsigned char aid_cmd_cnt;
	unsigned char aid_reg_offset;
	unsigned char tset_len;
	unsigned char tset_reg;
	unsigned char tset_minus_offset;
	unsigned char vint_reg2;
 	unsigned char vint_dim_offset[10];
	char elvss_minus_offset[3][5];
};
extern struct aid_dimming_dynamic_var aid_dimming_dynamic;

#define GAMMA_CMD_CNT 36

#define VINT_REG					0xF4
#define VINT_LEN					3

#define USE_PANEL_PARAMETER_MAX_SIZE 1
#define ELVSS_LEN_MAX S6E3HA2_ELVSS_LEN
#define TSET_LEN_MAX S6E3HA3_TSET_LEN
#define AID_CMD_CNT_MAX	S6E3HA3_AID_CMD_CNT

#define USE_VINT_REG2

static const unsigned int VINT_DIM_TABLE[] = {  /* HA2*/
        5, 6, 7, 8, 9,
        10, 11, 12, 13, 14
};

static const unsigned char VINT_TABLE_HA2[] = {     /* S6E3HA2*/
        0x18, 0x19, 0x1A, 0x1B, 0x1C,
        0x1D, 0x1E, 0x1F, 0x20, 0x21
};

static const unsigned char VINT_TABLE_HF3[] = {     /* HF3 */
        0x18, 0x19, 0x1A, 0x1B, 0x1C,
        0x1D, 0x1E, 0x1F, 0x20, 0x21
};

static const unsigned char VINT_TABLE_HA3[] = {     /* HA3 */
        0x15, 0x16, 0x17, 0x18, 0x19,
        0x1A, 0x1B, 0x1C, 0x1D, 0x1E
};

static const char ELVSS_OFFSET_HA2[][5] = {
//	2nit,  3nit, 4nit, 5nit, 6nit ~
	{0x00, 0x00, 0x00, 0x00, 0x00},					// T > 0
	{0x04, 0x03, 0x02, 0x01, 0x00},					// -20 < T < 0
	{0x00, -0x01, -0x02, -0x03, -0x04}				// -20 > T
};

static const char ELVSS_OFFSET_HA3[][5] = {
//	2nit,  3nit, 4nit, 5nit, 6nit ~
	{0x00, 0x00, 0x00, 0x00, 0x00}, 				// T > 0
	{0x04, 0x03, 0x02, 0x01, 0x00}, 				// -20 < T < 0
	{0x00, -0x01, -0x02, -0x03, -0x04}				// -20 > T
};

static const char ELVSS_OFFSET_HF3[][5] = {
//	2nit,  3nit, 4nit, 5nit, 6nit ~
	{0x00, 0x00, 0x00, 0x00, 0x00}, 				// T > 0
	{0x04, 0x03, 0x02, 0x01, 0x00}, 				// -20 < T < 0
	{0x00, -0x01, -0x02, -0x03, -0x04}				// -20 > T
};

static const unsigned char SEQ_TEST_KEY_ON_F0[] = {
        0xF0,
        0x5A, 0x5A
};

static const unsigned char SEQ_TEST_KEY_OFF_F0[] = {
        0xF0,
        0xA5, 0xA5
};

static const unsigned char SEQ_TEST_KEY_ON_F1[] = {
        0xF1,
        0x5A, 0x5A,
};

static const unsigned char SEQ_TEST_KEY_OFF_F1[] = {
        0xF1,
        0xA5, 0xA5,
};

static const unsigned char SEQ_TEST_KEY_ON_FC[] = {
        0xFC,
        0x5A, 0x5A
};

static const unsigned char SEQ_TEST_KEY_OFF_FC[] = {
        0xFC,
        0xA5, 0xA5
};

static const unsigned char SEQ_SLEEP_OUT[] = {
        0x11,
};

static const unsigned char SEQ_SLEEP_IN[] = {
        0x10
};

static const unsigned char SEQ_DISPLAY_ON[] = {
        0x29,
};

static const unsigned char SEQ_DISPLAY_OFF[] = {
        0x28
};

static const unsigned char SEQ_GAMMA_UPDATE[] = {
        0xF7,
        0x03
};

static const unsigned char SEQ_GAMMA_UPDATE_L[] = {
        0xF7,
        0x00
};

static const unsigned char SEQ_HBM_OFF[] = {
        0x53,
        0x00
};

static const unsigned char SEQ_HBM_ON[] = {
        0x53,
        0xC0
};

/************************ HA2 ************************/

#define S6E3HA2_MTP_ADDR		0xC8
#define S6E3HA2_MTP_SIZE		35

#define S6E3HA2_ID_REG			0x04
#define S6E3HA2_ID_LEN			3


#define S6E3HA2_RDDPM_ADDR		0x0A
#define S6E3HA2_RDDSM_ADDR		0x0E
#define S6E3HA2_MTP_DATE_SIZE	S6E3HA2_MTP_SIZE + 9

#define S6E3HA2_COORDINATE_LEN	4
#define S6E3HA2_COORDINATE_REG	0xA1

#define S6E3HA2_CODE_REG		0xD6
#define S6E3HA2_CODE_LEN		5

#define S6E3HA2_HBMGAMMA_REG		0xB4
#define S6E3HA2_HBMGAMMA_LEN		31

#define S6E3HA2_MAX_BRIGHTNESS	360
#define S6E3HA2_HBM_BRIGHTNESS	600

#define S6E3HA2_HBM_ELVSS_INDEX		21
#define S6E3HA2_HBM_ELVSS_COMP		0x06

#define S6E3HA2_ELVSS_CMD_CNT				3
#define S6E3HA2_ELVSS_REG					0xB6
#define S6E3HA2_ELVSS_LEN					23      /* elvss: Global para 22th */

#define S6E3HA2_AID_CMD_CNT					3
#define S6E3HA2_AID_REG_OFFSET		1

#define S6E3HA2_TSET_REG					0xB8    /* TSET: Global para 8th */
#define S6E3HA2_TSET_LEN					9
#define S6E3HA2_TSET_MINUS_OFFSET			0x04

#define S6E3HA2_VINT_REG2					0x8B

#define S6E3HA2_HBM_INDEX					73

static const unsigned char S6E3HA2_SEQ_SINGLE_DSI_1[] = {
        0xF2,
        0x67
};

static const unsigned char S6E3HA2_SEQ_SINGLE_DSI_2[] = {
        0xF9,
        0x29
};

static const unsigned char S6E3HA2_SEQ_GAMMA_CONDITION_SET[] = {
        0xCA,
        0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x00, 0x00, 0x00,
        0x00, 0x00
};

static const unsigned char S6E3HA2_SEQ_AOR_CONTROL[] = {
        0xB2,
        0x03, 0x10
};

static const unsigned char S6E3HA2_SEQ_ELVSS_SET[] = {
        0xB6,
        0x9C,                   /* MPS_CON: ACL OFF */
        0x0A                    /* ELVSS: MAX */
};

static const unsigned char S6E3HA2_SEQ_VINT_SET[] = {
        0xF4,
        0x8B, 0x21
};

static const unsigned char S6E3HA2_SEQ_TSET_GLOBAL[] = {
        0xB0,
        0x07
};

static const unsigned char S6E3HA2_SEQ_TE_ON[] = {
        0x35,
        0x00
};
static const unsigned char S6E3HA2_SEQ_TE_OFF[] = {
        0x34,
};

static const unsigned char S6E3HA2_SEQ_TSP_TE[] = {
        0xBD,
        0x11, 0x11, 0x02, 0x16, 0x02, 0x16
};

static const unsigned char S6E3HA2_SEQ_PENTILE_SETTING[] = {
        0xC0,
        0x00, 0x00, 0xD8, 0xD8
};

static const unsigned char S6E3HA2_SEQ_POC_SETTING1[] = {
        0xB0,
        0x20
};

static const unsigned char S6E3HA2_SEQ_POC_SETTING2[] = {
        0xFE,
        0x08
};

static const unsigned char S6E3HA2_SEQ_OSC_SETTING1[] = {
        0xFD,
        0x1C
};

static const unsigned char S6E3HA2_SEQ_OSC_SETTING2[] = {
        0xFE,
        0xA0
};

static const unsigned char S6E3HA2_SEQ_OSC_SETTING3[] = {
        0xFE,
        0x20
};


static const unsigned char S6E3HA2_SEQ_PCD_SETTING[] = {
        0xCC,
        0x4C
};

static const unsigned char S6E3HA2_SEQ_ERR_FG_SETTING[] = {
        0xED,
        0x44
};

static const unsigned char S6E3HA2_SEQ_ACL_OFF[] = {
        0x55,
        0x00
};
static const unsigned char S6E3HA2_SEQ_ACL_8[] = {
        0x55,
        0x02,
};

static const unsigned char S6E3HA2_SEQ_ACL_OFF_OPR[] = {
        0xB5,
        0x40
};

static const unsigned char S6E3HA2_SEQ_ACL_ON_OPR[] = {
        0xB5,
        0x50
};


static const unsigned char S6E3HA2_SEQ_TSET[] = {
        0xB8,
        0x19                    /* Global para(8th) + 25 degrees  : 0x19 */
};

#ifdef CONFIG_LCD_HMT
static const unsigned char SEQ_HMT_ON1[] = {    /* aid */
        0xB2,
        0x03, 0x13, 0x04, 0x0E, 0x0E, 0x00
};
static const unsigned char SEQ_HMT_ON2[] = {    /* porch */
        0xF3,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
};
static const unsigned char SEQ_HMT_ON3[] = {    /* PASET Setting  */
        0x2B,
        0x00, 0x00, 0x09, 0xFF
};


static const unsigned char SEQ_HMT_OFF1[] = {   /* aid */
        0xB2,
        0xA5, 0x30, 0x04, 0x0C, 0x0C, 0x40
};
static const unsigned char SEQ_HMT_OFF2[] = {   /* porch */
        0xF3,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


static const unsigned char SEQ_HMT_AID_FORWARD1[] = {   /* G.Param */
        0xF2,
        0x67, 0x41, 0xC5, 0x0A, 0x06
};
static const unsigned char SEQ_HMT_AID_FORWARD2[] = {   /* G.Param */
        0xCB,
        0x18, 0x31, 0x80, 0x00, 0x00, 0x20, 0x40, 0x00,
        0x00, 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x15, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
        0xDF, 0x00, 0x00, 0xCA, 0x0A, 0x0A, 0x0A, 0xCA,
        0x8A, 0xCA, 0x0A, 0x0A, 0x0A, 0xCD, 0xCF, 0xCB,
        0xC2, 0x43, 0xC0, 0xCA, 0xCA, 0x0A, 0x0A, 0x0A,
        0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0D, 0x0F, 0x0B
};

static const unsigned char SEQ_HMT_AID_REVERSE1[] = {   /* G.Param */
        0xF2,
        0x67, 0x41, 0xC5, 0x0E, 0x06
};
static const unsigned char SEQ_HMT_AID_REVERSE2[] = {   /* G.Param */
        0xCB,
        0x18, 0x31, 0x80, 0x00, 0x00, 0x20, 0x40, 0x00,
        0x00, 0x02, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x15, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00,
        0xDF, 0x00, 0x00, 0xCA, 0x0A, 0x0A, 0x0A, 0xCA,
        0x8A, 0xCA, 0x0A, 0x0A, 0x0A, 0xCD, 0xCF, 0xCC,
        0xC2, 0x43, 0xC0, 0xCA, 0xCA, 0x0A, 0x0A, 0x0A,
        0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0D, 0x0F, 0x0C
};
#endif

/************************ HA3 ************************/

#define S6E3HA3_MTP_ADDR		0xC8
#define S6E3HA3_MTP_SIZE		35
#define S6E3HA3_MTP_VT_ADDR		33

#define S6E3HA3_ID_REG			0x04
#define S6E3HA3_ID_LEN			3

#define S6E3HA3_RDDPM_ADDR		0x0A
#define S6E3HA3_RDDSM_ADDR		0x0E
#define S6E3HA3_MTP_DATE_SIZE	S6E3HA3_MTP_SIZE + 9

#define S6E3HA3_COORDINATE_LEN	4
#define S6E3HA3_COORDINATE_REG	0xA1

#define S6E3HA3_CODE_REG		0xD6
#define S6E3HA3_CODE_LEN		5

#define S6E3HA3_ELVSS_CMD_CNT				3
#define S6E3HA3_ELVSS_REG					0xB5
#define S6E3HA3_ELVSS_LEN					23      /* elvss: Global para 22th */

#define S6E3HA3_AID_CMD_CNT					11 /* AID length = REG(1) +address(8) +reg_length(2) */
#define S6E3HA3_AID_REG_OFFSET			9      /* AID address cmd = 0xB) 0x08 */
#define S6E3HA3_AID_REG					0xB1

#define S6E3HA3_TSET_REG					0xB5
#define S6E3HA3_TSET_LEN					30      /* TSET: Global para 29th */
#define S6E3HA3_TSET_MINUS_OFFSET			0x04

#define S6E3HA3_VINT_REG2					0xAB

#define S6E3HA3_HBMGAMMA_REG		0xB3
#define S6E3HA3_HBMGAMMA_LEN		31

#define S6E3HA3_MAX_BRIGHTNESS	360
#define S6E3HA3_HBM_BRIGHTNESS	600

#define S6E3HA3_HBM_ELVSS_INDEX		21
#define S6E3HA3_HBM_ELVSS_COMP		0x06

/*
static const unsigned char S6E3HA3_VINT_TABLE[] = {
0x15, 0x16, 0x17, 0x18, 0x19,
0x1A, 0x1B, 0x1C, 0x1D, 0x1E
};
*/

static const unsigned char S6E3HA3_SEQ_FREQ_CALIBRATION[] = {			// 59.90
	0xCE,
	0x1F, 0x68, 0x49, 0x4E, 0x38, 0x8A, 0xC6, 0x03, 0x00,
	0xFF, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0x0F, 0xAB
};


static const unsigned char S6E3HA3_SEQ_MIC[] = {
        0xF9,
        0x06
};

static const unsigned char S6E3HA3_SEQ_DCDC_GLOBAL[] = {
        0xB0,
        0x18
};

static const unsigned char S6E3HA3_SEQ_DCDC[] = {
        0xB5,
        0x50
};

static const unsigned char S6E3HA3_SEQ_GAMMA_CONDITION_SET[] = {
        0xCA,
        0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x00, 0x00, 0x00,
        0x00, 0x00
};

static const unsigned char S6E3HA3_SEQ_AOR[] = {
        0xB1,
        0x54, 0x20, 0x1A, 0x33, 0x5E, 0x8C, 0xB3, 0xD9,
        0x10, 0x03
};


static const unsigned char S6E3HA3_SEQ_MPS_ELVSS_SET[] = {
        0xB5,
        0xBC,                   /* MPS_CON: ACL OFF */
        0x0A                    /* ELVSS: MAX */
};

static const unsigned char S6E3HA3_SEQ_VINT_SET[] = {
        0xF4,
        0xAB,                   /* VINT */
        0x1E                    /* 360nit */
};

static const unsigned char S6E3HA3_SEQ_ACL_OFF[] = {
        0x55,
        0x00                    /* off = 00, on = 02 */
};

static const unsigned char S6E3HA3_SEQ_ACL_OFF_OPR[] = {
        0xB4,
        0x40                    /* 360nit = 0x40, else = 0x50 */
};

static const unsigned char S6E3HA3_SEQ_ACL_ON_OPR[] = {
        0xB4,
        0x50
};

static const unsigned char S6E3HA3_SEQ_TE_ON[] = {
        0x35,
        0x00
};

static const unsigned char S6E3HA3_SEQ_TSET_GLOBAL[] = {
        0xB0,
        0x05
};

static const unsigned char S6E3HA3_SEQ_TSET[] = {
        0xB8,
        0x19
};

static const unsigned char S6E3HA3_SEQ_ELVSS_GLOBAL[] = {
        0xB0,
        0x15
};

static const unsigned char S6E3HA3_SEQ_ELVSS[] = {
        0xB5,
        0x0A                    /* ELVSS: MAX */
};

static const unsigned char S6E3HA3_SEQ_TSP_TE[] = {
        0xBC,
        0x11, 0x11, 0x02, 0x16, 0x02, 0x16
};

static const unsigned char S6E3HA3_SEQ_PENTILE_SETTING[] = {
	0xC2,
	0x00, 0x00, 0xD8, 0xD8, 0x00, 0x80, 0x2B, 0x05,
	0x08, 0x0E, 0x07, 0x0B, 0x05, 0x0D, 0x0A, 0x15,
	0x13, 0x20, 0x1E
};

static const unsigned char S6E3HA3_SEQ_PCD[] = {
        0xCC,
        0x4C
};

static const unsigned char S6E3HA3_SEQ_ERR_FG[] = {
        0xED,
        0x44
};

static const unsigned char S6E3HA3_SEQ_TE_RISING_TIMING[] = {
	0xB9,
	0x01, 0x09, 0xFF, 0x00, 0x0A
};


/************************ HF3 ************************/

#define S6E3HF3_MTP_ADDR		0xC8
#define S6E3HF3_MTP_SIZE		35
#define S6E3HF3_MTP_VT_ADDR		33

#define S6E3HF3_ID_REG			0x04
#define S6E3HF3_ID_LEN			3

#define S6E3HF3_RDDPM_ADDR		0x0A
#define S6E3HF3_RDDSM_ADDR		0x0E
#define S6E3HF3_MTP_DATE_SIZE	S6E3HF3_MTP_SIZE + 9

#define S6E3HF3_COORDINATE_LEN	4
#define S6E3HF3_COORDINATE_REG	0xA1

#define S6E3HF3_CODE_REG		0xD6
#define S6E3HF3_CODE_LEN		5

#define S6E3HF3_ELVSS_CMD_CNT				3
#define S6E3HF3_ELVSS_REG					0xB5
#define S6E3HF3_ELVSS_LEN					23      /* elvss: Global para 22th */

#define S6E3HF3_AID_CMD_CNT					11 /* AID length = REG(1) +address(8) +reg_length(2) */
#define S6E3HF3_AID_REG_OFFSET			9      /* AID address cmd = 0xB) 0x08 */
#define S6E3HF3_AID_REG					0xB1

#define S6E3HF3_TSET_REG					0xB5
#define S6E3HF3_TSET_LEN					30      /* TSET: Global para 29th */
#define S6E3HF3_TSET_MINUS_OFFSET			0x04

#define S6E3HF3_VINT_REG2					0xAB

#define S6E3HF3_HBMGAMMA_REG		0xB3
#define S6E3HF3_HBMGAMMA_LEN		31

#define S6E3HF3_MAX_BRIGHTNESS	360
#define S6E3HF3_HBM_BRIGHTNESS	600

#define S6E3HF3_HBM_ELVSS_INDEX		21
#define S6E3HF3_HBM_ELVSS_COMP		0x06

/*
static const unsigned char S6E3HF3_VINT_TABLE[] = {
0x15, 0x16, 0x17, 0x18, 0x19,
0x1A, 0x1B, 0x1C, 0x1D, 0x1E
};
*/

static const unsigned char S6E3HF3_SEQ_MIC[] = {
        0xF9,
        0x06
};

static const unsigned char S6E3HF3_SEQ_CASET_SET[] = {	/* CASET Setting */
        0x2A,
        0x00, 0x50, 0x05, 0xEF, 0x00, 0x0A, 0x0A
};

static const unsigned char S6E3HF3_SEQ_POC_SET[] = {	/* POC Setting */
        0xEB,
        0xC0, 0x00, 0x33
};

static const unsigned char S6E3HF3_SEQ_GAMMA_CONDITION_SET[] = {
        0xCA,
        0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x80, 0x80, 0x80,
        0x00, 0x00, 0x00,
        0x00, 0x00
};

static const unsigned char S6E3HF3_SEQ_AOR[] = {	/* AOR 10% */
        0xB1,
       	0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       	0x10, 0x03
};

static const unsigned char S6E3HF3_SEQ_MPS_ELVSS_SET[] = {
        0xB5,
        0xBC,                   /* MPS_CON: ACL OFF */
        0x0A                    /* ELVSS: MAX */
};

static const unsigned char S6E3HF3_SEQ_VINT_SET[] = {
        0xF4,
        0xAB,                   /* VINT */
        0x1E                    /* 360nit */
};

static const unsigned char S6E3HF3_SEQ_ACL_OFF[] = {
        0x55,
        0x00                    /* off = 00, on = 02 */
};

static const unsigned char S6E3HF3_SEQ_ACL_OFF_OPR[] = {
        0xB4,
        0x40                    /* 360nit = 0x40, else = 0x50 */
};

static const unsigned char S6E3HF3_SEQ_ACL_ON_OPR[] = {
        0xB4,
        0x50
};

static const unsigned char S6E3HF3_SEQ_TE_ON[] = {
        0x35,
        0x00
};

static const unsigned char S6E3HF3_SEQ_TSET_GLOBAL[] = {
        0xB0,
        0x1C
};

static const unsigned char S6E3HF3_SEQ_TSET[] = {
        0xB5,
        0x19                    /* temperature 25 */
};

static const unsigned char S6E3HF3_SEQ_ELVSS_GLOBAL[] = {
        0xB0,
        0x15
};

static const unsigned char S6E3HF3_SEQ_ELVSS[] = {
        0xB5,
        0x0A                    /* ELVSS: MAX */
};

static const unsigned char S6E3HF3_SEQ_TSP_TE[] = {
        0xBC,
        0x11, 0x11, 0x02, 0x16, 0x02, 0x16
};

static const unsigned char S6E3HF3_SEQ_PENTILE_SETTING[] = {
	0xC2,
	0x00, 0x00, 0xD8, 0xD8, 0x00, 0x80, 0x2B, 0x05,
	0x08, 0x0E, 0x07, 0x0B, 0x05, 0x0D, 0x0A, 0x15,
	0x13, 0x20, 0x1E
};

static const unsigned char S6E3HF3_SEQ_PCD[] = {
        0xCC,
        0x00
};

static const unsigned char S6E3HF3_SEQ_ERR_FG[] = {
        0xED,
        0x44
};

static const unsigned char S6E3HF3_SEQ_TE_RISING_TIMING[] = {
	0xB9,
	0x01, 0x09, 0xFF, 0x00, 0x09
};

#endif /* __S6E3HA3_PARAM_H__ */
