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
 * \file dockbar.c
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file implements dock bar in system manager process.
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
 * $Id: dockbar.c 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <libgen.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <mgeff/mgeff.h>
#include <glib.h>
#include <librsvg/rsvg.h>

#include "../include/sysconfig.h"
#include "config.h"

static RsvgStylePair button_color_pair [6];
static char button_color[6][32];
//static RsvgStylePair button_color_pair [] = {
//    {"color", "#389CFA", 0},
//};

//static RsvgStylePair power_button_color_pair [] = {
//    {"color", "#ff0000", 0},
//};


extern HWND m_hStatusBar;                       // handle of status bar
extern HWND m_hDockBar;                         // handle of dock bar
static int m_DockBar_Height = 0;                // height of dock bar
static int m_DockBar_X = 0;                     // the X coordinate of top left corner
static MGEFF_ANIMATION m_animation = NULL;      // handle of animation
static int m_direction = DIRECTION_SHOW;        // the direction of animation
static float m_factor = 0;                      // DPI / 96
static RECT m_rect[BUTTON_COUNT];               // area for BUTTON
static float m_Arrow_angle = 0;                 // angle of arrow button
static RsvgHandle * m_arrow_svg_handle = NULL;  // svg handle for arrow file

static int m_DockBar_Start_x = 0;               // it is only for convenience for animation
static int m_DockBar_Start_y = 0;
static int m_DockBar_End_x = 0;
static int m_DockBar_End_y = 0;

static int m_DockBar_Left_Length = 0;           // the visible dock bar length when hidden
static int m_Button_Interval = 0;               // the interval length between dock buttons

static cairo_t *cr[BUTTON_COUNT];
static cairo_surface_t *surface[BUTTON_COUNT];

// start another process
static pid_t exec_app (char * app)
{
    pid_t pid = 0;
    char buff [PATH_MAX + NAME_MAX + 1];
    char execPath[PATH_MAX + 1];

    memset(buff, 0, PATH_MAX + NAME_MAX + 1);

    if ((pid = vfork ()) > 0) {
        fprintf (stderr, "new child, pid: %d.\n", pid);
    }
    else if (pid == 0) {
        readlink("/proc/self/exe", execPath, PATH_MAX);
        sprintf(buff, "%s/%s", dirname(execPath), app);
        execl (buff, app, NULL);
        perror ("execl");
        _exit (1);
    }
    else {
        perror ("vfork");
    }
    return pid;
}

// callback function of animation
static void animated_cb(MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    float factor = 0;
    if(m_DockBar_X != *value)
    {
        m_DockBar_X = *value;
        
        factor = (float)(m_DockBar_X - m_DockBar_Start_x) / (float)(m_DockBar_End_x - m_DockBar_Start_x - m_DockBar_Left_Length);
        m_Arrow_angle = -1.0 * M_PI * factor;
        MoveWindow(hWnd, m_DockBar_X, m_DockBar_Start_y, m_DockBar_End_x - m_DockBar_Start_x, m_DockBar_Height, TRUE);
    }
}

// the function which will be invoked at the end of animation
static void animated_end(MGEFF_ANIMATION handle)
{
    HWND hWnd = (HWND)mGEffAnimationGetTarget(handle);
    mGEffAnimationDelete(m_animation);
    m_animation = NULL;

    if((m_direction == DIRECTION_SHOW) && hWnd)
        SetTimer(hWnd, ID_SHOW_TIMER, DOCKBAR_VISIBLE_TIME);
}

