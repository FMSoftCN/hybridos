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

#ifndef HFCL_COMMON_HTMLPARAPARSER_H_
#define HFCL_COMMON_HTMLPARAPARSER_H_

#include "../common/common.h"

namespace hfcl {

typedef void* (*CB_ON_NEW_NODE)(void* context, const char* tag);
typedef void  (*CB_ON_NEW_ATTR)(void* context,
        const char* attrKey, const char* attrValue);
typedef int   (*CB_ON_NEW_CHAR)(void* context, const char* text);

class HtmlParaParser {
public:
    HtmlParaParser(size_t stackSize);
    ~HtmlParaParser();

    void reset(size_t stackSize);
    size_t parse(void* context, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char);

    typedef struct _TagState {
        const char* tag;
        int         tag_idx;
        void*       context;
    } TagState;

protected:
    size_t check_character_reference(const char* token, size_t len,
            char* mchar, int* mchar_len);

private:
    size_t          m_stack_size;
    size_t          m_stack_top;
    TagState*       m_stack;

    const char*     m_content;
    size_t          m_len_left;

    CB_ON_NEW_NODE  m_on_new_node;
    CB_ON_NEW_ATTR  m_on_new_attr;
    CB_ON_NEW_CHAR  m_on_new_char;
};

} // namespace hfcl

#endif /* HFCL_COMMON_HTMLPARAPARSER_H_ */

