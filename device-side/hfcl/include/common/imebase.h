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

#ifndef __IME_WINDOWD_BASE__
#define __IME_WINDOWD_BASE__

#include "imeres.h"

using ::hfcl;

typedef enum {
    SHIFT_STATE_INIT,
    SHIFT_STATE_UPPER,
    SHIFT_STATE_LOWER,
} MULTI_SHITF_STATE;

enum {
    MULTITAP_SHIFT_ABC,
    MULTITAP_UPPERCASE_ABC,
    MULTITAP_LOWERCASE_ABC
};

class ImeBase
{ 
public:
	ImeBase() {};
	virtual ~ImeBase() {};

    virtual void enable (bool b_enable) = 0;
    virtual void setInputMode (Uint16 mode) = 0;
    virtual void changeInputboxMode (Uint16 mode) = 0;
    virtual int  getCurrentInputMode () = 0;
	virtual int  getInputModeFlag () = 0;
	virtual void setInputModeFlag (Uint8 flag) = 0;
	virtual bool isArabic () = 0;
    virtual bool isSmartShiftInputMode () = 0;
    virtual int  getPreMultitapShiftMode () = 0;
    virtual void setPreMultitapShiftMode (int state) = 0;
    virtual void disableActiveMultitapWithoutDraw () = 0;
    virtual ui_char_t changeMultitapStateWithoutDraw (S32 chr) = 0;
    virtual void setMultitapShiftMode (int state) = 0;
    virtual int  getMultitapShiftMode () = 0;
    virtual bool isMultiShiftInputMode () = 0;
    virtual int  getPreviousActiveMultitap () = 0;
    virtual void changeKeyzeroMultitaps (bool mlflag, int keycode) = 0;
    virtual void checkMultitapPunctuationCharacter (string& str, int caretpos, bool flag, bool isSpaceMode) = 0;
    virtual void smartCheckPunctuationCharacter (string& str, int caretpos, bool flag) = 0;
    virtual void refreshString () = 0;

    virtual Uint16  getLanguage () = 0;
    virtual void setLanguage (Uint16 lang, Uint16 mode = MULTITAP_SHIFT_ABC) = 0;

#ifdef __MMI_T9__    
	virtual void IMESwitch() = 0;
    virtual void switchT9() = 0;
    virtual void clearT9ShiftMode() = 0;
    virtual void setT9ShiftMode() = 0;
    virtual bool isT9ShiftMode() = 0;
    virtual void T9SyncWithInputBoxMultilineInputBox (char *str, int cursor) = 0;
    virtual void T9KeyRSKClearHandlerMultilineInputBox (char *str, int cursor) = 0;
    virtual void T9KeyArrowLeftHandlerMultilineInputBox () = 0;
    virtual void T9KeyArrowRightHandlerMultilineInputBox () = 0;
    virtual void T9KeyArrowUpHandlerMultilineInputBox (int cursor) = 0;
    virtual void T9KeyArrowDownHandlerMultilineInputBox (int cursor) = 0;
    virtual int  T9GetActiveWordCount () = 0;
    virtual int  T9GetUnicodeCursor () = 0;
    virtual void T9SetUnicodeCursor (int cursor) = 0;
    virtual void T9CheckPunctuationCharacter (BOOL IsSpaceMode) = 0;
    virtual int  T9SetShifStateWithFieldInfo () = 0;
    virtual int  T9AddCustWordIntoLDB (unsigned short *buff, int len) = 0;
    virtual bool T9IsAddwordByArrowkey () = 0;
    virtual void T9ResetMode () = 0;
#endif

};

extern ImeBase *g_ime_window;
extern S32 MMI_active_multitap;

static inline bool imeDoesExist () {
    return (NULL != g_ime_window);
}

static inline void imeEnable (bool act) {
    if (NULL != g_ime_window) {
        g_ime_window->enable(act);
    }
}

static inline void imeSetInputMode (Uint16 mode) {
    if (NULL != g_ime_window) {
        g_ime_window->setInputMode (mode);
    }
}

static inline void imeChangeInputboxMode (Uint16 mode) {
    if (NULL != g_ime_window) {
        g_ime_window->changeInputboxMode (mode);
    }
}

static inline int imeGetCurrentInputMode () {
    if (NULL != g_ime_window) {
        return g_ime_window->getCurrentInputMode();
    }

    return INPUT_MODE_MULTITAP_SHIFT_ABC;
}

static inline int imeGetInputModeFlag() {
    if (NULL != g_ime_window) {
        return g_ime_window->getInputModeFlag();
    }

    return INPUT_MODE_NUMBER;
}

static inline void imeSetInputModeFlag(Uint8 mode) {
    if (NULL != g_ime_window) {
        g_ime_window->setInputModeFlag(mode);
    }
}

static inline bool imeIsArabic() {
    if (NULL != g_ime_window) {
        return g_ime_window->isArabic();
    }

    return false;
}

static inline int imeGetPreMultitapShiftMode () {
    if (NULL != g_ime_window) {
        return g_ime_window->getPreMultitapShiftMode();
    }

    return SHIFT_STATE_INIT;
}

static inline void imeSetPreMultitapShiftMode (int state) {
    if (NULL != g_ime_window) {
        g_ime_window->setPreMultitapShiftMode (state);
    }
}

static inline void imeDisableActiveMultitapWithoutDraw () {
    if (NULL != g_ime_window) {
        g_ime_window->disableActiveMultitapWithoutDraw ();
    }
}

static inline ui_char_t imeChangeMultitapStateWithoutDraw (S32 chr) {
    if (NULL != g_ime_window) {
        return g_ime_window->changeMultitapStateWithoutDraw (chr);
    }

    return 0;
}

