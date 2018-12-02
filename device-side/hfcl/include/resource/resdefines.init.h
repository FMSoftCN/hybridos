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

//#ifndef RES_DEFINE_INIT_H
//#define RES_DEFINE_INIT_H


//package resource
#define SYSRESID(name) R_sys_##name
#define begin_respkg(name, id) \
    bool FRRegister_##name##_resource(void) { \
        ResPackage* resPkg = NGUX_NEW_EX(ResPackage, (#name, id)); \
		if(!RegisterResPackage(resPkg)) return false;

#define end_respkg return true; }

#define data_get(id, type)  
#define data_get_any(id)   
#define data_get_image(id)
#define data_get_text(id)
#define data_get_text_id(id)
#define data_get_int(id)

//package sys package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg
#define set_common_style(style_id)  if(RESPKGID == 0) {Style::SetCommonStyle(GetStyleRes(style_id));}

//internal package resource
#define def_name(name)


//text resource
#define begin_text_res(lang, encoding) \
    resPkg->addTextResource(R_LANG_##lang, R_ENCODING_##encoding, text_##lang##encoding##res);

#define end_text_res

//#define text(name, text_value)

#define extern_lang    1
// #undef extern_lang


//image resource
#define begin_image_res() \
    resPkg->addImageResource(image_res);

#define end_image_res

#define image(name, img_file)
#define bmpfont_image(name, img_file)


//font resource
#define begin_font_res() \
    resPkg->addFontResource(font_res);

#define end_font_res

#define font(name, font_name)
#define bmpfont(nmae, font_name)

//devbmpfont info
#define begin_devbmpfont(devfont_name, font_name) resPkg->addBMPDevFont(font_name, devbmpfont_##devfont_name##_item_infos);

#define end_devbmpfont
#define devbmpfont(start_mchar, num_char, glyph_width, img_id)

//sys resource
#define sys_style(name)    GetStyleRes(SYSRESID(style_##name))
#define sys_image(name)    GetImageRes(SYSRESID(img_##name))
#define sys_font(name)     GetFontRes(SYSRESID(font_##name))

//style resource
#define self_style(name)    GetStyleRes(RESID(style_##name))
#define self_image(name)    GetImageRes(RESID(img_##name))
#define self_gif(name)    GetGifAnimateRes(RESID(img_##name))
#define self_font(name)     GetFontRes(RESID(font_##name))

#define rgba(r, g, b, a)

#define begin_styles

#define end_styles

#define begin_style_res(name, super_id) \
    resPkg->addStyleResource(RESID(style_##name), \
			super_id, \
			style_##name##_elements_res, \
			sizeof(style_##name##_elements_res)/sizeof(TRStyleElement));

#define end_style_res

#define begin_common_style \
	resPkg->addStyleResource(0, 0, \
			style_c_o_m_m_o_n_elements_res, \
			sizeof(style_c_o_m_m_o_n_elements_res)/sizeof(TRStyleElement));

#define end_common_style

//#define style(id, status, value)
//#define style_color_rgba(id, status, r, g, b, a)
//#define style_color(id, status, r, g, b)
//#define style_image(id, status, img_id)
//#define style_font(id, status, font_id)

#define style(id, value)
#define style_color_rgba(id, r, g, b, a)
#define style_color(id, r, g, b)
#define style_image(id, img_id)
#define style_font(id, font_id)


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
#define my_menu(name)  (RESID(menu_##name))
#define my_ui(name) (RESID(ui_##name))


//drawable resource
#define begin_dr_res \
	resPkg->addDrawableResource((TRDrawableArrayItem*)&drawable_res, sizeof(drawable_res));
#define end_dr_res

#define begin_drawable(name, basename, super_style_id)

#define end_drawable

#define begin_common_drawable_res
#define end_common_drawable_res

#define begin_common_drawable(common_id, basename, super_style_id) \
	RegisterCommonDrawableFromRes(common_id, basename, drawable_style_##common_id##_info, super_style_id);

#define end_common_drawable

#define begin_drset_res \
	resPkg->addDrawableSetResource((TRDrawableSetArrayItem *)&drawableset_res, sizeof(drawableset_res));

#define end_drset_res

#define begin_drset(name, super_drset)

#define end_drset

#define dr(id, dr_id)

#define begin_common_drset_res
#define end_common_drset_res
#define begin_common_drset(view, super_drset) \
	RegisterViewDrawableSetFromRes(#view, super_drset, drset_view_##view##_info);	
#define end_common_drset end_drset

/////////////////////////////////
//drawable set group
#define begin_drsetgroup_res \
	resPkg->addDrawableSetGroupResource((TRDrawableSetGroupArrayItem *)drawablesetgroup_res, sizeof(drawablesetgroup_res));

#define begin_drsetgroup(name, super_drsetgroup)

#define end_drsetgroup 

#define drset(id, drset_id) 

#define begin_common_drsetgroup_res

#define end_common_drsetgroup_res

#define begin_common_drsetgroup(view, super_group) \
	RegisterViewDrawableSetGroupFromRes(#view, super_group, drsetgroup_common_view_##view##_info);

#define end_common_drsetgroup end_drsetgroup

#define end_drsetgroup_res  

//////////////

//theme
#define begin_theme_res

#define begin_theme(name)	\
	resPkg->addThemeResource(RESID(theme_##name), "", theme_res_declare_##name##_info);

#define map_theme_drset(theme_drset_id, drset_id)

#define end_theme

#define end_theme_res(def_theme_name)	\
	resPkg->setTheme(RESID(theme_##def_theme_name), FALSE);

////////////////////////////////

//audio res : temp
#define begin_audio_res

#define audio(input, output)

#define end_audio_res


//Menu resource
#define begin_menus \
	static MENU_RES_ARRAY menuArray[] = { 

#define end_menus \
}; \
 resPkg->addMenuResource(menuArray, sizeof(menuArray)/sizeof(MENU_RES_ARRAY));



#define begin_menu_res(name) \
	{RESID(menu_##name),(void *)create_menu_##name},


#define end_menu_res \


#define begin_menu()
#define begin_theme_menu(list_theme_drset_id, item_theme_drset_id, menubar_theme_drset_id)
#define end_menu

#define setMenuRect(l, t, r, b)
#define setOnMenuListener()
#define setMenu(Name, value)
#define menu_set(Name, value)
#define menuAutoFit()
#define menuHilightFirstEnable()

#define addMenuItemEx(str, image, id, disable, drset)
#define addMenuItem(str, image, id)
#define addRadioItemEx(str, image, id, tag, checked, disable, drset)
#define addRadioItem(str, image, id, tag, checked)
#define addCheckItemEx(str, image, id, checked, disabled, drset)
#define addCheckItem(str, image, id, checked)
#define addSubMenuEx(str, imge, id, disable, drset)
#define addSubMenu(str, imge, id)


//UI resource
#define begin_uis \
	static UI_RES_ARRAY UIArray[] = { 

#define end_uis \
}; \
 resPkg->addUIResource(UIArray, sizeof(UIArray)/sizeof(UI_RES_ARRAY));

#define begin_ui_res(name)  \
		{RESID(ui_##name),(void *)create_UI_##name},

    //resPkg->addUIResource(RESID(ui_##name), create_UI_##name);

#define end_ui_res

#define begin_view(view_class)
#define begin_theme_view(view_class, theme_drset_id)
#define begin_view_ex(view_class, dr_id)
//#define begin_view_id_ex(view_class, view_classname, setid)
#define begin_composite_view(view_class, drsetgrpid)
#define end_composite_view

#define begin_scrollable_view(view_class)
#define begin_theme_scrollable_view(view_class, theme_drset_id)
#define begin_scrollable_view_ex(view_class, view_classname, setid)
#define begin_scrollable_view_id_ex(view_class, view_classname, setid)

#define end_view
#define end_scrollable_view
#define end_scrollable_view_ex
#define end_scrollable_view_id_ex

#define begin_item(view_class)
#define begin_theme_item(view_class, theme_drset_id)
#define begin_item_ex(view_class, drset)
#define end_item

#define begin_controller_modes
#define end_controller_modes(def_mode)
#define begin_mode(name)
#define end_mode
#define setMode(id, value, cmd)
#define add_mode(name)


#define set(Name, value)
#define addTablePage(imageNormal, imageSel, view)
#define setRect(left, top, right, bottom)
#define setRectWH(left, top, width, height)	//JYSEO_0218_add macro
#define setStyle(style_id)
#define _m(Name, args)
#define map(name)
#define id(id)
#define on(event_name, handle_name)
#define hide()
#define add(Name)
#define setSBVLocation(location)
// #define addSubMenu(id)
#define addLayoutView(__type, __min, __max)
#define import_view(view_id)
#define begin_import_view(ViewClass, view_id)
#define end_import_view
#define begin_foreach(n)
#define end_foreach
#define begin_resid(name) 
#define end_resid  
#define my_imageid(name) 
#define my_textid(name)    
//#endif  /* RES_DEFINE_INIT_H */

