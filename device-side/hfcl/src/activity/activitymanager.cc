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

#include "mgcl.h"

#include "log.h"
#include "contextstream.h"
#include "baseapp.h"
#include "appinfo.h"
#include "image.h"
#include "appfactory.h"
#include "appmanager.h"
#include "timerservice.h"
#include "lcdservice.h"
#include "multimedia.h"
#include "nguxmenu.h"
#include "application.h"

#include "hal_misc.h"

#ifdef ENABLE_AUTOTEST
extern void RecordCurrKeyHandler(S16 keyCode, S16 keyType);
#endif

namespace hfcl {

ActivityManager* ActivityManager::s_appManager = NULL;

ActivityManager* ActivityManager::getInstance()
{
    if (s_appManager == NULL)
        s_appManager = HFCL_NEW_EX(ActivityManager, ());
    return s_appManager;
}

BaseActivity* ActivityManager::getAppByName(const char * name)
{
	return m_appstack.getExistAppByName(name);
}

BaseActivity* ActivityManager::getCurrentApp()
{
    return getTopApp(0);
}

BaseActivity* ActivityManager::getTopApp(unsigned int iTop)
{
	ActivityInfo *ai = m_appstack.top(iTop);
    return (NULL != ai) ? ai->getApp() : NULL;
}

bool ActivityManager::exit(BaseActivity *p_app)
{
    ActivityInfo *ai = NULL;
	
    // check the application is exist ?
    if ( NULL == (ai = m_appstack.top())
			|| ( ! m_appstack.isExist(p_app))
			|| strcmp(p_app->name(), "launcher") == 0 )
    {   
        return false;
    }
	_DBG_PRINTF ("Appmanager :: exit () ---- exit app [%s]", p_app->name());
	// exit the application
	p_app->onDestroy(NULL);	
    m_appstack.remove(p_app);

    // restore another application
    if (NULL != (ai = m_appstack.top()))
    {	
    	_DBG_PRINTF ("Appmanager :: exit () ---- restore the pop app [%s] ", ai->getApp()->name());
        switch (ai->getApp()->state()) {
            case BaseActivity::RUNNING:
                // Do nothing
                break;
            case BaseActivity::SLEEP:
			{
				Menu *m = NULL;
				Activity* app = (Activity*)(ai->getApp());
				app->setState(BaseActivity::RUNNING);
                app->onWakeup();

				if(strcmp(app->name(), "sudoku") != 0 && (m = app->menu()) != NULL && ! m->isProcessing()) {
					m->showMenu(app);
				}
            }		
                break;
            case BaseActivity::SUSPEND: 
                {
                    BaseActivity *_app = getAppFromFactory(ai->getName());
					if (_app != NULL){
						ai->setApp(_app);
						_app->setState(BaseActivity::RUNNING);
                    	_app->onCreate(NULL, NULL);
                    	_app->onStart();
					}
                    break;
                }
            default:
                break;
        }
    }

    return true;
}

bool ActivityManager::appIsExist(BaseActivity *obj)
{
    return m_appstack.isExist(obj);
}

bool ActivityManager::appIsExist(const char * appName)
{
	return getAppByName(appName) != NULL;
}

bool ActivityManager::moveApp2Top(const char * name)
{
	BaseActivity* app = NULL;
	ActivityInfo* curApp = NULL;

	if(NULL == (app = getAppByName(name))) {
		return false;
	}
        
	if(NULL != (curApp = m_appstack.top()) 
            && curApp->getName() == name){
		return true;
    }
	
    if (curApp->getApp()->state() == BaseActivity::RUNNING) 
    {
        curApp->getApp()->setState(BaseActivity::SLEEP);
        curApp->getApp()->onSleep();
    }

    if(m_appstack.move2Top(app))
    {
        app->onMove2Top();
        
        if (app->state() == BaseActivity::SLEEP)
        {
            app->setState(BaseActivity::RUNNING);
			app->onWakeup();
        }
        
        return true;
    }
    return false;
}


bool ActivityManager::moveApp2Bottom(const char * name)
{
	BaseActivity* app = NULL;
	ActivityInfo* curApp = NULL;

	if(NULL == (app = getAppByName(name))) {
		return false;
	}
        
	if(NULL != (curApp = m_appstack.bottom()) 
            && curApp->getName() == name){
		return true;
    }
	
    if (app->state() == BaseActivity::RUNNING) 
    {
        app->setState(BaseActivity::SLEEP);
        app->onSleep();
    }

    if(m_appstack.move2Bottom(app))
    {
		if(NULL == (curApp = m_appstack.top())){
			return false;
    	}
	  	if(NULL == (app = curApp->getApp())) {
			return false;
		}
        app->onMove2Top();
        
        if (app->state() == BaseActivity::SLEEP)
        {
            app->setState(BaseActivity::RUNNING);
	      	app->onWakeup();
        }
        
        return true;
    }
    return false;
}

bool ActivityManager::pushAppRunningInBackground(const char * name)
{
	BaseActivity* app = NULL;
	ActivityInfo* curApp = NULL;

	if(NULL == (app = getAppByName(name))) {
		return false;
	}
        
	if(NULL != (curApp = m_appstack.bottom()) 
            && curApp->getName() == name){
		return true;
    }
	
    if (app->state() == BaseActivity::RUNNING) 
    {
        app->setState(BaseActivity::SLEEP);
        app->onSleep();
    }

    if(m_appstack.pushBackgroundRunningApp(app))
    {
	  	if(NULL == (curApp = m_appstack.top()) ){
			return false;
    	}
	  	if(NULL == (app = curApp->getApp())) {
			return false;
		}
        app->onMove2Top();
        
        if (app->state() == BaseActivity::SLEEP)
        {
            app->setState(BaseActivity::RUNNING);
	      	app->onWakeup();
        }
        
        return true;
    }
    return false;
}


BaseActivity* ActivityManager::popAppRunningToFrontdesk(const char * name)
{
	BaseActivity* app = NULL;
	ActivityInfo* curApp = NULL;

	if(NULL == (curApp = m_appstack.top())){
		return NULL;
    }
	if(NULL == (app = curApp->getApp())) {
		return NULL;
	}
    if (app->state() == BaseActivity::RUNNING) 
    {
        app->setState(BaseActivity::SLEEP);
        app->onSleep();
    }

    if(m_appstack.popBackgroundRunningApp(m_appstack.getExistAppRunBackgroundByName(name)))
    {
	  	if(NULL == (curApp = m_appstack.top()) ){
			return NULL;
    	}
	  	if(NULL == (app = curApp->getApp())) {
			return NULL;
		}
        app->onMove2Top();
        
        if (app->state() == BaseActivity::SLEEP)
        {
            app->setState(BaseActivity::RUNNING);
	      	app->onWakeup();
        }
        
        return app;
    }
    return NULL;
}

bool ActivityManager::isAppRunningInBackground(const char * name)
{
	return m_appstack.getExistAppRunBackgroundByName(name) != NULL;
}

void ActivityManager::onBoot()
{
    AppFactoryMap::iterator it;
    for (it = m_apps.begin(); it != m_apps.end(); ++it) {
        _DBG_PRINTF ("ActivityManager::onBoot: calling onBoot of app %s", (*it).first.c_str());
        const ActivityFactory::AppData* app_info = (*it).second->getAppInfo ();
        if (app_info && app_info->onBoot) {
            app_info->onBoot ();
        }
    }
}

BaseActivity* ActivityManager::startApp(string app_name, Intent *intent)
{
	ActivityInfo* _top = NULL;
	ActivityInfo* _curTopAppInfo = NULL;
	BaseActivity* _newApp = NULL;

	_DBG_PRINTF ("Appmanager :: startApp () ----  app [%s]", app_name.c_str());
    _newApp = getAppFromFactory(app_name);
    _curTopAppInfo = m_appstack.top();

	if(_curTopAppInfo != NULL) 
    {
		BaseActivity* _tapp = _curTopAppInfo->getApp();
		
	    if (_newApp == NULL /*|| _newApp->priority() < _tapp->priority()*/)
	        return NULL;

	    if (_tapp->state() == BaseActivity::RUNNING) {
	        _tapp->setState(BaseActivity::SLEEP);
	        _tapp->onSleep();
	    }
	}

	if(_newApp != NULL)
    {
        //ContextStream *cs = HFCL_NEW_EX(ContextStream, ());
		ActivityInfo *app_info = HFCL_NEW_EX(ActivityInfo, (app_name, _newApp, NULL));

		_newApp->setState(BaseActivity::RUNNING);
        _newApp->setName(app_name.c_str());
        m_appstack.push(app_info);
		
		_newApp->onCreate(NULL, intent);
               
        if(NULL != (_top = m_appstack.top()) && _top->getApp() == _newApp)
        {
            _newApp->onStart();
        }
    }
    return _newApp;
}

void ActivityManager::registerApp(string name, ActivityFactory *appfactory)
{
    m_apps[name] = appfactory;
}

BaseActivity* ActivityManager::getAppFromFactory(string name)
{
    AppFactoryMap::iterator it;
    for (it = m_apps.begin(); it != m_apps.end(); ++it) {
        if ((*it).first == name) {
            BaseActivity *app = (*it).second->create();
            app->setState(BaseActivity::ORIGIN);
            return app;
        }
    }
    return NULL;
}

extern "C" void CoolSand_DispatchMMIEvent(COS_EVENT* event);

typedef struct 
{
	int     wSrcId;
	int     wDestId;
    int     wMsgId;
    int     wSapId;
    void    *wDataPtr;
    void	*wPeerBuffPtr;
} MYQUEUE;

extern "C" void CoolSand_ExecuteProtocolEvent(unsigned int eventID, 
        void* MsgStruct, int mod_src, void *peerBuf);

int ActivityManager::defaultHostingProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CLOSE:
            DestroyMainWindow(hWnd);
            break;
        case HFCL_MSG_ADP_EVENT:
            CoolSand_DispatchMMIEvent((COS_EVENT *)wParam);
            break;
		case HFCL_MSG_SEND_NETWORK_EVENT:
			{
				MYQUEUE *msg = (MYQUEUE *)wParam;
				CoolSand_ExecuteProtocolEvent((U16)msg->wMsgId, 
                        (void*)msg->wDataPtr, 
                        (int)msg->wSrcId, 
                        (void*) msg->wPeerBuffPtr);
			}
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
#ifdef ENABLE_AUTOTEST
    RecordCurrKeyHandler(msg->wParam,msg->message);
#endif

