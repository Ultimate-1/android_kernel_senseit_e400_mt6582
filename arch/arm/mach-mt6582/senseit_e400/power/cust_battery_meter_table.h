#ifndef _CUST_BATTERY_METER_TABLE_H
#define _CUST_BATTERY_METER_TABLE_H

#include <mach/mt_typedefs.h>

// ============================================================
// define
// ============================================================
#define BAT_NTC_10 1
#define BAT_NTC_47 0

#if (BAT_NTC_10 == 1)
#define RBAT_PULL_UP_R             16900	
#define RBAT_PULL_DOWN_R		   27000	
#endif

#if (BAT_NTC_47 == 1)
#define RBAT_PULL_UP_R             61900	
#define RBAT_PULL_DOWN_R		  100000	
#endif
#define RBAT_PULL_UP_VOLT          1800



// ============================================================
// ENUM
// ============================================================

// ============================================================
// structure
// ============================================================

// ============================================================
// typedef
// ============================================================
typedef struct _BATTERY_PROFILE_STRUC
{
    kal_int32 percentage;
    kal_int32 voltage;
} BATTERY_PROFILE_STRUC, *BATTERY_PROFILE_STRUC_P;

typedef struct _R_PROFILE_STRUC
{
    kal_int32 resistance; // Ohm
    kal_int32 voltage;
} R_PROFILE_STRUC, *R_PROFILE_STRUC_P;

typedef enum
{
    T1_0C,
    T2_25C,
    T3_50C
} PROFILE_TEMPERATURE;

// ============================================================
// External Variables
// ============================================================

// ============================================================
// External function
// ============================================================

// ============================================================
// <DOD, Battery_Voltage> Table
// ============================================================
//BEGIN <battary> <DATE20130620> <battary tempreature> zhangxiaofei
#if (BAT_NTC_10 == 1)
    BATT_TEMPERATURE Batt_Temperature_Table[] = {
    	 {-30,124607},
    	 {-25,94918},
        {-20,68237},
        {-15,53650},
        {-10,42506},
        { -5,33892},
        {  0,27219},
        {  5,22021},
        { 10,17926},
        { 15,14674},
        { 20,12081},
        { 25,10000},
        { 30,8315},
        { 35,6948},
        { 40,5834},
        { 45,4917},
        { 50,4161},
        //{ 55,3367}, // huihurma
        { 55,3535},
        { 60,3014},
        { 65,2494},
        { 70,2130},
        { 75,1817},
        { 80,1562}
    };
#endif
//END <battary> <DATE20130620> <battary tempreature> zhangxiaofei
#if (BAT_NTC_47 == 1)
    BATT_TEMPERATURE Batt_Temperature_Table[] = {
        {-20,483954},
        {-15,360850},
        {-10,271697},
        { -5,206463},
        {  0,158214},
        {  5,122259},
        { 10,95227},
        { 15,74730},
        { 20,59065},
        { 25,47000},
        { 30,37643},
        { 35,30334},
        { 40,24591},
        { 45,20048},
        { 50,16433},
        { 55,13539},
        { 60,11210}        
    };
#endif

//BEGIN <battary> <DATE20140102> <battary > lishengli
// T0 -10C
BATTERY_PROFILE_STRUC battery_profile_t0[] =
{
//DOD0  OCV -10
{ 0 , 4200 },
{ 1 , 4184 },
{ 3 , 4169 },
{ 4 , 4152 },
{ 6 , 4138 },
{ 7 , 4119 },
{ 8 , 4106 },
{ 10 , 4094 },
{ 11 , 4081 },
{ 13 , 4069 },
{ 14 , 4057 },
{ 16 , 4046 },
{ 17 , 4034 },
{ 18 , 4023 },
{ 20 , 4013 },
{ 21 , 4002 },
{ 23 , 3992 },
{ 24 , 3982 },
{ 25 , 3973 },
{ 27 , 3963 },
{ 28 , 3954 },
{ 30 , 3945 },
{ 31 , 3936 },
{ 32 , 3927 },
{ 34 , 3919 },
{ 35 , 3910 },
{ 37 , 3901 },
{ 38 , 3890 },
{ 40 , 3878 },
{ 41 , 3865 },
{ 42 , 3855 },
{ 44 , 3847 },
{ 45 , 3840 },
{ 47 , 3833 },
{ 48 , 3828 },
{ 49 , 3821 },
{ 51 , 3817 },
{ 52 , 3812 },
{ 54 , 3807 },
{ 55 , 3803 },
{ 56 , 3799 },
{ 58 , 3795 },
{ 59 , 3791 },
{ 61 , 3788 },
{ 62 , 3785 },
{ 63 , 3782 },
{ 65 , 3778 },
{ 66 , 3775 },
{ 68 , 3773 },
{ 69 , 3767 },
{ 71 , 3758 },
{ 72 , 3749 },
{ 73 , 3744 },
{ 75 , 3738 },
{ 76 , 3733 },
{ 78 , 3728 },
{ 79 , 3724 },
{ 80 , 3719 },
{ 82 , 3714 },
{ 83 , 3708 },
{ 85 , 3701 },
{ 86 , 3694 },
{ 87 , 3687 },
{ 89 , 3678 },
{ 90 , 3674 },
{ 92 , 3672 },
{ 93 , 3668 },
{ 95 , 3659 },
{ 96 , 3628 },
{ 97 , 3607 },
{ 99 , 3545 },
{ 100, 3464 }
};              
                
