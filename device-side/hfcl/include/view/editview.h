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


#ifndef _HFCL_EDITVIEW_H
#define _HFCL_EDITVIEW_H

#define  EDITOR_MAX_LEN  1024
#define MAX_VALUES_IN_CALCULATOR_3COMMA     51
#define MAX_DIGIT_IN_CALCULATOR_3COMMA      20

namespace hfcl {

class EditView : public View
{
	DECLARE_CLASS_NAME(EditView)
public:
	EditView(View *p);
	EditView(View *p, DrawableSet* drset);
	virtual ~EditView();
    
    bool isWrapprVisible () { return false; }
    
    virtual void setText(const char * str) {}
	virtual void setText(string str) {}
	virtual string getText() { return string(""); }
	virtual unsigned int getTextLength() { return 0; }
	virtual void insertText(char *text) {}
#ifdef __MMI_T9__
	virtual void insertStringFromT9(char *ch, int cursor) {}
	virtual void entryAddWordMode(void) {}
	virtual void ResetT9Mode(bool  Neednotifyevent = FALSE) {}
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
    void setTextAlign(int align) { m_align = align; }
    int textAlign(void) { return m_align; }

    void setTextMaxLimit(unsigned int max, bool  IsCharCount = FALSE) { m_txtLimit = max; m_IsCharCount = IsCharCount; }
    unsigned int textMaxLimit(void) { return m_txtLimit; }

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
    void setBaseLineStyle(bool bSet)  { bSet ? m_style |= EDVS_BASELINE : m_style &= ~EDVS_BASELINE; }
    void setUpperCaseStyle(bool bSet) { bSet ? m_style |= EDVS_UPPERCASE : m_style &= ~EDVS_UPPERCASE; }
    void setLowerCaseStyle(bool bSet) { bSet ? m_style |= EDVS_LOWERCASE : m_style &= ~EDVS_LOWERCASE; }
    void setOmittedStyle(bool bSet)   { bSet ? m_style |= EDVS_OMITTED : m_style &= ~EDVS_OMITTED; }
    void setReadOnlyStyle(bool bSet)  { bSet ? m_style |= EDVS_READONLY : m_style &= ~EDVS_READONLY; }
    void setCommaStyle(bool bSet)  { bSet ? m_style |= EDVS_3COMMA : m_style &= ~EDVS_3COMMA; }
    void setOnlyNumber(bool bSet)  { bSet ? m_style |= EDVS_ONLY_NUM : m_style &= ~EDVS_ONLY_NUM; }
    void setABCMode(bool bSet)     { bSet ? m_style |= EDVS_ABC_MODE : m_style &= ~EDVS_ABC_MODE; }
    bool isBaseLine(void)  { return m_style & EDVS_BASELINE; }
    bool isUpperCase(void) { return m_style & EDVS_UPPERCASE; }
    bool isLowerCase(void) { return m_style & EDVS_LOWERCASE; }
    bool isOmitted(void)   { return m_style & EDVS_OMITTED; }
    bool isReadOnly(void)  { return m_style & EDVS_READONLY; }
    bool is3Comma(void)    { return m_style & EDVS_3COMMA; }
    bool isOnlyNumber(void){ return m_style & EDVS_ONLY_NUM; }
    bool isABCMode(void)   { return m_style & EDVS_ABC_MODE; }

	void setAutoOmitted(bool bSet)    { m_bAutoOmitted = bSet; }
	bool isAutoOmitted(void)    { return m_bAutoOmitted; }
    void setInputMode(Uint16 mode);
    void setDefaultInputMode(Uint16 mode) { m_default_input_mode = mode; }
    Uint16 getDefaultInputMode(void)    { return m_default_input_mode; }
   
    void setMargin(unsigned int horz, unsigned int vert) {
        m_hMargin = horz; m_vMargin = vert;
    }
    void margin(unsigned int *horz, unsigned int *vert) {
        *horz = m_hMargin ; *vert = m_vMargin ;
    }

    void setTextColor(unsigned int color) { m_txtColor = color; }
    unsigned int textColor(void) { return m_txtColor; }
    virtual void setFont (Logfont *f) { m_font = f; }

    void showCaret(bool bShow) { 
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
    bool isCaretShown(void) { return m_bCaretShown; }
    inline void blinkCaret(void) { showCaret(!m_bCaretShown); }
    virtual int setCaretPosition(int pos) { return 0; }
	virtual int caretPosition(void) { return 0; }
    virtual int getLineCount(void) { return 1; }

    void enableSpaceInsert(bool enable) { m_bInsertSpace = enable; }
    void setAutoTransChar(bool bTrans) { m_bAutoTrans = bTrans; }

	void adaptViewStyle(void);
	virtual void changeTheme(void);

    virtual int getTextMCharLen(void) { return 0; }
	
    virtual void onGetFocus(void);
	virtual void onLoseFocus(void);
    void setAddWordFlag(bool addflag) { m_addword = addflag;}	
    bool getAddWordFlag(void) { return m_addword;}
#ifdef __MMI_T9__	
    bool isStringSelect(void) { return m_bSelect; }
    void setActiveWordCount(int count) { m_activeword= count;}
    int getActiveWordCount(void) { return m_activeword;}	
#endif
    void setExchangeFlag(bool exchangeflag) { m_exchange = exchangeflag;}	
    bool getExchangeFlag(void) { return m_exchange;}
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
    void raiseNotifyEvent(int notiCode, int exParam1 = 0, int exParam2 = 0);
    void add_comma_in_calculator(char *str);
    void remove_comma_per3digit_in_calculator(char *str);
    void remove_Repeative_Zerodigit_WhenClear_in_calculator(char *str_all_index_merge);
    string apply_comma_multiple_value_in_calculator(char *str, int *cursor);
    ui_char_t  getMultitapCharCode(char chr);
    int mmi_chset_ucs2_to_utf8(unsigned char *utf8, unsigned short ucs2);
protected:
    string  m_tipText;
    int   m_align;
    unsigned int  m_txtLimit;
    unsigned long m_style;
    unsigned int  m_hMargin;
    unsigned int  m_vMargin;
    unsigned int  m_txtColor;
    bool  m_bCaretShown;
    bool  m_bInsertSpace;
    bool  m_bAutoTrans;
    bool  m_bAutoOmitted;
    bool  m_beditorflag;	
    IntRect m_caretRect;
    bool  m_bSelect;
    bool  m_addword;
#ifdef __MMI_T9__	
    unsigned int  m_inputmethod;
    bool  Neednotifyevent;
    int   m_activeword;	
#endif
    bool  m_exchange;
    bool  m_IsCharCount;
    Logfont *m_font;
    Uint16     m_input_mode;
    Uint16     m_default_input_mode;
    Uint16     m_writing_language;
};

} // namespace hfcl {

#endif

