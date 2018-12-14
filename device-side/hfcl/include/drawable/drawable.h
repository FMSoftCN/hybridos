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

#ifndef _HFCL_Drawable_H
#define _HFCL_Drawable_H

#include "../mgcl/mgcl.h"
#include "../common/object.h"
#include "../common/stlalternative.h"
#include "../resource/restypes.h"
#include "drawable-id.h"

namespace hfcl {

#include "drawable-id-def.h"
#include "common-drawable-ids.h"

class View;
class GraphicsContext;

//////////////////////////////////////////////////////////////////////////////////////
//style

//Value ascces
HTData ResValueToElement(int id, HTData value);

class Style : public CopyOnWriteable
{
protected:
    Style*  m_super;
    bool m_is_common;

public:
    Style() : CopyOnWriteable(), m_super(NULL), m_is_common(false) {}
    Style(const TRStyleElement *elements) : CopyOnWriteable(), m_super(NULL), m_is_common(false) { }
    Style(Style* s) : CopyOnWriteable(), m_super(s),m_is_common(false) {}
    virtual ~Style() {
        if(m_super && !(m_super->isCommon()) ) {
            HFCL_DELETE(m_super);
        }
        clean();
    }

    bool isCommon(void) { return m_is_common; }
    void setCommon(bool common) { m_is_common = common; }
    virtual bool setElement(int id, HTData value) = 0;
    virtual HTData getElement(int id) const = 0;
    virtual void clean() {}

    void init(const TRStyleElement *elements)
    {
        if(!elements)
            return;
        for(int i = 0; elements[i].id != -1; i++)
        {
            setElement(elements[i].id, ResValueToElement(elements[i].id, elements[i].value));
        }
    }

    Style * getSuper(){return m_super;}
    void setSuper(Style* s) {
        m_super = s;
    }
};


class HashedStyle : public Style
{
protected:
    MAP(int, HTData, StyleElementMap)
    StyleElementMap m_elements;

public:
    HashedStyle (){ }
    HashedStyle(const TRStyleElement* elements) ;
    HashedStyle(Style* s) : Style(s) { }
    virtual ~HashedStyle();

    bool setElement(int id, HTData value);
    HTData getElement(int id) const;
};

class StyleElementList
{
public:

    struct StyleElement
    {
        int id;
        HTData value;

        StyleElement(int id, HTData value);
        void setValue(HTData value);
        ~StyleElement();
    };

protected:

    LIST(StyleElement*, StyleElementListBase)

    StyleElementListBase m_elements;

public:
    StyleElementList( ) { }
    StyleElementList(const TRStyleElement* style_res);
    virtual    ~StyleElementList();
    StyleElement* find(int id) const;
    bool setElement(int id, HTData value);
    void clear() { m_elements.clear(); }
    friend bool operator==(StyleElement& se1, StyleElement& se2) {
        if ((se1.id == se2.id) && (se1.value == se2.value))
            return true;
        return false;
    }
};

class SimpleStyle : public Style
{
protected:
    StyleElementList m_elist;
public:
    SimpleStyle() { }
    SimpleStyle(const TRStyleElement* style_res) : m_elist(style_res) { }
    SimpleStyle(Style * s) : Style(s) { }
    virtual ~SimpleStyle() { m_elist.clear(); }

    bool setElement(int id, HTData value) { return m_elist.setElement(id, value); }
    HTData getElement(int id) const {
        StyleElementList::StyleElement* e = m_elist.find(id);
        return e ? e->value : (m_super?m_super->getElement(id):(unsigned int)NULL);
    }
};



//////////////////////////////////////////////////////////////////////////////////////

class Drawable : public Style
{
public:
    Drawable() { }
    Drawable(const TRStyleElement* elements) : Style(elements) { }
    Drawable(Style* s) : Style(s) { }

    virtual void draw(GraphicsContext* gc, int draw_state,
            const IntRect &rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE) = 0; //do nothing
    virtual bool calcDrawableSize(int draw_state,
            int& w, int& h, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE) {
        w = h = 0;
        return false;
    }

    virtual void clean() {/*do nothing */ }
};

class SimpleDrawable : public Drawable
{
protected:
    StyleElementList m_elist;
public:
    SimpleDrawable() { }
    SimpleDrawable(const TRStyleElement* style_res) : m_elist(style_res) { }
    SimpleDrawable(Style * s) : Drawable(s) { }
    virtual ~SimpleDrawable();