// create an animation and start, it is asynchronous
static void create_animation(HWND hWnd)
{
    if(m_animation)
    {
        mGEffAnimationDelete(m_animation);
        m_animation = NULL;
    }

    m_animation = mGEffAnimationCreate((void *)hWnd, (void *)animated_cb, 1, MGEFF_INT);
    if (m_animation) 
    {
        int start = 0;
        int end = 0;
        int duration = 0;
        enum EffMotionType motionType = InCirc;

        start = m_DockBar_X;
        if(m_direction == DIRECTION_HIDE)
        {
            end = g_rcScr.right - m_DockBar_Left_Length;
            motionType = OutCirc;
            duration = DOCKBAR_ANIMATION_TIME * (g_rcScr.right - m_DockBar_Left_Length - m_DockBar_X) / (g_rcScr.right - m_DockBar_Left_Length - m_DockBar_Start_x);
        }
        else
        {
            end = m_DockBar_Start_x;
            motionType = OutCirc;
            duration = DOCKBAR_ANIMATION_TIME * (m_DockBar_X - m_DockBar_Start_x) / (g_rcScr.right -  m_DockBar_Left_Length- m_DockBar_Start_x);
        }

        if(duration == 0)
            duration = DOCKBAR_ANIMATION_TIME;

        mGEffAnimationSetStartValue(m_animation, &start);
        mGEffAnimationSetEndValue(m_animation, &end);
        mGEffAnimationSetDuration(m_animation, duration);
        mGEffAnimationSetCurve(m_animation, motionType);
        mGEffAnimationSetFinishedCb(m_animation, animated_end);
        mGEffAnimationAsyncRun(m_animation);
    }
}

static HDC create_memdc_from_image_surface (cairo_surface_t* image_surface)
{
    MYBITMAP my_bmp = {
        flags: MYBMP_TYPE_RGB | MYBMP_FLOW_DOWN,
        frames: 1,
        depth: 32,
    };

    my_bmp.w = cairo_image_surface_get_width (image_surface);
    my_bmp.h = cairo_image_surface_get_height (image_surface);
    my_bmp.pitch = cairo_image_surface_get_stride (image_surface);
    my_bmp.bits = cairo_image_surface_get_data (image_surface);
    my_bmp.size = my_bmp.pitch * my_bmp.h;

    return CreateMemDCFromMyBitmap(&my_bmp, NULL);
}

static void paintSVGArrow(HDC hdc)
{
    GError *error = NULL;
    RsvgDimensionData dimensions;
    double factor_width = 0.0f;
    double factor_height = 0.0f;

    // read file from svg file
    if(m_arrow_svg_handle == NULL)
    {
        surface[0] = NULL;
        cr[0] = NULL;
        return;
    }
    rsvg_handle_get_dimensions(m_arrow_svg_handle, &dimensions);

    // create cairo_surface_t and cairo_t for one picture
    surface[0] = cairo_image_surface_create (CAIRO_FORMAT_RGB24, (int)(DOCK_ICON_WIDTH * m_factor), (int)(DOCK_ICON_HEIGHT * m_factor));
    cr[0] = cairo_create (surface[0]);

    cairo_save(cr[0]);

    cairo_translate(cr[0], (int)(DOCK_ICON_WIDTH * m_factor / 2), (int)(DOCK_ICON_HEIGHT * m_factor / 2));
    cairo_rotate(cr[0], m_Arrow_angle);
    cairo_translate(cr[0], (int)(-1 * DOCK_ICON_WIDTH * m_factor / 2), (int)(-1 * DOCK_ICON_HEIGHT * m_factor / 2));

    factor_width = (double)DOCK_ICON_WIDTH / (double)dimensions.width;
    factor_height = (double)DOCK_ICON_HEIGHT / (double)dimensions.height;
    factor_width = (factor_width > factor_height) ? factor_width : factor_height;
    cairo_scale(cr[0], factor_width, factor_width);

    float r = SysPixelColor[IDX_COLOR_darkgray].r / 255.0;
    float g = SysPixelColor[IDX_COLOR_darkgray].g / 255.0;
    float b = SysPixelColor[IDX_COLOR_darkgray].b / 255.0;
    float alpha = 0xE0 / 255.0;
    cairo_set_source_rgb (cr[0],  r*alpha, g*alpha, b*alpha);
    cairo_paint (cr[0]);
    rsvg_handle_render_cairo_style (m_arrow_svg_handle, cr[0], &button_color_pair[0], 1);

    HDC csdc = create_memdc_from_image_surface(surface[0]);
    if (csdc != HDC_SCREEN && csdc != HDC_INVALID)
    {
        SetMemDCColorKey(csdc, MEMDC_FLAG_SRCCOLORKEY,
            MakeRGB(SysPixelColor[IDX_COLOR_darkgray].r * alpha,
            SysPixelColor[IDX_COLOR_darkgray].g * alpha,
            SysPixelColor[IDX_COLOR_darkgray].b * alpha));
        BitBlt(csdc, 0, 0, DOCK_ICON_WIDTH, DOCK_ICON_HEIGHT, hdc, MARGIN_DOCK + (m_DockBar_Height - DOCK_ICON_WIDTH) * m_factor / 2, (int)((m_DockBar_Height - DOCK_ICON_HEIGHT) * m_factor / 2), 0);
        DeleteMemDC(csdc);
    }

    cairo_surface_destroy (surface[0]);
    cairo_destroy (cr[0]);
}

