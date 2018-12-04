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

#ifndef _HFCL_LANGUAGE_IDS_H_
#define _HFCL_LANGUAGE_IDS_H_

enum {
    /* 000 */ LANGID_AFRIKAANS, /* Afrikaans */
    /* 001 */ LANGID_ALBANIAN, /* Albanian */
    /* 002 */ LANGID_AMHARIC, /* Amharic */
    /* 003 */ LANGID_ARABIC, /* Arabic */
    /* 004 */ LANGID_ARMENIAN, /* Armenian */
    /* 005 */ LANGID_ASSAMESE, /* Assamese */
    /* 006 */ LANGID_AZERBAIJANI, /* Azerbaijani */
    /* 007 */ LANGID_BELARUSIAN, /* Belarusian */
    /* 008 */ LANGID_BENGALI_BD, /* Bengali (Bangladesh) */
    /* 009 */ LANGID_BENGALI_IN, /* Bengali (India) */
    /* 010 */ LANGID_BOSNIAN, /* Bosnian */
    /* 011 */ LANGID_BULGARIAN, /* Bulgarian */
    /* 012 */ LANGID_BURMESE, /* Burmese */
    /* 013 */ LANGID_CHINESE_CN, /* Chinese (China) */
    /* 014 */ LANGID_CHINESE_HK, /* Chinese (Hongkong) */
    /* 015 */ LANGID_CHINESE_TW, /* Chinese (Taiwan) */
    /* 016 */ LANGID_CROATIAN, /* Croatian */
    /* 017 */ LANGID_CZECH, /* Czech */
    /* 018 */ LANGID_DANISH, /* Danish */
    /* 019 */ LANGID_DUTCH, /* Dutch */
    /* 020 */ LANGID_ENGLISH_GB, /* English (United Kingdom) */
    /* 021 */ LANGID_ENGLISH_US, /* English (United States of America) */
    /* 022 */ LANGID_ESTONIAN, /* Estonian */
    /* 023 */ LANGID_FINNISH, /* Finnish */
    /* 024 */ LANGID_FRENCH_CA, /* French (Canada) */
    /* 025 */ LANGID_FRENCH_FR, /* French (France) */
    /* 026 */ LANGID_GEORGIAN, /* Georgian */
    /* 027 */ LANGID_GERMAN, /* German */
    /* 028 */ LANGID_GREEK, /* Greek */
    /* 029 */ LANGID_GUJARATI, /* Gujarati */
    /* 030 */ LANGID_HAUSA, /* Hausa */
    /* 031 */ LANGID_HEBREW, /* Hebrew */
    /* 032 */ LANGID_HINDI, /* Hindi */
    /* 033 */ LANGID_HUNGARIAN, /* Hungarian */
    /* 034 */ LANGID_ICELANDIC, /* Icelandic */
    /* 035 */ LANGID_IGBO, /* Igbo */
    /* 036 */ LANGID_INDONESIAN, /* Indonesian */
    /* 037 */ LANGID_ITALIAN, /* Italian */
    /* 038 */ LANGID_KANNADA, /* Kannada */
    /* 039 */ LANGID_KAZAKH, /* Kazakh */
    /* 040 */ LANGID_KHMER, /* Khmer */
    /* 041 */ LANGID_KIRGHIZ, /* Kirghiz */
    /* 042 */ LANGID_LAO, /* Lao */
    /* 043 */ LANGID_LATVIAN, /* Latvian */
    /* 044 */ LANGID_LINGALA, /* Lingala */
    /* 045 */ LANGID_LITHUANIAN, /* Lithuanian */
    /* 046 */ LANGID_MACEDONIAN, /* Macedonian */
    /* 047 */ LANGID_MALAGASY, /* Malagasy */
    /* 048 */ LANGID_MALAY, /* Malay */
    /* 049 */ LANGID_MALAYALAM, /* Malayalam */
    /* 050 */ LANGID_MARATHI, /* Marathi */
    /* 051 */ LANGID_MONGOLIAN, /* Mongolian */
    /* 052 */ LANGID_NORWEGIAN, /* Bokmål */
    /* 053 */ LANGID_ORIYA, /* Oriya */
    /* 054 */ LANGID_PERSIAN, /* Persian */
    /* 055 */ LANGID_POLISH, /* Polish */
    /* 056 */ LANGID_PORTUGUESE_BR, /* Portuguese (Brazil) */
    /* 057 */ LANGID_PORTUGUESE_PT, /* Portuguese (Portugal) */
    /* 058 */ LANGID_PUNJABI, /* Punjabi */
    /* 059 */ LANGID_PUSHTO, /* Pushto */
    /* 060 */ LANGID_ROMANIAN, /* Romanian */
    /* 061 */ LANGID_RUSSIAN, /* Russian */
    /* 062 */ LANGID_SERBIAN_LATN_CS, /* Serbian (Latin) */
    /* 063 */ LANGID_SESOTHO, /* Sesotho */
    /* 064 */ LANGID_SINHALA, /* Sinhala */
    /* 065 */ LANGID_SLOVAK, /* Slovak */
    /* 066 */ LANGID_SLOVENIAN, /* Slovenian */
    /* 067 */ LANGID_SPANISH_ES, /* Spanish (Spain) */
    /* 068 */ LANGID_SPANISH_US, /* Spanish (United States of America) */
    /* 069 */ LANGID_SWAHILI, /* Swahili */
    /* 070 */ LANGID_SWEDISH, /* Swedish */
    /* 071 */ LANGID_TAGALOG, /* Tagalog */
    /* 072 */ LANGID_TAJIK, /* Tajik */
    /* 073 */ LANGID_TAMIL, /* Tamil */
    /* 074 */ LANGID_TELUGU, /* Telugu */
    /* 075 */ LANGID_THAI, /* Thai */
    /* 076 */ LANGID_TURKISH, /* Turkish */
    /* 077 */ LANGID_TURKMEN_LATIN, /* Turkmen (Latin) */
    /* 078 */ LANGID_UKRAINIAN, /* Ukrainian */
    /* 079 */ LANGID_URDU, /* Urdu */
    /* 080 */ LANGID_UZBEK_CYRILLIC, /* Uzbek (Cyrillic) */
    /* 081 */ LANGID_VIETNAMESE, /* Vietnamese */
    /* 082 */ LANGID_XHOSA, /* Xhosa */
    /* 083 */ LANGID_YORUBA, /* Yoruba */
    /* 084 */ LANGID_ZULU, /* Zulu */
    MAX_LANGID, /* Not a real LANGID */
};

typedef struct _LANGUAGE_RAW_STRINGS {
    int            ime_lang_id;
    const char*    locale_name;
    const char*    self_name;
    const char**   raw_strings;
} LANGUAGE_RAW_STRINGS;

typedef struct _LANGUAGE_ZIPPED_STRINGS {
    int             ime_lang_id;
    const char*     locale_name;
    const char*     self_name;
    const char*     zipped_bytes;
    unsigned int    zipped_size;
    unsigned int    origin_size;
} LANGUAGE_ZIPPED_STRINGS;

#endif /* _HFCL_LANGUAGE_IDS_H_ */

