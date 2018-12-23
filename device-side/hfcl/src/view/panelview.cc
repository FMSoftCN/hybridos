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

#include "view/panelview.h"
#include "graphics/image.h"
#include "drawable/imagedrawable.h"

namespace hfcl {

PanelView::PanelView(View *p_parent)
    : ContainerView(p_parent, DEFAULT_VIEW_DRAWABLESET(PanelView))
    , m_bkImage(NULL)
    , m_imageDrawMode(DRAWMODE_NORMAL)
    , m_bkColorBeSet(false)
{
    m_update = true;
}

PanelView::PanelView(View* p_parent, DrawableSet* drset)
    : ContainerView(p_parent, drset)
    , m_bkImage(NULL)
    , m_imageDrawMode(DRAWMODE_NORMAL)
    , m_bkColorBeSet(false)
{
    m_update = true;
}

PanelView::PanelView(int i_id, int x, int y, int w, int h)
    : ContainerView(i_id, x, y, w, h)
    , m_bkImage(NULL)
    , m_imageDrawMode(DRAWMODE_NORMAL)
    , m_bkColorBeSet(false)

{
    m_update = true;
}

PanelView::~PanelView()
{
    if (m_bkImage != NULL)
    {
        HFCL_DELETE(m_bkImage);
    }
}

bool PanelView::setBkImage(const char * image_file)
{
    if (NULL == image_file) {
        return false;
    }

    return setBkImageEx(Image::loadImage(image_file));
}

bool PanelView::setBkImageEx(Image* image)
{
    if(!image)
        return false;

    if (m_bkImage != NULL){
        HFCL_DELETE(m_bkImage);
    }
    m_bkImage = image;

    m_bkColorBeSet = false;
    updateView();

    return true;
}

bool PanelView::setBkColor(const DWORD color)
{
    if (m_bkImage != NULL)
    {
        HFCL_DELETE(m_bkImage);
        m_bkImage = NULL;
    }
    m_bkColor = color;
    m_bkColorBeSet = true;

    updateView();
    return true;
}

void PanelView::setImageDrawMode(int mode)
{
    m_imageDrawMode = mode;
}

bool PanelView::setUpdate(bool update) {
    m_update = update;
    return true;
}

void PanelView::drawBackground(GraphicsContext* context, IntRect &rc)
{
    if(m_update) {
        if (m_bkImage != NULL) {
            ImageFormat f;
            f.drawMode = m_imageDrawMode;
            m_bkImage->paint(context, rc, f);
        }
        else if (m_bkColorBeSet) {
            //_DBG_PRINTF("PanelView::drawBackground1>>>(%d,%d,%d,%d)",rc.left(),rc.right(),rc.top(),rc.bottom());
            context->fillRect(rc, GetRValue(m_bkColor), GetGValue(m_bkColor), GetBValue(m_bkColor), GetAValue(m_bkColor));
        }
#if 0
        else if (m_drset) {
            //_DBG_PRINTF("PanelView::drawBackground2>>>(%d,%d,%d,%d)",rc.left(),rc.right(),rc.top(),rc.bottom());
            if(isFocused())
                m_drset->draw(context, DR_BKGND, DRAWSTATE_HILIGHT, rc);
            else
                m_drset->draw(context, DR_BKGND, DRAWSTATE_NORMAL, rc);
        }
#endif
    }
}

DEFINE_CLASS_NAME(PanelView)

} // namespace hfcl
