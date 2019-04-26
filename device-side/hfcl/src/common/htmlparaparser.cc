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

#include "common/htmlparaparser.h"

#include "common/helpers.h"

#define MIN_STACK_SIZE          8

namespace hfcl {

HtmlParaParser::HtmlParaParser(size_t stackSize)
    : m_stack_size((stackSize < MIN_STACK_SIZE) ? MIN_STACK_SIZE : stackSize)
    , m_stack_top(0)
    , m_content(NULL)
    , m_len_left(0)
{
    m_stack = new TagState[m_stack_size];
}

HtmlParaParser::~HtmlParaParser()
{
    delete [] m_stack;
}

void HtmlParaParser::reset(size_t stackSize)
{
    delete [] m_stack;

    m_stack_size = (stackSize < MIN_STACK_SIZE) ? MIN_STACK_SIZE : stackSize;
    m_stack = new TagState[m_stack_size];
    m_stack_top = 0;
    m_content = NULL;
    m_len_left = 0;
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

size_t HtmlParaParser::check_character_reference(const char* token, size_t len,
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

size_t HtmlParaParser::parse(void* context, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char)
{
    m_content = content;
    m_len_left = len;

    if (m_len_left > 2 && m_content[0] == '&') {
        char mchar[10];
        int mchar_len;
        check_character_reference(content + 1, len - 1, mchar, &mchar_len);
    }

    return 0;
}

} // namespace hfcl