    bool setElement(int id, HTData value);
    HTData getElement(int id) const;
};

/////////////////////////////////////////////////////////////////////////////
//drawableset used by drawable

class DrawableSet : public CopyOnWriteable{
protected:
    bool m_is_common;
//    DrawableSetPtr  m_super;
    DrawableSet*  m_super;
public:
    DrawableSet(){ m_is_common = false; }
    DrawableSet(DrawableSet *s) : m_super(s) { m_is_common = false; }
    virtual ~DrawableSet() {
        if (m_super != NULL && !m_super->isCommon()) {
            HFCL_DELETE(m_super);
            m_super = NULL;
        }
    }
    inline void draw(GraphicsContext *gc, int dr_id, int draw_state, const IntRect& rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE)
    {
        Drawable* dr = getDrawable(dr_id);
        if(dr)
            dr->draw(gc, draw_state, rc, data, type);
    }

    bool isCommon(void) { return m_is_common; }
    void setCommon(bool common) { m_is_common = common; }

    inline void setSuper(DrawableSet* s) {
        m_super = s;
    }

    virtual bool setDrawable(int id, Drawable* dr) = 0;
    virtual Drawable* getDrawable(int id) const = 0;

    virtual bool setDrawableElement(int dr_id, int se_id, HTData value) = 0;
    HTData getDrawableElement(int dr_id, int se_id) const {
        const Drawable* dr = getDrawable(dr_id);
        return dr ? dr->getElement(se_id) : 0;
    }

    inline bool calcDrawableSize(int dr_id, int draw_state, int& w, int &h, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE)
    {
        Drawable* dr = getDrawable(dr_id);
        if(dr)
            return dr->calcDrawableSize(draw_state, w, h, data, type);
        else
            w = h = 0;
        return false;
    }
};

class SimpleDrawableSet : public DrawableSet
{
    struct DrawableNode{
        int id;
        Drawable* dr;

        DrawableNode(int iid, Drawable* p_dr) :id(iid), dr(p_dr) { }
        DrawableNode() : id(0) { }
        DrawableNode(const DrawableNode& n) {
            id = n.id;
            dr = n.dr;
        }
    };

    LIST(DrawableNode *, DrawableList)

    DrawableList m_drawables;
public:
    SimpleDrawableSet() { }
    SimpleDrawableSet(DrawableSet* s) : DrawableSet(s) { }
    SimpleDrawableSet(SimpleDrawableSet* s) : DrawableSet((DrawableSet*)s) { }
    virtual ~SimpleDrawableSet();

    friend bool operator==(DrawableNode &dn1, DrawableNode &dn2) {
        if ((dn1.id == dn2.id) && (dn1.dr == dn2.dr))
            return true;
        return false;
    }
    virtual bool setDrawable(int dr_id, Drawable* dr);

    virtual Drawable* getDrawable(int dr_id) const;
    virtual bool setDrawableElement(int dr_id, int se_id, HTData value);

    CopyOnWriteable* clone(){ return (CopyOnWriteable*)(HFCL_NEW_EX(SimpleDrawableSet, (this))); }

private:
    DrawableNode* find(int dr_id) const ;
};

class HashedDrawableSet : public DrawableSet
{
    MAP(int, Drawable*, DrawableSetMap)
    DrawableSetMap m_drawables;
public:
    HashedDrawableSet() { }
    HashedDrawableSet(DrawableSet* s) : DrawableSet(s) { }
    HashedDrawableSet(HashedDrawableSet* s) : DrawableSet(s) { }

    virtual bool setDrawable(int dr_id, Drawable* dr);
    virtual Drawable* getDrawable(int dr_id) const;

    virtual bool setDrawableElement(int dr_id, int se_id, HTData value);

    CopyOnWriteable * clone() { return (CopyOnWriteable*) (HFCL_NEW_EX(SimpleDrawableSet, ((DrawableSet*)(this)))); }
};

/////////////////////////////////////////////////////////////////////////////
//DrawableSetGroup

class DrawableSetGroup : public CopyOnWriteable
{
protected:
    DrawableSetGroup* m_super;
    bool m_is_common;

public:

