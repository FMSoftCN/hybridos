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
 * \file compsor-mine.c 
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file is used to build a customer compostor.
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
 * $Id: compsor-mine.c 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgeff/mgeff.h>
#include <glib.h>
#include <librsvg/rsvg.h>

#include "../include/sysconfig.h"
#include "config.h"


#define SIZE_CLIPRC_HEAP        64

#define DIRTY_ZT_PPP            0x01
#define DIRTY_ZT_WIN            0x02
#define DIRTY_ZT_WPP            0x04

#define WS_EX_DIALOGBOX         0x40000000L
#define DEF_OVERLAPPED_OFFSET_X     30
#define DEF_OVERLAPPED_OFFSET_Y     30

struct _CompositorCtxt {
    RECT        rc_screen;  // screen rect - avoid duplicated GetScreenRect calls
    BLOCKHEAP   cliprc_heap;// heap for clipping rects
    CLIPRGN     wins_rgn;   // visible region for all windows (subtract popup menus)
    CLIPRGN     dirty_rgn;  // the dirty region
    CLIPRGN     inv_rgn;    // the invalid region for a specific znode
    Uint32      dirty_types;// the dirty znode types.
};

struct CompositorToggleCtxt {
    CompositorCtxt* fallback_ctxt;              // compositor context of fallback
    BOOL            fallback_toggle;            // the state of toggle if TRUE
    int             main_window_number;         // current number of main window
    int             current_window;             // index of current window or selected window
    int             display_page_number;        // the max icons in one page is 9, the number of pages
    int             current_page;               // index of current page or selected page
    int             unselected_page;            // index of unselected page
    RECT            window_rect[MAX_TOGGLE_APP];// max number of main window is 25
    BOOL            b_FirstTime;                // first time to make animation
    PLOGFONT        font;                       // font for caption in selected window
    PLOGFONT        font_old;                   // unselect font
};

static struct CompositorToggleCtxt m_fallback_toggle_ctxt;      // context of toggle window
static MSGHOOK m_OldMouseHook = NULL;                           // mouse hook function
static MSGHOOK m_OldKeyHook = NULL;                             // keyboard hook function
static MSGHOOK m_OldExtraInputHook = NULL;                      // extra input hook function
static void calculate_rect();                                   // function to calculate toggle window
static HDC m_AnimationDC = NULL;                                // memory DC for start animation
static float m_factor = 1.0;                                    // factor for input control

static HDC create_memdc_from_image_surface (cairo_surface_t* image_surface)
{
    MYBITMAP my_bmp = {
        flags: MYBMP_TYPE_RGB | MYBMP_FLOW_DOWN | MYBMP_TRANSPARENT | MYBMP_ALPHA,
        frames: 1,
        depth: 32,
    };

    my_bmp.w = cairo_image_surface_get_width (image_surface);
    my_bmp.h = cairo_image_surface_get_height (image_surface);
    my_bmp.pitch = cairo_image_surface_get_stride (image_surface);
    my_bmp.bits = cairo_image_surface_get_data (image_surface);
    my_bmp.size = my_bmp.pitch * my_bmp.h;
    my_bmp.transparent = 0;

    return CreateMemDCFromMyBitmap(&my_bmp, NULL);
}

static void paintCloseIcon()
{
    cairo_t *cr = NULL;
    cairo_surface_t * surface = NULL;
    HDC hdc_result = NULL;

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_factor * CLOSE_ICON_WIDTH, m_factor * CLOSE_ICON_HEIGHT);
    cr = cairo_create(surface);

    // draw picture
    cairo_save(cr);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, m_factor * CLOSE_ICON_WIDTH, m_factor * CLOSE_ICON_HEIGHT);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, (float)TOGGLE_FRAME_R / 255, (float)TOGGLE_FRAME_G / 255, (float)TOGGLE_FRAME_B / 255, 1);
    cairo_arc(cr, m_factor * CLOSE_ICON_WIDTH / 2, m_factor * CLOSE_ICON_WIDTH / 2, m_factor * CLOSE_ICON_WIDTH / 2, 0, 2 * M_PI);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_arc(cr, m_factor * CLOSE_ICON_WIDTH / 2, m_factor * CLOSE_ICON_WIDTH / 2, m_factor * CLOSE_ICON_WIDTH / 3, 0, 2 * M_PI);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, (float)TOGGLE_FRAME_R / 255, (float)TOGGLE_FRAME_G / 255, (float)TOGGLE_FRAME_B / 255, 1);
    cairo_set_line_width (cr, 3);
    cairo_move_to(cr, m_factor * CLOSE_ICON_WIDTH / 3, m_factor * CLOSE_ICON_WIDTH * 2 / 3);
    cairo_line_to(cr, m_factor * CLOSE_ICON_WIDTH * 2/ 3, m_factor * CLOSE_ICON_WIDTH / 3);
    cairo_move_to(cr, m_factor * CLOSE_ICON_WIDTH / 3, m_factor * CLOSE_ICON_WIDTH / 3);
    cairo_line_to(cr, m_factor * CLOSE_ICON_WIDTH * 2/ 3, m_factor * CLOSE_ICON_WIDTH  * 2 / 3);
    cairo_stroke(cr);

    // copy the result
    hdc_result = create_memdc_from_image_surface(surface);
    if(hdc_result != HDC_SCREEN && hdc_result != HDC_INVALID) 
    {
        SetMemDCColorKey(hdc_result, MEMDC_FLAG_SRCCOLORKEY, 0xFF000000);
        BitBlt(hdc_result, 0, 0, m_factor * CLOSE_ICON_WIDTH, m_factor * CLOSE_ICON_WIDTH, HDC_SCREEN_SYS, 
                                RECT_RIGHT(CUR_WIN) - m_factor * 20, RECT_TOP(CUR_WIN) - m_factor * 10, 0);
    }
    cairo_restore(cr);

    DeleteMemDC(hdc_result);
    cairo_destroy (cr);
    cairo_surface_destroy(surface);
}

static void paintWindowTitle(const char * title)
{
    cairo_t *cr = NULL;
    cairo_surface_t * surface = NULL;
    HDC hdc_result = NULL;
    int w = 0;
    int h = m_factor * CAPTION_BAR_HEIGHT;
    RECT rect;

    // calculate the size for caption
    SIZE size;
    int offset = GetTabbedTextExtentPoint(HDC_SCREEN_SYS, title, strlen(title),
                    RECT_RIGHT(CUR_WIN) - RECT_LEFT(CUR_WIN) - h - 10, NULL, NULL, NULL, &size);

    if((RECT_RIGHT(CUR_WIN) - RECT_LEFT(CUR_WIN) - h - 10) > size.cx)
        w = size.cx + h;
    else
        w = RECT_RIGHT(CUR_WIN) - RECT_LEFT(CUR_WIN) - h - 10;

    // create cairo
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    cr = cairo_create(surface);

    // draw picture
    cairo_save(cr);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, (float)TOGGLE_FRAME_R / 255, (float)TOGGLE_FRAME_G / 255, (float)TOGGLE_FRAME_B / 255, 1);
    cairo_arc(cr, h / 2, h / 2, h / 2, 0.5 * M_PI, 1.5 * M_PI);
    cairo_close_path(cr);
    cairo_fill(cr);

    cairo_arc(cr, w - h / 2, h / 2, h / 2, -0.5 * M_PI, 0.5 * M_PI);
    cairo_close_path(cr);
    cairo_fill(cr);

    cairo_rectangle(cr, h / 2, 0, w - h, h);
    cairo_fill(cr);
//    cairo_set_source_rgba(cr, 1, 1, 1, 1);
//    cairo_arc(cr, m_factor * CLOSE_ICON_WIDTH / 2, m_factor * CLOSE_ICON_WIDTH / 2, m_factor * CLOSE_ICON_WIDTH / 3, 0, 2 * M_PI);
//    cairo_fill(cr);

