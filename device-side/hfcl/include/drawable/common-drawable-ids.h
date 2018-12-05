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

#define MODULE_ID   0 
#define MAKE_ELEMENT_NAME(name)   SYS_SE_##name
#define MAKE_DRAWABLE_NAME(name)  DR_##name

BEGIN_STYLE_ELEMENT
	DEF_FONT(FONT, 1, "font")
	DEF_FONT(SELFONT, 2,"selfont")
	DEF_FONT(HILIGHTFONT, 3, "hilightfont")
	DEF_FONT(PUSHEDFONT, 4, "pushedfont")
	DEF_FONT(FOCUSEDFONT, 5, "focusedfont")
	DEF_FONT(DISABLEDFONT, 6, "disabledfont")
	DEF_COLOR(COLOR, 1, "color")
	DEF_COLOR(SELCOLOR, 2, "selcolor")
	DEF_COLOR(HILIGHTCOLOR,3, "hilightcolor")
	DEF_COLOR(PUSHEDCOLOR, 4, "pushedcolor")
	DEF_COLOR(FOCUSEDCOLOR, 5, "focusedcolor")
	DEF_COLOR(DISABLEDCOLOR, 6, "disabledcolor")
	DEF_IMAGE(IMAGE, 			1, "image")
	DEF_IMAGE(SELIMAGE, 		2, "selimage")
	DEF_IMAGE(HILIGHTIMAGE, 	3, "hilightimage")
	DEF_IMAGE(PUSHEDIMAGE, 		4, "pushedimage")
	DEF_IMAGE(FOCUSEDIMAGE, 	5, "focusedimage")
	DEF_IMAGE(DISABLEDIMAGE,	6, "disabledimage")
	DEF_IMAGE(HILIGHTSELIMAGE, 	7, "hilightselimage")
	DEF_METRICS(SEPARATOR_WIDTH , 1, "separator-width")
	DEF_METRICS(TEXTALIGN, 2, "textalign")
	DEF_METRICS(TEXTVALIGN, 3, "textvalign")
	DEF_METRICS(TEXTBREAK,  4, "textbreak")
	DEF_METRICS(TEXTOUTMODE, 5, "textoutmode")
	DEF_METRICS(IMAGEALIGN, 6, "imagealign")
	DEF_METRICS(IMAGEVALIGN, 7, "imagevalign")
	DEF_METRICS(IMAGEDRAWMODE, 8, "imagedrawmode")
	DEF_METRICS(TEXTOUTLINEMODE, 10, "textoutlinemode")
END_STYLE_ELEMENT

#ifndef SYS_GET_STATE_ID
#define SYS_GET_STATE_ID(base, state)   (base + ((state)>>DRAW_STATE_SHIFT))

#define SYS_GET_STATE_IMAGE_ID(state)   SYS_GET_STATE_ID(SYS_SE_IMAGE, state)
#define SYS_GET_STATE_FONT_ID(state)    SYS_GET_STATE_ID(SYS_SE_FONT, state)
#define SYS_GET_STATE_COLOR_ID(state)   SYS_GET_STATE_ID(SYS_SE_COLOR, state)
#endif

BEGIN_DRAWABLE_ID
	DEF_DRID(BKGND, "bkground")
	DEF_DRID(BKCOLOR, "bkcolor")
	DEF_DRID(CONTENT, "content")
	DEF_DRID(CONTENT2, "content2")
	DEF_DRID(CONTENT3, "content3")
	DEF_DRID(CONTENT4, "content4")
	DEF_DRID(HILIGHT, "hilight")
	DEF_DRID(HSCROLLBAR, "hscrollbar")
	DEF_DRID(VSCROLLBAR, "vscrollbar")
	DEF_DRID(BGHSCROLLBAR, "hscrollbarbg")
	DEF_DRID(BGVSCROLLBAR, "vscrollbarbg")
	DEF_DRID(CHECK, "check")
	DEF_DRID(RADIO, "raido")
	DEF_DRID(LEFTARROW, "leftarrow")
	DEF_DRID(RIGHTARROW, "rightarrow")
	DEF_DRID(UPARROW, "uparrow")
	DEF_DRID(DOWNARROW, "downarrow")
	DEF_DRID(SEPARATOR,"separator")
	DEF_DRID(THUMB, "thumb")
END_DRAWABLE_ID

#undef MODULE_ID
#undef MAKE_ELEMENT_NAME
#undef MAKE_DRAWABLE_NAME
