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


#ifndef _NGUX_TEXTMODE_h
#define _NGUX_TEXTMODE_h

#include "nguxcommon.h"
#include "drawable-id.h"

NAMESPACE_BEGIN

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

		static NGUInt setAlign(NGUInt format, NGInt align) {
			return (format & ~3) | (align & 3);
		}
		static NGInt getAlign(NGUInt format) {
			return format & 3;
		}

		static NGUInt setVAlign(NGUInt format, NGInt valign) {
			return (format & ~12) | (valign & 3) << 2;
		}
		static NGInt getVAlign(NGUInt format) {
			return (format >> 3) & 3;
		}
		static NGUInt setBreak(NGUInt format, NGInt brk) {
			format &= ~(WordBreak|CharBreak|SingleLine);
			switch(brk) {
			case TEXTBREAK_WORD: return format|WordBreak;
			case TEXTBREAK_CHAR: return format|CharBreak;
			default:
				return format|SingleLine;
			}
		}
		static NGInt getBreak(NGUInt brk) {
			switch(brk & (WordBreak|CharBreak|SingleLine)) {
			case WordBreak: return TEXTBREAK_WORD;
			case CharBreak: return TEXTBREAK_CHAR;
			default: return TEXTBREAK_SINGLE;
			}
		}

		static NGInt getTextOutMode(NGUInt brk) {
			return brk & TextOutOmitted ? TEXTOUT_OMITTED : TEXTOUT_NORMAL;
		}

		static NGUInt setTextOutMode(NGUInt format, NGInt txtmod) {
			return (txtmod == TEXTOUT_OMITTED || txtmod & TextOutOmitted)? 
				format | TextOutOmitted : format & (~TextOutOmitted);
		}

        static NGUInt setTextOutLineMode(NGUInt format, NGInt mode)
        {
            return mode?(format | OutLine):(format & ~OutLine);
        }

        static NGUInt getTextOutLineMode(NGUInt format)
        {
            return (format & OutLine);
        }        
};


NAMESPACE_END

#endif /* NGUX_TextMode_h */
