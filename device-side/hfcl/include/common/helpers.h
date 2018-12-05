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

#ifndef HFCL_COMMON_HELPERS_H_
#define HFCL_COMMON_HELPERS_H_

#include <minigui/common.h>

namespace hfcl {

int GetFirstUTF8CharLen (const char *str, int len);
int GetLastUTF8CharLen (const char *str, int len);
int GetUTF8CharInfo (const char *mstr, int len, int *retPosChars);
int GetUTF8CharCount (const char *mstr, int len);
BOOL HasUCS2Char (const char *mstr, int len);
int GetUTF8LenByCharCount (const char *mstr, int charCount);
Uint16 UTF8ToUCS2 (Uint8 *utf8);
BOOL IsNumberUCS2Char (Uint16 inChar);
BOOL IsSymbolUCS2Char (Uint16 inChar);
BOOL IsArabicSymbolUCS2Char (Uint16 inChar);

} // namespace hfcl

#endif // HFCL_COMMON_HELPERS_H_

