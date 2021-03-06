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

#include "css/csscomputed.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "css/cssinitial.h"
#include "view/view.h"
#include "view/rootview.h"
#include "resource/respkgmanager.h"
#include "resource/resloader.h"

namespace hfcl {

#if SIZEOF_PTR == 8
inline HTReal realmax(HTReal x, HTReal y)
{
    return fmax(x, y);
}
inline HTReal realmin(HTReal x, HTReal y)
{
    return fmin(x, y);
}
#define HTHUGE_VAL HUGE_VAL
#else
inline HTReal realmax(HTReal x, HTReal y)
{
    return fmaxf(x, y);
}
inline HTReal realmin(HTReal x, HTReal y)
{
    return fminf(x, y);
}
#define HTHUGE_VAL HUGE_VALF
#endif

CssComputed::CssComputed()
{
    CssInitial* initial = CssInitial::getSingleton();
    for (int pid = 0; pid < MAX_CSS_PID; pid++) {
        m_values[pid] = CSS_PPT_VALUE_NOMARKS(initial->m_values[pid]);
    }
    memcpy(&m_data, initial->m_values, sizeof(m_data));

    for (int pid = MAX_CSS_PID; pid < MAX_CSS_PID_COMPUTED; pid++) {
        m_values[pid] = 0;
        m_data[pid].p = 0;
    }
}

CssComputed::CssComputed(const CssComputed& init)
{
    for (int pid = 0; pid < MAX_CSS_PID; pid++) {
        inherit(&init, pid);
    }

    for (int pid = MAX_CSS_PID; pid < MAX_CSS_PID_COMPUTED; pid++) {
        m_values[pid] = 0;
        m_data[pid].p = 0;
    }

    if (CSS_PPT_VALUE_TYPE(init.m_values[PID_INTERNAL_FONT])
            == PVT_INTERNAL_FONT) {
        Font* font = (Font*)(init.m_data[PID_INTERNAL_FONT].p);
        if (font) {
            font->ref();
            m_values[PID_INTERNAL_FONT] = init.m_values[PID_INTERNAL_FONT];
            m_data[PID_INTERNAL_FONT].p = init.m_data[PID_INTERNAL_FONT].p;
        }
    }
}

/* we use PID_INTERNAL_FONT to store the Font object */
void CssComputed::setFont(Font* font)
{
    m_values[PID_INTERNAL_FONT]
        = MAKE_CSS_PPT_VALUE(PVT_INTERNAL_FONT, PVK_USER_DATA);
    m_data[PID_INTERNAL_FONT].p = font;

    font->ref();
}

/* the font-size for medium in pixels */
#define FONT_MEDIUM_SIZE 14

/* the minimal font-size for subpixel enabled in dots */
#define FONT_SUBPIXEL_SIZE 20

Font* CssComputed::createFont() const
{
    /* create Font object here */
    Font* font = NULL;
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_FONT]);
    int kw = CSS_PPT_VALUE_KEYWORD(m_values[PID_FONT]);

    if (type == PVT_KEYWORD && kw == PVK_USE_OTHERS) {
        Uint32 value;

        // assemble the font name by using the individual property values
        // TODO font type and charset
        char family[LEN_FONT_NAME + 1];
        char style[LEN_FONT_NAME + 1];

        value = CSS_PPT_VALUE_NOMARKS(m_values[PID_FONT_FAMILY]);
        switch (value) {
        case PV_SERIF:
            strcpy(family, "serif");
            break;
        case PV_SANS_SERIF:
            strcpy(family, "sanserif");
            break;
        case PV_CURSIVE:
            strcpy(family, "cursive");
            break;
        case PV_FANTASY:
            strcpy(family, "fantasy");
            break;
        case PV_MONOSPACE:
            strcpy(family, "monospace");
            break;
        case PV_STRING:
            strncpy(family, (const char*)(m_data[PID_FONT_FAMILY].p),
                LEN_FONT_NAME);
            break;
        default:
            goto always_ret;
        }

        // TODO: convert font size to physical pixels
        HFCL_ASSERT(m_values[PID_FONT_SIZE] == PV_LENGTH_PX);
        int size = m_data[PID_FONT_SIZE].r;

        strcpy(style, "rrncnn");
        Uint32 pv = m_values[PID_FONT_WEIGHT];
        switch (pv) {
        case PV_100:
            style[0] = FONT_WEIGHT_THIN;
            break;
        case PV_200:
            style[0] = FONT_WEIGHT_EXTRA_LIGHT;
            break;
        case PV_300:
            style[0] = FONT_WEIGHT_LIGHT;
            break;
        case PV_400:
            style[0] = FONT_WEIGHT_REGULAR;
            break;
        case PV_500:
            style[0] = FONT_WEIGHT_MEDIUM;
            break;
        case PV_600:
            style[0] = FONT_WEIGHT_DEMIBOLD;
            break;
        case PV_700:
            style[0] = FONT_WEIGHT_BOLD;
            break;
        case PV_800:
            style[0] = FONT_WEIGHT_EXTRA_BOLD;
            break;
        case PV_900:
            style[0] = FONT_WEIGHT_BLACK;
            break;
        default:
            goto always_ret;
        }

        pv = m_values[PID_FONT_STYLE];
        switch (pv) {
        case PV_ITALIC:
            style[1] = FONT_SLANT_ITALIC;
            break;
        case PV_OBLIQUE:
            style[1] = FONT_SLANT_OBLIQUE;
            break;
        default:
            goto always_ret;
        }

        if (size <= FONT_SUBPIXEL_SIZE) {
            style[5] = FONT_RENDER_SUBPIXEL;
        }
        else {
            style[5] = FONT_RENDER_GREY;
        }

        const char* font_name_format = "ttf-%s-%s-*-%d-utf-8";
        char font_name[LEN_FONT_NAME*3 + 1];
        sprintf(font_name, font_name_format, family, style, size);
        font = ResLoader::getInstance()->getFont(font_name);
    }
    else if (type == PVT_STRING) {
        const char* font_name = (const char*)m_data[PID_FONT].p;
        font = ResLoader::getInstance()->getFont(font_name);
    }
    else if (type == PVT_SYSID) {
        // get the Font object via sysid
        // for individual properties of font
        font = GetFontRes((HTResId)(m_data[PID_FONT].u));
    }

always_ret:

    if (!font) {
        // use system font here to always return a valid Font object
        font = Font::createFont(SYSLOGFONT_DEFAULT);
    }

    return font;
}

const Font* CssComputed::getFont() const
{
    /* if has been set, return it directly */
    if (CSS_PPT_VALUE_TYPE(m_values[PID_INTERNAL_FONT]) == PVT_INTERNAL_FONT) {
        return (const Font*)(m_data[PID_INTERNAL_FONT].p);
    }

    return NULL;
}

const Font* CssComputed::getFont()
{
    /* if has been set, return it directly */
    if (CSS_PPT_VALUE_TYPE(m_values[PID_INTERNAL_FONT]) == PVT_INTERNAL_FONT) {
        return (const Font*)(m_data[PID_INTERNAL_FONT].p);
    }

    Font* font = createFont();
    setFont(font);
    return font;
}

void CssComputed::releaseFont()
{
    if (CSS_PPT_VALUE_TYPE(m_values[PID_INTERNAL_FONT]) == PVT_INTERNAL_FONT) {
        Font* font = (Font*)(m_data[PID_INTERNAL_FONT].p);
        if (font)
            font->unref();
    }
}

void CssComputed::reset()
{
    freeArray();

    /* use initial CSS values */
    CssInitial* initial = CssInitial::getSingleton();
    for (int pid = 0; pid < MAX_CSS_PID; pid++) {
        m_values[pid] = CSS_PPT_VALUE_NOMARKS(initial->m_values[pid]);
    }
    memcpy(&m_data, initial->m_values, sizeof(m_data));

    /* release font if there is one */
    releaseFont();

    for (int pid = MAX_CSS_PID; pid < MAX_CSS_PID_COMPUTED; pid++) {
        m_values[pid] = 0;
        m_data[pid].p = 0;
    }
}

void CssComputed::inherit(const CssComputed* css, int pid)
{
    if (css == NULL)
        return;

    CssInitial* initial = CssInitial::getSingleton();

    if (IS_CSS_PPT_VALUE_ALLOCATED(css->m_values[pid])) {

        Uint32 type = CSS_PPT_VALUE_TYPE(css->m_values[pid]);
        void* p = NULL;

        if (type == PVT_ARRAY_LENGTH_PX) {
            Uint8 n = initial->m_arraysize[pid];
            p = malloc(sizeof(HTReal) * n);
            if (p)
                memcpy(p, css->m_data[pid].p, sizeof(HTReal)*n);
        }
        else if (type == PVT_STRING) {
            p = strdup ((const char*)css->m_data[pid].p);
        }

        if (p) {
            m_values[pid] = CSS_PPT_VALUE_NOMARKS(css->m_values[pid]);
            MARK_CSS_PPT_VALUE_ALLOCATED(m_values[pid]);
            m_data[pid].p = p;
        }
        else {
            /* use initial value */
            m_values[pid] = CSS_PPT_VALUE_NOMARKS(initial->m_values[pid]);
            m_data[pid] = initial->m_data[pid];
        }
    }
    else {
        m_values[pid] = CSS_PPT_VALUE_NOMARKS(css->m_values[pid]);
        m_data[pid] = css->m_data[pid];
    }
}

