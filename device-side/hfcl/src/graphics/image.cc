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

#include <string.h>

#include "image.h"
#include "ninepatchimage.h"
#include "threepatchimage.h"
#include "resloader.h"

#define IMAGE_TYPE_NORMAL    0
#define IMAGE_TYPE_9_PATCH   1
#define IMAGE_TYPE_3H_PATCH  2
#define IMAGE_TYPE_3V_PATCH  3
#define IMAGE_TYPE_JPEG      4

NAMESPACE_BEGIN

Image::Image()
    : RefCount(0)
    , m_bLoadOnPainting(true)
    , m_pBitmap(NULL)
    ,m_replaceColor(0)
{
	//TODO
}

Image::~Image()
{
    if (m_pBitmap != NULL)
    {
	    GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
	    NGUX_DELETE(m_pBitmap);
	    m_pBitmap = NULL;
    }
}

static NGInt get_image_type(NGCPStr file)
{
    NGInt len = strlen(file);
    
    if (len > 4 && (strcasecmp(file + len - 4, ".png") == 0
                || strcasecmp(file + len - 4, ".gif") == 0)) {
        if (len >6 && strncasecmp(file + len - 6, ".9", 2) == 0)
            return IMAGE_TYPE_9_PATCH;
        else if (len >7 && strncasecmp(file + len - 7, ".3h", 3) == 0)
            return IMAGE_TYPE_3H_PATCH;
        else if (len > 7 && strncasecmp(file + len - 7, ".3v", 3) == 0)
            return IMAGE_TYPE_3V_PATCH;
    }

    return IMAGE_TYPE_NORMAL;
}

Image* Image::loadImage(NGCPStr file_path)
{
    Image* pImg = NULL;

    if (!file_path) {
        return NULL;
    }

	switch (get_image_type(file_path)) {
        case IMAGE_TYPE_9_PATCH:
            pImg = NGUX_NEW_EX(NinePatchImage, ());
            break;
        case IMAGE_TYPE_3H_PATCH:
            pImg = NGUX_NEW_EX(ThreePatchImage, ());
            break;
        case IMAGE_TYPE_3V_PATCH:
            pImg = NGUX_NEW_EX(ThreePatchImage, (false));
            break;
        case IMAGE_TYPE_NORMAL:
            pImg = NGUX_NEW_EX(Image, ());
			break;
    }

	pImg->setImage(file_path);

    return pImg;
}

NGBool Image::setImage(NGCPStr file_path)
{
    if (file_path) {
		m_filePath = file_path;
		if (m_bLoadOnPainting) {
			setImageBitmap(NULL);
		} else {
			setImageBitmap(ResLoader::getInstance()->getBitmap(file_path));
		}
    } else {
        setImageBitmap(NULL);
    }
    return true;
}

NGBool Image::setImageBitmap(Bitmap* pbmp)
{
    if (m_pBitmap != pbmp)
    {
	    if (m_pBitmap != NULL) {
			GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
			NGUX_DELETE(m_pBitmap);
		}
	    m_pBitmap = pbmp;
    }
    return true;
}

NGInt Image::width(void) 
{ 
	NGInt w = 0;
	
	if (m_bLoadOnPainting) {
		setImageBitmap(ResLoader::getInstance()->getBitmap(m_filePath.c_str()));
	}

	w = m_pBitmap ? m_pBitmap->bmWidth : 0;
		
	if (m_bLoadOnPainting) {
		if(m_pBitmap)
		{
		    GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
	        NGUX_DELETE(m_pBitmap);
	        m_pBitmap = NULL;
		}
	} 

	return w;
}

NGInt Image::height(void) 
{ 
	NGInt h = 0;
	
	if (m_bLoadOnPainting) {
		setImageBitmap(ResLoader::getInstance()->getBitmap(m_filePath.c_str()));
	}

	h = m_pBitmap ? m_pBitmap->bmHeight : 0;
		
	if (m_bLoadOnPainting) {
		if(m_pBitmap)
		{
		    GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
	        NGUX_DELETE(m_pBitmap);
	        m_pBitmap = NULL;
		}
	} 

	return h;
}

NGBool Image::setReplaceColor(const DWORD color)
{
    m_replaceColor = color;
    return true;
}

void Image::calc_pos_align(NGInt *px, NGInt *py,
        const IntRect *rc, NGInt imgw, NGInt imgh, ImageFormat format)
{
    if (format.align == ALIGN_CENTER)
        *px = (rc->left() + rc->right() - imgw)/2;
    else if (format.align == ALIGN_RIGHT)
        *px = (rc->right() - imgw);
    else
        *px = rc->left();

    if (format.valign == VALIGN_MIDDLE)
        *py = (rc->top() + rc->bottom() - imgh)/2;
    else if (format.valign == VALIGN_BOTTOM)
        *py = (rc->bottom() - imgh);
    else
        *py = rc->top();
}

