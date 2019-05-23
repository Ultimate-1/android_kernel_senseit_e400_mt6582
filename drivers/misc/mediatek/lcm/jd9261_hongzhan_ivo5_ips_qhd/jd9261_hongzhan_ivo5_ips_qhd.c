// lcm driver for Senseit E400 (not tested!)

#ifdef BUILD_LK
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#else
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#endif

#ifndef BUILD_LK
#include <linux/string.h>
#endif

#include "lcm_drv.h"

#ifdef BUILD_LK
    #define LCM_DEBUG  printf
    #define LCM_FUNC_TRACE() printf("huyl [uboot] %s\n",__func__)
#else
    #define LCM_DEBUG  printk
    #define LCM_FUNC_TRACE() printk("huyl [kernel] %s\n",__func__)
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH                                         (540)
#define FRAME_HEIGHT                                        (960)
#define LCM_ID												(0x9292) // если у вас он другой, то сообщите мне! https://github.com/Ultimate-1/

#define REGFLAG_DELAY                                       0xFFE
#define REGFLAG_END_OF_TABLE                                0xFFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE                                    0
#define GPIO_LCD_ID_PIN 									55

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

//static kal_bool IsFirstBoot = KAL_TRUE;

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)            lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                                           lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                   lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)   

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
{0xBF, 3, {0x92,0x61,0xF3}},
{REGFLAG_DELAY, 1, {}},
{0xB3, 2, {0x00,0x48}},
{REGFLAG_DELAY, 1, {}},
{0xB4, 2, {0x00,0x52}},
{REGFLAG_DELAY, 1, {}},
{0xB8, 6, {0x00,0xCF,0x01,0x00,0xCF,0x01}},
{REGFLAG_DELAY, 1, {}},
{0xC3, 1, {0x04}},
{REGFLAG_DELAY, 1, {}},
{0xC4, 2, {0x00,0x78}},
{REGFLAG_DELAY, 1, {}},
{0xC7, 9, {0x00,0x01,0x32,0x08,0x68,0x2A,0x15,0xA5,0xA5}},
{REGFLAG_DELAY, 1, {}},
{0xC8, 38, {0x7F,0x62,0x4F,0x3F,0x38,0x27,0x2B,0x15,0x2F,0x30,0x30,0x4D,0x3A,0x3F,0x2E,0x29,0x1C,0x0B,0x02,0x7F,0x62,0x4F,0x3F,0x38,0x27,0x2B,0x15,0x2F,0x30,0x30,0x4D,0x3A,0x3F,0x2E,0x29,0x1C,0x0B,0x02}},
{REGFLAG_DELAY, 1, {}},
{0xD4, 19, {0x1F,0x1F,0x01,0x03,0x1F,0x1F,0x07,0x0B,0x1F,0x1F,0x09,0x05,0x13,0x11,0x1F,0x1F,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY, 1, {}},
{0xD5, 19, {0x00,0x02,0x1F,0x1F,0x06,0x0A,0x1F,0x1F,0x08,0x04,0x1F,0x1F,0x1F,0x1F,0x12,0x10,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY, 1, {}},
{0xD6, 19, {0x1F,0x1F,0x12,0x10,0x1F,0x1F,0x04,0x08,0x1F,0x1F,0x0A,0x06,0x00,0x02,0x1F,0x1F,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY, 1, {}},
{0xD7, 19, {0x13,0x11,0x1F,0x1F,0x05,0x09,0x1F,0x1F,0x0B,0x07,0x1F,0x1F,0x1F,0x1F,0x01,0x03,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY, 1, {}},
{0xD8, 20, {0x00,0x00,0x00,0x30,0x03,0x30,0x01,0x02,0x30,0x01,0x02,0x06,0x70,0x73,0xC7,0x72,0x06,0x30,0x70,0x08}},
{REGFLAG_DELAY, 1, {}},
{0xD9, 21, {0x00,0x0A,0x0A,0x88,0x00,0x00,0x06,0x7B,0x00,0x00,0x00,0x33,0x33,0x1F,0x00,0x00,0x00,0x06,0x70,0x01,0xE0}},
{REGFLAG_DELAY, 1, {}},
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29, 1, {0x00}},
{REGFLAG_DELAY, 10, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
    {0x28, 0, {0x00}},
    {REGFLAG_DELAY, 20, {}},
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++) {
        
        unsigned cmd;
        cmd = table[i].cmd;
        
        switch (cmd) {
            
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
                
            case REGFLAG_END_OF_TABLE :
                break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
                MDELAY(2);
        }
    }
    
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;
		params->dsi.mode   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED;  //LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
    params->dsi.mode   = BURST_VDO_MODE; //CMD_MODE;
#else
    params->dsi.mode   = SYNC_EVENT_VDO_MODE; //SYNC_PULSE_VDO_MODE;
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_TWO_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

		params->dsi.vertical_sync_active				= 2;
		params->dsi.vertical_backporch					= 8;
		params->dsi.vertical_frontporch					= 5;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 10;
		params->dsi.horizontal_backporch				= 10;
		params->dsi.horizontal_frontporch				= 10;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

#if 0    		
		params->dsi.pll_div1=0;//1;		// div1=0,1,2,3;div1_real=1,2,4,4
		params->dsi.pll_div2=2;//1;		// div2=0,1,2,3;div2_real=1,2,4,4
		//params->dsi.fbk_sel=1;		 // fbk_sel=0,1,2,3;fbk_sel_real=1,2,4,4
		params->dsi.fbk_div =26;//38;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	26=>351MHz Frame check=57
#else
		params->dsi.PLL_CLOCK=200;
		params->dsi.ssc_disable = 1;
#endif
}


//add by hyde for debug
static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(10); 
    SET_RESET_PIN(0);
    MDELAY(10); 
    
    SET_RESET_PIN(1);
    MDELAY(120); 

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}



static void lcm_suspend(void)
{
	MDELAY(100);
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
	MDELAY(50);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);

}


static void lcm_resume(void)
{
	lcm_init();
}


static int get_lcd_id(void)
{
    mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
    mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
    mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
    MDELAY(1);

    return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id;
	unsigned char buffer[4];
	unsigned int data_array[16];
	
	hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
	hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");

	SET_RESET_PIN(0);	//NOTE:should reset LCM firstly
	MDELAY(200);
	SET_RESET_PIN(1);
	MDELAY(200);

	data_array[0] = 0x00023700;
	dsi_set_cmdq(data_array, 1, 1);
	read_reg_v2(0x04, buffer, 2);
	
	id = (buffer[0] << 8) | buffer[1];
	
	//return (LCM_ID == id) ? 1 : 0;
	return ((LCM_ID == id)&&(get_lcd_id() == 1))?1:0;
}
/*
static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
    char  buffer[3];
    int   array[4];

    if(lcm_esd_test)
    {
        lcm_esd_test = FALSE;
        return TRUE;
    }

    array[0] = 0x00013700;
    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0x36, buffer, 1);
    if(buffer[0]==0x90)
    {
        return FALSE;
    }
    else
    {            
        return TRUE;
    }
 #endif

}

static unsigned int lcm_esd_recover(void)
{
    lcm_init();
    lcm_resume();

    return TRUE;
}
*/

LCM_DRIVER jd9261_hongzhan_ivo5_ips_qhd_lcm_drv = 
{
	.name = "JD9261_HONGZHAN_IVO5_IPS_QHD",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    //.esd_check = lcm_esd_check,
    //.esd_recover = lcm_esd_recover,
    };
