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


#ifndef _NGUX_EDITVIEW_H
#define _NGUX_EDITVIEW_H

#define  EDITOR_MAX_LEN  1024
#define MAX_VALUES_IN_CALCULATOR_3COMMA     51
#define MAX_DIGIT_IN_CALCULATOR_3COMMA      20

NAMESPACE_BEGIN

class EditView : public View
{
	DECLARE_CLASS_NAME(EditView)
public:
	EditView(View *p);
	EditView(View *p, DrawableSet* drset);
	virtual ~EditView();
    
    NGBool isWrapprVisible () { return false; }
    
    virtual void setText(NGCPStr str) {}
	virtual void setText(string str) {}
	virtual string getText() { return string(""); }
	virtual NGUInt getTextLength() { return 0; }
	virtual void insertText(char *text) {}
#ifdef __MMI_T9__
	virtual void insertStringFromT9(char *ch, int cursor) {}
	virtual void entryAddWordMode(void) {}
	virtual void ResetT9Mode(NGBool  Neednotifyevent = FALSE) {}
#endif

    typedef enum {
        // horz
        TextAlignLeft     = 0x00,
        TextAlignCenter   = 0x01,
        TextAlignRight    = 0x02,
        // vert
        TextVAlignTop     = 0x00,
        TextVAlignCenter  = 0x10,
        TextVAlignBottom  = 0x20,
        // mask
        TextAlignMask     = 0x0F,
        TextVAlignMask    = 0xF0,
    } eTextAlign;
    void setTextAlign(NGInt align) { m_align = align; }
    NGInt textAlign(void) { return m_align; }

    void setTextMaxLimit(NGUInt max, NGBool  IsCharCount = FALSE) { m_txtLimit = max; m_IsCharCount = IsCharCount; }
    NGUInt textMaxLimit(void) { return m_txtLimit; }

    void setTipText(const char *tipTxt) { m_tipText = tipTxt; }
    string tipText (void) { return m_tipText; }
    void SwitchShiftToLower(string str , int caretpos, bool flag);

    enum {
        EDVS_BASELINE  = 0x0001, //TODO
        EDVS_UPPERCASE = 0x0002, //TODO
        EDVS_LOWERCASE = 0x0004, //TODO
        EDVS_OMITTED   = 0x0008,
        EDVS_READONLY  = 0x0010,
        EDVS_3COMMA    = 0x0020,
        EDVS_ABC_MODE  = 0x0040,
        EDVS_ONLY_NUM  = 0x0080,
        EDVS_CHAR_BREAK  = 0x0100,
    };
    void setBaseLineStyle(NGBool bSet)  { bSet ? m_style |= EDVS_BASELINE : m_style &= ~EDVS_BASELINE; }
    void setUpperCaseStyle(NGBool bSet) { bSet ? m_style |= EDVS_UPPERCASE : m_style &= ~EDVS_UPPERCASE; }
    void setLowerCaseStyle(NGBool bSet) { bSet ? m_style |= EDVS_LOWERCASE : m_style &= ~EDVS_LOWERCASE; }
    void setOmittedStyle(NGBool bSet)   { bSet ? m_style |= EDVS_OMITTED : m_style &= ~EDVS_OMITTED; }
    void setReadOnlyStyle(NGBool bSet)  { bSet ? m_style |= EDVS_READONLY : m_style &= ~EDVS_READONLY; }
    void setCommaStyle(NGBool bSet)  { bSet ? m_style |= EDVS_3COMMA : m_style &= ~EDVS_3COMMA; }
    void setOnlyNumber(NGBool bSet)  { bSet ? m_style |= EDVS_ONLY_NUM : m_style &= ~EDVS_ONLY_NUM; }
    void setABCMode(NGBool bSet)     { bSet ? m_style |= EDVS_ABC_MODE : m_style &= ~EDVS_ABC_MODE; }
    NGBool isBaseLine(void)  { return m_style & EDVS_BASELINE; }
    NGBool isUpperCase(void) { return m_style & EDVS_UPPERCASE; }
    NGBool isLowerCase(void) { return m_style & EDVS_LOWERCASE; }
    NGBool isOmitted(void)   { return m_style & EDVS_OMITTED; }
    NGBool isReadOnly(void)  { return m_style & EDVS_READONLY; }
    NGBool is3Comma(void)    { return m_style & EDVS_3COMMA; }
    NGBool isOnlyNumber(void){ return m_style & EDVS_ONLY_NUM; }
    NGBool isABCMode(void)   { return m_style & EDVS_ABC_MODE; }

