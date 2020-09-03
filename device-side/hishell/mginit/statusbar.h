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
 * $Id: taskbar.h 368 2008-01-25 06:58:28Z zhounuohua $
 * 
 * The head file of task bar.
 */

#ifndef _TASK_BAR
#define _TASK_BAR

#define HEIGHT_TASKBAR  28
#define HEIGHT_IMEWIN   24

#define _ID_START_BUTTON    100
#define _ID_TIME_STATIC     110
#define _ID_APPS_COOLBAR    120
#define _ID_LAYER_BOX       200

#define _MARGIN             2
#define _HEIGHT_CTRL        24
#define _WIDTH_START        40
#define _WIDTH_TIME         52
#define _WIDTH_APPS         250
#define _ID_TIMER           100
#define _MAX_WIDTH_LAYER_BOX    80
#define _MIN_WIDTH_LAYER_BOX    20

#define _LEFT_BOXES     (_MARGIN + _WIDTH_START + _MARGIN + _WIDTH_APPS + _MARGIN)
#define _WIDTH_BOXES    (g_rcScr.right - _WIDTH_TIME - _MARGIN - _MARGIN - _LEFT_BOXES)

#define APP_INFO_FILE   "mginit.rc"

#define TIP_MAX         255

typedef struct tagAppItem {
    BOOL cdpath;
    char path [PATH_MAX + 1];
    char name [NAME_MAX + 1];
    char layer [LEN_LAYER_NAME + 1];
    char tip [TIP_MAX + 1];
    char bmp_path [PATH_MAX + NAME_MAX + 1];
    BITMAP bmp;
} APPITEM;

typedef struct tagAppInfo {
    int nr_apps;
    int autostart;
    char logo_path [PATH_MAX + NAME_MAX + 1];
    APPITEM* app_items;
} APPINFO;

extern APPINFO app_info;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

HWND create_status_bar (void);
pid_t exec_app (int app);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _TASK_BAR */
