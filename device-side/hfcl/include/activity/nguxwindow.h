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


#ifndef __HFCL_WINDOW_H__
#define __HFCL_WINDOW_H__

#include "panelview.h"

namespace hfcl {

class KeyEvent;

class Window : public PanelView
{
    public:
        enum{
            HFCL_WS_EX_TRANSPARENT = WS_EX_TRANSPARENT,
        };
        Window();
        virtual ~Window();

        void setAppRect(const IntRect& irc);
        void getAppClientRect(IntRect& irc);

        void show(bool bUpdateBack = true);
        void hide(void);
        void destroy(void);

        virtual bool onKey(int keyCode, KeyEvent* event);
        virtual void onClick(POINT pt, Event::EventType type);
        virtual void onBack(void) { }
		virtual void onIdle(void) { }

        //void paint(void);
        void freezePaint(bool bFreeze) { m_freezePaint = bFreeze; }
        bool isPaintFrozen(void) { return m_freezePaint; }

		// Synchronous
        void updateWindow(bool isUpdateBkg = TRUE);

        HWND viewWindow(void) const;
        static Window* window(HWND hwnd);

        unsigned int setActiveWindow(unsigned int hMainWnd);
        unsigned int getActiveWindow(void);

		// Asynchronous
        void asyncUpdateRect(int x, int y, int w, int h, bool upBackGnd = true);
		// Synchronous
        void syncUpdateRect(int x, int y, int w, int h, bool upBackGnd = true);

        bool isTopView(void) { return true; }

        virtual HPlatformOwner getPlatformOwner(void);

        void setBkgAlpha(unsigned char a){ m_bkg_alpha = a;}
        unsigned char bkgAlpha(){return m_bkg_alpha ;}

		static bool updateLocked(void) { return m_updateLocked; }
		static void setUpdateLock(bool lock) { m_updateLocked = lock; }

        int doModal(bool bAutoDestory = false);
        unsigned int doModalView();
        void endDlg(int endCode);
        
        void lockKeyOnPainting (bool bLockable) { m_keyLockable = bLockable; }
		virtual void drawScroll(GraphicsContext* context, IntRect &rc, int status);
        virtual void drawBackground(GraphicsContext* context, IntRect &rc, int status);

    protected:
        bool createMainWindow(void);
        virtual bool createMainWindow(int x, int y, int w, int h, bool visible = true);

		int sendKeyMessage(Event::EventType ketype, WPARAM wParam, LPARAM lParam);
		int sendMouseMessage(Event::EventType Mousetype, WPARAM wParam, LPARAM lParam);
		int sendIdleMessage();
        
        HWND m_viewWindow;
        HDC m_context;
        
		static int defaultAppProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
        static HWND createMainWindow(const char* caption, WNDPROC proc,
                int x, int y, int width, int height, DWORD addData, bool visible = true);
    private:
        unsigned char m_bkg_alpha;   //default the value is 0, window is transparent
        bool m_keyLockable;
        bool m_keyLocked;
        bool m_freezePaint;
		View *m_mouseDownView;
		View *m_mouseMoveView;
		static bool m_updateLocked;

        virtual void inner_updateView(int x, int y, int w, int h, bool upBackGnd = true);
};

} // namespace hfcl {

#endif /* __HFCL_WINDOW_H__ */
