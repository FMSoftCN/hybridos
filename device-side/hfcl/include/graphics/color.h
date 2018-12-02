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
    Color(unsigned long color);
    Color(Uint8 r, unsigned char g, unsigned char b, unsigned char a);
    virtual ~Color(){}

    Uint8 r(){ return m_r; }
    Uint8 g(){ return m_g; }
    Uint8 b(){ return m_b; }
    Uint8 a(){ return m_a; }
    void setR(Uint8 ir){ m_r = ir; }
    void setG(Uint8 ig){ m_g = ig; }
    void setB(Uint8 ib){ m_b = ib; }
    void setA(Uint8 ia){ m_a = ia; }
    void setColor(unsigned long lcolor){
        m_a = GetAValue(lcolor);
        m_b = GetBValue(lcolor);
        m_g = GetGValue(lcolor);
        m_r = GetRValue(lcolor);
    }

    DWORD color(){ return MakeRGBA(m_r, m_g, m_b, m_a); }

    const static unsigned long BLACK = 0xFF000000;
    const static unsigned long WHITE = 0xFFFFFFFF;
    const static unsigned long MILKY = 0xFFEEEEEE;
    const static unsigned long GRAY  = 0xFF808080;
    const static unsigned long LIGHTGRAY  = 0xFFD3D3D3;
    const static unsigned long DARKGRAY  = 0xFF404040;
    const static unsigned long RED   = 0xFF0000FF;
    const static unsigned long GREEN = 0xFF00FF00;
    const static unsigned long BLUE  = 0xFFFF0000;
    const static unsigned long YELLOW  = 0xFF00FFFF;
    const static unsigned long LIGHTBROWN  = 0xFF30A0C0;
    const static unsigned long OCEAN  = 0xFF5B1702;
    const static unsigned long ALPHA_PANEL = MakeRGBA (0, 0, 0, 256*3/10);
    const static unsigned long TRANSPARENT= 0x00000000;
    const static unsigned long LAYER_COLOR_KEY = MakeRGBA (0, 0, 0, 0); //0xFFFE0000;
    const static unsigned long HOLD_BK = 0xFFFFC4D2;

//===========Theme Color===========//
// C101 (BG)
    const static unsigned long ER_C101 = 0xFF00851E;
    const static unsigned long SL_C101 = 0xFFFFFFFF;
    const static unsigned long OC_C101 = 0xFF8F3C00;
    const static unsigned long RT_C101 = 0xFF000000;

// C102 (List focus BG)
    const static unsigned long ER_C102 = 0xFFFFFFFF;
    const static unsigned long SL_C102 = 0xFFFF8900;
    const static unsigned long OC_C102 = 0xFFFFFFFF;
    const static unsigned long RT_C102 = MakeRGBA (229, 50, 2, 255);

// C103 (Indicator BG)
    const static unsigned long ER_C103 = 0x0000851E;
    const static unsigned long SL_C103 = 0x00FFFFFF;
    const static unsigned long OC_C103 = 0x008F3C00;
    const static unsigned long RT_C103 = 0xFF000000;

// C104 (Title BG)
    const static unsigned long ER_C104 = 0x0000851E;
    const static unsigned long SL_C104 = 0x00FFFFFF;
    const static unsigned long OC_C104 = 0x008F3C00;
    const static unsigned long RT_C104 = 0xFF000000;

// C105 (Soft key BG)
    const static unsigned long ER_C105 = 0xFF004E07;
    const static unsigned long SL_C105 = 0xFF666666;
    const static unsigned long OC_C105 = 0xFF6A0700;
    const static unsigned long RT_C105 = MakeRGBA (26, 26, 26, 255);

// C106 (List line (Title/Soft key))
    const static unsigned long ER_C106 = 0x7FFFFFFF;
    const static unsigned long SL_C106 = 0xFF9C9C9C;
    const static unsigned long OC_C106 = 0x7FFFFFFF;
    const static unsigned long RT_C106 = 0x7FFFFFFF;

// C107 (Pop-up BG)
    const static unsigned long ER_C107 = 0xFFFFFFFF;
    const static unsigned long SL_C107 = 0xFFFFFFFF;
    const static unsigned long OC_C107 = 0xFFFFFFFF;
    const static unsigned long RT_C107 = MakeRGBA (244, 244, 244, 255);

