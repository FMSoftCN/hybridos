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

#ifndef _NGUX_Image_h
#define _NGUX_Image_h

#include "log.h"
#include "nguxobject.h"
#include "intrect.h"
#include "graphicscontext.h"
#include "drawable-id.h"

NAMESPACE_BEGIN


class Image : public RefCount {
    public:
        Image();
        virtual ~Image();

        /*static func to create a image or nine - patch image from file.*/
        static Image* loadImage(NGCPStr file_path);

        virtual void paint(GraphicsContext* context, const IntRect& rc,
                ImageFormat format, NGInt xo = 0, NGInt yo = 0);

        virtual NGBool setImage(NGCPStr file_path);
        virtual NGBool setImageBitmap(Bitmap* pbmp);

		void setLoadOnPainting(NGBool bLoadOnPainting) { m_bLoadOnPainting = bLoadOnPainting; }
		NGBool isLoadOnPainting(void) { return m_bLoadOnPainting; }
		
        NGInt width(void);
        NGInt height(void);
		NGBool setReplaceColor(const DWORD color);
		
    protected:
		void calc_pos_align(NGInt *px, NGInt *py, const IntRect *rc, NGInt imgw, NGInt imgh, ImageFormat format);

    protected:
		NGBool 		m_bLoadOnPainting;
	  	string      m_filePath;
        Bitmap*     m_pBitmap;
		DWORD 	m_replaceColor;
};


AUTOPTR(Image)

NAMESPACE_END

#endif  /* NGUX_Image_h */

