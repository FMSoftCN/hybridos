#define ACTIVITY_WIDTH   240
#define ACTIVITY_HEIGHT  240

begin_respkg(bootup, RES_PKG_BOOTUP_ID)
    //image resource
    begin_image_res()
        image(animation_logo, "res/images/splash/joopic.gif")
    end_image_res

    // dr
    begin_dr_res
        // for default bkg color
        begin_drawable(bootup_dr_default_bkg_color, "ColorDrawable", 0)
            style_color(SYS_SE_COLOR, 255, 255, 255)
        end_drawable

        // for default bkg color
        begin_drawable(bootup_dr_charger_bkg_color, "ColorDrawable", 0)
            style_color(SYS_SE_COLOR, 0, 0, 0)
        end_drawable

        // for default txt color
        begin_drawable(bootup_dr_default_txt_color, "TextDrawable", 0)
            style_color(SYS_SE_COLOR, 100, 100, 100)
        end_drawable

        // for white txt color
        begin_drawable(bootup_dr_white_txt_color, "TextDrawable", 0)
            style_color(SYS_SE_COLOR, 255, 255, 255)
        end_drawable
    end_dr_res

    // drset
    begin_drset_res
        // for default back color
        begin_drset(bootup_drset_default_bkg_color, R_sys_drset_null_bk)
            dr(DR_BKGND, my_dr(bootup_dr_default_bkg_color))
        end_drset

        begin_drset(bootup_drset_charger_bkg_color, R_sys_drset_null_bk)
            dr(DR_BKGND, my_dr(bootup_dr_charger_bkg_color))
        end_drset

        // for default txt color
        begin_drset(bootup_drset_default_txt_color, 0)
            dr(DR_BKGND, my_dr(bootup_dr_default_txt_color))
            dr(DR_CONTENT, my_dr(bootup_dr_default_txt_color))
        end_drset

        // for default txt color
        begin_drset(bootup_drset_white_txt_color, 0)
            dr(DR_BKGND, my_dr(bootup_dr_white_txt_color))
            dr(DR_CONTENT, my_dr(bootup_dr_white_txt_color))
        end_drset
    end_drset_res

    begin_uis

        begin_ui_res(logo)
            def_name(view_animation_logo)
            def_name(event_animation_logo)
            // back panel
            begin_view(PanelView)
                setRect(0, 0, ACTIVITY_WIDTH, ACTIVITY_HEIGHT)
                set(BkColor, MakeRGBA (255, 255, 255, 0))
                // animate view
                begin_view(GifAnimateView)
                    map(my(view_animation_logo))
                    setRect(0, 100, ACTIVITY_WIDTH, ACTIVITY_HEIGHT)
                    on(CustomEvent::CUSTOM_NOTIFY, my(event_animation_logo))
                end_view
            end_view
        end_ui_res

        begin_ui_res(greet)
            def_name(view_greet_text)
            // back panel
            begin_view(PanelView)
                setRect(0, 0, ACTIVITY_WIDTH, ACTIVITY_HEIGHT)
                set(BkColor, Color::LAYER_COLOR_KEY)
                begin_view_ex(TextView, my_drset(bootup_drset_default_txt_color))
                    setRect(0, 0, ACTIVITY_WIDTH, ACTIVITY_HEIGHT)
                    map(my(view_greet_text))
                end_view
            end_view
        end_ui_res
    end_uis

end_respkg

