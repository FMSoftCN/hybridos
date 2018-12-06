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

#ifndef HFCL_RESOURCE_RESLOADER_H_
#define HFCL_RESOURCE_RESLOADER_H_

#include "graphics/graphicscontext.h"
#include "graphics/font.h"
#include "graphics/image.h"
#include "graphics/gifanimate.h"
#include "view/animateimageview.h"
#include "resource/restypes.h"

namespace hfcl {

class ResLoader {
public:
    Logfont* getFont(const char* fontname);
    bool releaseFont(const char* fontname);

    Image* getImage(const char* filepath);
    GifAnimate* getGifAnimate(const char* filepath);
    bool releaseImage(const char* filepath);

    Bitmap* getBitmap(const char* filepath);
    bool releaseBitmap(const char* filepath);
    bool releaseBitmap(Bitmap* pbmp);

    void* loadData (const char* filepath, bool *isincore);
    const HFCL_INCORE_RES *getIncoreData (const char* resname);

    void registerIncoreRes (const char* resname,
            const HFCL_INCORE_RES *incores, int count);

#if 0 /* VM: deprecated code */
    struct InnerImage {
        INNER_RES_INFO * resInfo;
        Image * image;
        InnerImage() : resInfo(NULL), image(NULL) { }
        Image * get();
        BitmapFrameArray* getBitmapFrame();
        const char ** getBitmapdata();
        unsigned int getBitmapdatasize();
        INNER_RES_INFO* getResInfo(void);
    };

    void registerInnerRes(int res_type, INNER_RES_INFO *resources, int count);

    MAPCLASS(string, InnerImage, InnerImageResMap);
    InnerImageResMap m_innerImageRes;
#endif /* VM: deprecated code */

    static ResLoader* getInstance(void);

    ~ResLoader() { }

private:
    ResLoader();

    MAPCLASSKEY(string, Font*, FontResMap);
    MAPCLASSKEY(string, Image*, ImageResMap);
    MAPCLASSKEY(string, Bitmap*, BitmapResMap);
    MAPCLASSKEY(HTData, HTData, NameIncoresMap);
    PAIR(string, Font*, FontResPair);
    PAIR(string, Image*, ImageResPair);
    PAIR(string, Bitmap*, BitmapResPair);
    PAIR(HTData, HTData, NameIncoresPair);

    FontResMap   m_fontRes;
    ImageResMap  m_imageRes;
    BitmapResMap m_bitmapRes;

    NameIncoresMap m_nameIncores;

    static ResLoader* m_singleton;
};

INNER_RES_INFO* GetImageResInfo(const char *filepath);

} // namespace hfcl

#endif /* HFCL_RESOURCE_RESLOADER_H_ */

