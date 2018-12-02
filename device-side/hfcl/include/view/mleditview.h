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


#ifndef _NGUX_MLEDITVIEW_H_
#define _NGUX_MLEDITVIEW_H_

#include "editview.h"
#include "graphicscontext.h"
#include "timerservice.h"

NAMESPACE_BEGIN

LIST(int, IntList);

class MlEditView : public EditView, public TimerEventListener
{
	DECLARE_CLASS_NAME(MlEditView)
public:
	MlEditView(View *p);
	MlEditView(View *p, DrawableSet* drset);
	virtual ~MlEditView();

    void setScrollBarVisible(bool bVisible) { m_barVisible = bVisible; }
    bool isScrollBarVisible(void) { return m_barVisible; }
    void setScrollBarWidth(int width) { m_barWidth = width; }
    int scrollBarWidth(void) { return m_barWidth; }
	
    void setText(const char * str);
	void setText(string str);
	string getText();
	unsigned int getTextLength() { return m_strings.size(); }	
	void insertText(const char *text, int *pos, string &str);
	void insertText(const char *text);
	void replacePrevChar(const char *ch, int *pos, string &str);
	void replacePrevChar(const char *ch);
	void deleteAllCharacters(void);
    int getLineCount(void) { return m_lineCout; }
	int getLineCaret(void) { return m_caretPosY; }
    inline void setLineAboveHeight(unsigned int height) { m_lineAboveH = height; }
    inline void setLineBellowHeight(unsigned int height) { m_lineBellowH = height; }

	void onGetFocus(void);
	void onLoseFocus(void);
    void setFont (Logfont *f); 
    
    int setCaretPosition(int pos);
	int caretPosition(void) { return m_caretPos; }
    int startShowLine(void) { return m_startShowLine;}
    int getTextMCharLen(void);
    void  urlNumberParseEnable(bool enable){ m_urlNumberParseEnable = enable;}
	bool dispatchEvent(Event *event);
	void drawBackground(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
	void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
    void drawScroll(GraphicsContext* context, IntRect &rc, int status/* = Style::NORMAL*/);
    void MultitapKeyupNoDraw(void);
    void drawSelect(GraphicsContext* gc, IntRect &selRect, bool status);
	void setUrlParseLen(int parseLen){m_urlStringsLen = parseLen;}
	int getUrlParseLen() {return m_urlStringsLen;}
#ifdef __MMI_T9__	
	void insertStringFromT9(char *ch, int cursor);
	void SynText(void);
	void DeleteChar(void);
	void setCursorFromT9(void);
	void entryAddWordMode(void);
	void addWord(char *ch);
	void displayAddWord(char *ch, int* pos, int wordlen,string &str, bool internal);
	void ResetT9Mode(bool  Neednotifyevent = FALSE);
#endif	
    void   layoutStrings(int fromLine = 0);
    void   setLayoutWordBreakStyle(bool isWordBreak = TRUE){m_isWordBreak = isWordBreak;}
    bool layoutWordBreakStyle(void){ return m_isWordBreak;}

	void   setEnableScroll(bool enable){ m_scrollStyle = enable;}
	bool enableScroll(void){ return m_scrollStyle ;}
private:
    bool handleEvent(Event* event);
    bool switchLine(bool isUpLine);
    void   refreshCaretPos(bool fromXY);	
	void   _insertText(const char *text, int* pos, string& str, bool internal);
	void   _replacePrevChar(const char *ch, int* pos, string &str, bool internal);
	void   drawHilightNumber(GraphicsContext* gc, int x, int y, int startCharIdx, int charCnt, char* lineStrPtr);
	int  hilightNumberPos(int nextHilightIndex, int *startPos, int *len);
	int  updateHilightNumber(int keyCode);
	void   raiseHilightNumberNotify(bool isSelect);
	int  queryLineIndex(int pos);
	void   layoutStringsByWord(void );
	
private:
    int       m_timerId;
    string      m_strings;
	int       m_urlStringsLen;
    IntList     m_lineCharList;
    int       m_caretPos;
    int       m_caretPosX;
    int       m_caretPosY;
    unsigned int      m_lineAboveH;
    unsigned int      m_lineBellowH;
    unsigned int      m_startShowLine;
    int       m_lineCout;
    int       m_totalH;
    int       m_barWidth;
    bool      m_barVisible;
    int       m_multapTimerId;
	bool      m_urlNumberParseEnable;
	int       m_hilightNumberIndex;
	bool      m_hilightValid;
    #ifdef __MMI_T9__
    int       m_startPosX;
    int       m_startPosY;
    #endif
    unsigned int      m_prevEventType; 
	bool      m_scrollStyle;
	bool      m_isWordBreak;
};

NAMESPACE_END

#endif // _NGUX_MLEDITVIEW_H_


