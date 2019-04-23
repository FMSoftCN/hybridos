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
#define my_client(name)         (RESID(client_##name))
#define my_menu(name)           (RESID(menu_##name))

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

#define rgba(r, g, b, a)                \
    ((HTUint)(                          \
        (((HTUint)(r) & 0xFF)) |        \
        (((HTUint)(g) & 0xFF) << 8) |   \
        (((HTUint)(b) & 0xFF) << 16) |  \
        (((HTUint)(a) & 0xFF) << 24)    \
    ))

#define rgb(r, g, b)    rgba(r, g, b, 0xFF)


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

#define style(id, value, ...)

#define end_css

#define end_css_res

/////////////////////////////////
// css group
#define begin_css_group_res

#define begin_css_group(name)

#define css(name)

#define end_css_group

#define end_css_group_res

// UI resource
#define begin_clients
#define end_clients

#define begin_client(name)  \
    static View* create_client_##name(ViewContainer* parent, ViewContext* viewCtxt, ContentProvider* cntProvider) { \
        View *_view_created = NULL; \
        ViewContainer *_parent = parent;

#define end_client \
        return _view_created; \
    }

#define apply_css_group(cssg_id) \
        if (parent && parent->isRoot()) { \
            RootView* root = dynamic_cast<RootView*>(parent); \
            root->applyCssGroup(cssg_id); \
        }

// create an anonymous view without specific CSS class, name, and id
#define begin_view(ViewClass) \
    do { \
        ViewContainer *__saved_parent = _parent; \
        ViewClass* __view = HFCL_NEW_EX(ViewClass, (NULL, NULL, 0)); \
        if (_parent) \
            _parent->addChild(__view); \
        else \
            _DBG_PRINTF("begin_view: you are trying to create an ORPHAN view (Parent is not a contianer)\n"); \
        if (__view->isContainer()) \
            _parent = (ViewContainer*)__view; \
        else \
            _parent = NULL;

// create a view with CSS class, name, and id
#define begin_view_ex(ViewClass, css_class, vname, vid) \
    do { \
        ViewContainer *__saved_parent = _parent; \
        ViewClass* __view = HFCL_NEW_EX(ViewClass, (css_class, vname, vid)); \
        if (_parent) \
            _parent->addChild(__view); \
        else \
            _DBG_PRINTF("begin_view_ex: you are trying to create an ORPHAN view (Parent is not a contianer)\n"); \
        if (__view->isContainer()) \
            _parent = (ViewContainer*)__view; \
        else \
            _parent = NULL;

#define end_view  \
        _view_created = (View*)__view; \
        _parent = __saved_parent; \
    } while(0);

// import another view
#define import_view(view_resid) \
    do { \
        CB_CREATE_VIEW __create_view = (CB_CREATE_VIEW)GetRes(view_resid); \
        if (__create_view) { \
            View* __view = __create_view(_parent, viewCtxt, cntProvider); \
            if (_parent) \
                _parent->addChild(__view); \
            else \
                _DBG_PRINTF("import_view: you are trying to create an ORPHAN view (Parent is not a contianer)\n"); \
            _view_created = (View*)__view; \
        } \
    } while(0);

// import another view as a container
#define begin_import_view(view_resid) \
    do { \
        ViewContainer* __saved_parent = _parent; \
        CB_CREATE_VIEW __create_view = (CB_CREATE_VIEW)GetRes(view_resid); \
        if (__create_view) { \
            View* __view = __create_view(_parent, viewCtxt, cntProvider); \
            if (_parent) \
                _parent->addChild(__view); \
            else \
                _DBG_PRINTF("begin_import_view: you are trying to create an ORPHAN view (Parent is not a contianer)\n"); \
            if (__view && __view->isContainer()) \
                _parent = dynamic_cast<ViewContainer*>(__view); \
            else \
                _DBG_PRINTF("begin_import_view: the imported view is not a container\n");

#define end_import_view \
            _view_created = __view; \
        } \
        _parent = __saved_parent; \
    } while(0);

// helpers for set view specific attribute
#define set(Name, value) \
    __view->set##Name(value);

// set general attribute
#define attribute(name, value) \
    __view->setAttribute(name, value);

// set CSS style
#define mystyle(pid, value, ...) \
    __view->myCss()->set##pid((value), ##__VA_ARGS__);

#define map(name_id) \
    if (viewCtxt) { \
        __view->setId(name_id); \
        viewCtxt->setView(name_id, __view); \
    }

#define on(event_type, handle_id) \
    if (viewCtxt) { \
        EventListener* listener = \
            viewCtxt->getHandle(handle_id, event_type); \
        if (listener)                           \
            __view->addEventListener(listener); \
    }

#define begin_foreach(n) \
    do { \
        unsigned int i = 0; \
        for(i = 0; i < n; i++) { \
            char c[4]; \
            sprintf(c, "%d", i+1); \

#define end_foreach \
        } \
    } while(0);

#define data_get(id, type)  \
    (cntProvider ? cntProvider->getContentData(id, type) : NULL)
#define data_get_any(id)    \
    data_get(id, 0)
#define data_get_image(id)  \
    (Image*)data_get(id, R_TYPE_IMAGE)

#define data_get_text(id)   \
    (const char *) data_get(id, R_TYPE_TEXT)

#define data_get_text_id(id)   \
    (const int)(cntProvider ? \
        cntProvider->getContentStringId(id, R_TYPE_TEXT) : \
        NULL)

#define data_get_int(id)    \
    (int)data_get(id, 0)

// Resource identifiers
#define begin_resid(name) \
    static HTResId name[] = {

#define end_resid \
    };

// Controller
#define begin_mode(name)  \
    static ControllerMode name##_mode[] = {

#define end_mode \
    };

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
    if (viewCtxt) \
        viewCtxt->setControllerModeManager(&_controller_mode_manager);

#define add_mode(name) \
    { RESID(mode_##name), sizeof(name##_mode) / sizeof(ControllerMode), \
        name##_mode },

// Menu resource
#define begin_menus
#define end_menus

#define begin_menu_res(name) \
    static Menu* create_menu_##name(Menu* parent, EventListener* listener) { \
        Menu* ret_menu = NULL; \
        Menu* _parent = parent;

#define end_menu_res        \
    return ret_menu; }

#define begin_menu() \
    do { \
        Menu* __menu = HFCL_NEW_EX(Menu, ()); \
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

