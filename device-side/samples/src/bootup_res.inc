
begin_respkg(bootup, RES_PKG_BOOTUP_ID)
    //image resource
    begin_image_res()
        image(bootup_img_logo,              "res/bootup/cbplus.gif")
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

        begin_ui_res(bootup_ui_animate)
            def_name(bootup_view_animate_view)
            def_name(bootup_event_gifanimate_view)
            def_name(bootup_img_animate_end_view)
            def_name(bootup_img_animate_first_view)
            // back panel
            begin_view(PanelView)
                setRect(0, 0, _ngux_screen_w, _ngux_screen_h)
                set(BkColor, Color::LAYER_COLOR_KEY)
                // animate view
                begin_view(GifAnimateView)
                    map(my(bootup_view_animate_view))
                    setRect(0, 0, _ngux_screen_w, _ngux_screen_h)
                    on(CustomEvent::CUSTOM_NOTIFY, my(bootup_event_gifanimate_view))
                end_view

                begin_view(ImageView)
                    hide()
                    map(my(bootup_img_animate_end_view))
                    setRect(0, 0, _ngux_screen_w, _ngux_screen_h)
                    set(Image, self_image(bootup_img_end_animate))
                end_view

                begin_view(ImageView)
                    hide()
                    map(my(bootup_img_animate_first_view))
                    setRect(0, 0, _ngux_screen_w, _ngux_screen_h)
                    set(Image, self_image(bootup_img_first_animate))
                end_view
            end_view
        end_ui_res
    end_uis

end_respkg
