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

#include "baseactivity.h"

namespace hfcl {

enum {
    POS_HIDE = 0,
    POS_STANDBY = 0x01,
    POS_MAINFRAME = 0x02,
};

typedef bool (*CB_ONBOOT) (void);

class BaseActivity;

class ActivityFactory {
public:
    virtual BaseActivity* create(void) = 0;

    struct ActivityData {
        int         position;
        int         pos_row;
        int         pos_col;
        const char *name;
        const char *description;
        int         captionId;
        int         iconId;
        int         smallIconId;
        int         bigIconId;
        CB_ONBOOT   onBoot;
    };

    virtual const ActivityData * getActivityData() { return NULL; }
};

} // namespace hfcl

/*
 *  declare a act-factory to create a act
 *  USAGE : DECLARE_ACTIVITY_FACTORY(Desktop);
 */
#define DECLARE_ACTIVITY_FACTORY(actClassName)                  \
    class actClassName##Factory: public ActivityFactory {       \
        public:                                                 \
            static actClassName##Factory* getInstance(void);    \
            virtual BaseActivity* create(void);                 \
        private:                                                \
            ActivityFactory::ActivityData* getActivityInfo();   \
    };

/*
 *  define a act-factory to create a act
 *  USAGE : DEFINE_ACTIVITY_FACOTORY(Desktop);
 */
#define DEFINE_ACTIVITY_FACTORY(actClassName)                           \
    actClassName##Factory* actClassName##Factory::getInstance(void) {   \
        static actClassName##Factory* s_single = NULL;                  \
        if (!s_single) {                                                \
            s_single = HFCL_NEW_EX(actClassName##Factory, ());          \
        }                                                               \
        return s_single;                                                \
    }                                                                   \
    BaseActivity* actClassName##Factory::create(void) {                 \
        return (HFCL_NEW_EX(actClassName, ()));                         \
    }

#define BEGIN_DEFINE_ACTIVITY(actClassName)             \
    DEFINE_ACTIVITY_FACTORY(actClassName)               \
    ActivityFactory::ActivityData*                      \
        actClassName##Factory::getActivityInfo (){      \
            static ActivityData _act_info = {0};        \
            if(_act_info.name == NULL) {

#define END_DEFINE_ACTIVITY  } return &_act_info; }

#define ACTIVITY_SET(name, value)    _act_info.name = value;

/*
 *  get act-factory from act-class-name
 *  USAGE : GET_ACTIVITY_FACTORY(Desktop);
 */
#define GET_ACTIVITY_FACTORY(actClassName)              \
    actClassName##Factory::getInstance()

/*
 *  register act to actmanager
 *  USAGE : REGISTER_ACTIVITY("desktop", Desktop);
 */
#define REGISTER_ACTIVITY(actName, actClassName)        \
    ActivityManager::getInstance()->registerActivity(   \
            actName, GET_ACTIVITY_FACTORY(actClassName))

/*
 *  register act to actmanager with additional information
 *  USAGE : REGISTER_ACTIVITY_EX("desktop", textResId, iamgeResId, Desktop);
 */
#define REGISTER_ACTIVITY_EX(actName, textResId, imageResId, actClassName) \
    ActivityManager::getInstance()->registerActivity(actName, \
            textResId, imageResId, GET_ACTIVITY_FACTORY(actClassName))

#endif /* HFCL_ACTIVITY_ACTIVITYFACTORY_H_ */

