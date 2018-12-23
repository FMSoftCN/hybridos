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

#undef _DEBUG

#include "activity/activitymanager.h"

#include "activity/baseactivity.h"
#include "activity/activity.h"
#include "activity/activityinfo.h"
#include "activity/activityfactory.h"
#include "activity/activitystack.h"
#include "services/timerservice.h"

#if 0
#include "mgcl/mgcl.h"

#include "common/log.h"
#include "contextstream.h"
#include "lcdservice.h"
#include "multimedia.h"
#include "actlication.h"
#endif

namespace hfcl {

ActivityManager* ActivityManager::s_actManager = NULL;

ActivityManager* ActivityManager::getInstance()
{
    if (s_actManager == NULL)
        s_actManager = HFCL_NEW_EX(ActivityManager, ());
    return s_actManager;
}

BaseActivity* ActivityManager::getActivityByName(const char * name)
{
    return m_actstack.getExistActivityByName(name);
}

BaseActivity* ActivityManager::getCurrentActivity()
{
    return getTopActivity(0);
}

BaseActivity* ActivityManager::getTopActivity(unsigned int iTop)
{
    ActivityInfo *ai = m_actstack.top(iTop);
    return (NULL != ai) ? ai->getActivity() : NULL;
}

bool ActivityManager::exit(BaseActivity *p_act)
{
    ActivityInfo *ai = NULL;

    // check the actlication is exist ?
    if ( NULL == (ai = m_actstack.top())
            || ( ! m_actstack.isExist(p_act))
            || strcmp(p_act->name(), "launcher") == 0 )
    {
        return false;
    }
    _DBG_PRINTF ("Activitymanager :: exit () ---- exit act [%s]", p_act->name());
    // exit the actlication
    p_act->onDestroy(NULL);
    m_actstack.remove(p_act);

    // restore another actlication
    if (NULL != (ai = m_actstack.top()))
    {
        _DBG_PRINTF ("Activitymanager :: exit () ---- restore the pop act [%s] ", ai->getActivity()->name());
        switch (ai->getActivity()->state()) {
            case BaseActivity::RUNNING:
                // Do nothing
                break;
            case BaseActivity::SLEEP:
            {
                Activity* act = (Activity*)(ai->getActivity());
                act->setState(BaseActivity::RUNNING);
                act->onWakeup();
            }
                break;
            case BaseActivity::SUSPEND:
                {
                    BaseActivity *_act = getActivityFromFactory(ai->getName());
                    if (_act != NULL){
                        ai->setActivity(_act);
                        _act->setState(BaseActivity::RUNNING);
                        _act->onCreate(NULL, NULL);
                        _act->onStart();
                    }
                    break;
                }
            default:
                break;
        }
    }

    return true;
}

bool ActivityManager::actIsExist(BaseActivity *obj)
{
    return m_actstack.isExist(obj);
}

bool ActivityManager::actIsExist(const char * actName)
{
    return getActivityByName(actName) != NULL;
}

bool ActivityManager::moveActivity2Top(const char * name)
{
    BaseActivity* act = NULL;
    ActivityInfo* curActivity = NULL;

    if(NULL == (act = getActivityByName(name))) {
        return false;
    }

    if(NULL != (curActivity = m_actstack.top())
            && curActivity->getName() == name){
        return true;
    }

    if (curActivity->getActivity()->state() == BaseActivity::RUNNING)
    {
        curActivity->getActivity()->setState(BaseActivity::SLEEP);
        curActivity->getActivity()->onSleep();
    }

    if(m_actstack.move2Top(act))
    {
        act->onMove2Top();

        if (act->state() == BaseActivity::SLEEP)
        {
            act->setState(BaseActivity::RUNNING);
            act->onWakeup();
        }

        return true;
    }
    return false;
}


bool ActivityManager::moveActivity2Bottom(const char * name)
{
    BaseActivity* act = NULL;
    ActivityInfo* curActivity = NULL;

    if(NULL == (act = getActivityByName(name))) {
        return false;
    }

    if(NULL != (curActivity = m_actstack.bottom())
            && curActivity->getName() == name){
        return true;
    }

    if (act->state() == BaseActivity::RUNNING)
    {
        act->setState(BaseActivity::SLEEP);
        act->onSleep();
    }

    if(m_actstack.move2Bottom(act))
    {
        if(NULL == (curActivity = m_actstack.top())){
            return false;
        }
          if(NULL == (act = curActivity->getActivity())) {
            return false;
        }
        act->onMove2Top();

        if (act->state() == BaseActivity::SLEEP)
        {
            act->setState(BaseActivity::RUNNING);
              act->onWakeup();
        }

        return true;
    }
    return false;
}

bool ActivityManager::pushActivityRunningInBackground(const char * name)
{
    BaseActivity* act = NULL;
    ActivityInfo* curActivity = NULL;

    if(NULL == (act = getActivityByName(name))) {
        return false;
    }

    if(NULL != (curActivity = m_actstack.bottom())
            && curActivity->getName() == name){
        return true;
    }

    if (act->state() == BaseActivity::RUNNING)
    {
        act->setState(BaseActivity::SLEEP);
        act->onSleep();
    }

    if(m_actstack.pushBackgroundRunningActivity(act))
    {
          if(NULL == (curActivity = m_actstack.top()) ){
            return false;
        }
          if(NULL == (act = curActivity->getActivity())) {
            return false;
        }
        act->onMove2Top();

        if (act->state() == BaseActivity::SLEEP)
        {
            act->setState(BaseActivity::RUNNING);
              act->onWakeup();
        }

        return true;
    }
    return false;
}


BaseActivity* ActivityManager::popActivityRunningToFrontdesk(const char * name)
{
    BaseActivity* act = NULL;
    ActivityInfo* curActivity = NULL;

    if(NULL == (curActivity = m_actstack.top())){
        return NULL;
    }
    if(NULL == (act = curActivity->getActivity())) {
        return NULL;
    }
    if (act->state() == BaseActivity::RUNNING)
    {
        act->setState(BaseActivity::SLEEP);
        act->onSleep();
    }

    if(m_actstack.popBackgroundRunningActivity(m_actstack.getExistActivityRunBackgroundByName(name)))
    {
          if(NULL == (curActivity = m_actstack.top()) ){
            return NULL;
        }
          if(NULL == (act = curActivity->getActivity())) {
            return NULL;
        }
        act->onMove2Top();

        if (act->state() == BaseActivity::SLEEP)
        {
            act->setState(BaseActivity::RUNNING);
              act->onWakeup();
        }

        return act;
    }
    return NULL;
}

bool ActivityManager::isActivityRunningInBackground(const char * name)
{
    return m_actstack.getExistActivityRunBackgroundByName(name) != NULL;
}

void ActivityManager::onBoot()
{
    ActivityFactoryMap::iterator it;
    for (it = m_acts.begin(); it != m_acts.end(); ++it) {
        _DBG_PRINTF ("ActivityManager::onBoot: calling onBoot of act %s", (*it).first.c_str());
        const ActivityFactory::ActivityData* act_info = (*it).second->getActivityData ();
        if (act_info && act_info->onBoot) {
            act_info->onBoot ();
        }
    }
}

BaseActivity* ActivityManager::startActivity(string act_name, Intent *intent)
{
    ActivityInfo* _top = NULL;
    ActivityInfo* _curTopActivityInfo = NULL;
    BaseActivity* _newActivity = NULL;

    _DBG_PRINTF ("Activitymanager :: startActivity () ----  act [%s]", act_name.c_str());
    _newActivity = getActivityFromFactory(act_name);
    _curTopActivityInfo = m_actstack.top();

    if(_curTopActivityInfo != NULL)
    {
        BaseActivity* _tact = _curTopActivityInfo->getActivity();

        if (_newActivity == NULL /*|| _newActivity->priority() < _tact->priority()*/)
            return NULL;

        if (_tact->state() == BaseActivity::RUNNING) {
            _tact->setState(BaseActivity::SLEEP);
            _tact->onSleep();
        }
    }

    if(_newActivity != NULL)
    {
        //ContextStream *cs = HFCL_NEW_EX(ContextStream, ());
        ActivityInfo *act_info = HFCL_NEW_EX(ActivityInfo, (act_name, _newActivity, NULL));

        _newActivity->setState(BaseActivity::RUNNING);
        _newActivity->setName(act_name.c_str());
        m_actstack.push(act_info);

        _newActivity->onCreate(NULL, intent);

        if(NULL != (_top = m_actstack.top()) && _top->getActivity() == _newActivity)
        {
            _newActivity->onStart();
        }
    }
    return _newActivity;
}

void ActivityManager::registerActivity(string name, ActivityFactory *actfactory)
{
    m_acts[name] = actfactory;
}

BaseActivity* ActivityManager::getActivityFromFactory(string name)
{
    ActivityFactoryMap::iterator it;
    for (it = m_acts.begin(); it != m_acts.end(); ++it) {
        if ((*it).first == name) {
            BaseActivity *act = (*it).second->create();
            act->setState(BaseActivity::ORIGIN);
            return act;
        }
    }
    return NULL;
}

typedef struct
{
    int     wSrcId;
    int     wDestId;
    int     wMsgId;
    int     wSapId;
    void    *wDataPtr;
    void    *wPeerBuffPtr;
} MYQUEUE;

LRESULT ActivityManager::defaultHostingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            break;
        default:
            break;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int ActivityManager::broadcastMessage(int msg, int wParam, int lParam)
{
    return BroadcastMessage(msg, (WPARAM)wParam, (WPARAM)lParam);
}

bool ActivityManager::init()
{
    if (m_hostingWnd != HWND_INVALID)
        return false;

    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "backend window manager";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = defaultHostingProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 0;
    CreateInfo.by = 0;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    m_hostingWnd = CreateMainWindow(&CreateInfo);

    if (m_hostingWnd == HWND_INVALID) {
        return false;
    }
    ShowWindow(m_hostingWnd, SW_SHOWNORMAL);

    m_key_hook = NULL;
    m_charFreezon = true;
    m_gValue = 0;
    m_standby = false;
    m_disableLockTick = 0;

    return true;
}

bool ActivityManager::processKeyHook(MSG* msg)
{
    if (NULL != m_key_hook) {
        return m_key_hook(msg);
    }

    return DISPATCH_CONTINUE_MSG;
}

void ActivityManager::run(void)
{
    MSG Msg;
    while (TRUE) {
        if (GetMessage(&Msg, m_hostingWnd)) {
            // update lcd service and key tone FIXME
            if (Msg.hwnd == HWND_DESKTOP
                && ( Msg.message == MSG_KEYDOWN
                   || Msg.message == MSG_KEYUP
                   || Msg.message == MSG_KEYLONGPRESS
                   || Msg.message == MSG_KEYALWAYSPRESS)) {
                // stop process
                if(DISPATCH_STOP_MSG == processKeyHook(&Msg))
                    continue;
            }

            if (!m_charFreezon)
                TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        else
            break;
    }
}

void ActivityManager::startTimerService(void)
{
    TimerService::getInstance()->start();
}

void ActivityManager::stopTimerService(void)
{
    TimerService::getInstance()->stop();
}

} // namespace hfcl

