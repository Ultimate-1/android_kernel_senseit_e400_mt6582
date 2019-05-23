// lcm driver for Senseit E400 (not tested!)

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
#define GPIO_LCD_RST_PIN 									112

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
void upmu_set_rg_vgp2_vosel(kal_uint32 val);
void upmu_set_rg_vgp3_vosel(kal_uint32 val);
void upmu_set_rg_vgp2_en(kal_uint32 val);
void upmu_set_rg_vgp3_en(kal_uint32 val);


struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0xFF, 3, {0x96,0x05,0x01}},
{0x00, 1, {0x80}},
{0xFF, 2, {0x96,0x05}},
{0x00, 1, {0x92}},
{0xFF, 2, {0x10,0x02}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0x00}},
{0x00, 1, {0xB4}},
{0xC0, 1, {0x50}},
{0x00, 1, {0x89}},
{0xC0, 1, {0x01}},
{0x00, 1, {0xA0}},
{0xC1, 1, {0x00}},
{0x00, 1, {0x00}},
{0xA0, 1, {0x00}},
{0x00, 1, {0xA2}},
{0xC0, 3, {0x01,0x10,0x00}},
{0x00, 1, {0x90}},
{0xC5, 2, {0x96,0x77}},
{0x00, 1, {0x00}},
{0xD8, 2, {0x6F,0x6F}},
{0x00, 1, {0x00}},
{0xD9, 1, {0x3B}},
{0x00, 1, {0x80}},
{0xC1, 2, {0x36,0x66}},
{0x00, 1, {0xB1}},
{0xC5, 1, {0x28}},
{0x00, 1, {0xB2}},
{0xF5, 4, {0x15,0x00,0x15,0x00}},
{0x00, 1, {0x00}},
{0xE1, 16, {0x00,0x0D,0x12,0x0E,0x07,0x11,0x0C,0x0A,0x02,0x06,0x0B,0x08,0x0F,0x19,0x15,0x00}},
{0x00, 1, {0x00}},
{0xE2, 16, {0x00,0x0C,0x13,0x0E,0x07,0x11,0x0B,0x0B,0x02,0x06,0x0B,0x08,0x0F,0x19,0x15,0x00}},
{0x00, 1, {0x80}},
{0xCB, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0x90}},
{0xCB, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xA0}},
{0xCB, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xB0}},
{0xCB, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xC0}},
{0xCB, 15, {0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xD0}},
{0xCB, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00}},
{0x00, 1, {0xE0}},
{0xCB, 10, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xF0}},
{0xCB, 10, {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},
{0x00, 1, {0x80}},
{0xCC, 10, {0x00,0x00,0x09,0x0B,0x01,0x25,0x26,0x00,0x00,0x00}},
{0x00, 1, {0x90}},
{0xCC, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x0C,0x02}},
{0x00, 1, {0xA0}},
{0xCC, 15, {0x25,0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0x80}},
{0xCE, 12, {0x86,0x01,0x06,0x85,0x01,0x06,0x0F,0x00,0x00,0x0F,0x00,0x00}},
{0x00, 1, {0x90}},
{0xCE, 14, {0xF0,0x00,0x00,0xF0,0x00,0x00,0xF0,0x00,0x00,0xF0,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xA0}},
{0xCE, 14, {0x18,0x05,0x03,0xC0,0x00,0x06,0x00,0x18,0x04,0x03,0xC1,0x00,0x06,0x00}},
{0x00, 1, {0xB0}},
{0xCE, 14, {0x18,0x03,0x03,0xC2,0x00,0x06,0x00,0x18,0x02,0x03,0xC3,0x00,0x06,0x00}},
{0x00, 1, {0xC0}},
{0xCE, 14, {0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},
{0x00, 1, {0xD0}},
{0xCE, 14, {0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},
{0x00, 1, {0x80}},
{0xCF, 14, {0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},
{0x00, 1, {0x90}},
{0xCF, 14, {0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},
{0x00, 1, {0xA0}},
{0xCF, 14, {0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},
{0x00, 1, {0xB0}},
{0xCF, 14, {0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00}},
{0x00, 1, {0xC0}},
{0xCF, 10, {0x02,0x02,0x10,0x10,0x00,0x00,0x01,0x81,0x00,0x08}},
{0x00, 1, {0xB1}},
{0xC5, 1, {0x28}},
{0x00, 1, {0x80}},
{0xC4, 1, {0x9C}},
{0x00, 1, {0xC0}},
{0xC5, 1, {0x00}},
{0x00, 1, {0xB2}},
{0xF5, 4, {0x15,0x00,0x15,0x00}},
{0x00, 1, {0x93}},
{0xC5, 3, {0x03,0x55,0x55}},
{0x00, 1, {0x80}},
{0xC1, 2, {0x36,0x66}},
{0x00, 1, {0x89}},
{0xC0, 1, {0x01}},
{0x00, 1, {0xA0}},
{0xC1, 1, {0x00}},
{0x00, 1, {0xC5}},
{0xB0, 1, {0x03}},
{0x00, 1, {0x00}},
{0xFF, 3, {0xFF,0xFF,0xFF}},
{0x00, 1, {0x00}},
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x00, 1, {0x00}},
{0x29, 1, {0x00}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	{0x28, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
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

    params->dsi.fbk_sel = 1;
	params->dsi.fbk_div = 32;
	params->dsi.pll_div1 = 1;
	params->dsi.pll_div2 = 1;
}

static void lcm_init(void)
{
	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);
	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);

    MDELAY(100);
    mt_set_gpio_mode(GPIO_LCD_RST_PIN, 0);
	mt_set_gpio_dir(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_out(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_mode(GPIO_LCD_RST_PIN, 0);
	mt_set_gpio_dir(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_out(GPIO_LCD_RST_PIN, 0);
    MDELAY(30);
    mt_set_gpio_mode(GPIO_LCD_RST_PIN, 0);
	mt_set_gpio_dir(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_out(GPIO_LCD_RST_PIN, 1);
    MDELAY(120);

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	
	upmu_set_rg_vgp2_en(0);
	upmu_set_rg_vgp3_en(0);
	mt_set_gpio_mode(GPIO_LCD_RST_PIN, 0);
	mt_set_gpio_dir(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_out(GPIO_LCD_RST_PIN, 0);
}


static void lcm_resume(void)
{
    Lcd_Log("\n %s \n",__func__);

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
	int   array[4];
	char  buffer[5];
	unsigned int id=0;

	upmu_set_rg_vgp2_vosel(5);
	upmu_set_rg_vgp2_en(1);
	upmu_set_rg_vgp3_vosel(3);
	upmu_set_rg_vgp3_en(1);
	mt_set_gpio_mode(GPIO_LCD_RST_PIN, 0);
	mt_set_gpio_dir(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_out(GPIO_LCD_RST_PIN, 0);
	MDELAY(200);
	mt_set_gpio_mode(GPIO_LCD_RST_PIN, 0);
	mt_set_gpio_dir(GPIO_LCD_RST_PIN, 1);
	mt_set_gpio_out(GPIO_LCD_RST_PIN, 1);
	MDELAY(200);
	array[0] = 0x00083700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xA1,buffer,4);
	
	id=(buffer[2]<<8)+buffer[3];
#if defined(BUILD_LK)
		
	printf("MYCAT otm9605a, id = 0x%08x\n",  id);
	
#endif
	return ((LCM_ID == id)&&(get_lcd_id() == 1))?1:0;
}

LCM_DRIVER otm9605a_dzx_Y83708QHD50_BOE_dsi_2_vdo_qhd_lcm_drv =
    {
        .name			= "otm9605a_dzx_Y83708QHD50_BOE_dsi_2_vdo_qhd",
        .set_util_funcs = lcm_set_util_funcs,
        .get_params     = lcm_get_params,
        .init           = lcm_init,
        .suspend        = lcm_suspend,
        .resume         = lcm_resume,
        .compare_id     = lcm_compare_id
    };

