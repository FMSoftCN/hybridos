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

#include <algorithm>
#include <string>
#include <cctype>

namespace hfcl {

inline std::string& ltrim(std::string& str,
        const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

inline std::string& rtrim(std::string& str,
        const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

inline std::string& trim(std::string& str,
        const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

inline std::string& tolower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
            static_cast<int(*)(int)>(std::tolower));
    return str;
}

inline std::string& toupper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
            static_cast<int(*)(int)>(std::toupper));
    return str;
}

int GetFirstUTF8CharLen (const char *str, int len);
int GetLastUTF8CharLen (const char *str, int len);
int GetUTF8CharInfo (const char *mstr, int len, int *retPosChars);
int GetUTF8CharCount (const char *mstr, int len);
bool HasUCS2Char (const char *mstr, int len);
int GetUTF8LenByCharCount (const char *mstr, int charCount);
Uint16 UTF8ToUCS2 (Uint8 *utf8);
bool IsNumberUCS2Char (Uint16 inChar);
bool IsSymbolUCS2Char (Uint16 inChar);
bool IsArabicSymbolUCS2Char (Uint16 inChar);
bool IsTransformArabicUCS2Char (Uint16 inChar);

} // namespace hfcl

#endif // HFCL_COMMON_HELPERS_H_

