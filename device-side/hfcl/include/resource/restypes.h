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

#ifndef HFCL_RESOURCE_RESTYPE_H_
#define HFCL_RESOURCE_RESTYPE_H_

#include "common/common.h"

typedef enum {
    R_TYPE_TEXT_RAW,
    R_TYPE_TEXT_ZIPPED,
    R_TYPE_TEXT_GNUMSG,
    R_TYPE_IMAGE,
    R_TYPE_FONT,
    R_TYPE_STYLE,
    R_TYPE_UI,
    R_TYPE_DRAWABLE,
    R_TYPE_DRAWABLESET,
    R_TYPE_DRSETGROUP,
    R_TYPE_THEME,
    R_TYPE_MENU,
    R_TYPE_MEDIA,
    R_TYPE_ANIMATION,
    R_TYPE_BINARY,
    NR_RES_TYPE,
} HIDResType;

#define DR_DRSET_DRSETGROUP_ARRAY_SIZE 10

#define PKG_ID_MASK     0xFFF00000
#define RES_TYPE_MASK   0x000F0000
#define RES_IDX_MASK    0x0000FFFF

#define PKG_ID_SHIFT    20
#define RES_TYPE_SHIFT  16
#define RES_IDX_SHIFT   0

#define MAKE_RES_ID(pkgID,type,idx) ((pkgID << PKG_ID_SHIFT) & PKG_ID_MASK) \
                                 | ((type << RES_TYPE_SHIFT) & RES_TYPE_MASK) \
                                 | ((idx << RES_IDX_SHIFT) & RES_IDX_MASK)

#define RPKGID(resID)   ((resID & PKG_ID_MASK) >> PKG_ID_SHIFT)
#define RESTYPE(resID)  ((resID & RES_TYPE_MASK) >> RES_TYPE_SHIFT)
#define RESINDEX(resID) ((resID & RES_IDX_MASK) >> RES_IDX_SHIFT)

typedef enum {
    R_LANG_na_NA,
    R_LANG_zh_CN,
    R_LANG_zh_TW,
    R_LANG_zh_HK,
    R_LANG_en_HK,
    R_LANG_en_US,
    R_LANG_en_GB,
    R_LANG_en_WW,
    R_LANG_en_CA,
    R_LANG_en_AU,
    R_LANG_en_IE,
    R_LANG_en_FI,
    R_LANG_en_DK,
    R_LANG_en_IL,
    R_LANG_en_ZA,
    R_LANG_en_IN,
    R_LANG_en_NO,
    R_LANG_en_SG,
    R_LANG_en_NZ,
    R_LANG_en_ID,
    R_LANG_en_PH,
    R_LANG_en_TH,
    R_LANG_en_MY,
    R_LANG_en_XA,
    R_LANG_ko_KR,
    R_LANG_ja_JP,
    R_LANG_nl_NL,
    R_LANG_nl_BE,
    R_LANG_pt_PT,
    R_LANG_pt_BR,
    R_LANG_fr_FR,
    R_LANG_fr_LU,
    R_LANG_fr_CH,
    R_LANG_fr_BE,
    R_LANG_fr_CA,
    R_LANG_es_LA,
    R_LANG_es_ES,
    R_LANG_es_AR,
    R_LANG_es_US,
    R_LANG_es_MX,
    R_LANG_es_CO,
    R_LANG_es_PR,
    R_LANG_es_CL,
    R_LANG_de_DE,
    R_LANG_de_AT,
    R_LANG_de_CH,
    R_LANG_ru_RU,
    R_LANG_it_IT,
    R_LANG_el_GR,
    R_LANG_no_NO,
    R_LANG_fi_FI,
    R_LANG_da_DK,
    R_LANG_he_IL,
    R_LANG_hu_HU,
    R_LANG_tr_TR,
    R_LANG_cs_CZ,
    R_LANG_sl_SL,
    R_LANG_pl_PL,
    R_LANG_sv_SE,
} HIDLanguage;

