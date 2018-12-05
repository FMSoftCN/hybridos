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

#include "common/stlalternative.h"

namespace hfcl {

string::string(const char* str, int n)
{
    _str_buff = NULL;
    if (str == NULL) return;

    if (n <= 0)
        n = strlen(str);
    _str_buff =(char *)HFCL_MALLOC(n + 1);
    if (_str_buff != NULL)
    {
        strncpy(_str_buff, str, n);
        _str_buff[n] = 0;
        check_utf8_str(_str_buff, n);
    }
}

string::string(const string& str)
{
     int n;
     _str_buff = NULL;
     if(str.c_str() == NULL) return;
       n = strlen(str.c_str());
     _str_buff = (char *)HFCL_MALLOC(n + 1);
     if (_str_buff != NULL)
     {
         strncpy(_str_buff, str.c_str(), n);
         _str_buff[n] = 0;
         check_utf8_str(_str_buff, n);
     }
}

const string & string::operator=(const string &str) {
    int n;
    if (this == &str)     return *this;

    if(str.c_str() == NULL) return *this;
    if (_str_buff != NULL)  HFCL_FREE(_str_buff);
    _str_buff = NULL;

    n = strlen(str.c_str());
    _str_buff = (char *)HFCL_MALLOC(n + sizeof(short));

    if (_str_buff != NULL)
    {
        strncpy(_str_buff, str.c_str(), n);
        _str_buff[n] = 0;
        check_utf8_str(_str_buff, n);
    }
    return *this;
}

const string & string::operator=(const char* str) {
    int n;
    if (_str_buff != NULL)  HFCL_FREE(_str_buff);
    _str_buff = NULL;
    if(str ==  NULL) return *this;

    n = strlen(str);
    _str_buff = (char *)HFCL_MALLOC(n + sizeof(short));

    if (_str_buff != NULL)
    {
        strncpy(_str_buff, str, n);
        _str_buff[n] = 0;
        check_utf8_str(_str_buff, n);
    }
    return *this;
}

string& string::append(const char* str) {
    int n;
    char * new_str;

    if (str == NULL)
        return *this;

    if (_str_buff == NULL) {
        this->operator=(str);
        return *this;
    }

    new_str = NULL;
    n = strlen(str);
    new_str = (char *)HFCL_MALLOC(length() + n+1);

    if (new_str != NULL)
    {
        strcpy(new_str, _str_buff);
        strcat(new_str, str);
    }

    if (_str_buff != NULL)  HFCL_FREE(_str_buff);
    _str_buff = new_str;
    check_utf8_str(_str_buff, -1);

    return *this;
}

void vector_base::resize(int sz)
{
    if (sz < 0)
        return;
    if (sz == _count)
        return;

    if (sz < _count) {
        unsigned char* buff = (unsigned char*) _get(sz);
        for (int i = sz; i < _count; i++, buff += _size)
            delete_sub_type(buff);
    }

    if (sz == 0) {
        HFCL_FREE(_buffer);
        _buffer = NULL;
        _count = sz;
        return;
    }

    _buffer = (unsigned char*)HFCL_REALLOC(_buffer, _size * sz);
    if (sz > _count) {
        memset(_get(_count), 0, (sz - _count)*_size);
    }
    _count = sz;
}

} // namespace hfcl

