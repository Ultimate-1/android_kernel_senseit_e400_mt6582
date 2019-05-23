#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 2,
    .polling_mode_ps =1,
    .polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
//  .als_level  = {  0,  30,  25,  50,  100,  200, 1600, 10000,  65535, 0, 0, 0, 0, 0, 0},
//  .als_value  = {150, 210, 300, 640, 1280, 2600, 6400, 60000, 100000, 0, 0, 0, 0, 0, 0, 0},
    .als_level  = {0,  10,  25,  50, 100,  200, 1200,  5000, 10000,  65535, 0, 0, 0, 0, 0},
    .als_value  = {1, 150, 210, 300, 640, 1280, 6400, 20000, 60000, 100000, 0, 0, 0, 0, 0, 0},
    .ps_threshold = 25,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

