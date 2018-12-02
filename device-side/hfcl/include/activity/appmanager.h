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


#ifndef _HFCL_AppManager_h
#define _HFCL_AppManager_h

#include "mgcl.h"
#include "nguxcommon.h"
#include "appstack.h"

namespace hfcl {

class Intent;
class BaseApp;
class AppFactory;

/***
 * hook key msg from appManager
 *
 * @param MSG *msg - msg we get from msg queue
 * @return bool - DISPATCH_CONTINUE_MSG continue message loop, DISPATCH_STOP_MSG stop for process this msg.
 */
typedef bool (*KeyHookCallback)(MSG *msg);

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
	BaseApp* getTopApp(unsigned int top = 0);
	BaseApp* getAppByName(const char * name);
	BaseApp* getAppFromFactory(string name);
	inline AppFactory* getAppFactory(string name) { return m_apps[name]; }
    inline int appNumOnRun(void) { return m_appstack.size(); }
    const AppFactoryMap& applications() const { return m_apps; }

	void registerApp(string name, AppFactory *appFactory);

	bool appIsExist(BaseApp *obj);
	bool appIsExist(const char * appName);
	
    void onBoot();
    BaseApp* startApp(string app_name, Intent *intent);
    bool exit(BaseApp* app);
	bool isExist(BaseApp *app) { return m_appstack.isExist(app); }
	bool moveApp2Top(const char * name);
	bool moveApp2Bottom(const char * name);
	bool isAppRunningInBackground(const char * name);
	bool pushAppRunningInBackground(const char * name);
	BaseApp* popAppRunningToFrontdesk(const char * name);

    void changeSysLanguage(int langId);
	void addDisableLockFrameTick(void) { m_disableLockTick ++; }
	void subDisableLockFrameTick(void) { m_disableLockTick --; if(m_disableLockTick<0) m_disableLockTick = 0;}
    int disableLockFrameTicks() { return m_disableLockTick; }
	
    inline HWND hosting(void) { return m_hostingWnd; }
	inline bool AppBeStarted(BaseApp* app) { return m_appstack.isExist(app); }
	int broadcastMessage(int msg, int wParam, int lParam);
    
    void registerKeyMsgHook(KeyHookCallback callback) {
    	m_key_hook = callback;
    }
    /***
     * hook key
     *
     * @param MSG *msg - msg we get from msg queue.
     *
     * @return bool - DISPATCH_CONTINUE_MSG continue message loop, 
     *                - DISPATCH_STOP_MSG stop for process this msg.
     ***/
    bool processKeyHook(MSG* msg);

    void startTimerService(void);
    void stopTimerService(void);

    void freezeChar(bool f) { m_charFreezon = f; }
    bool isCharFreezon() {return m_charFreezon; }
    
    int m_gValue;
    bool m_standby;

private:
	bool init(void);
    static int defaultHostingProc(HWND hWnd, 
            int message, WPARAM wParam, LPARAM lParam);

    HWND m_hostingWnd;
    AppStack m_appstack;
    static AppManager* s_appManager;

    AppFactoryMap m_apps;
    KeyHookCallback m_key_hook;
    bool m_charFreezon;
	int  m_disableLockTick;
};

} // namespace hfcl {

#endif /* HFCL_AppManager_h */
