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

#include "nguxanimation.h"
#include "appmanager.h"

namespace hfcl {

int Animation::initAnimation()
{
	//FIXED: cannot call mGEffInit when init, 
	// because, mGEffInit need call SetTimerEx, this function need a message queue
	// a message queue wouble be create when a minigui window is created
    return mGEffInit();
	return 1;
}

void Animation::deinitAnimation()
{
    mGEffDeinit();
}

Animation::Animation(enum EffVariantType varianttype)
{
    m_stop_in_progress = false;
	static int _mgeff_is_inited = 0;
	// init the mGEff when needed!
	if(!_mgeff_is_inited)
	{
		mGEffInit();
		_mgeff_is_inited = 1;
	}
	m_animation = mGEffAnimationCreate(this, 
			(MGEFF_SETPROPERTY_CB)Animation::_setProperty, 0, varianttype);

	if(m_animation != 0)
	{
		mGEffAnimationSetContext(m_animation, (void*)this);
		mGEffAnimationSetFinishedCb(m_animation, 
				(MGEFF_FINISHED_CB)Animation::_finishcb);
	}
}

Animation::Animation(void)
	:m_animation(0) ,m_stop_in_progress(false)
{

}

Animation::~Animation(void)
{
	if (getCurState() != MGEFF_STATE_REMOVE)
		mGEffAnimationSetContext(m_animation, NULL);

    if (true == getKeepLive())
        mGEffAnimationDelete(m_animation);
}

void Animation::_finishcb(MGEFF_ANIMATION handle)
{
	Animation *animation = (Animation*)mGEffAnimationGetContext(handle);
	if(animation)
	{
		//把animation纳入到NGUX的事件机制中，这里暂不实�?	
	}
}

void Animation::_setProperty(MGEFF_ANIMATION handle, void *target, int id, void *value)
{
	Animation *animation = (Animation*)mGEffAnimationGetContext(handle);
	if(animation) {
		animation->setProperty(id, value);
	}
}

void Animation::setAttribute(int id, int value)
{
    mGEffAnimationSetProperty(m_animation, (enum EffAnimProperty)id, value);
}

int Animation::getAttribute(int id)
{
    return mGEffAnimationGetProperty(m_animation, (enum EffAnimProperty)id);
}

void Animation::setCurve(enum EffMotionType curve)
{
    mGEffAnimationSetCurve(m_animation, curve);
}

MGEFF_MOTIONCURVE Animation::getCurve()
{
    return mGEffAnimationGetCurve(m_animation);
}

void Animation::setStartValue(const void * value)
{
    mGEffAnimationSetStartValue(m_animation, value);
}

const void * Animation::getStartValue()
{
   return  mGEffAnimationGetStartValue(m_animation);
}

void Animation::setEndValue(const void * value)
{
    mGEffAnimationSetEndValue(m_animation, value);
}

const void * Animation::getEndValue()
{
    return mGEffAnimationGetEndValue(m_animation);
}

void Animation::start(bool auto_delete, bool sync)
{
    this->onStart();

    m_stop_in_progress = false;
    
	if (true == auto_delete)
		setKeepLive (false);
/*    
	if (true == sync)
        mGEffAnimationSyncRun(m_animation);
    else
        mGEffAnimationAsyncRun(m_animation);
*/
    mGEffAnimationAsyncRun(m_animation);
}

void Animation::stop(void)
{
    if (!m_stop_in_progress)
    {
        m_stop_in_progress = true;
        mGEffAnimationStop(m_animation);
    }
}

void Animation::pause(void)
{
    mGEffAnimationPause(m_animation);
}

void Animation::resume(void)
{
    mGEffAnimationResume(m_animation);
}

void Animation::wait(void)
{
    HWND hwnd = ActivityManager::getInstance()->hosting();
    mGEffAnimationWait((void*)&hwnd, m_animation);
}

} // namespace hfcl
