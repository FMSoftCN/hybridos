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

#ifndef HFCL_ACTIVITY_ACTIVITYWITHCLIENTS_H_
#define HFCL_ACTIVITY_ACTIVITYWITHCLIENTS_H_

#include "../common/event.h"
#include "../services/timerservice.h"

#include "../activity/intent.h"
#include "../activity/activity.h"
#include "../activity/controller.h"

namespace hfcl {

class ActivityWithClients;

class ActivityClient : public ControllerClient {
public:
    ActivityClient(Controller* owner, int view_id, View* parent)
        : ControllerClient(owner, view_id, parent, 0, 0) { }

    /* overloaded virtual functions */
    virtual bool isTip() { return false; }

    /* public operators */
    unsigned int showTipView (int view_id, HTData param1, HTData param2) {
        if(m_owner)
            return ((ActivityWithClients *)m_owner)->showTipView (view_id,
                    param1, param2);
        return 0;
    }
    void exitTip(int endCode = TipDlg::TIP_DESTROY) {
        if (m_owner)
            ((ActivityWithClients *)m_owner)->exitTip(endCode);
    }

    void exitHome() {
        if (m_owner)
            ((ActivityWithClients *)m_owner)->exitHome();
    }
};

class TipClient : public ActivityClient {
public:
    TipClient(Controller* owner, int view_id, View*parent)
        : ActivityClient(owner, view_id, parent) { }
    virtual ~TipClient() {}

    /* overloaded virtual functions */
    virtual bool isTip() { return true; }

    virtual void active() {
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
};

class ActivityWithClients : public FullScreenActivity,
                            public TimerEventListener {
public:
    ActivityWithClients ();
    ActivityWithClients (bool fullscreen);
    virtual ~ActivityWithClients();

    /* overloaded virtual functions */
    virtual void onCreate (ContextStream* contextstream, Intent* intent);
    virtual void onWakeup () {
        if (getTopActivityClient() == NULL) {
            ActivityManager* act_mgr = ActivityManager::getInstance();
            BaseActivity *back = act_mgr->getTopActivity(1);
            if (back != NULL) {
                back->onWakeup();
            }
        }

        FullScreenActivity::onWakeup();
    }

    virtual void onSleep() {
        closeTimer();
    }

    virtual Intent *onDestroy(ContextStream *contextStream) {
        return NULL;
    }

    // when open pop, this function will be call.
    virtual void hold() {}

    // when close(exit) pop, this function will be call.
    virtual void resume() {}

    virtual unsigned int onClientCommand(int sender,
            unsigned int cmd_id, HTData param1, HTData param2);

    bool handleEvent(Event * event);
    virtual bool onKey(int keyCode, KeyEvent* event);

    /* public operators */
    virtual unsigned int showView(int view_id, HTData param1, HTData param2);
    virtual unsigned int backView(unsigned int endcode = 0);
    unsigned int showModalView(int view_id, HTData param1, HTData param2);
    unsigned int showTipView(int view_id, HTData param1, HTData param2);
    virtual void exitTip(int endCode = TipDlg::TIP_DESTROY);

    bool isFullScreen() { return m_isFullScreen; }
    virtual void setFullScreen(bool bfullScreen);
    void setFullScreen(bool bfullScreen, bool bhidemenubar);

    virtual void exitHome();
    virtual void exitCall();

    ActivityClient *getTopClient(bool isTip);
    ActivityClient *getTopActivityClient() {
        return getTopClient(false);
    }

    TipClient *getTopTipClient() {
        return (TipClient *)getTopClient(true);
    }

protected:
    void openTimer();
    void closeTimer();

    virtual bool doCommand(int cmd);

    DECLARE_CONTROLLER_CLIENTS

protected:
    bool            m_isFullScreen;
    int             m_timerId;
    ActivityClient *m_keyDownTarget;
    bool            m_tipShown;
};

} // namespace hfcl

#endif // HFCL_ACTIVITY_ACTIVITYWITHCLIENTS_H_

