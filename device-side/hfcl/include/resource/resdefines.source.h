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

//#ifndef RES_DEFINE_SOURCE_H
//#define RES_DEFINE_SOURCE_H


//package resource
#define SYSRESID(name) R_sys_##name
#define begin_respkg(name, id)

#define end_respkg

//package sys package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg
#define set_common_style(style_id) 

//internal package resource
#define def_name(name)

//text resource
#define begin_text_res(lang, encoding) \
    static const TResourceEntry text_##lang##encoding##res[] = {

#define end_text_res {0, NULL} };

//#define text(name, text_value)  { RESID(txt_##name), text_value },

#define extern_lang 1

//image resource
#define begin_image_res() \
    static const TResourceEntry image_res[] = {

#define end_image_res {0, NULL} };

#define image(name, img_file) { RESID(img_##name), img_file },
#define bmpfont_image(name, img_file) { RESID(img_bmpfont_##name), img_file},

//font resource
#define begin_font_res() \
    static const TResourceEntry font_res[] = {

#define end_font_res {0, ""} };

#define font(name, font_name) { RESID(font_##name), font_name },
#define bmpfont(name, font_name) { RESID(bmpfont_##name), font_name },

#if 0
//bmpfont resource
#define begin_bmpfont_res() \
    static const TResourceEntry bmpfont_res[] = {

#define end_bmpfont_res {0, ""} };

#define bmpfont(name, font_name) { RESID(bmpfont_##name), font_name },

//... could be RESID(img_bmpfont_time_colon);
#define bmpfont(name, ...)  \
    { RESID(font_##name), __VA_ARGS__ }
#endif
//devbmp font resource
#define begin_devbmpfont(devfont_name, font_name) \
	static const TRBMPDevFontItemInfo devbmpfont_##devfont_name##_item_infos[] = {

#define end_devbmpfont  {NULL, 0, 0, 0 } };
#define devbmpfont(start_mchar, num_char, glyph_width, img_id) { \
	(const char*)(start_mchar),  \
	(unsigned int)(num_char), \
	(int)(glyph_width), \
	(HTResId)(img_id) },

//sys resource
#define sys_style(name)    GetStyleRes(SYSRESID(style_##name))
#define sys_image(name)    GetImageRes(SYSRESID(img_##name))
#define sys_bmpfont_image(name)     (SYSRESID(img_bmpfont_##name))
#define sys_font(name)     GetFontRes(SYSRESID(font_##name))
#define sys_bmpfont(name)           (SYSRESID(bmpfont_##name))
#define sys_dr(name)       GetDrawableRes(SYSRESID(dr_##name))

//style resource
#define self_style(name)    GetStyleRes(RESID(style_##name))
#define self_image(name)    GetImageRes(RESID(img_##name))
#define self_gif(name)    GetGifAnimateRes(RESID(img_##name))
#define self_bmpfont_image(name)     (RESID(img_bmpfont_##name))
#define self_font(name)     GetFontRes(RESID(font_##name))
#define self_bmpfont(name)     (RESID(bmpfont_##name))
#define self_dr(name)       GetDrawableRes(RESID(dr_##name))

#define rgba(r, g, b, a)   (((r) & 0xFF) | (((g) & 0xFF) << 8) | (((b) & 0xFF) << 16) | (((a) & 0xFF) << 24))


#define begin_styles
#define end_styles

#define begin_style_res(name, super_id) \
	const TRStyleElement style_##name##_elements_res[] = {

#define begin_common_style \
	const TRStyleElement style_c_o_m_m_o_n_elements_res[] = {


#define end_style_res    {-1, 0 } };
#define end_common_style {-1, 0 } };

//#define style(id, status, value)   { (Style::ElementId)id, (Style::ElementState)status, value },
//#define style_color_rgba(id, status, r, g, b, a)    {(Style::ElementId)id, (Style::ElementState)status, rgba(r, g, b, a) },
//#define style_color(id, status, r, g, b)    style_color_rgba((Style::ElementId)id, (Style::ElementState)status, r, g, b, 0xFF)
//#define style_image(id, status, img_id)    { (Style::ElementId)id, (Style::ElementState)status, (DWORD)img_id },
//#define style_font(id, status, font_id)    { (Style::ElementId)id, (Style::ElementState)status, (DWORD)font_id },
#define style(id, value)                    { (int)id, (DWORD)(value) },
#define style_color_rgba(id, r, g, b, a)    { (int)id, (DWORD)rgba(r, g, b, a) },
#define style_color(id, r, g, b)            style_color_rgba(id, r, g, b, 0xFF)
#define style_image(id, img_id)             { (int)id, (DWORD)(img_id) },
#define style_font(id,  font_id)            { (int)id, (DWORD)(font_id) },

//sys HTResId by name
#define sysid_font(name)  (SYSRESID(font_##name))
#define sysid_image(name) (SYSRESID(img_##name))
//#define sysid_text(name)  (SYSRESID(txt_##name))
#define sysid_style(name) (SYSRESID(style_##name))
#define sysid_dr(name)    (SYSRESID(dr_##name))
#define sysid_drset(name) (SYSRESID(drset_##name))
//get HTResId by name
#define my_font(name) (RESID(font_##name))
#define my_image(name) (RESID(img_##name))
//#define my_text(name) (RESID(txt_##name))
#define my_style(name) (RESID(style_##name))
#define my_dr(name) (RESID(dr_##name))
#define my_drset(name) (RESID(drset_##name))
#define my_drgroup(name) (RESID(drsetgroup_##name))
#define my_menu(name) (RESID(menu_##name))
#define my_ui(name) (RESID(ui_##name))

//drawable resource
#define begin_dr_res \
	static const TRDrawableArrayItem drawable_res[] = {

#define end_dr_res \
	{-1} };

#define drawable(name, basename, super_style_id) 

#define begin_drawable(name, basename, super_style_id) \
	{ RESID(dr_##name), basename, super_style_id, {

#define end_drawable {-1, 0} }, },

#define begin_common_drawable_res
#define end_common_drawable_res


#define begin_common_drawable(common_id, basename, super_style_id) \
	static const struct TRStyleElement drawable_style_##common_id##_info[] = {

#define end_common_drawable \
	{-1, 0} };

#define begin_drset_res \
	static const TRDrawableSetArrayItem drawableset_res[] = {

#define end_drset_res \
	{-1} };

#define begin_drset(name, super_drset) \
	{ RESID(drset_##name), super_drset, {

//#define end_drset  {-1, 0 } };
#define end_drset  {-1, 0 } }, },

#define dr(id, dr_id)   {(int)(id),  (HTResId)(dr_id)},

#define begin_common_drset_res
#define end_common_drset_res
#define begin_common_drset(view, super_drset) \
	static const TRDrawableSetItem drset_view_##view##_info[] = {

#define end_common_drset \
	{-1, 0 } }; 

/////////////////////////////////
//drawable set group
// #define begin_drsetgroup_res
#define begin_drsetgroup_res \
	static const TRDrawableSetGroupArrayItem drawablesetgroup_res[] = {

#define begin_drsetgroup(name, super_drsetgroup) \
	{ RESID(drsetgroup_##name), super_drsetgroup, {

		
//#define end_drsetgroup {-1, 0} };
#define end_drsetgroup {-1, 0} }, },

#define drset(id, drset_id) { (int)(id), (HTResId)(drset_id) },

#define begin_common_drsetgroup(view, super_group) \
		static const TRDrawableSetGroupItem drsetgroup_common_view_##view##_info[] = {

#define begin_common_drsetgroup_res

#define end_common_drsetgroup_res

//#define end_common_drsetgroup end_drsetgroup
#define end_common_drsetgroup \
	{-1, 0 } };

//#define end_drsetgroup_res
#define end_drsetgroup_res \
	{-1} };

//theme
#define begin_theme_res
#define begin_theme(name) \
	static const TRThemeItem theme_res_declare_##name##_info[] = {

#define map_theme_drset(theme_drset_id, drset_id) {theme_drset_id, drset_id},

#define end_theme {-1, -1} };

#define end_theme_res(def_theme_name)

////////////////////////////////


//audio res : temp
#define begin_audio_res

#define audio(input, output)

#define end_audio_res


//Menu resource
#define begin_menus
#define end_menus

#define begin_menu_res(name) \
    static Menu* create_menu_##name(Menu* parent, EventListener* listener) { \
        Menu* ret_menu = NULL; \
        Menu* _parent = parent;

#define end_menu_res    return ret_menu; }

#define begin_menu() \
    do { Menu* __menu = HFCL_NEW_EX(Menu, ()); \
        setMenu(Parent, _parent) \
        _parent = __menu;

#define begin_theme_menu(list_theme_drset_id, item_theme_drset_id, menubar_theme_drset_id) \
	do { Menu* __menu = HFCL_NEW_EX(Menu, (list_theme_drset_id, item_theme_drset_id, menubar_theme_drset_id)); \
		setMenu(Parent, _parent) \
		_parent = __menu;

#define end_menu ret_menu = __menu; \
    } while(0);

#define setMenuRect(l, t, r, b) 	__menu->setMenuRect(l, t, r, b);
#define setOnMenuListener()    		__menu->addEventListener(listener);
#define setMenu(Name, value)    	__menu->setMenu##Name(value);
#define menu_set(Name, value)   	__menu->set##Name(value);
#define menuAutoFit()           	__menu->autoFit();
#define menuHilightFirstEnable()    __menu->hilightFirstEnable();

#define addMenuItemEx(str, image, id, disable, drset)    __menu->addCommandMenuItem(str, image, (int)(id), disable, drset);
#define addMenuItem(str, image, id)   addMenuItemEx(str, image, (int)(id), false, NULL)
#define addCheckItemEx(str, image, id, checked, disable, drset)    __menu->addCheckMenuItem(str, image, (int)(id), disable, checked, drset);
#define addCheckItem(str, image, id, checked) addCheckItemEx(str, image, id, checked, false, NULL)
#define addRadioItemEx(str, image, id, tag, checked, disable, drset)    __menu->addRadioMenuItem(str, image, (int)(id), tag, disable, checked, drset);
#define addRadioItem(str, image, id, tag, checked)  addRadioItemEx(str, image, id, tag, checked, false, NULL)

#define addSubMenuEx(str, image, id, disable, drset)  __menu->addSubMenuItem(str, image, (int)id, (Menu*)(ret_menu), disable, drset);
#define addSubMenu(str, image, id)    addSubMenuEx(str, image, id, false, NULL)


//UI resource
#define begin_uis
#define end_uis

#define begin_ui_res(name)  \
    static View* create_UI_##name(View* parent, ViewContext* viewContext, ContentProvider * __content_provider__) { \
        View *ret_view = NULL, *_parent = NULL, *__view = NULL; \
        __view = (View*) ((DWORD)__view ^ 0x00); /* VW */ \
		_parent= parent; \
		__view = parent;

#define end_ui_res  return ret_view ; }

#define data_get(id, type)  ( __content_provider__ ? __content_provider__->getContentData(id, type) : NULL)
#define data_get_any(id)    data_get(id, 0)
#define data_get_image(id)  (Image*)data_get(id, R_TYPE_IMAGE)
#define data_get_text(id)   (const char *) data_get(id, R_TYPE_TEXT)
#define data_get_text_id(id)   (const int)( __content_provider__ ? __content_provider__->getContentStringId(id, R_TYPE_TEXT) : NULL)
#define data_get_int(id)    (int)data_get(id, 0)

#define begin_view(view_class) \
    do { view_class* __view = HFCL_NEW_EX(view_class, (_parent)); \
        View* _parent = (View*)__view; \
        _parent = _parent;

#define begin_theme_view(view_class, theme_drset_id) \
	    do { view_class* __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSetResFromTheme(theme_drset_id))); \
		__view->themeAble(true); \
		__view->setThemeDrsetId(theme_drset_id); \
        View* _parent = (View*)__view; \
        _parent = _parent;

#define begin_view_ex(view_class, drsetid) \
    do { \
        view_class* __view; \
        __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSetRes(drsetid))); \
        View* _parent = (View*)__view; \
        _parent = _parent;

#define begin_view_id_ex(view_class, view_classname, setid) \
    do { \
        view_class* __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSelector(RESPKGID, #view_classname, setid))); \
        View* _parent = (View*)__view; \
        _parent = _parent;

#define begin_scrollable_view(view_class) \
    do { view_class* __view = HFCL_NEW_EX(view_class, (_parent)); \
        ContainerView* _parent = __view->content(); \
        _parent = _parent;

#define begin_theme_scrollable_view(view_class, theme_drset_id) \
    do { view_class* __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSetResFromTheme(theme_drset_id))); \
		__view->themeAble(true); \
		__view->setThemeDrsetId(theme_drset_id); \
        ContainerView* _parent = __view->content(); \
        _parent = _parent;

#define begin_scrollable_view_ex(view_class, view_classname, setid) \
    do { \
        view_class* __view; \
        if(!strcmp(#setid,"0"))\
            __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSelector(RESPKGID, #view_classname, 0))); \
        else\
            __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSelector(RESPKGID, #view_classname, RESID(drset_##setid)))); \
        ContainerView* _parent = __view->content(); \
        _parent = _parent;

#define begin_scrollable_view_id_ex(view_class, view_classname, setid) \
    do { \
        view_class* __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSelector(RESPKGID, #view_classname, setid))); \
        ContainerView* _parent = __view->content(); \
        _parent = _parent;

#define begin_item(view_class) \
    do { view_class* __view = HFCL_NEW(view_class); \
        ((ListView*)_parent)->addItem((ItemView*)__view); \
        View *_parent = (View*)__view; 

#define begin_theme_item(view_class, theme_drset_id) \
    do { view_class* __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSetResFromTheme(theme_drset_id))); \
        __view->themeAble(true); \
        __view->setThemeDrsetId(theme_drset_id); \
        ((ListView*)_parent)->addItem((ItemView*)__view); \
        View* _parent = (View*)__view; \

#define begin_item_ex(view_class, drset) \
    do { view_class* __view = HFCL_NEW_EX(view_class, (NULL, GetDrawableSetRes(drset))); \
        ((ListView*)_parent)->addItem((ItemView*)__view); \
        View *_parent = (View*)__view;

#define begin_composite_view(view_class, drset_group_id) \
    do { view_class* __view = HFCL_NEW_EX(view_class, (_parent, GetDrawableSetGroupRes(drset_group_id))); \
        View* _parent = (View*)__view; 

#define end_item end_view

#define end_composite_view end_view


#define end_view  ret_view = (View*)__view; } while(0);
#define end_scrollable_view  __view->updateContentRect();  ret_view = (View*)__view; } while(0);
#define end_scrollable_view_ex  __view->updateContentRect(); ret_view = (View*)__view; } while(0);
#define end_scrollable_view_id_ex  __view->updateContentRect(); ret_view = (View*)__view; } while(0);

#define hide()   __view->hide();

#define setSBVLocation(location)  __view->setSBView(location, ret_view);
#define add(Name)   __view->add##Name(ret_view);
#define addLayoutView(__type, __min, __max)   __view->addView(ret_view, __type, __min, __max);

//import the other view
#define import_view(view_id) do { \
	CB_CREATE_VIEW _create_view = (CB_CREATE_VIEW)GetRes(view_id); \
	if(_create_view) \
		ret_view = _create_view((View*)_parent, viewContext, __content_provider__); \
	}while(0);

#define begin_import_view(ViewClass, view_id) do { \
	CB_CREATE_VIEW _create_view = (CB_CREATE_VIEW)GetRes(view_id); \
	if(_create_view) { \
		ViewClass* __view =  (ViewClass*)_create_view((View*)_parent, viewContext, __content_provider__); \
		if(__view) { \
			View* _parent = NULL; _parent = (View*)__view; 

#define end_import_view ret_view = (View*)__view; } } } while(0);
	
//controller
#define begin_mode(name)  \
		static ControllerMode name##_mode[] = {

#define end_mode };

#define setMode(id, value, cmd) { (int)(id), (unsigned int)(value), (unsigned int)(cmd) },

#define begin_controller_modes \
	static ControllerModeList _controller_mode_lists[] = {

#define end_controller_modes(def_mode) \
	}; \
	static ControllerModeManager _controller_mode_manager = { \
		_controller_mode_lists, \
		sizeof(_controller_mode_lists) / sizeof(ControllerModeList), \
		RESID(mode_##def_mode) }; \
	if(viewContext) viewContext->setControllerModeManager(&_controller_mode_manager);
	

#define add_mode(name)  { RESID(mode_##name), sizeof(name##_mode) / sizeof(ControllerMode), name##_mode },
		
/*
//#define addMenuItem(str, id) do { \
                                    MenuItem __menuitem(#str, id); \
                                    __view->addMenuItem( __menuitem); \
                                } while(0);

#define addSubMenu(id) __view->addSubMenu((MenuView *)ret_view, id);
*/

#define set(Name, value)    __view->set##Name(value);
#define addTablePage( value1, value2, value3)    __view->addTablePage(value1, value2, value3);
#define setRect(left, top, right, bottom)    __view->setRect(left, top, right, bottom);
#define setRectWH(left, top, width, height)    __view->setRectWH(left, top, width, height);   //JYSEO_0218_add macro

#define setStyle(style)    __view->setStyle(style);
#define _m(Name, args)    __view->Name args;
#define id(id)    __view->setId(id);
#define map(name_id)    if (viewContext) { __view->setId(name_id); viewContext->setView(name_id, __view); }
#define on(event_type, handle_id)    if (viewContext) { \
    EventListener* listener = viewContext->getHandle(handle_id, event_type); \
    if (listener) __view->addEventListener(listener); }

#define begin_foreach(n)  do { \
	unsigned int i = 0; \
	for(i = 0; i < n; i++) { \
	    char c[4];\
	    sprintf(c,"%d", i+1);\

#define end_foreach   } } while(0);
	
#define my(name)    RESID(var_##name)

#define begin_resid(name) static  HTResId name[] = {
#define end_resid  };
#define my_imageid(name) RESID(img_##name),
#define my_textid(name)    name,
//#endif  /* RES_DEFINE_SOURCE_H */

