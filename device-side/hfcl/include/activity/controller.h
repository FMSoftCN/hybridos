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
** along with this program.  If not, see <https://www.gnu.org/licenses/>.  */


#ifndef HFCL_ACTIVITY_CONTROLLER_H_
#define HFCL_ACTIVITY_CONTROLLER_H_

#include "../common/common.h"
#include "../common/log.h"
#include "../common/event.h"
#include "../view/viewcontext.h"
#include "activitymanager.h"

namespace hfcl {

class ControllerClient;

class Controller
{
public:
    Controller();
    virtual ~Controller();

    virtual unsigned int showView(int view_id, HTData param1, HTData param2); //show a view in the stack
    virtual unsigned int switchView(int view_id, HTData param1, HTData param2); //switch a view with stack
    virtual    unsigned int showModalView(int view_id, HTData param1, HTData param2);
    virtual unsigned int backView(unsigned int endcode = 0);

    virtual unsigned int setMode(ControllerModeList* modeList) { return 0; }

    virtual unsigned int onClientCommand(int sender, unsigned int cmd_id, HTData param1, HTData param2) { return 0; }

    void cleanAllClient();

    virtual void exit() { HFCL_DELETE(this); }

    ControllerClient* getTop(int index = 0);
    void moveClientToTop(int view_id);
    void pop(int pop_count);

    ControllerClient* find(int view_id);
    void deleteView(int view_id, bool bExit = true);
    int getClientCount() { return m_list.size();}
    int getTopViewId(void);

protected:
    LIST(ControllerClient*, ControllerClientList)

    ControllerClientList m_list;

    virtual void animationSwitch(ControllerClient* prev, ControllerClient* cur);

    void push(ControllerClient* client);
    void setTop(ControllerClient* client,  int index = 0);

    unsigned int passCommandToClient(int client_id, unsigned int cmd_id, HTData param1, HTData param);


    virtual ControllerClient*  createClient(int view_id, HTData param1, HTData param2) { return NULL; }

    virtual View * getViewParent(int view_id) = 0;

    int m_modalCount;
};

#define DECLARE_CONTROLLER_CLIENTS \
    protected: virtual ControllerClient* createClient(int view_id, HTData param1, HTData param2);

#define BEGIN_CONTROLLER_CLIENTS(clss) \
    ControllerClient* clss::createClient(int view_id, HTData param1, HTData param2) { switch(view_id) {

#define END_CONTROLLER_CLIENTS \
    } return NULL;}

#define END_CONTROLLER_CLIENTS_EX(super) \
    } return super::createClient(view_id, param1, param2); }

#define CONTROLLER_CLIENT(view_id, ClientClass) \
    case view_id:  \
        return HFCL_NEW_EX(ClientClass, (this, \
                view_id, \
                getViewParent(view_id), \
                param1, param2));

//////////////////////////////////////////////////////////////

class ControllerClient : public ViewContext
{
protected:
    Controller* m_owner;
    View*       m_baseView;
    int         m_id;
    int         m_inactiveTimes;
    bool        m_bModal;
    ControllerModeManager *m_modeManager;
    ControllerModeList    *m_currentList;

public:
    ControllerClient(Controller* owner)
        : m_owner(owner)
        , m_baseView(NULL)
        , m_id(0)
        , m_inactiveTimes(0)
        , m_modeManager(NULL)
        , m_currentList(NULL) {}

    ControllerClient(Controller* owner, int id, View *p_baseView)
        : m_owner(owner)
        , m_baseView(p_baseView)
        , m_id(id)
        , m_inactiveTimes(0)
        , m_modeManager(NULL)
        , m_currentList(NULL) {}

    ControllerClient(Controller* owner, int id, View * parent,
            HTData param1, HTData param2)
        : m_owner(owner)
        , m_baseView(NULL)
        , m_id(id)
        , m_inactiveTimes(0)
        , m_modeManager(NULL)
        , m_currentList(NULL) {}

    virtual ~ControllerClient();

    /* public methods */
    void setId(int id) { m_id = id; }
    int getId() const { return m_id; }
    int getInActiveTimes() const { return m_inactiveTimes; }

    bool isTop() const {
        ActivityManager* act_mgr = ActivityManager::getInstance();

        return ((m_owner->getTop() == this) &&
                (act_mgr->getCurrentActivity() == (BaseActivity *)m_owner));
    }

    void setModal(bool bModal ) { m_bModal = bModal; }
    bool isModal(void) const { return m_bModal; }

    View *baseView() { return m_baseView; }
    const View *baseView() const { return m_baseView; }
    void cleanBaseView();

    unsigned int sendCommand (unsigned int cmd_id,
            HTData param1, HTData param2) {
        if (m_owner)
            return m_owner->onClientCommand (getId(), cmd_id, param1, param2);
        return 0;
    }

    unsigned int getCurrentModeId() const {
        return m_currentList ? m_currentList->mode_id : 0;
    }

    unsigned int showView(int view_id, HTData param1, HTData param2) {
        if (m_owner)
            return m_owner->showView(view_id, param1, param2);
        return 0;
    }

    unsigned int switchView(int view_id, HTData param1, HTData param2) {
        if (m_owner)
            return m_owner->switchView(view_id, param1, param2);
        return 0;
    }

    unsigned int showModalView(int view_id, HTData param1, HTData param2) {
        if (m_owner)
            return m_owner->showModalView(view_id, param1, param2);
        return 0;
    }

    unsigned int backView(unsigned int end_code = 0) {
        if(m_owner)
            return m_owner->backView(end_code);
        return 0;
    }

    /* virtual functions */
    virtual void active();
    virtual void inactive();

    virtual unsigned int onControllerCommand(unsigned int cmd_id,
            HTData param1, HTData param2) {
        return 0;
    }

    virtual void onActive(void* param) { }
    virtual void onInactive() { }
    virtual void onSleep() {}
    virtual void onWakeup() {}

    virtual bool onKey(const KeyEvent* event) { return GOON_DISPATCH; }
    virtual void onBackView(unsigned int endcode) { }
    virtual void onPopView(unsigned int endcode) { }

    virtual bool setCurrentMode(int mode_id) {
        if (!m_modeManager)
            return false;

        ControllerModeList* modeList = m_modeManager->getModeById(mode_id);
        if (!modeList)
            return false;

        m_currentList = modeList;
        if (isTop())
            m_owner->setMode(m_currentList);
        return true;
    }

    virtual int GetValueFromCurrentMode(int mode_id, int sub_id);

    virtual void exit() {
        if(m_owner) {
            m_owner->exit();
        }
        else {
            HFCL_DELETE(this);
        }
    }

protected:
    virtual void setControllerModeManager(ControllerModeManager* managers) {
        m_modeManager = managers;
        if (m_modeManager)
            m_currentList = m_modeManager->getModeById (
                m_modeManager->def_mode_id);
        else
            m_currentList = NULL;
    }

    void setView(int view_id, View* view) { }
    EventListener* getHandle(int handle_id, int event_type) {
        return NULL;
    }
};

} // namespace hfcl

#endif

