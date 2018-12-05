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

#include "activity/intent.h"

#include "common/common.h"

namespace hfcl {

Intent::Intent(int i_action, string s_data, int i_tag)
    : m_action(i_action)
    , m_data(s_data)
    , m_tag(i_tag)
{
}

Intent::Intent(Intent *intent)
	: m_action(intent->action())
	, m_data(intent->data())
	, m_tag(intent->tag())
{
	setDataEx((void*)(intent->dataEx()));
}

void Intent::putValue(string& key, void* value)
{
	m_intentStore[key] = value;
}

void *Intent::getValue(string key)
{
	IntentMap::iterator it;
	it = m_intentStore.find(key);
	if(it != m_intentStore.end())
		return m_intentStore[key];
	return (void*)0;
}

} // namespace hfcl
