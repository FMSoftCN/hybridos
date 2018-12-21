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

#include "resource/respackage.h"

#include "common/stlalternative.h"
#include "drawable/drawable.h"
#include "resource/resloader.h"
#include "resource/respkgmanager.h"
#include "resource/themeres.h"
#include "activity/activitymanager.h"

namespace hfcl {

/*
 * ResourceBucket related
 */
ResourceBucket::ResourceBucket()
{
    resouces = NULL;
    type = 0;
}

ResourceBucket::~ResourceBucket()
{
    switch (type) {
    case R_TYPE_TEXT_RAW:
    case R_TYPE_TEXT_ZIPPED:
    case R_TYPE_TEXT_GNUMSG:
        if (resouces)
            HFCL_DELETE((TextResMap *)resouces);
        break;

    case R_TYPE_IMAGE:
        if (resouces) {
            //we must clean the image resource
            ImageResVec& imgs = imageRes();
            ImageResVec::iterator it;
            for (it = imgs.begin(); it != imgs.end(); ++it) {
                (*it).clean();
            }

            HFCL_DELETE((ImageResVec *)resouces);
        }
        break;

    case R_TYPE_FONT:
        if (resouces)
            HFCL_DELETE((FontResVec *)resouces);
        break;

    case R_TYPE_UI:
        if (resouces)
            HFCL_DELETE((UiResVec *)resouces);
        break;

    case R_TYPE_MENU:
        if (resouces)
            HFCL_DELETE((MenuResVec *)resouces);
        break;

    case R_TYPE_CSS:
        if (resouces) {
            CssResVec *v = (CssResVec*)resouces;

            CssResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it) {
                CssDeclared* css = *it;
                _DBG_PRINTF ("Removing css from resource bucket: %p, %d\n", css, css->getRefCnt());
                css->unref();
            }

            HFCL_DELETE(v);
        }
        break;

    case R_TYPE_CSSGROUP:
        if (resouces) {
            CssGroupResVec *v = (CssGroupResVec*)resouces;

            CssGroupResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it) {
                CssDeclaredGroup* css_group = *it;
                _DBG_PRINTF ("Removing css group from resource bucket: %p, %d\n", css_group, css_group->getRefCnt());
                css_group->unref();
            }

            HFCL_DELETE(v);
        }
        break;

    /* the following ones will be deperacated */
    case R_TYPE_STYLE:
        if (resouces) {
            StyleResVec *v = (StyleResVec*)resouces;

            StyleResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it) {
                (*it)->unref();
            }

            HFCL_DELETE(v);
        }
        break;

    case R_TYPE_DRAWABLE:
        if (resouces) {
            DrawableResVec *v = (DrawableResVec *)resouces;

            DrawableResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it) {
                (*it)->unref();
            }

            HFCL_DELETE(v);
        }
        break;

    case R_TYPE_DRAWABLESET:
        if (resouces) {
            DrawableSetResVec *v = (DrawableSetResVec *)resouces;

            DrawableSetResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it) {
                (*it)->unref();
            }

            HFCL_DELETE(v);
        }
        break;

    case R_TYPE_DRSETGROUP:
        if (resouces) {
            DrawableSetGroupResVec *v = (DrawableSetGroupResVec *)resouces;

            DrawableSetGroupResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it) {
                (*it)->unref();
            }

            HFCL_DELETE(v);
        }
        break;

    case R_TYPE_THEME:
        if (resouces) {
            ThemeResVec *v = (ThemeResVec *)resouces;

            ThemeResVec::iterator it;
            for (it = v->begin(); it != v->end(); ++it)
                HFCL_DELETE(*it);

            HFCL_DELETE(v);
        }
        break;

    default:
        break;
    }
}

TextResMap &ResourceBucket::textRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(TextResMap, ());
    }

    return ( *((TextResMap *)resouces) );
}

ImageResVec &ResourceBucket::imageRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(ImageResVec, ());
    }

    return ( *((ImageResVec *)resouces) );
}

FontResVec &ResourceBucket::fontRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(FontResVec, ());
    }

    return ( *((FontResVec *)resouces) );
}