    DrawableSetGroup() { }
    DrawableSetGroup(DrawableSetGroup* g) : m_super(g) { }
    virtual ~DrawableSetGroup();

    void setCommon(bool common) { m_is_common = common; }
    bool isCommon(void) { return m_is_common; }

    //bool setDrawableSet(int drset_id, DrawableSet* drset);

    DrawableSet* getDrawableSet(int drset_id);
    void setDrawableSetMapTable(TRDrawableSetGroupItem* items);

private:
    TRDrawableSetGroupItem* m_maptable;

};


/////////////////////////////////////////////////////////////////////////////
//common functions
//for config file
//Load from config file
bool LoadCommonStyleElementsFromFile(const char* file_name, const char* section = NULL);
//load from config handle
bool LoadCommonStyleElementsFromHandle(GHANDLE hetc, const char* section = NULL);

//define the name and id map table
struct StyleElementIDName {
    int id;
    const char* name;
};
struct StyleElementIDNameTable {
    const StyleElementIDName* idNames;
    struct StyleElementIDNameTable* next;
};
//add the key info the load table, end with id==-1
bool AddCommonStyleElementIdName(StyleElementIDNameTable *table);
bool AddCommonDrawableIdName(StyleElementIDNameTable* table);

//load from resource package
bool AddCommonStyleElementsFromRes(const TRStyleElement *style_res);


Style* GetCommonStyle() ;
/////////////////////////////////////////////////////////////////////////
//Drawable Factory
class DrawableFactory {
public:
    virtual Drawable* create(const TRStyleElement* style_res) = 0;
};

#define DECLARE_DRFACTORY_EX(name, DrClass) \
    class DrClass##Factory : public DrawableFactory { \
    public: \
        static DrClass##Factory* getInstance(void) { \
            static DrClass##Factory* s_single = NULL; \
            if (!s_single) { \
                s_single = HFCL_NEW_EX(DrClass##Factory, ()); \
            } \
            return s_single; \
        } \
        Drawable* create(const TRStyleElement* style_res) \
        { \
            return HFCL_NEW_EX(DrClass, (style_res)); \
        } \
    };

#define DECLARE_DRFACTORY(DrClass) DECLARE_DRFACTORY_EX(#DrClass, DrClass)

#define REGISTER_DRFACTORY(DrClass) \
            RegisterDrawableFactory(#DrClass, DrClass##Factory::getInstance())



bool RegisterDrawableFactory(const char* drclss_name, DrawableFactory* factory);

Drawable *CreateDrawable(const char* drclss_name, const TRStyleElement *style_res);

DrawableSet * GetCommonDrawableSet(void);
DrawableSet * GetViewDrawableSet(Uint32 view_name);
static inline DrawableSet* GetViewDrawableSet(const char* view_name) {
    return view_name ? GetViewDrawableSet(HFCL_STR2KEY(view_name)) : NULL;
}
bool RegisterViewDrawableSet(Uint32 view_name, DrawableSet* drset);
static inline bool RegisterViewDrawableSet(const char* view_name, DrawableSet* drset) { return RegisterViewDrawableSet(HFCL_STR2KEY(view_name), drset); }

bool RegisterCommonDrawable(int dr_id, Drawable* dr);
Drawable* GetCommonDrawable(int dr_id);

bool LoadCommonDrawablesFromHandle(GHANDLE hetc, const char* section = NULL);

bool LoadCommonDrawablesFromFile(const char* filename, const char* section = NULL);

bool LoadViewDrawableSetsFromHandle(GHANDLE hetc, const char* section = NULL);
bool LoadViewDrawableSetsFromFile(const char* filename, const char* section = NULL);

DrawableSetGroup* GetViewDrawableSetGroup(Uint32 view_name);
static inline DrawableSetGroup* GetViewDrawableSetGroup(const char * view_name) {
    return view_name ? GetViewDrawableSetGroup(HFCL_STR2KEY(view_name)) : NULL;
}
bool RegisterViewDrawableSetGroup(Uint32 view_name, DrawableSetGroup *g);
static inline bool RegisterViewDrawableSetGroup(const char * view_name, DrawableSetGroup* g) {
    return view_name ? RegisterViewDrawableSetGroup(HFCL_STR2KEY(view_name), g) : false;
}

} // namespace hfcl

#endif

