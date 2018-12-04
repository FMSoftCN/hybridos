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


#ifndef _HFCL_TrackBarView_h
#define _HFCL_TrackBarView_h

#include "view/view.h"

namespace hfcl {

class TrackBarView : public View
{
    public:
        typedef enum _TrackBarType{ VERTICAL, HORIZONTAL} TRACKBARTYPE;


        TrackBarView();
        TrackBarView(View* parent);
        TrackBarView(View* parent, DrawableSet* drset);
        TrackBarView(int i_id, int x, int y, int w, int h);
        //TrackBarView(Image* startImg, Image* pauseImg, GNInt min = 0, GNInt max = 100);
        ~TrackBarView();
        
        void setType(TRACKBARTYPE t_type){ m_type = t_type; }
        TRACKBARTYPE type(void){ return m_type; }

        void setBorder(int borderPixel){ m_border = borderPixel; }

        //set ThumbBlock image to startimage.
        //void start(void){ setThumbBlockImage(m_startImg); }
        //set ThumbBlock image to pauseimage.
        //void pause(void){ setThumbBlockImage(m_pauseImg); }

        void stepForward(void); //go forward a distance of m_stepValue.
        void stepBackward(void);//go backword a distance of m_stepValue.

        void setStepValue(int value){ m_stepValue = value; }
        int stepValue(void){ return m_stepValue; }

        void setMaxValue(int value){ m_maxValue = value; }
        int maxValue(void){ return m_maxValue; }
        void setMinValue(int value){ m_minValue = value; }
        int minValue(void){ return m_minValue; }
        void setPosition(int value); //update to the new position.
        int position(void){ return m_curValue; }

        //void setPauseImage(Image* img){ m_pauseImg = img; }
        //void setStartImage(Image* img){ m_startImg = img; }
        //void setTrackBarImage(Image* img);
        //void setThumbBlockImage(Image* img);

        virtual void drawContent(GraphicsContext* context, IntRect &rc, int status/* = Style::NORMAL*/);

    protected:
        //void initDrawable(DrawableSelector* selector);

    private:
        void initTrackBarView(void);

    protected:
        TRACKBARTYPE m_type;
        int m_curValue;
        int m_maxValue;
        int m_minValue;
        int m_stepValue;
        int m_border;
        //Image* m_startImg;
        //Image* m_pauseImg;
        //ImageDrawable* m_drTrackBar;
        //ImageDrawable* m_drThumbBlock;

	DECLARE_CLASS_NAME(TrackBarView)
};

} // namespace hfcl {
#endif /*_HFCL_TrackBarView_h*/
