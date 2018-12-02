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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _NGUX_COLOR_H_
#define _NGUX_COLOR_H_

#include "nguxcommon.h"

NAMESPACE_BEGIN

class Color /*: public Object*/{
public:
    Color(NGULong color);
    Color(NGUInt8 r, unsigned char g, unsigned char b, unsigned char a);
    virtual ~Color(){}

    NGUInt8 r(){ return m_r; }
    NGUInt8 g(){ return m_g; }
    NGUInt8 b(){ return m_b; }
    NGUInt8 a(){ return m_a; }
    void setR(NGUInt8 ir){ m_r = ir; }
    void setG(NGUInt8 ig){ m_g = ig; }
    void setB(NGUInt8 ib){ m_b = ib; }
    void setA(NGUInt8 ia){ m_a = ia; }
    void setColor(NGULong lcolor){
        m_a = GetAValue(lcolor);
        m_b = GetBValue(lcolor);
        m_g = GetGValue(lcolor);
        m_r = GetRValue(lcolor);
    }

    DWORD color(){ return MakeRGBA(m_r, m_g, m_b, m_a); }

    const static NGULong BLACK = 0xFF000000;
    const static NGULong WHITE = 0xFFFFFFFF;
    const static NGULong MILKY = 0xFFEEEEEE;
    const static NGULong GRAY  = 0xFF808080;
    const static NGULong LIGHTGRAY  = 0xFFD3D3D3;
    const static NGULong DARKGRAY  = 0xFF404040;
    const static NGULong RED   = 0xFF0000FF;
    const static NGULong GREEN = 0xFF00FF00;
    const static NGULong BLUE  = 0xFFFF0000;
    const static NGULong YELLOW  = 0xFF00FFFF;
    const static NGULong LIGHTBROWN  = 0xFF30A0C0;
    const static NGULong OCEAN  = 0xFF5B1702;
    const static NGULong ALPHA_PANEL = MakeRGBA (0, 0, 0, 256*3/10);
    const static NGULong TRANSPARENT= 0x00000000;
    const static NGULong LAYER_COLOR_KEY = MakeRGBA (0, 0, 0, 0); //0xFFFE0000;
    const static NGULong HOLD_BK = 0xFFFFC4D2;

//===========Theme Color===========//
// C101 (BG)
    const static NGULong ER_C101 = 0xFF00851E;
    const static NGULong SL_C101 = 0xFFFFFFFF;
    const static NGULong OC_C101 = 0xFF8F3C00;
    const static NGULong RT_C101 = 0xFF000000;

// C102 (List focus BG)
    const static NGULong ER_C102 = 0xFFFFFFFF;
    const static NGULong SL_C102 = 0xFFFF8900;
    const static NGULong OC_C102 = 0xFFFFFFFF;
    const static NGULong RT_C102 = MakeRGBA (229, 50, 2, 255);

// C103 (Indicator BG)
    const static NGULong ER_C103 = 0x0000851E;
    const static NGULong SL_C103 = 0x00FFFFFF;
    const static NGULong OC_C103 = 0x008F3C00;
    const static NGULong RT_C103 = 0xFF000000;

// C104 (Title BG)
    const static NGULong ER_C104 = 0x0000851E;
    const static NGULong SL_C104 = 0x00FFFFFF;
    const static NGULong OC_C104 = 0x008F3C00;
    const static NGULong RT_C104 = 0xFF000000;

// C105 (Soft key BG)
    const static NGULong ER_C105 = 0xFF004E07;
    const static NGULong SL_C105 = 0xFF666666;
    const static NGULong OC_C105 = 0xFF6A0700;
    const static NGULong RT_C105 = MakeRGBA (26, 26, 26, 255);

// C106 (List line (Title/Soft key))
    const static NGULong ER_C106 = 0x7FFFFFFF;
    const static NGULong SL_C106 = 0xFF9C9C9C;
    const static NGULong OC_C106 = 0x7FFFFFFF;
    const static NGULong RT_C106 = 0x7FFFFFFF;

// C107 (Pop-up BG)
    const static NGULong ER_C107 = 0xFFFFFFFF;
    const static NGULong SL_C107 = 0xFFFFFFFF;
    const static NGULong OC_C107 = 0xFFFFFFFF;
    const static NGULong RT_C107 = MakeRGBA (244, 244, 244, 255);

// C108 (Pop-up Progressbar)
    const static NGULong ER_C108 = 0xFF00851E;
    const static NGULong SL_C108 = 0xFFFF7E00;
    const static NGULong OC_C108 = 0xFF8F3C00;
    const static NGULong RT_C108 = 0xFF8F3C00;

// C201 (Unfocus Main menu icon)
    const static NGULong ER_C201 = 0x00FFFFFF;
    const static NGULong SL_C201 = 0x00FFFFFF;
    const static NGULong OC_C201 = 0x00FFFFFF;
    const static NGULong RT_C201 = 0x00FFFFFF;

// C202 (Focus Main menu icon)
    const static NGULong ER_C202 = 0x00FFFFFF;
    const static NGULong SL_C202 = 0x00FFFFFF;
    const static NGULong OC_C202 = 0x00FFFFFF;
    const static NGULong RT_C202 = 0x00FFFFFF;

// C203 (Indicator icon)
    const static NGULong ER_C203 = 0xFFFFFFFF;
    const static NGULong SL_C203 = 0xFFFFFFFF;
    const static NGULong OC_C203 = 0xFFFFFFFF;
    const static NGULong RT_C203 = 0xFFFFFFFF;

// C204 (Title font)
    const static NGULong ER_C204 = 0xFFFFFFFF;
    const static NGULong SL_C204 = 0xFF000000;
    const static NGULong OC_C204 = 0xFFFFFFFF;
    const static NGULong RT_C204 = MakeRGBA (229, 50, 2, 255);

// C205 (List icon (Number font))
    const static NGULong ER_C205 = 0xFFFFFFFF;
    const static NGULong SL_C205 = 0xFF000000;
    const static NGULong OC_C205 = 0xFFFFFFFF;
    const static NGULong RT_C205 = 0xFFFFFFFF;

// C206 (Focused list icon)
    const static NGULong ER_C206 = 0xFF000000;
    const static NGULong SL_C206 = 0xFFFFFFFF;
    const static NGULong OC_C206 = 0xFF000000;
    const static NGULong RT_C206 = 0xFF000000;

// C207 (List font)
    const static NGULong ER_C207 = 0xFFFFFFFF;
    const static NGULong SL_C207 = 0xFF000000;
    const static NGULong OC_C207 = 0xFFFFFFFF;
    const static NGULong RT_C207 = MakeRGBA (215, 215, 215, 255);

// C207D (Dimmed list font)
    const static NGULong ER_C207D = 0xFF000000;
    const static NGULong SL_C207D = 0xFFFFFFFF;
    const static NGULong OC_C207D = 0xFF000000;
    const static NGULong RT_C207D = MakeRGBA (255/3, 255/3, 255/3, 255);

// C208 (Focused list font)
    const static NGULong ER_C208 = 0xFF000000;
    const static NGULong SL_C208 = 0xFFFFFFFF;
    const static NGULong OC_C208 = 0xFF000000;
    const static NGULong RT_C208 = 0xFFFFFFFF;

// C209 (Soft key icon)
    const static NGULong ER_C209 = 0xFFFFFFFF;
    const static NGULong SL_C209 = 0xFFFFFFFF;
    const static NGULong OC_C209 = 0xFFFFFFFF;
    const static NGULong RT_C209 = 0xFFFFFFFF;

// C210 (Soft key font)
    const static NGULong ER_C210 = 0xFFFFFFFF;
    const static NGULong SL_C210 = 0xFFFFFFFF;
    const static NGULong OC_C210 = 0xFFFFFFFF;
    const static NGULong RT_C210 = 0xFFFFFFFF;

// C210B (Soft key BG)
    const static NGULong ER_C210B = 0xFF000000;
    const static NGULong SL_C210B = 0xFF000000;
    const static NGULong OC_C210B = 0xFF000000;
    const static NGULong RT_C210B = MakeRGBA (26, 26, 26, 255);

// C210B (Soft key pressed BG)
    const static NGULong ER_C210P = 0xFF000000;
    const static NGULong SL_C210P = 0xFF000000;
    const static NGULong OC_C210P = 0xFF000000;
    const static NGULong RT_C210P = MakeRGBA (255*15/100, 255*15/100, 255*15/100, 255);

// C211 (Animation)
    const static NGULong ER_C211 = 0xFFFFFFFF;
    const static NGULong SL_C211 = 0xFFFF8900;
    const static NGULong OC_C211 = 0xFFFFFFFF;
    const static NGULong RT_C211 = 0xFFFFFFFF;

// C212 (Pop-up focus BG)
    const static NGULong ER_C212 = 0xFF004500;
    const static NGULong SL_C212 = 0xFFFF7E00;
    const static NGULong OC_C212 = 0xFF512300;
    const static NGULong RT_C212 = MakeRGBA (229, 50, 2, 255);

// C213 (Pop-up list font)
    const static NGULong ER_C213 = 0xFF000000;
    const static NGULong SL_C213 = 0xFF000000;
    const static NGULong OC_C213 = 0xFF000000;
    const static NGULong RT_C213 = MakeRGBA (51, 51, 51, 255);

// C214 (Pop-up focused list font)
    const static NGULong ER_C214 = 0xFFFFFFFF;
    const static NGULong SL_C214 = 0xFFFFFFFF;
    const static NGULong OC_C214 = 0xFFFFFFFF;
    const static NGULong RT_C214 = 0xFFFFFFFF;

// C215 (Pop-up list icon)
    const static NGULong ER_C215 = 0xFF000000;
    const static NGULong SL_C215 = 0xFF000000;
    const static NGULong OC_C215 = 0xFF000000;
    const static NGULong RT_C215 = 0xFF000000;

// C216 (Pop-up focused list icon)
    const static NGULong ER_C216 = 0xFFFFFFFF;
    const static NGULong SL_C216 = 0xFFFFFFFF;
    const static NGULong OC_C216 = 0xFFFFFFFF;
    const static NGULong RT_C216 = 0xFFFFFFFF;

// C217 (Pop-up animation)
    const static NGULong ER_C217 = 0xFF004500;
    const static NGULong SL_C217 = 0xFFFF7E00;
    const static NGULong OC_C217 = 0xFF512300;
    const static NGULong RT_C217 = 0xFF512300;

// C218 (Pop-up Soft key font)
    const static NGULong ER_C218 = 0xFF000000;
    const static NGULong SL_C218 = 0xFF000000;
    const static NGULong OC_C218 = 0xFF000000;
    const static NGULong RT_C218 = 0xFFFFFFFF;

// C219 (Soft line)
    const static NGULong ER_C219 = 0x00FFFFFF;
    const static NGULong SL_C219 = 0x00FFFFFF;
    const static NGULong OC_C219 = 0x00FFFFFF;
    const static NGULong RT_C219 = 0x00FFFFFF;

// C220 (Pop-up soft line)
    const static NGULong ER_C220 = 0xFF000000;
    const static NGULong SL_C220 = 0xFF000000;
    const static NGULong OC_C220 = 0xFF000000;
    const static NGULong RT_C220 = 0xFF000000;

// C221 (Dialing font)
    const static NGULong ER_C221 = 0xFFFFFFFF;
    const static NGULong SL_C221 = 0xFFC34E00;
    const static NGULong OC_C221 = 0xFFFFFFFF;
    const static NGULong RT_C221 = 0xFFFFFFFF;

// C222 (Dialing BG)
    const static NGULong ER_C222 = 0xFF00851E;
    const static NGULong SL_C222 = 0xFFFFFFFF;
    const static NGULong OC_C222 = 0xFF8F3C00;
    const static NGULong RT_C222 = 0xFF000000;

// C223 (Scroll bar BG)
    const static NGULong ER_C223 = 0xFF00851E; //0x00FFFFFF;
    const static NGULong SL_C223 = 0xFFFFFFFF; //0x00FFFFFF;
    const static NGULong OC_C223 = 0xFF8F3C00; //0x00FFFFFF;
    const static NGULong RT_C223 = 0xFF000000;

// C224 (Pop-up Scroll bar BG)
    const static NGULong ER_C224 = 0xFFFFFFFF; //0x00FFFFFF;
    const static NGULong SL_C224 = 0xFFFFFFFF; //0x00FFFFFF;
    const static NGULong OC_C224 = 0xFFFFFFFF; //0x00FFFFFF;
    const static NGULong RT_C224 = 0xFFFFFFFF; //0x00FFFFFF;

// C225 (Pop-up Soft key icon)
    const static NGULong ER_C225 = 0x00FFFFFF;
    const static NGULong SL_C225 = 0x00FFFFFF;
    const static NGULong OC_C225 = 0x00FFFFFF;
    const static NGULong RT_C225 = 0x00FFFFFF;

// C226 (Scroll bar)
    const static NGULong ER_C226 = 0xB3FFFFFF; //C207 Opacity 70% //0xFF0052FF;
    const static NGULong SL_C226 = 0xB3000000; //C207 Opacity 70% //0xFF0052FF;
    const static NGULong OC_C226 = 0xB3FFFFFF; //C207 Opacity 70% //0xFF0052FF;
    const static NGULong RT_C226 = MakeRGBA (230, 230, 230, 255/3);

// C226 (Pop-up Scroll bar)
    const static NGULong ER_C227 = 0x7F000000; //C213 Opacity 50% //0xFF0052FF;
    const static NGULong SL_C227 = 0x7F000000; //C213 Opacity 50% //0xFF0052FF;
    const static NGULong OC_C227 = 0x7F000000; //C213 Opacity 50% //0xFF0052FF;
    const static NGULong RT_C227 = MakeRGBA (80, 80, 80, 255/3);

// C308 (Pop-up Title BG)
    const static NGULong ER_C308 = 0xFF00851E;
    const static NGULong SL_C308 = 0xFFFFFFFF;
    const static NGULong OC_C308 = 0xFF8F3C00;
    const static NGULong RT_C308 = 0xFF8F3C00;

// C309 (Pop-up Soft key BG)
    const static NGULong ER_C309 = 0xFFA0E9FF;
    const static NGULong SL_C309 = 0xFFC6C6C6;
    const static NGULong OC_C309 = 0xFFEEDEB7;
    const static NGULong RT_C309 = MakeRGBA (26, 26, 26, 255);

// C409 (Note/TipDlg BG)
    const static NGULong ER_C409 = 0xFFA0E9FF;
    const static NGULong SL_C409 = 0xFFC6C6C6;
    const static NGULong OC_C409 = 0xFFEEDEB7;
    const static NGULong RT_C409 = MakeRGBA (51, 51, 51, 255);

// C409 (Note/TipDlg font)
    const static NGULong ER_C410 = 0xFF101010;
    const static NGULong SL_C410 = 0xFF101010;
    const static NGULong OC_C410 = 0xFF101010;
    const static NGULong RT_C410 = 0xFFFFFFFF;

// (ALPHA BG)
    const static NGULong AER_C101 = 0x7F00851E;
    const static NGULong ASL_C101 = 0x7FFFFFFF;
    const static NGULong AOC_C101 = 0x7F8F3C00;
    const static NGULong ART_C101 = 0x7F8F3C00;

//===========Theme Color===========//

//=========Theme Old Color=========//
    const static NGULong EMERALD_HC  = 0xFF1CDAAB;
    const static NGULong SKYLINE_HC  = 0xFFBE9815;
    const static NGULong OCEAN_HC  = 0xFFDCDCDC;
    const static NGULong RETRO_HC  = MakeRGBA (229, 50, 2, 255);

    const static NGULong EMERALD_BG  = 0xFF183115;
    const static NGULong SKYLINE_BG  = 0xFFC3C0C0;
    const static NGULong OCEANBLUE_BG  = 0xFF5B1702;
    const static NGULong RETRO_BG  = 0xFF000000;

    const static NGULong EMERALD_FC  = 0xFF1C985B;   // 28,152,91
    const static NGULong SKYLINE_FC  = 0xFF985B15;   // 152,91,21
    const static NGULong OCEAN_FC  = 0xFF989898;    //124,124,124
    const static NGULong RETRO_FC  = MakeRGBA (215, 215, 215, 255);

    const static NGULong EMERALD_ONELINE  = 0xFFE7E7E7;   // 231,231,231
    const static NGULong SKYLINE_ONELINE  = 0xFFAAAAAA;   // 170,170,170
    const static NGULong OCEAN_ONELINE  = 0xFFF8F8F8;    //248,248,248
    const static NGULong RETRO_ONELINE  = 0xFFF8F8F8;
//=========Theme Old Color=========//

private:
    NGUInt8 m_r;
    NGUInt8 m_g;
    NGUInt8 m_b;
    NGUInt8 m_a;
};

NAMESPACE_END

#endif /*_NGUX_COLOR_H_*/
