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


#include "ngux.h"
#include "imeinterface.h"
#include "imebase.h"

namespace hfcl {

#define DEFAULT_H_MARGIN    1
#define DEFAULT_V_MARGIN    1

#define DEF_TXT_COLOR       0xFFFFFFFF

DEFINE_CLASS_NAME(EditView)

EditView::EditView(View *p)
    : View (p)
    , m_tipText("")
    , m_align(TextAlignLeft | TextVAlignCenter)
    , m_txtLimit(-1)
    , m_style(0)
    , m_hMargin(DEFAULT_H_MARGIN)
    , m_vMargin(DEFAULT_V_MARGIN)
    , m_txtColor(DEF_TXT_COLOR)
    , m_bCaretShown(true)
    , m_bInsertSpace(true)
    , m_bAutoTrans(false)
    , m_beditorflag(false)
#ifdef __MMI_T9__	
    , m_addword (false)
#endif
    , m_exchange(false)	
    , m_input_mode(-1)
    , m_default_input_mode(0)
    , m_writing_language(-1)
{
	adaptViewStyle();
}

EditView::EditView(View *p, DrawableSet *drs)
    : View (p, drs)
    , m_tipText("")
    , m_align(TextAlignLeft | TextVAlignCenter)
    , m_txtLimit(-1)
    , m_style(0)
    , m_hMargin(DEFAULT_H_MARGIN)
    , m_vMargin(DEFAULT_V_MARGIN)
    , m_txtColor(DEF_TXT_COLOR)
    , m_bCaretShown(true)
    , m_bInsertSpace(true)
    , m_bAutoTrans(false)
    , m_beditorflag(false)
#ifdef __MMI_T9__	
    , m_addword (false)
#endif
    , m_exchange(false)		
    , m_input_mode(-1)
    , m_default_input_mode(0)
    , m_writing_language(-1)
{
	adaptViewStyle();
}

EditView::~EditView()
{
    //TODO
    setImeTarget((int)NULL, 2);
}

void EditView::adaptViewStyle(void)
{
    setTextColor(m_drset->getDrawableElement(DR_CONTENT, 
                isFocus() ? SYS_SE_HILIGHTCOLOR : SYS_SE_COLOR));
}

void EditView::changeTheme(void)
{
	View::changeTheme();
	adaptViewStyle();
}

void EditView::raiseNotifyEvent(int notiCode, int exParam1, int exParam2)
{
    CustomEvent e(Event::CUSTOM_NOTIFY, notiCode, (int)this);
	e.setExParam(exParam1, exParam2);
    raiseEvent (&e);
}

void EditView::onGetFocus(void)
{
	View::onGetFocus();
	//adaptViewStyle();
    setImeTarget((int)this, 1);

#ifdef __MMI_T9__
    if(m_input_mode != 0xffff)
    {
        if(m_writing_language == imeGetLanguage() || m_writing_language == 0xffff)
        {
            imeChangeInputboxMode (m_input_mode);

            if(getTextLength() > 0)
            {
                if((m_input_mode & 0x8000) == FALSE) // From inputmethod menu?
                    SwitchShiftToLower(getText(),getTextLength(),true);
                ResetT9Mode();
            }
        }
        else
        {
            m_writing_language = imeGetLanguage();
            if(isABCMode() == FALSE)
#ifdef ENABLE_NVRAM
                imeSetInputModeFlag (ReadByteValue(NVRAM_IME_INPUTMODEFLAG));
#else
                imeSetInputModeFlag (0);
#endif
            imeSetLanguage(m_writing_language);
            m_input_mode = imeGetCurrentInputMode();
        }
    }
#endif
}

void EditView::onLoseFocus(void)
{
    showCaret (false);

	View::onLoseFocus();
	//adaptViewStyle();

    setImeTarget((int)NULL, 1);	
}

void EditView::setInputMode(Uint16 mode)
{
    m_input_mode = mode;
    m_writing_language = imeGetLanguage();
}

ui_char_t  EditView::getMultitapCharCode(char chr)
{
   return imeChangeMultitapStateWithoutDraw(chr);
}

int EditView::mmi_chset_ucs2_to_utf8(unsigned char *utf8, unsigned short ucs2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int count;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ucs2 < (unsigned short) 0x80)
    {
        count = 1;
    }
    else if (ucs2 < (unsigned short) 0x800)
    {
        count = 2;
    }
    else
    {
        count = 3;
    }
    switch (count)
    {
        case 3:
            utf8[2] = 0x80 | (ucs2 & 0x3f);
            ucs2 = ucs2 >> 6;
            ucs2 |= 0x800;
        case 2:
            utf8[1] = 0x80 | (ucs2 & 0x3f);
            ucs2 = ucs2 >> 6;
            ucs2 |= 0xc0;
        case 1:
            utf8[0] = (unsigned char) ucs2;
    }

    return count;
}

void EditView::SwitchShiftToLower(string str , int caretpos, bool flag)
{
   if(imeGetInputModeFlag() == INPUT_MODE_MULTITAP && imeIsMultiShiftInputMode())
   {
      imeSetMultitapShiftMode (SHIFT_STATE_LOWER);	
      imeCheckMultitapPunctuationCharacter (str, caretpos, flag);
   }
   else if(imeGetInputModeFlag() == INPUT_MODE_SMART)
   {
#ifdef __MMI_T9__
      imeClearT9ShiftMode();
#endif
      imeSmartCheckPunctuationCharacter (str, caretpos, flag);
   }

#ifdef __MMI_T9__
   imeSetSwitch();
#endif
}

