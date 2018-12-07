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

#include "components/tipdlg.h"
#include "components/menu.h"

#include "view/textview.h"
#include "resource/respkgmanager.h"
#include "services/timerservice.h"
#include "activity/activity.h"
#include "activity/activitymanager.h"

// FIXME
#define SCREEN_WIDTH        240
#define SCREEN_HEIGHT       320

using namespace hfcl;

TipDlg::TipDlg(HTResId resid)
{
    m_textView = NULL;
    m_rect = IntRect(10, 20, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 20);

    m_timerId = 0;
    m_autoCloseTime = 0;
    setBkgAlpha(0);
    create(resid);
}

TipDlg::~TipDlg()
{
    if (m_timerId > 0)
        removeTimer(m_timerId);
}

void TipDlg::create(HTResId resid)
{
    createMainWindow(m_rect.left(), m_rect.top(), m_rect.width(), m_rect.height());

    // FIXME: mgclSetWindowExStyle(m_viewWindow, Window::HFCL_WS_EX_TRANSPARENT,true);

    CreateViewFromRes(resid == 0 ? UI_TEMPL : resid, this, (ViewContext*)this, NULL);
}

Intent* TipDlg::onDestroy(ContextStream* contextstream)
{
    return NULL;
}

void TipDlg::showDlg(void)
{
    CustomEvent event(Event::CUSTOM_NOTIFY, CustomEvent::CUS_DLG_SHOW, (HTData)this);
    raiseEvent(&event);
    setActiveWindow(viewWindow());
}

void TipDlg::hideDlg(void)
{
    CustomEvent event(Event::CUSTOM_NOTIFY, CustomEvent::CUS_DLG_HIDE, (HTData)this);
    raiseEvent(&event);
    hide();
}

int TipDlg::closeDlg(int endCode)
{
    Menu *menu = Menu::getCurrentMenu();

    if (menu) {
        setActiveWindow(menu->viewWindow());
    } else {
        Activity* act = (Activity*)(ActivityManager::getInstance()->getCurrentActivity());
        setActiveWindow(act->viewWindow());
    }

    CustomEvent event(Event::CUSTOM_NOTIFY, CustomEvent::CUS_DLG_HIDE, (HTData)this);
    raiseEvent(&event);

    if(m_timerId)
        removeTimer(m_timerId);

    endDlg(endCode);

    return 0;
}

void TipDlg::resetRect(int left, int top, int right, int bottom)
{
    setRect(left, top, right, bottom);
    IntRect rc(left, top, right, bottom);
    setWindowRect(rc);
}

bool TipDlg::onKey(int keyCode, KeyEvent* event)
{
    if (event->eventType() != Event::KEY_UP)
        return true;

    switch (keyCode) {
        case KeyEvent::KEYCODE_ENTER:
        case KeyEvent::KEYCODE_SOFTKEY_LEFT:
        {
            onRespondCodeSL();
            break;
        }

        case KeyEvent::KEYCODE_SOFTKEY_RIGHT:
        {
            onRespondCodeSR();
            break;
        }

        default:
            closeDlg(TIP_CANCEL);
            break;
    }

    return true;
}

void TipDlg::setText(string s_text)
{
    if (NULL != m_textView)
        m_textView->setText(s_text);
}

string TipDlg::text()
{
    static string nullstr = "";
    if (NULL == m_textView) {
        return nullstr;
    }

    return m_textView->getText();
}

void TipDlg::onRespondCodeSL()
{
    closeDlg(TIP_OK);
}

void TipDlg::onRespondCodeSR()
{
    removeTimer(m_timerId);
    closeDlg(TIP_CANCEL);
}

int TipDlg::doModal()
{
    if (m_autoCloseTime > 0) {
        if (m_timerId > 0)
            removeTimer(m_timerId);
        m_timerId = registerTimer(m_autoCloseTime * 1000, "TipDlg");
    }
    return Window::doModal(true);
}

void TipDlg::setAutoCloseTime(int time)
{
    if (time <= 0) {
        m_autoCloseTime = 0;
        return;
    }
    m_autoCloseTime = time;
}

bool TipDlg::handleEvent(Event* event)
{
    switch (event->eventType()) {
        case Event::TIMER: {
            if(m_timerId == ((TimerEvent *)event)->timerID())
            {
                if(m_timerId)
                {
                    removeTimer(m_timerId);
                    closeDlg(TIP_CANCEL);
                }
            }

            break;
        }

        default:
            break;
    }

    return DISPATCH_CONTINUE_MSG;
}

BEGIN_SETVIEW(TipDlg)
    MAP_VIEW(m_textView, INFO_TEXT, TextView)
END_SETVIEW

BEGIN_GETHANDLE(TipDlg)
END_GETHANDLE

DEFINE_UI_TEMPL(TipDlg)


