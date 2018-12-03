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

#ifndef _MGCL_TRANSPARENT_H
#define _MGCL_TRANSPARENT_H


namespace hfcl {


///////////////////////////MgclTrans.cpp/////////////////////////
void mgclUpdateWindowClips(HWND hWnd, const HDC hdc);

void mgclUpdateTransparentWindow(HWND hWnd, const PCLIPRGN pclips, int xoff, int yoff);


typedef struct {
	HDC howner;
	HDC hmemdc;
	HDC htrans;
	int bkg_alpha;
}TRANSPARENTINFO;

HDC mgclBeginTransparentBkgnd(HWND hWnd, HDC hdc, int bkg_alpha, TRANSPARENTINFO *pinfo);

void mgclEndTransparentBkgnd(TRANSPARENTINFO *pinfo);

///////////////////////////MgclTrans.cpp/////////////////////////
} // namespace hfcl

#endif /* TRANSPARENT_H */

