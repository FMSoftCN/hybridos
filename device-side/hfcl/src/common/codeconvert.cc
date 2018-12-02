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

#include "ngux.h"
#include "nguxcommon.h"
#include "codeconvert.h"

#define NOT_CONVERTIBLE 0xFFFF

#ifndef _MIN_
#define _MIN_(x,y)    (((x)<(y))?(x):(y))
#endif
#ifndef _MAX_
#define _MAX_(x,y)    (((x)>(y))?(x):(y))
#endif

//GSM 7bit Default Alphabet Table
const unsigned short ui_GsmToUnicodeTab[128] = 
{ 
//	   0	  1		  2		  3		  4		  5		  6		  7		  8		  9		  A		  B		  C		  D		  E		  F	
//	0x0040, 0x00A3, 0x0024, 0x00A5, 0x00E8, 0x00E9, 0x00F9, 0x00EC, 0x00F2, 0x00E7, 0x000A, 0x00D8, 0x00F8, 0x000D, 0x00C5, 0x00E5,	//0x  godevil_040827
	0x0040, 0x00A3, 0x0024, 0x00A5, 0x00E8, 0x00E9, 0x00F9, 0x00EC, 0x00F2, 0x00C7, 0x000A, 0x00D8, 0x00F8, 0x000D, 0x00C5, 0x00E5,	//0x
	0x0394, 0x005F, 0x03A6, 0x0393, 0x039B, 0x03A9, 0x03A0, 0x03A8, 0x03A3, 0x0398, 0x039E, 0x0020, 0x00C6, 0x00E6, 0x00DF, 0x00C9,	//1x
	0x0020, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,	//2x
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,	//3x
	0x00A1, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,	//4x
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00C4, 0x00D6, 0x00D1, 0x00DC, 0x00A7,	//5x
	0x00BF, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,	//6x
	0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00E4, 0x00F6, 0x00F1, 0x00FC, 0x00E0	//7x
};

//GSM 7Bit default alphabet extention table
const unsigned short ui_EXT_GsmToUnicodeTab[128] = 
{
//	   0	  1		  2		  3		  4		  5		  6		  7		  8		  9		  A		  B		  C		  D		  E		  F	
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	//0x
	0x0000, 0x0000, 0x0000, 0x0000, 0x005E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000,	//1x
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007B, 0x007D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x005C,	//2x
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x005B, 0x007E, 0x005D, 0x0000,	//3x
	0x007C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	//4x
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	//5x
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20AC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	//6x
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,	//7x
};

const unsigned char ui_UnicodeToGsmTab[256] = 
{ 
//   0      1    2     3      4    5     6     7     8     9     A     B     C     D     E     F        
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0A, 0x20, 0x20, 0x0D, 0x20, 0x20, //0x
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //1x
    0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, //2x
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, //3x
    0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, //4x
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0xBC, 0xAF, 0xBE, 0x94, 0x11, //5x
    0x8A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, //6x
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0xA8, 0xC0, 0xA9, 0xBD, 0x8A, //7x

    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //8x
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //9x
    0x20, 0x40, 0x8A, 0x01, 0x24, 0x03, 0x8A, 0x5F, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, //Ax
    0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x60, //Bx
    0x8A, 0x8A, 0x8A, 0x8A, 0x5B, 0x0E, 0x1C, 0x09, 0x8A, 0x1F, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, //Cx
    0x8A, 0x5D, 0x8A, 0x8A, 0x8A, 0x8A, 0x5C, 0x8A, 0x0B, 0x8A, 0x8A, 0x8A, 0x5E, 0x8A, 0x8A, 0x1E, //Dx
//  0x7F, 0x8A, 0x8A, 0x8A, 0x7B, 0x0F, 0x1D, 0x8A, 0x04, 0x05, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, //Ex
    0x7F, 0x8A, 0x8A, 0x8A, 0x7B, 0x0F, 0x1D, 0x8A, 0x04, 0x05, 0x8A, 0x8A, 0x07, 0x8A, 0x8A, 0x8A, //Ex    // baek 050418 : ONS i grave problem
    0x8A, 0x7D, 0x08, 0x8A, 0x8A, 0x8A, 0x7C, 0x8A, 0x8A, 0x06, 0x8A, 0x8A, 0x7E, 0x8A, 0x8A, 0x8A  //Fx
};

