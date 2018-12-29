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

#include "../common/common.h"
#include "../common/stlalternative.h"
#include "../common/contextstream.h"
#include "../activity/intent.h"
#include "../activity/controller.h"

namespace hfcl {

class BaseActivity : public Controller {
public:
    enum ActState {
        ORIGIN = 0,
        INIT,
        RUNNING,
        SLEEP,
        SUSPEND,
    };

    enum ActPriority {
        PRIORITY_LOWEST,
        PRIORITY_LOW,
        PRIORITY_MIDDLE,
        PRIORITY_HIGH,
        PRIORITY_HIGHEST,
    };

    BaseActivity()
        : m_name(NULL)
        , m_priority(PRIORITY_HIGH)
        , m_state(INIT) {};

    virtual ~BaseActivity();

    const char* getName() const { return m_name; }
    void setName(const char* name);

    int getState() const { return m_state; }
    void setState(ActState state);

    int getPriority() const { return m_priority; }
    void setPriority(ActPriority priority) { m_priority = priority; }

    void close();

    virtual void onCreate(ContextStream *contextStream, Intent *intent){}
    virtual void onStart(){}
    virtual void onSleep(){}
    virtual void onWakeup(){}
    virtual void onMove2Top() {}
    virtual Intent *onDestroy(ContextStream *contextStream) {
        return NULL;
    }
    virtual bool isSuspendable();

    virtual void exit() { close(); }

protected:
    View *getViewParent(HTResId view_id) { return NULL; }

    char * m_name;
    ActPriority m_priority;
    ActState m_state;
};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_BASEACTIVITY_H_ */