    if (NULL != m_key_hook) {
        return m_key_hook(msg);
    }

    return DISPATCH_CONTINUE_MSG;
}

void ActivityManager::run(void)
{
    MSG Msg;
#if 1
    while (TRUE) {
        GetCosEvent(m_hostingWnd);
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
#else
    while (GetMessage(&Msg, m_hostingWnd)) {
		// update lcd service and key tone FIXME
		if (Msg.hwnd == HWND_DESKTOP
			&& ( Msg.message == MSG_KEYDOWN 
			   || Msg.message == MSG_KEYUP 
			   || Msg.message == MSG_KEYLONGPRESS 
			   || Msg.message == MSG_KEYALWAYSPRESS))
        {
            // stop process
            if(DISPATCH_STOP_MSG == processKeyHook(&Msg))
                continue;
        }
		
        if (!m_charFreezon)
            TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
#endif
}

void ActivityManager::startTimerService(void)
{
    TimerService::getInstance()->start();
}

void ActivityManager::stopTimerService(void)
{
    TimerService::getInstance()->stop();
}

} // namespace hfcl {

extern "C" void nguxDispatchAdpEvent(COS_EVENT* ev, unsigned int handle, int event)
{
    SendMessage (NGUX::ActivityManager::getInstance()->hosting(), HFCL_MSG_ADP_EVENT, (WPARAM)ev, 0); 
}

