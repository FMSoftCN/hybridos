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

#include "nguxcommon.h"
#include "nguxobject.h"


NAMESPACE_BEGIN

Object::Object()
{

}

Object::~Object()
{

}

NAMESPACE_END

#ifdef _NGUX_DONT_SUPPORT_TEMPLATE

//a Null buffer
unsigned int _null_buffer[] = {0,0,0,0};

size_t string::npos = (size_t)-1;


void* operator new(unsigned int size)
{
    void * pointer = 0x00;
    //NGUInt32 nCallerAdd = 0x00;
    //COS_GET_RA(&nCallerAdd);

    if (size == 0)
        size = 1;

    pointer = calloc(1, size);
    if(pointer != NULL) 
    {
        //COS_UpdateMemCallAdd(pointer, nCallerAdd);
    }
    return pointer;
}

void operator delete(void *p)
{
    if (p != NULL)
        free(p);
}


void* operator new[](unsigned int size)
{
    void * pointer = 0x00;
    //NGUInt32 nCallerAdd = 0x00;
    //COS_GET_RA(&nCallerAdd);

    if (size == 0)
        size = 1;
    pointer = calloc(1, size);
    if(pointer != NULL) 
    {
        //	 COS_UpdateMemCallAdd(pointer, nCallerAdd);
    }
    return pointer;
}

void operator delete[](void *p)
{
    if (p != NULL)
        free(p);
}


#endif

