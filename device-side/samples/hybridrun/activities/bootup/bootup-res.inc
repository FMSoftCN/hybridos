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
            style(BackgroundColor, PV_COLOR_RED)
        end_css
        begin_css(def2, ".test")
            style(BackgroundColor, PV_COLOR_RED)
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

    begin_client_res
        begin_client(logo)
            def_name(view_animation_logo)
            def_name(event_animation_logo)
            begin_view(PanelView)
                set(Id, 100)
                mystyle(BackgroundColor, rgba(255, 255, 255, 255))
                // animate view
                begin_view(GifView)
                    map(my_var(view_animation_logo))
                    on(Event::ET_VIEW, my_var(event_animation_logo))
                end_view
            end_view

            apply_css_group(sysid_css_group(global))
            apply_css_group(my_css_group(logo))
        end_client

        begin_client(greet)
            begin_view(TextView)
                set(Content, "Welcome to the world of <em>HybridOS</em>!")
            end_view

            apply_css_group(sysid_css_group(global))
            apply_css_group(my_css_group(greet))
        end_client
    end_client_res

end_respkg

