/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2019 Beijing FMSoft Technologies Co., Ltd.
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

/*
** htmlparaparser.cc: The implementation of HVMLParser class.
**
** Create by WEI Yongming at 2019/04/26
*/

#include "hvml/hvmlparser.h"

#include "common/helpers.h"
#include "view/view.h"
#include "view/viewcontainer.h"

#if SIZEOF_PTR == 8
#define MIN_STACK_SIZE          16
#else
#define MIN_STACK_SIZE          8
#endif

namespace hfcl {

HVMLParser::HVMLParser(size_t stackSize, const char* encoding)
{
    INIT_LIST_HEAD(&m_stack_oe);

    reset_stack_tim(stackSize);

    INIT_LIST_HEAD(&m_list_afe);

    memset(&m_ctxt_fragment, 0, sizeof(m_ctxt_fragment));

    m_head_element = NULL;
    m_form_element = NULL;

    memset(&m_ctxt_tokenizer, 0, sizeof(TokenizerContext));
}

HVMLParser::~HVMLParser()
{
    reset_stack_oe(0);
    reset_stack_tim(0);
    reset_list_afe();
    reset_tokenizer(NULL);
}

void HVMLParser::reset_stack_oe(int stackSize)
{
    while (!list_empty(&m_stack_oe)) {
        OpenElementNode* oes = (OpenElementNode*)m_stack_oe.next;
        list_del(m_stack_oe.next);
        mg_slice_delete(OpenElementNode, oes);
    }

    INIT_LIST_HEAD(&m_stack_oe);
}

void HVMLParser::reset_stack_tim(int stackSize)
{
    delete [] m_stack_tim;

    if (stackSize > 0) {
        if (stackSize < MIN_STACK_SIZE)
            m_stack_tim_size = MIN_STACK_SIZE;
        else
            m_stack_tim_size = stackSize;

        m_stack_tim = new int[m_stack_tim_size];
        m_stack_tim_top = 0;
    }
}

void HVMLParser::reset_list_afe()
{
    while (!list_empty(&m_list_afe)) {
        ActiveFormattingEle* afe = (ActiveFormattingEle*)m_list_afe.next;
        list_del(m_list_afe.next);
        mg_slice_delete(ActiveFormattingEle, afe);
    }

    INIT_LIST_HEAD(&m_list_afe);
}

bool HVMLParser::reset_tokenizer(const char* encoding)
{
    if (m_ctxt_tokenizer.dt) {
        delete m_ctxt_tokenizer.dt;
    }

    if (m_ctxt_tokenizer.tt) {
        delete m_ctxt_tokenizer.tt;
    }

    if (m_ctxt_tokenizer.lf)
        DestroyLogFont(m_ctxt_tokenizer.lf);
    memset(&m_ctxt_tokenizer, 0, sizeof(TokenizerContext));

    if (encoding) {
        m_ctxt_tokenizer.lf = CreateLogFontForMChar2UChar(encoding);
        if (m_ctxt_tokenizer.lf == NULL)
            return false;
    }

    return true;
}

bool HVMLParser::reset(size_t stackSize, const char* encoding)
{
    reset_stack_oe(stackSize);
    reset_stack_tim(stackSize);
    reset_list_afe();

    m_head_element = NULL;
    m_form_element = NULL;

    return reset_tokenizer(encoding);
}

void HVMLParser::push_new_afe(const View* view, bool isMarker)
{
    list_t* pos;
    int nr_elements;
    ActiveFormattingEle* last_marker = NULL;
    ActiveFormattingEle* afe;

    nr_elements = 0;
    for (pos = m_list_afe.prev; pos != &m_list_afe; pos = pos->prev) {
        afe = (ActiveFormattingEle*)pos;
        if (afe->is_marker) {
            last_marker = afe;
            break;
        }

        nr_elements++;
    }

    // find the element has the same tag name, namespace, and attributes
    // as new view
    if (last_marker && nr_elements >= 3) {
        pos = last_marker->list.next;
    }
    else if (nr_elements > 0) {
        pos = m_list_afe.next;
    }
    else {
        pos = NULL;
    }

    if (pos) {
        for (; pos != &m_list_afe; pos = pos->next) {
            afe = (ActiveFormattingEle*)pos;
            if (afe->view->hasSameTagAndAttributes(view)) {
                list_del(pos);
                mg_slice_delete(ActiveFormattingEle, afe);
            }
        }
    }

    afe = mg_slice_new(ActiveFormattingEle);
    afe->view = view;
    afe->is_marker = isMarker;
    list_add_tail(&afe->list, &m_list_afe);
}

void HVMLParser::rebuild_afe_list()
{
    if (list_empty(&m_list_afe))
        return;

    ActiveFormattingEle* afe = (ActiveFormattingEle*)m_list_afe.prev;
    if (afe->is_marker || is_open_element(afe->view))
        return;

rewind:

    if (afe->list.prev == &m_list_afe) {

        do {
            // Insert an HTML element for the token for which the element entry
            // was created, to obtain new element.
            View* v = insert_new_element(afe->view);
            afe->view = v;

            afe = (ActiveFormattingEle*)afe->list.next;
        } while (&afe->list != &m_list_afe);
    }
    else {
        afe = (ActiveFormattingEle*)afe->list.prev;
        if (!afe->is_marker && !is_open_element(afe->view)) {
            goto rewind;
        }
    }
}

void HVMLParser::clear_up_to_last_marker()
{
    while (!list_empty(&m_list_afe)) {
        bool is_marker;
        ActiveFormattingEle* afe = (ActiveFormattingEle*)m_list_afe.prev;
        is_marker = afe->is_marker;

        list_del(m_list_afe.prev);
        mg_slice_delete(ActiveFormattingEle, afe);

        if (is_marker)
            break;
    }
}

bool HVMLParser::isValidHvmlCharacter(Uchar32 uc)
{
    if (MG_UNLIKELY((uc >= 0x0001 && uc <= 0x0008) || uc == 0x000B ||
            (uc >= 0x000E && uc <= 0x001F) ||
            (uc >= 0x007F && uc <= 0x009F) ||
            (uc >= 0xFDD0 && uc <= 0xFDEF))) {
        return false;
    }

#if 0
            uc == 0x00FFFE || uc == 0x00FFFF ||
            uc == 0x01FFFE || uc == 0x01FFFF ||
            uc == 0x02FFFE || uc == 0x02FFFF ||
            uc == 0x03FFFE || uc == 0x03FFFF ||
            uc == 0x04FFFE || uc == 0x04FFFF ||
            uc == 0x05FFFE || uc == 0x05FFFF ||
            uc == 0x06FFFE || uc == 0x06FFFF ||
            uc == 0x07FFFE || uc == 0x07FFFF ||
            uc == 0x08FFFE || uc == 0x08FFFF ||
            uc == 0x09FFFE || uc == 0x09FFFF ||
            uc == 0x0AFFFE || uc == 0x0AFFFF ||
            uc == 0x0BFFFE || uc == 0x0BFFFF ||
            uc == 0x0CFFFE || uc == 0x0CFFFF ||
            uc == 0x0DFFFE || uc == 0x0DFFFF ||
            uc == 0x0EFFFE || uc == 0x0EFFFF ||
            uc == 0x0FFFFE || uc == 0x0FFFFF ||
            uc == 0x10FFFE || uc == 0x10FFFF
#endif
    WORD16 low16 = (WORD16)uc;
    if (MG_UNLIKELY((low16 == 0xFFFE) || (low16 == 0xFFFF))) {
        WORD16 hi16 = (WORD16)(uc >> 16);
        if (hi16 >= 0x00 && hi16 <= 0x10) {
            return false;
        }
    }

    if (MG_UNLIKELY(UCharGetCategory(uc) == UCHAR_CATEGORY_UNASSIGNED)) {
        return false;
    }

    return true;
}

#define UCHAR_NULL                      0x0000
#define UCHAR_TABULATION                0x0009
#define UCHAR_LINE_FEED                 0x000A
#define UCHAR_FORM_FEED                 0x000C

#define UCHAR_SPACE                     0x0020
#define UCHAR_EXCLAMATION_MARK          0x0021
#define UCHAR_QUOTATION_MARK            0x0022
#define UCHAR_NUMBER_SIGN               0x0023
#define UCHAR_AMPERSAND                 0x0026
#define UCHAR_APOSTROPHE                0x0027
#define UCHAR_HYPHEN_MINUS              0x002D
#define UCHAR_SOLIDUS                   0x002F

#define UCHAR_DIGIT_ZERO                0x0030
#define UCHAR_DIGIT_NINE                0x0039
#define UCHAR_SEMICOLON                 0x003B
#define UCHAR_LESS_THAN_SIGN            0x003C
#define UCHAR_EQUAL_SIGN                0x003D
#define UCHAR_GREATER_THAN_SIGN         0x003E
#define UCHAR_QUESTION_MARK             0x003F

#define UCHAR_LATIN_CAPITAL_LETTER_A    0x0041
#define UCHAR_LATIN_CAPITAL_LETTER_F    0x0046
#define UCHAR_LATIN_CAPITAL_LETTER_X    0x0058
#define UCHAR_LATIN_CAPITAL_LETTER_Z    0x005A

#define UCHAR_RIGHT_SQUARE_BRACKET      0x005D
#define UCHAR_GRAVE_ACCENT              0x0060

#define UCHAR_LATIN_SMALL_LETTER_A      0x0061
#define UCHAR_LATIN_SMALL_LETTER_F      0x0066
#define UCHAR_LATIN_SMALL_LETTER_X      0x0078
#define UCHAR_LATIN_SMALL_LETTER_Z      0x007A

#define UCHAR_REPLACEMENT               0xFFFD
#define UCHAR_EOF                       0xFFFFFFFF

void HVMLParser::on_parse_error()
{
    m_ctxt_tokenizer.errors++;
}

void HVMLParser::on_data_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_AMPERSAND:
        m_ctxt_tokenizer.ts_return = TS_DATA;
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE;
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_TAG_OPEN;
        break;

    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    case UCHAR_EOF:
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_rcdata_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_AMPERSAND:
        m_ctxt_tokenizer.ts_return = TS_RCDATA;
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE;
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_RCDATA_LESS_THAN_SIGN;
        break;

    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_rawtext_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_RAWTEXT_LESS_THAN_SIGN;
        break;

    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_LESS_THAN_SIGN;
        break;

    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_plaintext_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_tag_open_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_EXCLAMATION_MARK:
        m_ctxt_tokenizer.ts = TS_MARKUP_DECLARATION_OPEN;
        break;

