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

#include "common/log.h"
#include "contextstream.h"
#include "appinfo.h"
#include "baseapp.h"

NAMESPACE_BEGIN


AppInfo::AppInfo()
    : m_appname("")
    , m_app(NULL)
    //, m_stream(NULL)
{
}

AppInfo::AppInfo(string name, BaseApp* app, ContextStream* cs)
    : m_appname(name)
    , m_app(app)
    //, m_stream(cs)
{
}

AppInfo::~AppInfo()
{
    if (m_app)
        // delete m_app;
        NGUX_DELETE(m_app);
    m_app = NULL;
	/*
    if (m_stream)
        // delete m_stream;
        NGUX_DELETE(m_stream);
    m_stream = NULL;
    */
}

BaseApp* AppInfo::getApp(void)
{
	return m_app;
}

void AppInfo::setApp(BaseApp* app)
{
	m_app = app;
}

string AppInfo::getName(void)
{
	return m_appname;
}

void AppInfo::setName(string name)
{
	m_appname = name;
}
/*
ContextStream* AppInfo::getContextStream(void)
{
	return m_stream;
}

void AppInfo::setContextStream(ContextStream* cs)
{
	m_stream = cs;
}
*/
NAMESPACE_END

