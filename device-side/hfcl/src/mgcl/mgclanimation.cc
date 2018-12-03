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

#include "common/common.h"
#include "mgcl/mgcl.h"


namespace hfcl {

int mgclMGEffInit(void)
{
    return mGEffInit();
}

void mgclMGEffDeinit(void)
{
    return mGEffDeinit();
}

MGEFF_ANIMATION mgclMGEffAnimationCreate(void* target,
        MGEFF_SETPROPERTY_CB setproperty, int id, enum EffVariantType varianttype)
{
    return mGEffAnimationCreate(target, setproperty, id, varianttype);
}

void mgclMGEffAnimationSetContext(MGEFF_ANIMATION handle, void* context)
{
    return mGEffAnimationSetContext(handle, context);
}

void mgclMGEffAnimationSetFinishedCb(MGEFF_ANIMATION handle, MGEFF_FINISHED_CB cb)
{
    return mGEffAnimationSetFinishedCb(handle, cb);
}

void mgclMGEffAnimationDelete(MGEFF_ANIMATION handle)
{
    return mGEffAnimationDelete(handle);
}

void* mgclMGEffAnimationGetContext(MGEFF_ANIMATION handle)
{
    return mGEffAnimationGetContext(handle);
}

void mgclMGEffAnimationSetProperty(MGEFF_ANIMATION handle,
        enum EffAnimProperty id, int value)
{
    return mGEffAnimationSetProperty(handle, id, value);
}

int mgclMGEffAnimationGetProperty(MGEFF_ANIMATION handle, enum EffAnimProperty id)
{
    return mGEffAnimationGetProperty(handle, id);
}

void mgclMGEffAnimationSetCurve(MGEFF_ANIMATION handle, enum EffMotionType type)
{
    return mGEffAnimationSetCurve(handle, type);
}

MGEFF_MOTIONCURVE mgclMGEffAnimationGetCurve(MGEFF_ANIMATION handle)
{
    return mGEffAnimationGetCurve(handle);
}

void mgclMGEffAnimationSetStartValue(MGEFF_ANIMATION handle, const void* value)
{
    return mGEffAnimationSetStartValue(handle, value);
}

const void* mgclMGEffAnimationGetStartValue(MGEFF_ANIMATION handle)
{
    return mGEffAnimationGetStartValue(handle);
}

void mgclMGEffAnimationSetEndValue(MGEFF_ANIMATION handle, const void* value)
{
    return mGEffAnimationSetEndValue(handle, value);
}

const void* mgclMGEffAnimationGetEndValue(MGEFF_ANIMATION handle)
{
    return mGEffAnimationGetEndValue(handle);
}

int mgclMGEffAnimationAsyncRun(MGEFF_ANIMATION handle)
{
    return mGEffAnimationAsyncRun(handle);
}

void mgclMGEffAnimationStop(MGEFF_ANIMATION handle)
{
    return mGEffAnimationStop(handle);
}

void mgclMGEffAnimationPaus(MGEFF_ANIMATION handle)
{
    return mGEffAnimationPause(handle);
}

void mgclMGEffAnimationResume(MGEFF_ANIMATION handle)
{
    return mGEffAnimationResume(handle);
}

int mgclMGEffAnimationWait(void* hWnd, MGEFF_ANIMATION handle)
{
    return mGEffAnimationWait(hWnd, handle);
}

int mgclMGEffAnimationSyncRun(MGEFF_ANIMATION handle)
{
    return mGEffAnimationSyncRun(handle);
}

MGEFF_ANIMATION mgclMGEffAnimationCreateGroup(enum EffAnimationType type)
{
    return mGEffAnimationCreateGroup(type);
}

void mgclMGEffAnimationAddToGroup(MGEFF_ANIMATION group, MGEFF_ANIMATION animation)
{
    return mGEffAnimationAddToGroup(group, animation);
}

} // namespace hfcl

