$(call inherit-product-if-exists, target/linux/astar-common/common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := linux
PRODUCT_NAME := astar_joobot
PRODUCT_DEVICE := astar-joobot
PRODUCT_MODEL := Atar R16 CB-T
