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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef IME_INTERFACE_H
#define IME_INTERFACE_H
#define IME_STATUS_OPENED 	(IME_STATUS_USER_MIN + 1)

extern HWND g_imeWnd; //decleared in mFramework.cpp

inline void registerImeWindow(HWND hWnd) { g_imeWnd = hWnd; }
inline void unregisterImeWindow(void) { g_imeWnd = HWND_NULL; }

inline bool isImeExist(void) { return HWND_NULL != g_imeWnd; }

inline bool isImeOpen(void) {
    if (isImeExist())
        return SendMessage(g_imeWnd, MSG_IME_GETSTATUS, IME_STATUS_OPENED, 0);
    return false;
}

inline bool isImeEnable(void) {
    if (isImeExist())
		return SendMessage(g_imeWnd, MSG_IME_GETSTATUS, IME_STATUS_ENABLED, 0);
    return false;
}

inline void openImeWin(void) {
    if (isImeExist())
        SendMessage(g_imeWnd, MSG_IME_OPEN, 0, 0);
}

inline void setImeTarget(int tgt, int type) {
    if (isImeExist())
        SendMessage(g_imeWnd, MSG_IME_SETTARGET, tgt, type);
}

inline void imeInsertChar(char chr) {
    if (isImeExist())
        PostMessage(g_imeWnd, MSG_CHAR, (chr), 0);
}

#endif
