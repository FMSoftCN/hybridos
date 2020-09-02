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
/*
** $Id: custom_minigui.c 728 2009-03-24 08:51:07Z weiym $
**
** custom_minigui.c: The reference implementation file for custom input
**         engine, and configuration file for in-core MiniGUI.
** 
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#ifdef _MGIAL_CUSTOM

#include <minigui/customial.h>

#ifdef __ECOS__
/* for eCos Linux Synthetic target */
#include <cyg/hal/hal_io.h>
#else
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef __ECOS__

#define O_RDONLY CYG_HAL_SYS_O_RDONLY

#undef FD_SET
#define FD_SET CYG_HAL_SYS_FD_SET
#undef FD_ZERO
#define FD_ZERO CYG_HAL_SYS_FD_ZERO
#undef FD_CLR
#define FD_CLR CYG_HAL_SYS_FD_CLR
#undef FD_ISSET
#define FD_ISSET CYG_HAL_SYS_FD_ISSET

#define fd_set struct cyg_hal_sys_fd_set
#define read cyg_hal_sys_read
#define open(name, flags) cyg_hal_sys_open (name, flags, 0)
#define close cyg_hal_sys_close
#define select cyg_hal_sys__newselect
#define timeval cyg_hal_sys_timeval
#define tv_sec hal_tv_sec
#define tv_usec hal_tv_usec
#else
#include <fcntl.h>
#endif

#define __NONEED_FD_SET_TYPE

#define QT_VFB_MOUSE_PIPE       "/tmp/.qtvfb_mouse-%d"
#define QT_VFB_KEYBOARD_PIPE    "/tmp/.qtvfb_keyboard-%d"

struct QVFbKeyData
{
    unsigned int unicode;
    unsigned int modifiers;
    BYTE press;
    BYTE repeat;
};

#define NOBUTTON        0x0000
#define LEFTBUTTON      0x0001
#define RIGHTBUTTON     0x0002
#define MIDBUTTON       0x0004
#define MOUSEBUTTONMASK 0x00FF

#define SHIFTBUTTON     0x0100
#define CONTROLBUTTON   0x0200
#define ALTBUTTON       0x0400
#define METABUTTON      0x0800
#define KEYBUTTONMASK   0x0FFF
#define KEYPAD          0x4000

static int mouse_fd = -1;
static int kbd_fd = -1;
static POINT mouse_pt;
static int mouse_buttons;
static struct QVFbKeyData kbd_data;
static unsigned char kbd_state [NR_KEYS];
static unsigned char keycode_scancode [256];

