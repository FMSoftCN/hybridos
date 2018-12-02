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


#ifndef _NGUX_ViewContext_h
#define _NGUX_ViewContext_h

#include "nguxcommon.h"

NAMESPACE_BEGIN

class View;
class EventListener;

///controller mode of resource
typedef struct {
	NGInt      id;
	NGUInt     value;
	NGUInt     cmd;
}ControllerMode;

//static mode table
typedef struct {
	NGInt  mode_id;
	NGInt  mode_count;
	ControllerMode *modes;
}ControllerModeList;	

typedef struct {
	ControllerModeList * mode_lists;
	NGInt                list_count;
	NGInt                def_mode_id;
	ControllerModeList* getModeById(NGInt mode_id) {
		for(int i = 0; i < list_count; i++)
		{
			if(mode_lists[i].mode_id == mode_id)
				return &mode_lists[i];
		}
		return NULL;
	}
}ControllerModeManager;

class ViewContext
{
public:
	virtual ~ViewContext() { }
	virtual void setView(NGInt view_id, View* view) = 0;
	virtual EventListener* getHandle(NGInt handle_id, NGInt event_type) = 0;
	virtual void setControllerModeManager(ControllerModeManager* mangers) { }
};

#define DECLARE_VIEWCONTEXT   protected: void setView(NGInt, View*); EventListener* getHandle(NGInt, int);

#define BEGIN_SETVIEW(clss)  void clss::setView(NGInt view_id, View* view) { switch(view_id) {

#define END_SETVIEW  } }

#define MAP_VIEW(var, id, type)  case id : (var) = (type*)(view); break;
#define MAP_SUPPER_SETVIEW(super)  default: supper::setView(view_id, view); break;

#define BEGIN_GETHANDLE(clss) EventListener* clss::getHandle(NGInt handle_id, NGInt event_id) { switch(handle_id) { 

#define END_GETHANDLE  default: return NULL; } }

#ifdef WIN32
#define MAP_HANDLE(clss, handle_id, handle_name) \
	case handle_id: { \
		typedef NGBool (clss:: *PFUNC)(CustomEvent *e); \
		PFUNC p = &clss::handle_name; \
		MethodEventListener::EventHandle *h = (MethodEventListener::EventHandle *)(&p); \
		return NGUX_NEW_EX(MethodEventListener, ((void*)(this), *h, event_id)); \
	}
#else
#define MAP_HANDLE(clss, handle_id, handle_name) case handle_id: return METHOD_EVENT_HANDLE_EX(this, clss, handle_name, event_id);
#endif

#define DECLARE_UI_TEMPL  \
	public : static void setUITempl(ResID ui_id); \
	private: static ResID _ui_templ_id;

#define DEFINE_UI_TEMPL(clss) \
	ResID clss::_ui_templ_id = 0; \
	void clss::setUITempl(ResID ui_id) { _ui_templ_id = ui_id; }

#define UI_TEMPL    _ui_templ_id

#define SET_UI_TEMPL(clss, id)  clss::setUITempl((ResID)id)


class ContentProvider
{
public:
	virtual void* getContentData(NGInt data_id, NGInt type) = 0;
	virtual NGInt getContentStringId(NGInt data_id, NGInt type) = 0;
};

typedef View * (*CB_CREATE_VIEW)(View* parent, ViewContext* viewContext, ContentProvider * cotentProvider);

//The help class
typedef struct _ContentElement {
	NGInt data_id;
	void *data;
	NGInt string_id;
}ContentElement;

class DefaultContentProvider : public ContentProvider{
	ContentElement * m_elements;
	NGInt				 m_count;
public:
	DefaultContentProvider(ContentElement* elements, NGInt count)
		:m_elements(elements), m_count(count) { }
	~DefaultContentProvider() { }

	void * getContentData(NGInt data_id, NGInt type) {
		for(NGInt i = 0; i < m_count; i++)
		{
			if(m_elements[i].data_id == data_id)
				return m_elements[i].data;
		}
		return NULL;
	}
	NGInt getContentStringId(NGInt data_id, NGInt type) {
		for(NGInt i = 0; i < m_count; i++)
		{
			if(m_elements[i].data_id == data_id)
				return m_elements[i].string_id;
		}
		return -1;
	}
};


NAMESPACE_END

#endif /* _NGUX_ViewContext_h */
