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

#ifndef HFCL_COMMON_STLALTERNATIVE_H_
#define HFCL_COMMON_STLALTERNATIVE_H_

#ifndef __cplusplus
#   error "alternativestl.h: C++ only."
#endif

#include "../common/common.h"

// configure option: use STL or not
#ifdef _HFCL_USE_STL

#include <string>
#include <list>
#include <map>
#include <vector>

#define string std::string

#define LIST(type, name) typedef std::list<type> name;

#define MAP(type1, type2, name) typedef std::map<type1, type2> name;
#define MAPCLASS(type1, type2, name) MAP(type1, type2, name)
#define MAPCLASSVALUE(type1, type2, name) MAP(type1, type2, name)
#define MAPCLASSKEY(type1, type2, name) MAP(type1, type2, name)
#define MAPEX(TKey, TValue, ClassName,  cmp_key, del_key, del_value) \
        MAP(TKey, TValue, ClassName)

#define PAIR(type1, type2, name) typedef std::pair<type1, type2> name;

#define VECTOR(type, name) typedef std::vector<type> name;
#define VECTOREX(type, name, delete_code) VECTOR(type, name)
#define VECTORCLASS(type, name) VECTOR(type, name)

#else /* _HFCL_USE_STL */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>

#undef _HFCL_TRACE_LIST

namespace hfcl {

/*
** VW: no need to check UTF8 validation for string.
**/
inline int check_utf8_str (char *_buff, int _len)
{
    return _len;
}

class StrBuffer {
    char  buf[1];
    StrBuffer() { buf[0] = 0; }
public:
    static StrBuffer* NewStrBuffer(int len) {
        void *p = HFCL_MALLOC(len + sizeof(short));
        StrBuffer *buff = (StrBuffer*)p;
        buff->buf[0] = 0;
        return buff;
    }

    static StrBuffer* NewStrBuffer(const char* str, int n = -1) {
        if (!str)
            return NULL;
        if (n <= 0)
            n = strlen(str);

        StrBuffer* strbuf = NewStrBuffer(n + 1);
        if (strbuf)
        {
            strncpy(strbuf->str(), str, n);
            strbuf->str()[n] = 0;
        }

        return strbuf;
    }

    char* str() { return buf; }
    const char* str() const { return buf; }

    void release() {
        if(this != NULL) HFCL_FREE((void*)this);
    }
};

class string {
public:
    string() { _str_buff = NULL; }
    string(const char* str, int n = -1);
    string(const string& str);

    ~string() {
        if (_str_buff != NULL)
            HFCL_FREE(_str_buff);
        _str_buff = NULL;
    }

    const char* c_str() const {
        return _str_buff ;
    }

    const string & operator=(const string &str);
    const string & operator=(const char* str);
    friend int operator-(const string &s1, const string &s2) {
        const char* str1 = s1.c_str();
        const char* str2 = s2.c_str();
        return str1 == str2 ? 0 : strcmp(str1, str2);
    }

    int compare(const string & str) const {
        if (c_str() && str.c_str())
            return strcmp(c_str(), str.c_str());
        return c_str() ? 1 : (str.c_str() ? -1 : 0 );
    }
    int compare(const char* str) const {
        if (str && c_str())
            return strcmp(c_str(), str);
        return c_str() ? 1 : (str ? -1 : 0);
    }

    int compare(unsigned int pos1, unsigned int n1, const char* s) const {
        if (s && c_str()) {
            const char* str  = c_str();
            unsigned int len = strlen(str);
            if (len <= pos1)
                return -1;
            return strncmp(str + pos1, s, n1);
        }
        if (!s)
            return c_str() ? 1 : 0;
        else
            return -1;
    }

    string& append(const char* str);

    string& operator+=(const char* str) {
        return append(str);
    }

    string& operator+=(const string& s) {
        return append(s.c_str());
    }

    unsigned int length() const {
        if (c_str())
            return strlen(c_str());
        else
            return 0;
    }

    unsigned int size() const { return length(); }

    friend bool operator==(const string &s1, const char* s2) {
        return s1.compare(s2) == 0;
    }
    friend bool operator==(const string &s1, const string& s2) {
        return s1.compare(s2) == 0;
    }

