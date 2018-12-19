#define ACTIVITY_WIDTH   240
#define ACTIVITY_HEIGHT  240

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