// T1 0C        
BATTERY_PROFILE_STRUC battery_profile_t1[] =
{
	//DOD0	OCV 0
{ 0 , 4200 },
{ 1 , 4184 },
{ 3 , 4169 },
{ 4 , 4152 },
{ 6 , 4138 },
{ 7 , 4119 },
{ 8 , 4106 },
{ 10 , 4094 },
{ 11 , 4081 },
{ 13 , 4069 },
{ 14 , 4057 },
{ 16 , 4046 },
{ 17 , 4034 },
{ 18 , 4023 },
{ 20 , 4013 },
{ 21 , 4002 },
{ 23 , 3992 },
{ 24 , 3982 },
{ 25 , 3973 },
{ 27 , 3963 },
{ 28 , 3954 },
{ 30 , 3945 },
{ 31 , 3936 },
{ 32 , 3927 },
{ 34 , 3919 },
{ 35 , 3910 },
{ 37 , 3901 },
{ 38 , 3890 },
{ 40 , 3878 },
{ 41 , 3865 },
{ 42 , 3855 },
{ 44 , 3847 },
{ 45 , 3840 },
{ 47 , 3833 },
{ 48 , 3828 },
{ 49 , 3821 },
{ 51 , 3817 },
{ 52 , 3812 },
{ 54 , 3807 },
{ 55 , 3803 },
{ 56 , 3799 },
{ 58 , 3795 },
{ 59 , 3791 },
{ 61 , 3788 },
{ 62 , 3785 },
{ 63 , 3782 },
{ 65 , 3778 },
{ 66 , 3775 },
{ 68 , 3773 },
{ 69 , 3767 },
{ 71 , 3758 },
{ 72 , 3749 },
{ 73 , 3744 },
{ 75 , 3738 },
{ 76 , 3733 },
{ 78 , 3728 },
{ 79 , 3724 },
{ 80 , 3719 },
{ 82 , 3714 },
{ 83 , 3708 },
{ 85 , 3701 },
{ 86 , 3694 },
{ 87 , 3687 },
{ 89 , 3678 },
{ 90 , 3674 },
{ 92 , 3672 },
{ 93 , 3668 },
{ 95 , 3659 },
{ 96 , 3628 },
{ 97 , 3607 },
{ 99 , 3545 },
{ 100, 3464 }
};              

