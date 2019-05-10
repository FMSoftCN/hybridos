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

#undef DEBUG

#include "ngux.h"

#include "imeinterface.h"
#include "imebase.h"

#include "ucs2-utils.h"
#include "ucs2-bidi.h"
#include "codeconvert.h"
#include "urlparse.h"

/****************** TODO LIST ****************
 * 01. mouse                ---- ??
 * 02. select text          ---- ??
 * 03. scroll bar           ---- ??
 **********************************************/
extern BOOL IsArabicSet(void);

char m_edit_tempstr1[EDITOR_MAX_LEN];
char m_edit_tempstr2[EDITOR_MAX_LEN];
Uint16 bidiStrOrder[EDITOR_MAX_LEN];

namespace hfcl {

#define DEFAULT_EDIT_SCROLL_W 4

DEFINE_CLASS_NAME(MlEditView)

MlEditView::MlEditView(View *p)
    : EditView (p)
    , m_strings("")
    , m_urlStringsLen(-1)
    , m_lineAboveH (1)
    , m_lineBellowH (1)
    , m_startShowLine (0)
    , m_lineCout (0)
    , m_urlNumberParseEnable(0)
    , m_hilightNumberIndex(-1)
    , m_prevEventType(0)
    , m_scrollStyle(0)
    , m_isWordBreak(0)
{
    //TODO
    m_hilightValid = FALSE;
}

MlEditView::MlEditView(View *p, DrawableSet *drs)
    : EditView (p, drs)
    , m_strings("")
    , m_urlStringsLen(-1)
    , m_lineAboveH (1)
    , m_lineBellowH (1)
    , m_startShowLine (0)
    , m_lineCout (0)
    , m_urlNumberParseEnable(0)
    , m_hilightNumberIndex(-1)
    , m_prevEventType(0)
    , m_scrollStyle(0)
    , m_isWordBreak(0)
{
    //TODO
    m_hilightValid = FALSE;
}

MlEditView::~MlEditView()
{
#ifdef __MMI_T9__
    m_addword = false;
#endif

    if (0 != m_timerId) {
        removeTimer(m_timerId);
        m_timerId = 0;
    }

    if (0 != m_multapTimerId) {
        MMI_active_multitap = -1;
        removeTimer(m_multapTimerId);
        m_multapTimerId = 0;
    }
}

void MlEditView::onGotFocus(void)
{
    if (0 != m_timerId)
        removeTimer(m_timerId);

    m_timerId = registerTimer(500, "MlEditView");

#ifdef __MMI_T9__
    if(m_addword)
        imeEnable(true);
#endif

    EditView::onGotFocus();
}

void MlEditView::onLostFocus(void)
{
    if(m_timerId != 0)
    {
        removeTimer(m_timerId);
        m_timerId = 0;
    }

    if (0 != m_multapTimerId) {
        MMI_active_multitap = -1;
        removeTimer(m_multapTimerId);
        m_multapTimerId = 0;
    }

    EditView::onLostFocus();
}

void MlEditView::MultitapKeyupNoDraw(void)
{
    if (0 != m_multapTimerId)
        removeTimer(m_multapTimerId);
    m_multapTimerId = registerTimer(1000, "MlEditView-Multap");
}

void MlEditView::setFont (Logfont *f)
{
    EditView::setFont(f);
    layoutStrings();
    updateView();
}

void MlEditView::setText(const char * str)
{
    if(m_IsCharCount)
    {
        unsigned int maxcount  =0;
        maxcount = get_utf8_char_count (str,strlen(str));

        if (m_txtLimit < 0 || maxcount <= m_txtLimit){
            m_strings = str;
        }else{
            int Len = get_utf8_len_by_char_count(str,m_txtLimit);
            char *tmp = (char *)malloc(Len * sizeof(char) +2);
            memset(tmp, 0x0, Len * sizeof(char)+2);
            strncpy(tmp, str, Len);
            m_strings = tmp;
            free(tmp);
            tmp=NULL;
        }
    }
    else
    {
        if (m_txtLimit < 0 || strlen(str) <= m_txtLimit){
            m_strings = str;
        } else {
            char *tmp = (char *)malloc(m_txtLimit * sizeof(char));
            memset(tmp, 0x0, m_txtLimit * sizeof(char)+2);
            strncpy(tmp, str, m_txtLimit);
            m_strings = tmp;
            free(tmp);
         tmp=NULL;
        }
    }
    if(m_style & EDVS_3COMMA)
    {
        strcpy(m_edit_tempstr1, m_strings.c_str());
        m_strings = apply_comma_multiple_value_in_calculator(m_edit_tempstr1, &m_caretPos);
    }
    m_caretPos = m_strings.length();
#ifdef __MMI_T9__
    int count = get_utf8_char_count(m_strings.c_str(),m_caretPos);
    imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
#endif

    raiseNotifyEvent(NOTIFY_EDIT_SETTING_TEXT);

    layoutStrings();

    if(m_urlNumberParseEnable&& isReadOnly()) // reset the hilight
    {
        m_startShowLine         = 0;
        m_hilightValid          = FALSE;
        m_hilightNumberIndex    = -1;
    }
    SwitchShiftToLower(getText(),getTextLength(),true);
    updateView();
}

void MlEditView::setText(string str)
{
    if(m_IsCharCount)
    {
       unsigned int maxcount  =0;
       maxcount =  get_utf8_char_count(str.c_str(),str.size());
       if (m_txtLimit < 0 || maxcount <= m_txtLimit){
            m_strings = str;
        } else {
                int Len = get_utf8_len_by_char_count(str.c_str(),m_txtLimit);
                m_strings = str.substr(0, Len);
        }
    }
    else
    {
        if (m_txtLimit < 0 || str.size() <= m_txtLimit){
            m_strings = str;
        } else {
            m_strings = str.substr(0, m_txtLimit);
        }
    }
    if(m_style & EDVS_3COMMA)
    {
        strcpy(m_edit_tempstr1, m_strings.c_str());
        m_strings = apply_comma_multiple_value_in_calculator(m_edit_tempstr1, &m_caretPos);
    }
    m_caretPos = m_strings.length();
#ifdef __MMI_T9__
    int count = get_utf8_char_count(m_strings.c_str(),m_caretPos);
    imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
#endif
    raiseNotifyEvent(NOTIFY_EDIT_SETTING_TEXT);

    layoutStrings();

    if(m_urlNumberParseEnable && isReadOnly()) // reset the hilight
    {
        m_startShowLine         = 0;
        m_hilightValid          = FALSE;
        m_hilightNumberIndex    = -1;
    }
    SwitchShiftToLower(getText(),getTextLength(),true);
    updateView();
}

string MlEditView::getText()
{
    return m_strings;
}

void MlEditView::_insertText(const char *text, int* pos, string& str, bool internal)
{
    unsigned int txtLen, insLen, currLen;
    unsigned int txtcount=0,strcount=0;
    txtLen = strlen(text);
    currLen = str.size();
    insLen = txtLen;

    if(m_IsCharCount)
    {
        txtcount =  get_utf8_char_count(text,txtLen);
        strcount =  get_utf8_char_count(str.c_str(),str.length());
        if(m_txtLimit > 0  && (txtcount + strcount) > m_txtLimit) {
            txtcount = m_txtLimit - strcount;
        }
        insLen = get_utf8_len_by_char_count(text,txtcount);
    }
    else
    {
        if (m_txtLimit > 0 && currLen + insLen > m_txtLimit) {
            insLen = m_txtLimit - currLen;
        }
    }

    if (insLen > 1 || (insLen == 1 && (isprint(*text) || *text == '\n'))){
        char *src = (char *) str.c_str();
        memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
        if (insLen < txtLen) {
            const char *p = text;
            unsigned int charLen, tLen = 0;
            strncpy(m_edit_tempstr1, src, *pos);
            while (insLen > 0 && p != NULL){
                charLen = get_first_utf8_char_len(p, txtLen);
                if (insLen > charLen) {
                    strncat(m_edit_tempstr1, p, charLen);
                    tLen += charLen;
                }
                insLen -= charLen;
                txtLen -= charLen;
                p += charLen;
            }
            strncat(m_edit_tempstr1, src + *pos, currLen - *pos);
            if (internal) {
                *pos += tLen;
            }
        } else {
            strncpy(m_edit_tempstr1, src, *pos);
            strncat(m_edit_tempstr1, text, insLen);
            strncat(m_edit_tempstr1, src + *pos, currLen - *pos);
            if (internal) {
                *pos += insLen;
            }
        }

        if(m_style & EDVS_3COMMA) {
            if(m_edit_tempstr1[*pos] == ',' && (text[0] == '+' || text[0] == '-' || text[0] == 'x' || text[0] == '/'))
                (*pos)++;

            str = apply_comma_multiple_value_in_calculator(m_edit_tempstr1, pos);
        }
        else {
            str = m_edit_tempstr1;
        }

        if (internal) {
            if (*pos > (int)str.size()){
                *pos = (int)str.size();
            }
            layoutStrings((m_isWordBreak && m_caretPosY>0)?m_caretPosY-1:m_caretPosY);
            showCaret(true);
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                SynText();
#endif

            updateView();
            raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
        }
    }
}

void MlEditView::insertText(const char *text, int *pos, string &str)
{
    _insertText(text, pos, str, false);

}

void MlEditView::insertText(const char *text)
{
    if (text == NULL || (m_style & EDVS_READONLY))
        return;

    raiseNotifyEvent(NOTIFY_EDIT_READY_INSERT_TEXT, (int)text, (int)m_caretPos);
    _insertText(text, &m_caretPos, m_strings, true);
}

void MlEditView::layoutStringsByWord(void)
{
    unsigned int cCount;
    char *tmpStr;
    unsigned char *sub;
    char buf[256];
    int utfLen = 0;
    Uint16 tmpChar;
    int thisLineW = 0;

    char * ptr = (char *)(m_strings.c_str());
    if(enableScroll() && m_hMargin < 2)  // we should reserve more margin for scroll.
        m_hMargin = 2;

    int rcW = getRect().width() - (m_hMargin << 1);

    int sW = 0, sH = 0;
    unsigned int i = 0, j = 0;
    int lineend,counttmp;

    int oldLineCount = m_lineCout;

    m_lineCout = 0;
    m_lineCharList.clear();

    cCount = get_utf8_char_count(ptr, m_strings.length());

    if(cCount > 500) {
        tmpStr = ( char *)malloc ((cCount + 2) * 2 * sizeof (char));
        if(!tmpStr)
            return ;
        memset(tmpStr, 0x0, ((cCount + 2) * 2 * sizeof (char)));
    }
    else {
        tmpStr = m_edit_tempstr1;
    }

    sub = (unsigned char *)m_edit_tempstr2;

    NGUtf8ToUnicode((unsigned char *)ptr, (unsigned char *)tmpStr);
    ConvertToGlyphForm((Uint16*)tmpStr, cCount, 0);

    while(i < cCount)
    {
        tmpChar = ((Uint16*)sub)[j + 0] = *((Uint16*)tmpStr + i);
        ((Uint16*)sub)[j + 1] = '\0';


#if 0 /* VincentWei: Do not use RDA font API */
        Get_CharWidthHeight (tmpChar, &sW, &sH);
#else
        GLYPHINFO glyph_info;
        GetGlyphInfo (m_font, (Glyph32)tmpChar, &glyph_info);
        sH ^= sH;   // avoid compilation warning
        sH = glyph_info.height;
        sW = glyph_info.advance_x;
#endif
        thisLineW += sW;

        if (thisLineW > rcW
                || ((Uint16*)sub)[j] == '\n')
        {
            memset(buf, 0x0, sizeof(buf));
            if(DT_WORDBREAK)
            {
                if(((Uint16*)sub)[j] == '\n' && thisLineW <= rcW)
                {
                    utfLen = idxUnicodeGlyphToUTF8((Uint16 *)sub, j+1);
                    m_lineCharList.push_back(utfLen);
                    thisLineW = 0;
                    m_lineCout++;
                    i ++;
                }
                else if(thisLineW > rcW)
                {
                    if(((Uint16*)sub)[j] == ' ' || ((Uint16*)sub)[j] == '\n' )
                    {
                        ((Uint16*)sub)[j + 0] = '\0';
                        //i-= 2;
                        //j-= 2;
                        utfLen = idxUnicodeGlyphToUTF8((Uint16 *)sub, j+1);
                        m_lineCharList.push_back(utfLen);
                        thisLineW = 0;
                        m_lineCout++;
                    }
                    else
                    {
                        BOOL findflag=FALSE;
                        lineend = j;
                        counttmp = i;
                        while(j>0)
                        {
                            if(((Uint16*)sub)[j]  == ' ')
                            {
                                memset(&((Uint16*)sub)[j+1], 0x0, lineend- j);
                                findflag = TRUE;
                                utfLen = idxUnicodeGlyphToUTF8((Uint16 *)sub, j+1);
                                m_lineCharList.push_back(utfLen);
                                thisLineW = 0;
                                m_lineCout++;
                                i ++;
                                break;
                            }
                            i --;
                            j --;
                        }
                        if(findflag == FALSE)
                        {
                            j = lineend;
                            i = counttmp;
                            ((Uint16*)sub)[j] = '\0';
                            utfLen = idxUnicodeGlyphToUTF8((Uint16 *)sub, j+1);
                            m_lineCharList.push_back(utfLen);
                            thisLineW = 0;
                            m_lineCout++;
                        }
                    }
                }
                j = 0;
            }
            else
            {
                ((Uint16*)sub)[j] = '\0';
            }
            j = 0;
        }
        else
        {
            i ++;
            j ++;
        }
    }
    if(i >= cCount && j > 0)
    {
        utfLen = idxUnicodeGlyphToUTF8((Uint16 *)sub, j);
        m_lineCharList.push_back(utfLen);
        m_lineCout++;
    }

    if(cCount > 500) {
        if (tmpStr != NULL){
            free(tmpStr);
        }
    }

    if (m_lineCout < oldLineCount) {
        m_startShowLine -= (oldLineCount - m_lineCout);
        _DBG_PRINTF ("MlEditView::layoutStringByWord > sm_startShowLine: %d", m_startShowLine);
    }
}

void MlEditView::layoutStrings(int fromLine)
{
    int i, lineLen = 0;

    if(enableScroll() && m_hMargin < 2)  // we should reserve more margin for scroll.
        m_hMargin = 2;

    int rcW = getRect().width() - (m_hMargin << 1);

    char *_str = (char *)(m_strings.c_str());
    int _total = m_strings.size();
    int oldLineCount = m_lineCout;
    char *_start = _str;
    int thisLineW = 0;
    int lineEnd = 0;
    int w, h;
    int offset = 0;
    int count=0;
    BOOL findflag;
    GraphicsContext *gc = GraphicsContext::screenGraphics();

    if (fromLine == 0) {
        m_lineCout = 0;
        m_lineCharList.clear();
    }
    else {
        m_lineCout = fromLine;
        // erase the old layout info
        while (fromLine++ < oldLineCount){
            m_lineCharList.pop_back();
        }
        // get the start to layout
        for (IntList::iterator it = m_lineCharList.begin(); it != m_lineCharList.end(); it++) {
            _start += *it;
            _total -= *it;
        }
    }

    while (_start != NULL) {
        char *_end = strchr (_start, '\n');
        int _len = (_end == NULL) ? strlen(_start) : _end - _start + 1;

        int *info = (int *)malloc ((_len + 1) * sizeof(int));

        int realCount = get_utf_chars_info (_start, _len, info);
        if (realCount == 0) {
            lineLen = 0;
            m_lineCharList.push_back(lineLen);
            m_lineCout++;
        }
        else {
            thisLineW = 0;
            lineEnd = 0;
            offset = 0;
            count=0;

            NGUtf8ToUnicode((unsigned char *)_start, (unsigned char *)m_edit_tempstr1);

            info[realCount] = _len; //add the last char info

            for (i = 0; i < realCount; ) {
                ((Uint16*)m_edit_tempstr2)[i-count] = ((Uint16*)m_edit_tempstr1)[i-offset];
                ((Uint16*)m_edit_tempstr2)[i-count+1] = '\0';

                gc->getTextDrawSize((unsigned short *)m_edit_tempstr2, m_font, &w, &h);
                thisLineW = w;

                if (thisLineW > rcW){
                    findflag=FALSE;

                    if(m_isWordBreak == TRUE) {
                        int counttmp = i;
                        while(counttmp>count) {
                            if(((Uint16*)m_edit_tempstr1)[counttmp]  == ' ') {
                                findflag = TRUE;
                                lineLen = info[++counttmp] - lineEnd;
                                m_lineCharList.push_back(lineLen);
                                i = counttmp;
                                break;
                            }
                            counttmp--;
                        }
                    }

                    if(findflag == FALSE) {
                        lineLen = info[i] - lineEnd;
                        m_lineCharList.push_back(lineLen);
                    }

                    m_lineCout++;
                    lineEnd = info[i];
                    thisLineW = 0;
                    count = i;
                    continue;
                }

                if((offset == 0) && (i > 500)) {
                    offset = i;
                    NGUtf8ToUnicode((unsigned char *)(_start+info[offset]), (unsigned char *)m_edit_tempstr1);
                }
                i++;
            }

            //end line
            if (thisLineW > 0) {
                lineLen = _len - lineEnd;
                m_lineCharList.push_back(lineLen);
                m_lineCout++;
            }
        }

        free(info);
        info=NULL;
        _start = (_end != NULL) ? _end + 1 : NULL;
    }

    refreshCaretPos (false);

    if (m_lineCout < oldLineCount){
        m_startShowLine -= (oldLineCount - m_lineCout);
        if ((int)m_startShowLine < fromLine)
            m_startShowLine = fromLine;
        _DBG_PRINTF ("MlEditView::layoutStrings > m_startShowLine: %d", m_startShowLine);
    }
}

void MlEditView::refreshCaretPos(bool fromXY)
{
    if (fromXY) {
        m_caretPos = 0;
        IntList::iterator it = m_lineCharList.begin();

        for (int i = 0; i < m_caretPosY; i++) {
            m_caretPos += (*it);
            it++;
        }

        if (m_caretPosX > 0 && it != m_lineCharList.end() && *it > 0) {
            string lineString = m_strings.substr(m_caretPos, *it);
            char *_str = (char *)lineString.c_str();
            int len = lineString.size();

            if (len <= m_caretPosX) {
                m_caretPos += len;
                // at the end of a line,
                // and there is a line seperator.
                if (len > 0 && _str[len - 1] == '\n') {
                    m_caretPos--;
                }
            }
            else {
                int *info = (int *)malloc ((len + 1) * sizeof(int));
                int realCount = get_utf_chars_info (_str, len, info);

                info [realCount] = len;

                for (int i = 0; i <= realCount; i++) {
                    if (info[i] >= m_caretPosX) {
                        m_caretPos += info[i];
                        // at the end of a line,
                        // and there is a line seperator.
                        if (i == realCount && _str[info[i]] == '\n') {
                            m_caretPos--;
                        }
                        break;
                    }
                }
                free(info);
                  info = NULL;
            }
        }
    }
    else {
        int total = m_caretPos;

        m_caretPosX = 0;
        m_caretPosY = 0;

        for (IntList::iterator it = m_lineCharList.begin(); it != m_lineCharList.end(); it++) {

            if (total > *it) {
                total -= *it;
                m_caretPosY++;
            }
            else {
                m_caretPosX = total;
                if (m_caretPosX == (*it)) {
                    if (m_strings.compare(m_caretPos - 1, 1, "\n") == 0) {
                        m_caretPosY++;
                        m_caretPosX = 0;
                    }
                }

                break;
            }
        }
    }
}

void MlEditView::_replacePrevChar(const char *ch, int* pos, string &str, bool internal)
{
    unsigned int totalLen = str.size();
    unsigned int insLen = (ch != NULL ? strlen(ch) : 0);
    int bTail = (totalLen == (unsigned int)*pos);
    bool isNewline = FALSE;

    memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
    memset(m_edit_tempstr2,0,EDITOR_MAX_LEN);
    strcpy(m_edit_tempstr1, str.c_str());

    if (!bTail) {
        if(m_style & EDVS_3COMMA && m_edit_tempstr1[*pos] == ',')
            (*pos)++;
        strcpy(m_edit_tempstr2, m_edit_tempstr1 + *pos);

        if(m_style & EDVS_3COMMA && m_edit_tempstr1[*pos-1] == ',')
            (*pos)--;
        m_edit_tempstr1[*pos] = '\0';
    }
    int lastChLen = get_last_utf8_char_len(m_edit_tempstr1, strlen(m_edit_tempstr1));

    if(m_edit_tempstr1[*pos - lastChLen] == '\n')
        isNewline = TRUE;

    m_edit_tempstr1[*pos - lastChLen] = '\0';

    if (m_IsCharCount) {
        unsigned int strcount =  get_utf8_char_count(str.c_str(),str.size());
        unsigned int inscount = (ch != NULL ? get_utf8_char_count(ch,strlen(ch)) : 0) ;

        if (m_txtLimit > 0 && strcount - 1 + inscount > m_txtLimit) {
            inscount = m_txtLimit -( strcount - 1);
            insLen = get_utf8_len_by_char_count(ch,inscount);
        }
    }
    else {
        if (m_txtLimit > 0 && totalLen - lastChLen + insLen > m_txtLimit) {
            insLen = m_txtLimit - (totalLen - lastChLen);
        }
    }

    if (insLen > 1 || (insLen == 1 && isprint(*ch))) {
        strncat(m_edit_tempstr1, ch, insLen);
    }

    str = m_edit_tempstr1;

    if (!bTail) {
        str += m_edit_tempstr2;
    }

    if (m_style & EDVS_3COMMA) {
        strcpy(m_edit_tempstr1, str.c_str());
        str = apply_comma_multiple_value_in_calculator(m_edit_tempstr1, pos);
    }

    *pos += insLen - lastChLen;

    if(*pos < 0) {
        *pos = 0;
    }
    else if (*pos > (int)str.size()) {
        *pos = str.size();
    }

#ifdef __MMI_T9__
    if(*pos == 0) {
           switch(imeGetCurrentInputMode()) {
            case INPUT_MODE_SMART_SHIFT_ABC:
            case INPUT_MODE_SMART_SHIFT_FRENCH:
                imeT9SetShifStateWithFieldInfo ();
                imeSetSwitch();
                break;
            case INPUT_MODE_MULTITAP_SHIFT_FRENCH:
            case INPUT_MODE_MULTITAP_SHIFT_ABC:
                imeSetMultitapShiftMode(SHIFT_STATE_INIT);
                imeSetSwitch();
                break;
        }
    }
#endif

    if(internal) {
        layoutStrings((isNewline && m_caretPosY>0)?m_caretPosY-1:m_caretPosY);
        showCaret(true);
        updateView();
        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    }
}

void MlEditView::replacePrevChar(const char *ch, int *pos, string &str)
{
    _replacePrevChar(ch, pos, str, false);
}

void MlEditView::replacePrevChar(const char *ch)
{
    raiseNotifyEvent(NOTIFY_EDIT_REPLACING_TEXT, (int)ch, (int)m_caretPos);
    _replacePrevChar(ch, &m_caretPos, m_strings, true);
}

void MlEditView::deleteAllCharacters(void)
{
    m_strings = "";
    m_caretPos = m_strings.length();
    layoutStrings();
#ifdef __MMI_T9__
    if(imeGetInputModeFlag() == INPUT_MODE_SMART)
    {
        //m_bSelect = false;
        SynText();
    }
#endif
    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

#ifdef __MMI_T9__
void MlEditView::insertStringFromT9(char *ch, int cursor)
{
    unsigned int len ;

    if(m_IsCharCount) {

        unsigned int maxcount  =0;
        maxcount =  get_utf8_char_count(ch,strlen(ch));
        if (maxcount > m_txtLimit) {
            int Len = get_utf8_len_by_char_count (ch, m_txtLimit);
            memset (m_edit_tempstr1,0,EDITOR_MAX_LEN);
            strncpy (m_edit_tempstr1, ch,Len);
            int lastChLen = get_last_utf8_char_len (m_edit_tempstr1, strlen(m_edit_tempstr1));
            m_edit_tempstr1[Len - lastChLen] = '\0';
            m_strings = m_edit_tempstr1;
            cursor = m_strings.size();
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
            SynText();
        }
        else {
            m_strings = ch;
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
        }
    }
    else {
        len= strlen(ch);
        if(len > m_txtLimit) {
            memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
            strncpy(m_edit_tempstr1, ch,m_txtLimit);
            int lastChLen = get_last_utf8_char_len(m_edit_tempstr1, strlen(m_edit_tempstr1));
            m_edit_tempstr1[m_txtLimit - lastChLen] = '\0';
            m_strings = m_edit_tempstr1;
            cursor = m_strings.size();
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
            SynText();
        }
        else {
            m_strings = ch;
            if(cursor <0)
                cursor = 0;
            m_caretPos = cursor;
        }
    }

    m_addword = false;
    layoutStrings(m_startPosY);
    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

void MlEditView::SynText(void)
{
    //m_caretPos = m_strings.length();
    //int count;
    //m_bSelect = false;
    //count = get_utf8_char_count(m_strings.c_str(),m_caretPos);
    imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),m_caretPos);
}

void MlEditView::DeleteChar(void)
{
    int count;
    replacePrevChar(NULL);
    showCaret(false);
    count = get_utf8_char_count(m_strings.c_str(),m_caretPos);
    imeT9KeyRSKClearHandlerMultilineInputBox((char *)m_strings.c_str(),count);
    if(imeT9GetActiveWordCount() == 0)
        m_bSelect = false;
    imeRefreshString();
}

void MlEditView::ResetT9Mode (bool Neednotifyevent)
{
    m_addword = false;
    m_bSelect = false;
    if(imeGetInputModeFlag() == INPUT_MODE_SMART) {
        int count =get_utf8_char_count(m_strings.c_str(),m_caretPos);
        imeT9SyncWithInputBoxMultilineInputBox((char *)m_strings.c_str(),count);
    }

    if (Neednotifyevent)
        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

void MlEditView::setCursorFromT9(void)
{
    int unicodeCursor = imeT9GetUnicodeCursor();
    const char *utf8S = m_strings.c_str();
    int utf8Len = m_strings.size();
    m_caretPos = 0;

    for (int i = 0; i < unicodeCursor; i++) {
        int f_w_len = get_first_utf8_char_len(utf8S, utf8Len);
        m_caretPos += f_w_len;
        utf8S += f_w_len;
        utf8Len -= f_w_len;
    }

    if(m_IsCharCount) {
        unsigned int maxcount = get_utf8_char_count(m_strings.c_str(),m_caretPos);
        if(maxcount > m_txtLimit) {
            int Len = get_utf8_len_by_char_count(m_strings.c_str(),m_txtLimit);
            m_caretPos = Len;
        }
    }
    else {
        if (m_caretPos > (int)m_txtLimit)
            m_caretPos = m_txtLimit;
    }
}

void MlEditView::entryAddWordMode(void)
{
    m_addword = true;
    setActiveWordCount(imeT9GetActiveWordCount());
    m_inputmethod = imeGetCurrentInputMode();
    raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
    updateView();
}

void MlEditView::displayAddWord(char *ch, int* pos, int wordlen,string &str, bool internal)
{
    unsigned int totalLen = str.size();
    int insLen = (ch != NULL ? strlen(ch) : 0);
    int delcount = wordlen;
    int bTail = ((int)totalLen == *pos);
    int deslen = 0;
    memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
    memset(m_edit_tempstr2,0,EDITOR_MAX_LEN);
    strcpy(m_edit_tempstr1, str.c_str());

    if (!bTail) {
        strcpy(m_edit_tempstr2, m_edit_tempstr1 + *pos);
        m_edit_tempstr1[*pos] = '\0';
    }

    while(wordlen) {
        int lastChLen = get_last_utf8_char_len(m_edit_tempstr1, strlen(m_edit_tempstr1));
        deslen += lastChLen;
        m_edit_tempstr1[*pos - deslen] = '\0';
        wordlen--;
    }

    if(m_IsCharCount) {
        unsigned int strcount =  get_utf8_char_count(str.c_str(),str.size());
        unsigned int inscount = (ch != NULL ? get_utf8_char_count(ch,strlen(ch)) : 0) ;

        if (m_txtLimit > 0 && strcount - delcount + inscount > m_txtLimit) {
            inscount = m_txtLimit -( strcount - delcount);
            insLen = get_utf8_len_by_char_count(ch,inscount);
        }
    }
    else {
        if (m_txtLimit > 0 && totalLen - deslen + insLen > m_txtLimit) {
            insLen = m_txtLimit - (totalLen - deslen);
        }
    }

    if(imeIsSmartShiftInputMode()) {
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
        layoutStrings();
        showCaret(true);
        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
        updateView();
    }
}

void MlEditView::addWord(char *ch)
{
    int wordcount = getActiveWordCount();
    imeSetInputMode (m_inputmethod);
    imeChangeInputboxMode (m_inputmethod);

    if(ch==NULL) {
        SynText();
        m_addword = false;
        m_bSelect = false;
        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
        updateView();
    }
    else if(strlen(ch) ==0) {
        SynText();
        m_addword = false;
        //m_bSelect = false;
        raiseNotifyEvent(NOTIFY_EDIT_CHANGED);
        updateView();
    }
    else {
        int len;
        char temp[102];
        displayAddWord(ch, &m_caretPos, wordcount,m_strings, true);
        len = NGUtf8ToUnicode((unsigned char*)ch,(unsigned char*)temp);
        imeT9AddCustWordIntoLDB((unsigned short *)temp, len/2-1);
    }
}

#endif

int MlEditView::getTextMCharLen(void)
{
    return get_utf_chars_info(m_strings.c_str(), m_strings.size(), NULL);
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

#define MAX_HILIGHT_NUM_CNT   (100)

bool MlEditView::dispatchEvent(Event *event)
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
#ifdef __MMI_T9__
        if(imeGetInputModeFlag() == INPUT_MODE_NUMBER && code >= KeyEvent::KEYCODE_0 && code <= KeyEvent::KEYCODE_STAR)
        {
            char ch[2]={0, 0};
            m_bSelect = false;
            ch[0] = _charactor(code);
            insertText(ch);
        }
#endif
#ifdef __MMI_GB_V5__
        ui_char_t temp;
        unsigned char utf8[10] = {0};
        char ch[2]={0, 0};

        if ((ch[0] = _charactor(code)) > 0){

            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                if (isImeExist() && isImeEnable() && !isImeOpen()
                            && ch[0] >= '2' && ch[0] <= '9'){
                    openImeWin();
                    setImeTarget((int)this, 1);
                    imeInsertChar(ch[0]);
                }
            }
            else if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP)
            {
                if(::imeGetPreviousActiveMultitap() == code)
                {
                    temp =(Uint16)getMultitapCharCode(code);
                    mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
                    replacePrevChar((char *)utf8);
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
            return STOP_DISPATCH;
        }
#endif
        m_prevEventType = event->eventType();

        if (0 != m_multapTimerId && _charactor(code) == -1)
        {
            removeTimer(m_multapTimerId);
            imeDisableActiveMultitapWithoutDraw();
            m_multapTimerId = 0;
            blinkCaret();
            if(code == KeyEvent::KEYCODE_CURSOR_RIGHT)
                return STOP_DISPATCH;
        }

        switch(code)
        {
            case KeyEvent::KEYCODE_SOFTKEY_LEFT :
                // Left Soft Key .. Do Nothings ??
                break;
            case KeyEvent::KEYCODE_SOFTKEY_RIGHT :
                if (m_strings.size() > 0) {
#ifdef __MMI_T9__
                    if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                    {
                        if(m_caretPos <0)
                            m_caretPos = 0;
                        else
                            DeleteChar();
                        if(m_bSelect==false)
                            blinkCaret();
                    }
                    else
                    {
                        replacePrevChar(NULL);
                    }
                    return STOP_DISPATCH;
#else
                    replacePrevChar(NULL);
                    return STOP_DISPATCH;
#endif
                }
                return GOON_DISPATCH;
//#if 0
            case KeyEvent::KEYCODE_CURSOR_LEFT :
#ifdef __MMI_T9__
                if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                {
                    bool preT9ShiftMode = imeIsT9ShiftMode();

                    m_bSelect = false;
                    imeT9KeyArrowLeftHandlerMultilineInputBox();
                    setCursorFromT9();
                    refreshCaretPos(false);
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
                    return STOP_DISPATCH;
                }
                else
#endif
                if (m_caretPos > 0) {
                    int lastChLen = get_last_utf8_char_len(m_strings.c_str(), m_caretPos);
                    m_caretPos -= lastChLen;
                    refreshCaretPos(false);
                    showCaret(true);
                    updateView();
                    return STOP_DISPATCH;
                   }
                return GOON_DISPATCH;
            case KeyEvent::KEYCODE_CURSOR_RIGHT :
#ifdef __MMI_T9__
                   if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                   {
                    bool max_check = TRUE;
                    m_bSelect = false;
                    if(m_IsCharCount)
                    {
                        unsigned int strcount  =0;
                        strcount =  get_utf8_char_count(m_strings.c_str(),m_caretPos);
                        if(strcount >= m_txtLimit)
                            max_check = FALSE;
                    }
                    else
                    {
                        if(m_caretPos >= (int)m_txtLimit)
                            max_check = FALSE;
                    }
                    if (max_check)
                    {
                        bool preT9ShiftMode = imeIsT9ShiftMode();

                        imeT9KeyArrowRightHandlerMultilineInputBox();
                        imeRefreshString();
                        setCursorFromT9();
                        refreshCaretPos(false);
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
                    }
                    updateView();
                    return STOP_DISPATCH;
                   }
                   else
#endif
                if (m_bInsertSpace && m_caretPos == (int)getTextLength()){
                    insertText(" ");
                    return STOP_DISPATCH;
                } else if (m_caretPos < (int)getTextLength()) {
                    string sub = m_strings.substr(m_caretPos, -1);
                    int firstChLen = get_first_utf8_char_len(sub.c_str(), (int)getTextLength() - m_caretPos);
                    m_caretPos += firstChLen;
                    refreshCaretPos(false);
                    showCaret(true);
                    updateView();
                    return STOP_DISPATCH;
                }

                return GOON_DISPATCH;
            case KeyEvent::KEYCODE_CURSOR_UP :
                if(m_urlNumberParseEnable)
                {
                    updateHilightNumber(KeyEvent::KEYCODE_CURSOR_UP);
                    return TRUE;
                }
                else
                {
#ifdef __MMI_T9__
                    if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                    {
                        m_addword = false;
                        if(m_bSelect==true)
                        {
                            int count =get_utf8_char_count(m_strings.c_str(),m_caretPos);
                            imeT9KeyArrowUpHandlerMultilineInputBox(count);
                            imeRefreshString();
                            return STOP_DISPATCH;
                        }
                         else
                        {
                            int ret = switchLine(true);
                            int count =get_utf8_char_count(m_strings.c_str(),m_caretPos);
                            imeT9KeyArrowUpHandlerMultilineInputBox(count);

                            return ret;
                        }
                    }
                    else
#endif

                    {
                        return switchLine(true);
                    }
                    break;
                }
            case KeyEvent::KEYCODE_CURSOR_DOWN :
                if(m_urlNumberParseEnable)
                {
                    updateHilightNumber(KeyEvent::KEYCODE_CURSOR_DOWN);
                    return TRUE;
                }
                else
                {
#ifdef __MMI_T9__
                    if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                    {
                        m_addword = false;
                        if(m_bSelect==true)
                        {
                            int count =get_utf8_char_count(m_strings.c_str(),m_caretPos);
                            imeT9KeyArrowDownHandlerMultilineInputBox(count);
                            imeRefreshString();

                            if(imeT9IsAddwordByArrowkey())
                                entryAddWordMode();
                            return STOP_DISPATCH;
                        }
                        else
                        {
                           bool ret = switchLine(false);
                           int count =get_utf8_char_count(m_strings.c_str(),m_caretPos);
                           imeT9KeyArrowDownHandlerMultilineInputBox(count);

                           return ret;
                        }
                    }
                    else
#endif
                    {
                        return switchLine(false);
                    }
                }
            default :
                break;
        }
    }
    else if(event->eventType() == Event::KEY_UP)
    {
#ifdef __MMI_T9__
        char ch[2]={0, 0};
        ui_char_t temp;

        if ((ch[0] = _charactor(code)) > 0){
            if(m_prevEventType == Event::KEY_LONGPRESSED)
                return GOON_DISPATCH;

            ch[1] = '\0';
            if(imeGetInputModeFlag() == INPUT_MODE_SMART && ch[0] != '0')
            {
                ch[0] = '0' + code;

                if(::imeGetPreviousActiveMultitap() >= 0)
                {
                    imeDisableActiveMultitapWithoutDraw();
                    SynText();
                }

                if (isImeExist() && isImeEnable()&&
                     ch[0] >= '1' && ch[0] <= '9')
                {
                    if(m_IsCharCount)
                    {
                        unsigned int maxcount = get_utf8_char_count(m_strings.c_str(),m_strings.size());
                        if(maxcount >= m_txtLimit)
                            return STOP_DISPATCH;
                    }
                    else
                    {
                        if(getTextLength()>=m_txtLimit)
                            return STOP_DISPATCH;
                    }
                    if(m_bSelect == false)
                    {
                        m_bSelect =true;
                        m_startPosX=m_caretPosX;
                        m_startPosY=m_caretPosY;
                    }
                    showCaret(false);
                    setImeTarget((int)this, 1);
                    imeInsertChar(ch[0]);
                }
            }
            else if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP
                || (imeGetInputModeFlag() == INPUT_MODE_SMART && ch[0] == '0'))
            {
                unsigned char utf8[10] = {0};
                m_bSelect = false;

                if(::imeGetPreviousActiveMultitap() == code)
                {
                    temp =(Uint16)getMultitapCharCode(code);
                    if(temp == '\n')
                    {
                        replacePrevChar(NULL);
                        insertText("\n");
                    }
                    else
                    {
                        imeCheckMultitapPunctuationCharacter(m_strings,m_caretPos,true);
                        if(imeIsMultiShiftInputMode() &&
                            ((imeGetMultitapShiftMode()!= SHIFT_STATE_LOWER) || (imeGetPreMultitapShiftMode() == SHIFT_STATE_UPPER)))
                        {
                            temp=toupper(temp);
                        }
                        mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
                        replacePrevChar((char *)utf8);
                    }
                }
                else if(m_strings.length() < textMaxLimit()
                    || (m_IsCharCount && get_utf8_char_count(m_strings.c_str(),m_strings.size()) < (int)textMaxLimit()) )
                {
                    if(ch[0] == '0')
                    {
                        m_addword = false;
                        imeChangeKeyzeroMultitaps(true,code);
                    }
                    temp = (Uint16)getMultitapCharCode(code);
                    if(temp == '\n')
                            insertText("\n");
                    else
                    {
                        imeSetPreMultitapShiftMode(SHIFT_STATE_INIT);
                        if(imeIsMultiShiftInputMode() &&(imeGetMultitapShiftMode()!=SHIFT_STATE_LOWER))
                        {
                            temp=toupper(temp);
                            imeSetMultitapShiftMode(SHIFT_STATE_UPPER);
                            imeSetPreMultitapShiftMode(imeGetMultitapShiftMode());
                        }
                        mmi_chset_ucs2_to_utf8((unsigned char *)utf8,(unsigned short)temp);
                        insertText((const char *)utf8);
                    }
                }
                else if(m_multapTimerId != 0)
                {
                    removeTimer(m_multapTimerId);
                    imeDisableActiveMultitapWithoutDraw();
                    m_multapTimerId = 0;
                    blinkCaret();
                }
                else
                {
                    updateView();
                }
        if(imeGetInputModeFlag() == INPUT_MODE_SMART && ch[0] == '0')
            SynText();
            }
        }
#endif

        if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP
             || (imeGetInputModeFlag() == INPUT_MODE_SMART && code ==  KeyEvent::KEYCODE_0))
        {
            if((code >=  KeyEvent::KEYCODE_0) &&  (code<= KeyEvent::KEYCODE_9))
            {
                MultitapKeyupNoDraw();
            }
        }