/*
 * The properties of anonymous boxes are inherited from the enclosing
 * non-anonymous box. Non-inherited properties have their initial value.
 *
 * Ref: https://www.w3.org/TR/CSS22/visuren.html#anonymous-block-level
 */
void CssComputed::inherit(const CssComputed* css)
{
    if (css == NULL)
        return;

    CssInitial* initial = CssInitial::getSingleton();

    for (int pid = 0; pid < MAX_CSS_PID; pid++) {

        if (IS_CSS_PPT_VALUE_INHERITED(initial->m_values[pid])) {
            inherit(css, pid);
        }
        else {
            m_values[pid] = CSS_PPT_VALUE_NOMARKS(initial->m_values[pid]);
            m_data[pid] = initial->m_data[pid];
        }
    }
}

void CssComputed::inherit(const View* parent, int pid)
{
    if (parent) {
        inherit(parent->getCssComputed(), pid);
    }
    else {
        CssInitial* initial = CssInitial::getSingleton();
        m_values[pid] = CSS_PPT_VALUE_NOMARKS(initial->m_values[pid]);
        m_data[pid] = initial->m_data[pid];
    }
}

void CssComputed::freeArray()
{
    for (int i = 0; i < MAX_CSS_PID; i++) {
        if (IS_CSS_PPT_VALUE_ALLOCATED(m_values[i])) {
            if (m_data[i].p) {
                free((void*)(m_data[i].p));
                m_data[i].p = NULL;
            }

            CLEAR_CSS_PPT_VALUE_ALLOCATED(m_values[i]);
        }
    }
}

/* absolute lengths */
bool CssComputed::convertArray(int pid, int t)
{
    CssInitial* initial = CssInitial::getSingleton();
    Uint8 n = initial->m_arraysize[pid];
    const HTReal* old = (const HTReal*)m_data[pid].p;
    if (n == 0 || old == NULL)
        return false;

    HTReal* p = (HTReal*)malloc(sizeof(HTReal) * n);
    switch (t) {
    /* absolute lengths */
    case PVT_ARRAY_LENGTH_CM:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/2.54);
        }
        break;
    case PVT_ARRAY_LENGTH_IN:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0);
        }
        break;
    case PVT_ARRAY_LENGTH_MM:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/2.54/10);
        }
        break;
    case PVT_ARRAY_LENGTH_PC:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/6.0);
        }
        break;
    case PVT_ARRAY_LENGTH_PT:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/72.0);
        }
        break;
    case PVT_ARRAY_LENGTH_Q:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/2.54/40);
        }
        break;
    default:
        free(p);
        return false;
    }

    m_values[pid] = CSS_PPT_VALUE_MARK_ALLOCATED | PV_ARRAY_LENGTH_PX;
    m_data[pid].p = p;
    return true;
}

/* viewport-relative lengths */
bool CssComputed::convertArray(int pid, int t, const RealRect& viewport)
{
    CssInitial* initial = CssInitial::getSingleton();
    Uint8 n = initial->m_arraysize[pid];
    const HTReal* old = (const HTReal*)m_data[pid].p;
    if (n == 0 || old == NULL)
        return false;

    HTReal* p = (HTReal*)malloc(sizeof(HTReal) * n);
    switch (t) {
    case PVT_ARRAY_LENGTH_VW:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.width()/100);
        }
        break;
    case PVT_ARRAY_LENGTH_VH:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.height()/100);
        }
        break;
    case PVT_ARRAY_LENGTH_VMAX:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.maxWH()/100);
        }
        break;
    case PVT_ARRAY_LENGTH_VMIN:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.minWH()/100);
        }
        break;
    default:
        free(p);
        return false;
    }

    m_values[pid] = CSS_PPT_VALUE_MARK_ALLOCATED | PV_ARRAY_LENGTH_PX;
    m_data[pid].p = p;
    return true;
}

/* font-relative lengths */
bool CssComputed::convertArray(int pid, int t, const Font* font,
        const View* root)
{
    CssInitial* initial = CssInitial::getSingleton();
    Uint8 n = initial->m_arraysize[pid];
    const HTReal* old = (const HTReal*)m_data[pid].p;
    if (n == 0 || old == NULL)
        return false;

    HTReal* p = (HTReal*)malloc(sizeof(HTReal) * n);
    switch(t) {
    case PVT_ARRAY_LENGTH_EM:
        for (int i = 0; i < n; i++) {
            p[i] = old[i] * font->getFontSize();
        }
        break;

    case PVT_ARRAY_LENGTH_EX:
        for (int i = 0; i < n; i++) {
            p[i] = old[i] * font->getGlyphHeight_x();
        }
        break;

    case PVT_ARRAY_LENGTH_CH:
        for (int i = 0; i < n; i++) {
            p[i] = old[i] * font->getGlyphHeight_0();
        }
        break;

    case PVT_ARRAY_LENGTH_REM: {
        const Font* root_font = root->getCssComputed()->getFont();
        HFCL_ASSERT(root_font);
        for (int i = 0; i < n; i++) {
            p[i] = old[i] * root_font->getFontSize();
        }
        break;
    }

    default:
        free(p);
        return false;
    }

    m_values[pid] = CSS_PPT_VALUE_MARK_ALLOCATED | PV_ARRAY_LENGTH_PX;
    m_data[pid].p = p;
    return true;
}

void CssComputed::validateBorderWidth(int pid)
{
    if (m_values[pid] == PV_BW_THIN) {
        m_values[pid] = PV_LENGTH_PX;
        m_data[pid].r = 1.0;
    }
    else if (m_values[pid] == PV_BW_MEDIUM) {
        m_values[pid] = PV_LENGTH_PX;
        m_data[pid].r = 2.0;
    }
    else if (m_values[pid] == PV_BW_THICK) {
        m_values[pid] = PV_LENGTH_PX;
        m_data[pid].r = 4.0;
    }
    else if (m_values[pid] == PV_LENGTH_PX &&
            m_data[pid].r < 0) {
        m_data[pid].r = 0;
    }
    else {
        _ERR_PRINTF ("%s: invalid property value: %d\n",
            __FUNCTION__, pid);
    }
}

void CssComputed::validateNotNegative(int pid)
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[pid]);
    if ((type == PVT_PERCENTAGE || type == PVT_LENGTH_PX) &&
            m_data[pid].r < 0) {
        m_data[pid].r = 0;
    }
}

#include "pvcolors.inc"

inline HTReal hue2rgb(HTReal m1, HTReal m2, HTReal h)
{
    if (h < 0)
        h = h + 1;
    if (h > 1)
        h = h - 1;

    if (h * 6 < 1)
        return m1+(m2-m1)*h*6;

    if (h * 2 < 1)
        return m2;

    if (h * 3 < 2)
        return m1+(m2-m1)*(2/3-h)*6;

    return m1;
}

inline unsigned char rgba_real_to_char(HTReal real)
{
    // special values
    if (real > 0.999)
        return 0xFF;
    else if (real < 001)
        return 00;
    else if (real > 0.499 && real < 0.501)
        return 0x80;

    return (unsigned char)(real * 255);
}

static RGBCOLOR hsl2rgb(HTReal h, HTReal s, HTReal l, HTReal a)
{
    HTReal m1, m2;
    unsigned char r, g, b;

    h = h / 360;
    if (h < 0) h = 0;
    if (h > 1) h = 1;

    if (s < 0) s = 0;
    if (s > 1) s = 1;

    if (l < 0) l = 0;
    if (l > 1) l = 1;

    if (a < 0) a = 0;
    if (a > 1) a = 1;

    if (l < 0.5)
        m2 = l * (s + 1);
    else
        m2 = l + s - l * s;

    m1 = l * 2 - m2;
    r = rgba_real_to_char(hue2rgb(m1, m2, h+1/3));
    g = rgba_real_to_char(hue2rgb(m1, m2, h));
    b = rgba_real_to_char(hue2rgb(m1, m2, h-1/3));

    return MakeRGBA(r, g, b, rgba_real_to_char(a));
}

/*
 * Ref: https://www.w3.org/TR/css-color-3/
 */
