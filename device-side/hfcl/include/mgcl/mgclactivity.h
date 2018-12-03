/* 
** HFCL - HybridOS Foundation Class Library
** 
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __HFCL_MGCLACTIVITY_H__
#define __HFCL_MGCLACTIVITY_H__

namespace hfcl {

#define MGCL_WS_EX_MODALDISABLED     0x10000000L
#define MGCL_MSG_MNWND_ENDDIALOG   MSG_LASTUSERMSG + 4 


DWORD mgclDoModal(HWND hWnd, BOOL bAutoDestroy);
DWORD mgclDoModalView(HWND hWnd);
void mgclEndDialog(HWND hwnd, int endcode);
/**
 * @brief mgclCreateMainWindow Creates a main window.
 *
 * @param pCreateInfo The pointer to a MAINWINCREATE structure.
 *
 * @returns The handle to the new main window; HWND_INVALID indicates an error.
 */
HWND mgclCreateMainWindow(PMAINWINCREATE pCreateInfo);

/**
 * @brief mgclDestroyMainWindow Destroys a main window.
 *
 * @param hWnd The handle to the main window.
 *
 * @returns true on success, false on error.
 */
bool mgclDestroyMainWindow(HWND hWnd);

/**
 * @brief mgclDefaultMainWinProc Is the default main window callback procedure.
 *
 * @param hWnd The handle to the window.
 * @param msg The message identifier.
 * @param wParam The first parameter of the message.
 * @param lParam The second parameter of the message.
 *
 * @returns The return value of the message handler.
 */
int mgclDefaultMainWinProc(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam);

/**
 * @brief mgclShowWindow Shows or hides a window.
 *
 * @param hWnd The handle to the window.
 * @param iCmdShow The command to show or hide, can be one of 
 * the following values:
 *      - SW_SHOW\n
 *        Shows the window.
 *      - SW_HIDE\n
 *        Hides the window.
 *      - SW_SHOWNORMAL\n
 *        Shows the window, and if the window is a main window 
 *        sets it to be the top most main window.
 *
 * @returns true on sucess, otherwise false.
 */
bool mgclShowWindow(HWND hWnd, int iCmdShow);

/**
 * @brief mgclGetWindowAdditionalData Retrives the first additional data of a window.
 *
 * @param hWnd The handle to the window.
 *
 * @returns The first additional data of the window.
 */
DWORD mgclGetWindowAdditionalData(HWND hWnd);

/**
 * @brief mgclSetWindowAdditionalData Sets the first additional data of a window.
 *
 * @param hWnd The handle to the window.
 * @param newData The new first additional data of the window.
 *
 * @returns The old first additional data of the window.
 */
DWORD mgclSetWindowAdditionalData(HWND hWnd, DWORD newData);

/**
 * @brief mgclGetSystemCursor Gets the handle to a system cursor by its identifier.
 *
 * @param csrid The identifier of the system cursor.
 *
 * @returns Handle to the system cursor, otherwise zero.
 */
unsigned int mgclGetSystemCursor(int csrid);

/**
 * @brief mgclExitGUISafely Exits your MiniGUI application safely.
 *
 * @param exitcode The exit status will be passed to exit(3) function.
 */
void mgclExitGUISafely(int exitcode);

/**
 * @brief mgclMainWindowThreadCleanup Cleans up system resource associated
 *            with a main window.
 *
 * @param hMainWnd The handle to the main window.
 */
void mgclMainWindowThreadCleanup(HWND hMainWnd);

/**
 * @brief mgclGetMessage Gets a message from the message queue of a main window.
 *
 * @param pMsg Pointer to the result message.
 * @param hWnd Handle to the window.
 *
 * @returns false on MSG_QUIT have been found or on error, else gets a message.
 */
bool mgclGetMessage(PMSG pMsg, HWND hWnd);

/**
 * @brief mgclTranslateMessage Translates key down and key up messages to
 *        MSG_CHAR message and post it into the message queue.
 *
 * @param pMsg The pointer of message.
 *
 * @returns A boolean indicates whether the message is a key message.
 */
bool mgclTranslateMessage(PMSG pMsg);

/**
 * @brief mgclDispatchMessage Dispatches a message to the window's callback procedure.
 *
 * @param pMsg The pointer to the message.
 *
 * @returns The return value of the message handler.
 */
int mgclDispatchMessage(PMSG pMsg);

/**
 * @brief mgclInitGUI Initialize MiniGUI.
 *
 * @param args The meaning of parameter is same with parameter of main function.
 * @param argv The meaning of parameter is same with parameter of main function.
 *
 * @returns 0 on sucess, otherwise false.
 */
int mgclInitGUI(int args, const char** argv);

/**
 * @brief mgclGetSecondaryDC Retrives and returns the secondary DC of a
 *        specific window.
 *
 * @param hwnd The handle to the window.
 *
 * @returns The handle to the secondary DC, HDC_SCREEN indicates that 
 *          the window has no secondary DC.
 */
HDC mgclGetSecondaryDC(HWND hwnd);

