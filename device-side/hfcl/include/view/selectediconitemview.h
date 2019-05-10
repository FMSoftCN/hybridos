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


#ifndef _HFCL_IconItemView_h
#define _HFCL_IconItemView_h

#include "itemview.h"
#include "../graphics/image.h"

namespace hfcl {

class SelectedIconItemView : public ItemView
{
public:
    SelectedIconItemView() { }
    SelectedIconItemView(View *p_parent)
        :ItemView(p_parent, DEFAULT_VIEW_DRAWABLESET(SelectedIconItemView))
        ,m_normalImg(NULL)
        ,m_selImg(NULL)
    { }
    SelectedIconItemView(View *p_parent, DrawableSet* drset)
        :ItemView(p_parent ,drset)
        ,m_normalImg(NULL)
        ,m_selImg(NULL)
    { }

    virtual ~SelectedIconItemView();

    void setNormalImage(Image *img);
    void setSelectedImage(Image* img);

    Image* getNormalImage() { return m_normalImg; }
    Image* getSelectedImage() { return m_selImg; }

    void drawContent(GraphicsContext* context, IntRect &rc, int status);

protected:
    Image* m_normalImg;
    Image* m_selImg;

    DECLARE_CLASS_NAME(SelectedIconItemView)

};


} // namespace hfcl

#endif

