PATH_PREFIX:=..

include ../common.mk

HOST_STAMP_BUILT:=$(HOST_BUILD_DIR)/.headers_built
HOST_STAMP_INSTALLED:=$(TOOLCHAIN_DIR)/stamp/.musl_headers_installed

define Host/Compile
endef

define Host/Install
	$(CP) $(COMPILE_DIR_TOOLCHAIN)/linux-dev/include/*  $(HOST_BUILD_DIR)/include
endef

$(eval $(call HostBuild))

