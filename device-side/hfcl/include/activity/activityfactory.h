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

#ifndef HFCL_ACTIVITY_ACTIVITYFACTORY_H_
#define HFCL_ACTIVITY_ACTIVITYFACTORY_H_

#include "activity/baseactivity.h"

namespace hfcl {

enum {
	POS_HIDE = 0,
	POS_STANDBY = 0x01,
	POS_MAINFRAME = 0x02,
};

typedef bool (*CB_ONBOOT) (void);

class ActivityFactory {
public:
    virtual BaseActivity* create(void) = 0;

    struct AppData {
        int 	    position;
        int 	    pos_row;
        int 	    pos_col;
        const char *name;
        const char *description;
        int         captionId;
        int         iconId;
        int         smallIconId;
        int         bigIconId;
        CB_ONBOOT   onBoot;
    };

    virtual const AppData * getAppData() { return NULL; }
}

} // namespace hfcl

/*
 *  declare a app-factory to create a app
 *  USAGE : DECLARE_ACTIVITY_FACTORY(Desktop);
 */
#define DECLARE_ACTIVITY_FACTORY(appClassName)                     \
    class appClassName##Factory: public ActivityFactory {          \
        public:                                               \
            static appClassName##Factory* getInstance(void);  \
        	virtual BaseActivity* create(void);                    \
        private:                                              \
		    ActivityFactory::AppData* getAppInfo();         \
    };

/*
 *  define a app-factory to create a app
 *  USAGE : DEFINE_ACTIVITY_FACOTORY(Desktop);
 */
#define DEFINE_ACTIVITY_FACTORY(appClassName)                     \
    appClassName##Factory* appClassName##Factory::getInstance(void) { \
		static appClassName##Factory* s_single = NULL;                       \
        if (!s_single) {                                              \
            s_single = HFCL_NEW_EX(appClassName##Factory, ());        \
        }                                                             \
        return s_single;                                              \
    }                                                                 \
    BaseActivity* appClassName##Factory::create(void) {                    \
        return (HFCL_NEW_EX(appClassName, ()));                       \
    }

#define BEGIN_DEFINE_ACTIVITY(appClassName)             \
	DEFINE_ACTIVITY_FACTORY(appClassName)               \
	ActivityFactory::AppData*                    \
		appClassName##Factory::getAppInfo (){      \
			static AppData _app_info = {0}; \
			if(_app_info.name == NULL) { 

#define END_DEFINE_ACTIVITY  } return &_app_info; }

#define ACTIVITY_SET(name, value)    _app_info.name = value;

/*
 *  get app-factory from app-class-name 
 *  USAGE : GET_ACTIVITY_FACTORY(Desktop);
 */
#define GET_ACTIVITY_FACTORY(appClassName)    \
    appClassName##Factory::getInstance()

/*
 *  register app to appmanager
 *  USAGE : REGISTER_ACTIVITY("desktop", Desktop);
 */
#define REGISTER_ACTIVITY(appName, appClassName) \
    ActivityManager::getInstance()->registerApp(appName, GET_ACTIVITY_FACTORY(appClassName))

/*
 *  register app to appmanager with additional information
 *  USAGE : REGISTER_ACTIVITY_EX("desktop", textResId, iamgeResId, Desktop);
 */
#define REGISTER_ACTIVITY_EX(appName, textResId, imageResId, appClassName) \
    ActivityManager::getInstance()->registerApp(appName, textResId, imageResId, GET_ACTIVITY_FACTORY(appClassName))

#endif /* HFCL_ACTIVITY_ACTIVITYFACTORY_H_ */