// T2 25C
BATTERY_PROFILE_STRUC battery_profile_t2[] =
{
	//DOD0	OCV 25	
{ 0 , 4200 },
{ 1 , 4184 },
{ 3 , 4169 },
{ 4 , 4152 },
{ 6 , 4138 },
{ 7 , 4119 },
{ 8 , 4106 },
{ 10 , 4094 },
{ 11 , 4081 },
{ 13 , 4069 },
{ 14 , 4057 },
{ 16 , 4046 },
{ 17 , 4034 },
{ 18 , 4023 },
{ 20 , 4013 },
{ 21 , 4002 },
{ 23 , 3992 },
{ 24 , 3982 },
{ 25 , 3973 },
{ 27 , 3963 },
{ 28 , 3954 },
{ 30 , 3945 },
{ 31 , 3936 },
{ 32 , 3927 },
{ 34 , 3919 },
{ 35 , 3910 },
{ 37 , 3901 },
{ 38 , 3890 },
{ 40 , 3878 },
{ 41 , 3865 },
{ 42 , 3855 },
{ 44 , 3847 },
{ 45 , 3840 },
{ 47 , 3833 },
{ 48 , 3828 },
{ 49 , 3821 },
{ 51 , 3817 },
{ 52 , 3812 },
{ 54 , 3807 },
{ 55 , 3803 },
{ 56 , 3799 },
{ 58 , 3795 },
{ 59 , 3791 },
{ 61 , 3788 },
{ 62 , 3785 },
{ 63 , 3782 },
{ 65 , 3778 },
{ 66 , 3775 },
{ 68 , 3773 },
{ 69 , 3767 },
{ 71 , 3758 },
{ 72 , 3749 },
{ 73 , 3744 },
{ 75 , 3738 },
{ 76 , 3733 },
{ 78 , 3728 },
{ 79 , 3724 },
{ 80 , 3719 },
{ 82 , 3714 },
{ 83 , 3708 },
{ 85 , 3701 },
{ 86 , 3694 },
{ 87 , 3687 },
{ 89 , 3678 },
{ 90 , 3674 },
{ 92 , 3672 },
{ 93 , 3668 },
{ 95 , 3659 },
{ 96 , 3628 },
{ 97 , 3607 },
{ 99 , 3545 },
{ 100, 3464 }
};              

// T3 50C
BATTERY_PROFILE_STRUC battery_profile_t3[] =
{
	//DOD0	OCV 50	
{ 0 , 4200 },
{ 1 , 4184 },
{ 3 , 4169 },
{ 4 , 4152 },
{ 6 , 4138 },
{ 7 , 4119 },
{ 8 , 4106 },
{ 10 , 4094 },
{ 11 , 4081 },
{ 13 , 4069 },
{ 14 , 4057 },
{ 16 , 4046 },
{ 17 , 4034 },
{ 18 , 4023 },
{ 20 , 4013 },
{ 21 , 4002 },
{ 23 , 3992 },
{ 24 , 3982 },
{ 25 , 3973 },
{ 27 , 3963 },
{ 28 , 3954 },
{ 30 , 3945 },
{ 31 , 3936 },
{ 32 , 3927 },
{ 34 , 3919 },
{ 35 , 3910 },
{ 37 , 3901 },
{ 38 , 3890 },
{ 40 , 3878 },
{ 41 , 3865 },
{ 42 , 3855 },
{ 44 , 3847 },
{ 45 , 3840 },
{ 47 , 3833 },
{ 48 , 3828 },
{ 49 , 3821 },
{ 51 , 3817 },
{ 52 , 3812 },
{ 54 , 3807 },
{ 55 , 3803 },
{ 56 , 3799 },
{ 58 , 3795 },
{ 59 , 3791 },
{ 61 , 3788 },
{ 62 , 3785 },
{ 63 , 3782 },
{ 65 , 3778 },
{ 66 , 3775 },
{ 68 , 3773 },
{ 69 , 3767 },
{ 71 , 3758 },
{ 72 , 3749 },
{ 73 , 3744 },
{ 75 , 3738 },
{ 76 , 3733 },
{ 78 , 3728 },
{ 79 , 3724 },
{ 80 , 3719 },
{ 82 , 3714 },
{ 83 , 3708 },
{ 85 , 3701 },
{ 86 , 3694 },
{ 87 , 3687 },
{ 89 , 3678 },
{ 90 , 3674 },
{ 92 , 3672 },
{ 93 , 3668 },
{ 95 , 3659 },
{ 96 , 3628 },
{ 97 , 3607 },
{ 99 , 3545 },
{ 100, 3464 }
};              

// battery profile for actual temperature. The size should be the same as T1, T2 and T3
BATTERY_PROFILE_STRUC battery_profile_temperature[] =
{
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	}
};           
 
// ============================================================
// <Rbat, Battery_Voltage> Table
// ============================================================
// T0 -10C
R_PROFILE_STRUC r_profile_t0[] =
{
	//DOD0	R -10
{	204,	4335 	},
{	187,	4296 	},
{	181,	4254 	},
{	186,	4201 	},
{	184,	4167 	},
{	190,	4135 	},
{	193,	4104 	},
{	205,	4063 	},
{	210,	4030 	},
{	205,	4001 	},
{	215,	3980 	},
{	221,	3958 	},
{	213,	3921 	},
{	191,	3892 	},
{	174,	3867 	},
{	164,	3847 	},
{	164,	3832 	},
{	161,	3819 	},
{	159,	3808 	},
{	158,	3799 	},
{	158,	3791 	},
{	158,	3783 	},
{	158,	3777 	},
{	156,	3770 	},
{	157,	3761 	},
{	156,	3751 	},
{	162,	3741 	},
{	168,	3726 	},
{	176,	3710 	},
{	162,	3692 	},
{	199,	3687 	},
{	385,	3647 	},
{	534,	3575 	},
{	661,	3464 	}
};           

