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

#ifndef __RESPKGMANAGER_H
#define __RESPKGMANAGER_H

//typedef unsigned int ResID; 

//enum { RPKG_sys = 0 };

#include "restypes.h"
#include "graphicscontext.h"
#include "ngux_language_ids.h"
#include "ngux_string_ids.h"
#include "ngux_nl10n_string_ids.h"

NAMESPACE_BEGIN

class ResPackage;
class Menu;
class Image;
class GifAnimate;
class View;
class Style;
class Drawable;
class DrawableSet;
class DrawableSetGroup;
class ViewContext;
class EventListener;
class ContentProvider;
class ThemeRes;

class ResPkgManager
{
    private:
		LIST(ResPackage*, ResPkgList);
		
		static ResPkgManager* m_singleton;
		ResPkgList m_packageList;
		NGInt m_lang;

    private:
		~ResPkgManager();
		ResPkgManager():m_lang (-1) { }

    public:
		static ResPkgManager * getResPkgManager(); 
		void delResPkgManager(); 
		
		bool registerPackage(ResPackage *pkg);
		bool unregisterPackage(int id);
		bool unregisterPackage(const char *name);
		ResPackage *getPackage(int id); 
		ResPackage *getPackage(const char *name);
		
		inline int count(void) {return m_packageList.size();}
		
		bool setCurrentLang (NGInt langId);
		NGInt getCurrentLang (void) { return m_lang; }
};


/******      Global Interface      ******/

// FIXME assume syspackage has id 0
ResPackage* GetSystemPackage(void);
ResPkgManager * GetResPkgManager(void);
void ReleaseResPkgManager(void);

bool RegisterResPackage(ResPackage *resPkg);  
ResPackage * GetResPackageByName(const char* name);
ResPackage * GetResPackageById(int id);
bool UnregisterResPackageByName(const char* name);
bool UnregisterResPackageById(int id);

#define GetResPackage(id) 		 	GetResPackageById(id)
#define UnregisterResPackage(id) 	UnregisterResPackageById(id)

void * GetRes(ResID id);
Logfont * GetFontRes(ResID id/*, Style* style = NULL*/);
Style * GetStyleRes(ResID id);
Image * GetImageRes(ResID id);
Bitmap * GetBitmapRes(ResID id);
GifAnimate* GetGifAnimateRes(ResID id);
Drawable* GetDrawableRes(ResID id);
DrawableSet* GetDrawableSetRes(ResID id);
ThemeRes* GetThemeRes(ResID id);
DrawableSet* GetDrawableSetResFromTheme(NGInt theme_drset_id);
DrawableSetGroup* GetDrawableSetGroupRes(ResID id);

View * CreateViewFromRes(ResID id, View *parent, ViewContext *viewContext, ContentProvider* provider = NULL);
#define GetUITempl(id)   (CB_CREATE_VIEW)GetRes(id)

// add the support for the menu, by wangxin 2011-08-05
Menu * CreateMenuFromRes(ResID id, Menu* parent, EventListener* listener);

void RegisterInnerResource(int type, INNER_RES_INFO *resource, int count);
void RegisterAudiorResource();

#define REGISTER_RESPKG(name) FRRegister_##name##_resource()

bool SetResourceLanguage (NGInt lang);
NGInt GetResourceLanguage (void);
const char * GetTextRes (ResID id);
const char* GetPLMNDisplayName (const char* plmn);

NAMESPACE_END

#endif /* __RESPKGMANAGER_H */

