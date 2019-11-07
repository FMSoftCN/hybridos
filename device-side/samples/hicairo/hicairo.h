///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
** hicairo.h:
**  Headers of sample for hiCairo.
**
** Copyright (C) 2019 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef _HICAIRO_H_
#define _HICAIRO_H_

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <cairo/cairo.h>

#define HEIGHT  800
#define WIDTH   600

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int draw_animal (cairo_t *cr, const char* which, int width, int height);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _HICAIRO_H_ */
