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

#if 0
#include "common/common.h"
#include "common/log.h"
#include "common/object.h"
#include "common/intrect.h"
#include "graphics/graphicscontext.h"
#include "style.h"
#include "color.h"
#include "graphics/font.h"
#include "bmpfont.h"
#include "view/view.h"
#include "view/viewcontext.h"
#include "resource/respkgmanager.h"
#include "resource/respackage.h"
#include "resloader.h"
#include "drawable.h"
#include "graphics/image.h"

namespace hfcl {

BMPFont::GlyphMap::value_type BMPFont::m_initValue[] =
{
    BMPFont::GlyphMap::value_type(Style::GLYPHCOLON, ":"),
    BMPFont::GlyphMap::value_type(Style::GLYPHSLASH, "/"),
    BMPFont::GlyphMap::value_type(Style::GLYPHSUB, "-"),
    BMPFont::GlyphMap::value_type(Style::GLYPHPLUS, "+")
};

//BMPFont::GlyphMap BMPFont::m_glyphMap(m_initValue, m_initValue + 4);

BMPFont::BMPFont(const char* fontname, Style* style = NULL)
    :m_bitmap(NULL)
    ,m_style(NULL)
    ,m_devfont(NULL)
{
    int i;
    string st;
    //ResLoader* res = ResLoader::getInstance();
    m_gc = GraphicsContext::screenGraphics();
    m_style = style;
    if (NULL == m_style)
        m_style = Style::getCommStyle();
    m_glyphNum = Style::GLYPHEND - Style::GLYPHBEGIN - 1;
    //st = (const char*)m_style->getStyleElement(Style::BMPFONTFILE, Style::NORMAL);
    //m_bitmap = res->getBitmap(st.c_str());
    Image* img = m_style->getImage(Style::BMPFONTFILE, Style::NORMAL);
    if (img)
        m_bitmap = img->imageBitmap();
    m_devfont = m_gc->createDevBMPFontByName(fontname, m_bitmap);
    for (i = Style::GLYPHBEGIN + 1; i < Style::GLYPHEND; i++)
    {
        int w = (int)style->getStyleElement(Style::GLYPHWIDTH, Style::NORMAL);
        //st = ((const char*)m_style->getStyleElement((Style::ElementId)i, Style::NORMAL));
        //Bitmap* pbmp = res->getBitmap(st.c_str());
        Image* img = m_style->getImage((Style::ElementId)i, Style::NORMAL);
        if (img)
            m_gc->addGlyphsToBMPFont(m_devfont, img->imageBitmap(), m_glyphMap[(Style::ElementId)i], 1, w);
    }
    m_logfont = m_gc->createLogFontByName(fontname);
}

BMPFont::~BMPFont()
{
    int i;
    for (i = Style::GLYPHBEGIN + 1; i < Style::GLYPHEND; i++)
    {
        ResLoader::getInstance()->releaseBitmap((const char*)m_style->getStyleElement((Style::ElementId)i, Style::NORMAL));
    }
    m_gc->deleteDevBMPFont(m_devfont);
}



} // namespace hfcl
#endif
