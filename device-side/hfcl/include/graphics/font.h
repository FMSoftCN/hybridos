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

#ifndef HFCL_GRAPHICS_FONT_H_
#define HFCL_GRAPHICS_FONT_H_

#include "../common/object.h"
#include "../graphics/graphicscontext.h"

namespace hfcl {

class Font : public RefCount {
public:
    // Font objects must be created by calling the static creators
    static Font* createFont(const char* fontname);
    static Font* createFont(int idSysfont);
    virtual ~Font();

    Logfont* getLogfont() const {
        return m_logfont;
    }

    const char* getFontType() const {
        return (const char*)m_logfont->type;
    }

    const char* getFontFamily() const {
        return (const char*)m_logfont->family;
    }

    const char* getFontCharset() const {
        return (const char*)m_logfont->charset;
    }

    DWORD getFontStyle() const {
        return (DWORD)m_logfont->style;
    }

    HTReal getFontSize() const {
        return GraphicsContext::dots2px(m_logfont->size);
    }

    int getFontRotation() const {
        return m_logfont->rotation;
    }

    HTReal getWhiteSpaceWidth() const {
        return m_white_space_width;
    }

    HTReal getGlyphHeight_0() const {
        return m_glyph_height_0;
    }

    HTReal getGlyphHeight_x() const {
        return m_glyph_height_x;
    }

protected:
    Font(Logfont* logfont);

    Logfont* m_logfont;

private:
    HTReal m_font_size;
    HTReal m_white_space_width;
    HTReal m_glyph_height_0;
    HTReal m_glyph_height_x;

    /* No public constructor
    Font() : m_logfont(0) {};
    */
};

} // namespace hfcl

#endif /* HFCL_GRAPHICS_FONT_H_ */
