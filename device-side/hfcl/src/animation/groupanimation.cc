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

#include "animation/groupanimation.h"

namespace hfcl {

GroupAnimation::GroupAnimation(enum EffAnimationType type, GroupAnimation* parent)
	:Animation ()
{
	m_animation = mGEffAnimationCreateGroup(type);
        
	if(m_animation != 0) {
		mGEffAnimationSetContext(m_animation, (void*)this);  
		mGEffAnimationSetFinishedCb(m_animation, 
				(MGEFF_FINISHED_CB)Animation::_finishcb);
	}
	if (NULL != parent) {
		parent->add((Animation*)this);
	}
}

GroupAnimation::~GroupAnimation()
{
	AMILIST::iterator i;
	
	for (i = m_amiList.begin(); i != m_amiList.end(); i = m_amiList.erase(i))
	{
        // the animation will be deleted by group.
        (*i)->setKeepLive (false);
		// delete (*i);
		HFCL_DELETE(*i);
	}
}

void GroupAnimation::add (Animation *ani)
{
	/*
    // the animation will be deleted by group.
    (*i)->setKeepLive (false);
    */
	mGEffAnimationAddToGroup(m_animation, ani->m_animation);
	
	m_amiList.push_back(ani);
}

void GroupAnimation::onStart()
{
	AMILIST::iterator i;
	for (i = m_amiList.begin(); i != m_amiList.end(); ++i)
	{
        (*i)->onStart();
	}
}

} // namespace hfcl