static void loadSVGArrow(const char* file, int index)
{
    GError *error = NULL;
    RsvgDimensionData dimensions;

    // read file from svg file
    m_arrow_svg_handle = rsvg_handle_new_from_file(file, &error);
    if(error)
    {
        surface[0] = NULL;
        cr[0] = NULL;
        m_arrow_svg_handle = NULL;
        return;
    }
    return;
}

static void loadSVGFromFile(const char* file, int index)
{
    RsvgHandle *handle;
    GError *error = NULL;
    RsvgDimensionData dimensions;
    double factor_width = 0.0f;
    double factor_height = 0.0f;

    // read file from svg file
    handle = rsvg_handle_new_from_file(file, &error);
    if(error)
    {
        surface[index] = NULL;
        cr[index] = NULL;
        return;
    }
    rsvg_handle_get_dimensions(handle, &dimensions);

    // create cairo_surface_t and cairo_t for one picture
    surface[index] = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, DOCK_ICON_WIDTH * m_factor, DOCK_ICON_HEIGHT * m_factor);
    cr[index] = cairo_create(surface[index]);

    cairo_save(cr[index]);

    factor_width = (double)DOCK_ICON_WIDTH / (double)dimensions.width;
    factor_height = (double)DOCK_ICON_HEIGHT / (double)dimensions.height;
    factor_width = (factor_width > factor_height) ? factor_width : factor_height;

    cairo_scale(cr[index], factor_width, factor_width);

    float r = SysPixelColor[IDX_COLOR_darkgray].r / 255.0;
    float g = SysPixelColor[IDX_COLOR_darkgray].g / 255.0;
    float b = SysPixelColor[IDX_COLOR_darkgray].b / 255.0;
    float alpha = 0xE0 / 255.0;
    cairo_set_source_rgb (cr[index],  r*alpha, g*alpha, b*alpha);
    cairo_paint (cr[index]);
//    if(index == ID_SHUTDOWN_BUTTON)
//        rsvg_handle_render_cairo_style (handle, cr[index], power_button_color_pair, 1);
//    else
        rsvg_handle_render_cairo_style (handle, cr[index], &button_color_pair[index], 1);
    cairo_restore (cr[index]);

    g_object_unref (handle);
}

static void paintDockBarIcon(HDC hdc)
{
    int i = 0;

    paintSVGArrow(hdc);
    float r = SysPixelColor[IDX_COLOR_darkgray].r / 255.0;
    float g = SysPixelColor[IDX_COLOR_darkgray].g / 255.0;
    float b = SysPixelColor[IDX_COLOR_darkgray].b / 255.0;
    float alpha = 0xE0 / 255.0;
    for(i = 1; i < BUTTON_COUNT; i ++)
    {
        if(surface[i] && cr[i])
        {
            HDC csdc = create_memdc_from_image_surface(surface[i]);
            if (csdc != HDC_SCREEN && csdc != HDC_INVALID)
            {
                SetMemDCColorKey(csdc, MEMDC_FLAG_SRCCOLORKEY,
                        MakeRGB(SysPixelColor[IDX_COLOR_darkgray].r * alpha,
                            SysPixelColor[IDX_COLOR_darkgray].g * alpha,
                            SysPixelColor[IDX_COLOR_darkgray].b * alpha));
                BitBlt(csdc, 0, 0, DOCK_ICON_WIDTH, DOCK_ICON_HEIGHT, hdc, i * m_Button_Interval + MARGIN_DOCK + (m_DockBar_Height - DOCK_ICON_WIDTH) * m_factor / 2, (int)((m_DockBar_Height - DOCK_ICON_HEIGHT) * m_factor / 2), 0);
            }
            DeleteMemDC(csdc);
        }
    }
    SyncUpdateDC(hdc);
}

