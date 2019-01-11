###########################################################
## Return non-empty if $(1) is a C identifier; i.e., if it
## matches /^[a-zA-Z_][a-zA-Z0-9_]*$/.  We do this by first
## making sure that it isn't empty and doesn't start with
## a digit, then by removing each valid character.  If the
## final result is empty, then it was a valid C identifier.
##
## $(1): word to check
###########################################################

_ici_digits := 0 1 2 3 4 5 6 7 8 9
_ici_alphaunderscore := \
    a b c d e f g h i j k l m n o p q r s t u v w x y z \
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z _
define is-c-identifier
$(strip \
  $(if $(1), \
    $(if $(filter $(addsuffix %,$(_ici_digits)),$(1)), \
     , \
      $(eval w := $(1)) \
      $(foreach c,$(_ici_digits) $(_ici_alphaunderscore), \
        $(eval w := $(subst $(c),,$(w))) \
       ) \
      $(if $(w),,TRUE) \
      $(eval w :=) \
     ) \
   ) \
 )
endef
# ---------------------------------------------------------------
# Include the product definitions.
# We need to do this to translate TARGET_PRODUCT into its
# underlying TARGET_DEVICE before we start defining any rules.
#
include $(BUILD_SYSTEM)/node_fns.mk
include $(BUILD_SYSTEM)/product.mk
include $(BUILD_SYSTEM)/device.mk

all_product_configs := $(get-all-product-makefiles)

# Find the product config makefile for the current product.
# all_product_configs consists items like:
# <product_name>:<path_to_the_product_makefile>
# or just <path_to_the_product_makefile> in case the product name is the
# same as the base filename of the product config makefile.
current_product_makefile :=
all_product_makefiles :=
$(foreach f, $(all_product_configs),\
    $(eval _cpm_words := $(subst :,$(space),$(f)))\
    $(eval _cpm_word1 := $(word 1,$(_cpm_words)))\
    $(eval _cpm_word2 := $(word 2,$(_cpm_words)))\
    $(if $(_cpm_word2),\
        $(eval all_product_makefiles += $(_cpm_word2))\
        $(if $(filter $(TARGET_PRODUCT),$(_cpm_word1)),\
            $(eval current_product_makefile += $(_cpm_word2)),),\
        $(eval all_product_makefiles += $(f))\
        $(if $(filter $(TARGET_PRODUCT),$(basename $(notdir $(f)))),\
            $(eval current_product_makefile += $(f)),)))
_cpm_words :=
_cpm_word1 :=
_cpm_word2 :=
current_product_makefile := $(strip $(current_product_makefile))
all_product_makefiles := $(strip $(all_product_makefiles))

ifneq (,$(filter product-graph dump-products, $(MAKECMDGOALS)))
# Import all product makefiles.
$(call import-products, $(all_product_makefiles))
else
# Import just the current product.
ifndef current_product_makefile
$(error Can not locate config makefile for product "$(TARGET_PRODUCT)")
endif
ifneq (1,$(words $(current_product_makefile)))
$(error Product "$(TARGET_PRODUCT)" ambiguous: matches $(current_product_makefile))
endif
$(call import-products, $(current_product_makefile))
endif  # Import all or just the current product makefile

# Sanity check
$(check-all-products)

ifneq ($(filter dump-products, $(MAKECMDGOALS)),)
$(dump-products)
$(error done)
endif

# Convert a short name like "sooner" into the path to the product
# file defining that product.
#
INTERNAL_PRODUCT := $(call resolve-short-product-name, $(TARGET_PRODUCT))
ifneq ($(current_product_makefile),$(INTERNAL_PRODUCT))
$(error PRODUCT_NAME inconsistent in $(current_product_makefile) and $(INTERNAL_PRODUCT))
endif
current_product_makefile :=
all_product_makefiles :=
all_product_configs :=


#############################################################################
# TODO: Remove this hack once only 1 runtime is left.
# Include the runtime product makefile based on the product's PRODUCT_RUNTIMES
$(call clear-var-list, $(_product_var_list))

