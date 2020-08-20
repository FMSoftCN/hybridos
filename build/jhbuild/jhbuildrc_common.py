#!/usr/bin/env python

import multiprocessing
import sys
import os
import platform


script_dir = None


def script_path(*args):
    global script_dir
    if not script_dir:
        script_dir = os.path.join(os.path.dirname(__file__), '..', 'Scripts')
    return os.path.join(*(script_dir,) + args)


def top_level_path(*args):
    return os.path.join(*((os.path.join(os.path.dirname(__file__), '..', '..'),) + args))


def init(jhbuildrc_globals, jhbuild_platform):

    __tools_directory = os.path.join(os.path.dirname(__file__), "../", jhbuild_platform)
    sys.path.insert(0, __tools_directory)

    jhbuildrc_globals["build_policy"] = 'updated'

    __moduleset_file_uri = 'file://' + os.path.join(__tools_directory, 'jhbuild.modules')
    __extra_modulesets = os.environ.get("HYBRIDOS_EXTRA_MODULESETS", "").split(",")
    jhbuildrc_globals["moduleset"] = [__moduleset_file_uri, ]
    if __extra_modulesets != ['']:
        jhbuildrc_globals["moduleset"].extend(__extra_modulesets)

    __extra_modules = os.environ.get("HYBRIDOS_EXTRA_MODULES", "").split(",")
    jhbuildrc_globals["modules"] = ['hybridos' + jhbuild_platform + '-testing-dependencies', ]
    if __extra_modules != ['']:
        jhbuildrc_globals["modules"].extend(__extra_modules)

    if 'HYBRIDOS_OUTPUTDIR' in os.environ:
        jhbuildrc_globals["checkoutroot"] = checkoutroot = os.path.abspath(os.path.join(os.environ['HYBRIDOS_OUTPUTDIR'], 'Dependencies' + jhbuild_platform.upper(), 'Source'))
        jhbuildrc_globals["prefix"] = os.path.abspath(os.path.join(os.environ['HYBRIDOS_OUTPUTDIR'], 'Dependencies' + jhbuild_platform.upper(), 'Root'))
    else:
        jhbuildrc_globals["checkoutroot"] = checkoutroot = os.path.abspath(top_level_path('Output', 'Dependencies' + jhbuild_platform.upper(), 'Source'))
        jhbuildrc_globals["prefix"] = os.path.abspath(top_level_path('Output', 'Dependencies' + jhbuild_platform.upper(), 'Root'))

    jhbuildrc_globals["nonotify"] = True
    jhbuildrc_globals["notrayicon"] = True

    if 'NUMBER_OF_PROCESSORS' in os.environ:
        jhbuildrc_globals['jobs'] = os.environ['NUMBER_OF_PROCESSORS']

    # Avoid runtime conflicts with GStreamer system-wide plugins. We want
    # to use only the plugins we build in JHBuild.
    os.environ['GST_PLUGIN_SYSTEM_PATH'] = ''

    addpath = jhbuildrc_globals['addpath']

    prefix = jhbuildrc_globals['prefix']
    addpath('CMAKE_PREFIX_PATH', prefix)
    addpath('CMAKE_LIBRARY_PATH', os.path.join(prefix, 'lib'))

    if 'JHBUILD_MIRROR' in os.environ:
        jhbuildrc_globals['dvcs_mirror_dir'] = os.environ['JHBUILD_MIRROR']
        jhbuildrc_globals['tarballdir'] = os.environ['JHBUILD_MIRROR']

    if 'x86_64' in platform.machine():
        jhbuildrc_globals['conditions'].add('x86_64')