    friend string operator+(const string& s1, const char* s) {
        string news = s1;
        news += s;
        return news;
    }
    friend string operator+(const string& s1, const string& s2) {
        string news = s1;
        news += s2;
        return news;
    }

    static unsigned int npos;
    unsigned int find(int c, unsigned int pos = 0) const {
        const char *s = c_str();
        if (!s || pos >= length())
            return npos;
        const char* fd = strchr(s + pos, c);
        return fd ? fd - s : npos;
    }

    string substr(unsigned int pos = 0, unsigned int n = npos) const {
        const char* s = c_str();
        unsigned int len = length();
        if (!s || pos >= len)
            return string();

        if (n == npos || pos + n > len)
            n = len - pos;
        return string(s + pos, n);
    }

private:
    char * _str_buff;
};

class vector_base
{
    int _size;
    int _count;
    unsigned char* _buffer;
protected:
    vector_base(int isize) {
        _size = isize;
        _count = 0;
        _buffer = NULL;
    }
    vector_base(int isize, int count) {
        _size = isize;
        _count = count;
        _buffer = (unsigned char*)HFCL_CALLOC(isize, count);
    }

    void *_get(int idx){
        return _buffer + idx * _size;
    }

    const void* _get(int idx) const{
        return _buffer + idx * _size;
    }

    virtual void delete_sub_type(void *subtype)  {}
    virtual void value_copy(void *dest, const void* src) { memcpy(dest, src, _size); }

    void* _push_back() {
        resize(_count + 1);
        return _get(_count - 1);
    }

    void _pop_back() {
        resize(_count - 1);
    }

    void* _back() {
        return _get(_count - 1);
    }

    bool _empty() const {
        if (_count <= 0)
            return true;
        return false;
    }

    void _init_by(const vector_base& v OPT_TRACE_INFO) {
        OPTIMIZE_WARNING;
        _size = (int) v._size;
        _count = (int) v._count;
        _buffer = (unsigned char*) HFCL_CALLOC(_size, _count);
        unsigned char* buf = (unsigned char*)v._buffer;
        for (int i = 0; i < _count*_size; i += _size) {
            value_copy((void*)(_buffer+i), (const void*)(buf+i));
        }
    }

public:
    class iterator_base {
        friend class vector_base;
    protected:
        vector_base *_owner;
        int _cur;
    public:
        iterator_base() : _owner(NULL), _cur(0) { }
        iterator_base(vector_base* owner, int cur) {
            _owner = owner;
            _cur = cur;
        }

        iterator_base(const iterator_base&  it) {
            _init_by(it);
        }

        void _init_by(const iterator_base& it) {
            _owner = (vector_base*) (it._owner);
            _cur = (int) (it._cur);
        }

        void _next() {
            if (_cur < _owner->size())
                _cur ++;
        }
        void _prev() {
            if (_cur > 0)
                _cur --;
        }

        void _goto(int idx) {
            _cur += idx;
            if (_cur >= _owner->size())
                _cur = _owner->size();
            else if (_cur < 0)
                _cur = 0;
        }

        void *_value() {
            return _owner->_get(_cur);
        }

        friend bool iseq(const iterator_base* pit1, const iterator_base* pit2) {
            return pit1->_cur == pit2->_cur && pit1->_owner == pit2->_owner;
        }
    };

    int size() const {
        return _count;
    }

    void resize(int sz);

    void clear() {
        for (int i=0; i < _count; i ++)
            delete_sub_type(_buffer+ i * _size);
        if (_buffer) {
            HFCL_FREE(_buffer);
            _buffer = NULL;
        }
        _count = 0;
    }

    iterator_base _begin() { return iterator_base(this, 0); }
    iterator_base _end() { return iterator_base(this, size()); }

    void* _insert(const iterator_base& it, void* data);
    void _erase(const iterator_base &it);