    case UCHAR_SOLIDUS:
        m_ctxt_tokenizer.ts = TS_END_TAG_OPEN;
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        create_start_tag_token();
        m_ctxt_tokenizer.ts = TS_TAG_NAME;
        m_ctxt_tokenizer.consumed = 0;
        //on_tag_name_state();
        break;

    case UCHAR_QUESTION_MARK:
        on_parse_error();
        create_comment_token();
        m_ctxt_tokenizer.ts = TS_BOGUS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        //on_bogus_comment_state();
        break;

    default:
        on_parse_error();
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        m_ctxt_tokenizer.ts = TS_DATA;
        m_ctxt_tokenizer.consumed = 0;
        //on_data_state();
        break;
    }
}

void HVMLParser::on_end_tag_open_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        create_end_tag_token();
        m_ctxt_tokenizer.ts = TS_TAG_NAME;
        m_ctxt_tokenizer.consumed = 0;
        //on_tag_name_state();
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_DATA;
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        emit_eof_token();
        break;

    default:
        on_parse_error();
        create_comment_token();
        m_ctxt_tokenizer.ts = TS_BOGUS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        //on_bogus_comment_state();
        break;
    }
}

void HVMLParser::on_tag_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        break;

    case UCHAR_SOLIDUS:
        m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_tag_token();
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc + 0x0020);
        on_tag_name_state();
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_tag_name(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        append_to_tag_name(m_ctxt_tokenizer.next_uc);
        break;
    }
}

void HVMLParser::on_rcdata_less_than_sign_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_SOLIDUS:
        clear_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_RCDATA_END_TAG_OPEN;
        break;

    default:
        emit_character_token(UCHAR_REPLACEMENT);
        m_ctxt_tokenizer.ts = TS_RCDATA;
        m_ctxt_tokenizer.consumed = 0;
        //on_data_state();
        break;
    }
}

void HVMLParser::on_rcdata_end_tag_open_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        create_end_tag_token();
        m_ctxt_tokenizer.ts = TS_RCDATA_END_TAG_NAME;
        m_ctxt_tokenizer.consumed = 0;
        //on_rcdata_end_tag_name_state();
        break;

    default:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        m_ctxt_tokenizer.ts = TS_RCDATA;
        m_ctxt_tokenizer.consumed = 0;
        //on_data_state();
        break;
    }
}

void HVMLParser::on_rcdata_end_tag_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_SOLIDUS:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_GREATER_THAN_SIGN:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_DATA;
            emit_tag_token();
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc + 0x0020);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    default:
anythingelse:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        emit_character_token_from_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_RCDATA;
        m_ctxt_tokenizer.consumed = 0;
        //on_rcdata_state();
        break;
    }
}

void HVMLParser::on_rawtext_less_than_sign_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_SOLIDUS:
        clear_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_RAWTEXT_END_TAG_OPEN;
        break;

    default:
        emit_character_token(UCHAR_REPLACEMENT);
        m_ctxt_tokenizer.ts = TS_RAWTEXT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_rawtext_end_tag_open_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        create_end_tag_token();
        m_ctxt_tokenizer.ts = TS_RAWTEXT_END_TAG_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        m_ctxt_tokenizer.ts = TS_RAWTEXT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_rawtext_end_tag_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_SOLIDUS:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_GREATER_THAN_SIGN:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_DATA;
            emit_tag_token();
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc + 0x0020);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    default:
anythingelse:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        emit_character_token_from_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_RAWTEXT;
        m_ctxt_tokenizer.consumed = 0;
        //on_rawtext_state();
        break;
    }
}

void HVMLParser::on_script_data_less_than_sign_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_SOLIDUS:
        clear_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_END_TAG_OPEN;
        break;

    case UCHAR_EXCLAMATION_MARK:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPE_START;
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_EXCLAMATION_MARK);
        break;

    default:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_end_tag_open_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        create_end_tag_token();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_END_TAG_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_end_tag_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_SOLIDUS:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_GREATER_THAN_SIGN:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_DATA;
            emit_tag_token();
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc + 0x0020);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    default:
anythingelse:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        emit_character_token_from_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_escape_start_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPE_START_DASH;
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_escape_start_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_DASH_DASH;
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_escaped_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_DASH;
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
        break;

    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_escaped_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_DASH_DASH;
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
        break;

    case UCHAR_NULL:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_escaped_dash_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        emit_character_token(UCHAR_GREATER_THAN_SIGN);
        break;

    case UCHAR_NULL:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_escaped_less_than_sign_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_SOLIDUS:
        clear_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_END_TAG_OPEN;
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        clear_temporary_buffer();
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPE_START;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_escaped_end_tag_open_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        create_end_tag_token();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED_END_TAG_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_escaped_end_tag_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_SOLIDUS:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_GREATER_THAN_SIGN:
        if (is_appropriate_end_tag()) {
            m_ctxt_tokenizer.ts = TS_DATA;
            emit_tag_token();
        }
        else {
            goto anythingelse;
        }
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc + 0x0020);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        append_to_tag_name(m_ctxt_tokenizer.next_uc);
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        break;

    default:
