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


#include <ctype.h>

#include "ngux.h"

#include "ucs2-utils.h"
#include "ucs2-bidi.h"
#include "codeconvert.h"

#include "imeinterface.h"
#include "imebase.h"

/****************** TODO LIST ****************
 * 01. tip text             ---- ??
 * 02. mouse                ---- ??
 * 03. select text          ---- ??
 * 04. Base Line Draw       ---- ??
 **********************************************/
extern BOOL IsArabicSet(void);
extern char m_edit_tempstr1[EDITOR_MAX_LEN];
extern char m_edit_tempstr2[EDITOR_MAX_LEN];

#define DEF_PSWD_CHAR       '*'

namespace hfcl {

DEFINE_CLASS_NAME(SlEditView)

//static int m_char_pos = 0;

SlEditView::SlEditView(View *p)
    : EditView (p)
    , m_strings("")
    , m_pswdChar(DEF_PSWD_CHAR)
	, m_caretPos(0)
    , m_passwordStyleShow(0)
    , m_prevEventType(0)
    , m_highlight(false)
{
    //TODO
}

SlEditView::SlEditView(View *p, DrawableSet *drs)
    : EditView (p, drs)
    , m_strings("")
    , m_pswdChar(DEF_PSWD_CHAR)
	, m_caretPos(0)
    , m_passwordStyleShow(0)
    , m_prevEventType(0)
    , m_highlight(false)
{
    //TODO
}

SlEditView::~SlEditView()
{
    if (0 != m_timerId)
        removeTimer(m_timerId);
     if (0 != m_SlTimerId)
    {
        MMI_active_multitap = -1;	
        removeTimer(m_SlTimerId);
    }
}
    
void SlEditView::onGetFocus(void)
{
    if (0 != m_timerId)
        removeTimer(m_timerId);
    m_timerId = registerTimer(500, "SlEditView");
    if(m_addword)
        imeEnable(true);
	EditView::onGetFocus();

    if (m_bAutoOmitted){
        setOmittedStyle(false);
    }
}

void SlEditView::onLoseFocus(void)
{
    if(m_timerId)
    {
        removeTimer(m_timerId);
        m_timerId = 0;
        imeDisableActiveMultitapWithoutDraw();
    }
    if (0 != m_SlTimerId)
    {
        MMI_active_multitap = -1;	
        removeTimer(m_SlTimerId);
        m_SlTimerId = 0;
    }

	EditView::onLoseFocus();
    if (m_bAutoOmitted){
        setOmittedStyle(true);
    }
	m_bSelect = false;
	
}

void SlEditView::SlKeyupNoDraw(void)
{
    if (0 != m_SlTimerId)
        removeTimer(m_SlTimerId);
    m_SlTimerId = registerTimer(1000, "SlEditView");
}

void SlEditView::setFont (Logfont *f)
{
    EditView::setFont(f);
}

void SlEditView::setFont (HTResId id)
{
    EditView::setFont(GetFontRes(id));
}

void SlEditView::setText(const char * str)
{
	unsigned int len,valid_len;
	char *pStr = (char *)str;
	len = strlen(str);
	valid_len = 0;

	if(m_IsCharCount)
	{
	    unsigned int maxcount  =0;
	    maxcount =  GetUTF8CharCount(str,strlen(str));
	    if(maxcount > m_txtLimit && m_txtLimit > 0)
	    {
	        len= GetUTF8LenByCharCount(str,m_txtLimit);
	    }
	}
	else
	{
	    if(len > m_txtLimit && m_txtLimit > 0)
		len = m_txtLimit;
	}
	while(valid_len <= len)
	{
		int ch_len = 1;
		char ch = *pStr;
		if(ch&0x80)
		{
			while(ch&(0x80 >> ch_len))
				ch_len++;
		}
		if(valid_len+ch_len > len)
		{
			break;
		}
		valid_len += ch_len;
		pStr += ch_len;
	}
	
	char *tmp = (char *)malloc((len+1) * sizeof(char));
	memset(tmp, 0, (len+1) * sizeof(char));
	
	strncpy(tmp, str, valid_len);
	
	m_strings = tmp;
	free(tmp);
	tmp = NULL;

#if 0
    if (m_txtLimit < 0 || strlen(str) <= m_txtLimit){
        m_strings = str;
    } else {
        char *tmp = (char *)malloc(m_txtLimit * sizeof(char));
        strncpy(tmp, str, m_txtLimit);
        m_strings = tmp;
        free(tmp);
    }
#endif
    m_caretPos = m_strings.size();

	if(m_style & EDVS_PSWD)
	{
		m_passwordStyleShow = true;
		m_char_pos = m_caretPos;
		m_careTick = 0;
	}
	#ifdef __MMI_T9__
	else
	{
	    int count =GetUTF8CharCount(m_strings.c_str(),m_caretPos);
	    imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
	}
	#endif

    SwitchShiftToLower(getText(),getTextLength(),true);	
    updateView();
}
    
void SlEditView::setText(string str)
{
    if(m_IsCharCount)
    {
        unsigned int maxcount  =0;
        maxcount =  GetUTF8CharCount(str.c_str(),str.size());
        if (m_txtLimit < 0 || maxcount <= m_txtLimit){
                m_strings = str;
            }else{
                int Len = GetUTF8LenByCharCount(str.c_str(),m_txtLimit);
                m_strings = str.substr(0, Len);
            }
    }
    else
    {
        if (m_txtLimit < 0 || str.size() <= m_txtLimit)
            m_strings = str;
        else 
            m_strings = str.substr(0, m_txtLimit);
    }	
    m_caretPos = m_strings.size();

	if(m_style & EDVS_PSWD)
	{
		m_passwordStyleShow = true;
		m_char_pos = m_caretPos;
		m_careTick = 0;
	}
	#ifdef __MMI_T9__
	else
	{
	    int count =GetUTF8CharCount(m_strings.c_str(),m_caretPos);
	    imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
	}
	#endif

    SwitchShiftToLower(getText(),getTextLength(),true);		
    updateView();
}
    
string SlEditView::getText()
{
    return m_strings;
}

void SlEditView::insertText(const char *text)
{
    unsigned int txtLen, insLen, currLen;
    unsigned int txtcount=0, strcount=0;
    if (text == NULL || (m_style & EDVS_READONLY))
        return;
	
    txtLen = strlen(text);
    currLen = m_strings.size();
    insLen = txtLen;

	if( (m_style & EDVS_PSWD) && (m_strings.size() < m_txtLimit))
	{
		m_passwordStyleShow = false;
		m_char_pos = m_caretPos;
		m_careTick = 0;
	}

    if(m_IsCharCount)
    {
        txtcount =  GetUTF8CharCount(text,txtLen);
        strcount =  GetUTF8CharCount(m_strings.c_str(),m_strings.length());
        if(m_txtLimit > 0  && (txtcount + strcount) > m_txtLimit)
        {
            txtcount = m_txtLimit - strcount;
        }
        insLen = GetUTF8LenByCharCount(text,txtcount);
    }
    else
    {
        if (m_txtLimit > 0 && currLen + insLen > m_txtLimit) {
            insLen = m_txtLimit - currLen;
        }
    }

    if (insLen > 1 || (insLen == 1 && isprint(*text))){
        char *src = (char *) m_strings.c_str();
        memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
            if (insLen < txtLen) {
                const char *p = text;
                unsigned int charLen;
                strncpy(m_edit_tempstr1, src, m_caretPos);
                while (insLen > 0 && p != NULL){
                    charLen = GetFirstUTF8CharLen(p, txtLen);
                    if (insLen > charLen) {
                        strncat(m_edit_tempstr1, p, charLen);
                        m_caretPos += charLen;
                    }
                    insLen -= charLen;
                    txtLen -= charLen;
                    p += charLen;
                }
                strncat(m_edit_tempstr1, src + m_caretPos, currLen - m_caretPos);
            } else {
                strncpy(m_edit_tempstr1, src, m_caretPos);
                strncat(m_edit_tempstr1, text, insLen);
                strncat(m_edit_tempstr1, src + m_caretPos, currLen - m_caretPos);
                m_caretPos += insLen;
            }
            m_strings = m_edit_tempstr1;

        if (m_caretPos > m_strings.size()){
            m_caretPos = m_strings.size();
        }
        showCaret(true);
        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
        updateView();
    }
}

void SlEditView::replacePrevChar(const char *ch)
{
    unsigned int totalLen = m_strings.size();
    unsigned int insLen = (ch != NULL ? strlen(ch) : 0);
    int bTail = (totalLen == m_caretPos);

    memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
    memset(m_edit_tempstr2,0,EDITOR_MAX_LEN);
    strcpy(m_edit_tempstr1, m_strings.c_str());
    
    if (!bTail) {
        strcpy(m_edit_tempstr2, m_edit_tempstr1 + m_caretPos);
        m_edit_tempstr1[m_caretPos] = '\0';
    }
    int lastChLen = GetLastUTF8CharLen(m_edit_tempstr1, strlen(m_edit_tempstr1));
        
    m_edit_tempstr1[m_caretPos - lastChLen] = '\0';

    if(m_IsCharCount)
    {
        unsigned int strcount =  GetUTF8CharCount(m_strings.c_str(),m_strings.size());
        unsigned int inscount = (ch != NULL ? GetUTF8CharCount(ch,strlen(ch)) : 0) ;
   
        if (m_txtLimit > 0 && strcount - 1 + inscount > m_txtLimit) {
            inscount = m_txtLimit -( strcount - 1);	
            insLen = GetUTF8LenByCharCount(ch,inscount);
        }
    }
    else
    {
        if (m_txtLimit > 0 && totalLen - lastChLen + insLen > m_txtLimit) {
            insLen = m_txtLimit - (totalLen - lastChLen);
        }
    }
    if (insLen > 1 || (insLen == 1 && isprint(*ch))){
        strncat(m_edit_tempstr1, ch, insLen);
    }

    m_strings = m_edit_tempstr1;
    
    if (!bTail) {
        m_strings += m_edit_tempstr2;
    }
    
    m_caretPos += insLen - lastChLen;
    if (m_caretPos > m_strings.size()){
        m_caretPos = m_strings.size();
    }

    if(m_caretPos == 0)
    {
       	switch(imeGetCurrentInputMode())
        {
            case INPUT_MODE_SMART_SHIFT_ABC:
            case INPUT_MODE_SMART_SHIFT_FRENCH:
#ifdef __MMI_T9__
                imeT9SetShifStateWithFieldInfo();
                imeSetSwitch();
#endif
                break;
            case INPUT_MODE_MULTITAP_SHIFT_FRENCH:
            case INPUT_MODE_MULTITAP_SHIFT_ABC:
#ifdef __MMI_T9__
                imeSetMultitapShiftMode(SHIFT_STATE_INIT);
                imeSetSwitch();
#endif
                break;
        } 
    }

    showCaret(true);
    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

void SlEditView::deleteAllCharacters(void)
{
	m_strings = "";
	m_caretPos = m_strings.length();
#ifdef __MMI_T9__
    if(imeGetInputModeFlag() == INPUT_MODE_SMART) {
        SynText();
    }
#endif

    if(m_caretPos == 0)
    {
       	switch(imeGetCurrentInputMode())
        {
            case INPUT_MODE_SMART_SHIFT_ABC:
            case INPUT_MODE_SMART_SHIFT_FRENCH:
#ifdef __MMI_T9__
                imeT9SetShifStateWithFieldInfo();
                imeSetSwitch();
#endif
                break;
            case INPUT_MODE_MULTITAP_SHIFT_FRENCH:
            case INPUT_MODE_MULTITAP_SHIFT_ABC:
#ifdef __MMI_T9__
                imeSetMultitapShiftMode(SHIFT_STATE_INIT);
                imeSetSwitch();
#endif
                break;
        } 
    }	
}
#ifdef __MMI_T9__
void SlEditView::insertStringFromT9(char *ch, int cursor)
{
    unsigned int len ;
    if(m_IsCharCount)
    {
        unsigned int maxcount  =0;
        maxcount =  GetUTF8CharCount(ch,strlen(ch));
        if(maxcount > m_txtLimit)
        {
            int Len = GetUTF8LenByCharCount(ch,m_txtLimit);
            memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
            strncpy(m_edit_tempstr1, ch,Len);
            int lastChLen = GetLastUTF8CharLen(m_edit_tempstr1, strlen(m_edit_tempstr1));   
            m_edit_tempstr1[Len - lastChLen] = '\0';
            m_strings = m_edit_tempstr1;
            cursor = m_strings.size();
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
            SynText();
        }
        else   
        {
            m_strings = ch;
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
        }
    }
    else
    {
        len= strlen(ch);
        if(len > m_txtLimit)
        {
            memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
            strncpy(m_edit_tempstr1, ch,m_txtLimit);
            int lastChLen = GetLastUTF8CharLen(m_edit_tempstr1, strlen(m_edit_tempstr1));   
            m_edit_tempstr1[m_txtLimit - lastChLen] = '\0';
            m_strings = m_edit_tempstr1;
            cursor = m_strings.size();
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
            SynText();
        }
        else
        {
            m_strings = ch;
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
        }
    }
    m_addword = false;
    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

void SlEditView::SynText(void)
{
	//m_caretPos = m_strings.length();
	m_bSelect = false;
	int count =GetUTF8CharCount(m_strings.c_str(),m_caretPos);
	imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
}

void SlEditView::DeleteChar(void)
{
    int count;
    replacePrevChar(NULL);
    showCaret(false);
    count =GetUTF8CharCount(m_strings.c_str(),m_caretPos);
    imeT9KeyRSKClearHandlerMultilineInputBox((char *)m_strings.c_str(),count);
    if(imeT9GetActiveWordCount() == 0)
        m_bSelect = false;
    imeRefreshString();
}

void SlEditView::ResetT9Mode(bool  Neednotifyevent)
{
	m_addword = false;
	m_bSelect = false;
	if(imeGetInputModeFlag() == INPUT_MODE_SMART)
	{
		int count =GetUTF8CharCount(m_strings.c_str(),m_caretPos);
		imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
	}
	if(Neednotifyevent)	
		raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
	updateView();
}    

void SlEditView::setCursorFromT9(void)
{
    int unicodeCursor = imeT9GetUnicodeCursor();
    const char *utf8S = m_strings.c_str();
    int utf8Len = m_strings.size();
    m_caretPos = 0;
    for (int i = 0; i < unicodeCursor; i++)
    {
        int f_w_len = GetFirstUTF8CharLen(utf8S, utf8Len);
        m_caretPos += f_w_len;
        utf8S += f_w_len;
        utf8Len -= f_w_len;
    }
    if(m_IsCharCount)
    {
        unsigned int maxcount = GetUTF8CharCount(m_strings.c_str(),m_caretPos);
        if(maxcount > m_txtLimit)
        {
            int Len = GetUTF8LenByCharCount(m_strings.c_str(),m_txtLimit);
            m_caretPos = Len;
        }
    }
    else
    {
        if(m_caretPos > m_txtLimit)
            m_caretPos = m_txtLimit;
    }
}

void SlEditView::entryAddWordMode(void)
{
    m_addword = true;
    setActiveWordCount(imeT9GetActiveWordCount());	
    m_inputmethod = imeGetCurrentInputMode();
    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

void SlEditView:: displayAddWord(char *ch, int* pos, int wordlen,string &str, bool internal)
{
    unsigned int totalLen = str.size();
    unsigned int insLen = (ch != NULL ? strlen(ch) : 0);
    unsigned int delcount = wordlen;	
    int bTail = ((int)totalLen == *pos);
    int deslen = 0;
    memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
    memset(m_edit_tempstr2,0,EDITOR_MAX_LEN);
    strcpy(m_edit_tempstr1, str.c_str());
    
    if (!bTail) {
        strcpy(m_edit_tempstr2, m_edit_tempstr1 + *pos);
        m_edit_tempstr1[*pos] = '\0';
    }
    while(wordlen)
    {
        int lastChLen = GetLastUTF8CharLen(m_edit_tempstr1, strlen(m_edit_tempstr1));
        deslen += lastChLen;
        m_edit_tempstr1[*pos - deslen] = '\0';	
        wordlen--;
    }
    m_edit_tempstr1[*pos - deslen] = '\0';

    if(m_IsCharCount)
    {
        unsigned int strcount =  GetUTF8CharCount(str.c_str(),str.size());
        unsigned int inscount = (ch != NULL ? GetUTF8CharCount(ch,strlen(ch)) : 0) ;
   
        if (m_txtLimit > 0 && strcount - delcount + inscount > m_txtLimit) {
            inscount = m_txtLimit -( strcount - delcount);	
            insLen = GetUTF8LenByCharCount(ch,inscount);
        }
    }
    else
    {
        if (m_txtLimit > 0 && totalLen - deslen + insLen > m_txtLimit) {
            insLen = m_txtLimit - (totalLen - deslen);
        }
    }
    if(imeIsSmartShiftInputMode())
    {
        if(m_edit_tempstr1[0]  == '\0'  || 
                (m_edit_tempstr1[*pos - deslen -1] == ' ' && (m_edit_tempstr1[*pos - deslen -2] == '.'  
                ||m_edit_tempstr1[*pos - deslen -2] == '!' || m_edit_tempstr1[*pos - deslen -2] == '?')))
                ch[0]= toupper (ch[0]);
    }	
    if (insLen > 1 || (insLen == 1 && isprint(*ch))){
        strncat(m_edit_tempstr1, ch, insLen);
    }

    str = m_edit_tempstr1;
    
    if (!bTail) {
        str += m_edit_tempstr2;
    }
    
    *pos += insLen - deslen;
    if (*pos > (int)str.size()){
        *pos = str.size();
    }

	if(internal) {
	    m_addword = false;
	    //m_bSelect = false;
	    SynText();
	    showCaret(true);
	    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
	    updateView();
	}
}

void SlEditView:: addWord(char *ch)
{
    int wordlen = getActiveWordCount();
	imeSetInputMode(m_inputmethod);
	imeChangeInputboxMode(m_inputmethod);
	if(ch==NULL)
	{
		SynText();
		m_addword = false;
		m_bSelect = false;
		raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
		updateView();
	}	
	else if(strlen(ch) ==0)
	{
		SynText();
		m_addword = false;
		//m_bSelect = false;
		raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
		updateView();
	}
	else
	{
		int len;
		char temp[102];
		displayAddWord(ch, (int*)&m_caretPos, wordlen, m_strings, true);
		len = NGUtf8ToUnicode((unsigned char*)ch,(unsigned char*)temp);
		imeT9AddCustWordIntoLDB((unsigned short *)temp,len/2-1);
	}
}

#endif
int SlEditView::getTextMCharLen(void)
{
    return GetUTF8CharInfo(m_strings.c_str(), m_strings.size(), NULL);
}
    
static inline char _charactor(int key_code)
{
    if (key_code == KeyEvent::KEYCODE_STAR){
        return '*';
    } else if (key_code == KeyEvent::KEYCODE_POUND){
        return '#';
    } else if (key_code >= KeyEvent::KEYCODE_0
            && key_code <= KeyEvent::KEYCODE_9) {
        return '0' + key_code;
    }
    return -1;
}

bool SlEditView::dispatchEvent(Event *event)
{
    int code = ((KeyEvent *)event)->keyCode();

    if(getExchangeFlag())
    { 
        if(code == KeyEvent::KEYCODE_CURSOR_LEFT)
            code = KeyEvent::KEYCODE_CURSOR_RIGHT;
        else if(code == KeyEvent::KEYCODE_CURSOR_RIGHT)
            code = KeyEvent::KEYCODE_CURSOR_LEFT;
    }

    if(event->eventType() == Event::KEY_DOWN)
    {
#ifdef __MMI_GB_V5__            
        char ch[2]={0, 0};
        ui_char_t temp;
        unsigned char utf8[10] = {0};

        if ((ch[0] = _charactor(code)) > 0){
            ch[1] = '\0';
	       if(imeGetInputModeFlag() == INPUT_MODE_SMART)
		{
		    if (isImeExist() && isImeEnable() && !isImeOpen()
			&& ch[0] >= '1' && ch[0] <= '9'){
			openImeWin();
			imeInsertChar(ch[0]);
		    } 
		}
		else if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP)
		{
			if(imeGetPreviousActiveMultitap() == code)
			{
				temp = (Uint16)getMultitapCharCode(code);
				mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
				replacePrevChar((const char *)utf8);
			}else
			{
				temp = (Uint16)getMultitapCharCode(code);
				mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
				insertText((const char *)utf8);
			}
		}
		else
		{
		    insertText(ch);
		}
            return DISPATCH_STOP_MSG;
        }
#endif
#ifdef __MMI_T9__
        if(imeGetInputModeFlag() == INPUT_MODE_NUMBER && code >= KeyEvent::KEYCODE_0 && code <= KeyEvent::KEYCODE_STAR)
        {
            char ch[2]={0, 0};
            m_bSelect = false;
            ch[0] = _charactor(code);
            ch[1] = '\0';
            insertText(ch);
        }
#endif
        m_prevEventType = event->eventType();

        if (0 != m_SlTimerId && _charactor(code) == -1)
        {
            removeTimer(m_SlTimerId);
            imeDisableActiveMultitapWithoutDraw();
            m_SlTimerId = 0;
            blinkCaret();
            if(code == KeyEvent::KEYCODE_CURSOR_RIGHT)
                return DISPATCH_STOP_MSG;
        } 

        switch(code)
        {
            case KeyEvent::KEYCODE_SOFTKEY_LEFT :
                // Left Soft Key .. Do Nothings ??
                if(getTextHighlight())
                    setTextHighlight(false); 
                break;
            case KeyEvent::KEYCODE_SOFTKEY_RIGHT :
                if (m_strings.size() > 0) {
                    if(getTextHighlight())
                    {  
                        setTextHighlight(false);
                        deleteAllCharacters();
                        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
                        updateView();
                        return DISPATCH_STOP_MSG;
                    }
                    else
                    {
#ifdef __MMI_T9__
                    if(imeGetInputModeFlag() == INPUT_MODE_SMART)	
                    {
                        if(m_caretPos <0)
                            m_caretPos = 0;
                        else
                            DeleteChar();
                        if(m_bSelect == false)
                            blinkCaret();
                    }
                    else
                    {
                        replacePrevChar(NULL);
                    }
                    return DISPATCH_STOP_MSG;
#else //__MMI_T9__
                    replacePrevChar(NULL);
                    return DISPATCH_STOP_MSG;
#endif //__MMI_T9__
                    }
                }
                return DISPATCH_CONTINUE_MSG;
            case KeyEvent::KEYCODE_CURSOR_LEFT :
                if(getTextHighlight())
                {
                    setTextHighlight(false);
                    m_caretPos = 0;
                    showCaret(true);
                    updateView();
                    return DISPATCH_STOP_MSG;
                }
#ifdef __MMI_T9__
                if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                {
                    bool preT9ShiftMode = imeIsT9ShiftMode();

                    m_bSelect = false;
                    imeT9KeyArrowLeftHandlerMultilineInputBox();
                    setCursorFromT9();
                    showCaret(true);
                    if(m_addword == true)
                    {
                        m_addword = false;
                        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
                    }
                    else if(preT9ShiftMode != imeIsT9ShiftMode())
                    {
                        imeSetSwitch(); 
                    }
                    updateView();
                    return DISPATCH_STOP_MSG;
                }
                else
#endif	//__MMI_T9__		
                if (m_caretPos > 0) {
                    int lastChLen = GetLastUTF8CharLen(m_strings.c_str(), m_caretPos);
                    m_caretPos -= lastChLen;
                    showCaret(true);
                    updateView();
                    return DISPATCH_STOP_MSG;
                }
                return DISPATCH_CONTINUE_MSG;
            case KeyEvent::KEYCODE_CURSOR_RIGHT :
                if(getTextHighlight())
                {
                    setTextHighlight(false);
                    showCaret(true);
                    updateView();
                    return DISPATCH_STOP_MSG;
                }
#ifdef __MMI_T9__
                if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                {
                    bool max_check = TRUE;
                    m_bSelect = false;

                    if(m_IsCharCount)
                    {
                        unsigned int strcount  =0;
                        strcount =  GetUTF8CharCount(m_strings.c_str(),m_caretPos);
                        if(strcount >= m_txtLimit)	
                            max_check = FALSE;
                    }
                    else
                    {
                        if(m_caretPos >= m_txtLimit)
                            max_check = FALSE;
                    }
                    if (max_check)
                    {
                        bool preT9ShiftMode = imeIsT9ShiftMode();

                        imeT9KeyArrowRightHandlerMultilineInputBox();
                        imeRefreshString();
                        setCursorFromT9();
                        showCaret(true);
                        if(m_addword == true)
                        {
                            m_addword = false;
                            raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
                        }
                        else if(preT9ShiftMode != imeIsT9ShiftMode())
                        {
                            imeSetSwitch();
                        }
                        updateView();
                    }
                    return DISPATCH_STOP_MSG;
                }
                else
#endif	//__MMI_T9__
                if (!(m_style & EDVS_PSWD) && !m_AddWord && m_bInsertSpace && m_caretPos == getTextLength()){
                    insertText(" ");
                    return DISPATCH_STOP_MSG;
                } else if (m_caretPos < getTextLength()) {
                    string sub = m_strings.substr(m_caretPos, -1);
                    int firstChLen = GetFirstUTF8CharLen(sub.c_str(), getTextLength() - m_caretPos);
                    m_caretPos += firstChLen; 
                    showCaret(true);
                    updateView();
                    return DISPATCH_STOP_MSG;
                }
                return DISPATCH_CONTINUE_MSG;
            case KeyEvent::KEYCODE_CURSOR_UP :
#ifdef __MMI_T9__
                if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                {
                    m_addword = false;
                    if(m_bSelect)
                    {
                        imeT9KeyArrowUpHandlerMultilineInputBox(m_caretPos);
                        imeRefreshString();
                        return DISPATCH_STOP_MSG;
                    }
                }
#endif	//__MMI_T9__		

            case KeyEvent::KEYCODE_CURSOR_DOWN :
#ifdef __MMI_T9__ 
                if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                {
                    m_addword = false;
                    if(m_bSelect)
                    {
                        imeT9KeyArrowDownHandlerMultilineInputBox(m_caretPos);
                        imeRefreshString();

                        if(imeT9IsAddwordByArrowkey())
                            entryAddWordMode();                        
                        return DISPATCH_STOP_MSG;
                    }
                }
#endif	//__MMI_T9__		
            default :
                break;
        }
    }
    else if(event->eventType() == Event::KEY_UP)
    {
#ifdef __MMI_T9__
        char ch[2]={0, 0};
        ui_char_t temp;
        unsigned char utf8[10] = {0};

		if ((ch[0] = _charactor(code)) > 0){
            if(m_prevEventType == Event::KEY_LONGPRESSED)
                return DISPATCH_CONTINUE_MSG;

            ch[1] = '\0';

            if(getTextHighlight() && code >= KeyEvent::KEYCODE_0 && code <= KeyEvent::KEYCODE_9)
            {
                setTextHighlight(false);
                deleteAllCharacters();
            }
            if(imeGetInputModeFlag() == INPUT_MODE_SMART && ch[0] != '0')
            {
                ch[0] = '0' + code;

                if(imeGetPreviousActiveMultitap() >= 0)
                {
                    imeDisableActiveMultitapWithoutDraw();
                    SynText();
                }
                
                if (isImeExist() && isImeEnable()&&
                     ch[0] >= '1' && ch[0] <= '9')
                {
                    if(m_IsCharCount)
                    {
                        unsigned int maxcount =  GetUTF8CharCount(m_strings.c_str(),m_strings.size());
                        if(maxcount >=m_txtLimit)
                            return DISPATCH_STOP_MSG;
                    }
                    else
                    {
                        if(getTextLength()>=m_txtLimit)
                            return DISPATCH_STOP_MSG;
                    }
                    if(m_bSelect == false)
                    {
                        m_bSelect = true;
                        m_startPosX = m_caretPos;
                    }
                    showCaret(false);		
                    setImeTarget((int)this, 1);
                    imeInsertChar(ch[0]);
                } 
            }
            else if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP
                    || (imeGetInputModeFlag() == INPUT_MODE_SMART && ch[0] == '0'))                
            {
                m_bSelect = false;

                if(m_AddWord && (ch[0] == '0' || ch[0] == '1'))
                {
                    ch[0] = _charactor(code);
                    ch[1] = '\0';
                    insertText(ch);
                }
                else
                {
                    if(imeGetPreviousActiveMultitap() == code)
                    {
                        temp =(Uint16)getMultitapCharCode(code);
                        imeCheckMultitapPunctuationCharacter(m_strings,m_caretPos,true,isSpaceMode());		

                        if(imeIsMultiShiftInputMode() &&
                            ((imeGetMultitapShiftMode()!= SHIFT_STATE_LOWER) || (imeGetPreMultitapShiftMode() == SHIFT_STATE_UPPER)))
                        {
                            temp=toupper(temp);	
                        }	
                        mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
                        replacePrevChar((char *)utf8);
                    }
                    else if(m_strings.length() < textMaxLimit() 
						|| (m_IsCharCount && (unsigned int)GetUTF8CharCount(m_strings.c_str(),m_strings.size()) < textMaxLimit()) )
                    {
                        if(ch[0] == '0') 
                        {
                           m_addword = false;
                           imeChangeKeyzeroMultitaps(false,code);
                        }
                        temp = (Uint16)getMultitapCharCode(code);
                        imeSetPreMultitapShiftMode(SHIFT_STATE_INIT);		
                        imeCheckMultitapPunctuationCharacter(m_strings,m_caretPos,false,isSpaceMode());			
                        if(imeIsMultiShiftInputMode() &&(imeGetMultitapShiftMode()!=SHIFT_STATE_LOWER))
                        {
                            temp=toupper(temp);
                            imeSetMultitapShiftMode(SHIFT_STATE_UPPER);
                            imeSetPreMultitapShiftMode(imeGetMultitapShiftMode());			
                        }
                        mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
                        insertText((char *)utf8);
                    }
                    else if(m_SlTimerId != 0)
                    {
                        removeTimer(m_SlTimerId);
                        imeDisableActiveMultitapWithoutDraw();
                        m_SlTimerId = 0;
                        blinkCaret();
                    }                    
                }
                if(imeGetInputModeFlag() == INPUT_MODE_SMART && ch[0] == '0')
                    SynText();
            }
#ifdef __MMI_T9__
            else if(imeGetInputModeFlag() == INPUT_MODE_NUMBER && code >= KeyEvent::KEYCODE_0 && code <= KeyEvent::KEYCODE_STAR)
            {
               ;
            }
#endif            
            else
            {
                m_bSelect = false;
                ch[0] = _charactor(code);
                ch[1] = '\0';
                insertText(ch);
            }
		}				
#endif        
	    if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP
             || (imeGetInputModeFlag() == INPUT_MODE_SMART && code ==  KeyEvent::KEYCODE_0))
		{
			if((code >=  KeyEvent::KEYCODE_0) &&  (code<= KeyEvent::KEYCODE_9))
			{
			    SlKeyupNoDraw();
			}
		}

        if(((code >=  KeyEvent::KEYCODE_0) && (code<= KeyEvent::KEYCODE_9))
            || code == KeyEvent::KEYCODE_CURSOR_LEFT
            || code == KeyEvent::KEYCODE_CURSOR_RIGHT
            || code == KeyEvent::KEYCODE_SOFTKEY_RIGHT )
        {
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP)
                imeCheckMultitapPunctuationCharacter(m_strings,m_caretPos,false,isSpaceMode());
            else if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                imeT9CheckPunctuationCharacter(isSpaceMode());
            imeSetSwitch();
#endif
        }        
    }
    else if(event->eventType() == Event::KEY_LONGPRESSED)
    {
        m_prevEventType = event->eventType();
        
        if (code == KeyEvent::KEYCODE_0 && m_bAutoTrans) {
            if(getText().find('+') == string::npos)
                replacePrevChar("+");
            return DISPATCH_STOP_MSG;
        } else if (code == KeyEvent::KEYCODE_STAR && m_bAutoTrans) {
            replacePrevChar("P");
            return DISPATCH_STOP_MSG;
        } else if (code == KeyEvent::KEYCODE_CURSOR_LEFT) {
            setCaretPosition(0);
            showCaret(true);
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                    SynText();
            }
#endif	//__MMI_T9__
            updateView();
            return DISPATCH_STOP_MSG;
        } else if (code == KeyEvent::KEYCODE_CURSOR_RIGHT) {
            if(getTextHighlight())
                setTextHighlight(false);
            setCaretPosition(getTextLength());
            showCaret(true);
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                    SynText();
            }
#endif	//__MMI_T9__
            updateView();
            return DISPATCH_STOP_MSG;
        }
        else if(code >= KeyEvent::KEYCODE_0 && code <= KeyEvent::KEYCODE_9 && imeGetInputModeFlag() != INPUT_MODE_NUMBER)
        {
            char ch[2]={0, 0};
            m_bSelect = false;
            ch[0] = _charactor(code);
            insertText(ch);
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                m_bSelect = false;
                SynText();
            }
            else if (0 != m_SlTimerId)
            {
                imeDisableActiveMultitapWithoutDraw();
                removeTimer(m_SlTimerId);
                m_SlTimerId = 0;
                blinkCaret();
            }
