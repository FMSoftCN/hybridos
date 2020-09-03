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
 * $Id: statusbar.h 368 2008-01-25 06:58:28Z zhounuohua $
 * 
 * The head file of task bar.
 */

#ifndef _STATUS_BAR
#define _STATUS_BAR

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

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _STATUS_BAR */
