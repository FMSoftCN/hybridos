
begin_sys_respkg

    // L10N text resource
    begin_text_res()
        text_res_gnumsg (R_LANG_zh_CN, R_ENCODING_utf8, "res/messages/zh_CN/LC_MESSAGES/helloworld.mo")
        text_res_gnumsg (R_LANG_zh_TW, R_ENCODING_utf8, "res/messages/zh_TW/LC_MESSAGES/helloworld.mo")
    end_text_res

    // CSS resource
    begin_style_sheets
        begin_style_sheet_res(default, "view")
            style(BackgroundColor, PV_BACKGROUND_COLOR_COLOR_ALICEBLUE, 0)
        end_style_sheet_res
    end_style_sheets

end_sys_respkg