static inline void imeSetMultitapShiftMode (int state) {
    if (NULL != g_ime_window) {
        g_ime_window->setMultitapShiftMode (state);
    }
}

static inline int imeGetMultitapShiftMode () {
    if (NULL != g_ime_window) {
        return g_ime_window->getMultitapShiftMode ();
    }

    return 0;
}

static inline bool imeIsSmartShiftInputMode () {
    if (NULL != g_ime_window) {
        return g_ime_window->isSmartShiftInputMode ();
    }

    return false;
}

static inline bool imeIsMultiShiftInputMode () {
    if (NULL != g_ime_window) {
        return g_ime_window->isMultiShiftInputMode ();
    }

    return false;
}

static inline int imeGetPreviousActiveMultitap () {
    if (NULL != g_ime_window) {
        return g_ime_window->getPreviousActiveMultitap ();
    }

    return 0;
}

static inline void imeChangeKeyzeroMultitaps (bool mlflag, int keycode) {
    if (NULL != g_ime_window) {
        g_ime_window->changeKeyzeroMultitaps (mlflag, keycode);
    }
}

static inline void imeCheckMultitapPunctuationCharacter (string& str, int caretpos, bool flag, bool isSpaceMode = false) {
    if (NULL != g_ime_window) {
        g_ime_window->checkMultitapPunctuationCharacter (str, caretpos, flag, isSpaceMode);
    }
}

static inline void imeSmartCheckPunctuationCharacter (string& str, int caretpos, bool flag) {
    if (NULL != g_ime_window) {
        g_ime_window->smartCheckPunctuationCharacter (str, caretpos, flag);
    }
}

static inline void imeRefreshString () {
    if (NULL != g_ime_window) {
        g_ime_window->refreshString ();
    }
}

#ifdef __MMI_T9__
static inline void imeSetSwitch() {
    if (NULL != g_ime_window) {
        g_ime_window->IMESwitch();
    }
}

static inline void imeClearT9ShiftMode() {
    if (NULL != g_ime_window) {
        g_ime_window->clearT9ShiftMode();
    }
}

static inline void imeSetT9ShiftMode() {
    if (NULL != g_ime_window) {
        g_ime_window->setT9ShiftMode();
    }
}

static inline bool imeIsT9ShiftMode() {
    if (NULL != g_ime_window) {
        return g_ime_window->isT9ShiftMode();
    }

    return false;
}

static inline void imeT9SyncWithInputBoxMultilineInputBox (char *str, int cursor) {
    if (NULL != g_ime_window) {
        g_ime_window->T9SyncWithInputBoxMultilineInputBox (str, cursor);
    }
}

static inline void imeT9KeyRSKClearHandlerMultilineInputBox (char *str, int cursor) {
    if (NULL != g_ime_window) {
        g_ime_window->T9KeyRSKClearHandlerMultilineInputBox (str, cursor);
    }
}

static inline void imeT9KeyArrowRightHandlerMultilineInputBox () {
    if (NULL != g_ime_window) {
        g_ime_window->T9KeyArrowRightHandlerMultilineInputBox ();
    }
}

static inline void imeT9KeyArrowLeftHandlerMultilineInputBox () {
    if (NULL != g_ime_window) {
        g_ime_window->T9KeyArrowLeftHandlerMultilineInputBox ();
    }
}

static inline void imeT9KeyArrowUpHandlerMultilineInputBox (int cursor) {
    if (NULL != g_ime_window) {
        g_ime_window->T9KeyArrowUpHandlerMultilineInputBox (cursor);
    }
}

static inline void imeT9KeyArrowDownHandlerMultilineInputBox (int cursor) {
    if (NULL != g_ime_window) {
        g_ime_window->T9KeyArrowDownHandlerMultilineInputBox (cursor);
    }
}

static inline int imeT9GetActiveWordCount () {
    if (NULL != g_ime_window) {
        return g_ime_window->T9GetActiveWordCount ();
    }

    return 0;
}

static inline int imeT9GetUnicodeCursor() {
    if (NULL != g_ime_window) {
        return g_ime_window->T9GetUnicodeCursor ();
    }

    return 0;
}

static inline void imeT9SetUnicodeCursor(int cursor) {
    if (NULL != g_ime_window) {
        g_ime_window->T9SetUnicodeCursor (cursor);
    }
}

static inline void imeT9CheckPunctuationCharacter (BOOL IsSpaceMode) {
    if (NULL != g_ime_window) {
        g_ime_window->T9CheckPunctuationCharacter (IsSpaceMode);
    }
}

static inline int imeT9SetShifStateWithFieldInfo () {
    if (NULL != g_ime_window) {
        return g_ime_window->T9SetShifStateWithFieldInfo ();
    }

    return 0;
}

static inline bool imeT9IsAddwordByArrowkey () {
    if (NULL != g_ime_window) {
        return g_ime_window->T9IsAddwordByArrowkey ();
    }

    return false;
}

static inline int imeT9AddCustWordIntoLDB (unsigned short* buff, int len) {
    if (NULL != g_ime_window) {
        return g_ime_window->T9AddCustWordIntoLDB (buff, len);
    }

    return 0;
}

static inline void imeT9ResetMode () {
    if (NULL != g_ime_window) {
        g_ime_window->T9ResetMode ();
    }
}

static inline Uint16 imeGetLanguage () {
    if (NULL != g_ime_window) {
        return g_ime_window->getLanguage ();
    }

    return 0;
}

static inline void imeSetLanguage (Uint16 lang, Uint16 mode = MULTITAP_SHIFT_ABC)
{
    if (NULL != g_ime_window) {
        return g_ime_window->setLanguage (lang, mode);
    }
}

#endif /* __MMI_T9__ */

#endif //__IME_WINDOWD_BASE___
