#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#

unexport MAKEFILE_LIST
MK_NOGENDEPS := $(filter clean,$(MAKECMDGOALS))
override PKGDIR = xconfig_RoverMotherboard
XDCINCS = -I. -I$(strip $(subst ;, -I,$(subst $(space),\$(space),$(XPKGPATH))))
XDCCFGDIR = package/cfg/

#
# The following dependencies ensure package.mak is rebuilt
# in the event that some included BOM script changes.
#
ifneq (clean,$(MAKECMDGOALS))
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/utils.js:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/utils.js
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/xdc.tci:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/xdc.tci
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/template.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/template.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/om2.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/om2.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/xmlgen.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/xmlgen.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/xmlgen2.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/xmlgen2.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/Warnings.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/Warnings.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/IPackage.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/IPackage.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/package.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/package.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/global/Clock.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/global/Clock.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/global/Trace.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/global/Trace.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/bld.js:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/bld.js
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/BuildEnvironment.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/BuildEnvironment.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/PackageContents.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/PackageContents.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/_gen.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/_gen.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Library.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Library.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Executable.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Executable.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Repository.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Repository.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Configuration.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Configuration.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Script.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Script.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Manifest.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Manifest.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Utils.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/Utils.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITarget.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITarget.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITarget2.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITarget2.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITarget3.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITarget3.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITargetFilter.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/ITargetFilter.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/package.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/bld/package.xs
package.mak: config.bld
/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/ITarget.xs:
package.mak: /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/ITarget.xs
/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/C28_large.xs:
package.mak: /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/C28_large.xs
/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/C28_float.xs:
package.mak: /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/C28_float.xs
/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/package.xs:
package.mak: /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/package.xs
/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/arm/elf/IArm.xs:
package.mak: /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/arm/elf/IArm.xs
/opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/arm/elf/package.xs:
package.mak: /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/arm/elf/package.xs
package.mak: package.bld
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/compiler.opt.xdt:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/compiler.opt.xdt
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/io/File.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/io/File.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/io/package.xs:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/services/io/package.xs
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/compiler.defs.xdt:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/compiler.defs.xdt
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/custom.mak.exe.xdt:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/custom.mak.exe.xdt
/opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/package.xs.xdt:
package.mak: /opt/ti/xdctools_3_30_03_47_core/packages/xdc/tools/configuro/template/package.xs.xdt
endif

ti.targets.arm.elf.M4F.rootDir ?= /opt/ti/ccsv6/tools/compiler/arm_5.1.6
ti.targets.arm.elf.packageBase ?= /opt/ti/tirtos_tivac_2_10_01_38/products/bios_6_41_00_26/packages/ti/targets/arm/elf/
.PRECIOUS: $(XDCCFGDIR)/%.oem4f
.PHONY: all,em4f .dlls,em4f .executables,em4f test,em4f
all,em4f: .executables,em4f
.executables,em4f: .libraries,em4f
.executables,em4f: .dlls,em4f
.dlls,em4f: .libraries,em4f
.libraries,em4f: .interfaces
	@$(RM) $@
	@$(TOUCH) "$@"

.help::
	@$(ECHO) xdc test,em4f
	@$(ECHO) xdc .executables,em4f
	@$(ECHO) xdc .libraries,em4f
	@$(ECHO) xdc .dlls,em4f


all: .executables 
.executables: .libraries .dlls
.libraries: .interfaces

PKGCFGS := $(wildcard package.xs) package/build.cfg
.interfaces: package/package.xdc.inc package/package.defs.h package.xdc $(PKGCFGS)

-include package/package.xdc.dep
package/%.xdc.inc package/%_xconfig_RoverMotherboard.c package/%.defs.h: %.xdc $(PKGCFGS)
	@$(MSG) generating interfaces for package xconfig_RoverMotherboard" (because $@ is older than $(firstword $?))" ...
	$(XSRUN) -f xdc/services/intern/cmd/build.xs $(MK_IDLOPTS) -m package/package.xdc.dep -i package/package.xdc.inc package.xdc

.dlls,em4f .dlls: RoverMotherboard.pem4f

-include package/cfg/RoverMotherboard_pem4f.mak
-include package/cfg/RoverMotherboard_pem4f.cfg.mak
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/RoverMotherboard_pem4f.dep
endif
RoverMotherboard.pem4f: package/cfg/RoverMotherboard_pem4f.xdl
	@


ifeq (,$(wildcard .libraries,em4f))
RoverMotherboard.pem4f package/cfg/RoverMotherboard_pem4f.c: .libraries,em4f
endif

package/cfg/RoverMotherboard_pem4f.c package/cfg/RoverMotherboard_pem4f.h package/cfg/RoverMotherboard_pem4f.xdl: override _PROG_NAME := RoverMotherboard.xem4f
package/cfg/RoverMotherboard_pem4f.c: package/cfg/RoverMotherboard_pem4f.cfg

clean:: clean,em4f
	-$(RM) package/cfg/RoverMotherboard_pem4f.cfg
	-$(RM) package/cfg/RoverMotherboard_pem4f.dep
	-$(RM) package/cfg/RoverMotherboard_pem4f.c
	-$(RM) package/cfg/RoverMotherboard_pem4f.xdc.inc

