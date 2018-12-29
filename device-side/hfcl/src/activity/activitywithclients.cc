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

#include "activity/activitywithclients.h"

namespace hfcl {

void TipClient::active()
{
    if (m_owner->getClientCount() == 0) {
        ActivityManager* act_mgr = ActivityManager::getInstance();
        Activity *act = (Activity *)(act_mgr->getTopActivity(1));
        if (act != NULL) {
            act->show();
            act->updateWindow();
        }
    }
    ActivityClient::active();
}

ActivityWithClients::ActivityWithClients ()
    : TimerEventListener (1)
    , m_isFullScreen(false)
    , m_keyDownTarget(NULL)
    , m_tipShown(false)
    , m_timerId(0)
{
}

ActivityWithClients::ActivityWithClients (bool fullscreen)
    : TimerEventListener(1)
    , m_isFullScreen(fullscreen)
    , m_keyDownTarget(NULL)
    , m_tipShown(false)
    , m_timerId(0)
{
}

ActivityWithClients::~ActivityWithClients()
{
    closeTimer();
}

void ActivityWithClients::onCreate(ContextStream* contextStream, Intent* intent)
{
    FullScreenActivity::onCreate(contextStream, intent);
}

void ActivityWithClients::openTimer()
{
    if(m_timerId == 0) {
        m_timerId = registerTimer(50, "ActivityWithClients");
    }
}

void ActivityWithClients::closeTimer()
{
    if (m_timerId != 0)
        removeTimer(m_timerId);
    m_timerId = 0;
}

bool ActivityWithClients::doCommand(int cmd)
{
    switch(cmd) {
    case ACT_CMD_EXIT:
        exit();
        break;

    case ACT_CMD_BACK:
        backView();
        break;

    case ACT_CMD_OPTIONS:
        onCmdOptions();
        break;

    case ACT_CMD_OK:
        onCmdOk();
        break;

    case ACT_CMD_CANCEL:
        onCmdCancel();
        break;

    default: {
        ControllerClient *client = getTop(0);
        if (client) {
            client->onControllerCommand(cmd, 0, 0);
        }
        break;
    }
    }
    return STOP_DISPATCH;
}

unsigned int ActivityWithClients::showView (int view_id,
        HTData param1, HTData param2)
{
    unsigned int ret = Activity::showView(view_id, param1, param2);
    if (ret == 1) {
        return ret;
    }

    ActivityClient *clt = (ActivityClient *)getTop();
    if (NULL != clt) {
        RootView *root = clt->getRootView();
        if (NULL != root) {
            if (clt->isTip()) {
                ActivityClient *_lower = NULL;
                for(int i = 1; (_lower = (ActivityClient *)getTop(i)) != NULL; i++){
                    _lower->getRootView()->show();
                    if (!_lower->isTip()) {
                        break;
                    }
                }
            }
        }
    }

    return ret;
}

unsigned int ActivityWithClients::backView (unsigned int endCode)
{
    ActivityClient *_clt_top = (ActivityClient *)getTop(0);
    ActivityClient *_under = (ActivityClient *)getTop(1);

    if (_clt_top != NULL && _clt_top->isTip() && _under == NULL) {
        // only one tipdlg client
        // backview
        // don't exit...
        _clt_top->onBackView(endCode);
        _clt_top->inactive();
        if(m_modalCount > 0) {
            if(_clt_top && _clt_top->isModal()) {
                endDlg(endCode);
                m_modalCount --;
            }
        }

        _clt_top->cleanRootView();
        pop(1);
        return 0;
    }

    if (_clt_top == NULL) {
        return Activity::backView(endCode);
    }

    return Activity::backView(endCode);
}

unsigned int ActivityWithClients::showTipView(int view_id, HTData param1, HTData param2)
{
    unsigned int ret = (unsigned int)-1;

    if (!m_tipShown) {
        m_tipShown = true;
        ret = showModalView(view_id, param1, param2);
        m_tipShown = false;
    }
    else {
        _ERR_PRINTF ("ActivityWithClients::showTipView: TipView re-entrance occurred\n");
    }

    return ret;
}

void ActivityWithClients::exitTip(int endCode)
{
    ActivityClient *_ctop = (ActivityClient *)getTop(0);

    if(_ctop != NULL && _ctop->isTip()) {
        if (ActivityManager::getInstance()->getCurrentActivity() == this) {
            backView(endCode);
        }
        else {
            deleteView(_ctop->getId());
        }
    }
}

ActivityClient *ActivityWithClients::getTopClient(bool bPop)
{
    int count = getClientCount();

    for (int i = 0; i < count; i++) {
        ActivityClient *_clt = (ActivityClient *)getTop(i);
        if ((bPop && _clt->isTip())
            || (!bPop && !_clt->isTip()))
            return _clt;
    }

    return NULL;
}

void ActivityWithClients::drawBackground(GraphicsContext* gc, IntRect &rc)
{
    for (int i = 0; i < getClientCount(); i++) {
        ActivityClient *clt = (ActivityClient *)getTop(i);
        if (clt != NULL && !clt->isTip()) {
            Window::drawBackground(gc, rc);
            return;
        }
    }

    // If all the client are Tip client, should paint the Back Activity.
    if (ActivityManager::getInstance()->getCurrentActivity() == this) {
        Activity *act = (Activity *)(ActivityManager::getInstance()->getTopActivity(1));
        if (act != NULL) {
            IncludeWindowStyle(act->getSysWindow(), WS_VISIBLE);
            ShowWindow(act->getSysWindow(), SW_SHOWNORMAL);
            act->syncUpdateRect(rc.left(), rc.top(), rc.width(), rc.height(), false);
            ShowWindow(act->getSysWindow(), SW_HIDE);
            setActiveWindow(getSysWindow());
        }
    }
}

bool ActivityWithClients::onKeyEvent(const KeyEvent* event)
{
    return Window::onKeyEvent(event);
}

bool ActivityWithClients::onMouseEvent(const MouseEvent* event)
{
    return Window::onMouseEvent(event);
}

bool ActivityWithClients::onMouseWheelEvent(const MouseWheelEvent* event)
{
    return Window::onMouseWheelEvent(event);
}

unsigned int ActivityWithClients::showModalView(int view_id, HTData param1, HTData param2)
{
    return Activity::showModalView (view_id, param1, param2);
}

bool ActivityWithClients::handleEvent(Event* event)
{
    if (event->eventType() == Event::ET_TIMER
                && m_timerId == ((TimerEvent*)event)->timerID()) {
            closeTimer();
        return STOP_DISPATCH;
    }

    return GOON_DISPATCH;
}

} // namespace hfcl

