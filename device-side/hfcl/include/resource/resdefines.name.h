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

#define RESDEFINES_NAME

// resource package
#define SYSRESID(name) R_sys_##name

#if !defined(RESPKGID) || !defined(RESID)
#   error "Please define RESPKGID and RESID before including this file"
#endif

#define begin_respkg(name, id) \
    enum { RESID(var_begin_id) = MAKE_RES_ID(RESPKGID, 0, 0), \

#define end_respkg RESID(var_end_id) };

// system package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg

// static global array const variables
#define def_static_array_real(name, ...)
#define def_static_array_int(name, ...)
#define def_static_array_str(name, ...)

#define array_real(name)
#define array_int(name)
#define array_str(name)

// id of variable
#define def_name(name)  RESID(var_##name),

// system HTResId by name
#define sysid_font(name)
#define sysid_image(name)
#define sysid_css(name)
#define sysid_css_group(name)

// local HTResId by name
#define my_var(name)
#define my_font(name)
#define my_image(name)
#define my_css(name)
#define my_css_group(name)
#define my_client(name)
#define my_menu(name)

// text resource
#define begin_text_res()
#define text_res_raw(lang, encoding, txt_file)
#define text_res_zipped(lang, encoding, txt_file)
#define text_res_gnumsg(lang, encoding, txt_file)
#define end_text_res

//image resource
#define begin_image_res()
#define end_image_res

#define image(name, img_file)
#define bmpfont_image(name, img_file)

// font resource
#define begin_font_res()
#define end_font_res

#define font(name, font_name)
#define bmpfont(name, font_name)

// dev bmp font
#define begin_devbmpfont(devfont_name, font_name)
#define end_devbmpfont
#define devbmpfont(s,n,g,i)

// sys resource
#define sys_style(name)
#define sys_image(name)
#define sys_font(name)

// style resource
#define self_style(name)
#define self_image(name)
#define self_gif(name)
#define self_font(name)

#define rgba(r, g, b, a)
#define rgb(r, g, b)

#define begin_css_res
#define begin_css(name, selector)
#define style(pid, value, ...)
#define end_css
#define end_css_res

#define begin_css_group_res
#define begin_css_group(name)
#define css(name)
#define end_css_group
#define end_css_group_res

////////////////////////////////
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
#define begin_clients
#define end_clients

#define apply_css_group(cssg_id)

#define begin_client(name)
#define end_client

#define begin_view(ViewClass)
#define begin_view_ex(ViewClass, css_class, vname, vid)
#define end_view

#define import_view(view_resid)
#define begin_import_view(ViewClass, view_resid)
#define end_import_view

#define set(Name, value)
#define attribute(name, value)
#define mystyle(pid, value, ...)
#define map(name)
#define on(event_name, handle_name)

#define begin_foreach(n)
#define end_foreach

#define data_get(id, type)
#define data_get_any(id)
#define data_get_image(id)
#define data_get_text(id)
#define data_get_text_id(id)
#define data_get_int(id)

#define begin_resid(name)
#define end_resid

#define begin_controller_modes
#define end_controller_modes(def_mode)
#define begin_mode(name)  RESID(mode_##name),
#define end_mode
#define setMode(id, value,cmd)
#define add_mode(name)