#endif       
            return DISPATCH_STOP_MSG;
        }
#ifdef __SUPPORT_MANUAL_KEYPAD_LOCK_IN_ALLSCREEN__
        else if(code == KeyEvent::KEYCODE_STAR)
        {
            return DISPATCH_STOP_MSG;
        }
#endif
        return DISPATCH_CONTINUE_MSG;
    }
    else if(event->eventType() == Event::KEY_ALWAYSPRESS)
    {
        if (((KeyEvent *)event)->keyCode() == KeyEvent::KEYCODE_SOFTKEY_RIGHT
                && m_strings.size() > 0) {
            if(imeGetInputModeFlag() == INPUT_MODE_SMART){
                if(m_caretPos < 0)
                    m_caretPos = 0;
#ifdef __MMI_T9__
                else			
                    DeleteChar();
#endif
            }else{
                replacePrevChar(NULL);
            }
            return DISPATCH_STOP_MSG;
        }
        return DISPATCH_CONTINUE_MSG;
    }
    else if(event->eventType() == Event::MOTION_DOWN)
    {
        //TODO mouse support
    }
    return DISPATCH_CONTINUE_MSG;
}

void SlEditView::drawBackground(GraphicsContext* gc, IntRect &rc, int status)
{
	if(m_drset) {
		m_drset->draw(gc, DR_BKGND, isFocus() ? DRAWSTATE_HILIGHT : DRAWSTATE_NORMAL, rc);
	}
}