        if (((code >=  KeyEvent::KEYCODE_0) && (code<= KeyEvent::KEYCODE_9))
            || code == KeyEvent::KEYCODE_CURSOR_LEFT
            || code == KeyEvent::KEYCODE_CURSOR_RIGHT
            || code == KeyEvent::KEYCODE_SOFTKEY_RIGHT)
        {
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP)
                imeCheckMultitapPunctuationCharacter(m_strings,m_caretPos,false);
            else if(imeGetInputModeFlag() == INPUT_MODE_SMART)
                imeT9CheckPunctuationCharacter(FALSE);
            imeSetSwitch();
#endif
        }
    }
    else if(event->eventType() == Event::KEY_LONGPRESSED) {
        m_prevEventType = event->eventType();

        if (code == KeyEvent::KEYCODE_0 && m_bAutoTrans) {
            replacePrevChar("+");
            return STOP_DISPATCH;
        } else if (code == KeyEvent::KEYCODE_STAR && m_bAutoTrans) {
            replacePrevChar("P");
            return STOP_DISPATCH;
        } else if (code == KeyEvent::KEYCODE_CURSOR_LEFT) {
            setCaretPosition(0);
            showCaret(true);
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                //m_bSelect = false;
                SynText();
            }
#endif
            updateView();
            return STOP_DISPATCH;
        } else if (code == KeyEvent::KEYCODE_CURSOR_RIGHT) {
            setCaretPosition(getTextLength());
            showCaret(true);
#ifdef __MMI_T9__
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                //m_bSelect = false;
                SynText();
            }
