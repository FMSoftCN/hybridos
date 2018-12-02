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
		bool init(int count);
		// get count
		int count(void);
		// get mark
		bool getMark(int index);
		// set mark
		void setMark(int index, bool mark);

		void move(int from, int to, int count);

	private:
		int m_bitcount;
		Uint8* m_bitbuf;
};
class PointerKeeper
{
	public:
		PointerKeeper();
		~PointerKeeper();

		// init
		bool init(int count);
		// get count
		int count(void);
		// get pointer
		void* getPointer(int index);
		// set pointer
		void setPointer(int index, void* pointer);
		
		void move(int from, int to, int count);

	private:
		int m_pointercount;
		unsigned int* m_pointerbuf;
};

NAMESPACE_END

#endif
