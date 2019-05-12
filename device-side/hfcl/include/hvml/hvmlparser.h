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

#ifndef HFCL_VIEW_HVMLPARSER_H_
#define HFCL_VIEW_HVMLPARSER_H_

#include "../common/common.h"
#include "../common/helpers.h"
#include "../common/list.h"

#include <string>
#include <map>

#include <minigui/minigui.h>
#include <minigui/gdi.h>

namespace hfcl {

typedef void* (*CB_ON_NEW_NODE)(void* context, const char* tag);
typedef void  (*CB_ON_NEW_ATTR)(void* context,
        const char* attrKey, const char* attrValue);
typedef int   (*CB_ON_NEW_CHAR)(void* context, const char* text);

class View;
class ViewContainer;

class HVMLParser {
public:
    HVMLParser(size_t stackSize, const char* encoding = "UTF-8");
    ~HVMLParser();

    bool reset(size_t stackSize, const char* encoding = "UTF-8");
    size_t parse(View* parent, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char);

    /**
     * Check whether the given Unicode code point is valid for HVML.
     */
    static bool isValidHvmlCharacter(Uchar32 uc);

    /**
     * Match a well-defined named character reference, and
     * return the Unicode codepoint(s) if matched.
     *
     * The well-defined named character reference means the entity
     * consists of a U+0026 AMPERSAND character (&) followed by a
     * sequence of one or more alphanumeric ASCII characters and a
     * U+003B SEMICOLON character (;).
     *
     * If not matched, this function returns NULL.
     *
     * Note that the \a entity should be encoded in UTF-8.
     */
    static const Uchar32* matchNamedCharacterReference(const char* entity);

protected:
    // the tokenizer states
    typedef enum _TokenizerStates {
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
    } TokenizerStates;

    typedef enum _TokenTypes {
        TT_NONE = 0,
        TT_EOF,
        TT_CHARACTER,
    } TokenTypes;

    typedef enum _InsertingModes {
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
    } InsertingModes;

    typedef struct _OpenElementNode {
        list_t      list;
        const char* tag;
        int         tag_idx;
        const View* view;
    } OpenElementNode;

    bool is_open_element(const View* view);
    View* insert_new_element(const View* view);
    bool check_adjusted_current_node();

    typedef struct _ActiveFormattingEle {
        list_t      list;
        const View* view;
        bool        is_marker;
    } ActiveFormattingEle;

    void reset_inserting_mode();

    // operators for active formatting list
    void push_new_afe(const View* view, bool isMarker = false);
    void rebuild_afe_list();
    void clear_up_to_last_marker();

    class DoctypeToken {
    public:
        DoctypeToken() {
            force_quirks = false;
            pub_id_set = false;
            sys_id_set = false;
        }

        bool            force_quirks;
        bool            pub_id_set;
        bool            sys_id_set;
        std::string     name;
        std::string     pub_id;
        std::string     sys_id;
    };

    class TagToken {
    public:
        TagToken (bool start_or_end = true) {
            is_start = start_or_end;
            self_closing = false;
        }
        ~TagToken () {
            attrs.clear();
        }

        bool add_attr() {
            std::map<std::string, std::string>::iterator it;
            it = attrs.find(attr_name);
            if (it != attrs.end())
                return false;

            attrs[attr_name] = attr_value;
            return true;
        }

        bool            is_start;
        bool            self_closing;
        std::string     tag_name;
        std::string     attr_name;
        std::string     attr_value;
        std::map<std::string, std::string> attrs;
    };

    typedef struct _TokenizerContext {
        LOGFONT*        lf;
        const char*     mchar;      // pointer to current character position

        int             mclen;      // current mchar length
        int             total_len;  // left string length
        Uchar32         next_uc;    // Unicode code point of next character
        Uchar32         curr_uc;    // current/consumed character

        int             errors;     // the embedded level of errors
        int             consumed;   // consumed length

        Uchar32         ref_code;   // the character reference code

        TokenizerStates ts;         // current tokenizer state
        TokenizerStates ts_return;  // the return state
        //TokenTypes      tt;       // the token type

        Uchar32         char_ref_code;  // character reference code
        std::string     temp_buff;      // the temporary buffer
        std::string     last_emitted;   // the last emitted start tag name

        DoctypeToken    *dt;        // the DOCTYPE token
        TagToken        *tt;        // the start or end tag token

#ifdef DEBUG
        std::string     comment;    // the current comment
#endif
    } TokenizerContext;

private:
    CB_ON_NEW_NODE  m_on_new_node;
    CB_ON_NEW_ATTR  m_on_new_attr;
    CB_ON_NEW_CHAR  m_on_new_char;

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
    OpenElementNode m_ctxt_fragment;

