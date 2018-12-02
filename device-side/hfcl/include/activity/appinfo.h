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


#ifndef _NGUX_AppInfo_h
#define _NGUX_AppInfo_h

#include "nguxcommon.h"

NAMESPACE_BEGIN

class ContextStream;
class BaseApp;

class AppInfo {
	public:
		AppInfo();
		AppInfo(string name, BaseApp* app, ContextStream* cs);
		virtual ~AppInfo();

		BaseApp* getApp(void);
		void setApp(BaseApp* app);

		string getName(void);
		void setName(string name);

		//ContextStream* getContextStream(void);
		//void setContextStream(ContextStream* cs);

	private:
		string         m_appname;
		BaseApp*       m_app;
		//ContextStream* m_stream;
};

NAMESPACE_END

#endif /* NGUX_AppManager_h */

