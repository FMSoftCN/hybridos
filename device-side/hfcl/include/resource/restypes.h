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
#ifndef HFCL_RESTYPE_H
#define HFCL_RESTYPE_H

#include "nguxcommon.h"

enum enumResType {
	R_TYPE_VOID = 0,
	R_TYPE_STRING,
	R_TYPE_TEXT,
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
	NR_RES_TYPE_MAX,
};

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

enum ResLang {
	R_LANG_zh_cn,
	R_LANG_zh_tw,
	R_LANG_zh_hk,
	R_LANG_en_hk,
	R_LANG_en_us,
	R_LANG_en_gb,
	R_LANG_en_ww,
	R_LANG_en_ca,
	R_LANG_en_au,
	R_LANG_en_ie,
	R_LANG_en_fi,
	R_LANG_en_dk,
	R_LANG_en_il,
	R_LANG_en_za,
	R_LANG_en_in,
	R_LANG_en_no,
	R_LANG_en_sg,
	R_LANG_en_nz,
	R_LANG_en_id,
	R_LANG_en_ph,
	R_LANG_en_th,
	R_LANG_en_my,
	R_LANG_en_xa,
	R_LANG_ko_kr,
	R_LANG_ja_jp,
	R_LANG_nl_nl,
	R_LANG_nl_be,
	R_LANG_pt_pt,
	R_LANG_pt_br,
	R_LANG_fr_fr,
	R_LANG_fr_lu,
	R_LANG_fr_ch,
	R_LANG_fr_be,
	R_LANG_fr_ca,
	R_LANG_es_la,
	R_LANG_es_es,
	R_LANG_es_ar,
	R_LANG_es_us,
	R_LANG_es_mx,
	R_LANG_es_co,
	R_LANG_es_pr,
	R_LANG_es_cl,
	R_LANG_de_de,
	R_LANG_de_at,
	R_LANG_de_ch,
	R_LANG_ru_ru,
	R_LANG_it_it,
	R_LANG_el_gr,
	R_LANG_no_no,
	R_LANG_fi_fi,
	R_LANG_da_dk,
	R_LANG_he_il,
	R_LANG_hu_hu,
	R_LANG_tr_tr,
	R_LANG_cs_cz,
	R_LANG_sl_sl,
	R_LANG_pl_pl,
	R_LANG_sv_se,
};

enum ResEncoding 
{
	R_ENCODING_ascii, 		// Ascii charset
	R_ENCODING_araic, 		// Arabic charset
	R_ENCODING_big5,  		// Big5 charset
	R_ENCODING_cyrillic, 	// Cyrillic charset 
	R_ENCODING_eucjp, 		// EUCJP charset 
	R_ENCODING_euckr, 		// EUCKR charset 
	R_ENCODING_gb2312, 		// GB2312 charset 
	R_ENCODING_gb18030, 	// GB18030 charset 
	R_ENCODING_gbk, 		// GBK charset 
	R_ENCODING_greek, 		// Greek charset 
	R_ENCODING_hebrew, 		// Hebrew charset 
	R_ENCODING_latin10,		// Latin 10 charset 
	R_ENCODING_latin2, 		// Latin 2 charset 
	R_ENCODING_latin3, 		// Latin 3 charset 
	R_ENCODING_latin4, 		// Latin 4 charset 
	R_ENCODING_latin5, 		// Latin 5 charset 
	R_ENCODING_latin6, 		// Latin 6 charset 
	R_ENCODING_latin7, 		// Latin 7 charset 
	R_ENCODING_latin8, 		// Latin 8 charset 
	R_ENCODING_latin9, 		// Latin 9 charset 
	R_ENCODING_shiftjis, 	// SHIFTJIS charset 
	R_ENCODING_thai, 		// Thai charset 
	R_ENCODING_utf8, 		// UTF-8 charset
	R_ENCODING_utf16le, 	// UTF-16LE charset
	R_ENCODING_utf16be, 	// UTF-16BE charset
};

#ifdef __cplusplus
namespace hfcl {
/////////////////////////////////////////////////
//the inner resource support
typedef struct INNER_RES_INFO {
	const char* res_name;
	unsigned char* data;
	unsigned int size;
}INNER_RES_INFO;

typedef struct INNER_RES_ARRAY
{
    enumResType type;
    INNER_RES_INFO *pInner_res_info;
    unsigned int  number;
}INNER_RES_ARRAY;

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




} // namespace hfcl {

#endif
#endif
