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


#ifndef _HFCL_TEXTMODE_h
#define _HFCL_TEXTMODE_h

#include "common/common.h"
#include "drawable-id.h"

namespace hfcl {

class TextMode{
	public:
		enum {
		 AlignLeft   =   0x00000000,
		 AlignCenter =   0X00000001,
		 AlignRight  =   0X00000002,
		 ValignTop   =   0x00000000,
		 ValignMiddle=   0X00000004,
		 ValignBottom=   0X00000008,
		 WordBreak   =   0x00000010,
		 SingleLine  =   0x00000020,
		 ExpandTabs  =   0x00000040,
		 TabStop     =   0x00000080,   
		 NoClip      =   0x00000100,   
		 ExternallPading=0x00000200,
		 CalcRect    =   0x00000400,
		 NoPrefix    =   0x00000800,
		 Internal    =   0x00001000,
		 CharBreak   =   0x00002000,
		 OutLine     =   0x00004000,
		 // the use defined
		 SysMask    =   0x0000FFFF,
		 TextOutOmitted = 0x0010000,
		};

		static unsigned int setAlign(unsigned int format, int align) {
			return (format & ~3) | (align & 3);
		}
		static int getAlign(unsigned int format) {
			return format & 3;
		}

		static unsigned int setVAlign(unsigned int format, int valign) {
			return (format & ~12) | (valign & 3) << 2;
		}
		static int getVAlign(unsigned int format) {
			return (format >> 3) & 3;
		}
		static unsigned int setBreak(unsigned int format, int brk) {
			format &= ~(WordBreak|CharBreak|SingleLine);
			switch(brk) {
			case TEXTBREAK_WORD: return format|WordBreak;
			case TEXTBREAK_CHAR: return format|CharBreak;
			default:
				return format|SingleLine;
			}
		}
		static int getBreak(unsigned int brk) {
			switch(brk & (WordBreak|CharBreak|SingleLine)) {
			case WordBreak: return TEXTBREAK_WORD;
			case CharBreak: return TEXTBREAK_CHAR;
			default: return TEXTBREAK_SINGLE;
			}
		}

		static int getTextOutMode(unsigned int brk) {
			return brk & TextOutOmitted ? TEXTOUT_OMITTED : TEXTOUT_NORMAL;
		}

		static unsigned int setTextOutMode(unsigned int format, int txtmod) {
			return (txtmod == TEXTOUT_OMITTED || txtmod & TextOutOmitted)? 
				format | TextOutOmitted : format & (~TextOutOmitted);
		}

        static unsigned int setTextOutLineMode(unsigned int format, int mode)
        {
            return mode?(format | OutLine):(format & ~OutLine);
        }

        static unsigned int getTextOutLineMode(unsigned int format)
        {
            return (format & OutLine);
        }        
};


} // namespace hfcl {

#endif /* HFCL_TextMode_h */
