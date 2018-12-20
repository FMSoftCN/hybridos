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

#define RESDEFINES_HEAD

// resource package
#define SYSRESID(name) R_sys_##name

#if !defined(RESPKGID) || !defined(RESID)
#   error "Please define RESPKGID and RESID before including this file"
#endif

#define begin_respkg(name, id) \
    enum { RPKG_##name = id }; \
    bool __hfr_register_resource_##name(void);

#define end_respkg

// system resource package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg

// static global array const variables
#define def_static_array_real(name, ...)
#define def_static_array_int(name, ...)
#define def_static_array_str(name, ...)

#define array_real(name)
#define array_int(name)
#define array_str(name)

// if of variable
#define def_name(name)

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
#define my_ui(name)
#define my_menu(name)

// sys resource
#define sys_css(name)
#define sys_css_group(name)
#define sys_image(name)
#define sys_font(name)

// resource object
#define self_css(name)
#define self_css_group(name)
#define self_image(name)
#define self_gif(name)
#define self_font(name)

// text resource
#define begin_text_res() \
    enum { RESID(txt_res_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_VOID, 0),

#define text_res_raw(lang, encoding, txt_file)
#define text_res_zipped(lang, encoding, txt_file)
#define text_res_gnumsg(lang, encoding, txt_file)

#define end_text_res  RESID(txt_res_end_id) };

// image resource
#define begin_image_res() \
    enum { RESID(img_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_IMAGE, 0),

#define end_image_res  RESID(img_end_id) };

#define image(name, img_file) RESID(img_##name),
#define bmpfont_image(name, img_file) RESID(img_bmpfont_##name),

// font resource
#define begin_font_res() \
    enum { RESID(font_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_FONT, 0),

#define end_font_res RESID(font_end_id) };

#define font(name, font_name) RESID(font_##name),
#define bmpfont(name, font_name) RESID(bmpfont_##name),

// dev bmp font
#define begin_devbmpfont(devfont_name, font_name)
#define end_devbmpfont
#define devbmpfont(s,n,g,i)

#define rgba(r, g, b, a)    \
    (((r) & 0xFF) |         \
    (((g) & 0xFF) << 8) |   \
    (((b) & 0xFF) << 16) |  \
    (((a) & 0xFF) << 24))

/////////////////////////////////
// css
#define begin_css_res       \
    enum { RESID(css_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_CSS, 0),

#define begin_css(name, selector)   RESID(css_##name),

#define style(pid, value, user_data)

#define end_css

#define end_css_res     RESID(css_end_id), };

/////////////////////////////////
// css group
#define begin_css_group_res \
    enum { RESID(cssg_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_CSSGROUP, 0),

#define begin_css_group(name)   RESID(css_group_##name),

#define css(cssid)

#define end_css_group

#define end_css_group_res  RESID(cssg_end_id), };

//Menu resource
#define begin_menus \
    enum { RESID(menu_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_MENU, 0),
#define end_menus RESID(menu_end_id) };

#define begin_menu_res(name)  RESID(menu_##name),
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
#define begin_uis \
    enum { RESID(ui_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_UI, 0),

#define end_uis RESID(ui_end_id) };

#define begin_ui_res(name)  RESID(ui_##name),

#define end_ui_res

// get data from id
#define data_get(id, type)
#define data_get_any(id)
#define data_get_image(id)
#define data_get_text(id)
#define data_get_text_id(id)
#define data_get_int(id)

#define begin_view(view_class)
#define begin_theme_view(view_class, theme_drset_id)
#define begin_view_ex(view_class, drid)
#define end_view
#define begin_composite_view(view_class, drsetgrpid)
#define end_composite_view

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

#define begin_scrollable_view(view_class)
#define begin_theme_scrollable_view(view_class, theme_drset_id)
#define begin_scrollable_view_ex(view_class, view_classname, setid)
#define begin_scrollable_view_id_ex(view_class, view_classname, setid)
#define end_scrollable_view
#define end_scrollable_view_ex
#define end_scrollable_view_id_ex

#define set(Name, value)
#define addTablePage(value1, value2, value3)
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

