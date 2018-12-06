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

#include "resource/resloader.h"

#include "resource/respkgmanager.h"
#include "resource/restypes.h"

namespace hfcl {

ResLoader* ResLoader::m_singleton = NULL;

ResLoader::ResLoader()
{
}

ResLoader* ResLoader::getInstance()
{
    if (NULL == ResLoader::m_singleton)
        ResLoader::m_singleton = HFCL_NEW_EX(ResLoader, ());

    return ResLoader::m_singleton;
}

Logfont* ResLoader::getFont(const char* fontname)
{
    string fontName(fontname);
    FontResMap::iterator it = m_fontRes.find(fontName);

    if (it == m_fontRes.end()) {
        Font* font = Font::createFont(fontname);
        if(!font)
            return NULL;
        m_fontRes[fontname] = font;
        return font->getLogfont();
    }

    return it->second->getLogfont();
}

bool ResLoader::releaseFont(const char* fontname)
{
    string st(fontname);
    FontResMap::iterator it = m_fontRes.find(st);

    if (it == m_fontRes.end())
        return true;

    it->second->unref();

    if (0 == it->second->getRefCnt()) {
        m_fontRes.erase(it);
    }

    return true;
}

GifAnimate* ResLoader::getGifAnimate(const char* filepath)
{
    GifAnimate* gif = NULL;
    const HFCL_INCORE_RES* info = NULL;

    // GifAnimate : avoid mem leak TODO
    gif = HFCL_NEW_EX(GifAnimate, ());
    info = getIncoreData (filepath);

    if (info != NULL) {
        gif->createGifAnimateFromMem((const char*)info->data, info->size);
    }
    else {
        // 2. file system
        gif->createGifAnimateFromFile(filepath);
    }

    gif->safeRef();
    return gif;
}

Image* ResLoader::getImage(const char* filepath)
{
    return Image::loadImage(filepath);
}

bool ResLoader::releaseImage(const char* filepath)
{
    ImageResMap::iterator it = m_imageRes.find(filepath);
    if (it != m_imageRes.end() && it->second != NULL){
        it->second->safeUnref();
        m_imageRes.erase(it);
        return true;
    }

    return false;
}

Bitmap* ResLoader::getBitmap(const char* filename)
{
    Bitmap *pbmp = NULL;
    const HFCL_INCORE_RES* info = NULL;

    info = getIncoreData (filename);
    if (NULL != (pbmp = HFCL_NEW_EX(Bitmap, ()))) {
        if (NULL != info) {
            const char * externs_name = NULL;

            externs_name = strrchr(filename, '.');
            if (!externs_name) {
                HFCL_DELETE(pbmp);
                return NULL;
            }

            externs_name ++;

            if (GraphicsContext::screenGraphics()->loadBitmap(&pbmp,
                    info->data, info->size, externs_name)) {
                _DBG_PRINTF ("ResLoader::getBitmap: Failed to load image from memory: %s", filename);
                HFCL_DELETE(pbmp);
                return NULL;
            }
        }
        else {
            if (GraphicsContext::screenGraphics()->loadBitmap(pbmp, filename)) {
                _DBG_PRINTF ("ResLoader::getBitmap: Failed to load image: %s", filename);
                HFCL_DELETE(pbmp);
                return NULL;
            }
        }
    }

    return pbmp;
}

bool ResLoader::releaseBitmap(const char* filepath)
{
    BitmapResMap::iterator it = m_bitmapRes.find(filepath);

    if (it == m_bitmapRes.end()){
        return false;
    }

    GraphicsContext::screenGraphics()->unloadBitmap(it->second);
    HFCL_DELETE(it->second);
    m_bitmapRes.erase(it);

    return true;
}

bool ResLoader::releaseBitmap(Bitmap* pbmp)
{
    BitmapResMap::iterator it = m_bitmapRes.begin();
    while (it != m_bitmapRes.end()) {
        if (it->second == pbmp){
            GraphicsContext::screenGraphics()->unloadBitmap(it->second);
            HFCL_DELETE(it->second);
            m_bitmapRes.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

void* ResLoader::loadData (const char* filepath, bool *isincore)
{
    NameIncoresMap::iterator it = m_nameIncores.find ((HTData)filepath);
    if (it == m_nameIncores.end()) {
        return NULL;
    }

    const HFCL_INCORE_RES* incores = (const HFCL_INCORE_RES*)it->second;
    if (incores) {
        *isincore = true;
        return incores->data;
    }

    /* try to load from file */
    *isincore = false;

    MG_RWops* src;
    src = MGUI_RWFromFile (filepath, "r");
    if (src == NULL) {
        _ERR_PRINTF ("ResLoader::loadData: failed to open res file: %s\n",
                filepath);
        return NULL;
    }

    char* data = NULL;
    int nr_read;
    int size = MGUI_RWseek (src, 0, SEEK_END);
    if (size <= 0)
        goto error;

    data = (char*)malloc (size);
    if (data == NULL)
        goto error;

    MGUI_RWseek (src, 0, SEEK_SET);
    nr_read = MGUI_RWread (src, data, 1, size);
    if (nr_read < size) {
        free (data);
        data = NULL;
    }

error:
    MGUI_FreeRW (src);
    return data;
}

const HFCL_INCORE_RES* ResLoader::getIncoreData (const char* resname)
{
    NameIncoresMap::iterator it = m_nameIncores.find ((HTData)resname);
    if (it == m_nameIncores.end()) {
        return NULL;
    }

    return (const HFCL_INCORE_RES*)it->second;
}

void ResLoader::registerIncoreRes (const char* resname,
        const HFCL_INCORE_RES *incores, int count)
{
    for (int i = 0; i < count; i++) {
        m_nameIncores [(HTData)resname] = (HTData)(incores + i);
    }
}

#if 0 /* VM: deprecated code */
extern INNER_RES_ARRAY g_ResArray[];
extern int g_ResCount;

INNER_RES_INFO* GetImageResInfo(const char *filepath)
{
    int i, j;
    int cont;
    INNER_RES_INFO* info = NULL;

    for (i = 0; i < g_ResCount; i++) {
        info = g_ResArray[i].pInner_res_info;
        cont = g_ResArray[i].number;
        for (j = 0; j < cont; j++) {
            if (info[j].res_name != NULL &&
                    strcmp(filepath, info[j].res_name) == 0) {
                return &info[j];
            }
        }
    }

    return NULL;
}

void ResLoader::registerInnerRes(int res_type, INNER_RES_INFO * resources,
        int count)
{
    if (res_type != R_TYPE_IMAGE || resources == NULL || count <= 0)
        return;

    for (int i=0; i < count; i++) {
        if (resources[i].res_name != NULL &&
                resources[i].data != NULL && resources[i].size >= 0) {
            InnerImage & inner = m_innerImageRes[resources[i].res_name];
            inner.resInfo = &resources[i];
        }
    }
}

bool ResLoader::releaseImage(const char* filepath)
{
    InnerImageResMap::iterator iit = m_innerImageRes.find(filepath);
    if (iit != m_innerImageRes.end() && iit->second.image != NULL){
        iit->second.image->safeUnref();
        iit->second.image = NULL;
        return true;
    }

    ImageResMap::iterator it = m_imageRes.find(filepath);
    if (it != m_imageRes.end() && it->second != NULL){
        it->second->safeUnref();
        m_imageRes.erase(it);
        return true;
    }

    return false;
}

BitmapFrameArray* ResLoader::InnerImage::getBitmapFrame()
{
    // BitmapFrame * !!!
    return (BitmapFrameArray *)resInfo->data;
}

Image* ResLoader::InnerImage::get()
{
    if(!image) {
        image = Image::loadImage(resInfo->res_name);
    }
    return image;
}

const char ** ResLoader::InnerImage::getBitmapdata()
{
    return (const char * *)resInfo->data;
}

unsigned int ResLoader::InnerImage::getBitmapdatasize()
{
    return (unsigned int)resInfo->size;
}

INNER_RES_INFO* ResLoader::InnerImage::getResInfo(void)
{
    return resInfo;
}

void RegisterInnerResource(int type, INNER_RES_INFO *resource, int count)
{
    ResLoader::getInstance()->registerInnerRes(type, resource, count);
}

#endif /* VM: deprecated code */

} // namespace hfcl

