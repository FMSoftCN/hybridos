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

#ifndef __HFCL_MGCLSERVCES_H__
#define __HFCL_MGCLSERVCES_H__

namespace hfcl {

/**
 * @brief mgclSetTimerEx Creates a timer with the specified timeout value.
 *
 * @param hWnd The window receives the MSG_TIMER message. If \a timer_proc
 *        is not NULL, MiniGUI will call \a timer_proc instead sending
 *        MSG_TIMER message to this window. If you use timer callback 
 *        procedure, \a hWnd can be any value you can pass.
 * @param id The timeout value of the timer. Note that the timeout value 
 *        with MSG_TIMER message as the first parameter of the message.
 * @param speed The timeout value of the timer. Note that the timeout value 
 *        is in unit of 10 ms.
 * @param timer_proc The timer callback procedure. If this argument is NULL,
 *        MiniGUI will send MSG_TIMER to the window procedure of \a hWnd.
 *
 * @returns true on success, false on error.
 */
bool mgclSetTimerEx(HWND hWnd, int id, unsigned int speed, TIMERPROC timer_proc);

/**
 * @brief mgclKillTimer Destroys a timer.
 *
 * @param hWnd The window owns the timer.
 * @param id The identifier of the timer. If \a id equals or is less than 0, 
 *        this function will kill all timers in the system.
 *
 * @returns The number of actually killed timer.
 */
int mgclKillTimer(HWND hWnd, int id);

} // namespace hfcl

#endif /* __HFCL_MGCLSERVCES_H__ */

