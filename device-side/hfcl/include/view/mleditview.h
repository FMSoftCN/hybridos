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


#ifndef _NGUX_MLEDITVIEW_H_
#define _NGUX_MLEDITVIEW_H_

#include "editview.h"
#include "graphicscontext.h"
#include "timerservice.h"

NAMESPACE_BEGIN

LIST(NGInt, IntList);

class MlEditView : public EditView, public TimerEventListener
{
	DECLARE_CLASS_NAME(MlEditView)
public:
	MlEditView(View *p);
	MlEditView(View *p, DrawableSet* drset);
	virtual ~MlEditView();

    void setScrollBarVisible(NGBool bVisible) { m_barVisible = bVisible; }
    NGBool isScrollBarVisible(void) { return m_barVisible; }
    void setScrollBarWidth(NGInt width) { m_barWidth = width; }
    NGInt scrollBarWidth(void) { return m_barWidth; }
	
    void setText(NGCPStr str);
	void setText(string str);
	string getText();
	NGUInt getTextLength() { return m_strings.size(); }	
	void insertText(const char *text, NGInt *pos, string &str);
	void insertText(const char *text);
	void replacePrevChar(const char *ch, NGInt *pos, string &str);
	void replacePrevChar(const char *ch);
	void deleteAllCharacters(void);
    NGInt getLineCount(void) { return m_lineCout; }
	NGInt getLineCaret(void) { return m_caretPosY; }
    inline void setLineAboveHeight(NGUInt height) { m_lineAboveH = height; }
    inline void setLineBellowHeight(NGUInt height) { m_lineBellowH = height; }

	void onGetFocus(void);
	void onLoseFocus(void);
    void setFont (Logfont *f); 
    
    NGInt setCaretPosition(NGInt pos);
	NGInt caretPosition(void) { return m_caretPos; }
    NGInt startShowLine(void) { return m_startShowLine;}
    NGInt getTextMCharLen(void);
    void  urlNumberParseEnable(NGBool enable){ m_urlNumberParseEnable = enable;}
	NGBool dispatchEvent(Event *event);
	void drawBackground(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);
	void drawContent(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);
    void drawScroll(GraphicsContext* context, IntRect &rc, NGInt status/* = Style::NORMAL*/);
    void MultitapKeyupNoDraw(void);
    void drawSelect(GraphicsContext* gc, IntRect &selRect, NGBool status);
	void setUrlParseLen(NGInt parseLen){m_urlStringsLen = parseLen;}
	NGInt getUrlParseLen() {return m_urlStringsLen;}
#ifdef __MMI_T9__	
	void insertStringFromT9(char *ch, int cursor);
	void SynText(void);
	void DeleteChar(void);
	void setCursorFromT9(void);
	void entryAddWordMode(void);
	void addWord(char *ch);
	void displayAddWord(char *ch, NGInt* pos, NGInt wordlen,string &str, NGBool internal);
	void ResetT9Mode(NGBool  Neednotifyevent = FALSE);
#endif	
    void   layoutStrings(NGInt fromLine = 0);
    void   setLayoutWordBreakStyle(NGBool isWordBreak = TRUE){m_isWordBreak = isWordBreak;}
    NGBool layoutWordBreakStyle(void){ return m_isWordBreak;}

	void   setEnableScroll(NGBool enable){ m_scrollStyle = enable;}
	NGBool enableScroll(void){ return m_scrollStyle ;}
private:
    NGBool handleEvent(Event* event);
    NGBool switchLine(NGBool isUpLine);
    void   refreshCaretPos(NGBool fromXY);	
	void   _insertText(const char *text, NGInt* pos, string& str, NGBool internal);
	void   _replacePrevChar(const char *ch, NGInt* pos, string &str, NGBool internal);
	void   drawHilightNumber(GraphicsContext* gc, NGInt x, NGInt y, NGInt startCharIdx, NGInt charCnt, NGChar* lineStrPtr);
	NGInt  hilightNumberPos(NGInt nextHilightIndex, NGInt *startPos, NGInt *len);
	NGInt  updateHilightNumber(NGInt keyCode);
	void   raiseHilightNumberNotify(NGBool isSelect);
	NGInt  queryLineIndex(NGInt pos);
	void   layoutStringsByWord(void );
	
private:
    NGInt       m_timerId;
    string      m_strings;
	NGInt       m_urlStringsLen;
    IntList     m_lineCharList;
    NGInt       m_caretPos;
    NGInt       m_caretPosX;
    NGInt       m_caretPosY;
    NGUInt      m_lineAboveH;
    NGUInt      m_lineBellowH;
    NGUInt      m_startShowLine;
    NGInt       m_lineCout;
    NGInt       m_totalH;
    NGInt       m_barWidth;
    NGBool      m_barVisible;
    NGInt       m_multapTimerId;
	NGBool      m_urlNumberParseEnable;
	NGInt       m_hilightNumberIndex;
	NGBool      m_hilightValid;
    #ifdef __MMI_T9__
    NGInt       m_startPosX;
    NGInt       m_startPosY;
    #endif
    NGUInt      m_prevEventType; 
	NGBool      m_scrollStyle;
	NGBool      m_isWordBreak;
};

NAMESPACE_END

#endif // _NGUX_MLEDITVIEW_H_