# Set PRODUCT_RUNTIMES, allowing buildspec to override using OVERRIDE_RUNTIMES
product_runtimes := $(sort $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_RUNTIMES))
ifneq ($(OVERRIDE_RUNTIMES),)
  $(info Overriding PRODUCT_RUNTIMES=$(product_runtimes) with $(OVERRIDE_RUNTIMES))
  product_runtimes := $(OVERRIDE_RUNTIMES)
endif
$(foreach runtime, $(product_runtimes), $(eval include $(SRC_TARGET_DIR)/product/$(runtime).mk))
$(foreach v, $(_product_var_list), $(if $($(v)),\
    $(eval PRODUCTS.$(INTERNAL_PRODUCT).$(v) += $(sort $($(v))))))

$(call clear-var-list, $(_product_var_list))
# Now we can assign to PRODUCT_RUNTIMES
PRODUCT_RUNTIMES := $(product_runtimes)
product_runtimes :=

PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_PROPERTY_OVERRIDES += persist.sys.dalvik.vm.lib.2=$(DALVIK_VM_LIB)

ifeq ($(words $(PRODUCT_RUNTIMES)),1)
  # If we only have one runtime, we can strip classes.dex by default during dex_preopt
  DEX_PREOPT_DEFAULT := true
else
  # If we have more than one, we leave the classes.dex alone for post-boot analysis
  DEX_PREOPT_DEFAULT := nostripping
endif

#############################################################################

# A list of module names of BOOTCLASSPATH (jar files)
PRODUCT_BOOT_JARS := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_BOOT_JARS))
PRODUCT_SYSTEM_SERVER_JARS := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_SYSTEM_SERVER_JARS))

# Find the device that this product maps to.
TARGET_DEVICE := $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEVICE)

# Figure out which resoure configuration options to use for this
# product.
PRODUCT_LOCALES := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_LOCALES))
# TODO: also keep track of things like "port", "land" in product files.

# If CUSTOM_LOCALES contains any locales not already included
# in PRODUCT_LOCALES, add them to PRODUCT_LOCALES.
extra_locales := $(filter-out $(PRODUCT_LOCALES),$(CUSTOM_LOCALES))
ifneq (,$(extra_locales))
  ifneq ($(CALLED_FROM_SETUP),true)
    # Don't spam stdout, because envsetup.sh may be scraping values from it.
    $(info Adding CUSTOM_LOCALES [$(extra_locales)] to PRODUCT_LOCALES [$(PRODUCT_LOCALES)])
  endif
  PRODUCT_LOCALES += $(extra_locales)
  extra_locales :=
endif

# Add PRODUCT_LOCALES to PRODUCT_AAPT_CONFIG
PRODUCT_AAPT_CONFIG := $(strip $(PRODUCT_LOCALES) $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_AAPT_CONFIG))
PRODUCT_AAPT_PREF_CONFIG := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_AAPT_PREF_CONFIG))
PRODUCT_AAPT_PREBUILT_DPI := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_AAPT_PREBUILT_DPI))

# Keep a copy of the space-separated config
PRODUCT_AAPT_CONFIG_SP := $(PRODUCT_AAPT_CONFIG)

# Convert spaces to commas.
PRODUCT_AAPT_CONFIG := \
    $(subst $(space),$(comma),$(strip $(PRODUCT_AAPT_CONFIG)))

# product-scoped aapt flags
PRODUCT_AAPT_FLAGS :=
ifneq ($(filter en_XA ar_XB,$(PRODUCT_LOCALES)),)
# Force generating resources for pseudo-locales.
PRODUCT_AAPT_FLAGS += --pseudo-localize
endif

PRODUCT_BRAND := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_BRAND))

PRODUCT_MODEL := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_MODEL))
ifndef PRODUCT_MODEL
  PRODUCT_MODEL := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_NAME))
endif

PRODUCT_MANUFACTURER := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_MANUFACTURER))
ifndef PRODUCT_MANUFACTURER
  PRODUCT_MANUFACTURER := unknown
endif

ifeq ($(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_CHARACTERISTICS),)
  TARGET_AAPT_CHARACTERISTICS := default
else
  TARGET_AAPT_CHARACTERISTICS := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_CHARACTERISTICS))
