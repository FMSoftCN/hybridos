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


#ifndef _NGUX_AnimateImageView_h
#define _NGUX_AnimateImageView_h

#include "image.h"
#include "view.h"
#include "timerservice.h"

NAMESPACE_BEGIN

class AnimateImageView : public View, TimerEventListener
{
    public:
		typedef enum _tagPlayState
        {
            Play = 0,
            Pause,
            Stop
        }PlayState;
		
        AnimateImageView(View* parent);
        AnimateImageView(View* parent, DrawableSet* drset);
        AnimateImageView(Image* pImage, int id, int x, int y, int w, int h);
        AnimateImageView(const char * filePath, int id, int x, int y, int w, int h,
                int mode = DRAWMODE_NORMAL,
                int align = ALIGN_CENTER,
                int valign = VALIGN_MIDDLE);

        virtual ~AnimateImageView();

        bool setImage(Image* pImg);
		Image *getImage(void) { return m_image; }
		
        void setImageDrawMode(int  mode) {
            m_format.drawMode = mode;
        }
        void setImageAlign(int halign) {
            m_format.align = halign;
        }
        void setImageVAlign(int valign) {
            m_format.valign = valign;
        }
	 	void setImageRotationAngle(int RotationAngle) {
            m_format.rotationAngle = RotationAngle;
        }

        virtual void drawContent(GraphicsContext* context,
                IntRect &rc, int status/*= Style::NORMAL*/);

        virtual void drawBackground(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

        void setPartBoxXoYo(int xo = 0, int yo = 0);
        void getPartBoxXoYo(int& xo, int& yo);

        void setFromImgRect(bool bSet = false);
        int getImageWidth(void);
        int getImageHeight(void);
		bool setReplaceColor(const DWORD color);
		bool start(void);
        bool stop(void); 
		bool setImageIdArray(unsigned int * array, unsigned int num, int timeDuration,const DWORD color);
		bool handleEvent(Event* event);
		
    protected:
        Image*    		m_image;
        ImageFormat 	m_format;
		unsigned int  * m_imageId_array;
		unsigned int    m_imageId_array_num;
		unsigned int    m_imageId_array_index;
		int		        m_timeDuration;
		int         	m_elapsed_10ms;

	protected:
        void init() {
            m_format.drawMode = DRAWMODE_NORMAL;
            m_format.align    = ALIGN_CENTER;
            m_format.valign   = VALIGN_MIDDLE;
	     	m_format.rotationAngle   = 0;

            m_xo = 0;
            m_yo = 0;
			m_imageId_array_num = 0;
			m_imageId_array_index = 0;
			m_timeDuration = 0;
			m_elapsed_10ms = 0;
			m_image = NULL;
			m_state = Stop;
			m_replaceColor = 0;
        }

        DECLARE_CLASS_NAME(AnimateImageView)
    private:
        int m_xo;
        int m_yo;
        int         m_timer_id;
        PlayState   m_state;
        DWORD 	m_replaceColor;
};

NAMESPACE_END

#endif /* NGUX_ImageView_h */

