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

#include "view/textview.h"

#include "view/viewfactory.h"
#include "common/helpers.h"
#include "graphics/textmode.h"
#include "view/transition.h"

namespace hfcl {

//define for roll text
#define ROLLOFFSET 5

TextView::TextView(const char* vtag, const char* vtype,
            const char* vclass, const char* vname, int vid)
    : ViewContainer(vtag, vtype, vclass, vname, vid)
    , m_isLongText(false)
    , m_stringId(-1)
    , m_lineAboveH (0)
    , m_lineBellowH (0)
    , m_margin(0)
    , m_bidicheck (false)
{
    m_textBuf[0] = '\0';
}

TextView::~TextView()
{
    stopRoll();
}

bool TextView::setLongText(void)
{
    // already is
    if(m_textString.length() > 0 || m_isLongText)
        return false;

    // short text for now, need copy text?
    if(m_textBuf[0] != '\0')
        m_textString = m_textBuf;

    m_textBuf[0] = '\0';
    m_isLongText = true;

    return true;
}

bool TextView::isLongText(void)
{
    return m_isLongText;
}

void TextView::setBidiCheck(bool bidiflag)
{
    m_bidicheck = true;
}

bool TextView::isBidiCheck(void)
{
    return m_bidicheck;
}

void TextView::setText(const std::string& text)
{
    setText(text.c_str());
}

void TextView::setText(const char* text)
{
    int len = 0;
    m_stringId = -1;

    /* VincentWei > OPT: do not update the content if... */
    if (NULL == text) text = "";
    if (!m_isLongText && strcmp (m_textBuf, text) == 0) {
        return;
    }

    len = strlen((char *)text);
    if(len > TEXT_BUFFER_LEN_OF_DEFAULT || m_isLongText) {
        m_textString = text;
        m_isLongText = true;
    }
    else {
        strcpy (m_textBuf, text);
        m_isLongText = false;
    }

    resetRoll();
    autoFitSize();
    updateView();
}

void TextView::setText(int strId)
{
    /* VincentWei > OPT: Only update the content if... */
    if (!m_isLongText && m_stringId == strId) {
        return;
    }

    m_stringId = strId;
    m_isLongText = false;
    updateView();
}

void TextView::setTextColor(Color *c)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_COLOR, c->color()))
        updateView();
#endif
}

void TextView::setTextColor(unsigned char r, unsigned char g, unsigned char b)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_COLOR, MakeRGB(r,g,b)))
        updateView();
#endif
}

void TextView::setTextColor(DWORD color)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_COLOR, color))
        updateView();
#endif
}

char* TextView::getText(void)
{
    if(m_stringId > 0) {
        return (char *)GetText(m_stringId);
    } else if(m_isLongText){
        return (char *)(m_textString.c_str());
    } else {
        return m_textBuf;
    }
}

int TextView::getTextLength(void)
{
    if(m_stringId > 0) {
        return strlen(GetText(m_stringId));
    } else if(m_isLongText) {
        return m_textString.length();
    } else {
        return strlen(m_textBuf);
    }
}

void TextView::drawContent(GraphicsContext* context, IntRect &rc)
{
    char* buf = NULL;

    if(m_stringId > 0)
        buf = (char *)GetText(m_stringId);
    else if(m_isLongText)
        buf = (char *)(m_textString.c_str());
    else
        buf = m_textBuf;

    {
        char tmpchar[8]={0};
        int len = strlen(buf);
        char *str = buf;
        int *info = (int *)malloc ((len + 1) * sizeof(int));
        int realCount = GetUTF8CharInfo (buf, len, info);
        info [realCount] = len;
        for (int i = 1; i <= realCount; /*i++*/)
        {
            memcpy(tmpchar, str + info[i-1], info[i] - info[i-1]);
            tmpchar [info[i] - info[i-1]] = '\0';
            if (IsSymbolUCS2Char (UTF8ToUCS2((Uint8 *)tmpchar))
                    || IsNumberUCS2Char(UTF8ToUCS2((Uint8 *)tmpchar))) {
                i++;
            }
            else {
                break;
            }
        }
        free(info);
        info=NULL;
        if (IsTransformArabicUCS2Char (UTF8ToUCS2((Uint8 *)tmpchar))
                ||IsArabicSymbolUCS2Char (UTF8ToUCS2((Uint8 *)tmpchar))) {
            context->setBiDiFirstChType (BIDI_TYPE_RTL);
        }
        else {
            context->setBiDiFirstChType (BIDI_TYPE_LTR);
        }
    }

    context->setTextAboveLineExtra(m_lineAboveH);
    context->setTextBellowLineExtra(m_lineBellowH);

#if 0
    if(isDisabled())
        status = DRAWSTATE_DISABLED;

    rc.inset(m_margin, 0);
    if (getRollProp() && isRolling()) {
        if(RollTextTransition::DrawRollText(this, context, m_drset,
                DR_CONTENT, status, rc, (DWORD)buf, DRDT_TEXT) == false) {
            m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTMODE, TextMode::TextOutOmitted|TextMode::SingleLine);
            m_drset->draw(context, DR_CONTENT, status, rc, (DWORD)buf, DRDT_TEXT);
        }
    }
    else {
        m_drset->draw(context, DR_CONTENT, status, rc, (DWORD)buf, DRDT_TEXT);
    }
