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


#ifndef HFCL_VIEW_IMAGEVIEW_H_
#define HFCL_VIEW_IMAGEVIEW_H_

#include "graphics/image.h"
#include "view/view.h"

namespace hfcl {

class ImageView : public View {
public:
    ImageView(View* parent);
    ImageView(View* parent, DrawableSet* drset);
    ImageView(Image* pImage, int id, int x, int y, int w, int h);
    ImageView(const char * filePath, int id, int x, int y, int w, int h,
            int mode = DRAWMODE_NORMAL,
            int align = ALIGN_CENTER,
            int valign = VALIGN_MIDDLE);

    virtual ~ImageView();

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

protected:
    Image*    m_image;
    ImageFormat m_format;
    int m_xo;
    int m_yo;

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
};

} // namespace hfcl

#endif /* HFCL_VIEW_IMAGEVIEW_H_ */

