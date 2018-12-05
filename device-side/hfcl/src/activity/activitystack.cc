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

#include "activity/activitystack.h"

#include "activity/baseactivity.h"
#include "activity/activityinfo.h"

namespace hfcl {

void ActivityStack::push(ActivityInfo *act)
{
	m_acts.push_back(act);
}

void ActivityStack::pop()
{
	m_acts.pop_back();
}

bool ActivityStack::isEmpty()
{
	return m_acts.empty();
}

void ActivityStack::clear()
{
	while(!isEmpty()) {
		ActivityInfo *a = top();
		pop();
		// delete a;
        HFCL_DELETE(a);
	}
}

bool ActivityStack::isExist(BaseActivity *act)
{
	ActivityInfoList::iterator item;
	for(item = m_acts.begin(); item != m_acts.end(); item++)
	{
		if((*item)->getActivity() == act)
			return true;
	}
	return false;
}

BaseActivity* ActivityStack::getExistActivityByName(const char * name)
{
	ActivityInfoList::iterator item = m_acts.begin();

	for(int i = 0; i < size(); i++, item++)
	{
		BaseActivity *rt = (*item)->getActivity();
		if (rt != NULL) {
			const char* pname = rt->name();
			if(pname != NULL && strcmp(name, pname) == 0) {
				return rt;
			}
		}
	}
	
	return NULL;
}

BaseActivity* ActivityStack::getExistActivityRunBackgroundByName(const char * name)
{
	ActivityInfoList::iterator item;

	for(item = m_acts_runbackground.begin(); item != m_acts_runbackground.end(); item++)
	{
		BaseActivity *rt = (*item)->getActivity();
		if (rt != NULL) {
			const char* pname = rt->name();
			if(pname != NULL && strcmp(pname, name) == 0) {
				return rt;
			}
		}
	}
	
	return NULL;
}

bool ActivityStack::remove(BaseActivity *act)
{
	ActivityInfoList::iterator item;
	for(item = m_acts.begin(); item != m_acts.end(); item++)
	{
		ActivityInfo *a = *item;
		if(a != NULL && a->getActivity() == act)
		{
			m_acts.remove(a);
			HFCL_DELETE(a);
			return true;
		}
	}
	return false;
}

ActivityInfo* ActivityStack::top(int n)
{
	if (m_acts.size() > (unsigned int)n)
    {
		if (n == 0) {
			return m_acts.back();
		} else {
			ActivityInfoList::iterator item;

			n = m_acts.size() - n;
			for(item = m_acts.begin(); item != m_acts.end(); item++)
			{
				if (--n == 0){
					return (*item);
				}
			}
		}
    }
    return NULL;
}

ActivityInfo* ActivityStack::bottom(int n)
{
    if (m_acts.size() > (unsigned int)n)
    {
		if (n == 0) {
			return m_acts.front();
		} else {
			ActivityInfoList::iterator item;
			for(item = m_acts.begin(); item != m_acts.end(); item++)
			{
				if (n-- == 0){
					return (*item);
				}
			}
		}
    }
    return NULL;
}

int ActivityStack::size()
{
	return m_acts.size();
}

bool ActivityStack::move2Top(BaseActivity *act)
{
	ActivityInfoList::iterator item;
	for(item = m_acts.begin(); item != m_acts.end(); item++)
	{
		ActivityInfo *_old = *item;
		if(_old != NULL && _old->getActivity() == act)
		{
    		ActivityInfo *act_info = HFCL_NEW_EX(ActivityInfo, (_old->getName(), act, NULL));
   			if(act_info)
   			{
   				m_acts.erase(item);	
				// delete the old act info, but don't delete the baseact
				_old->setActivity(NULL);
				HFCL_DELETE(_old);
				m_acts.push_back(act_info);
				return true;
   			}
			return false;
		}
	}
	return false;
}

bool ActivityStack::move2Bottom(BaseActivity *act)
{
	ActivityInfoList::iterator item;
	
	for(item = m_acts.begin(); item != m_acts.end(); item++)
	{
		ActivityInfo *_old = *item;
		if(_old != NULL && _old->getActivity() == act)
		{
    		ActivityInfo *act_info = HFCL_NEW_EX(ActivityInfo, (_old->getName(), act, NULL));
   			if(act_info)
   			{
   				m_acts.erase(item);	
				// delete the old act info, but don't delete the baseact
				_old->setActivity(NULL);
				HFCL_DELETE(_old);
				m_acts.push_front(act_info);
				return true;
   			}
		}
	}
	return false;
}

bool ActivityStack::pushBackgroundRunningActivity(BaseActivity *act)
{
	ActivityInfoList::iterator item;
	for(item = m_acts.begin(); item != m_acts.end(); item++)
	{
		if((*item)->getActivity() == act)
		{
    		ActivityInfo *act_info = HFCL_NEW_EX(ActivityInfo, ((*item)->getName(), act, NULL));
   			if(act_info)
   			{
   				m_acts.pop_back();
				m_acts_runbackground.push_back(act_info);
				return true;
   			}
		}
	}
	return false;
}

bool ActivityStack::popBackgroundRunningActivity(BaseActivity *act)
{
	ActivityInfoList::iterator item;
	for(item = m_acts_runbackground.begin(); item != m_acts_runbackground.end(); item++)
	{
		if((*item)->getActivity() == act)
		{
    		ActivityInfo *act_info = HFCL_NEW_EX(ActivityInfo, ((*item)->getName(), act, NULL));
   			if(act_info)
   			{
				m_acts_runbackground.erase(item);
				m_acts.push_back(act_info);
				return true;
   			}
		}
	}
	return false;
}

} // namespace hfcl

