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

#include "../include/sysconfig.h"

static SysConfig m_SysConfig;

static const char* new_del_client_info [] =
{
    NULL,
    "New comming in client: %s\n",
    "Disconnecting client: %s\n"
};

static void child_wait (int sig)
{
    int pid;
    int status;

    while ((pid = waitpid (-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED (status))
            printf ("--pid=%d--status=%x--rc=%d---\n", 
                            pid, status, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf ("--pid=%d--signal=%d--\n", pid, WTERMSIG (status));
    }
}

static void on_new_del_client(int op, int cli)
{
    static int nr_clients = 0;
    MG_Client* client = mgClients + cli;

    if (op > 0 && op <= LCO_DEL_CLIENT)
        printf (new_del_client_info [op], client?client->name:"NULL");

    if (op == LCO_NEW_CLIENT) 
    {
        if(client->pid == m_SysConfig.iSystemConfigPid)
            m_SysConfig.iSystemConfigClientID = cli;
        else if(client->pid == m_SysConfig.iDyBKGndPid)
            m_SysConfig.iDyBKGndClientID = cli;
        nr_clients ++;
    }
    else if (op == LCO_DEL_CLIENT) 
    {
        nr_clients --;
        if (nr_clients == 0) 
        {
            printf ("There is no any client.\n");
        }
        else if (nr_clients < 0) 
        {
            printf ("Serious error: nr_clients less than zero.\n");
        }
    }
    else
        printf ("Serious error: incorrect operations.\n");
}

static pid_t exec_app (char * app)
{
    pid_t pid = 0;
    char buff [PATH_MAX + NAME_MAX + 1];

    memset(buff, 0, PATH_MAX + NAME_MAX + 1);

    if ((pid = vfork ()) > 0) {
        fprintf (stderr, "new child, pid: %d.\n", pid);
    }
    else if (pid == 0) {
        strcpy (buff, ".//");
        strcat (buff, app);
        execl (buff, app, NULL);
        perror ("execl");
        _exit (1);
    }
    else {
        perror ("vfork");
    }
    return pid;
}

static void on_znode_operation(int op, int cli, int idx_znode)
{
    MSG msg;
    int clientId = 0;
    int idx_topmost = 0;

    idx_topmost = ServerGetTopmostZNodeOfType(NULL, ZOF_TYPE_NORMAL, &clientId);
    m_SysConfig.iTopmostClientID = clientId;

    if(idx_topmost <= 0)            // todo
        return;

    if((op == ZNOP_CHANGECAPTION) && (clientId != cli))
    {
        if (clientId == 0)
        {
            msg.message = MSG_DYBKGND_DO_ANIMATION;
            msg.hwnd = HWND_NULL;
            Send2Client(&msg, m_SysConfig.iDyBKGndClientID);
        }
        return;
    }

    if((op == ZNOP_ALLOCATE) || (op == ZNOP_FREE) || (op == ZNOP_MOVE2TOP) || (op == ZNOP_SETACTIVE) || (op == ZNOP_CHANGECAPTION))
    {
        msg.message = MSG_MAINWINDOW_CHANGE;
        msg.hwnd = m_SysConfig.hWndStatusBar;
        msg.wParam = (idx_topmost << 16) | clientId;

        if(clientId)    // to other main window
        {
            const ZNODEHEADER * znodeheader = ServerGetWinZNodeHeader(NULL, idx_topmost, NULL, FALSE);
            if(znodeheader)
            {
                if(znodeheader->caption)
                    msg.lParam = (int)strlen(znodeheader->caption);
                else
                    msg.lParam = 0;
            }
            else
                msg.lParam = 0;
        }
        Send2Client(&msg, m_SysConfig.iSystemConfigClientID);
    }
}

static int GetStatusBarZnode(int cli, int clifd, void* buff, size_t len)
{
    if(m_SysConfig.iSystemConfigClientID == cli)
    {
        RequestInfo * requestInfo = (RequestInfo *)buff;
        if(requestInfo->id == REQ_SUBMIT_STATUSBAR_ZNODE)
        {
            ReplyInfo replyInfo;
            m_SysConfig.iStatusBarZNode = (int)(requestInfo->iData0);
            m_SysConfig.hWndStatusBar = requestInfo->hWnd;
            replyInfo.id = REQ_SUBMIT_STATUSBAR_ZNODE;
            replyInfo.iData0 = (int)TRUE;
            ServerSendReply(clifd, &replyInfo, sizeof(replyInfo));
        }
    }
    return 0;
}

static int GetTopMostTitle(int cli, int clifd, void* buff, size_t len)
{
    if(m_SysConfig.iSystemConfigClientID == cli)
    {
        RequestInfo * requestInfo = (RequestInfo *)buff;
        if(requestInfo->id == REQ_GET_TOPMOST_TITLE)
        {
            int clientId = requestInfo->iData0 & 0xFFFF;
            int idx_znode = (requestInfo->iData0 & 0xFFFF0000) >> 16;
            int length = (int)requestInfo->iData1;
            char buff[length + 1];

            int cur_clientId = 0;
            int idx_topmost = 0;
            idx_topmost = ServerGetTopmostZNodeOfType(NULL, ZOF_TYPE_NORMAL, &cur_clientId);

            memset(buff, 0, length + 1);

            if(clientId == cur_clientId)            // the same process
            {
                const ZNODEHEADER * znodeheader = ServerGetWinZNodeHeader(NULL, idx_topmost, NULL, FALSE);
                if(znodeheader)
                {
                    if(znodeheader->caption)
                        memcpy(buff, znodeheader->caption, length);
                }
            }
            ServerSendReply(clifd, buff, length + 1);
        }
    }
    return 0;
}

int MiniGUIMain (int args, const char* arg[])
{
    struct sigaction siga;
    MSG msg;
    pid_t pid = 0;

    memset(&m_SysConfig, 0, sizeof(SysConfig));

    siga.sa_handler = child_wait;
    siga.sa_flags  = 0;
    memset (&siga.sa_mask, 0, sizeof(sigset_t));
    sigaction (SIGCHLD, &siga, NULL);

    OnNewDelClient = on_new_del_client;
    OnZNodeOperation = on_znode_operation;

    if (!ServerStartup (0, 0, 0)) 
    {
        fprintf (stderr, "Can not start the server of MiniGUI-Processes: mginit.\n");
        return 1;
    }

    // registe request handler
    if(!RegisterRequestHandler(ZNODE_INFO_REQID, GetStatusBarZnode))
    {
        return 2;
    }

    if(!RegisterRequestHandler(GET_TITLE_REQID, GetTopMostTitle))
    {
        return 2;
    }

    // start system config process
    m_SysConfig.iSystemConfigPid = exec_app("sysmgr");
    if(m_SysConfig.iSystemConfigPid < 0)
    {
        return 3;
    }

    // start dynamic background process
    m_SysConfig.iDyBKGndPid = exec_app("dybkgnd");
    if(m_SysConfig.iDyBKGndPid < 0)
    {
        return 3;
    }

    while (GetMessage(&msg, HWND_DESKTOP)) {
        DispatchMessage(&msg);
    }

    return 0;
}
