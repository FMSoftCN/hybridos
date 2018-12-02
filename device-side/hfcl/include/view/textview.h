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


#ifndef _NGUX_TextView_h
#define _NGUX_TextView_h

#include "view.h"
#include "color.h"
#include "respkgmanager.h"

NAMESPACE_BEGIN

class TextView : public View
{
public:
    TextView(View *parent);
    TextView(View* parent, DrawableSet* drset);
    virtual ~TextView();

    void setText(const char *text);
    void setText(const string text);
    void setText(NGInt strId);
    NGInt getTextId(void) { return m_stringId; }
    char* getText(void);
	NGInt getTextLength(void);
    void setMargin(NGInt margin) { m_margin = margin; }
		
    void setTextColor(Color *c);
    void setTextColor(unsigned char r, unsigned char g, unsigned char b);
    void setTextColor(DWORD color);

	void setTextOutMode(NGUInt mode);
	void setTextAlign(NGUInt align);
	void setTextValign(NGUInt valign);

	NGUInt getTextAlign();
	NGUInt getTextValign();

    void setTextOutLineMode(NGBool mode);
    void setTextFont(NGUInt font);

	void setTextBreak(NGInt tBreak);
	NGInt getTextBreak(void);

    inline void setLineAboveHeight(NGInt height) { m_lineAboveH = height; }
    inline void setLineBellowHeight(NGInt height) { m_lineBellowH = height; }

	////////////////////////////////////////////// 
	//once is long-text, we can not go back to short text, cause that is NOT right.
	NGBool setLongText(void);
	NGBool isLongText(void);
	
	void setBidiCheck(NGBool bidiflag);
	NGBool isBidiCheck(void);
	
	virtual void drawContent(GraphicsContext* context, IntRect &rc, NGInt status/*= Style::NORMAL*/);

	void setRollProp(NGBool roll) { stopRoll(); setFlag(roll, ROLLTEXT); }
	NGBool getRollProp() { return m_flags & ROLLTEXT; }

	void startRoll();
	void stopRoll();
	void resetRoll();

	void setAutoSize(NGBool b) { setFlag(b, AUTOSIZE); }
	NGBool isAutoSize()        { return m_flags & AUTOSIZE; }
	virtual void autoFitSize(NGBool auto_child_fit = false);
	NGBool isNeedRoll();
	void setRolling(NGBool b) { setFlag(b, ROLLING); }
	NGBool isRolling() { return m_flags & ROLLING; }

DECLARE_CLASS_NAME(TextView)

protected:
	//define the flags of TextView
	enum {
		AUTOSIZE = (1 << View::FLAG_SHIFT),
		ROLLTEXT = (2 << View::FLAG_SHIFT),
		ROLLING  = (4 << View::FLAG_SHIFT),
		FLAG_SHIFT = ( 2 + View::FLAG_SHIFT)
	};

	NGBool  m_isLongText;
	string  m_textString;
	char    m_textBuf[TEXT_BUFFER_LEN_OF_DEFAULT+1];
    NGInt   m_stringId;
	NGInt   m_lineAboveH;
	NGInt   m_lineBellowH;
    NGInt   m_margin;
	NGBool  m_bidicheck;
};


NAMESPACE_END

#endif /* NGUX_TextView_h */
