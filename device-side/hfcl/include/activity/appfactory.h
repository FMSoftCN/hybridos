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


#ifndef _NGUX_APPFACTORY_H
#define _NGUX_APPFACTORY_H

NAMESPACE_BEGIN

#include "image.h"

class BaseApp;
class Image;

enum {
	POS_HIDE = 0,
	POS_STANDBY = 0x01,
	POS_MAINFRAME = 0x02,
};

typedef NGBool (*CB_APP_ONBOOT) (void);

class AppFactory {
    public:
        virtual BaseApp* create(void) = 0;

		struct AppInformation {
			NGInt 	position;
			NGInt 	pos_row;
			NGInt 	pos_col;
			NGCPStr name;
			NGCPStr description;
			NGInt   captionId;
			NGInt   iconId;
			NGInt   smallIconId;
			NGInt   bigIconId;
            CB_APP_ONBOOT onBoot;
		};

		virtual const AppInformation * getAppInfo() { return NULL; }
};

/*
 *  declare a app-factory to create a app
 *  USAGE : DECLARE_APP_FACTORY(Desktop);
 */
#define DECLARE_APP_FACTORY(appClassName)                     \
    class appClassName##Factory: public AppFactory {          \
        public:                                               \
            static appClassName##Factory* getInstance(void);  \
        	virtual BaseApp* create(void);                    \
        private:                                              \
		    AppFactory::AppInformation* getAppInfo();         \
    };

/*
 *  define a app-factory to create a app
 *  USAGE : DEFINE_APP_FACOTORY(Desktop);
 */
#define DEFINE_APP_FACTORY(appClassName)                     \
    appClassName##Factory* appClassName##Factory::getInstance(void) { \
		static appClassName##Factory* s_single = NULL;                       \
        if (!s_single) {                                              \
            s_single = NGUX_NEW_EX(appClassName##Factory, ());        \
        }                                                             \
        return s_single;                                              \
    }                                                                 \
    BaseApp* appClassName##Factory::create(void) {                    \
        return (NGUX_NEW_EX(appClassName, ()));                       \
    }

#define BEGIN_DEFINE_APP(appClassName)             \
	DEFINE_APP_FACTORY(appClassName)               \
	AppFactory::AppInformation*                    \
		appClassName##Factory::getAppInfo (){      \
			static AppInformation _app_info = {0}; \
			if(_app_info.name == NULL) { 

#define END_DEFINE_APP  } return &_app_info; }

#define APP_SET(name, value)    _app_info.name = value;


/*
 *  get app-factory from app-class-name 
 *  USAGE : GET_APP_FACTORY(Desktop);
 */
#define GET_APP_FACTORY(appClassName)    \
    appClassName##Factory::getInstance()

/*
 *  register app to appmanager
 *  USAGE : REGISTER_APP("desktop", Desktop);
 */
#define REGISTER_APP(appName, appClassName) \
    AppManager::getInstance()->registerApp(appName, GET_APP_FACTORY(appClassName))

/*
 *  register app to appmanager with additional information
 *  USAGE : REGISTER_APP_EX("desktop", textResId, iamgeResId, Desktop);
 */
#define REGISTER_APP_EX(appName, textResId, imageResId, appClassName) \
    AppManager::getInstance()->registerApp(appName, textResId, imageResId, GET_APP_FACTORY(appClassName))

NAMESPACE_END

#endif /* NGUX_APPFACTORY_H */

