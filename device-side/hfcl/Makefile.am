SUBDIRS = src include m4

COMM_HDRS = hfclconfig.h

# The directory where the include files will be installed
libhfclconfigdir = $(includedir)/hfcl

# Which header files to install
libhfclconfig_HEADERS = $(COMM_HDRS)

EXTRA_DIST = README.md RELEASE-NOTES.md INSTALL autogen.sh

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = hfcl.pc