void CssComputed::handleColor(int pid)
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[pid]);
    int kw = CSS_PPT_VALUE_KEYWORD(m_values[pid]);
    RGBCOLOR rgba = 0;

    if (type == PVT_KEYWORD &&
            kw >= PVK_COLOR_ALICEBLUE && PVK_COLOR_YELLOWGREEN) {
        rgba = _css_color_keyword_to_rgba[kw - PVK_COLOR_ALICEBLUE];
    }
    else if (type == PVT_KEYWORD && kw == PVK_TRANSPARENT) {
        rgba = MakeRGBA(0, 0, 0, 0);
    }
    else if (type == PVT_RGB) {
        rgba = MakeRGB(GetRValue(m_data[pid].u), GetGValue(m_data[pid].u),
                       GetBValue(m_data[pid].u));
    }
    else if (type == PVT_RGBA) {
        rgba = m_data[pid].u;
    }
    else if (type == PVT_HSL) {
        HTReal* p = (HTReal*)m_data[pid].p;
        rgba = hsl2rgb(p[0], p[1], p[2], 1.0);
    }
    else if (type == PVT_HSLA) {
        HTReal* p = (HTReal*)m_data[pid].p;
        rgba = hsl2rgb(p[0], p[1], p[2], p[3]);
    }
    else if (type == PVT_KEYWORD && kw == PVK_CURRENT_COLOR) {
        // use value of color
        // The used value of the ‘currentColor’ keyword is the computed value
        // of the ‘color’ property. If the ‘currentColor’ keyword is set on 
        // the ‘color’ property itself, it is treated as ‘color: inherit’.
        //
        // Ref: https://www.w3.org/TR/css-color-3/#currentcolor-def
        rgba = m_data[PID_COLOR].u;
    }

    m_values[pid] = PV_RGBA;
    m_data[pid].u = rgba;
}

bool CssComputed::getParentPropertyValue(const View* view, CssPropertyIds pid,
        Uint32* value, HTPVData* data) const
{
    const View* parent = view->getParent();
    const CssComputed* parent_css;
    if (parent && (parent_css = parent->getCssComputed())) {
        return parent_css->getProperty(pid, value, data);
    }
    else {
        CssInitial* initial = CssInitial::getSingleton();
        return initial->getProperty(pid, value, data);
    }

    return false;
}

void CssComputed::handleFontSize(const View* view)
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_FONT_SIZE]);
    int kw = CSS_PPT_VALUE_KEYWORD(m_values[PID_FONT_SIZE]);
    HTReal font_size;

    if (type == PVT_KEYWORD) {
        switch (kw) {
        case PVK_XX_SMALL:
            font_size = FONT_MEDIUM_SIZE/4.0;
            break;
        case PVK_X_SMALL:
            font_size = FONT_MEDIUM_SIZE*2/4.0;
            break;
        case PVK_SMALL:
            font_size = FONT_MEDIUM_SIZE*3/4.0;
            break;
        case PVK_MEDIUM:
            font_size = FONT_MEDIUM_SIZE;
            break;
        case PVK_LARGE:
            font_size = FONT_MEDIUM_SIZE*5/4.0;
            break;
        case PVK_X_LARGE:
            font_size = FONT_MEDIUM_SIZE*6/4.0;
            break;
        case PVK_XX_LARGE:
            font_size = FONT_MEDIUM_SIZE*7/4.0;
            break;
        case PVK_LARGER: {
            Uint32 value;
            HTPVData data;
            if (getParentPropertyValue(view, PID_FONT_SIZE, &value, &data)) {
                HFCL_ASSERT(value == PV_LENGTH_PX);
                font_size = data.r * 1.25;
            }
            else {
                /* use the default value of font-size */
                font_size = FONT_MEDIUM_SIZE * 1.25;
            }
            break;
        }

        case PVK_SMALLER: {
            Uint32 value;
            HTPVData data;
            if (getParentPropertyValue(view, PID_FONT_SIZE, &value, &data)) {
                HFCL_ASSERT(value == PV_LENGTH_PX);
                font_size = data.r * 0.8;
            }
            else {
                /* use the default value of font-size */
                font_size = FONT_MEDIUM_SIZE * 0.8;
            }
            break;
        }

        default:
            return;
        }
    }
    else {
        return;
    }

    m_values[PID_FONT_SIZE] = PV_LENGTH_PX;
    m_data[PID_FONT_SIZE].r = font_size;
}

inline Uint32 font_weight_bolder(Uint32 value)
{
    switch (value) {
    case PV_100:
    case PV_200:
    case PV_300:
        return PV_400;
    case PV_400:
    case PV_500:
        return PV_700;
        return PV_700;
    case PV_600:
    case PV_700:
    case PV_800:
    case PV_900:
        return PV_900;
    default:
        return PV_700;
    }
}

inline Uint32 font_weight_lighter(Uint32 value)
{
    switch (value) {
    case PV_100:
    case PV_200:
    case PV_300:
    case PV_400:
    case PV_500:
        return PV_100;
    case PV_600:
    case PV_700:
        return PV_400;
    case PV_800:
    case PV_900:
        return PV_700;
    default:
        return PV_100;
    }
}

void CssComputed::handleFontWeight(const View* view)
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_FONT_WEIGHT]);
    int kw = CSS_PPT_VALUE_KEYWORD(m_values[PID_FONT_WEIGHT]);
    Uint32 pv;

    if (type == PVT_KEYWORD) {
        switch (kw) {
        case PVK_100:
        case PVK_200:
        case PVK_300:
        case PVK_400:
        case PVK_500:
        case PVK_600:
        case PVK_700:
        case PVK_800:
        case PVK_900:
            return;
        case PVK_NORMAL:
            pv = PV_400;
            break;
        case PVK_BOLD:
            pv = PV_700;
            break;
        case PVK_BOLDER: {
            Uint32 value;
            if (!getParentPropertyValue(view, PID_FONT_WEIGHT, &value, NULL)) {
                value = PV_400;
            }
            pv = font_weight_bolder(value);
            break;
        }

        case PVK_SMALLER: {
            Uint32 value;
            if (!getParentPropertyValue(view, PID_FONT_WEIGHT, &value, NULL)) {
                value = PV_400;
            }
            pv = font_weight_bolder(value);
            break;
        }

        default:
            return;
        }
    }
    else {
        return;
    }

    m_values[PID_FONT_WEIGHT] = pv;
}

// MiniGUI internal APIs
extern "C" {
BOOL fontGetFamilyFromName(const char* name, char* family);
DWORD fontGetStyleFromName(const char* name);
int fontGetHeightFromName(const char* name);
}