static void toggle_application(HWND hWnd)
{
    REQUEST request;
    RequestInfo requestinfo;
    ReplyInfo replyInfo;

    requestinfo.id = REQ_SUBMIT_TOGGLE;
    requestinfo.hWnd = hWnd;
    requestinfo.iData0 = 0;
    request.id = FIXED_FORMAT_REQID;
    request.data = (void *)&requestinfo;
    request.len_data = sizeof(requestinfo);

    memset(&replyInfo, 0, sizeof(ReplyInfo));
    ClientRequest(&request, &replyInfo, sizeof(ReplyInfo));
    if((replyInfo.id == REQ_SUBMIT_TOGGLE) && (replyInfo.iData0))
    {
    }
    else
    {
    }
}

static void show_page(HWND hWnd, int page_req)
{
    REQUEST request;
    RequestInfo requestinfo;
    ReplyInfo replyInfo;

    requestinfo.id = page_req;
    requestinfo.hWnd = hWnd;
    requestinfo.iData0 = 0;

    request.id = FIXED_FORMAT_REQID;
    request.data = (void *)&requestinfo;
    request.len_data = sizeof(requestinfo);

    memset(&replyInfo, 0, sizeof(ReplyInfo));
    ClientRequest(&request, &replyInfo, sizeof(ReplyInfo));
    if(replyInfo.id == page_req && (replyInfo.iData0))
    {
    }
    else
    {
    }
}