    // The element pointers
    const View*     m_head_element;
    const View*     m_form_element;

    TokenizerContext m_ctxt_tokenizer;
    bool            reset_tokenizer(const char* encoding);

    bool does_characters_match_word(const char* word, int* consumed,
            bool case_insensitive = true);

    void append_uc32_to_utf8_string(Uchar32 uc, std::string& str) {
        char utf8[7];
        int len;

        len = uc32_to_utf8(uc, utf8);
        if (len > 0) {
            utf8[len] = 0;
            str += utf8;
        }
    }

    /* operators for temporary buffer */
    void clear_temporary_buffer() {
        m_ctxt_tokenizer.temp_buff.clear();
    }

    void append_to_temporary_buffer(Uchar32 uc) {
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.temp_buff);
    }

    bool check_temporary_buffer(const char* key) {
        return m_ctxt_tokenizer.temp_buff.compare(key) == 0;
    }

    void emit_character_token_from_temporary_buffer() {
        emit_character_token(m_ctxt_tokenizer.temp_buff);
    }

    bool does_tb_consist_named_character_reference();

    /* operators for character reference */
    void set_character_reference_code(Uchar32 uc) {
        m_ctxt_tokenizer.char_ref_code = uc;
    }

    void operate_character_reference_code(int multiplier, int addend) {
        m_ctxt_tokenizer.char_ref_code *= multiplier;
        m_ctxt_tokenizer.char_ref_code += addend;
    }

    Uchar32 get_character_reference_code() {
        return m_ctxt_tokenizer.char_ref_code;
    }

    bool is_in_attribute_value(Uchar32 last_matched, int consumed);
    const Uchar32* try_to_match_named_character_reference(int* consumed,
            Uchar32* last_matched);

    /* operators for character token */
    void emit_character_token(Uchar32 uc);
    void emit_character_token(const std::string& utf8_str);

    /* operators for EOF token */
    void emit_eof_token();

    /* operators for start/end tag token */
    void create_start_tag_token() {
        if (m_ctxt_tokenizer.tt) {
            delete m_ctxt_tokenizer.tt;
        }
        m_ctxt_tokenizer.tt = new TagToken(true);
    }

    void create_end_tag_token() {
        if (m_ctxt_tokenizer.tt) {
            delete m_ctxt_tokenizer.tt;
        }
        m_ctxt_tokenizer.tt = new TagToken(false);
    }

