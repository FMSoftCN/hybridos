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

#include "activity/activityinfo.h"

namespace hfcl {


ActivityInfo::ActivityInfo()
    : m_actname("")
    , m_act(NULL)
    //, m_stream(NULL)
{
}

ActivityInfo::ActivityInfo(string name, BaseActivity* act, ContextStream* cs)
    : m_actname(name)
    , m_act(act)
    //, m_stream(cs)
{
}

ActivityInfo::~ActivityInfo()
{
    if (m_act)
        // delete m_act;
        HFCL_DELETE(m_act);
    m_act = NULL;
    /*
    if (m_stream)
        // delete m_stream;
        HFCL_DELETE(m_stream);
    m_stream = NULL;
    */
}

BaseActivity* ActivityInfo::getActivity(void)
{
    return m_act;
}

void ActivityInfo::setActivity(BaseActivity* act)
{
    m_act = act;
}

string ActivityInfo::getName(void)
{
    return m_actname;
}

void ActivityInfo::setName(string name)
{
    m_actname = name;
}
/*
ContextStream* ActivityInfo::getContextStream(void)
{
    return m_stream;
}

void ActivityInfo::setContextStream(ContextStream* cs)
{
    m_stream = cs;
}
*/
} // namespace hfcl