static void init_code_map (void)
{
    keycode_scancode [0x00] = SCANCODE_ESCAPE;

    keycode_scancode [0x29] = SCANCODE_0;
    keycode_scancode [0x21] = SCANCODE_1;
    keycode_scancode [0x40] = SCANCODE_2;
    keycode_scancode [0x23] = SCANCODE_3;
    keycode_scancode [0x24] = SCANCODE_4;
    keycode_scancode [0x25] = SCANCODE_5;
    keycode_scancode [0x5E] = SCANCODE_6;
    keycode_scancode [0x26] = SCANCODE_7;
    keycode_scancode [0x2A] = SCANCODE_8;
    keycode_scancode [0x28] = SCANCODE_9;

    keycode_scancode [0x30] = SCANCODE_0;
    keycode_scancode [0x31] = SCANCODE_1;
    keycode_scancode [0x32] = SCANCODE_2;
    keycode_scancode [0x33] = SCANCODE_3;
    keycode_scancode [0x34] = SCANCODE_4;
    keycode_scancode [0x35] = SCANCODE_5;
    keycode_scancode [0x36] = SCANCODE_6;
    keycode_scancode [0x37] = SCANCODE_7;
    keycode_scancode [0x38] = SCANCODE_8;
    keycode_scancode [0x39] = SCANCODE_9;

//    keycode_scancode [0x30] = SCANCODE_F1;
//    keycode_scancode [0x31] = SCANCODE_F2;
//    keycode_scancode [0x32] = SCANCODE_F3;
//    keycode_scancode [0x33] = SCANCODE_F4;
//    keycode_scancode [0x34] = SCANCODE_F5;
//    keycode_scancode [0x35] = SCANCODE_F6;
//    keycode_scancode [0x36] = SCANCODE_F7;
//    keycode_scancode [0x37] = SCANCODE_F8;
//    keycode_scancode [0x38] = SCANCODE_F9;
//    keycode_scancode [0x39] = SCANCODE_F10;

    keycode_scancode [0x2D] = SCANCODE_MINUS;
    keycode_scancode [0x5F] = SCANCODE_MINUS;
    keycode_scancode [0x3D] = SCANCODE_EQUAL;
    keycode_scancode [0x2B] = SCANCODE_EQUAL;
    keycode_scancode [0x03] = SCANCODE_BACKSPACE;
    keycode_scancode [0x01] = SCANCODE_TAB;
    keycode_scancode [0x51] = SCANCODE_Q;
    keycode_scancode [0x57] = SCANCODE_W;
    keycode_scancode [0x45] = SCANCODE_E;
    keycode_scancode [0x52] = SCANCODE_R;
    keycode_scancode [0x54] = SCANCODE_T;
    keycode_scancode [0x59] = SCANCODE_Y;
    keycode_scancode [0x55] = SCANCODE_U;
    keycode_scancode [0x49] = SCANCODE_I;
    keycode_scancode [0x4F] = SCANCODE_O;
    keycode_scancode [0x50] = SCANCODE_P;
    keycode_scancode [0x5B] = SCANCODE_BRACKET_LEFT;
    keycode_scancode [0x7B] = SCANCODE_BRACKET_LEFT;
    keycode_scancode [0x5D] = SCANCODE_BRACKET_RIGHT;
    keycode_scancode [0x7D] = SCANCODE_BRACKET_RIGHT;
    keycode_scancode [0x04] = SCANCODE_ENTER;
    keycode_scancode [0x41] = SCANCODE_A;
    keycode_scancode [0x53] = SCANCODE_S;
    keycode_scancode [0x44] = SCANCODE_D;
    keycode_scancode [0x46] = SCANCODE_F;
    keycode_scancode [0x47] = SCANCODE_G;
    keycode_scancode [0x48] = SCANCODE_H;
    keycode_scancode [0x4A] = SCANCODE_J;
    keycode_scancode [0x4B] = SCANCODE_K;
    keycode_scancode [0x4C] = SCANCODE_L;

    keycode_scancode [0x3A] = SCANCODE_SEMICOLON;
    keycode_scancode [0x3B] = SCANCODE_SEMICOLON;
//    keycode_scancode [0x3B] = SCANCODE_F12;


    keycode_scancode [0x27] = SCANCODE_APOSTROPHE;
    keycode_scancode [0x22] = SCANCODE_APOSTROPHE;
    keycode_scancode [0x60] = SCANCODE_GRAVE;
    keycode_scancode [0x7E] = SCANCODE_GRAVE;

    keycode_scancode [0x20] = SCANCODE_SPACE;
//    keycode_scancode [0x20] = SCANCODE_LEFTSHIFT;

    keycode_scancode [0x5C] = SCANCODE_BACKSLASH;
    keycode_scancode [0x7C] = SCANCODE_BACKSLASH;
    keycode_scancode [0x5A] = SCANCODE_Z;
    keycode_scancode [0x58] = SCANCODE_X;
    keycode_scancode [0x43] = SCANCODE_C;
    keycode_scancode [0x56] = SCANCODE_V;
    keycode_scancode [0x42] = SCANCODE_B;
    keycode_scancode [0x4E] = SCANCODE_N;
    keycode_scancode [0x4D] = SCANCODE_M;
    keycode_scancode [0x2C] = SCANCODE_COMMA;
    keycode_scancode [0x3C] = SCANCODE_COMMA;
    keycode_scancode [0x2E] = SCANCODE_PERIOD;
    keycode_scancode [0x3E] = SCANCODE_PERIOD;
    keycode_scancode [0x2F] = SCANCODE_SLASH;
    keycode_scancode [0x3F] = SCANCODE_SLASH;
//    keycode_scancode [0x2A] = SCANCODE_KEYPADMULTIPLY;

    keycode_scancode [0x09] = SCANCODE_PRINTSCREEN;
    keycode_scancode [0x08] = SCANCODE_BREAK;
    keycode_scancode [0x06] = SCANCODE_INSERT;
    keycode_scancode [0x07] = SCANCODE_REMOVE;
    keycode_scancode [0x10] = SCANCODE_HOME;
    keycode_scancode [0x11] = SCANCODE_END;
    keycode_scancode [0x16] = SCANCODE_PAGEUP;
    keycode_scancode [0x17] = SCANCODE_PAGEDOWN;
    keycode_scancode [0x13] = SCANCODE_CURSORBLOCKUP;
    keycode_scancode [0x12] = SCANCODE_CURSORBLOCKLEFT;
    keycode_scancode [0x14] = SCANCODE_CURSORBLOCKRIGHT;
    keycode_scancode [0x15] = SCANCODE_CURSORBLOCKDOWN;

//    keycode_scancode [0x23] = SCANCODE_RIGHTALT;
//    keycode_scancode [0x2F] = SCANCODE_KEYPADDIVIDE;
//    keycode_scancode [0x20] = SCANCODE_RIGHTSHIFT;
//    keycode_scancode [0x21] = SCANCODE_RIGHTCONTROL;
//    keycode_scancode [0x37] = SCANCODE_KEYPAD7;
//    keycode_scancode [0x38] = SCANCODE_KEYPAD8;
//    keycode_scancode [0x39] = SCANCODE_KEYPAD9;
//    keycode_scancode [0x2D] = SCANCODE_KEYPADMINUS;
//    keycode_scancode [0x34] = SCANCODE_KEYPAD4;
//    keycode_scancode [0x35] = SCANCODE_KEYPAD5;
//    keycode_scancode [0x36] = SCANCODE_KEYPAD6;
//    keycode_scancode [0x2B] = SCANCODE_KEYPADPLUS;
//    keycode_scancode [0x31] = SCANCODE_KEYPAD1;
//    keycode_scancode [0x32] = SCANCODE_KEYPAD2;
//    keycode_scancode [0x33] = SCANCODE_KEYPAD3;
//    keycode_scancode [0x30] = SCANCODE_KEYPAD0;
//    keycode_scancode [0x2E] = SCANCODE_KEYPADPERIOD;
//    keycode_scancode [] = SCANCODE_LESS;
//    keycode_scancode [] = SCANCODE_KEYPADENTER;
//    keycode_scancode [] = SCANCODE_PAUSE;
//    keycode_scancode [] = SCANCODE_LEFTWIN;
//    keycode_scancode [] = SCANCODE_RIGHTWIN;
//    keycode_scancode [] = SCANCODE_MENU;
}

