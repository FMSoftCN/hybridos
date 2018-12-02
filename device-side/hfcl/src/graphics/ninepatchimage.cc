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

#include "ninepatchimage.h"
#include "resloader.h"


namespace hfcl {

#define EDGE_LENGTH         8

NinePatchImage::NinePatchImage()
{
	initSubBmp();
}

NinePatchImage::~NinePatchImage()
{
	clean();
}

void NinePatchImage::paint(GraphicsContext* context, const IntRect& rc,
		ImageFormat format, int xo, int yo)
{
	if (m_bLoadOnPainting) {
		setImageBitmap(ResLoader::getInstance()->getBitmap(m_filePath.c_str()));
	}

	if (NULL == m_pBitmap) {
		LOGERROR("bmp is null\n");
		return;
	}

	if (!initSubRect(&rc)) {
		LOGERROR("initSubRect\n");
		return;
	}

	for (int i = 0; i < PATCH_COUNT; i++) {
		context->fillBoxWithBitmap(m_subRc[i].left(), m_subRc[i].top(),
				m_subRc[i].width(), m_subRc[i].height(), &m_subBmp[i]);
	}
	
	if (m_bLoadOnPainting && NULL != m_pBitmap) {
	    GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
        HFCL_DELETE(m_pBitmap);
        m_pBitmap = NULL;
    }

}

bool NinePatchImage::makeSubDrawInfo()
{
	if (NULL == m_pBitmap) {
		LOGERROR("bmp is null\n");
		return false;
	}

	int nr_line = m_pBitmap->bmHeight;
	int nr_col = m_pBitmap->bmWidth;
	if (nr_line < 2 * EDGE_LENGTH || nr_col < 2 * EDGE_LENGTH) {
		LOGERROR("line =%d, col = %d, this bmp is too small!\n", nr_line, nr_col);
		return false;
	}

//	case 0:
	m_di[0].idx_line = 0;
	m_di[0].idx_col = 0;
	m_di[0].line_count = EDGE_LENGTH;
	m_di[0].col_count = EDGE_LENGTH;

//	case 1:
	m_di[1].idx_line = 0;
	m_di[1].idx_col = EDGE_LENGTH - 1;
	m_di[1].line_count = EDGE_LENGTH;
	m_di[1].col_count = nr_col - 2 * EDGE_LENGTH;

//	case 2:
	m_di[2].idx_line = 0;
	m_di[2].idx_col = nr_col - EDGE_LENGTH - 1;
	m_di[2].line_count = EDGE_LENGTH;
	m_di[2].col_count = EDGE_LENGTH;

//	case 3:
	m_di[3].idx_line = EDGE_LENGTH - 1;
	m_di[3].idx_col = 0;
	m_di[3].line_count = nr_line - 2 * EDGE_LENGTH;
	m_di[3].col_count = EDGE_LENGTH;

//	case 4:
	m_di[4].idx_line = EDGE_LENGTH - 1;
	m_di[4].idx_col = EDGE_LENGTH - 1;
	m_di[4].line_count = nr_line - 2 * EDGE_LENGTH;
	m_di[4].col_count = nr_col - 2 * EDGE_LENGTH;

//	case 5:
	m_di[5].idx_line = EDGE_LENGTH - 1;
	m_di[5].idx_col = nr_col - EDGE_LENGTH - 1;
	m_di[5].line_count = nr_line - 2 * EDGE_LENGTH;
	m_di[5].col_count = EDGE_LENGTH;

//	case 6:
	m_di[6].idx_line = nr_line - EDGE_LENGTH - 1;
	m_di[6].idx_col = 0;
	m_di[6].line_count = EDGE_LENGTH;
	m_di[6].col_count = EDGE_LENGTH;

//	case 7:
	m_di[7].idx_line = nr_line - EDGE_LENGTH - 1;
	m_di[7].idx_col = EDGE_LENGTH - 1;
	m_di[7].line_count = EDGE_LENGTH;
	m_di[7].col_count = nr_col - 2 * EDGE_LENGTH;

//	case 8:
	m_di[8].idx_line = nr_line - EDGE_LENGTH - 1;
	m_di[8].idx_col = nr_col - EDGE_LENGTH - 1;
	m_di[8].line_count = EDGE_LENGTH;
	m_di[8].col_count = EDGE_LENGTH;

	return true;
}

bool NinePatchImage::initSubRect(const IntRect* pRc)
{
	if (NULL == pRc || pRc->width() < 2 * EDGE_LENGTH || pRc->height() < 2 * EDGE_LENGTH) {
		LOGERROR("this rc is not right!\n");
		return false;
	}

	m_subRc[0].m_left = pRc->m_left;
	m_subRc[0].m_right = m_subRc[0].m_left + EDGE_LENGTH;
	m_subRc[0].m_top = pRc->m_top;
	m_subRc[0].m_bottom = m_subRc[0].m_top + EDGE_LENGTH;

	m_subRc[1].m_left = m_subRc[0].m_right;
	m_subRc[1].m_right = pRc->right() - EDGE_LENGTH;
	m_subRc[1].m_top = m_subRc[0].m_top;
	m_subRc[1].m_bottom = m_subRc[0].m_bottom;

	m_subRc[2].m_left = m_subRc[1].m_right;
	m_subRc[2].m_right = pRc->m_right;
	m_subRc[2].m_top = m_subRc[0].m_top;
	m_subRc[2].m_bottom = m_subRc[0].m_bottom;

	m_subRc[3].m_left = m_subRc[0].m_left;
	m_subRc[3].m_right = m_subRc[0].m_right;
	m_subRc[3].m_top = m_subRc[0].m_bottom;
	m_subRc[3].m_bottom = pRc->bottom() - EDGE_LENGTH;

	m_subRc[4].m_left = m_subRc[1].m_left;
	m_subRc[4].m_right = m_subRc[1].m_right;
	m_subRc[4].m_top = m_subRc[3].m_top;
	m_subRc[4].m_bottom = m_subRc[3].m_bottom;

	m_subRc[5].m_left = m_subRc[2].m_left;
	m_subRc[5].m_right = m_subRc[2].m_right;
	m_subRc[5].m_top = m_subRc[3].m_top;
	m_subRc[5].m_bottom = m_subRc[3].m_bottom;

	m_subRc[6].m_left = m_subRc[0].m_left;
	m_subRc[6].m_right = m_subRc[0].m_right;
	m_subRc[6].m_top = m_subRc[3].m_bottom;
	m_subRc[6].m_bottom = pRc->m_bottom;

	m_subRc[7].m_left = m_subRc[1].m_left;
	m_subRc[7].m_right = m_subRc[1].m_right;
	m_subRc[7].m_top = m_subRc[6].m_top;
	m_subRc[7].m_bottom = m_subRc[6].m_bottom;

	m_subRc[8].m_left = m_subRc[2].m_left;
	m_subRc[8].m_right = m_subRc[2].m_right;
	m_subRc[8].m_top = m_subRc[6].m_top;
	m_subRc[8].m_bottom = m_subRc[6].m_bottom;

	return true;
}

void NinePatchImage::initSubBmp()
{
	for (int i = 0; i < PATCH_COUNT; i++) {
        memset(&m_subBmp[i], 0x00, sizeof(Bitmap));
	}
}

bool NinePatchImage::setSubBmp()
{
	if (NULL == m_pBitmap || !makeSubDrawInfo()) {
		LOGERROR("bmp is NULL or makeSubDrawInfo error\n");
		return false;
	}

	Uint8* ams = NULL;
	Bitmap* pSubBmp = NULL;

	for (int index = 0; index < PATCH_COUNT; index++) {
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
				memcpy(pSubBmp->bmAlphaMask + alphaPitch * i, ams, m_di[index].col_count);
				ams += m_pBitmap->bmAlphaPitch;
			}
			pSubBmp->bmAlphaPitch = alphaPitch;
		} else {
			pSubBmp->bmAlphaPitch = 0;
			pSubBmp->bmAlphaMask  = NULL;
		}

		pSubBmp->bmWidth = m_di[index].col_count;
		pSubBmp->bmHeight = m_di[index].line_count;
		pSubBmp->bmType |= HFCL_BMP_TYPE_ALPHACHANNEL | HFCL_BMP_TYPE_COLORKEY;
		pSubBmp->bmAlpha = 0xFF; 
	}

	return true;
}

void NinePatchImage::clean()
{
	Bitmap *pSubBmp = NULL;
	for (int index = 0; index < PATCH_COUNT; index++) {
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

bool NinePatchImage::setImage(const char *image_file)
{
	clean();

	if (!Image::setImage(image_file)) {
		LOGERROR("Image::setImage\n");
		return false;
	}
	return true;
}

} // namespace hfcl {
