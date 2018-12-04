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


#ifndef HFCL_ACTIVITY_ACTIVITYSTACK_H_
#define HFCL_ACTIVITY_ACTIVITYSTACK_H_

#include "common/common.h"
#include "common/stlalternative.h"
#include "common/contextstream.h"

namespace hfcl {

class ActivityInfo;
class BaseActivity;

class ActivityStack
{
private:
    LIST(ActivityInfo*, ActivityInfoList);
    ActivityInfoList m_acts;
    ActivityInfoList m_acts_runbackground;

public:
    ActivityStack(){ }
    virtual ~ActivityStack(){ }
    
    void push(ActivityInfo *act);
    void pop();
    bool isEmpty();
    bool isExist(BaseActivity *act);
    bool remove(BaseActivity *act);
    bool move2Top(BaseActivity *act);
    bool move2Bottom(BaseActivity *act);
    bool pushBackgroundRunningActivity(BaseActivity *act);
    bool popBackgroundRunningActivity(BaseActivity *act);
    void clear();
    int size();
    ActivityInfo* top(int n = 0);
    ActivityInfo* bottom(int n = 0);
    BaseActivity* getExistActivityByName(const char * name);
    BaseActivity* getExistActivityRunBackgroundByName(const char * name);
};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_ACTIVITYSTACK_H_ */
