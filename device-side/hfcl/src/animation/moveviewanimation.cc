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

#include "moveviewanimation.h"
#include "view/view.h"

namespace hfcl {

/*
 * the param pt is target point in parent view.
 */
MoveViewAnimation::MoveViewAnimation(View *view, Point pt)
: ViewAnimation(view, MGEFF_POINT)
{
	setPoint(pt);
}

void MoveViewAnimation::onStart()
{
	Point oldPt;
	View *__view = getView();

	if(!__view)
		return;

	__view->getPosition(&oldPt.x, &oldPt.y);

	setStartValue((void *)&oldPt);
}

void MoveViewAnimation::setPoint(Point pt)
{
	setEndValue((void *)&pt);
}

void MoveViewAnimation::setProperty(int id, void *value)
{
	if(NULL == value)
		return;

	Point *pt = (Point*)value;

    IntRect rect = getView()->getRect();

	rect.offset(pt->x - rect.left(), pt->y - rect.top());

	getView()->setRect(rect);
}

} // namespace hfcl
