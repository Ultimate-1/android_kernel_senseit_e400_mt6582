include $(srctree)/drivers/misc/mediatek/Makefile.custom

# In case the platform does NOT support this type of sensors
obj-y   +=  alsps.o
obj-y   +=  alsps_factory.o
obj-y   +=  aal_control.o

ifeq ($(CONFIG_MTK_LTR559_LITEON),y)
obj-y	+=  ltr559_liteon/
endif

ifeq ($(CONFIG_MTK_GP2AP002S00F),y)
obj-y	+=  gp2ap002s00f/
endif

ifeq ($(CONFIG_MTK_CM36283),y)
obj-y	+=  cm36283/
endif

ifeq ($(CONFIG_MTK_APM_16D),y)
obj-y	+=  apm_16d/
endif

ifeq ($(CONFIG_MTK_EPL2182),y)
obj-y	+=  epl2182/
endif

ifeq ($(CONFIG_MTK_EPL2182_NEW),y)
obj-y	+=  EPL2182-new/
endif
ifeq ($(CONFIG_MTK_STK3X1X),y)
obj-y	+=  stk3x1x/
endif
ifeq ($(CONFIG_MTK_CM36652_NEW),y)
obj-y	+=  cm36652-new/
endif

ifeq ($(CONFIG_MTK_APDS9930),y)
obj-y	+=  APDS9930/
endif

ifeq ($(CONFIG_MTK_AP3216C),y)
obj-y	+=  ap3216c/
endif

ifeq ($(CONFIG_MTK_CM3232_NEW),y)
obj-y	+=  cm3232-new/
endif

ifeq ($(CONFIG_MTK_APDS9930_NEW),y)
obj-y	+=  APDS9930-new/
endif
