AUTOMAKE_OPTIONS = subdir-objects
noinst_PROGRAMS = hicairo direct-image-surface gen-lion-cmds

hicairo_SOURCES = \
    hicairo.c \
    animals.c \
    tiger.inc \
    lion.inc \
    world-map.c \
    world-map.inc \
    $(NULL)

direct_image_surface_SOURCES = \
    direct-image-surface.c \
    animals.c \
    tiger.inc \
    lion.inc \
    world-map.c \
    world-map.inc \
    $(NULL)

direct_image_surface_LDADD = $(HICAIRO_LIBS)

gen_lion_cmds_SOURCES = gen-lion-cmds.c

EXTRA_DIST=