CssResVec &ResourceBucket::cssRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(CssResVec, ());
    }

    return ( *((CssResVec *)resouces) );
}

CssGroupResVec &ResourceBucket::cssGroupRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(CssGroupResVec, ());
    }

    return ( *((CssGroupResVec *)resouces) );
}

UiResVec &ResourceBucket::uiRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(UiResVec, ());
    }

    return ( *((UiResVec *)resouces) );
}

MenuResVec& ResourceBucket::menuRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(MenuResVec, ());
    }

    return (*((MenuResVec*)resouces));
}

StyleResVec &ResourceBucket::styleRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(StyleResVec, ());
    }

    return ( *((StyleResVec *)resouces) );
}

DrawableResVec &ResourceBucket::drawableRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(DrawableResVec, ());
    }

    return ( *((DrawableResVec *)resouces) );
}

DrawableSetResVec &ResourceBucket::drawableSetRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(DrawableSetResVec, ());
    }
    return ( *((DrawableSetResVec *)resouces) );
}

ThemeResVec &ResourceBucket::themeRes(void)
{
    if (NULL == resouces) {
        resouces = HFCL_NEW_EX(ThemeResVec, ());
    }
    return ( *((ThemeResVec *)resouces) );
}

DrawableSetGroupResVec & ResourceBucket::drawableSetGroupRes(void)
{
    if(NULL == resouces) {
        resouces = HFCL_NEW_EX(DrawableSetGroupResVec, ());
    }
    return (*((DrawableSetGroupResVec*)resouces));
}

/*
 * ResPackage related
 */
ResPackage::ResPackage(const char *name, int id,
        const HFCL_INCORE_RES* incoreRes)
{
    int i = 0;
    m_id = id;
    m_name= string(name);
    m_packagePath = "";

    /*
     * init res package entry
     */
    for (i = 0; i < NR_RES_TYPE; ++i) {
        m_resBuckets[i].type = i;
        m_resBuckets[i].resouces = NULL;
    }

    m_imageResourceEntry = NULL;
    m_ImagReseSize = 0;
    m_pMenuResArray= NULL;
    m_MenuArrayCount = 0;
    m_pUIResArray = NULL;
    m_UIArrayCount = 0;

    // m_incoreRes = incoreRes;

    m_languageId = R_LANG_na_NA;
    m_encodingId = R_ENCODING_unknown;

    m_textResType = R_TYPE_TEXT_RAW;
    m_textRes = NULL;
}

ResPackage::~ResPackage()
{
    if (m_textRes) {
        HFCL_DELETE (m_textRes);
    }
}

const string &ResPackage::getPackagePath(void)
{
    return m_packagePath;
}

void ResPackage::setPackagePath(string &packagePath)
{
    m_packagePath = packagePath;
}

int ResPackage::getId(void)
{
    return m_id;
}

const char *ResPackage::getName(void)
{
    return m_name.c_str();
}

bool ResPackage::addImageResource(const ResourceEntry *images)
{
    int count = 0;

    while (images[count].id != 0)
        ++count;

    m_ImagReseSize  = count;
    m_imageResourceEntry = images;
    return true;
}

bool ResPackage::addFontResource(const ResourceEntry *fonts)
{
    int count = 0, _size = 0, index = 0;
    FontResVec & fontResVec = m_resBuckets[R_TYPE_FONT].fontRes();

    while (fonts[count].id != 0)
        ++count;

    _size = fontResVec.size(); //cause FontRes desconstruct
    fontResVec.resize(_size + count);

    for (index = 0; index < count; ++index) {
        FontRes fontRes(fonts[index].value);
        fontResVec[_size + index] = fontRes;
    }

    return true;
}

bool ResPackage::addCssResource(CssDeclared* cssDeclared)
{
    m_resBuckets[R_TYPE_CSS].cssRes().push_back(cssDeclared);
    return true;
}

bool ResPackage::addCssGroupResource(CssDeclaredGroup* cssGroup)
{
    m_resBuckets[R_TYPE_CSSGROUP].cssGroupRes().push_back(cssGroup);
    return true;
}

bool ResPackage::addUIResource(HTResId id, CB_CREATE_VIEW cb_createView)
{
    m_resBuckets[R_TYPE_UI].uiRes().push_back(cb_createView);

    return true;
}

