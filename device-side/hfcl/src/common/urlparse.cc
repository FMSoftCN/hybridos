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

////////////////////////////////////////////

#include "common/common.h"
#include "urlparse.h"

#define GUIURL_PHONE_NUMBER_LEN_MIN 3
#define GUIURL_PHONE_NUMBER_LEN_MAX 40

bool IsDigit(char c)
{
    return ('0' <= c && c <= '9');
}


bool IsPhoneNumberChar(char c)
{
    return (IsDigit(c)
        || '*' == c
        || '#' == c);
}

bool IsPhoneNumberHyphen(char c)
{
    return ('-' == c);
}

/*==============================================================================
Description: 判定一个字符串是不是满足电话号码的模式
Return: bool, *confirm_len_ptr会赋予不同的值
    TRUE - 是
        *confirm_len_ptr - 识别的长度
    FALSE - 否
        *confirm_len_ptr - 已检查过的长度
Note: 简单的regEx可以写作: ([+0][*#\d])?[*#\d]{5,}(([Pp]+|[Ww])[*#\d]+))*
    1 这个不是url的一部分，是为了程序处理的统一方便
    2 这个正则表达式比较难写...所以，这个正则表达式并不是真正的判别模式
    3 增加连字符"-"的识别，识别规则
        1 在首位不识别
        2 连续的连字符不识别
    4 增加字符限制，去除连字符之后超过最大字符数的，不识别
==============================================================================*/
bool URL_IsPhoneNumber(char const * str_ptr, int *confirm_len_ptr)
{
    int extra_char_count = 0;
    int extra_digit_count = 0;
    int hyphen_count = 0;
    int tailer_hyphen_count = 0;
    int min_number_len = GUIURL_PHONE_NUMBER_LEN_MIN;
    bool	is_atleast_onechar_numeric = FALSE;
    char const *str_pos_ptr = NULL;
    
    
    if (NULL == str_ptr || NULL == confirm_len_ptr)
    {
        return FALSE;
    }

    //首字符必须是 [+*#\d]
    *confirm_len_ptr = 1;

    if('+' == *str_ptr)
    {
        min_number_len += 1;
    }
   /*    else if('0' == *str_ptr)
    {
        min_number_len += 2;
    }*/
    else if(!IsPhoneNumberChar(*str_ptr))
    {
        return FALSE;
    }

    //然后必须有一连串的数字，五个(或者区号一类的一个，共六个)
    while(IsPhoneNumberChar(*(str_ptr + *confirm_len_ptr))
        || IsPhoneNumberHyphen(*(str_ptr + *confirm_len_ptr)))
    {
    	if(IsDigit(*(str_ptr + *confirm_len_ptr)) && !is_atleast_onechar_numeric)
    		is_atleast_onechar_numeric = TRUE;
		
        *confirm_len_ptr = *confirm_len_ptr + 1;
        
        if(IsPhoneNumberHyphen(*(str_ptr + *confirm_len_ptr - 1)))
        {
            hyphen_count++;
            tailer_hyphen_count++;
            if(tailer_hyphen_count > 1) break;
        }
        else
        {
            tailer_hyphen_count = 0;
        }
    }

    //([+0]\d{2,2})?\d{3,3}
    if(*confirm_len_ptr - hyphen_count < min_number_len || !is_atleast_onechar_numeric)
    {
        return FALSE;
    }

    if(tailer_hyphen_count <= 1)
    {
        //(([Pp]+|[Ww])[*#\d]+))*
        do
        {
            extra_char_count = 0;
            extra_digit_count = 0;

            str_pos_ptr = str_ptr + *confirm_len_ptr;

            if('w' == *str_pos_ptr || 'W' == *str_pos_ptr)
            {
                extra_char_count = 1;
            }
            else if('p' == *str_pos_ptr || 'P' == *str_pos_ptr)
            {
                extra_char_count = 1;
                
                while('p' == *(str_pos_ptr + extra_char_count)
                    || 'P' == *(str_pos_ptr + extra_char_count))
                {
                    extra_char_count++;
                }
            }

            if(extra_char_count == 0) break;

            tailer_hyphen_count = 0;

            str_pos_ptr = str_ptr + *confirm_len_ptr + extra_char_count;
            while(IsPhoneNumberChar(*(str_pos_ptr + extra_digit_count))
                || IsPhoneNumberHyphen(*(str_pos_ptr + extra_digit_count)))
            {
                extra_digit_count++;

                if(IsPhoneNumberHyphen(*(str_pos_ptr + extra_digit_count - 1)))
                {
                    hyphen_count++;
                    tailer_hyphen_count++;
                    if(tailer_hyphen_count > 1) break;
                }
                else
                {
                    tailer_hyphen_count = 0;
                }
            }

            if(extra_digit_count - tailer_hyphen_count == 0) break;

            *confirm_len_ptr = *confirm_len_ptr + extra_char_count + extra_digit_count;

            if(tailer_hyphen_count > 1) break;
        }while(1);/*lint !e506*/
    }

    *confirm_len_ptr = *confirm_len_ptr - tailer_hyphen_count;
    hyphen_count -= tailer_hyphen_count;
    

    return (*confirm_len_ptr - hyphen_count <= GUIURL_PHONE_NUMBER_LEN_MAX);
}

/*==============================================================================
Description: 解析字符串中的可识别字符串
==============================================================================*/
int URL_ParseDataGen(
    char const * str_ptr,
    int * addrTable,
    int tableSize
)
{
    char const *p = NULL;
    int pos = 0;
    int cnt = 0;

    int pn_confirm_len = 0;
    int pn_len = 0;
    bool is_pn = FALSE;


    if (NULL == str_ptr || NULL == addrTable)
    {
        return 0;
    }

    while (*(str_ptr + pos))
    {
        p = str_ptr + pos;

        do
        {
            if (pn_confirm_len <= pos)
            {
                is_pn = (URL_IsPhoneNumber(p, &pn_len));

                pn_confirm_len = pos + pn_len;

                if (is_pn)
                {
                    addrTable[cnt] = ((pos&0xFFFF)<<16)|(pn_len&0xFFFF);
                    pos += pn_len;
					cnt++;

					if(cnt >= tableSize)
						return cnt;
					
                    break;
                }
            }
            pos++;

        }
        while (0);
    }

    return cnt;
}

////////////////////////////////////////////


