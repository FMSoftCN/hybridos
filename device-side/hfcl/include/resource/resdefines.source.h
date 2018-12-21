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

#define RESDEFINES_SOURCE

// resource package
#define SYSRESID(name) R_sys_##name

#if !defined(RESPKGID) || !defined(RESID)
#   error "Please define RESPKGID and RESID before including this file"
#endif

#define begin_respkg(name, id)
#define end_respkg

// system package
#define begin_sys_respkg  begin_respkg(sys, 0)
#define end_sys_respkg  end_respkg

// static global array const variables
#define def_static_array_real(name, ...) \
    static const HTReal _var_array_real_##name[] = { __VA_ARGS__ };

#define def_static_array_int(name, ...) \
    static const int _var_array_int_##name[] = { __VA_ARGS__ };

#define def_static_array_str(name, ...) \
    static const char *_var_array_str_##name[] = { __VA_ARGS__ };

#define array_real(name)
#define array_int(name)
#define array_str(name)

// id of variable
#define def_name(name)

// system HTResId by name
#define sysid_font(name)        (SYSRESID(font_##name))
#define sysid_image(name)       (SYSRESID(img_##name))
#define sysid_css(name)         (SYSRESID(css_##name))
#define sysid_css_group(name)   (SYSRESID(css_group_##name))

// local HTResId by name
#define my_var(name)            (RESID(var_##name))
#define my_font(name)           (RESID(font_##name))
#define my_image(name)          (RESID(img_##name))
#define my_css(name)            (RESID(css_##name))
#define my_css_group(name)      (RESID(css_group_##name))
#define my_ui(name)             (RESID(ui_##name))
#define my_menu(name)           (RESID(menu_##name))

#if 0 /* to be deprecated */
#define my_textid(name)         name,
#define my_imageid(name)        (RESID(img_##name))
#endif

//text resource
#define begin_text_res()
#define text_res_raw(lang, encoding, txt_file)
#define text_res_zipped(lang, encoding, txt_file)
#define text_res_gnumsg(lang, encoding, txt_file)
#define end_text_res

//image resource
#define begin_image_res() \
    static const ResourceEntry image_res[] = {

#define end_image_res {0, NULL} };

#define image(name, img_file) { RESID(img_##name), img_file },
#define bmpfont_image(name, img_file) { RESID(img_bmpfont_##name), img_file},

//font resource
#define begin_font_res() \
    static const ResourceEntry font_res[] = {

#define end_font_res {0, ""} };

#define font(name, font_name) { RESID(font_##name), font_name },
#define bmpfont(name, font_name) { RESID(bmpfont_##name), font_name },

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
#define sys_css(name)           GetCssRes(SYSRESID(css_##name))
#define sys_image(name)         GetImageRes(SYSRESID(img_##name))
#define sys_font(name)          GetFontRes(SYSRESID(font_##name))
#define sys_bmpfont_image(name) (SYSRESID(img_bmpfont_##name))
#define sys_bmpfont(name)       (SYSRESID(bmpfont_##name))

//style resource
#define self_css(name)          GetCssRes(RESID(css_##name))
#define self_image(name)        GetImageRes(RESID(img_##name))
#define self_gif(name)          GetGifAnimateRes(RESID(img_##name))
#define self_bmpfont_image(name)    (RESID(img_bmpfont_##name))
#define self_font(name)         GetFontRes(RESID(font_##name))
#define self_bmpfont(name)      (RESID(bmpfont_##name))

#define rgba(r, g, b, a)

static HTReal* array_quad_real (HTReal a, HTReal b, HTReal c, HTReal d)
{
    static HTReal _array [] = {a, b, c, d};
    return _array;
}

static int* array_quad_int (int a, int b, int c, int d)
{
    static int _array [] = {a, b, c, d};
    return _array;
}

/////////////////////////////////
// style sheet
#define begin_css_res

#define begin_css(name, selector)

#define style(id, value, user_data)

#define end_css

#define end_css_res

/////////////////////////////////
// css group
#define begin_css_group_res

#define begin_css_group(name)

#define css(name)

#define end_css_group

#define end_css_group_res

////////////////////////////////
//Menu resource
#define begin_menus
#define end_menus

#define begin_menu_res(name) \
    static Menu* create_menu_##name(Menu* parent, EventListener* listener) { \
        Menu* ret_menu = NULL; \
        Menu* _parent = parent;

#define end_menu_res        \
    return ret_menu; }

#define begin_menu() \
    do { Menu* __menu = HFCL_NEW_EX(Menu, ()); \
        setMenu(Parent, _parent) \
        _parent = __menu;

#define begin_theme_menu(list_ssid, item_ssid, menubar_ssid) \
    do { \
        Menu* __menu = HFCL_NEW_EX(Menu, (list_ssid, item_ssid, menubar_ssid));\
        setMenu(Parent, _parent) \
        _parent = __menu;

#define end_menu ret_menu = __menu; \
    } while(0);

#define setMenuRect(l, t, r, b)     __menu->setMenuRect(l, t, r, b);
#define setOnMenuListener()         __menu->addEventListener(listener);
#define setMenu(Name, value)        __menu->setMenu##Name(value);
#define menu_set(Name, value)       __menu->set##Name(value);
#define menuAutoFit()               __menu->autoFit();
#define menuHilightFirstEnable()    __menu->hilightFirstEnable();

#define addMenuItemEx(str, image, id, disable, ssid)   \
    __menu->addCommandMenuItem(str, image, (int)(id), disable, ssid);
#define addMenuItem(str, image, id)   \
    addMenuItemEx(str, image, (int)(id), false, NULL)
#define addCheckItemEx(str, image, id, checked, disable, ssid) \
    __menu->addCheckMenuItem(str, image, (int)(id), disable, checked, ssid);
#define addCheckItem(str, image, id, checked)   \
    addCheckItemEx(str, image, id, checked, false, NULL)
#define addRadioItemEx(str, image, id, tag, checked, disable, ssid)    \
    __menu->addRadioMenuItem(str, image, (int)(id), tag, disable, checked, ssid);
#define addRadioItem(str, image, id, tag, checked)  \
    addRadioItemEx(str, image, id, tag, checked, false, NULL)

#define addSubMenuEx(str, image, id, disable, ssid)    \
    __menu->addSubMenuItem(str, image, (int)id, (Menu*)(ret_menu), disable, ssid);
#define addSubMenu(str, image, id)    addSubMenuEx(str, image, id, false, NULL)

// UI resource
#define begin_uis
#define end_uis

#define begin_ui_res(name)  \
    static View* create_ui_##name (View* parent, ViewContext* viewContext, \
            ContentProvider * contentProvider) { \
        View *ret_view = NULL, *_parent = NULL, *__view = NULL; \
        __view = (View*) ((DWORD)__view ^ 0x00); /* VW */ \
        _parent= parent; \
        __view = parent;

#define end_ui_res  return ret_view ; }

#define data_get(id, type)  \
    (contentProvider ? contentProvider->getContentData(id, type) : NULL)
#define data_get_any(id)    \
    data_get(id, 0)
#define data_get_image(id)  \
    (Image*)data_get(id, R_TYPE_IMAGE)

#define data_get_text(id)   \
    (const char *) data_get(id, R_TYPE_TEXT)

#define data_get_text_id(id)   \
    (const int)( contentProvider ? \
        contentProvider->getContentStringId(id, R_TYPE_TEXT) : \
        NULL)

#define data_get_int(id)    \
    (int)data_get(id, 0)

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

#define begin_item_ex(view_class, ssid)  \
    do {    \
        view_class* __view  \
            = HFCL_NEW_EX(view_class, (NULL, GetCssRes(ssid))); \
        ((ListView*)_parent)->addItem((ItemView*)__view);   \
        View *_parent = (View*)__view;

#define begin_composite_view(view_class, ss_set_id) \
    do {                            \
        view_class* __view =        \
            HFCL_NEW_EX(view_class, (_parent, GeCssSetRes(ss_set_id))); \
        View* _parent = (View*)__view;

#define end_item end_view

#define end_composite_view end_view

#define end_view  \
    ret_view = (View*)__view; \
} while(0);

#define end_scrollable_view         \
    __view->updateContentRect();    \
    ret_view = (View*)__view;       \
} while(0);

#define end_scrollable_view_ex      \
    __view->updateContentRect();    \
    ret_view = (View*)__view;       \
} while(0);

#define end_scrollable_view_id_ex   \
    __view->updateContentRect();    \
    ret_view = (View*)__view;       \
} while(0);

