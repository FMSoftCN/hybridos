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


#ifndef _NGUX_PanelView_h
#define _NGUX_PanelView_h

#define FILE_NAME_LEN 256

#include "containerview.h"
#include "graphicscontext.h"

NAMESPACE_BEGIN
class Image;

class PanelView : public ContainerView
{
public:
    PanelView(View* parent, DrawableSet* drset);
    PanelView(View* parent);
    PanelView(NGInt id, NGInt x, NGInt y, NGInt w, NGInt h);
    virtual ~PanelView();

    //Image* getBkImage();
    NGBool setBkColor(const DWORD color);
    NGBool setBkImage(NGCPStr image_file);
    NGBool setBkImageEx(Image* image);
    NGBool setUpdate(NGBool update);
    
    NGBool isUpdate(void){ return m_update;};
    
	virtual void drawBackground(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);

    //NGBool setBkOpacity(unsigned char opacity);

    void setImageDrawMode(NGInt mode);

protected:

	enum {
		FLAG_SHIFT = ContainerView::FLAG_SHIFT
	};

private:
	NGBool 	m_update;
	Image 	*m_bkImage;
	NGInt 	m_imageDrawMode;
	NGBool 	m_bkColorBeSet;
	DWORD 	m_bkColor;

DECLARE_CLASS_NAME(PanelView)

}; //end of PanelView

NAMESPACE_END

#endif /* NGUX_PanelView_h */
