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
#include "common/common.h"
#include "common/log.h"
#include "nguxobject.h"
#include "intrect.h"
#include "graphicscontext.h"
#include "color.h"

namespace hfcl {

// we assume color to be ABGR from high byte to low byte.
Color::Color(unsigned long l_color){
    m_a = l_color >> 24;
    m_b = (l_color & 0x00FF0000) >> 16;
    m_g = (l_color & 0x0000FF00) >> 8;
    m_r = (l_color & 0x000000FF);
}

Color::Color(unsigned char c_r, unsigned char c_g, unsigned char c_b, unsigned char c_a){
    m_r = c_r;
    m_g = c_g;
    m_b = c_b;
    m_a = c_a;
}



// END of namespace NGUX
} // namespace hfcl {