//    cairo_set_source_rgba(cr, (float)TOGGLE_FRAME_R / 255, (float)TOGGLE_FRAME_G / 255, (float)TOGGLE_FRAME_B / 255, 1);
//    cairo_set_line_width (cr, 3);
//    cairo_move_to(cr, m_factor * CLOSE_ICON_WIDTH / 3, m_factor * CLOSE_ICON_WIDTH * 2 / 3);
//    cairo_line_to(cr, m_factor * CLOSE_ICON_WIDTH * 2/ 3, m_factor * CLOSE_ICON_WIDTH / 3);
//    cairo_move_to(cr, m_factor * CLOSE_ICON_WIDTH / 3, m_factor * CLOSE_ICON_WIDTH / 3);
//    cairo_line_to(cr, m_factor * CLOSE_ICON_WIDTH * 2/ 3, m_factor * CLOSE_ICON_WIDTH  * 2 / 3);
//    cairo_stroke(cr);

    // copy the result
    hdc_result = create_memdc_from_image_surface(surface);
    if(hdc_result != HDC_SCREEN && hdc_result != HDC_INVALID) 
    {
        SetMemDCColorKey(hdc_result, MEMDC_FLAG_SRCCOLORKEY, 0xFF000000);
        BitBlt(hdc_result, 0, 0, w, h, HDC_SCREEN_SYS, 
              RECT_LEFT(CUR_WIN) + (RECT_RIGHT(CUR_WIN) - RECT_LEFT(CUR_WIN) - w) / 2, RECT_BOTTOM(CUR_WIN) - 3 - h, 0);
    }
    cairo_restore(cr);

    DeleteMemDC(hdc_result);
    cairo_destroy (cr);
    cairo_surface_destroy(surface);

    rect.top = RECT_BOTTOM(CUR_WIN) - 3 - h;
    rect.left = RECT_LEFT(CUR_WIN) + (RECT_RIGHT(CUR_WIN) - RECT_LEFT(CUR_WIN) - w) / 2;
    rect.bottom = rect.top + h;
    rect.right = rect.left + w;
    DrawText(HDC_SCREEN_SYS, title, strlen(title), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

static void draw_select_window(int index)
{
    int znode_index = 0;
    const ZNODEHEADER * znodeheader = NULL;
    int j = 0;

    znode_index = ZNODE_INDEX(index);
    znodeheader = ServerGetWinZNodeHeader(NULL, znode_index, NULL, FALSE);
    if(znodeheader)
    {
        // copy the window content
        SetBrushColor(HDC_SCREEN_SYS, RGBA2Pixel(HDC_SCREEN_SYS, 0x00, 0x00, 0x00, 0xFF));
        FillBox(HDC_SCREEN_SYS, RECT_RIGHT(CUR_WIN) - m_factor * 20, RECT_TOP(CUR_WIN) - m_factor * 10, m_factor * 30, m_factor * 30);

        if((RECTW(znodeheader->rc) == RECTW(g_rcScr)) && (RECTH(znodeheader->rc) == RECTH(g_rcScr)))
            StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                    RECT_LEFT(index), RECT_TOP(index), RECT_RIGHT(index) - RECT_LEFT(index), RECT_BOTTOM(index) - RECT_TOP(index), 0);
        else
        {
            float factor_x = (float)RECTW(znodeheader->rc) / (float)RECTW(g_rcScr);
            float factor_y = (float)RECTH(znodeheader->rc) / (float)RECTH(g_rcScr);
            float factor = (factor_x > factor_y)? factor_x: factor_y;
            int w = 0;
            int h = 0;

            w = (int)((float)RECTW(znodeheader->rc) / factor);
            h = (int)((float)RECTH(znodeheader->rc) / factor);
            factor = (float)(RECT_RIGHT(index) - RECT_LEFT(index)) / (float)RECTW(g_rcScr);
            w = (int)((float)w * factor);
            h = (int)((float)h * factor);

            StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                    RECT_LEFT(index), RECT_TOP(index), w, h, 0);
        }

        // draw select rectangle
        SetPenColor(HDC_SCREEN_SYS, RGB2Pixel(HDC_SCREEN_SYS, TOGGLE_FRAME_R, TOGGLE_FRAME_G, TOGGLE_FRAME_B));
        for(j = 0; j < 5; j++)
            Rectangle(HDC_SCREEN_SYS, RECT_LEFT(index) + j, RECT_TOP(index) + j,
                                      RECT_RIGHT(index) - j, RECT_BOTTOM(index) - j);

        // draw close icon
        paintCloseIcon();

        // draw window title
        paintWindowTitle(znodeheader->caption);
    }
}

static void draw_unselect_window(int index)
{
    int znode_index = 0;
    const ZNODEHEADER * znodeheader = NULL;

    znode_index = ZNODE_INDEX(index);
    znodeheader = ServerGetWinZNodeHeader(NULL, znode_index, NULL, FALSE);
    if(znodeheader)
    {
        // copy the window content
        SetBrushColor(HDC_SCREEN_SYS, RGBA2Pixel(HDC_SCREEN_SYS, 0x00, 0x00, 0x00, 0xFF));
        FillBox(HDC_SCREEN_SYS, RECT_RIGHT(index) - m_factor * 20, RECT_TOP(index) - m_factor * 10, m_factor * 30, m_factor * 30);

        if((RECTW(znodeheader->rc) == RECTW(g_rcScr)) && (RECTH(znodeheader->rc) == RECTH(g_rcScr)))
            StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                    RECT_LEFT(index), RECT_TOP(index), RECT_RIGHT(index) - RECT_LEFT(index),
                    RECT_BOTTOM(index) - RECT_TOP(index), 0);
        else
        {
            float factor_x = (float)RECTW(znodeheader->rc) / (float)RECTW(g_rcScr);
            float factor_y = (float)RECTH(znodeheader->rc) / (float)RECTH(g_rcScr);
            float factor = (factor_x > factor_y)? factor_x: factor_y;
            int w = 0;
            int h = 0;

            w = (int)((float)RECTW(znodeheader->rc) / factor);
            h = (int)((float)RECTH(znodeheader->rc) / factor);
            factor = (float)(RECT_RIGHT(index) - RECT_LEFT(index)) / (float)RECTW(g_rcScr);
            w = (int)((float)w * factor);
            h = (int)((float)h * factor);

            FillBox(HDC_SCREEN_SYS, RECT_LEFT(index), RECT_TOP(index), RECT_RIGHT(index) - RECT_LEFT(index) + 1,
                                    RECT_BOTTOM(index) - RECT_TOP(index) + 1);
            StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                                    RECT_LEFT(index), RECT_TOP(index), w, h, 0);
        }
    }
}

// callback function of animation
static void animated_start(MGEFF_ANIMATION handle, HWND hWnd, int id, RECT *value)
{
    RECT rect = *value;
    int zidx = ZNODE_INDEX(CUR_WIN);
    const ZNODEHEADER * znodeheader = NULL;

    // get the number of app in normal level
    znodeheader = ServerGetWinZNodeHeader(NULL, zidx, NULL, FALSE);
    BitBlt(m_AnimationDC, 0, 0, g_rcScr.right, g_rcScr.bottom, HDC_SCREEN_SYS, 0, 0, 0);
    if((RECTW(znodeheader->rc) == RECTW(g_rcScr)) && (RECTH(znodeheader->rc) == RECTH(g_rcScr)))
        StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                                   rect.left, rect.top, RECTW(rect), RECTH(rect), 0); 
    else
    {
        float factor_x = (float)RECTW(znodeheader->rc) / (float)RECTW(g_rcScr);
        float factor_y = (float)RECTH(znodeheader->rc) / (float)RECTH(g_rcScr);
        float factor = (factor_x > factor_y)? factor_x: factor_y;
        int w = 0;
        int h = 0;

        w = (int)((float)RECTW(znodeheader->rc) / factor);
        h = (int)((float)RECTH(znodeheader->rc) / factor);
        factor = (float)RECTW(rect) / (float)RECTW(g_rcScr);
        w = (int)((float)w * factor);
        h = (int)((float)h * factor);

        StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                rect.left, rect.top, w, h, 0);
    }

    SyncUpdateDC(HDC_SCREEN_SYS);
}

