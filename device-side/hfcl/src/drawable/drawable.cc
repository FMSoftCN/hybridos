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


#include "graphics/graphicscontext.h"
#include "drawable/drawable.h"
#include "resource/resloader.h"
#include "resource/respkgmanager.h"

namespace hfcl {

//#define HFCL_DR_TRACE 1

///////////////////////////////////
//style
/*
bool StylePtr::setElement(int id, HTData value) {
    Style* s = writeable();
    return s && s->setElement(id, value);
}
HTData StylePtr::getElement const(int id) {
    return ptr()->getElement(id, pfind);
}*/

///////////////////////////
//style element
static void release_element_value(int id, HTData value);
static HTData copy_element_value(int id, HTData value);


//////////////////////////////////////////////
//HashedStyle
HashedStyle::HashedStyle(const TRStyleElement* style_res)
{
    if (style_res)
    {
        for(int i=0; style_res[i].id != -1; i++)
        {
            m_elements[style_res[i].id] = ResValueToElement(style_res[i].id, style_res[i].value);
        }
    }
}

HashedStyle::~HashedStyle()
{
    for(StyleElementMap::iterator it = m_elements.begin(); it != m_elements.end(); ++it)
    {
        release_element_value(it->first, it->second);
    }
    m_elements.clear();
}

bool HashedStyle::setElement(int id, HTData value)
{
    StyleElementMap::iterator it = m_elements.find(id);

    if (it != m_elements.end()) {
        release_element_value(it->first, it->second);
        it->second = copy_element_value(it->first, value);
        return true;
    }

    m_elements[id] = copy_element_value(id, value);

    return true;
}

HTData HashedStyle::getElement(int id) const
{
    StyleElementMap::const_iterator it = m_elements.find(id);

    if (it != m_elements.end()) {
        return it->second;
    }

    return (m_super?m_super->getElement(id):0);
}

////////////////////////////////////
//StyleElementList
//// style Element
StyleElementList::StyleElement::StyleElement(int i_id, HTData dw_value)
{
    this->id = i_id;
    this->value = copy_element_value(i_id, dw_value);
}

void StyleElementList::StyleElement::setValue(HTData dw_value)
{
    release_element_value(id, this->value);
    this->value = copy_element_value(id, dw_value);
}

StyleElementList::StyleElement::~StyleElement()
{
    release_element_value(id, value);
}

StyleElementList::StyleElementList(const TRStyleElement* elements)
{
    if (elements)
    {
        for(int i=0; elements[i].id != -1; i++)
        {
            StyleElement *se = HFCL_NEW_EX(StyleElement, (elements[i].id, ResValueToElement(elements[i].id, elements[i].value)));
#ifdef HFCL_DR_TRACE
            _DBG_PRINTF ("HFCL_DR_TRACE --- StyleElementList::StyleElementList --- new StyleElement [%p]", se);
#endif
            m_elements.push_back(se);
        }
    }
}

StyleElementList::~StyleElementList()
{
    StyleElementListBase::iterator it_end = m_elements.end();
    for(StyleElementListBase::iterator it = m_elements.begin(); it != it_end; ++it) {
        if((*it) != NULL) {
            HFCL_DELETE(*it);
        }
    }
    clear();
}

bool StyleElementList::setElement(int id, HTData value)
{
    StyleElement* e = find(id);
    if (e)
    {
        e->setValue(value);
        return true;
    }

    // m_elements.push_back(StyleElement(id, copy_element_value(id, value)));
    StyleElement* se = HFCL_NEW_EX(StyleElement, (id, copy_element_value(id, value)));
#ifdef HFCL_DR_TRACE
    _DBG_PRINTF ("HFCL_DR_TRACE --- StyleElementList::StyleElementList --- set StyleElement [%p]", se);
#endif
    m_elements.push_back(se);
    return true;
}

StyleElementList::StyleElement* StyleElementList::find(int id) const
{
    StyleElementListBase::iterator it_end = m_elements.end();
    for(StyleElementListBase::iterator it = m_elements.begin(); it != it_end; ++it) {
        if ((*it)->id == id) {
            return (*it);
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
//Drawable

SimpleDrawable::~SimpleDrawable()
{
    // TODO release elements
}

bool SimpleDrawable::setElement(int id, HTData value)
{
    return m_elist.setElement(id, value);
}

HTData SimpleDrawable::getElement(int id) const
{
    StyleElementList::StyleElement* e = m_elist.find(id);
    return e ? e->value : (m_super?m_super->getElement(id):(unsigned int)NULL);
}

//#define HFCL_DR_TRACE 0


//////////////////////////////////////////////////////////////////////////////////
//DrawableSet

SimpleDrawableSet::~SimpleDrawableSet()
{
    int size = m_drawables.size(), i = 0;

    for(DrawableList::const_iterator it = m_drawables.begin();
            i < size; ++i, ++it)
    {
        Drawable* dr = (*it)->dr;
        if(dr && !(dr->isCommon())) {
#ifdef HFCL_DR_TRACE
            _DBG_PRINTF ("HFCL_DR_TRACE --- SimpleDrawableSet --- delete Drawable  [%p]", dr);
#endif
            HFCL_DELETE(dr); // delete Drawable
        }
#ifdef HFCL_DR_TRACE
        _DBG_PRINTF ("HFCL_DR_TRACE --- SimpleDrawableSet --- delete  [%p]", *it);
#endif
        HFCL_DELETE(*it); // delete DrawableNode
    }
    m_drawables.clear();
}

bool SimpleDrawableSet::setDrawable(int dr_id, Drawable* dr)
{
    DrawableNode* node = NULL;

    if(NULL != (node = find(dr_id))) {
        if(node->dr && !node->dr->isCommon()) {
            HFCL_DELETE(node->dr);
        }
        node->dr = dr;
    } else {
        node = HFCL_NEW_EX(DrawableNode, (dr_id, dr));
#ifdef HFCL_DR_TRACE
        _DBG_PRINTF ("HFCL_DR_TRACE --- SimpleDrawableSet --- setDrawable [%p]", dr);
#endif
        m_drawables.push_back(node);
    }

    return true;
}

Drawable* SimpleDrawableSet::getDrawable(int dr_id) const {
    DrawableNode* node = find(dr_id);
    return node ? node->dr : (m_super?m_super->getDrawable(dr_id):NULL);
}

bool SimpleDrawableSet::setDrawableElement(int dr_id, int se_id, HTData value) {
    DrawableNode* node = find(dr_id);
    Drawable* dr = NULL;
    if(node) {
        dr = node->dr;
        if(dr) {
            return dr->setElement(se_id, value);
        }
    }
    if(m_super) {
        dr = m_super->getDrawable(dr_id);
        //FIXME
        if(dr != NULL){
            DrawableNode *n = HFCL_NEW_EX(DrawableNode, (dr_id, dr));
#ifdef HFCL_DR_TRACE
            _DBG_PRINTF ("HFCL_DR_TRACE --- SimpleDrawableSet --- setDrawableElement [%p]", n);
#endif
            bool b = n->dr->setElement(se_id, value);
            m_drawables.push_back(n);
            return b;
        }
    }
    return false;
}


SimpleDrawableSet::DrawableNode* SimpleDrawableSet::find(int dr_id) const
{
    for(DrawableList::const_iterator it = m_drawables.begin();
            it != m_drawables.end(); ++it)
    {
        if ((*it)->id == dr_id)
            return (*it);
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////
//DrawableSetGroup
DrawableSetGroup::~DrawableSetGroup()
{
#if 0
    DrawableSet* drset = NULL;
    DrawableSetNode* node = NULL;

    // FIXME release all drset
    for(DrawableSetList::const_iterator it = m_drsets.begin();
            it != m_drsets.end(); ++it)
    {
        node = *it;
        if(node) {
    // FIXME mem leak when view do NOT release the sets.
#if 0
            drset = node->drset;
            // release drset
            if(drset && !(drset->isCommon())) {
                HFCL_DELETE(drset);
                drset = NULL;
            }
#endif
            // release node
            HFCL_DELETE(node);
            node = NULL;
        }
    }
#endif
}

#if 0

DrawableSetGroup::DrawableSetNode* DrawableSetGroup::find(int drset_id) const
{
    for(DrawableSetList::const_iterator it = m_drsets.begin();
            it != m_drsets.end(); ++it)
    {
        if ((*it)->id == drset_id)
            return (DrawableSetNode* )(*it);
    }
    return NULL;
}


bool DrawableSetGroup::setDrawableSet(int drset_id, DrawableSet* drset)
{
    DrawableSetNode* node = find(drset_id);

    if(node) {
        node->drset = drset;
        return true;
    }

    DrawableSetNode *dsn = HFCL_NEW_EX(DrawableSetNode, (drset_id, drset));
    m_drsets.push_back(dsn);

    return true;
}
#endif

void DrawableSetGroup::setDrawableSetMapTable(TRDrawableSetGroupItem * items)
{
    m_maptable = items;
}

DrawableSet* DrawableSetGroup::getDrawableSet(int drset_id)
{
    int drsetres_id = 0, i = 0;
    if(m_maptable != NULL) {
        while(m_maptable[i].drset_id != -1) {
            if(m_maptable[i].drset_id == drset_id) {
                drsetres_id = m_maptable[i].drsetres_id;
                break;
            }
            ++i;
        }
        if(drsetres_id > 0)
            return GetDrawableSetRes(drsetres_id);
    }

    return GetCommonDrawableSet();

}


///////////////////////////////////////////////////////////////////////////////////
//common styles
static HashedStyle *_common_styles = NULL;

Style* GetCommonStyle() {
    if (_common_styles == NULL) {
        _common_styles = HFCL_NEW_EX(HashedStyle, ());
    }
    return _common_styles;
}

#include "drawable/drawable-id-namemap.h"
#include "drawable/common-drawable-ids.h"
StyleElementIDNameTable* _id_name_table = &STYLE_ELEMENT_TABLE(SYS_SE); //TODO this is the common style defines

StyleElementIDNameTable* _dr_id_name_table = &STYLE_DRAWABLE_TABLE(DR);

static int _id_from_key(StyleElementIDNameTable* head, const char* key)
{
    StyleElementIDNameTable* t;
    for(t = head; t ; t = t->next)
    {
        if (!t->idNames)
        {
            continue;
        }
        const StyleElementIDName* idNames = t->idNames;

        for(int i = 0; idNames[i].id != -1; i++)
        {
            if (strcmp(idNames[i].name, key) == 0)
                return idNames[i].id;
        }
    }
    return 0;
}

static int element_id_from_key(const char* key)
{
    return _id_from_key(_id_name_table, key);
}

static int drawable_id_from_key(const char* key)
{
    return _id_from_key(_dr_id_name_table, key);
}

static Style* set_style_element_value(Style* s, GHANDLE hetc, const char* section);

static Drawable* _get_drawable_by_strvalue(const char* strvalue, GHANDLE hetc)
{
    if (!strvalue || !hetc)
        return NULL;

    while(*strvalue && ((*strvalue==' ' ) || (*strvalue == '\t')))
        strvalue ++;

    if (*strvalue == '[') //is sub section
    {
        char szsec[256];
        int i;
        strvalue ++;
        for(i=0; strvalue[i] != 0 && strvalue[i]!=']'; i++)
        {
            szsec[i] = strvalue[i];
        }
        szsec[i] = 0;
        return (Drawable*)set_style_element_value(NULL, hetc, szsec);
    }

    int dr_id = drawable_id_from_key(strvalue);
    if (!dr_id)
        return NULL;

    return GetCommonDrawable(dr_id);
}

static HTData get_metrics(const char* strvalue)
{
    struct metrics_info{
        const char* name;
        int         metrics;
    } metrics []= {
        {"left",         ALIGN_LEFT},
        {"center",       ALIGN_CENTER},
        {"right",        ALIGN_RIGHT},
        {"top",          VALIGN_TOP},
        {"middle",       VALIGN_MIDDLE},
        {"bottom",       VALIGN_BOTTOM},
        {"normal",       DRAWMODE_NORMAL},
        {"scaled",       DRAWMODE_SCALED},
        {"tiled",        DRAWMODE_TILED},
        {"single",       TEXTBREAK_SINGLE},
        {"wordbreak",    TEXTBREAK_WORD},
        {"charbreak",    TEXTBREAK_CHAR},
    };

    if (strvalue[0]>='0' && strvalue[0]<='9')
        return strtoul(strvalue, NULL, 0);

    for(int i=0; i < (int)(sizeof(metrics)/sizeof(metrics[0])); i++)
    {
        if (strcmp(metrics[i].name, strvalue) == 0)
            return (HTData)metrics[i].metrics;
    }

    LOGE("Metrics : [%s] is not a valid value\n", strvalue);

    return 0;
}

static HTData element_from_etc(int id, const char* strValue, GHANDLE hetc)
{
    if (strValue == NULL)
        return 0;

    switch(GET_ELEMENT_TYPE(id))
    {
    case DRET_METRICS:
        return get_metrics(strValue);
    case DRET_COLOR:
        return strtoul(strValue, NULL, 0);
    case DRET_IMAGE:
        return (HTData)ResLoader::getInstance()->getImage(strValue);

    case DRET_FONT:
        return (HTData)ResLoader::getInstance()->getFont(strValue);

    case DRET_DRAWABLE:
        return (HTData)_get_drawable_by_strvalue(strValue, hetc);
    default:
        return 0;
    }

    return strtol(strValue, NULL, 0);

}

static void set_drawable_element_frome_etc(Style* dr, int id, const char* strValue, GHANDLE hetc)  {
    HTData value = 0;
    if (*strValue == '$') //get the value from same name
    {
        int same_id = element_id_from_key(strValue+1);
        if (same_id == 0) {
            LOGE("Value \"%s\" is not exist!\n", strValue);
            return;
        }
        value = dr->getElement(same_id);
    }
    else {
        value = element_from_etc(id, strValue, hetc);
    }

    dr->setElement(id, value);
}

static const char* strim(char* buf, const char* strcont)
{
    if (!strcont)
        return NULL;

    while(*strcont && (*strcont == ' ' || *strcont =='\t'))
        strcont ++;

    int len = strlen(strcont)-1;
    while(len > 0 && (strcont[len] == ' ' || strcont[len] == '\t'))
        len --;

    strncpy(buf, strcont, len+1);
    buf[len+1] = 0;
    return buf;
}

static Style* set_style_element_value(Style* s, GHANDLE hetc, const char* section)
{
    if (section == NULL)
        section = STYLE_ELEMENT_SECTION;

    if (!hetc)
        return NULL;

    ETCSECTION *etcsec = (ETCSECTION*)FindSectionInEtc(hetc, section, FALSE);
    if (!etcsec)
    {
        LOGE("Section [%s] is not exits\n", section);
        return NULL;
    }


    char key[128];
    char value[256];
    if (!s)
    {
        for(int i=0; i < etcsec->key_nr ; i++)
        {
            strim(key, etcsec->keys[i]);
            if (strcmp("base", key) == 0)
            {
                value[0] = '\0';
                strim(value, etcsec->values[i]);
                if (value[0] == 0)
                    return NULL;
                s = (Style*)CreateDrawable(value, NULL);
                break;
            }
        }
        if (!s)
            return NULL;
    }

    for(int i=0; i < etcsec->key_nr; i++)
    {
        int id = element_id_from_key(strim(key, etcsec->keys[i]));
        if (id != 0)
        {
            set_drawable_element_frome_etc(s, id, strim(value, etcsec->values[i]), hetc);
        }
    }

    return s;
}

bool LoadCommonStyleElementsFromFile(const char* file_name, const char* section)
{
    GHANDLE hetc = LoadEtcFile(file_name);
    bool bret = LoadCommonStyleElementsFromHandle(hetc, section);
    UnloadEtcFile(hetc);
    return bret;
}


bool LoadCommonStyleElementsFromHandle(GHANDLE hetc, const char* section)
{
    if (_common_styles == NULL) {
        _common_styles = HFCL_NEW_EX(HashedStyle, ());
    }

    return set_style_element_value(_common_styles, hetc, section) != NULL;
}

static bool _add_id_name_table(StyleElementIDNameTable *table, StyleElementIDNameTable** phead)
{
    if (!table)
        return false;
    StyleElementIDNameTable* t = *phead;
    while(t && t != table)
        t = t->next;
    if (t)
        return true;

    table->next = *phead;
    *phead = table;
    return true;
}

bool AddCommonStyleElementIdName(StyleElementIDNameTable *table)
{
    return _add_id_name_table(table, &_id_name_table);
}

bool AddCommonDrawableIdName(StyleElementIDNameTable* table)
{
    return _add_id_name_table(table, &_dr_id_name_table);
}


bool AddCommonStyleElementsFromRes(const TRStyleElement *style_res)
{
    if (!style_res)
        return NULL;

    if (_common_styles == NULL) {
        _common_styles = HFCL_NEW_EX(HashedStyle, ());
        _common_styles->setCommon(true);
    }

    for(int i=0; style_res[i].id != -1; i++)
    {
        _common_styles->setElement(style_res[i].id, ResValueToElement(style_res[i].id, style_res[i].value));
    }
    return true;
}

//////////////////////////////////////////////////////
MAPCLASSKEY(utf8string, DrawableFactory*, DrawableFactoryMap)

MAP(int, Drawable*, CommonDrawableMap)

static DrawableFactoryMap* _drawableFactories;

bool RegisterDrawableFactory(const char* name, DrawableFactory* factory)
{
    if (!factory || !name)
        return false;

    if (!_drawableFactories)
        _drawableFactories = HFCL_NEW(DrawableFactoryMap);

    utf8string sname(name);

    DrawableFactoryMap::iterator it = _drawableFactories->find(sname);
    if (it == _drawableFactories->end())
    {
        (*_drawableFactories)[sname] = factory;
        return true;
    }
    it->second = factory;
    return true;
}

Drawable *CreateDrawable(const char* dr_name, const TRStyleElement *style_res)
{
    if (!dr_name)
        return NULL;

    DrawableFactoryMap::iterator it = _drawableFactories->find(dr_name);
    if (it == _drawableFactories->end())
    {
        LOGE("Drawable \"%s\" is not registered\n", dr_name);
        return NULL;
    }

    DrawableFactory* f = it->second;

    if (!f)
        return NULL;

    return f->create(style_res);
}


/////////////////////////////////////////////////////////////////
//common functions
HTData ResValueToElement(int id, HTData value)
{
    switch(GET_ELEMENT_TYPE(id))
    {
    case DRET_IMAGE:
        {
            return (HTData)GetImageRes(value);
        }
    case DRET_FONT:
        {
            return (HTData)GetFontRes(value);
        }
    case DRET_DRAWABLE:
        {
            return (HTData)GetDrawableRes(value);
        }
    case DRET_FONTLIST:  //TODO
    case DRET_COLORLIST: //TODO
    case DRET_IMGLIST:   //TODO
    case DRET_DRAWABLELIST: //TODO
        break;
    }

    return value;
}

static void release_element_value(int id, HTData value)
{
#if 0
    if (GET_ELEMENT_TYPE(id) & 0x80)
    {
        RefCount * ref = (RefCount*)(value);
        if (ref)
            ref->unref();
    }
#endif
    // image
    if (GET_ELEMENT_TYPE(id) == DRET_IMAGE) {
        if(((void *)value) != NULL) {
            HFCL_DELETE((Image *)value);
        }
    } else if (GET_ELEMENT_TYPE(id) == DRET_DRAWABLE) {
        if(((void *)value) != NULL) {
            HFCL_DELETE((Drawable *)value);
        }
    } else if (GET_ELEMENT_TYPE(id) == DRET_FONT) {
        /* do nothing                         */
        /* all the font res is global var     */
        /* don't release them                 */
    } else if (GET_ELEMENT_TYPE(id) & 0x80) {
        RefCount * ref = (RefCount*)(value);
        if (ref)
            ref->unref();
    }
}

static HTData copy_element_value(int id, HTData value)
{
    switch(GET_ELEMENT_TYPE(id))
    {
    case DRET_IMAGE:
    case DRET_FONT:
    case DRET_DRAWABLE:
#if 0
        RefCount* ref = (RefCount*)value;
        if (ref)
            ref->ref();
#endif
        return value;
    }
    return value;
}


//////////////////////////////////////////////////////////////
//drawableset
DrawableSet *_commonDrawableSet = NULL;

MAP(Uint32, DrawableSet*, ViewDrawableSets)

ViewDrawableSets* _view_drawblesets;

DrawableSet* GetCommonDrawableSet() {
    if (!_commonDrawableSet){
        _commonDrawableSet = HFCL_NEW(HashedDrawableSet);
        _commonDrawableSet->setCommon(true);
    }
    return _commonDrawableSet;
}

bool RegisterCommonDrawable(int dr_id, Drawable* dr)
{
    if (!_commonDrawableSet){
        _commonDrawableSet = HFCL_NEW(HashedDrawableSet);
        _commonDrawableSet->setCommon(true);
    }

    if(dr) {
        dr->setCommon(true);
    }
    _commonDrawableSet->setDrawable(dr_id, dr);
    return true;
}

Drawable* GetCommonDrawable(int dr_id)
{
    if (!_commonDrawableSet){
        return NULL;
    }
    return _commonDrawableSet->getDrawable(dr_id);
}


DrawableSet * GetViewDrawableSet(Uint32 view_name)
{
    if (!_view_drawblesets || !HFCL_IS_VALIDKEY(view_name))
        return GetCommonDrawableSet();

    ViewDrawableSets::iterator it = _view_drawblesets->find(view_name);
    if (it != _view_drawblesets->end()) {
        return it->second;
    }

    return GetCommonDrawableSet();
}

bool RegisterViewDrawableSet(Uint32 view_name, DrawableSet* drset)
{
    if (!drset || !HFCL_IS_VALIDKEY(view_name))
        return false;

    if (!_view_drawblesets)
        _view_drawblesets = HFCL_NEW(ViewDrawableSets);
    (*_view_drawblesets)[view_name] = drset;
    return true;
}

bool HashedDrawableSet::setDrawable(int dr_id, Drawable* dr) {
    m_drawables[dr_id] = dr;
    return true;
}
Drawable* HashedDrawableSet::getDrawable(int dr_id) const {
    DrawableSetMap::const_iterator it = m_drawables.find(dr_id);
    DrawableSetMap::const_iterator it_end = m_drawables.end();
    return it != it_end ? it->second : (m_super?m_super->getDrawable(dr_id):NULL);
}

bool HashedDrawableSet::setDrawableElement(int dr_id, int se_id, HTData value) {
    DrawableSetMap::iterator it = m_drawables.find(dr_id);
    Drawable* dr = NULL;
    if(it != m_drawables.end())
    {
        Drawable* ptr = it->second;
        return ptr->setElement(se_id, value);
    }

    if(m_super) {
        dr = m_super->getDrawable(dr_id);
    }
    if(!dr)
        return false;

    Drawable *ptr = m_drawables[dr_id];
    ptr = (Drawable *)dr->clone();
    return ptr->setElement(se_id, value);
}

//////////////////////////////
//DrawableSetGroup
//MAP(Uint32, DrawableSetGroupPtr, ViewDrawableSetGroups)
MAP(Uint32, DrawableSetGroup*, ViewDrawableSetGroups)
ViewDrawableSetGroups * _view_drset_groups;

bool RegisterViewDrawableSetGroup(Uint32 view_name, DrawableSetGroup *g)
{
    if (!g || !HFCL_IS_VALIDKEY(view_name))
        return false;

    if (!_view_drset_groups)
        _view_drset_groups = HFCL_NEW(ViewDrawableSetGroups);

    (*_view_drset_groups)[view_name] = g;
    return true;
}

DrawableSetGroup* GetViewDrawableSetGroup(Uint32 view_name)
{
    if (!HFCL_IS_VALIDKEY(view_name) || !_view_drset_groups)
        return NULL;

    ViewDrawableSetGroups::iterator it = _view_drset_groups->find(view_name);
    if (it != _view_drset_groups->end())
        return it->second;

    return NULL;
}

///////
static Drawable* _load_drawable_by_define(char* name)
{
    char* body = strchr(name, ':');

    if (body)
        *body = 0;

    Drawable * dr = CreateDrawable(name, NULL);
    if (!dr)
    {
        LOGE("\"%s\" is not a valid drawable name.\n", name);
        return NULL;
    }

    if (!body)
        return dr;


    while(body) {
        char* key = body + 1;
        char* value = strchr(key, '=');
        if (!value)
            break;
        *value = 0;
        value ++;

        body = strchr(value, ',');
        if (body)
            *body = 0;

        int id = element_id_from_key(key);
        if ( id == 0) {
            LOGI("\"%s\" is not a valid drawable element key, ignored.\n", key);
            continue;
        }

        set_drawable_element_frome_etc(dr, id, value, 0);

    }

    return dr;
}


static Drawable* _load_drawable_from_name(GHANDLE hetc, char* name)
{

    if (*name != '[') //is the drawable name
    {
        return _load_drawable_by_define(name);
    }

    while(*name && *name == '[') name ++;

    char* _tmp = name;
    while(*_tmp && *_tmp != ']') _tmp ++;
    *_tmp = '\0';

    ETCSECTION *psec = (ETCSECTION*)FindSectionInEtc(hetc, name, FALSE);

    if (!psec)
    {
        LOGE("cannot find the section [%s] in the etc file\n", name);
        return NULL;
    }

    Drawable* dr = NULL;

    char sz[128];
    //get the base value
    for(int i=0; i < psec->key_nr; i++)
    {
        if (strcmp(psec->keys[i], "base") == 0)
        {
            dr = CreateDrawable(strim(sz, psec->values[i]), NULL);
            if (!dr) {
                LOGE("The section [%s]'s \"base\" : \"%s\" is not a valid drawable name.\n", name, sz);
                return NULL;
            }
            break;
        }
    }

    if (!dr)
    {
        LOGE("The section [%s] cannot has the key \"base\"\n", name);
        return NULL;
    }

    for(int i=0; i < psec->key_nr; i++)
    {
        char buff[256];
        strim(buff, psec->keys[i]);

        if (strcmp(buff, "base") == 0)
            continue;

        int id = element_id_from_key(buff);
        if (id != 0)
            set_drawable_element_frome_etc(dr, id, strim(buff, psec->values[i]), hetc);
        else
            LOGI("\"%s\" is not a valid drawable element key in section [%s], ignored.\n", psec->keys[i], sz);
    }

    return dr;
}


bool LoadCommonDrawablesFromHandle(GHANDLE hetc, const char* section)
{
    if (!hetc)
        return false;

    if (!section)
        section = COMMON_DRAWABLESET_SECTION;

    ETCSECTION *etcsec = (ETCSECTION*)FindSectionInEtc(hetc, section, FALSE);

    for(int i = 0; i < etcsec->key_nr; i++)
    {
        char sz[512];
        int id = drawable_id_from_key(strim(sz, etcsec->keys[i]));
        if (id == 0)
            continue;

        strim(sz, etcsec->values[i]);
        Drawable* dr = _load_drawable_from_name(hetc, sz);
        if (dr)
        {
            RegisterCommonDrawable(id, dr);
        }
        else {
            LOGE("Load Drawable from \"%s=%s\" failed\n", etcsec->keys[i], etcsec->values[i]);
        }
    }


    return true;
}

bool LoadCommonDrawablesFromFile(const char* filename, const char* section)
{
    GHANDLE hetc = LoadEtcFile(filename);
    bool bret = LoadCommonDrawablesFromHandle(hetc, section);
    UnloadEtcFile(hetc);
    return bret;
}


static bool _set_drawables_from_section(DrawableSet* drset, GHANDLE hetc, const char* section) {

    ETCSECTION* etcsec = (ETCSECTION*)FindSectionInEtc(hetc, section, FALSE);
    if (!etcsec)
    {
        LOGE("[%s] is not found when load drawableset.\n", section);
        return false;
    }

    for(int i = 0; i < etcsec->key_nr; i++) {
        char sz[512];
        int id = drawable_id_from_key(strim(sz, etcsec->keys[i]));
        if (id == 0)
            continue;

        strim(sz, etcsec->values[i]);
        Drawable* dr = _load_drawable_from_name(hetc, sz);
        if (dr)
            drset->setDrawable(id, dr);
        else
            LOGE("Load Drawable from \"%s=%s\" failed\n", etcsec->keys[i], etcsec->values[i]);
    }
    return false;
}

static bool _set_drawables_from_name(DrawableSet* drset, GHANDLE hetc, char* name) {

    bool bset = false;

    while(name) {
        char * key = name;
        while(*key && (*key == ' ' || *key == '\t')) key ++;
        char * value = strchr(key, '=');
        if (!value)
            break;
        *value = '\0';
        value ++;

        name = value;
        while(*name && *name != ';' && *name !='}') name ++;
        if (*name == '\0' ||  *name == '}') {
            *name = '\0';
            name = NULL;
        }
        else {
            *name = '\0';
            name ++;
        }

        int id = drawable_id_from_key(key);
        if (id == 0)
        {
            LOGI("Unkonwn id [%s]\n", key);
            continue;
        }

        Drawable* dr = _load_drawable_from_name(hetc, value);
        if (!dr) {
            LOGE("load drawable from \"%s=%s\" failed\n", key, value);
            continue;
        }
        drset->setDrawable(id, dr);
        bset = true;
    }
    return bset;
}

static DrawableSet* _load_drawableset_from_name(char* name, GHANDLE hetc)
{
    if (!name)
        return NULL;

    if (*name != '{' && *name != '[')
    {
        LOGE("\"%s\" is not valid name.\n", name);
        return NULL;
    }


    DrawableSet* drset = HFCL_NEW_EX(SimpleDrawableSet, (GetCommonDrawableSet()));

    if (*name == '{') {
        name ++;
        if (!_set_drawables_from_name(drset, hetc, name))
            goto failed;
    } else if (*name == '['){
        name ++;
        char * _tmp = name;
        while(*_tmp && *_tmp != ']') _tmp ++;
        *_tmp = 0;
        if (!_set_drawables_from_section(drset, hetc, name))
            goto failed;
    }

    return drset;

failed:
    HFCL_DELETE(drset);
    return NULL;
}

bool LoadViewDrawableSetsFromHandle(GHANDLE hetc, const char* section)
{
    if (!hetc)
        return false;

    if (!section)
        section = VIEW_DRAWBABLESETS_SECTION;

    ETCSECTION* psec = (ETCSECTION*)FindSectionInEtc(hetc, section, FALSE);

    if (!psec)
    {
        LOGE("Cannot find the view drawablesets section :[%s]\n", section);
        return false;
    }

    for(int i = 0; i < psec->key_nr; i++)
    {
        char sz[512];
        strim(sz, psec->keys[i]);
        Uint32 key = HFCL_STR2KEY(sz);
        if (!HFCL_IS_VALIDKEY(key))
        {
            LOGI("[%s] is not a valid name!\n", sz);
            continue;
        }

        strim(sz, psec->values[i]);
        DrawableSet * drset = _load_drawableset_from_name(sz, hetc);
        if (!drset) {
            LOGE("%s=%s create drawableset failed!\n", psec->keys[i], psec->values[i]);
            continue;
        }
        RegisterViewDrawableSet(key, drset);
    }
    return true;
}

bool LoadViewDrawableSetsFromFile(const char* filename, const char* section)
{
    GHANDLE hetc = LoadEtcFile(filename);
    if (!hetc) {
        LOGE("Cannot open the etc file : %s\n", filename);
        return false;
    }

    bool bret = LoadViewDrawableSetsFromHandle(hetc, section);
    UnloadEtcFile(hetc);

    if (!bret) {
        LOGE("Load View DrawableSets From \"%s\" with section \"%s\" failed.\n", filename, section ? section : (VIEW_DRAWBABLESETS_SECTION "(default)"));
    }
    return bret;
}

} // namespace hfcl

//////////////////////////////////////////////////////////////
using namespace hfcl;

static GHANDLE find_default_etc_file()
{
// VW: REMOVEME
#if 0 // ndef __NOUNIX__
    GHANDLE hetc = (GHANDLE)0;
    const char* cfg_file = getenv(HFCL_CFG_ENV);
    if (cfg_file && (hetc = LoadEtcFile(cfg_file)))
        return hetc;

    char szbuf[MAX_PATH];
    if ((hetc = LoadEtcFile(HFCL_CFG)))
        return hetc;

    sprintf(szbuf, "%s/.%s", getenv("HOME"), HFCL_CFG);
    if ((hetc = LoadEtcFile(szbuf)))
        return hetc;

    sprintf(szbuf, "/usr/local/etc/%s", HFCL_CFG);
    if ((hetc = LoadEtcFile(szbuf)))
        return hetc;
#endif

    return (GHANDLE)0;
}

bool InitDrawableFromDefaultEtcFile(void)
{
    GHANDLE hetc = find_default_etc_file();
    if (!hetc)
        return false;

    bool bret = false;
    if (!LoadCommonStyleElementsFromHandle(hetc))
        goto failed;

    if (!LoadCommonDrawablesFromHandle(hetc))
        goto failed;

    if (!LoadViewDrawableSetsFromHandle(hetc))
        goto failed;

    bret = true;
failed:
    UnloadEtcFile(hetc);
    return bret;
}