int SlEditView::CalculateCursor(GraphicsContext *gc,IntRect rc, string tmpStr, int  caretPos, int *caretWidth)
{
    int subw, subh;
   if (caretPos > 0) {
        Uint16 *temp_unicode_str = NULL;
        Uint16 *bidiStrOrder = NULL;
        Uint16 visIndex, unicodeCaretPos;
        Uint16 tmpChar[2] = {0,};
        int w;
        int visual_str_len = GetUTF8CharCount(tmpStr.c_str(), tmpStr.length());
        string tempStr = tmpStr;

        temp_unicode_str = (Uint16*)malloc((visual_str_len+1) * sizeof(Uint16));
        bidiStrOrder = (Uint16*)malloc((visual_str_len*2+1) * sizeof(Uint16));

        NGUtf8ToUnicode((unsigned char*)tempStr.c_str(), (unsigned char *)temp_unicode_str);
        ConvertToGlyphForm(temp_unicode_str, visual_str_len, 0);
        getBidiString(temp_unicode_str, bidiStrOrder, visual_str_len, gc->getBiDiFlag(), TRUE);        
        
        unicodeCaretPos = GetUTF8CharCount(tempStr.c_str(), caretPos);
        if(getTextHighlight())
        {
            if(gc->getBiDiFlag())
                visIndex = 0;
            else
                visIndex = unicodeCaretPos;
        }
        else
        {
            visIndex = getBidiVisualIndex(temp_unicode_str, bidiStrOrder, visual_str_len, unicodeCaretPos, gc->getBiDiFlag(), (MMI_active_multitap<0)?2:0);
        }

        subw = 0;

        if(gc->getBiDiFlag()) {
            gc->getTextDrawSize(temp_unicode_str, m_font, &w, &subh);
            subw += rc.right() - rc.left() - w;
        }
        
        if(MMI_active_multitap >= 0)
        {
            tmpChar[0] = temp_unicode_str[unicodeCaretPos-1];
            gc->getTextDrawSize(tmpChar, m_font, caretWidth, &subh);
        }

        for(int i = 0; i < visIndex; i++)
        {
            tmpChar[0] = temp_unicode_str[bidiStrOrder[i]-1];
            gc->getTextDrawSize(tmpChar, m_font, &w, &subh);
            subw += w;
        }
        
        free(temp_unicode_str);
        free(bidiStrOrder);
    }
    else if (gc->getBiDiFlag()) {
        subw = rc.right() - rc.left() - 1;
    }
    else {
        subw = 0;
    }

    return subw;	
}

