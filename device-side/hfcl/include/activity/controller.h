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


#ifndef NGUX_CONTROLLER_H
#define NGUX_CONTROLLER_H

#include "nguxcommon.h"
#include "log.h"
#include "viewcontext.h"
#include "nguxevent.h"
#include "appmanager.h"

NAMESPACE_BEGIN

typedef NGULong NGParam;

class ControllerClient;
class View;

class Controller
{
public:
	Controller();
	virtual ~Controller();

	virtual NGUInt showView(NGInt view_id, NGParam param1, NGParam param2); //show a view in the stack
	virtual NGUInt switchView(NGInt view_id, NGParam param1, NGParam param2); //switch a view with stack
	virtual	NGUInt showModalView(NGInt view_id, NGParam param1, NGParam param2);
	virtual NGUInt backView(NGUInt endcode = 0);

	virtual NGUInt setMode(ControllerModeList* modeList) { return 0; }

	virtual NGUInt onClientCommand(NGInt sender, NGUInt cmd_id, NGParam param1, NGParam param2) { return 0; }

	void cleanAllClient();

	virtual void exit() { NGUX_DELETE(this); }
	
    ControllerClient* getTop(NGInt index = 0);
	void moveClientToTop(NGInt view_id);
	void pop(NGInt pop_count);

	ControllerClient* find(NGInt view_id);
    void deleteView(NGInt view_id, NGBool bExit = true);
	int getClientCount() { return m_list.size();}
    NGInt getTopViewId(void);

protected:
	LIST(ControllerClient*, ControllerClientList)

	ControllerClientList m_list;

	virtual void animationSwitch(ControllerClient* prev, ControllerClient* cur);

	void push(ControllerClient* client);
	void setTop(ControllerClient* client,  NGInt index = 0);

	NGUInt passCommandToClient(NGInt client_id, NGUInt cmd_id, NGParam param1, NGParam param);


	virtual ControllerClient*  createClient(NGInt view_id, NGParam param1, NGParam param2) { return NULL; }

	virtual View * getViewParent(NGInt view_id) = 0; 

	NGInt m_modalCount;
};

#define DECLARE_CONTROLLER_CLIENTS \
	protected: virtual ControllerClient* createClient(NGInt view_id, NGParam param1, NGParam param2);

#define BEGIN_CONTROLLER_CLIENTS(clss) \
	ControllerClient* clss::createClient(NGInt view_id, NGParam param1, NGParam param2) { switch(view_id) { 

#define END_CONTROLLER_CLIENTS \
	} return NULL;}

#define END_CONTROLLER_CLIENTS_EX(super) \
	} return super::createClient(view_id, param1, param2); }

#define CONTROLLER_CLIENT(view_id, ClientClass) \
	case view_id:  \
		return NGUX_NEW_EX(ClientClass, (this, \
				view_id, \
				getViewParent(view_id), \
				param1, param2));

//////////////////////////////////////////////////////////////

class ControllerClient : public ViewContext
{
protected:
	Controller* m_owner; //the controller onwer
	View*       m_baseView; //the view of ui
	NGInt       m_id;
	NGInt      m_inactiveTimes;
	NGBool  m_bModal;
	ControllerModeManager *m_modeManager;
	ControllerModeList    *m_currentList;
public:
	ControllerClient(Controller* owner) 
        : m_owner(owner)
         , m_baseView(NULL)
         , m_id(0)
         , m_inactiveTimes(0) 
         , m_modeManager(NULL)
         , m_currentList(NULL) 
	{}

	ControllerClient(Controller* owner, NGInt id, View *p_baseView) 
        : m_owner(owner)
          , m_baseView(p_baseView)
          , m_id(id)
          , m_inactiveTimes(0) 
          , m_modeManager(NULL)
          , m_currentList(NULL) 
    { }

	ControllerClient(Controller* owner, NGInt id, View * parent, NGParam param1, NGParam param2)
        : m_owner(owner)
          , m_baseView(NULL)
          , m_id(id)
          , m_inactiveTimes(0) 
          , m_modeManager(NULL)
          , m_currentList(NULL)
	{ }

	virtual ~ControllerClient();

	void setId(NGInt id) { m_id = id; }
	NGInt getId() { return m_id; }
	NGInt getInActiveTimes() { return m_inactiveTimes; }

	NGBool isTop() {
	    return ((m_owner->getTop() == this) && (AppManager::getInstance()->getCurrentApp() == (BaseApp *)m_owner));
	}
	void setModal(NGBool bModal ) { m_bModal = bModal;}
	NGBool isModal (void) {return m_bModal;}

	virtual void active();
	virtual void inactive();

	virtual NGUInt onControllerCommand(NGUInt cmd_id,NGParam param1, NGParam param2) {
		return 0;
	}

	virtual void onActive(void* param) { } //when a view is put into the front 
	virtual void onLostActive() { }
	virtual void onSleep() {} //when the view is destoried
	virtual void onWakeup() {}

	virtual NGBool onKey(NGInt keyCode, KeyEvent* event){ return DISPATCH_CONTINUE_MSG; }
	virtual void onBackView(NGUInt endcode) { }
	virtual void onPopView(NGUInt endcode) { }

	virtual NGBool setCurrentMode(NGInt mode_id) {
		if(!m_modeManager)
			return false;

		ControllerModeList* modeList = m_modeManager->getModeById(mode_id);
		if(!modeList)
			return false;

		m_currentList = modeList;
        if(isTop())
    		m_owner->setMode(m_currentList);
		return true;
	}
	
	virtual NGInt GetValueFromCurrentMode(NGInt mode_id,NGInt sub_id); 

	NGUInt getCurrentModeId() {
		return m_currentList ? m_currentList->mode_id : 0;
	}

	NGUInt showView(NGInt view_id, NGParam param1, NGParam param2){
		if(m_owner)
			return m_owner->showView(view_id, param1, param2);
		return 0;
	}

	NGUInt switchView(NGInt view_id, NGParam param1, NGParam param2) //switch a view with stack
	{
		if(m_owner)
			return m_owner->switchView(view_id, param1, param2);
		return 0;
	}

	NGUInt showModalView(NGInt view_id, NGParam param1, NGParam param2)
	{
		if(m_owner)
			return m_owner->showModalView(view_id, param1, param2);
		return 0;
	}

	NGUInt backView(NGUInt end_code = 0)
	{
		if(m_owner)
			return m_owner->backView(end_code);
		return 0;
	}

	virtual void exit() {
		if(m_owner) {
			m_owner->exit();
		}
		else {
			NGUX_DELETE(this);
		}
	}
	
	View *baseView() { return m_baseView; }

	void cleanBaseView();

	NGUInt sendCommand(NGUInt cmd_id, NGParam param1, NGParam param2) {
		if(m_owner)
			return m_owner->onClientCommand(getId(), cmd_id, param1, param2);
		return 0;
	}
protected:
	//virtual View* createView() = 0;
	//virtual void  onViewCreated() = 0;



	virtual void setControllerModeManager(ControllerModeManager* mangers) 
	{
		m_modeManager = mangers;
		if(m_modeManager)
			m_currentList = m_modeManager->getModeById(m_modeManager->def_mode_id);
		else
			m_currentList = NULL;
	}

	void setView(NGInt view_id, View* view) { }

	EventListener* getHandle(NGInt handle_id, NGInt event_type) { return NULL; }
};


NAMESPACE_END

#endif


