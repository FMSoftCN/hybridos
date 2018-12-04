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
#ifndef _HFCL_ANIMATION_H
#define _HFCL_ANIMATION_H

#include "mgcl/mgcl.h"
#include "common/object.h"

namespace hfcl {

class Animation : public Object
{
	friend class GroupAnimation;
	
	protected:
		MGEFF_ANIMATION m_animation;

		Animation(enum EffVariantType varianttype);  

	public:
		Animation();
		virtual ~Animation();
        static int initAnimation();
        static void deinitAnimation();

		void start(bool keepalive=true, bool sync=true);
		void stop();
		void pause();
		void resume();
        void wait();

		void setAttribute(int id, int value);
		int getAttribute(int id);

		inline void setDuration(int duration) { 
			setAttribute(MGEFF_PROP_DURATION, duration); 
		}
		inline int  getDuration() { return getAttribute(MGEFF_PROP_DURATION); }
		
		inline void setLoopCount(int loopcount = MGEFF_INFINITE) { 
			setAttribute(MGEFF_PROP_LOOPCOUNT, loopcount); 
		}
		inline int getLoopCount() { return getAttribute(MGEFF_PROP_LOOPCOUNT); }
		
		inline void setDirection(int direction = MGEFF_DIR_FORWARD) { 
			setAttribute(MGEFF_PROP_DIRECTION, direction); 
		}
		inline int  getDirection() { return getAttribute(MGEFF_PROP_DIRECTION); }
		
		inline void setKeepLive(bool bkeeplive = true) { 
			setAttribute(MGEFF_PROP_KEEPALIVE, bkeeplive); 
		}
		inline bool getKeepLive() { return getAttribute(MGEFF_PROP_KEEPALIVE); }
		
		inline int  getCurLoop() { return getAttribute(MGEFF_PROP_CURLOOP); }
		inline int  getCurFrame() { return getAttribute(MGEFF_PROP_CURFRAME); }
		inline int  getCurState() { return getAttribute(MGEFF_PROP_STATE); }

        void setCurve(enum EffMotionType curve);
        MGEFF_MOTIONCURVE getCurve(); 

        void setStartValue(const void * value);
        const void * getStartValue();
        void setEndValue(const void * value);
        const void * getEndValue();

	protected:
		static void _finishcb(MGEFF_ANIMATION handle);
		virtual void setProperty(int id, void *curvalue) = 0;
		static void _setProperty(MGEFF_ANIMATION handle, void *target, int id, void *value);
		virtual void onStart(){};


    private:
        bool m_stop_in_progress;
};

} // namespace hfcl

#endif
