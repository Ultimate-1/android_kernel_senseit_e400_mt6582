// working lcm driver for Senseit E400

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <platform/mt_pmic.h>

	#define Lcd_Log printf
#else
	#include <linux/string.h>
	#include <linux/kernel.h>
	#include <mach/mt_gpio.h>
	#include <mach/mt_pm_ldo.h>

	#define Lcd_Log printk
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Константы
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)

#define REGFLAG_DELAY             				0xFFE   // Задержка
#define REGFLAG_END_OF_TABLE      				0xFFF   // Конец таблицы

#define LCM_ID	0x9605   // Тип дисплея otm9605a

#define LCM_DSI_CMD_MODE									0
#define GPIO_LCD_ID_PIN 									55

#ifndef TRUE
#define   TRUE     1
#endif

#ifndef FALSE
#define   FALSE    0
#endif

// ---------------------------------------------------------------------------
//  Локальные переменные
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Локальные функции
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64]; // По стандарту остаётся 64
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0xff, 3, {0x96,0x05,0x01}},
{0x00, 1, {0x80}},
{0xff, 2, {0x96,0x05}},
{0x00, 1, {0x00}},
{0xa0, 1, {0x00}},
{0x00, 1, {0x00}},
{0xd9, 1, {0x47}},
{0x00, 1, {0xc5}},
{0xb0, 1, {0x03}},
{0x00, 1, {0x83}},
{0xb2, 1, {0x80}},
{0x00, 1, {0x89}},
{0xc0, 1, {0x01}},
{0x00, 1, {0xb4}},
{0xc0, 1, {0x50}},
{0x00, 1, {0x80}},
{0xc1, 2, {0x36,0x66}},
{0x00, 1, {0xa0}},
{0xc1, 1, {0x00}},
{0x00, 1, {0x80}},
{0xc5, 4, {0x08,0x00,0xa0,0x11}},
{0x00, 1, {0x90}},
{0xc5, 7, {0xa6,0x76,0x01,0x76,0x55,0x55,0x34}},
{0x00, 1, {0xb0}},
{0xc5, 2, {0x04,0x28}},
{0x00, 1, {0xc0}},
{0xc5, 1, {0x00}},
{0x00, 1, {0x00}},
{0xd0, 1, {0x40}},
{0x00, 1, {0x00}},
{0xd1, 2, {0x00,0x00}},
{0x00, 1, {0xb2}},
{0xf5, 4, {0x15,0x00,0x15,0x00}},
{0x00, 1, {0x80}},
{0xcb, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0x90}},
{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xa0}},
{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xb0}},
{0xcb, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xc0}},
{0xcb, 15, {0x04,0x04,0x04,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x04,0x04,0x00}},
{0x00, 1, {0xd0}},
{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x00,0x04,0x00,0x04,0x00,0x04}},
{0x00, 1, {0xe0}},
{0xcb, 10, {0x00,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xf0}},
{0xcb, 10, {0x00,0xcc,0xcc,0x00,0x00,0x00,0xcc,0xcc,0x0f,0x00}},
{0x00, 1, {0x80}},
{0xcc, 10, {0x26,0x25,0x21,0x22,0x00,0x0c,0x00,0x0a,0x00,0x10}},
{0x00, 1, {0x90}},
{0xcc, 15, {0x00,0x0e,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x25,0x21,0x22,0x00}},
{0x00, 1, {0xa0}},
{0xcc, 15, {0x0b,0x00,0x09,0x00,0x0f,0x00,0x0d,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xb0}},
{0xcc, 10, {0x25,0x26,0x21,0x22,0x00,0x0d,0x00,0x0f,0x00,0x09}},
{0x00, 1, {0xc0}},
{0xcc, 15, {0x00,0x0b,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x26,0x21,0x22,0x00}},
{0x00, 1, {0xd0}},
{0xcc, 15, {0x0e,0x00,0x10,0x00,0x0a,0x00,0x0c,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0x80}},
{0xce, 12, {0x8b,0x03,0x28,0x8a,0x03,0x28,0x89,0x03,0x28,0x88,0x03,0x28}},
{0x00, 1, {0x90}},
{0xce, 14, {0xf0,0x00,0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,0xf0,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xa0}},
{0xce, 14, {0x38,0x07,0x03,0xc0,0x00,0x18,0x00,0x38,0x06,0x03,0xc1,0x00,0x18,0x00}},
{0x00, 1, {0xb0}},
{0xce, 14, {0x38,0x05,0x03,0xc2,0x00,0x18,0x00,0x38,0x04,0x03,0xc3,0x00,0x18,0x00}},
{0x00, 1, {0xc0}},
{0xce, 14, {0x38,0x03,0x03,0xc4,0x00,0x18,0x00,0x38,0x02,0x03,0xc5,0x00,0x18,0x00}},
{0x00, 1, {0xd0}},
{0xce, 14, {0x38,0x01,0x03,0xc6,0x00,0x18,0x00,0x38,0x00,0x03,0xc7,0x00,0x18,0x00}},
{0x00, 1, {0xc0}},
{0xcf, 1, {0x02}},
{0x00, 1, {0xc7}},
{0xcf, 1, {0x00}},
{0x00, 1, {0xc9}},
{0xcf, 1, {0x08}},
{0x00, 1, {0x00}},
{0xd8, 2, {0x7f,0x7f}},
{0x00, 1, {0x00}},
{0xe1, 16, {0x05,0x0d,0x13,0x0e,0x06,0x0f,0x0a,0x09,0x04,0x08,0x0e,0x07,0x10,0x13,0x0d,0x05}},
{0x00, 1, {0x00}},
{0xe2, 16, {0x05,0x0d,0x12,0x0e,0x06,0x0e,0x0a,0x09,0x04,0x08,0x0e,0x07,0x10,0x13,0x0d,0x05}},
{0x00, 1, {0xb1}},
{0xc5, 2, {0xa8,0x21}},
{0x00, 1, {0x80}},
{0xc4, 1, {0x9c}},
{0xfe, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0x87}},
{0xc4, 1, {0x40}},
{0xfe, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xb2}},
{0xf5, 4, {0x15,0x00,0x15,0x00}},
{0x00, 1, {0x93}},
{0xc5, 3, {0x00,0x55,0x55}},
{0x00, 1, {0x80}},
{0xc1, 2, {0x36,0x66}},
{0x00, 1, {0x89}},
{0xc0, 1, {0x01}},
{0x00, 1, {0xa0}},
{0xc1, 1, {0x00}},
{0x00, 1, {0xb8}},
{0xb0, 1, {0x26}},
{0x00, 1, {0xc5}},
{0xb0, 1, {0x03}},
{0x00, 1, {0xd2}},
{0xb0, 1, {0x04}},
{0x00, 1, {0xb1}},
{0xb0, 4, {0x80,0x04,0x00,0x00}},
{0x00, 1, {0x00}},
{0xff, 3, {0xff,0xff,0xff}},
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29, 1, {0x00}},
{REGFLAG_DELAY, 20, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	{0x28, 0, {0x00}},
	{REGFLAG_DELAY, 20, {}},
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for (i = 0; i < count; i++) {

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
        }
    }

}