bool ResPackage::addUIResource(UI_RES_ARRAY *pUIArray, int nUICount)
{
    m_pUIResArray = pUIArray;
    m_UIArrayCount = nUICount;

    return true;
}

bool ResPackage::addMenuResource(MENU_RES_ARRAY *pMnuArray, int nMenuCount)
{
    m_pMenuResArray= pMnuArray;
    m_MenuArrayCount = nMenuCount;
    return true;
}

bool ResPackage::addMenuResource(HTResId id, CB_CREATE_MENU cb_createMenu)
{
    m_resBuckets[R_TYPE_MENU].menuRes().push_back(cb_createMenu);

    return true;
}

bool ResPackage::addStyleResource(HTResId id, HTResId superid,
        const TRStyleElement *elements, int count)
{
    // add the common resource
    if(id == 0) {
        AddCommonStyleElementsFromRes(elements);
        return true;
    }

    if (!elements || !count)
        return false;

    Style* style = NULL;
    if (count > 30)
        style = HFCL_NEW_EX (HashedStyle, (elements));
    else
        style = HFCL_NEW_EX (SimpleStyle, (elements));

    Style* super = GetStyleRes (superid);
    if (!super)
        super = GetCommonStyle();

    style->setSuper(super);
    m_resBuckets[R_TYPE_STYLE].styleRes().push_back(style);

    return true;
}

void ResPackage::addDrawableResource(TRDrawableArrayItem* all_elements,
        int size)
{
    m_drawableResource = all_elements;
    m_drawableResourceSize = size;
}

static DrawableSet* create_drawable_set_from_res(HTResId drset_super_id,
        const TRDrawableSetItem* items)
{
    DrawableSet *drset_super = NULL;
    if(drset_super_id != 0)
        drset_super = GetDrawableSetRes(drset_super_id);

    if(!drset_super)
        drset_super = GetCommonDrawableSet();

    DrawableSet * drset = HFCL_NEW_EX(SimpleDrawableSet,(drset_super));

    for(int i=0; items[i].dr_id != -1; i++)
    {
        Drawable *dr = NULL;
        if(items[i].drres_id != 0)
            dr = GetDrawableRes(items[i].drres_id);
        drset->setDrawable(items[i].dr_id, dr);
    }

    return drset;
}

static DrawableSetGroup* create_drawable_set_group_from_res (
        HTResId drsetgroup_super_id,
        const TRDrawableSetGroupItem* items)
{
    DrawableSetGroup* super = NULL;

    if (!drsetgroup_super_id != 0)
        super = GetDrawableSetGroupRes(drsetgroup_super_id);

    DrawableSetGroup* drsetgroup = HFCL_NEW_EX(DrawableSetGroup, (super));

    if (drsetgroup) {
        drsetgroup->setDrawableSetMapTable((TRDrawableSetGroupItem*)items);
    }

    return drsetgroup;
}

void ResPackage::addDrawableSetResource(TRDrawableSetArrayItem* all_items,
        int size)
{
    m_drawablesetResource = all_items;
    m_drawablesetResourceSize = size;
}

void ResPackage::addDrawableSetGroupResource(
        TRDrawableSetGroupArrayItem* all_groups, int size)
{
    m_drawablesetGroupResource = all_groups;
    m_drawablesetGroupResourceSize = size;
}

void ResPackage::addDrawableSetGroupResource(HTResId drsetgroup_id,
        HTResId drsetgroup_super_id, const TRDrawableSetGroupItem * items)
{
    DrawableSetGroup* drsetgroup =
        create_drawable_set_group_from_res(drsetgroup_super_id, items);
    if(!drsetgroup)
        return ;
    m_resBuckets[R_TYPE_DRSETGROUP].drawableSetGroupRes().push_back(drsetgroup);
}

void ResPackage::addThemeResource(HTResId theme_id, const char* theme_name,
        const TRThemeItem* items)
{
    ThemeRes* themeR = HFCL_NEW_EX(ThemeRes,());
    if (themeR != NULL) {
        themeR->setDrawableSetMapTable((TRThemeItem*)items);
        m_resBuckets[R_TYPE_THEME].themeRes().push_back(themeR);
    }
}

