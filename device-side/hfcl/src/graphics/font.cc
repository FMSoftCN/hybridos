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

    for(int i=0; _sys_font_name[i]; i++) {
        if(strcmp(_sys_font_name[i], fontname) == 0) {
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

Font* Font::createFont(int idSysfont)
{
    return HFCL_NEW_EX(Font, (GetSystemFont(idSysfont)));
}

Font::Font(Logfont* logfont)
    : m_logfont(logfont)
{
    m_font_size = GraphicsContext::dots2px(m_logfont->size);

    Glyph32 glyph;
    GLYPHINFO info;

    glyph = GetGlyphValue(m_logfont, " ", 1, NULL, 0);
    info.mask = GLYPH_INFO_METRICS;
    GetGlyphInfo(m_logfont, glyph, &info);
    m_white_space_width = GraphicsContext::dots2px(info.advance_x);

    glyph = GetGlyphValue(m_logfont, "0", 1, NULL, 0);
    info.mask = GLYPH_INFO_METRICS;
    GetGlyphInfo(m_logfont, glyph, &info);
    m_glyph_height_0 = GraphicsContext::dots2px(info.bbox_h);

    glyph = GetGlyphValue(m_logfont, "x", 1, NULL, 0);
    info.mask = GLYPH_INFO_METRICS;
    GetGlyphInfo(m_logfont, glyph, &info);
    m_glyph_height_x = GraphicsContext::dots2px(info.bbox_h);
}

Font::~Font()
{
    if(!m_logfont)
        return ;

    for (int i = 0; i < NR_SYSLOGFONTS; i++) {
        if (g_SysLogFont[i] == m_logfont)
            return;
    }

    DestroyLogFont(m_logfont);
}

} // namespace hfcl