// C108 (Pop-up Progressbar)
    const static unsigned long ER_C108 = 0xFF00851E;
    const static unsigned long SL_C108 = 0xFFFF7E00;
    const static unsigned long OC_C108 = 0xFF8F3C00;
    const static unsigned long RT_C108 = 0xFF8F3C00;

// C201 (Unfocus Main menu icon)
    const static unsigned long ER_C201 = 0x00FFFFFF;
    const static unsigned long SL_C201 = 0x00FFFFFF;
    const static unsigned long OC_C201 = 0x00FFFFFF;
    const static unsigned long RT_C201 = 0x00FFFFFF;

// C202 (Focus Main menu icon)
    const static unsigned long ER_C202 = 0x00FFFFFF;
    const static unsigned long SL_C202 = 0x00FFFFFF;
    const static unsigned long OC_C202 = 0x00FFFFFF;
    const static unsigned long RT_C202 = 0x00FFFFFF;

// C203 (Indicator icon)
    const static unsigned long ER_C203 = 0xFFFFFFFF;
    const static unsigned long SL_C203 = 0xFFFFFFFF;
    const static unsigned long OC_C203 = 0xFFFFFFFF;
    const static unsigned long RT_C203 = 0xFFFFFFFF;

// C204 (Title font)
    const static unsigned long ER_C204 = 0xFFFFFFFF;
    const static unsigned long SL_C204 = 0xFF000000;
    const static unsigned long OC_C204 = 0xFFFFFFFF;
    const static unsigned long RT_C204 = MakeRGBA (229, 50, 2, 255);

// C205 (List icon (Number font))
    const static unsigned long ER_C205 = 0xFFFFFFFF;
    const static unsigned long SL_C205 = 0xFF000000;
    const static unsigned long OC_C205 = 0xFFFFFFFF;
    const static unsigned long RT_C205 = 0xFFFFFFFF;

// C206 (Focused list icon)
    const static unsigned long ER_C206 = 0xFF000000;
    const static unsigned long SL_C206 = 0xFFFFFFFF;
    const static unsigned long OC_C206 = 0xFF000000;
    const static unsigned long RT_C206 = 0xFF000000;

// C207 (List font)
    const static unsigned long ER_C207 = 0xFFFFFFFF;
    const static unsigned long SL_C207 = 0xFF000000;
    const static unsigned long OC_C207 = 0xFFFFFFFF;
    const static unsigned long RT_C207 = MakeRGBA (215, 215, 215, 255);

// C207D (Dimmed list font)
    const static unsigned long ER_C207D = 0xFF000000;
    const static unsigned long SL_C207D = 0xFFFFFFFF;
    const static unsigned long OC_C207D = 0xFF000000;
    const static unsigned long RT_C207D = MakeRGBA (255/3, 255/3, 255/3, 255);

// C208 (Focused list font)
    const static unsigned long ER_C208 = 0xFF000000;
    const static unsigned long SL_C208 = 0xFFFFFFFF;
    const static unsigned long OC_C208 = 0xFF000000;
    const static unsigned long RT_C208 = 0xFFFFFFFF;

// C209 (Soft key icon)
    const static unsigned long ER_C209 = 0xFFFFFFFF;
    const static unsigned long SL_C209 = 0xFFFFFFFF;
    const static unsigned long OC_C209 = 0xFFFFFFFF;
    const static unsigned long RT_C209 = 0xFFFFFFFF;

// C210 (Soft key font)
    const static unsigned long ER_C210 = 0xFFFFFFFF;
    const static unsigned long SL_C210 = 0xFFFFFFFF;
    const static unsigned long OC_C210 = 0xFFFFFFFF;
    const static unsigned long RT_C210 = 0xFFFFFFFF;

// C210B (Soft key BG)
    const static unsigned long ER_C210B = 0xFF000000;
    const static unsigned long SL_C210B = 0xFF000000;
    const static unsigned long OC_C210B = 0xFF000000;
    const static unsigned long RT_C210B = MakeRGBA (26, 26, 26, 255);

// C210B (Soft key pressed BG)
    const static unsigned long ER_C210P = 0xFF000000;
    const static unsigned long SL_C210P = 0xFF000000;
    const static unsigned long OC_C210P = 0xFF000000;
    const static unsigned long RT_C210P = MakeRGBA (255*15/100, 255*15/100, 255*15/100, 255);

// C211 (Animation)
    const static unsigned long ER_C211 = 0xFFFFFFFF;
    const static unsigned long SL_C211 = 0xFFFF8900;
    const static unsigned long OC_C211 = 0xFFFFFFFF;
    const static unsigned long RT_C211 = 0xFFFFFFFF;

