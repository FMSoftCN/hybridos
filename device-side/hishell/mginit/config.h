///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/**
 * \file config.h
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file includes some interfaces used by system manager process.
 *
 \verbatim

    This file is part of HybridOS, a developing operating system based on
    MiniGUI. HybridOs will support embedded systems and smart IoT devices.

    Copyright (C) 2002~2020, Beijing FMSoft Technologies Co., Ltd.
    Copyright (C) 1998~2002, WEI Yongming

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/blog/minigui-licensing-policy/>.

 \endverbatim
 */

/*
 * $Id: config.h 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

// select frame color
#define TOGGLE_FRAME_R          250
#define TOGGLE_FRAME_G          156
#define TOGGLE_FRAME_B          56

// max number of main window in toggle
#define MAX_TOGGLE_APP          9

// animation time for showing and hidding toggle window
#define TOGGLE_ANIMATION_TIME   300

// physical dimensions of close button
#define CLOSE_ICON_WIDTH        30
#define CLOSE_ICON_HEIGHT       30

#define CAPTION_BAR_HEIGHT      30
#define CAPTION_FONT_SIZE       18 

// only for convenience
#define CUR_WIN         (m_fallback_toggle_ctxt.current_window)
#define RECT_TOP(i)     (m_fallback_toggle_ctxt.window_rect[(i)].top & 0xFFFF)
#define RECT_LEFT(i)    (m_fallback_toggle_ctxt.window_rect[(i)].left & 0xFFFF)
#define RECT_BOTTOM(i)  (m_fallback_toggle_ctxt.window_rect[(i)].bottom & 0xFFFF)
#define RECT_RIGHT(i)   (m_fallback_toggle_ctxt.window_rect[(i)].right & 0xFFFF)
#define RECT_W(i)       (RECT_RIGHT(i) - RECT_LEFT(i))
#define RECT_H(i)       (RECT_BOTTOM(i) - RECT_TOP(i))
#define ZNODE_INDEX(i)  (m_fallback_toggle_ctxt.window_rect[(i)].top >> 16)

// line width of selected frame
#define FRAME_WIDTH         5

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _CONFIG_h */