static void toggle_draw(int zidx, HDC hsdc, int sx, int sy, int sw, int sh, HDC hddc, int dx, int dy, DWORD dwRop)
{
    int cur_clientId = 0;
    int idx_topmost = 0;
    int znode_index = 0;
    int i = 0;
    int j = 0;
    RECT screen_rect = {0, 0, g_rcScr.right, g_rcScr.bottom};
    RECT rect;

    // if the main window in normal level is zero, calculate the number of main window
    if(m_fallback_toggle_ctxt.main_window_number == 0)
    {
        idx_topmost = ServerGetTopmostZNodeOfType(NULL, ZOF_TYPE_NORMAL, &cur_clientId);
        if(idx_topmost > 0)
        {
            // get the number of app in normal level
            znode_index = ServerGetNextZNode(NULL, 0, &cur_clientId);
            while(znode_index > 0)
            {
                ZNODEINFO znodeinfo;

                if(ServerGetZNodeInfo(NULL, znode_index, &znodeinfo))
                {
                    if((znodeinfo.type & ZOF_TYPE_MASK) == ZOF_TYPE_NORMAL)     // it the main window in normal level
                    {
                        m_fallback_toggle_ctxt.window_rect[m_fallback_toggle_ctxt.main_window_number].top = znode_index << 16; 
                        m_fallback_toggle_ctxt.window_rect[m_fallback_toggle_ctxt.main_window_number].bottom = 0; 
                        m_fallback_toggle_ctxt.window_rect[m_fallback_toggle_ctxt.main_window_number].left = 0; 
                        m_fallback_toggle_ctxt.window_rect[m_fallback_toggle_ctxt.main_window_number].right = 0; 
                        if(znode_index == idx_topmost)
                            m_fallback_toggle_ctxt.current_window = m_fallback_toggle_ctxt.main_window_number;
                        m_fallback_toggle_ctxt.main_window_number ++;
                    }
                }
                znode_index = ServerGetNextZNode(NULL, znode_index, &cur_clientId);
            }

            // no main window
            if(m_fallback_toggle_ctxt.main_window_number == 0)
            {
                m_fallback_toggle_ctxt.fallback_toggle = FALSE;

                RegisterEventHookFunc(HOOK_EVENT_MOUSE, m_OldMouseHook, NULL);
                RegisterEventHookFunc(HOOK_EVENT_KEY, m_OldKeyHook, NULL);
                RegisterEventHookFunc(HOOK_EVENT_EXTRA, m_OldExtraInputHook, NULL);
    
                CompositorCtxt * ctxt;
                ServerSelectCompositor("fallback", &ctxt);
                return;
            }

            SelectClipRect (HDC_SCREEN_SYS, &screen_rect);
            SetBrushColor(HDC_SCREEN_SYS, RGBA2Pixel(HDC_SCREEN_SYS, 0x00, 0x00, 0x00, 0xFF));
            FillBox(HDC_SCREEN_SYS, 0, 0, g_rcScr.right, g_rcScr.bottom);

            // calculate the rectangle
            calculate_rect();

            // draw the main window
            for(i = 0; i < m_fallback_toggle_ctxt.main_window_number; i++)
                  draw_unselect_window(i);

            if(m_fallback_toggle_ctxt.b_FirstTime)
            {
                MGEFF_ANIMATION animation = NULL;

                m_fallback_toggle_ctxt.b_FirstTime = FALSE;
                m_AnimationDC = CreateMemDC(g_rcScr.right, g_rcScr.bottom, 32, MEMDC_FLAG_SWSURFACE,
                    0x00000000, 0x00000000, 0x00000000, 0x00000000);
                BitBlt(HDC_SCREEN_SYS, 0, 0, g_rcScr.right, g_rcScr.bottom, m_AnimationDC, 0, 0, 0);

                // animation
                animation = mGEffAnimationCreate(NULL, (void *)animated_start, 1, MGEFF_RECT);
                if (animation) 
                {
                    enum EffMotionType motionType = InCirc;
                    int duration = TOGGLE_ANIMATION_TIME;

                    rect.left = RECT_LEFT(CUR_WIN);
                    rect.top = RECT_TOP(CUR_WIN);
                    rect.right = RECT_RIGHT(CUR_WIN);
                    rect.bottom = RECT_BOTTOM(CUR_WIN);

                    mGEffAnimationSetStartValue(animation, &screen_rect);
                    mGEffAnimationSetEndValue(animation, &rect);
                    mGEffAnimationSetDuration(animation, duration);
                    mGEffAnimationSetCurve(animation, motionType);
                    mGEffAnimationSyncRun(animation);
                    mGEffAnimationDelete(animation);
                }
                DeleteMemDC(m_AnimationDC);
                draw_select_window(m_fallback_toggle_ctxt.current_window);
                SyncUpdateDC(HDC_SCREEN_SYS);
                m_AnimationDC = NULL;
            }
            else
            {
                draw_select_window(m_fallback_toggle_ctxt.current_window);
                SyncUpdateDC(HDC_SCREEN_SYS);
            }
        }
        else
        {
            m_fallback_toggle_ctxt.fallback_toggle = FALSE;

            RegisterEventHookFunc(HOOK_EVENT_MOUSE, m_OldMouseHook, NULL);
            RegisterEventHookFunc(HOOK_EVENT_KEY, m_OldKeyHook, NULL);
            RegisterEventHookFunc(HOOK_EVENT_EXTRA, m_OldExtraInputHook, NULL);
    
            CompositorCtxt * ctxt;
            ServerSelectCompositor("fallback", &ctxt);
            return;
        }
    }
    else
    {
        SelectClipRect (HDC_SCREEN_SYS, &screen_rect);
        // whether the zidx is the main window idx
        for(i = 0; i < m_fallback_toggle_ctxt.main_window_number; i++)
        {
            znode_index = ZNODE_INDEX(i);
            if(zidx == znode_index)
                break;
        }

        // zidx is a main window
        if(i < m_fallback_toggle_ctxt.main_window_number)
        {
            if(m_fallback_toggle_ctxt.current_window == i)
                draw_select_window(i);
            else
                draw_unselect_window(i);
            SyncUpdateDC(HDC_SCREEN_SYS);
        }
    }
}

static CompositorCtxt* initialize (const char* name)
{
    CompositorCtxt* ctxt;

    ctxt = malloc (sizeof (CompositorCtxt));
    if (ctxt) {
        ctxt->rc_screen = GetScreenRect();
        InitFreeClipRectList (&ctxt->cliprc_heap, SIZE_CLIPRC_HEAP);
        InitClipRgn (&ctxt->wins_rgn, &ctxt->cliprc_heap);
        InitClipRgn (&ctxt->dirty_rgn, &ctxt->cliprc_heap);
        InitClipRgn (&ctxt->inv_rgn, &ctxt->cliprc_heap);

        SetClipRgn (&ctxt->wins_rgn, &ctxt->rc_screen);

        memset(&m_fallback_toggle_ctxt, 0, sizeof(m_fallback_toggle_ctxt));
        m_fallback_toggle_ctxt.fallback_ctxt = ctxt;
        m_fallback_toggle_ctxt.fallback_toggle = TRUE;
        m_fallback_toggle_ctxt.b_FirstTime = TRUE;
        m_fallback_toggle_ctxt.font = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, 
                        "ttf-Source", "UTF-8", FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN, 
                        FONT_FLIP_NIL, FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE, 
                        FONT_STRUCKOUT_NONE, m_factor * CAPTION_BAR_HEIGHT * 0.4, 0);
        m_fallback_toggle_ctxt.font_old = SelectFont(HDC_SCREEN_SYS, m_fallback_toggle_ctxt.font);
        m_factor = (float)GetGDCapability(HDC_SCREEN, GDCAP_DPI) / 96.0;
    }

    return ctxt;
}

static void terminate (CompositorCtxt* ctxt)
{
    if (ctxt) {
        EmptyClipRgn (&ctxt->wins_rgn);
        EmptyClipRgn (&ctxt->dirty_rgn);
        EmptyClipRgn (&ctxt->inv_rgn);
        DestroyFreeClipRectList (&ctxt->cliprc_heap);
        free (ctxt);
    }
    SelectFont(HDC_SCREEN_SYS, m_fallback_toggle_ctxt.font_old);
    DestroyLogFont(m_fallback_toggle_ctxt.font);
}

/* return the number of subtracted windows above */
static int subtract_opaque_win_znodes_above (CompositorCtxt* ctxt, int from)
{
    const ZNODEHEADER* znode_hdr;
    int nr_subtracted = 0;
    int prev;

    /* subtract opaque window znodes */
    prev = ServerGetPrevZNode (NULL, from, NULL);
    while (prev > 0) {
        CLIPRGN* rgn;

        znode_hdr = ServerGetWinZNodeHeader (NULL, prev, (void**)&rgn, FALSE);
        assert (znode_hdr);

        if ((znode_hdr->flags & ZNIF_VISIBLE) &&
                (znode_hdr->ct & CT_SYSTEM_MASK) == CT_OPAQUE) {

            assert (rgn);
            if (SubtractRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, rgn)) {
                nr_subtracted++;
            }
        }

        prev = ServerGetPrevZNode (NULL, prev, NULL);
    }

    return nr_subtracted;
}

static inline void tile_dirty_region_for_wallpaper (CompositorCtxt* ctxt)
{
    int wp_w = GetGDCapability (HDC_SCREEN, GDCAP_HPIXEL);
    int wp_h = GetGDCapability (HDC_SCREEN, GDCAP_VPIXEL);

    if (wp_w > 0 && wp_h > 0) {

        // tile the dirty region of the wallpaper pattern
        int y = 0;
        int left_h = RECTH (ctxt->rc_screen);

        CopyRegion (&ctxt->inv_rgn, &ctxt->dirty_rgn);
        while (left_h > 0) {

            int x = 0;
            int left_w = RECTW (ctxt->rc_screen);

            while (left_w > 0) {
                if (x > 0 || y > 0) {
                    OffsetRegion (&ctxt->inv_rgn, x, y);
                    UnionRegion (&ctxt->dirty_rgn,
                            &ctxt->dirty_rgn, &ctxt->inv_rgn);
                    OffsetRegion (&ctxt->inv_rgn, -x, -y);
                }

                x += wp_w;
                left_w -= wp_w;
            }

            left_h -= wp_h;
            y += wp_h;
        }

        EmptyClipRgn (&ctxt->inv_rgn);
    }
}

