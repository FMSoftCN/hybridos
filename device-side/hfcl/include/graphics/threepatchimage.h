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

#ifndef _HFCL_ThreePatchImage_h
#define _HFCL_ThreePatchImage_h

#include "graphics/image.h"


namespace hfcl {

#define THREEH_PATCH_COUNT  3

class ThreePatchImage : public Image
{
public:
    struct DRAWINFO
    {
        /* The index of line to current subbitmap. */
        unsigned int idx_line;
        /* The index of colume to current subbitmap. */
        unsigned int idx_col;
        /* The number of line in this subbitmap. */
        unsigned int line_count;
        /* The number of colume in this subbitmap. */
        unsigned int col_count;
    };

    ThreePatchImage(bool typeHoriz = true);
    virtual ~ThreePatchImage();

    virtual void paint(GraphicsContext* context, const IntRect& rc, 
			ImageFormat format, int xo = 0, int yo = 0);
    bool setImage(const char *image_file);

    bool typeHoriz() const { return m_typeHoriz; }
    void setTypeHoriz(bool type) { m_typeHoriz = type; }

	virtual bool setImageBitmap(Bitmap* pbmp) {
		if(!Image::setImageBitmap(pbmp)) {
			return false;
		}
		return setSubBmp();
	}

private:
    bool makeSubDrawInfo();
    void initSubBmp();
    bool setSubBmp();
    void clean();
    bool initSubRect(const IntRect* pRc, ImageFormat format);
    bool initSubHRect(const IntRect* pRc, ImageFormat format);
    bool initSubVRect(const IntRect* pRc, ImageFormat format);

private:
    DRAWINFO m_di[THREEH_PATCH_COUNT];
    IntRect m_subRc[THREEH_PATCH_COUNT];
    Bitmap m_subBmp[THREEH_PATCH_COUNT];
    bool m_typeHoriz;
};

} // namespace hfcl

#endif /* _HFCL_ThreePatchImage_h */
