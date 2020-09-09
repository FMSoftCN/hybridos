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
 * $Id: config.h 368 2020-08-25 06:58:28Z gengyue $
 * 
 * The head file of configuration.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

// some macro of animation 
#define DIRECTION_SHOW      0               // show the main window
#define DIRECTION_HIDE      1               // hide the main window

#define STATUSBAR_ANIMATION_TIME    1000    // 1000 ms
#define STATUSBAR_VISIBLE_TIME      200     // 200ms * 10 = 2s

#define DOCKBAR_ANIMATION_TIME      1000    // 1000 ms
#define DOCKBAR_VISIBLE_TIME        200     // 200ms * 10 = 2s

// some macro of physical dimension 
#define _MARGIN             2               // margin of text area in status bar
#define HEIGHT_STATUSBAR    40              // height of status bar 
#define HEIGHT_DOCKBAR      60              // height of dock bar
#define TIME_INFO_X         100             // the width of time area in status bar

// control ID in dock bar
#define BUTTON_COUNT        6               // the number of button on the dock bar
#define ID_DISPLAY_BUTTON   100             // show and hide button
#define ID_HOME_BUTTON      101             // home button
#define ID_TOGGLE_BUTTON    102             // toggle apps
#define ID_SETTING_BUTTON   103             // system settings
#define ID_SHUTDOWN_BUTTON  104             // shutdown the device
#define ID_ABOUT_BUTTON     105             // about button, navigate to hybridos.com

// timer
#define ID_TIMER            100             // for time display
#define ID_SHOW_TIMER       101             // for display status and dock bar

// default string in status bar, when none app exists.
#define STRING_OS_NAME  "Hybrid OS V1.1.0"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

HWND create_status_bar (void);
HWND create_dock_bar (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _CONFIG_h */