/* return true if individual properties of font has been handled */
bool CssComputed::handleFont()
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_FONT]);
    int kw = CSS_PPT_VALUE_KEYWORD(m_values[PID_FONT]);

    if (type == PVT_KEYWORD && kw == PVK_USE_OTHERS) {
        return false;
    }
    else if (type == PVT_STRING) {
        // use the font attributes from font name
        // for individual properties of font

        const char* font_name = (const char*)m_data[PID_FONT].p;
        char family[LEN_FONT_NAME + 1];
        DWORD style;
        int size;

        if (fontGetFamilyFromName(font_name, family)) {
            char* p = strdup(family);
            m_values[PID_FONT_FAMILY] = PV_STRING;
            m_data[PID_FONT_FAMILY].p = p;
            MARK_CSS_PPT_VALUE_ALLOCATED(m_values[PID_FONT_FAMILY]);
        }
        else {
            m_values[PID_FONT_FAMILY] = PV_SERIF;
        }

        if ((size = fontGetHeightFromName(font_name)) != -1) {
            m_values[PID_FONT_SIZE] = PV_LENGTH_PX;
            m_data[PID_FONT_SIZE].r = GraphicsContext::dots2px(size);
        }
        else {
            m_values[PID_FONT_SIZE] = PV_LENGTH_PX;
            m_data[PID_FONT_SIZE].r = 14;
        }

        if ((style = fontGetStyleFromName(font_name)) != 0xFFFFFFFF) {
            Uint32 weight = PV_400;
            switch (style & FS_WEIGHT_MASK) {
            case FS_WEIGHT_THIN:
                weight = PV_100;
                break;
            case FS_WEIGHT_EXTRA_LIGHT:
                weight = PV_200;
                break;
            case FS_WEIGHT_LIGHT:
                weight = PV_300;
                break;
            case FS_WEIGHT_REGULAR:
                weight = PV_400;
                break;
            case FS_WEIGHT_MEDIUM:
                weight = PV_500;
                break;
            case FS_WEIGHT_DEMIBOLD:
                weight = PV_600;
                break;
            case FS_WEIGHT_BOLD:
                weight = PV_700;
                break;
            case FS_WEIGHT_EXTRA_BOLD:
                weight = PV_800;
                break;
            case FS_WEIGHT_BLACK:
                weight = PV_900;
                break;
            default:
                weight = PV_400;
                break;
            }

            m_values[PID_FONT_WEIGHT] = weight;

            Uint32 slant = PV_NORMAL;
            switch (style & FS_SLANT_MASK) {
            case FS_SLANT_ITALIC:
                slant = PV_ITALIC;
                break;
            case FS_SLANT_OBLIQUE:
                slant = PV_OBLIQUE;
                break;
            }

            m_values[PID_FONT_STYLE] = slant;
        }

        return true;
    }
    else if (type == PVT_SYSID) {
        // get the system log font and use the font attributes
        // for individual properties of font
        Font* font = GetFontRes((HTResId)(m_data[PID_FONT].u));
        Logfont* lf;

        if (font && (lf = font->getLogfont())) {
            char* p = strdup(lf->family);
            m_values[PID_FONT_FAMILY] = PV_STRING;
            m_data[PID_FONT_FAMILY].p = p;
            MARK_CSS_PPT_VALUE_ALLOCATED(m_values[PID_FONT_FAMILY]);

            m_values[PID_FONT_SIZE] = PV_LENGTH_PX;
            m_data[PID_FONT_SIZE].r = GraphicsContext::dots2px(lf->size);

            Uint32 weight = PV_400;
            switch (lf->style & FS_WEIGHT_MASK) {
            case FS_WEIGHT_THIN:
                weight = PV_100;
                break;
            case FS_WEIGHT_EXTRA_LIGHT:
                weight = PV_200;
                break;
            case FS_WEIGHT_LIGHT:
                weight = PV_300;
                break;
            case FS_WEIGHT_REGULAR:
                weight = PV_400;
                break;
            case FS_WEIGHT_MEDIUM:
                weight = PV_500;
                break;
            case FS_WEIGHT_DEMIBOLD:
                weight = PV_600;
                break;
            case FS_WEIGHT_BOLD:
                weight = PV_700;
                break;
            case FS_WEIGHT_EXTRA_BOLD:
                weight = PV_800;
                break;
            case FS_WEIGHT_BLACK:
                weight = PV_900;
                break;
            default:
                weight = PV_400;
                break;
            }

            m_values[PID_FONT_WEIGHT] = weight;

            Uint32 slant = PV_NORMAL;
            switch (lf->style & FS_SLANT_MASK) {
            case FS_SLANT_ITALIC:
                slant = PV_ITALIC;
                break;
            case FS_SLANT_OBLIQUE:
                slant = PV_OBLIQUE;
                break;
            }

            m_values[PID_FONT_STYLE] = slant;

            setFont(font);
        }
        else {
            m_values[PID_FONT] = PV_USE_OTHERS;
            return false;
        }

        return true;
    }

    return false;
}

void CssComputed::handleTabSize(const Font* font)
{
    Uint32 value = CSS_PPT_VALUE_NOMARKS(m_values[PID_TAB_SIZE]);

    if (value == PV_NUMBER) {
        HTReal r;
        r = m_data[PID_TAB_SIZE].r;
        if (r < 0) r = 8;

        r *= font->getWhiteSpaceWidth();
        m_values[PID_TAB_SIZE] = PV_LENGTH_PX;
        m_data[PID_TAB_SIZE].r = r;
    }
}

void CssComputed::handleSpacing()
{
    Uint32 value;

    value = CSS_PPT_VALUE_NOMARKS(m_values[PID_WORD_SPACING]);
    if (value == PV_NORMAL) {
        m_values[PID_WORD_SPACING] = PV_LENGTH_PX;
        m_data[PID_WORD_SPACING].r = 0;
    }

    value = CSS_PPT_VALUE_NOMARKS(m_values[PID_LETTER_SPACING]);
    if (value == PV_NORMAL) {
        m_values[PID_LETTER_SPACING] = PV_LENGTH_PX;
        m_data[PID_LETTER_SPACING].r = 0;
    }
}

void CssComputed::handleBorders()
{
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_LEFT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_LEFT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_LEFT_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_LEFT_WIDTH].r = 0;
    }
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_TOP_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_TOP_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_TOP_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_TOP_WIDTH].r = 0;
    }
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_RIGHT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_RIGHT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_RIGHT_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_RIGHT_WIDTH].r = 0;
    }
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_BOTTOM_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOMARKS(m_values[PID_BORDER_BOTTOM_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_BOTTOM_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_BOTTOM_WIDTH].r = 0;
    }
}

// This value behaves the same as inherit (computes to its parent’s
// computed value) except that an inherited value of start or end is
// interpreted against the parent’s (or the initial containing block’s,
// if there is no parent) direction value and results in a computed value of
// either left or right. When specified on the text-align shorthand, sets
// both text-align-all and text-align-last to match-parent.
void CssComputed::handleTextAlign(const View* view, int pid)
{
    if (pid != PID_TEXT_ALIGN_ALL && pid != PID_TEXT_ALIGN_LAST)
        return;

    if (m_values[pid] == PV_MATCH_PARENT) {
        const View* parent = view->getParent();
        inherit(parent, pid);

        if (m_values[pid] == PV_START || m_values[pid] == PV_END) {
            Uint32 value;

            if (getParentPropertyValue(view, PID_DISPLAY, &value, NULL)) {
                if (value == PV_LTR) {
                    m_values[pid] = PV_LEFT;
                }
                else {
                    m_values[pid] = PV_RIGHT;
                }
            }
            else {
                // the default value
                m_values[pid] = PV_LEFT;
            }
        }
    }
}

/*
 * A specified value can be either absolute (i.e., not relative to
 * another value, as in red or 2mm) or relative (i.e., relative to
 * another value, as in auto, 2em). Computing a relative value
 * generally absolutizes it:
 *  - values with relative units (em, ex, vh, vw) must be made absolute
 *    by multiplying with the appropriate reference size.
 *  - certain keywords (e.g., smaller, bolder) must be replaced according to
 *    their definitions.
 *  - percentages on some properties must be multiplied by a reference value
 *    (defined by the property).
 *  - valid relative URLs must be resolved to become absolute.
 *
 * REF: https://www.w3.org/TR/css-cascade-3/
 */
bool CssComputed::makeAbsolute(const View* view)
{
    const RootView* root = view->getRoot();
    /* viewport should be defined in px */
    const RealRect& viewport = root->viewport();

    /* handle special properties here: e.g., font, font-size, and so on */
    if (!handleFont()) {
        handleFontSize(view);
        handleFontWeight(view);
    }

    // if it is root element, call getFont to set the Font object
    if (view == root) {
        getFont();
    }

    const Font* font = getFont();

    handleTabSize(font);
    handleSpacing();
    handleTextAlign(view, PID_TEXT_ALIGN_ALL);
    handleTextAlign(view, PID_TEXT_ALIGN_LAST);
    handleBorders();


    handleColor(PID_COLOR);
    handleColor(PID_BACKGROUND_COLOR);
    handleColor(PID_BORDER_LEFT_COLOR);
    handleColor(PID_BORDER_TOP_COLOR);
    handleColor(PID_BORDER_RIGHT_COLOR);
    handleColor(PID_BORDER_BOTTOM_COLOR);
    handleColor(PID_OUTLINE_COLOR);

    /* handle length and percentage values */
    for (int i = 0; i < MAX_CSS_PID; i++) {

        Uint32 type = CSS_PPT_VALUE_TYPE(m_values[i]);
        switch (type) {
        case PVT_PERCENTAGE:
            break;

        /* absolute lengths */
        case PVT_LENGTH_CM:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/2.54);
            break;
        case PVT_LENGTH_IN:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0);
            break;
        case PVT_LENGTH_MM:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/2.54/10);
            break;
        case PVT_LENGTH_PC:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/6.0);
            break;
        case PVT_LENGTH_PT:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/72.0);
            break;
        case PVT_LENGTH_Q:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/2.54/40);
            break;

        /* viewport-relative lengths */
        case PVT_LENGTH_VW:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.width()/100);
            break;
        case PVT_LENGTH_VH:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.height()/100);
            break;
        case PVT_LENGTH_VMAX:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.maxWH()/100);
            break;
        case PVT_LENGTH_VMIN:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.minWH()/100);
            break;

        /* font-relative lengths */
        case PVT_LENGTH_CH:
            // Equal to the used advance measure of the "0" glyph
            // TODO: The advance measure of a glyph is its advance
            // width or height, whichever is in the inline axis of the element.
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r *= font->getGlyphHeight_0();
            break;
        case PVT_LENGTH_EM:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r *= font->getFontSize();
            break;
        case PVT_LENGTH_EX:
            // The x-height is so called because it is often 
            // equal to the height of the lowercase "x".
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r *= font->getGlyphHeight_x();
            break;
        case PVT_LENGTH_REM: {
            if (root == view && i == PID_FONT_SIZE) {
                // If used in the font-size property of the root element,
                // or in a document with no root element, 1rem is equal
                // to the initial value of the font-size property.
                CssInitial* initial = CssInitial::getSingleton();
                m_values[i] = PV_LENGTH_PX;
                m_data[i].r *= initial->m_data[PID_FONT_SIZE].r;
            }
            else {
                const Font* root_font = root->getCssComputed()->getFont();
                HFCL_ASSERT(root_font);

                m_values[i] = PV_LENGTH_PX;
                m_data[i].r *= root_font->getFontSize();
            }
            break;
        }

        /* percentages */
        case PVT_ARRAY_PERCENTAGE:
            break;

        /* absolute lengths */
        case PVT_ARRAY_LENGTH_CM:
        case PVT_ARRAY_LENGTH_MM:
        case PVT_ARRAY_LENGTH_IN:
        case PVT_ARRAY_LENGTH_PC:
        case PVT_ARRAY_LENGTH_PT:
        case PVT_ARRAY_LENGTH_Q:
            convertArray(i, type);
            break;

        /* viewport-relative lengths */
        case PVT_ARRAY_LENGTH_VW:
        case PVT_ARRAY_LENGTH_VH:
        case PVT_ARRAY_LENGTH_VMAX:
        case PVT_ARRAY_LENGTH_VMIN:
            convertArray(i, type, viewport);
            break;

        /* font-relative lengths */
        case PVT_ARRAY_LENGTH_EM:
        case PVT_ARRAY_LENGTH_EX:
        case PVT_ARRAY_LENGTH_CH:
        case PVT_ARRAY_LENGTH_REM:
            convertArray(i, type, font, root);
            break;

        case PVT_ARRAY_LENGTH_PX:
        case PVT_LENGTH_PX:
        default:
            break;
        }
    }

    return true;
}

