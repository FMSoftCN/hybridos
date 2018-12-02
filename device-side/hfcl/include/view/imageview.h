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


#ifndef _NGUX_ImageView_h
#define _NGUX_ImageView_h

#include "image.h"
#include "view.h"

NAMESPACE_BEGIN

class ImageView : public View {
    public:
        ImageView(View* parent);
        ImageView(View* parent, DrawableSet* drset);
        ImageView(Image* pImage, NGInt id, NGInt x, NGInt y, NGInt w, NGInt h);
        ImageView(const NGPStr filePath, NGInt id, NGInt x, NGInt y, NGInt w, NGInt h,
                NGInt mode = DRAWMODE_NORMAL,
                NGInt align = ALIGN_CENTER,
                NGInt valign = VALIGN_MIDDLE);

        virtual ~ImageView();

        bool setImage(Image* pImg);
		Image *getImage(void) { return m_image; }
		
        void setImageDrawMode(NGInt  mode) {
            m_format.drawMode = mode;
        }
        void setImageAlign(NGInt halign) {
            m_format.align = halign;
        }
        void setImageVAlign(NGInt valign) {
            m_format.valign = valign;
        }
	 	void setImageRotationAngle(NGInt RotationAngle) {
            m_format.rotationAngle = RotationAngle;
        }

        virtual void drawContent(GraphicsContext* context,
                IntRect &rc, NGInt status/*= Style::NORMAL*/);

        virtual void drawBackground(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);

        void setPartBoxXoYo(NGInt xo = 0, NGInt yo = 0);
        void getPartBoxXoYo(NGInt& xo, NGInt& yo);

        void setFromImgRect(NGBool bSet = false);
        NGInt getImageWidth(void);
        NGInt getImageHeight(void);
		NGBool setReplaceColor(const DWORD color);

    protected:
        Image*    m_image;
        ImageFormat m_format;

        void init() {
            m_format.drawMode = DRAWMODE_NORMAL;
            m_format.align    = ALIGN_CENTER;
            m_format.valign   = VALIGN_MIDDLE;
	     	m_format.rotationAngle   = 0;

            m_xo = 0;
            m_yo = 0;

			m_image = NULL;
        }

        DECLARE_CLASS_NAME(ImageView)
    private:
        NGInt m_xo;
        NGInt m_yo;
};

NAMESPACE_END

#endif /* NGUX_ImageView_h */

