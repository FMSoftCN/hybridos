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

#ifndef __FONTRES_H
#define __FONTRES_H

#include "graphics/font.h"

namespace hfcl {

class FontRes {
    public:
        FontRes();
        FontRes(const char *fontname);
        ~FontRes();

        /*
         * load real font
         */
        Logfont *get(/*Style* style*/);

    private:
        const char* m_fontname;
        Logfont* m_font;
};

} // namespace hfcl {

#endif /* __FONTRES_H */

