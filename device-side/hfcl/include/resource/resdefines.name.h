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

//#ifndef RES_DEFINE_NAME_H
//#define RES_DEFINE_NAME_H


//internal package resource
#define SYSRESID(name) R_sys_##name
#define begin_respkg(name, id) \
    enum { RESID(var_begin_id) = MAKE_RES_ID(RESPKGID, 0, 0), \

#define end_respkg RESID(var_end_id) };

#define data_get(id, type)  
#define data_get_any(id)   
#define data_get_image(id)
#define data_get_text(id)
#define data_get_text_id(id)
#define data_get_int(id)

//package sys package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg
#define set_common_style(style_id) 


#define def_name(name)  RESID(var_##name),


//text resource
#define begin_text_res(lang, encoding)
#define end_text_res

//#define text(name, text_value)

#undef extern_lang


//image resource
#define begin_image_res()
#define end_image_res

#define image(name, img_file)
#define bmpfont_image(name, img_file)


//font resource
#define begin_font_res()
#define end_font_res

#define font(name, font_name)
#define bmpfont(name, font_name)

//dev bmp font
#define begin_devbmpfont(devfont_name, font_name)
#define end_devbmpfont
#define devbmpfont(s,n,g,i)

//sys resource
#define sys_style(name)    
#define sys_image(name)    
#define sys_font(name)        

//style resource
#define self_style(name)
#define self_image(name)
#define self_gif(name)
#define self_font(name)

#define rgba(r, g, b, a)

#define begin_styles
#define end_styles

#define begin_style_res(name, super_id)
#define end_style_res
#define begin_common_style
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

//sys ResID by name
#define sysid_font(name)  
#define sysid_image(name) 
//#define sysid_text(name)  
#define sysid_style(name) 
#define sysid_dr(name)    
#define sysid_drset(name)    
//get ResID by name
#define my_font(name)
#define my_image(name)
//#define my_text(name)
#define my_style(name)
#define my_dr(name)
#define my_drset(name)
#define my_drgroup(name)
#define my_menu(name)
#define my_ui(name)


//drawable resource
#define begin_dr_res
#define end_dr_res
//#define begin_dr(name)
//#define end_dr(name, type)
//#define setdr(prop_id, value)

#define begin_drawable(name, basename, super)
#define end_drawable
#define drawable(name, basename, super_id)
#define begin_common_drawable_res
#define end_common_drawable_res
#define begin_common_drawable(name, basename, super_style_id)
#define end_common_drawable

//drawableset resource
#define begin_drset_res
#define begin_drset(name, viewclassname)
#define dr(dr_id, name)
#define end_drset
#define end_drset_res
#define begin_common_drset_res
#define end_common_drset_res
#define begin_common_drset(view, super_id)
#define end_common_drset
/////////////////////////////////
//drawable set group
#define begin_drsetgroup_res  
#define begin_drsetgroup(name, super_drsetgroup)
#define end_drsetgroup 
#define drset(id, drset_id) 
#define begin_common_drsetgroup(view, super_group)
#define end_common_drsetgroup end_drsetgroup
#define end_drsetgroup_res  
#define begin_common_drsetgroup_res
#define end_common_drsetgroup_res


//////////////

//theme
#define begin_theme_res
#define begin_theme(name)

#define map_theme_drset(theme_drset_id, drset_id)

#define end_theme

#define end_theme_res(def_theme_name)

////////////////////////////////

//audio res : temp
#define begin_audio_res

#define audio(input, output)

#define end_audio_res

//Menu resource
#define begin_menus
#define end_menus

#define begin_menu_res(name)
#define end_menu_res

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
#define begin_uis
#define end_uis

#define begin_ui_res(name)
#define end_ui_res

#define begin_view(view_class)
#define begin_theme_view(view_class, theme_drset_id)
#define begin_view_ex(view_class, setid)
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
#define begin_mode(name)  RESID(mode_##name),
#define end_mode
#define setMode(id, value,cmd)
#define add_mode(name)


#define set(Name, value)
#define addTablePage(value1, value2, value3)
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
//#endif  /* RES_DEFINE_NAME_H */

