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
 * \file mginit.c
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file is used to initialize MiniGUI environment, and starts other
    application process.
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
 * $Id: mginit.c 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/fixedmath.h>
#include <mgeff/mgeff.h>

#include "../include/sysconfig.h"

static SysConfig m_SysConfig;
extern void mine_compositor_toggle_state(BOOL toggle);
extern CompositorOps mine_compositor;

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
        else if(client->pid == m_SysConfig.iBrowserPid)
            m_SysConfig.iBrowserClientID = cli;
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

static void on_znode_operation(int op, int cli, int idx_znode)
{
    MSG msg;
    int clientId = 0;
    int idx_topmost = 0;

    idx_topmost = ServerGetTopmostZNodeOfType(NULL, ZOF_TYPE_NORMAL, &clientId);

    if((m_SysConfig.iTopmostClientID == 0) && (clientId == 0))
        return;

    m_SysConfig.iTopmostClientID = clientId;


    if((op == ZNOP_CHANGECAPTION) && (clientId != cli))
    {
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

        if (clientId == 0 && idx_topmost == 0)
        {
            msg.message = MSG_DYBKGND_DO_ANIMATION;
            msg.hwnd = HWND_NULL;
            Send2Client(&msg, m_SysConfig.iDyBKGndClientID);
        }
    }
}

static int fix_format_request(int cli, int clifd, void* buff, size_t len)
{
    RequestInfo * requestInfo = (RequestInfo *)buff;
    ReplyInfo replyInfo;

    if(m_SysConfig.iSystemConfigClientID == cli)
    {
        // from status bar: znode index and status bar hwnd
        if(requestInfo->id == REQ_SUBMIT_STATUSBAR_ZNODE)
        {
            m_SysConfig.iStatusBarZNode = (int)(requestInfo->iData0);
            m_SysConfig.hWndStatusBar = requestInfo->hWnd;
            replyInfo.id = REQ_SUBMIT_STATUSBAR_ZNODE;
            replyInfo.iData0 = (int)TRUE;
            ServerSendReply(clifd, &replyInfo, sizeof(replyInfo));
        }
        // from status bar: press toggle button
        else if(requestInfo->id == REQ_SUBMIT_TOGGLE)
        {
            int cur_clientId = 0; 
            int idx_topmost = 0; 

            // reply client
            replyInfo.id = REQ_SUBMIT_TOGGLE;
            replyInfo.iData0 = (int)TRUE;
            ServerSendReply(clifd, &replyInfo, sizeof(replyInfo));

            idx_topmost = ServerGetTopmostZNodeOfType(NULL, ZOF_TYPE_NORMAL, &cur_clientId); 
            if(idx_topmost > 0)
                mine_compositor_toggle_state(TRUE);
        }
        else if(requestInfo->id == REQ_SHOW_PAGE)
        {
            int cur_clientId = 0; 
            int idx_topmost = 0; 
            MSG msg;

            // reply client
            replyInfo.id = requestInfo->id;
            replyInfo.iData0 = (int)TRUE;
            ServerSendReply(clifd, &replyInfo, sizeof(replyInfo));

            msg.message = MSG_BROWSER_SHOW;
            msg.hwnd = HWND_NULL;
            msg.wParam = requestInfo->iData0;
            Send2Client(&msg, m_SysConfig.iBrowserClientID);
        }
    }
    else if(m_SysConfig.iBrowserClientID == cli)
    {
        // from browser: bring hwnd to topmost
        if(requestInfo->id == REQ_SUBMIT_TOPMOST)
        {
            int cur_clientId = 0;
            int idx_topmost = 0;
            int znode_index = 0;
            int znode_result = -1;
            ZNODEINFO znodeinfo;
            const ZNODEHEADER * znodeheader = NULL;

            znode_index = ServerGetNextZNode(NULL, 0, &cur_clientId);

            while(znode_index > 0)
            {
                if(ServerGetZNodeInfo(NULL, znode_index, &znodeinfo))
                {
                    if((znodeinfo.type & ZOF_TYPE_MASK) == ZOF_TYPE_NORMAL)     // it the main window in normal level
                    {
                        znodeheader = ServerGetWinZNodeHeader(NULL, znode_index, NULL, FALSE);
                        if(znodeheader->hwnd == requestInfo->hWnd)
                        {
                            znode_result = znode_index;
                            break;
                        }
                    }
                }
                znode_index = ServerGetNextZNode(NULL, znode_index, &cur_clientId);
            }

            if(znode_result != -1)
                ServerDoZNodeOperation(NULL, znode_result, ZNOP_MOVE2TOP, NULL, FALSE);
        }
        // reply client
        replyInfo.id = requestInfo->id;
        replyInfo.iData0 = (int)TRUE;
        ServerSendReply(clifd, &replyInfo, sizeof(replyInfo));
    }
    return 0;
}

static int unfixed_format_request(int cli, int clifd, void* buff, size_t len)
{
    if(m_SysConfig.iSystemConfigClientID == cli)
    {
        RequestInfo * requestInfo = (RequestInfo *)buff;
        // from status bar: get the titile of main window
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

    mGEffInit();

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

    // register a compositor
    if(!ServerRegisterCompositor(MIME_COMPOSITOR, &mine_compositor))
    {
        return 4;
    }

    // register request handler
    if(!RegisterRequestHandler(FIXED_FORMAT_REQID, fix_format_request))
    {
        return 2;
    }

    // register request for status bar, to get main window title
    if(!RegisterRequestHandler(UNFIXED_FORMAT_REQID, unfixed_format_request))
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

    // start browser process
    m_SysConfig.iBrowserPid = exec_app("appagent");
    if(m_SysConfig.iBrowserPid < 0)
    {
        return 3;
    }

    while (GetMessage(&msg, HWND_DESKTOP)) {
        DispatchMessage(&msg);
    }

    // unregister customer compositor
    ServerUnregisterCompositor(MIME_COMPOSITOR);

    mGEffDeinit();

    return 0;
}