#endif
            updateView();
            return STOP_DISPATCH;
        }else if(code == KeyEvent::KEYCODE_SOFTKEY_RIGHT && m_strings.size() > 0){
            if(imeGetInputModeFlag() == INPUT_MODE_SMART){
                if(m_caretPos <0)
                    m_caretPos = 0;
#ifdef __MMI_T9__
                else
                    DeleteChar();
#endif
                blinkCaret();
            }else{
                replacePrevChar(NULL);
            }

            return STOP_DISPATCH;
        }
        else if(code >= KeyEvent::KEYCODE_0 && code <= KeyEvent::KEYCODE_9 && imeGetInputModeFlag() != INPUT_MODE_NUMBER)
        {
            char ch[2]={0, 0};
            ch[0] = _charactor(code);
            insertText(ch);

#ifdef __MMI_T9__
            m_bSelect = false;
            if(imeGetInputModeFlag() == INPUT_MODE_SMART)
            {
                m_bSelect = false;
                SynText();
            }
            else if (0 != m_multapTimerId)
            {
                imeDisableActiveMultitapWithoutDraw();
                removeTimer(m_multapTimerId);
                m_multapTimerId = 0;
                blinkCaret();
            }
#endif

            return STOP_DISPATCH;
        }
        return GOON_DISPATCH;
    }
    else if (event->eventType() == Event::KEY_ALWAYSPRESS) {
        if (code == KeyEvent::KEYCODE_SOFTKEY_RIGHT && m_strings.size() > 0) {
            if(imeGetInputModeFlag() == INPUT_MODE_SMART){
                if(m_caretPos <0)
                    m_caretPos = 0;
#ifdef __MMI_T9__
                else
                    DeleteChar();
#endif
                blinkCaret();
            }
            else {
                replacePrevChar(NULL);
            }

            return STOP_DISPATCH;
        }
        else if (code == KeyEvent::KEYCODE_CURSOR_DOWN) {
            return switchLine(false);
        }
        else if (code == KeyEvent::KEYCODE_CURSOR_UP) {
            return switchLine(true);
        }
        return GOON_DISPATCH;
    }
    else if(event->eventType() == Event::MOTION_DOWN) {
        //TODO mouse support
    }

    return GOON_DISPATCH;
}

