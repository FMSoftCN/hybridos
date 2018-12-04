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


#ifndef HFCL_ACTIVITY_BASEACTIVITY_H_
#define HFCL_ACTIVITY_BASEACTIVITY_H_

#include "common/common.h"
#include "common/stlalternative.h"
#include "common/contextstream.h"
#include "activity/intent.h"
#include "activity/controller.h"

namespace hfcl {

class BaseActivity : public Controller {
public:
    typedef enum _ACT_STATE
    {
        ORIGIN = 0,
        INIT, 
        RUNNING, 
        SLEEP, 
        SUSPEND, 
    } ACT_STATE;

    BaseActivity():m_priority(PRIORITY_HIGH), m_name(NULL), m_state(INIT){};
    virtual ~BaseActivity();

    const char * name(void);
    void setName(const char * name);
    ACT_STATE state(void);
    void setState(ACT_STATE state);
    void close(void);

    virtual void onCreate(ContextStream *contextStream, Intent *intent){}
    virtual void onStart(void){}
    virtual void onSleep(void){}
    virtual void onWakeup(void){}
    virtual void onMove2Top(void) {}
    virtual Intent *onDestroy(ContextStream *contextStream){return NULL;}
    virtual bool isSuspendable(void);

    /* VincentWei: add this method as virtual */
    virtual bool beInLockFrame(void) {return false;};

    virtual void exit() { close(); }

    enum {
        PRIORITY_LOWEST,
        PRIORITY_LOW,
        PRIORITY_MIDDLE,
        PRIORITY_HIGH,
        PRIORITY_HIGHEST,
    };

    void setPriority(int i_priority) { m_priority = i_priority; }
    int priority(void) { return m_priority; }

protected:
    int m_priority;
    char * m_name;
    ACT_STATE m_state;

    View* getViewParent(int view_id) { return NULL; }
};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_BASEACTIVITY_H_ */