void *ResPackage::getRes(HTResId id)
{
    switch (RESTYPE(id)) {
    case R_TYPE_IMAGE:
        return (void *)getImage(id);

    case R_TYPE_FONT:
        return (void *)getFont(id);

    case R_TYPE_CSS:
        return (void *)getCss(id);

    case R_TYPE_UI:
        return (void *)getUi(id);

    case R_TYPE_MENU:
        return (void *)getMenu(id);

    case R_TYPE_STYLE:
        return (void *)getStyle(id);

    case R_TYPE_DRAWABLE:
        return (void*)getDrawable(id);

    default:
        break;
    }

    return NULL;
}

void ResPackage::addTextResRaw(HIDLanguage lang, HIDEncoding enc,
        const char* fileName)
{
    m_resBuckets[R_TYPE_TEXT_RAW].textRes()[MAKELONG(enc, lang)]
        = fileName;
}

void ResPackage::addTextResZipped(HIDLanguage lang, HIDEncoding enc,
            const char* fileName)
{
    m_resBuckets[R_TYPE_TEXT_ZIPPED].textRes()[MAKELONG(enc, lang)]
        = fileName;
}

void ResPackage::addTextResGnuMsg(HIDLanguage lang, HIDEncoding enc,
            const char* fileName)
{
    m_resBuckets[R_TYPE_TEXT_GNUMSG].textRes()[MAKELONG(enc, lang)]
        = fileName;
}

bool ResPackage::setCurrentLang(HIDLanguage lang, HIDEncoding enc)
{
    if (m_textRes) {
        HFCL_DELETE (m_textRes);
        m_textRes = NULL;
    }

    TextResMap &res = m_resBuckets[R_TYPE_TEXT_RAW].textRes();
    TextResMap::iterator it = res.find(MAKELONG(enc, lang));
    if (it != res.end() && it->second) {
        m_textRes = HFCL_NEW_EX (TextResRaw, (it->second));
        return true;
    }

    res = m_resBuckets[R_TYPE_TEXT_ZIPPED].textRes();
    it = res.find(MAKELONG(enc, lang));
    if (it != res.end() && it->second) {
        m_textResType = R_TYPE_TEXT_ZIPPED;
        m_textRes = HFCL_NEW_EX (TextResZipped, (it->second));
        return true;
    }

    res = m_resBuckets[R_TYPE_TEXT_GNUMSG].textRes();
    it = res.find(MAKELONG(enc, lang));
    if (it != res.end() && it->second) {
        m_textResType = R_TYPE_TEXT_GNUMSG;
        m_textRes = HFCL_NEW_EX (TextResGnuMsg, (it->second));
        return true;
    }

    return false;
}

const char* ResPackage::getText (HTStrId id)
{
    if (m_textRes == NULL) {
        _DBG_PRINTF ("ResPackage::getText: NOT initialized text resource\n");
        return NULL;
    }

    return m_textRes->getText (id);
}

const char* ResPackage::getText (const char* str)
{
    if (m_textRes) {
        _DBG_PRINTF ("ResPackage::getText: NOT initialized text resource\n");
        return NULL;
    }

    return m_textRes->getText (str);
}

GifAnimate *ResPackage::getGifAnimate(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;

    if (R_TYPE_IMAGE != RESTYPE(id) || idx < 0
            || idx >= m_ImagReseSize)
        return (GifAnimate *)NULL;

    return ResLoader::getInstance()->getGifAnimate(m_imageResourceEntry[idx].value);
}

Image *ResPackage::getImage(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;

    if (R_TYPE_IMAGE != RESTYPE(id) || idx < 0
            || idx >= m_ImagReseSize)
        return (Image *)NULL;

    return ResLoader::getInstance()->getImage(m_imageResourceEntry[idx].value);
}

Bitmap *ResPackage::getBitmap(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;

    if (R_TYPE_IMAGE != RESTYPE(id) || idx < 0
            || idx >= m_ImagReseSize)
        return NULL;

    return ResLoader::getInstance()->getBitmap(m_imageResourceEntry[idx].value);
}