bool MlEditView::switchLine(bool isUpLine)
{
    if (isUpLine) //goto prev line
    {
        if (m_caretPos <= 0)
            return GOON_DISPATCH;

        if (m_caretPosY > 0) {
            m_caretPosY--;
            refreshCaretPos(true);
        } else {
#ifdef __MMI_SPREADSPECTRUM_STYLE__
            return GOON_DISPATCH;
#else
            m_caretPosX = 0;
            m_caretPosY = 0;
            m_caretPos = 0;
#endif
        }
    }
    else {
        // goto next line
        if (m_caretPos >= (int)getTextLength())
            return GOON_DISPATCH;

        if (m_caretPosY < m_lineCout - 1) {
            m_caretPosY++;
            refreshCaretPos(true);
        } else {
            m_caretPos = (int)getTextLength();
            refreshCaretPos(false);
        }
    }

    raiseNotifyEvent(NGTIFY_EDIT_LINE_CHANGED, m_caretPosY, 0);
    showCaret(true);
    updateView();
    return STOP_DISPATCH;
}

void MlEditView::drawBackground(GraphicsContext* gc, IntRect &rc, int status)
{
    if(m_drset) {
        m_drset->draw(gc, DR_BKGND, isFocused() ? DRAWSTATE_HILIGHT : DRAWSTATE_NORMAL, rc);
    }
}