void Image::paint(GraphicsContext* context, const IntRect& rc, ImageFormat format, NGInt xo, NGInt yo)
{
    NGInt x = 0, y = 0, w = 0, h = 0;
    
	if (m_bLoadOnPainting) {
		setImageBitmap(ResLoader::getInstance()->getBitmap(m_filePath.c_str()));
	}
	
    if (NULL == m_pBitmap) {
        return;	
    }
	context->setReplaceColor(0);
    if(m_replaceColor){
		context->setReplaceColor(m_replaceColor);
    }
   
    switch (format.drawMode) {
        case DRAWMODE_SCALED: //IMAGE_DRAWMODE_SCALED :
		{    
			context->fillBoxWithBitmap(rc.left(), rc.top(),
                    rc.width(), rc.height(), m_pBitmap);
            break;
		}
        case DRAWMODE_TILED: //IMAGE_DRAWMODE_TILED :
		{
			if (context->getBitmapSize(m_pBitmap, &w, &h)) {
                for (x = rc.left(); x < rc.right(); x += w) {
                    for (y = rc.top(); y < rc.bottom(); y += h)
                        context->fillBoxWithBitmap(x, y, w, h, m_pBitmap);
                }
            }
            break;
        }
        case DRAWMODE_ADAPTED: 
		{
            if (context->getBitmapSize(m_pBitmap, &w, &h)) {
                if (w == 0 || h == 0)
                    return;
                NGInt xCheck = rc.width() - w;
                NGInt yCHeck = rc.height() - h;

                if (xCheck < yCHeck)
                    context->fillBoxWithBitmap(rc.left(),
                            (rc.top() + (rc.height() - ((NGReal)h/(NGReal)w)*rc.width())/2),
                            rc.width(), (((NGReal)h/(NGReal)w)*rc.width()), m_pBitmap);
                else
                    context->fillBoxWithBitmap((rc.left() + (rc.width() - ((NGReal)w/(NGReal)h)*rc.height())/2),
                            rc.top(), (((NGReal)w/(NGReal)h)*rc.height()), rc.height(), m_pBitmap);
            }
            break;
        }

        case DRAWMODE_PARTBOX:
		{
			if(rc.width() > (int)m_pBitmap->bmWidth){
				if (format.align == ALIGN_CENTER)
        	x = (rc.left() + rc.right() - m_pBitmap->bmWidth)/2;
		    else if (format.align == ALIGN_RIGHT)
		      x = (rc.right() - m_pBitmap->bmWidth);
		    else
		      x = rc.left();
				
			}else x = rc.left();
			
			if(rc.height() > (int)m_pBitmap->bmHeight){
			  if (format.valign == VALIGN_MIDDLE)
        	y = (rc.top() + rc.bottom() - m_pBitmap->bmHeight)/2;
		    else if (format.valign == VALIGN_BOTTOM)
		        y = (rc.bottom() - m_pBitmap->bmHeight);
		    else
		       y = rc.top();
		  }else y = rc.top();
		  	
			w = rc.width() > (int)m_pBitmap->bmWidth ? m_pBitmap->bmWidth : rc.width();
			h = rc.height() > (int)m_pBitmap->bmHeight ? m_pBitmap->bmHeight : rc.height();
            context->fillBoxWithBitmapPart(x, y, w, h, m_pBitmap, xo, yo);
            break;
		}
		case DRAWMODE_ROTATIONANGLE:
		{
		if (context->getBitmapSize(m_pBitmap, &w, &h)) {
			calc_pos_align(&x, &y, &rc, w, h, format);
            		context->drawRotateBitmap(m_pBitmap,x,y, format.rotationAngle*64);
			}
            break;
		}

		default: 
		{
            if (context->getBitmapSize(m_pBitmap, &w, &h)) {
                calc_pos_align(&x, &y, &rc, w, h, format);
                context->fillBoxWithBitmap(x, y,
                        w > rc.width() ? rc.width() : w, h > rc.height() ? rc.height() : h, m_pBitmap);
            }
            break;
        }    //end of switch m_drawmode : case default.
    }    //end of switch m_drawmode.

	if (m_bLoadOnPainting) {
	    GraphicsContext::screenGraphics()->unloadBitmap(m_pBitmap);
        NGUX_DELETE(m_pBitmap);
        m_pBitmap = NULL;
    }
}

NAMESPACE_END

