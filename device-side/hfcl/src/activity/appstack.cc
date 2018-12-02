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


#include "log.h"
#include "baseapp.h"
#include "appinfo.h"
#include "appstack.h"
#include "nguxcommon.h"
#include "contextstream.h"

NAMESPACE_BEGIN

void AppStack::push(AppInfo *app)
{
	m_apps.push_back(app);
}

void AppStack::pop()
{
	m_apps.pop_back();
}

bool AppStack::isEmpty()
{
	return m_apps.empty();
}

void AppStack::clear()
{
	while(!isEmpty()) {
		AppInfo *a = top();
		pop();
		// delete a;
        NGUX_DELETE(a);
	}
}

bool AppStack::isExist(BaseApp *app)
{
	AppInfoList::iterator item;
	for(item = m_apps.begin(); item != m_apps.end(); item++)
	{
		if((*item)->getApp() == app)
			return true;
	}
	return false;
}

BaseApp* AppStack::getExistAppByName(NGCPStr name)
{
	AppInfoList::iterator item = m_apps.begin();

	for(NGInt i = 0; i < size(); i++, item++)
	{
		BaseApp *rt = (*item)->getApp();
		if (rt != NULL) {
			const char* pname = rt->name();
			if(pname != NULL && strcmp(name, pname) == 0) {
				return rt;
			}
		}
	}
	
	return NULL;
}

BaseApp* AppStack::getExistAppRunBackgroundByName(NGCPStr name)
{
	AppInfoList::iterator item;

	for(item = m_apps_runbackground.begin(); item != m_apps_runbackground.end(); item++)
	{
		BaseApp *rt = (*item)->getApp();
		if (rt != NULL) {
			const char* pname = rt->name();
			if(pname != NULL && strcmp(pname, name) == 0) {
				return rt;
			}
		}
	}
	
	return NULL;
}

bool AppStack::remove(BaseApp *app)
{
	AppInfoList::iterator item;
	for(item = m_apps.begin(); item != m_apps.end(); item++)
	{
		AppInfo *a = *item;
		if(a != NULL && a->getApp() == app)
		{
			m_apps.remove(a);
			NGUX_DELETE(a);
			return true;
		}
	}
	return false;
}

AppInfo* AppStack::top(NGInt n)
{
	if (m_apps.size() > (NGUInt)n)
    {
		if (n == 0) {
			return m_apps.back();
		} else {
			AppInfoList::iterator item;

			n = m_apps.size() - n;
			for(item = m_apps.begin(); item != m_apps.end(); item++)
			{
				if (--n == 0){
					return (*item);
				}
			}
		}
    }
    return NULL;
}

AppInfo* AppStack::bottom(NGInt n)
{
    if (m_apps.size() > (NGUInt)n)
    {
		if (n == 0) {
			return m_apps.front();
		} else {
			AppInfoList::iterator item;
			for(item = m_apps.begin(); item != m_apps.end(); item++)
			{
				if (n-- == 0){
					return (*item);
				}
			}
		}
    }
    return NULL;
}

int AppStack::size()
{
	return m_apps.size();
}

bool AppStack::move2Top(BaseApp *app)
{
	AppInfoList::iterator item;
	for(item = m_apps.begin(); item != m_apps.end(); item++)
	{
		AppInfo *_old = *item;
		if(_old != NULL && _old->getApp() == app)
		{
    		AppInfo *app_info = NGUX_NEW_EX(AppInfo, (_old->getName(), app, NULL));
   			if(app_info)
   			{
   				m_apps.erase(item);	
				// delete the old app info, but don't delete the baseapp
				_old->setApp(NULL);
				NGUX_DELETE(_old);
				m_apps.push_back(app_info);
				return true;
   			}
			return false;
		}
	}
	return false;
}

bool AppStack::move2Bottom(BaseApp *app)
{
	AppInfoList::iterator item;
	
	for(item = m_apps.begin(); item != m_apps.end(); item++)
	{
		AppInfo *_old = *item;
		if(_old != NULL && _old->getApp() == app)
		{
    		AppInfo *app_info = NGUX_NEW_EX(AppInfo, (_old->getName(), app, NULL));
   			if(app_info)
   			{
   				m_apps.erase(item);	
				// delete the old app info, but don't delete the baseapp
				_old->setApp(NULL);
				NGUX_DELETE(_old);
				m_apps.push_front(app_info);
				return true;
   			}
		}
	}
	return false;
}

bool AppStack::pushBackgroundRunningApp(BaseApp *app)
{
	AppInfoList::iterator item;
	for(item = m_apps.begin(); item != m_apps.end(); item++)
	{
		if((*item)->getApp() == app)
		{
    		AppInfo *app_info = NGUX_NEW_EX(AppInfo, ((*item)->getName(), app, NULL));
   			if(app_info)
   			{
   				m_apps.pop_back();
				m_apps_runbackground.push_back(app_info);
				return true;
   			}
		}
	}
	return false;
}

bool AppStack::popBackgroundRunningApp(BaseApp *app)
{
	AppInfoList::iterator item;
	for(item = m_apps_runbackground.begin(); item != m_apps_runbackground.end(); item++)
	{
		if((*item)->getApp() == app)
		{
    		AppInfo *app_info = NGUX_NEW_EX(AppInfo, ((*item)->getName(), app, NULL));
   			if(app_info)
   			{
				m_apps_runbackground.erase(item);
				m_apps.push_back(app_info);
				return true;
   			}
		}
	}
	return false;
}
NAMESPACE_END
