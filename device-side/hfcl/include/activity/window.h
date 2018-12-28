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

#ifndef HFCL_ACTIVITY_WINDOW_H_
#define HFCL_ACTIVITY_WINDOW_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "../common/common.h"
#include "../common/object.h"
#include "../common/intrect.h"
#include "../common/event.h"

namespace hfcl {

class RootView;

class Window : public Object {
public:
    Window();
    virtual ~Window();

    void setWindowRect(const IntRect& rc);
    void getClientRect(IntRect& rc);

    void show(bool bUpdateBack = true);
    void hide(void);
    void destroy(void);

    // Synchronous
    void updateWindow(bool isUpdateBkg = TRUE);

    HWND viewWindow(void) const;
    static Window* window(HWND hwnd);

    HWND setActiveWindow(HWND hMainWnd);
    HWND getActiveWindow(void);

    // Asynchronous
    void asyncUpdateRect(int x, int y, int w, int h, bool upBackGnd = true);
    // Synchronous
    void syncUpdateRect(int x, int y, int w, int h, bool upBackGnd = true);

    int doModal(bool bAutoDestory = false);
    unsigned int doModalView();
    void endDlg(int endCode);

    RootView* getRootView(int view_id);

protected:
    HWND m_sysWnd;
    bool createMainWindow(void);
    virtual bool createMainWindow(int x, int y, int w, int h, bool visible = true);
    static HWND createMainWindow(const char* caption, WNDPROC proc,
            int x, int y, int width, int height, DWORD addData, bool visible = true);
};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_WINDOW_H_ */

