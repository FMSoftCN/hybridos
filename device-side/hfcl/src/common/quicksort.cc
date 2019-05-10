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
#include "common/quicksort.h"

static void shortSort(char* lo, char* hi, unsigned width, Compare comp);
static void swap(char* p, char* q, unsigned int width);

const unsigned int CUTOFF = /*8*/100; /* testing shows that this is good value */
//Memo app: Max 10, Calendar app:Max 100

/* sort the array between lo and hi (inclusive) */

void quickSort(void* base, unsigned num, unsigned width, Compare comp)
{
    char *lo = NULL, *hi = NULL;
    char *mid = NULL;
    char *loguy = NULL, *higuy = NULL;
    unsigned size = 0;
    char *lostk[30], *histk[30];

    int stkptr;

    if (num < 2 || width == 0)
        return;

    stkptr = 0; /* initialize stack */

    lo = (char*)base;
    hi = (char*)base + width * (num-1); /* initialize limits */

recurse:

    size = (hi - lo) / width + 1; /* number of el's to sort */

    if (size <= CUTOFF) {
        shortSort(lo, hi, width, comp);
    } else {
        mid = lo + (size / 2) * width; /* find middle element */
        swap(mid, lo, width); /* swap it to beginning of array */

        loguy = lo;
        higuy = hi + width;

        while (1) {
            do {
                loguy += width;
            } while (loguy <= hi && comp(loguy, lo) <= 0);

            do {
                higuy -= width;
            } while (higuy > lo && comp(higuy, lo) >= 0);

            if (higuy < loguy)
                break;

            swap(loguy, higuy, width);
        }

        swap(lo, higuy, width); /* put partition element in place */

        if ( higuy - 1 - lo >= hi - loguy ) {
            if (lo + width < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy - width;
                ++stkptr;
            }

            if (loguy < hi) {
                lo = loguy;
                goto recurse; /* do small recursion */
            }
        } else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;
            }

            if (lo + width < higuy) {
                hi = higuy - width;
                goto recurse; /* do small recursion */
            }
        }
    }

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse; /* pop subarray from stack */
    }
}

static void shortSort(char* lo, char* hi, unsigned width, Compare comp)
{
    char* p = NULL;
    char* max = NULL;

    while (hi > lo) {
        max = lo;
        for (p = lo + width; p <= hi; p += width) {
            if (comp(p, max) > 0) {
                max = p;
            }
        }
        swap(max, hi, width);
        hi -= width;
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

