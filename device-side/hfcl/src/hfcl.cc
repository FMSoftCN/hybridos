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

#include "hfcl.h"

#include "activity/activitymanager.h"
#include "drawable/drawable.h"
#include "drawable/colordrawable.h"
#include "drawable/imagedrawable.h"
#include "drawable/statecolordrawable.h"
#include "drawable/stateimagedrawable.h"
#include "drawable/statetextdrawable.h"
#include "drawable/textdrawable.h"

namespace hfcl {

int InitHFCL (int argc, const char** argv)
{
    InitGUI (argc, argv);

    ActivityManager::getInstance()->startTimerService();

//  REGISTER_DRFACTORY(ButtonDrawable);
    REGISTER_DRFACTORY(ColorDrawable);
    REGISTER_DRFACTORY(ImageDrawable);
//  REGISTER_DRFACTORY(ItemDrawable);
//  REGISTER_DRFACTORY(SimpleItemContentDrawable);
    REGISTER_DRFACTORY(StateColorDrawable);
    REGISTER_DRFACTORY(StateImageDrawable);
    REGISTER_DRFACTORY(StateTextDrawable);
    REGISTER_DRFACTORY(PushableStateTextDrawable);
    REGISTER_DRFACTORY(TextDrawable);
    REGISTER_DRFACTORY(PushableTextDrawable);

    return 0;
};

} // namespace hfcl

