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

#ifndef HFCL_RESOURCE_RESPACKAGE_H_H
#define HFCL_RESOURCE_RESPACKAGE_H_H

#include "../resource/restypes.h"
#include "../resource/textres.h"
#include "../view/viewcontext.h"
#include "../resource/fontres.h"
#include "../resource/imageres.h"
#include "../css/cssdeclared.h"
#include "../css/cssdeclaredgroup.h"

namespace hfcl {

class DrawableSet;
class Drawable;
class DrawableSetGroup;

class Menu;
class GifAnimate;
class ThemeRes;

typedef struct _ResourceEntry {
    HTResId id;
    const char* value;
} ResourceEntry;

typedef Menu * (*CB_CREATE_MENU)(Menu* parent, EventListener* listener);

typedef const char* hfcl_const_str_t;

PAIR(DWORD, hfcl_const_str_t, TextResPair);
MAP(DWORD, hfcl_const_str_t, TextResMap);
VECTORCLASS(FontRes, FontResVec);
VECTORCLASS(ImageRes, ImageResVec);
VECTOR(CssDeclared*, CssResVec);
VECTOR(CssDeclaredGroup*, CssGroupResVec);
VECTOR(CB_CREATE_VIEW, UiResVec);
VECTOR(CB_CREATE_MENU, MenuResVec);

/* The following ones will be deprecated */
VECTOR(Drawable*, DrawableResVec);
VECTOR(DrawableSet*, DrawableSetResVec)
VECTOR(DrawableSetGroup*, DrawableSetGroupResVec)
VECTOR(ThemeRes*, ThemeResVec)
VECTOR(Style *, StyleResVec);

class ResourceBucket {
public:
    ResourceBucket();
    ~ResourceBucket();

    TextResMap &textRes(void);
    ImageResVec &imageRes(void);
    FontResVec &fontRes(void);
    CssResVec &cssRes(void);
    CssGroupResVec &cssGroupRes(void);
    UiResVec &uiRes(void);
    MenuResVec &menuRes(void);

    /* The following ones will be deprecated */
    StyleResVec &styleRes(void);
    DrawableResVec &drawableRes(void);
    DrawableSetResVec &drawableSetRes(void);
    DrawableSetGroupResVec& drawableSetGroupRes(void);
    ThemeResVec& themeRes(void);

public:
    int type;
    void *resouces;
};

class ResPackage {
public:
    ResPackage(const char *name, int id,
            const HFCL_INCORE_RES* incoreRes = NULL);
    ~ResPackage();
    /*
     * res path related
     */
    const utf8string &getPackagePath(void);
    void setPackagePath(utf8string &packagePath);

    /*
     * res package related
     */
    int getId(void);
    const char *getName(void);

    /*
     * adding resource
     */
    bool addImageResource(const ResourceEntry *images);
    bool addFontResource(const ResourceEntry *fonts);
    bool addCssResource(CssDeclared* cssDeclared);
    bool addCssGroupResource(CssDeclaredGroup* cssGroup);
    bool addUIResource(HTResId id, CB_CREATE_VIEW createView);
    bool addUIResource(UI_RES_ARRAY *pUIArray, int nUICount);
    bool addMenuResource(HTResId id, CB_CREATE_MENU createMenu);
    bool addMenuResource(MENU_RES_ARRAY *pMnuArray, int nMenuCount);

    /* to be deprecated */
    bool addStyleResource(HTResId id,  HTResId superid,
            const TRStyleElement *elements, int count);
    void addDrawableResource(TRDrawableArrayItem* all_elements, int size);
    void addDrawableSetResource(TRDrawableSetArrayItem *all_items, int size);
    void addThemeResource(HTResId theme_id, const char* theme_name,
            const TRThemeItem* items);
    void addDrawableSetGroupResource(HTResId drsetgroup_id,
            HTResId drsetgroup_super_id, const TRDrawableSetGroupItem * items);
    void addDrawableSetGroupResource(TRDrawableSetGroupArrayItem* all_groups,
            int size);

    /*
     * getting resource
     */
    void *getRes(HTResId id);
    Image *getImage(HTResId id);
    Bitmap *getBitmap(HTResId id);
    GifAnimate *getGifAnimate(HTResId id);
    Logfont *getFont(HTResId id);
    CssDeclared *getCss(HTResId id);
    CssDeclaredGroup *getCssGroup(HTResId id);

    CB_CREATE_VIEW getUi(HTResId id);
    CB_CREATE_MENU getMenu(HTResId id);
    View *createView(HTResId id, ViewContainer *parent, ViewContext *viewContext,
        ContentProvider *provider = NULL);
    Menu *createMenu(HTResId id, Menu *parent, EventListener* listener);

    /* to be deprecated */
    Style *getStyle(HTResId id);
    Drawable* getDrawable(HTResId id);
    DrawableSet* getDrawableSet(HTResId);
    DrawableSetGroup* getDrawableSetGroup(HTResId);
    ThemeRes* getThemeRes(HTResId);
    DrawableSet* getThemeDrawableSet(int theme_drset_id);

    /*
     * text related methods
     */
    void addTextResRaw(HIDLanguage lang, HIDEncoding enc,
            const char* fileName);
    void addTextResZipped(HIDLanguage lang, HIDEncoding enc,
            const char* fileName);
    void addTextResGnuMsg(HIDLanguage lang, HIDEncoding enc,
            const char* fileName);
    bool setCurrentLang(HIDLanguage lang, HIDEncoding enc);
    HIDLanguage getCurrentLanguage() { return m_languageId; }
    HIDEncoding getCurrentEncoding() { return m_encodingId; }

    const char *getText(HTStrId id);
    const char *getText(const char* str);

    /*
     * Theme apis
     */
    bool setTheme(HTResId theme_res_id, bool update_system = TRUE);
    HTResId themeId(void);
    ThemeRes* theme(void);

private:
    ResourceBucket  m_resBuckets[NR_RES_TYPE];
    const ResourceEntry *m_imageResourceEntry;
    MENU_RES_ARRAY *m_pMenuResArray;
    unsigned int    m_MenuArrayCount;

    UI_RES_ARRAY *  m_pUIResArray;
    unsigned int    m_UIArrayCount;

    // ---------------- \|/ -----------------
    const TRDrawableArrayItem* m_drawableResource;
    int m_drawableResourceSize;
    const TRDrawableSetArrayItem* m_drawablesetResource;
    int m_drawablesetResourceSize;
    const TRDrawableSetGroupArrayItem* m_drawablesetGroupResource;
    int m_drawablesetGroupResourceSize;
    // ---------------- /|\ -----------------
    unsigned int m_ImagReseSize;

    utf8string m_packagePath;
    ThemeRes* m_theme;
    HTResId m_theme_id;

protected:
    int m_id;
    utf8string m_name;

private:
    HIDLanguage m_languageId;
    HIDEncoding m_encodingId;

    HIDResType  m_textResType;
    TextRes*    m_textRes;
};

utf8string GetResRealPath(const char *packagePath, const char *resFilename);

bool RegisterCommonDrawableFromRes(int dr_id, const char* basename,
        const TRStyleElement *elements, int super_id);

bool RegisterViewDrawableSetFromRes(const char * view_name,
        HTResId super_drset, const TRDrawableSetItem *items);

bool RegisterViewDrawableSetGroupFromRes(const char * view_name,
        HTResId super_drsetgroup, const TRDrawableSetGroupItem *items);

} // namespace hfcl

#endif /* HFCL_RESOURCE_RESPACKAGE_H_H */