static void composite_wallpaper_rect (CompositorCtxt* ctxt,
            const RECT* dirty_rc)
{
    int wp_w = GetGDCapability (HDC_SCREEN, GDCAP_HPIXEL);
    int wp_h = GetGDCapability (HDC_SCREEN, GDCAP_VPIXEL);

    if (wp_w > 0 && wp_h > 0) {

        // tile the wallpaper pattern
        int y = 0;
        int left_h = RECTH (ctxt->rc_screen);
        while (left_h > 0) {

            int x = 0;
            int left_w = RECTW (ctxt->rc_screen);
            while (left_w > 0) {
                RECT rc = { x, y, x + wp_w, y + wp_h };
                RECT eff_rc;
                if (IntersectRect (&eff_rc, dirty_rc, &rc)) {
                    SelectClipRect (HDC_SCREEN_SYS, &eff_rc);
                    SetMemDCColorKey (HDC_SCREEN, 0, 0);
                    SetMemDCAlpha (HDC_SCREEN, 0, 0);
                    if(!m_fallback_toggle_ctxt.fallback_toggle)
                        BitBlt (HDC_SCREEN, 0, 0, RECTW (rc), RECTH (rc),
                                HDC_SCREEN_SYS, rc.left, rc.top, 0);
                    else
                        toggle_draw(0, HDC_SCREEN, 0, 0, RECTW (rc), RECTH (rc),
                                HDC_SCREEN_SYS, rc.left, rc.top, 0);
                }

                x += wp_w;
                left_w -= wp_w;
            }

            left_h -= wp_h;
            y += wp_h;
        }
    }
    else {
        SetBrushColor (HDC_SCREEN_SYS,
            GetWindowElementPixelEx (HWND_DESKTOP, HDC_SCREEN_SYS,
                WE_BGC_DESKTOP));
        SelectClipRect (HDC_SCREEN_SYS, &ctxt->rc_screen);
        FillBox (HDC_SCREEN_SYS, dirty_rc->left, dirty_rc->top,
                RECTWP(dirty_rc), RECTHP(dirty_rc));
    }
}

static inline void composite_wallpaper (CompositorCtxt* ctxt)
{
    if (!IsEmptyClipRgn (&ctxt->dirty_rgn)) {

        CLIPRECT* crc = ctxt->dirty_rgn.head;
        while (crc) {
            composite_wallpaper_rect (ctxt, &crc->rc);
            crc = crc->next;
        }
    }
}

static void composite_ppp_znodes (CompositorCtxt* ctxt)
{
    int zidx, nr_ppps;

    nr_ppps = ServerGetPopupMenusCount ();
    /* we start from the topmost popup menu znode */
    for (zidx = (nr_ppps - 1); zidx >= 0; zidx--) {
        const ZNODEHEADER* znode_hdr = NULL;
        CLIPRGN* rgn;
        znode_hdr = ServerGetPopupMenuZNodeHeader (zidx, (void**)&rgn, TRUE);
        assert (znode_hdr);
        assert (rgn);

        // we use invalid region as the clipping region of the popup menu znode
        ClipRgnIntersect (&ctxt->inv_rgn, &ctxt->dirty_rgn, rgn);
        if (IsEmptyClipRgn (&ctxt->inv_rgn)) {
            ServerReleasePopupMenuZNodeHeader (zidx);
            _DBG_PRINTF ("ppp %d skipped\n", zidx);
            continue;
        }
        else {
            _DBG_PRINTF ("ppp %d composited\n", zidx);
            // blit this popup menu to screen
            SelectClipRegion (HDC_SCREEN_SYS, &ctxt->inv_rgn);
            if(!m_fallback_toggle_ctxt.fallback_toggle)
                BitBlt (znode_hdr->mem_dc, 0, 0,
                        RECTW (znode_hdr->rc), RECTH (znode_hdr->rc),
                        HDC_SCREEN_SYS, znode_hdr->rc.left, znode_hdr->rc.top, 0);
            else
                toggle_draw(zidx, znode_hdr->mem_dc, 0, 0,
                        RECTW (znode_hdr->rc), RECTH (znode_hdr->rc),
                        HDC_SCREEN_SYS, znode_hdr->rc.left, znode_hdr->rc.top, 0);

            // subtract the clipping region of the popup menu znode
            // from the dirty region, because this compositor
            // handles any popup menu as opaque.
            SubtractRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, &ctxt->inv_rgn);
        }

        ServerReleasePopupMenuZNodeHeader (zidx);
    }

    EmptyClipRgn (&ctxt->inv_rgn);
}

static void composite_opaque_win_znode (CompositorCtxt* ctxt, int from)
{
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn;

    if (IsEmptyClipRgn (&ctxt->dirty_rgn)) {
        return;
    }

    znode_hdr = ServerGetWinZNodeHeader (NULL, from, (void**)&rgn, TRUE);
    assert (znode_hdr);

    if ((znode_hdr->flags & ZNIF_VISIBLE) &&
            ((znode_hdr->ct & CT_SYSTEM_MASK) == CT_OPAQUE) &&
            IntersectRegion (&ctxt->inv_rgn, &ctxt->dirty_rgn, rgn)) {

        assert (rgn);
        SetMemDCColorKey (znode_hdr->mem_dc, 0, 0);
        SetMemDCAlpha (znode_hdr->mem_dc, 0, 0);
        SelectClipRegion (HDC_SCREEN_SYS, &ctxt->inv_rgn);
        if(!m_fallback_toggle_ctxt.fallback_toggle)
            BitBlt (znode_hdr->mem_dc,
                    0, 0, RECTW(znode_hdr->rc), RECTH(znode_hdr->rc),
                    HDC_SCREEN_SYS,
                    znode_hdr->rc.left, znode_hdr->rc.top, 0);
        else
            toggle_draw(from, znode_hdr->mem_dc,
                    0, 0, RECTW(znode_hdr->rc), RECTH(znode_hdr->rc),
                    HDC_SCREEN_SYS,
                    znode_hdr->rc.left, znode_hdr->rc.top, 0);

        SubtractRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, &ctxt->inv_rgn);
        EmptyClipRgn (&ctxt->inv_rgn);
    }

    ServerReleaseWinZNodeHeader (NULL, from);
}

static void composite_all_lucent_win_znodes_above (CompositorCtxt* ctxt,
        const CLIPRGN* dirty_rgn, int zidx)
{
    int prev;

    prev = ServerGetPrevZNode (NULL, zidx, NULL);
    while (prev > 0) {
        const ZNODEHEADER* znode_hdr;
        CLIPRGN* rgn;

        znode_hdr = ServerGetWinZNodeHeader (NULL, prev, (void**)&rgn, TRUE);
        assert (znode_hdr);

        if ((znode_hdr->flags & ZNIF_VISIBLE) &&
               (znode_hdr->ct & CT_SYSTEM_MASK) != CT_OPAQUE &&
               IntersectRegion (&ctxt->inv_rgn, dirty_rgn, rgn)) {

            switch (znode_hdr->ct & CT_SYSTEM_MASK) {
            case CT_COLORKEY:
                SetMemDCAlpha (znode_hdr->mem_dc, 0, 0);
                SetMemDCColorKey (znode_hdr->mem_dc,
                        MEMDC_FLAG_SRCCOLORKEY,
                        DWORD2Pixel (znode_hdr->mem_dc, znode_hdr->ct_arg));
                break;

            case CT_LOGICALPIXEL:
                _WRN_PRINTF("CT_LOGICALPIXEL is not implemented\n");
                break;

            case CT_ALPHACHANNEL:
                SetMemDCColorKey (znode_hdr->mem_dc, 0, 0);
                SetMemDCAlpha (znode_hdr->mem_dc,
                        MEMDC_FLAG_SRCALPHA, (BYTE)znode_hdr->ct_arg);
                break;

            case CT_ALPHAPIXEL:
                SetMemDCColorKey (znode_hdr->mem_dc, 0, 0);
                SetMemDCAlpha (znode_hdr->mem_dc,
                        MEMDC_FLAG_SRCPIXELALPHA, 0);
                break;

            case CT_BLURRED:
                _WRN_PRINTF("CT_BLURRED is not implemented\n");
                break;

            default:
                assert (0); // never touch here.
                break;
            }

            SelectClipRegion (HDC_SCREEN_SYS, &ctxt->inv_rgn);
            //SelectClipRect (HDC_SCREEN_SYS, &znode_hdr->rc);
            if(!m_fallback_toggle_ctxt.fallback_toggle)
                BitBlt (znode_hdr->mem_dc,
                        0, 0, RECTW(znode_hdr->rc), RECTH(znode_hdr->rc),
                        HDC_SCREEN_SYS,
                        znode_hdr->rc.left, znode_hdr->rc.top, 0);
            else
                toggle_draw(prev, znode_hdr->mem_dc,
                        0, 0, RECTW(znode_hdr->rc), RECTH(znode_hdr->rc),
                        HDC_SCREEN_SYS,
                        znode_hdr->rc.left, znode_hdr->rc.top, 0);

            EmptyClipRgn (&ctxt->inv_rgn);
        }

        ServerReleaseWinZNodeHeader (NULL, prev);

        prev = ServerGetPrevZNode (NULL, prev, NULL);
    }
}

