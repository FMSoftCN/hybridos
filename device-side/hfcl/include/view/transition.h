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


#ifndef NGUX_RollableText_h
#define NGUX_RollableText_h

#include "nguxevent.h"
#include "view.h"
#include "timerservice.h"

NAMESPACE_BEGIN


class Transition;

class TransitionManager : TimerEventListener
{	
	public:
		MAP(NGUInt, Transition*, TransitionMap)

		TransitionMap m_trMaps;

		NGInt m_timerId;
		NGInt m_interval;

	public:
		TransitionManager() { 
			m_timerId = 0; 
			m_interval = 300;
		}

		~TransitionManager();

		void addTransition(NGUInt key, Transition* t);
		void removeTransition(NGUInt key);
		Transition* getTranition(NGUInt key);

		NGInt setInterval(NGInt interval);
		NGInt interval() { return m_interval; }

	private:
		NGBool handleEvent(Event *event);
};

class Transition 
{
	public:
		virtual NGBool isEnd() = 0;
		virtual void update() = 0;
		virtual ~Transition() { }

		//use for mem manager
		virtual void release() { NGUX_DELETE(this); }
};

class RollTextTransition : public Transition 
{
	protected:
		View * m_owner;
		NGInt  m_offset:20;
		NGUInt  m_offsetStep:12;
	    NGInt m_countdown;
	public:	
		RollTextTransition(View *owner, NGInt step) : m_owner(owner) {
			m_offsetStep = step;
			m_offset = 0;
			m_countdown = 5;
		}
		~RollTextTransition() { }

		NGInt getOffset() { return m_offset; }

		void update() { 
			if(--m_countdown > 0) 
				return; 

			nextOffset(); 

			if(m_owner) 
				m_owner->updateView(); 
		}

		NGBool isEnd() { return m_owner == NULL; }

		void reset() {
			m_offset = 0;
			m_countdown = 5;
		}

		void nextOffset()  { m_offset += m_offsetStep; }


		static NGBool DrawRollText(View* view, GraphicsContext* context,DrawableSet *drset, NGInt draw_id, NGInt draw_state, const IntRect& rc, NGDword data, DR_DATA_TYPE type);

		static NGBool NeedRollText(DrawableSet *drset, NGInt draw_id, NGInt draw_state, const IntRect& rc, NGDword data, DR_DATA_TYPE type);
};

///the common functions
TransitionManager* GetCommonTransitionManager();
Transition* GetTransition(NGUInt key);	
void AddRollText(View *view, Transition* t);
void RemoveRollText(View* view);
void ResetRollText(View *view);

NAMESPACE_END

#endif

