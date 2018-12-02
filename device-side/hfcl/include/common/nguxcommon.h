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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _NGUX_COMM_H
#define _NGUX_COMM_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#if defined(__TARGET_SIMULATOR__)
#include <stdarg.h>
#endif

#include "nguxconfig.h"
#include "common.h"
#include "fixedmath.h"

#include "checkstltemplate.h"
#include "mmi_profile_codes.h"
#include "hal_debug.h"
#include "dbg.h"

typedef char* PS8;
typedef char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;

typedef signed int S32;
typedef unsigned int U32;

typedef char INT8;
typedef short INT16;
typedef int INT32;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

typedef unsigned short pBOOL;

#ifndef VOID
typedef void            VOID;
#endif

typedef void *          HANDLE;

#ifdef USE_UCS2_ENCODING
typedef unsigned short ui_char_t;
#else
typedef unsigned char ui_char_t;
#endif

#define UI_TEST_8895_1_CHAR_IN_GSM_DEF_CHAR(c)  \
    (((c)==128 ||(c)==162 ||(c)==163 ||(c)==165  \
    ||(c)==232 ||(c)==233 ||(c)==238 ||(c)==249  \
    ||(c)==236 ||(c)==242 ||(c)==199 ||(c)==216 \
    ||(c)==248 ||(c)==197 ||(c)==229 ||(c)==198 \
    ||(c)==230 ||(c)==223 ||(c)==200 ||(c)==164 \
    ||(c)==161 ||(c)==196 ||(c)==214 ||(c)==209 \
    ||(c)==220 ||(c)==167 ||(c)==191 ||(c)==228 \
    ||(c)==246 ||(c)==241 ||(c)==252 ||(c)==224)?(1):(0))


#define UI_TEST_UCS2_CHARACTER(c)            (((c)&0xff80) && ( !UI_TEST_8895_1_CHAR_IN_GSM_DEF_CHAR(c)))?(1):(0)

#define UI_STRING_END_OF_STRING_CHARACTER(c) (((unsigned short)(c)==(unsigned short)'\0')?(1):(0))

#define UI_TEST_ESC_CHARACTER(c)          (((ui_char_t)(c)==(ui_char_t)0x1B)?(1):(0))      

#define UI_TEST_GSM_EXTENDED(c)     \
(     (c==0xA2)                     \
   || (c=='^')                      \
   || (c=='{')                      \
   || (c=='}')                      \
   || (c=='\\')                     \
   || (c=='[')                      \
   || (c=='~')                      \
   || (c==']')                      \
   || (c=='|')                      \
)                                   \

#ifdef USE_UCS2_ENCODING

/* Warning: Currently, these macros assume Little endian format only */

#define UI_STRING_GET_NEXT_CHARACTER(p,c)                             \
      {                                                               \
         c=(ui_char_t)((*((p)+0))|((*((p)+1))<<8));                   \
         (p)+=2;                                                      \
      }

#define UI_STRING_GET_PREVIOUS_CHARACTER(p,c)                         \
      {                                                               \
         p-=2;                                                        \
         c=(ui_char_t)((*((p)+0))|((*((p)+1))<<8));                   \
      }

#define UI_STRING_INSERT_CHARACTER(p,c)                               \
      {                                                               \
         (*((p))++)=(unsigned char)(((c)&0xff));                      \
         (*((p))++)=(unsigned char)(((c)>>8));                        \
      }
#else

#define UI_STRING_GET_NEXT_CHARACTER(p,c)                             \
      {                                                               \
         (c)=*((unsigned char*)(p))++;                                \
      }

#define UI_STRING_GET_PREVIOUS_CHARACTER(p,c)                         \
      {                                                               \
         (c)=*(--((unsigned char*)(p)));                              \
      }

#define UI_STRING_INSERT_CHARACTER(p,c)                               \
      {                                                               \
         *((unsigned char*)(p))++=(c);                                \
      }

#endif /* USE_UCS2_ENCODING */ 

/*
typedef struct stFontAttribute
{
    U8 bold;
    U8 italic;
    U8 underline;
    U8 size;
    U8 color_t;
    U8 type;
    U8 oblique;
    U8 smallCaps;
} stFontAttribute;
*/



#ifdef LCDSIZE_320_240
#define LCD_WIDTH     320
#define LCD_HEIGHT    240                                          
#endif  

#ifdef    LCDSIZE_240_400
#define LCD_WIDTH    240
#define LCD_HEIGHT    400
#endif

#ifdef    LCDSIZE_240_320
#define LCD_WIDTH    240
#define LCD_HEIGHT    320   
#endif

#ifdef    LCDSIZE_176_220
#define LCD_WIDTH    176
#define LCD_HEIGHT    220   
#endif

#ifdef    LCDSIZE_220_176
#define LCD_WIDTH    220
#define LCD_HEIGHT    176   
#endif