static void composite_on_dirty_region (CompositorCtxt* ctxt, int from)
{
    CLIPRGN tmp_rgn;
    int next;

    /* tmp_rgn will be used to save the dirty region for lucent window znodes
       above the current znode. */
    InitClipRgn (&tmp_rgn, &ctxt->cliprc_heap);

    /* subtract the opaque window znodes above the current znode from
       the dirty region. */
    if (from > 0 && subtract_opaque_win_znodes_above (ctxt, from) > 0) {
        if (IsEmptyClipRgn (&ctxt->dirty_rgn)) {
            return;
        }
    }

    /* generate the effective dirty region for windows by substracting
       the region of visible popup menus. */
    IntersectRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, &ctxt->wins_rgn);
    if (IsEmptyClipRgn (&ctxt->dirty_rgn)) {
        return;
    }

    /* compositing the current window znode and the znodes below it */
    if (from <= 0)
        next = ServerGetNextZNode (NULL, 0, NULL);
    else
        next = from;

    while (next > 0) {

        if (!IsEmptyClipRgn (&ctxt->dirty_rgn)) {
            // save dirty region for compositing the lucent znodes above
            // the current znode.
            CopyRegion (&tmp_rgn, &ctxt->dirty_rgn);
        }
        else
            break;

        composite_opaque_win_znode (ctxt, next);
        composite_all_lucent_win_znodes_above (ctxt, &tmp_rgn, next);

        next = ServerGetNextZNode (NULL, next, NULL);
    }

    /* compositing the wallpaper */
    if (!IsEmptyClipRgn (&ctxt->dirty_rgn)) {
        composite_wallpaper (ctxt);

        /* composite all lucent znodes intersected with dirty region. */
        CopyRegion (&tmp_rgn, &ctxt->dirty_rgn);
        composite_all_lucent_win_znodes_above (ctxt, &tmp_rgn, 0);
    }

    EmptyClipRgn (&tmp_rgn);
    SyncUpdateDC (HDC_SCREEN_SYS);
}

static void rebuild_wins_region (CompositorCtxt* ctxt)
{
    int i, nr_ppps;
    const ZNODEHEADER* znode_hdr = NULL;

    SetClipRgn (&ctxt->wins_rgn, &ctxt->rc_screen);

    nr_ppps = ServerGetPopupMenusCount ();
    for (i = 0; i < nr_ppps; i++) {
        CLIPRGN* rgn;
        znode_hdr = ServerGetPopupMenuZNodeHeader (i, (void**)&rgn, FALSE);
        assert (znode_hdr);
        assert (rgn);

        SubtractRegion (&ctxt->wins_rgn, &ctxt->wins_rgn, rgn);
    }
}

static void calc_mainwin_pos (CompositorCtxt* ctxt, MG_Layer* layer,
            DWORD zt_type, int first_for_type, CALCPOSINFO* info)
{
    /* give a default size first */
    if (IsRectEmpty (&info->rc)) {

        info->rc.left = 0;
        info->rc.top = 0;
        if (info->ex_style & WS_EX_DIALOGBOX) {
            info->rc.right = g_rcScr.right / 2;
            info->rc.bottom = g_rcScr.bottom / 3;
        }
        else {
            info->rc.right = g_rcScr.right / 2;
            info->rc.bottom = g_rcScr.bottom;
        }
    }

    if (info->ex_style & WS_EX_DIALOGBOX) {
        // center the window vertically and horinzontally
        int width = info->rc.right - info->rc.left;
        int height = info->rc.bottom - info->rc.top;

        OffsetRect (&info->rc, (g_rcScr.right - width) >> 1,
                    (g_rcScr.bottom - height) >> 1);
    }
    else {
        if (first_for_type == 0) {
            info->rc.left = 0;
            info->rc.top = 0;
        }
        else {
            const ZNODEHEADER* znode_hdr;
            znode_hdr =
                ServerGetWinZNodeHeader (layer, first_for_type, NULL, FALSE);

            info->rc = znode_hdr->rc;
            OffsetRect (&info->rc,
                    DEF_OVERLAPPED_OFFSET_X, DEF_OVERLAPPED_OFFSET_Y);

            /* adjust to a reasonable postion */
            if (info->rc.top > (g_rcScr.bottom * 3 / 4)) {
                OffsetRect (&info->rc, 0, -info->rc.top);
            }

            if (info->rc.left > (g_rcScr.right * 3 / 4)) {
                OffsetRect (&info->rc, -info->rc.left, 0);
            }
        }
    }
}

static void purge_ppp_data (CompositorCtxt* ctxt, int zidx, void* data)
{
    EmptyClipRgn ((CLIPRGN*)data);
    mg_slice_delete (CLIPRGN, data);
}

static void purge_win_data (CompositorCtxt* ctxt, MG_Layer* layer,
        int zidx, void* data)
{
    EmptyClipRgn ((CLIPRGN*)data);
    mg_slice_delete (CLIPRGN, data);
}

static void on_showing_ppp (CompositorCtxt* ctxt, int zidx)
{
    CLIPRGN* rgn;

    rgn = mg_slice_new (CLIPRGN);
    InitClipRgn (rgn, &ctxt->cliprc_heap);
    ServerGetPopupMenuZNodeRegion (zidx, RGN_OP_SET | RGN_OP_FLAG_ABS, rgn);
    ServerSetPopupMenuZNodePrivateData (zidx, rgn);

    /* subtract region of this ppp from wins_rgn */
    SubtractRegion (&ctxt->wins_rgn, &ctxt->wins_rgn, rgn);
}

static BOOL reset_dirty_region (CompositorCtxt* ctxt, MG_Layer* layer)
{
    if (layer != mgTopmostLayer)
        return FALSE;

    ctxt->dirty_types = 0;
    EmptyClipRgn (&ctxt->dirty_rgn);
    return TRUE;
}

static BOOL merge_dirty_region (CompositorCtxt* ctxt,
        const ZNODEHEADER* znode_hdr, const CLIPRGN* znode_rgn)
{
    EmptyClipRgn (&ctxt->inv_rgn);
    if (znode_hdr && znode_hdr->dirty_rcs) {
        int i;
        for (i = 0; i < znode_hdr->nr_dirty_rcs; i++) {
            RECT rc;

            // device coordinates to screen coordinates
            rc = znode_hdr->dirty_rcs [i];
            OffsetRect (&rc, znode_hdr->rc.left, znode_hdr->rc.top);
            AddClipRect (&ctxt->inv_rgn, &rc);
        }

        if (znode_rgn)
            IntersectRegion (&ctxt->inv_rgn, &ctxt->inv_rgn, znode_rgn);
        else
            IntersectClipRect (&ctxt->inv_rgn, &ctxt->rc_screen);

    }

    if (IsEmptyClipRgn (&ctxt->inv_rgn)) {
        return FALSE;
    }

    UnionRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, &ctxt->inv_rgn);
    EmptyClipRgn (&ctxt->inv_rgn);
    return TRUE;
}

static BOOL merge_dirty_ppp (CompositorCtxt* ctxt, MG_Layer* layer, int zidx)
{
    BOOL rc = FALSE;
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn;

    _DBG_PRINTF("called: %d\n", zidx);

    /* the fallback compositor only manages znodes on the topmost layer. */
    if (layer != mgTopmostLayer)
        return rc;

    znode_hdr = ServerGetPopupMenuZNodeHeader (zidx, (void**)&rgn, TRUE);
    if (merge_dirty_region (ctxt, znode_hdr, rgn)) {
        ctxt->dirty_types |= DIRTY_ZT_PPP;
        rc = TRUE;
    }
    ServerReleasePopupMenuZNodeHeader (zidx);

    return rc;
}

