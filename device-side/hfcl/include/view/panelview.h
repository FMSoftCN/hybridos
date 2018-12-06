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


#ifndef _HFCL_VIEW_PANELVIEW_H_
#define _HFCL_VIEW_PANELVIEW_H_

#define FILE_NAME_LEN 256

#include "containerview.h"
#include "../graphics/graphicscontext.h"

namespace hfcl {
class Image;

class PanelView : public ContainerView
{
public:
    PanelView(View* parent, DrawableSet* drset);
    PanelView(View* parent);
    PanelView(int id, int x, int y, int w, int h);
    virtual ~PanelView();

    //Image* getBkImage();
    bool setBkColor(const DWORD color);
    bool setBkImage(const char * image_file);
    bool setBkImageEx(Image* image);
    bool setUpdate(bool update);
    
    bool isUpdate(void){ return m_update;};
    
	virtual void drawBackground(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

    //bool setBkOpacity(unsigned char opacity);

    void setImageDrawMode(int mode);

protected:
	enum {
		FLAG_SHIFT = ContainerView::FLAG_SHIFT
	};

private:
	bool 	m_update;
	Image 	*m_bkImage;
	int 	m_imageDrawMode;
	bool 	m_bkColorBeSet;
	DWORD 	m_bkColor;

    DECLARE_CLASS_NAME(PanelView)
};

} // namespace hfcl

#endif /* HFCL_VIEW_PANELVIEW_H_ */