#endif

    context->setBiDiFirstChType (BIDI_TYPE_LTR);
}

void TextView::setTextOutMode(unsigned int mode)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTMODE, mode))
        updateView();
#endif
}

void TextView::setTextOutLineMode(bool mode)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTLINEMODE, mode))
        updateView();
#endif
}

void TextView::setTextAlign(unsigned int align)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTALIGN, align))
        updateView();
#endif
}

unsigned int TextView::getTextAlign()
{
#if 0
    return m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTALIGN);
#else
    return 0;
#endif
}

unsigned int TextView::getTextValign()
{
#if 0
    return m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTVALIGN);
#else
    return 0;
#endif
}

void TextView::setTextValign(unsigned int valign)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTVALIGN, valign))
        updateView();
#endif
}

void TextView::setTextBreak(int tBreak)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTBREAK, tBreak))
        updateView();
#endif
}

int TextView::getTextBreak(void)
{
#if 0
    return m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTBREAK);
#else
    return 0;
#endif
}

void TextView::setTextFont(unsigned int font)
{
#if 0
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_FONT, (HTData)GetFontRes(font)))
        updateView();
#endif
}

void TextView::autoFitSize(bool auto_child_fit)
{
#if 0
    char* buf = NULL;
    if (!isAutoSize())
        return ;

    int w = m_rect.width();
    int h = m_rect.height();

    if(m_stringId > 0)
        buf = (char *)GetText(m_stringId);
    else if(m_isLongText)
        buf = (char *)(m_textString.c_str());
    else
        buf = m_textBuf;

    GraphicsContext::screenGraphics()->setTextAboveLineExtra(m_lineAboveH);
    GraphicsContext::screenGraphics()->setTextBellowLineExtra(m_lineBellowH);

    if (m_drset->calcDrawableSize(DR_CONTENT, DRAWSTATE_NORMAL, w, h, (DWORD)buf, DRDT_TEXT)) {
        m_rect.setSize(w + (m_margin*2), h);
        if (parent())
            parent()->onChildSizeChanged(this);
    }
#endif
}

void TextView::startRoll()
{
    stopRoll();
    if (getRollProp() && isNeedRoll()) {
        AddRollText(this, HFCL_NEW_EX(RollTextTransition, (this, ROLLOFFSET)));
        setRolling(true);
    }
}

void TextView::stopRoll()
{
    if (getRollProp()) {
        RemoveRollText(this);
        setRolling(false);
    }
}

void TextView::resetRoll()
{
    if (getRollProp()) {
        ResetRollText(this);
    }
}

bool TextView::isNeedRoll()
{
#if 0
    char* buf = NULL;
    IntRect rc(0, 0, m_rect.width(), m_rect.height());

    if(m_stringId > 0)
        buf = (char *)GetText(m_stringId);
    else if(m_isLongText)
        buf = (char *)(m_textString.c_str());
    else
        buf = m_textBuf;

    return RollTextTransition::NeedRollText(
        m_drset,
        DR_CONTENT,
        DRAWSTATE_NORMAL,
        rc,
        (DWORD)buf,
        DRDT_TEXT);
#else
    return false;
#endif
}

DEFINE_CLASS_NAME(TextView)

AUTO_REGISTER_VIEW(hvtext, TextView);

// the following tags are supported by TextView
// <p> <span> <em> <strong> <dfn> <code> <samp> <kbd> <var> <cite> <a>

AUTO_REGISTER_VIEW(p, TextView);
AUTO_REGISTER_VIEW(span, TextView);
AUTO_REGISTER_VIEW(em, TextView);
AUTO_REGISTER_VIEW(strong, TextView);
AUTO_REGISTER_VIEW(dfn, TextView);
AUTO_REGISTER_VIEW(code, TextView);
AUTO_REGISTER_VIEW(samp, TextView);
AUTO_REGISTER_VIEW(kbd, TextView);
AUTO_REGISTER_VIEW(var, TextView);
AUTO_REGISTER_VIEW(cite, TextView);
AUTO_REGISTER_VIEW(a, TextView);

} // namespace hfcl

