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


#ifndef  _TIMEEDITOR_H 
#define  _TIMEEDITOR_H

#include "formateditview.h"

NAMESPACE_BEGIN

class TimeEditor : public FormatEditView 
{
    public:
        typedef enum __enum_TimeFormat
        {
            F12HOUR = 0,
            F24HOUR
        }TimeFormat;
        /**
         * Constructor/Destructor
         */
        TimeEditor(View* parent);
        TimeEditor(View* parent, DrawableSet* drset);
		TimeEditor(NGInt id, NGInt x, NGInt y, NGInt w, NGInt h);
        ~TimeEditor();

        /*
         * Refresh
         */
        void refresh(void);

        /**
         * Dispatch Events From Parent
         */
        virtual NGBool dispatchEvent(Event* event);

        /**
         * Set/Get Time Format
         */
        void setTimeFormat(TimeFormat format);
        TimeFormat timeFormat(void) { return m_format; }
        /**
         * Set/Get Hour/Minute
         */
        void setHour(NGInt hour);
        NGInt hour(void);
        void setMinute(NGInt minute);
        NGInt minute(void);
        void setAm(NGBool am);
        NGBool isAm(void);

    protected:
        /**
         * Init
         */
        void timeEditorInit(void);
        /**
         * Dispatch Events From Parent
         */
        virtual NGBool checkContent(void);
        /**
         * Pre Change Field Content
         *
         * @retval true means continue process filed content
         */
        virtual NGBool onChangeField(void);


    private:
        TimeFormat m_format;
        NGInt m_max_h;
        NGInt m_min_h;
        NGInt m_max_m;
        NGInt m_min_m;
        NGInt m_field_h;
        NGInt m_field_m;
        NGInt m_field_ap;

		DECLARE_CLASS_NAME(TimeEditor)
};

NAMESPACE_END

#endif