Logfont *ResPackage::getFont(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (R_TYPE_FONT != RESTYPE(id) || idx < 0
            || idx >= (unsigned int)m_resBuckets[RESTYPE(id)].fontRes().size())
        return (Logfont *)NULL;

    return m_resBuckets[RESTYPE(id)].fontRes()[idx].get();
}

CssDeclared *ResPackage::getCss(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (id == 0)
        return NULL;

    if (R_TYPE_CSS != RESTYPE(id) || idx < 0
            || idx >= (unsigned int)m_resBuckets[RESTYPE(id)].cssRes().size())
        return NULL;

    return m_resBuckets[RESTYPE(id)].cssRes()[idx];
}

CssDeclaredGroup *ResPackage::getCssGroup(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (id == 0)
        return NULL;

    if (R_TYPE_CSSGROUP != RESTYPE(id) || idx < 0
            || idx >= (unsigned int)m_resBuckets[RESTYPE(id)].cssGroupRes().size())
        return NULL;

    return m_resBuckets[RESTYPE(id)].cssGroupRes()[idx];
}

CB_CREATE_VIEW ResPackage::getUi(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (R_TYPE_UI != RESTYPE(id) || idx < 0
            || idx >= m_UIArrayCount)
        return (CB_CREATE_VIEW)NULL;

    return (CB_CREATE_VIEW)m_pUIResArray[idx].UiFunction;
}

View *ResPackage::createView(HTResId id,
        View *parent, ViewContext *viewContext, ContentProvider* provider)
{
    CB_CREATE_VIEW cb = (CB_CREATE_VIEW)getRes(id);
    if (NULL == cb)
        return (View *)NULL;
    return cb(parent, viewContext, provider);
}

CB_CREATE_MENU ResPackage::getMenu(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (R_TYPE_MENU != RESTYPE(id) || idx < 0
            || idx >= m_MenuArrayCount)
        return NULL;

    return(CB_CREATE_MENU) m_pMenuResArray[idx].Menufunction;
}

Menu* ResPackage::createMenu(HTResId id, Menu *parent, EventListener* listener)
{
    CB_CREATE_MENU cbm = (CB_CREATE_MENU)getRes(id);
    if (NULL == cbm)
        return NULL;

    return cbm(parent, listener);
}

Style *ResPackage::getStyle(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (id == 0)
        return NULL;

    if (R_TYPE_STYLE != RESTYPE(id) || idx < 0
            || idx >= (unsigned int)m_resBuckets[RESTYPE(id)].styleRes().size())
        return NULL;

    return m_resBuckets[RESTYPE(id)].styleRes()[idx];
}

Drawable* ResPackage::getDrawable(HTResId id)
{
    int idx = RESINDEX(id) - 1;

    if (R_TYPE_DRAWABLE != RESTYPE(id) || idx < 0
            || idx >= m_drawableResourceSize)
        return NULL;

    Drawable * dr = NULL;
    TRStyleElement* elements = NULL;
    const char* name = m_drawableResource[idx].base_name;
    HTResId super_style_id = m_drawableResource[idx].super_style_id;

    elements = (TRStyleElement*)m_drawableResource[idx].style_element;

    Style * s = NULL;
    if(super_style_id != 0)
        s = (Style*)GetRes(super_style_id);

    if(!s)
        s = GetCommonStyle();

    if (name == NULL) {
        dr = SAFE_CAST(Drawable*, s->clone());
        if(!dr)
            return NULL;
        if(elements)
            dr->init(elements);
    }
    else {
        dr = CreateDrawable(name, elements);
        if(!dr)
            return NULL;
    }

    dr->setSuper(s);

    return dr;
}

DrawableSet* ResPackage::getDrawableSet(HTResId id)
{
    int idx = RESINDEX(id) - 1;

    if (R_TYPE_DRAWABLESET != RESTYPE(id) || idx < 0
            || idx >= m_drawablesetResourceSize)
        return (DrawableSet *)NULL;

    int drset_super_id = m_drawablesetResource[idx].super_drset_id;
    TRDrawableSetItem *items;
    items = (TRDrawableSetItem *)m_drawablesetResource[idx].drawables;

    return create_drawable_set_from_res(drset_super_id, items);
}