endif

PRODUCT_DEFAULT_WIFI_CHANNELS := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEFAULT_WIFI_CHANNELS))

PRODUCT_DEFAULT_DEV_CERTIFICATE := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEFAULT_DEV_CERTIFICATE))
ifdef PRODUCT_DEFAULT_DEV_CERTIFICATE
ifneq (1,$(words $(PRODUCT_DEFAULT_DEV_CERTIFICATE)))
    $(error PRODUCT_DEFAULT_DEV_CERTIFICATE='$(PRODUCT_DEFAULT_DEV_CERTIFICATE)', \
      only 1 certificate is allowed.)
endif
endif

# A list of words like <source path>:<destination path>[:<owner>].
# The file at the source path should be copied to the destination path
# when building  this product.  <destination path> is relative to
# $(PRODUCT_OUT), so it should look like, e.g., "system/etc/file.xml".
# The rules for these copy steps are defined in build/core/Makefile.
# The optional :<owner> is used to indicate the owner of a vendor file.
PRODUCT_COPY_FILES := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_COPY_FILES))

# A list of property assignments, like "key = value", with zero or more
# whitespace characters on either side of the '='.
PRODUCT_PROPERTY_OVERRIDES := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_PROPERTY_OVERRIDES))

# A list of property assignments, like "key = value", with zero or more
# whitespace characters on either side of the '='.
# used for adding properties to default.prop
PRODUCT_DEFAULT_PROPERTY_OVERRIDES := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEFAULT_PROPERTY_OVERRIDES))

# Should we use the default resources or add any product specific overlays
PRODUCT_PACKAGE_OVERLAYS := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_PACKAGE_OVERLAYS))
DEVICE_PACKAGE_OVERLAYS := \
        $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).DEVICE_PACKAGE_OVERLAYS))

# An list of whitespace-separated words.
PRODUCT_TAGS := $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_TAGS))

# The list of product-specific kernel header dirs
PRODUCT_VENDOR_KERNEL_HEADERS := \
    $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_VENDOR_KERNEL_HEADERS)

# Add the product-defined properties to the build properties.
ADDITIONAL_BUILD_PROPERTIES := \
    $(ADDITIONAL_BUILD_PROPERTIES) \
    $(PRODUCT_PROPERTY_OVERRIDES)

# The OTA key(s) specified by the product config, if any.  The names
# of these keys are stored in the target-files zip so that post-build
# signing tools can substitute them for the test key embedded by
# default.
PRODUCT_OTA_PUBLIC_KEYS := $(sort \
    $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_OTA_PUBLIC_KEYS))

PRODUCT_EXTRA_RECOVERY_KEYS := $(sort \
    $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_EXTRA_RECOVERY_KEYS))

# If there is no room in /system for the image, place it in /data
PRODUCT_DEX_PREOPT_IMAGE_IN_DATA := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEX_PREOPT_IMAGE_IN_DATA))

PRODUCT_DEX_PREOPT_DEFAULT_FLAGS := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEX_PREOPT_DEFAULT_FLAGS))
PRODUCT_DEX_PREOPT_BOOT_FLAGS := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEX_PREOPT_BOOT_FLAGS))
# Resolve and setup per-module dex-preopot configs.
PRODUCT_DEX_PREOPT_MODULE_CONFIGS := \
    $(strip $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEX_PREOPT_MODULE_CONFIGS))
# If a module has multiple setups, the first takes precedence.
_pdpmc_modules :=
$(foreach c,$(PRODUCT_DEX_PREOPT_MODULE_CONFIGS),\
  $(eval m := $(firstword $(subst =,$(space),$(c))))\
  $(if $(filter $(_pdpmc_modules),$(m)),,\
    $(eval _pdpmc_modules += $(m))\
    $(eval cf := $(patsubst $(m)=%,%,$(c)))\
    $(eval cf := $(subst $(_PDPMC_SP_PLACE_HOLDER),$(space),$(cf)))\
    $(eval DEXPREOPT.$(TARGET_PRODUCT).$(m).CONFIG := $(cf))))
_pdpmc_modules :=
