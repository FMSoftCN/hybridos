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
#ifndef UTIL_H
#define UTIL_H

#if 0
#ifdef __cplusplus
extern "C" {
#endif
#endif

int ngux_atoi(const char* str);
double ngux_atof(const char* str);
const char* ngux_itoa(char* str, int len, int n);
const char* ngux_ftoa(char* str, int len, double f);
long ngux_strtol(const char* str, const char **end, int base);
char* ngux_strcat(char* str, int len, const char* first, ...);
#if 0
#ifdef __cplusplus
}
#endif
#endif

#endif
