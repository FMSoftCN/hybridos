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
 * $Id: sysconfig.h 368 2008-01-25 06:58:28Z zhounuohua $
 * 
 * The head file of system config
 */

#ifndef _SYSTEM_CONFIG
#define _SYSTEM_CONFIG

// Customer Require Id
#define ZNODE_INFO_REQID            (MAX_SYS_REQID + 1)
#define GET_TITLE_REQID             (MAX_SYS_REQID + 2)

typedef struct tagSysConfig 
{
    int iSystemConfigClientID;  // clientID of system manager
    int iDyBKGndClientID;       // clientID of dynamic background
    pid_t iSystemConfigPid;     // pid of system manager
    pid_t iDyBKGndPid;          // pid of dynamic background
    int iStatusBarZNode;        // z node index of status bar
} SysConfig;


#define REQ_SUBMIT_STATUSBAR_ZNODE  0   // status bar send znode index to server

typedef struct tagRequestInfo 
{
    int id;                     // sub request ID
    unsigned int iData0;
    unsigned int request1;
    unsigned int request2;
    unsigned int request3;
} RequestInfo;

typedef struct tagReplyInfo 
{
    int id;                     // sub request ID
    unsigned int iData0;
    unsigned int request1;
    unsigned int request2;
    unsigned int request3;
} ReplyInfo;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SYSTEM_CONFIG */