	void setAutoOmitted(NGBool bSet)    { m_bAutoOmitted = bSet; }
	NGBool isAutoOmitted(void)    { return m_bAutoOmitted; }
    void setInputMode(U16 mode);
    void setDefaultInputMode(U16 mode) { m_default_input_mode = mode; }
    U16 getDefaultInputMode(void)    { return m_default_input_mode; }
   
    void setMargin(NGUInt horz, NGUInt vert) {
        m_hMargin = horz; m_vMargin = vert;
    }
    void margin(NGUInt *horz, NGUInt *vert) {
        *horz = m_hMargin ; *vert = m_vMargin ;
    }

    void setTextColor(NGUInt color) { m_txtColor = color; }
    NGUInt textColor(void) { return m_txtColor; }
    virtual void setFont (Logfont *f) { m_font = f; }

    void showCaret(NGBool bShow) { 
		if(isReadOnly())
		{
			m_bCaretShown = false;
		}
		else
		{
			m_bCaretShown = bShow; 
        	updateView(m_caretRect);
		}
    }
    NGBool isCaretShown(void) { return m_bCaretShown; }
    inline void blinkCaret(void) { showCaret(!m_bCaretShown); }
    virtual NGInt setCaretPosition(NGInt pos) { return 0; }
	virtual NGInt caretPosition(void) { return 0; }
    virtual NGInt getLineCount(void) { return 1; }

    void enableSpaceInsert(NGBool enable) { m_bInsertSpace = enable; }
    void setAutoTransChar(NGBool bTrans) { m_bAutoTrans = bTrans; }

	void adaptViewStyle(void);
	virtual void changeTheme(void);

    virtual NGInt getTextMCharLen(void) { return 0; }
	
    virtual void onGetFocus(void);
	virtual void onLoseFocus(void);
    void setAddWordFlag(NGBool addflag) { m_addword = addflag;}	
    NGBool getAddWordFlag(void) { return m_addword;}
#ifdef __MMI_T9__	
    NGBool isStringSelect(void) { return m_bSelect; }
    void setActiveWordCount(NGInt count) { m_activeword= count;}
    NGInt getActiveWordCount(void) { return m_activeword;}	
#endif
    void setExchangeFlag(NGBool exchangeflag) { m_exchange = exchangeflag;}	
    NGBool getExchangeFlag(void) { return m_exchange;}
	enum {
		NOTIFY_BEGIN = CustomEvent::CUS_MAX,
		NOTIFY_EDIT_CHANGED = 1000,
		NGTIFY_EDIT_LINE_CHANGED,
		NOTIFY_EDIT_ENTER,  // only in single-line edit
		NOTIFY_EDIT_MAXTEXT,
		NOTIFY_EDIT_READY_INSERT_TEXT,
		NOTIFY_EDIT_REPLACING_TEXT,
		NOTIFY_EDIT_SETTING_TEXT,
		NOTIFY_EDIT_HILIGHT_NUMBER_FOCUS,
		NOTIFY_EDIT_HILIGHT_NUMBER_DATA,
	};
protected:
    void raiseNotifyEvent(int notiCode, NGInt exParam1 = 0, NGInt exParam2 = 0);
    void add_comma_in_calculator(char *str);
    void remove_comma_per3digit_in_calculator(char *str);
    void remove_Repeative_Zerodigit_WhenClear_in_calculator(char *str_all_index_merge);
    string apply_comma_multiple_value_in_calculator(char *str, NGInt *cursor);
    ui_char_t  getMultitapCharCode(char chr);
    int mmi_chset_ucs2_to_utf8(unsigned char *utf8, unsigned short ucs2);
protected:
    string  m_tipText;
    NGInt   m_align;
    NGUInt  m_txtLimit;
    NGULong m_style;
    NGUInt  m_hMargin;
    NGUInt  m_vMargin;
    NGUInt  m_txtColor;
    NGBool  m_bCaretShown;
    NGBool  m_bInsertSpace;
    NGBool  m_bAutoTrans;
    NGBool  m_bAutoOmitted;
    NGBool  m_beditorflag;	
    IntRect m_caretRect;
    NGBool  m_bSelect;
    NGBool  m_addword;
#ifdef __MMI_T9__	
    NGUInt  m_inputmethod;
    NGBool  Neednotifyevent;
    NGInt   m_activeword;	
#endif
    NGBool  m_exchange;
    NGBool  m_IsCharCount;
    Logfont *m_font;
    U16     m_input_mode;
    U16     m_default_input_mode;
    U16     m_writing_language;
};

NAMESPACE_END

#endif

