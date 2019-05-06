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

#include <minigui/minigui.h>
#include <minigui/gdi.h>

namespace hfcl {

typedef void* (*CB_ON_NEW_NODE)(void* context, const char* tag);
typedef void  (*CB_ON_NEW_ATTR)(void* context,
        const char* attrKey, const char* attrValue);
typedef int   (*CB_ON_NEW_CHAR)(void* context, const char* text);

class View;
class ViewContainer;

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
    // the tokenizer states
    enum {
        /* 8.2.4.1  */ TS_DATA = 0,
        /* 8.2.4.2  */ TS_RCDATA,
        /* 8.2.4.3  */ TS_RAWTEXT,
        /* 8.2.4.4  */ TS_SCRIPT_DATA,
        /* 8.2.4.5  */ TS_PLAINTEXT,
        /* 8.2.4.6  */ TS_TAG_OPEN,
        /* 8.2.4.7  */ TS_END_TAG_OPEN,
        /* 8.2.4.8  */ TS_TAG_NAME,
        /* 8.2.4.9  */ TS_RCDATA_LESS_THAN_SIGN,
        /* 8.2.4.10 */ TS_RCDATA_END_TAG_OPEN,
        /* 8.2.4.11 */ TS_RCDATA_END_TAG_NAME,
        /* 8.2.4.12 */ TS_RAWTEXT_LESS_THAN_SIGN,
        /* 8.2.4.13 */ TS_RAWTEXT_END_TAG_OPEN,
        /* 8.2.4.14 */ TS_RAWTEXT_END_TAG_NAME,
        /* 8.2.4.15 */ TS_SCRIPT_DATA_LESS_THAN_SIGN,
        /* 8.2.4.16 */ TS_SCRIPT_DATA_END_TAG_OPEN,
        /* 8.2.4.17 */ TS_SCRIPT_DATA_END_TAG_NAME,
        /* 8.2.4.18 */ TS_SCRIPT_DATA_ESCAPE_START,
        /* 8.2.4.19 */ TS_SCRIPT_DATA_ESCAPE_START_DASH,
        /* 8.2.4.20 */ TS_SCRIPT_DATA_ESCAPED,
        /* 8.2.4.21 */ TS_SCRIPT_DATA_ESCAPED_DASH,
        /* 8.2.4.22 */ TS_SCRIPT_DATA_ESCAPED_DASH_DASH,
        /* 8.2.4.23 */ TS_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN,
        /* 8.2.4.24 */ TS_SCRIPT_DATA_ESCAPED_END_TAG_OPEN,
        /* 8.2.4.25 */ TS_SCRIPT_DATA_ESCAPED_END_TAG_NAME,
        /* 8.2.4.26 */ TS_SCRIPT_DATA_DOUBLE_ESCAPE_START,
        /* 8.2.4.27 */ TS_SCRIPT_DATA_DOUBLE_ESCAPED,
        /* 8.2.4.28 */ TS_SCRIPT_DATA_DOUBLE_ESCAPED_DASH,
        /* 8.2.4.29 */ TS_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN,
        /* 8.2.4.30 */ TS_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH,
        /* 8.2.4.31 */ TS_SCRIPT_DATA_DOUBLE_ESCAPE_END,
        /* 8.2.4.32 */ TS_BEFORE_ATTRIBUTE_NAME,
        /* 8.2.4.33 */ TS_ATTRIBUTE_NAME,
        /* 8.2.4.34 */ TS_AFTER_ATTRIBUTE_NAME,
        /* 8.2.4.35 */ TS_BEFORE_ATTRIBUTE_VALUE,
        /* 8.2.4.36 */ TS_ATTRIBUTE_VALUE_DOUBLE_QUOTED,
        /* 8.2.4.37 */ TS_ATTRIBUTE_VALUE_SINGLE_QUOTED,
        /* 8.2.4.38 */ TS_ATTRIBUTE_VALUE_UNQUOTED,
        /* 8.2.4.39 */ TS_AFTER_ATTRIBUTE_VALUE_QUOTED,
        /* 8.2.4.40 */ TS_SELF_CLOSING_START_TAG,
        /* 8.2.4.41 */ TS_BOGUS_COMMENT,
        /* 8.2.4.42 */ TS_MARKUP_DECLARATION_OPEN,
        /* 8.2.4.43 */ TS_COMMENT_START,
        /* 8.2.4.44 */ TS_COMMENT_START_DASH,
        /* 8.2.4.45 */ TS_COMMENT,
        /* 8.2.4.46 */ TS_COMMENT_LESS_THAN_SIGN,
        /* 8.2.4.47 */ TS_COMMENT_LESS_THAN_SIGN_BANG,
        /* 8.2.4.48 */ TS_COMMENT_LESS_THAN_SIGN_BANG_DASH,
        /* 8.2.4.49 */ TS_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH,
        /* 8.2.4.50 */ TS_COMMENT_END_DASH,
        /* 8.2.4.51 */ TS_COMMENT_END,
        /* 8.2.4.52 */ TS_COMMENT_END_BANG,
        /* 8.2.4.53 */ TS_DOCTYPE,
        /* 8.2.4.54 */ TS_BEFORE_DOCTYPE_NAME,
        /* 8.2.4.55 */ TS_DOCTYPE_NAME,
        /* 8.2.4.56 */ TS_AFTER_DOCTYPE_NAME,
        /* 8.2.4.57 */ TS_AFTER_DOCTYPE_PUBLIC_KEYWORD,
        /* 8.2.4.58 */ TS_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER,
        /* 8.2.4.59 */ TS_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED,
        /* 8.2.4.60 */ TS_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED,
        /* 8.2.4.61 */ TS_AFTER_DOCTYPE_PUBLIC_IDENTIFIER,
        /* 8.2.4.62 */ TS_BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIERS,
        /* 8.2.4.63 */ TS_AFTER_DOCTYPE_SYSTEM_KEYWORD,
        /* 8.2.4.64 */ TS_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER,
        /* 8.2.4.65 */ TS_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED,
        /* 8.2.4.66 */ TS_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED,
        /* 8.2.4.67 */ TS_AFTER_DOCTYPE_SYSTEM_IDENTIFIER,
        /* 8.2.4.68 */ TS_BOGUS_DOCTYPE,
        /* 8.2.4.69 */ TS_CDATA_SECTION,
        /* 8.2.4.70 */ TS_CDATA_SECTION_BRACKET,
        /* 8.2.4.71 */ TS_CDATA_SECTION_END,
        /* 8.2.4.72 */ TS_CHARACTER_REFERENCE,
        /* 8.2.4.73 */ TS_NUMERIC_CHARACTER_REFERENCE,
        /* 8.2.4.74 */ TS_HEXADECIMAL_CHARACTER_REFERENCE_START,
        /* 8.2.4.75 */ TS_DECIMAL_CHARACTER_REFERENCE_START,
        /* 8.2.4.76 */ TS_HEXADECIMAL_CHARACTER_REFERENCE,
        /* 8.2.4.77 */ TS_DECIMAL_CHARACTER_REFERENCE,
        /* 8.2.4.78 */ TS_NUMERIC_CHARACTER_REFERENCE_END,
        /* 8.2.4.79 */ TS_CHARACTER_REFERENCE_END,
    };

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

