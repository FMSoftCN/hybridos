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

#include "zlib.h"

#include "resource/respkgmanager.h"
#include "resource/respackage.h"
#include "drawable.h"
//#include "custstrlist.c"
//#include "CustLanguage.h"

namespace hfcl {

ResPkgManager* ResPkgManager::m_singleton = NULL;

ResPkgManager:: ~ResPkgManager()
{
    ResPkgList::iterator it;

    for(it = m_packageList.begin(); it !=m_packageList.end(); it++)
    {
        m_packageList.erase(it);
        HFCL_DELETE(*it);
    }
}

ResPackage* ResPkgManager::getPackage(int id)
{
    ResPkgList::iterator it;
    ResPackage* package = NULL;

    for(it = m_packageList.begin(); it !=m_packageList.end(); it++)
    {
        package = (*it);
        if(package && (package->getId() == id))
            return package;
    }
   return NULL;
}

ResPkgManager* ResPkgManager::getResPkgManager()
{ 
    if (m_singleton == NULL) 
        m_singleton = HFCL_NEW_EX(ResPkgManager, ()); 
    return m_singleton; 
}

void ResPkgManager::delResPkgManager()
{ 
    if (NULL != m_singleton) 
        HFCL_DELETE(m_singleton);
    m_singleton = NULL; 
}

bool ResPkgManager::registerPackage(ResPackage *pkg)
{
	ResPkgList::iterator it;

	for(it = m_packageList.begin(); it !=m_packageList.end(); it++)
	{
		if(*it == pkg)
			return true;
	}
	
	m_packageList.push_back(pkg);
	
	return true;
}

bool ResPkgManager::unregisterPackage(int id){
    ResPkgList::iterator it;
	for(it = m_packageList.begin(); it !=m_packageList.end(); it++)
	{
		if((*it)->getId() == id) {
			m_packageList.erase(it);
			// VincentWei > FIXME: memory leap? HFCL_DELETE(*it);
			return true;
		}
	}
	return false;
}

bool ResPkgManager::unregisterPackage(const char * name)
{
    ResPkgList::iterator it;
	
	for(it = m_packageList.begin(); it !=m_packageList.end(); it++)
	{
		if(strcmp((*it)->getName(), name) == 0) {
			m_packageList.erase(it);
			HFCL_DELETE(*it);
			return true;
		}
	}

    return false;
}

ResPackage* ResPkgManager::getPackage(const char * name)
{
    ResPkgList::iterator it;

    for(it = m_packageList.begin(); it !=m_packageList.end(); it++)
    {
        if((strcmp(name, (*it)->getName()) == 0))
            return (*it);
    }

   return NULL;
}

static char* resPkgStringsBucket = NULL;
static const char* resPkgStrings [MAX_STRID];

bool ResPkgManager::setCurrentLang (int langId)
{
    if (langId < 0 || langId >= MAX_LANGID)
        return false;

    if (__ngux_map_language_to_zipped_bytes [langId].zipped_bytes == NULL) {
        _ERR_PRINTF ("ResPkgManager::setCurrentLang: no strings for language: %d.\n", langId);
        return false;
    }

    if (resPkgStringsBucket == NULL) {
        size_t max_bucket_size = 0;
        for (int i = 0; i < MAX_LANGID; i++) {
            size_t current_size = __ngux_map_language_to_zipped_bytes [i].origin_size;
            if (current_size > max_bucket_size) {
                max_bucket_size = current_size;
            }
        }

        if (max_bucket_size == 0) {
            _ERR_PRINTF ("ResPkgManager::setCurrentLang: no any language defined.\n");
            return false;
        }

        resPkgStringsBucket = (char*)malloc (max_bucket_size);
        if (resPkgStringsBucket == NULL) {
            return false;
        }
    }

    uLongf len_uncompressed = __ngux_map_language_to_zipped_bytes [langId].origin_size;
    int ret = uncompress ((Bytef*)resPkgStringsBucket, &len_uncompressed, 
            (Bytef*)__ngux_map_language_to_zipped_bytes [langId].zipped_bytes,
            __ngux_map_language_to_zipped_bytes [langId].zipped_size);
    if (ret != Z_OK) {
        _ERR_PRINTF ("ResPkgManager::setCurrentLang: failed when calling uncompress: %d.\n", ret);
        return false;
    }

#ifdef _DEBUG
    if (len_uncompressed != __ngux_map_language_to_zipped_bytes [langId].origin_size) {
        _DBG_PRINTF ("ResPkgManager::setCurrentLang: length not matched: %lu, %u.\n",
            len_uncompressed, __ngux_map_language_to_zipped_bytes [langId].origin_size);
        return false;
    }
#endif

    size_t offset = 0;
    for (int i = 0; i < MAX_STRID; i++) {
        resPkgStrings [i] = resPkgStringsBucket + offset;
        offset += strlen (resPkgStrings [i]);
        offset += 1;
    }

#ifdef _DEBUG
    if (offset > len_uncompressed) {
        _DBG_PRINTF ("ResPkgManager::setCurrentLang: buffer overflow: %lu, %u.\n",
            len_uncompressed, offset);
        return false;
    }
#endif
    m_lang = langId;
    return true;
}

/////////////////////////////////////////////////////////////////////
/////////////////         Global Interface      /////////////////////
/////////////////////////////////////////////////////////////////////

ResPkgManager * GetResPkgManager(void)
{
    return  ResPkgManager::getResPkgManager();
} 

void ReleaseResPkgManager(void)
{
    ResPkgManager *manager = ResPkgManager::getResPkgManager();

    if(NULL != manager ) 
        manager->delResPkgManager();
}

bool RegisterResPackage(ResPackage *resPkg)  
{
    return GetResPkgManager()->registerPackage(resPkg);
}

ResPackage * GetResPackageByName(const char* name)
{
    return GetResPkgManager()->getPackage(name);
}

ResPackage * GetResPackageById(int id)
{
    return GetResPkgManager()->getPackage(id);
}

bool UnregisterResPackageByName(const char* name)
{
    return GetResPkgManager()->unregisterPackage(name);
}

bool UnregisterResPackageById(int id)
{
    return GetResPkgManager()->unregisterPackage(id);
}

void* GetRes(HTResId id)
{
    ResPackage *resPkg = GetResPackage(RPKGID(id));

    if(resPkg)
        return resPkg->getRes(id);
    
	return NULL;
}

Logfont* GetFontRes(HTResId id/*, Style* style*/)
{
    ResPackage *resPkg = GetResPackage(RPKGID(id));
    if(resPkg && (RESTYPE(id)& R_TYPE_FONT))
        return resPkg->getFont(id/*, style*/);

	return NULL;
}

Style* GetStyleRes(HTResId id)
{
    ResPackage * resPkg = NULL;
    if(id == 0)
        return GetCommonStyle();
    resPkg = GetResPackage(RPKGID(id));
    if(resPkg && (RESTYPE(id) & R_TYPE_STYLE))
        return resPkg->getStyle(id);
    
	return NULL;
}

GifAnimate* GetGifAnimateRes(HTResId id)
{
    ResPackage *resPkg = GetResPackage(RPKGID(id));

    if(resPkg && (RESTYPE(id) & R_TYPE_IMAGE))
        return resPkg->getGifAnimate(id);
    
	return NULL;
}

Image * GetImageRes(HTResId id)
{
    ResPackage *resPkg = GetResPackage(RPKGID(id));

    if(resPkg && (RESTYPE(id) & R_TYPE_IMAGE))
        return resPkg->getImage(id);
    
	return NULL;
}

Bitmap * GetBitmapRes(HTResId id)
{
    ResPackage *resPkg = GetResPackage(RPKGID(id));

    if (resPkg && (RESTYPE(id) & R_TYPE_IMAGE))
        return resPkg->getBitmap(id);
    
	return NULL;
}

const char * GetTextRes(HTResId id)
{
    if (id >= MAX_NL10N_STRID) {
        _DBG_PRINTF ("GetTextRes: bad STRID: %d", id);
        return "BadID";
    }
    else if (id >= MAX_STRID) {
        _DBG_PRINTF ("GetTextRes: A non L10N string retruned: %d", id);
        return __ngux_non_l10n_raw_strings [id - MAX_STRID];
    }

    const char* text = resPkgStrings [id];
    if (text == NULL) {
        return "";
    }

    return text;
}

Drawable* GetDrawableRes(HTResId id)
{
	ResPackage *resPkg = GetResPackage(RPKGID(id));

	if(resPkg && (RESTYPE(id) & R_TYPE_DRAWABLE))
		return resPkg->getDrawable(id);

	return NULL;
}

DrawableSet* GetDrawableSetRes(HTResId id)
{
	if(!(RESTYPE(id) & R_TYPE_DRAWABLESET))
		return NULL;

	ResPackage *resPkg = GetResPackage(RPKGID(id));

	if(resPkg)
		return resPkg->getDrawableSet(id);
	return NULL;
}

ThemeRes* GetThemeRes(HTResId id)
{
	if(!(RESTYPE(id) & R_TYPE_THEME))
		return NULL;

	ResPackage *resPkg = GetResPackage(RPKGID(id));

	if(resPkg)
		return resPkg->getThemeRes(id);
	return NULL;
}

ResPackage* GetSystemPackage(void)
{
	ResPackage* package = NULL;
	package = ResPkgManager::getResPkgManager()->getPackage(0);
	
	return package;
}

// we just get drawable set res from sysres.res.c package
DrawableSet* GetDrawableSetResFromTheme(int theme_drset_id)
{
	if (theme_drset_id <= 0)
		return NULL;

	// FIXME just in sys package
	ResPackage *resPkg = GetResPackage(0);

	if(resPkg)
		return resPkg->getThemeDrawableSet(theme_drset_id);
	
	return NULL;
}

DrawableSetGroup* GetDrawableSetGroupRes(HTResId id)
{
	if(!(RESTYPE(id) & R_TYPE_DRSETGROUP))
		return NULL;

	ResPackage *resPkg = GetResPackage(RPKGID(id));

	if(resPkg)
		return resPkg->getDrawableSetGroup(id);
	return NULL;

}

View* CreateViewFromRes(HTResId id, View *parent, ViewContext *viewContext, ContentProvider* provider /*= NULL*/)
{
    ResPackage * resPkg = GetResPackage(RPKGID(id));

    if(resPkg && (RESTYPE(id) & R_TYPE_UI))
        return resPkg->createView(id , parent, viewContext, provider);
    
	return NULL;
}

Menu* CreateMenuFromRes(HTResId id, Menu* parent, EventListener* listener)
{
    ResPackage* resPkg = GetResPackage(RPKGID(id));

    if(resPkg && (RESTYPE(id) & R_TYPE_MENU))
        return resPkg->createMenu(id , parent, listener);
    
	return NULL;
}

int GetResourceLanguage (void)
{
    return GetResPkgManager()->getCurrentLang();
}

bool SetResourceLanguage (int lang)
{
    return GetResPkgManager()->setCurrentLang (lang);
}

} // namespace hfcl {

