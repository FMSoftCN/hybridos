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

    def_static_array_real(def_padding, 4, 2, 4, 2)

    // L10N text resource
    begin_text_res()
        text_res_gnumsg (R_LANG_zh_CN, R_ENCODING_utf8, "res/messages/zh_CN/LC_MESSAGES/helloworld.mo")
        text_res_gnumsg (R_LANG_zh_TW, R_ENCODING_utf8, "res/messages/zh_TW/LC_MESSAGES/helloworld.mo")
    end_text_res

    // global images
    begin_image_res()
        image(arrow_left,   "res/images/arrow-left.png")
        image(arrow_right,  "res/images/arrow-right.png")
        image(arrow_up,     "res/images/arrow-up.png")
        image(arrow_down,   "res/images/arrow-down.png")
        image(mark_check,   "res/images/mark-check.png")
        image(ok,           "res/images/icon-ok.png")
        image(cancel,       "res/images/icon-cancel.png")
        image(cloud,        "res/images/icon-cloud.png")
        image(usb,          "res/images/icon-camera-usb.png")
        image(wlan,         "res/images/icon-camera-wlan.png")
        image(loading_nm,   "res/images/loading-nm.gif")
        image(loading_sm,   "res/images/loading-sm.gif")
        image(loading_xm,   "res/images/loading-xm.gif")
    end_image_res

    // global fonts
    begin_font_res()
        font(xxs,  "ttf-HanSans-srncnn-*-10-UTF-8")
        font(xs,   "ttf-HanSans-srncnn-*-12-UTF-8")
        font(sm,   "ttf-HanSans-srncnn-*-14-UTF-8")
        font(nm,   "ttf-HanSans-srncnn-*-16-UTF-8")
        font(lg,   "ttf-HanSans-srncnn-*-20-UTF-8")
        font(xl,   "ttf-HanSans-srncnn-*-26-UTF-8")
        font(xxl,  "ttf-HanSans-srncnn-*-36-UTF-8")
    end_font_res

    // CSS
    begin_css_res
        begin_css(body, "hvbody")
            style(BackgroundColor, PV_BACKGROUND_COLOR_COLOR_BLACK, 0)
            style(Color,    PV_BACKGROUND_COLOR_RGBA, rgba(0x16, 0xF2, 0x23, 0xFF))
            style(Font,     PV_FONT_SYSID, sysid_font(nm))
        end_css
        begin_css(title, "hvtext.title")
            style(Display,  PV_DISPLAY_BLOCK, 0)
            style(Height,   PV_HEIGHT_LENGTH_PX, 28.0f)
            style(Width,    PV_WIDTH_AUTO, 0)
            style(TextAlign,PV_TEXT_ALIGN_CENTER, 0)
            style(Color,    PV_BACKGROUND_COLOR_RGBA, rgba(0x64, 0x96, 0xc7, 0xFF))
            style(Padding,  PV_PADDING_LEFT_LENGTH_PX, array_real(def_padding))
        end_css
        begin_css(subtitle, "hvtext.subtitle")
            style(Display,  PV_DISPLAY_BLOCK, 0)
            style(Height,   PV_HEIGHT_LENGTH_PX, 20.0f)
            style(Width,    PV_WIDTH_AUTO, 0)
            style(TextAlign,PV_TEXT_ALIGN_CENTER, 0)
            style(Color,    PV_BACKGROUND_COLOR_RGBA, rgba(0xDD, 0xDD, 0xDD, 0xFF))
            style(Font,     PV_FONT_SYSID, sysid_font(sm))
        end_css
    end_css_res

    // CSS Group
    begin_css_group_res
        begin_css_group(global)
            css(body)
            css(title)
            css(subtitle)
        end_css_group
    end_css_group_res

end_sys_respkg
