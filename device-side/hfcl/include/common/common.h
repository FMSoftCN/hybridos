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
#include <assert.h>
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

// TUNNING CONDITION: define for assert safe cast
#undef _HFCL_SAFE_CAST

// TUNNING CONDITION: default text buffer length for text view
#define TEXT_BUFFER_LEN_OF_DEFAULT  63

// FIXME: this will be removed
#define LAYER_MAXNUM                2

/* HFCL types */
typedef unsigned int    HTResId; 
typedef float           HTReal;
typedef intptr_t        HTHandle;
typedef intptr_t        HTData;

#undef _HFCL_SHOW_OPTIMIZE_INFO

/*
 * We use the following macros for new/delete/malloc/calloc/realloc/free.
 * You can easily re-define them to optimize the memory usage of HFCL.
 */
#define HFCL_NEW(classname)             new classname
#define HFCL_NEW_EX(classname, param)   new classname param
#define HFCL_DELETE(addr)               delete (addr)

#define HFCL_NEW_ARR(classname, param)  new classname[param]
#define HFCL_DELETE_ARR(addr)           delete[] (addr)

#define HFCL_MALLOC(size)               malloc(size)
#define HFCL_CALLOC(size, count)        calloc(size, count)
#define HFCL_REALLOC(buf, size)         realloc(buf, size)
#define HFCL_FREE(paddr)                free(paddr)

#ifdef _HFCL_SHOW_OPTIMIZE_INFO
#define OPT_TRACE_PARAM   ,_trace_info_
#define OPT_TRACE_INFO   ,const char* _trace_info_
#define OPTIMIZE_WARNING  _DBG_PRINTF ("------ Object is copyed in %s, Please optimize it\n", _trace_info_)
#define OPT_CLASSNAME(clss)  ,#clss
#else
#define OPT_TRACE_INFO
#define OPT_TRACE_PARAM
#define OPTIMIZE_WARNING
#define OPT_CLASSNAME(clss)
#endif

#define HFCL_ASSERT(expression)    do {                         \
    if (!(expression)) {                                        \
        _DBG_PRINTF (0, "HFCL assert failed in %s (Line: %d)",  \
                __FILE__, __LINE__);                            \
        assert (expression);                                    \
    }                                                           \
} while (0)

#ifdef _HFCL_SAFE_CAST

#define SAFE_CAST(DestType, srcobj) dynamic_cast<DestType>(srcobj)

inline static void * _assert_safe_cast (void *obj, const char* file, int line,
                                 const char* assert_info)
{
    if (!obj) {
        _DBG_PRINTF ("HFCL SAFE_CAST assert faield >> %s[%d]: %s\n",
                    file, line, assert_info);
    }

    return obj;
}

#define ASSERT_SAFE_CAST(DestType, srcobj)                              \
    ((DestType)_assert_safe_cast ((void*)SAFE_CAST (DestType, srcobj),  \
            __FILE__, __LINE__, "Cast " #srcobj " To " #DestType)

#else /* _HFCL_SAFE_CAST */

#define SAFE_CAST(DestType, srcobj)         (DestType)(srcobj)
#define ASSERT_SAFE_CAST(DestType, srcobj)  SAFE_CAST(DestType, srcobj)

#endif /* !_HFCL_SAFE_CAST */

#endif // HFCL_COMMON_COMMON_H_

