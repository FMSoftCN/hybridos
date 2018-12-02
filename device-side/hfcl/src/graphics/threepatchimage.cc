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

#include "threepatchimage.h"
#include "resloader.h"

namespace hfcl {

ThreePatchImage::ThreePatchImage(bool b_typeHoriz)
	: Image(), m_typeHoriz(b_typeHoriz)
{
	initSubBmp();
}

ThreePatchImage::~ThreePatchImage()
{
	clean();
}

void ThreePatchImage::paint(GraphicsContext* context, const IntRect& rc, ImageFormat format, int xo, int yo)
{
	if (m_bLoadOnPainting) {
		setImageBitmap(ResLoader::getInstance()->getBitmap(m_filePath.c_str()));
	}

	if (NULL == m_pBitmap) {
		return;
	}

	if (!initSubRect(&rc, format)) {
		return;
	}

	for (int i = 0; i < THREEH_PATCH_COUNT; i++) {
		context->fillBoxWithBitmap(m_subRc[i].left(), m_subRc[i].top(),
				m_subRc[i].width(), m_subRc[i].height(), &m_subBmp[i]);
	}
	
	if (m_bLoadOnPainting && NULL != m_pBitmap) {
	    GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
        HFCL_DELETE(m_pBitmap);
        m_pBitmap = NULL;
    }

}

bool ThreePatchImage::makeSubDrawInfo()
{
	if (NULL == m_pBitmap) {
		LOGERROR("bmp is null\n");
		return false;
	}

	int nr_line = m_pBitmap->bmHeight;
	int nr_col = m_pBitmap->bmWidth;

    if (m_typeHoriz) {
        int edge_len = nr_col / THREEH_PATCH_COUNT;
        m_di[0].idx_line = 0;
        m_di[0].idx_col = 0;
        m_di[0].line_count = nr_line;
        m_di[0].col_count = edge_len;

        m_di[1].idx_line = 0;
        m_di[1].idx_col = edge_len/* - 1*/;
        m_di[1].line_count = nr_line;
        m_di[1].col_count = nr_col - 2 * edge_len;

        m_di[2].idx_line = 0;
        m_di[2].idx_col = nr_col - edge_len/* - 1*/;
        m_di[2].line_count = nr_line;
        m_di[2].col_count = edge_len;
    } else {
        int edge_len = nr_line / THREEH_PATCH_COUNT;
        m_di[0].idx_line = 0;
        m_di[0].idx_col = 0;
        m_di[0].line_count = edge_len;
        m_di[0].col_count = nr_col;

        m_di[1].idx_line = edge_len/* - 1*/;
        m_di[1].idx_col = 0;
        m_di[1].line_count = nr_line - 2 * edge_len;
        m_di[1].col_count = nr_col;

        m_di[2].idx_line = nr_line - edge_len/* - 1*/;
        m_di[2].idx_col = 0;
        m_di[2].line_count = edge_len;
        m_di[2].col_count = nr_col;
    }

	return true;
}

bool ThreePatchImage::initSubHRect(const IntRect* pRc, ImageFormat format)
{
    IntRect rc;
    
	if (NULL == m_pBitmap || NULL == pRc) {
        LOGERROR("bmp is null or rect is null\n");
        return false;
    }

    int i_height = m_pBitmap->bmHeight;
    int edge_len = m_pBitmap->bmWidth / THREEH_PATCH_COUNT;

    switch (format.valign) {
        case VALIGN_TOP :
            rc.setRect(pRc->left(), pRc->top(), pRc->right(), pRc->top() + i_height);
            break;

        case VALIGN_BOTTOM :
            rc.setRect(pRc->left(), pRc->bottom() - i_height, pRc->right(), pRc->bottom());
            break;

        case VALIGN_MIDDLE :
        default:
            rc.setRect(pRc->left(), pRc->top() + (pRc->height() - i_height) / 2, pRc->right(),
                pRc->top() + (pRc->height() + i_height) / 2);
            break;
    }

    m_subRc[0].m_left  	= rc.m_left;
    m_subRc[0].m_right 	= m_subRc[0].m_left + edge_len;
    m_subRc[0].m_top    = rc.m_top;
    m_subRc[0].m_bottom = rc.m_bottom;

    m_subRc[1].m_left 	= m_subRc[0].m_right;
    m_subRc[1].m_right 	= rc.right() - edge_len;
    m_subRc[1].m_top 	= m_subRc[0].m_top;
    m_subRc[1].m_bottom = m_subRc[0].m_bottom;

    m_subRc[2].m_left 	= m_subRc[1].m_right;
    m_subRc[2].m_right 	= rc.m_right;
    m_subRc[2].m_top 	= m_subRc[0].m_top;
    m_subRc[2].m_bottom = m_subRc[0].m_bottom;

	return true;
}

bool ThreePatchImage::initSubVRect(const IntRect* pRc, ImageFormat format)
{
    IntRect rc;
    
	if (NULL == m_pBitmap || NULL == pRc) {
        LOGERROR("bmp is null or rect is null\n");
        return false;
    }

    int i_width  = m_pBitmap->bmWidth;
    int edge_len = m_pBitmap->bmHeight / THREEH_PATCH_COUNT;

	switch (format.align) {
		case ALIGN_LEFT :
			rc.setRect(pRc->left(), pRc->top(), pRc->left() + i_width, pRc->bottom());
			break;

		case ALIGN_RIGHT :
			rc.setRect(pRc->right() - i_width, pRc->top(), pRc->right(), pRc->bottom());
			break;

		case ALIGN_CENTER :
		default:
			rc.setRect(pRc->left() + (pRc->width() - i_width) / 2, pRc->top(),
					pRc->left() + (pRc->width() + i_width) / 2, pRc->bottom());
			break;
    }

    m_subRc[0].m_left 	= rc.m_left;
    m_subRc[0].m_right 	= rc.m_right;
    m_subRc[0].m_top 	= rc.m_top;
    m_subRc[0].m_bottom = rc.m_top + edge_len;

    m_subRc[1].m_left 	= m_subRc[0].m_left;
    m_subRc[1].m_right 	= m_subRc[0].m_right;
    m_subRc[1].m_top 	= m_subRc[0].m_bottom;
    m_subRc[1].m_bottom = rc.m_bottom - edge_len;

    m_subRc[2].m_left 	= m_subRc[1].m_left;
    m_subRc[2].m_right 	= m_subRc[1].m_right;
    m_subRc[2].m_top 	= m_subRc[1].m_bottom;
    m_subRc[2].m_bottom = rc.m_bottom;

    return true;
}

bool ThreePatchImage::initSubRect(const IntRect* pRc, ImageFormat format)
{
    if (NULL == m_pBitmap || NULL == pRc) {
        LOGERROR("initSubRect: bmp is null or rect is null\n");
        return false;
    }

    if (m_typeHoriz) {
        if (pRc->width() < (int)(2 * m_pBitmap->bmWidth / THREEH_PATCH_COUNT)) {
            LOGERROR("this rc is not right!\n");
            return false;
        }
        return initSubHRect(pRc, format);
    } else {
        if (pRc->height() < (int)(2 * m_pBitmap->bmHeight / THREEH_PATCH_COUNT)) {
            LOGERROR("this rc is not right!\n");
            return false;
        }
        return initSubVRect(pRc, format);
    }
}

void ThreePatchImage::initSubBmp()
{
	for (int i = 0; i < THREEH_PATCH_COUNT; i++) {
        memset(&m_subBmp[i], 0x00, sizeof(Bitmap));
	}
}

bool ThreePatchImage::setSubBmp()
{
	if (NULL == m_pBitmap || !makeSubDrawInfo()) {
		LOGERROR("bmp is NULL or makeSubDrawInfo error\n");
		return false;
	}

	Uint8* ams = NULL;
	Bitmap* pSubBmp = NULL;

	for (int index = 0; index < THREEH_PATCH_COUNT; index++) {
		pSubBmp = &m_subBmp[index];
		memcpy(pSubBmp, m_pBitmap, sizeof(Bitmap));

		pSubBmp->bmBits = m_pBitmap->bmBits + m_di[index].idx_line * m_pBitmap->bmPitch
			+ m_di[index].idx_col * m_pBitmap->bmBytesPerPixel;

		if (pSubBmp->bmType & HFCL_BMP_TYPE_ALPHA_MASK) {
			int alphaPitch = ((m_di[index].col_count + 3) & ~3);
            int size = m_di[index].line_count * alphaPitch;
			
			pSubBmp->bmAlphaMask = HFCL_NEW_ARR(Uint8, size);
            memset(pSubBmp->bmAlphaMask, 0x00, size);
			ams = m_pBitmap->bmAlphaMask + m_di[index].idx_line * m_pBitmap->bmAlphaPitch + m_di[index].idx_col;
			
			for (unsigned int i = 0; i < m_di[index].line_count; i++) {
				memcpy(pSubBmp->bmAlphaMask + alphaPitch * i, ams, alphaPitch);
				ams += m_pBitmap->bmAlphaPitch;
			}
			pSubBmp->bmAlphaPitch = alphaPitch;
		} else {
			pSubBmp->bmAlphaPitch = 0;
			pSubBmp->bmAlphaMask = NULL;
		}

		pSubBmp->bmWidth = m_di[index].col_count;
		pSubBmp->bmHeight = m_di[index].line_count;
		pSubBmp->bmType |= HFCL_BMP_TYPE_ALPHACHANNEL | HFCL_BMP_TYPE_COLORKEY;
		pSubBmp->bmAlpha = 0xFF;
	}

	return true;
}

void ThreePatchImage::clean()
{
	Bitmap *pSubBmp = NULL;
	for (int index = 0; index < THREEH_PATCH_COUNT; index++) {
		pSubBmp = &m_subBmp[index];
		if (pSubBmp->bmType & HFCL_BMP_TYPE_ALPHA_MASK) {
			if (NULL != pSubBmp->bmAlphaMask) {
                HFCL_DELETE_ARR(pSubBmp->bmAlphaMask);
				pSubBmp->bmAlphaMask = NULL;
				pSubBmp->bmAlphaPitch = 0;
			}
		}
		pSubBmp->bmBits = NULL;
	}
}

bool ThreePatchImage::setImage(const char* image_file)
{
	int len = strlen(image_file);

	if (len > 4 && (strcasecmp(image_file + len-4, ".png") == 0 
				|| strcasecmp(image_file + len-4, ".gif") == 0))
	{
		if (len >7 && strncasecmp(image_file + len - 7, ".3h", 3) == 0)
		{
			m_typeHoriz = true;
		} else if (len >7 && strncasecmp(image_file + len - 7, ".3v", 3) == 0)
		{
			m_typeHoriz = false;
		} else {
			return false;
		}
		
		clean();
		return Image::setImage(image_file);
	}
	return false;

}

} // namespace hfcl {