bool CssComputed::validate(const View* view)
{
    /* make sure border widths are not negative */
    validateBorderWidth(PID_BORDER_LEFT_WIDTH);
    validateBorderWidth(PID_BORDER_TOP_WIDTH);
    validateBorderWidth(PID_BORDER_RIGHT_WIDTH);
    validateBorderWidth(PID_BORDER_BOTTOM_WIDTH);

    /* make sure paddings are not negative */
    validateNotNegative(PID_PADDING_LEFT);
    validateNotNegative(PID_PADDING_TOP);
    validateNotNegative(PID_PADDING_RIGHT);
    validateNotNegative(PID_PADDING_BOTTOM);

    /*
     * adjust display, position, and float
     * NOTE: HFCL only supports the following display values:
     *  inline block inline-block
     */
    if (view->isRoot()) {
        m_values[PID_DISPLAY] = PV_BLOCK;
    }
    else {
        if (m_values[PID_POSITION] == PV_FIXED ||
                m_values[PID_POSITION] == PV_ABSOLUTE) {
            m_values[PID_FLOAT] = PV_NONE;
            if (m_values[PID_DISPLAY] == PV_INLINE ||
                    m_values[PID_DISPLAY] == PV_INLINE_BLOCK) {
                m_values[PID_DISPLAY] = PV_BLOCK;
            }
        }
        else if (m_values[PID_FLOAT] != PV_NONE) {
            if (m_values[PID_DISPLAY] == PV_INLINE ||
                    m_values[PID_DISPLAY] == PV_INLINE_BLOCK) {
                m_values[PID_DISPLAY] = PV_BLOCK;
            }
        }
    }

    return true;
}

bool CssComputed::getProperty(CssPropertyIds pid, Uint32 *value,
        HTPVData *data) const
{
    if (value) {
        *value = CSS_PPT_VALUE_NOMARKS(m_values[pid]);
    }
    if (data) {
        *data = m_data[pid];
    }
    return true;
}

bool CssComputed::setProperty(CssPropertyIds pid, Uint32 value,
        HTPVData data)
{
    m_values[pid] = value;
    m_data[pid] = data;
    return true;
}

/* return true for auto */
bool CssComputed::getZIndex(int& zindex) const
{
    if (m_values[PID_Z_INDEX] == PV_AUTO)
        return true;

    zindex = m_data[PID_Z_INDEX].i;
    return false;
}

unsigned char CssComputed::getOpacity() const
{
    return rgba_real_to_char(m_data[PID_OPACITY].r);
}

void CssComputed::autoHMarginsAsZero(const CssBox* ctnBlock,
        HTReal& ml, HTReal& mr) const
{
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_MARGIN_LEFT]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        ml = 0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_LEFT]) ==
            PVT_LENGTH_PX) {
            ml = m_data[PID_MARGIN_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_LEFT]) ==
            PVT_PERCENTAGE) {
            ml = ctnBlock->getContentWidth() * m_data[PID_MARGIN_LEFT].r / 100;
        }
    }

    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_MARGIN_RIGHT]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        mr = 0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_RIGHT]) ==
            PVT_LENGTH_PX) {
            mr = m_data[PID_MARGIN_RIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_RIGHT]) ==
            PVT_PERCENTAGE) {
            mr = ctnBlock->getContentWidth() * m_data[PID_MARGIN_RIGHT].r / 100;
        }
    }
}

void CssComputed::autoVMarginsAsZero(const CssBox* ctnBlock,
        HTReal& mt, HTReal& mb) const
{
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_MARGIN_TOP]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        mt = 0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_TOP]) ==
            PVT_LENGTH_PX) {
            mt = m_data[PID_MARGIN_TOP].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_TOP]) ==
            PVT_PERCENTAGE) {
            mt = ctnBlock->getContentWidth() * m_data[PID_MARGIN_TOP].r / 100;
        }
    }

    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_MARGIN_BOTTOM]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        mb = 0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_BOTTOM]) ==
            PVT_LENGTH_PX) {
            mb = m_data[PID_MARGIN_BOTTOM].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_BOTTOM]) ==
            PVT_PERCENTAGE) {
            mb = ctnBlock->getContentWidth() * m_data[PID_MARGIN_BOTTOM].r / 100;
        }
    }
}

#if 0
bool CssComputed::hasComputedMarginLeft(const CssBox* ctnBlock, HTReal& ml)
{
    Uint32 v = m_values[PID_MARGIN_LEFT];

    if (CSS_PPT_VALUE_NOMARKS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            ml = m_data[PID_MARGIN_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            ml = ctnBlock->getContentWidth() * m_data[PID_MARGIN_LEFT].r / 100;
        }
    }

    return true;
}

bool CssComputed::hasComputedMarginRight(const CssBox* ctnBlock, HTReal& mr)
{
    Uint32 v = m_values[PID_MARGIN_RIGHT];

    if (CSS_PPT_VALUE_NOMARKS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            mr = m_data[PID_MARGIN_RIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            mr = ctnBlock->getContentWidth() * m_data[PID_MARGIN_RIGHT].r / 100;
        }
    }

    return true;
}

bool CssComputed::hasComputedWidth(const CssBox* ctnBlock, HTReal& w)
{
    Uint32 v = m_values[PID_WIDTH];

    if (CSS_PPT_VALUE_NOMARKS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            w = m_data[PID_WIDTH].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            w = ctnBlock->getContentWidth() * m_data[PID_WIDTH].r / 100;
        }
    }

    return true;
}

bool CssComputed::hasComputedHeight(const CssBox* ctnBlock, HTReal& h)
{
    Uint32 v = m_values[PID_HEIGHT];

    if (CSS_PPT_VALUE_NOMARKS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            h = m_data[PID_HEIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            h = ctnBlock->getContentHeight() * m_data[PID_HEIGHT].r / 100;
        }
    }

    return true;
}
#endif

/* return true for auto
 *
 * TODO:
 * For absolutely positioned elements whose containing block
 * is based on a block container element, the percentage is
 * calculated with respect to the width of the padding box of
 * that element. This is a change from CSS1, where the percentage
 * width was always calculated with respect to the content box
 * of the parent element.
 */
bool CssComputed::getHSize(const CssBox* ctnBlock, int pid, HTReal& s) const
{
    Uint32 v = m_values[pid];

    if (CSS_PPT_VALUE_NOMARKS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return true;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            s = m_data[pid].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            s = ctnBlock->getContentWidth() * m_data[pid].r / 100;
        }
    }

    return false;
}

/* return false if min-width was illegal */
bool CssComputed::getMinWidth(const CssBox* ctnBlock, HTReal& minw) const
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_MIN_WIDTH]);

    if (type == PVT_PERCENTAGE) {
        minw = ctnBlock->getContentWidth() * m_data[PID_MIN_WIDTH].r / 100;
    }
    else if (type == PVT_LENGTH_PX) {
        minw = m_data[PID_MIN_WIDTH].r;
    }
    else {
        minw = 0;
        return false;
    }

    if (minw < 0) {
        minw = 0;
        return false;
    }

    return true;
}

