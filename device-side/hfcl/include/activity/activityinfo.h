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

#ifndef HFCL_ACTIVITY_ACTIVITYINFO_H_
#define HFCL_ACTIVITY_ACTIVITYINFO_H_

#include "common/common.h"
//#include "common/contextstream.h"
#include "activity/baseactivity.h"

namespace hfcl {

class ActivityInfo {
public:
    ActivityInfo();
    ActivityInfo(string name, BaseActivity* act, ContextStream* cs);
    virtual ~ActivityInfo();

    BaseActivity* getActivity(void);
    void setActivity(BaseActivity* act);

    string getName(void);
    void setName(string name);

    //ContextStream* getContextStream(void);
    //void setContextStream(ContextStream* cs);

private:
    string          m_actname;
    BaseActivity*   m_act;
    //ContextStream* m_stream;
};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_ACTIVITYINFO_H_ */

