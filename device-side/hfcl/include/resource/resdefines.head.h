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

//package resource
#define SYSRESID(name) R_sys_##name
#define begin_respkg(name, id) \
    enum { RPKG_##name = id }; \
    bool FRRegister_##name##_resource(void);

#define end_respkg

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


//internal package resource
#define def_name(name)


//text resource
#define begin_text_res() \
    enum { RESID(txt_res_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_VOID, 0),

#define text_res_raw(lang, encoding, txt_file)
#define text_res_zipped(lang, encoding, txt_file)
#define text_res_gnumsg(lang, encoding, txt_file)

#define end_text_res  RESID(txt_res_end_id) };

#undef extern_lang


//image resource
#define begin_image_res() \
    enum { RESID(img_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_IMAGE, 0),

#define end_image_res  RESID(img_end_id) };

#define image(name, img_file) RESID(img_##name),
#define bmpfont_image(name, img_file) RESID(img_bmpfont_##name),


//font resource
#define begin_font_res() \
    enum { RESID(font_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_FONT, 0),

#define end_font_res RESID(font_end_id) };

#define font(name, font_name) RESID(font_##name),
#define bmpfont(name, font_name) RESID(bmpfont_##name),

//dev bmp font
#define begin_devbmpfont(devfont_name, font_name)
#define end_devbmpfont
#define devbmpfont(s,n,g,i)

//sys resource
#define sys_style_sheet(name)
#define sys_image(name)
#define sys_font(name)

//style resource
#define self_style_sheet(name)
#define self_image(name)
#define self_gif(name)
#define self_font(name)

#define rgba(r, g, b, a)    \
    (((r) & 0xFF) |         \
    (((g) & 0xFF) << 8) |   \
    (((b) & 0xFF) << 16) |  \
    (((a) & 0xFF) << 24))

#define begin_style_sheets \
    enum { RESID(ss_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_STYLE_SHEET, 0),

#define end_style_sheets    RESID(ss_end_id), };

#define begin_style_sheet_res(name)   RESID(style_sheet_##name),

#define end_style_sheet_res

#define style(pid, value, user_data)

//sys HTResId by name
#define sysid_font(name)
#define sysid_image(name)
#define sysid_style_sheet(name)

//get HTResId by name
#define my_font(name)
#define my_image(name)
#define my_style_sheet(name)
#define my_ui(name)

//drawable resource
#define begin_dr_res enum { RESID(dr_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_DRAWABLE, 0),

#define end_dr_res RESID(dr_end_id) };

//drawableset resource
#define begin_drset_res enum { RESID(drset_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_DRAWABLESET, 0),

#define end_drset_res RESID(drset_end_id) };

#define begin_drset(name, viewclass) RESID(drset_##name),

#define end_drset

#define dr(dr_id, name)

#define begin_common_drset_res
#define end_common_drset_res
#define begin_common_drset(view, super)
#define end_common_drset

/////////////////////////////////
//drawable set group
#define begin_drsetgroup_res  enum { RESID(drsetgroup_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_DRSETGROUP, 0),

#define begin_drsetgroup(name, super_drsetgroup) RESID(drsetgroup_##name),

#define end_drsetgroup

#define drset(id, drset_id)

#define begin_common_drsetgroup(view, super_group)
#define end_common_drsetgroup end_drsetgroup

#define end_drsetgroup_res  RESID(drsetgroup_end_id) };

#define begin_common_drsetgroup_res

#define end_common_drsetgroup_res


//////////////

//theme
#define begin_theme_res  enum { RESID(theme_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_THEME, 0),

#define begin_theme(name) RESID(theme_##name),

#define map_theme_drset(theme_drset_id, drset_id)

#define end_theme

#define end_theme_res(def_theme_name) RESID(theme_end_id) };

////////////////////////////////

//audio res : temp
#define begin_audio_res

#define audio(input, output)

#define end_audio_res



//drawable
#define drawable(name, base_dr_id, super_style_id) RESID(dr_##name),

#define begin_drawable(name, base_dr_id, super_style_id) RESID(dr_##name),

#define end_drawable

#define begin_common_drawable_res
#define end_common_drawable_res

#define begin_common_drawable(common_id, basename, super_style_id)

#define end_common_drawable


//Menu resource
#define begin_menus \
    enum { RESID(menu_begin_id) = MAKE_RES_ID(RESPKGID, R_TYPE_MENU, 0),
#define end_menus RESID(menu_end_id) };

#define begin_menu_res(name)  RESID(menu_##name),
#define end_menu_res

#define begin_menu()
#define begin_theme_menu(list_theme_drset_id, item_theme_drset_id, menubar_theme_drset_id)
#define end_menu

#define my_menu(name)

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

#define begin_view(view_class)
#define begin_theme_view(view_class, theme_drset_id)
#define begin_view_ex(view_class, drid)
//#define begin_view_id_ex(view_class, view_classname, setid)
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
#define setRect(left, top, right, bottom)
#define setRectWH(left, top, width, height)
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

