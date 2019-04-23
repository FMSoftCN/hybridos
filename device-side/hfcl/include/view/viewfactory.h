/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018, 2019 Beijing FMSoft Technologies Co., Ltd.
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

#ifndef HFCL_VIEW_VIEWFACTORY_H_
#define HFCL_VIEW_VIEWFACTORY_H_

#include "../view/view.h"

#include <string>
#include <map>

namespace hfcl {

typedef View* (*CB_VIEW_CREATOR)(const char* vclass, const char* vname, int vid);
typedef std::map<std::string, CB_VIEW_CREATOR> TagViewMap;

class ViewFactory {
public:
    static ViewFactory *singleton() {
       if (!s_single) {
           s_single = new ViewFactory();
       }

       return s_single;
    }

    bool registerView(const char *vtag, CB_VIEW_CREATOR creator);

    View *create(const char* vtag, const char* vclass, const char* vname, int vid);

    void list();

private:
    ViewFactory() { }

    TagViewMap m_map;
    static ViewFactory *s_single;

    class Gc {
    public:
        ~Gc() {
            if (ViewFactory::s_single)
                delete ViewFactory::s_single;
        }
    };

    static Gc gc;
};

#define REGISTER_VIEW_IMPL(_vtag, _class) \
    static View* create_##_class(const char* vclass, const char* vname, int vid) \
    { \
        return new _class(vclass, vname vid); \
    } \
    void realRegisterView##_class(const char* tag) \
    { \
        ViewFactory::singleton()->registerView(#_vtag, create_##_class); \
    }

#define DO_REGISTER_VIEW(_vtag, _class) \
    extern void realRegisterView##_class(const char*); \
    realRegister##_class(#_vtag)

#define AUTO_REGISTER_VIEW(_vtag, _class) \
    static struct _ViewFactory_##_class { \
        static View *create(const char* vclass, const char* vname, int vid) { \
            return new _class(vclass, vname, vid); \
        } \
        _ViewFactory_##_class() { \
            ViewFactory::singleton()->registerView(#_vtag, create); \
        } \
    } _autoRegister##_class


inline View* CreateViewByTag(const char* vtag,
        const char* vclass = NULL, const char* vname = NULL, int vid = 0)
{
    return ViewFactory::singleton()->create(vtag, vclass, vname, vid);
}

} // namespace hfcl

#endif /* HFCL_VIEW_VIEWFACTORY_H_ */
