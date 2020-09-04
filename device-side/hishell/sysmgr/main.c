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
** $Id: mginit.c 558 2008-03-04 09:02:19Z wangjian $
**
** The mginit program of MiniGUI Demostration Envirionment.
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
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/fixedmath.h>

#include "statusbar.h"
#include "dockbar.h"

static HWND hStatusBar = HWND_INVALID;
static HWND hDockBar = HWND_INVALID;

int MiniGUIMain (int args, const char* arg[])
{
    MSG msg;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "sysmgr" , 0 , 0);
#endif

    if ((hStatusBar = create_status_bar ()) == HWND_INVALID) 
    {
        fprintf (stderr, "Can not create Status bar.\n");
        return 2;
    }
    
    if ((hDockBar = create_dock_bar ()) == HWND_INVALID) 
    {
        fprintf (stderr, "Can not create Dock bar.\n");
        return 2;
    }

    while (GetMessage (&msg, hStatusBar) || GetMessage (&msg, hDockBar))
    {
        TranslateMessage(&msg);
        DispatchMessage (&msg);
    }

    return 0;
}
