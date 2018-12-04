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

#include "imageres.h"
#include "resource/resloader.h"


namespace hfcl {

ImageRes::ImageRes(const char* imagefile)
    : m_imagefile(imagefile)
    , m_image(NULL)
    , m_gifAnimate(NULL)
{
}

ImageRes::ImageRes()
    : m_imagefile(NULL)
    , m_image(NULL)
    , m_gifAnimate(NULL)
{
}

//ImageResVec's size method would cause a disconstruct called for this,
//I don't want it call this, so, we don't release any resource, or call 
//method clean
void ImageRes::clean()
{
    if (m_gifAnimate) {
        m_gifAnimate->unref();
    }
    else if (m_image && m_imagefile) {
        m_image->unref();
        ResLoader::getInstance()->releaseImage(m_imagefile);
    }

    m_imagefile = NULL;
    m_image = NULL;
    m_gifAnimate = NULL;
}

GifAnimate* ImageRes::getGifAnimate(const char* resPath)
{
    if (NULL == m_gifAnimate) {
        m_gifAnimate = ResLoader::getInstance()->getGifAnimate(m_imagefile/*GetResRealPath(resPath, m_imagefile).c_str()*/);
        //if (m_gifAnimate)
        //    m_gifAnimate->safeRef();
        if (!m_gifAnimate) {
            _DBG_PRINTF ("ImageRes::getGifAnimate: Failed to load GIF animation: %s", resPath);
        }
    }

    return m_gifAnimate;
}

Image* ImageRes::get(const char* resPath)
{
    if (NULL == m_image) {
        m_image = ResLoader::getInstance()->getImage(m_imagefile);
        if (m_image) {
            m_image->safeRef();
        }
        else {
            _DBG_PRINTF ("ImageRes::get: Failed to load image: %s", resPath);
        }
    }

    return m_image;
}

} // namespace hfcl

