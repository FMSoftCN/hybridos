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

#undef BEGIN_STYLE_ELEMENT
#undef END_STYLE_ELEMENT
#undef DEF_METRICS
#undef DEF_FONT
#undef DEF_COLOR
#undef DEF_IMAGE
#undef DEF_DRAWABLE
#undef DEF_FONTLIST
#undef DEF_COLORLIST
#undef DEF_IMGLIST
#undef DEF_DRAWABLELIST
#undef DEF_DEF_STYLE_ELEMENT

#define BEGIN_STYLE_ELEMENT \
	enum { 

#define END_STYLE_ELEMENT };

#define DEF_STYLE_ELEMENT(name, type, id, str_name)  MAKE_ELEMENT_NAME(name) = MAKE_ELEMENT_ID(MODULE_ID, type, id),

#define DEF_FONT(name, id, str_name)            DEF_STYLE_ELEMENT(name, DRET_FONT, id, str_name)
#define DEF_COLOR(name, id, str_name)           DEF_STYLE_ELEMENT(name, DRET_COLOR, id, str_name)
#define DEF_METRICS(name, id, str_name)         DEF_STYLE_ELEMENT(name, DRET_METRICS, id, str_name)
#define DEF_IMAGE(name, id, str_name)           DEF_STYLE_ELEMENT(name, DRET_IMAGE, id, str_name)
#define DEF_DRAWABLE(name, id, str_name)        DEF_STYLE_ELEMENT(name, DRET_DRAWABLE, id, str_name)
#define DEF_FONTLIST(name, id, str_name)        DEF_STYLE_ELEMENT(name, DRET_FONTLIST, id, str_name)
#define DEF_IMGLIST(name, id, str_name)         DEF_STYLE_ELEMENT(name, DRET_IMGLIST, id, str_name)
#define DEF_DRAWABLELIST(name, id, str_name)    DEF_STYLE_ELEMENT(name, DRET_DRAWABLELIST, id, str_name)
#define DEF_COLORLIST(name, id, str_name)       DEF_STYLE_ELEMENT(name, DRET_COLORLIST, id, str_name)

//////////////////////////////////////////////////
//drawable id define
#undef BEGIN_DRAWABLE_ID
#undef DEF_DRID
#undef END_DRAWABLE_ID

#define BEGIN_DRAWABLE_ID \
	enum { MAKE_DRAWABLE_NAME(__drawable_id_begin) = MAKE_DR_ID(MODULE_ID, 0), 

#define DEF_DRID(name, str_name)  MAKE_DRAWABLE_NAME(name),

#define END_DRAWABLE_ID };

