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

#include "activity/baseactivity.h"

namespace hfcl {

BaseActivity::~BaseActivity()
{
	if(m_name)
	   free(m_name);
}

const char * BaseActivity::name(void)
{
	return m_name;
}

void BaseActivity::setName(const char * p_name)
{
	if (m_name != NULL)
		free(m_name);
	m_name = strdup(p_name);
}

BaseActivity::ACT_STATE BaseActivity::state(void)
{
	return m_state;
}

void BaseActivity::setState(ACT_STATE i_state)
{
	m_state = i_state;
}

void BaseActivity::close(void)
{
	ActivityManager::getInstance()->exit(this);
}

bool BaseActivity::isSuspendable(void)
{
	return true;
}

} // namespace hfcl