    vector_base(const vector_base& v OPT_TRACE_INFO) {
        _init_by(v OPT_TRACE_PARAM);
    }
    virtual ~vector_base() {
        //clear();
    }
};

// use rbtree for map
#include "rbtree.h"

class map_base {
protected:
    struct entry_t {
        struct rb_node node;
        char buf[1];
        void* key() { return (void*)buf; }
        void* value(const map_base *owner) { return (void*)(buf + owner->_key_size); }
        void setValue(const map_base* owner, void *v) {
            if (v)
                ((map_base*)owner)->value_copy(value(owner), v);
            else
                memset(value(owner), 0, owner->_value_size);
        }
    };

    unsigned int _key_size; //the size of key object
    unsigned int _value_size; //the size of value object
    unsigned int _count;
    struct rb_root _root;

    virtual int delete_key(void* key) {
        return 0;
    }
    virtual void delete_value(void* value) { int i = 0; i = i; ++i; i = 1; }
    virtual int key_cmp(void *key1, void *key2) const {
        return key1 == key2 ? 0 : memcmp(key1, key2, _key_size);
    }
    virtual void value_copy(void* dest, const void *src) { memcpy(dest, src, _value_size); }
    virtual void key_copy(void* dest, const void* src) { memcpy(dest, src,_key_size); }

    entry_t * new_entry(void* key, void* value) {
        unsigned int e_size = sizeof(struct rb_node) + _key_size + _value_size;
        entry_t * entry = (entry_t*)HFCL_CALLOC(1, e_size);
        if (entry) {
            key_copy(entry->key(), key);
            entry->setValue(this, value);
        }
        return entry;
    }

    void clear_node(struct rb_node* node) {
        if (node) {
            entry_t * entry = (entry_t*)node;
            delete_key(entry->key());
            delete_value(entry->value(this));
            clear_node(node->rb_left);
            clear_node(node->rb_right);
            HFCL_FREE(node); //HFCL_FREE the node
        }
    }

    ///////////////////////////////////////////////////////
    //iterator_base
    public:
    class iterator_base {
        friend class map_base;
    protected:
        entry_t *_cur_entry;
        map_base *_owner;

        iterator_base() : _cur_entry(NULL), _owner(NULL) { }

        iterator_base(map_base* owner, entry_t *entry) {
            _cur_entry = entry;
            _owner = owner;
        }

        void *_first() {
            return _cur_entry->key();
        }
        void *_second() {
            return _cur_entry->value(_owner);
        }

        void _next() {
            if (_cur_entry) {
                _cur_entry = (entry_t*)rb_next(&_cur_entry->node);
            }
        }

        void _init_by(const iterator_base & it) {
            _cur_entry =(entry_t*)(it._cur_entry);
            _owner = (map_base*)(it._owner);
        }

        void *_getbase() {
            if (_cur_entry)
                return _cur_entry->key();
            return NULL;
        }

        void *_getbase() const {
            return _getbase();
        }

        entry_t *_entry() const { return _cur_entry; }

    public:
        bool isempty() const {
            return _cur_entry == NULL;
        }

        friend bool iseq(const iterator_base* pit1, const iterator_base* pit2) {
            return pit1->_owner == pit2->_owner && pit1->_cur_entry == pit2->_cur_entry;
        }
    };

protected:
    ////////////////////////////////////////////////////////
    map_base(unsigned int key_size, unsigned int value_size)
        : _key_size(key_size), _value_size(value_size), _count(0) {
            _root.rb_node = NULL;
    }

    entry_t * _find(void *key) const;
    void _insert(void *key, void *value);
    void* _find_or_insert_value(void* key);

    void _erase(entry_t *entry) {
        rb_erase(&entry->node, &_root);
        clear_node(&entry->node);
        _count --;
    }

    void _erase(void *key) {
        entry_t* entry = _find(key);
        if (entry)
            _erase(entry);
    }

    void _erase(const iterator_base& it)  {
        _erase(it._entry());
    }

    iterator_base _begin() {
        return iterator_base((map_base*)this, (entry_t*)(rb_first(&_root)));
    }

    iterator_base _end() {
        return iterator_base((map_base*)this, (entry_t*)NULL);
    }

    iterator_base _begin() const {
        return iterator_base((map_base*)this, (entry_t*)(rb_first(&_root)));
    }

    iterator_base _end() const {
        return iterator_base((map_base*)this, (entry_t*)NULL);
    }