/* return true if max-width is defined and not negative */
bool CssComputed::getMaxWidth(const CssBox* ctnBlock, HTReal& maxw) const
{
    Uint32 v = m_values[PID_MAX_WIDTH];

    maxw = HTHUGE_VAL;
    if (CSS_PPT_VALUE_NOMARKS(v) == PV_NONE) {
        return false;
    }

    Uint32 type = CSS_PPT_VALUE_TYPE(v);
    if (type == PVT_PERCENTAGE) {
        maxw = ctnBlock->getContentWidth() * m_data[PID_MAX_WIDTH].r / 100;
    }
    else if (type == PVT_LENGTH_PX) {
        maxw = m_data[PID_MAX_WIDTH].r;
    }
    else {
        return false;
    }

    if (maxw < 0) {
        return false;
    }

    return true;
}

/* return false if min-height was illegal */
bool CssComputed::getMinHeight(const CssBox* ctnBlock, HTReal& minw) const
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_MIN_HEIGHT]);

    if (type == PVT_PERCENTAGE) {
        minw = ctnBlock->getContentHeight() * m_data[PID_MIN_HEIGHT].r / 100;
    }
    else if (type == PVT_LENGTH_PX) {
        minw = m_data[PID_MIN_HEIGHT].r;
    }
    else {
        minw = 0;
        return false;
    }

    if (minw < 0) {
        minw = 0;
        return false;
    }

    return true;
}

/* return true if max-height is defined and not negative */
bool CssComputed::getMaxHeight(const CssBox* ctnBlock, HTReal& maxh) const
{
    Uint32 v = m_values[PID_MAX_HEIGHT];

    maxh = HTHUGE_VAL;
    if (CSS_PPT_VALUE_NOMARKS(v) == PV_NONE) {
        return false;
    }

    Uint32 type = CSS_PPT_VALUE_TYPE(v);
    if (type == PVT_PERCENTAGE) {
        maxh = ctnBlock->getContentHeight() * m_data[PID_MAX_HEIGHT].r / 100;
    }
    else if (type == PVT_LENGTH_PX) {
        maxh = m_data[PID_MAX_HEIGHT].r;
    }
    else {
        return false;
    }

    if (maxh < 0) {
        return false;
    }

    return true;
}

/* return true for auto */
bool CssComputed::getVSize(const CssBox* ctnBlock, int pid, HTReal& s) const
{
    Uint32 v = m_values[pid];

    if (CSS_PPT_VALUE_NOMARKS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return true;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            s = m_data[pid].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            s = ctnBlock->getContentHeight() * m_data[pid].r / 100;
        }
        else
            s = 0;
    }

    return false;
}

void CssComputed::calcPaddings(const CssBox* ctnBlock,
        HTReal* pl, HTReal* pt, HTReal* pr, HTReal* pb) const
{
    if (pl) {
        Uint32 v = m_values[PID_PADDING_LEFT];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pl = m_data[PID_PADDING_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pl = ctnBlock->getContentWidth() * m_data[PID_PADDING_LEFT].r / 100;
        }
        else {
            *pl = 0;
        }

        if (*pl < 0)
            *pl = 0;
    }

    if (pt) {
        Uint32 v = m_values[PID_PADDING_TOP];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pt = m_data[PID_PADDING_TOP].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pt = ctnBlock->getContentHeight() * m_data[PID_PADDING_TOP].r / 100;
        }
        else {
            *pt = 0;
        }

        if (*pt < 0)
            *pt = 0;
    }

    if (pr) {
        Uint32 v = m_values[PID_PADDING_RIGHT];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pr = m_data[PID_PADDING_RIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pr = ctnBlock->getContentWidth() * m_data[PID_PADDING_RIGHT].r / 100;
        }
        else {
            *pr = 0;
        }

        if (*pr < 0)
            *pr = 0;
    }

    if (pb) {
        Uint32 v = m_values[PID_PADDING_BOTTOM];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pb = m_data[PID_PADDING_BOTTOM].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pb = ctnBlock->getContentHeight() * m_data[PID_PADDING_BOTTOM].r / 100;
        }
        else {
            *pb = 0;
        }

        if (*pb < 0)
            *pb = 0;
    }
}

// Inline, replaced elements
void CssComputed::calcWidthForIR(const View* view, const CssBox* ctnBlock,
        HTReal& w)
{
    HTReal inw, inh, inr;
    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_INTERNAL_WIDTH]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
        CSS_PPT_VALUE_NOMARKS(m_values[PID_INTERNAL_HEIGHT]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        if (view->getIntrinsicWidth(&inw)) {
            w = inw;
        }
        else if (view->getIntrinsicHeight(&inh) &&
                 view->getIntrinsicRatio(&inr)) {
            w = inh * inr;
        }
        else if (view->getIntrinsicRatio(&inr)) {
            w = 0; // undefined
        }
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_INTERNAL_WIDTH]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        HTReal h;
        bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
        if (!autoh && view->getIntrinsicRatio(&inr)) {
            w = h * inr;
        }
        else if (view->getIntrinsicWidth(&inw)) {
            w = inw;
        }
        else {
            w = 300;
            // TODO:
            // If 300px is too wide to fit the device,
            // UAs should use the width of the largest rectangle
            // that has a 2:1 ratio and fits the device instead.
        }
    }
}

// Inline, replaced elements
void CssComputed::calcHeightForIR(const View* view, const CssBox* ctnBlock,
        HTReal &w, HTReal& h)
{
    HTReal inh, inr;
    bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
    if (autoh) {
        bool autoinh = view->getIntrinsicHeight(&inh);
        bool autoinr = view->getIntrinsicRatio(&inr);
        if (CSS_PPT_VALUE_NOMARKS(m_values[PID_INTERNAL_WIDTH]) ==
                MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) && autoinh) {
            h = inh;
        }
        else if (autoinr) {
            h = w / inr;
        }
        else if (autoinh) {
            h = inh;
        }
        else {
            // the used value of 'height' must be set to the height of
            // the largest rectangle that has a 2:1 ratio, has a height
            // not greater than 150px, and has a width not greater than
            // the device width.
            h = w / 2.0;
            if (h > 150) {
                h = 150;
            }
        }
    }
}

void CssComputed::calcHeightForBlockVNR(const View* view,
        const CssBox* ctnBlock, CssBox* cssBox)
{
    HTReal h;

    bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
    if (autoh) {
        // TODO
        h = 0;
    }

    cssBox->setContentHeight(h);
}

void CssComputed::calcWidthForBlock(const CssBox* ctnBlock,
        HTReal& w, HTReal& ml, HTReal& mr, bool calced_w)
{
    // NOTE:
    // Computed border width must be absolute length;
    // '0' if the border style is 'none' or 'hidden'.
    HTReal blw = m_data[PID_BORDER_LEFT_WIDTH].r;
    HTReal brw = m_data[PID_BORDER_RIGHT_WIDTH].r;

    HTReal pl, pr;
    calcPaddings(ctnBlock, &pl, NULL, &pr, NULL);

    ml = mr = 0;
    bool automl = getHSize(ctnBlock, PID_MARGIN_LEFT, ml);
    bool automr = getHSize(ctnBlock, PID_MARGIN_RIGHT, mr);
    bool autow;
    if (calced_w) {
        /* w already determined for block and replaced element */
        autow = false;
    }
    else {
        autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
    }

    if (!autow && ((ml + blw + pl + w + pr + brw + mr) > ctnBlock->getContentWidth())) {
        if (automl) {
            ml = ctnBlock->getContentWidth() - w - mr - brw - blw - pr - pl;
        }
        else if (automr) {
            mr = ctnBlock->getContentWidth() - w - ml - brw - blw - pr - pl;
        }
        else {
            // over-constrained
            if (ctnBlock->getCss()->isLtr()) {
                // if direction of containing block is ltr
                mr = ctnBlock->getContentWidth() - w - ml - brw - blw - pr - pl;
            }
            else {
                // if direction of containing block is rtl
                ml = ctnBlock->getContentWidth() - w - mr - brw - blw - pr - pl;
            }
        }
    }
    else if (autow && !automl && !automr) {
        w = ctnBlock->getContentWidth() - ml - mr - brw - blw - pr - pl;
    }
    else if (!autow && automl && !automr) {
        ml = ctnBlock->getContentWidth() - w - mr - brw - blw - pr - pl;
    }
    else if (!autow && !automl && automr) {
        mr = ctnBlock->getContentWidth() - w - ml - brw - blw - pr - pl;
    }
    else if (autow) {
        if (automl) ml = 0;
        if (automr) mr = 0;
        w = ctnBlock->getContentWidth() - ml - mr - brw - blw - pr - pl;
    }
    else if (automl && automr) {
        ml = mr = (ctnBlock->getContentWidth() - w - brw - blw - pr - pl) / 2.0;
    }
}

