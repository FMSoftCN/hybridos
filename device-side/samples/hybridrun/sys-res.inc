/*
** HFCL Samples - Samples for HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL Samples.
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

begin_sys_respkg

    // L10N text resource
    begin_text_res()
        text_res_gnumsg (R_LANG_zh_CN, R_ENCODING_utf8, "res/messages/zh_CN/LC_MESSAGES/helloworld.mo")
        text_res_gnumsg (R_LANG_zh_TW, R_ENCODING_utf8, "res/messages/zh_TW/LC_MESSAGES/helloworld.mo")
    end_text_res

    // CSS
    begin_css_res
        begin_css(allview, "view")
            style(BackgroundColor, PV_BACKGROUND_COLOR_COLOR_ALICEBLUE, 0)
        end_css
        begin_css(textview, "view[type=text]")
            style(BackgroundColor, PV_BACKGROUND_COLOR_COLOR_ALICEBLUE, 0)
        end_css
    end_css_res

    // CSS Group
    begin_css_group_res
        begin_css_group(logo)
            css(allview)
        end_css_group
        begin_css_group(greet)
            css(textview)
        end_css_group
    end_css_group_res

end_sys_respkg
