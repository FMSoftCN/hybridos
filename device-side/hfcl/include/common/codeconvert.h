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

#ifndef _CODECONVERT_H
#define _CODECONVERT_H

#include "common.h"
#include "gdi.h"

#if defined(__cplusplus)
extern "C" {
#endif

enum {
    UCS2MODE_NOCOMPRESS = 0x08,
    UCS2MODE_COMPRESS81 = 0x09,
    UCS2MODE_COMPRESS82 = 0x10
};

typedef struct UCS2_NOCOMPRESS_ST
{
    unsigned char id;
    unsigned char data;
}UCS2_NoCompress_st;

typedef struct UCS2_COMPRESS81_ST
{
    unsigned char id;
    unsigned char nb_characters;
    unsigned char base_pointer;
    unsigned char data;
}UCS2_Compress81_st;

typedef struct UCS2_COMPRESS82_ST
{
    unsigned char id;
    unsigned char nb_characters;
    unsigned char base_pointer[2];
    unsigned char data;
}UCS2_Compress82_st;

int NGUnicodeToUtf8(const unsigned char *_in, unsigned char *out);

int NGUtf8ToUnicode(const unsigned char *in, unsigned char *out);

int NGUnicodeStrlen( char * str);

unsigned short NGAnsiiToUnicodeString(char *pOutBuffer, char *pInBuffer );

unsigned short NGConvertUCS2ToUnicode(unsigned short * target, unsigned char * source, unsigned short tarlength, unsigned short srclength, unsigned char mode);

unsigned short NGConvertUnicodeToUCS2(unsigned char * target, unsigned short * source, unsigned short tarlength, unsigned short srclength, unsigned char * mode);

unsigned short NGUCS2Len(unsigned short * source, unsigned short maxlength);

unsigned short NGUCS2NBCharLen(unsigned short * source, unsigned short maxlength);

unsigned char NGFindUCS2Mode(unsigned short *str, unsigned short length, unsigned short *Page);

unsigned short NGIsGsmChar(unsigned short code);

void dump_utf8(unsigned char *utf8);

void dump_unicode(unsigned char *utf16);

/* VincentWei: the following functions are reviewed */
int NGUCS2Strlen (const unsigned short *a);
Glyph32* NGUCS2ConvertToGlyph32 (const unsigned short* ucs2Text, int* nrGlyphs, bool forceMBC = true);

#if defined(__cplusplus)
}
#endif

#endif //_NGUX_COMM_H
