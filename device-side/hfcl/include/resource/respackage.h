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

#ifndef __RESPACKAGE_H
#define __RESPACKAGE_H

#include "restypes.h"
#include "view/viewcontext.h"

namespace hfcl {

class DrawableSet;
class Drawable;
class Menu;
class DrawableSetGroup;
class GifAnimate;
class ThemeRes;

typedef struct  _TResouceEntry 
{
	HTResId id;
	const char* value;  
}TResourceEntry;

typedef Menu * (*CB_CREATE_MENU)(Menu* parent, EventListener* listener);

} // namespace hfcl

#include "fontres.h"
#include "imageres.h"

namespace hfcl {

typedef const TResourceEntry*  CONST_PTR_TResourceEntry;

PAIR(DWORD, CONST_PTR_TResourceEntry, TextResPair);
MAP(DWORD, CONST_PTR_TResourceEntry, TextResMap);
VECTORCLASS(FontRes, FontResVec);
VECTORCLASS(ImageRes, ImageResVec);
//VECTORCLASS(DrawableRes, DrawableResVec);
VECTOR(Drawable*, DrawableResVec);
VECTOR(DrawableSet*, DrawableSetResVec)
VECTOR(DrawableSetGroup*, DrawableSetGroupResVec)
VECTOR(ThemeRes*, ThemeResVec)
//VECTOR(DrawableSet *, DrawableSetVec);
VECTOR(Style *, StyleResVec);
VECTOR(CB_CREATE_VIEW, UiResVec);
VECTOR(CB_CREATE_MENU, MenuResVec);
//MAPCLASSKEY(string, DrawableSet *, DrawableSetMap);

class ResEntry
{
	public:
		ResEntry();
		~ResEntry();

		TextResMap &textRes(void);
		ImageResVec &imageRes(void);
		FontResVec &fontRes(void);
		StyleResVec &styleRes(void);
		UiResVec &uiRes(void);
		MenuResVec &menuRes(void);
		DrawableResVec &drawableRes(void);
		DrawableSetResVec &drawableSetRes(void);
		DrawableSetGroupResVec& drawableSetGroupRes(void);
		ThemeResVec& themeRes(void);

	public:
		int type;
		void *resouces;
		
};


class ResPackage
{
	public:
		ResPackage(const char *name, int id);
		~ResPackage();
		/*
		 * res path related
		 */
		const string &getPackagePath(void);
		void setPackagePath(string &packagePath);

		/*
		 * res package related
		 */
		int getId(void);
		const char *getName(void);

		/*
		 * res add related
		 */
		bool addTextResource(enum ResLang lang, enum ResEncoding enc, const TResourceEntry *texts);
		bool addImageResource(const TResourceEntry *images);
		bool addFontResource(const TResourceEntry *fonts);
		//bool addStyleResource(HTResId id, const Style::Element *elements, HTResId superid);
		bool addStyleResource(HTResId id,  HTResId superid,const TRStyleElement *elements, int count);
		bool addUIResource(HTResId id, CB_CREATE_VIEW createView);
		bool addUIResource(UI_RES_ARRAY *pUIArray, int nUICount);

		bool addMenuResource(HTResId id, CB_CREATE_MENU createMenu);
		bool addMenuResource(MENU_RES_ARRAY *pMnuArray, int nMenuCount);

		// ----------
        void addDrawableResource(TRDrawableArrayItem* all_elements, int size);		
        // ----------
		void addDrawableSetResource(TRDrawableSetArrayItem *all_items, int size);		
		void addThemeResource(HTResId theme_id, const char* theme_name, const TRThemeItem* items);
		void addDrawableSetGroupResource(HTResId drsetgroup_id, HTResId drsetgroup_super_id, const TRDrawableSetGroupItem * items);
		void addDrawableSetGroupResource(TRDrawableSetGroupArrayItem* all_groups, int size);

		/*
		 * res get related
		 */
		void *getRes(HTResId id);
		const char *getText(HTResId id);
		Style *getStyle(HTResId id);
		Image *getImage(HTResId id);
        Bitmap *getBitmap(HTResId id);
		GifAnimate *getGifAnimate(HTResId id);
	
		Logfont *getFont(HTResId id);

		CB_CREATE_VIEW getUi(HTResId id);
		CB_CREATE_MENU getMenu(HTResId id);
		View *createView(HTResId id, View *parent, ViewContext *viewContext, ContentProvider *provider = NULL);
		Menu *createMenu(HTResId id, Menu *parent, EventListener* listener);
		Drawable* getDrawable(HTResId id);
		DrawableSet* getDrawableSet(HTResId);
		DrawableSetGroup* getDrawableSetGroup(HTResId);
		ThemeRes* getThemeRes(HTResId);		
		DrawableSet* getThemeDrawableSet(int theme_drset_id);
		//DrawableSet* getDrawableSetByClass(const char* viewclass_name);
		//DrawableSet* getCommonDrawableSet();
		/*
		 * getDrawableSelector
		 */
		//DrawableSelector * getDrawableSelector(const char* viewclass_name, HTResId drset_id);


		/*
		 * text & lang & encoding related
		 */
		//TODO
		void setCurrentLang(enum ResLang lang, enum ResEncoding enc);

		/*
		 * Theme apis
		 */
		bool setTheme(HTResId theme_res_id, bool update_system = TRUE);
		HTResId themeId(void);
		ThemeRes* theme(void);

	private:
		ResEntry m_resources[NR_RES_TYPE_MAX];
	    const TResourceEntry *m_imageTResourceEntry;
		MENU_RES_ARRAY *m_pMenuResArray;
		unsigned int    m_MenuArrayCount;
		
		UI_RES_ARRAY *m_pUIResArray;
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

		string m_packagePath;
        //DrawableSetMap m_setMap;
        //DrawableSet *m_commonSet;
        ThemeRes* m_theme;
		HTResId m_theme_id;

	protected:
		int m_id;
		string m_name;
		const TResourceEntry *m_curLangTextRes;
};

string GetResRealPath(const char *packagePath, const char *resFilename);

bool RegisterCommonDrawableFromRes(int dr_id, const char* basename, const TRStyleElement *elements, int super_id);

bool RegisterViewDrawableSetFromRes(const char * view_name,HTResId super_drset, const TRDrawableSetItem *items);

bool RegisterViewDrawableSetGroupFromRes(const char * view_name, HTResId super_drsetgroup, const TRDrawableSetGroupItem *items);


} // namespace hfcl

#endif /* __RESPACKAGE_H */