static BOOL merge_dirty_win (CompositorCtxt* ctxt, MG_Layer* layer, int zidx)
{
    BOOL rc = FALSE;
    const ZNODEHEADER* znode_hdr = NULL;
    CLIPRGN *rgn;

    /* the fallback compositor only manages znodes on the topmost layer. */
    if (layer != mgTopmostLayer)
        return rc;

    _DBG_PRINTF("called: %d\n", zidx);

    /* merge the dirty region */
    znode_hdr = ServerGetWinZNodeHeader (NULL, zidx, (void**)&rgn, TRUE);
    if (merge_dirty_region (ctxt, znode_hdr, rgn)) {
        ctxt->dirty_types |= DIRTY_ZT_WIN;
        rc = TRUE;
    }
    ServerReleaseWinZNodeHeader (NULL, zidx);
    return rc;
}

static BOOL merge_dirty_wpp (CompositorCtxt* ctxt, MG_Layer* layer)
{
    BOOL rc = FALSE;
    const ZNODEHEADER* znode_hdr;

    znode_hdr = ServerGetWinZNodeHeader (NULL, 0, NULL, TRUE);
    if (merge_dirty_region (ctxt, znode_hdr, NULL)) {
        tile_dirty_region_for_wallpaper (ctxt);
        //subtract_opaque_win_znodes_above (ctxt, 0);
        ctxt->dirty_types |= DIRTY_ZT_WPP;
        rc = TRUE;
    }

    ServerReleaseWinZNodeHeader (NULL, 0);
    return rc;
}

static BOOL refresh_dirty_region (CompositorCtxt* ctxt, MG_Layer* layer)
{
    /* the fallback compositor only manages znodes on the topmost layer. */
    if (layer != mgTopmostLayer || IsEmptyClipRgn (&ctxt->dirty_rgn))
        return FALSE;

    if (ctxt->dirty_types & DIRTY_ZT_PPP)
        composite_ppp_znodes (ctxt);
    composite_on_dirty_region (ctxt, 0);

    EmptyClipRgn (&ctxt->dirty_rgn);

    SyncUpdateDC (HDC_SCREEN_SYS);
    return TRUE;
}

static void on_hiding_ppp (CompositorCtxt* ctxt, int zidx)
{
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn = NULL;

    znode_hdr = ServerGetPopupMenuZNodeHeader (zidx, (void**)&rgn, FALSE);
    assert (znode_hdr);

    assert (rgn);
    EmptyClipRgn ((CLIPRGN*)rgn);
    mg_slice_delete (CLIPRGN, rgn);
    ServerSetPopupMenuZNodePrivateData (zidx, NULL);
}

static void on_dirty_screen (CompositorCtxt* ctxt,
        MG_Layer* layer, DWORD cause_type, const RECT* rc_dirty)
{
    /* this compositor only manages znodes on the topmost layer. */
    if (layer == NULL)
        layer = mgTopmostLayer;
    else if (layer != mgTopmostLayer)
        return;

    _DBG_PRINTF ("called\n");
    /* generate the dirty region */
    if (rc_dirty) {
        SetClipRgn (&ctxt->dirty_rgn, &ctxt->rc_screen);
        IntersectClipRect (&ctxt->dirty_rgn, rc_dirty);
        if (IsEmptyClipRgn (&ctxt->dirty_rgn)) {
            return;
        }
    }
    else {
        // all screen is dirty.
        SetClipRgn (&ctxt->dirty_rgn, &ctxt->rc_screen);
    }

    /* if the cause type is popup menu:
     * - rebuild the region for windows;
     * - composite all popup menus.
     *
     * note that if any popup menu was composited to the screen,
     * the dirty region will be shrank, because this compositor
     * handles any popup menu as opaque.
     */
    if (cause_type == ZNIT_POPUPMENU) {
        rebuild_wins_region (ctxt);
        composite_ppp_znodes (ctxt);
    }

    composite_on_dirty_region (ctxt, 0);
}

static void refresh (CompositorCtxt* ctxt)
{
    rebuild_wins_region (ctxt);
    on_dirty_screen (ctxt, NULL, ZNIT_NULL, NULL);
}

static void on_closed_menu (CompositorCtxt* ctxt, const RECT* rc_bound)
{
    _DBG_PRINTF ("called\n");
    /* reset the wins_rgn with screen rectangle, because the menu closed */
    SetClipRgn (&ctxt->wins_rgn, &ctxt->rc_screen);

    /* composite the dirty screen rectangle.
     * we pass cause_type with ZNIT_NULL for no any popup menu currently. */
    on_dirty_screen (ctxt, NULL, ZNIT_NULL, rc_bound);
}

static void on_showing_win (CompositorCtxt* ctxt, MG_Layer* layer, int zidx)
{
    CLIPRGN* rgn;

    // the fallback compositor only manages znodes on the topmost layer.
    if (layer != mgTopmostLayer)
        return;

    _DBG_PRINTF ("called: %d\n", zidx);
    rgn = mg_slice_new (CLIPRGN);
    InitClipRgn (rgn, &ctxt->cliprc_heap);
    ServerGetWinZNodeRegion (layer, zidx, RGN_OP_SET | RGN_OP_FLAG_ABS, rgn);
    ServerSetWinZNodePrivateData (layer, zidx, rgn);

    m_fallback_toggle_ctxt.main_window_number = 0;
}

static void on_hiding_win (CompositorCtxt* ctxt, MG_Layer* layer, int zidx)
{
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn = NULL;

    // the fallback compositor only manages znodes on the topmost layer.
    if (layer != mgTopmostLayer)
        return;

    _DBG_PRINTF ("called: %d\n", zidx);
    znode_hdr = ServerGetWinZNodeHeader (layer, zidx, (void**)&rgn, FALSE);
    assert (znode_hdr);

    assert (rgn);
    EmptyClipRgn (rgn);
    mg_slice_delete (CLIPRGN, rgn);
    ServerSetWinZNodePrivateData (layer, zidx, NULL);

    m_fallback_toggle_ctxt.main_window_number = 0;
}

/* for both on_raised_win and on_changed_ct */
static void refresh_win (CompositorCtxt* ctxt, MG_Layer* layer, int zidx)
{
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn = NULL;

    // the fallback compositor only manages znodes on the topmost layer.
    if (layer != mgTopmostLayer)
        return;

    _DBG_PRINTF ("called\n");
    znode_hdr = ServerGetWinZNodeHeader (layer, zidx, (void**)&rgn, FALSE);
    assert(znode_hdr);

    if (!(znode_hdr->flags & ZNIF_VISIBLE)) {
        return;
    }

    assert (rgn);
    CopyRegion (&ctxt->dirty_rgn, rgn);
    if (IntersectClipRect (&ctxt->dirty_rgn, &ctxt->rc_screen))
        composite_on_dirty_region (ctxt, zidx);
}

static void on_moved_win (CompositorCtxt* ctxt, MG_Layer* layer, int zidx,
        const RECT* rc_org)
{
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn = NULL;

    _DBG_PRINTF("called: %d\n", zidx);
    // the fallback compositor only manages znodes on the topmost layer.
    if (layer != mgTopmostLayer)
        return;

    znode_hdr = ServerGetWinZNodeHeader (layer, zidx, (void**)&rgn, FALSE);
    assert (znode_hdr);

    if (!(znode_hdr->flags & ZNIF_VISIBLE)) {
        return;
    }

    assert (rgn);
    CopyRegion (&ctxt->dirty_rgn, rgn);
    // re-generate the region
    ServerGetWinZNodeRegion (layer, zidx,
            RGN_OP_SET | RGN_OP_FLAG_ABS, rgn);
    ServerSetWinZNodePrivateData (layer, zidx, rgn);
    UnionRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, rgn);

    /* calculate the dirty region */
    if (IntersectClipRect (&ctxt->dirty_rgn, &ctxt->rc_screen))
        composite_on_dirty_region (ctxt, zidx);
}