/**
 * @brief mgclGetCapture Retrives the handle to the window (if any)
 *        that has captured the mouse.
 *
 * @returns The handle to the window that has captured the mouse, 0 for
 *          no window captures the mouse.
 */
HWND mgclGetCapture(void);

/**
 * @brief mgclSetCapture Sets the mouse capture to the specified window.
 *
 * @param hWnd The handle to the window.
 *
 * @returns The old capture window.
 */
HWND mgclSetCapture(HWND hWnd);

/**
 * @brief mgclReleaseCapture Releases the mouse capture from a window and
 * restores normal mouse input processing.
 */
void mgclReleaseCapture(void);

/**
 * @brief mgclGetClientRect Retrives the client rectangle of a window.
 *
 * @param hWnd The handle to the window.
 * @param prc The pointer to a RECT structure receives the client rectangle.
 *
 * @returns true on sucess, otherwise false.
 */
bool mgclGetClientRect(HWND hWnd, PRECT prc);

/**
 * @brief mgclPtInRect Determines whether a point lies within an rectangle.
 *
 * @param prc The pointer to the rectangle.
 * @param x x,y: The point.
 * @param y x,y: The point.
 *
 * @returns true on sucess, otherwise false.
 */
bool mgclPtInRect(const RECT* prc, int x, int y);

/**
 * @brief mgclInvalidateRect Makes a rectangle region in the client area of a
 *        window invalid.
 *
 * @param hWnd The handle to the window.
 * @param prc The pointer to a RECT structure which defines the invalid rectangle.
 * @param bEraseBkgnd Indicates whether the background should be erased.
 *
 * @returns true on success, otherwise false.
 */
bool mgclInvalidateRect(HWND hWnd, const RECT* prc, bool bEraseBkgnd);

/**
 * @brief mgclGetCursorPos Gets position of the current cursor.
 *
 * @param ppt The position will be saved in this buffer.
 */
void mgclGetCursorPos(POINT* ppt);

/**
 * @brief mgclScreenToClient Converts the screen coordinates of a point to
 *        client coordinates.
 *
 * @param hWnd The handle to the window.
 * @param x The pointer to the x coordinate.
 * @param y The pointer to the y coordinate.
 */
void mgclScreenToClient(HWND hWnd, int* x, int* y);

/**
 * \brief Converts the client coordinates of a point to screen coordinates.
 *
 * This function converts the client coordinates of the specified point
 * \a (*x,*y) in the specified window \a hWnd to screen coordinates.
 *
 * \param hWnd The handle to the window.
 * \param x The pointer to the x coordinate.
 * \param y The pointer to the y coordinate.
 */
void mgclClientToScreen(HWND hWnd, int* x, int* y);

/**
 * \brief Determines whether a window is a main window.
 *
 * This function determines whether the specified window \a hWnd is
 * a main window or not.
 *
 * \param hWnd The handle to the window.
 *
 * \return TRUE for main window, otherwise FALSE.
 */
bool mgclIsMainWindow(HWND hWnd);

/**
 * @brief mgclBeginPaint Prepares a window for painting.
 *
 * @param hWnd The handle to the window.
 *
 * @returns The device context.
 */
HDC mgclBeginPaint(HWND hWnd);

/**
 * @brief mgclEndPaint Marks the end of painting in a window.
 *
 * @param hWnd The handle to the window.
 * @param hdc The device context returned by BeginPaint.
 */
void mgclEndPaint(HWND hWnd, HDC hdc);

/**
 * @brief mgclMoveWindow Changes the position and dimensions of a window.
 *
 * @param hWnd The handle to the window.
 * @param x The new x coordinate of the upper-left corner of the window.
 * @param y The new y coordinate of the upper-left corner of the window.
 * @param w The new width of the window.
 * @param h The new height of the window.
 * @param fPaint Indicates whether the window should be repainted.
 *
 * @return TRUE on success, otherwise FALSE.
 */
bool mgclMoveWindow(HWND hWnd, int x, int y, int w, int h, bool fPaint);

/**
 * @brief mgclSetActiveWindow Sets a main window to be the active main window.
 *
 * @param hMainWnd The handle to the new active main window.
 *
 * @returns The handle to the old active main window.
 */
unsigned int mgclSetActiveWindow(unsigned int hMainWnd);

/**
 * @brief mgclGetActiveWindow Retrieves the main window handle to the active main window.
 *
 * @returns The handle to the active main window.
 */
unsigned int mgclGetActiveWindow(void);

/**
 * @brief mgclUpdateWindow Updates a window.
 *
 * @param hWnd The handle to the window.
 * @param bErase Indicates whether to erase the client area of the window.
 */
void mgclUpdateWindow(unsigned int hWnd, bool bErase);


void mgclSetWindowExStyle(HWND hwnd ,int dwStyle, bool isInclude);
long unsigned mgclGetWindowExStyle(HWND hwnd);
int mgclGetTextMCharInfo(HWND hWnd, const char* mstr, int len, int* pos_chars);

} // namespace hfcl

#endif /* __HFCL_MGCLACTIVITY_H__ */

