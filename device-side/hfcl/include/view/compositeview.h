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


#ifndef NGUX_CompositeView_h
#define NGUX_CompositeView_h

#include "panelview.h"

NAMESPACE_BEGIN

enum {
	DRSET_SELF = 0,
};

class CompositeView : public PanelView
{
public:
	enum {
		DRSET_MAX = 1,
	};

	CompositeView(View *p_parent, DrawableSet* drset, DrawableSetGroup* g = NULL) : PanelView(p_parent, drset), m_drsets(g) { }
	CompositeView(View *p_parent, DrawableSetGroup* g = NULL) : PanelView(p_parent), m_drsets(g) { }
	CompositeView(int i_id, int x, int y, int w, int h, DrawableSetGroup* g = NULL) : PanelView(i_id, x, y, w, h) , m_drsets(g) { }
	virtual ~CompositeView();


	// bool setDrawableSet(int i_id, DrawableSet* drset);

	DrawableSet* getDrawableSet(int i_id, unsigned int view_class = 0);

	bool setDrawableSetGroup(DrawableSetGroup *g);


protected:
	DrawableSetGroup* m_drsets;
	virtual void updateDrawableSets(int i_id=-1);

private:	
	DECLARE_CLASS_NAME(PanelView)
};

#define GROUP_GETDRAWABLESET(drset_id, clss) getDrawableSet(drset_id, clss::CLASS_NAME)
#define SETDRAWABLESET(var, drset_id, cmp_id) do { \
	if(var && (drset_id == -1  || cmp_id == drset_id)) { \
		DrawableSet* __drset = getDrawableSet(cmp_id); \
		if(__drset) var->setDrawableSet(__drset); } }while(0)



NAMESPACE_END

#endif