static unsigned char keycode_to_scancode (unsigned char keycode, BOOL asscii)
{
    if(!asscii)
    {
        switch (keycode) {
            case 0x30 ... 0x39:
                return SCANCODE_F1 + keycode - 0x30;
            case 0x3B:
                return SCANCODE_F12;
            case 0x20:
                return SCANCODE_LEFTSHIFT;
            case 0x21:
                return SCANCODE_LEFTCONTROL;
            case 0x23:
                return SCANCODE_LEFTALT;
            case 0x24:
                return SCANCODE_CAPSLOCK;
            case 0x25:
                return SCANCODE_NUMLOCK;
            case 0x26:
                return SCANCODE_SCROLLLOCK;
            case 0x3A:
                return SCANCODE_F11;
        }
    }

    return keycode_scancode [keycode];
}

/************************  Low Level Input Operations **********************/
/*
 * Mouse operations -- Event
 */
static int mouse_update (void)
{
    int ret1, ret2;
    POINT l_mouse_pt;
    int l_mouse_buttons;

    ret1 = read (mouse_fd, &l_mouse_pt, sizeof (POINT));
    ret2 = read (mouse_fd, &l_mouse_buttons, sizeof (int));

    if (ret1 == sizeof (POINT) && ret2 && sizeof (int)
            && l_mouse_buttons < 0x08) {
        mouse_pt = l_mouse_pt;
        mouse_buttons = l_mouse_buttons;
    }
    else
        return 0;

    return 1;
}

static void mouse_getxy (int *x, int* y)
{
    *x = mouse_pt.x;
    *y = mouse_pt.y;
}

static int mouse_getbutton (void)
{
    int buttons = 0;

    if (mouse_buttons & LEFTBUTTON)
        buttons |= IAL_MOUSE_LEFTBUTTON;
    if (mouse_buttons & RIGHTBUTTON)
        buttons |= IAL_MOUSE_RIGHTBUTTON;
    if (mouse_buttons & MIDBUTTON)
        buttons |= IAL_MOUSE_MIDDLEBUTTON;

    return buttons;
}

static unsigned char nr_changed_keys = 0;

static int keyboard_update (void)
{
    return nr_changed_keys;
}