clean,em4f::
	-$(RM) RoverMotherboard.pem4f
.executables,em4f .executables: RoverMotherboard.xem4f

RoverMotherboard.xem4f: |RoverMotherboard.pem4f

-include package/cfg/RoverMotherboard.xem4f.mak
RoverMotherboard.xem4f: package/cfg/RoverMotherboard_pem4f.oem4f 
	$(RM) $@
	@$(MSG) lnkem4f $@ ...
	$(RM) $(XDCCFGDIR)/$@.map
	$(ti.targets.arm.elf.M4F.rootDir)/bin/armlnk -w -q -u _c_int00 -fs $(XDCCFGDIR)$(dir $@)  -q -o $@ package/cfg/RoverMotherboard_pem4f.oem4f   package/cfg/RoverMotherboard_pem4f.xdl --silicon_version=7M4 --strict_compatibility=on -c -m $(XDCCFGDIR)/$@.map -l $(ti.targets.arm.elf.M4F.rootDir)/lib/libc.a
	
RoverMotherboard.xem4f: export C_DIR=
RoverMotherboard.xem4f: PATH:=$(ti.targets.arm.elf.M4F.rootDir)/bin/:$(PATH)

RoverMotherboard.test test,em4f test: RoverMotherboard.xem4f.test

RoverMotherboard.xem4f.test:: RoverMotherboard.xem4f
ifeq (,$(_TESTLEVEL))
	@$(MAKE) -R -r --no-print-directory -f $(XDCROOT)/packages/xdc/bld/xdc.mak _TESTLEVEL=1 RoverMotherboard.xem4f.test
else
	@$(MSG) running $<  ...
	$(call EXEC.RoverMotherboard.xem4f, ) 
endif

clean,em4f::
	-$(RM) $(wildcard .tmp,RoverMotherboard.xem4f,*)


clean:: clean,em4f

clean,em4f::
	-$(RM) RoverMotherboard.xem4f
%,copy:
	@$(if $<,,$(MSG) don\'t know how to build $*; exit 1)
	@$(MSG) cp $< $@
	$(RM) $@
	$(CP) $< $@
RoverMotherboard_pem4f.oem4f,copy : package/cfg/RoverMotherboard_pem4f.oem4f
RoverMotherboard_pem4f.sem4f,copy : package/cfg/RoverMotherboard_pem4f.sem4f

$(XDCCFGDIR)%.c $(XDCCFGDIR)%.h $(XDCCFGDIR)%.xdl: $(XDCCFGDIR)%.cfg .interfaces $(XDCROOT)/packages/xdc/cfg/Main.xs
	@$(MSG) "configuring $(_PROG_NAME) from $< ..."
	$(CONFIG) $(_PROG_XSOPTS) xdc.cfg $(_PROG_NAME) $(XDCCFGDIR)$*.cfg $(XDCCFGDIR)$*

.PHONY: release,xconfig_RoverMotherboard
xconfig_RoverMotherboard.tar: package/package.bld.xml
xconfig_RoverMotherboard.tar: package/build.cfg
xconfig_RoverMotherboard.tar: package/package.xdc.inc
ifeq (,$(MK_NOGENDEPS))
-include package/rel/xconfig_RoverMotherboard.tar.dep
endif
package/rel/xconfig_RoverMotherboard/xconfig_RoverMotherboard/package/package.rel.xml: .force
	@$(MSG) generating external release references $@ ...
	$(XS) $(JSENV) -f $(XDCROOT)/packages/xdc/bld/rel.js $(MK_RELOPTS) . $@

xconfig_RoverMotherboard.tar: package/rel/xconfig_RoverMotherboard.xdc.inc package/rel/xconfig_RoverMotherboard/xconfig_RoverMotherboard/package/package.rel.xml
	@$(MSG) making release file $@ "(because of $(firstword $?))" ...
	-$(RM) $@
	$(call MKRELTAR,package/rel/xconfig_RoverMotherboard.xdc.inc,package/rel/xconfig_RoverMotherboard.tar.dep)


release release,xconfig_RoverMotherboard: all xconfig_RoverMotherboard.tar
clean:: .clean
	-$(RM) xconfig_RoverMotherboard.tar
	-$(RM) package/rel/xconfig_RoverMotherboard.xdc.inc
	-$(RM) package/rel/xconfig_RoverMotherboard.tar.dep

clean:: .clean
	-$(RM) .libraries $(wildcard .libraries,*)
clean:: 
	-$(RM) .dlls $(wildcard .dlls,*)
#
# The following clean rule removes user specified
# generated files or directories.
#

ifneq (clean,$(MAKECMDGOALS))
ifeq (,$(wildcard package))
    $(shell $(MKDIR) package)
endif
ifeq (,$(wildcard package/cfg))
    $(shell $(MKDIR) package/cfg)
endif
ifeq (,$(wildcard package/lib))
    $(shell $(MKDIR) package/lib)
endif
ifeq (,$(wildcard package/rel))
    $(shell $(MKDIR) package/rel)
endif
ifeq (,$(wildcard package/internal))
    $(shell $(MKDIR) package/internal)
endif
endif
clean::
	-$(RMDIR) package

include custom.mak