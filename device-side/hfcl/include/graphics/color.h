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

#ifndef HFCL_GRAPHICS_COLOR_H_
#define HFCL_GRAPHICS_COLOR_H_

#include "common/common.h"

namespace hfcl {

class Color {
public:
    Color(Uint32 color) {
        m_a = color >> 24;
        m_b = (color & 0x00FF0000) >> 16;
        m_g = (color & 0x0000FF00) >> 8;
        m_r = (color & 0x000000FF);
    }

    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF) {
        m_r = r;
        m_g = g;
        m_b = b;
        m_a = a;
    }

    virtual ~Color(){}

    Uint8 r(){ return m_r; }
    Uint8 g(){ return m_g; }
    Uint8 b(){ return m_b; }
    Uint8 a(){ return m_a; }
    void setR(Uint8 ir){ m_r = ir; }
    void setG(Uint8 ig){ m_g = ig; }
    void setB(Uint8 ib){ m_b = ib; }
    void setA(Uint8 ia){ m_a = ia; }
    void setColor(Uint32 color){
        m_a = GetAValue(color);
        m_b = GetBValue(color);
        m_g = GetGValue(color);
        m_r = GetRValue(color);
    }

    Uint32 color(){ return MakeRGBA(m_r, m_g, m_b, m_a); }

    const static Uint32 BLACK = 0xFF000000;
    const static Uint32 WHITE = 0xFFFFFFFF;
    const static Uint32 MILKY = 0xFFEEEEEE;
    const static Uint32 GRAY  = 0xFF808080;
    const static Uint32 LIGHTGRAY  = 0xFFD3D3D3;
    const static Uint32 DARKGRAY  = 0xFF404040;
    const static Uint32 RED   = 0xFF0000FF;
    const static Uint32 GREEN = 0xFF00FF00;
    const static Uint32 BLUE  = 0xFFFF0000;
    const static Uint32 YELLOW  = 0xFF00FFFF;
    const static Uint32 LIGHTBROWN  = 0xFF30A0C0;
    const static Uint32 OCEAN  = 0xFF5B1702;
    const static Uint32 ALPHA_PANEL = MakeRGBA (0, 0, 0, 256*3/10);
    const static Uint32 TRANSPARENT= 0x00000000;
    const static Uint32 LAYER_COLOR_KEY = MakeRGBA (0, 0, 0, 0); //0xFFFE0000;
    const static Uint32 HOLD_BK = 0xFFFFC4D2;

private:
    Uint8 m_r;
    Uint8 m_g;
    Uint8 m_b;
    Uint8 m_a;
};

} // namespace hfcl

#endif /* HFCL_GRAPHICS_COLOR_H_ */
