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

#ifndef HFCL_DRAWABLE_ID_H
#define HFCL_DRAWABLE_ID_H

#include "common/common.h"

#define MAKE_ELEMENT_ID(m, t, id) \
	(((m) << 24) | ((t)<<16) | ((id) & 0xFFFF))

#define GET_ELEMENT_TYPE(id) (((id)>>16)&0xFF)

#define SYS_ELEMENT 0

#define   DRET_METRICS   0  //Drawable Eelment Type
#define   DRET_COLOR  1
#define   DRET_IMAGE    0x81
#define   DRET_FONT     0x82
#define   DRET_DRAWABLE 0x83
#define   DRET_FONTLIST 0x84
#define   DRET_COLORLIST 0x85
#define   DRET_IMGLIST 0x86
#define   DRET_DRAWABLELIST 0x87
#define   DRET_BITMAP 0x88


#define MAKE_DR_ID(mod, id)  (((mod)<<24) | ((id)&0xFFFFFF))

#define STYLE_ELEMENT_SECTION           "style-elements"
#define COMMON_DRAWABLESET_SECTION      "common-drawableset"
#define VIEW_DRAWBABLESETS_SECTION      "view-drawablesets"

namespace hfcl {

//define the drawable id
#define SYS 0

//define the draw id
#define DRAW_STATE_SHIFT  16
enum {
	DRAW_CONTENT = 0,
	DRAW_BKGROUND,
	DRAW_SELECTION, //draw the selection
	DRAW_SCROLLBAR,
	DRAW_HSCROLLBAR = DRAW_SCROLLBAR,
	DRAW_VSCROLLBAR,
	DRAW_SEPARATOR,
};

typedef enum {
	//the state of selection
	DRAWSTATE_NORMAL = 0,
	DRAWSTATE_SELECTED = 1 <<DRAW_STATE_SHIFT,
	DRAWSTATE_HILIGHT = 2 <<DRAW_STATE_SHIFT,
	DRAWSTATE_PUSHED = 3 <<DRAW_STATE_SHIFT,
	DRAWSTATE_FOCUSED = 4 <<DRAW_STATE_SHIFT,
	DRAWSTATE_DISABLED = 5 <<DRAW_STATE_SHIFT,
	DRAWSTATE_HILIGHT_SEL = 6 <<DRAW_STATE_SHIFT,
}DrawableStatus;

#define GET_DRAW_STATE(draw_id)  ((draw_id) & 0xFF0000)
#define GET_DRAW_CONTENT(draw_id) ((draw_id) & 0xFFFF)

//define the data type
typedef enum {
	DRDT_NONE = 0, // no data
	DRDT_TEXT,
	DRDT_FORMATTEXT,
	DRDT_IMAGE,
	DRDT_COLOR,
	DRDT_INT,
	DRDT_VIEW
}DR_DATA_TYPE;

typedef struct {
	const char * text;
	HTData textFormat; //the combine of TextMode, sucha TextMode::AlignRight | TextMode::ValignBottom | TextMode::SingleLine ...
}FormatText; //the type of DRDT_FORMATTEXT

#ifdef HFCL_LOG_NDEBUG
static inline HTData _drawable_check_data_type(HTData data, int except_type_id, int type_id, const char* file, int line) {
	if(except_type_id != type_id) {
		LOGERROR("Drawable Data Check Error (%s:%d): except %d, but get %d\n", file, line, except_type_id, type_id);
		return 0;
	}
	return data;
}
#define DR_ASSERT_GET_DATA(Type,except_type_id, type_id, data) \
	(Type) _drawable_check_data_type(data, except_type_id, type_id, __FILE__, __LINE__)

#else
#define DR_ASSERT_GET_DATA(Type, except_type_id, type_id, data) (Type)(data) 
#endif
	
#define DR_ASSERT_GET_TEXT(type_id, data)    DR_ASSERT_GET_DATA(const char*, DRDT_TEXT, type_id, data)
#define DR_ASSERT_GET_IMAGE(type_id, data)   DR_ASSERT_GET_DATA(Image*, DRDT_IMAGE, type_id, data)
#define DR_ASSERT_GET_COLOR(type_id, data)   DR_ASSERT_GET_DATA(HTData, DRDT_COLOR, type_id, data)
#define DR_ASSERT_GET_INT(type_id, data)     DR_ASSERT_GET_DATA(int, DRDT_INT, type_id, data)
#define DR_ASSERT_GET_VIEW(ViewType, type_id, data)    ASSERT_SAFE_CAST(ViewType*, DR_ASSERT_GET_DATA(View*, DRDT_VIEW, type_id, data))


//The Metrics info
enum {
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT,

	VALIGN_TOP = 0,
	VALIGN_MIDDLE,
	VALIGN_BOTTOM,

	TEXTBREAK_SINGLE = 0,
	TEXTBREAK_WORD,
	TEXTBREAK_CHAR,

	TEXTOUT_NORMAL = 0,
	TEXTOUT_OMITTED,

	DRAWMODE_NORMAL = 0,
	DRAWMODE_SCALED,
	DRAWMODE_TILED,
	DRAWMODE_ADAPTED,
	DRAWMODE_PARTBOX,
	DRAWMODE_ROTATIONANGLE,

	UNCHECKED = 0,
	CHECKED,
};

//// the imag format 
typedef struct {
	unsigned int  drawMode:8;
	unsigned int  align:8;
	unsigned int  valign:8;
	unsigned int  rotationAngle;
}ImageFormat;


#define HFCL_DEFAULT_OPACITY         255
#define HFCL_HALF_OPACITY            128
#define HFCL_ZERO_OPACITY            0

//define the default font id
enum {
	SYSFONT_DEFAULT = 0,
	SYSFONT_WCHAR,
	SYSFONT_FIXED,
	SYSFONT_CAPTION,
	SYSFONT_MENU,
	SYSFONT_CONTROL
};


} // namespace hfcl

#endif