#ifdef    LCDSIZE_160_128
#define LCD_WIDTH    160
#define LCD_HEIGHT    128   
#endif

#ifdef    LCDSIZE_128_160
#define LCD_WIDTH    128
#define LCD_HEIGHT    160   
#endif
     
#ifdef    LCDSIZE_128_128
#define LCD_WIDTH    128
#define LCD_HEIGHT    128 
#endif
 
#define SUBLCD_WIDTH          96
#define SUBLCD_HEIGHT         64

#define UI_DEVICE_WIDTH       LCD_WIDTH
#define UI_DEVICE_HEIGHT      LCD_HEIGHT

#define LAYER_MAXNUM 2

/* VincentWei: bad code
#ifdef __TARGET_SIMULATOR__
#define snprintf    _snprintf
#endif
*/

#ifdef _NGUX_NAME_SPACE
#define NAMESPACE_BEGIN \
    namespace NGUX {   
#define NAMESPACE_END  }
#define NGUX_BEGIN_EXTERN_C   extern "C" {
#define NGUX_END_EXTERN_C   }
#define USE_NGUX_NAMESPACE using namespace NGUX;
#else
#define NAMESPACE_BEGIN
#define NAMESPACE_END
#define USE_NGUX_NAMESPACE
#endif

#define NGUX_TIMER_BASEID   (KEY_TIMER_ID0)

#ifdef _NGUX_ASSERT
#    define NGUX_ASSERT(n_assert)       \
    if (!(n_assert)) {                  \
        DBG_ASSERT(0, "NGUX Assert in File (%s) at Line (%d)", __FILE__, __LINE__); \
    }
#else
#    define NGUX_ASSERT(n_assert)
#endif

//safe cast
//#define _NGUX_SAFE_CAST // add the compile option to control it
#ifdef _NGUX_SAFE_CAST
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

#ifdef WIN32
#define __align(n) 
#else
#define __align(n)  __attribute__((aligned(n)))
#endif

#define _ngux_screen_w                          LCD_WIDTH
#define _ngux_screen_h                          LCD_HEIGHT

#ifdef LCDSIZE_240_320
  #define _ngux_screen_margin                   5
  #define _ngux_content_margin_top              4
  #define _ngux_phonebar_h                      30
  #define _ngux_menubar_h                       30
  #define _ngux_title_h                         0
  #define _ngux_content_w                       (_ngux_screen_w - _ngux_screen_margin * 2)
  #define _ngux_content_h                       (_ngux_screen_h - _ngux_phonebar_h - _ngux_menubar_h)
  #define _ngux_sl_item_h                       40
  #define _ngux_sl_item_padding_v               6
  #define _ngux_sl_item_content_h               28
  #define _ngux_ml_item_h                       64
  #define _ngux_ml_item_padding_v               4
  #define _ngux_ml_item_half                    28
  #define _ngux_ml_item_secondary               26
  #define _ngux_ml_item_spacing_v               2
  #define _ngux_select_list_item_h              34
  #define _ngux_select_list_item_padding_v      2
  #define _ngux_select_list_item_content_h      28
  #define _ngux_list_icon_size                  30
  #define _ngux_menuitem_h                      40

  #define _ngux_sl_item_h1                      40
  #define _ngux_sl_item_h2                      20
  #define _ngux_margin_w                        2
  #define _ngux_margin_h                        2
  #define _ngux_icon_img_w                      15
  #define _ngux_icon_img_h                      15
  #define _ngux_title_icon_w                    20
  #define _ngux_arrow_image_w                   15
#elif defined (LCDSIZE_128_160)
  #define _ngux_screen_margin                   2
  #define _ngux_phonebar_h                      13
  #define _ngux_menubar_h                       16
  #define _ngux_title_h                         17
  #define _ngux_menuitem_h                      17
  #define _ngux_sl_item_h1                      32
  #define _ngux_sl_item_h2                      19
  #define _ngux_ml_item_h                       38
  #define _ngux_ml_item_half                    19
  #define _ngux_content_w                       (_ngux_screen_w - _ngux_screen_margin * 2)
  #define _ngux_content_h                       (_ngux_screen_h - _ngux_phonebar_h - _ngux_menubar_h)
  #define _ngux_margin_w                        2
  #define _ngux_margin_h                        2
  #define _ngux_icon_img_w                      15
  #define _ngux_icon_img_h                      15
  #define _ngux_title_icon_w                    20
#elif defined (LCDSIZE_176_220)
  #define _ngux_screen_margin                   2
  #define _ngux_phonebar_h                      16
  #define _ngux_menubar_h                       20
  #define _ngux_title_h                         20
  #define _ngux_menuitem_h                      20
  #define _ngux_sl_item_h1                      30
  #define _ngux_sl_item_h2                      20
  #define _ngux_ml_item_h                       40
  #define _ngux_ml_item_half                    20
  #define _ngux_content_w                       (_ngux_screen_w - _ngux_screen_margin * 2)
  #define _ngux_content_h                       (_ngux_screen_h - _ngux_phonebar_h - _ngux_menubar_h)
  #define _ngux_margin_w                        2
  #define _ngux_margin_h                        2
  #define _ngux_icon_img_w                      15
  #define _ngux_icon_img_h                      15
  #define _ngux_title_icon_w                    20
#elif defined (LCDSIZE_128_128)
  #define _ngux_screen_margin                   2
  #define _ngux_phonebar_h                      13
  #define _ngux_menubar_h                       16
  #define _ngux_title_h                         17
  #define _ngux_menuitem_h                      18
#ifndef GT1200_CIS  // GT_choph 130518  For GT1200_MEA
  #define _ngux_sl_item_h1                      32
#else
  #define _ngux_sl_item_h1                      19
#endif 
  #define _ngux_sl_item_h2                      19
  #define _ngux_ml_item_h                       38
  #define _ngux_ml_item_half                    19
  #define _ngux_arrow_image_w                   15
  #define _ngux_content_w                       (_ngux_screen_w - _ngux_screen_margin * 2)
  #define _ngux_content_h                       (_ngux_screen_h - _ngux_menubar_h)
  #define _ngux_margin_w                        2
  #define _ngux_margin_h                        2
  #define _ngux_icon_img_w                      15
  #define _ngux_icon_img_h                      15
  #define _ngux_title_icon_w                    20
  #define _ngux_scroll_w                        6
#else
  #define _ngux_screen_margin                   2
  #define _ngux_phonebar_h                      13
  #define _ngux_menubar_h                       16
  #define _ngux_title_h                         17
  #define _ngux_menuitem_h                      17
  #define _ngux_sl_item_h1                      32
  #define _ngux_sl_item_h2                      19
  #define _ngux_ml_item_h                       38
  #define _ngux_ml_item_half                    19
  #define _ngux_content_w                       (_ngux_screen_w - _ngux_screen_margin * 2)
  #define _ngux_content_h                       (_ngux_screen_h - _ngux_phonebar_h - _ngux_menubar_h)
  #define _ngux_margin_w                        2
  #define _ngux_margin_h                        2
  #define _ngux_icon_img_w                      15
  #define _ngux_icon_img_h                      15
  #define _ngux_title_icon_w                    20
  #define _ngux_scroll_w                        6
#endif

#define _ngux_menu_h                            (_ngux_screen_h / 2)
#define _ngux_menu_h_max                        (_ngux_menubar_h + ( _ngux_menuitem_h * 4))

#define IME_TITLE_LEFT                          30
#define IME_TITLE_RIGHT                         (_ngux_screen_w - 30)

#define LIST_ITEM_GAP                           2

#define VSCROLL_WIDTH                           3
#define INPUTBOX_WIDTH                          6

static inline int ngux_atomic_inc(int* addr)
{
    int value = *addr;
    *addr = value + 1;
    return value;
}

static inline int ngux_atomic_dec(int* addr)
{
    int value = *addr;
    *addr = value - 1;
    return value;
}

typedef unsigned int ResID; 

//////////////////////////////////////////
typedef char* NGPStr;
typedef const char* NGCPStr;
typedef int NGInt;
typedef signed int NGSInt;
typedef unsigned int NGUInt;
typedef long NGLong;
typedef unsigned long NGULong;
typedef unsigned long long NGULongLong;
typedef char NGChar;
typedef unsigned char NGByte;
typedef int NGInt32;
typedef unsigned int NGUInt32;
typedef unsigned long long  NGUInt64;
typedef long long NGInt64;
typedef short NGInt16;
typedef unsigned short NGUInt16;
typedef char NGInt8;
typedef unsigned char NGUInt8;
typedef float NGReal;
typedef unsigned int NGSize;  // typedef size_t NGSize_t;
typedef size_t NGSize_t;
typedef int  NGOff;
#ifdef __cplusplus
typedef bool NGBool;
#else
typedef int NGBool;
#endif
typedef void* NGPointer;
typedef DWORD NGDword;
typedef long  NGFsHandle;

////////////////////////////////////////////
//define the common type

#define NGUX_CFG   "ngux.cfg"
#define NGUX_CFG_ENV "NGUX_CFG"

/////////// ngux self defined MSG ////////////
#define NGUX_MSG_BASE (MSG_USER)
#define NGUX_MSG_CHANGE_THEME (NGUX_MSG_BASE + 1)
#define NGUX_MSG_ADP_EVENT (NGUX_MSG_BASE + 2)

#define NGUX_MSG_SEND_NETWORK_EVENT (NGUX_MSG_BASE + 3)

/////////// global defines of framework /////////
#define TEXT_BUFFER_LEN_OF_DEFAULT 63

#endif //_NGUX_COMM_H

