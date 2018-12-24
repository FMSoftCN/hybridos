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

#include "common/stlalternative.h"

namespace hfcl {

#ifndef _HFCL_USE_STL

string::string(const char* str, int n)
{
    _str_buff = NULL;
    if (str == NULL) return;

    if (n <= 0)
        n = strlen(str);
    _str_buff =(char *)HFCL_MALLOC(n + 1);
    if (_str_buff != NULL)
    {
        strncpy(_str_buff, str, n);
        _str_buff[n] = 0;
        check_utf8_str(_str_buff, n);
    }
}

string::string(const string& str)
{
     int n;
     _str_buff = NULL;
     if(str.c_str() == NULL) return;
       n = strlen(str.c_str());
     _str_buff = (char *)HFCL_MALLOC(n + 1);
     if (_str_buff != NULL)
     {
         strncpy(_str_buff, str.c_str(), n);
         _str_buff[n] = 0;
         check_utf8_str(_str_buff, n);
     }
}

const string & string::operator=(const string &str) {
    int n;
    if (this == &str)     return *this;

    if(str.c_str() == NULL) return *this;
    if (_str_buff != NULL)  HFCL_FREE(_str_buff);
    _str_buff = NULL;

    n = strlen(str.c_str());
    _str_buff = (char *)HFCL_MALLOC(n + sizeof(short));

    if (_str_buff != NULL)
    {
        strncpy(_str_buff, str.c_str(), n);
        _str_buff[n] = 0;
        check_utf8_str(_str_buff, n);
    }
    return *this;
}

const string & string::operator=(const char* str) {
    int n;
    if (_str_buff != NULL)  HFCL_FREE(_str_buff);
    _str_buff = NULL;
    if(str ==  NULL) return *this;

    n = strlen(str);
    _str_buff = (char *)HFCL_MALLOC(n + sizeof(short));

    if (_str_buff != NULL)
    {
        strncpy(_str_buff, str, n);
        _str_buff[n] = 0;
        check_utf8_str(_str_buff, n);
    }
    return *this;
}

string& string::append(const char* str) {
    int n;
    char * new_str;

    if (str == NULL)
        return *this;

    if (_str_buff == NULL) {
        this->operator=(str);
        return *this;
    }

    new_str = NULL;
    n = strlen(str);
    new_str = (char *)HFCL_MALLOC(length() + n+1);

    if (new_str != NULL)
    {
        strcpy(new_str, _str_buff);
        strcat(new_str, str);
    }

    if (_str_buff != NULL)  HFCL_FREE(_str_buff);
    _str_buff = new_str;
    check_utf8_str(_str_buff, -1);

    return *this;
}

void vector_base::resize(int sz)
{
    if (sz < 0)
        return;
    if (sz == _count)
        return;

    if (sz < _count) {
        unsigned char* buff = (unsigned char*) _get(sz);
        for (int i = sz; i < _count; i++, buff += _size)
            delete_sub_type(buff);
    }

    if (sz == 0) {
        HFCL_FREE(_buffer);
        _buffer = NULL;
        _count = sz;
        return;
    }

    _buffer = (unsigned char*)HFCL_REALLOC(_buffer, _size * sz);
    if (sz > _count) {
        memset(_get(_count), 0, (sz - _count)*_size);
    }
    _count = sz;
}

void* vector_base::_insert(const iterator_base& it, void* data)
{
    int cur = it._cur;
    resize(_count + 1);
    if (cur >= _count)
        cur = _count - 1;
    else {
        unsigned char* buf = _buffer + (_count - 1) * _size;
        for (int i = _count-1; i >cur; -- i, buf -= _size) {
            memcpy(buf, buf-_size, _size);
        }
    }
    return memcpy(_buffer + cur * _size, data, _size);
}

void vector_base::_erase(const iterator_base &it)
{
    int cur = it._cur;
    if (cur < 0 || cur >= _count)
        return ;
    unsigned char* buf = _buffer + cur * _size;
    delete_sub_type(buf);

    int copies = _count - 1;
    for (int i = cur; i < copies; i ++, buf += _size) {
        memcpy(buf, buf + _size, _size);
    }
    resize(_count - 1);
}

map_base::entry_t * map_base::_find(void *key) const
{
    if (!key)
        return (map_base::entry_t*)NULL;
    map_base::entry_t * entry = (map_base::entry_t*)(_root.rb_node);
    while (entry) {
        int ret = key_cmp(key, entry->key());
        if (ret < 0)
            entry = (map_base::entry_t*)(entry->node.rb_left);
        else if (ret > 0)
            entry = (map_base::entry_t*)(entry->node.rb_right);
        else
            return entry;
    }
    return (map_base::entry_t*)NULL;
}

void map_base::_insert(void *key, void *value)
{
    if (!key)
        return ;
    map_base::entry_t ** pentry = (entry_t**)&(_root.rb_node);
    map_base::entry_t * entry = NULL;
    map_base::entry_t * parent = NULL;
    while (*pentry) {
        int ret = key_cmp(key, (*pentry)->key());

        parent = *pentry;

        if (ret < 0)
            pentry = (map_base::entry_t**)&((*pentry)->node.rb_left);
        else if (ret > 0)
            pentry = (map_base::entry_t**)&((*pentry)->node.rb_right);
        else{
            entry = *pentry;
            break;
        }
    }

    if (!entry) { //new the entry
        entry = new_entry(key, value);
        rb_link_node(&entry->node, (struct rb_node*)parent,(struct rb_node**)pentry);
        rb_insert_color(&entry->node, &_root);
        _count ++;
        return ;
    }

    delete_key(entry->key());
    delete_value(entry->value(this));
    entry->setValue(this, value);
}

void* map_base::_find_or_insert_value(void* key)
{
    if (!key)
        return NULL;
    map_base::entry_t ** pentry = (map_base::entry_t**)&(_root.rb_node);
    map_base::entry_t * entry = NULL;
    map_base::entry_t * parent = NULL;
    while (*pentry) {
        int ret = key_cmp(key, (*pentry)->key());

        parent = *pentry;

        if (ret < 0)
            pentry = (map_base::entry_t**)&((*pentry)->node.rb_left);
        else if (ret > 0)
            pentry = (map_base::entry_t**)&((*pentry)->node.rb_right);
        else{
            return (*pentry)->value(this);
        }
    }

    entry = new_entry(key, NULL);
    rb_link_node(&entry->node, (struct rb_node*)parent,(struct rb_node**)pentry);
    rb_insert_color(&entry->node, &_root);
    _count ++;
    return entry->value(this);
}

map_base::entry_t* map_base::_copy_tree(map_base::entry_t *entry, struct rb_node* parent)
{
    if (!entry)
        return NULL;
    map_base::entry_t *newentry = new_entry(entry->key(), entry->value(this));
    newentry->node.rb_color = entry->node.rb_color;
    newentry->node.rb_parent = entry->node.rb_parent;
    rb_set_parent(&newentry->node, parent);
    newentry->node.rb_left = (struct rb_node*)_copy_tree((map_base::entry_t*)(entry->node.rb_left), &newentry->node);
    newentry->node.rb_right = (struct rb_node*)_copy_tree((map_base::entry_t*)(entry->node.rb_right), &newentry->node);
    return newentry;
}

void map_base::_init_by(const map_base& m OPT_TRACE_INFO)
{
    OPTIMIZE_WARNING;
    _value_size = (unsigned int)m._value_size;
    _key_size = (unsigned int)m._key_size;
    _count = (unsigned int)m._count;
    struct rb_root * proot = (struct rb_root*)&m._root;
    _root.rb_node = (struct rb_node*)_copy_tree((map_base::entry_t*)proot->rb_node, NULL);
}

void list_base::_push_front(void *data)
{
    list_t * node = new_node();
    if (!node)
        return ;

    value_copy(LIST_DATA(node), data);
    list_add(node, &_head);
    _count ++;
#ifdef _HFCL_TRACE_LIST
    _DBG_PRINTF ("_HFCL_TRACE_LIST : new and push node (%p) size = (%d) ---- in func [%s]  -- size = %d\n",
            node, sizeof(list_t) + _size, __func__, size());
#endif
}

void list_base::_push_back(void* data)
{
    list_t *node = new_node();
    if (!node)
        return ;
    value_copy(LIST_DATA(node), data);
    list_add_tail(node, &_head);
    _count ++;
#ifdef _HFCL_TRACE_LIST
    _DBG_PRINTF ("_HFCL_TRACE_LIST : new and push node (%p) size = (%d) ---- in func [%s]  -- size = %d\n",
        node, sizeof(list_t) + _size, __func__, size());
#endif
}

void list_base::_pop_back()
{
    if (_head.prev == &_head) //is empty
        return;
    list_t * node = _head.prev;
    delete_node(LIST_DATA(node));
    list_del(node);
    HFCL_FREE(node);
    _count --;
#ifdef _HFCL_TRACE_LIST
    _DBG_PRINTF ("_HFCL_TRACE_LIST : pop and delete node (%p)  ---- in func [%s]    -- size = %d\n",
            node,  __func__, size());
#endif
}

void list_base::_init_by(const list_base& l OPT_TRACE_INFO)
{
    OPTIMIZE_WARNING;

    _size = (unsigned int)l._size;
    _count = (unsigned int)l._count;

    INIT_LIST_HEAD(&_head);

    list_t *node;

    list_for_each(node, (list_t*)&l._head)
    {
        list_t *newnode = new_node();
        value_copy(LIST_DATA(newnode), node);
        __list_add(newnode, _head.prev, &_head);
#ifdef _HFCL_TRACE_LIST
        _DBG_PRINTF ("_HFCL_TRACE_LIST : new node (%p) size = (%d) ---- in func [%s]    -- size = %d\n",
                newnode, sizeof(list_t) + _size, __func__, size());
#endif
    }
}

void list_base::clear()
{
    list_t *node = _head.next;
    while (node != &_head) {
        list_t *tmp = node;
        node = node->next;
        delete_node(LIST_DATA(tmp));
        HFCL_FREE(tmp);
        _count--;
#ifdef _HFCL_TRACE_LIST
        _DBG_PRINTF ("_HFCL_TRACE_LIST : delete node (%p) size = (%d) ---- in func [%s]  -- size = %d\n",
                tmp, sizeof(list_t) + _size, __func__, size());
#endif
    }
    INIT_LIST_HEAD(&_head);
    _count = 0;
}

list_base::iterator_base list_base::_erase(const list_base::iterator_base & it)
{
    if (it._current != &_head) {
        list_t *node = (list_t*)it._current;
        list_t *next = node->next;
        delete_node(LIST_DATA(node));
        list_del(node);
        HFCL_FREE(node);
        _count --;
#ifdef _HFCL_TRACE_LIST
        _DBG_PRINTF ("_HFCL_TRACE_LIST : delete node (%p) size = (%d) ---- in func [%s]    -- size = %d\n",
                node, sizeof(list_t) + _size, __func__, size());
#endif
        return list_base::iterator_base(this, next);
    }
    return list_base::iterator_base(this, (list_t*)it._current);
}

list_base::iterator_base list_base::_erase(const list_base::iterator_base& itb,
        const list_base::iterator_base& ite)
{
    list_t * begin = (list_t*)itb._current;
    list_t * end = (list_t*)ite._current;
    while (begin && begin != end && begin != &_head) {
        list_t *tmp = begin;
        begin = begin->next;
        delete_node(LIST_DATA(tmp));
        list_del(tmp);
        HFCL_FREE(tmp);
        _count --;
#ifdef _HFCL_TRACE_LIST
        _DBG_PRINTF ("_HFCL_TRACE_LIST : delete node (%p) size = (%d) ---- in func [%s]    -- size = %d\n",
                tmp, sizeof(list_t) + _size, __func__, size());
#endif
    }
    return list_base::iterator_base(this, begin);
}

void list_base::_remove(void *data)
{
    list_t * node = _head.next;
    while (node != &_head) {
        if (value_equal(LIST_DATA(node), data)) {
            list_t *tmp = node;
            node = node->next;
            list_del(tmp);
            delete_node(LIST_DATA(tmp));
            HFCL_FREE(tmp);
            _count --;
#ifdef _HFCL_TRACE_LIST
            _DBG_PRINTF ("_HFCL_TRACE_LIST : delete node (%p) size = (%d) ---- in func [%s]  -- size = %d\n",
                    tmp, sizeof(list_t) + _size, __func__, size());
#endif
            return;

        } else {
            node = node->next;
        }
    }
}

list_base::iterator_base list_base::_insert(const list_base::iterator_base& it,
        void *data)
{
    list_t * node = new_node();

    value_copy(LIST_DATA(node), data);
    list_t * pos = (list_t*)it._current;
    list_add(node, pos);
    _count ++;
#ifdef _HFCL_TRACE_LIST
    _DBG_PRINTF ("_HFCL_TRACE_LIST : new and insert node (%p) size = (%d) ---- in func [%s]  -- size = %d\n",
            node, sizeof(list_t) + _size, __func__, size());
#endif

    return list_base::iterator_base(this, node);
}

#endif /* !_HFCL_USE_STL */

} // namespace hfcl

