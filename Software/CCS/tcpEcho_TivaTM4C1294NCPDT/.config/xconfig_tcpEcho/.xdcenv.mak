#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /opt/ti/tirtos_tivac_2_10_01_38/packages;/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages;/opt/ti/tirtos_tivac_2_10_01_38/products/ndk_2_24_01_18/packages;/opt/ti/tirtos_tivac_2_10_01_38/products/uia_2_00_02_39/packages;/opt/ti/ccsv6/ccs_base;/media/owen/7E46880B4687C1FF/Dev/MRDT/Motherboard/Software/CCS/tcpEcho_TivaTM4C1294NCPDT/.config
override XDCROOT = /opt/ti/xdctools_3_30_03_47_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /opt/ti/tirtos_tivac_2_10_01_38/packages;/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages;/opt/ti/tirtos_tivac_2_10_01_38/products/ndk_2_24_01_18/packages;/opt/ti/tirtos_tivac_2_10_01_38/products/uia_2_00_02_39/packages;/opt/ti/ccsv6/ccs_base;/media/owen/7E46880B4687C1FF/Dev/MRDT/Motherboard/Software/CCS/tcpEcho_TivaTM4C1294NCPDT/.config;/opt/ti/xdctools_3_30_03_47_core/packages;..
HOSTOS = Linux
endif