void CssComputed::calcWidthsForAPNR(const View* view, const CssBox* ctnBlock,
        CssBox* box)
{
    HTReal w, ml, mr, l = 0, r = 0;
    bool autol = getHSize(ctnBlock, PID_LEFT, l);
    bool autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
    bool autor = getHSize(ctnBlock, PID_RIGHT, r);

    HTReal blw, brw;
    box->getHBorderWidths(blw, brw);

    HTReal pl, pr;
    box->getHPaddings(pl, pr);

    HTReal ctnw = ctnBlock->getContentWidth();

    if (autol && autow && autor) {
        autoHMarginsAsZero(ctnBlock, ml, mr);

        if (ctnBlock->getCss()->isLtr()) {
            // TODO: left is static position
            l = 0;

            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - l - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
        else {
            // TODO: right is static position
            r = 0;

            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - ml - blw - pl - pr - brw - mr - r;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
    }
    else if (!autol && !autow && !autor) {
        bool automl = getHSize(ctnBlock, PID_MARGIN_LEFT, ml);
        bool automr = getHSize(ctnBlock, PID_MARGIN_RIGHT, mr);
        if (automl && automr) {
            ml = (ctnw - l - blw - pl - pr - brw - r) / 2.0;
            mr = ml;
            if (ml < 0) {
                if (ctnBlock->getCss()->isLtr()) {
                    ml = 0;
                    mr = ctnw - l - ml - blw - pl - pr - brw - r;
                }
                else {
                    mr = 0;
                    ml = ctnw - l - blw - pl - pr - brw - mr - r;
                }
            }
        }
        else if (automl) {
            ml = ctnw - l - blw - pl - pr - brw - mr - r;
        }
        else if (automr) {
            mr = ctnw - l - ml - blw - pl - pr - brw - r;
        }
        else {
            if (ctnBlock->getCss()->isLtr()) {
                // ignore right
                r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
            }
            else {
                // ignore left
                l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
            }
        }
    }
    else {
        autoHMarginsAsZero(ctnBlock, ml, mr);

        if (autol && autow && !autor) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - ml - blw - pl - pr - brw - mr - r;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
        else if (autol && autor && !autow) {
            if (ctnBlock->getCss()->isLtr()) {
                // TODO: left is the static position
                l = 0;
                r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
            }
            else {
                // TODO: right is the static position
                r = 0;
                l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
            }
        }
        else if (autow && autor && !autol) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - l - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
        else if (autol && !autow && !autor) {
            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
        else if (!autol && autow && !autor) {
            w = ctnw - l - ml - blw - pl - pr - brw - mr - r;
        }
        else if (!autol && !autow && autor) {
            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
    }
}

void CssComputed::calcWidthsForAPR(const View* view, const CssBox* ctnBlock,
        CssBox* box)
{
    HTReal w, ml, mr, l = 0, r = 0;
    bool autol = getHSize(ctnBlock, PID_LEFT, l);
    bool autor = getHSize(ctnBlock, PID_RIGHT, r);
    bool automl = getHSize(ctnBlock, PID_MARGIN_LEFT, ml);
    bool automr = getHSize(ctnBlock, PID_MARGIN_RIGHT, mr);

    HTReal blw, brw;
    box->getHBorderWidths(blw, brw);

    HTReal pl, pr;
    box->getHPaddings(pl, pr);

    HTReal ctnw = ctnBlock->getContentWidth();

    calcWidthForIR(view, ctnBlock, w);

    if (automl || automr) {
        if (autol && autor) {
            if (automl) {
                ml = 0;
                automl = false;
            }
            if (automr) {
                mr = 0;
                automr = false;
            }

            if (ctnBlock->getCss()->isLtr()) {
                // TODO: left is static position
                l = 0;
                autol = false;
                r = ctnw - ml - blw - pl - w - pr - brw - mr;
                autor = false;
            }
            else {
                // TODO: right is static position
                r = 0;
                autor = false;
                l = ctnw - ml - blw - pl - w - pr - brw - mr;
                autor = false;
            }
        }
        else if (autol && automl) {
            ml = 0;
            automl = false;
        }
        else if (autor && automr) {
            mr = 0;
            automr = false;
        }
    }

    if (automl && automr) {
        ml = (ctnw - l - blw - pl - pr - brw - r) / 2.0;
        mr = ml;
        if (ml < 0) {
            if (ctnBlock->getCss()->isLtr()) {
                ml = 0;
                mr = ctnw - l - blw - pl - pr - brw - r;
            }
            else {
                mr = 0;
                ml = ctnw - l - blw - pl - pr - brw - r;
            }
        }
    }

    if (autol) {
        l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
    }
    else {
        if (ctnBlock->getCss()->isLtr()) {
            // ignore right
            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
        else {
            // ignore left
            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
    }
}

/*
 * Ref: https://www.w3.org/TR/CSS22/visudet.html#Computing_widths_and_margins
 * Return false for undefined width
 */
bool CssComputed::doCalcWidthsMargins(const View* view,
        const CssBox* ctnBlock,
        CssBox* cssBox)
{
    HTReal ctnw = ctnBlock->getContentWidth();

    HTReal blw = m_data[PID_BORDER_LEFT_WIDTH].r;
    HTReal brw = m_data[PID_BORDER_RIGHT_WIDTH].r;
    cssBox->setHBorderWidths(blw, brw);

    HTReal pl, pr;
    calcPaddings(ctnBlock, &pl, NULL, &pr, NULL);
    cssBox->setHPaddings(pl, pr);

    // determine width and margins
    HTReal w, ml, mr;

    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_INLINE &&
            !view->isReplaced()) {
        // inline, non-replaced elements

        w = 0; /* undefined so far */
        autoHMarginsAsZero(ctnBlock, ml, mr);
        cssBox->setHMargins(ml, mr);
        return false;
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_INLINE &&
            view->isReplaced()) {
        // inline, replaced elements

        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_BLOCK &&
            !view->isReplaced()) {
        // Block-level, non-replaced elements in normal flow
        calcWidthForBlock(ctnBlock, w, ml, mr, false);
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_BLOCK &&
            view->isReplaced()) {
        // Block-level, replaced elements in normal flow
        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
        calcWidthForBlock(ctnBlock, w, ml, mr, true);
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_FLOAT]) != PV_NONE &&
            !view->isReplaced()) {
        // Floating, non-replaced elements

        autoHMarginsAsZero(ctnBlock, ml, mr);
        bool autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
        if (autow) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);

            HTReal available = ctnw - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);
        }
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_FLOAT]) != PV_NONE &&
            view->isReplaced()) {
        // Floating, replaced elements

        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
    }
    else if ((CSS_PPT_VALUE_NOMARKS(m_values[PID_POSITION]) == PV_ABSOLUTE ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_POSITION]) == PV_FIXED) &&
            !view->isReplaced()) {
        // Absolutely positioned, non-replaced elements

        calcWidthsForAPNR(view, ctnBlock, cssBox);
    }
    else if ((CSS_PPT_VALUE_NOMARKS(m_values[PID_POSITION]) == PV_ABSOLUTE ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_POSITION]) == PV_FIXED) &&
            view->isReplaced()) {
        // Absolutely positioned, replaced elements

        calcWidthsForAPR(view, ctnBlock, cssBox);
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY])
                == PV_INLINE_BLOCK && !view->isReplaced()) {
        // 'Inline-block', non-replaced elements in normal flow

        autoHMarginsAsZero(ctnBlock, ml, mr);
        bool autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
        if (autow) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);
        }
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY])
                == PV_INLINE_BLOCK && view->isReplaced()) {
        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
    }
    else {
        _ERR_PRINTF("%s: why we are here?", __FUNCTION__);
        return false;
    }

    return true;
}

