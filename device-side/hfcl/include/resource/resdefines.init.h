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

#define RESDEFINES_INIT

// resource package
#define SYSRESID(name) R_sys_##name

#if !defined(RESPKGID) || !defined(RESID)
#   error "Please define RESPKGID and RESID before including this file"
#endif

#define begin_respkg(name, id) \
    bool __hfr_register_resource_##name(void) { \
        ResPackage* resPkg = HFCL_NEW_EX(ResPackage, (#name, id)); \
        if (!RegisterResPackage(resPkg)) \
            return false;

#define end_respkg return true; }

// system resouce package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg

// static global array const variables
#define def_static_array_real(name, ...)
#define def_static_array_int(name, ...)
#define def_static_array_str(name, ...)

#define array_real(name) _var_array_real_##name
#define array_int(name) _var_int_int_##name
#define array_str(name) _var_int_str_##name

// id of variable
#define def_name(name)

// system HTResId by name
#define sysid_font(name)        (SYSRESID(font_##name))
#define sysid_image(name)       (SYSRESID(img_##name))
#define sysid_css(name)         (SYSRESID(css_##name))
#define sysid_css_group(name)   (SYSRESID(css_group_##name))

// local HTResId by name
#define my_font(name)           (RESID(font_##name))
#define my_image(name)          (RESID(img_##name))
#define my_css(name)            (RESID(css_##name))
#define my_css_group(name)      (RESID(css_group_##name))
#define my_client(name)         (RESID(client_##name))
#define my_menu(name)           (RESID(menu_##name))

// sys resource
#define sys_css(name)           GetCssRes(SYSRESID(css_##name))
#define sys_css_group(name)     GetCssGroupRes(SYSRESID(css_group_##name))
#define sys_image(name)         GetImageRes(SYSRESID(img_##name))
#define sys_font(name)          GetFontRes(SYSRESID(font_##name))

// resource object
#define self_css(name)          GetCssRes(RESID(css_##name))
#define self_css_group(name)    GetCssGroupRes(RESID(css_group_##name))
#define self_image(name)        GetImageRes(RESID(img_##name))
#define self_gif(name)          GetGifAnimateRes(RESID(img_##name))
#define self_font(name)         GetFontRes(RESID(font_##name))

// text resource
#define begin_text_res()

#define text_res_raw(lang, encoding, txt_file) \
    resPkg->addTextResRaw(lang, encoding, txt_file);

#define text_res_zipped(lang, encoding, txt_file) \
    resPkg->addTextResZipped(lang, encoding, txt_file);

#define text_res_gnumsg(lang, encoding, txt_file) \
    resPkg->addTextResGnuMsg(lang, encoding, txt_file);

#define end_text_res

// image resource
#define begin_image_res() \
    resPkg->addImageResource(image_res);

#define end_image_res

#define image(name, img_file)
#define bmpfont_image(name, img_file)

// font resource
#define begin_font_res() \
    resPkg->addFontResource(font_res);

#define end_font_res

#define font(name, font_name)
#define bmpfont(nmae, font_name)

// devbmpfont info
#define begin_devbmpfont(devfont_name, font_name) \
    resPkg->addBMPDevFont(font_name, devbmpfont_##devfont_name##_item_infos);

#define end_devbmpfont
#define devbmpfont(start_mchar, num_char, glyph_width, img_id)

#define rgba(r, g, b, a)                \
    ((HTUint)(                          \
        (((HTUint)(r) & 0xFF)) |        \
        (((HTUint)(g) & 0xFF) << 8) |   \
        (((HTUint)(b) & 0xFF) << 16) |  \
        (((HTUint)(a) & 0xFF) << 24)    \
    ))

#define rgb(r, g, b)    rgba(r, g, b, 0xFF)

#define begin_css_res

#define begin_css(name, selector) {              \
    CssDeclared* curr_css =                      \
        HFCL_NEW_EX(CssDeclared, (selector));    \
    resPkg->addCssResource(curr_css);

#define style(pid, value, ...) \
    curr_css->set##pid((value), ##__VA_ARGS__);

#define end_css }

#define end_css_res

/////////////////////////////////
// css group
#define begin_css_group_res

#define begin_css_group(name) {                         \
    CssDeclaredGroup* curr_css_group =           \
        HFCL_NEW(CssDeclaredGroup);              \
    resPkg->addCssGroupResource(curr_css_group);

#define css(name) \
    curr_css_group->append(self_css(name));

#define end_css_group }

#define end_css_group_res

// client resource
#define begin_client_res \
    static CLIENT_RES_ARRAY ClientArray[] = {

#define end_client_res \
    }; \
    resPkg->addClientResource(ClientArray, sizeof(ClientArray)/sizeof(CLIENT_RES_ARRAY));

#define apply_css_group(cssg_id)

#define begin_client(name)  \
        { RESID(client_##name), (void *)create_client_##name},

#define end_client

#define begin_view(ViewClass)
#define begin_view_ex(ViewClass, view_css_class, vname, vid)
#define end_view

#define import_view(view_res_id)
#define begin_import_view(ViewClass, view_resid)
#define end_import_view

#define set(Name, value)
#define attribute(name, value)
#define mystyle(pid, value, ...)

#define map(name)
#define on(event_name, handle_name)

// get data from id
#define data_get(id, type)
#define data_get_any(id)
#define data_get_image(id)
#define data_get_text(id)
#define data_get_text_id(id)
#define data_get_int(id)

#define begin_foreach(n)
#define end_foreach

#define begin_resid(name)
#define end_resid

// controller
#define begin_controller_modes
#define end_controller_modes(def_mode)

#define begin_mode(name)
#define end_mode

#define setMode(id, value, cmd)
#define add_mode(name)

// menu resource
#define begin_menus \
    static MENU_RES_ARRAY menuArray[] = {

#define end_menus \
    }; \
    resPkg->addMenuResource(menuArray, \
            sizeof(menuArray)/sizeof(MENU_RES_ARRAY));

#define begin_menu_res(name) \
    {RESID(menu_##name),(void *)create_menu_##name},

#define end_menu_res

#define begin_menu()
#define end_menu

#define setMenuRect(l, t, r, b)
#define setOnMenuListener()
#define setMenu(Name, value)
#define menu_set(Name, value)

#define addMenuItemEx(str, image, id, disable, drset)
#define addMenuItem(str, image, id)
#define addRadioItemEx(str, image, id, tag, checked, disable, drset)
#define addRadioItem(str, image, id, tag, checked)
#define addCheckItemEx(str, image, id, checked, disabled, drset)
#define addCheckItem(str, image, id, checked)
#define addSubMenuEx(str, imge, id, disable, drset)
#define addSubMenu(str, imge, id)