unsigned char NGUnicodeToUCS2Encoding(unsigned short unicode,unsigned char  *charLength,unsigned char  *arrOut)
{
	unsigned char  status =0;
	unsigned char  index = 0;

	if(arrOut != 0)
	{
		unicode = unicode & 0xff;
		if( unicode  < 256 )
		{
			if(unicode == 0x80)
			{
				arrOut[index++] = 0xac ;//*((U8*)(&unicode));
				arrOut[index] = 0x20;
			}
			else
			{
				arrOut[index++] = *((unsigned char *)(&unicode));
				arrOut[index] = 0;
			}
		}
		else
		{
			arrOut[index++] = *((unsigned char *)(&unicode));
			arrOut[index] =   *(((unsigned char *)(&unicode)) + 1);
		}
		*charLength = 2;
	}	

	return status;
}

unsigned short NGIsGsmChar(unsigned short code)
{
    unsigned char StrOut;
    unsigned short ret = FALSE;

    if ( code == 0 ) return FALSE;

    if ( code <= 0x00FF )
    {
        StrOut = (unsigned char)ui_UnicodeToGsmTab[code];
        if ( StrOut > 0x7F )
        {
#if 0
            if ( StrOut != 0x8A )
            {
                ret = 0x1B00 + (StrOut-0x80);
            }
            else
#endif
                ret = NOT_CONVERTIBLE;
        }
        else ret = StrOut;
    }
    else 
    {
        switch ( code ) {
            case 0x0394: ret = 0x10; break;
            case 0x03A6: ret = 0x12; break;
            case 0x0393: ret = 0x13; break;
            case 0x039B: ret = 0x14; break;
            case 0x03A9: ret = 0x15; break;
            case 0x03A0: ret = 0x16; break;
            case 0x03A8: ret = 0x17; break;
            case 0x03A3: ret = 0x18; break;
            case 0x0398: ret = 0x19; break;
            case 0x039E: ret = 0x1A; break;

#if 0
            case 0x20AC: 
                ret = 0x1B65;
                break;
#endif
            default:
                ret = NOT_CONVERTIBLE;
                break;
        }
    }

    return ret;
}

unsigned char NGFindUCS2Mode(unsigned short *str, unsigned short length, unsigned short *Page)
{
    unsigned short srclen = 0, i = 0;
    unsigned short FirstUCS = 0,SecondUCS = 0;
    unsigned char mode = 0;

    if ( str == NULL )
        return (unsigned char)NOT_CONVERTIBLE;
    
    srclen = NGUCS2Strlen(str);
    
    if ( length == 0 )
        length = srclen;

    if ( !srclen )
        return (unsigned char)NOT_CONVERTIBLE;

    for ( i=0; i<srclen && i < length; i++ )
    {
        if ( NGIsGsmChar(str[i]) == NOT_CONVERTIBLE )
        {
            if ( FirstUCS )
            {
                FirstUCS = _MIN_(FirstUCS, str[i]);
                SecondUCS = _MAX_(str[i], SecondUCS);

                if ( (SecondUCS & 0xFF80) == (FirstUCS & 0xFF80) )
                {
                    *Page = FirstUCS;
                    mode =  UCS2MODE_COMPRESS81;
                }
                else 
                {
                    if ( (SecondUCS - FirstUCS) < 0x80 )
                    {
                        mode =  UCS2MODE_COMPRESS82;
                        *Page = FirstUCS;
                    }
                    else
                    {
                        mode = UCS2MODE_NOCOMPRESS;
                        *Page = 0;
                        break;
                    }
                }
            }
            else
            {
                FirstUCS = SecondUCS = str[i];
                mode =  UCS2MODE_COMPRESS81;
                *Page = FirstUCS;
            }
        }
    }

    return mode;
}