bool CssComputed::resolveWHForRIR(const View* view, const CssBox* ctnBlock,
        HTReal& w, HTReal& h)
{
    HTReal inw, inh, inr;

    if (view->getIntrinsicWidth(&inw)) {
        w = inw;
    }
    else if (view->getIntrinsicHeight(&inh) &&
            view->getIntrinsicRatio(&inr)) {
        w = inh * inr;
    }
    else if (view->getIntrinsicRatio(&inr)) {
        w = 0; // undefined
        return false;
    }

    if (view->getIntrinsicHeight(&inh)) {
        h = inh;
    }
    else if (view->getIntrinsicWidth(&inw) &&
             view->getIntrinsicRatio(&inr)) {
        h = inw / inr;
    }
    else if (view->getIntrinsicRatio(&inr)) {
        h = 0; // undefined
        return false;
    }

    HTReal minw, maxw, minh, maxh;
    bool valid_minw = getMinWidth(ctnBlock, minw);
    bool valid_maxw = getMaxWidth(ctnBlock, maxw);
    bool valid_minh = getMinHeight(ctnBlock, minh);
    bool valid_maxh = getMaxHeight(ctnBlock, maxh);

    maxw = realmax(minw, maxw);
    maxh = realmax(minh, maxh);

    if (valid_maxw && w > maxw && valid_maxh && h > maxh) {
        if ((maxw/w) <= (maxh/h)) {
            w = maxw;
            h = realmax(minh, (maxw*h/w));
        }
        else {
            w = realmax(minw, (maxw*w/h));
            h = maxh;
        }
    }
    else if (valid_minw && w < minw && valid_minh && h < minh) {
        if ((minw / w) <= (minh / h)) {
            w = realmin(maxw, (minh * w / h));
            h = minh;
        }
        else {
            w = minw;
            h = realmin(maxh, (minw * h / w));
        }
    }
    else if (valid_minw && w < minw && valid_maxh && h > maxh) {
        w = minw;
        h = maxh;
    }
    else if (valid_maxw && w > maxw && valid_minh && h < minh) {
        w = maxw;
        h = minh;
    }
    else if (valid_maxw && w > maxw) {
        w = maxw;
        h = realmax((maxw * h / w), minh);
    }
    else if (valid_minw && w < minw) {
        w = minw;
        h = realmin((minw * h / w), maxh);
    }
    else if (valid_maxh && h > maxh) {
        w = realmax((maxh * w / h), minw);
        h = maxh;
    }
    else if (valid_minh && h < minh) {
        w = realmin((minh * w / h), maxw);
        h = minh;
    }

    return true;
}

void CssComputed::calcHeightsForAPNR(const View* view, const CssBox* ctnBlock,
        CssBox* CssBox)
{
}

void CssComputed::calcHeightsForAPR(const View* view, const CssBox* ctnBlock,
        CssBox* CssBox)
{
}

void CssComputed::resolveAutoHeights(const View* view,
            const CssBox* ctnBlock,
            CssBox* cssBox)
{
    // TODO
}

/*
 * Ref: https://www.w3.org/TR/CSS22/visudet.html#Computing_heights_and_margins
 * Return false for undefined height
 */
bool CssComputed::doCalcHeightsMargins(const View* view,
        const CssBox* ctnBlock, CssBox* cssBox)
{
    //HTReal ctnh = ctnBlock->getContentHeight();

    HTReal btw = m_data[PID_BORDER_TOP_WIDTH].r;
    HTReal bbw = m_data[PID_BORDER_BOTTOM_WIDTH].r;
    cssBox->setVBorderWidths(btw, bbw);

    HTReal pt, pb;
    calcPaddings(ctnBlock, NULL, &pt, NULL, &pb);
    cssBox->setVPaddings(pt, pb);

    HTReal h, mt, mb;

    if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_INLINE &&
            !view->isReplaced()) {
        // inline, non-replaced elements

        h = 0; /* undefined so far */
        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);
        return false;
    }
    else if ((CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_INLINE ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_BLOCK ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_INLINE_BLOCK ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_FLOAT]) != PV_NONE) &&
            view->isReplaced()) {
        // Inline replaced elements, block-level replaced elements
        // in normal flow, 'inline-block' replaced elements in normal flow,
        // and floating replaced elements

        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);

        HTReal w = cssBox->getContentWidth();
        calcHeightForIR(view, ctnBlock, w, h);
    }
    else if (CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_BLOCK &&
            CSS_PPT_VALUE_NOMARKS(m_values[PID_OVERFLOW]) == PV_VISIBLE &&
            !view->isReplaced()) {

        // Block-level non-replaced elements in normal flow
        // when 'overflow' computes to 'visible'
        // NOTE:
        // This section also applies to block-level non-replaced elements
        // in normal flow when 'overflow' does not compute to 'visible'
        // but has been propagated to the viewport.

        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);
        calcHeightForBlockVNR(view, ctnBlock, cssBox);
    }
    else if ((CSS_PPT_VALUE_NOMARKS(m_values[PID_POSITION]) == PV_ABSOLUTE ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_POSITION]) == PV_FIXED)) {

        if (view->isReplaced()) {
            // Absolutely positioned, replaced elements
            calcHeightsForAPR(view, ctnBlock, cssBox);
        }
        else {
            // Absolutely positioned, non-replaced elements
            calcHeightsForAPNR(view, ctnBlock, cssBox);
        }
    }
    else if (!view->isReplaced() &&
            ((CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_BLOCK &&
             CSS_PPT_VALUE_NOMARKS(m_values[PID_OVERFLOW]) != PV_VISIBLE) ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_DISPLAY]) == PV_INLINE_BLOCK ||
            CSS_PPT_VALUE_NOMARKS(m_values[PID_FLOAT]) != PV_NONE)) {

        // Block-level, non-replaced elements in normal flow when 'overflow'
        // does not compute to 'visible' (except if the 'overflow' property's
        // value has been propagated to the viewport).
        //
        // 'Inline-block', non-replaced elements.
        //
        // Floating, non-replaced elements.
        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);

        bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
        if (autoh) {
            resolveAutoHeights(view, ctnBlock, cssBox);
        }
    }
    else {
        bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
        if (autoh) {
            resolveAutoHeights(view, ctnBlock, cssBox);
        }
    }

    cssBox->setContentHeight(h);
    return true;
}

bool CssComputed::calcWidthsMargins(const View* view, const CssBox* ctnBlock,
        CssBox* cssBox)
{
    // use PID_INTERNAL_WIDTH for the actually used property of width.
    // use PID_INTERNAL_HEIGHT for the actually used property of height.
    m_values[PID_INTERNAL_WIDTH] = m_values[PID_WIDTH];
    m_data[PID_INTERNAL_WIDTH] = m_data[PID_WIDTH];
    m_values[PID_INTERNAL_HEIGHT] = m_values[PID_HEIGHT];
    m_data[PID_INTERNAL_HEIGHT] = m_data[PID_HEIGHT];

    if (doCalcWidthsMargins(view, ctnBlock, cssBox)) {
        // check min-width, and max-width
        HTReal minw, maxw;
        bool valid_minw = getMinWidth(ctnBlock, minw);
        bool valid_maxw = getMaxWidth(ctnBlock, maxw);
        HTReal inr, rw, rh;

        if (valid_minw && cssBox->getContentWidth() < minw) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_WIDTH] = m_values[PID_MIN_WIDTH];
                m_data[PID_INTERNAL_WIDTH] = m_data[PID_MIN_WIDTH];
            }

            doCalcWidthsMargins(view, ctnBlock, cssBox);
        }

        if (valid_maxw && cssBox->getContentWidth() > maxw) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_WIDTH] = m_values[PID_MAX_WIDTH];
                m_data[PID_INTERNAL_WIDTH] = m_data[PID_MAX_WIDTH];
            }

            doCalcWidthsMargins(view, ctnBlock, cssBox);
        }

        return true;
    }

    return false;
}

bool CssComputed::calcHeightsMargins(const View* view, const CssBox* ctnBlock,
        CssBox* cssBox)
{
    // use PID_INTERNAL_WIDTH for the actually used property of width.
    // use PID_INTERNAL_HEIGHT for the actually used property of height.
    m_values[PID_INTERNAL_WIDTH] = m_values[PID_WIDTH];
    m_data[PID_INTERNAL_WIDTH] = m_data[PID_WIDTH];
    m_values[PID_INTERNAL_HEIGHT] = m_values[PID_HEIGHT];
    m_data[PID_INTERNAL_HEIGHT] = m_data[PID_HEIGHT];

    if (doCalcHeightsMargins(view, ctnBlock, cssBox)) {
        // check min-height, and max-height
        HTReal minh, maxh;
        bool valid_minh = getMinHeight(ctnBlock, minh);
        bool valid_maxh = getMaxHeight(ctnBlock, maxh);
        HTReal inr, rw, rh;

        if (valid_minh && cssBox->getContentHeight() < minh) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_HEIGHT] = m_values[PID_MIN_HEIGHT];
                m_data[PID_INTERNAL_HEIGHT] = m_data[PID_MIN_HEIGHT];
            }

            doCalcHeightsMargins(view, ctnBlock, cssBox);
        }

        if (valid_maxh && cssBox->getContentHeight() > maxh) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOMARKS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_HEIGHT] = m_values[PID_MAX_HEIGHT];
                m_data[PID_INTERNAL_HEIGHT] = m_data[PID_MAX_HEIGHT];
            }

            doCalcHeightsMargins(view, ctnBlock, cssBox);
        }

        return true;
    }

    return false;
}

} // namespace hfcl