anythingelse:
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        emit_character_token(UCHAR_SOLIDUS);
        emit_character_token_from_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_double_escape_start_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
    case UCHAR_SOLIDUS:
    case UCHAR_GREATER_THAN_SIGN:
        if (check_temporary_buffer("script")) {
            m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        }
        else {
            m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        }
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc + 0x0020);
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        append_to_temporary_buffer(m_ctxt_tokenizer.next_uc);
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_double_escaped_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED_DASH;
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        break;

    case UCHAR_NULL:
        on_parse_error();
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_double_escaped_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH;
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        break;

    case UCHAR_NULL:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_double_escaped_dash_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        emit_character_token(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_LESS_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
        emit_character_token(UCHAR_LESS_THAN_SIGN);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA;
        emit_character_token(UCHAR_GREATER_THAN_SIGN);
        break;

    case UCHAR_NULL:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        emit_character_token(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_script_data_double_escaped_less_than_sign_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_SOLIDUS:
        clear_temporary_buffer();
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPE_END;
        emit_character_token(UCHAR_SOLIDUS);
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_script_data_double_escape_end_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
    case UCHAR_SOLIDUS:
    case UCHAR_GREATER_THAN_SIGN:
        if (check_temporary_buffer("script")) {
            m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_ESCAPED;
        }
        else {
            m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        }
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_temporary_buffer(m_ctxt_tokenizer.curr_uc + 0x0020);
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_Z:
        append_to_temporary_buffer(m_ctxt_tokenizer.curr_uc);
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;

    default:
        m_ctxt_tokenizer.ts = TS_SCRIPT_DATA_DOUBLE_ESCAPED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_before_attribute_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_SOLIDUS:
    case UCHAR_GREATER_THAN_SIGN:
    case UCHAR_EOF:
        m_ctxt_tokenizer.ts = TS_AFTER_ATTRIBUTE_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;

    case UCHAR_EQUAL_SIGN:
        on_parse_error();
        start_new_attribute();
        append_to_attribute_name(m_ctxt_tokenizer.curr_uc);
        m_ctxt_tokenizer.ts = TS_ATTRIBUTE_NAME;
        break;

    default:
        start_new_attribute();
        m_ctxt_tokenizer.ts = TS_ATTRIBUTE_NAME;
        break;
    }
}