unsigned short NGUCS2Len(unsigned short * source, unsigned short maxlength)
{
    unsigned short srclen=0, i=0, length=0;
    unsigned short temp;
    unsigned short basePointer = 0;
    unsigned char mode = 0;

    if ( source == NULL )
        return length;
    
    srclen = NGUCS2Strlen(source);

    mode = NGFindUCS2Mode(source, srclen, &basePointer);

    switch ( mode ) {
        case UCS2MODE_NOCOMPRESS:
            {
                if ( maxlength % 2 == 0 )
                    length = (srclen + 1) * 2;
                else
                    length = (srclen * 2) + 1;
            }
            break;
            
        case UCS2MODE_COMPRESS81:
            {
                for ( i = 0; i < srclen && source[i] ; i++ )
                {
                    temp = NGIsGsmChar(source[i]);

                    if ( temp > 0x7F && temp != NOT_CONVERTIBLE )
                        length += 2;
                    else
                        length++;
                }

                length += 3;
            }
            break;
        
        case UCS2MODE_COMPRESS82:
            {
                for ( i = 0; i < srclen && source[i] ; i++ )
                {
                    temp = NGIsGsmChar(source[i]);

                    if ( temp > 0x7F && temp != NOT_CONVERTIBLE )
                        length += 2;
                    else
                        length++;
                }

                length += 4;
            }
            break;

        default:
            length = srclen;
            break;
    }

    return length;
}

unsigned short NGUCS2NBCharLen(unsigned short * source, unsigned short maxlength)
{
    unsigned short srclen=0, i=0, length=0;
    unsigned short temp;
    unsigned short basePointer = 0;
    unsigned char mode = 0;

    if ( source == NULL )
        return length;
    
    srclen = NGUCS2Strlen(source);

    mode = NGFindUCS2Mode(source, srclen, &basePointer);

    switch ( mode ) {
        case UCS2MODE_NOCOMPRESS:
            {
                if ( maxlength % 2 == 0 )
                {
                    if ( srclen < ((maxlength - 2)  / 2))  
                        length = srclen;
                    else
                        length = (maxlength - 2)  / 2;
                }
                else
                {
                    if ( srclen < ((maxlength  / 2) - 1))  
                        length = srclen;
                    else
                        length = (maxlength  / 2) - 1;
                }
            }
            break;
            
        case UCS2MODE_COMPRESS81:
            {
                unsigned short nbchar = 0;
                
                length += 3;
                
                for ( i = 0; i < srclen && i < maxlength && source[i] ; i++ )
                {
                    if ( length >= maxlength )
                    {
                        length = nbchar;
                        break;
                    }
                                    
                    temp = NGIsGsmChar(source[i]);

                    if ( temp > 0x7F && temp != NOT_CONVERTIBLE )
                        length += 2;
                    else
                        length++;

                    nbchar++;
                }
            }
            break;
        
        case UCS2MODE_COMPRESS82:
            {  
                unsigned short nbchar = 0;
                
                length += 4;
                
                for ( i = 0; i < srclen && i < maxlength && source[i] ; i++ )
                {
                    if ( length >= maxlength )
                    {
                        length = nbchar;
                        break;
                    }
                        
                    temp = NGIsGsmChar(source[i]);

                    if ( temp > 0x7F && temp != NOT_CONVERTIBLE )
                        length += 2;
                    else
                        length++;

                    nbchar++;
                }
            }
            break;

        default:
            {
                if ( srclen < maxlength )  
                    length = srclen;
                else
                    length = maxlength; 
            }
            break;
    }

    return length;
}