    void append_to_tag_name(Uchar32 uc) {
        assert(m_ctxt_tokenizer.tt);
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.tt->tag_name);
    }

    void start_new_attribute(const char* name = "", const char* value = "") {
        assert(m_ctxt_tokenizer.tt);
        if (!m_ctxt_tokenizer.tt->attr_name.empty()) {
            if (!m_ctxt_tokenizer.tt->add_attr())
                on_parse_error();
        }

        m_ctxt_tokenizer.tt->attr_name.clear();
        m_ctxt_tokenizer.tt->attr_value.clear();
    }

    void append_to_attribute_name(Uchar32 uc) {
        assert(m_ctxt_tokenizer.tt);
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.tt->attr_name);
    }

    void append_to_attribute_value(Uchar32 uc) {
        assert(m_ctxt_tokenizer.tt);
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.tt->attr_value);
    }

    void append_to_attribute_value_from_temporary_buffer() {
        assert(m_ctxt_tokenizer.tt);
        m_ctxt_tokenizer.tt->attr_value += m_ctxt_tokenizer.temp_buff;
    }

    void set_self_closing_flag() {
        assert(m_ctxt_tokenizer.tt);
        m_ctxt_tokenizer.tt->self_closing = true;
    }

    bool is_appropriate_end_tag() {
        assert(m_ctxt_tokenizer.tt);

        if ((!m_ctxt_tokenizer.tt->is_start) &&
                m_ctxt_tokenizer.tt->tag_name == m_ctxt_tokenizer.last_emitted)
            return true;

        return false;
    }

    void emit_tag_token();

    /* operators for comment token; we just ignore the comment */
    void create_comment_token() { }
    void append_to_comment(Uchar32 uc) {
#ifdef DEBUG
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.comment);
#endif
    }
    void emit_comment_token() {
#ifdef DEBUG
        _DBG_PRINTF("HVML Parser: got a comment: %s\n",
            m_ctxt_tokenizer.comment.c_str());
#endif
    }

    /* operators for DOCTYPE token */
    void create_doctype_token() {
        if (m_ctxt_tokenizer.dt) {
            delete m_ctxt_tokenizer.dt;
        }
        m_ctxt_tokenizer.dt = new DoctypeToken;
    }

    void set_force_quirks_flag(bool on_or_off = true) {
        assert(m_ctxt_tokenizer.dt);
        m_ctxt_tokenizer.dt->force_quirks = on_or_off;
    }

    void append_to_doctype_token_name(Uchar32 uc) {
        assert(m_ctxt_tokenizer.dt);
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.dt->name);
    }

    void set_doctype_public_identifier_empty() {
        assert(m_ctxt_tokenizer.dt);
        m_ctxt_tokenizer.dt->pub_id.clear();
        m_ctxt_tokenizer.dt->pub_id_set = true;
    }

    void append_to_doctype_public_identifier(Uchar32 uc) {
        assert(m_ctxt_tokenizer.dt);
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.dt->pub_id);
    }

    void set_doctype_system_identifier_empty() {
        assert(m_ctxt_tokenizer.dt);
        m_ctxt_tokenizer.dt->sys_id.clear();
        m_ctxt_tokenizer.dt->sys_id_set = true;
    }

    void append_to_doctype_system_identifier(Uchar32 uc) {
        assert(m_ctxt_tokenizer.dt);
        append_uc32_to_utf8_string(uc, m_ctxt_tokenizer.dt->sys_id);
    }

    void emit_doctype_token();

    bool tokenize();

    void on_parse_error();
    void on_data_state();
    void on_rcdata_state();
    void on_rawtext_state();
    void on_script_data_state();
    void on_plaintext_state();
    void on_tag_open_state();
    void on_end_tag_open_state();
    void on_tag_name_state();
    void on_rcdata_less_than_sign_state();
    void on_rcdata_end_tag_open_state();
    void on_rcdata_end_tag_name_state();
    void on_rawtext_less_than_sign_state();
    void on_rawtext_end_tag_open_state();
    void on_rawtext_end_tag_name_state();
    void on_script_data_less_than_sign_state();
    void on_script_data_end_tag_open_state();
    void on_script_data_end_tag_name_state();
    void on_script_data_escape_start_state();
    void on_script_data_escape_start_dash_state();
    void on_script_data_escaped_state();
    void on_script_data_escaped_dash_state();
    void on_script_data_escaped_dash_dash_state();
    void on_script_data_escaped_less_than_sign_state();
    void on_script_data_escaped_end_tag_open_state();
    void on_script_data_escaped_end_tag_name_state();
    void on_script_data_double_escape_start_state();
    void on_script_data_double_escaped_state();
    void on_script_data_double_escaped_dash_state();
    void on_script_data_double_escaped_dash_dash_state();
    void on_script_data_double_escaped_less_than_sign_state();
    void on_script_data_double_escape_end_state();
    void on_before_attribute_name_state();
    void on_attribute_name_state();
    void on_after_attribute_name_state();
    void on_before_attribute_value_state();
    void on_attribute_value_double_quoted_state();
    void on_attribute_value_single_quoted_state();
    void on_attribute_value_unquoted_state();
    void on_after_attribute_value_quoted_state();
    void on_self_closing_start_tag_state();
    void on_bogus_comment_state();
    void on_markup_declaration_open_state();
    void on_comment_start_state();
    void on_comment_start_dash_state();
    void on_comment_state();
    void on_comment_less_than_sign_state();
    void on_comment_less_than_sign_bang_state();
    void on_comment_less_than_sign_bang_dash_state();
    void on_comment_less_than_sign_bang_dash_dash_state();
    void on_comment_end_dash_state();
    void on_comment_end_state();
    void on_comment_end_bang_state();
    void on_doctype_state();
    void on_before_doctype_name_state();
    void on_doctype_name_state();
    void on_after_doctype_name_state();
    void on_after_doctype_public_keyword_state();
    void on_before_doctype_public_identifier_state();
    void on_doctype_public_identifier_double_quoted_state();
    void on_doctype_public_identifier_single_quoted_state();
    void on_after_doctype_public_identifier_state();
    void on_between_doctype_public_system_identifiers_state();
    void on_after_doctype_system_keyword_state();
    void on_before_doctype_system_identifier_state();
    void on_doctype_system_identifier_double_quoted_state();
    void on_doctype_system_identifier_single_quoted_state();
    void on_after_doctype_system_identifier_state();
    void on_bogus_doctype_state();
    void on_cdata_section_state();
    void on_cdata_section_bracket_state();
    void on_cdata_section_end_state();
    void on_character_reference_state();
    void on_numeric_character_reference_state();
    void on_hexadecimal_character_reference_start_state();
    void on_decimal_character_reference_start_state();
    void on_hexadecimal_character_reference_state();
    void on_decimal_character_reference_state();
    void on_numeric_character_reference_end_state();
    void on_character_reference_end_state();

};

} // namespace hfcl

#endif /* HFCL_VIEW_HVMLPARSER_H_ */

