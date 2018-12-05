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


#ifndef _HFCL_SLEDITVIEW_H_
#define _HFCL_SLEDITVIEW_H_

#include "editview.h"
#include "graphics/graphicscontext.h"
#include "services/timerservice.h"

namespace hfcl {


class SlEditView : public EditView, public TimerEventListener
{
	DECLARE_CLASS_NAME(SlEditView)
public:
	SlEditView(View *p);
	SlEditView(View *p, DrawableSet* drset);
	~SlEditView();
	
    void setText(const char * str);
	void setText(string str);
	string getText();
	unsigned int getTextLength() { return m_strings.size(); }
	void insertText(const char *text);
	void replacePrevChar(const char *ch);
	void deleteAllCharacters(void);
	void setTextHighlight(bool IsHighlight = false) {  m_highlight = IsHighlight; }
	bool getTextHighlight(void) { return m_highlight; }

    enum {
        EDVS_PSWD      = 0x0100,
        EDVS_SPACE  = 0x0200,	
    };
    void setPasswordStyle(bool bSet)  { bSet ? m_style |= EDVS_PSWD : m_style &= ~EDVS_PSWD; }
	void setCharBreakStyle(bool bSet)  { bSet ? m_style |= EDVS_CHAR_BREAK : m_style &= ~EDVS_CHAR_BREAK; }
    void setSpaceStyle(bool bSet)  { bSet ? m_style |= EDVS_SPACE : m_style &= ~EDVS_SPACE; }
    bool isSpaceMode(void)   { return m_style & EDVS_SPACE; }

    #ifdef __MMI_T9__
    void setAddWordStyle(bool bAddWord)  { m_AddWord = bAddWord;}
    #endif
    void setPasswordChar (char pswch) { m_pswdChar = pswch; }
    char passwordChar (void) { return m_pswdChar; }

    int setCaretPosition(int pos) { int slen = m_strings.size(); (pos < slen)? (m_caretPos = pos) : m_caretPos = slen; return 0; }
	int caretPosition(void) { return m_caretPos; }

    int getTextMCharLen(void);

	void onGetFocus(void);
	void onLoseFocus(void);
	void setFont(Logfont *f);
	void setFont (HTResId id);
	void SlKeyupNoDraw(void);
	bool dispatchEvent(Event *event);
	void drawBackground(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
	void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
#ifdef __MMI_T9__	
	void insertStringFromT9(char *ch, int cursor);
	void entryAddWordMode(void);
	void setCursorFromT9(void);
	void SynText(void);
	void DeleteChar(void);
	void addWord(char *ch);
	void displayAddWord(char *ch, int* pos, int wordlen,string &str, bool internal);
	void ResetT9Mode(bool  Neednotifyevent = FALSE);
#endif	

private:
    void innerDrawText(GraphicsContext *gc, const string s, const IntRect &rc, unsigned int color, Logfont *f, unsigned int format);
    bool handleEvent(Event* event);
    int CalculateCursor(GraphicsContext *gc,IntRect rc,string tmpStr ,int  caretPos ,int  *caretWidth);

private:
    int   m_timerId;
    string  m_strings;
    char  m_pswdChar;
    unsigned int  m_caretPos;
    int   m_SlTimerId;
	bool  m_passwordStyleShow;
	unsigned int  m_careTick;
	unsigned int  m_char_pos;
    bool  m_AddWord;
    #ifdef __MMI_T9__
    int   m_startPosX;
    #endif
    unsigned int  m_prevEventType; 
    bool  m_highlight;
};

} // namespace hfcl

#endif // _HFCL_SLEDITVIEW_H_


