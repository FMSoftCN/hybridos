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


#ifndef _NGUX_GifAnimateView_h
#define _NGUX_GifAnimateView_h

#include "view.h"
#include "timerservice.h"

NAMESPACE_BEGIN

class GifAnimate;

class GifAnimateView : public View, TimerEventListener {
    public:

        typedef enum _tagPlayState
        {
            Play = 0,
            Pause,
            Stop
        }PlayState;

        typedef enum _tagLoopType
        {
            Loop = 0,
            NoLoop
        }LoopType;

        typedef enum _tagPlayType
        {
            AutoPlay,
            NoAutoPlay
        }PlayType;

		enum {
			NOTIFY_BEGIN = CustomEvent::CUS_MAX,
			NOTIFY_GIFANIMATE_STOP = CustomEvent::CUS_GIFANIMATE_STOP,
		};

		GifAnimateView();
        GifAnimateView(View* parent);
		GifAnimateView(View* parent, DrawableSet* drset);
		GifAnimateView(int id, int x, int y, int w, int h);
		virtual ~GifAnimateView();

        void setPlayType(PlayType type) {m_playType = type;}
        PlayType playType(void) {return m_playType;}
        void setLoopType(LoopType type) {m_loopType = type;}
        LoopType loopType(void) {return m_loopType;}

        virtual void onPaint(GraphicsContext* context, int status /*= Style::NORMAL*/);
        virtual bool handleEvent(Event* event);
		//return True if the event was handled, false otherwise.
		virtual bool dispatchEvent(Event* event);

        NGBool start(void);
        NGBool pause(void);
	  	NGBool resume(void);
        NGBool stop(void); 
        NGBool reset(void);
        PlayState state(void) {return m_state;}

        void setGifAnimate(GifAnimate* animate);
        void setGifFile(const char* animate);
        GifAnimate *gifAnimate(void){return m_animate;}
	  	void setLayer(int layerNo) { m_drawLayer = layerNo; }
	  	int layer () { return m_drawLayer; }

    private:
        GifAnimate* m_animate;
        int         m_timer_id;
        int         m_elapsed_10ms;
        PlayState   m_state;
        PlayType    m_playType;
        LoopType    m_loopType;
        NGUInt32    m_start_ticks;
	 	NGInt m_drawLayer;
};

NAMESPACE_END

#endif /* NGUX_GifAnimateView_h */
