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

#ifndef HFCL_RESOURCE_TEXTRES_H_
#define HFCL_RESOURCE_TEXTRES_H_

#include "graphics/gifanimate.h"
#include "view/animateimageview.h"

namespace hfcl {

typedef const char* my_text_t;
PAIR(my_text_t, my_text_t, TextMsgPair);
MAP(my_text_t, my_text_t, TextMsgMap);

class TextRes {
public:
    TextRes (HTData res_data) : m_res_data (res_data) { }

    virtual ~TextRes () { release(); }

    virtual bool load () {
        m_raw_strings = (const char**)m_res_data;
        return true;
    }
    virtual void release () {};

    virtual const char* getText (HTStrId strId) {
        if (m_raw_strings)
            return m_raw_strings [strId];
        return "";
    }
    virtual const char* getText (const char* str) {
        return str;
    }

protected:
    HTData m_res_data;
    const char** m_raw_strings;
};

class TextResZipped : public TextRes {
public:
    TextResZipped (HTData res_data)
        : TextRes (res_data)
        , m_loaded (false)
        , m_string_bucket (NULL)
    { }
    ~TextResZipped () { }

    virtual bool load ();
    virtual void release ();

private:
    bool m_loaded;
    char* m_string_bucket;
};

class TextResGnuMsg : public TextRes {
public:
    TextResGnuMsg (HTData res_data) : TextRes (res_data) { }
    ~TextResGnuMsg () {}

    int loadFromFile (const char* mo_file);
    virtual bool load ();
    virtual void release () {
        m_text_map.clear ();
    }

    virtual const char* getText (HTStrId strId) {
        return "";
    }
    virtual const char* getText (const char* str);

private:
    bool m_loaded;
    TextMsgMap m_text_map;
};

} // namespace hfcl

#endif /* HFCL_RESOURCE_TEXTRES_H_ */

