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

#include "graphics/font.h"

namespace hfcl {

#ifdef USE_RDA_FONT

extern "C" stFontAttribute		MMI_DEFAULT_FONT;
extern "C" stFontAttribute		MMI_SMALL_FONT;
extern "C" stFontAttribute		MMI_MEDIUM_FONT;
extern "C" stFontAttribute		MMI_MEDIUM_BOLD_FONT;
extern "C" stFontAttribute      MMI_LARGE_FONT;
extern "C" stFontAttribute      MMI_DIALER_FONT1;
extern "C" stFontAttribute      MMI_DIALER_FONT2;
extern "C" stFontAttribute      MMI_DIALER_FONT3;
extern "C" stFontAttribute		MMI_SUBLCD_FONT;
extern "C" stFontAttribute		MMI_VIRTUAL_KEYBOARD_FONT;

Font* Font::createFont(const char* fontname)
{
    if (fontname == NULL)
        return NULL;

    if (strcmp(fontname, "small font") == 0){
        return HFCL_NEW_EX(Font, (&MMI_SMALL_FONT));
    } else if (strcmp(fontname, "medium font") == 0){
        return HFCL_NEW_EX(Font, (&MMI_MEDIUM_FONT));
    } else if (strcmp(fontname, "medium bold font") == 0){
        return HFCL_NEW_EX(Font, (&MMI_MEDIUM_BOLD_FONT));
    } else if (strcmp(fontname, "large font") == 0){
        return HFCL_NEW_EX(Font, (&MMI_LARGE_FONT));
    } else if (strcmp(fontname, "dial font 1") == 0){
        return HFCL_NEW_EX(Font, (&MMI_DIALER_FONT1));
    } else if (strcmp(fontname, "dial font 2") == 0){
        return HFCL_NEW_EX(Font, (&MMI_DIALER_FONT2));
    }

	return HFCL_NEW_EX(Font, (&MMI_DEFAULT_FONT));
}

Font::~Font()
{
}

Logfont* Font::getLogfont()
{
    return m_logfont;
}

const char* Font::getFontType()
{
    return NULL;
}

const char* Font::getFontFamily()
{
    return NULL;
}

const char* Font::getFontCharset()
{
    return NULL;
}

DWORD Font::getFontStyle()
{
    return 0L;
}

int Font::getFontSize()
{
    return 0;
}

int Font::getFontRotation()
{
    return 0;
}

#else

Font* Font::createFont(const char* fontname)
{
	if(!fontname)
		return NULL;

	static const char* _sys_font_name[] = {
		"default",
		"defwchar",
		"fixed",
		"caption",
		"menu",
		"control",
		NULL
	};

	Logfont * logfont = NULL;
	
	for(int i=0; _sys_font_name[i]; i++)
	{
		if(strcmp(_sys_font_name[i], fontname) == 0)
		{
			logfont = GetSystemFont(i);
			break;
		}
	}

	if(!logfont)
		logfont = CreateLogFontByName(fontname);
	
	if(!logfont)
		return NULL;

	return HFCL_NEW_EX(Font, (logfont));
}

Font::~Font()
{
	if(!m_logfont)
		return ;

	for(int i=0; i < NR_SYSLOGFONTS; i++)
		if(g_SysLogFont[i] == m_logfont)
			return;
	DestroyLogFont(m_logfont);
}

Logfont* Font::getLogfont()
{
    return m_logfont;
}

const char* Font::getFontType()
{
    return (const char*)m_logfont->type;
}

const char* Font::getFontFamily()
{
    return (const char*)m_logfont->family;
}

const char* Font::getFontCharset()
{
    return (const char*)m_logfont->charset;
}

DWORD Font::getFontStyle()
{
    return (DWORD)m_logfont->style;
}

int Font::getFontSize()
{
    return m_logfont->size;
}

int Font::getFontRotation()
{
    return m_logfont->rotation;
}

} // namespace hfcl

#endif
