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


#include "nguxevent.h"
#include "graphicscontext.h"
#include "drawable.h"

#include "bitmark.h"
#include "hal_misc.h"

NAMESPACE_BEGIN

BitMark::BitMark()
{
	m_bitcount = 0;
	m_bitbuf = NULL;
}

BitMark::~BitMark()
{
	m_bitcount = 0;
	if(m_bitbuf)
		free(m_bitbuf);
	m_bitbuf = NULL;
}

// init mark
NGBool BitMark::init(NGInt cnt)
{
	NGInt c = 0;
	if( m_bitbuf != NULL )
		free(m_bitbuf);
    
    m_bitcount = 0;
	c = cnt & 7 ? ((cnt>>3)+1) : cnt>>3;
	
	m_bitbuf = (NGUInt8 *)malloc(c);
	if (m_bitbuf != NULL) {
		memset(m_bitbuf, 0, c);
		m_bitcount = cnt;
		return true;
	}

	return false;
}

// get count
NGInt BitMark::count(void)
{
	return m_bitcount;
}


// get mark
NGBool BitMark::getMark(NGInt index)
{
	if(m_bitcount == 0 || m_bitbuf == NULL || index >= m_bitcount || index < 0)
		return false;

	return m_bitbuf[index>>3] & 1<<(index&7);
}
// set mark
void BitMark::setMark(NGInt index, NGBool mark)
{
	if(0 == m_bitcount || NULL == m_bitbuf || index >= m_bitcount || index < 0) {
		return;
	}
	
	if(mark)
		m_bitbuf[index>>3] |= (1<<(index&7));
	else
		m_bitbuf[index>>3] &= ~(1<<(index&7));
}

void BitMark::move(NGInt from, NGInt to, NGInt cnt)
{
	if(to == from || cnt == 0)
		return;

	if(cnt == -1)
		cnt = m_bitcount - to;
	
	if(to > from) {
		to = to + cnt - 1;
		from = from + cnt - 1;
		while(cnt) {
			setMark(to, getMark(from));
			--cnt;
			--to;
			--from;
		}
	}
	else {
		while(cnt) {
			setMark(to, getMark(from));
			--cnt;
			++to;
			++from;
		}
	}
}

PointerKeeper::PointerKeeper()
{
	m_pointercount = 0;
	m_pointerbuf = NULL;
}
PointerKeeper::~PointerKeeper()
{
	m_pointercount = 0;
	if(m_pointerbuf)
		free(m_pointerbuf);
	m_pointerbuf = NULL;
}

// init pointer
NGBool PointerKeeper::init(NGInt cnt)
{
	NGInt c = 0;
	if( cnt <= 0 )
		return false;
		
    if( m_pointerbuf )
        free(m_pointerbuf);
    m_pointercount = 0;
    
	c = sizeof(void *);
	m_pointerbuf = (NGUInt *)malloc(c*cnt);
	if (m_pointerbuf != NULL) {
		memset(m_pointerbuf, 0, c*cnt);
		m_pointercount = cnt;
		return true;
	}

	return false;
}

// get count
NGInt PointerKeeper::count(void)
{
	return m_pointercount;
}

// get pointer
void* PointerKeeper::getPointer(NGInt index)
{
	if(m_pointercount == 0 || m_pointerbuf == NULL || index >= m_pointercount || index < 0)
		return NULL;

	return (void *)m_pointerbuf[index];
}
// set pointer
void PointerKeeper::setPointer(NGInt index, void* pointer)
{
	if(0 == m_pointercount || NULL == m_pointerbuf || index >= m_pointercount || index < 0) {
		return;
	}

	m_pointerbuf[index] = (NGUInt)pointer;
}
// move
void PointerKeeper::move(NGInt from, NGInt to, NGInt cnt)
{
	if(to == from || cnt == 0)
		return;

	if(cnt == -1)
		cnt = m_pointercount - to;
	
	if(to > from) {
		to = to + cnt - 1;
		from = from + cnt - 1;
		while(cnt) {
			setPointer(to, getPointer(from));
			--cnt;
			--to;
			--from;
		}
	}
	else {
		while(cnt) {
			setPointer(to, getPointer(from));
			--cnt;
			++to;
			++from;
		}
	}
}

NAMESPACE_END
