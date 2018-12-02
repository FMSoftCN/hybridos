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

#ifndef __RESLOADER_H
#define __RESLOADER_H

#include "graphicscontext.h"
#include "font.h"
#include "image.h"
#include "gifanimate.h"
#include "animateimageview.h"
#include "restypes.h"

NAMESPACE_BEGIN

class ResLoader {
	// Operations
	public:

		Logfont* getFont(const char* fontname/*, Style* style*/);
		bool releaseFont(const char* fontname);

		Image* getImage(const char* filepath);
        GifAnimate* getGifAnimate(const char* filepath);
		bool releaseImage(const char* filepath);

		Bitmap* getBitmap(const char* filepath);
		bool releaseBitmap(const char* filepath);
        bool releaseBitmap(Bitmap* pbmp);

		void registerInnerRes(int res_type, INNER_RES_INFO * resources, int count);
		
		static ResLoader* getInstance(void);

		~ResLoader() { }

	private:
		ResLoader();

		struct InnerImage {
			INNER_RES_INFO * resInfo;
			Image * image;
			InnerImage() : resInfo(NULL), image(NULL) { }
			Image * get();
			BitmapFrameArray* getBitmapFrame();
			NGCPStr* getBitmapdata();
			NGUInt getBitmapdatasize();
			INNER_RES_INFO* getResInfo(void);
		};

		MAPCLASSKEY(string, Font*, FontResMap);
		MAPCLASSKEY(string, Image*, ImageResMap);
		MAPCLASSKEY(string, Bitmap*, BitmapResMap);
		MAPCLASS(string, InnerImage, InnerImageResMap);
		PAIR(string, Font*, FontResPair);
		PAIR(string, Image*, ImageResPair);
        PAIR(string, Bitmap*, BitmapResPair);

		FontResMap   m_fontRes;
		ImageResMap  m_imageRes;
		InnerImageResMap m_innerImageRes;
		BitmapResMap m_bitmapRes;

		static ResLoader* m_singleton;
};

INNER_RES_INFO* GetImageResInfo(const char *filepath);


NAMESPACE_END

#endif

