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

#include "common/common.h"

#include "common/selectsort.h"

static void swap(char* p, char* q, unsigned int width);

// we need the method stable !!!!
// inc means increase or descrease
void selectSort(void* base, unsigned num, unsigned width, Compare comp, bool inc)
{
    unsigned int i = 0, j = 0, idx = 0;
    char* max_min = NULL;
    for (i = 0; i < num; ++i) {
        idx = i;
        max_min = (char *)base + i*width;
        for (j = i + 1; j < num; ++j) {
            if (inc) {
                // 1 means <, 0 means == , 1 means >
                if (comp((char *)base+j*width, max_min) < 0) {
                    max_min = (char *)base+j*width;
                    idx = j;
                }
            }
            else {
                // 1 means <, 0 means == , 1 means >
                if (comp((char *)base+j*width, max_min) > 0) {
                    max_min = (char *)base+j*width;
                    idx = j;
                }
            }
        }

        if (idx != i) {
            swap((char *)base+idx*width, (char *)base+i*width, width);
        }
    }
}

static void swap(char* a, char* b, unsigned width)
{
    char tmp = '\0';

    if (a == b)
        return;

    while (width--) {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
}