unsigned short NGConvertUnicodeToUCS2(unsigned char * target, unsigned short * source, unsigned short tarlength, unsigned short srclength, unsigned char * mode)
{
    unsigned short i=0, j=0, length=0;
    unsigned short temp;
    unsigned short basePointer = 0;

    *mode = NGFindUCS2Mode(source, srclength, &basePointer);

    switch ( *mode ) {
        case UCS2MODE_NOCOMPRESS:
        {
            UCS2_NoCompress_st * ucs2;
            ucs2 = (UCS2_NoCompress_st *)target;
            ucs2->id = 0x80;
            tarlength--;

            for (i = 0; length < tarlength-1 && i < srclength && source[i]; i++)
            {
                *(&ucs2->data+i*2) = (source[i]&0xFF00)>>8; 
                *(&ucs2->data+i*2+1) = (source[i]&0x00FF);  
            }
            length = i*2+(unsigned char *)&ucs2->data-(unsigned char *)ucs2; 
        }
        break;

        case UCS2MODE_COMPRESS81:
        {
            UCS2_Compress81_st * ucs2;
            ucs2 = (UCS2_Compress81_st *)target;

            memset((void *)ucs2, 0x00, (unsigned short)sizeof(UCS2_Compress81_st));
            ucs2->id = 0x81;
            tarlength -= 3;

            basePointer = (basePointer & 0x7F80);
            ucs2->base_pointer = (unsigned char)((basePointer)>>7);

            for ( i = 0, j = 0; j < srclength && i < tarlength && source[j] ; i++,j++ )
            {
                temp = NGIsGsmChar(source[j]);

                if ( temp == NOT_CONVERTIBLE )
                {
                    *(&ucs2->data+i) = (unsigned char)((source[j] - basePointer) + 0x80);
                }
                else
                {
                    if ( temp > 0x7F )
                    {
                        *(&ucs2->data+i++) = 0x1B;
                        *(&ucs2->data+i) = (unsigned char)(temp & 0x007F);
                    }
                    else
                    {
                        *(&ucs2->data+i) = (unsigned char)temp;
                    }
                }
            }
            ucs2->nb_characters = (unsigned char)i;

            length = i + (unsigned char *)&ucs2->data-(unsigned char *)ucs2;
        }
        break;

        case UCS2MODE_COMPRESS82:
        {
            UCS2_Compress82_st * ucs2;
            ucs2 = (UCS2_Compress82_st *)target;

            memset((void *)ucs2, 0x00, sizeof(UCS2_Compress82_st));
            ucs2->id = 0x82;

            ucs2->base_pointer[0] = (unsigned char)((basePointer & 0xFF00)>>8);
            ucs2->base_pointer[1] = (unsigned char)((basePointer & 0x00FF));   

            for ( i = 0, j=0; j < srclength && i < tarlength && source[j] ; i++, j++ )
            {
                temp = NGIsGsmChar(source[j]);
                if ( temp == NOT_CONVERTIBLE )
                {
                    *(&ucs2->data+i) = (unsigned char)((source[j]-basePointer) + 0x80);
                }
                else
                {
                    if ( temp > 0x7F )
                    {
                        *(&ucs2->data+i++) = 0x1B;
                        *(&ucs2->data+i) = (unsigned char)(temp & 0x007F);
                    }
                    else
                    {
                        *(&ucs2->data+i) = (unsigned char)temp;
                    }
                }
            }

            ucs2->nb_characters = (unsigned char)i;
            length = i + (unsigned char *)&ucs2->data-(unsigned char *)ucs2;
        }
        break;

    }

    return length;
}

