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
    ActivityClient(Controller* owner, int view_id, RootView* root)
        : ControllerClient(owner, view_id, root, 0, 0) { }

    /* overloaded virtual functions */
    virtual bool isTip() { return false; }
};

class TipClient : public ActivityClient {
public:
    TipClient(Controller* owner, int view_id, RootView* root)
        : ActivityClient(owner, view_id, root) { }
    virtual ~TipClient() {}

    /* overloaded virtual functions */
    virtual bool isTip() { return true; }

    virtual void active();
};

class ActivityWithClients : public FullScreenActivity,
                            public TimerEventListener {
public:
    enum ActCmd {
        ACT_CMD_EXIT,
        ACT_CMD_BACK,
        ACT_CMD_OPTIONS,
        ACT_CMD_OK,
        ACT_CMD_CANCEL,
    };

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

    virtual unsigned int onClientCommand(int sender,
            unsigned int cmd_id, HTData param1, HTData param2) {
        return 0;
    }

    /* new virtual functions specific to ActivityWithClients */
    virtual void onCmdOk() { }
    virtual void onCmdCancel() { }
    virtual void onCmdOptions() { }

    // when open pop, this function will be call.
    virtual void hold() {}

    // when close(exit) pop, this function will be call.
    virtual void resume() {}

    virtual void drawBackground(GraphicsContext* gc, IntRect &rc);
    bool handleEvent(Event * event);
    virtual bool onKey(int keyCode, KeyEvent* event);

    /* public operators */
    virtual unsigned int showView(int view_id, HTData param1, HTData param2);
    virtual unsigned int backView(unsigned int endcode = 0);

    unsigned int showModalView(int view_id, HTData param1, HTData param2);
    unsigned int showTipView(int view_id, HTData param1, HTData param2);
    virtual void exitTip(int endCode);

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
    ActivityClient *m_keyDownTarget;
    bool            m_tipShown;
    int             m_timerId;
};

} // namespace hfcl

#endif // HFCL_ACTIVITY_ACTIVITYWITHCLIENTS_H_