void HVMLParser::on_attribute_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
    case UCHAR_SOLIDUS:
    case UCHAR_GREATER_THAN_SIGN:
    case UCHAR_EOF:
        m_ctxt_tokenizer.ts = TS_AFTER_ATTRIBUTE_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;

    case UCHAR_EQUAL_SIGN:
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_VALUE;
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_attribute_name(m_ctxt_tokenizer.curr_uc + 0x0020);
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_attribute_name(UCHAR_REPLACEMENT);
        break;

    case UCHAR_QUOTATION_MARK:
    case UCHAR_APOSTROPHE:
    case UCHAR_LESS_THAN_SIGN:
        on_parse_error();

    default:
        append_to_attribute_name(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_after_attribute_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_SOLIDUS:
        m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        break;

    case UCHAR_EQUAL_SIGN:
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_VALUE;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_tag_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        start_new_attribute();
        m_ctxt_tokenizer.ts = TS_ATTRIBUTE_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_before_attribute_value_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_QUOTATION_MARK:
        m_ctxt_tokenizer.ts = TS_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        m_ctxt_tokenizer.ts = TS_ATTRIBUTE_VALUE_SINGLE_QUOTED;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();

    default:
        m_ctxt_tokenizer.ts = TS_ATTRIBUTE_VALUE_UNQUOTED;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_attribute_value_double_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_QUOTATION_MARK:
        m_ctxt_tokenizer.ts = TS_AFTER_ATTRIBUTE_VALUE_QUOTED;
        break;

    case UCHAR_AMPERSAND:
        m_ctxt_tokenizer.ts_return = TS_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_attribute_value(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        append_to_attribute_value(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_attribute_value_single_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_APOSTROPHE:
        m_ctxt_tokenizer.ts = TS_AFTER_ATTRIBUTE_VALUE_QUOTED;
        break;

    case UCHAR_AMPERSAND:
        m_ctxt_tokenizer.ts_return = TS_ATTRIBUTE_VALUE_SINGLE_QUOTED;
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_attribute_value(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        append_to_attribute_value(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_attribute_value_unquoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        break;

    case UCHAR_AMPERSAND:
        m_ctxt_tokenizer.ts_return = TS_ATTRIBUTE_VALUE_UNQUOTED;
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_tag_token();
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_attribute_value(UCHAR_REPLACEMENT);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    case UCHAR_QUOTATION_MARK:
    case UCHAR_APOSTROPHE:
    case UCHAR_LESS_THAN_SIGN:
    case UCHAR_EQUAL_SIGN:
    case UCHAR_GRAVE_ACCENT:
        on_parse_error();

    default:
        append_to_attribute_value(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_after_attribute_value_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        break;

    case UCHAR_SOLIDUS:
        m_ctxt_tokenizer.ts = TS_SELF_CLOSING_START_TAG;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_tag_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_self_closing_start_tag_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_GREATER_THAN_SIGN:
        set_self_closing_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_tag_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_BEFORE_ATTRIBUTE_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_bogus_comment_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_comment_token();
        break;

    case UCHAR_EOF:
        emit_comment_token();
        emit_eof_token();
        break;

    case UCHAR_NULL:
        append_to_comment(UCHAR_REPLACEMENT);
        break;

    default:
        append_to_comment(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

bool HVMLParser::does_characters_match_word(const char* word, int* consumed,
            bool case_insensitive)
{
    size_t word_len = strlen(word);

    *consumed = 0;

    if (word_len == 0)
        return false;

    Uchar32 ucword[word_len];
    int nr_ucs = 0;

    int t;
    while ((t = utf8_to_uc32(word, word_len, ucword + nr_ucs)) > 0) {
        nr_ucs ++;
        word += t;
        word_len -= t;
    }

    if (nr_ucs == 0)
        return false;

    const char* mstr = m_ctxt_tokenizer.mchar;
    int mstr_len = m_ctxt_tokenizer.total_len;
    int left_ucs = nr_ucs;
    int i = 0;
    while (mstr_len > 0 && left_ucs > 0) {

        Uchar32 uc;
        int mclen;
        mclen = GetNextUChar(m_ctxt_tokenizer.lf, mstr, mstr_len, &uc);

        if (mclen > 0) {
            Uchar32 uc0 = ucword[i];
            if (case_insensitive) {
                if (uc0 >= UCHAR_LATIN_CAPITAL_LETTER_A &&
                        uc0 <= UCHAR_LATIN_CAPITAL_LETTER_Z)
                    uc0 += 0x0020;

                if (uc >= UCHAR_LATIN_CAPITAL_LETTER_A &&
                        uc <= UCHAR_LATIN_CAPITAL_LETTER_Z)
                    uc += 0x0020;
            }

            if (uc0 != uc)
                return false;
        }
        else {
            return false;
        }

        mstr += mclen;
        mstr_len -= mclen;
        left_ucs--;
        i++;

        *consumed += mclen;
    }

    return true;
}

void HVMLParser::on_markup_declaration_open_state()
{
    int consumed;

    if (does_characters_match_word("--", &consumed)) {
        m_ctxt_tokenizer.consumed = consumed;
        create_comment_token();
        m_ctxt_tokenizer.ts = TS_COMMENT_START;
    }
    else if (does_characters_match_word("DOCTYPE", &consumed)) {
        m_ctxt_tokenizer.consumed = consumed;
        m_ctxt_tokenizer.ts = TS_DOCTYPE;
    }
    else if (check_adjusted_current_node() &&
            does_characters_match_word("[CDATA[", &consumed, false)) {
        m_ctxt_tokenizer.consumed = consumed;
        m_ctxt_tokenizer.ts = TS_CDATA_SECTION;
    }
    else {
        on_parse_error();
        create_comment_token();
        m_ctxt_tokenizer.ts = TS_BOGUS_COMMENT;
    }
}

void HVMLParser::on_comment_start_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_COMMENT_START_DASH;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_comment_token();
        break;

    default:
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_start_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_COMMENT_END;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_comment_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_comment_token();
        emit_eof_token();
        break;

    default:
        append_to_comment(UCHAR_HYPHEN_MINUS);
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LESS_THAN_SIGN:
        append_to_comment(UCHAR_LESS_THAN_SIGN);
        m_ctxt_tokenizer.ts = TS_COMMENT_LESS_THAN_SIGN;
        break;

    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_COMMENT_END_DASH;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_comment(UCHAR_REPLACEMENT);
        emit_comment_token();
        emit_eof_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_comment_token();
        emit_eof_token();
        break;

    default:
        append_to_comment(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_comment_less_than_sign_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_EXCLAMATION_MARK:
        append_to_comment(m_ctxt_tokenizer.curr_uc);
        m_ctxt_tokenizer.ts = TS_COMMENT_LESS_THAN_SIGN_BANG;
        break;

    case UCHAR_LESS_THAN_SIGN:
        append_to_comment(m_ctxt_tokenizer.curr_uc);
        break;

    default:
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_less_than_sign_bang_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_COMMENT_LESS_THAN_SIGN_BANG_DASH;
        break;

    default:
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_less_than_sign_bang_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH;
        break;

    default:
        m_ctxt_tokenizer.ts = TS_COMMENT_END_DASH;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_less_than_sign_bang_dash_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_GREATER_THAN_SIGN:
    case UCHAR_EOF:
        m_ctxt_tokenizer.ts = TS_COMMENT_END;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_COMMENT_END;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_end_dash_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        m_ctxt_tokenizer.ts = TS_COMMENT_END;
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_comment_token();
        emit_eof_token();
        break;

    default:
        append_to_comment(UCHAR_HYPHEN_MINUS);
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_end_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_comment_token();
        break;

    case UCHAR_EXCLAMATION_MARK:
        m_ctxt_tokenizer.ts = TS_COMMENT_END_BANG;
        break;

    case UCHAR_HYPHEN_MINUS:
        append_to_comment(UCHAR_HYPHEN_MINUS);
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_comment_token();
        emit_eof_token();
        break;

    default:
        append_to_comment(UCHAR_HYPHEN_MINUS);
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_comment_end_bang_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_HYPHEN_MINUS:
        append_to_comment(UCHAR_HYPHEN_MINUS);
        append_to_comment(UCHAR_EXCLAMATION_MARK);
        m_ctxt_tokenizer.ts = TS_COMMENT_END_DASH;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_comment_token();
        break;

    case UCHAR_EXCLAMATION_MARK:
        m_ctxt_tokenizer.ts = TS_COMMENT_END_BANG;
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_comment_token();
        emit_eof_token();
        break;

    default:
        append_to_comment(UCHAR_HYPHEN_MINUS);
        append_to_comment(UCHAR_EXCLAMATION_MARK);
        m_ctxt_tokenizer.ts = TS_COMMENT;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_doctype_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BEFORE_DOCTYPE_NAME;
        break;

    case UCHAR_EOF:
        on_parse_error();
        create_doctype_token();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_BEFORE_DOCTYPE_NAME;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_before_doctype_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        create_doctype_token();
        append_to_doctype_token_name(m_ctxt_tokenizer.curr_uc + 0x0020);
        m_ctxt_tokenizer.ts = TS_DOCTYPE_NAME;
        break;

    case UCHAR_NULL:
        on_parse_error();
        create_doctype_token();
        append_to_doctype_token_name(UCHAR_REPLACEMENT);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        create_doctype_token();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        create_doctype_token();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        create_doctype_token();
        append_to_doctype_token_name(m_ctxt_tokenizer.curr_uc);
        m_ctxt_tokenizer.ts = TS_DOCTYPE_NAME;
        break;
    }
}

void HVMLParser::on_doctype_name_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_NAME;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_Z:
        append_to_doctype_token_name(m_ctxt_tokenizer.curr_uc + 0x0020);
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_doctype_token_name(UCHAR_REPLACEMENT);
        break;


    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        append_to_doctype_token_name(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_after_doctype_name_state()
{
    int consumed;
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        if (does_characters_match_word("PUBLIC", &consumed)) {
            m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_PUBLIC_KEYWORD;
            m_ctxt_tokenizer.consumed = consumed;
        }
        else if (does_characters_match_word("SYSTEM", &consumed)) {
            m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_SYSTEM_KEYWORD;
            m_ctxt_tokenizer.consumed = consumed;
        }
        else {
            on_parse_error();
            set_force_quirks_flag();
            m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        }
        break;
    }
}

void HVMLParser::on_after_doctype_public_keyword_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
        break;

    case UCHAR_QUOTATION_MARK:
        on_parse_error();
        set_doctype_public_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        on_parse_error();
        set_doctype_public_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_before_doctype_public_identifier_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_QUOTATION_MARK:
        set_doctype_public_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        set_doctype_public_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_doctype_public_identifier_double_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_QUOTATION_MARK:
        m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_doctype_public_identifier(UCHAR_REPLACEMENT);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        append_to_doctype_public_identifier(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_doctype_public_identifier_single_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_APOSTROPHE:
        m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_doctype_public_identifier(UCHAR_REPLACEMENT);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        append_to_doctype_public_identifier(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_after_doctype_public_identifier_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIERS;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_QUOTATION_MARK:
        on_parse_error();
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        on_parse_error();
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_between_doctype_public_system_identifiers_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_QUOTATION_MARK:
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_after_doctype_system_keyword_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        m_ctxt_tokenizer.ts = TS_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
        break;

    case UCHAR_QUOTATION_MARK:
        on_parse_error();
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        on_parse_error();
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_before_doctype_system_identifier_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_QUOTATION_MARK:
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
        break;

    case UCHAR_APOSTROPHE:
        set_doctype_system_identifier_empty();
        m_ctxt_tokenizer.ts = TS_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_doctype_system_identifier_double_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_QUOTATION_MARK:
        m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_doctype_system_identifier(UCHAR_REPLACEMENT);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        append_to_doctype_system_identifier(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_doctype_system_identifier_single_quoted_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_APOSTROPHE:
        m_ctxt_tokenizer.ts = TS_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
        break;

    case UCHAR_NULL:
        on_parse_error();
        append_to_doctype_system_identifier(UCHAR_REPLACEMENT);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        on_parse_error();
        set_force_quirks_flag();
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        append_to_doctype_system_identifier(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_after_doctype_system_identifier_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        on_parse_error();
        set_force_quirks_flag();
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_BOGUS_DOCTYPE;
        break;
    }
}

void HVMLParser::on_bogus_doctype_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        emit_doctype_token();
        break;

    case UCHAR_EOF:
        emit_doctype_token();
        emit_eof_token();
        break;

    default:
        break;
    }
}

void HVMLParser::on_cdata_section_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_RIGHT_SQUARE_BRACKET:
        m_ctxt_tokenizer.ts = TS_CDATA_SECTION_BRACKET;
        break;

    case UCHAR_EOF:
        on_parse_error();
        emit_eof_token();
        break;

    default:
        emit_character_token(m_ctxt_tokenizer.curr_uc);
        break;
    }
}

void HVMLParser::on_cdata_section_bracket_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_RIGHT_SQUARE_BRACKET:
        m_ctxt_tokenizer.ts = TS_CDATA_SECTION_END;
        break;

    default:
        emit_character_token(UCHAR_RIGHT_SQUARE_BRACKET);
        m_ctxt_tokenizer.ts = TS_CDATA_SECTION;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_cdata_section_end_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_RIGHT_SQUARE_BRACKET:
        emit_character_token(UCHAR_RIGHT_SQUARE_BRACKET);
        break;

    case UCHAR_GREATER_THAN_SIGN:
        m_ctxt_tokenizer.ts = TS_DATA;
        break;

    default:
        emit_character_token(UCHAR_RIGHT_SQUARE_BRACKET);
        emit_character_token(UCHAR_RIGHT_SQUARE_BRACKET);
        m_ctxt_tokenizer.ts = TS_CDATA_SECTION;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

bool HVMLParser::is_in_attribute_value(Uchar32 last_matched, int consumed)
{
    if ((m_ctxt_tokenizer.ts_return == TS_ATTRIBUTE_VALUE_DOUBLE_QUOTED ||
            m_ctxt_tokenizer.ts_return == TS_ATTRIBUTE_VALUE_SINGLE_QUOTED ||
            m_ctxt_tokenizer.ts_return == TS_ATTRIBUTE_VALUE_UNQUOTED) &&
            last_matched != UCHAR_SEMICOLON &&
            m_ctxt_tokenizer.total_len > consumed) {

        Uchar32 next_uc = 0;
        if (GetNextUChar(m_ctxt_tokenizer.lf,
                    m_ctxt_tokenizer.mchar + consumed,
                    m_ctxt_tokenizer.total_len - consumed, &next_uc) > 0 &&
                (next_uc == UCHAR_EQUAL_SIGN ||
                (next_uc >= UCHAR_DIGIT_ZERO &&
                    next_uc <= UCHAR_DIGIT_NINE) ||
                (next_uc >= UCHAR_LATIN_CAPITAL_LETTER_A &&
                    next_uc <= UCHAR_LATIN_CAPITAL_LETTER_Z) ||
                (next_uc >= UCHAR_LATIN_SMALL_LETTER_A &&
                    next_uc <= UCHAR_LATIN_SMALL_LETTER_Z))) {
            return true;
        }
    }

    return false;
}

bool HVMLParser::does_tb_consist_named_character_reference()
{
    const char* tb = m_ctxt_tokenizer.temp_buff.c_str();
    size_t tb_len = strlen(tb);

    if (MG_UNLIKELY (tb_len == 0))
        return false;

    if (tb[0] == '&' && tb[tb_len - 1] == ';') {
        for (size_t i = 1; i < tb_len - 1; i++) {
            if (!isalnum(tb[i]))
                return false;
        }

        return true;
    }

    return false;
}

#include "htmlentitiestable.inc"

static int entity_token_cmp(const void *a, const void *b)
{
    return strcmp(((struct _HtmlEntity*)a)->token,
            ((struct _HtmlEntity*)b)->token);
}

const Uchar32* HVMLParser::matchNamedCharacterReference(const char* entity)
{
    size_t len = strlen(entity);

    if (MG_UNLIKELY (len == 0 || len > MAX_ENTITY_TOKEN_LEN))
        return NULL;

    struct _HtmlEntity *found, key;

    key.token = entity;
    found = (struct _HtmlEntity *)bsearch (&key,
            _html_entities, TABLESIZE(_html_entities),
            sizeof(struct _HtmlEntity), entity_token_cmp);

    if (found)
        return found->ucs;

    return NULL;
}

#if 0
#define MAX_ENTITY_NUMERIC_CHAR_LEN     15

size_t HVMLParser::checkCharacterReference(const char* token, size_t len,
        char* mchar, int* mchar_len)
{
    size_t token_len = 0;
    const char* found;
    char real_token[MAX_ENTITY_TOKEN_LEN + 1];

    // find terminator ';' first
    do {
        if (token[token_len] == ';') {
            break;
        }
        else if (!isalnum(token[token_len])) {
            goto error;
        }

        token_len++;
        if (token_len >= len)
            goto error;

    } while (1);

    memset(real_token, 0, sizeof(real_token));
    memcpy(real_token, token, token_len);

    if (token_len > 1 && (real_token[0] == 'x' || real_token[0] == 'X') &&
            isxdigit(real_token[1])) {
        Uint32 uc;
        uc = (Uint32)strtol(real_token, NULL, 16);
        *mchar_len = uc32_to_utf8(uc, mchar);
    }
    else if (token_len > 1 && isdigit(real_token[0])) {
        Uint32 uc;
        uc = (Uint32)strtol(real_token, NULL, 10);
        *mchar_len = uc32_to_utf8(uc, mchar);
    }
    else if ((found = search_entity(real_token))) {
        *mchar_len = strlen(found);
        memcpy(mchar, found, *mchar_len);
    }

    if (*mchar_len > 0)
        // size of consumed bytes
        return token_len + 1;

error:
    return 0;
}
#endif

static int match_entity_cmp(const void *a, const void *b)
{
    return strncmp(((struct _HtmlEntity*)a)->token,
            ((struct _HtmlEntity*)b)->token, strlen(((struct _HtmlEntity*)a)->token));
}

static int match_entity(const char *token, int start_pos)
{
    struct _HtmlEntity *found, key;

    key.token = token;
    found = (struct _HtmlEntity *)bsearch (&key,
            _html_entities + start_pos, TABLESIZE(_html_entities) - start_pos,
            sizeof(struct _HtmlEntity), match_entity_cmp);

    if (found)
        return found - _html_entities;

    return -1;
}

const Uchar32* HVMLParser::try_to_match_named_character_reference(
            int* consumed, Uchar32* last_matched)
{
    int start_match_pos = 0;
    int last_matched_pos = -1;
    const char* mstr = m_ctxt_tokenizer.mchar;
    int mstr_len = m_ctxt_tokenizer.total_len;

    *consumed = 0;
    while (mstr_len > 0) {

        Uchar32 uc;
        int mclen;
        mclen = GetNextUChar(m_ctxt_tokenizer.lf, mstr, mstr_len, &uc);

        if (mclen > 0) {
            int matched_pos;

            append_to_temporary_buffer(uc);
            matched_pos = match_entity(m_ctxt_tokenizer.temp_buff.c_str(),
                start_match_pos);

            if (matched_pos < 0) {
                break;
            }
            else {
                *last_matched = uc;
                last_matched_pos = matched_pos;
                start_match_pos = matched_pos;
            }
        }
        else {
            break;
        }

        mstr += mclen;
        mstr_len -= mclen;
        *consumed += mclen;
    }

    if (last_matched_pos < 0) {
        return NULL;
    }
    else {
        return _html_entities[last_matched_pos].ucs;
    }
}

void HVMLParser::on_character_reference_state()
{
    clear_temporary_buffer();
    append_to_temporary_buffer(UCHAR_AMPERSAND);

    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_TABULATION:
    case UCHAR_LINE_FEED:
    case UCHAR_FORM_FEED:
    case UCHAR_SPACE:
    case UCHAR_LESS_THAN_SIGN:
    case UCHAR_AMPERSAND:
    case UCHAR_EOF:
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE_END;
        m_ctxt_tokenizer.consumed = 0;
        break;

    case UCHAR_NUMBER_SIGN:
        append_to_temporary_buffer(m_ctxt_tokenizer.curr_uc);
        m_ctxt_tokenizer.ts = TS_NUMERIC_CHARACTER_REFERENCE;
        break;

    default: {
        int consumed;
        const Uchar32* maped_ucs;
        Uchar32 last_matched;
        if ((maped_ucs = try_to_match_named_character_reference(
                &consumed, &last_matched))) {

            if (is_in_attribute_value(last_matched, consumed)) {
                m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE_END;
                break;
            }

            if (last_matched != UCHAR_SEMICOLON)
                on_parse_error();

            clear_temporary_buffer();
            append_to_temporary_buffer(maped_ucs[0]);
            if (maped_ucs[1])
                append_to_temporary_buffer(maped_ucs[1]);

            m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE_END;
            m_ctxt_tokenizer.consumed = consumed;
        }
        else {
            if (does_tb_consist_named_character_reference()) {
                on_parse_error();
            }

            m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE_END;
        }

        break;
    }
    }
}

void HVMLParser::on_numeric_character_reference_state()
{
    set_character_reference_code(0);

    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_LATIN_SMALL_LETTER_X:
    case UCHAR_LATIN_CAPITAL_LETTER_X:
        append_to_temporary_buffer(m_ctxt_tokenizer.curr_uc);
        m_ctxt_tokenizer.ts = TS_HEXADECIMAL_CHARACTER_REFERENCE_START;
        break;

    default:
        m_ctxt_tokenizer.ts = TS_DECIMAL_CHARACTER_REFERENCE_START;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_hexadecimal_character_reference_start_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_DIGIT_ZERO ... UCHAR_DIGIT_NINE:
    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_F:
    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_F:
        m_ctxt_tokenizer.ts = TS_HEXADECIMAL_CHARACTER_REFERENCE;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE_END;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_decimal_character_reference_start_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_DIGIT_ZERO ... UCHAR_DIGIT_NINE:
        m_ctxt_tokenizer.ts = TS_DECIMAL_CHARACTER_REFERENCE;
        m_ctxt_tokenizer.consumed = 0;
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_CHARACTER_REFERENCE_END;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_hexadecimal_character_reference_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_DIGIT_ZERO ... UCHAR_DIGIT_NINE:
        operate_character_reference_code(16,
                m_ctxt_tokenizer.curr_uc - 0x0030);
        break;

    case UCHAR_LATIN_CAPITAL_LETTER_A ... UCHAR_LATIN_CAPITAL_LETTER_F:
        operate_character_reference_code(16,
                m_ctxt_tokenizer.curr_uc - 0x0037);
        break;

    case UCHAR_LATIN_SMALL_LETTER_A ... UCHAR_LATIN_SMALL_LETTER_F:
        operate_character_reference_code(16,
                m_ctxt_tokenizer.curr_uc - 0x0057);
        break;

    case UCHAR_SEMICOLON:
        m_ctxt_tokenizer.ts = TS_NUMERIC_CHARACTER_REFERENCE_END;
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_NUMERIC_CHARACTER_REFERENCE_END;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

void HVMLParser::on_decimal_character_reference_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.next_uc) {
    case UCHAR_DIGIT_ZERO ... UCHAR_DIGIT_NINE:
        operate_character_reference_code(10,
                m_ctxt_tokenizer.curr_uc - 0x0030);
        break;

    case UCHAR_SEMICOLON:
        m_ctxt_tokenizer.ts = TS_NUMERIC_CHARACTER_REFERENCE_END;
        break;

    default:
        on_parse_error();
        m_ctxt_tokenizer.ts = TS_NUMERIC_CHARACTER_REFERENCE_END;
        m_ctxt_tokenizer.consumed = 0;
        break;
    }
}

static struct uc2uc {
    Uchar32 one;
    Uchar32 other;
} _bad_char_ref_code_map_table [] = {
    { 0x00,    0xFFFD }, //  REPLACEMENT CHARACTER
    { 0x80,    0x20AC }, //  EURO SIGN (€)
    { 0x82,    0x201A }, //  SINGLE LOW-9 QUOTATION MARK (‚)
    { 0x83,    0x0192 }, //  LATIN SMALL LETTER F WITH HOOK (ƒ)
    { 0x84,    0x201E }, //  DOUBLE LOW-9 QUOTATION MARK („)
    { 0x85,    0x2026 }, //  HORIZONTAL ELLIPSIS (…)
    { 0x86,    0x2020 }, //  DAGGER (†)
    { 0x87,    0x2021 }, //  DOUBLE DAGGER (‡)
    { 0x88,    0x02C6 }, //  MODIFIER LETTER CIRCUMFLEX ACCENT (ˆ)
    { 0x89,    0x2030 }, //  PER MILLE SIGN (‰)
    { 0x8A,    0x0160 }, //  LATIN CAPITAL LETTER S WITH CARON (Š)
    { 0x8B,    0x2039 }, //  SINGLE LEFT-POINTING ANGLE QUOTATION MARK (‹)
    { 0x8C,    0x0152 }, //  LATIN CAPITAL LIGATURE OE (Œ)
    { 0x8E,    0x017D }, //  LATIN CAPITAL LETTER Z WITH CARON (Ž)
    { 0x91,    0x2018 }, //  LEFT SINGLE QUOTATION MARK (‘)
    { 0x92,    0x2019 }, //  RIGHT SINGLE QUOTATION MARK (’)
    { 0x93,    0x201C }, //  LEFT DOUBLE QUOTATION MARK (“)
    { 0x94,    0x201D }, //  RIGHT DOUBLE QUOTATION MARK (”)
    { 0x95,    0x2022 }, //  BULLET (•)
    { 0x96,    0x2013 }, //  EN DASH (–)
    { 0x97,    0x2014 }, //  EM DASH (—)
    { 0x98,    0x02DC }, //  SMALL TILDE (˜)
    { 0x99,    0x2122 }, //  TRADE MARK SIGN (™)
    { 0x9A,    0x0161 }, //  LATIN SMALL LETTER S WITH CARON (š)
    { 0x9B,    0x203A }, //  SINGLE RIGHT-POINTING ANGLE QUOTATION MARK (›)
    { 0x9C,    0x0153 }, //  LATIN SMALL LIGATURE OE (œ)
    { 0x9E,    0x017E }, //  LATIN SMALL LETTER Z WITH CARON (ž)
    { 0x9F,    0x0178 }, //  LATIN CAPITAL LETTER Y WITH DIAERESIS (Ÿ)
};

static bool correct_char_ref_code (Uchar32 uc, Uchar32* corrected)
{
    int lower = 0;
    int upper = TABLESIZE (_bad_char_ref_code_map_table) - 1;
    int mid = TABLESIZE (_bad_char_ref_code_map_table) >> 1;

    if (uc < _bad_char_ref_code_map_table[lower].one
            || uc > _bad_char_ref_code_map_table[upper].one)
        return false;

    do {
        if (uc < _bad_char_ref_code_map_table[mid].one)
            upper = mid - 1;
        else if (uc > _bad_char_ref_code_map_table[mid].one)
            lower = mid + 1;
        else {
            *corrected = _bad_char_ref_code_map_table[mid].other;
            return true;
        }

        mid = (lower + upper) / 2;

    } while (lower <= upper);

    return false;
}

void HVMLParser::on_numeric_character_reference_end_state()
{
    Uchar32 uc = get_character_reference_code();

    if (correct_char_ref_code(uc, &uc))
        set_character_reference_code(uc);

    if (MG_UNLIKELY ((uc >= 0xD800 && uc <= 0xDFFF) || uc > 0x10FFFF)) {
        on_parse_error();
        set_character_reference_code(0xFFFD);
    }
    else if (MG_UNLIKELY (!isValidHvmlCharacter(uc))) {
        on_parse_error();
    }

    clear_temporary_buffer();
    append_to_temporary_buffer(uc);
    m_ctxt_tokenizer.ts = TS_NUMERIC_CHARACTER_REFERENCE_END;
}

void HVMLParser::on_character_reference_end_state()
{
    m_ctxt_tokenizer.consumed = m_ctxt_tokenizer.mclen;
    m_ctxt_tokenizer.curr_uc = m_ctxt_tokenizer.next_uc;

    switch (m_ctxt_tokenizer.ts_return) {
    case TS_ATTRIBUTE_VALUE_DOUBLE_QUOTED:
    case TS_ATTRIBUTE_VALUE_SINGLE_QUOTED:
    case TS_ATTRIBUTE_VALUE_UNQUOTED:
        append_to_attribute_value_from_temporary_buffer();
        break;

    default:
        emit_character_token_from_temporary_buffer();
        break;
    }

    m_ctxt_tokenizer.ts = m_ctxt_tokenizer.ts_return;
}

bool HVMLParser::tokenize()
{
    switch (m_ctxt_tokenizer.ts) {
    case TS_DATA:
        on_data_state();
        break;

    case TS_RCDATA:
        on_rcdata_state();
        break;

    case TS_RAWTEXT:
        on_rawtext_state();
        break;

    case TS_SCRIPT_DATA:
        on_script_data_state();
        break;

    case TS_PLAINTEXT:
        on_plaintext_state();
        break;

    case TS_TAG_OPEN:
        on_tag_open_state();
        break;

    case TS_END_TAG_OPEN:
        on_end_tag_open_state();
        break;

    case TS_TAG_NAME:
        on_tag_name_state();
        break;

    case TS_RCDATA_LESS_THAN_SIGN:
        on_rcdata_less_than_sign_state();
        break;

    case TS_RCDATA_END_TAG_OPEN:
        on_rcdata_end_tag_open_state();
        break;

    case TS_RCDATA_END_TAG_NAME:
        on_rcdata_end_tag_name_state();
        break;

    case TS_RAWTEXT_LESS_THAN_SIGN:
        on_rawtext_less_than_sign_state();
        break;

    case TS_RAWTEXT_END_TAG_OPEN:
        on_rawtext_end_tag_open_state();
        break;

    case TS_RAWTEXT_END_TAG_NAME:
        on_rawtext_end_tag_name_state();
        break;

    case TS_SCRIPT_DATA_LESS_THAN_SIGN:
        on_script_data_less_than_sign_state();
        break;

    case TS_SCRIPT_DATA_END_TAG_OPEN:
        on_script_data_end_tag_open_state();
        break;

    case TS_SCRIPT_DATA_END_TAG_NAME:
        on_script_data_end_tag_name_state();
        break;

    case TS_SCRIPT_DATA_ESCAPE_START:
        on_script_data_escape_start_state();
        break;

    case TS_SCRIPT_DATA_ESCAPE_START_DASH:
        on_script_data_escape_start_dash_state();
        break;

    case TS_SCRIPT_DATA_ESCAPED:
        on_script_data_escaped_state();
        break;

    case TS_SCRIPT_DATA_ESCAPED_DASH:
        on_script_data_escaped_dash_state();
        break;

    case TS_SCRIPT_DATA_ESCAPED_DASH_DASH:
        on_script_data_escaped_dash_dash_state();
        break;

    case TS_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN:
        on_script_data_escaped_less_than_sign_state();
        break;

    case TS_SCRIPT_DATA_ESCAPED_END_TAG_OPEN:
        on_script_data_escaped_end_tag_open_state();
        break;

    case TS_SCRIPT_DATA_ESCAPED_END_TAG_NAME:
        on_script_data_escaped_end_tag_name_state();
        break;

    case TS_SCRIPT_DATA_DOUBLE_ESCAPE_START:
        on_script_data_double_escape_start_state();
        break;

    case TS_SCRIPT_DATA_DOUBLE_ESCAPED:
        on_script_data_double_escaped_state();
        break;

    case TS_SCRIPT_DATA_DOUBLE_ESCAPED_DASH:
        on_script_data_double_escaped_dash_state();
        break;

    case TS_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH:
        on_script_data_double_escaped_dash_dash_state();
        break;

    case TS_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN:
        on_script_data_double_escaped_less_than_sign_state();
        break;

    case TS_SCRIPT_DATA_DOUBLE_ESCAPE_END:
        on_script_data_double_escape_end_state();
        break;

    case TS_BEFORE_ATTRIBUTE_NAME:
        on_before_attribute_name_state();
        break;

    case TS_ATTRIBUTE_NAME:
        on_attribute_name_state();
        break;

    case TS_AFTER_ATTRIBUTE_NAME:
        on_after_attribute_name_state();
        break;

    case TS_BEFORE_ATTRIBUTE_VALUE:
        on_before_attribute_value_state();
        break;

    case TS_ATTRIBUTE_VALUE_DOUBLE_QUOTED:
        on_attribute_value_double_quoted_state();
        break;

    case TS_ATTRIBUTE_VALUE_SINGLE_QUOTED:
        on_attribute_value_single_quoted_state();
        break;

    case TS_ATTRIBUTE_VALUE_UNQUOTED:
        on_attribute_value_unquoted_state();
        break;

    case TS_AFTER_ATTRIBUTE_VALUE_QUOTED:
        on_after_attribute_value_quoted_state();
        break;

    case TS_SELF_CLOSING_START_TAG:
        on_self_closing_start_tag_state();
        break;

    case TS_BOGUS_COMMENT:
        on_bogus_comment_state();
        break;

    case TS_MARKUP_DECLARATION_OPEN:
        on_markup_declaration_open_state();
        break;

    case TS_COMMENT_START:
        on_comment_start_state();
        break;

    case TS_COMMENT_START_DASH:
        on_comment_start_dash_state();
        break;

    case TS_COMMENT:
        on_comment_state();
        break;

    case TS_COMMENT_LESS_THAN_SIGN:
        on_comment_less_than_sign_state();
        break;

    case TS_COMMENT_LESS_THAN_SIGN_BANG:
        on_comment_less_than_sign_bang_state();
        break;

    case TS_COMMENT_LESS_THAN_SIGN_BANG_DASH:
        on_comment_less_than_sign_bang_dash_state();
        break;

    case TS_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH:
        on_comment_less_than_sign_bang_dash_dash_state();
        break;

    case TS_COMMENT_END_DASH:
        on_comment_end_dash_state();
        break;

    case TS_COMMENT_END:
        on_comment_end_state();
        break;

    case TS_COMMENT_END_BANG:
        on_comment_end_bang_state();
        break;

    case TS_DOCTYPE:
        on_doctype_state();
        break;

    case TS_BEFORE_DOCTYPE_NAME:
        on_before_doctype_name_state();
        break;

    case TS_DOCTYPE_NAME:
        on_doctype_name_state();
        break;

    case TS_AFTER_DOCTYPE_NAME:
        on_after_doctype_name_state();
        break;

    case TS_AFTER_DOCTYPE_PUBLIC_KEYWORD:
        on_after_doctype_public_keyword_state();
        break;

    case TS_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:
        on_before_doctype_public_identifier_state();
        break;

    case TS_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED:
        on_doctype_public_identifier_double_quoted_state();
        break;

    case TS_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED:
        on_doctype_public_identifier_single_quoted_state();
        break;

    case TS_AFTER_DOCTYPE_PUBLIC_IDENTIFIER:
        on_after_doctype_public_identifier_state();
        break;

    case TS_BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDENTIFIERS:
        on_between_doctype_public_system_identifiers_state();
        break;

    case TS_AFTER_DOCTYPE_SYSTEM_KEYWORD:
        on_after_doctype_system_keyword_state();
        break;

    case TS_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER:
        on_before_doctype_system_identifier_state();
        break;

    case TS_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED:
        on_doctype_system_identifier_double_quoted_state();
        break;

    case TS_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED:
        on_doctype_system_identifier_single_quoted_state();
        break;

    case TS_AFTER_DOCTYPE_SYSTEM_IDENTIFIER:
        on_after_doctype_system_identifier_state();
        break;

    case TS_BOGUS_DOCTYPE:
        on_bogus_doctype_state();
        break;

    case TS_CDATA_SECTION:
        on_cdata_section_state();
        break;

    case TS_CDATA_SECTION_BRACKET:
        on_cdata_section_bracket_state();
        break;

    case TS_CDATA_SECTION_END:
        on_cdata_section_end_state();
        break;

    case TS_CHARACTER_REFERENCE:
        on_character_reference_state();
        break;

    case TS_NUMERIC_CHARACTER_REFERENCE:
        on_numeric_character_reference_state();
        break;

    case TS_HEXADECIMAL_CHARACTER_REFERENCE_START:
        on_hexadecimal_character_reference_start_state();
        break;

    case TS_DECIMAL_CHARACTER_REFERENCE_START:
        on_decimal_character_reference_start_state();
        break;

    case TS_HEXADECIMAL_CHARACTER_REFERENCE:
        on_hexadecimal_character_reference_state();
        break;

    case TS_DECIMAL_CHARACTER_REFERENCE:
        on_decimal_character_reference_state();
        break;

    case TS_NUMERIC_CHARACTER_REFERENCE_END:
        on_numeric_character_reference_end_state();
        break;

    case TS_CHARACTER_REFERENCE_END:
        on_character_reference_end_state();
        break;
    }

    return true;
}

size_t HVMLParser::parse(View* parent, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char)
{
    // TODO: construct a fack node for the fragment parsing
    m_ctxt_fragment.view = parent;

    size_t consumed = 0;
    if (content == NULL) {
        // EOF

        len = 0;

        m_ctxt_tokenizer.mchar = "EOF";
        m_ctxt_tokenizer.mclen = 3;
        m_ctxt_tokenizer.total_len = 3;
        m_ctxt_tokenizer.next_uc = UCHAR_EOF;
        m_ctxt_tokenizer.curr_uc = 0;

        m_ctxt_tokenizer.errors = 0;
        m_ctxt_tokenizer.consumed = 0;

        tokenize();
    }
    else {

        const char* mstr = content;
        int mstr_len = len;

        while (mstr_len > 0) {
            Uchar32 uc;
            int mclen;

            mclen = GetNextUChar(m_ctxt_tokenizer.lf, mstr, mstr_len, &uc);
            if (mclen == 0) {
                // badly encoded or end of text
                break;
            }

            if (!isValidHvmlCharacter(uc)) {
                on_parse_error();
                break;
            }

            m_ctxt_tokenizer.mchar = mstr;
            m_ctxt_tokenizer.mclen = mclen;
            m_ctxt_tokenizer.total_len = mstr_len;
            m_ctxt_tokenizer.next_uc = uc;
            m_ctxt_tokenizer.curr_uc = 0;

            m_ctxt_tokenizer.errors = 0;
            m_ctxt_tokenizer.consumed = 0;

            do {
                tokenize();
                if (m_ctxt_tokenizer.errors > 0) {
                    goto done;
                }
            } while (m_ctxt_tokenizer.consumed == 0);

            consumed += m_ctxt_tokenizer.consumed;
            mstr += m_ctxt_tokenizer.consumed;
            mstr_len -= m_ctxt_tokenizer.consumed;
        }
    }

#if 0
    if (m_len_left > 2 && m_input_content[0] == '&') {
        char mchar[10];
        int mchar_len;
        checkCharacterReference(content + 1, len - 1, mchar, &mchar_len);
    }
#endif

done:
    if (m_ctxt_tokenizer.errors > 0) {
        _DBG_PRINTF("%s: Parse error at %d (%c",
                __func__, consumed, content[consumed]);

        if (consumed + 1 < len) {
            _DBG_PRINTF("%c", content[1]);
            if (consumed + 2 < len) {
                _DBG_PRINTF("%c", content[2]);
                if (consumed + 3 < len) {
                    _DBG_PRINTF("%c", content[3]);
                }
            }
        }
        _DBG_PRINTF("...)\n");
    }

    return consumed;
}

/*
 * This function implements the steps to reset the inserting mode appropriately.
 * For more information, please refer to:
 *
 * https://www.w3.org/TR/html52/syntax.html#the-insertion-mode
 */
void HVMLParser::reset_inserting_mode()
{
    bool last = false;
    OpenElementNode* node = (OpenElementNode*)m_stack_oe.prev;

    assert (!(list_empty(&m_stack_oe)));

    do {

        if (node == (OpenElementNode*)m_stack_oe.next) {
            last = true;
            if (m_ctxt_fragment.view) {
                node = &m_ctxt_fragment;
            }
        }

        const char* tag = node->view->tag();
        if (strcmp(tag, "select") == 0) {

            if (last) {
                m_im_current = IM_IN_SELECT;
                goto done;
            }

            int im = IM_IN_SELECT;
            while (1) {

                OpenElementNode* ancestor = node;
                //  If ancestor is the first node
                if (ancestor == (OpenElementNode*)m_stack_oe.next) {
                    break;
                }

                ancestor = (OpenElementNode*)ancestor->list.prev;
                if (strcmp(ancestor->view->tag(), "template") == 0) {
                    break;
                }
                else if (strcmp(ancestor->view->tag(), "table") == 0) {
                    im = IM_IN_SELECT_IN_TABLE;
                    break;
                }
            }

            m_im_current = im;
            goto done;
        }
        else if (!last && (strcmp(tag, "td") == 0 || strcmp(tag, "th") == 0)) {
            m_im_current = IM_IN_CELL;
            goto done;
        }
        else if (strcmp(tag, "tr") == 0) {
            m_im_current = IM_IN_ROW;
            goto done;
        }
        else if (strcmp(tag, "tbody") == 0 || strcmp(tag, "thead") == 0 ||
                 strcmp(tag, "tfoot") == 0) {
            m_im_current = IM_IN_TABLE_BODY;
            goto done;
        }
        else if (strcmp(tag, "caption") == 0) {
            m_im_current = IM_IN_CAPTION;
            goto done;
        }
        else if (strcmp(tag, "colgroup") == 0) {
            m_im_current = IM_IN_COLUMN_GROUP;
            goto done;
        }
        else if (strcmp(tag, "table") == 0) {
            m_im_current = IM_IN_TABLE;
            goto done;
        }
        else if (strcmp(tag, "template") == 0) {
            // switch the insertion mode to the current template insertion mode
            m_im_current = m_stack_tim[m_stack_tim_top];
            goto done;
        }
        else if (!last && strcmp(tag, "head") == 0) {
            m_im_current = IM_IN_HEAD;
            goto done;
        }
        else if (strcmp(tag, "body") == 0) {
            m_im_current = IM_IN_BODY;
            goto done;
        }
        else if (strcmp(tag, "frameset") == 0) {
            m_im_current = IM_IN_FRAMESET;
            goto done;
        }
        else if (strcmp(tag, "html") == 0) {
            if (m_head_element == NULL) {
                m_im_current = IM_BEFORE_HEAD;
                goto done;
            }
            else {
                m_im_current = IM_AFTER_HEAD;
                goto done;
            }
        }
        else if (last) {
            m_im_current = IM_IN_BODY;
            goto done;
        }

        node = (OpenElementNode*)node->list.prev;
    } while (1);

done:
    return;
}

bool HVMLParser::is_open_element(const View* view)
{
    return false;
}

View* HVMLParser::insert_new_element(const View* view)
{
    return NULL;
}

bool HVMLParser::check_adjusted_current_node()
{
    return false;
}

void HVMLParser::emit_character_token(Uchar32 uc)
{
    // TODO
}

void HVMLParser::emit_character_token(const std::string& utf8_str)
{
    const char* utf8 = utf8_str.c_str();

    int t;
    Uchar32 uc;
    while ((t = utf8_to_uc32(utf8, -1, &uc)) > 0) {
        emit_character_token(uc);
        utf8 += t;
    }
}

void HVMLParser::emit_eof_token()
{
    // TODO
}

void HVMLParser::emit_tag_token()
{
    assert(m_ctxt_tokenizer.dt);

    if (!m_ctxt_tokenizer.tt->attr_name.empty()) {
        if (!m_ctxt_tokenizer.tt->add_attr())
            on_parse_error();
    }

    m_ctxt_tokenizer.tt->attr_name.clear();
    m_ctxt_tokenizer.tt->attr_value.clear();
    m_ctxt_tokenizer.tt->attrs.clear();

    if (m_ctxt_tokenizer.tt->is_start) {
        m_ctxt_tokenizer.last_emitted = m_ctxt_tokenizer.tt->tag_name;
    }

    // TODO
}

void HVMLParser::emit_doctype_token()
{
    assert(m_ctxt_tokenizer.dt);

    // TODO

    delete m_ctxt_tokenizer.dt;
    m_ctxt_tokenizer.dt = NULL;
}

} // namespace hfcl