// C212 (Pop-up focus BG)
    const static unsigned long ER_C212 = 0xFF004500;
    const static unsigned long SL_C212 = 0xFFFF7E00;
    const static unsigned long OC_C212 = 0xFF512300;
    const static unsigned long RT_C212 = MakeRGBA (229, 50, 2, 255);

// C213 (Pop-up list font)
    const static unsigned long ER_C213 = 0xFF000000;
    const static unsigned long SL_C213 = 0xFF000000;
    const static unsigned long OC_C213 = 0xFF000000;
    const static unsigned long RT_C213 = MakeRGBA (51, 51, 51, 255);

// C214 (Pop-up focused list font)
    const static unsigned long ER_C214 = 0xFFFFFFFF;
    const static unsigned long SL_C214 = 0xFFFFFFFF;
    const static unsigned long OC_C214 = 0xFFFFFFFF;
    const static unsigned long RT_C214 = 0xFFFFFFFF;

// C215 (Pop-up list icon)
    const static unsigned long ER_C215 = 0xFF000000;
    const static unsigned long SL_C215 = 0xFF000000;
    const static unsigned long OC_C215 = 0xFF000000;
    const static unsigned long RT_C215 = 0xFF000000;

// C216 (Pop-up focused list icon)
    const static unsigned long ER_C216 = 0xFFFFFFFF;
    const static unsigned long SL_C216 = 0xFFFFFFFF;
    const static unsigned long OC_C216 = 0xFFFFFFFF;
    const static unsigned long RT_C216 = 0xFFFFFFFF;

// C217 (Pop-up animation)
    const static unsigned long ER_C217 = 0xFF004500;
    const static unsigned long SL_C217 = 0xFFFF7E00;
    const static unsigned long OC_C217 = 0xFF512300;
    const static unsigned long RT_C217 = 0xFF512300;

// C218 (Pop-up Soft key font)
    const static unsigned long ER_C218 = 0xFF000000;
    const static unsigned long SL_C218 = 0xFF000000;
    const static unsigned long OC_C218 = 0xFF000000;
    const static unsigned long RT_C218 = 0xFFFFFFFF;

// C219 (Soft line)
    const static unsigned long ER_C219 = 0x00FFFFFF;
    const static unsigned long SL_C219 = 0x00FFFFFF;
    const static unsigned long OC_C219 = 0x00FFFFFF;
    const static unsigned long RT_C219 = 0x00FFFFFF;

// C220 (Pop-up soft line)
    const static unsigned long ER_C220 = 0xFF000000;
    const static unsigned long SL_C220 = 0xFF000000;
    const static unsigned long OC_C220 = 0xFF000000;
    const static unsigned long RT_C220 = 0xFF000000;

// C221 (Dialing font)
    const static unsigned long ER_C221 = 0xFFFFFFFF;
    const static unsigned long SL_C221 = 0xFFC34E00;
    const static unsigned long OC_C221 = 0xFFFFFFFF;
    const static unsigned long RT_C221 = 0xFFFFFFFF;

// C222 (Dialing BG)
    const static unsigned long ER_C222 = 0xFF00851E;
    const static unsigned long SL_C222 = 0xFFFFFFFF;
    const static unsigned long OC_C222 = 0xFF8F3C00;
    const static unsigned long RT_C222 = 0xFF000000;

// C223 (Scroll bar BG)
    const static unsigned long ER_C223 = 0xFF00851E; //0x00FFFFFF;
    const static unsigned long SL_C223 = 0xFFFFFFFF; //0x00FFFFFF;
    const static unsigned long OC_C223 = 0xFF8F3C00; //0x00FFFFFF;
    const static unsigned long RT_C223 = 0xFF000000;

// C224 (Pop-up Scroll bar BG)
    const static unsigned long ER_C224 = 0xFFFFFFFF; //0x00FFFFFF;
    const static unsigned long SL_C224 = 0xFFFFFFFF; //0x00FFFFFF;
    const static unsigned long OC_C224 = 0xFFFFFFFF; //0x00FFFFFF;
    const static unsigned long RT_C224 = 0xFFFFFFFF; //0x00FFFFFF;

