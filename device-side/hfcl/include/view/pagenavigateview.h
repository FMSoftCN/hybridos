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

#ifndef _HFCL_PageNavigateView_h
#define _HFCL_PageNavigateView_h

#include "panelview.h"

namespace hfcl {

class Image;
class ImageView;
class PageView;

class PageNavigateView : public PanelView
{
    public:
        /*
         * \create PageNavigateview.
         *
         * param id is view id.
         * param x,y,w,h is rect value.
         * param pageview can not be null.
         * param normalImageFile is normal page image file name.
         * param curImageFile is current page image file name.
         * param leftImageFile is left arrow image file name.
         * param rightImageFile is right arrow image file name.
         */
        PageNavigateView(int id,
                            int x, int y, int w, int h,
                            PageView* pageView,
                            const char* normalImageFile,
                            const char* curImageFile,
                            const char* leftImageFile,
                            const char* rightImageFile
                            );
        /*
         * if use this func create PageNavigateView, later:
         *
         * 1\ create PageView
         * 2\ PageNavigateView->setRect();
         * 3\ PageNavigateView->setPageView()
         *                    ->setNormalImage()
         *                    ->setCurImage()
         *                    ->setLeftImage()  // or ignore.
         *                    ->setRightImage() // or ignore.
         * 4\ PageNavigateView->setScaled();    // or ignore.
         * 5\ PageNavigateView->init();
         */
        PageNavigateView(View *parent, DrawableSet* drset);

        ~PageNavigateView();

        void setNormalImage(Image *image){
            m_normalImage = image;
        }

        void setCurImage(Image *image){
            m_curImage = image;
        }

        void setLeftImage(Image *image){
            m_leftImage = image;
        }

        void setRightImage(Image *image){
            m_rightImage = image;
        }

        void setPageView(PageView* pageView){
            m_pageView = pageView;
        }

        /*
         * set NormalImageView 、CurImageView 、LeftImageView and
         * RightImageView Scaled or Normal.
         *
         * if use this func sould called in front of init().
         */
        void setScaled(bool b_isScaled) { setFlag(b_isScaled, SCALED); }
		bool isScaled() { return m_flags & SCALED; }

        /*
         * by m_pageview, m_normalImage, m_curImage, m_leftImage
         * and m_rightImage init pageNavigateView.
         *
         * will auto call isInit(), if it return true, will return.
         *
         * PageNavigateView(...) will auto
         * call this func.
         */
        bool init();

        /*
         * judge m_pageView, m_normalImageView and m_curImageView not NULL.
         */
        bool isInit();

        /*left move current image.*/
        bool next();

        /*right move current image.*/
        bool prev();

        /*get focus will show two arrows.*/
        void onGetFocus();

        /*lose focus will hide two arrows.*/
        void onLoseFocus();

        bool dispatchEvent(Event *event);


        PageNavigateView(View *parent);

//        bool pageViewChanged();        //no use.

    private:
        int m_width;

        Image* m_normalImage;
        Image* m_curImage;
        Image* m_leftImage;
        Image* m_rightImage;

        ImageView* m_curImageView;
        ImageView* m_leftImageView;
        ImageView* m_rightImageView;

        PageView* m_pageView;

        //PanelView* m_panelView;
        ContainerView* m_panelView;

		enum {
			SCALED = (1 << PanelView::FLAG_SHIFT),
			FLAG_SHIFT = (1 + PanelView::FLAG_SHIFT)
		};

};

} // namespace hfcl

#endif /* _HFCL_PageNavigateView_h */
