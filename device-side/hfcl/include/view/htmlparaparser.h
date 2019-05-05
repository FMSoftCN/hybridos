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
#include "../common/list.h"
#include "../view/view.h"

#include <minigui/minigui.h>
#include <minigui/gdi.h>

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
    size_t parse(View* parent, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char);

    /**
     * Check whether the given Unicode code point is valid for HTML.
     */
    bool isValidCharacter(Uchar32 uc);

    /**
     * Check if the given token is a valid named or numeric character
     * reference. If it was (return value > 0), the function returns
     *  the character in UTF-8 encoding via mchar and mchar_len.
     */
    size_t checkCharacterReference(const char* token, size_t len,
            char* mchar, int* mchar_len);

protected:
    enum {
        IM_INITIAL = 0,
        IM_BEFORE_HTML,
        IM_BEFORE_HEAD,
        IM_IN_HEAD,
        IM_IN_HEAD_NOSCRIPT,
        IM_AFTER_HEAD,
        IM_IN_BODY,
        IM_TEXT,
        IM_IN_TABLE,
        IM_IN_TABLE_TEXT,
        IM_IN_CAPTION,
        IM_IN_COLUMN_GROUP,
        IM_IN_TABLE_BODY,
        IM_IN_ROW,
        IM_IN_CELL,
        IM_IN_SELECT,
        IM_IN_SELECT_IN_TABLE,
        IM_IN_TEMPLATE,
        IM_AFTER_BODY,
        IM_IN_FRAMESET,
        IM_AFTER_FRAMESET,
        IM_AFTER_AFTER_BODY,
    };

    typedef struct _OpenElementState {
        list_t      list;
        const char* tag;
        int         tag_idx;
        View*       view;
    } OpenElementState;

    typedef struct _TemplateInsertingMode {
        list_t      list;
        int         im;
    } TemplateInsertingMode;

    void resetInsertingMode();

private:
    // current inserting mode
    int             m_im_current;
    // original inserting mode
    int             m_im_original;

    // stack of open elements
    list_t          m_stack_oe;
    void            reset_stack_oe();

    // stack of template inserting modes.
    list_t          m_stack_tim;
    void            reset_stack_tim();

    // fragment context; a fake stack entry
    OpenElementState  m_context;
    OpenElementState* m_head_element;

    const char*     m_input_content;
    size_t          m_len_left;

    CB_ON_NEW_NODE  m_on_new_node;
    CB_ON_NEW_ATTR  m_on_new_attr;
    CB_ON_NEW_CHAR  m_on_new_char;
};

} // namespace hfcl

#endif /* HFCL_COMMON_HTMLPARAPARSER_H_ */