int MlEditView::hilightNumberPos(int nextHilightIndex, int *startPos, int *len)
{
    int i=0;
    int cutStartPos = 0,    cut_len = 0, CaretPosition = -1;
    int __table[MAX_HILIGHT_NUM_CNT];
    string str;
    memset(__table, 0x0, sizeof(__table));

    char * ptr = NULL;

    if(m_urlStringsLen < 0) // means parse all context, don't cut.
        ptr = (char*)m_strings.c_str();
    else {
        str         = m_strings;
        int  len     = m_strings.size();
        ptr         = (char *)str.c_str();
        memset(ptr+m_urlStringsLen, 0x0, len - m_urlStringsLen);
    }

    int numberCnt = URL_ParseDataGen(ptr, __table, MAX_HILIGHT_NUM_CNT);

    if(nextHilightIndex >= numberCnt)
        nextHilightIndex = numberCnt ;

    for(i = 0; i < numberCnt; i++) {
        cutStartPos     = (__table[i]&0xFFFF0000)>>16;
        cut_len         = (__table[i]&0xFFFF);
        //cutEndPos         = cutStartPos + cut_len;

        if(nextHilightIndex == i) {
            CaretPosition     =      cutStartPos;
            *startPos         =     cutStartPos;
            *len              =    cut_len;
        }
    }

    return CaretPosition;

}

void MlEditView::raiseHilightNumberNotify(bool isSelect)
{
    if(isSelect) {
        char number[40+1];
        int __table[MAX_HILIGHT_NUM_CNT];

        memset(__table, 0x0, sizeof(__table));
        memset(number, 0x0, sizeof(number));
        char * ptr = (char*)(m_strings.c_str());
        int startPos = (__table[m_hilightNumberIndex] >>16)&0xFFFF;
        int len = __table[m_hilightNumberIndex] &0xFFFF;
        memcpy(number, ptr+startPos, len);
        raiseNotifyEvent(NOTIFY_EDIT_HILIGHT_NUMBER_FOCUS, isSelect, (int)number);
    }
    else {
        raiseNotifyEvent(NOTIFY_EDIT_HILIGHT_NUMBER_FOCUS, isSelect, (int)0);
    }
}

int MlEditView::queryLineIndex(int pos)
{
    unsigned int i;
    int lineStart = 0, lineEnd = 0;

    IntList::iterator it = m_lineCharList.begin();
    for(i =0; i< m_lineCharList.size(); i++) {
        lineEnd += (*it);

        if(pos >= lineStart && pos < lineEnd)
            return i;

        lineStart += (*it);
        it ++;
    }

    return 0;
}

