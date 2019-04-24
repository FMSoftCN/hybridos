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
        begin_css(centerimg, ".img-center")
            style(Width,        PV_AUTO)
            style(Height,       PV_LENGTH_PX, 100)
            style(Margin,       PV_AUTO)
            style(TextAlign,    PV_CENTER)
            style(LineHeight,   PV_LENGTH_PX, 100.0f)
            style(WhiteSpace,   PV_NOWRAP)
            style(TextOverflow, PV_ELLIPSIS)
        end_css
        begin_css(vcenter1, ".line-vcenter")
            style(Width,        PV_AUTO)
            style(Height,       PV_LENGTH_PX, 100.0f)
            style(Margin,       PV_AUTO)
            style(TextAlign,    PV_CENTER)
            style(LineHeight,   PV_LENGTH_PX, 100.0f)
            style(WhiteSpace,   PV_NOWRAP)
            style(Overflow,     PV_ELLIPSIS)
        end_css
        begin_css(vcenter2, ".text-vcenter")
            style(Width,            PV_AUTO)
            style(Margin,           PV_AUTO)
            style(TextAlign,        PV_CENTER)
            style(PaddingTop,       PV_LENGTH_PX, 100.0f)
            style(PaddingBottom,    PV_LENGTH_PX, 100.0f)
            style(PaddingLeft,      PV_LENGTH_PX, 10.0f)
            style(PaddingRight,     PV_LENGTH_PX, 10.0f)
        end_css
    end_css_res

    begin_css_group_res
        begin_css_group(logo)
            css(centerimg)
        end_css_group
        begin_css_group(greet)
            css(vcenter1)
            css(vcenter2)
        end_css_group
    end_css_group_res

    begin_client_res
        begin_client(logo)
            def_name(view_animation_logo)
            def_name(event_animation_logo)
            begin_view(hvpanel)
                set(Id, 100)
                mystyle(BackgroundColor, rgba(255, 255, 255, 255))
                // animate view
                begin_view(hvani)
                    set(ResContent, my_image(animation_logo))
                    set(Alt, _("HybridOS Animation Logo"))
                    map(my_var(view_animation_logo))
                    on(Event::ET_VIEW, my_var(event_animation_logo))
                end_view
            end_view

            apply_css_group(sysid_css_group(global))
            apply_css_group(my_css_group(logo))
        end_client

        begin_client(greet)
            begin_view(hvpanel)
                begin_view_full(hvimg, "logo", "", "logo", 100)
                    set(ResContent, my_image(animation_logo))
                    set(Alt, "HybridOS Logo")
                    attribute("data", "the static logo")
                end_view

                begin_view_ex(hvtext, "single-line-vcenter")
                    set(TextContent, _("Welcome to the world of <em>HybridOS</em>!"))
                end_view

                begin_view_ex(hvtext, "multi-line-vcenter")
                    set(TextContent, _("HybridOS is a totally new open source operating system designed for smart IoT devices and cloud computing environment."))
                end_view
            end_view

            apply_css_group(sysid_css_group(global))
            apply_css_group(my_css_group(greet))
        end_client
    end_client_res

end_respkg