// T1 0C
R_PROFILE_STRUC r_profile_t1[] =
{
	//DOD0	R 0
{	204,	4335 	},
{	187,	4296 	},
{	181,	4254 	},
{	186,	4201 	},
{	184,	4167 	},
{	190,	4135 	},
{	193,	4104 	},
{	205,	4063 	},
{	210,	4030 	},
{	205,	4001 	},
{	215,	3980 	},
{	221,	3958 	},
{	213,	3921 	},
{	191,	3892 	},
{	174,	3867 	},
{	164,	3847 	},
{	164,	3832 	},
{	161,	3819 	},
{	159,	3808 	},
{	158,	3799 	},
{	158,	3791 	},
{	158,	3783 	},
{	158,	3777 	},
{	156,	3770 	},
{	157,	3761 	},
{	156,	3751 	},
{	162,	3741 	},
{	168,	3726 	},
{	176,	3710 	},
{	162,	3692 	},
{	199,	3687 	},
{	385,	3647 	},
{	534,	3575 	},
{	661,	3464 	}
};           

// T2 25C
R_PROFILE_STRUC r_profile_t2[] =
{
	//DOD0	R 25
	
{	204,	4335 	},
{	187,	4296 	},
{	181,	4254 	},
{	186,	4201 	},
{	184,	4167 	},
{	190,	4135 	},
{	193,	4104 	},
{	205,	4063 	},
{	210,	4030 	},
{	205,	4001 	},
{	215,	3980 	},
{	221,	3958 	},
{	213,	3921 	},
{	191,	3892 	},
{	174,	3867 	},
{	164,	3847 	},
{	164,	3832 	},
{	161,	3819 	},
{	159,	3808 	},
{	158,	3799 	},
{	158,	3791 	},
{	158,	3783 	},
{	158,	3777 	},
{	156,	3770 	},
{	157,	3761 	},
{	156,	3751 	},
{	162,	3741 	},
{	168,	3726 	},
{	176,	3710 	},
{	162,	3692 	},
{	199,	3687 	},
{	385,	3647 	},
{	534,	3575 	},
{	661,	3464 	}
};           

// T3 50C
R_PROFILE_STRUC r_profile_t3[] =
{
	//DOD0	R 50
{	204,	4335 	},
{	187,	4296 	},
{	181,	4254 	},
{	186,	4201 	},
{	184,	4167 	},
{	190,	4135 	},
{	193,	4104 	},
{	205,	4063 	},
{	210,	4030 	},
{	205,	4001 	},
{	215,	3980 	},
{	221,	3958 	},
{	213,	3921 	},
{	191,	3892 	},
{	174,	3867 	},
{	164,	3847 	},
{	164,	3832 	},
{	161,	3819 	},
{	159,	3808 	},
{	158,	3799 	},
{	158,	3791 	},
{	158,	3783 	},
{	158,	3777 	},
{	156,	3770 	},
{	157,	3761 	},
{	156,	3751 	},
{	162,	3741 	},
{	168,	3726 	},
{	176,	3710 	},
{	162,	3692 	},
{	199,	3687 	},
{	385,	3647 	},
{	534,	3575 	},
{	661,	3464 	},
};           

// r-table profile for actual temperature. The size should be the same as T1, T2 and T3
R_PROFILE_STRUC r_profile_temperature[] =
{
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	},
{	0,	0 	}
};           
//END <battary> <DATE20140102> <battary > lishengli

// ============================================================
// function prototype
// ============================================================
int fgauge_get_saddles(void);
BATTERY_PROFILE_STRUC_P fgauge_get_profile(kal_uint32 temperature);

int fgauge_get_saddles_r_table(void);
R_PROFILE_STRUC_P fgauge_get_profile_r_table(kal_uint32 temperature);

#endif	//#ifndef _CUST_BATTERY_METER_TABLE_H