#define hide()   __view->hide();
#define add(Name)   __view->add##Name(ret_view);

//import the other view
#define import_view(view_id) do { \
    CB_CREATE_VIEW _create_view = (CB_CREATE_VIEW)GetRes(view_id); \
    if (_create_view) \
        ret_view = _create_view((View*)_parent, viewContext, \
            contentProvider); \
    } while(0);

#define begin_import_view(ViewClass, view_id) do { \
    CB_CREATE_VIEW _create_view = (CB_CREATE_VIEW)GetRes(view_id); \
    if (_create_view) { \
        ViewClass* __view = (ViewClass*)_create_view((View*)_parent, \
            viewContext, contentProvider); \
        if (__view) { \
            View* _parent = NULL; _parent = (View*)__view;

#define end_import_view \
            ret_view = (View*)__view; \
        }   \
    }       \
} while(0);

//controller
#define begin_mode(name)  \
        static ControllerMode name##_mode[] = {

#define end_mode };

#define setMode(id, value, cmd) \
    { (int)(id), (unsigned int)(value), (unsigned int)(cmd) },

#define begin_controller_modes \
    static ControllerModeList _controller_mode_lists[] = {

#define end_controller_modes(def_mode) \
    }; \
    static ControllerModeManager _controller_mode_manager = { \
        _controller_mode_lists, \
        sizeof(_controller_mode_lists) / sizeof(ControllerModeList), \
        RESID(mode_##def_mode) }; \
    if (viewContext) \
        viewContext->setControllerModeManager(&_controller_mode_manager);


#define add_mode(name) \
    { RESID(mode_##name), sizeof(name##_mode) / sizeof(ControllerMode), \
        name##_mode },

#define set(Name, value)        __view->set##Name(value);

#define setProperty(pid, value, user_data)   \
    __view->setProperty((pid), (value), (HTData)(user_data));

#define addCss(style)    __view->addCss(style);
#define removeCss(style) __view->removeCss(style);
#define _m(Name, args)          __view->Name args;

#define map(name_id)                            \
    if (viewContext) {                          \
        __view->setId(name_id);                 \
        viewContext->setView(name_id, __view);  \
    }

#define on(event_type, handle_id)               \
    if (viewContext) {                          \
        EventListener* listener                 \
            = viewContext->getHandle(handle_id, event_type); \
        if (listener)                           \
            __view->addEventListener(listener); \
    }

#define begin_foreach(n)            \
    do {                            \
        unsigned int i = 0;         \
        for(i = 0; i < n; i++) {    \
            char c[4];              \
            sprintf(c,"%d", i+1);   \

#define end_foreach                 \
        }                           \
    } while(0);

#define begin_resid(name) \
    static  HTResId name[] = {

#define end_resid \
    };