    iterator_base _find_it(void *key) const {
        return iterator_base((map_base*)this, _find(key));
    }

    iterator_base _find_it(void *key) {
        return iterator_base((map_base*)this, _find(key));
    }

    entry_t* _copy_tree(entry_t *entry, struct rb_node* parent);

    void _init_by(const map_base& m OPT_TRACE_INFO);

public:
    map_base(const map_base& m OPT_TRACE_INFO) {
        _init_by(m OPT_TRACE_PARAM);
    }
    void clear() {
        clear_node(_root.rb_node);
        _root.rb_node = NULL;
        _count = 0;
    }

    unsigned int size() const {
        return _count ;
    }
    virtual ~map_base() {
        //clear();
    }
};

#include "list.h"

#define LIST_DATA(l)  ((void*)((unsigned char*)(l) + sizeof(list_t)))
class list_base {
protected:
    list_t _head;

    unsigned int _size;
    int _count;

    list_base(unsigned int isize) : _size(isize), _count(0) {
        INIT_LIST_HEAD(&_head);
    }

    list_t* new_node() {
        return (list_t*)HFCL_CALLOC(1, sizeof(list_t) + _size);
    }

    virtual void delete_node(void* data) { }
    virtual bool value_equal(void* data1, void* data2) { return (data1 == data2); }
    virtual void value_copy(void* dest, const void* src) { memcpy(dest, src, _size); }

    void *_front() {
        if (list_empty(&_head))
            return NULL;
        return LIST_DATA(_head.next);
    }

    void *_back() {
        if (list_empty(&_head))
            return NULL;
        return LIST_DATA(_head.prev);
    }

    void *_front() const {
        if (list_empty(&_head))
            return NULL;
        return LIST_DATA(_head.next);
    }

    void *_back() const {
        if (list_empty(&_head))
            return NULL;
        return LIST_DATA(_head.prev);
    }

    void _push_front(void *data);
    void _push_back(void* data);
    void _pop_back();

    void _init_by(const list_base& l OPT_TRACE_INFO);

public:
    list_base(const list_base& l OPT_TRACE_INFO) {
        _init_by(l OPT_TRACE_PARAM);
    }
    virtual ~list_base() {
        clear();
    }
    class iterator_base {
    protected:
        friend class list_base;
        list_base *_owner;
        list_t *_current;

        iterator_base(list_base* owner, list_t *current) {
            _owner = owner;
            _current = current;
        }

        void _init_by(const iterator_base& it) {
            _owner = (list_base*)it._owner;
            _current = (list_t*)it._current;
        }

        void _next() {
            if (_current && _current != &_owner->_head)
                _current = _current->next;
        }

        void* _getdata() {
            return _current ? LIST_DATA(_current) : NULL;
        }
    public:
        iterator_base() : _owner(NULL), _current(NULL) { }
        iterator_base(const iterator_base& it) {
            _init_by(it);
        }

        friend bool iseq(const iterator_base * pit1, const iterator_base *pit2) {
            return (pit1->_current == pit2->_current && pit1->_owner == pit2->_owner);
        }
    };

protected:
    iterator_base _begin() {
        return iterator_base(this, _head.next);
    }

    iterator_base _end() {
        return iterator_base(this, &_head);
    }

    iterator_base _begin() const {
        return iterator_base((list_base*)this, (list_t*)_head.next);
    }

    iterator_base _end() const {
        return iterator_base((list_base*)this, (list_t*)&_head);
    }

    iterator_base _erase(const iterator_base & it);
    iterator_base _erase(const iterator_base& itb, const iterator_base& ite);

    void _remove(void *data);
    iterator_base _insert(const iterator_base& it, void *data);

public:
    unsigned int size() const {
        return _count;
    }

