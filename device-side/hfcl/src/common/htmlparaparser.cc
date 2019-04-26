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

size_t HtmlParaParser::parse(void* context, const char* content, size_t len,
            CB_ON_NEW_NODE on_new_node,
            CB_ON_NEW_ATTR on_new_attr,
            CB_ON_NEW_CHAR on_new_char)
{
    search_entity("COPY");
    return 0;
}

} // namespace hfcl

