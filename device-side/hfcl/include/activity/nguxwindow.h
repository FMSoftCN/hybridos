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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef __NGUX_WINDOW_H__
#define __NGUX_WINDOW_H__

#include "panelview.h"

NAMESPACE_BEGIN

class KeyEvent;

class Window : public PanelView
{
    public:
        enum{
            NGUX_WS_EX_TRANSPARENT = WS_EX_TRANSPARENT,
        };
        Window();
        virtual ~Window();

        void setAppRect(const IntRect& irc);
        void getAppClientRect(IntRect& irc);

        void show(NGBool bUpdateBack = true);
        void hide(void);
        void destroy(void);

        virtual NGBool onKey(NGInt keyCode, KeyEvent* event);
        virtual void onClick(POINT pt, Event::EventType type);
        virtual void onBack(void) { }
		virtual void onIdle(void) { }

        //void paint(void);
        void freezePaint(NGBool bFreeze) { m_freezePaint = bFreeze; }
        NGBool isPaintFrozen(void) { return m_freezePaint; }

		// Synchronous
        void updateWindow(NGBool isUpdateBkg = TRUE);

        HWND viewWindow(void) const;
        static Window* window(HWND hwnd);

        NGUInt setActiveWindow(NGUInt hMainWnd);
        NGUInt getActiveWindow(void);

		// Asynchronous
        void asyncUpdateRect(NGInt x, NGInt y, NGInt w, NGInt h, NGBool upBackGnd = true);
		// Synchronous
        void syncUpdateRect(NGInt x, NGInt y, NGInt w, NGInt h, NGBool upBackGnd = true);

        NGBool isTopView(void) { return true; }

        virtual HPlatformOwner getPlatformOwner(void);

        void setBkgAlpha(unsigned char a){ m_bkg_alpha = a;}
        unsigned char bkgAlpha(){return m_bkg_alpha ;}

		static NGBool updateLocked(void) { return m_updateLocked; }
		static void setUpdateLock(NGBool lock) { m_updateLocked = lock; }

        NGInt doModal(NGBool bAutoDestory = false);
        NGUInt doModalView();
        void endDlg(NGInt endCode);
        
        void lockKeyOnPainting (NGBool bLockable) { m_keyLockable = bLockable; }
		virtual void drawScroll(GraphicsContext* context, IntRect &rc, NGInt status);
        virtual void drawBackground(GraphicsContext* context, IntRect &rc, NGInt status);

    protected:
        NGBool createMainWindow(void);
        virtual NGBool createMainWindow(NGInt x, NGInt y, NGInt w, NGInt h, NGBool visible = true);

		NGInt sendKeyMessage(Event::EventType ketype, WPARAM wParam, LPARAM lParam);
		NGInt sendMouseMessage(Event::EventType Mousetype, WPARAM wParam, LPARAM lParam);
		NGInt sendIdleMessage();
        
        HWND m_viewWindow;
        HDC m_context;
        
		static NGInt defaultAppProc(HWND hWnd, NGInt message, WPARAM wParam, LPARAM lParam);
        static HWND createMainWindow(const char* caption, WNDPROC proc,
                NGInt x, NGInt y, NGInt width, NGInt height, DWORD addData, NGBool visible = true);
    private:
        unsigned char m_bkg_alpha;   //default the value is 0, window is transparent
        NGBool m_keyLockable;
        NGBool m_keyLocked;
        NGBool m_freezePaint;
		View *m_mouseDownView;
		View *m_mouseMoveView;
		static NGBool m_updateLocked;

        virtual void inner_updateView(NGInt x, NGInt y, NGInt w, NGInt h, NGBool upBackGnd = true);
};

NAMESPACE_END

#endif /* __NGUX_WINDOW_H__ */