static LRESULT DockBarWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    int x = 0;
    int y = 0;
    char picture_file[ETC_MAXLINE]; 
    char config_path[MAX_PATH + 1];
    char* etc_value = NULL;
    HDC hdc;
    BOOL ret = FALSE;

    switch (message)
    {
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            paintDockBarIcon(hdc);
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CREATE:
            if ((etc_value = getenv ("HISHELL_CFG_PATH")))
            {
                int len = strlen(etc_value);
                if (etc_value[len-1] == '/')
                {
                    sprintf(config_path, "%s%s", etc_value, SYSTEM_CONFIG_FILE);
                }
                else
                {
                    sprintf(config_path, "%s/%s", etc_value, SYSTEM_CONFIG_FILE);
                }
            }
            else
            {
                sprintf(config_path, "%s", SYSTEM_CONFIG_FILE);
            }

            // arrow
            memset(picture_file, 0, ETC_MAXLINE);
            if(GetValueFromEtcFile(config_path, "dock_arrow", "icon_file", picture_file, ETC_MAXLINE) == ETC_OK)
            {
                memset(button_color[ID_DISPLAY_BUTTON], 0, 24);
                if(GetValueFromEtcFile(config_path, "dock_arrow", "icon_color", button_color[ID_DISPLAY_BUTTON], 24) == ETC_OK)
                {
                    button_color_pair[ID_DISPLAY_BUTTON].name = "color";
                    button_color_pair[ID_DISPLAY_BUTTON].value = button_color[ID_DISPLAY_BUTTON];
                    button_color_pair[ID_DISPLAY_BUTTON].important = 0;
                    loadSVGArrow(picture_file, ID_DISPLAY_BUTTON);
                }
            }
            
            // home
            memset(picture_file, 0, ETC_MAXLINE);
            if(GetValueFromEtcFile(config_path, "dock_home", "icon_file", picture_file, ETC_MAXLINE) == ETC_OK)
            {
                memset(button_color[ID_HOME_BUTTON], 0, 24);
                if(GetValueFromEtcFile(config_path, "dock_home", "icon_color", button_color[ID_HOME_BUTTON], 24) == ETC_OK)
                {
                    button_color_pair[ID_HOME_BUTTON].name = "color";
                    button_color_pair[ID_HOME_BUTTON].value = button_color[ID_HOME_BUTTON];
                    button_color_pair[ID_HOME_BUTTON].important = 0;
                    loadSVGFromFile(picture_file, ID_HOME_BUTTON);
                }
            }

            // toggle
            memset(picture_file, 0, ETC_MAXLINE);
            if(GetValueFromEtcFile(config_path, "dock_toggle", "icon_file", picture_file, ETC_MAXLINE) == ETC_OK)
            {
                memset(button_color[ID_TOGGLE_BUTTON], 0, 24);
                if(GetValueFromEtcFile(config_path, "dock_toggle", "icon_color", button_color[ID_TOGGLE_BUTTON], 24) == ETC_OK)
                {
                    button_color_pair[ID_TOGGLE_BUTTON].name = "color";
                    button_color_pair[ID_TOGGLE_BUTTON].value = button_color[ID_TOGGLE_BUTTON];
                    button_color_pair[ID_TOGGLE_BUTTON].important = 0;
                    loadSVGFromFile(picture_file, ID_TOGGLE_BUTTON);
                }
            }

            // setting
            memset(picture_file, 0, ETC_MAXLINE);
            if(GetValueFromEtcFile(config_path, "dock_setting", "icon_file", picture_file, ETC_MAXLINE) == ETC_OK)
            {
                memset(button_color[ID_SETTING_BUTTON], 0, 24);
                if(GetValueFromEtcFile(config_path, "dock_setting", "icon_color", button_color[ID_SETTING_BUTTON], 24) == ETC_OK)
                {
                    button_color_pair[ID_SETTING_BUTTON].name = "color";
                    button_color_pair[ID_SETTING_BUTTON].value = button_color[ID_SETTING_BUTTON];
                    button_color_pair[ID_SETTING_BUTTON].important = 0;
                    loadSVGFromFile(picture_file, ID_SETTING_BUTTON);
                }
            }

            // about
            memset(picture_file, 0, ETC_MAXLINE);
            if(GetValueFromEtcFile(config_path, "dock_about", "icon_file", picture_file, ETC_MAXLINE) == ETC_OK)
            {
                memset(button_color[ID_ABOUT_BUTTON], 0, 24);
                if(GetValueFromEtcFile(config_path, "dock_about", "icon_color", button_color[ID_ABOUT_BUTTON], 24) == ETC_OK)
                {
                    button_color_pair[ID_ABOUT_BUTTON].name = "color";
                    button_color_pair[ID_ABOUT_BUTTON].value = button_color[ID_ABOUT_BUTTON];
                    button_color_pair[ID_ABOUT_BUTTON].important = 0;
                    loadSVGFromFile(picture_file, ID_ABOUT_BUTTON);
                }
            }
    
            // shutdown
            memset(picture_file, 0, ETC_MAXLINE);
            if(GetValueFromEtcFile(config_path, "dock_shutdown", "icon_file", picture_file, ETC_MAXLINE) == ETC_OK)
            {
                memset(button_color[ID_SHUTDOWN_BUTTON], 0, 24);
                if(GetValueFromEtcFile(config_path, "dock_shutdown", "icon_color", button_color[ID_SHUTDOWN_BUTTON], 24) == ETC_OK)
                {
                    button_color_pair[ID_SHUTDOWN_BUTTON].name = "color";
                    button_color_pair[ID_SHUTDOWN_BUTTON].value = button_color[ID_SHUTDOWN_BUTTON];
                    button_color_pair[ID_SHUTDOWN_BUTTON].important = 0;
                    loadSVGFromFile(picture_file, ID_SHUTDOWN_BUTTON);
                }
            }

            SetTimer(hWnd, ID_SHOW_TIMER, DOCKBAR_VISIBLE_TIME);
            m_direction = DIRECTION_HIDE;
            m_DockBar_X = m_DockBar_Start_x;
            m_Arrow_angle = 0;
            break;

        case MSG_LBUTTONUP:
            x = LOSWORD (lParam);
            y = HISWORD (lParam);
            for(i = 0; i < BUTTON_COUNT; i++)
            {
                if(PtInRect(m_rect + i, x, y))
                    break;
            }
            if(i < BUTTON_COUNT)
            {
                switch(i)
                {
                    case ID_DISPLAY_BUTTON:
                        if(m_direction == DIRECTION_HIDE)
                            m_direction = DIRECTION_SHOW;
                        else
                            m_direction = DIRECTION_HIDE;
                        create_animation(hWnd);
                        break;
                    case ID_HOME_BUTTON:
                        show_page(hWnd, REQ_SHOW_HOME_PAGE);
                        break;
                    case ID_TOGGLE_BUTTON:
                        toggle_application(hWnd);
                        break;
                    case ID_SETTING_BUTTON:
                        show_page(hWnd, REQ_SHOW_SETTING_PAGE);
                        break;
                    case ID_SHUTDOWN_BUTTON:
                        break;
                    case ID_ABOUT_BUTTON:
                        show_page(hWnd, REQ_SHOW_ABOUT_PAGE);
//                        SendMessage(HWND_DESKTOP, MSG_BROWSER_SHOW, 10, 0);
//                        exec_app("appagent");
                        break;
                }
            }

            break;

        case MSG_COMMAND:
#if 0
            code = HIWORD (wParam);
            id   = LOWORD (wParam);
            switch(id) 
            {
                case ID_DISPLAY_BUTTON:
                    break;
                case ID_HOME_BUTTON:
                    break;
                case ID_TOGGLE_BUTTON:
                    break;
                case ID_SETTING_BUTTON:
                    break;
                case ID_SHUTDOWN_BUTTON:
                    break;
                case ID_ABOUT_BUTTON:
                    break;
            }
#endif
            break;

        case MSG_TIMER:
            if(wParam == ID_SHOW_TIMER)
            {
                m_direction = DIRECTION_HIDE;
                create_animation(hWnd);
                KillTimer(hWnd, ID_SHOW_TIMER);
            }
            break;
        
        case MSG_CLOSE:
            for(i = 1; i < BUTTON_COUNT; i++)
            {
                cairo_surface_destroy(surface[i]);
                cairo_destroy(cr[i]);
            }
            if(m_arrow_svg_handle)
                g_object_unref(m_arrow_svg_handle);

            KillTimer (hWnd, ID_SHOW_TIMER);
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

HWND create_dock_bar (void)
{
    MAINWINCREATE CreateInfo;
    HWND hDockBar;
    int i = 0;

    m_factor = (float)GetGDCapability(HDC_SCREEN, GDCAP_DPI) / 96.0;
    m_DockBar_Height = HEIGHT_DOCKBAR * m_factor;

    CreateInfo.dwStyle = WS_ABSSCRPOS | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_WINTYPE_DOCKER | WS_EX_TROUNDCNS | WS_EX_BROUNDCNS;
    CreateInfo.spCaption = "DockBar" ;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DockBarWinProc;
    CreateInfo.lx = g_rcScr.right * (1 - 0.618);
    CreateInfo.ty = g_rcScr.bottom - m_DockBar_Height;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;

    m_DockBar_Start_x = CreateInfo.lx;
    m_DockBar_Start_y = CreateInfo.ty;
    m_DockBar_End_x = CreateInfo.rx;
    m_DockBar_End_y = CreateInfo.by;
    m_DockBar_Left_Length = 2 * MARGIN_DOCK + m_DockBar_Height * m_factor;
    m_Button_Interval = (m_DockBar_End_x - m_DockBar_Start_x) / BUTTON_COUNT;

    CreateInfo.iBkColor = RGBA2Pixel(HDC_SCREEN, 0xFF, 0xFF, 0xFF, 0xFF);
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hDockBar = CreateMainWindowEx2 (&CreateInfo, 0L, NULL, NULL, ST_PIXEL_ARGB8888,
                                MakeRGBA (SysPixelColor[IDX_COLOR_darkgray].r,
                                          SysPixelColor[IDX_COLOR_darkgray].g,
                                          SysPixelColor[IDX_COLOR_darkgray].b,
                                          0xE0),
                                CT_ALPHAPIXEL, 0xFF);

    for(i = 0; i < BUTTON_COUNT; i++)
    {
        m_rect[i].left = i * m_Button_Interval + MARGIN_DOCK + (m_DockBar_Height - DOCK_ICON_WIDTH) * m_factor / 2;
        m_rect[i].top = (m_DockBar_Height - DOCK_ICON_HEIGHT) * m_factor / 2;
        m_rect[i].right = m_rect[i].left + DOCK_ICON_WIDTH * m_factor;
        m_rect[i].bottom = m_rect[i].top + DOCK_ICON_HEIGHT * m_factor;
    }

    return hDockBar;
}

