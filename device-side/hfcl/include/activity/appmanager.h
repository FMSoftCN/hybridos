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


#ifndef _NGUX_AppManager_h
#define _NGUX_AppManager_h

#include "mgcl.h"
#include "nguxcommon.h"
#include "appstack.h"

NAMESPACE_BEGIN

class Intent;
class BaseApp;
class AppFactory;

/***
 * hook key msg from appManager
 *
 * @param MSG *msg - msg we get from msg queue
 * @return NGBool - DISPATCH_CONTINUE_MSG continue message loop, DISPATCH_STOP_MSG stop for process this msg.
 */
typedef NGBool (*KeyHookCallback)(MSG *msg);

class AppManager 
{
public:
    MAPCLASSKEY(string, AppFactory*, AppFactoryMap); 
    PAIR(string, AppFactory*, AppFactoryPair); 

	AppManager() : m_hostingWnd(HWND_INVALID) { init(); }
	// AppManager() : m_hostingWnd(HWND_INVALID) { };
	~AppManager();

	static AppManager* getInstance(void);
	void run(void);
	
    BaseApp* getCurrentApp(void);
	BaseApp* getTopApp(NGUInt top = 0);
	BaseApp* getAppByName(NGCPStr name);
	BaseApp* getAppFromFactory(string name);
	inline AppFactory* getAppFactory(string name) { return m_apps[name]; }
    inline NGInt appNumOnRun(void) { return m_appstack.size(); }
    const AppFactoryMap& applications() const { return m_apps; }

	void registerApp(string name, AppFactory *appFactory);

	bool appIsExist(BaseApp *obj);
	bool appIsExist(const NGChar * appName);
	
    void onBoot();
    BaseApp* startApp(string app_name, Intent *intent);
    NGBool exit(BaseApp* app);
	bool isExist(BaseApp *app) { return m_appstack.isExist(app); }
	bool moveApp2Top(NGCPStr name);
	bool moveApp2Bottom(NGCPStr name);
	bool isAppRunningInBackground(NGCPStr name);
	bool pushAppRunningInBackground(NGCPStr name);
	BaseApp* popAppRunningToFrontdesk(NGCPStr name);

    void changeSysLanguage(NGInt langId);
	void addDisableLockFrameTick(void) { m_disableLockTick ++; }
	void subDisableLockFrameTick(void) { m_disableLockTick --; if(m_disableLockTick<0) m_disableLockTick = 0;}
    NGInt disableLockFrameTicks() { return m_disableLockTick; }
	
    inline HWND hosting(void) { return m_hostingWnd; }
	inline NGBool AppBeStarted(BaseApp* app) { return m_appstack.isExist(app); }
	NGInt broadcastMessage(NGInt msg, NGInt wParam, NGInt lParam);
    
    void registerKeyMsgHook(KeyHookCallback callback) {
    	m_key_hook = callback;
    }
    /***
     * hook key
     *
     * @param MSG *msg - msg we get from msg queue.
     *
     * @return NGBool - DISPATCH_CONTINUE_MSG continue message loop, 
     *                - DISPATCH_STOP_MSG stop for process this msg.
     ***/
    NGBool processKeyHook(MSG* msg);

    void startTimerService(void);
    void stopTimerService(void);

    void freezeChar(NGBool f) { m_charFreezon = f; }
    NGBool isCharFreezon() {return m_charFreezon; }
    
    NGInt m_gValue;
    NGBool m_standby;

private:
	NGBool init(void);
    static NGInt defaultHostingProc(HWND hWnd, 
            NGInt message, WPARAM wParam, LPARAM lParam);

    HWND m_hostingWnd;
    AppStack m_appstack;
    static AppManager* s_appManager;

    AppFactoryMap m_apps;
    KeyHookCallback m_key_hook;
    NGBool m_charFreezon;
	NGInt  m_disableLockTick;
};

NAMESPACE_END

#endif /* NGUX_AppManager_h */