int MlEditView::updateHilightNumber(int keyCode)
{
    if(!m_urlNumberParseEnable)
        return -1;

    int __table[MAX_HILIGHT_NUM_CNT];

    memset(__table, 0x0, sizeof(__table));
    unsigned int startLine, endLine, startCharPos = 0, endCharPos = 0;
    int w, h;

    getSize(&w, &h);
    int fontH = m_font->size; //GraphicsContext::screenGraphics()->getFontHeight(m_font);
    int lineH = fontH + m_lineAboveH + m_lineBellowH;
    int cnt = (h- m_vMargin)/lineH;

    char * ptr = NULL;
    string str;

    if(m_urlStringsLen < 0) // means parse all context, don't cut.
        ptr = (char*)m_strings.c_str();
    else {
        str = m_strings;
        int  len = m_strings.size();
        ptr = (char *)str.c_str();
        memset(ptr+m_urlStringsLen, 0x0, len - m_urlStringsLen);
    }

    int numberCnt = URL_ParseDataGen(ptr, __table, MAX_HILIGHT_NUM_CNT);
    int curHilightIdx;
    if(numberCnt > 0) {
        bool isChange = FALSE;

        if(m_startShowLine > 0) {
            startLine = m_startShowLine ;
            endLine   = m_startShowLine + cnt;
            if(endLine > m_lineCharList.size())
                endLine = m_lineCharList.size();
        }
        else {
            startLine = 0;
            endLine   = cnt;
        }

        unsigned int i;
        IntList::iterator it = m_lineCharList.begin();
        for(i =0; i< m_lineCharList.size(); i++) {
            if(i < startLine) {
                startCharPos += (*it);
            }

            if(i < endLine) {
                endCharPos += (*it);
            }

            if(i== endLine)
                break;
            it ++;
        }

        if(keyCode == KeyEvent::KEYCODE_CURSOR_UP) {
            if(!m_hilightValid) {
                if(m_hilightNumberIndex >= numberCnt) {
                    m_hilightNumberIndex = numberCnt -1;
                }
                curHilightIdx = m_hilightNumberIndex;

                if(curHilightIdx >= 0) {

                    int pos, len;
                    unsigned int nextPos = hilightNumberPos(curHilightIdx, &pos, &len);

                    if(nextPos>= startCharPos && nextPos<= endCharPos) {

                        if(nextPos + len > endCharPos) {
                            int line = queryLineIndex(nextPos + len);
                            m_startShowLine = line;
                            _DBG_PRINTF ("MlEditView::updateHilightNumber_1 > m_startShowLine: %d", m_startShowLine);
                        }
                        m_hilightValid = TRUE;
                        isChange = TRUE;
                    }
                    else if(nextPos < startCharPos && nextPos + len >= startCharPos) {

                        isChange = TRUE;
                        m_hilightValid = TRUE;
                        m_startShowLine -= (cnt -1);
                        _DBG_PRINTF ("MlEditView::updateHilightNumber_2 > m_startShowLine: %d", m_startShowLine);
                    }
                }

            }
            else {
                int pos, len;
                curHilightIdx = m_hilightNumberIndex -1;

                if(curHilightIdx >= 0) {
                    unsigned int startPos = hilightNumberPos(curHilightIdx, &pos, &len);

                    if(startPos>= startCharPos && startPos <= endCharPos) {
                        m_hilightNumberIndex = curHilightIdx;
                        isChange = TRUE;
                    }
                    else if(startPos < startCharPos && (startPos + len) >= startCharPos) {
                        m_startShowLine -= (cnt -1);
                        _DBG_PRINTF ("MlEditView::updateHilightNumber_3 > m_startShowLine: %d", m_startShowLine);
                        m_hilightNumberIndex = curHilightIdx;
                        isChange = TRUE;
                        m_hilightValid = TRUE;
                    }
                    else {

                        int curPos = hilightNumberPos(m_hilightNumberIndex, &pos, &len);
                        unsigned int line = queryLineIndex(curPos);
                        if(line == (m_startShowLine + cnt -1)) {
                            m_startShowLine -= (cnt -1);
                            _DBG_PRINTF ("MlEditView::updateHilightNumber_4 > m_startShowLine: %d", m_startShowLine);
                            updateView();
                            return TRUE;
                        }
                    }
                }
            }
        }
        else if(keyCode == KeyEvent::KEYCODE_CURSOR_DOWN) {

            if(!m_hilightValid) {
                curHilightIdx = m_hilightNumberIndex;
                if(curHilightIdx < 0)
                    curHilightIdx = 0;

                int pos, len;
                unsigned int nextPos = hilightNumberPos(curHilightIdx, &pos, &len);
                if(nextPos>= startCharPos && nextPos <= endCharPos) {
                    if(nextPos + len > endCharPos) {
                        int line = queryLineIndex(nextPos);
                        m_startShowLine = line;
                        _DBG_PRINTF ("MlEditView::updateHilightNumber_5 > m_startShowLine: %d", m_startShowLine);
                        if(m_startShowLine + cnt > m_lineCharList.size())
                            m_startShowLine = m_lineCharList.size() - cnt;
                    }

                    isChange = TRUE;
                    m_hilightNumberIndex  = curHilightIdx;
                    m_hilightValid = TRUE;
                }
            }
            else {
                curHilightIdx = m_hilightNumberIndex +1;
                int pos, len;
                unsigned int nextPos = hilightNumberPos(curHilightIdx, &pos, &len);

                if (nextPos>= startCharPos && nextPos <= endCharPos) {

                    if(nextPos + len > endCharPos) {
                        int line = queryLineIndex(nextPos);
                        m_startShowLine = line;
                        _DBG_PRINTF ("MlEditView::updateHilightNumber_6 > m_startShowLine: %d", m_startShowLine);
                        if(m_startShowLine + cnt > m_lineCharList.size()) {
                            m_startShowLine = m_lineCharList.size() - cnt;
                            _DBG_PRINTF ("MlEditView::updateHilightNumber_7 > m_startShowLine: %d", m_startShowLine);
                        }
                    }
                    m_hilightNumberIndex = curHilightIdx;
                    isChange = TRUE;
                }
                else {
                    if(m_startShowLine + cnt < m_lineCharList.size()) {

                        int curPos = hilightNumberPos(m_hilightNumberIndex, &pos, &len);
                        unsigned int line = queryLineIndex(curPos);
                        if(line == (m_startShowLine + cnt -1)) {
                            m_startShowLine = line ;
                            _DBG_PRINTF ("MlEditView::updateHilightNumber_8 > m_startShowLine: %d", m_startShowLine);
                            updateView();
                            return TRUE;
                        }
                    }
                }

            }
        }

        if(isChange == TRUE) {
            raiseHilightNumberNotify(TRUE);
            updateView();
            return TRUE;
        }
    }

    if(keyCode == KeyEvent::KEYCODE_CURSOR_UP) {
        m_startShowLine = m_startShowLine -(cnt-1);
        _DBG_PRINTF ("MlEditView::updateHilightNumber_9 > m_startShowLine: %d", m_startShowLine);
        if (m_startShowLine < 0) {
            m_startShowLine = 0;
        }
    }
    else {
        if (cnt + m_startShowLine < m_lineCharList.size()) {
            m_startShowLine = m_startShowLine + (cnt - 1);
            if (m_startShowLine + cnt > m_lineCharList.size()) {
                m_startShowLine = m_lineCharList.size() - cnt;
                _DBG_PRINTF ("MlEditView::updateHilightNumber_10 > m_startShowLine: %d", m_startShowLine);
            }
        }

    }

    if(m_hilightValid) {
        if(keyCode == KeyEvent::KEYCODE_CURSOR_UP && m_hilightNumberIndex >= 0)
            m_hilightNumberIndex --;
        else
            m_hilightNumberIndex ++;

        m_hilightValid = FALSE;
        raiseHilightNumberNotify(FALSE);
    }

    updateView();
    return 0;
}

void MlEditView::drawHilightNumber(GraphicsContext* gc, int x, int y, int startCharIdx, int charCnt, char* lineStrPtr)
{
    int i=0;
    int startChar, endChar;
    int subTable[20];
    bool hilightBkgTable[20];
    char hilightStrTable[20][40];

    int subTableCnt =0;
    string str;
    int lineLen = charCnt;
    bool isDrawHilightBkg = FALSE;
    int cutStartPos = 0,  cut_len = 0, cutEndPos =0; //, CaretPosition = 0;

    int __table[MAX_HILIGHT_NUM_CNT];
    int fontH = gc->getFontHeight(m_font);

    memset(__table, 0x0, sizeof(__table));
    memset(hilightStrTable, 0x0, sizeof(hilightStrTable));
    char * ptr = NULL;

    if(m_urlStringsLen < 0) // means parse all context, don't cut.
        ptr = (char*)m_strings.c_str();
    else {
        str = m_strings;
        int  len = m_strings.size();
        ptr = (char *)str.c_str();
        memset(ptr+m_urlStringsLen, 0x0, len - m_urlStringsLen);
    }

    int numberCnt = URL_ParseDataGen(ptr, __table, MAX_HILIGHT_NUM_CNT);

    startChar = startCharIdx;
    endChar   = startCharIdx + charCnt;

    memset(subTable, 0x0, sizeof(subTable));
    memset(hilightBkgTable, 0x0, sizeof(hilightBkgTable));

    for(i = 0; i < numberCnt; i++) {
        cutStartPos = (__table[i]&0xFFFF0000)>>16;
        int _x = 0, _w = 0, _h = 0;
        int off; //, copy_len = 0;
        char buf[256];
        char sub_buf[256];
        char * _ptr = (char*)lineStrPtr;

        memset(buf, 0x0, sizeof(buf));
        memset(sub_buf, 0x0, sizeof(sub_buf));

        cut_len = (__table[i]&0xFFFF);
        cutEndPos = cutStartPos + cut_len;

        if(cutStartPos > endChar)
            continue;
        else if(cutEndPos < startChar)
            continue;
        else if(cutStartPos >= startChar && cutEndPos <= endChar) {
            if(cutStartPos == startChar) {
                off = 0;
                memcpy(buf, _ptr, cut_len);
                //copy_len = cut_len;
                gc->getTextDrawSize(buf, m_font, &_w, &_h);
            }
            else {
                memcpy(sub_buf, _ptr, (cutStartPos - startChar));
                gc->getTextDrawSize(sub_buf, m_font, &off, &_h);
                _ptr = _ptr + (cutStartPos - startChar);
                memcpy(buf, _ptr, cut_len);
                //copy_len = cut_len;
                gc->getTextDrawSize(buf, m_font, &_w, &_h);
            }
            _x = off + x;

            subTable[subTableCnt] = ((_x&0xFFFF)<<16)| (_w&0xFFFF);

        }
        else if(cutStartPos >= startChar && cutEndPos > endChar) {
            if(cutStartPos == startChar) {
                off = 0;
                memcpy(buf, _ptr, lineLen);
                //copy_len = lineLen;
                gc->getTextDrawSize(buf, m_font, &_w, &_h);
            }
            else {
                memcpy(sub_buf, _ptr, (cutStartPos - startChar));
                gc->getTextDrawSize(sub_buf, m_font, &off, &_h);
                _ptr = _ptr + (cutStartPos - startChar);
                memcpy(buf, _ptr, lineLen - (cutStartPos - startChar));
                gc->getTextDrawSize(buf, m_font, &_w, &_h);
            }
            _x = off + x;

            subTable[subTableCnt] = ((_x&0xFFFF)<<16)| (_w&0xFFFF);

        }
        else if(cutStartPos <= startChar && cutEndPos <= endChar) {
            off = 0;
            memcpy(buf, _ptr, cut_len -(startChar - cutStartPos));
            gc->getTextDrawSize(buf, m_font, &_w, &_h);

            _x += off + x;

            subTable[subTableCnt] = ((_x& 0xFFFF)<<16) | (_w&0xFFFF);
        }
        else if(cutStartPos <= startChar && cutEndPos >= endChar) {
            off = 0;
            gc->getTextDrawSize(_ptr, m_font, &_w, &_h);
            memcpy(buf, _ptr, cut_len);
            _x += off +x;
            subTable[subTableCnt] = ((_x& 0xFFFF)<<16) | (_w&0xFFFF);
        }
        else {
            continue;
        }

        if(m_hilightNumberIndex == i) {
            hilightBkgTable[subTableCnt] = TRUE;
            strcpy(hilightStrTable[subTableCnt], buf);
            //CaretPosition = cutEndPos;
        }
        subTableCnt++;

        if(m_hilightValid)
            isDrawHilightBkg = TRUE ;
    }

    if(subTableCnt > 0) {

        int idx =0;
        for(idx =0; idx < subTableCnt; idx ++) {

            int _x1        = (subTable[idx]&0xFFFF0000)>>16;
            int _width    = subTable[idx]&0xFFFF;

            if(isDrawHilightBkg && hilightBkgTable[idx]) {
                RECT rc = {_x1, y, _x1 + _width, y + (int)m_lineAboveH + fontH};
                gc->fillRect(rc,
                            (Color::GRAY >> 16) & 0xFF,
                            (Color::GRAY >> 8) & 0xFF,
                            (Color::GRAY >> 0) & 0xFF,
                            (Color::GRAY >> 24) & 0xFF);
                gc->drawText( hilightStrTable[idx], rc,Color::WHITE, m_font, DT_LEFT | DT_TOP);

            //    gc->textOut(_x1, y, hilightStrTable[idx], Color::WHITE, m_font);
            }
            gc->drawLine(_x1 , y + m_lineAboveH + fontH-2, _x1+_width ,  y + m_lineAboveH + fontH-2, 1,
                            (m_txtColor >> 16) & 0xFF,
                            (m_txtColor >> 8) & 0xFF,
                            (m_txtColor >> 0) & 0xFF,
                            (m_txtColor >> 24) & 0xFF);
        }
    }
}

