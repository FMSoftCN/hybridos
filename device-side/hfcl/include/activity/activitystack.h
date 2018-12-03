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


#ifndef _HFCL_AppStack_h
#define _HFCL_AppStack_h

#include "nguxcommon.h"

namespace hfcl {
class ActivityInfo;

class BaseActivity;

class AppStack
{
	private:
		LIST(ActivityInfo*, AppInfoList);
		AppInfoList m_apps;
		AppInfoList m_apps_runbackground;
	
    public:
		AppStack(){ }
		virtual ~AppStack(){ }
		
        void push(ActivityInfo *app);
		void pop();
		bool isEmpty();
		bool isExist(BaseActivity *app);
		bool remove(BaseActivity *app);
		bool move2Top(BaseActivity *app);
		bool move2Bottom(BaseActivity *app);
		bool pushBackgroundRunningApp(BaseActivity *app);
		bool popBackgroundRunningApp(BaseActivity *app);
		void clear();
		int size();
		ActivityInfo* top(int n = 0);
		ActivityInfo* bottom(int n = 0);
		BaseActivity* getExistAppByName(const char * name);
		BaseActivity* getExistAppRunBackgroundByName(const char * name);
};

} // namespace hfcl

#endif /* _HFCL_AppStack_h */
