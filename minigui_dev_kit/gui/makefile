
MODULES:= freetype tslib zlib jpeg libpng Chipmunk2D minigui libmgplus libmgutils libmgeff libmgncs
MODULES+= minigui-res

APP_MODULES+= libmgi libmgncs4touch  helloworld


MODULES_CONFIG:=$(patsubst %,%_config,$(MODULES))
MODULES_BUILD:=$(patsubst %,%_build,$(MODULES))
MODULES_INSTALL:=$(patsubst %,%_install,$(MODULES))
MODULES_CLEAN:=$(patsubst %,%_clean,$(MODULES))


APP_MODULES_CONFIG:=$(patsubst %,%_config,$(APP_MODULES))
APP_MODULES_BUILD:=$(patsubst %,%_build,$(APP_MODULES))
APP_MODULES_INSTALL:=$(patsubst %,%_install,$(APP_MODULES))
APP_MODULES_CLEAN:=$(patsubst %,%_clean,$(APP_MODULES))

ALKAID_PROJECT_PATH:=/home/yongliang.li/i2m/project

PWD=`pwd`
EPREFIX=$(PWD)/library
PREFIX=$(PWD)/library

include $(ALKAID_PROJECT_PATH)/configs/current.configs

SDKLIBS=$(PROJ_ROOT)/release/$(PRODUCT)/$(CHIP)/$(BOARD)/$(TOOLCHAIN)/$(TOOLCHAIN_VERSION)/lib/dynamic
SDKINCS=$(PROJ_ROOT)/release//include
UAPIINCS=$(PROJ_ROOT)/kbuild/$(KERNEL_VERSION)/include/uapi/mstar

PLATFORM:=$(shell echo $(CHIP) | tr A-Z a-z )
CHIP_WITH_GFX:=i2m i2  
CHIP_WITH_DMA:=i5 i6
GRAPHIC_ACCEL:=
ifneq ($(findstring $(PLATFORM),$(CHIP_WITH_GFX)),)
PLAT_DEPENDENT_LIB="-lmi_gfx -lmi_sys -lmi_common -ldl"
GRAPHIC_ACCEL=GFX
else
PLAT_DEPENDENT_LIB=" -lmi_sys -lmi_common -ldl"
GRAPHIC_ACCEL=DMA
endif

MINIGUI_CROSS_COMPILE:=$(patsubst %-,%,$(TOOLCHAIN_REL))

GCCVERSION = $(shell $(MINIGUI_CROSS_COMPILE)-gcc -dumpversion)

.PHONY: all strip build install clean info $(MODULES_BUILD) $(MODULES_INSTALL) $(MODULES_CLEAN)

all: info libs apps strip

clean: lib_clean app_clean

info:
	@echo =====================PLATFORM INFO=========================
	@echo chip=$(PLATFORM) board=$(BOARD)
	@echo kversion=$(KERNEL_VERSION) 
	@echo toolchain=$(TOOLCHAIN) version=$(TOOLCHAIN_VERSION)
	@echo product=$(PRODUCT)
	@echo ===========================================================
ifneq ($(GCCVERSION),$(TOOLCHAIN_VERSION))
	@echo -e "\033[31mexpected gcc $(TOOLCHAIN_VERSION),but got $(GCCVERSION) \033[0m"
	@exit 1
endif
	@sleep 3

libs: lib_config lib_build lib_install

$(MODULES_CONFIG):%_config:
ifeq ($(wildcard $(SDKLIBS)),)
	@echo -e "\033[31mno SDKLIBS:$(SDKLIBS) modify ALKAID_PROJECT_PATH int ./makefile!!!! \033[0m"
	@exit 1
else
ifeq ($(wildcard $(SDKINCS)),)
	@echo -e "\033[31mno SDKINCS:$(SDKINCS) modify ALKAID_PROJECT_PATH int ./makefile!!!! \033[0m"
	@exit 1
else
ifeq ($(wildcard $(UAPIINCS)),)
	@echo -e "\033[31mno UAPIINCS:$(UAPIINCS) modify ALKAID_PROJECT_PATH int ./makefile!!!! \033[0m"
	@exit 1
else
	@sh config.sh $(patsubst %_config,src/%,$@) $(PREFIX) $(EPREFIX) $(SDKLIBS) $(SDKINCS) $(UAPIINCS) $(MINIGUI_CROSS_COMPILE) $(PLAT_DEPENDENT_LIB) $(GRAPHIC_ACCEL)
endif
endif
endif

lib_config:$(MODULES_CONFIG)
	@echo done!

$(MODULES_BUILD):%_build:
	$(MAKE) -C $(patsubst %_build,src/%,$@)
	$(MAKE) -C $(patsubst %_build,src/%,$@) install
lib_build:$(MODULES_BUILD)
	@echo done!

$(MODULES_INSTALL):%_install:
	$(MAKE) -C $(patsubst %_install,src/%,$@) install
lib_install:$(MODULES_INSTALL)
	@echo done!

$(MODULES_CLEAN):%_clean:
	$(MAKE) -C $(patsubst %_clean,src/%,$@) clean
lib_clean:$(MODULES_CLEAN)
	@echo done!

apps: app_config app_build

$(APP_MODULES_CONFIG):%_config:
ifeq ($(wildcard $(SDKLIBS)),)
	@echo -e "\033[31mno SDKLIBS:$(SDKLIBS) modify ALKAID_PROJECT_PATH int ./makefile!!!! \033[0m"
	@exit 1
else
ifeq ($(wildcard $(SDKINCS)),)
	@echo -e "\033[31mno SDKINCS:$(SDKINCS) modify ALKAID_PROJECT_PATH int ./makefile!!!! \033[0m"
	@exit 1

else
ifeq ($(wildcard $(UAPIINCS)),)
	@echo -e "\033[31mno UAPIINCS:$(UAPIINCS) modify ALKAID_PROJECT_PATH int ./makefile!!!! \033[0m"
	@exit 1
else
	@sh config.sh $(patsubst %_config,src/%,$@) $(PREFIX) $(EPREFIX) $(SDKLIBS) $(SDKINCS) $(UAPIINCS) $(MINIGUI_CROSS_COMPILE) $(PLAT_DEPENDENT_LIB) $(GRAPHIC_ACCEL)
endif
endif
endif

app_config:$(APP_MODULES_CONFIG)
	@echo done!

$(APP_MODULES_BUILD):%_build:
	$(MAKE) -C $(patsubst %_build,src/%,$@)
	$(MAKE) -C $(patsubst %_build,src/%,$@) install
app_build:$(APP_MODULES_BUILD)
	@echo done!

$(APP_MODULES_INSTALL):%_install:
	$(MAKE) -C $(patsubst %_install,src/%,$@) install
app_install:$(APP_MODULES_INSTALL)
	@echo done!

$(APP_MODULES_CLEAN):%_clean:
	$(MAKE) -C $(patsubst %_clean,src/%,$@) clean
app_clean:$(APP_MODULES_CLEAN)
	@echo done!

strip:
	$(MINIGUI_CROSS_COMPILE)-strip $(EPREFIX)/lib/*.so* --strip-unneeded
