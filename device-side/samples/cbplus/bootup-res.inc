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

begin_respkg(bootup, RES_PKG_BOOTUP_ID)

    // image resource
    begin_image_res()
        image(animation_logo, "res/images/splash/joopic.gif")
    end_image_res

    // CSS resource
    begin_css_res
        begin_css(def1, ".test")
            style(BackgroundColor, PV_BACKGROUND_COLOR_COLOR_RED, 0)
        end_css
        begin_css(def2, ".test")
            style(BackgroundColor, PV_BACKGROUND_COLOR_COLOR_RED, 0)
        end_css
    end_css_res

    begin_css_group_res
        begin_css_group(logo)
            css(def1)
        end_css_group
        begin_css_group(greet)
            css(def2)
        end_css_group
    end_css_group_res

    begin_uis
        begin_ui_res(logo)
            def_name(view_animation_logo)
            def_name(event_animation_logo)
            // back panel
            begin_view(PanelView)
                set(BkColor, MakeRGBA (255, 255, 255, 0))
                // animate view
                begin_view(GifAnimateView)
                    map(my_var(view_animation_logo))
                    on(CustomEvent::CUSTOM_NOTIFY, my_var(event_animation_logo))
                end_view
            end_view
        end_ui_res

        begin_ui_res(greet)
            def_name(view_greet_text)
            // back panel
            begin_view(PanelView)
                set(BkColor, Color::LAYER_COLOR_KEY)
                begin_view(TextView)
                    map(my_var(view_greet_text))
                end_view
            end_view
        end_ui_res
    end_uis

end_respkg