static void on_changed_rgn (CompositorCtxt* ctxt, MG_Layer* layer,
        int zidx, const RECT* rc_org_bound)
{
    const ZNODEHEADER* znode_hdr;
    CLIPRGN* rgn = NULL;

    _DBG_PRINTF("called: %d\n", zidx);
    /* the fallback compositor only manages znodes on the topmost layer. */
    if (layer != mgTopmostLayer)
        return;

    /* update the region of the current window znode. */
    znode_hdr = ServerGetWinZNodeHeader (layer, zidx, (void**)&rgn, FALSE);
    assert (znode_hdr);

    if (!(znode_hdr->flags & ZNIF_VISIBLE)) {
        return;
    }

    assert (rgn);
    CopyRegion (&ctxt->dirty_rgn, rgn);

    // re-generate the region
    ServerGetWinZNodeRegion (layer, zidx,
            RGN_OP_SET | RGN_OP_FLAG_ABS, rgn);
    ServerSetWinZNodePrivateData (layer, zidx, rgn);

    /* calculate the dirty region */
    UnionRegion (&ctxt->dirty_rgn, &ctxt->dirty_rgn, rgn);
    if (IntersectClipRect (&ctxt->dirty_rgn, &ctxt->rc_screen))
        composite_on_dirty_region (ctxt, zidx);
}

CompositorOps mine_compositor = {
    initialize: initialize,
    terminate: terminate,
    refresh: refresh,
    calc_mainwin_pos: calc_mainwin_pos,
    purge_ppp_data: purge_ppp_data,
    purge_win_data: purge_win_data,
    reset_dirty_region: reset_dirty_region,
    merge_dirty_ppp: merge_dirty_ppp,
    merge_dirty_win: merge_dirty_win,
    merge_dirty_wpp: merge_dirty_wpp,
    refresh_dirty_region: refresh_dirty_region,
    on_dirty_screen: on_dirty_screen,
    on_showing_ppp: on_showing_ppp,
    on_hiding_ppp: on_hiding_ppp,
    on_closed_menu: on_closed_menu,
    on_showing_win: on_showing_win,
    on_hiding_win: on_hiding_win,
    on_raised_win: refresh_win,
    on_changed_ct: refresh_win,
    on_changed_rgn: on_changed_rgn,
    on_moved_win: on_moved_win,
};

// callback function of animation
static void animated_end(MGEFF_ANIMATION handle, HWND hWnd, int id, RECT *value)
{
    RECT rect = *value;
    int zidx = ZNODE_INDEX(CUR_WIN);
    const ZNODEHEADER * znodeheader = NULL;

    // get the number of app in normal level
    znodeheader = ServerGetWinZNodeHeader(NULL, zidx, NULL, FALSE);
    if((RECTW(znodeheader->rc) == RECTW(g_rcScr)) && (RECTH(znodeheader->rc) == RECTH(g_rcScr)))
        StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, 
        HDC_SCREEN_SYS, rect.left, rect.top, RECTW(rect), RECTH(rect), 0); 
    else
    {
        float factor_x = (float)RECTW(znodeheader->rc) / (float)RECTW(g_rcScr);
        float factor_y = (float)RECTH(znodeheader->rc) / (float)RECTH(g_rcScr);
        float factor = (factor_x > factor_y)? factor_x: factor_y;
        int w = 0;
        int h = 0;

        w = (int)((float)RECTW(znodeheader->rc) / factor);
        h = (int)((float)RECTH(znodeheader->rc) / factor);
        factor = (float)RECTW(rect) / (float)RECTW(g_rcScr);
        w = (int)((float)w * factor);
        h = (int)((float)h * factor);

        StretchBlt(znodeheader->mem_dc, 0, 0, znodeheader->rc.right, znodeheader->rc.bottom, HDC_SCREEN_SYS, 
                rect.left, rect.top, w, h, 0);
    }
    SyncUpdateDC(HDC_SCREEN_SYS);
}

static int MouseHook(void* context, HWND dst_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    BOOL b_close = FALSE;

    if(msg == MSG_LBUTTONDOWN)
    {
    }
    else if(msg == MSG_LBUTTONUP)
    {
        int x = LOWORD (lparam); 
        int y = HIWORD (lparam);
        int i = 0;
        RECT rect;

        for(i = 0; i < m_fallback_toggle_ctxt.main_window_number; i++)
        {
            rect.left = RECT_RIGHT(i) - 20;
            rect.top = RECT_TOP(i)- 10;
            rect.right = rect.left + 30;
            rect.bottom = rect.top + 30;
            
            if(PtInRect (&rect, x, y))
            {
                b_close = TRUE;
                break;
            }

            rect.left = RECT_LEFT(i);
            rect.top = RECT_TOP(i);
            rect.right = RECT_RIGHT(i);
            rect.bottom = RECT_BOTTOM(i);

            if(PtInRect (&rect, x, y))
                break;
        }

        if(i < m_fallback_toggle_ctxt.main_window_number)
        {
            if(b_close)
            {
                MSG msg;
                int node_index = 0;
                const ZNODEHEADER * znodeheader = NULL;

                msg.message = MSG_CLOSE;
                msg.wParam = 0;
                msg.lParam = 0;

                node_index = ZNODE_INDEX(i);
                znodeheader = ServerGetWinZNodeHeader(NULL, node_index, NULL, FALSE);
                if(znodeheader)
                {
                    msg.hwnd = znodeheader->hwnd;
                    Send2Client(&msg, znodeheader->cli);
                }
            }
            else
            {
                MGEFF_ANIMATION animation = NULL;
                int  znode_index = ZNODE_INDEX(i);

                // set current window for animation
                m_fallback_toggle_ctxt.current_window = i;

                // animation
                animation = mGEffAnimationCreate(NULL, (void *)animated_end, 1, MGEFF_RECT);
                if (animation) 
                {
                    enum EffMotionType motionType = InCirc;
                    int duration = TOGGLE_ANIMATION_TIME;
                    RECT screen_rect = {0, 0, g_rcScr.right, g_rcScr.bottom};

                    SelectClipRect (HDC_SCREEN_SYS, &screen_rect);

                    mGEffAnimationSetStartValue(animation, &rect);
                    mGEffAnimationSetEndValue(animation, &screen_rect);
                    mGEffAnimationSetDuration(animation, duration);
                    mGEffAnimationSetCurve(animation, motionType);
                    mGEffAnimationSyncRun(animation);
                    mGEffAnimationDelete(animation);
                }

                // set this main window to the top
                ServerDoZNodeOperation(NULL, znode_index, ZNOP_MOVE2TOP, NULL, FALSE);

                // unregister hook
                RegisterEventHookFunc(HOOK_EVENT_MOUSE, m_OldMouseHook, NULL);
                RegisterEventHookFunc(HOOK_EVENT_KEY, m_OldKeyHook, NULL);
                RegisterEventHookFunc(HOOK_EVENT_EXTRA, m_OldExtraInputHook, NULL);

                // toggle compositor
                CompositorCtxt * ctxt;
                ServerSelectCompositor("fallback", &ctxt);
            }
        }
        return HOOK_STOP;
    }
    else if(msg == MSG_MOUSEMOVE)
    {
        int x = LOWORD (lparam); 
        int y = HIWORD (lparam);
        int i = 0;
        RECT rect;

        for(i = 0; i < m_fallback_toggle_ctxt.main_window_number; i++)
        {
            rect.left = RECT_LEFT(i);
            rect.top = RECT_TOP(i);
            rect.right = RECT_RIGHT(i);
            rect.bottom = RECT_BOTTOM(i);
            
            if(PtInRect (&rect, x, y))
                break;
        }

        if(i < m_fallback_toggle_ctxt.main_window_number)
        {
            if(i != m_fallback_toggle_ctxt.current_window)
            {
                rect.top = 0;
                rect.left = 0;
                rect.right = g_rcScr.right;
                rect.bottom = g_rcScr.bottom;

                SelectClipRect (HDC_SCREEN_SYS, &rect);
                draw_unselect_window(m_fallback_toggle_ctxt.current_window);
                m_fallback_toggle_ctxt.current_window = i;
                draw_select_window(m_fallback_toggle_ctxt.current_window);
                SyncUpdateDC(HDC_SCREEN_SYS);
            }
        }
    }

    return HOOK_STOP;
}

