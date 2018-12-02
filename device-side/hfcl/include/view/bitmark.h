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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef  _BIT_MARK_H 
#define  _BIT_MARK_H

NAMESPACE_BEGIN

////////////////////// Features /////////////////////////
class BitMark
{
	public:
		BitMark();
		~BitMark();

		// init
		NGBool init(NGInt count);
		// get count
		NGInt count(void);
		// get mark
		NGBool getMark(NGInt index);
		// set mark
		void setMark(NGInt index, NGBool mark);

		void move(NGInt from, NGInt to, NGInt count);

	private:
		NGInt m_bitcount;
		NGUInt8* m_bitbuf;
};
class PointerKeeper
{
	public:
		PointerKeeper();
		~PointerKeeper();

		// init
		NGBool init(NGInt count);
		// get count
		NGInt count(void);
		// get pointer
		void* getPointer(NGInt index);
		// set pointer
		void setPointer(NGInt index, void* pointer);
		
		void move(NGInt from, NGInt to, NGInt count);

	private:
		NGInt m_pointercount;
		NGUInt* m_pointerbuf;
};

NAMESPACE_END

#endif
