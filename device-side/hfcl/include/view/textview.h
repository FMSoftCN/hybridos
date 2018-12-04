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


#ifndef HFCL_VIEW_TEXTVIEW_H_
#define HFCL_VIEW_TEXTVIEW_H_

#include "view/view.h"
#include "graphics/color.h"
#include "resource/respkgmanager.h"

namespace hfcl {

class TextView : public View
{
public:
    TextView(View *parent);
    TextView(View* parent, DrawableSet* drset);
    virtual ~TextView();

    void setText(const char *text);
    void setText(const string text);
    void setText(int strId);
    int getTextId(void) { return m_stringId; }
    char* getText(void);
	int getTextLength(void);
    void setMargin(int margin) { m_margin = margin; }
		
    void setTextColor(Color *c);
    void setTextColor(unsigned char r, unsigned char g, unsigned char b);
    void setTextColor(DWORD color);

	void setTextOutMode(unsigned int mode);
	void setTextAlign(unsigned int align);
	void setTextValign(unsigned int valign);

	unsigned int getTextAlign();
	unsigned int getTextValign();

    void setTextOutLineMode(bool mode);
    void setTextFont(unsigned int font);

	void setTextBreak(int tBreak);
	int getTextBreak(void);

    inline void setLineAboveHeight(int height) { m_lineAboveH = height; }
    inline void setLineBellowHeight(int height) { m_lineBellowH = height; }

	////////////////////////////////////////////// 
	//once is long-text, we can not go back to short text, cause that is NOT right.
	bool setLongText(void);
	bool isLongText(void);
	
	void setBidiCheck(bool bidiflag);
	bool isBidiCheck(void);
	
	virtual void drawContent(GraphicsContext* context, IntRect &rc, int status/*= Style::NORMAL*/);

	void setRollProp(bool roll) { stopRoll(); setFlag(roll, ROLLTEXT); }
	bool getRollProp() { return m_flags & ROLLTEXT; }

	void startRoll();
	void stopRoll();
	void resetRoll();

	void setAutoSize(bool b) { setFlag(b, AUTOSIZE); }
	bool isAutoSize()        { return m_flags & AUTOSIZE; }
	virtual void autoFitSize(bool auto_child_fit = false);
	bool isNeedRoll();
	void setRolling(bool b) { setFlag(b, ROLLING); }
	bool isRolling() { return m_flags & ROLLING; }

protected:
	//define the flags of TextView
	enum {
		AUTOSIZE = (1 << View::FLAG_SHIFT),
		ROLLTEXT = (2 << View::FLAG_SHIFT),
		ROLLING  = (4 << View::FLAG_SHIFT),
		FLAG_SHIFT = ( 2 + View::FLAG_SHIFT)
	};

	bool  m_isLongText;
	string  m_textString;
	char    m_textBuf[TEXT_BUFFER_LEN_OF_DEFAULT+1];
    int   m_stringId;
	int   m_lineAboveH;
	int   m_lineBellowH;
    int   m_margin;
	bool  m_bidicheck;

    DECLARE_CLASS_NAME(TextView)
};


} // namespace hfcl

#endif /* HFCL_VIEW_TEXTVIEW_H_ */