unsigned short NGConvertUCS2ToUnicode(unsigned short * target, unsigned char * source, unsigned short tarlength, unsigned short srclength, unsigned char mode)
{
    unsigned short i,j;
    unsigned short base_pointer, length=0;
    unsigned char * data;

    // 80-> the first octet in the alpha string is '80'
    // 81-> the first octet of the alpha string is set to '81'
    // 82-> the first octet of the alpha string is set to '82'

    switch ( mode ) {
        case UCS2MODE_NOCOMPRESS:  // 0  '80'
            {
                UCS2_NoCompress_st * ucs2;
                ucs2 = (UCS2_NoCompress_st *)source;

                data = (unsigned char *)&(ucs2->data);

                srclength = (srclength - ((unsigned char *)data - (unsigned char *)ucs2))/2;  // only string length 

                for ( i = 0; i < srclength && i < tarlength; i++ )
                {
                    target[i] = ((unsigned short)*(data+i*2) << 8) + (unsigned short)*(data+i*2+1);
                    if (target[i] == 0xFFFF) break;
                }
                length = i;
            }
            break;

        case UCS2MODE_COMPRESS81: // 1  '81'
            {
                UCS2_Compress81_st * ucs2;
                ucs2 = (UCS2_Compress81_st *)source;
                data = (unsigned char *)&(ucs2->data);
                srclength = ucs2->nb_characters;
                base_pointer = (unsigned short)ucs2->base_pointer;
                base_pointer <<= 7;

                for (i = 0, j=0; j < srclength && i < tarlength; i++, j++)
                {
                    if (*(data+j) > 0x7F)
                    {
                        target[i] = (unsigned short)((unsigned short)(*(data+j)&0x7F) + base_pointer);
                    }
                    else
                    {
                        if((unsigned char)*(data+j) != 0x1B)
                        {
                            target[i] = (unsigned short) ui_GsmToUnicodeTab[(unsigned char)*(data+j)];
                        }
                        else
                        {
                            target[i] = (unsigned short) ui_EXT_GsmToUnicodeTab[(unsigned char)*(data+(++j))];
                        }
                    }
                }
                length = i;
            }
            break;


        case UCS2MODE_COMPRESS82: // 2  '82'
            {
                UCS2_Compress82_st * ucs2;
                ucs2 = (UCS2_Compress82_st *)source;
                data = (unsigned char *)&(ucs2->data);
                srclength = ucs2->nb_characters;
                base_pointer = (unsigned short)(((unsigned short)ucs2->base_pointer[0] << 8) + (unsigned short)(ucs2->base_pointer[1]));

                for (i = 0, j = 0; j < srclength && i < tarlength; i++, j++)
                {
                    if (*(data+j) > 0x7F)
                    {
                        target[i] = (unsigned short)((*(data+j)-0x80) + base_pointer);
                    }
                    else
                    {
                        if((unsigned char)*(data+j) != 0x1B)
                        {
                            target[i] = (unsigned short) ui_GsmToUnicodeTab[(unsigned char)*(data+j)];
                        }
                        else
                        {
                            target[i] = (unsigned short) ui_EXT_GsmToUnicodeTab[(unsigned char)*(data+(++j))];
                        }
                    }
                }
                length = i;
            }
            break;
    }

    if ( length < tarlength )
    {
        target[length] = 0;
    }
    
    return length;
} /*--- End of gapi_GetUnicodefromUCS2() ---*/

unsigned short  NGAnsiiToUnicodeString(char *pOutBuffer, char *pInBuffer )
{
	short count = -1;
	unsigned char charLen = 0;
	unsigned char  arrOut[2];

	while( *pInBuffer != '\0')
	{
		NGUnicodeToUCS2Encoding(*pInBuffer,&charLen,arrOut);
		pOutBuffer[++count] = arrOut[0];
		pOutBuffer[++count] = arrOut[1];
		pInBuffer++;
	}

	pOutBuffer[++count] = '\0';
    pOutBuffer[++count] = '\0';

	return count + 1;
}
 
int NGUnicodeToUtf8 (const unsigned char *_in, unsigned char *out)
{
    int i = 0;
    int outsize = 0;
    //unsigned char *result = NULL;
    unsigned char *tmp = NULL;
    unsigned char *in = (unsigned char*)_in;
    unsigned short unicode = 0;
	
    int charscount =  NGUnicodeStrlen((char *)_in);

	if(charscount < 2 ) return 0;
	
    tmp = out;

    for (i = 0; i < charscount; i += 2)
    {
        unicode = (in[i+1] << 8) | in[i];
        
        if (unicode >= 0x0000 && unicode <= 0x007f)
        {
            *tmp = (unsigned char)unicode;
            tmp += 1;
            outsize += 1;
        }
        else if (unicode >= 0x0080 && unicode <= 0x07ff)
        {
            *tmp = 0xc0 | (unicode >> 6);
            tmp += 1;
            *tmp = 0x80 | (unicode & (0xff >> 2));
            tmp += 1;
            outsize += 2;
        }
        else if (unicode >= 0x0800 && unicode <= 0xffff)
        {
            *tmp = 0xe0 | (unicode >> 12);
            tmp += 1;
            *tmp = 0x80 | ((unicode >> 6 )& 0x003f);
            tmp += 1;
            *tmp = 0x80 | (unicode & (0xff >> 2));
            tmp += 1;
            outsize += 3;
        }

    }

    *tmp = '\0';
    //*out = result;
    return outsize;
}

