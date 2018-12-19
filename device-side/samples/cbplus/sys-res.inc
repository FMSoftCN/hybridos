
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