DrawableSetGroup* ResPackage::getDrawableSetGroup(HTResId id)
{
    int idx = RESINDEX(id) - 1;
    if (R_TYPE_DRSETGROUP != RESTYPE(id) || idx < 0
            || idx >= m_drawablesetGroupResourceSize)
        return (DrawableSetGroup *)NULL;

    int drgroup_super_id = m_drawablesetGroupResource[idx].drsetgroup_super_id;
    TRDrawableSetGroupItem* items;
    items = (TRDrawableSetGroupItem *)m_drawablesetGroupResource[idx].drawablesets;

    return create_drawable_set_group_from_res(drgroup_super_id , items);
}

ThemeRes* ResPackage::getThemeRes(HTResId id)
{
    unsigned int idx = RESINDEX(id) - 1;
    if (R_TYPE_THEME!= RESTYPE(id) || idx < 0
            || idx >= (unsigned int)m_resBuckets[RESTYPE(id)].themeRes().size())
        return (ThemeRes *)NULL;
    return m_resBuckets[RESTYPE(id)].themeRes()[idx];
}

DrawableSet* ResPackage::getThemeDrawableSet(int theme_drset_id)
{
    if (NULL != m_theme) {
        return m_theme->getDrawableSet(theme_drset_id);
    }
    return NULL;
}

bool ResPackage::setTheme(HTResId theme_res_id, bool update_system)
{
    ThemeRes* themeR = getThemeRes(theme_res_id);
    if (NULL == themeR)
        return false;

    if (m_theme == themeR)
        return false;

    // FIXME we need update all views in the system
    m_theme = themeR;
    m_theme_id = theme_res_id;
    if (update_system) {
        _DBG_PRINTF ("ResPackage::setTheme: NOT IMPLEMENTED\n");
        // ActivityManager::getInstance()->broadcastMessage(
        //      HFCL_MSG_CHANGE_THEME, theme_res_id, 0);
    }

    return true;
}

ThemeRes* ResPackage::theme(void)
{
    return m_theme;
}

HTResId ResPackage::themeId(void)
{
    return m_theme_id;
}

static inline bool is_special_dir(const char* str) {
    if(!str)
        return false;
    return str[0] == '/'
        || (str[0] == '.' &&
                (str[1] == '/' || (str[1] == '.' && str[2] == '/')));
}

string GetResRealPath(const char *packagePath, const char *resFilename)
{
    int len;
    if(NULL == resFilename)
        return string("");
    if(is_special_dir(resFilename))
        return string(resFilename);

    if(packagePath == NULL)
        return string(resFilename);

    len = strlen(packagePath);
    if(len == 0)
        return string(resFilename);

    string str = packagePath;
    if(packagePath[len-1] == '/' || packagePath[len-1] =='\\')
        str += "/";
    return str + resFilename;

}

bool RegisterCommonDrawableFromRes(int dr_id,
        const char* base_name, const TRStyleElement *elements, int super_id)
{
    Drawable *dr = CreateDrawable(base_name, elements);
    if(!dr)
        return false;

    Style* s = NULL;
    if(super_id != 0)
        s = (Style*) GetRes(super_id);

    if(!s)
        s = GetCommonStyle();

    dr->setSuper(s);

    RegisterCommonDrawable(dr_id, dr);
    return true;
}

bool RegisterViewDrawableSetFromRes(const char* view_name,
        HTResId super_drset, const TRDrawableSetItem *items)
{
    if(!view_name)
        return false;

    DrawableSet *drset = create_drawable_set_from_res(super_drset, items);
    if(!drset)
        return false;

    drset->setCommon(true);
    RegisterViewDrawableSet(view_name, drset);
    return true;
}

bool RegisterViewDrawableSetGroupFromRes(const char * view_name,
        HTResId super_drsetgroup, const TRDrawableSetGroupItem *items)
{
    if(!view_name)
        return false;

    DrawableSetGroup* group;
    group = create_drawable_set_group_from_res(super_drsetgroup, items);
    if (!group)
        return false;

    group->setCommon(true);
    RegisterViewDrawableSetGroup (view_name, group);
    return true;
}

} // namespace hfcl

