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

#include "mgcl/mgcl.h"

#include "activity/activitymanager.h"
#include "common/common.h"
#include "common/log.h"
#include "common/event.h"

namespace hfcl {

bool mgclGetMessage(PMSG pmsg, HWND hwnd)
{
    return GetMessage(pmsg, hwnd);
}

int mgclGetTextMCharInfo(HWND hWnd, const char* mstr, int len, int* pos_chars)
{
    if (!IsWindow(hWnd))
         return -1;

    return GetTextMCharInfo(GetWindowFont(hWnd), mstr, len, pos_chars);
}

DWORD mgclDoModal(HWND hWnd, BOOL bAutoDestroy)
{
    MSG Msg;
    HWND hOwner, hFocus;
    BOOL isActive;
    DWORD ret = 0;
    BOOL bNoCleanOwner = FALSE;
    HWND hwnd;

    hFocus = GetNextDlgTabItem (hWnd, (HWND)0, FALSE);
    if (hFocus)
        SetFocus (hFocus);

//    IncludeWindowStyle(hWnd, NCSS_MNWND_MODE);

    if (!(GetWindowExStyle(hWnd) & WS_EX_DLGHIDE))
        ShowWindow (hWnd, SW_SHOWNORMAL);

    hOwner = GetHosting(hWnd);

    if (hOwner && hOwner != HWND_DESKTOP) {
        if (IsWindowEnabled(hOwner)) {
            EnableWindow(hOwner, FALSE);
            IncludeWindowExStyle (hOwner, MGCL_WS_EX_MODALDISABLED);
        }
        /*Throw the messages of hOwner*/
         while (PeekPostMessage (&Msg, hOwner,
             MSG_KEYDOWN, MSG_KEYUP, PM_REMOVE));
    }

    hwnd = hWnd;
    /*
        VERY IMPORTANT: In windows, when user call DestroyMainWindow, the memory pointed by "self"
        would be free. and self's memory would be pad by unknown values. so we should save hwnd to
        avoid the hWnd changed
    */

    while (mgclGetMessage(&Msg, hwnd)) {
        // hook key
        // update lcd service and key tone FIXME
        if (Msg.hwnd == HWND_DESKTOP
            && ( Msg.message == MSG_KEYDOWN
               || Msg.message == MSG_KEYUP
               || Msg.message == MSG_KEYLONGPRESS
               || Msg.message == MSG_KEYALWAYSPRESS))
        {
            // stop process
            if(STOP_DISPATCH == ActivityManager::getInstance()->processKeyHook(&Msg))
                continue;
        }
        if (Msg.hwnd == hwnd) {
            if (Msg.message == MGCL_MSG_MNWND_ENDDIALOG) {
                ret = Msg.lParam;
                break;
            } else if (Msg.message == MSG_NCDESTROY) {
                bAutoDestroy = TRUE;
                ret = 0;
                break;
            }
        } else {
            if (Msg.message == MSG_NCDESTROY) {
                HWND hosting = hOwner;
                while(Msg.hwnd != hosting && hosting != HWND_INVALID &&
                        hosting != HWND_NULL && hosting != HWND_DESKTOP) {
                    hosting = GetHosting(hosting);
                }
                if (IsWindow(hosting) && hosting != HWND_DESKTOP) {
                    bAutoDestroy = TRUE;
                    ret = 0;
                    //reput the message destory
                    PostMessage(Msg.hwnd, Msg.message, Msg.wParam, Msg.lParam);
                    break;
                }
            }
        }

        if (!ActivityManager::getInstance()->isCharFreezon())
            TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    if (hWnd != hwnd) //Maybe DestroyMainWindow is called
        return ret;

    isActive = (GetActiveWindow() == hWnd);

    if (!bNoCleanOwner &&  hOwner != HWND_DESKTOP && hOwner != HWND_NULL) {
        if (GetWindowExStyle (hOwner) & MGCL_WS_EX_MODALDISABLED) {
            EnableWindow (hOwner, TRUE);
            ExcludeWindowExStyle (hOwner, MGCL_WS_EX_MODALDISABLED);
            if (isActive && IsWindowVisible (hOwner)) {
                ShowWindow (hOwner, SW_SHOWNORMAL);
                SetActiveWindow (hOwner);
            }
        }
    }
#if 0
    if ((GetWindowStyle(hWnd)&NCSS_MNWND_MODE) && bAutoDestroy) {
        HWND hwnd = hWnd;
        //destroy
        DestroyMainWindow(hWnd);
        MainWindowCleanup(hwnd);
    } else {
        ExcludeWindowStyle(hWnd, NCSS_MNWND_MODE);
    }
#else
    if (bAutoDestroy) {
        HWND _hwnd = hWnd;
        //destroy
        DestroyMainWindow(hWnd);
        MainWindowCleanup(_hwnd);
    }
#endif

    return ret;
}

DWORD mgclDoModalView(HWND hwnd)
{
    MSG Msg;
    BOOL bAutoDestroy = FALSE;
    DWORD ret = -1;
    HWND  hwnd_shadow = hwnd;

    if (!(GetWindowExStyle(hwnd) & WS_EX_DLGHIDE))
        ShowWindow (hwnd, SW_SHOWNORMAL);

    SetActiveWindow(hwnd);

    while (mgclGetMessage(&Msg, hwnd)) {
        // hook key
        // update lcd service and key tone FIXME
        if (Msg.hwnd == HWND_DESKTOP
            && ( Msg.message == MSG_KEYDOWN
               || Msg.message == MSG_KEYUP
               || Msg.message == MSG_KEYLONGPRESS
               || Msg.message == MSG_KEYALWAYSPRESS))
        {
            // stop process
            if(STOP_DISPATCH == ActivityManager::getInstance()->processKeyHook(&Msg))
                continue;
        }
        if (Msg.hwnd == hwnd) {
            if (Msg.message == MGCL_MSG_MNWND_ENDDIALOG) {
                ret = Msg.lParam;
                break;
            } else if (Msg.message == MSG_NCDESTROY) {
                bAutoDestroy = TRUE;
                ret = 0;
                break;
            }
        } else {
            if (Msg.message == MSG_NCDESTROY) {
                HWND hosting = GetHosting(hwnd);
                while (Msg.hwnd != hosting && hosting != HWND_INVALID &&
                        hosting != HWND_NULL && hosting != HWND_DESKTOP) {
                    hosting = GetHosting(hosting);
                }
                if (IsWindow(hosting) && hosting != HWND_DESKTOP) {
                    bAutoDestroy = TRUE;
                    ret = 0;
                    //reput the message destory
                    PostMessage(Msg.hwnd, Msg.message, Msg.wParam, Msg.lParam);
                    break;
                }
            }
        }

        if (!ActivityManager::getInstance()->isCharFreezon())
            TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    if (hwnd_shadow != hwnd) //Maybe DestroyMainWindow is called
        return ret;

    if (bAutoDestroy) {
        //destroy
        DestroyMainWindow(hwnd_shadow);
        MainWindowCleanup(hwnd_shadow);
    }

    return ret;
}


void mgclEndDialog(HWND hwnd, int endcode)
{
    SendNotifyMessage(hwnd, MGCL_MSG_MNWND_ENDDIALOG, 0, (LPARAM)endcode);
}

#if 0
HWND mgclCreateMainWindow(PMAINWINCREATE pCreateInfo)
{
    return CreateMainWindow(pCreateInfo);
}

bool mgclDestroyMainWindow(HWND hWnd)
{
    return DestroyMainWindow(hWnd);
}

int mgclDefaultMainWinProc(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam)
{
    return DefaultMainWinProc(hWnd, msg, wParam, lParam);
}

bool mgclShowWindow(HWND hWnd, int iCmdShow)
{
    return ShowWindow(hWnd, iCmdShow);
}

DWORD mgclGetWindowAdditionalData(HWND hWnd)
{
    return GetWindowAdditionalData(hWnd);
}

DWORD mgclSetWindowAdditionalData(HWND hWnd, DWORD newData)
{
    return SetWindowAdditionalData(hWnd, newData);
}

unsigned int mgclGetSystemCursor(int csrid)
{
    return GetSystemCursor(csrid);
}

void mgclExitGUISafely(int exitcode)
{
    return ExitGUISafely(exitcode);
}

void mgclMainWindowThreadCleanup(HWND hMainWnd)
{
    return MainWindowThreadCleanup(hMainWnd);
}

bool mgclGetMessage(PMSG pMsg, HWND hWnd)
{
    return GetMessage(pMsg, hWnd);
}

bool mgclTranslateMessage(PMSG pMsg)
{
    return TranslateMessage(pMsg);
}

int mgclDispatchMessage(PMSG pMsg)
{
    return DispatchMessage(pMsg);
}

int mgclInitGUI(int args, const char** argv)
{
    return InitGUI(args, argv);
}

HDC mgclGetSecondaryDC(HWND hwnd)
{
    return GetSecondaryDC(hwnd);
}

HWND mgclGetCapture(void)
{
    return GetCapture();
}

HWND mgclSetCapture(HWND hWnd)
{
    return SetCapture(hWnd);
}

void mgclReleaseCapture(void)
{
    return ReleaseCapture();
}

bool mgclGetClientRect(HWND hWnd, PRECT prc)
{
    return GetClientRect(hWnd, prc);
}

bool mgclPtInRect(const RECT* prc, int x, int y)
{
    return PtInRect(prc, x, y);
}

bool mgclInvalidateRect(HWND hWnd, const RECT* prc, bool bEraseBkgnd)
{
    return InvalidateRect(hWnd, prc, bEraseBkgnd);
}

void mgclGetCursorPos(POINT* ppt)
{
    return GetCursorPos(ppt);
}

void mgclScreenToClient(HWND hWnd, int* x, int* y)
{
    return ScreenToClient(hWnd, x, y);
}

void mgclClientToScreen(HWND hWnd, int* x, int* y)
{
    return ClientToScreen(hWnd, x, y);
}

bool mgclIsMainWindow(HWND hWnd)
{
    return IsMainWindow(hWnd);
}

HDC mgclBeginPaint(HWND hWnd)
{
    return BeginPaint(hWnd);
}

void mgclEndPaint(HWND hWnd, HDC hdc)
{
    return EndPaint(hWnd, hdc);
}

bool mgclMoveWindow(HWND hWnd, int x, int y, int w, int h, bool fPaint)
{
    return MoveWindow(hWnd, x, y, w, h, fPaint);
}

unsigned int mgclSetActiveWindow(unsigned int hMainWnd)
{
    return SetActiveWindow(hMainWnd);
}

unsigned int mgclGetActiveWindow(void)
{
    return GetActiveWindow();
}

void mgclUpdateWindow(unsigned int hWnd, bool bErase)
{
    return UpdateWindow(hWnd, bErase);
}

void mgclSetWindowExStyle(HWND hwnd ,int dwStyle, bool isInclude)
{
    if (isInclude)
        IncludeWindowExStyle(hwnd, (unsigned long)dwStyle);
    else
        ExcludeWindowExStyle(hwnd, (unsigned long)dwStyle);
}

unsigned long mgclGetWindowExStyle(HWND hwnd)
{
    return GetWindowExStyle(hwnd);
}
#endif

} // namespace hfcl

