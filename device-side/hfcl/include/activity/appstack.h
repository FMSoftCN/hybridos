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


#ifndef _NGUX_AppStack_h
#define _NGUX_AppStack_h

#include "nguxcommon.h"

NAMESPACE_BEGIN
class AppInfo;

class BaseApp;

class AppStack
{
	private:
		LIST(AppInfo*, AppInfoList);
		AppInfoList m_apps;
		AppInfoList m_apps_runbackground;
	
    public:
		AppStack(){ }
		virtual ~AppStack(){ }
		
        void push(AppInfo *app);
		void pop();
		bool isEmpty();
		bool isExist(BaseApp *app);
		bool remove(BaseApp *app);
		bool move2Top(BaseApp *app);
		bool move2Bottom(BaseApp *app);
		bool pushBackgroundRunningApp(BaseApp *app);
		bool popBackgroundRunningApp(BaseApp *app);
		void clear();
		int size();
		AppInfo* top(int n = 0);
		AppInfo* bottom(int n = 0);
		BaseApp* getExistAppByName(const char * name);
		BaseApp* getExistAppRunBackgroundByName(const char * name);
};

NAMESPACE_END

#endif /* _NGUX_AppStack_h */