void MlEditView::drawContent(GraphicsContext* gc, IntRect &rc, int status)
{
    Point caretPos = {0, 0};
    IntRect irc ( rc.left() + m_hMargin, rc.top() + m_vMargin,
            rc.right() - m_hMargin, rc.bottom() - m_vMargin);

    gc->setTextAboveLineExtra(m_lineAboveH);
    gc->setTextBellowLineExtra(m_lineBellowH);

    int fontH = gc->getFontHeight(m_font);
    int lineH = fontH + m_lineAboveH + m_lineBellowH;
    int len = m_strings.size();
    int caretWidth = 0;

    if (len <= 0) {
        // only draw the caret
        caretPos.x = m_hMargin;
        caretPos.y = m_vMargin;

        if(imeIsArabic()) {
            caretPos.x = rc.right() - rc.left() - caretPos.x;
        }
    }
    else {
        unsigned int start = 0;
        unsigned int lineNo = 0;
        int x = irc.left();
        int y = irc.top();

        _DBG_PRINTF ("MleditView: drawContent m_caretPosY: %d, m_startShowLine: %d", m_caretPosY, m_startShowLine);
        if(m_urlNumberParseEnable) {
             m_caretPosY = m_startShowLine;
        }
        else {
            // calculator which line to show
            if (m_caretPosY > (int)m_startShowLine) {
                int rcH = irc.height();
                while ((m_caretPosY - (int)m_startShowLine + 1) * lineH > rcH) {
                    m_startShowLine++;
                }
            }
            else {
                m_startShowLine = m_caretPosY;
            }
        }

        IntList::iterator it = m_lineCharList.begin();
#ifdef __MMI_T9_ARABIC__
        int *info = (int *)malloc ((len + 1) * sizeof(int));
        int realCount = get_utf_chars_info (m_strings.c_str(), len, info);
        info [realCount] = len;
        for (int i = 1; i <= realCount; /*i++*/) {

            memcpy(tmpchar, str + info[i-1], info[i] - info[i-1]);
            tmpchar [info[i] - info[i-1]] = '\0';
            if (IsSymbolUCS2Char(UTF8ToUCS2((Uint8 *)tmpchar))
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
                || IsArabicSymbolUCS2Char(UTF8ToUCS2((Uint8 *)tmpchar))) {
            SetBiDiFlag(true);
            setExchangeFlag(true);
        }
        else {
            SetBiDiFlag(false);
            setExchangeFlag(false);
        }
#endif

        while(it != m_lineCharList.end() && y <= irc.bottom() - lineH) {

            if (lineNo >= m_startShowLine) {

                string lineStr;
                IntRect text_rc( rc.left() + m_hMargin, y,
                                rc.right() - m_hMargin, rc.bottom() - m_vMargin);
                if (m_strings.compare(start + *it - 1, 1, "\n") == 0) {
                    lineStr = *it > 1 ? m_strings.substr(start, *it - 1) : "";
                }
                else {
                    lineStr = m_strings.substr(start, *it);
                }

                _DBG_PRINTF ("MleditView: drawText (%s) for line %d", lineStr.c_str(), lineNo);

                // Draw a Line Text
                /*
                 * TODO
                 * Now, the ommitted style only used by Dial Screen,
                 *      and must used the DIAL font.
                 * For other fonts , should to do :
                 *  - re calculate the width of "..."
                 *  - and cut the sub string in one line by the above width .
                 */
                if ((m_style & EDVS_OMITTED)
                        && m_startShowLine > 0
                        && lineNo == m_startShowLine) {

                    //first line, omitted head
                    string oStr = ".";
                    oStr += lineStr.substr(1, lineStr.length() - 1);
                    gc->drawText(oStr, text_rc, m_txtColor, m_font, DT_LEFT | DT_TOP);
                }
                else {
                    // normal line
                    gc->drawText(lineStr, text_rc, m_txtColor, m_font, DT_LEFT | DT_TOP);
                    if(m_urlNumberParseEnable) {
                        string str = lineStr;
                        drawHilightNumber(gc, x, y, start, *it, (char*)(str.c_str()));
                    }
                }

                if(!isReadOnly()) {
#ifdef __MMI_T9__
                    if(m_bSelect) {
                        int w = 0, h;
                        int activeWordLen = 0;
                        int activeWordCount = imeT9GetActiveWordCount();
                        int i;
                        int total = 0;
                        Uint16 visIndex = 0xffff;
                        Uint16 visual_str_len, charDir;
                        Uint16 tmpChar[2] = {0,};
                        int unicodeStartPosX, unicodeCaretPosX;

                        memset(m_edit_tempstr1,0,EDITOR_MAX_LEN);
                        strcpy(m_edit_tempstr1, m_strings.c_str());

                        if(activeWordCount >0) {
                            while(activeWordCount) {
                                int lastChLen = get_last_utf8_char_len(m_edit_tempstr1, strlen(m_edit_tempstr1));
                                activeWordLen += lastChLen;
                                m_edit_tempstr1[m_caretPos - activeWordLen] = '\0';
                                activeWordCount--;
                            }
                            total = m_caretPos - activeWordLen;

                            m_startPosX = 0;
                            m_startPosY = 0;

                            for(IntList::iterator it = m_lineCharList.begin(); it != m_lineCharList.end(); it++) {
                                if (total >= *it) {
                                    total -= *it;
                                    m_startPosY++;
                                }
                                else {
                                    m_startPosX = total;
                                    if (m_startPosX == (*it)) {
                                        if (m_strings.compare(m_caretPos - 1, 1, "\n") == 0) {
                                            m_startPosY++;
                                            m_startPosX = 0;
                                        }
                                    }
                                    break;
                                }
                            }

                            if (m_startPosY <= (int)lineNo && (int)lineNo <= m_caretPosY) {
                                // lineStr is occasionally the value changes by get_utf8_char_count()
                                strcpy(m_edit_tempstr2, lineStr.c_str());
                                unicodeStartPosX = get_utf8_char_count(m_edit_tempstr2, m_startPosX);
                                strcpy(m_edit_tempstr2, lineStr.c_str());
                                unicodeCaretPosX = get_utf8_char_count(m_edit_tempstr2, m_caretPosX);

                                memset(m_edit_tempstr2, 0x0, sizeof(m_edit_tempstr2));
                                NGUtf8ToUnicode((unsigned char*)lineStr.c_str(), (unsigned char*)m_edit_tempstr2);

                                visual_str_len = UCS2Strlen(m_edit_tempstr2);

                                ConvertToGlyphForm((Uint16*)m_edit_tempstr2, visual_str_len, 0);

                                memset(bidiStrOrder, 0x00, sizeof(bidiStrOrder));
                                getBidiString((Uint16*)m_edit_tempstr2, bidiStrOrder, visual_str_len, gc->getBiDiFlag(), TRUE);

                                if(gc->getBiDiFlag()) {
                                    gc->getTextDrawSize((unsigned short*)m_edit_tempstr2, m_font, &w, &h);
                                    w = irc.right() - irc.left() - w;
                                }

                                caretPos.x = irc.left() + w;
                                caretPos.y = y;
                                caretWidth = 0;

                                if (/*m_startPosY == m_caretPosY ||*/ m_startPosY == (int)lineNo) {
                                    if (getBidiTypeDirection(((unsigned short*)m_edit_tempstr2), unicodeStartPosX, visual_str_len, gc->getBiDiFlag()) == 1)
                                        charDir = 1;
                                    else
                                        charDir = 0;
                                    visIndex = getBidiVisualIndex(((unsigned short*)m_edit_tempstr2), bidiStrOrder, visual_str_len, unicodeStartPosX+1, gc->getBiDiFlag(), charDir);


                                    for(i=0; i<visIndex; i++) {
                                        tmpChar[0] = ((unsigned short*)m_edit_tempstr2)[bidiStrOrder[i]-1];
                                        gc->getTextDrawSize(tmpChar, m_font, &w, &h);
                                        caretPos.x += w;
                                    }

                                    if(m_startPosY == m_caretPosY) {
                                        for(i=unicodeStartPosX; i<unicodeCaretPosX; i++) {
                                            tmpChar[0] = ((unsigned short*)m_edit_tempstr2)[i/*bidiStrOrder[i]-1*/];
                                            gc->getTextDrawSize(tmpChar, m_font, &w, &h);
                                            caretWidth += w;
                                        }
                                    }
                                    else {
                                        for(i=unicodeStartPosX; i<visual_str_len; i++) {
                                            tmpChar[0] = ((unsigned short*)m_edit_tempstr2)[i/*bidiStrOrder[i]-1*/];
                                            gc->getTextDrawSize(tmpChar, m_font, &w, &h);
                                            caretWidth += w;
                                        }
                                    }

                                    if (getBidiTypeDirection(((unsigned short*)m_edit_tempstr2), unicodeStartPosX, visual_str_len, gc->getBiDiFlag()) == 1)
                                        caretPos.x -= caretWidth;
                                }
                                else if ((int)lineNo == m_caretPosY) {
                                    if (unicodeCaretPosX > 0) {
                                        if (getBidiTypeDirection(((unsigned short*)m_edit_tempstr2), unicodeCaretPosX-1, visual_str_len, gc->getBiDiFlag()) == 1)
                                            charDir = 0;
                                        else
                                            charDir = 1;
                                    }
                                    else {
                                        charDir = 1;
                                    }
                                    visIndex = getBidiVisualIndex(((unsigned short*)m_edit_tempstr2), bidiStrOrder, visual_str_len, unicodeCaretPosX, gc->getBiDiFlag(), charDir);

                                    for(i=0; i< unicodeCaretPosX; i++) {
                                        tmpChar[0] = ((unsigned short*)m_edit_tempstr2)[i/*bidiStrOrder[i]-1*/];
                                        gc->getTextDrawSize(tmpChar, m_font, &w, &h);
                                        caretWidth += w;
                                    }
                                }
                                else {
                                    gc->getTextDrawSize((unsigned short*)m_edit_tempstr2, m_font, &w, &h);
                                    caretWidth += w;
                                }

                                int oldrop = SetRasterOperation (gc->context(), ROP_XOR);

                                m_caretRect.setRect (caretPos.x, caretPos.y,
                                    caretPos.x + caretWidth, caretPos.y + fontH);

                                gc->fillRect(m_caretRect, GetRValue(m_txtColor), GetGValue(m_txtColor), GetBValue(m_txtColor), GetAValue(m_txtColor));

                                SetRasterOperation (gc->context(), oldrop);
                            }
                        }
                    }
                    else
#endif
                    {
                        // Calc caret Pos
                        if (lineNo == (unsigned int)m_caretPosY)
                        {
                            //FIXME , last is '\n' ?
                            Uint16 visIndex = 0xffff;
                            Uint16 visual_str_len;
                            int index =0;
                            int utf8len =0;

                            if(lineStr.size() > 0)
                            {
                                utf8len = lineStr.size();
                                if(m_caretPosX > utf8len)
                                        m_caretPosX = utf8len;
                                index =get_utf8_char_count(lineStr.c_str(),m_caretPosX);

                                memset(m_edit_tempstr2, 0x0, sizeof(m_edit_tempstr2));
                                NGUtf8ToUnicode((unsigned char*)lineStr.c_str(), (unsigned char*)m_edit_tempstr2);

                                visual_str_len = UCS2Strlen(m_edit_tempstr2);

                                ConvertToGlyphForm((Uint16*)m_edit_tempstr2, visual_str_len, 0);

                                memset(bidiStrOrder, 0x00, sizeof(Uint16) * EDITOR_MAX_LEN);
                                getBidiString((Uint16*)m_edit_tempstr2, bidiStrOrder, visual_str_len, gc->getBiDiFlag(), TRUE);
                                visIndex = getBidiVisualIndex((Uint16*)m_edit_tempstr2, bidiStrOrder, visual_str_len, index, gc->getBiDiFlag(), (MMI_active_multitap<0)?2:0);
                            }

                            if (visIndex == 0xffff) {
                                if(gc->getBiDiFlag())
                                    caretPos.x += irc.right() - irc.left();
                                else
                                    caretPos.x = irc.left();
                                caretPos.y = m_vMargin + y;
                            }
                            else {
                                int w = 0, h;
                                Uint16 tmpChar[2] = {0, };

                                caretPos.x += irc.left();
                                caretPos.y = y;

                                if(gc->getBiDiFlag()) {
                                    gc->getTextDrawSize((unsigned short*)m_edit_tempstr2, m_font, &w, &h);
                                    caretPos.x += irc.right() - irc.left() - w;
                                }

                                for(int i=0; i<visIndex; i++) {
                                    tmpChar[0] = ((unsigned short*)m_edit_tempstr2)[bidiStrOrder[i]-1];
                                    gc->getTextDrawSize(tmpChar, m_font, &w, &h);
                                    caretPos.x += w;
                                }

                                if (MMI_active_multitap >= 0) {
                                    tmpChar[0] = ((unsigned short*)m_edit_tempstr2)[index-1];
                                    gc->getTextDrawSize(tmpChar, m_font, &w, &h);
                                    caretWidth = w;
                                }
                            }
                        }
                    }
                }
                y += lineH;
            }
            start += *it;
            lineNo++;
            it++;
        }
    }

    if (m_style & EDVS_BASELINE) {
        for (int y = irc.top() + lineH; y < irc.bottom() ; y += lineH){
            gc->drawHVDotLine(irc.left(), y, irc.right(), y,
                    (m_txtColor >> 16) & 0xFF,
                    (m_txtColor >> 8) & 0xFF,
                    (m_txtColor >> 0) & 0xFF,
                    (m_txtColor >> 24) & 0xFF);
        }
    }

    caretPos.y += m_lineAboveH;

    if (isFocused() && m_bCaretShown && !isReadOnly()){
        int oldrop = 0;
        if(caretWidth) {
            oldrop = SetRasterOperation (gc->context(), ROP_XOR);
        }
        else {
            caretPos.y++;
            fontH--;
            if(m_caretPos != (int)getTextLength()) // P130819-02596
                caretPos.x--;
            caretWidth++;
        }

        m_caretRect.setRect (caretPos.x, caretPos.y,
                caretPos.x + caretWidth, caretPos.y + fontH);

        gc->fillRect(m_caretRect, GetRValue(m_txtColor), GetGValue(m_txtColor), GetBValue(m_txtColor), GetAValue(m_txtColor));

        if(caretWidth)
            SetRasterOperation (gc->context(), oldrop);
    }

#ifdef __MMI_T9_ARABIC__
    SetBiDiFlag(IsArabicSet());
#endif
   //drawScroll(gc, rc, status);
}


bool MlEditView::handler(Event* event)
{
    if (event->eventType() == Event::TIMER
            && m_timerId == ((TimerEvent *)event)->timerID()
            && isFocused()) {
#ifdef __MMI_T9__
       if(m_bSelect==false && m_multapTimerId == 0)
#endif
            blinkCaret();
    }

    if (event->eventType() == Event::TIMER
        && m_multapTimerId == ((TimerEvent *)event)->timerID()) {

        imeDisableActiveMultitapWithoutDraw();
        if (imeGetPreMultitapShiftMode() == SHIFT_STATE_UPPER) {
            imeSetPreMultitapShiftMode(SHIFT_STATE_INIT);
#ifdef __MMI_T9__
            imeSetSwitch();
#endif
        }

        removeTimer(m_multapTimerId);
        m_multapTimerId = 0;
#ifdef __MMI_T9__
        if(m_bSelect==false)
#endif
            blinkCaret();
    }

    return true;
}

int MlEditView::setCaretPosition(int pos)
{
    int old = m_caretPos;
    m_caretPos = pos;
#ifdef __MMI_T9__
    if (imeGetInputModeFlag() == INPUT_MODE_SMART) {
        imeT9SetUnicodeCursor(get_utf8_char_count(m_strings.c_str(), pos));
    }
#endif

    refreshCaretPos(false);
    return old;
}

void MlEditView::drawScroll (GraphicsContext* gc, IntRect &rc, int status)
{
    if(!enableScroll() || !m_font )
        return;

    int w, h;
    IntRect _rc, bkgRc;

    getSize(&w, &h);

    int fontH         = m_font->size; // GraphicsContext::screenGraphics()->getFontHeight(m_font);
    int lineH         = fontH + m_lineAboveH + m_lineBellowH;

    int showCnt         = (h- m_vMargin)/lineH;
    int lines         = m_lineCharList.size();
    int scrollWith     = DEFAULT_EDIT_SCROLL_W;

    if(lines <= showCnt)
        return ;

    int posY;

    if(m_startShowLine > 0)
        posY = rc.top() + (m_startShowLine*100*h/lines)/100;
    else
        posY = rc.top();

    int foreH = showCnt*h/lines;

    if((int)m_startShowLine + showCnt >= lines) {
        if(gc->getBiDiFlag())
            _rc.setRect(rc.left(), posY, rc.left() +scrollWith/2, h);
        else
            _rc.setRect(w - scrollWith, posY, w-scrollWith/2, h);
    }
    else {
        if(gc->getBiDiFlag())
            _rc.setRect(rc.left(), posY, rc.left() +scrollWith/2, posY +foreH);
        else
            _rc.setRect(w - scrollWith, posY, w-scrollWith/2, posY +foreH);
    }
    _rc.offset(1,0);

    if(gc->getBiDiFlag())
        bkgRc.setRect(rc.left(), rc.top(), rc.left()+ scrollWith, rc.bottom());
    else
        bkgRc.setRect(w - scrollWith, rc.top(), w, rc.bottom());

    m_drset->draw(gc, DR_BGVSCROLLBAR, DRAWSTATE_NORMAL, bkgRc);
    m_drset->draw(gc, DR_VSCROLLBAR, DRAWSTATE_NORMAL, _rc);
//    gc->fillRect(bkgRc,0x80,0x00,0x00,0xFF);
//    gc->fillRect(_rc,0x80,0x80,0x80,0xFF);
}

} // namespace hfcl
