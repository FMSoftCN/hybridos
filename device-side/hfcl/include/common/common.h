/* 
** HFCL - HybridOS Foundation Class Library
** 
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HFCL_COMMON_COMMON_H_
#define HFCL_COMMON_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#include <minigui/common.h>

#ifndef HFCL_MAJOR_VERSION
#   undef PACKAGE
#   undef VERSION
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_NAME
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   ifdef __HFCL_LIB__
#       include "../../hfclconfig.h"
#   else
#       include "../hfclconfig.h"
#   endif
#endif

#include "common/checkstltemplate.h"

#define TEXT_BUFFER_LEN_OF_DEFAULT 63

/* HFCL types */
typedef intptr_t    HTHandle;
typedef intptr_t    HTResId; 
typedef intptr_t    HTData; typedef float       HTReal;

#ifdef _HFCL_ASSERT
#    define HFCL_ASSERT(n_assert)       \
    if (!(n_assert)) {                  \
        DBG_ASSERT(0, "NGUX Assert in %s (Line: %d)", __FILE__, __LINE__); \
    }
#else
#    define HFCL_ASSERT(n_assert)
#endif

//safe cast
//#define _HFCL_SAFE_CAST // add the compile option to control it
#ifdef _HFCL_SAFE_CAST
#define SAFE_CAST(DestType, srcobj) dynamic_cast<DestType>(srcobj)
static void * _assert_safe_cast(void *obj, const char* file, int line, const char* assert_info) {
    if(!obj)
        _DBG_PRINTF("NGUX SAFE CAST  ASSERT>> %s[%d]: %s\n", file, line, assert_info);
    return obj;
}
#define ASSERT_SAFE_CAST(DestType, srcobj)  \
    ((DestType)_assert_safe_cast((void*)SAFE_CAST(DestType, srcobj), __FILE__, __LINE__, "Cast " #srcobj " To " #DestType)
#else
#define SAFE_CAST(DestType, srcobj)  (DestType)(srcobj)
#define ASSERT_SAFE_CAST(DestType, srcobj)  SAFE_CAST(DestType, srcobj)
#endif

#endif // HFCL_COMMON_COMMON_H_

