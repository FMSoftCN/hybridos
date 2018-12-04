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


#ifndef _HFCL_Intent_h
#define _HFCL_Intent_h

#include "common/common.h"
#include "common/alternativestl.h"

namespace hfcl {

/*
 * class Intent
 * it is used to transfer data between applications.
 */
class Intent {
public:
    Intent(int action = 0, string data = "", int tag = 0);
    Intent(Intent * intent);

    const int action() const {return m_action;}
    const string data() const {return m_data;}
    const void* dataEx() const {return m_dataEx;}
    const int tag() const {return m_tag;}

    void setAction(int iaction) {m_action = iaction;}
    void setData(string sdata) {m_data = sdata;}
    void setDataEx(void* sdata) {m_dataEx = sdata;}
    void settag(int itag) {m_tag = itag;}
   /////////////////////////////////////////
    void putValue(string& key, void* value);
    void *getValue(string key);
#if 0
    void putInt(string key, int value);
    int getInt(string key);
    void putString(string key, string& value);
    string& getString(string key);
#endif

protected:
    /* 
     * m_action is globally defined ACTION.
     * e.g. ACTION_MOVE, ACTION_VIEW, ACTION_EDIT, ACTION_DIAL etc.
     */
    int m_action;

    /* 
     * m_date is transfer data.
     * e.g. "13800138000" or "zhang san" etc.
     */
    string m_data;
    void* m_dataEx;

    /*
     * m_tag is tag of return by Which an application.
     * e.g. App1 call App2 , App2 save m_tag value.
     * when App2 exit, return a intent(..., ..., m_tagvalue).
     */
    int m_tag;
private:
    MAPCLASSKEY(string, void*, IntentMap);
    IntentMap m_intentStore;
};

} // namespace hfcl

#endif /* HFCL_Intent_h */