void SlEditView::innerDrawText(GraphicsContext *gc, const string text, const IntRect &rc, unsigned int color, Logfont *f, unsigned int format)
{
    int subw, subh, strw, strh, sw;;
    int caretWidth = 0;
    int tmpcur =0;	
    string tmpStr(text);

    int len = m_strings.size();	
    int *info = (int *)malloc ((len + 1) * sizeof(int));

#ifdef __MMI_T9_ARABIC__	
    const char *str = m_strings.c_str();
    char tmpchar[8]={0};
    int realCount = GetUTF8CharInfo (m_strings.c_str(), len, info);	

    info [realCount] = len; 
    for (int i = 1; i <= realCount; /*i++*/)
    {
        memcpy(tmpchar, str + info[i-1], info[i] - info[i-1]);
        tmpchar [info[i] - info[i-1]] = '\0';
        if(IsSymbolUCS2Char(UTF8ToUCS2((Uint8 *)tmpchar)) ||IsNumberUCS2Char(UTF8ToUCS2((Uint8 *)tmpchar)))
        {
            i++;
        }
        else
        {  
            break;
        }			
    }

    if(isTransformArabic(UTF8ToUCS2((Uint8 *)tmpchar))||IsArabicSymbolUCS2Char(UTF8ToUCS2((Uint8 *)tmpchar)))
    {
        SetBiDiFlag(true);
        setExchangeFlag(true);
    }
    else
    {
        SetBiDiFlag(false);
        setExchangeFlag(false);
    }
#endif

    string sub = text.substr(0, m_caretPos);
    gc->getTextDrawSize (sub, f, &sw, &subh);

    if ((sw >= 0 && sw <= rc.width()) || (m_caretPos == 0) ) {
        subw = CalculateCursor(gc, rc,tmpStr, m_caretPos, &caretWidth);	
        gc->getTextDrawSize (tmpStr, f, &strw, &strh);

        if(strw> rc.width()) {
            int len=0;
            char *str = (char *)tmpStr.c_str() ;
            char tmp[100] = {0};
            memset(tmp,0,100);
            while ( str != NULL){
                int _w, _h;
                int charLen = GetFirstUTF8CharLen((const char *)str, strlen((char *)str));
                if (charLen > 0) {
                    strncat(tmp, (char *)str, charLen);
                    gc->getTextDrawSize(tmp, f, &_w, &_h);
                    if (_w > rc.width()) {
                        *(tmp + len) = '\0';
                        break;
                    }
                }
                str += charLen;
                len += charLen;
            }
            tmpStr = tmp;
        }

        gc->drawText (tmpStr, rc, color, f, format); 

    } else {

        unsigned int add_charLen=0,charLen =0,count=0;      
        char sub0[100]={0};   
        memset(sub0,0,100);
        strncpy(sub0, tmpStr.c_str(),m_caretPos);
        char *str = sub0;
        memset(info, 0, (len + 1) * sizeof(int));

        while ( str != NULL){
            int charLen = GetFirstUTF8CharLen((const char *)str, strlen((char *)str));
            info[count] = charLen;
            str = str+charLen;
            if(*str == 0)
                break;
            count++;
        }		
        while (1){
            int _w, _h;			
            charLen = info[count--];
            if (charLen <=  0)
            {
                break;
            }
            str -= charLen;
            add_charLen = add_charLen + charLen;

            if (charLen > 0) {
                gc->getTextDrawSize(str, f, &_w, &_h);
                if (_w  > rc.width()) {
                    add_charLen = add_charLen - charLen;
                    str = str + charLen;	
                    tmpcur = add_charLen;					
                    break;
                }
                else if(_w  == rc.width())
                {
                    tmpcur = add_charLen;
                    break;
                }
            }
            if(add_charLen > m_caretPos )
            {
                tmpcur = m_caretPos;
                break;
            }

        }
        if(strlen(str) !=0)	
            tmpStr = str;	
        subw = CalculateCursor(gc, rc,tmpStr,tmpcur,&caretWidth);	
        gc->drawText (tmpStr, rc, color, f, format);
    }

#ifdef __MMI_T9__
    if(m_bSelect && ((int)m_caretPos != m_startPosX))
    {
        int startx = 0;
        int w, h;
        int caretWidth = 0;
        string subCaretStr;
        int oldrop;
        oldrop = SetRasterOperation (gc->context(), ROP_XOR);

        if(m_startPosX == 0)
        {
            startx = rc.left();
            subCaretStr = text.substr(m_startPosX, m_caretPos - m_startPosX); 
            gc->getTextDrawSize(subCaretStr, m_font, &w, &h);
            caretWidth = w;	
        }
        else
        {

            if (sw <= rc.width()) {
                subCaretStr  = text.substr(0, m_startPosX);
                gc->getTextDrawSize(subCaretStr, m_font, &w, &h);		
                startx=  rc.left() + w;
                subCaretStr = text.substr(m_startPosX, m_caretPos - m_startPosX); 
                gc->getTextDrawSize(subCaretStr, m_font, &w, &h);
                caretWidth = w;				
            }else{
                int tmpstartx=0;
                if ((int)m_caretPos - tmpcur >= m_startPosX)
                    tmpstartx = 0;		
                else
                    tmpstartx = m_startPosX -(m_caretPos -tmpcur); 

                subCaretStr  = tmpStr.substr(0, tmpstartx);
                gc->getTextDrawSize(subCaretStr, m_font, &w, &h);			
                startx=rc.left() + w;
                subCaretStr = tmpStr.substr(tmpstartx, tmpcur - tmpstartx); 
                gc->getTextDrawSize(subCaretStr, m_font, &w, &h);
                caretWidth = w;		
            }
        }

        if (gc->getBiDiFlag() && !(m_style & EDVS_PSWD)) 
        {
            startx = rc.right() - (startx - rc.left());
            m_caretRect.setRect (startx -caretWidth , rc.top(), 
                    startx, rc.top() + h);
        }
        else
        {
            m_caretRect.setRect (startx, rc.top(), 
                    startx+ caretWidth, rc.top() + h);
        }
        gc->fillRect(m_caretRect, 
                (m_txtColor >> 16) & 0xFF, 
                (m_txtColor >> 8) & 0xFF, 
                (m_txtColor >> 0) & 0xFF, 
                (m_txtColor >> 24) & 0xFF);

        SetRasterOperation (gc->context(), oldrop); 
    }
    else
#endif	//__MMI_T9__
        // draw caret 
        if (isFocus() && m_bCaretShown){
            int startx = 0;
            int starty = 0;
            //int careth = f != NULL ? f->size : gc->getFontHeight(m_font);
            int careth = gc->getFontHeight(m_font);
            int oldrop;
            oldrop = SetRasterOperation (gc->context(), ROP_XOR);          

            if (sw >= rc.width()) {
                startx = rc.left();
            } else {
                switch(m_align & TextAlignMask)
                {
                    case TextAlignRight:
                        startx = rc.left() + (rc.width() - sw);
                        break;
                    case TextAlignCenter:
                        startx = rc.left() + ((rc.width() - sw)>>1);
                        break;
                    case TextAlignLeft:
                    default:
                        startx = rc.left();
                        break;
                } 
            }

            switch(m_align & TextVAlignMask)
            {
                case TextVAlignBottom:
                    starty = rc.bottom() - careth;
                    break;
                case TextVAlignTop:
                    starty = rc.top();
                    break;
                case TextVAlignCenter:
                default:
                    starty = rc.top() + ((rc.height() - careth)>>1);
                    break;
            }

            if(getTextHighlight())
            {
                if (gc->getBiDiFlag() && !(m_style & EDVS_PSWD)) 
                {
                    m_caretRect.setRect (subw, starty, 
                            rc.right(), starty + careth);
                }
                else
                {
                    m_caretRect.setRect (rc.left(), starty, 
                            rc.left() + subw, starty + careth);
                }
            }
            else
            {
                if(caretWidth == 0)
                {
                    if(m_caretPos != getTextLength()) // P130903-06868
                        startx--;
                    caretWidth++;
                }

                if (gc->getBiDiFlag() && !(m_style & EDVS_PSWD)) 
                {
                    m_caretRect.setRect (startx + subw, starty, 
                            startx + subw + caretWidth, starty + careth);
                }
                else
                {
                    m_caretRect.setRect (startx + subw, starty, 
                            startx + subw + caretWidth, starty + careth);
                }
            }	
            gc->fillRect(m_caretRect, 
                    (m_txtColor >> 16) & 0xFF, 
                    (m_txtColor >> 8) & 0xFF, 
                    (m_txtColor >> 0) & 0xFF, 
                    (m_txtColor >> 24) & 0xFF);

            SetRasterOperation (gc->context(), oldrop);
        }
#ifdef __MMI_T9_ARABIC__
    free(info);
    info=NULL;
    SetBiDiFlag(IsArabicSet());
#endif

}