    void clear();
    bool empty() const {
        return list_empty(&_head);
    }
};

#define VECTOREX(Type, ClassName, delete_code) \
    class ClassName : public vector_base { \
        public: \
        ClassName() : vector_base(sizeof(Type)) { } \
        ClassName(int count) : vector_base(sizeof(Type), count){} \
        Type& operator[](int idx) { \
                return * (Type*)_get(idx); \
        } \
        ClassName(const ClassName& c): vector_base((const vector_base)c OPT_CLASSNAME(ClassName)){} \
        const Type& operator[](int idx) const { \
                return * (const Type *)_get(idx); \
        } \
        void push_back(Type& t) { \
            *((Type*)_push_back()) = (Type)t; \
        } \
        void pop_back() { \
            _pop_back(); \
        } \
        Type& back() { return *(Type*)_back(); } \
        bool empty() { \
            return _empty(); \
        } \
        void delete_sub_type(void *subtype) { Type* e = (Type*)subtype; e = e; delete_code; } \
        void value_copy(void *dest, const void* src) {*((Type*)dest) = *((Type*)src); } \
        ClassName& operator=(const ClassName& c) { _init_by((const vector_base)c OPT_CLASSNAME(ClassName)); return *this; } \
        ~ClassName() { clear(); } \
        class iterator : public vector_base::iterator_base { \
        public: \
            iterator() { } \
            iterator(const iterator& it) : vector_base::iterator_base((const vector_base::iterator_base)it) { } \
            iterator(const vector_base::iterator_base&it) : vector_base::iterator_base(it) { } \
            iterator& operator++() { _next(); return *this; } \
            iterator& operator++(int) { _next(); return *this; } \
            iterator& operator--() { _prev(); return *this; } \
            iterator& operator--(int) { _prev(); return *this; } \
            iterator& operator=(const iterator& it) { _init_by((const vector_base::iterator_base&)it); return *this; } \
            Type& operator*() { return *(Type*)_value(); } \
            iterator& operator+(int idx) { _goto(idx); return *this; } \
            bool operator!=(const iterator& it) {  \
                return !iseq((const vector_base::iterator_base*)this, (const vector_base::iterator_base*)&it); \
            } \
            bool operator==(const iterator &it) { \
                return iseq((const vector_base::iterator_base*)this, (const vector_base::iterator_base*)&it); \
            } \
        }; \
        iterator begin() { return vector_base::iterator_base(_begin()); } \
        iterator end() { return vector_base::iterator_base(_end()); } \
        void insert(const iterator& it, Type& t) { _insert((const iterator_base)it, (void*)&t); }\
        void erase(const iterator& it) { _erase((const iterator_base)it); } \
    };

#define VECTOR(Type, ClassName) VECTOREX(Type, ClassName, do{}while (0))
#define VECTORCLASS(Type, ClassName) VECTOREX(Type, ClassName, do{e->~Type();}while (0))

#define LISTEX(Type, ClassName, cmp_code, delete_code) \
    class ClassName : public list_base { \
    public: \
        ClassName() : list_base(sizeof(Type)) { } \
        ClassName(const ClassName& c): list_base((const list_base)c OPT_CLASSNAME(ClassName)){} \
        ~ClassName() { clear(); } \
        void delete_node(void* data) { Type* n = (Type*)data; n = n; delete_code; } \
        void value_copy(void *dest, const void* src) {*((Type*)dest) = *((Type*)src); } \
        bool value_equal(void* data1, void* data2) { Type* v1 = (Type*)data1; Type* v2 = (Type*)data2; cmp_code; } \
        Type& front() { return *(Type*)_front(); } \
        Type& back() { return *(Type*)_back(); } \
        Type& front()const { return *(Type*)_front(); } \
        Type& back()const { return *(Type*)_back(); } \
        void push_front(Type& t) {     \
            _push_front((void*)&t); \
        } \
        void push_back(Type& t) {     \
            _push_back((void*)&t);  \
        } \
        void pop_back() {             \
            _pop_back();             \
        } \
        void remove(Type& t) {         \
            _remove((void*)&t);     \
        } \
        ClassName& operator=(const ClassName& c) { _init_by((const list_base)c OPT_CLASSNAME(ClassName)); return *this; } \
        class iterator : public list_base::iterator_base { \
        public: \
            iterator() { } \
            iterator(const iterator & it) : list_base::iterator_base((const list_base::iterator_base)it) { } \
            iterator(const list_base::iterator_base & it) : list_base::iterator_base(it) { } \
            iterator& operator++()  { _next(); return *this; } \
            iterator& operator++(int)  { _next(); return *this; } \
            iterator& operator=(const iterator& it) { _init_by((const list_base::iterator_base&)it); return *this; } \
            Type* operator->() { return (Type*)_getdata(); } \
            Type& operator*() { return *(Type*)_getdata(); } \
            bool operator == (const iterator& it) { \
            return iseq((const list_base::iterator_base*)this, (const list_base::iterator_base*)&it); \
        } \
        bool operator != (const iterator& it) { \
            return !iseq((const list_base::iterator_base*)this, (const list_base::iterator_base*)&it); \
        } \
        }; \
        class const_iterator : public list_base::iterator_base { \
        public: \
            const_iterator() { } \
            const_iterator(const const_iterator & it) : list_base::iterator_base((const list_base::iterator_base)it) { } \
            const_iterator(const list_base::iterator_base & it) : list_base::iterator_base(it) { } \
            const_iterator& operator++()  { _next(); return *this; } \
            const_iterator& operator++(int)  { _next(); return *this; } \
            const_iterator& operator=(const iterator& it) { _init_by((const list_base::iterator_base&)it); return *this; } \
            Type* operator->() { return (Type*)_getdata(); } \
            Type& operator*() { return *(Type*)_getdata(); } \
            bool operator == (const iterator& it) { \
                return iseq((const list_base::iterator_base*)this, (const list_base::iterator_base*)&it); \
            } \
            bool operator != (const iterator& it) { \
                return !iseq((const list_base::iterator_base*)this, (const list_base::iterator_base*)&it); \
            } \
        }; \
        iterator begin() { return iterator(_begin()); } \
        iterator end() { return iterator(_end()); } \
        iterator begin() const { return iterator(_begin()); } \
        iterator end() const { return iterator(_end()); } \
        iterator insert(const iterator& it, Type& t) { return iterator(_insert((const iterator_base)it, (void*)&t)); }\
        iterator erase(const iterator& it) { return iterator(_erase((const list_base::iterator_base)it)); } \
        iterator erase(const iterator& itb, const iterator& ite) { return iterator(_erase((const list_base::iterator_base)itb, (const list_base::iterator_base)ite)); } \
    };
#define LIST(Type, ClassName) LISTEX(Type, ClassName, do{return (*v1) == (*v2);}while (0),do{} while (0))
#define LISTCLASS(Type, ClassName) LISTEX(Type, ClassName, do{return (*v1) == (*v2); }while (0), do{n->~Type(); } while (0))

#define PAIR(TKey, TValue, ClassName) \
    class ClassName { \
    public: \
        TKey first; \
        TValue second; \
        ClassName(const TKey& f, const TValue& s) { \
            first = (TKey)f; \
            second = (TValue)s; \
        } \
    };

#define MAPEX(TKey, TValue, ClassName,  cmp_key, del_key, del_value)  \
class ClassName : public map_base { \
public: \
    PAIR(TKey, TValue, value_type); \
    ClassName(const ClassName& c): map_base((const map_base)c OPT_CLASSNAME(ClassName)){} \
    ~ClassName() { clear(); } \
    class iterator : public map_base::iterator_base {\
        friend class ClassName; \
        iterator(const map_base::iterator_base &it) : map_base::iterator_base(it) { } \
    public: \
        iterator() { } \
        iterator(const iterator& it) : map_base::iterator_base((const map_base::iterator_base)it)  { } \
        iterator& operator++() { _next(); return *this; } \
        iterator& operator++(int) { _next(); return *this; } \
        value_type* operator->() { return (value_type*)_getbase(); } \
        value_type* operator->() const { return (value_type*)_getbase(); } \
        value_type& operator*() { return *(value_type*)_getbase(); } \
        iterator& operator=(const iterator& it) { _init_by((const map_base::iterator_base)it); return *this; } \
        friend bool operator==(const iterator& it1, const iterator&it2) { \
            return iseq((const map_base::iterator_base*)&it1, (const map_base::iterator_base*)&it2); \
        } \
        friend bool operator!=(const iterator& it1, const iterator&it2) { return !(it1 == it2); } \
    }; \
\
    class const_iterator : public map_base::iterator_base {\
        friend class ClassName; \
        friend class iterator; \
        const_iterator(const map_base::iterator_base &it) : map_base::iterator_base(it) { } \
    public: \
        const_iterator() { } \
        const_iterator(const iterator& it) : map_base::iterator_base((const map_base::iterator_base)it)  { } \
        const_iterator(const const_iterator& it) : map_base::iterator_base((const map_base::iterator_base)it)  { } \
        const_iterator& operator++() { _next(); return *this; } \
        const_iterator& operator++(int) { _next(); return *this; } \
        value_type* operator->() { return (value_type*)_getbase(); } \
        value_type* operator->() const { return (value_type*)_getbase(); } \
        value_type& operator*() { return *(value_type*)_getbase(); } \
        const_iterator& operator=(const iterator& it) { _init_by((const map_base::iterator_base)it); return *this; } \
        const_iterator& operator=(const const_iterator& it) { _init_by((const map_base::iterator_base)it); return *this; } \
        friend bool operator==(const const_iterator& it1, const const_iterator&it2) { \
            return iseq((const map_base::iterator_base*)&it1, (const map_base::iterator_base*)&it2); \
        } \
        friend bool operator!=(const const_iterator& it1, const const_iterator&it2) { return !(it1 == it2); } \
    }; \
\
    ClassName() : map_base(sizeof(TKey), sizeof(TValue)) { } \
    \
    TValue& operator[](const TKey & key) {  \
        return *(TValue*)_find_or_insert_value((void*)&key); \
    } \
\
    const TValue& operator[](const TKey &key) const{ \
        map_base::entry_t *entry = _find((void*)&key); \
        return *(const TValue*)(entry->value((map_base*)this));\
    } \
\
    iterator begin() { return iterator(_begin()); } \
    iterator end() { return iterator(_end()); } \
    const_iterator begin() const { return iterator(_begin()); } \
    const_iterator end() const { return iterator(_end()); } \
    iterator find(const TKey& key) { return iterator(_find_it((void*)&key)); } \
    const_iterator find(const TKey& key)const { return iterator(_find_it((void*)&key)); } \
    void erase(const iterator& it){ _erase((const map_base::iterator_base)it); } \
    void erase(const TKey& key) { _erase((void*)&key); } \
    ClassName& operator=(const ClassName& c) { _init_by((const map_base)c OPT_CLASSNAME(ClassName)); return *this; } \
protected: \
    virtual int delete_key(void* key) { \
        TKey *k = (TKey*)key; k = k; del_key; \
        return 0; \
    } \
    void delete_value(void* value) { TValue *v = (TValue*)value; v = v; del_value; } \
    int key_cmp(void* key1, void *key2) const { TKey *k1 = (TKey*)key1; TKey* k2 = (TKey*)key2; cmp_key; } \
    void value_copy(void *dest, const void* src) {*((TValue*)dest) = *((TValue*)src); } \
    void key_copy(void *dest, const void* src) {*((TKey*)dest) = *((TKey*)src); } \
};

#define MAPCLASS(TKey, TValue, ClassName)  \
    MAPEX(TKey, TValue, ClassName, do { return (*k1)-(*k2); } while (0), do { k->~TKey(); } while (0), do { v->~TValue(); } while (0))
#define MAPCLASSVALUE(TKey, TValue, ClassName) \
    MAPEX(TKey, TValue, ClassName, do { return (*k1)-(*k2); } while (0), do { } while (0), do { v->~TValue(); } while (0))
#define MAPCLASSKEY(TKey, TValue, ClassName) \
    MAPEX(TKey, TValue, ClassName, do { return (*k1)-(*k2); } while (0), do { k->~TKey(); } while (0), do { } while (0))

#define MAP(TKey, TValue, ClassName) \
    MAPEX(TKey, TValue, ClassName, do { return (*k1)-(*k2); } while (0), do { } while (0), do { } while (0))

} // namespace hfcl

#endif /* !_HFCL_USE_STL */

#endif /* HFCL_COMMON_STLALTERNATIVE_H_ */