static int read_key (void)
{
    static unsigned char last;
    struct QVFbKeyData l_kbd_data;
    int ret;
    unsigned char scancode;

    ret = read (kbd_fd, &l_kbd_data, sizeof (struct QVFbKeyData));

    if (ret == sizeof (struct QVFbKeyData)) {
        kbd_data = l_kbd_data;
    }
    else
        return 0;

    if (kbd_data.repeat) {
        return 0;
    }

    if (kbd_data.unicode == 0 && !kbd_data.press) {
        kbd_state [last] = 0;
    }
    else {
        scancode = keycode_to_scancode (HIWORD (kbd_data.unicode) & 0x00FF, 
                    LOWORD (kbd_data.unicode));
        kbd_state [scancode] = kbd_data.press ? 1 : 0;
        last = scancode;
    }

    nr_changed_keys = last + 1;
    return 1;
}

static const char* keyboard_getstate (void)
{
    return (char*)kbd_state;
}

/* NOTE by weiym: Do not ignore the fd_set in, out, and except */
static int wait_event (int which, int maxfd, fd_set *in, fd_set *out, fd_set *except,
                struct timeval *timeout)
{
    fd_set rfds;
    int    retvalue = 0;
    int    fd, e;

    if (!in) {
        in = &rfds;
        FD_ZERO (in);
    }

    if (which & IAL_MOUSEEVENT && mouse_fd >= 0) {
        fd = mouse_fd;
        FD_SET (fd, in);
#ifndef _MGRM_THREADS
        if (fd > maxfd) maxfd = fd;
#endif
    }

    if (which & IAL_KEYEVENT && kbd_fd >= 0) {
        fd = kbd_fd;
        FD_SET (kbd_fd, in);
#ifndef _MGRM_THREADS
        if (fd > maxfd) maxfd = fd;
#endif
    }

    /* FIXME: pass the real set size */
#ifndef _MGRM_THREADS
    e = select (maxfd + 1, in, out, except, timeout) ;
#else
    e = select (FD_SETSIZE, in, out, except, timeout) ;
#endif

    if (e > 0) { 
        fd = mouse_fd;
        /* If data is present on the mouse fd, service it: */
        if (fd >= 0 && FD_ISSET (fd, in)) {
            FD_CLR (fd, in);
            retvalue |= IAL_MOUSEEVENT;
        }

        fd = kbd_fd;
        /* If data is present on the keyboard fd, service it: */
        if (fd >= 0 && FD_ISSET (fd, in)) {
            FD_CLR (fd, in);
            if (read_key ())
                retvalue |= IAL_KEYEVENT;
            else {  /* play at a timeout event */
                if (timeout) {
                    timeout->tv_sec = 0;
                    timeout->tv_usec = 0;
                }
            }
        }

    } else if (e < 0) {
        return -1;
    }

    return retvalue;
}

BOOL InitCustomInput (INPUT* input, const char* mdev, const char* mtype)
{
    char file [50];
    int display;

#ifdef _MGINCORE_RES
    display = 0;
#else
    /* get display number */
    if (GetMgEtcIntValue ("qvfb", "display", &display) < 0)
        return FALSE;
#endif

    /* open mouse pipe */
    sprintf (file, QT_VFB_MOUSE_PIPE, display);
    if ((mouse_fd = open (file, O_RDONLY)) < 0) {
        fprintf (stderr, "QVFB IAL engine: can not open mouse pipe.\n");
        return FALSE;
    }
    /* open keyboard pipe */
    sprintf (file, QT_VFB_KEYBOARD_PIPE, display);
    if ((kbd_fd = open (file, O_RDONLY)) < 0) {
        fprintf (stderr, "QVFB IAL engine: can not open keyboard pipe.\n");
        return FALSE;
    }

    input->update_mouse = mouse_update;
    input->get_mouse_xy = mouse_getxy;
    input->set_mouse_xy = NULL;
    input->get_mouse_button = mouse_getbutton;
    input->set_mouse_range = NULL;
    input->suspend_mouse= NULL;
    input->resume_mouse = NULL;

    input->update_keyboard = keyboard_update;
    input->get_keyboard_state = keyboard_getstate;
    input->suspend_keyboard = NULL;
    input->resume_keyboard = NULL;
    input->set_leds = NULL;

    input->wait_event = wait_event;

    init_code_map ();

    return TRUE;
}

void TermCustomInput (void)
{
    if (mouse_fd >= 0)
        close (mouse_fd);
    if (kbd_fd >= 0)
        close (kbd_fd);
}

#endif /* _MGIAL_CUSTOM */