// C225 (Pop-up Soft key icon)
    const static unsigned long ER_C225 = 0x00FFFFFF;
    const static unsigned long SL_C225 = 0x00FFFFFF;
    const static unsigned long OC_C225 = 0x00FFFFFF;
    const static unsigned long RT_C225 = 0x00FFFFFF;

// C226 (Scroll bar)
    const static unsigned long ER_C226 = 0xB3FFFFFF; //C207 Opacity 70% //0xFF0052FF;
    const static unsigned long SL_C226 = 0xB3000000; //C207 Opacity 70% //0xFF0052FF;
    const static unsigned long OC_C226 = 0xB3FFFFFF; //C207 Opacity 70% //0xFF0052FF;
    const static unsigned long RT_C226 = MakeRGBA (230, 230, 230, 255/3);

// C226 (Pop-up Scroll bar)
    const static unsigned long ER_C227 = 0x7F000000; //C213 Opacity 50% //0xFF0052FF;
    const static unsigned long SL_C227 = 0x7F000000; //C213 Opacity 50% //0xFF0052FF;
    const static unsigned long OC_C227 = 0x7F000000; //C213 Opacity 50% //0xFF0052FF;
    const static unsigned long RT_C227 = MakeRGBA (80, 80, 80, 255/3);

// C308 (Pop-up Title BG)
    const static unsigned long ER_C308 = 0xFF00851E;
    const static unsigned long SL_C308 = 0xFFFFFFFF;
    const static unsigned long OC_C308 = 0xFF8F3C00;
    const static unsigned long RT_C308 = 0xFF8F3C00;

// C309 (Pop-up Soft key BG)
    const static unsigned long ER_C309 = 0xFFA0E9FF;
    const static unsigned long SL_C309 = 0xFFC6C6C6;
    const static unsigned long OC_C309 = 0xFFEEDEB7;
    const static unsigned long RT_C309 = MakeRGBA (26, 26, 26, 255);

// C409 (Note/TipDlg BG)
    const static unsigned long ER_C409 = 0xFFA0E9FF;
    const static unsigned long SL_C409 = 0xFFC6C6C6;
    const static unsigned long OC_C409 = 0xFFEEDEB7;
    const static unsigned long RT_C409 = MakeRGBA (51, 51, 51, 255);

// C409 (Note/TipDlg font)
    const static unsigned long ER_C410 = 0xFF101010;
    const static unsigned long SL_C410 = 0xFF101010;
    const static unsigned long OC_C410 = 0xFF101010;
    const static unsigned long RT_C410 = 0xFFFFFFFF;

// (ALPHA BG)
    const static unsigned long AER_C101 = 0x7F00851E;
    const static unsigned long ASL_C101 = 0x7FFFFFFF;
    const static unsigned long AOC_C101 = 0x7F8F3C00;
    const static unsigned long ART_C101 = 0x7F8F3C00;

//===========Theme Color===========//

//=========Theme Old Color=========//
    const static unsigned long EMERALD_HC  = 0xFF1CDAAB;
    const static unsigned long SKYLINE_HC  = 0xFFBE9815;
    const static unsigned long OCEAN_HC  = 0xFFDCDCDC;
    const static unsigned long RETRO_HC  = MakeRGBA (229, 50, 2, 255);

    const static unsigned long EMERALD_BG  = 0xFF183115;
    const static unsigned long SKYLINE_BG  = 0xFFC3C0C0;
    const static unsigned long OCEANBLUE_BG  = 0xFF5B1702;
    const static unsigned long RETRO_BG  = 0xFF000000;

    const static unsigned long EMERALD_FC  = 0xFF1C985B;   // 28,152,91
    const static unsigned long SKYLINE_FC  = 0xFF985B15;   // 152,91,21
    const static unsigned long OCEAN_FC  = 0xFF989898;    //124,124,124
    const static unsigned long RETRO_FC  = MakeRGBA (215, 215, 215, 255);

    const static unsigned long EMERALD_ONELINE  = 0xFFE7E7E7;   // 231,231,231
    const static unsigned long SKYLINE_ONELINE  = 0xFFAAAAAA;   // 170,170,170
    const static unsigned long OCEAN_ONELINE  = 0xFFF8F8F8;    //248,248,248
    const static unsigned long RETRO_ONELINE  = 0xFFF8F8F8;
//=========Theme Old Color=========//

private:
    Uint8 m_r;
    Uint8 m_g;
    Uint8 m_b;
    Uint8 m_a;
};

NAMESPACE_END

#endif /*_NGUX_COLOR_H_*/