void EditView::add_comma_in_calculator(char * str)
{
    Uint16 len=0;
    Uint16  index_buf=0, index_str=0;
    char buf[MAX_VALUES_IN_CALCULATOR_3COMMA];
    char *str_int;
    char *str_decimal;

    memset(buf, 0x00, MAX_VALUES_IN_CALCULATOR_3COMMA);

    // if a value has decimal in first position like ".0001"
    if( str[0] == '.') return;
    // if a value has integers and decimals
    if( strchr( str, '.') != '\0' )
    { 
        char str_tmp[MAX_VALUES_IN_CALCULATOR_3COMMA];
        memset(str_tmp, 0x00, MAX_VALUES_IN_CALCULATOR_3COMMA);
        strncpy(str_tmp, str,strlen(str) + 1);

        //divide integers and decimals
        str_int = strtok( str_tmp, "." );
        str_decimal = strtok( NULL , "." );
        //make comma per 3
        len = strlen(str_int);

        if( len > 0)
        {
            while(str_int[index_str]) {
                buf[index_buf++] = str_int[index_str++];
                if(--len && (len % 3) == 0)
                    buf[index_buf++]= ',';
            }

            buf[index_buf] = '\0';
            strcat(buf, ".");
            if( str_decimal != NULL) strncat(buf, str_decimal, strlen(str_decimal) );
        }

    }
    //unless a value has integers and decimals
    else
    {
        //make comma per 3
        len = strlen(str);

        if( len > 0)
        {
            while(str[index_str]) {
                buf[index_buf++] = str[index_str++];
                if(--len && (len % 3) == 0)
                    buf[index_buf++]= ',';
            }
            buf[index_buf] = '\0';
        }
    }	
    // copy to str
    strncpy(str , buf, strlen(buf) + 1);
}

void EditView::remove_comma_per3digit_in_calculator(char *str)
{
	Uint16  index_sou=0, index_des=0;
	char des[MAX_VALUES_IN_CALCULATOR_3COMMA];

	memset(des, 0x00, MAX_VALUES_IN_CALCULATOR_3COMMA);
	while(str[index_sou])	
	{
		if (str[index_sou] == ',')
            index_sou++;
		else
            des[index_des++] = str[index_sou++];
	}
	des[index_des] = '\0';	
	strncpy(str, des, strlen(des) + 1);
}

void EditView::remove_Repeative_Zerodigit_WhenClear_in_calculator(char *str_all_index_merge)
{
	Uint16 i =0;
	for(i=0;str_all_index_merge[i] == '0' && str_all_index_merge[i+1] != 0 && str_all_index_merge[i+1] != '.'; i++)
	{	
		setCaretPosition(caretPosition()-1);
	}
	strcpy(str_all_index_merge, (str_all_index_merge + i) );

}

string EditView::apply_comma_multiple_value_in_calculator(char *str, int *cursor)
{
	Uint16 index_value=0,index_1_digit=0,index_merge=0,index_cursor=0;
	char str_all[MAX_VALUES_IN_CALCULATOR_3COMMA][MAX_DIGIT_IN_CALCULATOR_3COMMA];
	char str_merge[MAX_VALUES_IN_CALCULATOR_3COMMA]="";
    char *pStr = str;
	Uint16 nbcursor_before=0, nbcursor_after=0;
	Uint16 i=0;
	
	for(i=0;i<MAX_VALUES_IN_CALCULATOR_3COMMA;i++)
	{
		memset(str_all[i], 0x00, MAX_DIGIT_IN_CALCULATOR_3COMMA);
	}
	memset(str_merge, 0x00, MAX_VALUES_IN_CALCULATOR_3COMMA);

	while(*pStr)	
	{
		//when it is opertator
		if( *pStr == '+' || *pStr == '-' || *pStr == 'x' || *pStr == '/' || *pStr == '(' || *pStr == ')' )	
		{
			//insert null
			if(index_1_digit != 0)	str_all[index_value++][index_1_digit] = '\0';
			//
			index_1_digit=0;
			//
			str_all[index_value][index_1_digit++] = *pStr;
			str_all[index_value++][index_1_digit] = '\0';
			index_1_digit=0;
			pStr++;
		}
		//when it is digit
		else
		{
			str_all[index_value][index_1_digit++] = *pStr++;
		}
	}
	str_all[index_value][index_1_digit] = '\0';
	
	//merge all
	for(index_merge=0;index_merge < index_value+1;index_merge++)
	{
		remove_comma_per3digit_in_calculator(str_all[index_merge]);
		remove_Repeative_Zerodigit_WhenClear_in_calculator(str_all[index_merge]);
		add_comma_in_calculator( str_all[index_merge]);
		strcat(str_merge, str_all[index_merge] );
	}

	//count cursor before
	for(index_cursor=0;str[index_cursor]!=0;index_cursor++)
	{
		//count cursor after
		if( str[index_cursor] == ',' ) nbcursor_before++;
	}
	
	//copy to pp_Editor->p_EditResult
	//strncpy(pp_Editor->p_EditResult , str_merge , strlen(str_merge) + 1 );

	//count cursor after
	for(index_cursor=0; str_merge[index_cursor] != 0; index_cursor++)
	{
		if( str_merge[index_cursor] == ',' ) nbcursor_after++;
	}

	//when inserting a digit
	if( nbcursor_after > nbcursor_before && str_merge[*cursor-1] != '(')
	{
		 (*cursor)++; 
	}
	//when removing a digit
	else if( nbcursor_after < nbcursor_before && str_merge[*cursor-1] != '.')
	{
		 (*cursor)--; 
	}
    return string(str_merge);

	
}
} // namespace hfcl