static int KeyHook(void* context, HWND dst_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    int znode_index = 0;
    MGEFF_ANIMATION animation = NULL;
    RECT screen_rect = {0, 0, g_rcScr.right, g_rcScr.bottom};
    RECT rect;

    if (msg == MSG_KEYUP) 
    {
        switch (wparam) 
        {
            case SCANCODE_TAB:
            case SCANCODE_DOWN:
            case SCANCODE_RIGHT:
                if(m_fallback_toggle_ctxt.main_window_number > 1)
                {
                    SelectClipRect (HDC_SCREEN_SYS, &screen_rect);
                    draw_unselect_window(m_fallback_toggle_ctxt.current_window);

                    if(++ m_fallback_toggle_ctxt.current_window == m_fallback_toggle_ctxt.main_window_number)
                        m_fallback_toggle_ctxt.current_window = 0;
                    draw_select_window(m_fallback_toggle_ctxt.current_window);
                    SyncUpdateDC(HDC_SCREEN_SYS);
                }
                return HOOK_STOP;
            case SCANCODE_UP:
            case SCANCODE_LEFT:
                if(m_fallback_toggle_ctxt.main_window_number > 1)
                {
                    SelectClipRect (HDC_SCREEN_SYS, &screen_rect);
                    draw_unselect_window(m_fallback_toggle_ctxt.current_window);
                    if(-- m_fallback_toggle_ctxt.current_window < 0)
                        m_fallback_toggle_ctxt.current_window = m_fallback_toggle_ctxt.main_window_number - 1;
                    draw_select_window(m_fallback_toggle_ctxt.current_window);
                    SyncUpdateDC(HDC_SCREEN_SYS);
                }
                return HOOK_STOP;
            case SCANCODE_ENTER:
                // animation
                animation = mGEffAnimationCreate(NULL, (void *)animated_end, 1, MGEFF_RECT);
                if (animation) 
                {
                    enum EffMotionType motionType = InCirc;
                    int duration = TOGGLE_ANIMATION_TIME;

                    rect.left = RECT_LEFT(CUR_WIN);
                    rect.top = RECT_TOP(CUR_WIN);
                    rect.right = RECT_RIGHT(CUR_WIN);
                    rect.bottom = RECT_BOTTOM(CUR_WIN);

                    SelectClipRect (HDC_SCREEN_SYS, &screen_rect);

                    mGEffAnimationSetStartValue(animation, &rect);
                    mGEffAnimationSetEndValue(animation, &screen_rect);
                    mGEffAnimationSetDuration(animation, duration);
                    mGEffAnimationSetCurve(animation, motionType);
                    mGEffAnimationSyncRun(animation);
                    mGEffAnimationDelete(animation);
                }

                znode_index = ZNODE_INDEX(CUR_WIN);
                ServerDoZNodeOperation(NULL, znode_index, ZNOP_MOVE2TOP, NULL, FALSE);

                RegisterEventHookFunc(HOOK_EVENT_MOUSE, m_OldMouseHook, NULL);
                RegisterEventHookFunc(HOOK_EVENT_KEY, m_OldKeyHook, NULL);
                RegisterEventHookFunc(HOOK_EVENT_EXTRA, m_OldExtraInputHook, NULL);
    
                CompositorCtxt * ctxt;
                ServerSelectCompositor("fallback", &ctxt);
                return HOOK_STOP;
        }
    }
    return HOOK_STOP;
}

static int ExtraInputHook(void* context, HWND dst_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return HOOK_STOP;
}

void mine_compositor_toggle_state(BOOL toggle)
{
    if(toggle)
    {
        const CompositorOps * compositorOps = NULL;
        CompositorCtxt * ctxt;

        m_fallback_toggle_ctxt.main_window_number = 0;
        m_fallback_toggle_ctxt.b_FirstTime = TRUE;

        // select customer compositor
        compositorOps = ServerSelectCompositor(MIME_COMPOSITOR, &ctxt);
        if(compositorOps != &mine_compositor)
        {
            return;
        }

        m_OldMouseHook = RegisterEventHookFunc(HOOK_EVENT_MOUSE, MouseHook, NULL);
        m_OldKeyHook = RegisterEventHookFunc(HOOK_EVENT_KEY, KeyHook, NULL);
        m_OldExtraInputHook = RegisterEventHookFunc(HOOK_EVENT_EXTRA, ExtraInputHook, NULL);

    }
}

static void calculate_rect()
{
    if(m_fallback_toggle_ctxt.main_window_number == 1)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.382 / 2;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.382 / 2;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.618;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.618;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 2)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.06;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.59 / 2;
        m_fallback_toggle_ctxt.window_rect[0].right =  RECT_LEFT(0)+ g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.41;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.53;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.59 / 2;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1)+ g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.41;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 3)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.59 / 2;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.06;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.41;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.06;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.53;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.41;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.53;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.53;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.41;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 4)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.06;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.06;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.41;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.53;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.06;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.41;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.06;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.53;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.41;

        m_fallback_toggle_ctxt.window_rect[3].left += g_rcScr.right * 0.53;
        m_fallback_toggle_ctxt.window_rect[3].top += g_rcScr.bottom * 0.53;
        m_fallback_toggle_ctxt.window_rect[3].right = RECT_LEFT(3) + g_rcScr.right * 0.41;
        m_fallback_toggle_ctxt.window_rect[3].bottom = RECT_TOP(3) + g_rcScr.bottom * 0.41;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 5)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.20;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.14;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.52;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.14;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.56;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[3].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[3].top += g_rcScr.bottom * 0.56;
        m_fallback_toggle_ctxt.window_rect[3].right = RECT_LEFT(3) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[3].bottom = RECT_TOP(3) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[4].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[4].top += g_rcScr.bottom * 0.56;
        m_fallback_toggle_ctxt.window_rect[4].right = RECT_LEFT(4) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[4].bottom = RECT_TOP(4) + g_rcScr.bottom * 0.28;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 6)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.14;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.14;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.14;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[3].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[3].top += g_rcScr.bottom * 0.56;
        m_fallback_toggle_ctxt.window_rect[3].right = RECT_LEFT(3) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[3].bottom = RECT_TOP(3) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[4].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[4].top += g_rcScr.bottom * 0.56;
        m_fallback_toggle_ctxt.window_rect[4].right = RECT_LEFT(4) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[4].bottom = RECT_TOP(4) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[5].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[5].top += g_rcScr.bottom * 0.56;
        m_fallback_toggle_ctxt.window_rect[5].right = RECT_LEFT(5) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[5].bottom = RECT_TOP(5) + g_rcScr.bottom * 0.28;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 7)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.20;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.52;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.20;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[3].left += g_rcScr.right * 0.52;
        m_fallback_toggle_ctxt.window_rect[3].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[3].right = RECT_LEFT(3) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[3].bottom = RECT_TOP(3) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[4].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[4].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[4].right = RECT_LEFT(4) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[4].bottom = RECT_TOP(4) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[5].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[5].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[5].right = RECT_LEFT(5) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[5].bottom = RECT_TOP(5) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[6].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[6].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[6].right = RECT_LEFT(6) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[6].bottom = RECT_TOP(6) + g_rcScr.bottom * 0.28;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 8)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.20;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.52;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[3].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[3].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[3].right = RECT_LEFT(3) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[3].bottom = RECT_TOP(3) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[4].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[4].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[4].right = RECT_LEFT(4) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[4].bottom = RECT_TOP(4) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[5].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[5].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[5].right = RECT_LEFT(5) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[5].bottom = RECT_TOP(5) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[6].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[6].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[6].right = RECT_LEFT(6) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[6].bottom = RECT_TOP(6) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[7].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[7].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[7].right = RECT_LEFT(7) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[7].bottom = RECT_TOP(7) + g_rcScr.bottom * 0.28;
    }
    else if(m_fallback_toggle_ctxt.main_window_number == 9)
    {
        m_fallback_toggle_ctxt.window_rect[0].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[0].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[0].right = RECT_LEFT(0) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[0].bottom = RECT_TOP(0) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[1].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[1].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[1].right = RECT_LEFT(1) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[1].bottom = RECT_TOP(1) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[2].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[2].top += g_rcScr.bottom * 0.04;
        m_fallback_toggle_ctxt.window_rect[2].right = RECT_LEFT(2) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[2].bottom = RECT_TOP(2) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[3].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[3].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[3].right = RECT_LEFT(3) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[3].bottom = RECT_TOP(3) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[4].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[4].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[4].right = RECT_LEFT(4) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[4].bottom = RECT_TOP(4) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[5].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[5].top += g_rcScr.bottom * 0.36;
        m_fallback_toggle_ctxt.window_rect[5].right = RECT_LEFT(5) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[5].bottom = RECT_TOP(5) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[6].left += g_rcScr.right * 0.04;
        m_fallback_toggle_ctxt.window_rect[6].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[6].right = RECT_LEFT(6) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[6].bottom = RECT_TOP(6) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[7].left += g_rcScr.right * 0.36;
        m_fallback_toggle_ctxt.window_rect[7].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[7].right = RECT_LEFT(7) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[7].bottom = RECT_TOP(7) + g_rcScr.bottom * 0.28;

        m_fallback_toggle_ctxt.window_rect[8].left += g_rcScr.right * 0.68;
        m_fallback_toggle_ctxt.window_rect[8].top += g_rcScr.bottom * 0.68;
        m_fallback_toggle_ctxt.window_rect[8].right = RECT_LEFT(8) + g_rcScr.right * 0.28;
        m_fallback_toggle_ctxt.window_rect[8].bottom = RECT_TOP(8) + g_rcScr.bottom * 0.28;
    }
}

