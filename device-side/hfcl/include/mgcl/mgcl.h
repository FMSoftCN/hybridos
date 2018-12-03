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

#ifndef __HFCL_MGCL_H__
#define __HFCL_MGCL_H__

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgeff/mgeff.h>

#include "common/intrect.h"

#include "mgcl/mgclactivity.h"
//#include "mgclanimation.h"
#include "mgcl/mgclcommon.h"
//#include "mgclgraphics.h"
//#include "mgclservices.h"
#include "mgcl/mgcltransparent.h"


#define HFCL_STR2KEY(str)   ((Uint32)Str2Key(str))
#define HFCL_INVALIDKEY     ((Uint32)-1)
#define HFCL_IS_VALIDKEY(key)  ((key) != 0 && (key) != HFCL_INVALIDKEY)

#define DECLARE_CLASS_NAME(clss)  public: static const Uint32 CLASS_NAME;
#define DEFINE_CLASS_NAME(clss)   const Uint32 clss::CLASS_NAME = HFCL_STR2KEY(#clss);
#define DEFAULT_VIEW_DRAWABLESET(clss) \
		GetViewDrawableSet((clss::CLASS_NAME == 0) ? HFCL_STR2KEY(#clss) : clss::CLASS_NAME)
#define DEFAULT_VIEW_DRAWABLESETGROUP(clss) GetViewDrawableSetGroup(clss::CLASS_NAME)


#endif /* __HFCL_MGCL_H__ */

