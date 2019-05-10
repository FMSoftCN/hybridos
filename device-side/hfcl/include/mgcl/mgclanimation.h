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

#ifndef __HFCL_MGCLANIMATION_H__
#define __HFCL_MGCLANIMATION_H__

#include <mgeff/mgeff.h>


namespace hfcl {

/**
 * @brief mgclMGEffInit Initializes the mgeff library. It creates the default
 *        schedular.
 *
 * @returns If succeed, return 0; otherwise return -1;
 */
int mgclMGEffInit(void);

/**
 * @brief mgclMGEffDeinit de-initializes the mgeff library. It will desktroy the
 *        default scheduar.
 */
void mgclMGEffDeinit(void);

/**
 * @brief mgclMGEffAnimationCreate Creates an animation.
 *
 * @param target The target to be animated.
 * @param setproperty The animation set property call back, this function is
 *        used to genareate every frame of the animation.
 * @param id The identifier of the animation, which may be used in SetProperty
 *        callback to distinguish different animation.
 * @param varianttype The data type of the property variable.
 *
 * @returns handle to the newly created animation.
 */
MGEFF_ANIMATION mgclMGEffAnimationCreate(void* target,
        MGEFF_SETPROPERTY_CB setproperty, int id, enum EffVariantType varianttype);

/**
 * @brief mgclMGEffAnimationSetContext Sets the animation context.
 *
 * @param handle The hanle of the animation.
 * @param context The context to be set.
 */
void mgclMGEffAnimationSetContext(MGEFF_ANIMATION handle, void* context);

/**
 * @brief mgclMGEffAnimationSetFinishedCb Sets the animation Finished callback function.
 *
 * @param handle The handle of the animation.
 * @param cb The Finished callback to be set.
 */
void mgclMGEffAnimationSetFinishedCb(MGEFF_ANIMATION handle, MGEFF_FINISHED_CB cb);

/**
 * @brief mgclMGEffAnimationDelete Deletes an animation.
 *
 * @param handle The animation to be deleted.
 */
void mgclMGEffAnimationDelete(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationGetContext Gets the animation context.
 *
 * @param handle The hanle of the animation.
 *
 * @returns The context of the animation.
 */
void* mgclMGEffAnimationGetContext(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationSetProperty Sets property of an animation.
 *
 * @param handle The handle of the animation.
 * @param id The identifier of the property.
 * @param value The new value to be set.
 */
void mgclMGEffAnimationSetProperty(MGEFF_ANIMATION handle,
        enum EffAnimProperty id, int value);

/**
 * @brief mgclMGEffAnimationGetProperty Gets property of an animation.
 *
 * @param handle The handle of the animation.
 * @param id The identifier of the property.
 *
 * @returns The property value.
 */
int mgclMGEffAnimationGetProperty(MGEFF_ANIMATION handle, enum EffAnimProperty id);

/**
 * @brief mgclMGEffAnimationSetCurve Sets the motion cureve of the animation.
 *
 * @param handle The hanle of the animation.
 * @param type The type of the motion curve to be set.
 */
void mgclMGEffAnimationSetCurve(MGEFF_ANIMATION handle, enum EffMotionType type);

/**
 * @brief mgclMGEffAnimationGetCurve Gets the motion cureve of the animation.
 *
 * @param handle The hanle of the animation.
 *
 * @returns The handle of the motion curve of the animation.
 */
MGEFF_MOTIONCURVE mgclMGEffAnimationGetCurve(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationSetStartValue Set the animation start value.
 *
 * @param handle The hanle of the animation.
 * @param value The start value to be set.
 */
void mgclMGEffAnimationSetStartValue(MGEFF_ANIMATION handle, const void* value);

/**
 * @brief mgclMGEffAnimationGetStartValue Get the animation start value.
 *
 * @param handle The handle of the animation.
 *
 * @returns Start value of the animation specified by \a handle
 */
const void* mgclMGEffAnimationGetStartValue(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationSetEndValue Set the animation end value.
 *
 * @param handle The handle of the animation.
 * @param value The end value.
 */
void mgclMGEffAnimationSetEndValue(MGEFF_ANIMATION handle, const void* value);

/**
 * @brief mgclMGEffAnimationGetEndValue Get the animation end value.
 *
 * @param handle The handle of the animation.
 *
 * @returns The end value of the animation specified by \a handle.
 */
const void* mgclMGEffAnimationGetEndValue(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationAsyncRun Run the animation in the default schedular asynchronous.
 *
 * @param handle The handle of the animation to be attached.
 *
 * @returns If succeed, return 0; otherwise return -1.
 */
int mgclMGEffAnimationAsyncRun(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationStop Stops a running animation.
 *
 * @param handle The animation to be deleted.
 */
void mgclMGEffAnimationStop(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationPaus Pauses a running animation.
 *
 * @param handle The animation to be paused.
 */
void mgclMGEffAnimationPaus(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationResume Resumes a paused animation.
 *
 * @param handle The animation to be resumed.
 */
void mgclMGEffAnimationResume(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationWait Wait an animation.
 *
 * @param hWnd The window handle of that animation's timer relative.
 * @param handle The handle of the animation to be wait.
 *
 * @returns If succeed, return 1; otherwise return 0.
 */
int mgclMGEffAnimationWait(void* hWnd, MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationSyncRun Run the animation in the default schedular synchronous.
 *
 * @param handle The handle of the animation to be attached.
 *
 * @returns If succeed, return 0; otherwise return -1.
 */
int mgclMGEffAnimationSyncRun(MGEFF_ANIMATION handle);

/**
 * @brief mgclMGEffAnimationCreateGroup Create mgeff animation group.
 *
 * @param type Animation group type. It can be the following values:
 *
 *        -MGEFF_PARALLEL
 *        Parallel animation group: all animation in the group will run
 *        simultaneously.
 *
 *        -MGEFF_SEQUENTIAL
 *        Sequential animation group: all animation in the group will run one
 *        after another as when they are added to this animation group.
 *
 * @returns The handle to the newly created animation group.
 */
MGEFF_ANIMATION mgclMGEffAnimationCreateGroup(enum EffAnimationType type);

/**
 * @brief mgclMGEffAnimationAddToGroup Adds an animation to an animation group.
 *
 * @param group Animation group to which new animation will be added.
 * @param animation The animation to be added.
 */
void mgclMGEffAnimationAddToGroup(MGEFF_ANIMATION group, MGEFF_ANIMATION animation);

} // namespace hfcl

#endif /* __HFCL_MGCLANIMATION_H__ */