typedef enum {
    R_ENCODING_unknown,     // Unknown charset (default)
    R_ENCODING_utf8,        // UTF-8 charset (default)
    R_ENCODING_ascii,       // ASCII charset
    R_ENCODING_latin1,      // West European (ISO-8859-1, Latin 1) charset
    R_ENCODING_latin2,      // East European (ISO-8859-2, Latin 2) charset
    R_ENCODING_latin3,      // South European (ISO-8859-3, Latin 3) charset
    R_ENCODING_latin4,      // North European (ISO-8859-4, Latin 4) charset
    R_ENCODING_cyrillic,    // Cyrillic (ISO-8859-5) charset
    R_ENCODING_araic,       // Arabic (ISO-8859-6) charset
    R_ENCODING_greek,       // Greek (ISO-8859-7) charset
    R_ENCODING_hebrew,      // Hebrew (ISO-8859-8) charset
    R_ENCODING_latin5,      // Turkish (ISO-8859-9, Latin 5) charset
    R_ENCODING_latin6,      // Nordic (ISO-8859-10, Latin 6) charset
    R_ENCODING_thai,        // Thai (ISO-8859-11) charset
    R_ENCODING_latin7,      // ISO-8859-13 (Latin 7) charset
    R_ENCODING_latin8,      // ISO-8859-14 (Latin 8) charset
    R_ENCODING_latin9,      // ISO-8859-15 (Latin 9) charset
    R_ENCODING_latin10,     // ISO-8859-16 (Latin 10) charset
    R_ENCODING_big5,        // Big5 charset
    R_ENCODING_gb2312,      // GB2312 charset
    R_ENCODING_gbk,         // GBK charset
    R_ENCODING_gb18030,     // GB18030 charset
    R_ENCODING_eucjp,       // EUCJP charset
    R_ENCODING_euckr,       // EUCKR charset
    R_ENCODING_shiftjis,    // SHIFTJIS charset
    R_ENCODING_utf16le,     // UTF-16LE charset
    R_ENCODING_utf16be,     // UTF-16BE charset
} HIDEncoding;

typedef struct _HFCL_INCORE_RES {
    HIDResType type;
    const char* res_name;
    unsigned char* data;
    unsigned int size;
} HFCL_INCORE_RES;

typedef struct _HFCL_ZIPPED_STRINGS {
    Uint32  lang_id;
    Uint32  enc_id;
    Uint32  zipped_size;
    Uint32  origin_size;
    Uint32  nr_strings;
    Uint32  reserved;
    char    zipped_bytes [0];
} __attribute__((__packed__)) HFCL_ZIPPED_STRINGS;

#ifdef __cplusplus

namespace hfcl {

typedef struct INNER_RES_INFO {
    const char* res_name;
    unsigned char* data;
    unsigned int size;
} INNER_RES_INFO;

typedef struct INNER_RES_ARRAY {
    HIDResType type;
    INNER_RES_INFO *pInner_res_info;
    unsigned int number;
} INNER_RES_ARRAY;

typedef struct MENU_RES_ARRAY
{
    unsigned int MenuID;
    void * Menufunction;
}MENU_RES_ARRAY;

typedef struct UI_RES_ARRAY
{
    unsigned int UiID;
    void * UiFunction;
}UI_RES_ARRAY;

///// BMPFont information
typedef struct TRBMPDevFontItemInfo {
    const char *      start_mchar;
    unsigned int       num_char;
    unsigned int       glyph_width;
    HTResId        bmp_id;
}TRBMPDevFontItemInfo;

typedef struct TRStyleElement {
    int id;
    DWORD value;
}TRStyleElement;

typedef struct TRDrawableArrayItem {
    int id;
    const char* base_name;
    int super_style_id;
    TRStyleElement style_element[DR_DRSET_DRSETGROUP_ARRAY_SIZE];
}TRDrawableArrayItem;

//DrawableSet Data
typedef struct TRDrawableSetItem {
    int  dr_id;
    int  drres_id;
}TRDrawableSetItem;

//DrawableSet Data
typedef struct TRDrawableSetArrayItem {
    int    drset_id;
    int    super_drset_id;
    TRDrawableSetItem drawables[DR_DRSET_DRSETGROUP_ARRAY_SIZE];
}TRDrawableSetArrayItem;

//Theme Data
typedef struct TRThemeItem {
    int   theme_drset_id;
    int   drset_id;
}TRThemeItem;

//DrawableSet Group
typedef struct TRDrawableSetGroupItem {
    int    drset_id;
    int    drsetres_id;
}TRDrawableSetGroupItem;

typedef struct TRDrawableSetGroupArrayItem {
    int    drsetgroup_id;
    HTResId    drsetgroup_super_id;
    TRDrawableSetGroupItem drawablesets[DR_DRSET_DRSETGROUP_ARRAY_SIZE];
}TRDrawableSetGroupArrayItem;

} // namespace hfcl

#endif /* __cplusplus */
#endif /* HFCL_RESOURCE_RESTYPE_H_ */

