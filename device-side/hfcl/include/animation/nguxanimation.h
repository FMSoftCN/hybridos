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
#ifndef _NGUX_ANIMATION_H
#define _NGUX_ANIMATION_H

#include "mgcl.h"
#include "nguxobject.h"

NAMESPACE_BEGIN

class Animation : public Object
{
	friend class GroupAnimation;
	
	protected:
		MGEFF_ANIMATION m_animation;

		Animation(enum EffVariantType varianttype);  

	public:
		Animation();
		virtual ~Animation();
        static NGInt initAnimation();
        static void deinitAnimation();

		void start(NGBool keepalive=true, NGBool sync=true);
		void stop();
		void pause();
		void resume();
        void wait();

		void setAttribute(NGInt id, NGInt value);
		NGInt getAttribute(NGInt id);

		inline void setDuration(NGInt duration) { 
			setAttribute(MGEFF_PROP_DURATION, duration); 
		}
		inline NGInt  getDuration() { return getAttribute(MGEFF_PROP_DURATION); }
		
		inline void setLoopCount(NGInt loopcount = MGEFF_INFINITE) { 
			setAttribute(MGEFF_PROP_LOOPCOUNT, loopcount); 
		}
		inline NGInt getLoopCount() { return getAttribute(MGEFF_PROP_LOOPCOUNT); }
		
		inline void setDirection(NGInt direction = MGEFF_DIR_FORWARD) { 
			setAttribute(MGEFF_PROP_DIRECTION, direction); 
		}
		inline NGInt  getDirection() { return getAttribute(MGEFF_PROP_DIRECTION); }
		
		inline void setKeepLive(NGBool bkeeplive = true) { 
			setAttribute(MGEFF_PROP_KEEPALIVE, bkeeplive); 
		}
		inline NGBool getKeepLive() { return getAttribute(MGEFF_PROP_KEEPALIVE); }
		
		inline NGInt  getCurLoop() { return getAttribute(MGEFF_PROP_CURLOOP); }
		inline NGInt  getCurFrame() { return getAttribute(MGEFF_PROP_CURFRAME); }
		inline NGInt  getCurState() { return getAttribute(MGEFF_PROP_STATE); }

        void setCurve(enum EffMotionType curve);
        MGEFF_MOTIONCURVE getCurve(); 

        void setStartValue(const void * value);
        const void * getStartValue();
        void setEndValue(const void * value);
        const void * getEndValue();

	protected:
		static void _finishcb(MGEFF_ANIMATION handle);
		virtual void setProperty(NGInt id, void *curvalue) = 0;
		static void _setProperty(MGEFF_ANIMATION handle, void *target, NGInt id, void *value);
		virtual void onStart(){};


    private:
        NGBool m_stop_in_progress;
};

NAMESPACE_END

#endif
