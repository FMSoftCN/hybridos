# -----------------------------------------------------------------------------
# This file is included individually from various subdirectories (WTF,
# WebCore, HFCL) in order to allow scripts to build only part of HFCL.
# We want to run this file only once.
# -----------------------------------------------------------------------------
if (NOT HAS_RUN_HFCL_COMMON)
    set(HAS_RUN_HFCL_COMMON TRUE)

    # -----------------------------------------------------------------------------
    # Find common packages (used by all ports)
    # -----------------------------------------------------------------------------
    if (WIN32)
        list(APPEND CMAKE_PROGRAM_PATH $ENV{SystemDrive}/cygwin/bin)
    endif ()

    # TODO Enforce version requirement for perl
    find_package(Perl 5.10.0 REQUIRED)
#   find_package(PerlModules COMPONENTS JSON::PP REQUIRED)

    set(Python_ADDITIONAL_VERSIONS 3)
    find_package(PythonInterp 2.7.0 REQUIRED)

    # We cannot check for RUBY_FOUND because it is set only when the full package is installed and
    # the only thing we need is the interpreter. Unlike Python, cmake does not provide a macro
    # for finding only the Ruby interpreter.
#    find_package(Ruby 1.9)
#    if (NOT RUBY_EXECUTABLE OR RUBY_VERSION VERSION_LESS 1.9)
#        message(FATAL_ERROR "Ruby 1.9 or higher is required.")
#    endif ()

    # -----------------------------------------------------------------------------
    # Helper macros and feature defines
    # -----------------------------------------------------------------------------

    # To prevent multiple inclusion, most modules should be included once here.
    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)
    include(CheckCXXSourceCompiles)
    include(CheckFunctionExists)
    include(CheckIncludeFile)
    include(CheckSymbolExists)
    include(CheckStructHasMember)
    include(CheckTypeSize)
    include(CMakeDependentOption)
    include(CMakeParseArguments)
    include(ProcessorCount)

#    include(HFCLPackaging)
    include(HFCLMacros)
    include(HFCLFS)
#    include(HFCLCCache)
    include(HFCLCompilerFlags)
    include(HFCLFeatures)

    include(OptionsCommon)
    include(Options${PORT})

    # -----------------------------------------------------------------------------
    # Job pool to avoid running too many memory hungry linker processes
    # -----------------------------------------------------------------------------
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
        set_property(GLOBAL PROPERTY JOB_POOLS link_pool_jobs=4)
    else ()
        set_property(GLOBAL PROPERTY JOB_POOLS link_pool_jobs=2)
    endif ()
    set(CMAKE_JOB_POOL_LINK link_pool_jobs)

    # -----------------------------------------------------------------------------
    # config.h
    # -----------------------------------------------------------------------------
    CREATE_CONFIGURATION_HEADER()

    SET_CONFIGURATION_FOR_UNIFIED_SOURCE_LISTS()
endif ()