    typedef struct _OpenElementNode {
        list_t      list;
        const char* tag;
        int         tag_idx;
        const View* view;
    } OpenElementNode;

    bool isOpenElement(const View* view);
    View* insertNewElement(const View* view);

    typedef struct _ActiveFormattingEle {
        list_t      list;
        const View* view;
        bool        is_marker;
    } ActiveFormattingEle;

    void resetInsertingMode();

    // operators for active formatting list
    void pushNewAfe(const View* view, bool isMarker = false);
    void rebuildAfeList();
    void clearUpToLastMarker();

private:
    // current inserting mode
    int             m_im_current;
    // original inserting mode
    int             m_im_original;

    // stack of open elements
    list_t          m_stack_oe;
    void            reset_stack_oe(int stackSize);

    // stack of template inserting modes.
    size_t          m_stack_tim_top;
    size_t          m_stack_tim_size;
    int*            m_stack_tim;
    void            reset_stack_tim(int stackSize);

    // list of active formatting elements.
    list_t          m_list_afe;
    void            reset_list_afe();

    // fragment context; a fake stack entry
    OpenElementNode  m_context;

    // The element pointers
    const View*     m_head_element;
    const View*     m_form_element;

    const char*     m_input_content;
    size_t          m_len_left;

    CB_ON_NEW_NODE  m_on_new_node;
    CB_ON_NEW_ATTR  m_on_new_attr;
    CB_ON_NEW_CHAR  m_on_new_char;
};

} // namespace hfcl

#endif /* HFCL_COMMON_HTMLPARAPARSER_H_ */

