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


#ifndef  _MULTIFIELDEDITOR_H
#define  _MULTIFIELDEDITOR_H

#include "formateditview.h"

namespace hfcl {

typedef enum
{
    TIME_FORMAT =0,
    DATE_FORMAT   =1,
}FIELD_FORMAT;

class MultiFieldEditor : public FormatEditView
{
    public:
        /**
         * Constructor/Destructor
         */
        MultiFieldEditor(View* parent);
        MultiFieldEditor(View* parent, DrawableSet* drset);
        MultiFieldEditor(int id, int x, int y, int w, int h);
        ~MultiFieldEditor();

        /**
         * Init. Param like FormatEditView.init
         */
        void multiFieldEditorInit(int field_count,
                const char ** p_content_array,
                const char ** p_formator_array,
                char* p_content_max_nr,
                int* value_range,
                FIELD_FORMAT field_format,
                int date_system = 0
                );
        /**
         * Refresh
         */
        void refresh(void);
                /**
         * Dispatch Events From Parent
         */
        virtual bool dispatchEvent(Event* event);
        /**
         * Set Range
         */
        bool setRange(int index, int min, int max);
        /**
         * Set Value
         *
         * @param value value to set.Notice : i will NOT check the value!
         * @param index field index to set the value.
         * @return true when set, false when not set
         */
        bool setValue(int index, int value);
        int getValue(int index);
        void setTextAlign(unsigned int align);
        void checkDays(void);
    protected:
        /**
         * Dispatch Events From Parent
         */
        virtual bool checkContent(void);
        /**
         * Pre Change Field Content
         *
         * @retval true means continue process filed content
         */
        virtual bool onChangeField(void);


    private:
        // max vals : FE_MAX_FIELD_NR from FormatEditView
        int m_max[FE_MAX_FIELD_NR];
        // min vals : FE_MAX_FIELD_NR from FormatEditView
        int m_min[FE_MAX_FIELD_NR];
        FIELD_FORMAT m_field_format;
        int m_dateFormat;
        int m_dateSystem;
        DECLARE_CLASS_NAME(MultiFieldEditor)
};

} // namespace hfcl

#endif //_MULTIFIELDEDITOR_H
