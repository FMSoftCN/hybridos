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

#include "common/object.h"

namespace hfcl {

/*
** check a utf8 string
** check the tail characters correct or not.
** if incorrect, cut the tail.
**/
static int check_utf8_str(char *_buff, int _len)
{
    int _tail_utf8_len = 1;
    int _tail_real_len = 1;

    if (_buff == NULL)
        return 0;

    if (_len <= 0) {
        _len = strlen(_buff);
    }
    char *_tail = _buff + _len - 1;

    if (!((*_tail) & 0x80)) {
        // tail is assic charater
        return _len;
    }

    while(((*_tail) & 0x80) && !((*_tail) & 0x40)) {
        _tail_real_len++;
        _tail--;
    }

    if (!((*_tail) & 0x80)) {
        *(_tail + 1) = '\0';
        return _len - (_tail_real_len - 1);
    }

    while ((*_tail)  & (0x80 >> _tail_utf8_len)) {
        _tail_utf8_len++;
    }

    if (_tail_utf8_len != _tail_real_len) {
        // cut the error tail
        *_tail = '\0';
        return _len - _tail_real_len;
    }

    return _len;
}

static int utf8_len_first_char (unsigned char* mstr, int len)
{
    int t, c = *((unsigned char *)(mstr++));

    if (c & 0x80) {
        int n = 1, ch_len = 0;
        while (c & (0x80 >> n))
            n++;

        if (n > len)
            return 0;

        ch_len = n;
        while (--n > 0) {
            t = *((unsigned char *)(mstr++));

            if ((!(t & 0x80)) || (t & 0x40))
                return 0;
        }

        return ch_len;
    }

    /* for ascii character */
    return 1;
}

#if 0
Uint32 utf8_to_uc32 (const char* mchar, int mchar_len)
{
    Uint32 wc = *((unsigned char *)(mchar++));
    Uint32 t;
    int n;

    if (wc & 0x80) {
        n = 1;
        while (wc & (0x80 >> n))
            n++;

        wc &= (1 << (8-n)) - 1;
        while (--n > 0) {
            t = *((unsigned char *)(mchar++));

            wc = (wc << 6) | (t & 0x3F);
        }
    }

    return wc;
}
#endif

int utf8_to_uc32 (const char* mchar, int mchar_len, Uint32* uc)
{
    Uint32 wc = *((unsigned char *)(mchar++));
    Uint32 t;
    int n, ch_len = 0;

    if (wc & 0x80) {
        n = 1;
        while (wc & (0x80 >> n))
            n++;

        if (mchar_len > 0 && n > mchar_len)
            return 0;

        ch_len = n;
        wc &= (1 << (8-n)) - 1;
        while (--n > 0) {
            t = *((unsigned char *)(mchar++));

            if ((!(t & 0x80)) || (t & 0x40))
                return 0;

            wc = (wc << 6) | (t & 0x3F);
        }
    }
    else if (wc == 0)
        return 0;

    if (uc) {
        *uc = wc;
    }

    return ch_len;
}

int uc32_to_utf8(Uint32 c, char* outbuf)
{
    int len = 0;
    int first;
    int i;

    if (c < 0x80) {
        first = 0;
        len = 1;
    }
    else if (c < 0x800) {
        first = 0xc0;
        len = 2;
    }
    else if (c < 0x10000) {
        first = 0xe0;
        len = 3;
    }
    else if (c < 0x200000) {
        first = 0xf0;
        len = 4;
    }
    else if (c < 0x4000000) {
        first = 0xf8;
        len = 5;
    }
    else {
        first = 0xfc;
        len = 6;
    }

    if (outbuf) {
        for (i = len - 1; i > 0; --i) {
            outbuf[i] = (c & 0x3f) | 0x80;
            c >>= 6;
        }
        outbuf[0] = c | first;
    }

    return len;
}

int get_utf_chars_info(const char *mstr, int len, int *retPosChars)
{
    int count = 0;
    int left_bytes = len;
    int len_cur_char;
    unsigned char *str_temp = NULL , *_p_str = NULL;

    if (mstr == NULL || len <= 0) {
        return 0;
    }

    if ((_p_str = (unsigned char *)HFCL_MALLOC(len + 1)) == NULL) {
        return 0;
    }

    memcpy(_p_str, mstr, len + 1);
    _p_str[len] = '\0';

    str_temp = (unsigned char *)_p_str;

    len = check_utf8_str((char *)str_temp, len);

    while (left_bytes > 0) {
        if (retPosChars)
            retPosChars [count] = len - left_bytes;

        len_cur_char = utf8_len_first_char(str_temp, left_bytes);
        if (len_cur_char > 0) {
            count ++;
            left_bytes -= len_cur_char;
            str_temp += len_cur_char;
            continue;
        }
        else if (len_cur_char == 0) {
            int   c = *((unsigned char *)(str_temp));
            int n = 1;
            if (c & 0x80) {
                while (c & (0x80 >> n))
                n++;
            }

            if(left_bytes < n) {
                break;
            }
            else {
                count ++;
                left_bytes -= n;
                str_temp += n;
            }
         }
         else {
            break;
        }
    }

    HFCL_FREE(_p_str);

    return count;
}

int get_last_utf8_char_len(const char *str, int len)
{
    int left_bytes = len;
    int len_cur_char;
    int lastlen = 0;
    unsigned char *str_temp = NULL;

    if(str == NULL || len <= 0)
    {
        return 0;
    }

    str_temp = (unsigned char *)str;

    len = check_utf8_str((char *)str_temp, len);

    while (left_bytes > 0) {
        len_cur_char = utf8_len_first_char(str_temp, left_bytes);
        if (len_cur_char > 0) {
            left_bytes -= len_cur_char;
            str_temp += len_cur_char;
            lastlen = len_cur_char;
            continue;
        } else {
            break;
        }
    }
    return lastlen;
}

int get_first_utf8_char_len(const char *str, int len)
{
    if(str == NULL || len <= 0)
    {
        return 0;
    }

    len = check_utf8_str((char*)str, len);

    return utf8_len_first_char((unsigned char*)str, len);
}

int get_utf8_char_count(const char *mstr, int len)
{
    int count = 0;
    int left_bytes = len;
    int len_cur_char;
    unsigned char *str_temp = (unsigned char *)mstr;

    if(mstr == NULL || len <= 0)
    {
        return 0;
    }

    //len = check_utf8_str((char *)str_temp, len);

    while (left_bytes > 0) {
        len_cur_char = utf8_len_first_char(str_temp, left_bytes);
        if (len_cur_char > 0) {
            count ++;
            left_bytes -= len_cur_char;
            str_temp += len_cur_char;
            continue;
        } else {
            break;
        }
    }
    return count;
}

int get_utf8_len_by_char_count(const char *mstr, int charcount)
{
   char *str_temp = NULL;
   int charLen, tLen = 0;

    if(mstr == NULL || charcount <= 0)
    {
        return 0;
    }
    str_temp = (char *)mstr;
    while (charcount){
        charLen = get_first_utf8_char_len(str_temp, strlen(str_temp));
        tLen += charLen;
        str_temp += charLen;
        charcount --;
    }
    return  tLen;
}

Uint16 UTF8ToUCS2 (Uint8 *utf8)
{
    Uint16  ucs2;
    unsigned char c = utf8[0];

    if (c < 0x80) {
        ucs2 = c;
    } else if (c < 0xe0) {
        ucs2 = ((Uint16) (c & 0x1f) << 6) | (Uint16) (utf8[1] ^ 0x80);
    } else {
        ucs2 =      ((Uint16) (c & 0x0f) << 12)
                | ((Uint16) (utf8[1] ^ 0x80) << 6)
                |  (Uint16) (utf8[2] ^ 0x80);
    }

    return ucs2;
}

bool IsNumberUCS2Char (Uint16 inChar)
{
   if (inChar >= 0x0030  && inChar <= 0x0039)
      return true;
   else
      return false;
}

bool IsSymbolUCS2Char (Uint16 inChar)
{
   if ((inChar >= 0x0020  && inChar <= 0x002F) || (inChar >= 0x003A  && inChar <= 0x0040)
      ||(inChar >= 0x005B  && inChar <= 0x0060)|| (inChar >= 0x007B  && inChar <= 0x007E)
      || (inChar >= 0x00A1 && inChar <= 0x00BF)||(inChar == 0x20AC)||(inChar == 0x060C))
      return true;
   else
      return false;
}

bool IsArabicSymbolUCS2Char (Uint16 inChar)
{
   if ((inChar == 0x061B )||(inChar == 0x061F ))
      return true;
   else
      return false;
}

bool IsTransformArabicUCS2Char (Uint16 inChar)
{
    if ((inChar >= 0x0621 && inChar <= 0x0650)
            || (inChar >= 0x0671 && inChar <= 0x06D3))
        return true;

    return false;
}

} // namespace hfcl

