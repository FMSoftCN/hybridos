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


#ifndef  _FORMATEDITVIEW_H
#define  _FORMATEDITVIEW_H

#include "view.h"

namespace hfcl {

#if defined(FE_MAX_FIELD_NR)
#undef FE_MAX_FIELD_NR
#endif

#define FE_MAX_FIELD_NR 10

/**
 * @brief FormatEditView is a Edit like : xxxField xxxFormator xxxField xxxFormator ....
 *
 * This View needs two kinds of Drawable :
 * (FormatEditView::DR_CONTENT) DR_CONTENT and (FormatEditView::DR_FIELD_BKG) DR_CONTENT2
 *
 * 1. Support Max 10 Fields
 * 2. Support Set/Get Field Width, by default, string's width
 * 3. Support Set/Get Formator Width, by default, string's width
 * 4. Support Reinit Field At Runtime
 */
class FormatEditView : public View
{
    public:
        /**
         * Constructor/Destructor
         */
        FormatEditView(View* parent);
        FormatEditView(View* parent, DrawableSet* drset);
        FormatEditView(int id, int x, int y, int w, int h);
        ~FormatEditView();

        /**
         * Relayout
         */
        void reLayout(void);
                /**
         * Focus Event Handle
         */
        virtual void onGetFocus();
        virtual void onLoseFocus();
        int getCurrentIndex(void) { return m_current_index; }
        int getAmPos();
        void setAlignMargin();

        void setTextFont(unsigned int font);

    protected:
        /**
         * InitEditor With Content And Format
         *
         * @param field_count Field Count, Formator Count Is field_count - 1
         * @param p_content_array Pointer To Field Content Array
         * e.g. :
         * const char *content[] =
         * {
         *  "12",
         *  "58",
         *  "29"
         * };
         *
         * @param p_formator_array Pointer To Formator Array
         * e.g. :
         * const char *formator[] =
         * {
         *  "/",
         *  "/",
         *  ":"
         * };
         *
         * @param p_content_max_nr Pointer To Max Char Nr Array
         * e.g. :
         * char max_char[] =
         * {
         *  2,
         *  2,
         *  2
         * };
         */
        void initEditor(int field_count, const char ** p_content_array, const char ** p_formator_array, char* p_content_max_nr);
        /**
         * Dispatch Events From Parent
         */
        virtual bool dispatchEvent(Event* event);


        /**
         * GetVisibleWidth
         *
         * This interface return width of visible things in edit
         */
        int visibleWidth(void);
        /**
         * GetVisibleHeight
         *
         * This interface return height of visible things in edit
         */
        int visibleHeight(void);
        /**
         * Process Number Key Event
         */
        virtual bool processNumberKey(int key_code);
        /**
         * Move To Filed
         */
        void nextFiled(void);
        void prevFiled(void);
        void moveToNext(void);
        void moveToPrev(void);
        void updateZero(void);
        /**
         * Auto Add Zero Flag Set/Get
         */
        bool autoAddZero(void) { return m_auto_add_zero; }
        void setAutoAddZero(bool add) { m_auto_add_zero = add; }
        void addZero(void);
        /**
         * Auto Next
         */
        bool autoNext(void) { return m_auto_next; }
        void setAutoNext(bool next) { m_auto_next = next; }

        /**
         * Check Content
         */
        virtual bool checkContent(void);

    protected:
        /**
         * Get Input Char From KeyCode
         */
        const char * getInputChar(int key_code);
        const char * getAlignZero(int count);
        /**
         * Pre Change Field Content
         *
         * @retval true means continue process filed content
         */
        virtual bool onChangeField(void) { return true; }

        /**
         * Internal Init
         */
        void init(void);
        /**
         * Focus Move
         */
        void focusNext(void);
        void focusPrev(void);
        /**
         * DrawContent
         */
        virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
        /**
         * calcFieldWidth
         */
        int calcTextWidth(char * str);

    protected:
        /**
         * Field Count
         */
        int m_field_count;
        /**
         * Field Content
         */
        string m_field_content[FE_MAX_FIELD_NR];
        /**
         * Formator
         */
        string m_formators[FE_MAX_FIELD_NR];
        /**
         * Field Max Char Nr
         */
        unsigned char m_field_max_char[FE_MAX_FIELD_NR];
        /**
         * margin_left
         */
        int m_margin_left;
        /**
         * margin_top
         */
        int m_margin_top;
        /**
         * Current Edit
         */
        int m_current_index;
        /**
         * Show Flag Of Current Hilight
         */
        bool m_show_current_hilight;
        /**
         * Auto Patch Zero Flag
         */
        bool m_auto_add_zero;
        /**
         * Auto Next Filed
         */
        bool m_auto_next;

        DECLARE_CLASS_NAME(FormatEditView)
};

} // namespace hfcl

#endif
