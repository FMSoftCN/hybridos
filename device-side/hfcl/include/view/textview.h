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

#include <string>

#include "view.h"
#include "../graphics/color.h"
#include "../resource/respkgmanager.h"

namespace hfcl {

class TextView : public View {
public:
    TextView(const char* cssClass, const char* name, int id = 0);
    virtual ~TextView();

    /* public methods */
    void setText(const char *text);
    void setText(const std::string& text);
    void setText(int strId);
    int getTextId() const { return m_stringId; }
    char* getText();
    int getTextLength();

    bool setLongText();
    bool isLongText();

    void setBidiCheck(bool bidiflag);
    bool isBidiCheck();

    void setRollProp(bool roll) { stopRoll(); setFlag(roll, ROLLTEXT); }
    bool getRollProp() { return m_flags & ROLLTEXT; }

    void startRoll();
    void stopRoll();
    void resetRoll();

    bool isNeedRoll();
    void setRolling(bool b) { setFlag(b, ROLLING); }
    bool isRolling() { return m_flags & ROLLING; }

    /* overloaded virtual functions */
    virtual const char* type() const { return "hvtext"; }
    virtual void drawContent(GraphicsContext* context, IntRect &rc);

    /* to be deprecated */
    void setAutoSize(bool b) { setFlag(b, AUTOSIZE); }
    bool isAutoSize()        { return m_flags & AUTOSIZE; }
    virtual void autoFitSize(bool auto_child_fit = false);

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
    int getTextBreak();

    inline void setLineAboveHeight(int height) { m_lineAboveH = height; }
    inline void setLineBellowHeight(int height) { m_lineBellowH = height; }

protected:
    //define the flags of TextView
    enum {
        AUTOSIZE = (1 << View::FLAG_SHIFT),
        ROLLTEXT = (2 << View::FLAG_SHIFT),
        ROLLING  = (4 << View::FLAG_SHIFT),
        FLAG_SHIFT = ( 2 + View::FLAG_SHIFT)
    };

    bool  m_isLongText;
    std::string  m_textString;
    char  m_textBuf[TEXT_BUFFER_LEN_OF_DEFAULT+1];
    int   m_stringId;
    int   m_lineAboveH;
    int   m_lineBellowH;
    int   m_margin;
    bool  m_bidicheck;

    DECLARE_CLASS_NAME(TextView)
};

} // namespace hfcl

#endif /* HFCL_VIEW_TEXTVIEW_H_ */
