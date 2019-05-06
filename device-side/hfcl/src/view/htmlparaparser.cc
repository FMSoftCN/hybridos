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
** htmlparaparser.cc: The implementation of HtmlParaParser class.
**
** Create by WEI Yongming at 2019/04/26
*/

#include "view/htmlparaparser.h"

#include "common/helpers.h"
#include "view/view.h"
#include "view/viewcontainer.h"

#if SIZEOF_PTR == 8
#define MIN_STACK_SIZE          16
#else
#define MIN_STACK_SIZE          8
#endif

namespace hfcl {

HtmlParaParser::HtmlParaParser(size_t stackSize)
    : m_input_content(NULL)
    , m_len_left(0)
{
    INIT_LIST_HEAD(&m_stack_oe);

    reset_stack_tim(stackSize);

    INIT_LIST_HEAD(&m_list_afe);

    memset(&m_context, 0, sizeof(m_context));

    m_head_element = NULL;
    m_form_element = NULL;
}

HtmlParaParser::~HtmlParaParser()
{
    reset_stack_oe(0);
    reset_stack_tim(0);
    reset_list_afe();
}

void HtmlParaParser::reset_stack_oe(int stackSize)
{
    while (!list_empty(&m_stack_oe)) {
        OpenElementNode* oes = (OpenElementNode*)m_stack_oe.next;
        list_del(m_stack_oe.next);
        mg_slice_delete(OpenElementNode, oes);
    }

    INIT_LIST_HEAD(&m_stack_oe);
}

void HtmlParaParser::reset_stack_tim(int stackSize)
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

void HtmlParaParser::reset_list_afe()
{
    while (!list_empty(&m_list_afe)) {
        ActiveFormattingEle* afe = (ActiveFormattingEle*)m_list_afe.next;
        list_del(m_list_afe.next);
        mg_slice_delete(ActiveFormattingEle, afe);
    }

    INIT_LIST_HEAD(&m_list_afe);
}

void HtmlParaParser::pushNewAfe(const View* view, bool isMarker)
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

void HtmlParaParser::rebuildAfeList()
{
    if (list_empty(&m_list_afe))
        return;

    ActiveFormattingEle* afe = (ActiveFormattingEle*)m_list_afe.prev;
    if (afe->is_marker || isOpenElement(afe->view))
        return;

rewind:

    if (afe->list.prev == &m_list_afe) {

        do {
            // Insert an HTML element for the token for which the element entry
            // was created, to obtain new element.
            View* v = insertNewElement(afe->view);
            afe->view = v;

            afe = (ActiveFormattingEle*)afe->list.next;
        } while (&afe->list != &m_list_afe);
    }
    else {
        afe = (ActiveFormattingEle*)afe->list.prev;
        if (!afe->is_marker && !isOpenElement(afe->view)) {
            goto rewind;
        }
    }
}

void HtmlParaParser::clearUpToLastMarker()
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

void HtmlParaParser::reset(size_t stackSize)
{
    reset_stack_oe(stackSize);
    reset_stack_tim(stackSize);
    reset_list_afe();

    m_head_element = NULL;
    m_form_element = NULL;

    m_input_content = NULL;
    m_len_left = 0;
}

bool HtmlParaParser::isValidCharacter(Uchar32 uc)
{
    if (MG_UNLIKELY((uc >= 0x0001 && uc <= 0x0008) || uc == 0x000B ||
            (uc >= 0x000E && uc <= 0x001F) ||
            (uc >= 0x007F && uc <= 0x009F) ||
            (uc >= 0xFDD0 && uc <= 0xFDEF))) {
            _DBG_PRINTF("%s(0x%06X): Any occurrences of any characters in the ranges U+0001 to U+0008, U+000E to U+001F, U+007F to U+009F, U+FDD0 to U+FDEF, and characters U+000B, U+FFFE, U+FFFF, U+1FFFE, U+1FFFF, U+2FFFE, U+2FFFF, U+3FFFE, U+3FFFF, U+4FFFE, U+4FFFF, U+5FFFE, U+5FFFF, U+6FFFE, U+6FFFF, U+7FFFE, U+7FFFF, U+8FFFE, U+8FFFF, U+9FFFE, U+9FFFF, U+AFFFE, U+AFFFF, U+BFFFE, U+BFFFF, U+CFFFE, U+CFFFF, U+DFFFE, U+DFFFF, U+EFFFE, U+EFFFF, U+FFFFE, U+FFFFF, U+10FFFE, and U+10FFFF are parse errors.\n",
                    __func__, uc);
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
            _DBG_PRINTF("%s(0x%06X): Any occurrences of any characters in the ranges U+0001 to U+0008, U+000E to U+001F, U+007F to U+009F, U+FDD0 to U+FDEF, and characters U+000B, U+FFFE, U+FFFF, U+1FFFE, U+1FFFF, U+2FFFE, U+2FFFF, U+3FFFE, U+3FFFF, U+4FFFE, U+4FFFF, U+5FFFE, U+5FFFF, U+6FFFE, U+6FFFF, U+7FFFE, U+7FFFF, U+8FFFE, U+8FFFF, U+9FFFE, U+9FFFF, U+AFFFE, U+AFFFF, U+BFFFE, U+BFFFF, U+CFFFE, U+CFFFF, U+DFFFE, U+DFFFF, U+EFFFE, U+EFFFF, U+FFFFE, U+FFFFF, U+10FFFE, and U+10FFFF are parse errors.\n",
                    __func__, uc);
            return false;
        }
    }

    if (MG_UNLIKELY(UCharGetCategory(uc) == UCHAR_CATEGORY_UNASSIGNED)) {
        _DBG_PRINTF("%s(0x%06X): Any character that is a not a Unicode character, i.e., any isolated surrogate, is a parse error.\n", __func__, uc);
        return false;
    }

    return true;
}

#include "htmlentitiestable.inc"

static int entity_token_comp(const void *a, const void *b)
{
    return strcmp(((struct _HtmlEntity*)a)->token,
            ((struct _HtmlEntity*)b)->token);
}

static const char* search_entity(const char *token)
{
    struct _HtmlEntity *found, key;

    key.token = token;
    found = (struct _HtmlEntity *)bsearch (&key,
            _html_entities, TABLESIZE(_html_entities),
            sizeof(struct _HtmlEntity), entity_token_comp);

    if (found)
        return found->utf8;

    return NULL;
}

#define MAX_ENTITY_NUMERIC_CHAR_LEN     15

size_t HtmlParaParser::checkCharacterReference(const char* token, size_t len,
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

size_t HtmlParaParser::parse(View* parent, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char)
{
    m_context.view = parent;
    m_input_content = content;
    m_len_left = len;

    if (m_len_left > 2 && m_input_content[0] == '&') {
        char mchar[10];
        int mchar_len;
        checkCharacterReference(content + 1, len - 1, mchar, &mchar_len);
    }

    return 0;
}

/*
 * This function implements the steps to reset the inserting mode appropriately.
 * For more information, please refer to:
 *
 * https://www.w3.org/TR/html52/syntax.html#the-insertion-mode
 */
void HtmlParaParser::resetInsertingMode()
{
    bool last = false;
    OpenElementNode* node = (OpenElementNode*)m_stack_oe.prev;

    assert (!(list_empty(&m_stack_oe)));

    do {

        if (node == (OpenElementNode*)m_stack_oe.next) {
            last = true;
            if (m_context.view) {
                node = &m_context;
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

} // namespace hfcl

