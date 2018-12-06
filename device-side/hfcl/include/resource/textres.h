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

class TextRes {
public:
    TextRes (const char* res_name) : m_res_name (res_name) { }

    virtual ~TextRes () { }

    virtual bool load () = 0 ;
    virtual void release () = 0;

    virtual const char* getText (HTStrId strId) {
        return "";
    }
    virtual const char* getText (const char* str) {
        return str;
    }

protected:
    const char* m_res_name;
};

class TextResRaw : public TextRes {
public:
    TextResRaw (const char* res_name) : TextRes (res_name) {
        m_is_incore = false;
        m_raw_strings = NULL;
    }

    ~TextResRaw () { release (); }

    virtual bool load ();
    virtual void release ();

    virtual const char* getText (HTStrId strId) {
        if (m_raw_strings)
            return m_raw_strings [strId];
        return "";
    }

private:
    bool m_is_incore;
    const char** m_raw_strings;
};

class TextResZipped : public TextRes {
public:
    TextResZipped (const char* res_name)
        : TextRes (res_name)
        , m_string_bucket (NULL)
    { }
    ~TextResZipped () { release (); }

    virtual bool load ();
    virtual void release ();
    virtual const char* getText (HTStrId strId) {
        if (m_raw_strings)
            return m_raw_strings [strId];
        return "";
    }

private:
    char* m_string_bucket;
    const char** m_raw_strings;
};

class TextResGnuMsg : public TextRes {
public:
    TextResGnuMsg (const char* res_name) : TextRes (res_name) { }
    ~TextResGnuMsg () { release (); }

    int doLoad (MG_RWops* src);

    virtual bool load ();
    virtual void release ();

    virtual const char* getText (const char* str);

private:
    MAPCLASSKEY(string, string, TextMsgMap);
    PAIR(string, string, TextMsgPair);

    TextMsgMap m_text_map;
};

} // namespace hfcl

#endif /* HFCL_RESOURCE_TEXTRES_H_ */

