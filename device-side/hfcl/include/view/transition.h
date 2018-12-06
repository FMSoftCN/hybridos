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

#ifndef HFCL_VIEW_TRANSITION_H_
#define HFCL_VIEW_TRANSITION_H_

#include "view.h"
#include "../common/event.h"
#include "../services/timerservice.h"

namespace hfcl {

class Transition 
{
public:
    virtual bool isEnd() = 0;
    virtual void update() = 0;
    virtual ~Transition() { }

    //use for mem manager
    virtual void release() { HFCL_DELETE(this); }
};

class TransitionManager : TimerEventListener {	
public:
    MAP(unsigned int, Transition*, TransitionMap)

    TransitionMap m_trMaps;

    int m_timerId;
    int m_interval;

public:
    TransitionManager() { 
        m_timerId = 0; 
        m_interval = 300;
    }

    ~TransitionManager();

    void addTransition(HTData key, Transition* t);
    void removeTransition(HTData key);
    Transition* getTranition(HTData key);

    int setInterval(int interval);
    int interval() { return m_interval; }

private:
    bool handleEvent(Event *event);
};

class RollTextTransition : public Transition {
protected:
    View * m_owner;
    int  m_offset:20;
    unsigned int  m_offsetStep:12;
    int m_countdown;

public:	
    RollTextTransition(View *owner, int step) : m_owner(owner) {
        m_offsetStep = step;
        m_offset = 0;
        m_countdown = 5;
    }
    ~RollTextTransition() { }

    int getOffset() { return m_offset; }

    void update() { 
        if(--m_countdown > 0) 
            return; 

        nextOffset(); 

        if(m_owner) 
            m_owner->updateView(); 
    }

    bool isEnd() { return m_owner == NULL; }

    void reset() {
        m_offset = 0;
        m_countdown = 5;
    }

    void nextOffset()  { m_offset += m_offsetStep; }


    static bool DrawRollText(View* view, GraphicsContext* context,
            DrawableSet *drset, int draw_id, int draw_state,
            const IntRect& rc, HTData data, DR_DATA_TYPE type);

    static bool NeedRollText(DrawableSet *drset, int draw_id, int draw_state,
            const IntRect& rc, HTData data, DR_DATA_TYPE type);
};

///the common functions
TransitionManager* GetCommonTransitionManager();
Transition* GetTransition(HTData key);	
void AddRollText(View *view, Transition* t);
void RemoveRollText(View* view);
void ResetRollText(View *view);

} // namespace hfcl

#endif // HFCL_VIEW_TRANSITION_H_

