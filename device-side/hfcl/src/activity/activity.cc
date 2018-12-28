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

#include "activity/activity.h"

namespace hfcl {

Activity::Activity() : Window()
{
}

Activity::~Activity()
{

}

void Activity::onCreate(ContextStream* contextStream, Intent* intent)
{
    // TODO: create main window according to POS property
    createMainWindow(0, 0,
            GetGDCapability(HDC_SCREEN, GDCAP_HPIXEL),
            GetGDCapability(HDC_SCREEN, GDCAP_VPIXEL));
}

void Activity::onWakeup(void)
{
    setActiveWindow (viewWindow());
    show();
};

void Activity::onMove2Top(void)
{
    setActiveWindow (viewWindow());
    show();
}

HWND Activity::getSysWindow(void)
{
    return m_sysWnd;
}

Activity* Activity::activity(HWND hwnd)
{
    return reinterpret_cast<Activity*>(window(hwnd));
}

void Activity::updateNow(void)
{
    updateWindow();
}

void Activity::setFullScreen(bool isFullScreen)
{
    if (isFullScreen) {
        setWindowRect(IntRect(0, 0, GetGDCapability(HDC_SCREEN, GDCAP_HPIXEL),
                GetGDCapability(HDC_SCREEN, GDCAP_VPIXEL)));
    }
    else {
        // TODO: Restore old rectangle
        setWindowRect(m_old_rect);
    }
}

void FullScreenActivity::onCreate(ContextStream* contextStream, Intent* intent)
{
    createMainWindow();
}

} // namespace hfcl

