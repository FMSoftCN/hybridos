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

#ifndef _HFCL_Image_h
#define _HFCL_Image_h

#include "common/log.h"
#include "common/object.h"
#include "common/intrect.h"
#include "graphics/graphicscontext.h"
#include "drawable/drawable-id.h"

namespace hfcl {


class Image : public RefCount {
    public:
        Image();
        virtual ~Image();

        /*static func to create a image or nine - patch image from file.*/
        static Image* loadImage(const char * file_path);

        virtual void paint(GraphicsContext* context, const IntRect& rc,
                ImageFormat format, int xo = 0, int yo = 0);

        virtual bool setImage(const char * file_path);
        virtual bool setImageBitmap(Bitmap* pbmp);

		void setLoadOnPainting(bool bLoadOnPainting) { m_bLoadOnPainting = bLoadOnPainting; }
		bool isLoadOnPainting(void) { return m_bLoadOnPainting; }
		
        int width(void);
        int height(void);
		bool setReplaceColor(const DWORD color);
		
    protected:
		void calc_pos_align(int *px, int *py, const IntRect *rc, int imgw, int imgh, ImageFormat format);

    protected:
		bool 		m_bLoadOnPainting;
	  	string      m_filePath;
        Bitmap*     m_pBitmap;
		DWORD 	m_replaceColor;
};


AUTOPTR(Image)

} // namespace hfcl

#endif  /* HFCL_Image_h */