int NGUtf8ToUnicode (const unsigned char *in, unsigned char *out)
{
    const unsigned char *p = in;
    //unsigned short *result = NULL;
    int resultsize = 0;
    unsigned char *tmp = NULL;

    tmp = (unsigned char *)out;

    while(*p)
    {
        if (*p >= 0x00 && *p <= 0x7f)
        {
            *tmp = *p;
            tmp++;
            *tmp = '\0';
            tmp++;
            resultsize += 2;
        }
        else if ((*p & (0xff << 5))== 0xc0)
        {
            unsigned char t1 = 0;
            unsigned char t2 = 0;

            t1 = *p & (0xff >> 3);
            p++;
            t2 = *p & (0xff >> 2);

            *tmp = t2 | ((t1 & (0xff >> 6)) << 6);//t1 >> 2;
            tmp++;

            *tmp = t1 >> 2;//t2 | ((t1 & (0xff >> 6)) << 6);
            tmp++;

            resultsize += 2;
        }
        else if ((*p & (0xff << 4))== 0xe0)
        {
            unsigned char t1 = 0;
            unsigned char t2 = 0;
            unsigned char t3 = 0;

            t1 = *p & (0xff >> 3);
            p++;
            t2 = *p & (0xff >> 2);
            p++;
            t3 = *p & (0xff >> 2);
#if 0    //Big Endian

            *tmp = (t1 << 4) | (t2 >> 2);
            tmp++;

            *tmp = ((t2 & (0xff >> 6)) << 6) | t3;
            tmp++;

#else    //Little Endian

            *tmp = ((t2 & (0xff >> 6)) << 6) | t3;//(t1 << 4) | (t2 >> 2);
            tmp++;
			
            *tmp = (t1 << 4) | (t2 >> 2);//((t2 & (0xff >> 6)) << 6) | t3;
            tmp++;
#endif
            resultsize += 2;
        }

        p++;
    }

    *tmp = '\0';
    tmp++;
    *tmp = '\0';
    resultsize += 2;

    return resultsize;
}

int NGUnicodeStrlen( char * str)
{
    int i = -1;
    if(str == NULL)
        return i;
	
    for(i = 0;;i+=2)
    {
        if(0 == str[i] && 0 == str[i + 1])
        {
            break;
        }
    }
    
    return i;
}

void dump_utf8 (const unsigned char *utf8)
{
    const unsigned char *p = utf8;

    _MG_PRINTF ("\n OUTPUT STR Utf8 : \n");
    while(*p)
    {
        _MG_PRINTF ("%02X", *p);
        p++;
    }
    _MG_PRINTF ("\n");
}

void dump_unicode(const unsigned char *utf16)
{
    int i = 0;
    unsigned short *p = (unsigned short *)utf16;
    int size = NGUnicodeStrlen((char*)utf16);
    
    _MG_PRINTF ("OUTPUT STR Unicode : \n");
    for (i = 0; i < size; i++)
    {
        _MG_PRINTF ("%02X\n", *p);
        p++;
    }
    _MG_PRINTF ("\n");
}

int NGUCS2Strlen (const unsigned short *a)
{
    int i=0;
    if(a == NULL)
        return 0;
    while(*a++ != 0)
        i++;
    return i;
}

Glyph32* NGUCS2ConvertToGlyph32 (const unsigned short * ucs2Text, int* nrGlyphs, bool forceMBC)
{
    int nr_chars = NGUCS2Strlen (ucs2Text);
    if (nr_chars <= 0) {
        if (nrGlyphs)
            *nrGlyphs = 0;
        return NULL;
    }

    Glyph32* glyphs = (Glyph32*) malloc (sizeof (Glyph32) * nr_chars);
    if (glyphs) {
        for (int i = 0; i < nr_chars; i++) {
            if (forceMBC) {
                glyphs [i] = SET_MBC_GLYPH ((Glyph32)ucs2Text [i]);
            }
            else {
                glyphs [i] = ucs2Text [i];
            }
        }
    }

    if (nrGlyphs)
        *nrGlyphs = nr_chars;

    return glyphs;
}

