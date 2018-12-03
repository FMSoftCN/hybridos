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

#ifndef __HFCL_TipDlg_H__
#define __HFCL_TipDlg_H__

#include "nguxwindow.h"
#include "common/event.h"
#include "viewcontext.h"
#include "services/timerservice.h"

namespace hfcl {
class TextView;
class Intent;
class ContextStream;

class TipDlg : public Window, ViewContext, TimerEventListener
{
public:

    typedef enum _TIP_DLG_RES_TYPE {
        TIP_DESTROY = -1,
        TIP_LIST_CANCEL = -2,
        TIP_CANCEL = 0,
        TIP_OK,
        TIP_SOS,
        TIP_EXITTIP,
    }TIP_DLG_RES_TYPE;

	TipDlg(HTResId id = 0);

    virtual ~TipDlg();

	void resetRect(int left, int top, int right, int bottom);
    void create(HTResId resid = 0);
    Intent* onDestroy(ContextStream* contextstream);

    bool onKey(int keyCode, KeyEvent* event);

    void setText(string text);
    string text();

    //void setImage(const char * image_file);
    //Image* image();

    void showDlg(void);
    void hideDlg(void);
    int closeDlg(int endCode = 0);
    virtual int doModal();

    bool handleEvent(Event* event);


    /*
     * auto close time unit is second.
     * if m_autoCloseTime is 0, not auto close tipdlg.
     */
    int autoCloseTime() {return m_autoCloseTime;}
    void setAutoCloseTime(int time);

	//define the sub view id
	enum {
		INFO_TEXT = 1,	
	};

protected:
    void onRespondCodeSL();
    void onRespondCodeSR();

	DECLARE_VIEWCONTEXT

private:
    TextView* m_textView;

    int m_timerId;

    /*
     * auto close time unit is second.
     * if m_autoCloseTime is 0, not auto close tipdlg.
     */
    int m_autoCloseTime;

	DECLARE_UI_TEMPL
}; 

} // namespace hfcl

#endif /* __HFCL_TipDlg_H__ */

