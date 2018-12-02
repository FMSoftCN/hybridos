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

#include "compositeview.h"

NAMESPACE_BEGIN

CompositeView::~CompositeView()
{
	if(m_drsets &&
		!(m_drsets->isCommon()) ) {
		NGUX_DELETE(m_drsets);
		m_drsets = NULL;
	}
}

#if 0
NGBool CompositeView::setDrawableSet(NGInt i_id, DrawableSet* drset)
{
	if(m_drsets->setDrawableSet(i_id, drset))
	{
		updateDrawableSets(i_id);
		return true;
	}
	return false;
}
#endif

DrawableSet* CompositeView::getDrawableSet(NGInt i_id, NGUInt view_class)
{
	return m_drsets->getDrawableSet(i_id);
}

NGBool CompositeView::setDrawableSetGroup(DrawableSetGroup *g) 
{
	if(m_drsets != g && !(m_drsets->isCommon()) ) {
		NGUX_DELETE(m_drsets);
		m_drsets = NULL;
	}
	m_drsets = g;
	updateDrawableSets();
	return true;
}


void CompositeView::updateDrawableSets(NGInt i_id) 
{ 
	if(i_id == -1 || i_id == DRSET_SELF) {
		View::setDrawableSet(getDrawableSet(DRSET_SELF));
	}
}


DEFINE_CLASS_NAME(CompositeView)

NAMESPACE_END