void SlEditView::drawContent(GraphicsContext* gc, IntRect &rc, int status)
{
    unsigned int color = m_txtColor;
    unsigned int format = DT_SINGLELINE;
    IntRect irc ( rc.left() + m_hMargin, rc.top() + m_vMargin,
            rc.right() - m_hMargin, rc.bottom() - m_vMargin);

	if(m_style & EDVS_CHAR_BREAK)
	{
		format |= DT_CHARBREAK;
	}

    if (m_style & EDVS_BASELINE)
    {
        //FIXME , top vcernter bottom ??
        int y = irc.bottom();
        gc->drawHVDotLine(irc.left(), y, irc.right(), y, 
                (m_txtColor >> 16) & 0xFF, 
                (m_txtColor >> 8) & 0xFF, 
                (m_txtColor >> 0) & 0xFF, 
                (m_txtColor >> 24) & 0xFF);
    }

    // draw tip strings
    if (!isFocus() && m_strings.size() == 0 && m_tipText.size() > 0){
       gc->drawText(m_tipText, irc, color, m_font, DT_LEFT);
       return;
    }

    switch (m_align & TextAlignMask)
    {
        case TextAlignLeft:   format |= DT_LEFT;   break;
        case TextAlignCenter: format |= DT_CENTER; break;
        case TextAlignRight:  format |= DT_RIGHT;  break;
    }
    
    switch (m_align & TextVAlignMask)
    {
        case TextVAlignTop:     format |= DT_TOP;     break;
        case TextVAlignCenter:  format |= DT_VCENTER; break;
        case TextVAlignBottom:  format |= DT_BOTTOM;  break;
    }

    // draw content text string 
    if (m_strings.size() > 0) {
        if ((m_style & EDVS_PSWD) && m_pswdChar){
			
            unsigned int len = m_strings.size();
            memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
			
			if(m_passwordStyleShow) {
				m_careTick = 0;
				memset(m_edit_tempstr1, m_pswdChar, len);
			}
			else {
				if(++m_careTick >= 2) {
					m_careTick = 0;
					m_passwordStyleShow = true;
				}
				string str = m_strings;
				char* ptr = (char*)str.c_str();
	            memset(m_edit_tempstr1, m_pswdChar, len);
				if(m_char_pos < len)
					m_edit_tempstr1[m_char_pos]  = *(ptr+m_char_pos);
			}
            m_edit_tempstr1[len] = '\0';
            innerDrawText(gc, string(m_edit_tempstr1), irc, color, m_font, format);
        }
        else if (m_style & EDVS_OMITTED) {
            gc->drawText(m_strings, irc, color, m_font, format | TextMode::TextOutOmitted);
        }
        else {
            innerDrawText(gc, m_strings, irc, color, m_font, format);
        }
    }
    else if (isFocus() && m_bCaretShown) {
        int startx = 0;
        int starty = 0;
        //int careth = m_font != NULL ? m_font->size : gc->getFontHeight(m_font);
        int careth = gc->getFontHeight(m_font);

        switch (m_align & TextAlignMask) {
            case TextAlignLeft:   
                startx = irc.left();   
                break;
            case TextAlignCenter: 
                startx = (irc.left() + irc.right()) >> 1;   
                break;
            case TextAlignRight:  
                startx = irc.right() - 2;   
                break;
        }
        
        switch(m_align & TextVAlignMask) {
            case TextVAlignBottom:
                starty = irc.bottom() - careth;
                break;
            case TextVAlignTop:
                starty = irc.top();
                break;
            case TextVAlignCenter:
            default:
                starty = irc.top() + ((irc.height() - careth)>>1);
                break;
        }

		if (imeIsArabic() && !(m_style & EDVS_PSWD)) {
			startx = irc.right() - (startx - irc.left()) - 1;
		}

        m_caretRect.setRect(startx, starty, startx + 1, starty + careth);
        
        gc->fillRect(m_caretRect, 
                (m_txtColor >> 16) & 0xFF, 
                (m_txtColor >> 8) & 0xFF, 
                (m_txtColor >> 0) & 0xFF, 
                (m_txtColor >> 24) & 0xFF);
    }

#ifdef __MMI_T9_ARABIC__	
    gc->setBiDiFlag (IsArabicSet());
#endif
}

bool SlEditView::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER 
            && m_timerId == ((TimerEvent *)event)->timerID()
            && isFocus())   
    {
        if(!getTextHighlight() && m_SlTimerId == 0)
            blinkCaret();
        //lionel remove to fixed the blink bug;
		//updateView();
    }
    if (event->eventType() == Event::TIMER 
        && m_SlTimerId == ((TimerEvent *)event)->timerID())   
    {
        imeDisableActiveMultitapWithoutDraw();
        if(imeGetPreMultitapShiftMode() == SHIFT_STATE_UPPER)
        {
            imeSetPreMultitapShiftMode(SHIFT_STATE_INIT);
#ifdef __MMI_T9__
            imeSetSwitch();	
#endif
        }	
            
        removeTimer(m_SlTimerId);
        m_SlTimerId = 0;
        blinkCaret();
    }
	return true;
}

} // namespace hfcl