// ---------------------------------------------------------------------------
//  Реализация драйвера LCM
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DSI;

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

    // Сильно зависит от возможностей драйвера LCM.
    // Не поддерживается в MT6573
    params->dsi.packet_size=256;

    // Настройки видео
    params->dsi.intermediat_buffer_num = 2;

    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

    params->dsi.vertical_sync_active				= 4;
    params->dsi.vertical_backporch					= 16;
    params->dsi.vertical_frontporch					= 15;
    params->dsi.vertical_active_line				= FRAME_HEIGHT;

    params->dsi.horizontal_sync_active				= 10;
    params->dsi.horizontal_backporch				= 37;
    params->dsi.horizontal_frontporch				= 37;
    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

    // Добавлено
    params->dsi.PLL_CLOCK = 200;
	params->dsi.ssc_disable = 1;
}

static void lcm_init(void)
{
#if defined(BUILD_LK)
		upmu_set_rg_vgp2_vosel(5);
		upmu_set_rg_vgp2_en(1);
	
		upmu_set_rg_vgp3_vosel(3);
		upmu_set_rg_vgp3_en(1); 
#else
		hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
		hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif

    MDELAY(100);
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(30);
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
	
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
    Lcd_Log("\n %s \n",__func__);

	lcm_init(); // Да, это в стоковом ядре именно так
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
	int   array[4];
	char  buffer[5];
	unsigned int id=0;

#if defined(BUILD_LK)
		upmu_set_rg_vgp2_vosel(5);
		upmu_set_rg_vgp2_en(1);
	
		upmu_set_rg_vgp3_vosel(3);
		upmu_set_rg_vgp3_en(1); 
#else
		hwPowerOn(MT6323_POWER_LDO_VGP2, VOL_2800, "Lance_LCM");
		hwPowerOn(MT6323_POWER_LDO_VGP3, VOL_1800, "Lance_LCM");
#endif	
	SET_RESET_PIN(0);
	MDELAY(200);
	SET_RESET_PIN(1);
	MDELAY(200);
	array[0] = 0x00083700; // С оригиналом совпало
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1,buffer,4);
	
	id=(buffer[2]<<8)+buffer[3];
#if defined(BUILD_LK)
		
	printf("MYCAT otm9605a, id = 0x%08x\n",  id);
	
#endif
	return ((LCM_ID == id)&&(get_lcd_id() == 1))?1:0; // return 1
}

LCM_DRIVER otm9605a_dzx_Y83856_HSD_dsi_2_vdo_qhd_lcm_drv =
    {
        .name			= "otm9605a_dzx_Y83856_HSD_dsi_2_vdo_qhd",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .compare_id     = lcm_compare_id
    };

