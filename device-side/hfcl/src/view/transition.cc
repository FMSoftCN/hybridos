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


#include <ctype.h>

#include "graphicscontext.h"
#include "transition.h"
#include "timerservice.h"

NAMESPACE_BEGIN

void TransitionManager::addTransition(NGUInt key, Transition* t)
{
    if (!t)
        return;
#if 0
    Transition* &oldt = m_trMaps[key];
    if (oldt)
        oldt->release();

    oldt = t;
#else
	removeTransition(key);
	m_trMaps[key] = t;
#endif

    if (m_timerId == 0)
        m_timerId = registerTimer(m_interval, "TransitionManager");
}

void TransitionManager::removeTransition(NGUInt key)
{
    TransitionMap::iterator it = m_trMaps.find(key);
    if (it == m_trMaps.end())
        return;
	
    it->second->release();
    m_trMaps.erase(it);
}

NGBool TransitionManager::handleEvent(Event* event)
{
    // wait next time to delete
    if (m_trMaps.size() <= 0 && m_timerId != 0) {
        removeTimer(m_timerId);
        m_timerId = 0;
        return true;
    }

    TransitionMap::iterator it = m_trMaps.begin();
    while (it != m_trMaps.end()) {
        it->second->update();
        if (it->second->isEnd()) {
            TransitionMap::iterator tit = it;
            ++it;
            tit->second->release();
            m_trMaps.erase(tit);
        } else {
            ++it;
        }
    }

    return true;
}


Transition* TransitionManager::getTranition(NGUInt key)
{
    TransitionMap::iterator it = m_trMaps.find(key);

    if (it == m_trMaps.end())
        return NULL;

    return it->second;
}

TransitionManager::~TransitionManager()
{
    for (TransitionMap::iterator it = m_trMaps.begin(); it != m_trMaps.end(); ++it) {
        it->second->release();
    }
	if(m_timerId>0) removeTimer(m_timerId);

}


static TransitionManager* _commonTransitionManager;

TransitionManager* GetCommonTransitionManager()
{
    if (!_commonTransitionManager) {
        _commonTransitionManager = NGUX_NEW(TransitionManager);
    }
    return _commonTransitionManager;
}

Transition* GetTransition(NGUInt key)
{
    TransitionManager* tm = GetCommonTransitionManager();
    return tm ? tm->getTranition(key) : NULL;
}


//////////////////////////////////////////////////////////////////////////////

static NGInt utf8_to_ucs2 (U16 *ucs2, U8 *utf8)
{
	unsigned char c = utf8[0];

	if (c < 0x80) 
	{
		*ucs2 = c;
		return 1;
	} 
	else if (c < 0xe0)
	{
		*ucs2 = ((U16) (c & 0x1f) << 6) | (U16) (utf8[1] ^ 0x80);
		return 2;
	} 
	else
	{
	    *ucs2 =	  ((U16) (c & 0x0f) << 12)
				| ((U16) (utf8[1] ^ 0x80) << 6)
				|  (U16) (utf8[2] ^ 0x80);
		return 3;
	}
}

static inline NGBool is_bidi_right(unsigned char *_str)
{
	NGInt _clen;
	Uint32 _dir;
	U16 _unicode = 0;
	
	if (_str == NULL){
		return false;
	}

	while (*_str != '\0' && (_clen = utf8_to_ucs2(&_unicode, _str)) == 1) {
		if (isalpha(*_str)) {
			return false;
		}
		_str++;
	}

	if (*_str == '\0') {
		return false;
	}

    if (GetGlyphBIDIType (GetSystemFont (SYSLOGFONT_WCHAR_DEF), (Glyph32)_unicode, &_dir))
		return (_dir == BIDI_TYPE_RTL || _dir == BIDI_TYPE_AL || _dir == BIDI_TYPE_AN);
        
	return false;
}

NGBool RollTextTransition::DrawRollText(View *view, GraphicsContext* context,
        DrawableSet* drset,  NGInt draw_id, NGInt draw_state,
        const IntRect& rc, NGDword data, DR_DATA_TYPE type)
{
    NGInt length, height;
    IntRect rcroll = rc;
	unsigned char *_str = NULL;

    if(type == DRDT_TEXT)
    {
        _str = (unsigned char*)data;
    }
    else if(type == DRDT_FORMATTEXT)
    {
        FormatText * ft = (FormatText*)data;
        if(ft) {
            _str =  (unsigned char*)(ft->text);
        }
    }

	if (!context  || !drset || data == 0)
        return false;

    RollTextTransition* t = (RollTextTransition*)GetTransition((NGUInt)view);
    if (t == NULL)
        return false;


    if (!drset->calcDrawableSize(draw_id, draw_state, length, height, data, type))
        return false;

    if (length <= rc.width()) {
        return false;
    }
	
	if ( t->m_offset > length - rc.width()+5) {
//		t->m_offset = rc.width() * (-1);
        t->m_owner = NULL;
        return false;
	}

	if (is_bidi_right(_str)) {
	    rcroll.m_right += t->m_offset;
	    rcroll.m_left = rcroll.m_right - length;
	} else {
	    rcroll.m_left -= t->m_offset;
	    rcroll.m_right = rcroll.m_left + length;
	}
    context->save();

    context->clip(rc);
    drset->draw(context, draw_id, draw_state, rcroll, data, type);

    context->restore();

    return true;
}

NGBool RollTextTransition::NeedRollText(DrawableSet* drset, NGInt draw_id, NGInt draw_state,
        const IntRect& rc, NGDword data, DR_DATA_TYPE type)
{
    if (!drset || data == 0)
        return false;

    NGInt length = rc.width();
	NGInt height = rc.height();

    if (!drset->calcDrawableSize(draw_id, draw_state, length, height, data, type))
        return false;

    if (length <= rc.width()) {
        return false;
    }

    return true;
}


/////////////////////////////////////////////

void AddRollText(View* view, Transition* t)
{
    if (!view || !t)
        return;

    TransitionManager* tm = GetCommonTransitionManager();
    tm->addTransition((NGUInt)view, t);
}

void RemoveRollText(View* view)
{
    if (!view)
        return;
	
    TransitionManager* tm = GetCommonTransitionManager();
    tm->removeTransition((NGUInt)view);
}

void ResetRollText(View *view)
{
    if (!view)
        return;

    TransitionManager* tm = GetCommonTransitionManager();
    RollTextTransition* t = (RollTextTransition*)tm->getTranition((NGUInt)view);
    if (t)
        t->reset();
}

NAMESPACE_END

