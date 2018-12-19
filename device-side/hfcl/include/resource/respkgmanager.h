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

#ifndef HFCL_RESOURCE_RESPKGMANAGER_H_
#define HFCL_RESOURCE_RESPKGMANAGER_H_

#include "../resource/restypes.h"
#include "../graphics/graphicscontext.h"
#include "../css/stylesheetdeclared.h"
#include "../css/stylesheetdeclaredgroup.h"

namespace hfcl {

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
public:
    static ResPkgManager * getResPkgManager();
    void delResPkgManager();

    bool registerPackage(ResPackage *pkg);
    bool unregisterPackage(int id);
    bool unregisterPackage(const char *name);
    ResPackage *getPackage(int id);
    ResPackage *getPackage(const char *name);

    inline int count(void) {
        return m_packageList.size();
    }

private:
    ~ResPkgManager();
    ResPkgManager() { }

    LIST(ResPackage*, ResPkgList);

    static ResPkgManager* m_singleton;

    ResPkgList m_packageList;
};

/******      Global Interface      ******/

// The system package id is 0
ResPackage* GetSystemPackage(void);
ResPkgManager* GetResPkgManager(void);
void ReleaseResPkgManager(void);

bool RegisterResPackage(ResPackage *resPkg);
ResPackage* GetResPackageByName(const char* name);
ResPackage* GetResPackageById(int id);
bool UnregisterResPackageByName(const char* name);
bool UnregisterResPackageById(int id);

#define GetResPackage(id)         GetResPackageById(id)
#define UnregisterResPackage(id)  UnregisterResPackageById(id)

void * GetRes(HTResId id);
Logfont * GetFontRes(HTResId id);
Style * GetStyleRes(HTResId id);
Image * GetImageRes(HTResId id);
Bitmap * GetBitmapRes(HTResId id);
GifAnimate* GetGifAnimateRes(HTResId id);
StyleSheetDeclared* GetCssRes(HTResId id);
StyleSheetDeclaredGroup* GetCssGroupRes(HTResId id);

Drawable* GetDrawableRes(HTResId id);
DrawableSet* GetDrawableSetRes(HTResId id);
ThemeRes* GetThemeRes(HTResId id);
DrawableSet* GetDrawableSetResFromTheme(int theme_drset_id);
DrawableSetGroup* GetDrawableSetGroupRes(HTResId id);

View * CreateViewFromRes(HTResId id, View *parent,
        ViewContext *viewContext, ContentProvider* provider = NULL);
#define GetUITempl(id)   (CB_CREATE_VIEW)GetRes(id)

// add the support for the menu, by wangxin 2011-08-05
Menu * CreateMenuFromRes(HTResId id, Menu* parent, EventListener* listener);

#define REGISTER_RESPKG(name) FRRegister_##name##_resource()

bool SetResourceLanguage (HIDLanguage lang,
        HIDEncoding enc = R_ENCODING_utf8, int pkgId = 0);
HIDLanguage GetResourceLanguage (int pkgId = 0);
HIDEncoding GetResourceEncoding (int pkgId = 0);

const char *GetText (HTStrId id, int pkgId = 0);
const char *GetText (const char* str, int pkgId = 0);

void RegisterIncoreRes (const char* resname,
            const HFCL_INCORE_RES *incores, int count);

} // namespace hfcl

#endif /* HFCL_RESOURCE_RESPKGMANAGER_H_ */

