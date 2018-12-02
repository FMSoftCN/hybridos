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


#ifndef _NGUX_SLEDITVIEW_H_
#define _NGUX_SLEDITVIEW_H_

#include "editview.h"
#include "graphicscontext.h"
#include "timerservice.h"

NAMESPACE_BEGIN


class SlEditView : public EditView, public TimerEventListener
{
	DECLARE_CLASS_NAME(SlEditView)
public:
	SlEditView(View *p);
	SlEditView(View *p, DrawableSet* drset);
	~SlEditView();
	
    void setText(NGCPStr str);
	void setText(string str);
	string getText();
	NGUInt getTextLength() { return m_strings.size(); }
	void insertText(const char *text);
	void replacePrevChar(const char *ch);
	void deleteAllCharacters(void);
	void setTextHighlight(NGBool IsHighlight = false) {  m_highlight = IsHighlight; }
	NGBool getTextHighlight(void) { return m_highlight; }

    enum {
        EDVS_PSWD      = 0x0100,
        EDVS_SPACE  = 0x0200,	
    };
    void setPasswordStyle(NGBool bSet)  { bSet ? m_style |= EDVS_PSWD : m_style &= ~EDVS_PSWD; }
	void setCharBreakStyle(NGBool bSet)  { bSet ? m_style |= EDVS_CHAR_BREAK : m_style &= ~EDVS_CHAR_BREAK; }
    void setSpaceStyle(NGBool bSet)  { bSet ? m_style |= EDVS_SPACE : m_style &= ~EDVS_SPACE; }
    NGBool isSpaceMode(void)   { return m_style & EDVS_SPACE; }

    #ifdef __MMI_T9__
    void setAddWordStyle(NGBool bAddWord)  { m_AddWord = bAddWord;}
    #endif
    void setPasswordChar (char pswch) { m_pswdChar = pswch; }
    char passwordChar (void) { return m_pswdChar; }

    NGInt setCaretPosition(NGInt pos) { NGInt slen = m_strings.size(); (pos < slen)? (m_caretPos = pos) : m_caretPos = slen; return 0; }
	NGInt caretPosition(void) { return m_caretPos; }

    NGInt getTextMCharLen(void);

	void onGetFocus(void);
	void onLoseFocus(void);
	void setFont(Logfont *f);
	void setFont (ResID id);
	void SlKeyupNoDraw(void);
	NGBool dispatchEvent(Event *event);
	void drawBackground(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);
	void drawContent(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);
#ifdef __MMI_T9__	
	void insertStringFromT9(char *ch, int cursor);
	void entryAddWordMode(void);
	void setCursorFromT9(void);
	void SynText(void);
	void DeleteChar(void);
	void addWord(char *ch);
	void displayAddWord(char *ch, NGInt* pos, NGInt wordlen,string &str, NGBool internal);
	void ResetT9Mode(NGBool  Neednotifyevent = FALSE);
#endif	

private:
    void innerDrawText(GraphicsContext *gc, const string s, const IntRect &rc, NGUInt color, Logfont *f, NGUInt format);
    NGBool handleEvent(Event* event);
    int CalculateCursor(GraphicsContext *gc,IntRect rc,string tmpStr ,NGInt  caretPos ,NGInt  *caretWidth);

private:
    NGInt   m_timerId;
    string  m_strings;
    NGChar  m_pswdChar;
    NGUInt  m_caretPos;
    NGInt   m_SlTimerId;
	NGBool  m_passwordStyleShow;
	NGUInt  m_careTick;
	NGUInt  m_char_pos;
    NGBool  m_AddWord;
    #ifdef __MMI_T9__
    NGInt   m_startPosX;
    #endif
    NGUInt  m_prevEventType; 
    NGBool  m_highlight;
};

NAMESPACE_END

#endif // _NGUX_SLEDITVIEW_H_


