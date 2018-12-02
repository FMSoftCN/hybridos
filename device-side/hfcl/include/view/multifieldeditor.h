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


#ifndef  _MULTIFIELDEDITOR_H 
#define  _MULTIFIELDEDITOR_H

#include "formateditview.h"

NAMESPACE_BEGIN

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
		MultiFieldEditor(NGInt id, NGInt x, NGInt y, NGInt w, NGInt h);
        ~MultiFieldEditor();

        /**
         * Init. Param like FormatEditView.init
         */
        void multiFieldEditorInit(NGInt field_count, 
                NGCPStr* p_content_array, 
                NGCPStr* p_formator_array, 
                NGChar* p_content_max_nr,
                NGInt* value_range,
                FIELD_FORMAT field_format,
                NGInt date_system = 0
                );
        /**
         * Refresh
         */
        void refresh(void);
                /**
         * Dispatch Events From Parent
         */
        virtual NGBool dispatchEvent(Event* event);
        /**
         * Set Range
         */
        NGBool setRange(NGInt index, NGInt min, NGInt max);
        /**
         * Set Value
         *
         * @param value value to set.Notice : i will NOT check the value!
         * @param index field index to set the value.
         * @return true when set, false when not set
         */
        NGBool setValue(NGInt index, NGInt value);
        NGInt getValue(NGInt index);
        void setTextAlign(NGUInt align);
        void checkDays(void);
    protected:
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
        // max vals : FE_MAX_FIELD_NR from FormatEditView
        NGInt m_max[FE_MAX_FIELD_NR];
        // min vals : FE_MAX_FIELD_NR from FormatEditView
        NGInt m_min[FE_MAX_FIELD_NR];
        FIELD_FORMAT m_field_format;
        NGInt m_dateFormat;
        NGInt m_dateSystem;        
		DECLARE_CLASS_NAME(MultiFieldEditor)
};

NAMESPACE_END

#endif //_MULTIFIELDEDITOR_H
