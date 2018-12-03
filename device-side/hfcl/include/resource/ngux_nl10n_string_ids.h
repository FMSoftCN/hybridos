/* 
** HFCL - HybridOS Foundation Class Library
** 
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
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

#ifndef _NL10N_STRING_IDS_H_
#define _NL10N_STRING_IDS_H_

enum {
    ETXT_AUDIO_SETTINGS = MAX_STRID, // RDA; Audio Settings
    ETXT_BATTERY_INFO, // RDA; Battery info
    ETXT_HW_TEST, // RDA; H/W test
    ETXT_LOCK_STATE, // RDA; Lock state
    ETXT_NOT_SUPPORT, // RDA; Not support
    ETXT_PRODUCTION, // RDA; Production
    ETXT_REFLASH, // RDA; Reflash
    ETXT_SAT_TEST, // RDA; SAT TEST
    ETXT_SIM_INFO, // RDA; SIM info
    ETXT_TEST_MODE, // RDA; Test Mode
    ETXT_TEXT_MSG, // RDA; Text message
    ETXT_VERSION, // RDA; Version
    ETXT_VIB_TEST, // RDA; Vib test
    GS_CONVERTING_TO_MMS_MESSAGE_ING, // RDA; Converting to MMS message...
    HS_ACTIVATE, // RDA; Activate
    HS_ACTIVA_LOCK_DEACTIVATED, // RDA; Activa lock deactivated
    HS_ACTIVA_LOCK_PERSONALISED, // RDA; Activa lock personalised
    HS_ALREADY_PERSONALISED, // RDA; Already personalised
    HS_AUTO_CP_LOCK_DEACTIVATED, // RDA; Auto CP lock deactivated
    HS_AUTO_CP_LOCK_PERSONALISED, // RDA; Auto CP lock personalised
    HS_AUTO_NETWORK_LOCK_DEACTIVATED, // RDA; Auto network lock deactivated
    HS_AUTO_NETWORK_LOCK_PERSONALISED, // RDA; Auto network lock personalised
    HS_AUTO_SIM_LOCK_DEACTIVATED, // RDA; Auto SIM lock deactivated
    HS_AUTO_SIM_LOCK_PERSONALISED, // RDA; Auto SIM lock personalised
    HS_AUTO_SP_LOCK_DEACTIVATED, // RDA; Auto SP lock deactivated
    HS_AUTO_SP_LOCK_PERSONALISED, // RDA; Auto SP lock personalised
    HS_AUTO_SUBSET_LOCK_DEACTIVATED, // RDA; Auto subset lock deactivated
    HS_AUTO_SUBSET_LOCK_PERSONALISED, // RDA; Auto subset lock personalised
    HS_BDN_FILE_RESTRICTED, // RDA; BDN file restricted
    HS_BINARY_COMMAND_ORIGINATING_PORT, // RDA; Binary Command Originating Port
    HS_BOOKMARK, // RDA; Bookmark
    HS_CALL_CLEARING, // RDA; Call clearing
    HS_CALL_ENDED_FAILED, // RDA; Call Ended failed
    HS_CHANGED, // RDA; Changed
    HS_COMPLETED, // RDA; Completed
    HS_CONTACT_UPDATE_MESSAGE_ORIGINATING_PORT, // RDA; Contact Update Message Originating Port
    HS_CP_LOCK_DEACTIVATED, // RDA; CP lock deactivated
    HS_CP_LOCK_PERSONALISED, // RDA; CP lock personalised
    HS_DELETE, // RDA; DELETE
    HS_DESTINATION_PORT, // RDA; Destination Port
    HS_DIR_STORED, // RDA; Dir stored
    HS_EMPTY, // RDA; Empty
    HS_ENTRY_EMPTY, // RDA; Entry empty
    HS_ERROR, // RDA; Error
    HS_FAILED_TO_END_CALL, // RDA; Failed to end call
    HS_HTTP_HEADER, // RDA; HTTP Header
    HS_HTTP_STREAM_MANAGER, // RDA; HTTP Stream manager
    HS_INITIALISATION_COMPLETED, // RDA; Initialisation completed
    HS_INPUTS_DO_NOT_MATCH, // RDA; Inputs do not match
    HS_INVALID_ENTRY_VALUE, // RDA; Invalid entry value
    HS_INVALID_PARAMETERS, // RDA; Invalid parameters
    HS_LIST_EMPTY, // RDA; List empty
    HS_LIST_INITIALISED, // RDA; List initialised
    HS_MAKE_MULTI_IMPOSSIBLE, // RDA; Make multi impossible
    HS_MYFAVES_NETWORK_INTERACTION_OFF, // RDA; MyFaves network interaction OFF
    HS_MYFAVES_NETWORK_INTERACTION_ON, // RDA; MyFaves network interaction ON
    HS_NETWORK_LOCK_DEACTIVATED, // RDA; Network lock deactivated
    HS_NETWORK_LOCK_PERSONALISED, // RDA; Network lock personalised
    HS_NETWORK_LOCK_PREREQUISITE, // RDA; Network lock prerequisite
    HS_NOT_YET_PERSONALISED, // RDA; Not yet personalised
    HS_NO_ERROR, // RDA; No error
    HS_NO_EXIST_ENTRY, // RDA; No exist entry
    HS_NO_LOCK_CODE_IN_SIM, // RDA; No lock code in SIM
    HS_NULL, // RDA; Null
    HS_NUMBER_FDN_RESTRICTED, // RDA; Number FDN restricted
    HS_NUMBER_TOO_LONG, // RDA; Number too long
    HS_PDP_ACTIVATE_ERROR, // RDA; PDP activate error
    HS_PDP_DE_ACTIVATE_ERROR, // RDA; PDP de-activate error
    HS_PHONEBOOK_INITIALISING, // RDA; Phonebook initialising
    HS_PHONE_MEMORY_FULL, // RDA; Phone memory full
    HS_PHONE_WILL_REBOOT_APPLY_NEW_CUSTOMER_SETTING_Q, // RDA; Phone will reboot. Do you want to apply new customer setting?
    HS_PIN_CHANGE_ERROR, // RDA; PIN change error
    HS_PIN_DISABLE_ERROR, // RDA; PIN disable error
    HS_PIN_ENABLE_ERROR, // RDA; PIN enable error
    HS_PIN_UNBLOCK_ERROR, // RDA; PIN unblock error
    HS_PIN_VERIFY_ERROR, // RDA; PIN verify error
    HS_PIPELINE_NUMBER, // RDA; Pipeline number
    HS_RECORD_EMPTY, // RDA; Record empty
    HS_RETRIEVE_REFUSED, // RDA; Retrieve refused
    HS_RUN, // RDA; Run
    HS_SAVED, // RDA; Saved
    HS_SAVING_ERROR, // RDA; Saving error
    HS_SERVICE_IS_NOT_AVAILABLE, // RDA; Service is not available
    HS_SETTING, // RDA; Setting
    HS_SIM_INFO, // RDA; SIM info
    HS_SIM_LOCK_DEACTIVATED, // RDA; SIM lock deactivated
    HS_SIM_LOCK_PERSONALISED, // RDA; SIM lock personalised
    HS_SIM_MEMORY_FULL, // RDA; SIM memory full
    HS_SMSC_NUMBER, // RDA; SMSC number
    HS_SOCKET_PIPELINE, // RDA; Socket / Pipeline
    HS_SP_LOCK_DEACTIVATED, // RDA; SP lock deactivated
    HS_SP_LOCK_PERSONALISED, // RDA; SP lock personalised
    HS_SUBSET_LOCK_DEACTIVATED, // RDA; Subset lock deactivated
    HS_SUBSET_LOCK_PERSONALISED, // RDA; Subset lock personalised
    HS_SWAP_REFUSED, // RDA; Swap refused
    HS_TEST_MODE, // RDA; Test mode
    HS_TMO_SERVER_SHORT_CODE, // RDA; TMO Server short code
    HS_TOO_LONG, // RDA; Too long
    HS_TRANSFER_ERROR, // RDA; Transfer error
    HS_WELCOME_MESSAGE_ORIGINATING_PORT, // RDA; Welcome Message Originating Port
    HS_WIFI_OFF, // RDA; Wi-Fi Off
    IM_TXT_IM_ADD_SYMBOL, // RDA; Add symbol
    IM_TXT_IM_ADD_WORD, // RDA; Spell word
    IM_TXT_IM_INPUT_METHOD, // RDA; Input method
    IM_TXT_IM_SHORT_ABC_LEAD_UPPER, // RDA; Abc
    IM_TXT_IM_SHORT_ABC_LOWER, // RDA; abc
    IM_TXT_IM_SHORT_ABC_UPPER, // RDA; ABC
    IM_TXT_IM_SHORT_PINYIN, // RDA; Pinyin
    IM_TXT_IM_SHORT_SMART_LEAD_UPPER, // RDA; T9 Abc
    IM_TXT_IM_SHORT_SMART_LOWER, // RDA; T9 abc
    IM_TXT_IM_SHORT_SMART_UPPER, // RDA; T9 ABC
    IM_TXT_IM_SHORT_STROKE, // RDA; Stroke
    IM_TXT_IM_SPELL, // RDA; Spell
    IM_TXT_INPUT_MODE_SET, // RDA; Input method selection
    IM_TXT_INPUT_PY_FUZZY, // RDA; Pinyin fuzzy settings
    IM_TXT_INPUT_PY_FUZZY_AN_ANG, // RDA; an/ang
    IM_TXT_INPUT_PY_FUZZY_C_CH, // RDA; c/ch
    IM_TXT_INPUT_PY_FUZZY_EN_ENG, // RDA; en/eng
    IM_TXT_INPUT_PY_FUZZY_H_F, // RDA; h/f
    IM_TXT_INPUT_PY_FUZZY_IAN_IANG, // RDA; ian/iang
    IM_TXT_INPUT_PY_FUZZY_IN_ING, // RDA; in/ing
    IM_TXT_INPUT_PY_FUZZY_L_H, // RDA; l/n
    IM_TXT_INPUT_PY_FUZZY_SET, // RDA; Pinyin fuzzy settings
    IM_TXT_INPUT_PY_FUZZY_S_SH, // RDA; s/sh
    IM_TXT_INPUT_PY_FUZZY_UAN_UANG, // RDA; uan/uang
    IM_TXT_INPUT_PY_FUZZY_Z_ZH, // RDA; z/zh
    IM_TXT_SET_INPUT_123, // RDA; Number
    IM_TXT_SET_INPUT_ENGLISH, // RDA; English
    IM_TXT_SET_INPUT_MODE, // RDA; Input method settings
    IM_TXT_SET_INPUT_PY, // RDA; Pinyin
    IM_TXT_SET_INPUT_STROKE, // RDA; Stroke
    SS_0_01_KCAL_159999_99_KCAL, // RDA; 0.01 Kcal ~ 159999.99 Kcal
    SS_0_01_KM_1349_99_KM, // RDA; 0.01 Km ~ 1349.99 Km
    SS_1, // RDA; 1
    SS_10000_0_G_300000_0_G, // RDA; 10000.0 g ~ 300000.0 g
    SS_10_0_IN_120_0_IN, // RDA; 10.0 in ~ 120.0 in
    SS_10_0_KG_300_0_KG, // RDA; 10.0 kg ~ 300.0 kg
    SS_10_MIN, // RDA; 10 mins
    SS_10_MINS, // RDA; 10 mins
    SS_10_MINUTES, // RDA; 10 minutes
    SS_10_MIN_BEFORE, // RDA; 10 mins before
    SS_10_PIXELS, // RDA; 10 pixels
    SS_10_P_OF_SERVER_CAPACITY_REMAINED, // RDA; 10% of server capacity remained
    SS_10_SEC, // RDA; 10 secs
    SS_10_SECONDS, // RDA; 10 seconds
    SS_10_TIMES, // RDA; 10 times
    SS_11IN_X_17IN, // RDA; 11 x 17
    SS_123, // RDA; 123
    SS_123_ARABIC_NUMBER, // RDA; 123
    SS_128_X_96, // RDA; 128 X 96
    SS_12_HOURS, // RDA; 12 hours
    SS_15MINS, // RDA; 15 mins
    SS_15_MINUTES, // RDA; 15 minutes
    SS_15_SEC, // RDA; 15 secs
    SS_15_SECONDS, // RDA; 15 seconds
    SS_160_X_120, // RDA; 160 X 120
    SS_176_X_144, // RDA; 176 X 144
    SS_1PX_LINE, // RDA; 1px line
    SS_1ST_WEEK, // RDA; 1st week
    SS_1_0_FT_10_0_FT, // RDA; 1.0 ft ~ 10.0 ft
    SS_1_DAY, // RDA; 1 day
    SS_1_HOUR, // RDA; 1 hour
    SS_1_HOUR_BEFORE, // RDA; 1 hour before
    SS_1_MIN, // RDA; 1 min
    SS_1_MINUTE, // RDA; 1 minute
    SS_1_MONTH, // RDA; 1 month
    SS_1_MREPEAT, // RDA; 1
    SS_1_SECOND, // RDA; 1 second
    SS_1_STEPS_999999_STEPS, // RDA; 1 steps ~ 999999 steps
    SS_1_TIME, // RDA; 1 time
    SS_1_WEEK, // RDA; 1 week
    SS_2, // RDA; 2
    SS_20_CONTACTS_AVAILABLE_ONLY, // RDA; 20 contacts available only
    SS_20_MINS, // RDA; 20 mins
    SS_20_MIN_BEFORE, // RDA; 20 mins before
    SS_20_PIXELS, // RDA; 20 pixels
    SS_20_SECONDS, // RDA; 20 seconds
    SS_22_0_POUND_660_0_POUND, // RDA; 22.0 pound ~ 660.0 pound
    SS_24_HOURS, // RDA; 24 hours
    SS_24_HOURS_MPERIOD, // RDA; 24 hours
    SS_25_MINS, // RDA; 25 mins
    SS_25_SECONDS, // RDA; 25 seconds
    SS_2L, // RDA; 2L
    SS_2ND_WEEK, // RDA; 2nd week
    SS_2PX_LINE, // RDA; 2px line
    SS_2_DAYS, // RDA; 2 days
    SS_2_HOURS, // RDA; 2 hours
    SS_2_SEC, // RDA; 2 secs
    SS_2_SECONDS, // RDA; 2 seconds
    SS_2_TIMES, // RDA; 2 times
    SS_2_WEEKS, // RDA; 2 weeks
    SS_3, // RDA; 3
    SS_30_0_CM_300_0_CM, // RDA; 30.0 cm ~ 300.0 cm
    SS_30_MIN, // RDA; 30 mins
    SS_30_MINUTES, // RDA; 30 minutes
    SS_30_MIN_BEFORE, // RDA; 30 mins before
    SS_30_SEC, // RDA; 30 secs
    SS_30_SECONDS, // RDA; 30 seconds
    SS_320_X_240, // RDA; 320 X 240
    SS_352_X_288, // RDA; 352 x 288
    SS_3D_SOUND, // RDA; 3D sound
    SS_3PX_LINE, // RDA; 3px line
    SS_3RD_WEEK, // RDA; 3rd week
    SS_3_DAYS, // RDA; 3 days
    SS_3_HOURS, // RDA; 3 hours
    SS_3_MIN, // RDA; 3 mins
    SS_3_MINUTES, // RDA; 3 minutes
    SS_3_SEC, // RDA; 3 secs
    SS_3_SECONDS, // RDA; 3 seconds
    SS_3_TIMES, // RDA; 3 times
    SS_3_WEEKS, // RDA; 3 weeks
    SS_4, // RDA; 4
    SS_40_PIXELS, // RDA; 40 pixels
    SS_4IN_X_6IN, // RDA; 4 x 6
    SS_4TH_WEEK, // RDA; 4th week
    SS_4_HOURS, // RDA; 4 hours
    SS_4_SECONDS, // RDA; 4 seconds
    SS_4_X_6, // RDA; 4 x 6
    SS_5, // RDA; 5
    SS_5_DIGITS_REQUIRED, // RDA; 5 digits required
    SS_5_MINS, // RDA; 5 mins
    SS_5_MINUTES, // RDA; 5 minutes
    SS_5_MIN_BEFORE, // RDA; 5 mins before
    SS_5_SEC, // RDA; 5 secs
    SS_5_SECONDS, // RDA; 5 seconds
    SS_5_TIMES, // RDA; 5 times
    SS_6, // RDA; 6
    SS_640_X_480, // RDA; 640 X 480
    SS_6_HOURS, // RDA; 6 hours
    SS_7, // RDA; 7
    SS_7_MINS, // RDA; 7 mins
    SS_8IN_X_10IN, // RDA; 8 x 10
    SS_8_SEC, // RDA; 8 secs
    SS_8_X_10, // RDA; 8 x 10
    SS_A4, // RDA; A4
    SS_ABAN_MEA, // RDA; Aban
    SS_ABC_LC, // RDA; abc
    SS_ABC_MC, // RDA; Abc
    SS_ABC_MODE, // RDA; ABC mode
    SS_ABC_SHIFT, // RDA; Abc
    SS_ABC_UC, // RDA; ABC
    SS_ABEOKUTA, // RDA; Abeokuta
    SS_ABHA, // RDA; Abha
    SS_ABORT, // RDA; Abort
    SS_ABORT_BY_USER, // RDA; Abort by user
    SS_ABOUT, // RDA; About
    SS_ABOUT_BROWSER, // RDA; About browser
    SS_ABOUT_INTERNET, // RDA; About Internet
    SS_ABOUT_MYFAVES, // RDA; About MyFaves
    SS_ABQAIQ, // RDA; Abqaiq
    SS_ABUJA, // RDA; Abuja
    SS_ABU_AZ_ZULUF, // RDA; Abu az Zuluf
    SS_ABU_DHABI, // RDA; Abu Dhabi
    SS_ABU_HALIFA, // RDA; Abu Halifa
    SS_ABU_THAYLAH, // RDA; Abu Thaylah
    SS_ACCEPT, // RDA; Accept
    SS_ACCEPT_ALL, // RDA; Accept all
    SS_ACCEPT_CALL, // RDA; Accept call
    SS_ACCEPT_COOKIE_Q, // RDA; Accept cookie?
    SS_ACCESS_AUTHORISATION_WARNING_VODA, // RDA; This authorisation may allow remote device to access your device without notice. Continue?
    SS_ACCESS_NAME, // RDA; Access name
    SS_ACCOUNT, // RDA; Account
    SS_ACCOUNT_1, // RDA; Account 1
    SS_ACCOUNT_2, // RDA; Account 2
    SS_ACCOUNT_NAME, // RDA; Account name
    SS_ACCOUNT_SETTINGS, // RDA; Account settings
    SS_ACCOUNT_SETTING_CONFIGURE_Q, // RDA; Account setting is not configured. Configure now?
    SS_ACRE, // RDA; acre
    SS_ACRES_MCVT, // RDA; Acres
    SS_ACTIVATE, // RDA; Activate
    SS_ACTIVATED, // RDA; Activated
    SS_ACTIVATED_M_ON, // RDA; activated
    SS_ACTIVATE_ALARM, // RDA; Activate alarm
    SS_ACTIVATE_CURRENT_ACCOUNT_Q, // RDA; Activate current account?
    SS_ACTIVATE_CURRENT_PROFILE_Q, // RDA; Activate current profile?
    SS_ACTIVATE_FAKE_CALL_Q, // RDA; Activate fake call?
    SS_ACTIVATE_INFRARED, // RDA; Activate infrared
    SS_ACTIVATE_MSENTBOX, // RDA; Activate
    SS_ACTIVATE_OFFLINE_PROFILE_Q, // RDA; Activate offline profile?
    SS_ACTIVATE_SHORTCUTS_TOOLBAR_Q, // RDA; Activate Shortcuts toolbar?
    SS_ACTIVATE_TOOLBAR_LA, // RDA; SIM info will be hidden on idle screen. Do you want to continue?
    SS_ACTIVATE_YOUTH_PROTECTION_Q, // RDA; Activate youth protection?
    SS_ACTIVATING_BLUETOOTH_ING, // RDA; Activating Bluetooth...
    SS_ACTIVATING_ING, // RDA; Activating...
    SS_ACTIVATING_IRDA_ING, // RDA; Activating IrDA...
    SS_ACTIVATING_KEY_ING, // RDA; Activating key...
    SS_ACTIVATING_OFFLINE_PROFILE_WILL_DISCONNECT_ALL_NETWORK_SERVICES_CONTINUE_Q, // RDA; Activating Offline profile will disconnect all network services. Continue?
    SS_ACTIVATION, // RDA; Activation
    SS_ACTIVATION_MBT, // RDA; Activation
    SS_ACTIVE, // RDA; Active
    SS_ACTIVE_CALL, // RDA; Active call
    SS_ACTIVE_CONTACTS, // RDA; Active contacts
    SS_ACTIVE_FLIP, // RDA; Active flip
    SS_ACTIVE_LINE, // RDA; Active line
    SS_ACTIVITY_LOG, // RDA; Activity log
    SS_ADAM, // RDA; Adam
    SS_ADANA, // RDA; Adana
    SS_ADD, // RDA; Add
    SS_ADDED, // RDA; Added
    SS_ADDEMAILADDR_CHANGETO_MMS_CONTINUE_Q, // RDA; Adding an email address will change message to a multimedia message. Continue?
    SS_ADDING_CONTACT_ING, // RDA; Adding contact...
    SS_ADDING_ING, // RDA; Adding...
    SS_ADDING_PD_CONTACTS_ING, // RDA; Adding %d contacts...
    SS_ADDING_SUBJECT_WILL_CHANGE_TEXT_MESSAGE_TO_MULTIMEDIA_MESSAGE_CONTINUE_Q, // RDA; Adding subject will change text message to multimedia message. Continue?
    SS_ADDITIONAL_INFO, // RDA; Additional info
    SS_ADDITIONAL_INFORMATION, // RDA; Additional information
    SS_ADDRESS, // RDA; Address
    SS_ADDRESS_BOOK, // RDA; Address book
    SS_ADDRESS_BOOK_SETTINGS, // RDA; Address book settings
    SS_ADDRESS_FACEBOOK, // RDA; Address
    SS_ADDRESS_FIELDS, // RDA; Address fields
    SS_ADDRESS_LIST, // RDA; Address list
    SS_ADDRESS_UNRESOLVED, // RDA; Address unresolved
    SS_ADD_ALL, // RDA; Add all
    SS_ADD_ANIMATION, // RDA; Add animation
    SS_ADD_ATTACHMENT, // RDA; Add attachment
    SS_ADD_AUDIO, // RDA; Audio
    SS_ADD_BOOKMARK, // RDA; Add bookmark
    SS_ADD_CHANNEL, // RDA; Add channel
    SS_ADD_CONTACTS, // RDA; Add contacts
    SS_ADD_CONTACT_TO, // RDA; Add contact to
    SS_ADD_CURRENCY, // RDA; Add currency
    SS_ADD_DETAILS, // RDA; Add details
    SS_ADD_FROM, // RDA; Add from
    SS_ADD_FROM_CONTACTS, // RDA; Add from Contacts
    SS_ADD_FROM_MEMORY_CARD, // RDA; Add from memory card
    SS_ADD_GROUP, // RDA; Add group
    SS_ADD_IMAGE, // RDA; Add image
    SS_ADD_MELODY, // RDA; Add melody
    SS_ADD_MEMBER, // RDA; Add member
    SS_ADD_MEMBERS, // RDA; Add members
    SS_ADD_MULTIMEDIA, // RDA; Add multimedia
    SS_ADD_MULTIPLE, // RDA; Add multiple
    SS_ADD_MUSIC, // RDA; Add music
    SS_ADD_MUSIC_FROM, // RDA; Add music from
    SS_ADD_MUSIC_MP, // RDA; Add music
    SS_ADD_MYFAVES_CONTACT, // RDA; Add MyFaves contact
    SS_ADD_MY_NAMECARD, // RDA; Add my namecard
    SS_ADD_NEW, // RDA; Add new
    SS_ADD_NEW_CONTACT, // RDA; Add new contact
    SS_ADD_ORGANISER, // RDA; Add organiser
    SS_ADD_OTHER_FILES, // RDA; Add other files
    SS_ADD_PAGE, // RDA; Add page
    SS_ADD_PICTURE, // RDA; Add picture
    SS_ADD_RATE, // RDA; Add rate
    SS_ADD_RECIPIENTS, // RDA; Add recipients
    SS_ADD_SIGNATURE, // RDA; Add signature
    SS_ADD_SOUND, // RDA; Sound
    SS_ADD_SOUNDS, // RDA; Add sounds
    SS_ADD_SUBJECT, // RDA; Add subject
    SS_ADD_SYMBOL, // RDA; Add symbol
    SS_ADD_SYMBOLS, // RDA; Add symbols
    SS_ADD_TEXT, // RDA; Add text
    SS_ADD_TO, // RDA; Add to
    SS_ADD_TO_BLOCK_LIST, // RDA; Add to block list
    SS_ADD_TO_BOOKMARKS, // RDA; Add to Bookmarks
    SS_ADD_TO_CONTACT, // RDA; Add to contact
    SS_ADD_TO_CONTACTS, // RDA; Add to Contacts
    SS_ADD_TO_FAVOURITES, // RDA; Add to Favourites
    SS_ADD_TO_FAVOURITES_T_RADIO, // RDA; Add to Favourites
    SS_ADD_TO_GROUP, // RDA; Add to group
    SS_ADD_TO_PHONEBOOK, // RDA; Add to Phonebook
    SS_ADD_TO_PLAYLIST, // RDA; Add to playlist
    SS_ADD_TO_REJECT_LIST, // RDA; Add to reject list
    SS_ADD_TO_SPEED_DIAL, // RDA; Add to speed dial
    SS_ADD_VIDEO, // RDA; Video
    SS_ADD_VIDEOS, // RDA; Add videos
    SS_ADD_WORD, // RDA; Add word
    SS_ADELAIDE, // RDA; Adelaide
    SS_ADEN, // RDA; Aden
    SS_ADHAN, // RDA; Azan
    SS_ADJUST, // RDA; Adjust
    SS_ADVANCED, // RDA; Advanced
    SS_ADVANCED_ADD, // RDA; Advanced add
    SS_ADVANCED_SETTINGS, // RDA; Advanced settings
    SS_ADVERTISEMENT, // RDA; Advertisement
    SS_ADVERT_ALLOWED, // RDA; Advert allowed
    SS_AD_DAHI, // RDA; Ad Dahi
    SS_AD_DAKHILAH, // RDA; Dakhla Oasis
    SS_AD_DAWHAH_AL_JADIDAH, // RDA; Ad Dawhah al Jadidah
    SS_AD_DAWR, // RDA; Ad-Dawr
    SS_AD_DIWANIYAH, // RDA; Diwaniyah
    SS_AED, // RDA; AED
    SS_AFAK, // RDA; Afak
    SS_AFGHANISTAN_M_COUNTRYNAME, // RDA; Afghanistan
    SS_AFIF, // RDA; Afif
    SS_AFTER_10_SECONDS, // RDA; After 10 seconds
    SS_AFTER_15_SECONDS, // RDA; After 15 seconds
    SS_AFTER_1_HOUR, // RDA; After 1 hour
    SS_AFTER_20_SECONDS, // RDA; After 20 seconds
    SS_AFTER_2_HOURS, // RDA; After 2 hours
    SS_AFTER_30_MIN, // RDA; After 30 mins
    SS_AFTER_5_SECONDS, // RDA; After 5 seconds
    SS_AGADIR, // RDA; Agadir
    SS_AGELAT, // RDA; Al Ajaylat
    SS_AHMADI, // RDA; Al Ahmadi
    SS_AHMEDABAD, // RDA; Ahmedabad
    SS_AHVAZ, // RDA; Ahvaz
    SS_AJIM, // RDA; Ajim
    SS_AJLUN, // RDA; Ajloun
    SS_AJMAN, // RDA; Ajman
    SS_AKHMIM, // RDA; Akhmim
    SS_ALARM, // RDA; Alarm
    SS_ALARMS, // RDA; Alarms
    SS_ALARM_1, // RDA; Alarm 1
    SS_ALARM_10, // RDA; Alarm 10
    SS_ALARM_2, // RDA; Alarm 2
    SS_ALARM_3, // RDA; Alarm 3
    SS_ALARM_4, // RDA; Alarm 4
    SS_ALARM_5, // RDA; Alarm 5
    SS_ALARM_6, // RDA; Alarm 6
    SS_ALARM_7, // RDA; Alarm 7
    SS_ALARM_8, // RDA; Alarm 8
    SS_ALARM_9, // RDA; 
    SS_ALARM_ACTIVATION, // RDA; Alarm activation
    SS_ALARM_DATE, // RDA; Alarm date
    SS_ALARM_DATE_SHOULD_NOT_EXCEED_30_DAYS_BEFORE_START_DATE, // RDA; Alarm date should not exceed 30 days before start date
    SS_ALARM_DAY, // RDA; Alarm day
    SS_ALARM_OFF, // RDA; Alarm off
    SS_ALARM_TIME, // RDA; Alarm time
    SS_ALARM_TIME_AND_DATE, // RDA; Alarm time and date
    SS_ALARM_TIME_AND_DATE_SHOULD_BE_BEFORE_START_TIME_AND_DATE, // RDA; Alarm time and date should be before start time and date
    SS_ALARM_TIME_IS_ALREADY_PASSED, // RDA; Alarm time is already passed
    SS_ALARM_TIME_MEVENT, // RDA; Alarm time
    SS_ALARM_TONE, // RDA; Alarm tone
    SS_ALARM_TONE_MMELODY, // RDA; Alarm tone
    SS_ALARM_TYPE, // RDA; Alarm type
    SS_ALARM_T_CALENDAR, // RDA; Alarm
    SS_ALARM_VOLUME, // RDA; Alarm volume
    SS_ALARM_WILL_RING_AGAIN_IN_1_MIN_POWER_REMAINS_ON, // RDA; Alarm will ring again in 1 min. Power remains on
    SS_ALASKA, // RDA; Alaska
    SS_ALAVI, // RDA; Alavi
    SS_ALBUM, // RDA; Album
    SS_ALBUMS, // RDA; Albums
    SS_ALBUM_ART, // RDA; Album art
    SS_ALBUM_LIST, // RDA; Album list
    SS_ALBUM_MOVISTAR, // RDA; Álbum movistar
    SS_ALBUM_NAME, // RDA; Album name
    SS_ALEPPO, // RDA; Aleppo
    SS_ALERT, // RDA; Alert
    SS_ALERTAS_EMOCION, // RDA; Alertas emoción
    SS_ALERTS_ON_CALL, // RDA; Alerts during calls
    SS_ALERT_ON_CALL, // RDA; Alert on call
    SS_ALERT_REPETITION, // RDA; Alert repetition
    SS_ALERT_TONE, // RDA; Alert tone
    SS_ALERT_TYPE, // RDA; Alert type
    SS_ALEUTIAN, // RDA; Aleutian Islands
    SS_ALEXANDRIA, // RDA; Alexandria
    SS_ALGERIA_M_COUNTRYNAME, // RDA; Algeria
    SS_ALGIERS, // RDA; Algiers
    SS_ALICE_MAIL, // RDA; Alice Mail
    SS_ALIGNMENT, // RDA; Alignment
    SS_ALL, // RDA; All
    SS_ALLOW_ADD_FOLDER_BY_PS_Q, // RDA; Allow add folder by %s?
    SS_ALLOW_ADVERTISEMENT, // RDA; Allow advertisement
    SS_ALLOW_ANONYMOUS, // RDA; Allow anonymous
    SS_ALLOW_PS_TO_CONNECT_Q, // RDA; Allow %s to connect?
    SS_ALLOW_PS_TO_DELETE_YOUR_FILE_Q, // RDA; Allow %s to delete your file?
    SS_ALLOW_PS_TO_GET_YOUR_FILE_Q, // RDA; Allow %s to get your file?
    SS_ALLOW_PS_TO_GET_YOUR_NAMECARD_Q, // RDA; Allow %s to get your namecard?
    SS_ALLOW_PS_TO_PUT_A_FILE_IN_Q, // RDA; Allow %s to put a file in?
    SS_ALLOW_TO_READ_USER_DATA_Q, // RDA; Allow to read user data?
    SS_ALLOW_TO_USE_AUTOMATIC_START_Q, // RDA; Allow to use automatic start?
    SS_ALLOW_TO_USE_CONNECTIVITY_Q, // RDA; Allow to use connectivity?
    SS_ALLOW_TO_USE_MEDIA_RECORDING_Q, // RDA; Allow to use media recording?
    SS_ALLOW_TO_USE_MESSAGING_Q, // RDA; Allow to use messaging?
    SS_ALLOW_TO_USE_NETWORK_ACCESS_Q, // RDA; Allow to use network access?
    SS_ALLOW_TO_WRITE_USER_DATA_Q, // RDA; Allow to write user data?
    SS_ALL_ASYNCHRONOUS_DATA, // RDA; All asynchronous data
    SS_ALL_ASYNCHRONOUS_SERVICES, // RDA; All asynchronous services
    SS_ALL_BARRING, // RDA; All barring
    SS_ALL_BUT_REPEATING_OR_OVERLAPPING_EVENTS, // RDA; All but repeating or overlapping events
    SS_ALL_CALLS, // RDA; All calls
    SS_ALL_CALLS_BARRED, // RDA; All calls barred
    SS_ALL_CALLS_FORWARDED, // RDA; All calls forwarded
    SS_ALL_CHANNELS, // RDA; All channels
    SS_ALL_CONTACTS, // RDA; All contacts
    SS_ALL_DATA, // RDA; All data
    SS_ALL_DATA_AND_SETTINGS_WILL_BE_ERASED_CONTINUE_Q, // RDA; All data and settings will be erased. Continue?
    SS_ALL_DATA_Q, // RDA; All data?
    SS_ALL_DIVERTING, // RDA; All diverting
    SS_ALL_EMAILS, // RDA; All emails
    SS_ALL_EVENTS, // RDA; All events
    SS_ALL_INCOMING, // RDA; All incoming
    SS_ALL_INCOMING_CALLS, // RDA; All incoming calls
    SS_ALL_INCOMING_CALLS_BARRED, // RDA; All incoming calls barred
    SS_ALL_INCOMING_CALLS_WHEN_ROAMING, // RDA; All Incoming calls when roaming
    SS_ALL_INCOMING_CALLS_WHEN_ROAMING_GA, // RDA; All Incoming calls when roaming
    SS_ALL_LOGS, // RDA; All logs
    SS_ALL_MEMORY, // RDA; All memories
    SS_ALL_MMENU_SUBJECT, // RDA; All
    SS_ALL_MOPT_OBJECT, // RDA; All
    SS_ALL_MREMOVE_ALL_OBJECT, // RDA; All
    SS_ALL_OUTGOING, // RDA; All outgoing
    SS_ALL_OUTGOING_CALLS, // RDA; All outgoing calls
    SS_ALL_OUTGOING_CALLS_BARRED, // RDA; All outgoing calls barred
    SS_ALL_PADCA_SERVICES, // RDA; All PADCA services
    SS_ALL_PAGES, // RDA; All pages
    SS_ALL_PDS_SERVICES, // RDA; All PDS services
    SS_ALL_PHONE_CONTACTS, // RDA; All phone contacts
    SS_ALL_PRAYERS_RESET, // RDA; All prayers reset
    SS_ALL_SIM_CONTACTS, // RDA; All SIM contacts
    SS_ALL_STATIONS, // RDA; All stations
    SS_ALL_STEPS, // RDA; All steps
    SS_ALL_SYNCHRONOUS_DATA, // RDA; All synchronous data
    SS_ALL_SYNCHRONOUS_SERVICES, // RDA; All synchronous services
    SS_ALL_TELECOM_SERVICES, // RDA; All telecom services
    SS_ALL_TRACKS, // RDA; All tracks
    SS_ALL_UNLOCKED, // RDA; All unlocked
    SS_ALL_VIA_EMAIL, // RDA; All via Email
    SS_ALL_VIA_MMS, // RDA; All via MMS
    SS_ALMATY, // RDA; Almaty
    SS_ALREADY_ACTIVATED, // RDA; Already activated
    SS_ALREADY_ASSIGNED_SELECT_ANOTHER_ICON, // RDA; Already assigned. Select another icon
    SS_ALREADY_DEACTIVATED, // RDA; Already deactivated
    SS_ALREADY_EXISTS, // RDA; Already exists
    SS_ALREADY_EXIST_LOCKED_FILE_CANNOT_BE_OVERWRITTEN_RENAME_Q, // RDA; Already exists. Protected file cannot be overwritten. Rename?
    SS_ALREADY_IN_USE, // RDA; Already in use
    SS_ALREADY_IN_USE_D_VIEW_CONTACT_DETAILS_Q, // RDA; Already in use. View contact details?
    SS_ALREADY_IN_USE_OVERWRITE_Q, // RDA; Already in use. Overwrite?
    SS_ALREADY_IN_USE_RENAME_PS_Q, // RDA; Already in use. Rename %s?
    SS_ALREADY_IN_USE_RENAME_Q, // RDA; Already in use. Rename?
    SS_ALREADY_IN_USE_SAVE_Q, // RDA; Already in use. Save?
    SS_ALREADY_IN_USE_UNABLE_TO_OVERWRITE_FILE_BEING_PRINTED_RENAME_Q, // RDA; Already in use. Unable to overwrite file being printed. Rename?
    SS_ALREADY_SET_PRAYER, // RDA; is already set at the same day.Would you like to save current prayer?
    SS_ALSO_ON_COVER_DISPLAY, // RDA; Also on cover display
    SS_ALTERNATE_BETWEEN_SPEECH_AND_ASYNCHRONOUS_DATA_SERVICES, // RDA; Alternate between speech and asynchronous data services
    SS_ALTERNATE_BETWEEN_SPEECH_AND_SYNCHRONOUS_DATA_SERVICES, // RDA; Alternate between speech and synchronous data services
    SS_ALTERNATIVE_FREQUENCY_P, // RDA; Alternative frequency
    SS_ALWAYS, // RDA; Always
    SS_ALWAYS_ALLOW, // RDA; Always allow
    SS_ALWAYS_ALLOWED, // RDA; Always allowed
    SS_ALWAYS_ASK, // RDA; Always ask
    SS_ALWAYS_OFF, // RDA; Always off
    SS_ALWAYS_ON, // RDA; Always on
    SS_ALWAYS_ON_METC, // RDA; Always on
    SS_ALWAYS_ON_METC_ABB, // RDA; Always on
    SS_AL_ABRAG, // RDA; Al Abraq
    SS_AL_ADHBAH, // RDA; Al `Adhbah
    SS_AL_AIN, // RDA; Al Ain
    SS_AL_ARISH, // RDA; Arish
    SS_AL_AWDAH, // RDA; Al 'Awdah
    SS_AL_AWJA, // RDA; Al-Awja
    SS_AL_AZIZIYAH, // RDA; Al 'Aziziyah
    SS_AL_BAHAH, // RDA; Al Bahah
    SS_AL_BAYDA, // RDA; Al Bayda
    SS_AL_BIDA_ASH_SHARQIYAH, // RDA; Al Bida` ash Sharqiyah
    SS_AL_BUDAYYI, // RDA; Budaiya
    SS_AL_FAHLAYN, // RDA; Al Fahlayn
    SS_AL_FARA, // RDA; Al Fara'
    SS_AL_FAYYUM, // RDA; Faiyum
    SS_AL_FULAYYAH, // RDA; Al Fulayyah
    SS_AL_GHABAH, // RDA; Al Ghabah
    SS_AL_GHABAM, // RDA; Al Ghabam
    SS_AL_GHANIM, // RDA; Al Ghanim
    SS_AL_GHARIYAH, // RDA; Al Ghariyah
    SS_AL_GHASHBAN, // RDA; Al Ghashban
    SS_AL_HAMRA, // RDA; Al Hamra
    SS_AL_HAMRANIYAH, // RDA; Al Hamraniyah
    SS_AL_HAMRIYAH, // RDA; Al Hamriyah
    SS_AL_HAREEQ, // RDA; Al-Hareeq
    SS_AL_HASAKAH, // RDA; Al-Hasakah
    SS_AL_HAYBAH, // RDA; Al Haybah
    SS_AL_HAYL, // RDA; Al Hayl
    SS_AL_HAYR, // RDA; Al Hayr
    SS_AL_HAYRAH, // RDA; Al Hayrah
    SS_AL_HILAL_AL_GHARBIYAH, // RDA; Al Hilal al Gharbiyah
    SS_AL_HILAL_ASH_SHARQIYAH, // RDA; Al Hilal ash Sharqiyah
    SS_AL_HILLAH, // RDA; Al Hillah
    SS_AL_HISN, // RDA; Al Husn
    SS_AL_HITMI, // RDA; Al Hitmi
    SS_AL_HULAYLAH, // RDA; Al Hulaylah
    SS_AL_ISMAILIYAH, // RDA; Ismailia
    SS_AL_JADDAH, // RDA; Al Jaddah
    SS_AL_JAGHBUB, // RDA; Al-Jaghbub
    SS_AL_JASRAH, // RDA; Al Jasrah
    SS_AL_JAWF, // RDA; Al Jawf
    SS_AL_JUMALIYAH, // RDA; Al Jumaliyah
    SS_AL_KABIYAH, // RDA; Al Ka`biyah
    SS_AL_KARAK, // RDA; Al Karak
    SS_AL_KHALIFAT, // RDA; Al Khalifat
    SS_AL_KHARI, // RDA; Al Khari
    SS_AL_KHARIJAH, // RDA; Al-Kharijah
    SS_AL_KHARJ, // RDA; Al Kharj
    SS_AL_KHASHFAH, // RDA; Al Khashfah
    SS_AL_KHOR, // RDA; Al Khor
    SS_AL_KHUMS, // RDA; Al Khums
    SS_AL_KHUTT, // RDA; Al-Khutt
    SS_AL_KHUWAYR, // RDA; Al Khuwayr
    SS_AL_KUFRAH, // RDA; Al Kufrah
    SS_AL_LITH, // RDA; Al Lith
    SS_AL_MAFJAR, // RDA; Al Mafjar
    SS_AL_MAHAMM, // RDA; Al Mahamm
    SS_AL_MAHWIT, // RDA; Al Mahwit
    SS_AL_MAJMAAH, // RDA; Al Majma'ah
    SS_AL_MALIKIYAH, // RDA; al-Malikiyah
    SS_AL_MANSURAH, // RDA; Mansoura
    SS_AL_MARJ, // RDA; Al Marj
    SS_AL_MASAFIRAH, // RDA; Al Masafirah
    SS_AL_MATAF, // RDA; Al Mataf
    SS_AL_MUAMURAH, // RDA; Al Mu'amurah
    SS_AL_MUHARRAQ, // RDA; Al Muharraq
    SS_AL_MUKALLA, // RDA; Al Mukalla
    SS_AL_NASLAH, // RDA; Al Naslah
    SS_AL_QAABIYAH, // RDA; Al Qa`abiyah
    SS_AL_QAIM, // RDA; Al-Qa'im
    SS_AL_QIR, // RDA; Al Qir
    SS_AL_QUNFUDHAH, // RDA; Al Qunfudhah
    SS_AL_QUWAYZ, // RDA; Al Quwayz
    SS_AL_SALT, // RDA; Salt
    SS_AL_SUWAIQ, // RDA; Suwayq
    SS_AL_ULA, // RDA; Al `Ula
    SS_AL_USAYLI, // RDA; Al Usayli
    SS_AL_WAJH, // RDA; Al Wajh
    SS_AL_WAKRAH, // RDA; Al Wakrah
    SS_AL_ZAWIA, // RDA; Az Zawiyah
    SS_AM, // RDA; AM
    SS_AMARA, // RDA; Amara
    SS_AMMAN, // RDA; Amman
    SS_AMOUNT, // RDA; Amount
    SS_AMPM, // RDA; AM/PM
    SS_AMR, // RDA; Amr
    SS_AMSTERDAME, // RDA; Amsterdam
    SS_ANALOGUE, // RDA; Analogue
    SS_AND, // RDA; and
    SS_ANDISHEH, // RDA; Andisheh
    SS_ANGEL, // RDA; Angel
    SS_ANGRY, // RDA; Angry
    SS_ANIMATION, // RDA; Animation
    SS_ANKARA, // RDA; Ankara
    SS_ANNABA, // RDA; Annaba
    SS_ANNIVERSARIES, // RDA; Anniversaries
    SS_ANNIVERSARY, // RDA; Anniversary
    SS_ANNIVERSARY_IN_PD_DAYS, // RDA; Anniversary in %d days
    SS_ANNOUNCE, // RDA; Announce
    SS_ANONYMOUS, // RDA; Anonymous
    SS_ANSWER, // RDA; Answer
    SS_ANSWERING_MACHINE, // RDA; Answering machine
    SS_ANSWERING_MACHINE_INBOX, // RDA; Answering machine inbox
    SS_ANSWERING_MODE, // RDA; Answering mode
    SS_ANSWER_LIST_FULL_DELETE_SOME_ITEMS, // RDA; Answer list full. Delete some items
    SS_ANSWER_PHONE_NOT_ENTERED_ORANGE, // RDA; Answer phone not entered
    SS_ANTAKYA, // RDA; Antakya
    SS_ANTALYA, // RDA; Antalya
    SS_ANTIQUE, // RDA; Antique
    SS_ANYKEY_ANSWER, // RDA; Anykey answer
    SS_ANY_KEY, // RDA; Any key
    SS_ANY_PC, // RDA; Any PC
    SS_AN_NAJMAH, // RDA; An Najmah
    SS_AN_NUBARIYAH, // RDA; An Nubariyah
    SS_AOL, // RDA; AOL
    SS_APN, // RDA; APN
    SS_APN_ID, // RDA; APN ID
    SS_APOP_LOGIN_FAILED, // RDA; APOP login failed
    SS_APPLICATION, // RDA; Activity
    SS_APPLICATIONS, // RDA; Applications
    SS_APPLICATIONS_SETTING, // RDA; Applications setting
    SS_APPLICATION_EXIT, // RDA; Activity exit
    SS_APPLICATION_PAUSED_CONTINUE_Q, // RDA; Activity paused. Continue?
    SS_APPLICATION_PROFILE, // RDA; Activity profiles
    SS_APPLICATION_SETTINGS, // RDA; Activity settings
    SS_APPLIED, // RDA; Applied
    SS_APPLY_DAYLIGHT_SAVING_TIME, // RDA; Apply daylight saving time
    SS_APPLY_DAYLIGHT_SAVING_TIME_Q, // RDA; Apply daylight saving time?
    SS_APPLY_THEME_Q, // RDA; Apply theme?
    SS_APPLY_TO_ALL_ACCOUNTS_Q, // RDA; Apply to all accounts?
    SS_APPOINTMENT, // RDA; Appointment
    SS_APPOINTMENTS, // RDA; Appointments
    SS_APPS_T_MAINMENU_ABB2, // RDA; Apps
    SS_APR, // RDA; Apr.
    SS_APRIL, // RDA; April
    SS_AP_MAC, // RDA; AP MAC
    SS_AP_ROAMING, // RDA; AP roaming
    SS_AQABA, // RDA; Aqaba
    SS_AQTAU, // RDA; Aktau
    SS_AQUA, // RDA; Aqua
    SS_AQUA_MARINE, // RDA; Aqua marine
    SS_ARAK, // RDA; Arak
    SS_ARAR, // RDA; Arar
    SS_ARCHIVE, // RDA; Archive
    SS_AREA, // RDA; Area
    SS_ARES_MCVT, // RDA; Ares
    SS_ARE_YOU_SURE_Q, // RDA; Are you sure?
    SS_ARIZONA, // RDA; Arizona
    SS_ARS, // RDA; ARS
    SS_ARTHABAN, // RDA; Arthaban
    SS_ARTIST, // RDA; Artist
    SS_ARTISTS, // RDA; Artists
    SS_AR_RAFAAH, // RDA; Ar Rafa'ah
    SS_AR_RAKIYAT, // RDA; Ar Rakiyat
    SS_AR_RAQQAH, // RDA; Ar-Raqqah
    SS_AR_RASS, // RDA; Ar Rass
    SS_AR_RAYYAN, // RDA; Ar Rayyan
    SS_AR_RIFACA, // RDA; Riffa
    SS_AR_RUAYS, // RDA; Ar Ru'ays
    SS_AR_RUTBA, // RDA; Ar Rutba
    SS_ASH_SHAM, // RDA; Ash Sha'm
    SS_ASILAH, // RDA; Asilah
    SS_ASK_EVERY_TIME, // RDA; Ask every time
    SS_ASK_ONCE_PER_USE, // RDA; Ask once per use
    SS_ASK_ONE_TIME_TMO, // RDA; Ask one-time
    SS_ASK_ON_CONNECTION, // RDA; Ask on connection
    SS_ASR, // RDA; Asr
    SS_ASSAMESE_INPUT, // RDA; কখগ
    SS_ASSIGN, // RDA; Assign
    SS_ASSIGNED, // RDA; Assigned
    SS_ASSIGN_PS_PS_AS_MYFAVES_CONTACT_Q_VERIFY_NUM_BEFORE_CONTINUING, // RDA; Assign %s %s as a MyFaves contact?\nPlease verify number before continuing
    SS_ASSOCIATED_WITH, // RDA; Associated with
    SS_ASTANA, // RDA; Astana
    SS_ASWAN, // RDA; Aswan
    SS_ASYNCHRONOUS_DATA_SERVICES, // RDA; Asynchronous data services
    SS_ASYUT, // RDA; Asyut
    SS_AS_SALATAH, // RDA; As Salatah
    SS_AS_SALATAH_AL_JADIDAH, // RDA; As Salatah al Jadidah
    SS_AS_SANI, // RDA; As Sani`
    SS_AS_SAWQ, // RDA; As Sawq
    SS_AS_SIB, // RDA; Seeb
    SS_AS_SULAYYIL, // RDA; As Sulayyil
    SS_AS_SUN_HAS_PASSED_BELOW_HORIZON_DIRECTIONS_CANNOT_BE_FOUND, // RDA; As Sun has passed below horizon, directions cannot be found
    SS_AS_SUR, // RDA; As Sur
    SS_AS_SUWAYDA, // RDA; As-Suwayda
    SS_ATAK, // RDA; Ataq
    SS_ATHABAT, // RDA; Athabat
    SS_ATHENS, // RDA; Athens
    SS_ATH_THAQAB, // RDA; Ath Thaqab
    SS_ATLANTIC_CANADA, // RDA; Atlantic Canada
    SS_ATTACH, // RDA; Attach
    SS_ATTACHED_FILES, // RDA; Attached files
    SS_ATTACHED_ITEMS, // RDA; Attached items
    SS_ATTACHMENT, // RDA; Attachment
    SS_ATTACHMENT_LIST, // RDA; Attachment list
    SS_ATTACH_FILE, // RDA; Files 
    SS_ATTACH_ITEMS, // RDA; Attach items
    SS_ATTACH_MEMO, // RDA; Memo
    SS_ATTACH_MULTIMEDIA, // RDA; Attach multimedia
    SS_ATTACH_NAMECARD, // RDA; Namecard
    SS_ATTACH_ORGANISER, // RDA; Attach organiser
    SS_ATTENTION_E, // RDA; Attention!
    SS_AUCKLAND, // RDA; Auckland
    SS_AUDIO_CONNECT_WITH_PS_Q, // RDA; Audio connect with %s?
    SS_AUDIO_OFF, // RDA; Audio off
    SS_AUDIO_ON, // RDA; Audio on
    SS_AUDIO_RECORDING, // RDA; Audio Recording
    SS_AUDIO_REMOTE_CONTROL, // RDA; Audio remote control
    SS_AUG, // RDA; Aug.
    SS_AUGUST, // RDA; August
    SS_AUTHENTICATED_REGISTERING_TO_NETWORK_SERVER, // RDA; Registering device with the network server...
    SS_AUTHENTICATING_WITH_NETWORK_SERVER_ING, // RDA; Authenticating network server and device...
    SS_AUTHENTICATION, // RDA; Authentication
    SS_AUTHENTICATION_ERROR, // RDA; Authentication error
    SS_AUTHENTICATION_FAILED, // RDA; Authentication failed
    SS_AUTHENTICATION_MODE, // RDA; Authentication mode
    SS_AUTHENTICATION_TYPE_FAILED, // RDA; Authentication failed
    SS_AUTHOR, // RDA; Author
    SS_AUTHORISATION_FAILED, // RDA; Authorisation failed
    SS_AUTHORISE, // RDA; Authorise
    SS_AUTHORISED, // RDA; Authorised
    SS_AUTHORISE_DEVICE, // RDA; Authorise device
    SS_AUTHORIZATION_REQUIRED_P, // RDA; Authorization required
    SS_AUTH_TYPE, // RDA; Auth type
    SS_AUTO, // RDA; Auto
    SS_AUTOEQ, // RDA; AutoEQ
    SS_AUTOFILL, // RDA; Autofill
    SS_AUTOLLAMADA, // RDA; Fake call
    SS_AUTOLLAMADA_HOT_KEY, // RDA; Fake call hot key
    SS_AUTOLLAMADA_SETTINGS, // RDA; Fake call settings
    SS_AUTOLLAMADA_TIMER, // RDA; Fake call timer
    SS_AUTOMATIC, // RDA; Automatic
    SS_AUTOMATIC_ANSWERING_IS_NOT_POSSIBLE_WHEN_RINGTONE_IS_SET_TO_MUTE, // RDA; Automatic answering is not possible when ringtone is set to mute
    SS_AUTOMATIC_ANSWERING_WITH_EARPIECE_HELP_TEXT_MSG, // RDA; Incoming calls will be answered automatically. Automatic answering will operate only with earpiece connected. Automatic answering is not possible when call alert type is set to mute
    SS_AUTOMATIC_DELETION, // RDA; Automatic deletion
    SS_AUTOMATIC_START, // RDA; Automatic start
    SS_AUTOMATIC_SYNC_TMO, // RDA; Automatic sync
    SS_AUTOMATIC_TIME_ZONE_UPDATE_ACTIVATED_DEACTIVATE_AND_SET_TIMENDATE_Q, // RDA; Automatic time zone update is activated. Deactivate and set time and date?
    SS_AUTOMATIC_T_ANSWERINGMODE, // RDA; Automatic
    SS_AUTO_DOWNLOAD, // RDA; Auto download
    SS_AUTO_EMAIL_SYNC, // RDA; Auto email sync
    SS_AUTO_EMAIL_SYNC_OFF, // RDA; Auto email sync off
    SS_AUTO_EMAIL_SYNC_ON, // RDA; Auto email sync on
    SS_AUTO_EMAIL_SYNC_SUPPORT_ONLY_ONE_MAILBOX_THIS_MAY_INCUR_HIGH_COST, // RDA; Auto email sync supports only one mailbox. This may incur high costs
    SS_AUTO_FM_RADIO_OFF, // RDA; FM radio auto off
    SS_AUTO_FOLDER_SETTINGS, // RDA; Auto folder settings
    SS_AUTO_KEYPAD_LOCK, // RDA; Keypad auto-lock
    SS_AUTO_LEVEL, // RDA; Auto level
    SS_AUTO_POLLING, // RDA; Auto polling
    SS_AUTO_POWER, // RDA; Auto power
    SS_AUTO_POWERUP, // RDA; Auto power-up
    SS_AUTO_POWER_ON, // RDA; Auto power on
    SS_AUTO_REDIAL, // RDA; Auto redial
    SS_AUTO_REJECT, // RDA; Auto reject
    SS_AUTO_RESIZE_IMAGE, // RDA; Auto resize image
    SS_AUTO_SCAN, // RDA; Auto scan
    SS_AUTO_SCAN_FAILED_SIGNAL_TOO_WEAK, // RDA; Auto scan failed. Signal too weak
    SS_AUTO_SCAN_LA, // RDA; Auto scan
    SS_AUTO_T_WHITEBALANCE, // RDA; Auto
    SS_AUTO_UPDATE, // RDA; Auto update
    SS_AUTUMN_GOLD, // RDA; Autumn gold
    SS_AVAILABLE, // RDA; Available
    SS_AVAILABLE_ABB, // RDA; Available
    SS_AVAILABLE_USES, // RDA; Available uses
    SS_AWANAT, // RDA; Awanat
    SS_AZAAN, // RDA; Azan
    SS_AZAN_DISABLE_ALL, // RDA; All other prayers will be turned off. Set Azan?
    SS_AZAR_MEA, // RDA; Azar
    SS_AZILAL, // RDA; Azilal
    SS_AZORES, // RDA; Azores
    SS_AZROU, // RDA; Azrou
    SS_AZ_ZAIMAH, // RDA; Az Zaimah
    SS_AZ_ZARQA, // RDA; Az Zarqa
    SS_A_QUESTION_MESSAGE_VVM_HAS_BEEN_SET_TO_ACTIVATE_TMO, // RDA; Since messages are automatically deleted on your mail server after a certain time you save store any message you want to keep. As default callers will only hear your phone number. They will more likely leave a message if your record a personal greeting.
    SS_A_QUESTION_MESSAGE_WHEN_FIRST_USE_MOBILBOX_TMO, // RDA; People hearing your voicemail currently just hear your phone number. They are more likely to leave a message if they hear a recorded greeting from you. Would you like to record on now?
    SS_B, // RDA; B
    SS_BAALBEK, // RDA; Baalbek
    SS_BABY_BLUE, // RDA; Baby blue
    SS_BABY_GIRL, // RDA; Baby girl
    SS_BAB_EZZOUAR, // RDA; Bab Ezzouar
    SS_BACK, // RDA; Back
    SS_BACKGROUND, // RDA; Background
    SS_BACKGROUND_COLOUR, // RDA; Background colour
    SS_BACKGROUND_PLAYING, // RDA; Background playing
    SS_BACKLIGHT, // RDA; Backlight
    SS_BACKLIGHT_MCAMERASCENE, // RDA; Backlight
    SS_BACKLIGHT_ON, // RDA; Backlight on
    SS_BACKLIGHT_TIME, // RDA; Backlight time
    SS_BACKWARD, // RDA; Backward
    SS_BADR, // RDA; Badr
    SS_BAD_GATEWAY, // RDA; Bad gateway
    SS_BAD_REQUEST, // RDA; Bad request
    SS_BAGHDAD, // RDA; Baghdad
    SS_BAGHDADI, // RDA; Khan al Baghdadi
    SS_BAHARESTAN, // RDA; Baharestan
    SS_BAHAWALPUR, // RDA; Bahawalpur
    SS_BAHLA, // RDA; Bahla
    SS_BAHMAN_MEA, // RDA; Bahman
    SS_BAHRAIN, // RDA; Bahrain
    SS_BAIJI, // RDA; Baiji
    SS_BAJA_CALIFORNIA_NORTE, // RDA; Baja California
    SS_BAJA_CALIFORNIA_SUR, // RDA; Baja California Sur
    SS_BAKRI_ID_SWA, // RDA; Bakri Id
    SS_BALAD, // RDA; Balad
    SS_BALDING, // RDA; Balding
    SS_BALJURASHI, // RDA; Baljurashi
    SS_BALKH, // RDA; Balkh
    SS_BALL, // RDA; Ball
    SS_BALLOON, // RDA; Balloon
    SS_BAM, // RDA; Bam
    SS_BAMYAN, // RDA; Bamyan
    SS_BAND_SELECTION, // RDA; Band selection
    SS_BANGKOK, // RDA; Bangkok
    SS_BANHA, // RDA; Banha
    SS_BANI_WALID, // RDA; Bani Walid
    SS_BAQAL, // RDA; Baqal
    SS_BAQUBA, // RDA; Ba'qubah
    SS_BARAKI, // RDA; Baraki
    SS_BARIKA, // RDA; Barika
    SS_BARKA, // RDA; Barka
    SS_BARRING, // RDA; Barring
    SS_BARRING_INCOMING_CALLS, // RDA; Barring incoming calls
    SS_BASE, // RDA; Base
    SS_BASIC_CONSTRAINTS_ERROR, // RDA; Basic constraints error
    SS_BASIC_DETAILS, // RDA; Basic details
    SS_BASIC_INFORMATION, // RDA; Basic information
    SS_BASIC_MBROWSER, // RDA; Basic
    SS_BASIC_MCOLOR, // RDA; Basic
    SS_BASIC_PRINTING, // RDA; Basic printing
    SS_BASRA, // RDA; Basra
    SS_BASS, // RDA; Bass
    SS_BASSBOOSTER, // RDA; Bassbooster
    SS_BATNA, // RDA; Batna
    SS_BATTERY_FULLY_CHARGED_UNPLUG_CHARGER, // RDA; Battery fully charged.\nUnplug charger
    SS_BATTERY_FULLY_CHARGED_UNPLUG_CHARGER_ABB, // RDA; Battery fully charged. Unplug charger
    SS_BAYAN, // RDA; Bayan
    SS_BAYT_AL_FAQIH, // RDA; Bayt al-Faqih
    SS_BCC, // RDA; Bcc
    SS_BEARER, // RDA; Bearer
    SS_BEARER_CAPABILITY_CHANGED, // RDA; Bearer capability changed
    SS_BEARER_SELECTION, // RDA; Bearer selection
    SS_BEATS, // RDA; Beats
    SS_BEAT_1, // RDA; Beat 1
    SS_BEAT_2, // RDA; Beat 2
    SS_BEAT_3, // RDA; Beat 3
    SS_BEAT_4, // RDA; Beat 4
    SS_BEAT_5, // RDA; Beat 5
    SS_BEAT_6, // RDA; Beat 6
    SS_BEAT_7, // RDA; Beat 7
    SS_BEAT_8, // RDA; Beat 8
    SS_BEAVER_GREY, // RDA; Beaver grey
    SS_BECHAR, // RDA; Bechar
    SS_BEEP, // RDA; Beep
    SS_BEER, // RDA; Beer
    SS_BEFORE, // RDA; Before
    SS_BEFORE_DATE, // RDA; Before date
    SS_BEFORE_LC, // RDA; before
    SS_BEIJING, // RDA; Beijing
    SS_BEIRUT, // RDA; Beirut
    SS_BEJAIA, // RDA; Bejaia
    SS_BEKALTA, // RDA; Bekalta
    SS_BELGRADE, // RDA; Belgrade
    SS_BELO_HORIZONTE, // RDA; Belo Horizonte
    SS_BENGALI_INPUT, // RDA; কখগ
    SS_BENGALURU, // RDA; Bangalore
    SS_BENGALURU_IND, // RDA; Bangalore
    SS_BENGHAZI, // RDA; Benghazi
    SS_BENI_MELLAL, // RDA; Beni Mellal
    SS_BENI_MTIR, // RDA; Beni M'Tir
    SS_BENI_SUEF, // RDA; Beni Suef
    SS_BEN_GARDANE, // RDA; Ben Gardane
    SS_BERET, // RDA; Beret
    SS_BERKANE, // RDA; Berkane
    SS_BERLIN, // RDA; Berlin
    SS_BERN, // RDA; Bern
    SS_BE_RIGHT_BACK, // RDA; Be right back
    SS_BIDBID, // RDA; Bidbid
    SS_BIDIYAH, // RDA; Bidiyah
    SS_BIKE, // RDA; Bike
    SS_BIRTHDAY, // RDA; Birthday
    SS_BIRTHDAYHAT, // RDA; Birthdayhat
    SS_BIRTHDAY_IN_PD_DAYS, // RDA; Birthday in %d days
    SS_BIRTHDAY_TODAY, // RDA; Birthday today
    SS_BISHA, // RDA; Bisha
    SS_BISKRA, // RDA; Biskra
    SS_BIT_RATE, // RDA; Bit rate
    SS_BLACK, // RDA; Black
    SS_BLACK_AND_WHITE, // RDA; Black and white
    SS_BLACK_MSUBJECT, // RDA; Black
    SS_BLIDA, // RDA; Blida
    SS_BLOAT_BOTTOM_HLARGE, // RDA; Bloat bottom (large)
    SS_BLOAT_BOTTOM_HSMALL, // RDA; Bloat bottom (small)
    SS_BLOAT_CENTRE, // RDA; Bloat centre
    SS_BLOCKED, // RDA; Blocked
    SS_BLOCK_ADDRESS, // RDA; Block address
    SS_BLOCK_BY, // RDA; Block by
    SS_BLOCK_BY_ADDRESS, // RDA; Block by address
    SS_BLOCK_ENTRY_FULL, // RDA; Block entry full
    SS_BLOCK_LIST, // RDA; Block list
    SS_BLOCK_LIST_FULL, // RDA; Block list full
    SS_BLOCK_MESSAGES, // RDA; Block messages
    SS_BLOCK_MNOUN, // RDA; Block
    SS_BLOCK_NUMBER, // RDA; Block number
    SS_BLOCK_SUBJECT, // RDA; Block subject
    SS_BLUE, // RDA; Blue
    SS_BLUEBIRD, // RDA; Bluebird
    SS_BLUETOOTH, // RDA; Bluetooth
    SS_BLUETOOTH_ACTIVATED, // RDA; Bluetooth activated
    SS_BLUETOOTH_DUAL_HEADSET, // RDA; Bluetooth stereo dual headset
    SS_BLUETOOTH_HEADSET, // RDA; Bluetooth headset
    SS_BLUETOOTH_HEADSET_DISCONNECTED_PLAY_VIA_PHONE_Q, // RDA; Bluetooth headset disconnected. Play via phone?
    SS_BLUETOOTH_IS_NOT_ACTIVATED_ACTIVATE_BLUETOOTH_Q, // RDA; Bluetooth is not activated. Activate Bluetooth?
    SS_BLUETOOTH_NOT_ACTIVE_ACTIVE_Q, // RDA; Bluetooth is not activated.\nActivate Bluetooth?
    SS_BLUETOOTH_NOW_IN_USE, // RDA; bluetooth now in use
    SS_BLUETOOTH_N_DEACTIVATED, // RDA; Bluetooth\ndeactivated
    SS_BLUETOOTH_PRINT_SERVICES_INFO, // RDA; Used to print images, text messages, personal data such as namecards and appointments, etc. via remote Bluetooth printers
    SS_BLUETOOTH_SERVICES, // RDA; Bluetooth Services
    SS_BLUETOOTH_SETTINGS_GA, // RDA; Bluetooth settings
    SS_BLUETOOTH_STEREO_HEADSET, // RDA; Bluetooth stereo headset
    SS_BLUETOOTH_STEREO_HEADSET_DISCONNECTED, // RDA; Bluetooth stereo headset disconnected
    SS_BLUETOOTH_STEREO_SINGLE_HEADSET, // RDA; Bluetooth stereo single headset
    SS_BLUETOOTH_TESTS, // RDA; Bluetooth tests
    SS_BLUETOOTH_VISIBILITY, // RDA; Bluetooth visibility
    SS_BLUETOOTH_VISIBILITY_DISABLED, // RDA; Bluetooth visibility disabled
    SS_BLUETOOTH_VISIBILITY_ENABLED, // RDA; Bluetooth visibility enabled
    SS_BLUETOOTH_VISIBILITY_OFF, // RDA; Bluetooth visibility off
    SS_BLUETOOTH_VISIBILITY_ON, // RDA; Bluetooth visibility on
    SS_BLUE_VIOLET, // RDA; Blue violet
    SS_BLUR, // RDA; Blur
    SS_BMI, // RDA; BMI
    SS_BOB, // RDA; BOB
    SS_BOBBED, // RDA; Bobbed
    SS_BOLD, // RDA; Bold
    SS_BOLT_ONS, // RDA; Bolt Ons
    SS_BOOKMARK, // RDA; Bookmark
    SS_BOOKMARKS, // RDA; Bookmarks
    SS_BOOKMARK_LIST_FULL, // RDA; Bookmark list full
    SS_BOOTSTRAP_INSTALLATION_SUCCEEDED, // RDA; Bootstrap installation succeeded
    SS_BORDER, // RDA; Border
    SS_BORDJ_BOU_ARRERIDJ, // RDA; Bordj Bou Arreridj
    SS_BOTTOM, // RDA; Bottom
    SS_BOUQUET, // RDA; Bouquet
    SS_BOU_SAADA, // RDA; Bou Saada
    SS_BOWL_CUT, // RDA; Bowl cut
    SS_BOX_IN, // RDA; Box in
    SS_BOX_OUT, // RDA; Box out
    SS_BRAK, // RDA; Birak
    SS_BRASILIA, // RDA; Brasilia
    SS_BRATISLAVA, // RDA; Bratislava
    SS_BREEZE_MTHEME, // RDA; Breeze
    SS_BRIGHTNESS, // RDA; Brightness
    SS_BRIGHTNESS_DOWN, // RDA; Brightness down
    SS_BRIGHTNESS_SOUND, // RDA; Brightness sound
    SS_BRIGHTNESS_UP, // RDA; Brightness up
    SS_BRIGHT_PINE_GREEN, // RDA; Bright pine green
    SS_BRIGHT_TURQUOISE, // RDA; Bright turquoise
    SS_BRISBANE, // RDA; Brisbane
    SS_BRL, // RDA; BRL
    SS_BROADCAST_MESSAGES, // RDA; Broadcast messages
    SS_BROKEN_HEART, // RDA; Broken heart
    SS_BROWSE, // RDA; Browse
    SS_BROWSER, // RDA; Browser
    SS_BROWSER_OPTIONS, // RDA; Browser options
    SS_BROWSER_PROFILES, // RDA; Browser profiles
    SS_BROWSER_SETTINGS, // RDA; Browser settings
    SS_BROWSER_SETTINGS_MBROWSER, // RDA; Browser settings
    SS_BROWSE_FILES, // RDA; Browse files
    SS_BROWSING_FILES_ING, // RDA; Browsing files...
    SS_BRUSSELS, // RDA; Brussels
    SS_BT_AUTHORIZE_SERIALCONNECT_EXCEPTION, // RDA; Used to exchange personal data (namecards, Phonebook contacts, etc.) with Bluetooth devices
    SS_BT_AUTO_CONNECT, // RDA; auto connect
    SS_BT_HEADSET_OFF, // RDA; BT headset off
    SS_BT_HEADSET_ON, // RDA; BT headset on
    SS_BT_SERVICE_AVHEADSET, // RDA; Used to make calls and listen to music via Bluetooth stereo headsets
    SS_BT_SERVICE_DIALUP, // RDA; Used to provide Internet access for Bluetooth devices via the phone's modem
    SS_BT_SERVICE_SERIALPORT, // RDA; Used to connect to other Bluetooth devices via a virtual serial port
    SS_BUCHAREST, // RDA; Bucharest
    SS_BUDAPEST, // RDA; Budapest
    SS_BUDDHA_PURNIMA_SWA, // RDA; Buddha purnima
    SS_BUENOS_AIRES, // RDA; Buenos Aires
    SS_BUILD_MULTIPARTY, // RDA; Build multiparty
    SS_BURAIMI, // RDA; Buraimi
    SS_BURAYDAH, // RDA; Buraydah
    SS_BURSA, // RDA; Bursa
    SS_BUSINESS, // RDA; Business
    SS_BUSY, // RDA; Busy
    SS_BUTTERFLY, // RDA; Butterfly
    SS_BUTTERFLYMASK, // RDA; Butterflymask
    SS_BUTTON_PAIRING, // RDA; Button pairing
    SS_BUY_CALLTIME, // RDA; Buy calltime
    SS_BY, // RDA; By
    SS_BYTE_LC, // RDA; byte
    SS_BY_ADDRESS_ABB, // RDA; By address
    SS_BY_DATE, // RDA; By date
    SS_BY_DAY, // RDA; By day
    SS_BY_NETWORK, // RDA; By network
    SS_CACHE_CLEARED, // RDA; Cache cleared
    SS_CACTUS, // RDA; Cactus
    SS_CAD, // RDA; CAD
    SS_CAIRO, // RDA; Cairo
    SS_CAKE, // RDA; Cake
    SS_CALCULATOR, // RDA; Calculator
    SS_CALENDAR, // RDA; Calendar
    SS_CALENDAR_CANNOT_BE_USED_WHILE_SHORTCUTS_TOOLBAR_IS_ACTIVE, // RDA; Calendar cannot be used while Shortcuts toolbar is active
    SS_CALENDAR_DISPLAY, // RDA; Calendar display
    SS_CALENDAR_EVENT, // RDA; Calendar event
    SS_CALENDAR_EVENTS, // RDA; Calendar events
    SS_CALENDAR_INITIALISING_ING, // RDA; Calendar initialising...
    SS_CALENDAR_RECEIVED, // RDA; Calendar received
    SS_CALENDAR_SENT, // RDA; Calendar sent
    SS_CALENDAR_SETTINGS, // RDA; Calendar settings
    SS_CALENDAR_SYNC_IN_PROGRESS, // RDA; Calendar sync in progress
    SS_CALENDAR_SYNC_STARTED, // RDA; Calendar sync started
    SS_CALENDAR_T_MAINMENU_ABB, // RDA; Calendar
    SS_CALIBRATION, // RDA; Calibration
    SS_CALL, // RDA; Call
    SS_CALLERS_NUMBER_DISPLAYED, // RDA; Caller's number displayed
    SS_CALLERS_NUMBER_NOT_DISPLAYED, // RDA; Caller's number not displayed
    SS_CALLER_AND_SENDER_INFORMATION_DISPLAYED, // RDA; Caller and sender information will be displayed
    SS_CALLER_AND_SENDER_INFORMATION_NOT_DISPLAYED, // RDA; Caller and sender information will not be displayed
    SS_CALLER_ID, // RDA; Caller ID
    SS_CALLER_ID_MNUMBER, // RDA; Caller ID
    SS_CALLER_IMAGE, // RDA; Caller image
    SS_CALLER_INFORMATION, // RDA; Caller information
    SS_CALLER_NAME, // RDA; Caller name
    SS_CALLER_RINGTONE, // RDA; Caller ringtone
    SS_CALLER_RINGTONE_MP, // RDA; Caller ringtone
    SS_CALLING, // RDA; Calling
    SS_CALLS, // RDA; Calls
    SS_CALLSTATUSTONES_CONNECT_TONE, // RDA; There will be a signal sound when the line is connected.
    SS_CALLSTATUSTONES_MUNITE_MINDER, // RDA; There will be signal sounds every minute
    SS_CALLS_BLOCKED_MAXIMUM_AMOUNT_OF_ACM_REACHED_RESET_ACM_TO_UNBLOCK, // RDA; Calls blocked. Maximum amount of ACM reached. Reset ACM to unblock
    SS_CALLTIME_BALANCE, // RDA; Calltime balance
    SS_CALLTIME_RELEASE, // RDA; If you accept incoming call, Call time limit will be set off temporaily
    SS_CALL_ALERT, // RDA; Call alert
    SS_CALL_ALERT_TYPE, // RDA; Call alert type
    SS_CALL_BACK, // RDA; Call back
    SS_CALL_BARRED, // RDA; Call barred
    SS_CALL_BARRING, // RDA; Call barring
    SS_CALL_BARRING_C_ALL_INCOMING_CALLS_ALL_CALLS_ACTIVE, // RDA; Call barring: All incoming calls, All calls, Active
    SS_CALL_BARRING_C_ALL_INCOMING_CALLS_ALL_CALLS_INACTIVE, // RDA; Call barring: All incoming calls, All calls, Inactive
    SS_CALL_BARRING_C_ALL_INCOMING_CALLS_WHEN_ROAMING_ALL_CALLS_ACTIVE, // RDA; Call barring: All incoming calls when roaming, All calls, Active
    SS_CALL_BARRING_C_ALL_INCOMING_CALLS_WHEN_ROAMING_ALL_CALLS_INACTIVE, // RDA; Call barring: All incoming calls when roaming, All calls, Inactive
    SS_CALL_BARRING_C_ALL_OUTGOING_CALLS_ALL_CALLS_ACTIVE, // RDA; Call barring: All outgoing calls, All calls, Active
    SS_CALL_BARRING_C_ALL_OUTGOING_CALLS_ALL_CALLS_INACTIVE, // RDA; Call barring: All outgoing calls, All calls, Inactive
    SS_CALL_BARRING_C_OUTGOING_INTERNATIONAL_CALLS_ALL_CALLS_ACTIVE, // RDA; Call barring: Outgoing international calls, All calls, Active
    SS_CALL_BARRING_C_OUTGOING_INTERNATIONAL_CALLS_ALL_CALLS_INACTIVE, // RDA; Call barring: Outgoing international calls, All calls, Inactive
    SS_CALL_BARRING_C_OUTGOING_INTERNATIONAL_CALLS_EXCEPT_HOME_BARRED_ALL_CALLS_ACTIVE, // RDA; Call barring: Outgoing international calls except home barred, All calls, Active
    SS_CALL_BARRING_C_OUTGOING_INTERNATIONAL_CALLS_EXCEPT_HOME_BARRED_ALL_CALLS_INACTIVE, // RDA; Call barring: Outgoing international calls except home barred, All calls, Inactive
    SS_CALL_BARRING_LA, // RDA; Call barring
    SS_CALL_BUSY, // RDA; Call busy
    SS_CALL_CONNECT_TONE, // RDA; Call connect tone
    SS_CALL_COST, // RDA; Call cost
    SS_CALL_COSTS, // RDA; Call costs
    SS_CALL_COSTS_RESET, // RDA; Call costs reset
    SS_CALL_COST_NOT_PROVIDED, // RDA; Call cost not provided
    SS_CALL_DISCONNECTED_DUE_TO_SWITCH_OFF, // RDA; Call disconnected due to switch off
    SS_CALL_DIVERTING, // RDA; Call diverting
    SS_CALL_DIVERTING_ACTIVE_C_UNREACHABLE, // RDA; Divert calls: Unreachable
    SS_CALL_DIVERTING_ACTIVE_C_WHEN_BUSY, // RDA; Divert calls: When busy
    SS_CALL_DIVERTING_C_VOICE_CALL_INACTIVE_ALWAYS, // RDA; Call diverting: Voice call, Inactive, Always
    SS_CALL_DURATION, // RDA; Call duration
    SS_CALL_ENDED, // RDA; Call ended
    SS_CALL_ENDED_ABB, // RDA; Call ended
    SS_CALL_END_TONE, // RDA; Call end tone
    SS_CALL_FAILED, // RDA; Call failed
    SS_CALL_FORWARDED, // RDA; Call forwarded
    SS_CALL_FORWARDING, // RDA; Call forwarding
    SS_CALL_FORWARDING_ALWAYS_IS_ENABLED, // RDA; Call forwarding always is enabled
    SS_CALL_FORWARDING_ON_NO_REPLY, // RDA; Call forwarding on no reply
    SS_CALL_HISTORY, // RDA; Call history
    SS_CALL_LIMIT_EXCEEDED, // RDA; Call limit exceeded
    SS_CALL_LOG, // RDA; Call log
    SS_CALL_MANAGER, // RDA; Call manager
    SS_CALL_MANAGER_LA, // RDA; Call manager
    SS_CALL_MISSED, // RDA; Call missed
    SS_CALL_MODIFIED, // RDA; Call modified
    SS_CALL_MOPT, // RDA; Call
    SS_CALL_NETWORK_FAILURE, // RDA; Network failure
    SS_CALL_NOT_ALLOWED, // RDA; Call not allowed
    SS_CALL_PLAN, // RDA; Call plan
    SS_CALL_RESUMED_BY_NETWORK, // RDA; Call resumed by network
    SS_CALL_RINGTONE, // RDA; Call ringtone
    SS_CALL_SENDER, // RDA; Call Sender
    SS_CALL_SETTINGS, // RDA; Call settings
    SS_CALL_SOS_Q, // RDA; Call SOS?
    SS_CALL_STATUS_TONES, // RDA; Call status tones
    SS_CALL_SUSPENDED_BY_NETWORK, // RDA; Call suspended by network
    SS_CALL_TIME, // RDA; Call time
    SS_CALL_TIME_CAN_BE_LONGER_THAN_SET_CALL_TIME_ACCORDING_TO_THE_NETWORK_STATUS, // RDA; Call time can be longer than the set call time according to the network status.
    SS_CALL_TIME_LIMIT, // RDA; Call time limit
    SS_CALL_TIME_LIMIT_HAS_BEEN_SET_INCOMING_AND_OUTGOING_CALLS_NOT_AVAILABLE_DURING_CALL, // RDA; Call time limit has been set. Incoming and outgoing calls not available during call
    SS_CALL_TIME_LIMIT_RELEASED, // RDA; Call time limit released
    SS_CALL_TIME_LIMIT_SET_INCOMING_AND_OUTGOING_CALLS_UNAVAILABLE_DURING_CALL, // RDA; Call time limit set. Incoming and outgoing calls unavailable during call.
    SS_CALL_TRANSFERRED, // RDA; Calls transferred
    SS_CALL_TYPE, // RDA; Call type
    SS_CALL_T_APPLICATIONSETTINGS, // RDA; Call
    SS_CALL_UNAVAILABLE_CHECK_CALL_SETTINGS_AS_SEND_NUMBER, // RDA; Call not available. Check call settings as Send number
    SS_CALL_WAITING, // RDA; Call waiting
    SS_CALL_WAITING_ING, // RDA; Call waiting...
    SS_CALL_YOU_BACK_LATER, // RDA; Call you back later
    SS_CAMCORDER, // RDA; Camcorder
    SS_CAMCORDER_SETTINGS, // RDA; Camcorder Settings
    SS_CAMCORDER_SHORTCUTS, // RDA; Camcorder shortcuts
    SS_CAMERA, // RDA; Camera
    SS_CAMERA_IMAGE, // RDA; Camera image
    SS_CANBERRA, // RDA; Canberra
    SS_CANCEL, // RDA; Cancel
    SS_CANCELLED, // RDA; Cancelled
    SS_CANCELLED_MPRINT, // RDA; Cancelled
    SS_CANCELLING_ING, // RDA; Cancelling…
    SS_CANCEL_ALL, // RDA; Cancel all
    SS_CANCEL_DOWNLOADING, // RDA; Cancel downloading
    SS_CANCEL_DOWNLOADING_Q, // RDA; Cancel downloading?
    SS_CANCEL_INSTALLATION_Q, // RDA; Cancel installation?
    SS_CANCEL_RETRIEVING, // RDA; Cancel retrieving
    SS_CANCEL_RETRIEVING_MMS_Q, // RDA; Cancel retrieving multimedia message?
    SS_CANCEL_SK1, // RDA; Cancel
    SS_CANCEL_SK2, // RDA; Cancel
    SS_CANCEL_SK_LITE, // RDA; Cancel
    SS_CANNOT_FIND_THE_STRING, // RDA; Unable to find the string
    SS_CANT_PLAY_CONTENT, // RDA; Content not supported
    SS_CANT_TRANSFER, // RDA; Can't transfer
    SS_CAN_ONLY_DISPLAY_FROM_PS_TO_PS, // RDA; Can only display from %s to %s
    SS_CAP, // RDA; Cap
    SS_CAPE_TOWN, // RDA; Cape Town
    SS_CAPTURE_ERROR, // RDA; Capture error
    SS_CARACAS, // RDA; Caracas
    SS_CARD, // RDA; Card
    SS_CARD_MEMORY_FULL, // RDA; Memory card full
    SS_CARD_MEMORY_FULL_DELETE_SOME_ITEMS, // RDA; Memory card full. Delete some items
    SS_CARD_MEMORY_FULL_FORMATING_WILL_EARASE_ALL_DATA_CONTINUE_Q, // RDA; Memory card full. Formatting will erase all data. Continue?
    SS_CARD_MEMORY_FULL_SAVE_TO_PHONE_Q, // RDA; Memory card full. Save to phone?
    SS_CARD_NAME, // RDA; Card name
    SS_CARKIT, // RDA; Car kit
    SS_CARRIER, // RDA; Carrier
    SS_CARRIER_MATCHING, // RDA; Carrier Matching
    SS_CARTHAGE, // RDA; Carthage
    SS_CARTOON, // RDA; Cartoon
    SS_CASABLANCA, // RDA; Casablanca
    SS_CAT, // RDA; Cat
    SS_CAUSE, // RDA; Cause
    SS_CA_CERTIFICATES, // RDA; CA certificates
    SS_CC, // RDA; Cc
    SS_CCBS, // RDA; CCBS
    SS_CCBS_ACTIVATED, // RDA; CCBS activated
    SS_CCBS_FAILED, // RDA; CCBS failed
    SS_CC_CALL_REJECTED, // RDA; Call rejected
    SS_CELESTIAL, // RDA; Celestial
    SS_CENTRAL_CANADA, // RDA; Central Canada
    SS_CENTRAL_USA, // RDA; Central U.S.A
    SS_CENTRAL_WEST_MEXICO, // RDA; Central/Western Mexico
    SS_CENTRE, // RDA; Centre
    SS_CENTRE_PART, // RDA; Centre part
    SS_CERTIFICATES, // RDA; Certificates
    SS_CERTIFICATE_EXPIRED, // RDA; Certificate expired
    SS_CERTIFICATE_OF_ISSUER_NOT_FOUND, // RDA; Certificate of issuer is not found
    SS_CERTIFICATE_REVOKED, // RDA; Certificate revoked
    SS_CHANDIGARH, // RDA; Chandigarh
    SS_CHANGE, // RDA; Change
    SS_CHANGED, // RDA; Changed
    SS_CHANGED_TO_SMS, // RDA; Changed to SMS
    SS_CHANGE_ACCOUNT, // RDA; Change account
    SS_CHANGE_BARRING_PASSWORD, // RDA; Change barring password
    SS_CHANGE_CURRENT_PROFILE_Q, // RDA; Change current profile?
    SS_CHANGE_GROUP, // RDA; Change group
    SS_CHANGE_HOME_ZONE_TIME_AS_SELECTED_CLOCK_Q, // RDA; Change home zone time as selected clock?
    SS_CHANGE_ICON, // RDA; Change icon
    SS_CHANGE_IMAGE, // RDA; Change image
    SS_CHANGE_MSGTYPE_TO_MMS_Q, // RDA; Change message type to multimedia message?
    SS_CHANGE_MSGTYPE_TO_TEXTMSG_Q, // RDA; Change message type to text message?
    SS_CHANGE_MUSIC, // RDA; Change music
    SS_CHANGE_NAME, // RDA; Change name
    SS_CHANGE_ORDER, // RDA; Change order
    SS_CHANGE_PASSWORD, // RDA; Change password
    SS_CHANGE_PICTURE, // RDA; Change picture
    SS_CHANGE_PIN, // RDA; Change PIN
    SS_CHANGE_PIN1_CODE, // RDA; Change PIN
    SS_CHANGE_PIN2, // RDA; Change PIN2
    SS_CHANGE_Q, // RDA; Change?
    SS_CHANGE_RECIPIENT, // RDA; Change recipient
    SS_CHANGE_SOUND, // RDA; Change sound
    SS_CHANGE_THE_TIME_ZONE, // RDA; Change time zone
    SS_CHANGE_THE_TIME_ZONE_UPDATING_MODE, // RDA; Change time zone update mode
    SS_CHANGE_TO_MULTIMEDIA_MESSAGE, // RDA; Change to multimedia message
    SS_CHANGE_TO_TEXT_MESSAGE, // RDA; Change to text message
    SS_CHANGE_TYPE, // RDA; Change type
    SS_CHANGE_VIDEO, // RDA; Change video
    SS_CHANNEL, // RDA; Channel
    SS_CHANNELS, // RDA; Channels
    SS_CHANNEL_ADDED, // RDA; Channel added
    SS_CHANNEL_CHANGED, // RDA; Channel changed
    SS_CHANNEL_DELETED, // RDA; Channel deleted
    SS_CHANNEL_NAME, // RDA; Channel name
    SS_CHAP, // RDA; CHAP
    SS_CHARACTER_SUPPORT, // RDA; Character support
    SS_CHARGER_UNPLUGGED, // RDA; Charger unplugged
    SS_CHARGING_ING, // RDA; Charging...
    SS_CHARGING_PAUSED_FOR_BATTERY_DURABILITY, // RDA; Charging paused for battery durability
    SS_CHARIKAR, // RDA; Charikar
    SS_CHAR_XXX_NOT_ALLOWED, // RDA; */>|<?\\:\\xEE\xA0\x98 not allowed
    SS_CHAT, // RDA; Chat
    SS_CHAT_FULL, // RDA; Chat list full
    SS_CHAT_MOVISTAR, // RDA; Chat movistar
    SS_CHECKING_ING, // RDA; Checking...
    SS_CHECKING_STATUS_ING, // RDA; Checking status...
    SS_CHECK_INTERVAL, // RDA; Check interval
    SS_CHECK_NEW_EMAIL, // RDA; Check new email
    SS_CHECK_NUMBER, // RDA; Check number
    SS_CHECK_STATUS, // RDA; Check status
    SS_CHECK_STATUS_SCREEN_AS_NETWORK_ERROR_MAY_OCCUR, // RDA; Check status screen as network error may occur.
    SS_CHECK_THE_PRINTER_STATUS, // RDA; Check the printer status
    SS_CHEFCHAOUEN, // RDA; Chefchaouen
    SS_CHENNAI, // RDA; Chennai
    SS_CHF, // RDA; CHF
    SS_CHICAGO, // RDA; Chicago
    SS_CHILDRENS_DAY_SWA, // RDA; Children's day
    SS_CHILLED_OUT, // RDA; Chilled out
    SS_CHIMES_HIGH, // RDA; Chimes high
    SS_CHIMES_LOW, // RDA; Chimes low
    SS_CHITA, // RDA; Chita
    SS_CHLEF, // RDA; Chlef
    SS_CHOOSE_OPERATOR_HBROWSER, // RDA; Choose operator (Browser)
    SS_CHOOSE_OPERATOR_HEMAIL, // RDA; Choose operator (email)
    SS_CHOOSE_OPERATOR_HMMS, // RDA; Choose operator (MMS)
    SS_CHORD_HIGH, // RDA; Chord high
    SS_CHORD_LOW, // RDA; Chord low
    SS_CHOSEN_NEVER_ALLOW_IN_MYPHONEBOOK_IF_WANT_TO_SYNCHRONISE_CHANGE_ROAMING_SETTINGS_TMO, // RDA; You have chosen “Never allow” in the MyPhonebook settings menu. If you want to synchronise, you have to change your roaming settings accordingly
    SS_CHRISTIAN_PRAYERS, // RDA; Christian prayers
    SS_CHRISTMAS, // RDA; Christmas
    SS_CHRISTMAS_1, // RDA; Christmas1
    SS_CHRISTMAS_2, // RDA; Christmas 2
    SS_CHRISTMAS_RED, // RDA; Christmas red
    SS_CIF_H352_X_288, // RDA; CIF (352 x 288)
    SS_CITY, // RDA; City
    SS_CLAPS, // RDA; Claps
    SS_CLASS, // RDA; Class
    SS_CLASSIC, // RDA; Classic
    SS_CLDC_OR_MIDP_INCOMPATIBLE, // RDA; CLDC or MIDP incompatible
    SS_CLEAR, // RDA; Clear
    SS_CLEARED, // RDA; Cleared
    SS_CLEARING_FAILED, // RDA; Clearing failed
    SS_CLEARING_ING, // RDA; Clearing...
    SS_CLEARING_WILL_ERASE_ALL_DATA_CONTINUE_Q, // RDA; Clearing will erase all data. Continue?
    SS_CLEAR_CACHE, // RDA; Clear cache
    SS_CLEAR_CACHE_Q, // RDA; Clear cache?
    SS_CLEAR_PHONE_MEMORY, // RDA; Clear phone memory
    SS_CLEAR_SHORT, // RDA; Clear
    SS_CLEAR_SK_ABB, // RDA; Clear
    SS_CLEAR_SUBJECT, // RDA; Clear subject
    SS_CLIPBRD_COPY, // RDA; Copy
    SS_CLIPBRD_CUT, // RDA; Cut
    SS_CLIPBRD_MARK, // RDA; Mark
    SS_CLIPBRD_PASTE, // RDA; Paste
    SS_CLIPBRD_UNMARK, // RDA;    Unmark
    SS_CLIP_ART, // RDA; Clip art
    SS_CLIR_SUPPRESS_REJECTED, // RDA; CLIR suppress rejected
    SS_CLOCK, // RDA; Clock
    SS_CLOCK_AND_CALENDAR, // RDA; Clock and calendar
    SS_CLOSE_PARENTHESES, // RDA; Close parenthesis
    SS_CLOSE_SEARCH, // RDA; Close search
    SS_CLOSE_SK, // RDA; Close
    SS_CLOUD, // RDA; Cloud
    SS_CLOUDY, // RDA; Cloudy
    SS_CLOVER, // RDA; Clover
    SS_CLP, // RDA; CLP
    SS_CM, // RDA; cm
    SS_CM2, // RDA; cm²
    SS_CM3, // RDA; cm³
    SS_CNY, // RDA; CNY
    SS_CN_C_NONE, // RDA; CN:None
    SS_CODE, // RDA; Code
    SS_CODE_ALREADY_EXISTS, // RDA; Code already exists
    SS_CODE_PAIRING, // RDA; Code pairing
    SS_COFFEE, // RDA; Coffee
    SS_COLOMBO, // RDA; Colombo
    SS_COLOUR, // RDA; Colour
    SS_COLOUR_BOOST, // RDA; Colour boost
    SS_COLOUR_MOOD, // RDA; Colour mood
    SS_COLOUR_STYLE, // RDA; Colour style
    SS_COMMINUCATION_IN_PROGRESS, // RDA; Communication in progress
    SS_COMMON_ALL, // RDA; All
    SS_COMMUNICATIONS, // RDA; Communications
    SS_COMM_SAVE_CONTACT, // RDA; Save contacts
    SS_COMPANY, // RDA; Company
    SS_COMPLETE, // RDA; Complete
    SS_COMPLETED, // RDA; Completed
    SS_COMPOSERS, // RDA; Composers
    SS_CONDITIONAL, // RDA; Conditional
    SS_CONDITIONAL_CALL_FORWARDING_ACTIVE, // RDA; Conditional call forwarding active
    SS_CONDITIONAL_FORWARDING, // RDA; Conditional forwarding
    SS_CONDITIONAL_FORWARDING_ACTIVATED, // RDA; Conditional forwarding activated
    SS_CONFERENCE, // RDA; Conference
    SS_CONFERENCE_CALL, // RDA; Conference call
    SS_CONFIGURATION, // RDA; Configuration
    SS_CONFIGURATION_ERROR, // RDA; Configuration error
    SS_CONFIGURATION_MESSAGE, // RDA; Configuration message
    SS_CONFIRM, // RDA; Confirm
    SS_CONFIRMATION, // RDA; Confirmation
    SS_CONFIRM_CALL, // RDA; Making call?
    SS_CONFIRM_CALL_Q, // RDA; Confirm call?
    SS_CONFIRM_NEW_PASSWORD, // RDA; Confirm new password
    SS_CONFIRM_NEW_PHONE_PASSWORD, // RDA; Confirm new phone password
    SS_CONFIRM_NEW_PIN, // RDA; Confirm new PIN
    SS_CONFIRM_NEW_PIN2, // RDA; Confirm new PIN2
    SS_CONFIRM_PASSWORD, // RDA; Confirm password
    SS_CONFLICT, // RDA; Conflict
    SS_CONGRATULATIONS_E, // RDA; Congratulations!
    SS_CONNECT, // RDA; Connect
    SS_CONNECTED_MPRINTER, // RDA; Connected
    SS_CONNECTED_POP, // RDA; Connected
    SS_CONNECTED_WITH_PS, // RDA; Connected with %s
    SS_CONNECTING_FAILED, // RDA; Connecting failed
    SS_CONNECTING_ING, // RDA; Connecting...
    SS_CONNECTING_TO_PS_ING, // RDA; Connecting to %s...
    SS_CONNECTION, // RDA; Connection
    SS_CONNECTIONS, // RDA; Connections
    SS_CONNECTION_LOG, // RDA; Connection log
    SS_CONNECTION_MODE_WILL_BE_CHANGED_TO_AUTOMATIC_CONNECT_THIS_NETWORK_Q, // RDA; Connection mode will be changed to Automatic. Connect network?
    SS_CONNECTION_MODE_WILL_BE_CHANGED_TO_DISABLE_DISCONNECT_THIS_NETWORK_Q, // RDA; Connection mode will be changed to Disable. Disconnect network?
    SS_CONNECTION_TONE, // RDA; Connection tone
    SS_CONNECTION_TYPE, // RDA; Connection type
    SS_CONNECTIVITY, // RDA; Connectivity
    SS_CONNECT_Q, // RDA; Connect?
    SS_CONNECT_TO_INTERNET_Q, // RDA; Connect to Internet?
    SS_CONNECT_TO_VOICE_MAIL, // RDA; Connect to voicemail
    SS_CONNECT_TO_VOICE_SERVER, // RDA; Connect to voice server
    SS_CONNECT_URL, // RDA; Connect to URL?
    SS_CONNECT_WITH_UNKNOWN_DEVICE_Q, // RDA; Connect with unknown device?
    SS_CONSTANTINE, // RDA; Constantine
    SS_CONSTRAIN_PROPORTIONS, // RDA; Constrain proportions
    SS_CONTACT, // RDA; Contact
    SS_CONTACTS, // RDA; Contacts
    SS_CONTACTS_FROM_PREVIOUS_USER_PHONE_DELETE_THEM_Q_TMO, // RDA; There are contacts from the previous user of this phone. Would you like to delete them?
    SS_CONTACTS_ON_YOUR_PHONE_IN_THE_SYNCHRONISATION_THAT_FOLLOWS_IF_CONTACTS_FOUND_ONLINE_FOR_YOU_WOULD_YOU_LIKE_TO_TMO, // RDA; You have contacts on your phone. In the synchronisation that follows, if contacts are found online for you would you like to...
    SS_CONTACTS_ON_YOUR_SIM_CARD_IMPORT_THEM_INTO_MYPHONEBOOK_Q_TMO, // RDA; There are contacts on your SIM card. Would you like to import them into MyPhonebook?
    SS_CONTACTS_SETTINGS, // RDA; Contacts settings
    SS_CONTACTS_SOFTKEY, // RDA; Contacts
    SS_CONTACTS_SYNC_IN_PROGRESS, // RDA; Contacts sync in progress
    SS_CONTACTS_SYNC_STARTED, // RDA; Contacts sync started
    SS_CONTACTS_WERE_COPIED, // RDA; Contacts copied
    SS_CONTACTS_WERE_MOVED, // RDA; Contacts moved
    SS_CONTACT_CHANGED_TO_SBPS, // RDA; Contact has been changed to [%s] [%s]
    SS_CONTACT_COPIED, // RDA; contact copied
    SS_CONTACT_LIST, // RDA; Contact list
    SS_CONTACT_SETTINGS, // RDA; Contact settings
    SS_CONTAINS, // RDA; Contains
    SS_CONTENTS, // RDA; Contents
    SS_CONTENTS_EXPLORER, // RDA; Contents Explorer
    SS_CONTENT_MISMATCH, // RDA; Content mismatch
    SS_CONTENT_NOT_ACCEPTED, // RDA; Content not accepted
    SS_CONTINUE_CAMCORDER_Q, // RDA; Continue camcorder?
    SS_CONTINUE_CAMERA_Q, // RDA; Continue camera?
    SS_CONTINUE_OPERATION, // RDA; Continue operation
    SS_CONTINUE_PLAYING_FM_RADIO_Q, // RDA; Continue playing FM Radio?
    SS_CONTINUE_PLAYING_Q, // RDA; Continue playing?
    SS_CONTINUE_PRINTING_NEXT_PAGE_Q, // RDA; Continue printing next page?
    SS_CONTINUE_Q, // RDA; Continue?
    SS_CONTINUE_SK_ABB, // RDA; Continue
    SS_CONTINUE_SLIDE_SHOW_Q, // RDA; Continue slide show?
    SS_CONTINUOUS, // RDA; Continuous
    SS_CONTRAST, // RDA; Contrast
    SS_CONTRAST_ABB, // RDA; Contrast
    SS_CONTROLLER, // RDA; Controller
    SS_CONVENTIONAL_VOICEMAIL_TMO, // RDA; Conventional voicemail
    SS_CONVERTING_TO_MULTIMEDIA_MESSAGE_ING, // RDA; Converting to multimedia message…
    SS_CONVERTING_TO_SMS_MESSAGE_ING, // RDA; Converting to SMS message…
    SS_CONVERTING_TO_TEXT_MESSAGE_ING, // RDA; Converting to text message…
    SS_CONVERTOR, // RDA; Convertor
    SS_CONVERT_TO, // RDA; Convert to
    SS_COOKIE_OPTIONS, // RDA; Cookie options
    SS_COP, // RDA; COP
    SS_COPIED, // RDA; Copied
    SS_COPIED_TO_PS, // RDA; Copied to %s
    SS_COPY, // RDA; Copy
    SS_COPYING_ING, // RDA; Copying...
    SS_COPY_ALL, // RDA; Copy all
    SS_COPY_COMPLETED, // RDA; Copy completed
    SS_COPY_FROM, // RDA; Copy from
    SS_COPY_HERE, // RDA; Copy here
    SS_COPY_OR_CUT, // RDA; Copy or cut
    SS_COPY_TO, // RDA; Copy to
    SS_COPY_TO_CALCULATOR, // RDA; Copy to calculator
    SS_COPY_TO_CARD, // RDA; Copy to card
    SS_COPY_TO_DIALLING_SCREEN, // RDA; Copy to dialling screen
    SS_COPY_TO_DIALOGUE_SCREEN, // RDA; Copy to dialogue screen
    SS_COPY_TO_FDN_CONTACTS, // RDA; Copy to FDN contacts
    SS_COPY_TO_MEMORY_CARD, // RDA; Copy to memory card
    SS_COPY_TO_PHONE, // RDA; Copy to phone
    SS_COPY_TO_PHONE_AND_SET_AS_ALARMTONE_Q, // RDA; Copy to phone and set as alarm tone?
    SS_COPY_TO_PHONE_AND_SET_AS_CALLER_ID_Q, // RDA; Copy to phone and set as caller ID?
    SS_COPY_TO_PHONE_AND_SET_AS_CALLER_RINGTONE_Q, // RDA; Copy to phone and set as caller ringtone?
    SS_COPY_TO_PHONE_AND_SET_AS_RINGTONE_Q, // RDA; Copy to phone and set as ringtone?
    SS_COPY_TO_PHONE_AND_SET_AS_WALLPAPER_Q, // RDA; Copy to phone and set as wallpaper?
    SS_COPY_TO_PHONE_Q, // RDA; Copy to phone?
    SS_COPY_TO_SIM, // RDA; Copy to SIM
    SS_COPY_URL_TO, // RDA; Copy URL to
    SS_CORDLESS_TERMINAL, // RDA; Cordless terminal
    SS_CORNFLOWER_BLUE, // RDA; Cornflower blue
    SS_CORRECT, // RDA; Correct
    SS_CORREO_MOVIL, // RDA; Correo móvil
    SS_COST_LIMIT, // RDA; Cost limit
    SS_COULD_NOT_FIND_A_MATCH_FOR_THE_SONG, // RDA; Could not find a match for the song
    SS_COUNT, // RDA; Count
    SS_COUNTER, // RDA; Counter
    SS_COUNTER_OFF, // RDA; Counter off
    SS_COUNTER_OFF_Q, // RDA; Counter off?
    SS_COUNTER_ON, // RDA; Counter on
    SS_COUNTING_ING, // RDA; Counting...
    SS_COUNTRY, // RDA; Country
    SS_COUNTY, // RDA; County
    SS_COUNT_IN_PERIOD, // RDA; Count in period
    SS_COVER_DISPLAY, // RDA; Cover display
    SS_COVER_TITLE, // RDA; Cover title
    SS_CP_LOCK, // RDA; CP lock
    SS_CP_LOCK_PASSED, // RDA; CP lock passed
    SS_CRANBERRY, // RDA; Cranberry
    SS_CREATE, // RDA; Create
    SS_CREATE_ALARM, // RDA; Create alarm
    SS_CREATE_CONTACT, // RDA; Create contact
    SS_CREATE_EMAIL, // RDA; Create email
    SS_CREATE_FDN_CONTACT, // RDA; Create FDN contact
    SS_CREATE_FDN_CONTACTS, // RDA; Create FDN contacts
    SS_CREATE_FOLDER, // RDA; Create folder
    SS_CREATE_GROUP, // RDA; Create group
    SS_CREATE_ITEM, // RDA; Create item
    SS_CREATE_MEMO, // RDA; Create memo
    SS_CREATE_MESSAGE, // RDA; Create message
    SS_CREATE_MESSAGE_ABB, // RDA; Create message
    SS_CREATE_MMS, // RDA; Create MMS
    SS_CREATE_MORG, // RDA; Create
    SS_CREATE_MPHOTO, // RDA; Create
    SS_CREATE_MY_NAMECARD_FIRST, // RDA; Create my namecard first
    SS_CREATE_NEW, // RDA; Create
    SS_CREATE_PHONEBOOK, // RDA; Create phonebook
    SS_CREATE_PLAYLIST, // RDA; Create playlist
    SS_CREATE_REJECT_NUMBER, // RDA; Create reject number
    SS_CREATE_REMINDER, // RDA; Create reminder
    SS_CREATE_SMS, // RDA; Create SMS
    SS_CREATE_SYNC_PROFILE, // RDA; Create sync profile
    SS_CREATION_MODE, // RDA; Creation mode
    SS_CREATION_TIME, // RDA; Creation time
    SS_CREW_CUT, // RDA; Crew cut
    SS_CROP, // RDA; Crop
    SS_CSD_FAIL, // RDA; CSD fail
    SS_CUIABA, // RDA; Cuiaba
    SS_CURRENCY, // RDA; Currency
    SS_CURRENT, // RDA; Current
    SS_CURRENT_ALBUM, // RDA; Current album
    SS_CURRENT_MUSIC_FILE_REPLAY_TRANSFER_TO_BT_DUAL_STEREO_HEADSET_Q, // RDA; Current music file will be replayed. Transfer to Bluetooth dual stereo headset?
    SS_CURRENT_MUSIC_FILE_REPLAY_TRANSFER_TO_PHONE_Q, // RDA; Current music file will be replayed. Transfer to phone?
    SS_CURRENT_MUSIC_FILE_WILL_BE_REPLAYED_TRANSFER_TO_BLUETOOTH_SINGLE_STEREO_HEADSET_Q, // RDA; Current music file will be replayed. Transfer to Bluetooth single stereo headset?
    SS_CURRENT_MUSIC_FILE_WILL_BE_REPLAYED_TRANSFER_TO_BLUETOOTH_STEREO_HEADSET_Q, // RDA; Current music file will be replayed. Transfer to Bluetooth stereo headset?
    SS_CURRENT_NETWORK_CHANGED, // RDA; Current network changed
    SS_CURRENT_NETWORK_WILL_BE_DISCONNECTED_CONTINUE_Q, // RDA; Current network will be disconnected. Continue?
    SS_CURRENT_PASSWORD, // RDA; Current password
    SS_CURRENT_PROFILE, // RDA; Current profile
    SS_CURRENT_PROFILE_CHANGED, // RDA; Current profile changed
    SS_CUSTOM, // RDA; Custom
    SS_CUSTOMISE, // RDA; Customise
    SS_CUSTOM_ONLY, // RDA; Custom only
    SS_CUSTOM_TIME, // RDA; Custom time
    SS_CUT, // RDA; Cut
    SS_CUTE_FLIP, // RDA; Cute flip
    SS_CUT_ADDRESS, // RDA; Cut address
    SS_CUT_NUMBER, // RDA; Cut number
    SS_CVT_ARE, // RDA; are
    SS_CVT_FEET2, // RDA; feet²
    SS_CVT_FEET3, // RDA; feet³
    SS_CVT_POUND, // RDA; pound
    SS_CVT_YARD2, // RDA; yard²
    SS_CVT_YARD3, // RDA; yard³
    SS_C_MTEMPERATURE, // RDA; °C
    SS_DAFTA, // RDA; Dafta
    SS_DAHBAN, // RDA; Dahaban
    SS_DAHUK, // RDA; Dohuk
    SS_DAILY, // RDA; Daily
    SS_DAILY_REPEAT, // RDA; Daily repeat
    SS_DALLAS, // RDA; Dallas
    SS_DAMANHUR, // RDA; Damanhur
    SS_DAMASCUS, // RDA; Damascus
    SS_DAMIETTA, // RDA; Damietta
    SS_DAMMAM, // RDA; Dammam
    SS_DANCE, // RDA; Dance
    SS_DANDARAH, // RDA; Dendera
    SS_DANDELION, // RDA; Dandelion
    SS_DARAA, // RDA; Dera'a
    SS_DARK_LAVENDER_BLUE, // RDA; Dark lavender blue
    SS_DARK_TARQUOISE, // RDA; Dark turquoise
    SS_DARNAH, // RDA; Darnah
    SS_DARWIN, // RDA; Darwin
    SS_DAR_OULD_ZIDOUH, // RDA; Dar Ould Zidouh
    SS_DATA, // RDA; Data
    SS_DATA_CALL, // RDA; Data call
    SS_DATA_CALLS, // RDA; Data calls
    SS_DATA_CALL_TYPE, // RDA; Data call type
    SS_DATA_CONNECTED, // RDA; Data connected
    SS_DATA_COUNTER, // RDA; Data counter
    SS_DATA_SENT, // RDA; Data sent
    SS_DATE, // RDA; Date
    SS_DATE_C, // RDA; Date:
    SS_DATE_FORMAT, // RDA; Date format
    SS_DATE_OF_BIRTH, // RDA; Date of birth
    SS_DATE_PRINT, // RDA; Date print
    SS_DATE_SYSTEM, // RDA; Date system
    SS_DAWADMI, // RDA; Dawadmi
    SS_DAY, // RDA; Day
    SS_DAYLIGHT, // RDA; Daylight
    SS_DAYLIGHT_SAVING_OFF, // RDA; Daylight saving off
    SS_DAYLIGHT_SAVING_ON, // RDA; Daylight saving on
    SS_DAYS_LC, // RDA; days
    SS_DAY_LC, // RDA; day
    SS_DAY_MEA, // RDA; Day
    SS_DAY_OFF, // RDA; Day off
    SS_DB_ERROR, // RDA; DB error
    SS_DDMMYYYY_DOT, // RDA; DD.MM.YYYY
    SS_DD_MM_YYYY, // RDA; DD/MM/YYYY
    SS_DEACTIVATE, // RDA; Deactivate
    SS_DEACTIVATED, // RDA; Deactivated
    SS_DEACTIVATED_M_OFF, // RDA; Deactivated
    SS_DEACTIVATE_ALARM, // RDA; Deactivate alarm
    SS_DEACTIVATE_FLIGHT_MODE_TO_USE_FM_RADIO, // RDA; Deactivate Flight mode to use FM radio
    SS_DEACTIVATE_FLIGHT_MODE_TO_USE_NETWORK_SERVICES, // RDA; Deactivate Flight mode to use network services
    SS_DEACTIVATE_OFFLINE_PROFILE_N_SELECT_ANOTHER, // RDA; Deactivate Offline profile and select another one
    SS_DEACTIVATE_OFFLINE_PROFILE_TO_MAKE_CALLS, // RDA; Deactivate Offline profile to make calls
    SS_DEACTIVATE_OFFLINE_PROFILE_TO_SEND_MESSAGES, // RDA; Deactivate Offline profile to send messages
    SS_DEACTIVATE_OFFLINE_PROFILE_TO_USE_NETWORK_SERVICES, // RDA; Deactivate Offline profile to use network services
    SS_DEACTIVATE_OFFLINE_TO_MAKE_CALLS, // RDA; Deactivate Offline profile to make calls or send messages.
    SS_DEACTIVATE_SHORTCUTS_TOOLBAR, // RDA; Deactivate Shortcuts toolbar
    SS_DEACTIVATE_SILENT_PROFILE_AND_ACTIVATE_NORMAL_PROFILE_Q, // RDA; Deactivate Silent profile and activate Normal profile?
    SS_DEACTIVATE_SILENT_PROFILE_AND_SELECT_ANOTHER_ONE, // RDA; Deactivate Silent profile and select another one
    SS_DEACTIVATING_ING, // RDA; Deactivating...
    SS_DEC, // RDA; Dec.
    SS_DECEMBER, // RDA; December
    SS_DECIDE_LATER_TO_USE_MYPHONEBOOK_SERVICE_OPEN_CONFIGURATION_SMS_AND_SELECT_SAVE_CONTACT_OUR_CUSTOMER_SERVICE_TMO, // RDA; If you decide later to use the MyPhonebook service, open the configuration SMS and select ‘save’. Alternatively you can contact our customer service
    SS_DEFAULT, // RDA; Default
    SS_DEFAULT_ALARM, // RDA; Default alarm
    SS_DEFAULT_GREETING_1, // RDA; Default greeting 1
    SS_DEFAULT_GREETING_2, // RDA; Default greeting 2
    SS_DEFAULT_IMAGE, // RDA; Default image
    SS_DEFAULT_IMAGES, // RDA; Default images
    SS_DEFAULT_MCALLIDSET, // RDA; Default
    SS_DEFAULT_MELODIES, // RDA; Default melodies
    SS_DEFAULT_MEMORY, // RDA; Default memory
    SS_DEFAULT_NAME, // RDA; Default name
    SS_DEFAULT_NUMBERS, // RDA; Default numbers
    SS_DEFAULT_RINGTONES, // RDA; Default ringtones
    SS_DEFAULT_SIM, // RDA; Default SIM
    SS_DEFAULT_SIM_CHANGED_PHONE_RESTART, // RDA; Default SIM changed. The phone will restart
    SS_DEFAULT_STORAGE, // RDA; Default storage
    SS_DEFAULT_STORAGE_LOCATION_CHANGED_TO_MEMORY_CARD, // RDA; Default storage location changed to memory card
    SS_DEFAULT_STORAGE_LOCATION_CHANGED_TO_PHONE, // RDA; Default storage location changed to phone
    SS_DEFAULT_STORAGE_LOCATION_SWITCHED_TO_MEMORY_CARD, // RDA; The default storage location is switched to memory card
    SS_DEFAULT_STORAGE_LOCATION_SWITCHED_TO_PHONE, // RDA; The default storage location is switched to phone
    SS_DEFAULT_STYLE, // RDA; Default style
    SS_DEFAULT_VIEW_BY, // RDA; Default view by
    SS_DEFAULT_VIEW_TYPE, // RDA; Default view type
    SS_DEFAULT_WALLPAPER, // RDA; Default wallpaper
    SS_DEFAULT_WALLPAPERS, // RDA; Default wallpapers
    SS_DEFFERED, // RDA; Deferred
    SS_DEFLECTING_ING, // RDA; Deflecting...
    SS_DEF_NAME_PHOTO, // RDA; Photo
    SS_DEF_NAME_VIDEO, // RDA; Video
    SS_DEIR_EZ_ZOR, // RDA; Deirezzor
    SS_DELETE, // RDA; Delete
    SS_DELETED, // RDA; Deleted
    SS_DELETED_ALL, // RDA; Deleted all
    SS_DELETED_EXCEPT, // RDA; Deleted except
    SS_DELETE_ALL, // RDA; Delete all
    SS_DELETE_ALL_CHANNELS, // RDA; Delete all channels
    SS_DELETE_ALL_CONTACTS_IN_PHONE_Q, // RDA; Delete all contacts in phone?
    SS_DELETE_ALL_CONTACTS_IN_PS_Q, // RDA; Delete all contacts in %s?
    SS_DELETE_ALL_CONTACTS_IN_SIM_Q, // RDA; Delete all contacts in SIM?
    SS_DELETE_ALL_CONTACTS_Q, // RDA; Delete all contacts?
    SS_DELETE_ALL_DATES_BEFORE_PS_Q, // RDA; Delete all dates before %s?
    SS_DELETE_ALL_EMAILS_IN_ACCOUNT_Q, // RDA; Delete all emails in account?
    SS_DELETE_ALL_INCLUDING_PROTECTED_MESSAGES_Q, // RDA; Delete all including protected messages?
    SS_DELETE_ALL_Q, // RDA; Delete all?
    SS_DELETE_BY_FOLDER, // RDA; Delete by folder
    SS_DELETE_CHANNEL, // RDA; Delete channel
    SS_DELETE_CONTACT, // RDA; Delete contact
    SS_DELETE_COOKIES, // RDA; Delete cookies
    SS_DELETE_CURRENT_PAGE_Q, // RDA; Delete current page?
    SS_DELETE_DETAIL, // RDA; Delete detail
    SS_DELETE_DETAILS, // RDA; Delete details
    SS_DELETE_EMAILS_IN_SEVER_Q, // RDA; Delete emails in sever?
    SS_DELETE_ERROR, // RDA; Delete error
    SS_DELETE_EVERYTHING_IN_FOLDER_CONTINUE_Q, // RDA; This will delete everything in the folder. Continue?
    SS_DELETE_FOLDER, // RDA; Delete folder
    SS_DELETE_HISTORY, // RDA; Delete history
    SS_DELETE_LIST, // RDA; Delete list
    SS_DELETE_MULTIPLE, // RDA; Delete multiple
    SS_DELETE_OPTION, // RDA; Delete option
    SS_DELETE_OPTION_WILL_DISCONNECT_CURRENT_NETWORK, // RDA; Delete option will disconnect current network
    SS_DELETE_PAGE, // RDA; Delete page
    SS_DELETE_PHONEBOOK, // RDA; Delete phonebook
    SS_DELETE_Q, // RDA; Delete?
    SS_DELETE_SK, // RDA; Delete
    SS_DELETE_SK_ABB, // RDA; Delete
    SS_DELETE_SOME_ITEMS, // RDA; Delete some items
    SS_DELETE_SOME_ITEMS_FROM_MEMORY_CARD, // RDA; Delete some items from memory card
    SS_DELETE_SOME_ITEMS_FROM_PHONE, // RDA; Delete some items from phone
    SS_DELETE_SUBJECT, // RDA; Delete subject
    SS_DELETE_VIDEO, // RDA; Delete video
    SS_DELETING, // RDA; Deleting
    SS_DELETING_FILE_ING, // RDA; Deleting file...
    SS_DELETING_ING, // RDA; Deleting...
    SS_DELETING_ITEM_REVERT_ALARM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_ALARM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_CALLER_ALARM_Q, // RDA; Deleting item will revert caller ringtone and alarm tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_CALLER_ID_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert caller ID to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_CALLER_MSGTONE_Q, // RDA; Deleting item will revert caller ringtone and message tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_CALLER_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert caller ringtone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_CALLER_RINGTONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert caller ringtone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_FM_ALERT_TONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert FM alert tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_MESSAGE_TONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert message tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONECALLER_RINGTONE_AND_ALARM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert ringtone, caller ringtone, and alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONECALLER_RINGTONE_AND_ALARM_TONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert ringtone,caller ringtone and alarm tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONE_AND_ALARM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert ringtone and alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONE_AND_ALARM_TONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert ringtone and alarm tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONE_AND_CALLER_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert ringtone and caller ringtone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONE_AND_CALLER_RINGTONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert ringtone and caller ringtone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will revert ringtone to default. Continue?
    SS_DELETING_ITEM_WILL_REVERT_RINGTONE_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert ringtone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_RING_CALLER_ALARM_MSGTONE_Q, // RDA; Deleting item will revert ringtone, caller ringtone and alarm tone and message tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_RING_CALLER_MSGTONE_Q, // RDA; Deleting item will revert ringtone, caller ringtone and message tone to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_WALLPAPER_AND_CALLER_ID_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert wallpaper and caller ID to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_WALLPAPER_TO_DEFAULT_DELETE_Q, // RDA; Deleting item will revert wallpaper to default. Delete?
    SS_DELETING_ITEM_WILL_REVERT_YOUR_PS_TO_DEFAULT_ONE_CONTINUE_Q, // RDA; Deleting the item will revert your %s to the default one. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_AND_REVERT_FM_ALERT_TONE_TO_DEFAULT, // RDA; Deleting item will stop playing and revert FM alert tone to default. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_AND_REVERT_RINGTONE_CALLER_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will stop playing and revert ringtone and caller ringtone to default. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_CONTINUE_Q, // RDA; Deleting item will stop playing. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_REVERT_ALAM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will stop playing and revert alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_REVERT_CALLER_RINGTONE_ALAM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will stop playing and revert caller ringtone and alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_REVERT_CALLER_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will stop playing and revert caller ringtone to default. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_REVERT_RINGTONE_ALAM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will stop playing and revert ringtone and alarm tone to default. Continue?
    SS_DELETING_ITEM_WILL_STOP_PLAYING_REVERT_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Deleting item will stop playing and revert ringtone to default. Continue?
    SS_DELETING_MESSAGE_ING, // RDA; Deleting message...
    SS_DELETING_OPTIONS, // RDA; Deleting options
    SS_DELETING_WILL_ERASE_FOLDER_DATA_CONTINUE_Q, // RDA; Deleting will erase folder and data. Continue?
    SS_DELIVERED, // RDA; Delivered
    SS_DELIVERY_REPORT, // RDA; Delivery report
    SS_DELIVERY_TIME, // RDA; Delivery time
    SS_DENVER, // RDA; Denver
    SS_DEREGISTERING_FROM_NETWORK_SERVER_ING, // RDA; Disconnecting from the server...
    SS_DESCRIPTION, // RDA; Description
    SS_DESELECTED, // RDA; Deselected
    SS_DESKTOP_VIEW, // RDA; Desktop view
    SS_DESKTOP_VIEW_MODE, // RDA; Desktop view mode
    SS_DESPECKLE, // RDA; Despeckle
    SS_DETAILS, // RDA; Details
    SS_DETAILS_MCALENDAR, // RDA; Details
    SS_DEVICE_ABORTED, // RDA; Device aborted
    SS_DEVICE_DETAILS, // RDA; device details
    SS_DEVICE_MAC, // RDA; Device MAC
    SS_DEVICE_MANAGEMENT_SESSION_CANCELLED, // RDA; Device management session cancelled
    SS_DEVICE_MANAGEMENT_SESSION_STARTED, // RDA; Device management session started
    SS_DEVIL, // RDA; Devil
    SS_DHADNA, // RDA; Dadna
    SS_DHAHRAN, // RDA; Dhahran
    SS_DHAKA, // RDA; Dhaka
    SS_DHAMAR, // RDA; Dhamar
    SS_DHUHR, // RDA; Dhuhr
    SS_DHURMA, // RDA; Dhurma
    SS_DIAL, // RDA; Dial
    SS_DIALING_DISPLAY, // RDA; Dialling display
    SS_DIALLED_CALLS, // RDA; Dialled calls
    SS_DIALLING_ING, // RDA; Dialling...
    SS_DIALUP, // RDA; Dial-up
    SS_DIALUP_NETWORKING, // RDA; Dial-up networking
    SS_DIAL_NUMBER, // RDA; Dial number
    SS_DIBBA, // RDA; Dibba
    SS_DICE, // RDA; Dice
    SS_DICTIONARY, // RDA; Dictionary
    SS_DIM, // RDA; Dim
    SS_DIMENSIONS, // RDA; Dimensions
    SS_DING, // RDA; Ding
    SS_DINNER, // RDA; Dinner
    SS_DIRAZ, // RDA; Diraz
    SS_DIRECTORY_ENQUIRIES, // RDA; Directory enquiries
    SS_DIRIYAH, // RDA; Diriyah
    SS_DISABLE, // RDA; Disable
    SS_DISABLED, // RDA; Disabled
    SS_DISABLE_CALL_TIME_LIMIT_TO_ACCEPT_NEW_CALL, // RDA; Disable call time limit to accept new call?
    SS_DISABLE_POWER_SAVING_MODE_Q, // RDA; Disable power saving mode?
    SS_DISCONNECT, // RDA; Disconnect
    SS_DISCONNECTED, // RDA; Disconnected
    SS_DISCONNECTED_BY_SERVER, // RDA; Disconnected by server
    SS_DISCONNECTED_MPRINTER, // RDA; Disconnected
    SS_DISCONNECTING_ING, // RDA; Disconnecting...
    SS_DISCONNECT_PS_Q, // RDA; Disconnect %s?
    SS_DISPLAY, // RDA; Display
    SS_DISPLAY_ADDRESS_FIELDS, // RDA; Display address fields
    SS_DISPLAY_AND_LIGHT, // RDA; Display and light
    SS_DISPLAY_IMAGES, // RDA; Display images
    SS_DISPLAY_MSCREEN, // RDA; Display
    SS_DISPLAY_MSHOW, // RDA; Display
    SS_DISPLAY_NAME, // RDA; Display name
    SS_DISPLAY_NUMBER, // RDA; Display number
    SS_DISPLAY_ON_IDLE, // RDA; Display on idle
    SS_DISPLAY_PEDOMETER_WHILE_IDLE, // RDA; Display pedometer while idle
    SS_DISPLAY_REMINDER_AT, // RDA; Display reminder at
    SS_DISPLAY_SETTINGS, // RDA; Display settings
    SS_DISUQ, // RDA; Desouk
    SS_DIVERTED_CALL, // RDA; Diverted call
    SS_DIVERT_ALWAYS, // RDA; Divert always
    SS_DIVERT_TO, // RDA; Divert to
    SS_DIWALI_SWA, // RDA; Diwali
    SS_DIYARBAKIR, // RDA; Diyarbakir
    SS_DIZZY, // RDA; Dizzy
    SS_DJELFA, // RDA; Djelfa
    SS_DKK, // RDA; DKK
    SS_DM_ACCOUNT, // RDA; DM account
    SS_DNS, // RDA; DNS
    SS_DNS1, // RDA; DNS1
    SS_DNS2, // RDA; DNS2
    SS_DNS_1, // RDA; DNS 1
    SS_DNS_2, // RDA; DNS 2
    SS_DNS_ADDRESS1, // RDA; DNS address 1
    SS_DNS_ADDRESS2, // RDA; DNS address 2
    SS_DNS_FAILED, // RDA; DNS failed
    SS_DNS_UNAVAILABLE, // RDA; DNS not available
    SS_DOG, // RDA; Dog
    SS_DOHA, // RDA; Doha
    SS_DOMAIN, // RDA; Domain
    SS_DONE, // RDA; Done
    SS_DONE_MTASKSTATUS, // RDA; Done
    SS_DOOR, // RDA; Door
    SS_DOUZ, // RDA; Douz
    SS_DOWN, // RDA; Down
    SS_DOWNLOADED, // RDA; Downloaded
    SS_DOWNLOADED_IMAGES, // RDA; Downloaded images
    SS_DOWNLOADED_JAR_INVALID, // RDA; Downloaded JAR are invalid
    SS_DOWNLOADED_SOUNDS, // RDA; Downloaded sounds
    SS_DOWNLOADED_VIDEOS, // RDA; Downloaded videos
    SS_DOWNLOADING_ING, // RDA; Downloading...
    SS_DOWNLOADS, // RDA; Downloads
    SS_DOWNLOAD_ANIMATIONS, // RDA; Download animations
    SS_DOWNLOAD_CANCELLED, // RDA; Download cancelled
    SS_DOWNLOAD_COMPLETED, // RDA; Download completed
    SS_DOWNLOAD_DRM_CONTENT_OR_LICENSE_Q, // RDA; Download DRM content or license?
    SS_DOWNLOAD_FAILED, // RDA; Download failed
    SS_DOWNLOAD_FILE, // RDA; Download file
    SS_DOWNLOAD_GAMES, // RDA; Download games
    SS_DOWNLOAD_IMAGES, // RDA; Download images
    SS_DOWNLOAD_LIMIT, // RDA; Download limit
    SS_DOWNLOAD_MEMAIL, // RDA; Download
    SS_DOWNLOAD_MINTERNET, // RDA; Download
    SS_DOWNLOAD_MORE_ITEMS_Q, // RDA; Download more items?
    SS_DOWNLOAD_MULTIPLE, // RDA; Download multiple
    SS_DOWNLOAD_MUSIC, // RDA; Download music
    SS_DOWNLOAD_Q, // RDA; Download?
    SS_DOWNLOAD_RINGBACK_TONES, // RDA; Download ringback tones
    SS_DOWNLOAD_SCREENSAVERS, // RDA; Download screensavers
    SS_DOWNLOAD_SKINS, // RDA; Download skins
    SS_DOWNLOAD_SOUNDS, // RDA; Download sounds
    SS_DOWNLOAD_STOPPED, // RDA; Download stopped
    SS_DOWNLOAD_TEMPLATES, // RDA; Download templates
    SS_DOWNLOAD_THEMES, // RDA; Download themes
    SS_DOWNLOAD_VIDEOS, // RDA; Download videos
    SS_DOWNWARD, // RDA; Downward
    SS_DO_NOT_REDIAL_AUTOMATICALLY_IF_CALL_UNABLE, // RDA; Do not redial automatically if call is unable to connect or is cut off.
    SS_DRAFT, // RDA; Draft
    SS_DRAFTS, // RDA; Drafts
    SS_DRAFT_MPRINTINGQUALITY, // RDA; Draft
    SS_DRIVING, // RDA; Driving
    SS_DRM, // RDA; DRM
    SS_DRM_FILE_CANNOT_SAVE, // RDA; Unable to save DRM file
    SS_DRM_FILE_CANNOT_SEND, // RDA; DRM file. Cannot send
    SS_DRM_MESSAGE_UNABLE_TO_FORWARD, // RDA; DRM message. Unable to forward
    SS_DRM_PROTECTED_CONTENT_CAN_NOT_BE_CROPPED, // RDA; DRM-protected content cannot be cropped
    SS_DRM_TOOLKIT, // RDA; DRM toolkit
    SS_DRUM, // RDA; Drum
    SS_DRUM_1, // RDA; Drum 1
    SS_DRUM_2, // RDA; Drum 2
    SS_DST_OFF, // RDA; DST off
    SS_DST_ON, // RDA; DST on
    SS_DTMF_SENT, // RDA; DTMF sent
    SS_DUA, // RDA; Dua
    SS_DUAL_CLOCK, // RDA; Dual clock
    SS_DUAL_CLOCK_CANNOT_BE_USED_WHILE_SHORTCUTS_TOOLBAR_IS_ACTIVE, // RDA; Dual clock cannot be used while Shortcuts toolbar is active
    SS_DUAL_MCLOCK, // RDA; Dual
    SS_DUAL_SIM_ACTIVE, // RDA; Smart dual SIM
    SS_DUAL_SIM_ALWAYS_ON, // RDA; Dual SIM always on
    SS_DUAL_SIM_ALWAYS_ON_HELP_MSG_1, // RDA; Enabling dual SIM always on lets you receive calls from both SIM cards even during calls.
    SS_DUAL_SIM_ALWAYS_ON_HELP_MSG_2, // RDA; Dual SIM always on affects the call waiting and call forwarding settings for each SIM card if you are out of reach.
    SS_DUAL_SIM_ALWAYS_ON_HELP_MSG_3, // RDA; Dual SIM always on depends on network. There may be occasional network errors that may affect its performance
    SS_DUAL_STEREO_HEADSET, // RDA; Dual stereo headset
    SS_DUA_DAY, // RDA; Dua day
    SS_DUBA, // RDA; Duba
    SS_DUBAI, // RDA; Dubai
    SS_DUE_DATE, // RDA; Due date
    SS_DUE_DATE_SET_AFTER_START_DATE, // RDA; Due date should be set after start date
    SS_DUMAT_AL_JANDAL, // RDA; Dumat Al-Jandal
    SS_DUN_CONNECT_WITH_PS_Q, // RDA; DUN connect with %s?
    SS_DUPLICATED_NETWORK, // RDA; Duplicated network
    SS_DUPLICATED_RECIPIENT, // RDA; Duplicated recipient
    SS_DUPLICATE_MVERB, // RDA; Duplicate
    SS_DUQM, // RDA; Duqm
    SS_DURATION, // RDA; Duration
    SS_DURATION_ABB, // RDA; Duration
    SS_DURING_NEXT_CONNECTION, // RDA; During next connection
    SS_DYNAMIC, // RDA; Dynamic
    SS_EARPHONE, // RDA; Earphone
    SS_EARPHONE_NOT_CONNECTED_UNABLE_TO_USE_FM_RADIO_AS_ALARM_TONE, // RDA; Earphones not connected. Unable to use FM radio as alarm tone
    SS_EARPHONE_ON, // RDA; Earphone on
    SS_EARPHONE_REMOVED_RADIO_TURNED_OFF, // RDA; Earphone removed. Radio will be turned off
    SS_EASTERN_CANADA, // RDA; Eastern Canada
    SS_EASTERN_USA, // RDA; Eastern U.S.A
    SS_EASY, // RDA; Easy
    SS_EASY_PAIRING, // RDA; Easy pairing
    SS_EASY_PAIRING_INFO_ORANGE, // RDA; If AP supports WPS, press Yes to continue. Else, press No to start manual pairing
    SS_ECONOMY, // RDA; Economy
    SS_ECS, // RDA; ECS
    SS_EDIT, // RDA; Edit
    SS_EDIT_ATTACHMENT, // RDA; Edit attachment
    SS_EDIT_BLOCKED_ADDRESS, // RDA; Edit blocked address
    SS_EDIT_BOOKMARK, // RDA; Edit bookmark
    SS_EDIT_CONTACT, // RDA; Edit contact
    SS_EDIT_COST_LIMIT, // RDA; Edit cost limit
    SS_EDIT_DM_PROFILE, // RDA; Edit DM profile
    SS_EDIT_EXCHANGE_RATE, // RDA; Edit exchange rate
    SS_EDIT_FAVOURITES, // RDA; Edit favourite
    SS_EDIT_ITEM, // RDA; Edit item
    SS_EDIT_MESSAGE, // RDA; Edit message
    SS_EDIT_NAME, // RDA; Edit name
    SS_EDIT_NOTE, // RDA; Edit note
    SS_EDIT_NUMBER, // RDA; Edit number
    SS_EDIT_PAGE, // RDA; Edit Page
    SS_EDIT_PROFILE, // RDA; Edit profile
    SS_EDIT_REJECT_NUMBER, // RDA; Edit reject number
    SS_EDIT_SHORTCUTS, // RDA; Edit shortcuts
    SS_EDIT_STYLE, // RDA; Edit style
    SS_EDIT_SYNC_PROFILE, // RDA; Edit sync profile
    SS_EDIT_TELEPHONE_NUMBER, // RDA; Edit telephone number
    SS_EDIT_TEXT, // RDA; Edit text
    SS_EDIT_UC_RATE, // RDA; Edit Rate
    SS_EFFECT, // RDA; Effect
    SS_EFFECTS, // RDA; Effects
    SS_EGYPT, // RDA; Egypt
    SS_EKATERINBURG, // RDA; Ekaterinburg
    SS_ELECTRIC_BLUE, // RDA; Electric blue
    SS_EL_ALAMEIN, // RDA; El Alamein
    SS_EL_EULMA, // RDA; El Eulma
    SS_EL_GUETTAR, // RDA; El Guettar
    SS_EL_JADIDA, // RDA; El Jadida
    SS_EL_JEM, // RDA; El Jem
    SS_EL_MAHALLA_EL_KUBRA, // RDA; El-Mahalla El-Kubra
    SS_EL_OUED, // RDA; El Oued
    SS_EL_SENBELLAWEIN, // RDA; El Senbellawein
    SS_EMAIL, // RDA; Email
    SS_EMAILS, // RDA; Email
    SS_EMAIL_ACCOUNTS, // RDA; Email accounts
    SS_EMAIL_ACTIVATED, // RDA; Email activated
    SS_EMAIL_ACTIVATED_UNABLE_TO_DELETE, // RDA; Email activated. Unable to delete
    SS_EMAIL_ACTIVATED_UNABLE_TO_SAVE, // RDA; Email activated. Unable to save
    SS_EMAIL_ADDRESS, // RDA; Email address
    SS_EMAIL_ADDRESS_NOT_AVAILABLE, // RDA; Email address not available
    SS_EMAIL_ADDR_RECIPIENT_LOST_CONTINUE_Q, // RDA; Email address recipient might be lost. Continue?
    SS_EMAIL_ALERT_TONE, // RDA; Email alert tone
    SS_EMAIL_ALERT_TYPE, // RDA; Email alert type
    SS_EMAIL_BOX_FULL, // RDA; Email box full
    SS_EMAIL_BUSINESS, // RDA; Email, business
    SS_EMAIL_CONNECTION_TIMEOUT, // RDA; Email connection timeout
    SS_EMAIL_DELETED, // RDA; Email deleted
    SS_EMAIL_FILE_SYSTEM_BUSY, // RDA; Email file system busy
    SS_EMAIL_INBOX, // RDA; Email inbox
    SS_EMAIL_LIST_FULL_UNABLE_TO_SAVE_DELETE_SOME_EMAILS, // RDA; Email list full. Unable to save. Delete some emails
    SS_EMAIL_MBOX, // RDA; Email
    SS_EMAIL_MMESSAGE, // RDA; Email
    SS_EMAIL_NOT_FOUND, // RDA; Email not found
    SS_EMAIL_PRIVATE, // RDA; Email, private
    SS_EMAIL_PROFILE, // RDA; Email profile
    SS_EMAIL_PROFILES, // RDA; Email profiles
    SS_EMAIL_RETRIEVAL_LIMIT, // RDA; Email retrieval limit
    SS_EMAIL_RETRIEVED, // RDA; Email retrieved
    SS_EMAIL_SENT, // RDA; Email sent
    SS_EMAIL_SERVER_LOCKED, // RDA; Email server locked
    SS_EMAIL_SETUP_WIZARD, // RDA; Email setup wizard
    SS_EMBOSS, // RDA; Emboss
    SS_EMERALD, // RDA; Emerald
    SS_EMERALD_GREEN, // RDA; Emerald green
    SS_EMERGENCY_CALL, // RDA; Emergency call
    SS_EMERGENCY_CALLS_ONLY, // RDA; Emergency calls only
    SS_EMERGENCY_CALL_LA, // RDA; Emergency call
    SS_EMERGENCY_CALL_ONLY_INSERT_SIM_CARD_TO_ACCESS_NETWORK_SERVICE, // RDA; Emergency call only. Insert SIM card to access network service
    SS_EMERGENCY_CALL_Q, // RDA; Emergency call?
    SS_EMERGENCY_CALL_UNAVAILABLE, // RDA; Emergency call not available
    SS_EMERGENCY_M_CALL_SK_ABB, // RDA; Emergency
    SS_EMOTICON, // RDA; Emoticon
    SS_EMOTICONS, // RDA; Emoticons
    SS_EMOTIONS, // RDA; Emotions
    SS_EMPTIED, // RDA; Emptied
    SS_EMPTY, // RDA; Empty
    SS_EMPTY_COOKIES, // RDA; Empty cookies
    SS_EMPTY_COOKIES_Q, // RDA; Empty cookies?
    SS_EMPTY_MADJ, // RDA; Empty
    SS_EMPTY_MVERB, // RDA; Empty
    SS_ENABLE, // RDA; Enable
    SS_ENABLE_FREQUENT_CHECKING_OF_EMAIL_ONLY_IN_DOMESTIC, // RDA; Enable frequent checking of email only in domestic
    SS_ENABLE_OFF, // RDA; No
    SS_ENABLING_DUAL_SIM_ACTIVE_LETS_YOU_RECEIVE_CALLS_FROM_BOTH_SIM_CARDS_EVEN_DURING_CALLS_NOTI_MSG, // RDA; Enabling smart dual SIM lets you receive calls from both SIM cards even during calls Smart dual SIM affects the call waiting and call forwarding settings for each SIM card if out of reach Smart dual SIM depends on network. Occasional network errors may affect its performance.
    SS_END, // RDA; End
    SS_ENDS_WITH, // RDA; Ends with
    SS_END_ACTIVE_CALL, // RDA; End active call
    SS_END_CALL, // RDA; End call
    SS_END_DATE, // RDA; End date
    SS_END_DATE_ABB, // RDA; End date
    SS_END_DATE_SET_AFTER_START_DATE, // RDA; End date should be set after start date
    SS_END_HELD_CALL, // RDA; End held call
    SS_END_HELD_MULTI_PARTY_CALL, // RDA; End held multiparty call
    SS_END_MCALL, // RDA; End
    SS_END_OUTGOING_CALL, // RDA; End outgoing call
    SS_END_TIME, // RDA; End time
    SS_END_TIME_AND_DATE, // RDA; End time and date
    SS_END_TIME_CUST, // RDA; End time
    SS_END_TIME_FOR_IMAGE, // RDA; End time for image
    SS_END_TIME_FOR_SOUND, // RDA; End time for sound
    SS_END_TIME_FOR_TEXT, // RDA; End time for text
    SS_END_TIME_FOR_VIDEO, // RDA; End time for video
    SS_END_TIME_SHOULD_BE_SET_AFTER_START_TIME, // RDA; End time should be set after start time
    SS_END_UC, // RDA; END
    SS_ENERGETIC, // RDA; Energetic
    SS_ENERGETIC_ORANGE, // RDA; Energetic orange
    SS_ENFIDHA, // RDA; Enfidha
    SS_ENJOY_E, // RDA; Enjoy!
    SS_ENTER, // RDA; Enter
    SS_ENTERING_SECURE_AREA_OK_Q, // RDA; Entering secure area. OK?
    SS_ENTERTAINMENT, // RDA; Entertainment
    SS_ENTER_ADDITIONAL_INFORMATION, // RDA; Enter additional information
    SS_ENTER_ADDRESS, // RDA; Enter address
    SS_ENTER_A_NON_SECURE_AREA_OK_Q, // RDA; Entering a non-secure area. OK?
    SS_ENTER_A_VALUE_HIGHER_THAN_O, // RDA; Enter a value higher than 0
    SS_ENTER_CENTRE, // RDA; Enter centre
    SS_ENTER_CITY, // RDA; Enter city
    SS_ENTER_CODE, // RDA; Enter code
    SS_ENTER_CODE_OF_YOUR_WI_FI_ACCESS_POINT_IN_2_MINUTE_AFTER_SELECTING_START, // RDA; Enter code of your Wi-Fi access point in 2 minutes after selecting start
    SS_ENTER_COUNTRY, // RDA; Enter country
    SS_ENTER_COUNTRY_CODE_Q, // RDA; Enter country code?
    SS_ENTER_DATE, // RDA; Enter date
    SS_ENTER_FILE_NAME, // RDA; Enter file name
    SS_ENTER_MANUALLY, // RDA; Enter manually
    SS_ENTER_NAME, // RDA; Enter name
    SS_ENTER_NEW_CURRENCY, // RDA; Enter new currency
    SS_ENTER_NEW_PASSWORD, // RDA; Enter new password
    SS_ENTER_NEW_PHONE_PASSWORD, // RDA; Enter new phone password
    SS_ENTER_NEW_PIN, // RDA; Enter new PIN
    SS_ENTER_NEW_PIN2, // RDA; Enter new PIN2
    SS_ENTER_NEW_RATE, // RDA; Enter new rate
    SS_ENTER_NEW_VALUE, // RDA; Enter new value
    SS_ENTER_NUMBER, // RDA; Enter number
    SS_ENTER_NUMBER_FIRST, // RDA; Enter number first
    SS_ENTER_NUMBER_IT_MAY_NOT_WORK_WITHOUT_THE_CORRECT_NUMBER, // RDA; Enter number. It may not work without the correct number
    SS_ENTER_OCCASION, // RDA; Enter occasion
    SS_ENTER_OPERATOR_FIRST, // RDA; Enter operator first
    SS_ENTER_PASSWORD, // RDA; Enter password
    SS_ENTER_PASSWORD_POP, // RDA; Enter password
    SS_ENTER_PIN, // RDA; Enter PIN
    SS_ENTER_PIN2, // RDA; Enter PIN2
    SS_ENTER_PIN_TO_PAIR_WITH_PS, // RDA; Enter PIN to pair with %s
    SS_ENTER_PS_OR_PS_NUMBER_IT_MAY_NOT_WORK_WITHOUT_THE_CORRECT_NUMBER, // RDA; Enter %s or %s number. It may not work without the correct number.
    SS_ENTER_PUK, // RDA; Enter PUK
    SS_ENTER_PUK2, // RDA; Enter PUK2
    SS_ENTER_RECIPIENT, // RDA; Enter recipient
    SS_ENTER_RESPONSE, // RDA; Enter response
    SS_ENTER_SSID, // RDA; Enter SSID
    SS_ENTER_TASK, // RDA; Enter task
    SS_ENTER_URL, // RDA; Enter URL
    SS_ENTER_VOICE_MAIL_ADDRESS, // RDA; Enter voicemail address
    SS_ENTIRE_MESSAGE, // RDA; Entire message
    SS_ENTRIES_DO_NOT_MATCH, // RDA; Entries do not match
    SS_ENTRY_EMPTY, // RDA; Entry empty
    SS_ENTRY_IS_EMPTY, // RDA; Entry is empty
    SS_EQUALISER, // RDA; Equaliser
    SS_EQUALISER_TYPE, // RDA; Equaliser type
    SS_ERASE, // RDA; Erase
    SS_ERBIL, // RDA; Erbil
    SS_ERMES, // RDA; ERMES
    SS_ERROR, // RDA; Error
    SS_ERROR_DURING_INSTALLATION, // RDA; Error during installation
    SS_ERROR_NETWORK_UNAVAILABLE, // RDA; Error: Network not available
    SS_ERROR_SETTING_DATE, // RDA; Error setting date
    SS_ERROR_SETTING_FREQUENCY, // RDA; Error setting frequency
    SS_ERROR_SETTING_TIME, // RDA; Error setting time
    SS_ESFAND_MEA, // RDA; Esfand
    SS_ESPRESSO_MTHEME, // RDA; Espresso
    SS_ESSAOUIRA, // RDA; Essaouira
    SS_ETC_1, // RDA; Etc. 1
    SS_ETC_2, // RDA; Etc. 2
    SS_ETC_3, // RDA; Etc. 3
    SS_ETC_4, // RDA; Etc. 4
    SS_ETC_5, // RDA; Etc. 5
    SS_ETC_LC, // RDA; Etc.
    SS_ETIQUETTE_MODE_ACTIVATED_KEEP_PLAYING_SOUND_IN_ETIQUETTE_MODE_Q, // RDA; Etiquette mode activated. Keep playing sound in Etiquette mode?
    SS_ETIQUETTE_PAUSE, // RDA; Etiquette pause
    SS_ETIQUETTE_PAUSE_EXPLANATION, // RDA; If you turn over the phone, the current sound is temporarily muted. The sound profile is not changed
    SS_EUR, // RDA; EUR
    SS_EVENT, // RDA; Event
    SS_EVENTS_DURATION_SHOULD_BE_SHORTER_THAN_REPEAT_FREQUENCY, // RDA; Event's duration should be shorter than repeat frequency
    SS_EVENT_LIST, // RDA; Event list
    SS_EVERY, // RDA; Every
    SS_EVERYDAY, // RDA; Everyday
    SS_EVERY_10_MINUTES, // RDA; Every 10 minutes
    SS_EVERY_2_MINUTES, // RDA; Every 2 minutes
    SS_EVERY_5_MINUTES, // RDA; Every 5 minutes
    SS_EVERY_MONTH, // RDA; Every month
    SS_EVERY_WEEK, // RDA; Every week
    SS_EVERY_YEAR, // RDA; Every year
    SS_EXACTLY_THE_SAME_AS, // RDA; Exactly the same as
    SS_EXCEEDS_LENGTH_LIMIT, // RDA; Exceeds length limit
    SS_EXCEPT_WEEKENDS, // RDA; Except weekends
    SS_EXCHANGE_CHANNELS, // RDA; Exchange channels
    SS_EXCHANGE_DATA_WITH_PS_Q, // RDA; Exchange data with %s?
    SS_EXCHANGE_NAMECARD, // RDA; Exchange namecard
    SS_EXCHANGE_OBJECT_WITH_PS_Q, // RDA; Exchange object with %s?
    SS_EXCHANGE_RATE, // RDA; Exchange rate
    SS_EXECUTE, // RDA; Execute
    SS_EXECUTE_Q, // RDA; Execute?
    SS_EXISTING_MYFAVES_CONTACT, // RDA; Existing MyFaves contact
    SS_EXIT, // RDA; Exit
    SS_EXIT_BROWSER, // RDA; Exit Browser
    SS_EXIT_COMPOSER, // RDA; Exit composer
    SS_EXIT_CURRENT_JAVA_APPLICATION_AND_INSTALL_NEW_JAVA_APPLICATION_Q, // RDA; Exit current Java application and install new Java application?
    SS_EXIT_Q, // RDA; Exit?
    SS_EXIT_SK_ABB, // RDA; Exit
    SS_EXIT_WITHOUT_SAVING, // RDA; Exit without saving
    SS_EXPIRED, // RDA; Expired
    SS_EXPIRY, // RDA; Expiry
    SS_EXPIRY_DATE, // RDA; Expiry date
    SS_EXPIRY_DATE_IS_ALREADY_PASSED, // RDA; Expiry date is already passed
    SS_EXTERNAL_LCD, // RDA; External LCD
    SS_EXTERNAL_LIGHT, // RDA; External light
    SS_EXTRACTING_DRM_ING, // RDA; Extracting DRM...
    SS_FABULOUS, // RDA; Fabulous
    SS_FADE, // RDA; Fade
    SS_FAHAHEEL, // RDA; Fahaheel
    SS_FAILED, // RDA; Failed
    SS_FAILED_TO_ADD_MEDIA, // RDA; Failed to add media
    SS_FAILED_TO_ANSWER_CALL, // RDA; Failed to answer call
    SS_FAILED_TO_ASSOCIATE_WITH_PS, // RDA; Failed to associate with %s
    SS_FAILED_TO_CANCEL_ABB, // RDA; Failed to connect
    SS_FAILED_TO_CONNECT_NETWORK, // RDA; Failed to connect network
    SS_FAILED_TO_UPDATE, // RDA; Update failed
    SS_FAILED_TRY_AGAIN, // RDA; Failed. Try again
    SS_FAILURE_STATE, // RDA; Error state
    SS_FAIL_TO_DECODE, // RDA; Failed to decode
    SS_FAIL_TO_VERIFY, // RDA; Failed to verify
    SS_FAISALABAD, // RDA; Faisalabad
    SS_FAJR, // RDA; Fajr
    SS_FAKE_CALL, // RDA; Fake call
    SS_FAKE_CALL_ACTIVATED, // RDA; Fake call activated
    SS_FAKE_CALL_ACTIVATED_LA, // RDA; Fake call activated
    SS_FAKE_CALL_HOT_KEY, // RDA; Fake call hot key
    SS_FAKE_CALL_SETTINGS, // RDA; Fake call settings
    SS_FAKE_CALL_TIMER, // RDA; Fake call timer
    SS_FAKE_CALL_TIMER_HELP_MSG, // RDA; After activating fake call, you can accept the fake call as soon as the time you will select has elapsed
    SS_FAKE_CALL_VOICE, // RDA; Fake call voice
    SS_FAKE_CALL_VOICE_HELP_MSG, // RDA; During a fake call, you will hear the voice that you are about to record
    SS_FALLUJAH, // RDA; Falluja
    SS_FAMILY, // RDA; Family
    SS_FANFARE, // RDA; Fanfare
    SS_FARASAN_CITY, // RDA; Farasan
    SS_FARVARDIN_MEA, // RDA; Farvardin
    SS_FARWANIYAH, // RDA; Al Farwaniyah
    SS_FAST, // RDA; Fast
    SS_FAVORITES_TWITTER, // RDA; Favorites
    SS_FAVOURITES, // RDA; Favourites
    SS_FAVOURITES_T_CONTACTS, // RDA; Favourites
    SS_FAVOURITES_T_RADIO, // RDA; Favourites
    SS_FAVOURITE_IMAGES, // RDA; Favourite images
    SS_FAVOURITE_SOUNDS, // RDA; Favourite sounds
    SS_FAVOURITE_VIDEOS, // RDA; Favourite videos
    SS_FAX, // RDA; Fax
    SS_FAX_1, // RDA; Fax 1
    SS_FAX_2, // RDA; Fax 2
    SS_FAX_3, // RDA; Fax 3
    SS_FAX_4, // RDA; Fax 4
    SS_FAX_5, // RDA; Fax 5
    SS_FAX_CALLS, // RDA; Fax calls
    SS_FDN, // RDA; FDN
    SS_FDN_ALREADY_DISABLED, // RDA; FDN already disabled
    SS_FDN_ALREADY_ENABLED, // RDA; FDN already enabled
    SS_FDN_CONTACT, // RDA; FDN contact
    SS_FDN_CONTACTS, // RDA; FDN contacts
    SS_FDN_CONTACT_LIST_FULL_DELETE_SOME_ITEMS, // RDA; FDN contact list full. Delete some items
    SS_FDN_DISABLED, // RDA; FDN disabled
    SS_FDN_ENABLED, // RDA; FDN enabled
    SS_FDN_MODE, // RDA; FDN mode
    SS_FDN_MODE_ACTIVATED, // RDA; FDN mode activated
    SS_FDN_SETTINGS, // RDA; FDN settings
    SS_FEB, // RDA; Feb.
    SS_FEBRUARY, // RDA; February
    SS_FEET2_MCVT, // RDA; Feet²
    SS_FEET3_MCVT, // RDA; Feet³
    SS_FEET_HINT, // RDA; Feet (int.)
    SS_FEET_HUS, // RDA; feet (US s.)
    SS_FEET_HUS_MCVT, // RDA; Feet (US)
    SS_FEZ, // RDA; Fez
    SS_FIGUIG, // RDA; Figuig
    SS_FILE, // RDA; File
    SS_FILES, // RDA; Files
    SS_FILE_ATTACH, // RDA; File attach
    SS_FILE_BROWSING_FAILED, // RDA; File browsing failed
    SS_FILE_DETAIL, // RDA; File detail
    SS_FILE_ERROR, // RDA; File error
    SS_FILE_FORMAT, // RDA; File Format
    SS_FILE_MANAGER, // RDA; File manager
    SS_FILE_NAME, // RDA; File name
    SS_FILE_NAME_ALREADY_EXISTS, // RDA; File name already exists
    SS_FILE_NAME_ALREADY_IN_USE, // RDA; File name already in use
    SS_FILE_NAME_EMPTY, // RDA; File name empty
    SS_FILE_NOT_FOUND, // RDA; File not found
    SS_FILE_RECEIVED, // RDA; File received
    SS_FILE_RIGHTS_RECEIVED, // RDA; File rights received
    SS_FILE_TRANSFER, // RDA; File transfer
    SS_FILE_TRANSFER_ALERT, // RDA; Used to connect to other Bluetooth devices via a virtual serial port
    SS_FILE_VIEWR_ERROR, // RDA; File viewer error
    SS_FILTER, // RDA; Filter
    SS_FIND_TAG, // RDA; Find tag
    SS_FIND_TAG_SETTINGS, // RDA; Find tag settings
    SS_FINE, // RDA; Fine
    SS_FIRMWARE_DATA_EXCEEDS_MAXIMUM_SIZE, // RDA; Firmware data exceeds maximum size
    SS_FIRST_NAME, // RDA; First name
    SS_FIRST_NAME_LAST_NAME, // RDA; First name Last name
    SS_FIRST_PAGE, // RDA; First page
    SS_FISH, // RDA; Fish
    SS_FIT_TO, // RDA; Fit to
    SS_FIXED_DIALING_MODE, // RDA; Fixed dialling mode
    SS_FIXED_DIALING_NUMBERS_ONLY, // RDA; Fixed dialing numbers only
    SS_FIXED_DIALLING_MODE_DEACTIVATED_ACTIVATE_Q, // RDA; Fixed dialling mode deactivated. Activate?
    SS_FLAPPER, // RDA; Flapper
    SS_FLASH_MCAMERA, // RDA; Flash
    SS_FLASH_MCAMERA_TMO, // RDA; Flash
    SS_FLASH_OFF, // RDA; Flash off
    SS_FLASH_ON, // RDA; Flash on
    SS_FLIGHT_MODE, // RDA; Flight mode
    SS_FLIP, // RDA; Flip
    SS_FLIP_HORIZONTALLY, // RDA; Flip horizontally
    SS_FLIP_VERTICALLY, // RDA; Flip vertically
    SS_FLOWER, // RDA; Flower
    SS_FLUORESCENT, // RDA; Flourescent
    SS_FM_RADIO, // RDA; FM Radio
    SS_FM_RADIO_AUTO_OFF, // RDA; FM Radio auto off
    SS_FM_RADIO_CLIPS, // RDA; FM radio clips
    SS_FM_RADIO_CLIPS_LIST_FULL_DELETE_SOME_ITMES, // RDA; FM radio clips list full. Delete some items
    SS_FM_RADIO_FAVOURITES, // RDA; FM radio favourites
    SS_FM_RADIO_NOT_SUPPORTED_WHILE_CALLING, // RDA; FM radio not supported while calling
    SS_FM_RADIO_OFF, // RDA; FM radio off
    SS_FM_RADIO_ON, // RDA; FM radio on
    SS_FM_RADIO_PROGRAMME_REMINDER, // RDA; FM radio programme reminder
    SS_FM_RADIO_PROG_REMINDER_ABB, // RDA; FM radio prog. reminder
    SS_FOCUS, // RDA; Focus
    SS_FOCUSED_EMAIL_ADDRESS, // RDA; Focused email address
    SS_FOCUSED_FILE, // RDA; Focused file
    SS_FOCUSED_FONT, // RDA; Focused font
    SS_FOCUSED_MENU_ICON, // RDA; Focused menu icon
    SS_FOCUSED_PHONE_NUMBER, // RDA; Focused phone number
    SS_FOCUSED_WEB_ADDRESS, // RDA; Focused Web address
    SS_FOCUS_EFFECT, // RDA; Focus effect
    SS_FOG, // RDA; Fog
    SS_FOLDER, // RDA; Folder
    SS_FOLDERS, // RDA; Folders
    SS_FOLDER_EMPTY, // RDA; Folder empty
    SS_FONT_COLOUR, // RDA; Font colour
    SS_FONT_SIZE, // RDA; Font size
    SS_FONT_STYLE, // RDA; Font style
    SS_FONT_STYLE_T_MMS, // RDA; Font style
    SS_FONT_TYPE, // RDA; Font type
    SS_FOR, // RDA; for
    SS_FORMATTING, // RDA; Formatting...
    SS_FORMATTING_ERASES_ALL_DATA_N_LICENSES_BUT_MAINTAINS_PARTITION_ASSIGNED, // RDA; Formatting erases all data and licences but maintains the partition assigned
    SS_FORMATTING_ERASE_ALL_DATA_CONTINUE_Q, // RDA; Formatting will erase all data. Continue?
    SS_FORMATTING_ING, // RDA; Formatting...
    SS_FORMAT_COMPLETED, // RDA; Format completed
    SS_FORMAT_MEMROY_CARD, // RDA; Format memory card
    SS_FORMAT_MNOUN, // RDA; Format
    SS_FORMAT_MVERB, // RDA; Format
    SS_FORMAT_SDCARD, // RDA; Format
    SS_FORTNIGHTLY, // RDA; Fortnightly
    SS_FORWARD, // RDA; Forward
    SS_FORWARDED, // RDA; Forwarded
    SS_FORWARDING, // RDA; Forwarding
    SS_FORWARDING_ALLOWED, // RDA; Forwarding allowed
    SS_FORWARD_MBROWSER, // RDA; Forward
    SS_FORWARD_MMESSAGE, // RDA; Forward
    SS_FOR_FULLMEMORY, // RDA; Use all available memory
    SS_FOUND, // RDA; Found
    SS_FOUNTAIN_PEN, // RDA; Fountain pen
    SS_FRAME, // RDA; Frame
    SS_FRAMES, // RDA; Frames
    SS_FREE, // RDA; Free
    SS_FREE_MAVAILABLE, // RDA; Free
    SS_FREE_SPACE, // RDA; Free space
    SS_FREQUENCY, // RDA; Frequency
    SS_FREQUENCY_ALREADY_IN_USE, // RDA; Frequency already in use
    SS_FREQUENCY_M_BANDWIDTH, // RDA; Frequency
    SS_FREQUENT_CHECKING_OF_EMAIL_INCUR_HIGH_COSTS, // RDA; Frequent checking of email may incur high costs, especially when abroad
    SS_FREQUENT_CHECKING_OF_EMAIL_MAY_INCUR_HIGH_COSTS_WHEN_ABROAD_CONTINUE_Q, // RDA; Frequent checking of email may incur high costs, especially when abroad. Continue?
    SS_FRI, // RDA; Fri
    SS_FRIDAY, // RDA; Friday
    SS_FRIENDS, // RDA; Friends
    SS_FRI_FM, // RDA; F
    SS_FROM_1971_TO_2098_AVAILABLE, // RDA; From 1971 to 2098 available
    SS_FROM_2000_TO_2098_AVAILABLE, // RDA; From 2000 to 2098 available
    SS_FROM_C, // RDA; From:
    SS_FROM_MSENDER, // RDA; From
    SS_FROM_MSTARTING, // RDA; From
    SS_FROM_PHONE, // RDA; From phone
    SS_FROM_PHONEBOOK, // RDA; From Phonebook
    SS_FROM_PHONE_AND_SIM, // RDA; From phone and SIM
    SS_FROM_SIM, // RDA; From SIM
    SS_FROSTED_GLASS, // RDA; Frosted glass
    SS_FT, // RDA; ft
    SS_FUJAIRAH, // RDA; Fujairah
    SS_FULL, // RDA; Full
    SS_FULLY_CHARGED, // RDA; Fully charged
    SS_FULL_BEARD, // RDA; Full beard
    SS_FULL_FORMAT, // RDA; Full format
    SS_FULL_FORMATTING_ERASES_ALL_DATA_AND_LICENSES, // RDA; Full formatting erases all data and licences
    SS_FULL_GREETING, // RDA; Full greeting
    SS_FULL_M_BATTERY, // RDA; Full
    SS_FULL_RECORDED_GREETING, // RDA; Full recorded greeting
    SS_FULL_SCREEN, // RDA; Full screen
    SS_FULL_SCREEN_VIEW, // RDA; Full screen view
    SS_FULL_SCREEN_VIEW_AND_INDICATOR, // RDA; Full screen view and indicator
    SS_FULL_SYNC, // RDA; Full sync
    SS_FUNCTIONS, // RDA; Functions
    SS_FUNCTIONS_OF_SECOND_HEADSET_WILL_BE_RESTRICTED, // RDA; Functions of secondary headset will be restricted
    SS_FUNKY, // RDA; Funky
    SS_FWD_C, // RDA; Fwd:
    SS_FW_C_UC, // RDA; FW:
    SS_F_MFRIYDAY, // RDA; F
    SS_F_MTEMPERATURE, // RDA; °F
    SS_G, // RDA; g
    SS_GALLERY, // RDA; Gallery
    SS_GALLONS_HUK_MCVT, // RDA; Gallons (UK)
    SS_GALLONS_HUS_MCVT, // RDA; Gallons (US)
    SS_GALLON_HUK, // RDA; Gallon (UK)
    SS_GALLON_HUS, // RDA; Gallon (US)
    SS_GAME, // RDA; Game
    SS_GAMES, // RDA; Games
    SS_GAMES_AND_MORE, // RDA; Games and more
    SS_GANDHI_JAYANTI_SWA, // RDA; Gandhi jayanti
    SS_GANESH_CHATURATHI_SWA, // RDA; Ganesh chaturathi
    SS_GATEWAY_TIMEOUT_UC, // RDA; Gateway timeout
    SS_GATE_WAY, // RDA; Gateway
    SS_GAZIANTEP, // RDA; Gaziantep
    SS_GBP, // RDA; GBP
    SS_GEELONG, // RDA; Geelong
    SS_GENARAL, // RDA; General
    SS_GENERAL_MODE, // RDA; General mode
    SS_GENERAL_SETTINGS, // RDA; General settings
    SS_GENERIC, // RDA; Generic
    SS_GENRE, // RDA; Genre
    SS_GENRES, // RDA; Genres
    SS_GERANUM, // RDA; Geranium
    SS_GET, // RDA; Get
    SS_GETTING_KEY_N_NOTIFY_WHEN_RECEIVED, // RDA; Getting key...\nYou will be notified when it is received
    SS_GETTING_SERVICE_LIST_ING, // RDA; Retrieving service list…
    SS_GET_ALL, // RDA; Get all
    SS_GET_KEY_ONLINE_TO_ACTIVATE_THIS_ITEM, // RDA; Get the key online to activate this item
    SS_GET_LICENCE, // RDA; Get licence
    SS_GET_MULTIPLE, // RDA; Get multiple
    SS_GET_WELL_SOON_ING, // RDA; Get well soon...
    SS_GHADAMIS, // RDA; Ghadames
    SS_GHARDAIA, // RDA; Ghardaia
    SS_GHARDIMAOU, // RDA; Ghardimaou
    SS_GHAR_EL_MELH, // RDA; Ghar El Melh
    SS_GHAT, // RDA; Ghat
    SS_GHAZNI, // RDA; Ghazni
    SS_GHIRYAN, // RDA; Gharyan
    SS_GHOMRASSEN, // RDA; Ghomrassen
    SS_GIRL, // RDA; Girl
    SS_GIRL1, // RDA; Girl1
    SS_GIRL2, // RDA; Girl2
    SS_GIZA, // RDA; Giza
    SS_GLASSES_MALE, // RDA; Glasses male
    SS_GLASSES_MALE_2, // RDA; Glasses male 2
    SS_GLOSSY, // RDA; Glossy
    SS_GMT, // RDA; GMT
    SS_GOAL_ACTIVATON, // RDA; Goal activation
    SS_GOAL_SETTINGS, // RDA; Goal settings
    SS_GOATEE, // RDA; Goatee
    SS_GOATEE_2, // RDA; Goatee 2
    SS_GOOD_FRIDAY_SWA, // RDA; Good Friday
    SS_GOOD_M_STRENGTH, // RDA; Good
    SS_GOOGLESEARCH, // RDA; Google Search
    SS_GO_BACKWARD, // RDA; Go backward
    SS_GO_FORWARD, // RDA; Go forward
    SS_GO_MBROWSER, // RDA; Go
    SS_GO_NOW_Q, // RDA; Go now?
    SS_GO_SK_ABB, // RDA; Go
    SS_GO_TO, // RDA; Go to
    SS_GO_TO_ALL_PLAYLIST, // RDA; Go to all playlists
    SS_GO_TO_CONVERTOR, // RDA; Go to Convertor
    SS_GO_TO_DATE, // RDA; Go to date
    SS_GO_TO_HOMEPAGE, // RDA; Go to homepage
    SS_GO_TO_JAVA_WORLD, // RDA; Go to Java world
    SS_GO_TO_MEDIA_ALBUM, // RDA; Go to Media album
    SS_GO_TO_MESSAGE, // RDA; Go to message
    SS_GO_TO_MY_FILES, // RDA; Go to My files
    SS_GO_TO_MY_MESSAGES, // RDA; Go to My messages
    SS_GO_TO_MY_PHOTOS, // RDA; Go to My photos
    SS_GO_TO_MY_VIDEO_CLIPS, // RDA; Go to My video clips
    SS_GO_TO_MY_VOICE_CLIPS, // RDA; Go to My voice clips
    SS_GO_TO_URL, // RDA; Go to URL
    SS_GPRS_FAIL, // RDA; GPRS fail
    SS_GPRS_FIRST, // RDA; GPRS first
    SS_GPRS_ONLY, // RDA; GPRS only
    SS_GPRS_PREFERRED, // RDA; GPRS preferred
    SS_GPRS_SETTINGS, // RDA; GPRS settings
    SS_GRADATION, // RDA; Gradation
    SS_GRAIN, // RDA; grain
    SS_GRAINS_MCVT, // RDA; Grains
    SS_GRAM, // RDA; gram
    SS_GRAPHICS, // RDA; Graphics
    SS_GRAPHIC_ID, // RDA; Graphic ID
    SS_GREEN, // RDA; Green
    SS_GREETING_MESSAGE, // RDA; Greeting message
    SS_GREGORIAN_MEA, // RDA; Gregorian
    SS_GREY, // RDA; Gray
    SS_GROUP, // RDA; Group
    SS_GROUPS, // RDA; Groups
    SS_GROUP_MPBMAINMENU, // RDA; Group
    SS_GROUP_NAME, // RDA; Group name
    SS_GROUP_NAME_ALREADY_IN_USE, // RDA; Group name already in use
    SS_GROUP_RINGTONE, // RDA; Group ringtone
    SS_GROUP_SETTINGS, // RDA; Group settings
    SS_GSM, // RDA; GSM
    SS_GSM_1900, // RDA; GSM 1900
    SS_GSM_850_1900, // RDA; GSM 850/1900
    SS_GSM_900_1800, // RDA; GSM 900/1800
    SS_GSM_ALPHABET, // RDA; GSM alphabet
    SS_GSM_ONLY, // RDA; GSM only
    SS_GSM_SETTINGS, // RDA; GSM settings
    SS_GUAM, // RDA; Guam
    SS_GUELMA, // RDA; Guelma
    SS_GUELMIM, // RDA; Guelmim
    SS_GUIDE_TO_RECORD_GREETING_FOR_VOICE_MAIL, // RDA; Only phone number without greeting is heard from voicemail. Record greeting message?
    SS_GUITAR_1, // RDA; Guitar 1
    SS_GUITAR_2, // RDA; Guitar 2
    SS_GUITAR_3, // RDA; Guitar 3
    SS_GUITAR_4, // RDA; Guitar 4
    SS_GUJARATI_INPUT, // RDA; કખગ
    SS_GUJRANWALA, // RDA; Gujranwala
    SS_GUJRAT, // RDA; Gujrat
    SS_GURAYAT, // RDA; Gurayat
    SS_HADITHA, // RDA; Haditha
    SS_HAFR_AL_BATIN, // RDA; Hafar al Batin
    SS_HAGAKI_POSTCARD, // RDA; Hagaki postcard
    SS_HAIL, // RDA; Hali
    SS_HAJJAH, // RDA; Hajjah
    SS_HAJRAH, // RDA; Hajrah
    SS_HALABJA, // RDA; Halabja
    SS_HALAIB, // RDA; Halaib
    SS_HAMA, // RDA; Hama
    SS_HAMEDAN, // RDA; Hamedan
    SS_HAMMAMET, // RDA; Hammamet
    SS_HAMMAM_LIF, // RDA; Hammam-Lif 
    SS_HAMMAM_SOUSSE, // RDA; Hammam Sousse
    SS_HAMRAH_DAWM, // RDA; Hamrah Dawm
    SS_HANDSFREE, // RDA; Handsfree
    SS_HANOI, // RDA; Hanoi
    SS_HANUMAN_JAYANTI_SWA, // RDA; Hanuman jayanti
    SS_HAPPY, // RDA; Happy
    SS_HAPPY_ANNIVERSARY_E, // RDA; Happy Anniversary!
    SS_HAPPY_BIRTHDAY_E, // RDA; Happy birthday!
    SS_HAQL, // RDA; Haql
    SS_HARAD, // RDA; Harad
    SS_HARDWARE_ERROR, // RDA; Hardware error
    SS_HARMAH, // RDA; Harmah
    SS_HASHTGERD, // RDA; Hashtgerd
    SS_HAWAII, // RDA; Hawaii
    SS_HAWALLI, // RDA; Hawalli
    SS_HAZELNUT_BROWN, // RDA; Hazelnut brown
    SS_HD_PD, // RDA; (D-%d)
    SS_HEADER, // RDA; Header
    SS_HEADERS_ONLY, // RDA; Headers only
    SS_HEADSET, // RDA; Headset
    SS_HEART, // RDA; Heart
    SS_HEARTBREAK, // RDA; Heartbreak
    SS_HEART_1, // RDA; Heart 1
    SS_HEART_2, // RDA; Heart 2
    SS_HECTARE, // RDA; hectare
    SS_HECTARES_MCVT, // RDA; Hectares
    SS_HEIGHT, // RDA; Height
    SS_HEIGHT_MHUMAN, // RDA; Height
    SS_HELD, // RDA; Held
    SS_HELD_CALL, // RDA; Held call
    SS_HELP, // RDA; Help
    SS_HELPTEXT_POWER_SAVING_MODE, // RDA; Conserve battery power by enabling power saving mode. LCD brightness and contrast are set to minimum value
    SS_HELP_INFO_SEND_KEY, // RDA; In an emergency press the send key 4 times continuously when key lock is active.
    SS_HELP_TEXT_POWER_SAVING_MODE, // RDA; When Power saving mode is enabled to conserve battery, backlight time and LCD brightness will be set to minimum value and LCD will be off without dimming. Keypad light will keep its custom setting.
    SS_HELSINKI, // RDA; Helsinki
    SS_HERAT, // RDA; Herat
    SS_HERGLA, // RDA; Hergla
    SS_HH_C_MM, // RDA; HH:MM
    SS_HIDDEN, // RDA; Hidden
    SS_HIDDEN_KEY_IMSI_OFF_REPLACEMENT, // RDA; IMSI Replacement Off
    SS_HIDDEN_KEY_IMSI_ONOFF_CAPTION, // RDA; Phone test
    SS_HIDDEN_KEY_IMSI_ON_REPLACEMENT, // RDA; IMSI Replacement On
    SS_HIDDEN_KEY_LOOPBACK_CAPTION, // RDA; LoopBack Test Mode
    SS_HIDDEN_KEY_LOOPBACK_EXIT, // RDA; EXIT
    SS_HIDDEN_KEY_LOOPBACK_E_P, // RDA; E/P Mode
    SS_HIDDEN_KEY_LOOPBACK_LOUD, // RDA; Loud Mode
    SS_HIDDEN_KEY_LOOPBACK_MODE_OFF, // RDA; Mode off
    SS_HIDDEN_KEY_LOOPBACK_RECEIVER, // RDA; Receiver Mode
    SS_HIDDEN_NETWORK, // RDA; Hidden network
    SS_HIDDEN_NETWORKS, // RDA; Hidden networks
    SS_HIDE, // RDA; Hide
    SS_HIDE_ADDRESS, // RDA; Hide address
    SS_HIDE_NUMBER, // RDA; Hide number
    SS_HIGH, // RDA; High
    SS_HIGHEST_NUMBER, // RDA; Highest number
    SS_HIGHT_MPRIORITY, // RDA; High
    SS_HIGH_HAT_1, // RDA; High-hat 1
    SS_HIGH_HAT_2, // RDA; High-hat 2
    SS_HIGH_MPRIORITY, // RDA; High
    SS_HIGH_MQUALITY, // RDA; High
    SS_HIJRI_GHAMARI_MEA, // RDA; Hijri/Ghamari
    SS_HINDI_INPUT, // RDA; कखग
    SS_HINDU_NEW_YEAR_SWA, // RDA; Hindu new year
    SS_HINDU_PRARTHANA, // RDA; Hindu prarthana
    SS_HISTORY_MPLAYLIST, // RDA; History
    SS_HISTORY_MWEBPAGE, // RDA; History
    SS_HIT, // RDA; Hit
    SS_HMAC, // RDA; HMAC
    SS_HNO_SUBJECT, // RDA; (No subject)
    SS_HOBART, // RDA; Hobart
    SS_HODEIDAH, // RDA; Hodeidah
    SS_HOFUF, // RDA; Hofuf
    SS_HOLD, // RDA; Hold
    SS_HOLDING_ING, // RDA; Holding...
    SS_HOLD_ACTIVE_CALL, // RDA; Hold active call
    SS_HOLD_DOWN_1_KEY_CONNECT_TO_VOICE_MAIL_SERVER, // RDA; Hold down the '1' key to connect to the voicemail server when you are not in an application or call
    SS_HOLIDAY, // RDA; Holiday
    SS_HOLIDAYS, // RDA; Holidays
    SS_HOLI_SWA, // RDA; Holi
    SS_HOMEPAGE, // RDA; Homepage
    SS_HOMEPAGE_NOT_SET, // RDA; Homepage not set
    SS_HOME_1, // RDA; Home 1
    SS_HOME_2, // RDA; Home 2
    SS_HOME_3, // RDA; Home 3
    SS_HOME_4, // RDA; Home 4
    SS_HOME_5, // RDA; Home 5
    SS_HOME_MBROWSER, // RDA; Home
    SS_HOME_MPB, // RDA; Home
    SS_HOME_NETWORK, // RDA; Home network
    SS_HOME_PAGE, // RDA; Home page
    SS_HOME_RECEPTION, // RDA; Home reception
    SS_HOME_SCREEN, // RDA; Home screen
    SS_HOME_SCREEN_TOOLBAR_CAN_CONTAIN_UP_TO_15_SHORTCUTS, // RDA; Home screen toolbar can contain up to 15 shortcuts
    SS_HOME_SCREEN_TOOLBAR_CAN_CONTAIN_UP_TO_18_SHORTCUTS, // RDA; Home screen toolbar can contain up to 18 shortcuts
    SS_HOME_URL, // RDA; Home URL
    SS_HOME_ZONE, // RDA; Home zone
    SS_HOME_ZONE_TIME_CHANGED, // RDA; Home zone time changed
    SS_HOMS, // RDA; Homs
    SS_HONGKONG, // RDA; Hong Kong
    SS_HONOLULU, // RDA; Honolulu
    SS_HOUMT_SOUK, // RDA; Houmt Souk
    SS_HOUR, // RDA; Hour
    SS_HOURS, // RDA; hours
    SS_HOURS_LC, // RDA; hours
    SS_HOUR_LC, // RDA; hour
    SS_HOUR_MIN_SEC, // RDA; HH:MM:SS
    SS_HOW_ARE_YOU_TODAY_Q, // RDA; How are you today?
    SS_HOW_TO_ACTIVATE_FAKE_CALL_USING_THE_DOWN_KEY_HELP_MSG, // RDA; In an emergency, you can receive a fake call after a set time. Press and hold the down key to activate Fake call. Press the down key 4 times continuously when key lock is active
    SS_HOW_TO_ACTIVATE_FAKE_CALL_USING_THE_DOWN_KEY_HELP_MSG_ABB, // RDA; In an emergency, you can receive a fake call after a set time. Press and hold the down key to activate Fake call. Press the down key 4 times continuously when key lock is active.
    SS_HTTP, // RDA; HTTP
    SS_HTTPSTREAM_NUMBER, // RDA; Httpstream number
    SS_HTTP_AUTHENTICATION, // RDA; HTTP authentication
    SS_HURGHADA, // RDA; Hurghada
    SS_HYDERABAD, // RDA; Hyderabad
    SS_Help, // RDA; Help
    SS_IBADAN, // RDA; Ibadan
    SS_IBB, // RDA; Ibb
    SS_IBRA, // RDA; Ibra
    SS_IBRI, // RDA; Ibri
    SS_ICONS, // RDA; Icons
    SS_ICQ, // RDA; ICQ
    SS_ID, // RDA; ID
    SS_IDLIB, // RDA; Idlib
    SS_ID_E_MILAD_SWA, // RDA; Id-E-Milad
    SS_ID_UL_FITR_HEND_OF_RAMADAN_SWA, // RDA; Id ul Fitr (End of Ramadan)
    SS_IF, // RDA; If
    SS_IFRANE, // RDA; Ifrane
    SS_IF_BUSY, // RDA; If busy
    SS_IF_NO_REPLY, // RDA; If no reply
    SS_IF_OUT_OF_REACH, // RDA; If out of reach
    SS_IF_THEY_PHONE_YOU_BACK_THE_PHONE_WILL_NOT_RING_BUT_THEIR_CALL_WILL_BE_AUTOMATICALLY_ANSWERED, // RDA; If they call you back, the phone will not ring but their call will be automatically answered.
    SS_IF_YOU_ACCEPT_THE_INCOMING_CALL_CALL_TIME_LIMIT_WILL_BE_TEMPORARILY_DISABLED, // RDA; If you accept the incoming call, call time limit will be temporarily disabled
    SS_IF_YOU_RENAME_THIS_ITEM_THE_MUSIC_WILL_STOP_CONTINUE_Q, // RDA; If you rename this item the music will stop. Continue?
    SS_IF_YOU_TICK_THIS_OPTION_CM_YOUR_OPERATOR_MAY_NOT_SEND_MULTIMEDIA_MESSAGES, // RDA; If you tick this option, your operator may not send multimedia messages
    SS_ILORIN, // RDA; Ilorin
    SS_ILS, // RDA; ILS
    SS_IMAGE, // RDA; Image
    SS_IMAGES, // RDA; Images
    SS_IMAGE_BASIC, // RDA; Image basic
    SS_IMAGE_EDITOR, // RDA; Image editor
    SS_IMAGE_IS_TOO_SMALL_CHOOSE_ANOTHER_IMAGE, // RDA; Image is too small. Choose another image
    SS_IMAGE_OR_VIDEO, // RDA; Image or video
    SS_IMAGE_QUALITY, // RDA; Image quality
    SS_IMAGE_RICH, // RDA; Image rich
    SS_IMAGE_SAVE_CANCEL, // RDA; Saving image cancelled
    SS_IMAGE_SIZE, // RDA; Image size
    SS_IMAGE_TOO_LARGE_RESIZE_Q, // RDA; Image too large. Resize?
    SS_IMAGE_TOO_LARGE_UNABLE_TO_SET_AS_CALLER_ID, // RDA; Image too large. Unable to set as caller ID
    SS_IMAP4, // RDA; IMAP4
    SS_IMAP4_INCOMING_OPTION, // RDA; IMAP4 incoming option
    SS_IMAP4_PASSWORD, // RDA; IMAP4 password
    SS_IMAP4_PORT, // RDA; IMAP4 port
    SS_IMAP4_SERVER, // RDA; IMAP4 server
    SS_IMAP4_USER_ID, // RDA; IMAP4 user ID
    SS_IMBABAH, // RDA; Imbaba
    SS_IMEI, // RDA; IMEI
    SS_IMEI_CORRUPT, // RDA; IMEI corrupt
    SS_IMMEDIATELY, // RDA; Immediately
    SS_IMPORTANT, // RDA; Important
    SS_IMPORTANT_CONTACTS, // RDA; Important contacts
    SS_IMPORTANT_ONLY, // RDA; Important only
    SS_IMPOSSIBLE, // RDA; Impossible
    SS_IMSAAD, // RDA; Imsaad
    SS_IMSI_OUT_OF_RANGE, // RDA; IMSI out of range
    SS_IM_DRIVING, // RDA; I'm driving
    SS_IM_HAVING_A_MEAL, // RDA; I'm having a meal
    SS_IM_IN_CLASS, // RDA; I'm in class
    SS_IM_SORRY, // RDA; I'm sorry
    SS_IM_UC, // RDA; IM
    SS_INACTIVE, // RDA; Inactive
    SS_INBAND_INFORMATION, // RDA; Inband information
    SS_INBOX, // RDA; Inbox
    SS_INBOX_ABB, // RDA; Inbox
    SS_INCANDESCENT, // RDA; Incandescent
    SS_INCH, // RDA; inch
    SS_INCH2, // RDA; inch²
    SS_INCH3, // RDA; inch³
    SS_INCHES2_MCVT, // RDA; Inches²
    SS_INCHES3_MCVT, // RDA; Inches³
    SS_INCHES_MCVT, // RDA; Inches
    SS_INCLUDES, // RDA; Includes
    SS_INCLUDE_ATTACHMENTS, // RDA; Include attachments
    SS_INCLUDE_ORIGINAL_MESSAGE_TEXT_WHEN_REPLYING, // RDA; Include original message text when replying
    SS_INCOMING_BARRING, // RDA; Incoming barring
    SS_INCOMING_CALLS_AUTOMATIC_ANSWERING_HELP, // RDA; Incoming calls will be answered automatically. Automatic answering will operate only with headset connected. Automatic answering is not possible when ringtone is set to mute
    SS_INCOMING_CALLS_BARRED_WHEN_ROAMING, // RDA; Incoming calls barred when roaming
    SS_INCOMING_CALL_DEFLECTED, // RDA; Incoming call deflected
    SS_INCOMING_CALL_DIVERTED, // RDA; Incoming call diverted
    SS_INCOMING_CALL_REJECTED, // RDA; Incoming call rejected
    SS_INCOMING_EMAIL_OPTION, // RDA; Incoming email option
    SS_INCOMING_EMAIL_SERVER, // RDA; Incoming email server
    SS_INCOMING_TEXT_MESSAGE, // RDA; Incoming text message
    SS_INCOMING_WHILE_ABROAD, // RDA; Incoming while abroad
    SS_INCOMMING_CALLS, // RDA; Incoming calls
    SS_INCOMMING_WHEN_ROAMING, // RDA; Incoming when roaming
    SS_INCOMPATIBLE, // RDA; Incompatible
    SS_INCOMPATIBLE_MESSAGE, // RDA; Incompatible message
    SS_INCOMPLETE, // RDA; Incomplete
    SS_INCOMPLETE_MESSAGE, // RDA; Incomplete message
    SS_INCORRECT_ADDRESS, // RDA; Incorrect address
    SS_INCORRECT_JAR_SIZE, // RDA; Incorrect JAR size
    SS_INCORRECT_KEY_USAGE, // RDA; Incorrect key usage
    SS_INCORRECT_PASSWORD, // RDA; Incorrect password
    SS_INCORRECT_PIN, // RDA; Incorrect PIN
    SS_INCORRECT_PIN2, // RDA; Incorrect PIN2
    SS_INCORRECT_PIN_PLEASE_TRY_AGAIN, // RDA; Incorrect PIN. Try again?
    SS_INCORRECT_PUK, // RDA; Incorrect PUK
    SS_INCORRECT_PUK2, // RDA; Incorrect PUK2
    SS_INCORRECT_SIM, // RDA; Incorrect SIM
    SS_INCREASING_MELODY, // RDA; Increasing melody
    SS_INDEPENDENCE_DAY_SWA, // RDA; Independence day
    SS_INDETERMINATE, // RDA; Indeterminate
    SS_INDEX, // RDA; Index
    SS_INDEX_OUT_OF_RANGE, // RDA; Index out of range
    SS_INDIANA_EAST, // RDA; Eastern Indiana
    SS_INDIA_M_COUNTRYNAME, // RDA; India
    SS_INDICATOR, // RDA; Indicator
    SS_INDIVIDUAL_INSTRUMENTS, // RDA; Individual instruments
    SS_INFORMATION, // RDA; Information
    SS_INFORMATIONAL, // RDA; Informational
    SS_INFORMATION_Y_RESERVE_TELEFONICA, // RDA; Información y reservas
    SS_INFO_HOTLINE, // RDA; Info hotline
    SS_INFRARED, // RDA; Infrared
    SS_INITIALISING_ING, // RDA; Initialising...
    SS_INITIATE_VOICEMAIL_FIRST_TIME_MSG_3_TMO, // RDA; Select the kind of greeting you want:
    SS_INK_ERROR, // RDA; Ink error
    SS_INPUTMETHOD_ARABIC, // RDA; اب
    SS_INPUT_LANGUAGE, // RDA; Input language
    SS_INPUT_METHOD, // RDA; Input method
    SS_INPUT_MODE, // RDA; Input mode
    SS_INPUT_NEW_PASSWORD, // RDA; Input new password
    SS_INPUT_PASSWORD, // RDA; Input password
    SS_INPUT_VALUE, // RDA; Input value
    SS_INPUT_WORD, // RDA; Input word
    SS_INR, // RDA; INR
    SS_INSERT, // RDA; Insert
    SS_INSERT_CHIP, // RDA; Insert your Chip
    SS_INSERT_CLIP_ART, // RDA; Insert clip art
    SS_INSERT_EMOTICON, // RDA; Insert emoticon
    SS_INSERT_IMAGE, // RDA; Insert image
    SS_INSERT_SIM, // RDA; Insert SIM
    SS_INSERT_TEXT, // RDA; Insert text
    SS_INSET_DOWN, // RDA; Inset, down
    SS_INSET_DOWN_LEFT, // RDA; Inset, down left
    SS_INSET_DOWN_RIGHT, // RDA; Inset, down right
    SS_INSET_LEFT, // RDA; Inset, left
    SS_INSET_RIGHT, // RDA; Inset, right
    SS_INSET_UP, // RDA; Inset, up
    SS_INSET_UP_LEFT, // RDA; Inset, up left
    SS_INSET_UP_RIGHT, // RDA; Inset, up right
    SS_INSTALL, // RDA; Install
    SS_INSTALLATION_FAILED, // RDA; Installation failed
    SS_INSTALLED, // RDA; Installed
    SS_INSTALLING_CONFIGURATION_FILE_TRANSFER_VIA_BT_DISABLED, // RDA; If you install this configuration, file transfer via Bluetooth will be disabled
    SS_INSTALLING_CONFIGURATION_FILE_TRANSFER_VIA_BT_ENABLED, // RDA; If you install this configuration, file transfer via Bluetooth will be enabled
    SS_INSTALLING_ING, // RDA; Installing...
    SS_INSTANT_MESSENGER, // RDA; Instant messenger
    SS_INSTRUMENT_1, // RDA; Instrument 1
    SS_INSTRUMENT_2, // RDA; Instrument 2
    SS_INSUFFICIENT_MEMORY, // RDA; Insufficient memory
    SS_INTERCOM_TERMINAL, // RDA; Intercom terminal
    SS_INTERNAL_ERROR, // RDA; Internal error
    SS_INTERNAL_SERVER_ERROR, // RDA; Internal server error
    SS_INTERNATIONAL, // RDA; International
    SS_INTERNATIONAL_CALLS_EXCEPT_HOME, // RDA; International calls except home
    SS_INTERNATIONAL_EXCEPT_HOME, // RDA; International except home
    SS_INTERNET, // RDA; Internet
    SS_INTERNET_PROFILE, // RDA; Internet profile
    SS_INTERNET_SETTINGS, // RDA; Internet settings
    SS_INTERVAL, // RDA; Interval
    SS_INVAILD_CHAR_NOT_ALLOWED, // RDA; * / : < > ? \ | not allowed
    SS_INVALID, // RDA; Invalid
    SS_INVALID_ACCESS_LEVEL, // RDA; Invalid access level
    SS_INVALID_ADDRESS, // RDA; Invalid address
    SS_INVALID_ALARMVALUE, // RDA; Invalid alarm\nvalue
    SS_INVALID_ALARM_VALUE, // RDA; Invalid alarm value
    SS_INVALID_CERTIFICATE_CONTINUE_Q, // RDA; Invalid certificate. Continue?
    SS_INVALID_DATA, // RDA; Invalid data
    SS_INVALID_DATE, // RDA; Invalid date
    SS_INVALID_DESCRIPTOR, // RDA; Invalid descriptor
    SS_INVALID_EMAIL_ADDRESS, // RDA; Invalid email address
    SS_INVALID_ENTRY_VALUE, // RDA; Invalid entry value
    SS_INVALID_FILE_FORMAT, // RDA; Invalid file format
    SS_INVALID_INPUT, // RDA; Invalid Input
    SS_INVALID_JAD_DESCRIPTOR, // RDA; Invalid JAD descriptor
    SS_INVALID_KEY, // RDA; Invalid key
    SS_INVALID_MEDIUM, // RDA; Invalid medium
    SS_INVALID_MESSAGE, // RDA; Invalid message
    SS_INVALID_PASSWORD, // RDA; Invalid password
    SS_INVALID_SEARCH_PASSWORD, // RDA; Invalid password
    SS_INVALID_SERVER_RESPONSE, // RDA; Invalid server response
    SS_INVALID_TIME, // RDA; Invalid time
    SS_INVALID_TIMER_FOR_CALL_DIVERTING, // RDA; Invalid timer for call diverting
    SS_INVALID_TIME_ENTER_NUMBER_BETWEEN_1_AND_60, // RDA; Invalid time. Enter number between 1 to 60
    SS_INVALID_TIME_LA, // RDA; Invalid time
    SS_INVALID_USER_OR_PASSWORD, // RDA; Invalid user or password
    SS_INVALIED_FILE_NAME, // RDA; Invalid file name
    SS_IN_1_DAY, // RDA; in 1 day
    SS_IN_DUAL_STANDBY_MODE_NETWORKHELP_MSG, // RDA; In Dual standby mode, you can change your current network by pressing the network switch key. Indicator displays current network selected for calls and text messages.
    SS_IN_LOVE, // RDA; In love
    SS_IN_MINCH, // RDA; in
    SS_IN_MULTIPARTY, // RDA; In multiparty
    SS_IN_MZOOM, // RDA; In
    SS_IN_PRAYER_ALARM_TYPE, // RDA; In-prayer alarm type
    SS_IN_TRIPLE_SIM_STANDBY_MODE_YOU_CAN_SET_THE_SIM_CARD_PREFERENCES_FOR_CALLS_AND_SMS_MESSAGES_NOTI_MSG, // RDA; In Triple SIM standby mode, you can set the SIM card preferences for calls and SMS messages. The preferred SIM cards will be used for the functions above. You can change these preferences using the network switch key or under SIM card manager
    SS_IP_ADDRESS, // RDA; IP address
    SS_IP_ADDRESS_PROBLEM, // RDA; Problem with IP address
    SS_IP_PORT, // RDA; IP port
    SS_IP_SETTING, // RDA; IP setting
    SS_IRAN_M_COUNTRYNAME, // RDA; Iran
    SS_IRAQ, // RDA; Iraq
    SS_IRBID, // RDA; Irbid
    SS_IRDA, // RDA; IrDA
    SS_IRDA_ACTIVATED, // RDA; IrDA activated
    SS_IRDA_CANCELLED, // RDA; IrDA cancelled
    SS_IRDA_DEACTIVATED, // RDA; IrDA deactivated
    SS_IRDA_NOW_IN_USE, // RDA; IrDA now in use
    SS_IRIAN_JAYA, // RDA; Irian Jaya
    SS_IRKUTSK, // RDA; Irkutsk
    SS_ISDN, // RDA; ISDN
    SS_ISFAHAN, // RDA; Isfahan
    SS_ISHAA, // RDA; Isha'a
    SS_ISKANDARIYAH, // RDA; Iskandariya
    SS_ISLAMABAD, // RDA; Islamabad
    SS_ISLAMIC_SALAT, // RDA; Islamic salat
    SS_ISTANBUL, // RDA; Istanbul
    SS_ITALIC, // RDA; Italic
    SS_ITEM, // RDA; Item
    SS_ITEM_1, // RDA; Item 1
    SS_ITEM_WILL_REVERT_RING_MSGTONE_Q, // RDA; Deleting item will revert ringtone and message tone to default. Delete?
    SS_IZKI, // RDA; Izki
    SS_IZMIR, // RDA; Izmir
    SS_IZMIT, // RDA; Izmit
    SS_I_AM_ANGRY, // RDA; I am angry
    SS_I_AM_CONFUSED, // RDA; I am confused
    SS_I_AM_CRYING, // RDA; I am crying
    SS_I_AM_GLAD, // RDA; I am glad
    SS_I_AM_INDIFFERENT, // RDA; I am indifferent
    SS_I_AM_IN_EMERGENCY_PLEASE_HELP_ME, // RDA; I am in an emergency. Please help me.
    SS_I_AM_IRONIC, // RDA; I am ironic
    SS_I_AM_LAUGHING, // RDA; I am laughing
    SS_I_AM_SAD, // RDA; I am sad
    SS_I_AM_SCEPTIC, // RDA; I am sceptic
    SS_I_AM_WINKING, // RDA; I am winking
    SS_I_LOVE_YOU_E, // RDA; I love you!
    SS_JABALPUR, // RDA; Jabalpur
    SS_JABAL_UMM_AL_RUUS, // RDA; Jabal Umm al Ru'us
    SS_JADU, // RDA; Jadu
    SS_JAD_AND_MANIFEST_ATTRIBUTES_MISMATCH, // RDA; JAD and Manifest attributes mismatch
    SS_JAHRA, // RDA; Al Jahra
    SS_JAIPUR, // RDA; Jaipur
    SS_JAKARTA, // RDA; Jakarta
    SS_JALAJIL, // RDA; Jalajil
    SS_JALALABAD, // RDA; Jalalabad
    SS_JAMNAH, // RDA; Jamnah
    SS_JAN, // RDA; Jan.
    SS_JANUARY, // RDA; January
    SS_JAR_FILE_IS_OVER_MAX_SIZE, // RDA; JAR file is over max size
    SS_JAR_NOT_FOUND, // RDA; JAR not found
    SS_JAVASCRIPT_ALERT, // RDA; JavaScript - Alert
    SS_JAVASCRIPT_CONFIRM, // RDA; JavaScript - Confirm
    SS_JAVASCRIPT_PROMPT, // RDA; JavaScript - Prompt
    SS_JAVA_APPLICATIONS, // RDA; Java applications
    SS_JAVA_ERROR, // RDA; Java error
    SS_JAVA_ERROR_NO_MEMORY_AVAILABLE, // RDA; Java error.\nNo memory available
    SS_JAVA_POP_EXITQ, // RDA; Do you want to finish?
    SS_JAZZ, // RDA; Jazz
    SS_JEBEL_ALI, // RDA; Jebel Ali
    SS_JEDDAH, // RDA; Jeddah
    SS_JEMMAL, // RDA; Jemmal
    SS_JERASH, // RDA; Jerash
    SS_JERMA, // RDA; Germa
    SS_JERUSALEM, // RDA; Jerusalem
    SS_JHANG, // RDA; Jhang
    SS_JIDD_HAFS, // RDA; Jidhafs
    SS_JIJEL, // RDA; Jijel
    SS_JIZAN, // RDA; Jizan
    SS_JIZAN_ECONOMIC_CITY, // RDA; Jazan Economic City
    SS_JLEEB_AL_SHUYOUKH, // RDA; Jleeb Al-Shuyoukh
    SS_JOB_TITLE, // RDA; Job title
    SS_JOINED, // RDA; Joined
    SS_JORDAN, // RDA; Jordan
    SS_JPEG, // RDA; JPEG
    SS_JPY, // RDA; JPY
    SS_JUBAIL, // RDA; Jubail
    SS_JUL, // RDA; Jul.
    SS_JULY, // RDA; July
    SS_JUMADA_H1_MEA, // RDA; Jumada (1)
    SS_JUMADA_H2_MEA, // RDA; Jumada (2)
    SS_JUN, // RDA; Jun.
    SS_JUNE, // RDA; June
    SS_KABUL, // RDA; Kabul
    SS_KAFR_EL_DAWWAR, // RDA; Kafr el-Dawwar
    SS_KALBA, // RDA; Kalba
    SS_KALININGRAD, // RDA; Kaliningrad
    SS_KANDAHAR, // RDA; Kandahar
    SS_KANNADA_INPUT, // RDA; ಕಖಗ
    SS_KANO, // RDA; Kano
    SS_KANPUR, // RDA; Kanpur
    SS_KARACHI, // RDA; Karachi
    SS_KARAMEH, // RDA; Karameh
    SS_KARBALA, // RDA; Karbala
    SS_KATHMANDU, // RDA; Kathmandu
    SS_KAWM_UMBU, // RDA; Kawm Umbu
    SS_KB, // RDA; KB
    SS_KCAL, // RDA; Kcal
    SS_KEEP_APPLICATION_DATA_Q, // RDA; Keep application data?
    SS_KEEP_A_COPY, // RDA; Keep a copy
    SS_KEEP_IN_SERVER, // RDA; Keep in server
    SS_KEEP_RADIO_ON, // RDA; Keep radio on?
    SS_KEEP_RADIO_ON_Q, // RDA; Keep radio on?
    SS_KEEP_THE_CABLE_CONNECTED_TO_PRINTER, // RDA; Keep the cable connected to printer
    SS_KEEP_THIS_MESSAGE, // RDA; Keep this message
    SS_KENITRA, // RDA; Kenitra
    SS_KERKENNAH, // RDA; Kerkennah
    SS_KERKOUANE, // RDA; Kerkouane
    SS_KERMAN, // RDA; Kerman
    SS_KEY, // RDA; Key
    SS_KEYPAD_LIGHT, // RDA; Keypad light
    SS_KEYPAD_LOCKED, // RDA; Keypad locked
    SS_KEYPAD_LOCKED_LONG_PRESS_STAR, // RDA; Keypad is locked. Long press the * key
    SS_KEYPAD_LOCKED_PRESS_PWR_KEY, // RDA; Keypad is locked. Press the power key
    SS_KEYPAD_TONE, // RDA; Keypad tone
    SS_KEYPAD_UNLOCKED, // RDA; Keypad unlocked
    SS_KEYPAD_WILL_BE_LOCKED_WHEN_LCD_IS_OFF, // RDA; Keypad will be locked when LCD is off
    SS_KEYPAD_WILL_NOT_BE_AUTOMATICALLY_LOCKED_WHEN_LCD_IS_OFF, // RDA; Keypad will not be locked when LCD is off
    SS_KEYS_TO_BUY, // RDA; Buy key
    SS_KEY_CONTACT_1, // RDA; Key contact 1
    SS_KEY_CONTACT_2, // RDA; Key contact 2
    SS_KEY_CONTACT_3, // RDA; Key contact 3
    SS_KEY_CONTACT_LIST, // RDA; Key Contact list
    SS_KEY_INDEX, // RDA; Key index
    SS_KEY_MANAGEMENT, // RDA; Key management
    SS_KEY_VOLUME, // RDA; Key volume
    SS_KG, // RDA; kg
    SS_KHABAROVSK, // RDA; Khabarovsk
    SS_KHAFJI, // RDA; Khafji
    SS_KHAMIS_MUSHAYT, // RDA; Khamis Mushayt
    SS_KHANAQIN, // RDA; Khanaqin
    SS_KHARTOUM, // RDA; Khartoum
    SS_KHASAB, // RDA; Khasab
    SS_KHAYBAR, // RDA; Khaybar
    SS_KHEMISSET, // RDA; Khemisset
    SS_KHENCHELA, // RDA; Khenchela
    SS_KHNISS, // RDA; Khniss
    SS_KHOBAR, // RDA; Khobar
    SS_KHORDAD_MEA, // RDA; Khordad
    SS_KHOR_FAKKAN, // RDA; Khor Fakkan
    SS_KHOURIBGA, // RDA; Khouribga
    SS_KIEV, // RDA; Kiev
    SS_KIRKUK, // RDA; Kirkuk
    SS_KISS, // RDA; Kiss
    SS_KM, // RDA; km
    SS_KOLKATA, // RDA; Kolkata
    SS_KONYA, // RDA; Konya
    SS_KORBA, // RDA; Korba
    SS_KRANSNOYARSK, // RDA; Kransnoyarsk
    SS_KRASNOYARSK, // RDA; Krasnoyarsk
    SS_KRW, // RDA; KRW
    SS_KUALA_LUMPUR, // RDA; Kuala Lumpur
    SS_KUNDUZ, // RDA; Kunduz
    SS_KUT, // RDA; Al Kut
    SS_KUWAIT, // RDA; Kuwait
    SS_KUWAIT_CITY, // RDA; Kuwait City
    SS_LADYBIRD, // RDA; Ladybird
    SS_LAGHOUAT, // RDA; Laghouat
    SS_LAGOS, // RDA; Lagos
    SS_LAHIJ, // RDA; Lahij
    SS_LAHORE, // RDA; Lahore
    SS_LAMTA, // RDA; Lamta
    SS_LANDLINE_BUSINESS, // RDA; Landline, business
    SS_LANDLINE_PRIVATE, // RDA; Landline, private
    SS_LANGUAGE, // RDA; Language
    SS_LANGUAGES, // RDA; Languages
    SS_LANGUAGE_RESET, // RDA; Language Reset
    SS_LANGUAGE_SELECT, // RDA; Language select
    SS_LANG_DUTCH, // RDA; Nederlands
    SS_LANG_ENGLISH, // RDA; English
    SS_LANG_ENGLISH_LATIN, // RDA; English
    SS_LANG_FRENCH, // RDA; Français
    SS_LANG_GERMAN, // RDA; Deutsch
    SS_LANG_ITALIAN, // RDA; Italiano
    SS_LANG_PORTUGUESE, // RDA; Português
    SS_LANG_SPANISH, // RDA; Español
    SS_LAN_ACCESS, // RDA; LAN access
    SS_LAP, // RDA; Lap
    SS_LARGE, // RDA; Large
    SS_LARGE_MTEXTSIZE, // RDA; Large
    SS_LARKANA, // RDA; Larkana
    SS_LAST_CALL, // RDA; Last call
    SS_LAST_CALL_COST, // RDA; Last call cost
    SS_LAST_CALL_TIME, // RDA; Last call time
    SS_LAST_MODIFIED_TIME, // RDA; Last modified time
    SS_LAST_NAME, // RDA; Last name
    SS_LAST_NAME_FIRST_NAME, // RDA; Last name First name
    SS_LAST_PAGE, // RDA; Last page
    SS_LAST_PLAYED, // RDA; Last played
    SS_LAST_RESET_ON_M_DATE, // RDA; Last reset on
    SS_LAST_SAVED_AT_PDPDPD_PD_C_PD, // RDA; Last saved at %d/%d/%d-%d:%d
    SS_LAST_WEEK, // RDA; Last week
    SS_LATAKIA, // RDA; Latakia
    SS_LATEST_STEP_COUNT, // RDA; Latest step count
    SS_LAUNCH, // RDA; Launch
    SS_LAUNCH_BROWSER_Q, // RDA; Launch Browser?
    SS_LAUNCH_CALL_Q, // RDA; Launch call?
    SS_LAUNCH_Q, // RDA; Launch?
    SS_LAVENDER_BLUE, // RDA; Lavender blue
    SS_LAYLA, // RDA; Layla
    SS_LA_GOULETTE, // RDA; La Goulette
    SS_LA_MARSA, // RDA; La Marsa
    SS_LA_PAZ, // RDA; La Paz
    SS_LCD_BRIGHTNESS, // RDA; LCD brightness
    SS_LCD_CONTRAST, // RDA; LCD contrast
    SS_LEBANON, // RDA; Lebanon
    SS_LEFT, // RDA; Left
    SS_LEFT_AND_RIGHT_NAVIGATION_SHORTCUTS_CANNOT_BE_USED_WHILE_SHORTCUTS_TOOLBAR_IS_ACTIVE, // RDA; Left and right Navigation shortcuts cannot be used while Shortcuts toolbar is active
    SS_LEFT_M_LOCATION, // RDA; Left
    SS_LEMON, // RDA; Lemon
    SS_LENGTH, // RDA; Length
    SS_LENGTH_MCVT, // RDA; Length
    SS_LENGTH_MTIME, // RDA; Length
    SS_LETTER, // RDA; Letter
    SS_LEVEL, // RDA; Level
    SS_LE_BARDO, // RDA; Le Bardo
    SS_LIBYA_M_COUNTRYNAME, // RDA; Libya
    SS_LICENCES, // RDA; Licences
    SS_LICENCES_ACQUIRED_AGAIN_TO_USE_CONTENTS_DELETE_Q, // RDA; Licences must be acquired again if you want to use contents that requires them. Delete licences?
    SS_LIGHT, // RDA; Light
    SS_LIGHTING, // RDA; Lighting
    SS_LIGHT_OFF, // RDA; Light off
    SS_LIGHT_ON, // RDA; Light on
    SS_LIGHT_ONLY, // RDA; Light only
    SS_LIMA_PERU, // RDA; Lima
    SS_LIME_GREEN, // RDA; Lime green
    SS_LIMITED_CALLS, // RDA; Limited calls
    SS_LIMITED_SERVICE, // RDA; Limited service
    SS_LIMIT_FOR_EMAILS, // RDA; Limit for emails
    SS_LIMIT_FOR_MMS, // RDA; Limit for MMS
    SS_LIMIT_FOR_MULTIMEDIA_MESSAGES, // RDA; Limit for multimedia messages
    SS_LINE, // RDA; Line
    SS_LINE_1, // RDA; Line 1
    SS_LINE_2, // RDA; Line 2
    SS_LINE_LOCK, // RDA; Line lock
    SS_LINE_SELECTION, // RDA; Line selection
    SS_LINE_TYPE, // RDA; Line type
    SS_LINE_VIEW, // RDA; Line view
    SS_LINGER_TIME_HSEC, // RDA; Linger time (sec)
    SS_LINGVO_DICTIONARY, // RDA; Lingvo Dictionary
    SS_LINK, // RDA; Link
    SS_LINK_LOSS_PS, // RDA; Link loss %s
    SS_LIPS, // RDA; Lips
    SS_LIPSTICK, // RDA; Lipstick
    SS_LISBON, // RDA; Lisbon
    SS_LIST, // RDA; List
    SS_LISTEN, // RDA; Listen
    SS_LIST_BY, // RDA; List by
    SS_LIST_EMPTY, // RDA; List empty
    SS_LIST_FONT, // RDA; List font
    SS_LIST_FULL, // RDA; List full
    SS_LIST_INITIALISING_ING, // RDA; List initialising...
    SS_LIST_MENU, // RDA; List menu
    SS_LIST_MNOUN, // RDA; List
    SS_LITRE, // RDA; litre
    SS_LITRES_MCVT, // RDA; Litres
    SS_LIVE, // RDA; Live
    SS_LIVE_E_STUDIO, // RDA; live! Studio
    SS_LOAD, // RDA; Load
    SS_LOADING_ING, // RDA; Loading...
    SS_LOADING_MYFAVES_ING, // RDA; Loading MyFaves...
    SS_LOAD_AN_IMAGE, // RDA; Load an image
    SS_LOCATION, // RDA; Location
    SS_LOCATION_MCALENDAR, // RDA; Location
    SS_LOCATION_MPATH, // RDA; Location
    SS_LOCATION_MSAVETO, // RDA; Location
    SS_LOCK, // RDA; Lock
    SS_LOCKED, // RDA; Locked
    SS_LOCKED_ITEMS, // RDA; Locked items
    SS_LOCKED_SLIDE_HOLD_KEY_TO_UNLOCK, // RDA; Locked. Slide the hold key to unlock
    SS_LOCK_UNLOCK, // RDA; Lock/Unlock
    SS_LOGIN_ID, // RDA; Login ID
    SS_LOGS, // RDA; Logs
    SS_LOG_ALL_CALLS, // RDA; All calls
    SS_LOG_CREATE_CONTACT, // RDA; Create contact
    SS_LOG_MANAGER, // RDA; Log manager
    SS_LOG_UPDATE_EXISTING, // RDA; Update existing
    SS_LONDON, // RDA; London
    SS_LONDON_UK, // RDA; London
    SS_LONG_AND_CURLY, // RDA; Long and curly
    SS_LONG_AND_FULL, // RDA; Long and full
    SS_LONG_HAIR, // RDA; Long hair
    SS_LOS_ANGELES, // RDA; Los Angeles
    SS_LOTTERY, // RDA; Lottery
    SS_LOUDSPEAKER_SK_LITE, // RDA; Loudspk
    SS_LOVE, // RDA; Love
    SS_LOVEANGEL, // RDA; Loveangel
    SS_LOVER, // RDA; Lover
    SS_LOVE_LETTER, // RDA; Love letter
    SS_LOW, // RDA; Low
    SS_LOWEST_NUMBER, // RDA; Lowest number
    SS_LOW_BATTERY, // RDA; Low battery
    SS_LOW_MEMAILPRIORITY, // RDA; Low
    SS_LOW_MPRIORITY, // RDA; Low
    SS_LOW_MQUALITY, // RDA; Low
    SS_LOW_MSPEED, // RDA; Low
    SS_LOW_M_POWER, // RDA; Low
    SS_LOW_T_MSG, // RDA; Low
    SS_LUNAR_GLOW, // RDA; Lunar Glow
    SS_LUXEMBOURG, // RDA; Luxembourg
    SS_LUXOR, // RDA; Luxor
    SS_L_MPRINT_PAPERSIZE, // RDA; L
    SS_L_MUNIT, // RDA; ℓ
    SS_M, // RDA; m
    SS_M2, // RDA; m²
    SS_M3, // RDA; m³
    SS_MAAN, // RDA; Ma'an 
    SS_MADABA, // RDA; Madaba
    SS_MADHA, // RDA; Madha
    SS_MADINAT_HAMAD, // RDA; Hamad Town
    SS_MADINAT_ISA, // RDA; Isa Town
    SS_MADRID, // RDA; Madrid
    SS_MADRID_SPAIN, // RDA; Madrid
    SS_MAGADAN, // RDA; Magadan
    SS_MAGHRIB, // RDA; Maghrib
    SS_MAGIC_PEN, // RDA; Magic pen
    SS_MAGNOLIA_MTHEME, // RDA; Magnolia
    SS_MAHASHIVRATRI_SWA, // RDA; Mahashivratri
    SS_MAIL, // RDA; Mail
    SS_MAILBOX, // RDA; Mailbox
    SS_MAILBOX_NAME, // RDA; Mailbox name
    SS_MAILBOX_SETUP_WIZARD, // RDA; Mailbox setup wizard
    SS_MAILBOX_TYPE, // RDA; Mailbox type
    SS_MAIL_BOX_FULL_DELETE_SOME_EMAIL, // RDA; Mailbox full. Delete some email
    SS_MAIL_LC, // RDA; mail
    SS_MAINMENU_FACEBOOK, // RDA; Facebook
    SS_MAINMENU_STYLE, // RDA; Main menu style
    SS_MAINMENU_TWITTER, // RDA; Twitter
    SS_MAIN_DISPLAY, // RDA; Main display
    SS_MAIN_LCD, // RDA; Main LCD
    SS_MAIN_MENU, // RDA; Main menu
    SS_MAIN_MENU_ICON, // RDA; Main menu icon
    SS_MAIN_MENU_TYPE, // RDA; Main menu type
    SS_MAIN_STK, // RDA; SIM services
    SS_MAKARASANKRANTI_SWA, // RDA; Makarasankranti
    SS_MAKE_CALL, // RDA; Call
    SS_MAKE_SURE_THE_EARPHONE_BE_CONNECTED, // RDA; Make sure the earphone be connected at alarm time. Otherwise, unable to use FM radio as alarm tone.
    SS_MAKTHAR, // RDA; Makthar
    SS_MALAYALAM_INPUT, // RDA; കഖഗ
    SS_MANAGEMENT, // RDA; Management
    SS_MANAGE_CONTACTS, // RDA; Manage contacts
    SS_MANAH, // RDA; Manah
    SS_MANAMA, // RDA; Manama
    SS_MANGAF, // RDA; Mangaf
    SS_MANILA, // RDA; Manila
    SS_MANISA, // RDA; Manisa
    SS_MANUAL, // RDA; Manual
    SS_MANUAL_ATTACH, // RDA; Manual attach
    SS_MANUAL_ONLY, // RDA; Manual only
    SS_MANUAL_PAIRING_INFO, // RDA; Press the WPS key at AP, unless you didn't. Pairing...
    SS_MANUAL_TUNING, // RDA; Manual tuning
    SS_MAP_ON, // RDA; Map on
    SS_MAR, // RDA; Mar.
    SS_MARATHI_INPUT, // RDA; कखग
    SS_MARCH, // RDA; March
    SS_MARDAN, // RDA; Mardan
    SS_MARIB, // RDA; Marib
    SS_MARK, // RDA; Mark
    SS_MARKED, // RDA; Marked
    SS_MARK_AS_DEFAULT_GROUP, // RDA; Mark as default group
    SS_MARK_AS_DEFAULT_NUMBER, // RDA; Mark as default number
    SS_MARK_AS_READ, // RDA; Mark as read
    SS_MARK_AS_SECOND_CLOCK, // RDA; Mark as second clock
    SS_MARRAKECH, // RDA; Marrakech
    SS_MARSA_AL_BURAYQAH, // RDA; Marsa al Burayqah
    SS_MARSA_MATRUH, // RDA; Mersa Matru
    SS_MASHHAD, // RDA; Mashhad
    SS_MASIRAH, // RDA; Masirah
    SS_MASS_STORAGE, // RDA; Mass storage
    SS_MASS_STORAGE_MODE_ENABLED, // RDA; Mass storage mode enabled
    SS_MASTER_RESET, // RDA; Master reset
    SS_MASTOORAH, // RDA; Mastoorah
    SS_MATCH_CASE, // RDA; Match case
    SS_MATCH_CRITERIA, // RDA; Match criteria
    SS_MATCH_PASSKEYS_ON_PS_Q, // RDA; Match passkeys on %s?
    SS_MATCH_WORD, // RDA; Match word
    SS_MATEUR, // RDA; Mateur
    SS_MATMATA, // RDA; Matmata
    SS_MATRAH, // RDA; Muttrah
    SS_MATRIX, // RDA; Matrix
    SS_MAX, // RDA; Max.
    SS_MAXIMUM, // RDA; Maximum
    SS_MAXIMUM_15_DIGITS, // RDA; Maximum 15 digits
    SS_MAXIMUM_2_CONCURRENT_CALLS_ALLOWED, // RDA; Maximum 2 concurrent calls allowed
    SS_MAXIMUM_40_DIGITS_SAVED, // RDA; Maximum 40 digits saved
    SS_MAXIMUM_CAP, // RDA; Maximum
    SS_MAXIMUM_CONTENT_SIZE_IS_100KB, // RDA; Maximum content size is 100KB
    SS_MAXIMUM_FILE_NUMBER_EXCEEDED, // RDA; Maximum file number exceeded
    SS_MAXIMUM_MESSAGE_SIZE_EXCEEDED, // RDA; Maximum message size exceeded
    SS_MAXIMUM_NUMBER_OF_DURATION_REACHED, // RDA; Maximum number of duration reached
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED, // RDA; Maximum number of email reached
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED_DELETE_EMAILS_OLDER_THAN_1_DAY_Q, // RDA; Maximum number of email reached. Delete emails older than 1 day?
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED_DELETE_EMAILS_OLDER_THAN_1_MONTH_Q, // RDA; Maximum number of email reached. Delete emails older than 1 month?
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED_DELETE_EMAILS_OLDER_THAN_1_WEEK_Q, // RDA; Maximum number of email reached. Delete emails older than 1 week?
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED_DELETE_EMAILS_OLDER_THAN_3_DAYS_Q, // RDA; Maximum number of email reached. Delete emails older than 3 days?
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED_DELETE_EMAILS_OLDER_THAN_3_WEEKS_Q, // RDA; Maximum number of email reached. Delete emails older than 3 weeks?
    SS_MAXIMUM_NUMBER_OF_EMAIL_REACHED_DELETE_EMAILS_OLDER_THAN_CUSTOM_TIME_Q, // RDA; Maximum number of email reached. Delete emails older than the custom time?
    SS_MAXIMUM_NUMBER_OF_ENTRIES_EXCEEDED, // RDA; Maximum number of entries exceeded
    SS_MAXIMUM_NUMBER_OF_FILES_REACHED, // RDA; Maximum number of files reached
    SS_MAXIMUM_NUMBER_OF_GROUPS_REACHED, // RDA; Maximum number of groups reached
    SS_MAXIMUM_NUMBER_OF_LETTERS_REACHED, // RDA; Maximum number of letters reached
    SS_MAXIMUM_NUMBER_OF_LINES_IS_3, // RDA; Maximum number of lines is 3
    SS_MAXIMUM_NUMBER_OF_MULTIPARTIES_EXCEEDED, // RDA; Maximum number of multiparties exceeded
    SS_MAXIMUM_NUMBER_OF_PAGES_IS_PD_DELETE_SOME_PAGES_TRY_AGAIN, // RDA; Maximum number of pages is %d. Delete some pages. Try again
    SS_MAXIMUM_NUMBER_OF_RECIPIENTS_REACHED, // RDA; Maximum number of recipients reached
    SS_MAXIMUM_NUMBER_OF_THEMES_REACHED, // RDA; Maximum number of themes reached
    SS_MAXIMUM_NUMBER_OF_TRACKS_REACHED, // RDA; Maximum number of tracks reached
    SS_MAXIMUM_TEMPLATE_SIZE_REACHED_SAVE_Q, // RDA; Maximum template size reached. Save?
    SS_MAX_COST, // RDA; Max. cost
    SS_MAX_MESSAGE_SIZE_EXCEEDED, // RDA; Maximum message size exceeded
    SS_MAX_MESSAGE_SIZE_EXCEEDED_RESIZE_Q, // RDA; Maximum message size exceeded. Resize?
    SS_MAX_MESSAGE_SIZE_EXCEEDED_RESIZING_ING, // RDA; Max. message size exceeded. Resizing...
    SS_MAX_NUM_EMAIL_REACHED_DELETE, // RDA; Maximum number of email reached. Delete some emails permanently
    SS_MAY, // RDA; May
    SS_MAY_ABB, // RDA; May
    SS_MAY_DAY_SWA, // RDA; May day
    SS_MAZAR_E_SHARIF, // RDA; Mazar-i-Sharif
    SS_MD5, // RDA; MD5
    SS_MEASURING_WI_FI_LINK_QUALITY_ING, // RDA; Measuring Wi-Fi link quality...
    SS_MECCA, // RDA; Makkah
    SS_MEDAN, // RDA; Medan
    SS_MEDEA, // RDA; Medea
    SS_MEDIA_ALBUM, // RDA; Media album
    SS_MEDIA_INFO, // RDA; Media info
    SS_MEDIA_NOT_DISPLAY_TO_RECIPIENTS_CONTINUE_Q, // RDA; This media may not be correctly displayed to recipients. Continue?
    SS_MEDIA_PLAYER, // RDA; Media player
    SS_MEDIA_PLAYER_MODE_ENABLED, // RDA; Media player mode enabled
    SS_MEDIA_PLAYER_SETTINGS, // RDA; Media player settings
    SS_MEDIA_RECORDING, // RDA; Media recording
    SS_MEDIA_TIME_SHOULD_BE_SHORTER_THAN_PAGE_DURATION, // RDA; Media time should be shorter than page duration
    SS_MEDIA_TRANSFER, // RDA; Media transfer
    SS_MEDINA, // RDA; Medina
    SS_MEDIUM, // RDA; Medium
    SS_MEDIUM_M_FONTSIZE, // RDA; Medium
    SS_MEDJEZ_EL_BAB, // RDA; Majaz al Bab
    SS_MEET, // RDA; Meet
    SS_MEETING, // RDA; Meeting
    SS_MEGAPIXEL, // RDA; Megapixel
    SS_MEHR_MEA, // RDA; Mehr
    SS_MEKNES, // RDA; Meknes
    SS_MELBOURNE, // RDA; Melbourne
    SS_MELODY, // RDA; Melody
    SS_MELODY_AND_VIBRATION, // RDA; Melody and vibration
    SS_MELODY_THEN_VIBRATION, // RDA; Melody then vibration
    SS_MEMO, // RDA; Memo
    SS_MEMORY, // RDA; Memory
    SS_MEMORY_CARD, // RDA; Memory card
    SS_MEMORY_CARD_DETAILS, // RDA; Memory card details
    SS_MEMORY_CARD_ERROR, // RDA; Memory Card Error
    SS_MEMORY_CARD_INSERTED, // RDA; Memory card inserted
    SS_MEMORY_CARD_LIST_FULL_DELETE_SOME_ITEMS, // RDA; Memory card list full. Delete some items
    SS_MEMORY_CARD_NOT_SUPPORTED_MEMORY_CARD_SIZE_TOO_LARGE, // RDA; Memory card not supported. Memory card size too large
    SS_MEMORY_CARD_REMOVED, // RDA; Memory card removed
    SS_MEMORY_CARD_SETTINGS, // RDA; Memory card settings
    SS_MEMORY_CARD_UNRECOGNISED_FORMAT_NEEDED, // RDA; Memory card unrecognised. Format needed
    SS_MEMORY_FULL, // RDA; Memory full
    SS_MEMORY_FULL_DELETE_SOME_GAMES, // RDA; Memory full. Delete games?
    SS_MEMORY_FULL_DELETE_SOME_ITEMS, // RDA; Memory full. Delete some items
    SS_MEMORY_FULL_DELETE_SOME_ITEMS_MAXIMUM_NUMBER_OF_MMS_IS_150, // RDA; Memory full. Delete some items. Maximum number of MMS is 150
    SS_MEMORY_FULL_DELETE_SOME_ITEMS_Q, // RDA; Memory full, Delete some items?
    SS_MEMORY_FULL_UNABLE_TO_RECEIVE_MMS, // RDA; Memory full. Unable to receive MMS
    SS_MEMORY_SETTINGS, // RDA; Memory settings
    SS_MEMORY_STATUS, // RDA; Memory status
    SS_MEMO_DELETED, // RDA; Memo deleted
    SS_MEMO_FULL, // RDA; Memo full
    SS_MEMO_LIST_FULL_DELETE_ITEMS, // RDA; Memo list full. Delete some items
    SS_MEMO_MMAINMENU, // RDA; Memo
    SS_MEMO_RECEIVED, // RDA; Memo received
    SS_MENU_SK, // RDA; Menu
    SS_MENU_TRANSITION_EFFECT, // RDA; Menu transition effect
    SS_MENZEL_BOURGUIBA, // RDA; Menzel Bourguiba
    SS_MENZEL_TEMIME, // RDA; Menzel Temime
    SS_MERGE, // RDA; Merge
    SS_MERRY_CHRISTMAS_E, // RDA; Merry Christmas!
    SS_MERSIN, // RDA; Mersin
    SS_MESAIEED, // RDA; Mesaieed
    SS_MESSAAD, // RDA; Messaad
    SS_MESSAGE, // RDA; Message
    SS_MESSAGES, // RDA; Messages
    SS_MESSAGES_MMAINMENU, // RDA; Messages
    SS_MESSAGE_1, // RDA; Message 1
    SS_MESSAGE_2, // RDA; Message 2
    SS_MESSAGE_3, // RDA; Message 3
    SS_MESSAGE_4, // RDA; Message 4
    SS_MESSAGE_ALERT, // RDA; Message alert
    SS_MESSAGE_ALERT_TONE, // RDA; Message alert tone
    SS_MESSAGE_ALERT_TYPE, // RDA; Message alert type
    SS_MESSAGE_ALERT_VIBRATION, // RDA; Message alert vibration
    SS_MESSAGE_ALREADY_RETRIEVED, // RDA; Message already retrieved
    SS_MESSAGE_BOX, // RDA; Message box
    SS_MESSAGE_CHANGE_TO_MMS, // RDA; Message changed to multimedia message
    SS_MESSAGE_CLASS, // RDA; Message class
    SS_MESSAGE_DEFERRED, // RDA; Message deferred
    SS_MESSAGE_DETAILS, // RDA; Message details
    SS_MESSAGE_DOWNLOAD, // RDA; Message download
    SS_MESSAGE_DOWNLOADS, // RDA; Message downloads
    SS_MESSAGE_EXPIRED, // RDA; Message expired
    SS_MESSAGE_FORMAT_CORRUPT, // RDA; Message format corrupt
    SS_MESSAGE_MOVED, // RDA; Message moved
    SS_MESSAGE_NEEDS_A_REPLY, // RDA; Message needs a reply
    SS_MESSAGE_NOTIFICATION, // RDA; Message notification
    SS_MESSAGE_NOT_FOUND, // RDA; Message not found
    SS_MESSAGE_RECEIVED, // RDA; Message received
    SS_MESSAGE_REJECTED_P, // RDA; Message rejected
    SS_MESSAGE_REMOVED, // RDA; Message removed
    SS_MESSAGE_RETRIEVED, // RDA; Message retrieved
    SS_MESSAGE_SENT, // RDA; Message sent
    SS_MESSAGE_SETTINGS, // RDA; Message settings
    SS_MESSAGE_TONE, // RDA; Message tone
    SS_MESSAGE_TONE_1, // RDA; Message tone 1
    SS_MESSAGE_TONE_2, // RDA; Message tone 2
    SS_MESSAGE_TONE_3, // RDA; Message tone 3
    SS_MESSAGE_TONE_4, // RDA; Message tone 4
    SS_MESSAGE_TONE_5, // RDA; Message tone 5
    SS_MESSAGE_TONE_6, // RDA; Message tone 6
    SS_MESSAGE_TOO_LARGE, // RDA; Message too large
    SS_MESSAGE_TOO_LONG, // RDA; Message too long
    SS_MESSAGE_TOO_LONG_MESSAGE_WILL_BE_SENT_AS_PS_MESSAGES_CONTINUE_Q, // RDA; Message too long. Message will be sent as %s messages. Continue?
    SS_MESSAGE_TYPE, // RDA; Message type
    SS_MESSAGE_T_APPLICATIONSETTINGS, // RDA; Message
    SS_MESSAGE_UNRECOGNISED, // RDA; Message unrecognised
    SS_MESSAGING, // RDA; Messaging
    SS_MESSENGER_UC, // RDA; Messenger
    SS_METALLIC, // RDA; Metallic
    SS_METHOD_NOT_ALLOWED, // RDA; Method not allowed
    SS_METRE2, // RDA; metre²
    SS_MEXICO_CITY, // RDA; Mexico City
    SS_MG, // RDA; mg
    SS_MHZ, // RDA; MHz
    SS_MICROPHONE, // RDA; Microphone
    SS_MIDDLE, // RDA; Middle
    SS_MIDLET_STARTED_GET_INFORMATION_CURRENT_APPLICATION_STOPPED_Q, // RDA; This MIDlet will be started periodically to get information. Current application will be stopped. Continue?
    SS_MIDWAY, // RDA; Midway
    SS_MID_ATLANTIC, // RDA; Mid-Atlantic
    SS_MILE, // RDA; mile
    SS_MILE2, // RDA; mile²
    SS_MILES2_MCVT, // RDA; Miles²
    SS_MILES_MCVT, // RDA; Miles
    SS_MILKY, // RDA; Milky
    SS_MIMETYPE, // RDA; Mime type
    SS_MINA_ZAYID, // RDA; Mina Zayed
    SS_MINIMIZED, // RDA; Minimized
    SS_MINSK, // RDA; Minsk
    SS_MINUTE_MINDER, // RDA; Minute minder
    SS_MINYA, // RDA; Minya
    SS_MIN_ABB, // RDA; mins
    SS_MIRROR, // RDA; Mirror
    SS_MISCELLANIES, // RDA; Miscellanies
    SS_MISCELLANY, // RDA; Miscellany
    SS_MISRATAH, // RDA; Misurata
    SS_MISSED_ALARM_EVENT, // RDA; Missed alarm event
    SS_MISSED_ALARM_EVENTS, // RDA; Missed alarm events
    SS_MISSED_CALL, // RDA; Missed call
    SS_MISSED_CALLS, // RDA; Missed calls
    SS_MISSED_CALLS_ABB, // RDA; Missed calls
    SS_MISSED_CALLS_MCLOG, // RDA; Missed calls
    SS_MISSED_CALLS_T_POP, // RDA; Missed calls
    SS_MISSED_CALL_ABB, // RDA; Missed call
    SS_MIZDAH, // RDA; Mizdah
    SS_MI_COACH_MEDIA_PLAYER, // RDA; miCoach/Media Player
    SS_MM, // RDA; mm
    SS_MMC_IS_NOT_RECOGNISED_AND_NEED_TO_BE_FORMATTED_REFORMATTING_Q, // RDA; Memory card is not recognised and needs to be formatted. Reformatting will erase all data. Continue?
    SS_MMC_REMOVED, // RDA; Memory card removed
    SS_MMI_SAVEFAILED, // RDA; Save failed
    SS_MML_VODA_PICTURE_ALBUM_IE, // RDA; Picture Album
    SS_MMS, // RDA; MMS
    SS_MMS_CONTENTCLASS_HELP_FREE, // RDA; No size and content restrictions
    SS_MMS_CONTENTCLASS_HELP_RESTRICT, // RDA; Message class will restrict size and content
    SS_MMS_CONTENTCLASS_HELP_WARNING, // RDA; You will be notified when size and content exceed the maximum allowed
    SS_MMS_MEMORY_FULL, // RDA; MMS memory full
    SS_MMS_POSTCARD, // RDA; MMS Postcard
    SS_MMS_PROFILES, // RDA; Multimedia messages
    SS_MMS_SESSION_ALREADY_USED, // RDA; MMS session already used
    SS_MMS_SET_RECEIVE_AUTO, // RDA; Auto download
    SS_MMS_SET_RECEIVE_FOREIGN, // RDA; Foreign network
    SS_MMS_SET_RECEIVE_HOME, // RDA; Home network
    SS_MMS_SET_RECEIVE_MANUAL, // RDA; Manual
    SS_MMS_SET_RECEIVE_REJECT, // RDA; Reject
    SS_MMS_TEMPLATES, // RDA; MMS templates
    SS_MM_SS, // RDA; MM:SS
    SS_MOBILE, // RDA; Mobile
    SS_MOBILE_1, // RDA; Mobile 1
    SS_MOBILE_2, // RDA; Mobile 2
    SS_MOBILE_3, // RDA; Mobile 3
    SS_MOBILE_4, // RDA; Mobile 4
    SS_MOBILE_5, // RDA; Mobile 5
    SS_MOBILE_ACCESS_SERVICE, // RDA; Mobile access service
    SS_MOBILE_ACCESS_SERVICE_ACTIVATED, // RDA; Mobile access service activated
    SS_MOBILE_ACCESS_SERVICE_DEACTIVATED, // RDA; Mobile access service deactivated
    SS_MOBILE_BUSINESS, // RDA; Mobile, business
    SS_MOBILE_PHONE_FACEBOOK, // RDA; Mobile phone
    SS_MOBILE_PRAYER_SWA, // RDA; Mobile prayer
    SS_MOBILE_PRIVATE, // RDA; Mobile, private
    SS_MOBILE_TRACKER, // RDA; Mobile tracker
    SS_MOBILE_TRACKER_MSG, // RDA; WARNING: Activating this feature means that mobile tracker may send, via SMS, personal information about the user to pre-set mobile phone numbers. It is therefore the user's responsibility to ensure that mobile tracker is deactivated before the handset is transferred to other persons. Failure to deactivate mobile tracker may leave the user liable for damages caused by the sending of such personal information including, for example, the costs incurred by a third party as a result of such personal information being transmitted. For further details, please refer to the mobile tracker description in the manual and the instructions on activating and deactivating the feature.
    SS_MOBILE_TV, // RDA; Mobile TV
    SS_MOCHA, // RDA; Mocha
    SS_MODE, // RDA; Mode
    SS_MODIFIED, // RDA; Modified
    SS_MOHAMMEDIA, // RDA; Mohammedia
    SS_MOHARRAM_SWA, // RDA; Moharram
    SS_MON, // RDA; Mon
    SS_MONDAY, // RDA; Monday
    SS_MONOCHROME, // RDA; Monochrome
    SS_MONTH, // RDA; Month
    SS_MONTHLY, // RDA; Monthly
    SS_MONTHLY_REPEAT, // RDA; Monthly repeat
    SS_MONTHS_LC, // RDA; months
    SS_MONTH_LC, // RDA; month
    SS_MON_FM, // RDA; M
    SS_MOODS, // RDA; Moods
    SS_MOONLIGHT, // RDA; Moonlight
    SS_MORDAD_MEA, // RDA; Mordad
    SS_MORE, // RDA; More
    SS_MORE_DEVICES, // RDA; More devices
    SS_MORE_GAMES, // RDA; More games
    SS_MORE_IMAGES, // RDA; More images
    SS_MORE_MUSIC, // RDA; More music
    SS_MORE_NETWORKS, // RDA; More networks
    SS_MORE_PICTURES, // RDA; More pictures
    SS_MORE_SOUNDS, // RDA; More sounds
    SS_MORE_VIDEOS, // RDA; More videos
    SS_MOROCCO_M_COUNTRYNAME, // RDA; Morocco
    SS_MOSAIC, // RDA; Mosaic
    SS_MOSAIC_SHOT, // RDA; Mosaic shot
    SS_MOSCOW, // RDA; Moscow
    SS_MOSCOW_RUS, // RDA; Moscow-Rus
    SS_MOSTAGANEM, // RDA; Mostaganem
    SS_MOST_PLAYED, // RDA; Most played
    SS_MOSUL, // RDA; Mosul
    SS_MOTION_BEATBOX, // RDA; Motion beatbox
    SS_MOTION_CONTROL, // RDA; Motion control
    SS_MOTION_WALLPAPER, // RDA; Motion wallpaper
    SS_MOTIVATION_SONGLIST_IS_EMPTY, // RDA; Motivation song list is empty
    SS_MOTIVATION_SONGS, // RDA; Motivation songs
    SS_MOUNTAIN_CANADA, // RDA; Mountain Canada
    SS_MOUNTAIN_USA, // RDA; Mountain U.S.A
    SS_MOVE, // RDA; Move
    SS_MOVED, // RDA; Moved
    SS_MOVED_PERMANENTLY, // RDA; Moved permanently
    SS_MOVE_DOWN, // RDA; Move down
    SS_MOVE_HERE, // RDA; Move here
    SS_MOVE_PAGE_BACKWARD, // RDA; Move page backward
    SS_MOVE_PAGE_FORWARD, // RDA; Move page forward
    SS_MOVE_TO, // RDA; Move to
    SS_MOVE_TO_FAVOURITE_IMAGES, // RDA; Move to Favourite images
    SS_MOVE_TO_FAVOURITE_SOUNDS, // RDA; Move to Favourite sounds
    SS_MOVE_TO_FAVOURITE_VIDEOS, // RDA; Move to Favourite videos
    SS_MOVE_TO_MEMORY_CARD, // RDA; Move to memory card
    SS_MOVE_TO_MESSAGE_ARCHIVE, // RDA; Move to message archive
    SS_MOVE_TO_MY_FOLDER, // RDA; Move to My folder
    SS_MOVE_TO_MY_FOLDERS, // RDA; Move to My folders
    SS_MOVE_TO_NEW_FOLDER, // RDA; Move to new folder
    SS_MOVE_TO_PHONE, // RDA; Move to phone
    SS_MOVE_TO_SELECTED_FOLDER, // RDA; Move to selected folder
    SS_MOVE_TO_SIM, // RDA; Move to SIM
    SS_MOVE_TRACK_IN_PLAYLIST, // RDA; Move track in playlist
    SS_MOVE_UP, // RDA; Move up
    SS_MOVING_ING, // RDA; Moving...
    SS_MOVING_ITEM_REVERT_ALARM_TONE_TO_DEFAULT_CONTINUE_Q, // RDA; Moving item will revert alarm tone to default. Continue?
    SS_MOVING_ITEM_REVERT_WALLPAPER_TO_DEFAULT_CONTINUE_Q, // RDA; Moving item will revert Wallpaper to default. Continue?
    SS_MOVING_ITEM_WILL_REVERT_CALLER_ID_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert caller ID to default. Move?
    SS_MOVING_ITEM_WILL_REVERT_CALLER_RINGTONE_ALAM_TONE_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert caller ringtone and alarm tone to default. Move?
    SS_MOVING_ITEM_WILL_REVERT_CALLER_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Moving item will revert Caller Ringtone to default. Continue?
    SS_MOVING_ITEM_WILL_REVERT_FM_ALERT_TONE_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert FM alert tone to default. Move?
    SS_MOVING_ITEM_WILL_REVERT_RINGTONE_ALAM_TONE_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert ringtone and alarm tone to default. Move?
    SS_MOVING_ITEM_WILL_REVERT_RINGTONE_CALLER_RINGTONE_ALAM_TONE_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert ringtone, caller ringtone and alarm tone to default. Move?
    SS_MOVING_ITEM_WILL_REVERT_RINGTONE_CALLER_RINGTONE_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert ringtone and caller ringtone to default. Move?
    SS_MOVING_ITEM_WILL_REVERT_RINGTONE_TO_DEFAULT_CONTINUE_Q, // RDA; Moving item will revert Ringtone to default. Continue?
    SS_MOVING_ITEM_WILL_REVERT_WALLPAPER_AND_CALLER_ID_TO_DEFAULT_MOVE_Q, // RDA; Moving item will revert wallpaper and caller ID to default. Move?
    SS_MOVISTAR, // RDA; Movistar
    SS_MSAKEN, // RDA; Msaken
    SS_MSALLATA, // RDA; Msallata
    SS_MSG_NOT_CORRECTLY_DISPLAYED_TO_RECIPIENTS_DELETE_THIS_ITEM_Q, // RDA; Message contains incompatible item and may not display correctly on recipient's device. Delete this item?
    SS_MSILA, // RDA; M'sila
    SS_MUHARRAM_MEA, // RDA; Muharram
    SS_MULTAN, // RDA; Multan
    SS_MULTIMEDIA, // RDA; Multimedia
    SS_MULTIMEDIA_FUNCTION_CANNOT_BE_ACCESSED_WHEN_DB_REBUILDING_IS_IN_PROGRESS, // RDA; Multimedia function cannot be accessed when DB rebuilding is in progress
    SS_MULTIMEDIA_MESSAGE, // RDA; Multimedia message
    SS_MULTIMEDIA_MESSAGES, // RDA; Multimedia messages
    SS_MULTIMEDIA_MESSAGE_PROFILES, // RDA; Multimedia message profiles
    SS_MULTIMEDIA_MESSAGE_SENT, // RDA; Multimedia message sent
    SS_MULTIMEDIA_MESSAGE_TEMPLATES, // RDA; Multimedia message templates
    SS_MULTIMEDIA_MSG_TMO, // RDA; Multimedia Msg
    SS_MULTIMEIDA_MESSAGE, // RDA; Multimedia message
    SS_MULTIPARTY, // RDA; Multiparty
    SS_MULTIPLE, // RDA; Multiple
    SS_MULTIPLE_T_CAMERA, // RDA; Multiple
    SS_MULTI_PHONEBOOK, // RDA; Multi-phonebook
    SS_MULTI_SHOT, // RDA; Multi-shot
    SS_MULTI_SIM_ALWAYS_ON, // RDA; Multi SIM always on
    SS_MUMBAI, // RDA; Mumbai
    SS_MUQDADIYAH, // RDA; Muqdadiyah
    SS_MURZUK, // RDA; Murzuq
    SS_MUSCAT, // RDA; Muscat
    SS_MUSIC, // RDA; Music
    SS_MUSIC_AUTO_OFF, // RDA; Music auto off
    SS_MUSIC_BOX, // RDA; Music box
    SS_MUSIC_DB_IN_MEMORY_CORRUPTED_PART_OF_MEMORY_NEEDS_TO_BE_FORMATTED_REFORMATTING_ERASE_SOME_OF_DATA_Q, // RDA; Music DB file in memory chip is corrupted. Part of the memory needs to be formatted. Reformatting will erase some of the data. Continue?
    SS_MUSIC_LIBRARY, // RDA; Music library
    SS_MUSIC_OR_SOUND, // RDA; Music or sound
    SS_MUSIC_PLAYER, // RDA; Music player
    SS_MUSIC_PLAYER_SETTINGS, // RDA; Music player settings
    SS_MUSIC_RECOGNITION, // RDA; Music recognition
    SS_MUSIC_RECOGNITION_HELP, // RDA; Music recognition helps you to find the name of a song and gives you instructions on how to buy songs from Samsung Fun Club. Simply record a clip of a song and send it to the server
    SS_MUSIC_STORE, // RDA; Music store
    SS_MUSIC_VOLUME, // RDA; Music volume
    SS_MUSTACHE, // RDA; Mustache
    SS_MUTE, // RDA; Mute
    SS_MUTE_CANNOT_BE_SET_WHEN_ANSWERING_MODE_IS_AUTOMATIC, // RDA; Mute cannot be set when answering mode is automatic
    SS_MUTE_KEYS, // RDA; Mute keys
    SS_MUTE_KEYTONES_OPT, // RDA; Mute keytones
    SS_MUTE_KEY_TONE, // RDA; Mute keytone
    SS_MUTE_MCALL, // RDA; Mute
    SS_MUTTONCHOPS, // RDA; Muttonchops
    SS_MUZAHMIYYA, // RDA; Al-Muzahmiyya
    SS_MYFAVES_DISPLAY_OFF, // RDA; MyFaves display off
    SS_MYFAVES_DISPLAY_ON, // RDA; MyFaves display on
    SS_MYFAVES_EXTRAS, // RDA; MyFaves Extras
    SS_MYFAVES_ICON, // RDA; MyFaves icon
    SS_MYFAVES_ICONS, // RDA; MyFaves icons
    SS_MYFAVES_ITEMS, // RDA; with MyFaves items
    SS_MYFAVES_LIST, // RDA; MyFaves list
    SS_MYFAVES_RINGTONE, // RDA; MyFaves ringtone
    SS_MYFAVES_RINGTONES, // RDA; MyFaves ringtones
    SS_MYFAVES_SERVICE, // RDA; MyFaves service
    SS_MYFAVES_SETTINGS, // RDA; MyFaves settings
    SS_MYFAVES_UPDATED, // RDA; MyFaves updated
    SS_MYMESSAGES, // RDA; My messages
    SS_MYPHONEBOOK_SETUP_TMO, // RDA; MyPhonebook setup
    SS_MYPHONEBOOK_SET_UP_PHONE_AUTOMATICALLY_SAVED_ONLINE_ACCESS_THEM_PS_TMO, // RDA; MyPhonebook has now been set up on your phone. Your contacts will be automatically saved online and you can access them here: <%s>. Your preferences can be changed in the MyPhonebook settings
    SS_MYPHONEBOOK_TMO, // RDA; MyPhonebook
    SS_MY_ANIMATION, // RDA; My animation
    SS_MY_BUSINESS_CARD, // RDA; My business card
    SS_MY_CHANNELS, // RDA; My channels
    SS_MY_DEVICES, // RDA; My devices
    SS_MY_DEVICE_VISIBLE_TO_THE_OTHERS_FOR_PD_MINUTES_MY_VISIBILITY_WILL_BE_OFF_AFTER_PD_MINUTES, // RDA; My device will be visible to the others for %d minutes. My visibility will be off after %d minutes
    SS_MY_EMAIL_ADDRESS, // RDA; My email address
    SS_MY_EMAIL_INFORMATION, // RDA; My email information
    SS_MY_FAVORITES, // RDA; My favorites
    SS_MY_FILES, // RDA; My files
    SS_MY_FOLDER, // RDA; My folder
    SS_MY_FOLDERS, // RDA; My folders
    SS_MY_IMAGE, // RDA; My image
    SS_MY_MELODY, // RDA; My melody
    SS_MY_MENU, // RDA; My menu
    SS_MY_MENU_EMPTY_ASSIGN_NOW_Q, // RDA; My menu empty. Assign now?
    SS_MY_NAMECARD, // RDA; My namecard
    SS_MY_NAMECARD_LA, // RDA; My namecard
    SS_MY_NAMECARD_SENT, // RDA; My namecard sent
    SS_MY_NETWORKS, // RDA; My networks
    SS_MY_NEW_DEVICES, // RDA; My new devices
    SS_MY_NUMBER_NOT_SENT_WHEN_I_CALL, // RDA; My number not sent when I call
    SS_MY_NUMBER_SENT_WHEN_I_CALL, // RDA; My number sent when I call
    SS_MY_PHONE, // RDA; My phone
    SS_MY_PHONES_NAME, // RDA; My phone's name
    SS_MY_PHONES_VISIBILITY, // RDA; My phone's visibility
    SS_MY_PHOTOS, // RDA; My photos
    SS_MY_PHOTOS_LIST_FULL_DELETE_SOME_ITEMS, // RDA; My photos list full. Delete some items
    SS_MY_PROFILE_1, // RDA; My profile 1
    SS_MY_PROFILE_2, // RDA; My profile 2
    SS_MY_RATING, // RDA; My rating
    SS_MY_THEME, // RDA; My theme
    SS_MY_VIDEO_CLIPS, // RDA; My video clips
    SS_MY_VIDEO_CLIPS_LIST_FULL_DELETE_SOME_ITEMS, // RDA; My video clips list full. Delete some items
    SS_MY_VIDEO_CLIPS_MDOWNLOADED, // RDA; My video clips
    SS_MY_VIEW_LC_COSMOTE, // RDA; my view
    SS_MY_VODAFONE, // RDA; My Vodafone
    SS_MY_VOICE_CLIPS, // RDA; My voice clips
    SS_MY_VOICE_CLIPS_LIST_FULL_DELETE_SOME_ITEMS, // RDA; My voice clips list full. Delete some items
    SS_M_MMONDAY, // RDA; M
    SS_NAB_WIZARD, // RDA; NAB wizard
    SS_NADOR, // RDA; Nador
    SS_NAFTA, // RDA; Nefta 
    SS_NAIROBI, // RDA; Nairobi
    SS_NAJAF, // RDA; Najaf
    SS_NAJRAN, // RDA; Najran
    SS_NAJ_HAMMADI, // RDA; Nag Hammadi
    SS_NAME, // RDA; Name
    SS_NAMECARD, // RDA; Namecard
    SS_NAMECARD_RECEIVED, // RDA; Namecard received
    SS_NAMECARD_SENT, // RDA; Namecard sent
    SS_NAME_C, // RDA; Name:
    SS_NAME_IS_NEEDED, // RDA; Name is needed
    SS_NAME_MHUMAN, // RDA; Name
    SS_NAME_MSENDER, // RDA; Name
    SS_NAME_ONLY, // RDA; Name only
    SS_NAME_OR_NUMBER, // RDA; Name or number
    SS_NAME_RECORED_ONLY, // RDA; Name recorded only
    SS_NAME_TOO_LONG, // RDA; Name too long
    SS_NASIRIYAH, // RDA; Nasiriyah
    SS_NAUT_MILE, // RDA; naut. mile
    SS_NAUT_MILES_MCVT, // RDA; Naut.Miles
    SS_NAVARATRI_BEGIN_SWA, // RDA; Navaratri begin
    SS_NAVARATRI_SWA, // RDA; Navaratri 
    SS_NAVIGATE, // RDA; Navigate
    SS_NAVIGATION, // RDA; Navigation
    SS_NAVIGATION_SHORTCUTS_CANNOT_BE_USED_WHILE_SMART_IDLE_IS_ACTIVE, // RDA; Navigation shortcuts cannot be used while smart idle is active
    SS_NAVIGATION_SHORTCUTS_NOT_BE_USED_WHILE_SHORTCUTS_TOOLBAR_IS_ACTIVE, // RDA; Navigation shortcuts cannot be used while Shortcuts toolbar is active
    SS_NAVY_BLUE, // RDA; Navy blue
    SS_NAYARIT, // RDA; Nayarit
    SS_NEGATIVE, // RDA; Negative
    SS_NETWORK, // RDA; Network
    SS_NETWORK_ACCESS, // RDA; Network access
    SS_NETWORK_BUSY, // RDA; Network busy
    SS_NETWORK_BUSY_TRY_LATER, // RDA; Network busy. Try later
    SS_NETWORK_ERROR_POP, // RDA; Network error
    SS_NETWORK_LOCK_PASSED, // RDA; Network lock passed
    SS_NETWORK_MODE, // RDA; Network mode
    SS_NETWORK_PROBLEM, // RDA; Network problem
    SS_NETWORK_PROFILE, // RDA; Network profile
    SS_NETWORK_PROFILES, // RDA; Network profiles
    SS_NETWORK_SELECTION, // RDA; Network selection
    SS_NETWORK_SELECTION_FAILED, // RDA; Network selection failed
    SS_NETWORK_SERVICES, // RDA; Network services
    SS_NETWORK_SETTINGS, // RDA; Network settings
    SS_NETWORK_SWITCH_KEY, // RDA; Network switch key
    SS_NETWORK_UNAVAILABLE, // RDA; Network not available
    SS_NETWORK_UNAVAILABLE_RETRY_Q, // RDA; Network not available. Try again?
    SS_NETWORK_WHEN_ABROAD, // RDA; Network when abroad
    SS_NEVER, // RDA; Never
    SS_NEVER_ALLOW, // RDA; Never allow
    SS_NEW, // RDA; New
    SS_NEWCASTLE, // RDA; Newcastle
    SS_NEWFOUNDLAND, // RDA; Newfoundland
    SS_NEWS, // RDA; News
    SS_NEW_ACTIVATED_ITEMS, // RDA; New activated items
    SS_NEW_BLOCKED_ADDRESS, // RDA; New blocked address
    SS_NEW_CALL, // RDA; New call
    SS_NEW_CB, // RDA; New CB
    SS_NEW_DELHI, // RDA; New Delhi
    SS_NEW_EMAIL, // RDA; New email
    SS_NEW_EMAILS_T_POPUP, // RDA; New emails
    SS_NEW_FOLDER, // RDA; New folder
    SS_NEW_GROUP, // RDA; New group
    SS_NEW_IMAGE, // RDA; New image
    SS_NEW_MEMAIL, // RDA; New
    SS_NEW_MESSAGE, // RDA; New message
    SS_NEW_MESSAGES, // RDA; New messages
    SS_NEW_MESSAGES_ABB, // RDA; New messages
    SS_NEW_MESSAGE_ABB, // RDA; New message
    SS_NEW_MMS, // RDA; New MMS
    SS_NEW_MOBILBOXPRO_MESSAGES_TMO, // RDA; New voicemails
    SS_NEW_MOBILBOXPRO_MESSAGE_TMO, // RDA; New voicemail
    SS_NEW_MSG, // RDA; new Msg
    SS_NEW_MSGS, // RDA; new Msgs
    SS_NEW_MSG_ABB, // RDA; New msg
    SS_NEW_NAME, // RDA; New name
    SS_NEW_NETWORKS, // RDA; New networks
    SS_NEW_PAGE_CREATED_AND_MEDIA_ADDED, // RDA; New page created and media added
    SS_NEW_PASSWORD, // RDA; New password
    SS_NEW_PASSWORD_SAVED, // RDA; New password saved
    SS_NEW_PHOTO_NAME, // RDA; New photo name
    SS_NEW_SCHEDULE, // RDA; New schedule
    SS_NEW_SETTINGS_HAVE_BEEN_SAVED_TAP_SCREEN, // RDA; New settings have been saved. Tap the screen
    SS_NEW_SIM_DETECTED, // RDA; New SIM card detected
    SS_NEW_TEXT_MSG_TMO, // RDA; New text Msg
    SS_NEW_THEMES, // RDA; New themes
    SS_NEW_T_ACCOUNTSETTINGSINCOMINGEMAILOPTION, // RDA; New
    SS_NEW_URL_ACHEADER3, // RDA; NEW URL
    SS_NEW_VOICEMAILS, // RDA; New voicemails
    SS_NEW_VOICEMAILS_TMO, // RDA; New voicemails
    SS_NEW_VOICEMAIL_TMO, // RDA; New voicemail
    SS_NEW_YEAR_DAY_SWA, // RDA; New year day
    SS_NEW_YORK, // RDA; New York
    SS_NEXT, // RDA; Next
    SS_NEXT_PAGE, // RDA; Next page
    SS_NFC, // RDA; NFC
    SS_NFC_SETTINGS, // RDA; NFC settings
    SS_NICKNAME, // RDA; Nickname
    SS_NICOSIA, // RDA; Nicosia
    SS_NIGERIA, // RDA; Nigeria
    SS_NIGHT_MODE_OFF, // RDA; Night mode off
    SS_NIGHT_MODE_ON, // RDA; Night mode on
    SS_NIGHT_TIME_LANDSCAPE, // RDA; Night-time landscape
    SS_NIZWA, // RDA; Nizwa
    SS_NO, // RDA; No
    SS_NOISE, // RDA; Noise
    SS_NOK, // RDA; NOK
    SS_NONE, // RDA; None
    SS_NONE_M_GENERAL, // RDA; None
    SS_NONE_M_INTENSITY, // RDA; None
    SS_NON_PERSONAL_GREETING, // RDA; Non-personal greeting
    SS_NORMAL, // RDA; Normal
    SS_NORMAL_AND_VIBRATION, // RDA; Normal and vibration
    SS_NORMAL_MBRIGHTNESS, // RDA; Normal
    SS_NORMAL_MCALL, // RDA; Normal
    SS_NORMAL_MDEFAULT, // RDA; Normal
    SS_NORMAL_MEMAILPRIORITY, // RDA; Normal
    SS_NORMAL_MMSGTEXTSIZE, // RDA; Normal
    SS_NORMAL_MPRIORITY, // RDA; Normal
    SS_NORMAL_MQUALITY, // RDA; Normal
    SS_NORMAL_MSOUNDEFFECT, // RDA; Normal
    SS_NORMAL_PHONEBOOK, // RDA; Normal phonebook
    SS_NORMAL_SCREEN, // RDA; Normal screen
    SS_NORMAL_T_RECORDINGMODE, // RDA; Normal
    SS_NORMAL_VIEW, // RDA; Normal view
    SS_NOSEBLEED, // RDA; Nosebleed
    SS_NOTE, // RDA; Note
    SS_NOTES, // RDA; Notes
    SS_NOTHING_RECORDED, // RDA; Nothing recorded
    SS_NOTHING_SELECTED, // RDA; Nothing selected
    SS_NOTHING_TO_SAVE, // RDA; Nothing to save
    SS_NOTIFICATION, // RDA; Notification
    SS_NOTIFICATION_AFTER_SAING_GREETING_MSG_TMO, // RDA; Greeting saved. This is now your active greeting. If you want to change it you can do this in the Voicemail settings
    SS_NOTIFICATION_NOT_SELECTS_GREETING_MESSAGE_TMO, // RDA; If you change your mind you can always record a greeting in the Mobilbox Pro settings.
    SS_NOTIFICATION_SET_TO_STADNARD_GREETING_MESSAGE_TMO, // RDA; You will be reminded to record a personal greeting in a week. Until then people hearing your voicemail will just hear your voicemail will just hear your phone number unless you choose to record a message in the Mobilbox Pro settings.
    SS_NOTIFIED_WHEN_SIZE_N_CONTENT_EXCEED_MAXIMUM_ALLOWED, // RDA; You will be notified when size and content exceed maximum allowed
    SS_NOTIFY, // RDA; Notify
    SS_NOTIFYING_ING, // RDA; Notifying...
    SS_NOT_ACCEPTABLE, // RDA; Not acceptable
    SS_NOT_ALLOWED, // RDA; Not allowed
    SS_NOT_ALLOWED_DURING_CALL, // RDA; Not allowed during call
    SS_NOT_ALLOWED_LC, // RDA; Not allowed
    SS_NOT_ASSIGNED, // RDA; Not assigned
    SS_NOT_ASSIGNED_M_EMPTY, // RDA; Not assigned
    SS_NOT_AVAILABLE, // RDA; Not available
    SS_NOT_DONE, // RDA; Not Done
    SS_NOT_ENOUGH_CARD_MEMORY_SAVE_TO_PHONE, // RDA; Not enough card memory. Save to phone?
    SS_NOT_ENOUGH_CARD_MEMORY_SAVE_TO_PHONE_Q, // RDA; Not enough card memory. Save to phone?
    SS_NOT_ENOUGH_CARD_MEMORY_TO_SAVE_LIST, // RDA; Playlist is saved in Memory card which is Full
    SS_NOT_ENOUGH_MEMORY, // RDA; Not enough memory
    SS_NOT_ENOUGH_MEMORY_DELETE_EMAILS_OLDER_THAN_1_DAY_Q, // RDA; Not enough memory. Delete emails older than 1 day?
    SS_NOT_ENOUGH_MEMORY_DELETE_EMAILS_OLDER_THAN_1_MONTH_Q, // RDA; Not enough memory. Delete emails older than 1 month?
    SS_NOT_ENOUGH_MEMORY_DELETE_EMAILS_OLDER_THAN_1_WEEK_Q, // RDA; Not enough memory. Delete emails older than 1 week?
    SS_NOT_ENOUGH_MEMORY_DELETE_EMAILS_OLDER_THAN_3_DAYS_Q, // RDA; Not enough memory. Delete emails older than 3 days?
    SS_NOT_ENOUGH_MEMORY_DELETE_EMAILS_OLDER_THAN_3_WEEKS_Q, // RDA; Not enough memory. Delete emails older than 3 weeks?
    SS_NOT_ENOUGH_MEMORY_DELETE_EMAILS_OLDER_THAN_CUSTOM_TIME_Q, // RDA; Not enough memory. Delete emails older than the custom time?
    SS_NOT_ENOUGH_MEMORY_DELETE_SOME_ITEMS, // RDA; Not enough memory. Delete some items
    SS_NOT_ENOUGH_MEMORY_DOCUMENT_TRUNCATED, // RDA; Not enough memory. Document may be truncated
    SS_NOT_ENOUGH_MEMORY_IN_MEMORY_CARD_SAVE_TO_PHONE_Q, // RDA; Not enough space on memory card. Save to phone?
    SS_NOT_ENOUGH_MEMORY_IN_PHONE_SAVE_TO_MEMORY_CARD_Q, // RDA; Not enough memory in phone. Save to memory card?
    SS_NOT_ENOUGH_MEMORY_UNABLE_TO_DOWNLOAD, // RDA; Not enough memory. Unable to download
    SS_NOT_ENOUGH_PHONE_MEMORY_SAVE_TO_MEMORY_CARD, // RDA; Not enough phone memory. Save to memory card
    SS_NOT_ENOUGH_PHONE_MEMORY_SAVE_TO_MEMORY_CARD_Q, // RDA; Not enough phone memory. Save to memory card?
    SS_NOT_ENOUGH_RESOURCES, // RDA; Not enough resources
    SS_NOT_ENOUGH_SPACE, // RDA; Not enough space
    SS_NOT_ENOUGH_SPACE_FOR_MUSIC_DB_DLETE_SOME_ITEMS, // RDA; Not enough space for Music DB. Delete some items
    SS_NOT_FOUND, // RDA; Not found
    SS_NOT_IMPLEMENTED, // RDA; Not implemented
    SS_NOT_PERFORMED, // RDA; Not performed
    SS_NOT_SAVED, // RDA; Not saved
    SS_NOT_SUPPORTED, // RDA; Not supported
    SS_NOT_SUPPORTED_CONTENT, // RDA; Not supported content
    SS_NOT_SUPPORTED_CONTENTS, // RDA; Not supported contents
    SS_NOT_SUPPORTED_CONTENT_SELECT_ANOTHER_IMAGE_Q, // RDA; Not a supported file type. Select another image?
    SS_NOT_SURE, // RDA; Not sure
    SS_NOT_SYNCHRONISED, // RDA; Not synchronised
    SS_NOT_SYNCHRONISED_CHANGE_ROAMING_SETTING_IN_MYPHONEBOOK_TMO, // RDA; Your contacts have not been synchronised. You can change this roaming setting in the MyPhonebook settings menu
    SS_NOT_USED, // RDA; Not used
    SS_NOT_YET_PERSONALISED, // RDA; Not yet personalised
    SS_NOV, // RDA; Nov.
    SS_NOVALID_NUM_EDIT_CONTACT, // RDA; No valid number.\nEdit contact?
    SS_NOVEMBER, // RDA; November
    SS_NOVOSIBIRSK, // RDA; Novosibirsk
    SS_NOW, // RDA; Now
    SS_NOW_LC_IN_USE, // RDA; now in use
    SS_NOW_PLAYING, // RDA; Now playing
    SS_NO_ADDRESS, // RDA; No address
    SS_NO_ALARMS, // RDA; No alarms
    SS_NO_ANSWER, // RDA; No answer
    SS_NO_ATTEMPTS_LEFT, // RDA; No attempts left
    SS_NO_BLUETOOTH_DEVICE_FOUND, // RDA; No Bluetooth device found
    SS_NO_BLUETOOTH_STEREO_HEADSETS_PAIRED_SEARCH_NOW_Q, // RDA; No Bluetooth stereo headsets paired
    SS_NO_CALLS, // RDA; No calls
    SS_NO_CHANNELS, // RDA; No channels
    SS_NO_CONTACT, // RDA; No contacts
    SS_NO_CONTACTS, // RDA; No contacts
    SS_NO_CONTENT, // RDA; No content
    SS_NO_CREDIT_LEFT, // RDA; No credit left
    SS_NO_CRL, // RDA; No CRL
    SS_NO_DATABASE_SELECTED, // RDA; No database selected
    SS_NO_DAY_SELECTED, // RDA; No day selected
    SS_NO_DETAILS, // RDA; No details
    SS_NO_EFFECTS, // RDA; No effects
    SS_NO_EMAILS_OLDER_THAN_1DAY_PLEASE_DELETE_SOME_EMAILS_PERMANENTLY, // RDA; No emails older than 1 day. Delete some emails permanently
    SS_NO_EMAILS_OLDER_THAN_1MONTH_PLEASE_DELETE_SOME_EMAILS_PERMANENTLY, // RDA; No emails older than 1 month. Delete some emails permanently
    SS_NO_EMAILS_OLDER_THAN_1WEEK_PLEASE_DELETE_SOME_EMAILS_PERMANENTLY, // RDA; No emails older than 1 week. Delete some emails permanently
    SS_NO_EMAILS_OLDER_THAN_3DAYS_PLEASE_DELETE_SOME_EMAILS_PERMANENTLY, // RDA; No emails older than 3 days. Delete some emails permanently
    SS_NO_EMAILS_OLDER_THAN_3WEEKS_PLEASE_DELETE_SOME_EMAILS_PERMANENTLY, // RDA; No emails older than 3 weeks. Delete some emails permanently
    SS_NO_EMAILS_OLDER_THAN_3_DAYS_PLEASE_DELETE_SOME_ITEM, // RDA; No emails older than 3 days. Delete some items
    SS_NO_EMAILS_OLDER_THAN_3_WEEKS_PLEASE_DELETE_SOME_ITEM, // RDA; No emails older than 3 weeks. Delete some items
    SS_NO_EMAILS_OLDER_THAN_CUSTOM_TIME_PLEASE_DELETE_SOME_EMAILS_PERMANENTLY, // RDA; No emails older than custom time. Delete some emails permanently
    SS_NO_EMAILS_OLDER_THAN_CUSTOM_TIME_PLEASE_DELETE_SOME_ITEMS, // RDA; No emails older than custom time. Delete some items
    SS_NO_EMAILS_OLDER_THAN_I_DAY_PLEASE_DELETE_SOME_ITEM, // RDA; No emails older than 1 day. Delete some items
    SS_NO_EMAILS_OLDER_THAN_I_MONTH_PLEASE_DELETE_SOME_ITEM, // RDA; No emails older than 1 month. Delete some items
    SS_NO_EMAILS_OLDER_THAN_I_WEEK_PLEASE_DELETE_SOME_ITEM, // RDA; No emails older than 1 week. Delete some items
    SS_NO_EMAIL_ACCOUNT_CONFIGURE_NOW_Q, // RDA; No email account is set up. Configure now?
    SS_NO_EMAIL_ADDRESS, // RDA; No email address
    SS_NO_ENTRY, // RDA; No entry
    SS_NO_EVENT, // RDA; No event
    SS_NO_EXIST_ENTRY, // RDA; No entry exists
    SS_NO_FAVOURITES_M_IMPORTANT_MSG, // RDA; No favourites
    SS_NO_FDN_CONTACTS, // RDA; No FDN contacts
    SS_NO_FILE, // RDA; No file
    SS_NO_FILES, // RDA; No files
    SS_NO_FUNCTION, // RDA; No function
    SS_NO_GAME_TO_RUN, // RDA; No game to run
    SS_NO_GROUPS, // RDA; No groups
    SS_NO_IMAGE, // RDA; No image
    SS_NO_IMAGE_TO_SAVE, // RDA; No image to save
    SS_NO_INK, // RDA; No ink
    SS_NO_ITEMS, // RDA; No items
    SS_NO_ITEMS_TICKED, // RDA; No items ticked
    SS_NO_ITEM_SELECTED, // RDA; No items selected
    SS_NO_LIMIT, // RDA; No limit
    SS_NO_LIST, // RDA; No list
    SS_NO_LOGS, // RDA; No logs
    SS_NO_MATCH_FOUND, // RDA; No match found
    SS_NO_MEMBERS, // RDA; No members
    SS_NO_MEMORY_CARD_INSERTED, // RDA; No memory card inserted
    SS_NO_MEMOS, // RDA; No memos
    SS_NO_MESSAGE, // RDA; No message
    SS_NO_MESSAGES, // RDA; No messages
    SS_NO_MORE_NEW_MESSAGE, // RDA; No more new message
    SS_NO_MORE_VALID_CONTENT, // RDA; No more valid content
    SS_NO_MULTIMEDIA_MESSAGE_PROFILES, // RDA; Unable to send MMS. Set message profile.
    SS_NO_MUSIC_FILES, // RDA; No music files
    SS_NO_NAME, // RDA; No name
    SS_NO_NETWORK, // RDA; NO NETWORK
    SS_NO_NETWORKS_FOUND, // RDA; No network found
    SS_NO_NEW_MESSAGE, // RDA; No new message
    SS_NO_NUMBER, // RDA; No number
    SS_NO_OF_ATTEMPTS_LEFT, // RDA; 
    SS_NO_OTHER_PLAYLISTS_CREATE_NEW_PLAYLIST_AND_ADD_MUSIC_IN_IT_Q, // RDA; There are no other playlists. Create playlist and add music in it?
    SS_NO_PAGE, // RDA; No page
    SS_NO_PAGES_BACKWARD, // RDA; No pages backward
    SS_NO_PAGES_FORWARD, // RDA; No pages forward
    SS_NO_PAPER, // RDA; No paper
    SS_NO_PAPER_LOADED, // RDA; No paper loaded
    SS_NO_PHONEBOOK, // RDA; No phonebook
    SS_NO_PRINTABLE_IMAGE, // RDA; No printable image
    SS_NO_PRINT_HEAD, // RDA; No print head
    SS_NO_QUESTION, // RDA; No question
    SS_NO_RATING, // RDA; No rating
    SS_NO_RECIPIENT, // RDA; No recipient
    SS_NO_REMINDERS, // RDA; No reminders
    SS_NO_REPLY, // RDA; No reply
    SS_NO_REPLY_TIME, // RDA; No reply time
    SS_NO_RESOURCES, // RDA; No resources
    SS_NO_RESPONSE_FROM_AP, // RDA; No response from AP
    SS_NO_RESPONSE_FROM_RECIPIENT, // RDA; No response from the recipient
    SS_NO_SECURITY, // RDA; No security
    SS_NO_SERVICE, // RDA; No service
    SS_NO_SIGNAL, // RDA; No signal
    SS_NO_SIM, // RDA; No SIM
    SS_NO_SIM_CARD_GA, // RDA; No SIM card
    SS_NO_SK, // RDA; No
    SS_NO_SPACE, // RDA; No Space
    SS_NO_SPACE_FOR_NEW_DEVICE_REPLACE_AN_EXISTING_DEVICE_Q, // RDA; No space for new device. Replace an existing device?
    SS_NO_STATIONS, // RDA; No stations
    SS_NO_SUBJECT, // RDA; No subject
    SS_NO_TEMPLATES, // RDA; No templates
    SS_NO_TITLE, // RDA; No title
    SS_NO_TRANSITION, // RDA; No transition
    SS_NO_UDISK_OR_SD, // RDA; No Udisk or Card
    SS_NO_UPDATE_AVAILABLE, // RDA; No update available
    SS_NO_VALID_EMAIL_EDIT_CONTACT_Q, // RDA; No valid email. Edit contact?
    SS_NO_VOICE, // RDA; No voice
    SS_NO_WI_FI_SERVICE_CAN_USE_THIS_AP, // RDA; Wi-Fi services unable to use this AP
    SS_NTSC, // RDA; NTSC
    SS_NULL, // RDA; 
    SS_NUMBER, // RDA; Number
    SS_NUMBER_FDN_RESTRICTED, // RDA; Number FDN restricted
    SS_NUMBER_MODE, // RDA; Number mode
    SS_NUMBER_OF_BALLS, // RDA; Number of balls
    SS_NUMBER_OF_COPIES, // RDA; Number of copies
    SS_NUMBER_TOO_LONG, // RDA;  Number Too Long
    SS_NUMERIC, // RDA; Numeric
    SS_O2_CUSTOMER_SERVICE, // RDA; O2 Customer Service
    SS_OBEX_OPERATION_ERROR, // RDA; Obex operation error
    SS_OBJECTURI, // RDA; objectURI
    SS_OBJECT_PUSH, // RDA; Object push
    SS_OCCASION, // RDA; Occasion
    SS_OCEAN_BLUE, // RDA; Ocean blue
    SS_OCEAN_MTHEME, // RDA; Ocean
    SS_OCT, // RDA; Oct.
    SS_OCTOBER, // RDA; October
    SS_OFF, // RDA; Off
    SS_OFFICE, // RDA; Office
    SS_OFFICE_1, // RDA; Office 1
    SS_OFFICE_2, // RDA; Office 2
    SS_OFFICE_3, // RDA; Office 3
    SS_OFFICE_4, // RDA; Office 4
    SS_OFFICE_5, // RDA; Office 5
    SS_OFFICE_T_SIM, // RDA; Office
    SS_OFFLINE, // RDA; Offline
    SS_OFFLINE_MODE, // RDA; Offline mode
    SS_OFFLINE_PROFILE_ACTIVATE_CHANGE_TO_SILENT_PROFILE_SEARCH_NETWORK_Q, // RDA; Offline profile active. Change to Silent profile and search for network?
    SS_OFFLINE_PROFILE_ACTIVATE_MESSAGE_WILL_BE_SENT_WHEN_PROFILE_CHANGED, // RDA; Offline profile is active. Message will be sent when profile is changed
    SS_OFFLINE_PROFILE_ACTIVE_MESSAGE_WILL_BE_DELETED, // RDA; Offline profile active. Message will be deleted when Offline profile is changed
    SS_OFFLINE_PROFILE_ACTIVE_MESSAGE_WILL_BE_RETRIVED, // RDA; Offline profile active. Message will be retrieved when Offline profile is changed
    SS_OFFPEAK_TARIFF_INFORMATION, // RDA; Offpeak tariff information
    SS_OFF_LA, // RDA; deactivated
    SS_OFF_SK, // RDA; Off
    SS_OFF_T_CAM, // RDA; Off
    SS_OGBOMOSHO, // RDA; Ogbomosho
    SS_OIL_PAINT, // RDA; Oil paint
    SS_OK, // RDA; OK
    SS_OK_E, // RDA; OK!
    SS_OMAN, // RDA; Oman
    SS_OMNIX_ERA, // RDA; Omnix
    SS_ON, // RDA; On
    SS_ONCE, // RDA; Once
    SS_ONCE_ONLY, // RDA; Once only
    SS_ONCE_REMOVED_PS_WILL_HAVE_TO_BE_REINSTALLED, // RDA; Once removed, %s will have to be reinstalled
    SS_ONE_MORE_USE_OF_PS, // RDA; You have one more use of %s
    SS_ONE_T_MUSIC, // RDA; One
    SS_ONLY_BETWEEN_0_AND_9_AVAILABLE, // RDA; Only between 0 to 9 available
    SS_ONLY_ONE_CALL_IN_MULTIPARTY, // RDA; Only one call in multiparty
    SS_ON_CARD, // RDA; On card
    SS_ON_DEMAND, // RDA; On demand
    SS_ON_FOR_3_MIN, // RDA; On for 3 mins
    SS_ON_HOLD_ABB, // RDA; On hold
    SS_ON_HOLD_BUTTON, // RDA; On hold
    SS_ON_PHONE, // RDA; On phone
    SS_ON_SK, // RDA; On
    SS_ON_T_CAM, // RDA; On
    SS_OPACITY, // RDA; Opacity
    SS_OPEN, // RDA; Open
    SS_OPEN_ALL_CHANNELS, // RDA; Open all channels
    SS_OPEN_ALL_STATIONS, // RDA; Open all stations
    SS_OPEN_APPLICATIONS_MAY_BE_CLOSED, // RDA; Open applications may be closed
    SS_OPEN_FAVOURTIES, // RDA; Open Favourites
    SS_OPEN_FILE, // RDA; Open file
    SS_OPEN_FLIP_TO_ANSWER_INCOMING_CALLS, // RDA; Open phone to answer incoming calls
    SS_OPEN_PLAYLIST, // RDA; Open playlist
    SS_OPEN_Q, // RDA; Open?
    SS_OPEN_SYSTEM, // RDA; Open system
    SS_OPEN_URL, // RDA; Open URL
    SS_OPERATOR, // RDA; Operator
    SS_OPTIMUM, // RDA; Optimum
    SS_OPTIONAL, // RDA; Optional
    SS_OPTIONS, // RDA; Options
    SS_OPTIONS_SK, // RDA; Options
    SS_OPTIONS_SK2, // RDA; Opt.
    SS_OPTIONS_SK_ABB, // RDA; Options
    SS_ORAN, // RDA; Oran
    SS_ORANGE_DOWNLOADS, // RDA; Orange Downloads
    SS_ORANGE_HOMESCREEN, // RDA; Orange Homescreen
    SS_ORANGE_MUSIC, // RDA; Orange Music
    SS_ORANGE_M_CARRIER, // RDA; Orange
    SS_ORANGE_M_COLOUR, // RDA; Orange
    SS_ORANGE_WORLD, // RDA; Orange World
    SS_ORANGE_WORLD_ORANGE, // RDA; Orange World
    SS_ORDER_OF_DISPLAY_NAME, // RDA; Order of display name
    SS_ORDIBEHESHT_MEA, // RDA; Ordibehesht
    SS_ORGANISER, // RDA; Organiser
    SS_ORGANISE_SIDEBAR, // RDA; Organise sidebar
    SS_ORIYA_INPUT, // RDA; କଖଗ
    SS_OSHOGBO, // RDA; Osogbo
    SS_OTHER, // RDA; Other
    SS_OTHERS, // RDA; Others
    SS_OTHER_1, // RDA; Other 1
    SS_OTHER_2, // RDA; Other 2
    SS_OTHER_3, // RDA; Other 3
    SS_OTHER_4, // RDA; Other 4
    SS_OTHER_5, // RDA; Other 5
    SS_OTHER_COUNTRIES, // RDA; Other countries
    SS_OTHER_DOCUMENT, // RDA; Other document
    SS_OTHER_FILES, // RDA; Other files
    SS_OTHER_FOLDER, // RDA; Other folder
    SS_OTHER_MAILBOX_SERVER_OPERATION_SUSPEND_AUTO_EMAIL_SYNC, // RDA; Other mailbox server operation may suspend Auto email sync
    SS_OTHER_MEDIUM_IS_ALREADY_ACTIVATED, // RDA; Other medium is already activated
    SS_OUARGLA, // RDA; Ouargla
    SS_OUARZAZATE, // RDA; Ouarzazate
    SS_OUJDA, // RDA; Oujda
    SS_OUNCE, // RDA; ounce
    SS_OUNCES_MCVT, // RDA; Ounces
    SS_OUTBOX, // RDA; Outbox
    SS_OUTBOX_ABB, // RDA; Outbox
    SS_OUTBOX_MMENU, // RDA; Outbox
    SS_OUTDOOR, // RDA; Outdoor
    SS_OUTGOING_BARRING, // RDA; Outgoing barring
    SS_OUTGOING_CALLS_BARRED, // RDA; Outgoing calls barred
    SS_OUTGOING_CALL_DEFLECTED, // RDA; Outgoing call deflected
    SS_OUTGOING_CALL_FORWARDED, // RDA; Outgoing call forwarded
    SS_OUTGOING_EMAIL_SERVER, // RDA; Outgoing email server
    SS_OUTGOING_INTERNATIONAL_CALLS_BARRED, // RDA; Outgoing international calls barred
    SS_OUTGOING_INTERNATIONAL_CALLS_EXCEPT_HOME_BARRED, // RDA; Outgoing international calls except home barred
    SS_OUTGOING_INTER_CALLS, // RDA; Outgoing international calls
    SS_OUTGOING_TEXT_MESSAGE, // RDA; Outgoing text message
    SS_OUTLINE, // RDA; Outline
    SS_OUT_MZOOM, // RDA; Out
    SS_OUT_OF_MEMORY, // RDA; Out of memory
    SS_OUT_OF_RANGE, // RDA; Out of range
    SS_OUT_OF_RANGE_MMESSAGE, // RDA; Out of range
    SS_OVERFLOW, // RDA; Overflow
    SS_OVERFLOW_NOT_ALLOWED, // RDA; Overflow. Not allowed
    SS_OVERLAPPING, // RDA; Overlapping
    SS_OVERWRITE, // RDA; Overwrite
    SS_OVERWRITE_Q, // RDA; Overwrite?
    SS_OVERWRITE_Q_OTHERWISE_RECEIVING_WILL_BE_FAILED, // RDA; Overwrite? Otherwise receiving will be failed.
    SS_OWN_NUMBER, // RDA; Own number
    SS_OWN_NUMBERS, // RDA; Own numbers
    SS_PACIFIC_CANADA, // RDA; British Columbia
    SS_PACIFIC_USA, // RDA; Pacific U.S.A
    SS_PAGE, // RDA; Page
    SS_PAGES_PER_SHEET, // RDA; Pages per sheet
    SS_PAGE_DETAILS, // RDA; Page details
    SS_PAGE_DURATION, // RDA; Page duration
    SS_PAGE_FORMAT, // RDA; Page format
    SS_PAGE_PILOT, // RDA; Page pilot
    SS_PAGE_SIZE, // RDA; Page size
    SS_PAGE_SIZE_IS_TOO_LARGE, // RDA; Page size too large
    SS_PAGE_TRANSITION, // RDA; Page transition
    SS_PAINT, // RDA; Paint
    SS_PAIR, // RDA; Pair
    SS_PAIRED_INSTRUMENTS, // RDA; Paired instruments
    SS_PAIRED_WITH_PS, // RDA; Paired with %s
    SS_PAIRING_CODE, // RDA; Pairing code
    SS_PAIRING_FAILED_CODE_INCORRECT, // RDA; Pairing failed. Code is incorrect
    SS_PAIRING_FAILED_IRRELEVANT_AP_INTERFERED, // RDA; Pairing failed. Irrelevant AP interfered
    SS_PAIR_ONE_MORE_HEADSET_FOR_BT_DUAL_STEREO_HEADSET, // RDA; Pair one more headset for Bluetooth dual stereo headset
    SS_PAKISTAN_M_COUNTRYNAME, // RDA; Pakistan
    SS_PAL, // RDA; PAL
    SS_PANNING, // RDA; Pan
    SS_PAN_ACCESS_POINT, // RDA; PAN access point
    SS_PAN_GROUP_NETWORK, // RDA; PAN group network
    SS_PAN_USER, // RDA; PAN user
    SS_PAP, // RDA; PAP
    SS_PAPER_ERROR, // RDA; Paper error
    SS_PAPER_JAM, // RDA; Paper jam
    SS_PAPER_LELVEL_ERROR, // RDA; Paper lever error
    SS_PAPER_SIZE, // RDA; Paper size
    SS_PARAND, // RDA; Parand
    SS_PARIS, // RDA; Paris
    SS_PARROT, // RDA; Parrot
    SS_PARSER_ERROR, // RDA; Parser error
    SS_PARSING_ERROR, // RDA; Parsing error
    SS_PARTIAL_BLUR, // RDA; Partial blur
    SS_PARTIAL_EFFECTS, // RDA; Partial effects
    SS_PASSWORD, // RDA; Password
    SS_PASSWORD_CHANGED, // RDA; Password changed
    SS_PASSWORD_DOES_NOT_MATCH, // RDA; Password does not match
    SS_PASSWORD_EMPTY, // RDA; Password empty
    SS_PASSWORD_MUST_BE_4_DIGITS_LONG, // RDA; Password must be 4 digits long
    SS_PASSWORD_TOO_SHORT, // RDA; Password too short
    SS_PASTE, // RDA; Paste
    SS_PASTE_HERE, // RDA; Paste here
    SS_PASTE_TO_NEW_FOLDER, // RDA; Paste to new folder
    SS_PASTE_TO_SELECTED_FOLDER, // RDA; Paste to selected folder
    SS_PATTERN, // RDA; Pattern
    SS_PATTERN_COLOUR, // RDA; Pattern colour
    SS_PAUSE, // RDA; Pause
    SS_PAYMENT, // RDA; Payment
    SS_PB_COPY_TO_PHONE, // RDA; Copy to Phone
    SS_PB_EMPTY, // RDA; Empty
    SS_PB_EMPTY_LIST, // RDA; <Empty>
    SS_PC_CONNECTIONS, // RDA; PC connections
    SS_PC_STUDIO, // RDA; PC studio
    SS_PD_1_ATTEMPTS_LEFT, // RDA; %d attempt left
    SS_PD_ATTEMPTS_LEFT, // RDA; %d attempts left
    SS_PD_ATTEMPT_LEFT, // RDA; %d attempt left
    SS_PD_CHANNELS, // RDA; %d channels
    SS_PD_CONTACTS_DELETED, // RDA; %d contacts deleted
    SS_PD_DAYS_BEFORE, // RDA; %d days before
    SS_PD_DIGITS, // RDA; %d digits
    SS_PD_FILES_UNLOCKED, // RDA; %d files unlocked
    SS_PD_GROUPS_DELETED, // RDA; %d groups deleted
    SS_PD_KEY_IS_ASSIGNED_TO_PS_MHZ, // RDA; %d key is assigned to %s MHz
    SS_PD_LANGUAGES, // RDA; %d languages
    SS_PD_MINUTES, // RDA; %d minutes
    SS_PD_NEW_MESSAGES, // RDA; New messages
    SS_PD_STATIONS_FOUND, // RDA; %d stations found
    SS_PEACEFUL, // RDA; Peaceful
    SS_PEAK_TARIFF_INFORMATION, // RDA; Peak tariff information
    SS_PEDOMETER, // RDA; Pedometer
    SS_PEN, // RDA; PEN
    SS_PENDING, // RDA; Pending
    SS_PERFILESINTERNET, // RDA; Connections
    SS_PERFORM_MASTER_RESET_Q, // RDA; Perform master reset?
    SS_PERIOD, // RDA; Period
    SS_PERIODS, // RDA; Periods
    SS_PERMANENT, // RDA; Permanent
    SS_PERMISSION, // RDA; Permission
    SS_PERSONAL, // RDA; Personal
    SS_PERSONALISED, // RDA; Personalised
    SS_PERSONAL_GREETING_TMO, // RDA; Personal greeting
    SS_PERTH, // RDA; Perth
    SS_PESHAWAR, // RDA; Peshawar
    SS_PETROL_BLUE, // RDA; Petrol blue
    SS_PETROPAVLOVSK_KAMCHATSKIY, // RDA; Petropavlovsk-Kamchatskiy
    SS_PHOENIX, // RDA; Phoenix
    SS_PHONE, // RDA; Phone
    SS_PHONEBOOK, // RDA; Phonebook
    SS_PHONEBOOK_INITIALISING_ING, // RDA; Phonebook initialising...
    SS_PHONEBOOK_LIST, // RDA; Phonebook list
    SS_PHONEBOOK_LIST_SETTING, // RDA; Phonebook list setting
    SS_PHONEBOOK_NAME, // RDA; Phonebook name
    SS_PHONEBOOK_SETTINGS, // RDA; Phonebook settings
    SS_PHONEBOOK_SYNC_IN_PROGRESS, // RDA; Phonebook sync in progress
    SS_PHONEBOOK_SYNC_STARTED, // RDA; Phonebook sync started
    SS_PHONE_AND_SERVER, // RDA; Phone and server
    SS_PHONE_AND_SIM, // RDA; Phone and SIM
    SS_PHONE_CONTACTS_FIRST, // RDA; Phone contacts first
    SS_PHONE_CONTACT_LIST_FULL_DELETE_SOME_ITEMS, // RDA; Phone contact list full.Delete some items
    SS_PHONE_FREEZED, // RDA; Phone freezed
    SS_PHONE_IP_ADDRESS, // RDA; Phone IP address
    SS_PHONE_LOCK, // RDA; Phone lock
    SS_PHONE_LOCK_DEACTIVATED, // RDA; Phone lock deactivated
    SS_PHONE_LOCK_PASSED, // RDA; Phone lock passed
    SS_PHONE_MEMORY, // RDA; Phone memory
    SS_PHONE_MEMORY_FULL, // RDA; Phone memory full
    SS_PHONE_MEMORY_FULL_DELETE_SOME_ITEMS, // RDA; Phone memory full. Delete some items
    SS_PHONE_MEMORY_FULL_SAVE_TO_CARD_Q, // RDA; Phone memory full. Save to memory card?
    SS_PHONE_MEMORY_LIST_FULL_DELETE_SOME_ITEMS, // RDA; Phone memory list full. Delete some items
    SS_PHONE_MESSAGE, // RDA; Phone message
    SS_PHONE_NUMBER, // RDA; Phone number
    SS_PHONE_NUMBER_EMPTY, // RDA; Phone number empty
    SS_PHONE_ONLY, // RDA; Phone only
    SS_PHONE_PASSWORD, // RDA; Phone password
    SS_PHONE_PASSWORD_CHANGED, // RDA; Phone Password Changed 
    SS_PHONE_PROFILES, // RDA; Phone profiles
    SS_PHONE_PROFILE_S, // RDA; Phone profile's
    SS_PHONE_SETTINGS, // RDA; Phone settings
    SS_PHONE_TO_SERVER, // RDA; Phone to server
    SS_PHOTO, // RDA; Photo
    SS_PHOTOS, // RDA; Photos
    SS_PHOTO_M_DEFAULTFILENAME, // RDA; Photo
    SS_PHOTO_SETTINGS, // RDA; Photo Settings
    SS_PICTURE, // RDA; Picture
    SS_PICTURES, // RDA; Pictures
    SS_PICTURE_OR_VIDEO, // RDA; Picture or video
    SS_PIG_TAIL, // RDA; Pig tail
    SS_PIN, // RDA; PIN
    SS_PIN2, // RDA; PIN2
    SS_PIN2_BLOCKED, // RDA; PIN2 blocked
    SS_PIN2_CHANGED, // RDA; PIN2 changed
    SS_PIN2_CONFLICT, // RDA; Different PIN2 codes
    SS_PIN2_PUK2_N_NO_LONGER_VALID, // RDA; PIN2/PUK2 no longer valid
    SS_PIN2_TOO_LONG, // RDA; PIN2 too long
    SS_PIN2_TOO_SHORT, // RDA; PIN2 too short
    SS_PIN2_UNBLOCKED, // RDA; PIN2 unblocked
    SS_PIN2_UNLOCKED, // RDA; PIN2 unlocked
    SS_PINK, // RDA; Pink
    SS_PINKBIRD, // RDA; Pinkbird
    SS_PINKCAT, // RDA; Pinkcat
    SS_PIN_BLOCKED, // RDA; PIN blocked
    SS_PIN_CHANGED, // RDA; PIN changed
    SS_PIN_CHECK, // RDA; PIN check
    SS_PIN_CHECK_DISABLED, // RDA; PIN check disabled
    SS_PIN_CHECK_ENABLED, // RDA; PIN check enabled
    SS_PIN_CHECK_MANDATORY, // RDA; PIN check mandatory
    SS_PIN_CHECK_MUST_BE_ENABLED, // RDA; PIN check must be enabled
    SS_PIN_CONFLICT, // RDA; Different PIN codes
    SS_PIN_LOCK, // RDA; PIN lock
    SS_PIN_LOCKED, // RDA; PIN locked
    SS_PIN_TOO_LONG, // RDA; PIN too long
    SS_PIN_TOO_SHORT, // RDA; PIN too short
    SS_PIN_UNBLOCKED, // RDA; PIN unblocked
    SS_PIN_UNLOCKED, // RDA; PIN unlocked
    SS_PISTACHIO_GREEN, // RDA; Pistachio green
    SS_PLANNER, // RDA; Planner
    SS_PLANT, // RDA; Plant
    SS_PLAY, // RDA; Play
    SS_PLAYER_SETTINGS, // RDA; Player settings
    SS_PLAYING_MY_ANSWER, // RDA; Playing my answer
    SS_PLAYLIST, // RDA; Playlist
    SS_PLAYLISTS, // RDA; Playlists
    SS_PLAYLISTS_ABB, // RDA; Playlists
    SS_PLAYLIST_1, // RDA; Playlist 1
    SS_PLAYLIST_1_HMTP, // RDA; Playlist 1(MTP)
    SS_PLAYLIST_2, // RDA; Playlist 2
    SS_PLAYLIST_2_HPHONE, // RDA; Playlist 2(Phone)
    SS_PLAYLIST_3, // RDA; Playlist 3
    SS_PLAYLIST_4, // RDA; Playlist 4
    SS_PLAY_AGAIN, // RDA; Play again
    SS_PLAY_AGAIN_UC, // RDA; Play again
    SS_PLAY_IN_SILENT_PROFILE_Q, // RDA; Play in Silent profile?
    SS_PLAY_JAVA_APPLICATION_SOUND_AND_TURN_FM_RADIO_OFF_Q, // RDA; Play Java application sound and turn FM radio off?
    SS_PLAY_MUSIC_NOW_Q, // RDA; Play music now?
    SS_PLAY_MUSIC_PLAYER_AGAIN_Q, // RDA; Play Music player again?
    SS_PLAY_Q, // RDA; Play?
    SS_PLAY_SOUND, // RDA; Play sound
    SS_PLAY_SOUND_IN_SILENT_MODE_Q, // RDA; Play sound in Silent mode?
    SS_PLAY_SOUND_IN_SILENT_PROFILE_Q, // RDA; Play sound in Silent profile?
    SS_PLAY_VIA, // RDA; Play via
    SS_PLAY_VIA_BLUETOOTH_STEREO_HEADSET, // RDA; Play via Bluetooth stereo headset
    SS_PLEASE_CREATE_NAME_CARD_FIRST, // RDA; Create namecard first
    SS_PLEASE_DELETE_SOME_ITEMS_FROM_PHONE_MEMORY, // RDA; Delete some items from phone memory
    SS_PLEASE_ENTER_NAME_NUMBER, // RDA; Enter name and phone number
    SS_PLEASE_INSERT_CARKIT_USB, // RDA; Insert car kit USB
    SS_PLEASE_INSERT_MMC, // RDA; Insert memory card
    SS_PLEASE_INSERT_PROVINCE, // RDA; Enter county
    SS_PLEASE_INSERT_SIM_CARD, // RDA; Insert SIM card
    SS_PLEASE_INSERT_ZIP_CODE, // RDA; Enter postcode
    SS_PLEASE_WAIT, // RDA; Please wait
    SS_PLEASE_WAIT_ING, // RDA; Please wait...
    SS_PLEASE_WAIT_NETWORK_ACTION, // RDA; Wait network action
    SS_PLMN_MODE_REJECTED, // RDA; PLMN mode rejected
    SS_PLMN_NOT_ALLOWED, // RDA; PLMN not allowed
    SS_PLUM, // RDA; Plum
    SS_PLUSPL_PLUS, // RDA; Plus.pl
    SS_PM, // RDA; PM
    SS_PODCASTS, // RDA; Podcasts
    SS_PODCASTS_1, // RDA; Podcasts 1
    SS_POINT_COLOUR, // RDA; Point colour
    SS_POINT_THE_IMAGE_OF_SUN_IN_PHONE_TOWARDS_PHYSICAL_SUN_HORIZONTALLY_AND_PRESS_UPDATE, // RDA; Point the image of\nSun in phone\ntowards physical\nSun horizontally,\nand press Update
    SS_POISKI, // RDA; Polski
    SS_POO, // RDA; Poo
    SS_POP, // RDA; Pop
    SS_POP3, // RDA; POP3
    SS_POP3_PASSWORD, // RDA; POP3 password
    SS_POP3_PORT, // RDA; POP3 port
    SS_POP3_SERVCER, // RDA; POP3 server
    SS_POP3_USER_ID, // RDA; POP3 user ID
    SS_POPULAR_EMAIL_ACCOUNTS, // RDA; Popular email accounts
    SS_POPUP_LIST_MENU, // RDA; Pop-up list menu
    SS_POP_DRM_FILE_DELETE, // RDA; %s is a paid item purchased before. Continue?
    SS_POP_EARPHONE_NOT_CONNECTED, // RDA; Earphone not connected
    SS_POP_EARPHONE_NOT_CONNECTED_WORKS_AS_ANTENNA, // RDA; Earphone not connected. Earphone works as the radio antenna. Connect earphone.
    SS_PORT, // RDA; Port
    SS_PORTO_ALEGRE, // RDA; Porto Alegre
    SS_PORT_HARCOURT, // RDA; Port Harcourt
    SS_PORT_RANGE, // RDA; Port range
    SS_PORT_SAFAGA, // RDA; Port Safaga
    SS_PORT_SAID, // RDA; Port Said
    SS_POSITION_THE_PHONE_NEAR_THE_PRINTER, // RDA; Position the phone near the printer
    SS_POSSIBLE, // RDA; Possible
    SS_POST, // RDA; POST
    SS_POSTAL_BUSINESS, // RDA; Postal, business
    SS_POSTAL_PRIVATE, // RDA; Postal, private
    SS_POSTCARD, // RDA; Postcard
    SS_POSTCODE, // RDA; Postcode
    SS_POSTERIZE, // RDA; Posterize
    SS_POSTPONED_E_UPDATE_SHALL_PROCEED_AT_NEXT_REMINDER_IN_1_DAY, // RDA; Postponed! Update shall proceed at next reminder in 1 day.\nThank you!
    SS_POTAL_OF_VOICE, // RDA; Portal of voice
    SS_POULAD_SHAHR, // RDA; Poulad-shahr
    SS_POUNDS_MCVT, // RDA; Pounds
    SS_POWDER_BLUE, // RDA; Powder blue
    SS_POWER_ON_OFF_TONE, // RDA; Power on/off tone
    SS_POWER_SAVING_MODE, // RDA; Power saving mode
    SS_PRAHA, // RDA; Praha
    SS_PRAYER_ACTIVATION, // RDA; Prayer activation
    SS_PRAYER_TIME, // RDA; Prayer time
    SS_PRAYER_TIMES, // RDA; Prayer times
    SS_PRAYER_TONE, // RDA; Prayer tone
    SS_PRAYER_VOLUME, // RDA; Prayer volume
    SS_PRAYER_WALLPAPER, // RDA; Prayer wallpaper
    SS_PREDICTIVE_TEXT_OFF, // RDA; Predictive text off
    SS_PREDICTIVE_TEXT_ON, // RDA; Predictive text on
    SS_PREFERENCES, // RDA; Preferences
    SS_PREFERENCES_T_INTERNET, // RDA; Preferences
    SS_PREFERRED_SIM_CARD, // RDA; Preferred SIM card
    SS_PREFIX_DIALLING, // RDA; Prefix dialling
    SS_PREFIX_DIALLING_ACTIVATION_HELP_MSG, // RDA; When making a call, you can add frequently used prefix number automatically into dialling screen
    SS_PREFIX_NUMBER, // RDA; Prefix number
    SS_PRELISTEN, // RDA; Prelisten
    SS_PRESENT, // RDA; Present
    SS_PRESET_ANIMATION, // RDA; Preset animation
    SS_PRESET_IMAGE, // RDA; Preset image
    SS_PRESET_IMAGES, // RDA; Preset images
    SS_PRESET_MELODY, // RDA; Preset melody
    SS_PRESET_PICTURE, // RDA; Preset picture
    SS_PRESET_PROFILE_DETECTED_APPLY_AND_REPLACE_PREVIOUS_PROFILE_Q, // RDA; Preset profile detected. Apply and replace previous profile?
    SS_PRESS_AND_HOLD_THE_ALT_KEY_TO_UNLOCK, // RDA; Press and hold the Alt key to unlock
    SS_PRESS_ANY_KEY_TO_ANSWER_INCOMING_CALLS, // RDA; Press any key to answer incoming calls
    SS_PRESS_OK, // RDA; Press OK
    SS_PRESS_OK_TO_ADD, // RDA; Press OK to add
    SS_PRESS_RECORD_TO_IDENTYFY_MUSIC, // RDA; Press Record to identify music
    SS_PRESS_THE_BACK_KEY_TO_UNLOCK, // RDA; Press the back key to unlock
    SS_PREVIEW, // RDA; Preview
    SS_PREVIEW_AS_VIVID_MESSAGE, // RDA; Preview as vivid message
    SS_PREVIEW_ERROR, // RDA; Preview error
    SS_PREVIEW_TRACK_VIA, // RDA; Preview track via
    SS_PREVIOUS_PAGE, // RDA; Previous page
    SS_PRICE, // RDA; Price
    SS_PRICE_INFORMATION, // RDA; Price information
    SS_PRICE_UNIT, // RDA; Price/Unit
    SS_PRICE_UNIT_SAVED, // RDA; Price/Unit saved
    SS_PRIMARY_SIM_SCANNING_NETWORK_Q, // RDA; Primary SIM. Scanning network ?
    SS_PRINT, // RDA; Print
    SS_PRINTED_ON, // RDA; Printed on
    SS_PRINTER, // RDA; Printer
    SS_PRINTING, // RDA; Printing
    SS_PRINTING_ERROR, // RDA; Printing error
    SS_PRINTTER_BUSY, // RDA; Printer busy
    SS_PRINTTER_COVER_OPEN, // RDA; Printer cover open
    SS_PRINTTER_IN_USE, // RDA; Printer in use
    SS_PRINT_COMPLETED, // RDA; Print completed
    SS_PRINT_QUALITY, // RDA; Print quality
    SS_PRINT_SETTINGS, // RDA; Print settings
    SS_PRINT_TYPE, // RDA; Print type
    SS_PRINT_VIA, // RDA; Print via
    SS_PRINT_VIA_BLUETOOTH, // RDA; Print via Bluetooth
    SS_PRINT_VIA_USB, // RDA; Print via USB
    SS_PRIORITY, // RDA; Priority
    SS_PRIVACY, // RDA; Privacy
    SS_PRIVACY_ACTIVATED, // RDA; Privacy activated
    SS_PRIVACY_DEACTIVATED, // RDA; Privacy deactivated
    SS_PRIVACY_LOCK, // RDA; Privacy lock
    SS_PRIVACY_M_DATA_MANAGEMENT, // RDA; Privacy
    SS_PRIVATE, // RDA; Private
    SS_PRIVATE_CALL, // RDA; Private call
    SS_PRIVATE_CALL_SELECTED_OTHER_CALLS_HELD, // RDA; Private call selected. Other calls held
    SS_PRIVATE_T_CLD, // RDA; Private
    SS_PROCESSING_ERROR, // RDA; Processing error
    SS_PROCESSING_ING, // RDA; Processing...
    SS_PROFILE, // RDA; Profile
    SS_PROFILES, // RDA; Profiles
    SS_PROFILES_LA, // RDA; Profiles
    SS_PROFILES_T_GPRSPROFILES, // RDA; Profiles
    SS_PROFILE_CHANGED_IN_ALL_ACCOUNTS, // RDA; Profile changed in all accounts
    SS_PROFILE_CHANGED_IN_CURRENT_ACCOUNT, // RDA; Profile changed in current account
    SS_PROFILE_CHANGED_TO_PS, // RDA; Profile changed to %s
    SS_PROFILE_IN_USE, // RDA; Profile in use
    SS_PROFILE_NAME, // RDA; Profile name
    SS_PROFILE_NOT_SUPPROTED, // RDA; Profile is not supported
    SS_PROFILE_READ_FAILED, // RDA; Profile read failed
    SS_PROFILE_SETTING, // RDA; Profile setting
    SS_PROFILE_SETTINGS, // RDA; Profile settings
    SS_PROFILE_WRITE_FAILED, // RDA; Profile write failed
    SS_PROGRAMME_REMINDER, // RDA; Programme reminder
    SS_PROGRESIVE_DOWNLOAD, // RDA; Progressive download
    SS_PROMPT, // RDA; Prompt
    SS_PROMPT_FIRST, // RDA; Prompt first
    SS_PROPERTIES, // RDA; Properties
    SS_PROTECTED, // RDA; Protected
    SS_PROTECTED_ITEM_DELETE_Q, // RDA; Protected item. Delete?
    SS_PROTECTED_MESSAGES, // RDA; Protected messages
    SS_PROTECTED_MESSAGE_DELETE_Q, // RDA; Protected message. Delete?
    SS_PROTECTION, // RDA; Protection
    SS_PROTECTION_OFF, // RDA; Protection off
    SS_PROTECTION_OFF_LA, // RDA; Protection off
    SS_PROTECTION_OFF_MPOPUP, // RDA; Protection off
    SS_PROTECTION_ON, // RDA; Protection on
    SS_PROTECTION_ON_LA, // RDA; Protection on
    SS_PROTECTION_ON_MPOPUP, // RDA; Protection on
    SS_PROTECTION_ON_OFF, // RDA; Protection on/off
    SS_PROVIDER, // RDA; Provider
    SS_PROXY, // RDA; Proxy
    SS_PROXY_ADDRESS, // RDA; Proxy address
    SS_PROXY_IP, // RDA; Proxy IP
    SS_PROXY_PORT, // RDA; Proxy port
    SS_PROXY_PROFILE_NAME, // RDA; Proxy profile name
    SS_PROXY_SETTINGS, // RDA; Proxy settings
    SS_PS_ALREADY_IN_USE, // RDA; %s already in use
    SS_PS_ALREADY_IN_USE_CONTINUE_Q, // RDA;  already in use. Continue?
    SS_PS_AND_PS_CALL_SETTINGS_HAVE_BEEN_CHANGED, // RDA; %s and %s call settings have been changed
    SS_PS_CALL_WAITING_DISABLED_IF_PS_IS_OUT_OF_REACH_INCOMING_CALLS_WILL_BE_FORWARDED_TO_PS, // RDA; %s call waiting disabled.If %s is out of reach, incoming calls will be forwarded to %s
    SS_PS_CALL_WAITING_DISABLED_PS_CALL_FORWARDING_IF_OUT_OF_REACH_IS_DISABLED, // RDA; %s call waiting disabled.%s call forwarding if out of reach is disabled
    SS_PS_CALL_WAITING_ENABLED_IF_PS_IS_OUT_OF_REACH_INCOMING_CALLS_WILL_BE_FORWARDED_TO_PS, // RDA; %s call waiting enabled.If %s is out of reach, incoming calls will be forwarded to %s
    SS_PS_CALL_WAITING_ENABLED_PS_CALL_FORWARDING_IF_OUT_OF_REACH_IS_DISABLED, // RDA; %s call waiting enabled.%s call forwarding if out of reach is disabled
    SS_PS_CHANGED_IN_THE_PHONEBOOK_UPDATE_CHANGES_TO_MY5_Q, // RDA; %s changed in Phonebook. Update changes to MyFaves?
    SS_PS_CREATED, // RDA; %s created
    SS_PS_CURRENTLY_UNLOCKED, // RDA; %s is currently unlocked
    SS_PS_DELETED, // RDA; %s deleted
    SS_PS_DISCONNECTED, // RDA; %s disconnected
    SS_PS_DOWNLOAD_IN_PS, // RDA; %s download in %s
    SS_PS_HAS_BEEN_ADDED_TO_MYFAVES, // RDA; %s has been added to MyFaves
    SS_PS_HD_PD, // RDA; %s (D-%d)
    SS_PS_IS_AVAILABLE_CONNECT_Q, // RDA; %s is available. Connect?
    SS_PS_IS_NEW_VERSION, // RDA; %s is new version
    SS_PS_IS_OLD_VERSION, // RDA; %s is old version
    SS_PS_LOCKED, // RDA; %s locked
    SS_PS_LOCKED_AND_ALARM_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and alarm will be reverted to default
    SS_PS_LOCKED_AND_CALLER_ID_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and caller ID will be reverted to default
    SS_PS_LOCKED_AND_CALLER_RINGTONE_AND_CALLER_ID_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and caller ringtone and caller ID will be reverted to default
    SS_PS_LOCKED_AND_CALLER_RINGTONE_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and caller ringtone will be reverted to default
    SS_PS_LOCKED_AND_RINGTONE_AND_CALLER_ID_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and ringtone and caller ID will be reverted to default
    SS_PS_LOCKED_AND_RINGTONE_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and ringtone will be reverted to default
    SS_PS_LOCKED_AND_WALLPAPER_WILL_BE_REVERTED_TO_DEFAULT, // RDA; %s locked and wallpaper will be reverted to default
    SS_PS_NOT_ALLOWED, // RDA; %s not allowed
    SS_PS_OVERLAPS_WITH_PS, // RDA; %s overlaps with %s
    SS_PS_PD_TIMES, // RDA; %s %d times
    SS_PS_PROFILE_ON, // RDA; %s profile on
    SS_PS_RECEIVED, // RDA; %s received
    SS_PS_REMOVED_FROM_YOUR_KEY_CONTACTS, // RDA; %s has been removed from your key contacts
    SS_PS_SENT, // RDA; %s sent
    SS_PS_SETUP_WIZARD_COMPLETED_RERUN_AGAIN_ANYTIME, // RDA; %s setup wizard is completed. You can re-run it again at any time from the Settings menu
    SS_PS_SET_AS_WALLPAPER, // RDA; %s set as wallpaper
    SS_PS_S_BIRTHDAY, // RDA; %s's birthday
    SS_PS_TIME_ENDS, // RDA; %s time ends
    SS_PS_TIME_STARTS, // RDA; %s time starts
    SS_PS_UNBLOCKED, // RDA; %s unblocked
    SS_PS_WILL_START_IN_PS, // RDA; will start in 
    SS_PUK, // RDA; PUK
    SS_PUK2, // RDA; PUK2
    SS_PUK2_TOO_SHORT, // RDA; PUK2 too short
    SS_PUK_BLOCKED, // RDA; PUK blocked
    SS_PUK_TOO_SHORT, // RDA; PUK too short
    SS_PUK_UNDEFINED, // RDA; PUK not defined
    SS_PUL_I_KHUMRI, // RDA; Pul-i-Khumri
    SS_PUMPKIN, // RDA; Pumpkin
    SS_PUNE, // RDA; Pune
    SS_PUNJABI_INPUT, // RDA; ਕਖਗ
    SS_PUSH_INBOX, // RDA; Push inbox
    SS_PUSH_MESSAGE, // RDA; Push message
    SS_PUSH_MESSAGES, // RDA; Push messages
    SS_PUSH_REGISTRATION_FAILED, // RDA; Push registration failed
    SS_PUT, // RDA; PUT
    SS_PYG, // RDA; PYG
    SS_QADEIMAH, // RDA; Qadeimah
    SS_QAISUMAH, // RDA; Qaisumah
    SS_QATAR, // RDA; Qatar
    SS_QATIF, // RDA; Qatif
    SS_QCIF_H176_X_144, // RDA; QCIF (176X144)
    SS_QIBLAH_DIRECTION, // RDA; Qiblah direction
    SS_QIBLAH_DIRECTION_AND_PRAYER_TIMES, // RDA; Qiblah direction and Prayer times
    SS_QOM, // RDA; Qom
    SS_QQVGA_160_X_120, // RDA; QQVGA (160 x 120)
    SS_QUAD_BAND, // RDA; Quad band
    SS_QUALITY, // RDA; Quality
    SS_QUERY_DUAL_ACTIVE_SIM, // RDA; Enable Smart Dual SIM? 
    SS_QUERY_TRIPLE_ACTIVE_SIM, // RDA; Enable Smart triple SIM?(In some carriers, this function may not work properly or you may be charged)
    SS_QUESTION_E, // RDA; Question!
    SS_QUETTA, // RDA; Quetta
    SS_QUICK_SEND, // RDA; Quick send
    SS_QUILL_PEN, // RDA; Quill pen
    SS_QUINTANA_ROO, // RDA; Quintana Roo
    SS_QUIT, // RDA; Quit
    SS_QUNEITRA, // RDA; Quneitra
    SS_QVGA_H320_X_240, // RDA; QVGA (320 x 240)
    SS_RABAT, // RDA; Rabat
    SS_RABBA, // RDA; Rabba
    SS_RABBIT, // RDA; Rabbit
    SS_RABIGH, // RDA; Rabigh
    SS_RABI_H1_MEA, // RDA; Rabi (1)
    SS_RABI_H2_MEA, // RDA; Rabi (2)
    SS_RADIO_M_DEFAULTFILENAME, // RDA; Radio
    SS_RADIO_TEXT, // RDA; Radio text
    SS_RAFHA, // RDA; Rafha
    SS_RAHIMYAR_KHAN, // RDA; Rahimyar Khan
    SS_RAJAB_MEA, // RDA; Rajab
    SS_RAKSHA_BANDHAN_SWA, // RDA; Raksha bandhan
    SS_RAMADAN_MEA, // RDA; Ramadan
    SS_RAMADI, // RDA; Ramadi
    SS_RAM_NAVMI_SWA, // RDA; Ram navmi
    SS_RANDOM, // RDA; Random
    SS_RANDOMLY_DISPLAY_DEFAULT_WALLPAPERS, // RDA; Randomly display default wallpapers
    SS_RANDOM_WALLPAPERS, // RDA; Random wallpapers
    SS_RANDOM_WALLPAPER_LIST_FULL, // RDA; Random wallpaper list full
    SS_RASHT, // RDA; Rasht
    SS_RAS_AL_KHAIMAH, // RDA; Ras al-Khaimah
    SS_RAS_TANURA, // RDA; Ras Tanura
    SS_RATED, // RDA; Rated
    SS_RATE_SETTINGS, // RDA; Rate settings
    SS_RATE_TRACK, // RDA; Rate track
    SS_RAWALPINDI, // RDA; Rawalpindi
    SS_RAYSUT, // RDA; Raysut
    SS_REACTIVATE, // RDA; Reactivate
    SS_REACTIVE, // RDA; Reactivate
    SS_READ, // RDA; Read
    SS_READ_C, // RDA; Read:
    SS_READ_MESSAGE_MYSPACE, // RDA; Read message
    SS_READ_MSTATUS, // RDA; Read
    SS_READ_MVERB, // RDA; Read
    SS_READ_REPORT, // RDA; Read report
    SS_READ_REPORT_SENT, // RDA; Read report sent
    SS_READ_UNREAD, // RDA; Read/Unread
    SS_READ_USER_DATA, // RDA; Read user data
    SS_REBOOTING_ING, // RDA; Rebooting...
    SS_RECEIVE, // RDA; Receive
    SS_RECEIVED, // RDA; Received
    SS_RECEIVED_CALLS, // RDA; Received calls
    SS_RECEIVED_MESSAGES, // RDA; Received messages
    SS_RECEIVED_MESSAGE_EXCEEDED_THE_MAX_PAGE_NOT_THE_FULL_SMS_IS_RECEIVED, // RDA; Message truncated. Maximum number of pages exceeded
    SS_RECEIVEING_CHANNELS, // RDA; Receiving channels
    SS_RECEIVE_CALENDAR_DATA_FROM_PS_Q, // RDA; Receive calendar data from %s?
    SS_RECEIVE_CALENDAR_DATA_Q, // RDA; Receive calendar data?
    SS_RECEIVE_CALL_Q, // RDA; Receive call?
    SS_RECEIVE_CHANNELS, // RDA; Receiving channels
    SS_RECEIVE_FAILED, // RDA; Receive failed
    SS_RECEIVE_FILE_FROM_PS_Q, // RDA; Receive file from %s?
    SS_RECEIVE_MCALL, // RDA; Received
    SS_RECEIVE_MEMO_FROM_PS_Q, // RDA; Receive memo from %s?
    SS_RECEIVE_NAMECARD_FROM_PS_Q, // RDA; Receive the namecard from %s?
    SS_RECEIVE_OPTION, // RDA; Receive option
    SS_RECEIVE_THE_NAMECARD_FROM_PS_Q, // RDA; Receive the namecard from %s?
    SS_RECEIVE_THE_NAMECARD_Q, // RDA; Receive the namecard?
    SS_RECEIVING_ALERT_TYPE, // RDA; Receiving alert type
    SS_RECEIVING_ING, // RDA; Receiving
    SS_RECEIVING_OPTION, // RDA; Receiving option
    SS_RECEIVING_OPTIONS, // RDA; Receiving options
    SS_RECENTLY_ADDED, // RDA; Recently added
    SS_RECENTLY_PLAYED, // RDA; Recently played
    SS_RECENT_CALLS, // RDA; Recent calls
    SS_RECENT_CONTACTS, // RDA; Recent contacts
    SS_RECENT_RECIPIENT, // RDA; Recent recipient
    SS_RECENT_RECIPIENTS, // RDA; Recent recipients
    SS_RECENT_RESET_DATE, // RDA; Recent reset date
    SS_RECEVIED_MMSGDATA, // RDA; Received
    SS_RECIFE, // RDA; Recife
    SS_RECIPIENT, // RDA; Recipient
    SS_RECIPIENTS, // RDA; Recipients
    SS_RECIPIENT_LIST, // RDA; Recipient list
    SS_RECIPIENT_LIST_FULL, // RDA; Recipient list full
    SS_RECIPIENT_NEEDS_COMPATIBLE_PHONE_AND_RELEVANT_KEY_TO_USE_FILE, // RDA; Recipient needs compatible phone and relevant key to use this file
    SS_RECOGNITION_RESULT, // RDA; Recognition result
    SS_RECORDER, // RDA; Recorder
    SS_RECORDING_DATE, // RDA; Recording date
    SS_RECORDING_ING, // RDA; Recording...
    SS_RECORDING_MODE_M_VIDEO, // RDA; Recording mode
    SS_RECORDING_QUALITY, // RDA; Recording quality
    SS_RECORDING_SOUND, // RDA; Recording sound
    SS_RECORDING_TIME, // RDA; Recording time
    SS_RECORD_MNOUN, // RDA; Record
    SS_RECORD_MVERB, // RDA; Record
    SS_RECORD_SOUND, // RDA; Record sound
    SS_RECORD_T_STOPWATCH_SK, // RDA; Record
    SS_RECORD_VIDEO, // RDA; Record video
    SS_REC_UC_M_VOICE, // RDA; REC
    SS_RED, // RDA; Red
    SS_REDEYE_REDUCTION, // RDA; Red-eye reduction
    SS_REDIALING_ING, // RDA; Redialling...
    SS_REDIALLING, // RDA; Redialling
    SS_REDIALLING_MODE, // RDA; Redialling mode
    SS_REDIAL_AUTOMATICALLY_IF_CALL_UNABLE, // RDA; Redial automatically if call is unable to connect or is cut off.
    SS_REDIRECTING_ING, // RDA; Redirecting...
    SS_REENTER_NEW_PASSWORD, // RDA; Re-enter new password
    SS_REFRESH, // RDA; Refresh
    SS_REFRESHING_LIST, // RDA; Refreshing list
    SS_REFRESHING_MAY_TAKE_A_FEW_MOMENTS, // RDA; Refreshing may take a few moments
    SS_REFRESH_CONTACTS, // RDA; Refresh contacts
    SS_REFRESH_LIST, // RDA; Refresh list
    SS_REFRESH_TO_MOBILE, // RDA; Refresh to mobile
    SS_REFRESH_TO_SERVER, // RDA; Refresh to server
    SS_REGISTER, // RDA; Register
    SS_REGISTERED, // RDA; Registered
    SS_REGISTERED_MSAVED, // RDA; Registered
    SS_REGISTERED_MSAVED_NOT_ALLOWED, // RDA; Not allowed
    SS_REGISTERED_TO_CALENDAR, // RDA; Registered to Calendar
    SS_REGISTERED_TO_NETWORKS_SERVER, // RDA; Registered to networks server
    SS_REGISTERED_TO_TASK, // RDA; Registered to Task
    SS_REGISTERING, // RDA; Registering…
    SS_REGISTERING_ING, // RDA; Registering...
    SS_REGISTERING_ING_MSAVE, // RDA; Registering...
    SS_REGISTERING_NETWORK_CARD_HELP_MSG, // RDA; You can register your network cards. Network card names and icons are used to distinguish between different network cards.
    SS_REGISTER_CARD, // RDA; Register card
    SS_REGISTER_SIM, // RDA; Register SIM card
    SS_REGISTER_SIM_SET_SIM, // RDA; Register SIM / Set SIM
    SS_REGISTER_SIM_SET_SIM_HELPMESSAGE1, // RDA; You can register SIM. To distinguish SIMs, register SIM's name
    SS_REGISTER_SIM_SET_SIM_HELPMESSAGE2, // RDA; To deactivate SIM card, untick the enable box. Deactivated SIM card cannot be primary. Primary SIM card will be used for default SIM card when you call or send text messages. The phone will restart when primary SIM card is changed
    SS_REGISTER_TO_CALENDAR, // RDA; Register to Calendar
    SS_REGISTER_TO_MEMO, // RDA; Register to Memo
    SS_REGISTER_TO_PHONEBOOK, // RDA; Register to Phonebook
    SS_REGISTER_TO_TASK, // RDA; Register to Task
    SS_REGULAR, // RDA; Regular
    SS_REG_TO_MEMO, // RDA; Register to Memo
    SS_REJECT, // RDA; Reject
    SS_REJECTED, // RDA; Rejected
    SS_REJECT_ALL, // RDA; Reject all
    SS_REJECT_ANONYMOUS, // RDA; Reject anonymous
    SS_REJECT_CALL_WITH_MESSAGE, // RDA; Reject call with message
    SS_REJECT_CALL_WITH_MESSAGE_HELP_MSG, // RDA; You can send explanatory text message to callers when you reject their calls
    SS_REJECT_LA, // RDA; Reject
    SS_REJECT_LIST, // RDA; Reject list
    SS_REJECT_LIST_FULL, // RDA; Reject list full
    SS_REJECT_MCALL, // RDA; Reject
    SS_REJECT_Q, // RDA; Reject?
    SS_RELEASE_ALL_DATA_ON_MEMORY_CARD_CONTINUE_Q, // RDA; Release all data on memory card. Continue?
    SS_RELEASE_CALL_TIME_LIMIT, // RDA; Release call time limit
    SS_RELEASE_CALL_TIME_LIMIT_Q, // RDA; Release call time limit?
    SS_RELEASE_CALL_TIME_LIMIT_Q_LA, // RDA; Release call time limit?
    SS_RELEASE_TIME_LIMIT, // RDA; Release time limit
    SS_RELIZANE, // RDA; Relizane
    SS_RELOAD, // RDA; Reload
    SS_REMADA, // RDA; Remada
    SS_REMAIN_SK_ABB, // RDA; Remain
    SS_REMEMBER_LAST_PAGE_VIEWED, // RDA; Remember last page viewed
    SS_REMINDER, // RDA; Reminder
    SS_REMINDER_1, // RDA; Reminder 1
    SS_REMINDER_2, // RDA; Reminder 2
    SS_REMINDER_3, // RDA; Reminder 3
    SS_REMINDER_4, // RDA; Reminder 4
    SS_REMINDER_5, // RDA; Reminder 5
    SS_REMINDER_6, // RDA; Reminder 6
    SS_REMINDER_ACTIVATION, // RDA; Reminder activation
    SS_REMINDER_ALREADY_EXISTS, // RDA; Reminder already exists
    SS_REMINDER_DATE, // RDA; Reminder date
    SS_REMINDER_INTERVAL, // RDA; Reminder interval
    SS_REMINDER_NAME, // RDA; Reminder name
    SS_REMINDER_OFF, // RDA; Reminder off
    SS_REMINDER_ON, // RDA; Reminder on
    SS_REMINDER_TIME, // RDA; Reminder time
    SS_REMINDER_TONE, // RDA; Reminder tone
    SS_REMIND_AGAIN_Q, // RDA; Remind again?
    SS_REMIND_LATER_Q, // RDA; Remind later?
    SS_REMIND_ME, // RDA; Remind me
    SS_REMOTE_CALENDAR, // RDA; Remote calendar
    SS_REMOTE_CONTACTS, // RDA; Remote contacts
    SS_REMOTE_NOTE, // RDA; Remote note
    SS_REMOTE_SIM_MODE, // RDA; Remote SIM mode
    SS_REMOTE_SIM_MODE_ACTIVATED, // RDA; Remote SIM mode activated
    SS_REMOTE_SIM_MODE_DEACTIVATED, // RDA; Remote SIM mode deactivated
    SS_REMOTE_SIM_MODE_DISABLED, // RDA; Remote SIM mode disabled
    SS_REMOTE_SIM_MODE_ENABLED, // RDA; Remote SIM mode enabled
    SS_REMOTE_TASK, // RDA; Remote task
    SS_REMOVE, // RDA; Remove
    SS_REMOVED, // RDA; Removed
    SS_REMOVED_ALL, // RDA; Removed all
    SS_REMOVE_ALL, // RDA; Remove all
    SS_REMOVE_ALL_ATTACHMENTS, // RDA; Remove all attachments
    SS_REMOVE_ALL_KEY_CONTACTS, // RDA; Remove all key contacts
    SS_REMOVE_ALL_Q, // RDA; Remove all?
    SS_REMOVE_ATTACHMENT, // RDA; Remove attachment
    SS_REMOVE_ATTACHMENT_FIELD, // RDA; Remove attachment field
    SS_REMOVE_FROM_KEY_CONTACTS, // RDA; Remove from key contacts
    SS_REMOVE_FROM_PLAYLIST, // RDA; Remove from playlist
    SS_REMOVE_MEDIA, // RDA; Remove media
    SS_REMOVE_MESSAGE, // RDA; Remove message
    SS_REMOVE_MMULTICALL, // RDA; Remove
    SS_REMOVE_MULTIPLE, // RDA; Remove multiple
    SS_REMOVE_MUSIC, // RDA; Remove music
    SS_REMOVE_PICTURE, // RDA; Remove picture
    SS_REMOVE_PS_FROM_SHORTCUTS_Q, // RDA; Remove %s from shortcuts?
    SS_REMOVE_PS_FROM_YOUR_VIP_CONTACTS_Q, // RDA; Do you really want to remove %s from your VIP contacts?
    SS_REMOVE_PS_Q, // RDA; Remove %s?
    SS_REMOVE_Q, // RDA; Remove?
    SS_REMOVE_SOUND, // RDA; Remove sound
    SS_REMOVE_SUBJECT, // RDA; Remove subject
    SS_REMOVE_TEXT, // RDA; Remove text
    SS_REMOVE_TMO, // RDA; Remove
    SS_REMOVE_T_PLAYLIST, // RDA; Remove
    SS_REMOVE_VIDEO, // RDA; Remove video
    SS_REMOVING, // RDA; Removing...
    SS_REMOVING_PD_CONTACTS_ING, // RDA; Removing %d contacts...
    SS_RENAME, // RDA; Rename
    SS_RENAME_FOLDER, // RDA; Rename folder
    SS_RENDERING_MODE, // RDA; Rendering mode
    SS_REPEATING, // RDA; Repeating
    SS_REPEATS, // RDA; Repeats
    SS_REPEAT_1, // RDA; Repeat 1
    SS_REPEAT_ALL, // RDA; Repeat all
    SS_REPEAT_DAY, // RDA; Repeat day
    SS_REPEAT_EVERY_YEAR, // RDA; Repeat every year
    SS_REPEAT_EXCEPT_WEEKENDS_FOR_EVERY_DAY_ONLY, // RDA; Repeat except weekends for every day only
    SS_REPEAT_IS_AVAILABLE_ON_SAME_START_AND_END_DATE, // RDA; Repeat is available on same start and end date
    SS_REPEAT_MNOUN, // RDA; Repeat
    SS_REPEAT_MODE, // RDA; Repeat mode
    SS_REPEAT_UNTIL, // RDA; Repeat until
    SS_REPEAT_UNTIL_DATE_SHOULD_BE_SET_AFTER_END_DATE, // RDA; Repeat until date should be set after end date
    SS_REPEAT_VALUE_SHOULD_BE_SMALLER_THAN_PD, // RDA; Repeat value should be smaller than %d
    SS_REPLACED, // RDA; Replace?
    SS_REPLACE_CONTACT, // RDA; Replace contact
    SS_REPLACE_EXISTING, // RDA; Replace existing
    SS_REPLACE_MNOUN, // RDA; Replace
    SS_REPLACE_PS_Q, // RDA; Replace %s?
    SS_REPLAY, // RDA; Replay
    SS_REPLAY_ADRESS, // RDA; Reply address
    SS_REPLY, // RDA; Reply
    SS_REPLY_MVERB, // RDA; Reply
    SS_REPLY_PATH, // RDA; Reply path
    SS_REPLY_TO, // RDA; Reply to
    SS_REPLY_TO_ALL, // RDA; Reply to all
    SS_REPLY_TO_SENDER, // RDA; Reply to sender
    SS_REPLY_VIA, // RDA; Reply via
    SS_REPLY_VIA_MESSAGE, // RDA; Reply via Message
    SS_REPLY_VIA_MMS_TO, // RDA; Reply via MMS to
    SS_REPLY_VIA_MULTIMEDIA_MESSAGE_TO, // RDA; Reply via multimedia message to
    SS_REPLY_VIA_SMS, // RDA; Reply via SMS
    SS_REPLY_VIA_TEXT_MESSAGE, // RDA; Reply via text message
    SS_REPLY_VIA_VOICE_MESSAGE_TO, // RDA; Reply via voice message to
    SS_REPUBLIC_DAY_SWA, // RDA; Republic day
    SS_REQUESTED_READ_REPLY_SEND_Q, // RDA; Requested read reply. Send?
    SS_REQUESTING_ING, // RDA; Requesting...
    SS_REQUESTING_SERVICE_ING, // RDA; Requesting service...
    SS_REQUEST_DELIVERY_RECEIPT, // RDA; Request delivery receipt
    SS_REQUEST_DELIVERY_REPORT, // RDA; Request delivery report
    SS_REQUEST_ENTITY_TOO_LARGE, // RDA; Request entity too large
    SS_REQUEST_FAILED, // RDA; Request failed
    SS_REQUEST_MNOUN, // RDA; Request
    SS_REQUEST_MVERB, // RDA; Request
    SS_REQUEST_READ_RECEIPT, // RDA; Request read receipt
    SS_REQUEST_READ_REPORT, // RDA; Request read report
    SS_REQUEST_TIMEOUT, // RDA; Request timeout
    SS_REQUEST_TIMEOUT_TRY_AGAIN_Q, // RDA; Request timeout. Try again?
    SS_REQUEST_URI_TOO_LARGE, // RDA; Request URI too large
    SS_RESEND, // RDA; Resend
    SS_RESET, // RDA; Reset
    SS_RESETTING_ING, // RDA; Resetting...
    SS_RESET_ALL, // RDA; Reset all
    SS_RESET_ALL_MCOUNTER, // RDA; Reset all
    SS_RESET_ALL_MESSAGE_COUNTER_Q, // RDA; Reset all message counter?
    SS_RESET_ALL_PRAYERS, // RDA; Reset all prayers?
    SS_RESET_ALL_Q, // RDA; Reset all?
    SS_RESET_ALL_SK, // RDA; Reset all
    SS_RESET_CALL_COST, // RDA; Reset call cost
    SS_RESET_COMPLETED_PHONE_WILL_BE_RESTARTED, // RDA; Resetting completed. Your phone will be restarted
    SS_RESET_COUNTERS, // RDA; Reset counters
    SS_RESET_DIALLED_N_CALL_TIMES_Q, // RDA; Reset dialled\ncall times?
    SS_RESET_LAST_CALL_TIME_Q, // RDA; Reset last call time?
    SS_RESET_MCALLDATA, // RDA; Reset
    SS_RESET_MPAST, // RDA; Reset
    SS_RESET_Q, // RDA; Reset?
    SS_RESET_RECEIVED_MESSAGES_Q, // RDA; Reset received messages?
    SS_RESET_RECEIVED_N_CALL_TIMES_Q, // RDA; Reset received\ncall times?
    SS_RESET_SENT_MESSAGES_Q, // RDA; Reset sent messages?
    SS_RESET_SETTINGS, // RDA; Reset settings
    SS_RESET_SETTINGS_Q, // RDA; This will reset all settings without personal data. Continue?
    SS_RESET_SK_ABB, // RDA; Reset
    SS_RESET_SK_LITE, // RDA; Reset
    SS_RESET_TIMERS, // RDA; Reset timers
    SS_RESIZE, // RDA; Resize
    SS_RESIZE_Q, // RDA; Resize?
    SS_RESIZING_FAILED, // RDA; Resizing failed
    SS_RESIZING_FAILED_IMAGE_TOO_BIG, // RDA; Resizing failed. Image too big
    SS_RESIZING_FAILED_NOT_ENOUGH_MEMORY, // RDA; Resizing failed. Not enough memory
    SS_RESIZING_FAILED_NOT_SUITABLE_RESOLUTION, // RDA; Resizing failed. Not suitable resolution
    SS_RESIZING_FAILED_UNABLE_TO_READ_IMAGE_FILE, // RDA; Resizing failed. Unable to read image file
    SS_RESIZING_ING, // RDA; Resizing...
    SS_RESOLUTION, // RDA; Resolution
    SS_RESTORE, // RDA; Restore
    SS_RESTORE_TO_MEMO, // RDA; Restore to Memo
    SS_RESTRICTED, // RDA; Restricted
    SS_RESULTS_LIST, // RDA; Results list
    SS_RESUME, // RDA; Resume
    SS_RESUME_PS_Q, // RDA; Resume %s?
    SS_RESUME_Q, // RDA; Resume?
    SS_RESUME_SK, // RDA; Resume
    SS_RETRIEVAL_ONLY_VIA_CALL, // RDA; Retrieval only via call
    SS_RETRIEVAL_ONLY_VIA_CALL_TMO, // RDA; Retrieval only via call
    SS_RETRIEVED_MCALL, // RDA; Retrieved
    SS_RETRIEVED_MMESSAGE, // RDA; Retrieved
    SS_RETRIEVE_MCALL, // RDA; Retrieve
    SS_RETRIEVE_MMESSAGE, // RDA; Retrieve
    SS_RETRIEVE_OPTION, // RDA; Retrieve option
    SS_RETRIEVE_T_MSGF_SK_ABB, // RDA; Retrieve
    SS_RETRIEVING_FAIL, // RDA; Retrieving failed
    SS_RETRIEVING_ING, // RDA; Retrieving...
    SS_RETRIEVING_MESSAGE_FAILED, // RDA; Retrieving message failed
    SS_RETRIEVING_MMS_ING, // RDA; Retrieving MMS…
    SS_RETRIEVING_PRINTER_ATTRIBUTES_ING, // RDA; Retrieving printer attributes...
    SS_RETRY, // RDA; Retry
    SS_RETRY_Q, // RDA; Retry?
    SS_REVERSE, // RDA; Reverse
    SS_REVIEW, // RDA; Review
    SS_RE_C, // RDA; RE:
    SS_RIBBON, // RDA; Ribbon
    SS_RIGA, // RDA; Riga
    SS_RIGHT, // RDA; Right
    SS_RIGHTS, // RDA; Rights
    SS_RIGHTS_TO_THIS_FILE_EXPIRED, // RDA; The rights to this file have expired
    SS_RIGHT_M_LOCATION, // RDA; Right
    SS_RIGHT_SATAUS, // RDA; Right status
    SS_RINGTONE, // RDA; Ringtone
    SS_RINGTONES, // RDA; Ringtones
    SS_RINGTONE_ITEMS, // RDA; Ringtone items
    SS_RING_TONE, // RDA; Ringtone
    SS_RING_VOLUME, // RDA; Ring volume
    SS_RIODE_BRAZ, // RDA; Riode-Braz
    SS_RIO_BRANCO, // RDA; Rio Branco
    SS_RIO_DE_JANEIRO, // RDA; Rio de Janeiro
    SS_RIQQAH, // RDA; Riqqah
    SS_RIYADH, // RDA; Riyadh
    SS_ROAMING_INCURS_EXTRA_CHARGES_CONTINUE_Q, // RDA; Roaming incurs extra charges. Continue?
    SS_ROAMING_NETWORK, // RDA; Roaming network
    SS_ROAMING_OUTSIDE_HOME_COUNTRY_SYNCHRONISATION_OF_CONTACTS_NOT_POSSIBLE_UNTIL_BACK_IN_HOME_COUNTRY_TMO, // RDA; You are currently roaming outside your home country. Synchronisation of contacts is not possible until you are back in your home country
    SS_ROAMING_OUTSIDE_SYNCHRONISATION_CONTACTS_WILL_BE_MORE_EXPENSIVE_CONTINUE_Q_TMO, // RDA; You are currently roaming outside your home country. Synchronisation of your contacts will therefore be more expensive because roaming data rates apply. Continue?
    SS_ROAMING_SETTINGS_ACTIVATED_MSG_TMO, // RDA; You are currently roaming outside your home country. As set in your voicemail settings you will now be directed to your conventional voicemail
    SS_ROAMING_SETTINGS_NOT_ACTIVATED_MSG_TMO, // RDA; You are currently roaming outside your home country. Access to your visual voicemail will therefore be more expensive because roaming data rates apply. Continue with visual voicemail or use normal non-visual voicemail?
    SS_ROCK, // RDA; Rock
    SS_ROME, // RDA; Rome
    SS_ROOT_CERTIFICATES, // RDA; Root certificates
    SS_ROOT_CERTIFICATE_EXPIRED, // RDA; Root certificate expired
    SS_ROOT_CERTIFICATE_IS_UNTRUSTED, // RDA; Untrusted root certificate
    SS_ROSES, // RDA; Roses
    SS_ROSETTA, // RDA; Rosetta
    SS_ROTATE, // RDA; Rotate
    SS_ROTATE_MVERB, // RDA; Rotate
    SS_RSSI_PARAMETER_EMPTY, // RDA; RSSI parameter empty
    SS_RSS_FAILED, // RDA; Download failed
    SS_RUN_IN_BACKGROUND_Q, // RDA; Run in background?
    SS_RUN_JAVASCRIPT, // RDA; Run JavaScript
    SS_RUSSIAN_INPUT, // RDA; Абв 
    SS_RUSSIAN_LOWERINPUT, // RDA; абв 
    SS_RUSSIAN_UPPERINPUT, // RDA; АбВ
    SS_RUSSIEFA, // RDA; Russeifa
    SS_RUSTAQ, // RDA; Rustaq
    SS_RUWAIS, // RDA; Ruwais
    SS_RUWI, // RDA; Ruwi
    SS_SABAH_AL_SALEM, // RDA; Sabah Al-Salem
    SS_SABHA, // RDA; Sabha
    SS_SABRATAH, // RDA; Sabratah
    SS_SAD, // RDA; Sad
    SS_SAFAR_MEA, // RDA; Safar
    SS_SAFI, // RDA; Safi
    SS_SAFWA_CITY, // RDA; Safwa city
    SS_SAHAB, // RDA; Sahab
    SS_SAHAM, // RDA; Saham
    SS_SAHAR, // RDA; Sahar
    SS_SAIDA, // RDA; Saïda
    SS_SAKAKAH, // RDA; Sakakah
    SS_SALALAH, // RDA; Salalah
    SS_SALE, // RDA; Sale
    SS_SALMIYA, // RDA; Salmiya
    SS_SALWA, // RDA; Salwa
    SS_SAMAIL, // RDA; Samail
    SS_SAMARA, // RDA; Samara
    SS_SAMARRA, // RDA; Samarra
    SS_SAMAWAH, // RDA; Samawa
    SS_SAME_ID_AND_PASSWORD, // RDA; Same ID and password
    SS_SAME_NAME_ALREADY_IN_USE, // RDA; Same name already in use
    SS_SAMOA, // RDA; Samoa
    SS_SAMPLING_FREQUENCY, // RDA; Sampling frequency
    SS_SAMPLING_MUSIC_PLEASE_WAIT, // RDA; Sampling music. Please wait...
    SS_SAMSUN, // RDA; Samsun
    SS_SAMSUNG_APPS_T_MAINMENU_ABB, // RDA; Samsung Apps
    SS_SAMSUNG_FUN_CLUB, // RDA; Samsung Fun Club
    SS_SAMSUNG_PC_STUDIO, // RDA; Samsung PC studio
    SS_SAMSUNG_THEME, // RDA; Samsung theme
    SS_SAMSUNG_TUNE, // RDA; Samsung tune
    SS_SANAA, // RDA; Sana’a
    SS_SANDIEGO_USA, // RDA; San Diego, USA
    SS_SANLIURFA, // RDA; Sanliurfa
    SS_SANTIAGO, // RDA; Santiago
    SS_SAN_FRANCISCO, // RDA; San Francisco
    SS_SAN_SALVADOR, // RDA; San Salvador
    SS_SAO_PAULO, // RDA; Sao Paulo
    SS_SARAJEVO, // RDA; Blazuj, Sarajevo 
    SS_SARGODHA, // RDA; Sargodha
    SS_SAR_E_POL, // RDA; Sar-e Pol
    SS_SASKATCHEWAN, // RDA; Saskatchewan
    SS_SAT, // RDA; Sat
    SS_SATURDAY, // RDA; Saturday
    SS_SAT_FM, // RDA; S
    SS_SAUDI_ARABIA_M_COUNTRYNAME, // RDA; Saudi Arabia
    SS_SAVE, // RDA; Save
    SS_SAVED, // RDA; Saved
    SS_SAVED_AS_PS, // RDA; Saved as %s
    SS_SAVED_CONVERSATIONS, // RDA; Saved conversations
    SS_SAVED_IMAGE, // RDA; Saved image
    SS_SAVED_IN_ANSWER_LIST, // RDA; Saved in answer list
    SS_SAVED_IN_DRAFT, // RDA; Saved in Draft
    SS_SAVED_IN_DRAFTS, // RDA; Saved in Drafts
    SS_SAVED_IN_OUTBOX, // RDA; Saved in Outbox
    SS_SAVED_PAGES, // RDA; Saved pages
    SS_SAVED_TO_SIM, // RDA; Saved to SIM
    SS_SAVED_VOICEMAIL_TMO, // RDA; Saved voicemail
    SS_SAVE_ALL_Q, // RDA; Save all?
    SS_SAVE_AND_SEND, // RDA; Save and send
    SS_SAVE_ANSWER, // RDA; Save answer
    SS_SAVE_AS, // RDA; Save as
    SS_SAVE_AS_TEMPLATE, // RDA; Save as template
    SS_SAVE_ATTACHMENT, // RDA; Save attachment
    SS_SAVE_CONTACT, // RDA; Save?
    SS_SAVE_CONTACT_TO, // RDA; Save contact to
    SS_SAVE_FOCUSED_FILE, // RDA; Save focused file
    SS_SAVE_IMAGE, // RDA; Save image
    SS_SAVE_IN, // RDA; Save in
    SS_SAVE_IN_DRAFTS, // RDA; Save in Drafts
    SS_SAVE_IN_MY_FILES, // RDA; Save in My files
    SS_SAVE_MESSAGE, // RDA; Save message
    SS_SAVE_MNOUN, // RDA; Save
    SS_SAVE_NEW_CONTACTS_TO, // RDA; Save new contacts to
    SS_SAVE_NEW_CONTACT_TO, // RDA; Save new contact to
    SS_SAVE_PAGE_ERROR, // RDA; Save page error
    SS_SAVE_PAGE_UC, // RDA; Save Page
    SS_SAVE_PRIVACY, // RDA; Save
    SS_SAVE_Q, // RDA; Save?
    SS_SAVE_SENT_MESSAGE, // RDA; Save sent message
    SS_SAVE_SK_ABB, // RDA; Save
    SS_SAVE_SK_ABB2, // RDA; Save
    SS_SAVE_TO, // RDA; Save to
    SS_SAVE_TO_DRAFTS_Q, // RDA; Save in Drafts?
    SS_SAVE_TO_PHONEBOOK, // RDA; Save in Phonebook
    SS_SAVE_TO_Q, // RDA; Save to?
    SS_SAVE_WEB_PAGE, // RDA; Save Web page
    SS_SAVING, // RDA; Saving
    SS_SAVING_ERROR_ING, // RDA; Saving error...
    SS_SAVING_FAILED, // RDA; Saving failed
    SS_SAVING_ING, // RDA; Saving...
    SS_SAYHAT, // RDA; Sayhat
    SS_SBPD_CHANGED_ENTRIES_TMO, // RDA; [%d] Changed entries
    SS_SBPD_DELETED_ENTRIES_TMO, // RDA; [%d] Deleted entries
    SS_SBPD_NEW_ENTRIES_TMO, // RDA; [%d] New entries
    SS_SCANNING_NETWORK, // RDA; Scanning network...
    SS_SCANNING_NETWORK_Q, // RDA; Scanning network?
    SS_SCARLET_RED, // RDA; Scarlet red
    SS_SCENE, // RDA; Scene
    SS_SCHEDULE, // RDA; Schedule
    SS_SCHOOL, // RDA; School
    SS_SCRAPER, // RDA; Scraper
    SS_SCRATCH_1, // RDA; Scratch 1
    SS_SCRATCH_2, // RDA; Scratch 2
    SS_SCRATCH_3, // RDA; Scratch 3
    SS_SCRATCH_4, // RDA; Scratch 4
    SS_SCREEN_IS_LOCKED_PRESS_HOLD_KEY_TO_UNLOCK, // RDA; Screen is locked.\nPress the hold key to unlock
    SS_SCREEN_PANNING, // RDA; Screen panning
    SS_SCREEN_SETTINGS, // RDA; Screen settings
    SS_SCULPTED, // RDA; Sculpted
    SS_SDCARD_IN_USE, // RDA; SD card is being used as a mass storage device
    SS_SEARCH, // RDA; Search
    SS_SEARCHING_FOR_DEVICES_ING, // RDA; Searching for devices…
    SS_SEARCHING_FOR_NETWORKS, // RDA; Searching for networks
    SS_SEARCHING_ING, // RDA; Searching...
    SS_SEARCHING_ING_FACEBOOK, // RDA; Searching…
    SS_SEARCHING_ING_FM_NEXT, // RDA; Searching...
    SS_SEARCH_AGAIN, // RDA; Search again
    SS_SEARCH_FOR_NEW_DEIVCES, // RDA; Search for new devices
    SS_SEARCH_FOR_NUMBER, // RDA; Search for number
    SS_SEARCH_HIDDEN_NETWORKS, // RDA; Search for hidden networks
    SS_SEARCH_NEW_DEVICE, // RDA; Search new device
    SS_SEARCH_RESULT, // RDA; Search result
    SS_SEARCH_SK, // RDA; Search
    SS_SEARCH_TEXT, // RDA; Search text
    SS_SEARCH_WORD, // RDA; Search word
    SS_SEA_GREEN, // RDA; Sea green
    SS_SECONDARY_SIM, // RDA; Secondary SIM
    SS_SECONDARY_SIM_SCANNING_NETWORK_Q, // RDA; Secondary SIM. Scanning network?
    SS_SECURE, // RDA; Secure
    SS_SECURE_CONNECTION, // RDA; Secure connection
    SS_SECURE_LOGIN_HAPOP, // RDA; Secure login (APOP)
    SS_SECURE_MODE, // RDA; Secure mode
    SS_SECURITY, // RDA; Security
    SS_SECURITY_CHECK_DISABLE, // RDA; Security check disabled
    SS_SECURITY_CHECK_ENABLE, // RDA; Security check enabled
    SS_SECURITY_EMERGENCY, // RDA; Emergency
    SS_SECURITY_ERROR_EXPIRED, // RDA; Security error: expired server certificate. Continue?
    SS_SECURITY_ERROR_UNTRUSTED, // RDA; Security error: untrusted server certificate. Continue?
    SS_SECURITY_TYPE, // RDA; Security type
    SS_SECURITY_WARNING_UNTRUSTED_SERVER_CERTIFICATE_CONTINUE_Q, // RDA; Security warning. Untrusted server certificate. Continue?
    SS_SEC_LC, // RDA; secs
    SS_SEEB, // RDA; Seeb
    SS_SEK, // RDA; SEK
    SS_SELECT, // RDA; Select
    SS_SELECTED, // RDA; Selected
    SS_SELECTED_MADJ, // RDA; Selected
    SS_SELECTION_OFF, // RDA; Selection off
    SS_SELECTION_ON, // RDA; Selection on
    SS_SELECTION_UNAVAILABLE_LOCAL_SYNCHRONISATION_IN_PROGRESS, // RDA; Selection not available. Local synchronisation in progress...
    SS_SELECT_ACCEPT_KEY_AND_SEND_KEY_TO_ANSWER_INCOMING_CALL, // RDA; Select 'Accept' or press the send key to answer incoming calls
    SS_SELECT_ACTIVATING_SIM, // RDA; Select activating SIM
    SS_SELECT_ALL, // RDA; Select all
    SS_SELECT_AT_LEAST_ONE_CONTACT, // RDA; Select at least one contact
    SS_SELECT_A_IMAGE, // RDA; Select image
    SS_SELECT_CARRIER, // RDA; Select carrier
    SS_SELECT_CONTACT, // RDA; Select contact
    SS_SELECT_DB, // RDA; Select DB
    SS_SELECT_FILE, // RDA; Select file
    SS_SELECT_FOLDER, // RDA; Select folder
    SS_SELECT_GROUP, // RDA; Select group
    SS_SELECT_LOCATION, // RDA; Select location
    SS_SELECT_MAIN_STEREO_HEADSET, // RDA; Select main stereo headset
    SS_SELECT_MESSAGE, // RDA; Select message
    SS_SELECT_NETWORK, // RDA; Select network
    SS_SELECT_NETWORK_M_NOUN, // RDA; Select network
    SS_SELECT_NUMBER, // RDA; Select number
    SS_SELECT_NUMBER_TYPE, // RDA; Select number type
    SS_SELECT_OPTIONS_AND_THEN_UPDATE_TO_SEE_QIBLAH_DIRECTION, // RDA; Select Options and then Update to see Qiblah direction
    SS_SELECT_OTHER_ONE_Q, // RDA; Select other one?
    SS_SELECT_PARTICIPANT, // RDA; Select participant
    SS_SELECT_PROFILE, // RDA; Select profile
    SS_SELECT_SIM, // RDA; Select SIM
    SS_SELECT_SIM_CARD, // RDA; Select SIM card
    SS_SELECT_STATION_NUMBER, // RDA; Select station number
    SS_SELECT_TODO, // RDA; Select All
    SS_SELECT_TYPE, // RDA; Select type
    SS_SELECT_USB_MODE, // RDA; Select USB mode
    SS_SELECT_VGA_OR_MEGAPIXEL, // RDA; Select VGA or Megapixel
    SS_SELF_TUNING, // RDA; Self-tuning
    SS_SELF_TUNING_FAILED_SIGNAL_TOO_WEAK, // RDA; Self-tuning failed. Signal too weak
    SS_SEND, // RDA; Send
    SS_SENDER, // RDA; Sender
    SS_SENDER_REQUESTED_READ_REPORT_SEND_READ_REPORT_Q, // RDA; The sender has requested a read report. Send read report?
    SS_SENDER_VIA_EMAIL, // RDA; Send via Email
    SS_SENDER_VIA_MMS, // RDA; Send via MMS
    SS_SENDING_CALENDAR_FAILED, // RDA; Sending calendar failed
    SS_SENDING_EMAIL_FAILED, // RDA; Sending email failed
    SS_SENDING_FAILED, // RDA; Sending failed
    SS_SENDING_ING, // RDA; Sending...
    SS_SENDING_MEMO_ING, // RDA; Sending memo...
    SS_SENDING_MMS_ING, // RDA; Sending MMS...
    SS_SENDING_MSG, // RDA; Sending msg...
    SS_SENDING_MULTIMEDIA_MESSAGE_FAILED, // RDA; Sending multimedia message failed
    SS_SENDING_NAMECARD_FAILED, // RDA; Sending namecard failed
    SS_SENDING_NAMECARD_ING, // RDA; Sending namecard...
    SS_SENDING_OPTIONS, // RDA; Sending options
    SS_SENDING_READ_REPLY_ING, // RDA; Sending read reply...
    SS_SENDING_SCHEDULE_DATA_ING, // RDA; Sending schedule data...
    SS_SENDING_SOS, // RDA; Sending SOS
    SS_SEND_AGAIN, // RDA; Send again
    SS_SEND_DELIVERY_RECEIPT, // RDA; Send delivery receipt
    SS_SEND_DTMF, // RDA; Send DTMF
    SS_SEND_DTMF_ABB, // RDA; Send DTMF
    SS_SEND_EMAIL, // RDA; Send email
    SS_SEND_FAILED, // RDA; Send failed
    SS_SEND_INFORMATION_VIA_TEXT_MESSAGE, // RDA; Send information via text message
    SS_SEND_KEYS, // RDA; Send keys
    SS_SEND_MESSAGE, // RDA; Send message
    SS_SEND_MESSAGE_Q, // RDA; Send message?
    SS_SEND_ME_A_COPY, // RDA; Send me a copy
    SS_SEND_MMS_TMO, // RDA; Send MMS
    SS_SEND_MULTIMEDIA_MESSAGE, // RDA; Send multimedia message
    SS_SEND_NAMECARD_VIA, // RDA; Send namecard via
    SS_SEND_NOTIFICATIONS_TO_NEW_RECIPIENTS_Q, // RDA; Send notifications to new recipients?
    SS_SEND_NUMBER, // RDA; Send number
    SS_SEND_ONLY, // RDA; Send only
    SS_SEND_ONLY_ORANGE, // RDA; Send only,
    SS_SEND_POSTCARD, // RDA; Send postcard
    SS_SEND_QUERY, // RDA; Send query
    SS_SEND_READ_RECEIPT, // RDA; Send read receipt
    SS_SEND_SCHEDULE_DATA_TO_PS_Q, // RDA; Send schedule data to %s?
    SS_SEND_SMS, // RDA; Send SMS
    SS_SEND_SOS_MESSAGES_DEACTIVATED, // RDA; Send SOS messages deactivated
    SS_SEND_TEXT_MESSAGE, // RDA; Send text message
    SS_SEND_TEXT_MSG, // RDA; Send text msg
    SS_SEND_TO_LIVE_STUDIO_VODA_SA, // RDA; Send to live! Studio
    SS_SEND_TO_PICTURE_ALBUM_VODA_IR, // RDA; Send to Picture Album
    SS_SEND_URL, // RDA; Send URL
    SS_SEND_URL_VIA, // RDA; Send URL via
    SS_SEND_VIA, // RDA; Send via
    SS_SEND_VIA_BLUETOOTH, // RDA; Send via Bluetooth
    SS_SEND_VIA_MULTIMEDIA_MESSAGE, // RDA; Send via multimedia message
    SS_SENT, // RDA; Sent
    SS_SENTBOX, // RDA; Sentbox
    SS_SENTBOX_ABB, // RDA; Sentbox
    SS_SENT_MESSAGES, // RDA; Sent messages
    SS_SEOUL, // RDA; Seoul
    SS_SEOUL_SKOREA, // RDA; Seoul-SKorea
    SS_SEP, // RDA; Sep.
    SS_SEPIA, // RDA; Sepia
    SS_SEPTEMBER, // RDA; September
    SS_SERIAL_CONNECT_WITH_PS_Q, // RDA; Serial connect with %s?
    SS_SERIAL_PORT, // RDA; Serial port
    SS_SERVER, // RDA; Server
    SS_SERVER_ADDRESS, // RDA; Server address
    SS_SERVER_CONNECT_FAILED, // RDA; Server connect failed
    SS_SERVER_ERROR, // RDA; Server error
    SS_SERVER_ID, // RDA; Server ID
    SS_SERVER_LOGIN_ID, // RDA; Server login ID
    SS_SERVER_NOT_RESPONDING, // RDA; Server not responding
    SS_SERVER_NOT_SUPPORT_AUTO_EMAIL_SYNC, // RDA; Server does not support Auto email sync
    SS_SERVER_PASSWORD, // RDA; Server password
    SS_SERVER_TO_PHONE, // RDA; Server to phone
    SS_SERVER_TYPE, // RDA; Server type
    SS_SERVER_UNAVAILABLE, // RDA; Server not available
    SS_SERVER_UNAVAILABLE_TRY_AGAIN_LATER, // RDA; Server not available. Try later
    SS_SERVER_URL, // RDA; Server URL
    SS_SERVICE, // RDA; Service
    SS_SERVICE_CENTRE, // RDA; Service centre
    SS_SERVICE_DENIED, // RDA; Service denied
    SS_SERVICE_INITIALISING_ING, // RDA; Service initialising...
    SS_SERVICE_LIGHT, // RDA; Service light
    SS_SERVICE_LIST, // RDA; Service list
    SS_SERVICE_LOADING, // RDA; Service loading
    SS_SERVICE_LOADING_OPERATION, // RDA; Service loading operation
    SS_SERVICE_NOT_ALLOWED_FROM_THIS_LOCATION, // RDA; Service not allowed in this area
    SS_SERVICE_NOT_AVAILABLE, // RDA; Service not available
    SS_SERVICE_NOT_FOUND, // RDA; Service not found
    SS_SERVICE_NOT_SUBSCRIBED, // RDA; Service not subscribed to
    SS_SERVICE_NUMBER, // RDA; Service number
    SS_SERVICE_NUMBERS, // RDA; Service numbers
    SS_SERVICE_SPECIFIC, // RDA; Service specific
    SS_SERVICE_UNAVAILABLE, // RDA; Service not available
    SS_SERVIER_CERTIFICATES, // RDA; Server certificates
    SS_SETIF, // RDA; Setif
    SS_SETTAT, // RDA; Settat
    SS_SETTING, // RDA; Setting
    SS_SETTINGS, // RDA; Settings
    SS_SETTINGS_MUSIC_OVERRIDES_OTHER_SOUND, // RDA; Music overrides other sounds
    SS_SETTINGS_T_MAINMENU_ABB, // RDA; Settings
    SS_SETTING_ING, // RDA; Setting...
    SS_SETUPWIZARD_COMPLETE, // RDA; Setup Wizard is completed. You can re-run it again at any time from the Settings menu
    SS_SETUP_CURRENCY, // RDA; Setup currency
    SS_SETUP_ERROR, // RDA; Setup error
    SS_SETUP_WIZARD, // RDA; Setup wizard
    SS_SETUP_WIZARD_NOT_COMPLETED, // RDA; Setup wizard did not finish. You can re-run it at any time from Settings
    SS_SET_1_60_MINUTES, // RDA; Set 1-60 minute
    SS_SET_ACCOUNT_INFORMATION_FIRST, // RDA; Set account information first
    SS_SET_ALL_CONDITIONS, // RDA; Conditional
    SS_SET_AS, // RDA; Set as 
    SS_SET_AS_ALARM, // RDA; Set as alarm
    SS_SET_AS_CALLER_ID, // RDA; Set as caller ID
    SS_SET_AS_CALLER_IMAGE, // RDA; Set as caller image
    SS_SET_AS_DEFAULT_NUMBER, // RDA; Set as default number
    SS_SET_AS_HOMEPAGE, // RDA; Set as homepage
    SS_SET_AS_HOME_ZONE, // RDA; Set as home zone
    SS_SET_AS_ICON, // RDA; Set as icon
    SS_SET_AS_MESSAGE_TONE_Q, // RDA; Set as message tone?
    SS_SET_AS_N_RINGTONE_Q, // RDA; Set as\nringtone?
    SS_SET_AS_N_WALLPAPER_Q, // RDA; Set as\nwallpaper?
    SS_SET_AS_RINGTONE, // RDA; Set as ringtone
    SS_SET_AS_RINGTONE_Q, // RDA; Set as ringtone?
    SS_SET_AS_SECOND_CLOCK, // RDA; Set as second clock
    SS_SET_AS_T_POPUP, // RDA; Set as
    SS_SET_AS_WALLPAPER, // RDA; Set as wallpaper
    SS_SET_BRIGHTNESS, // RDA; Set brightness
    SS_SET_CALL_TIME_LIMIT, // RDA; Set call time limit
    SS_SET_COLOUR, // RDA; Set colour
    SS_SET_DAYLIGHT_SAVING_TIME, // RDA; Set daylight saving time
    SS_SET_MAX_COST, // RDA; Set max. cost
    SS_SET_MAX_COST_CHANGED, // RDA; Set max. cost changed
    SS_SET_MCOMMAND, // RDA; Set
    SS_SET_MEMORY_CARD_AS_DEFAULT_STORAGE_Q, // RDA; Set memory card as default storage?
    SS_SET_MULTI_PHONEBOOK, // RDA; Set multi-phonebook
    SS_SET_NAME, // RDA; Set name
    SS_SET_OFFLINE_MODE_OFF_NETWORK_SERVICES, // RDA; Set Offline mode off for network services
    SS_SET_PHONEBOOK_STYLE, // RDA; Set phonebook style
    SS_SET_PROFILE_INFORMATION, // RDA; Set profile information
    SS_SET_SIM, // RDA; Set SIM
    SS_SET_SK, // RDA; Set
    SS_SET_TIME, // RDA; Set time
    SS_SET_TIMER, // RDA; Set timer
    SS_SET_TIMER_FOR_CALL_DIVERTING, // RDA; Set timer for call diverting
    SS_SET_T_POP, // RDA; Set
    SS_SEVER_CAPACITY, // RDA; Server capacity
    SS_SFC_MESSAGE, // RDA; SFC Message
    SS_SGD, // RDA; SGD
    SS_SHABAN_MEA, // RDA; Shaban
    SS_SHADOW, // RDA; Shadow
    SS_SHAHRIVAR_MEA, // RDA; Shahrivar
    SS_SHAKER, // RDA; Shaker
    SS_SHAKE_AND_PLAY, // RDA; Shake and play
    SS_SHAKE_PHONE, // RDA; Shake phone
    SS_SHAMSI_FARSI_MEA, // RDA; Shamsi/Farsi
    SS_SHANGHAI_CHINA, // RDA; Shanghai
    SS_SHAPE, // RDA; Shape
    SS_SHAQRAA, // RDA; Shaqraa
    SS_SHARED_KEY, // RDA; Shared key
    SS_SHARED_MEMORY, // RDA; Shared memory
    SS_SHARED_PHONEBOOK, // RDA; Shared phonebook
    SS_SHARE_ALL_DATA_ON_MEMORY_CARD_CONTINUE_Q, // RDA; Share all data on memory card. Continue?
    SS_SHARE_CONTACT, // RDA; Share contact
    SS_SHARJAH, // RDA; Sharjah
    SS_SHARM_EL_SHEIKH, // RDA; Sharm el-Sheikh
    SS_SHARPEN, // RDA; Sharpen
    SS_SHAWWAL_MEA, // RDA; Shawwal
    SS_SHAYBAH, // RDA; Shaybah
    SS_SHEBA, // RDA; Sheba
    SS_SHEBERGHAN, // RDA; Sheberghan
    SS_SHEIKHUPURA, // RDA; Sheikhupura
    SS_SHIBIN_AL_KAWM, // RDA; Shibin El Kom
    SS_SHIBIN_AL_QANATER, // RDA; Shibin el-Qanater
    SS_SHINAS, // RDA; Shinas
    SS_SHIRAZ, // RDA; Shiraz
    SS_SHOOTING_MODE, // RDA; Shooting mode
    SS_SHORTCUTS, // RDA; Shortcuts
    SS_SHORTCUTS_TOOLBAR, // RDA; Shortcuts toolbar
    SS_SHORT_AND_CURLY, // RDA; Short and curly
    SS_SHORT_FRIDAY, // RDA; F
    SS_SHORT_MONDAY, // RDA; M
    SS_SHORT_SATURDAY, // RDA; S
    SS_SHORT_SUNDAY, // RDA; S
    SS_SHORT_THURSDAY, // RDA; T
    SS_SHORT_TUESDAY, // RDA; T
    SS_SHORT_WEDNESDAY, // RDA; W
    SS_SHORT_WITH_STYLE, // RDA; Short with style
    SS_SHOW, // RDA; Show
    SS_SHOW_ANSWER, // RDA; Show answer
    SS_SHOW_COVER_INDICATORS, // RDA; Show cover indicators
    SS_SHOW_DATE, // RDA; Show date
    SS_SHOW_MY_NUMBER, // RDA; Show my number
    SS_SHOW_STATION_ID, // RDA; Show station ID
    SS_SHOW_URL, // RDA; Show URL
    SS_SHUBRA_AL_KHAYMAH, // RDA; Shubra El-Kheima
    SS_SHUFFLE, // RDA; Shuffle
    SS_SHUFFLE_OFF, // RDA; Shuffle off
    SS_SHUFFLE_ON, // RDA; Shuffle on
    SS_SHUSHTAR_NEW_TOWN, // RDA; Shushtar New Town
    SS_SHUTTER_SOUND, // RDA; Shutter sound
    SS_SHUWAIKH, // RDA; Shuwaikh
    SS_SIALKOT, // RDA; Sialkot
    SS_SIDEBURNS, // RDA; Sideburns
    SS_SIDE_PART, // RDA; Side part
    SS_SIDE_SWIPE, // RDA; Side sweep
    SS_SIDI_BEL_ABBES, // RDA; Sidi Bel Abbes
    SS_SIGNAL_SOUND_WHEN_LINE_DISCONNECTED, // RDA; There will be a signal sound when the line is disconnected
    SS_SIGNAL_STRENGTH, // RDA; Signal strength
    SS_SIGNATURE, // RDA; Signature
    SS_SIKH_GURBANI_SWA, // RDA; Sikh Gurbani
    SS_SILENT, // RDA; Silent
    SS_SILENT_MODE, // RDA; Silent mode
    SS_SILENT_PROFILE_ON, // RDA; Silent profile on
    SS_SIM, // RDA; SIM
    SS_SIM1_HCARD_INSERTED, // RDA; SIM1
    SS_SIM1_TO_SIM2, // RDA; SIM1 to SIM2
    SS_SIM2_HCARD_INSERTED, // RDA; SIM2
    SS_SIM2_TO_SIM1, // RDA; SIM2 to SIM1
    SS_SIM3_HCARD_INSERTED, // RDA; SIM3
    SS_SIM4_HCARD_INSERTED, // RDA; SIM4
    SS_SIMPLE_SEARCH, // RDA; Simple search
    SS_SIM_1, // RDA; SIM 1
    SS_SIM_2, // RDA; SIM 2
    SS_SIM_ACCESS, // RDA; SIM access
    SS_SIM_AUTO_SWITCH, // RDA; SIM auto switch
    SS_SIM_BUSY, // RDA; SIM busy
    SS_SIM_CARDS_MANAGEMENT, // RDA; SIM Cards Management
    SS_SIM_CARDS_MANAGEMENT_HELP_TEXT_2, // RDA; You can register your network card. Given network card name and icon will be used for distinguishing your network cards.
    SS_SIM_CARDS_NETWORK_PROFILE_NOT_FOUND, // RDA; SIM card's network profile not found
    SS_SIM_CARD_INSERTED, // RDA; SIM card inserted
    SS_SIM_CARD_MANAGEMENT_TRIPLE_HELP_TEXT1, // RDA; In Triple standby status,You can change current network by pressing Network switch key Current network is displayed on indicator. Current network will be used for sending when you call or send text message.Default network used for base on CB, SIMAT.
    SS_SIM_CARD_MANAGEMENT_TRIPLE_HELP_TEXT_PREFERRED_SIM, // RDA; In Triple standby status,You can change current network by pressing Network switch key Current network is displayed on indicator. Current network will be used for sending when you call or send text message.
    SS_SIM_CARD_REMOVED, // RDA; SIM card removed
    SS_SIM_CONTACTS_FIRST, // RDA; SIM contacts first
    SS_SIM_CONTACT_LIST_FULL_DELETE_SOME_ITEMS, // RDA; SIM contact list full. Delete some items
    SS_SIM_ERROR, // RDA; SIM error
    SS_SIM_FULL, // RDA; SIM full
    SS_SIM_INITIALIZING_ING, // RDA; SIM Initialising...
    SS_SIM_LOCATION, // RDA; SIM location
    SS_SIM_LOCK, // RDA; SIM lock
    SS_SIM_LOCK_DEACTIVATED, // RDA; SIM lock deactivated
    SS_SIM_LOCK_PASSED, // RDA; SIM lock passed
    SS_SIM_MEMORY, // RDA; SIM memory
    SS_SIM_MEMORY_FULL, // RDA; SIM memory full
    SS_SIM_MESSAGE, // RDA; SIM message
    SS_SIM_OPTIONS, // RDA; SIM options
    SS_SIM_PASSWORD, // RDA; SIM password
    SS_SIM_PERMANENTLY_BLOCKED, // RDA; SIM permanently blocked
    SS_SIM_PHONE_MESSAGE, // RDA; SIM/Phone message
    SS_SIM_REFRESHING_ING, // RDA; SIM refreshing...
    SS_SIM_SERVICE, // RDA; SIM service
    SS_SIM_SERVICES, // RDA; SIM services
    SS_SIM_SETTINGS, // RDA; SIM settings
    SS_SIM_SWITCHING, // RDA; SIM switching
    SS_SINALA, // RDA; Sinaloa
    SS_SINGAPORE_M_COUNTRYNAME, // RDA; Singapore
    SS_SINGLE, // RDA; Single
    SS_SINGLE_CLOCK, // RDA; Single clock
    SS_SINGLE_CLOCK_IS_SET_WHILE_SHORTCUTS_TOOLBAR_IS_ACTIVE, // RDA; Single clock is set while Shortcuts toolbar is active
    SS_SINGLE_MCLOCK, // RDA; Single
    SS_SIRT, // RDA; Sirte
    SS_SITRAH, // RDA; Sitra
    SS_SIWAH, // RDA; Siwa
    SS_SIZE, // RDA; Size
    SS_SKELETON, // RDA; Skeleton
    SS_SKETCH, // RDA; Sketch
    SS_SKETCHMEMO, // RDA; Sketchmemo
    SS_SKETCH_MEMO, // RDA; Sketch memo
    SS_SKHIRA, // RDA; Skhira
    SS_SKIKDA, // RDA; Skikda
    SS_SKIN, // RDA; Skin
    SS_SKIN_TONE, // RDA; Skin tone
    SS_SKIP_MARKED, // RDA; Skip marked
    SS_SKIP_THIS_TRACK_UNABLE_TO_PLAY_AACAAC_FORMAT_VIA_BLUETOOTH_STEREO_HEADSET, // RDA; Skip this track. Unable to play AAC/AAC+ format via Bluetooth stereo headset
    SS_SKYLINE, // RDA; Skyline
    SS_SLEIGH_BELL, // RDA; Sleigh bell
    SS_SLIDE, // RDA; Slide
    SS_SLIDE_SETTINGS, // RDA; Slide settings
    SS_SLIDE_SHOW, // RDA; Slide show
    SS_SLIDE_TRANSITIONS, // RDA; Slide transitions
    SS_SLIDE_UP, // RDA; Slide up
    SS_SLIDE_UP_TO_USE_LOUDSPEAKER, // RDA; Slide up to use loud speaker
    SS_SLOT_1, // RDA; Slot 1
    SS_SLOT_1_CARD_NAME, // RDA; Slot 1 card name
    SS_SLOT_1_ICON, // RDA; Slot 1 icon
    SS_SLOT_2, // RDA; Slot 2
    SS_SLOT_2_CARD_NAME, // RDA; Slot 2 card name
    SS_SLOT_2_ICON, // RDA; Slot 2 icon
    SS_SLOT_3_CARD_NAME, // RDA; Slot 3 card name
    SS_SLOT_3_ICON, // RDA; Slot 3 icon
    SS_SLOW, // RDA; Slow
    SS_SMAF, // RDA; SMAF
    SS_SMALL, // RDA; Small
    SS_SMALL_BALLOONS, // RDA; Small balloons
    SS_SMALL_MTEXTSIZE, // RDA; Small
    SS_SMALL_M_TEXT_SIZE, // RDA; Small
    SS_SMARTFIT_VIEW_MODE, // RDA; Smart-fit view mode
    SS_SMART_FIT, // RDA; Smart-Fit
    SS_SMART_HOME, // RDA; Smart home
    SS_SMART_IDLE, // RDA; Smart idle
    SS_SMS, // RDA; SMS
    SS_SMSC_SPECIFIC, // RDA; SMS-C specific
    SS_SMS_BARRED, // RDA; SMS barred
    SS_SMS_INITIALISING_ING, // RDA; SMS initialising...
    SS_SMS_MEMORY_FULL, // RDA; SMS memory full
    SS_SMS_READ, // RDA; Read
    SS_SMS_SYNC_IN_PROGRESS, // RDA; SMS synchronising...
    SS_SMTP, // RDA; SMTP
    SS_SMTP_PASSWORD, // RDA; SMTP password
    SS_SMTP_PORT, // RDA; SMTP port
    SS_SMTP_SERVER, // RDA; SMTP server
    SS_SMTP_USER_ID, // RDA; SMTP user ID
    SS_SNOOZE, // RDA; Snooze
    SS_SNOOZE_DURATION, // RDA; Snooze duration
    SS_SNOOZE_REPEAT, // RDA; Snooze repeat
    SS_SOCKET_NUMBER, // RDA; Socket number
    SS_SOFIA, // RDA; Sofia
    SS_SOFTKEY_RESERVED_FOR_MY5, // RDA; Left/right key is reserved for MyFaves
    SS_SOFTWARE_UPDATE, // RDA; Software update
    SS_SOFTWARE_UPDATE_IS_CANCELLED, // RDA; Software update is cancelled
    SS_SOFT_KEY_FONT, // RDA; Softkey font
    SS_SOHAR, // RDA; Sohar
    SS_SOLARIZE, // RDA; Solarize
    SS_SOLID, // RDA; Solid
    SS_SOMEITEM_LOST, // RDA; Some item may be lost.
    SS_SOMESETTING_LOST_CONTINUE_Q, // RDA; Some settings may be lost. Continue?
    SS_SOMETEXT_LOST_CONTINUE_Q, // RDA; Some text may be lost unless character set is changed to Automatic. Continue?
    SS_SOME_FILES_IN_MESSAGE_LOCKED_UNLOCK_Q, // RDA; Some files in the message are locked. Unlock?
    SS_SOME_FILES_LOCKED, // RDA; Some files locked
    SS_SOME_INFOMATION_MIGHT_BE_LOST_CONTINUE_Q, // RDA; Some information might be lost. Continue?
    SS_SOME_OF_THE_DATA_MIGHT_BE_LOST_CONTINUE_Q, // RDA; Some of the data might be lost. Continue?
    SS_SONORA, // RDA; Sonora
    SS_SORMAN, // RDA; Sorman
    SS_SORT_BY, // RDA; Sort by
    SS_SORT_BY_CONTACTS, // RDA; Sort by contacts
    SS_SOS, // RDA; SOS
    SS_SOS_CALL, // RDA; SOS call
    SS_SOS_HELP_INFO_01, // RDA; In an emergency press either end of the volume key 4 times when the phone is closed.
    SS_SOS_HELP_INFO_02, // RDA; This will send the SOS message to your selected recipients.
    SS_SOS_HELP_INFO_04, // RDA; After sending the SOS message all other functionality on the phone will remain unavailable until you press the end key.
    SS_SOS_HELP_INFO_1_C_KEY, // RDA; In an emergency press Cancel key 4 times continuously when keylock is active.
    SS_SOS_HELP_INFO_1_C_SLIDE_BAR, // RDA; In an emergency, press either end of the volume key 4 times when keylock is active.
    SS_SOS_HELP_INFO_O4_TOUCH, // RDA; After sending the SOS message all other functionality on the phone will remain unavailable until key lock is inactive.
    SS_SOS_MESSAGE, // RDA; SOS message
    SS_SOS_MESSAGES, // RDA; SOS messages
    SS_SOUK_AHRAS, // RDA; Souk Ahras
    SS_SOUND, // RDA; Sound
    SS_SOUNDS, // RDA; Sounds
    SS_SOUND_1, // RDA; Sound 1
    SS_SOUND_2, // RDA; Sound 2
    SS_SOUND_3, // RDA; Sound 3
    SS_SOUND_EFFECTS, // RDA; Sound effects
    SS_SOUND_PROFILES, // RDA; Sound profiles
    SS_SPEAKER_OFF_Q, // RDA; Speaker off?
    SS_SPEAKER_ON, // RDA; Speaker on
    SS_SPEAKER_ON_MCALL, // RDA; Speaker on
    SS_SPEAKER_ON_MCALL_Q, // RDA; Speaker on?
    SS_SPEAKER_ON_Q, // RDA; Speaker on?
    SS_SPEAKER_ON_Q_M_SPAN_CALL, // RDA; Speaker on?
    SS_SPEAKER_VOLUME, // RDA; Speaker volume
    SS_SPEECH_FOLLOWED_BY_ASYNCHRONOUS_DATA_SERVICE, // RDA; Speech followed by asynchronous data service
    SS_SPEECH_FOLLOWED_BY_SYNCHRONOUS_DATA_SERVICE, // RDA; Speech followed by synchronous data service
    SS_SPEED, // RDA; Speed
    SS_SPEED_DIAL, // RDA; Speed dial
    SS_SPELL_SK, // RDA; Spell
    SS_SPELL_WORD, // RDA; Spell word
    SS_SPIKED1, // RDA; Spiked 1
    SS_SPIKED_RANDOM, // RDA; Spiked random
    SS_SPKR_OFF_SK_ABB, // RDA; Spkr off
    SS_SPLIT, // RDA; Split
    SS_SPLIT_HORIZONTAL_IN, // RDA; Split horizontal in
    SS_SPLIT_HORIZONTAL_OUT, // RDA; Split horizontal out
    SS_SPLIT_MULTIPARTY, // RDA; Split multiparty
    SS_SPLIT_T_POP, // RDA; Split
    SS_SPLIT_T_STOPWATCH, // RDA; Split
    SS_SPLIT_VERTICAL_IN, // RDA; Split vertical in
    SS_SPLIT_VERTICAL_OUT, // RDA; Split vertical out
    SS_SPOKENWORD, // RDA; Spokenword
    SS_SPORT, // RDA; Sport
    SS_SPORTS, // RDA; Sports
    SS_SPOTLIGHT, // RDA; Spotlight
    SS_SP_LOCK, // RDA; SP lock
    SS_SP_LOCK_PASSED, // RDA; SP lock passed
    SS_SRI_KRISHNA_JANMASHTAMI_SWA, // RDA; Sri krishna janmashtami
    SS_SSID, // RDA; SSID
    SS_SS_ERROR, // RDA; SS error
    SS_STAMP, // RDA; Stamp
    SS_STANDARD_GREETING, // RDA; Standard greeting
    SS_STANDARD_MADJ, // RDA; Standard
    SS_STANDARD_RATIO, // RDA; Standard ratio
    SS_STANDARD_RATIO_VIEW, // RDA; Standard ratio view
    SS_STAR, // RDA; Star
    SS_STARS, // RDA; Stars
    SS_START, // RDA; Start
    SS_STARTING_DAY, // RDA; Starting day
    SS_STARTS_WITH, // RDA; Starts with
    SS_START_APPLICATION_Q, // RDA; Start application?
    SS_START_AUTOMATIC_SEARCHING_Q_AVAILABLE_STATIONS_WILL_BE_SEARCHED_AND_SAVED, // RDA; Start Auto scan? Available stations will be scanned and saved
    SS_START_DATE, // RDA; Start date
    SS_START_DATE_CHANGED_TO_DDMMYYYY, // RDA; Start date changed to dd/mm/yyyy
    SS_START_DATE_CHANGED_TO_PS, // RDA; Start date changed to %s
    SS_START_DATE_CHANGED_TO_PS_BY_REPEAT_SETTING, // RDA; Start date changed to %s by repeat setting
    SS_START_DATE_CHANGED_TO_YYYYMMDD_BY_REPEAT_SETTINGS, // RDA; Start date changed to yyyy/mm/dd by repeat setting
    SS_START_DEVICE_MANAGEMENT_SESSION_Q, // RDA; Start device management session?
    SS_START_DM, // RDA; Start DM
    SS_START_MGAME, // RDA; Start
    SS_START_PS_TIME_Q, // RDA; Start %s time?
    SS_START_SYNC, // RDA; Start sync
    SS_START_TIME, // RDA; Start time
    SS_START_TIME_AND_DATE, // RDA; Start time and date
    SS_START_TIME_CUST, // RDA; Start time
    SS_START_TIME_FOR_IMAGE, // RDA; Start time for image
    SS_START_TIME_FOR_SOUND, // RDA; Start time for sound
    SS_START_TIME_FOR_TEXT, // RDA; Start time for text
    SS_START_TIME_FOR_VIDEO, // RDA; Start time for video
    SS_START_TIME_SHOULD_BE_SET_BEFORE_END_TIME, // RDA; Start time should be set before end time
    SS_STAR_1, // RDA; *
    SS_STAR_2, // RDA; **
    SS_STAR_3, // RDA; ***
    SS_STAR_4, // RDA; ****
    SS_STAR_5, // RDA; *****
    SS_STATE_MINCOUNTRY, // RDA; State
    SS_STATE_MSTATUS, // RDA; State
    SS_STATIC, // RDA; Static
    SS_STATIC_DNS, // RDA; Static DNS
    SS_STATION_NAME, // RDA; Station name
    SS_STATUS, // RDA; Status
    SS_STATUS_C, // RDA; Status:
    SS_STATUS_ERROR_C_SERVER_BUSY, // RDA; Status error: Server busy
    SS_STEEL, // RDA; Steel
    SS_STEP, // RDA; Step
    SS_STEPS, // RDA; Steps
    SS_STICKY_NOTE, // RDA; Sticky note
    SS_STK_INPUT_1, // RDA; Valid length: 1
    SS_STK_INPUT_MIN, // RDA; Valid length:
    SS_STK_SENDING, // RDA; Sending
    SS_STK_WAITING_ING, // RDA; Please Wait
    SS_STOCKHOLM, // RDA; Stockholm
    SS_STONE, // RDA; stone
    SS_STONES_MCVT, // RDA; Stones
    SS_STOP, // RDA; Stop
    SS_STOPPED, // RDA; Stopped
    SS_STOPWATCH, // RDA; Stopwatch
    SS_STOPWATCH_CAN_CONTAIN_UP_TO_PD_RECORDS, // RDA; Stopwatch can contain up to %d records
    SS_STOP_DOWNLOAD_ABB, // RDA; Stop download
    SS_STOP_MMUSIC, // RDA; Stop
    SS_STOP_MUSIC_PLAYER_Q, // RDA; Stop Music player?
    SS_STOP_OPERATION, // RDA; Stop operation
    SS_STOP_Q, // RDA; Stop?
    SS_STOP_SEARCHING, // RDA; Stop searching
    SS_STORAGE, // RDA; Storage
    SS_STRAW, // RDA; Straw
    SS_STREAMING_VIDEO, // RDA; Streaming video
    SS_STREET, // RDA; Street
    SS_STRIDE, // RDA; Stride
    SS_STRIKETHROUGH, // RDA; Strikethrough
    SS_STRING_TOO_LONG, // RDA; String too long
    SS_STYLE, // RDA; Style
    SS_SUBJECT, // RDA; Subject
    SS_SUBJECTCN_NOT_MATCH_DOMAIN_NAME, // RDA; Subject CN do not match domain name
    SS_SUBJECT_LIST, // RDA; Subject list
    SS_SUBJECT_LOST_CONTINUE_Q, // RDA; Subject may be lost. Continue?
    SS_SUBJECT_REMOVED, // RDA; Subject removed
    SS_SUBNET_MASK, // RDA; Subnet mask
    SS_SUBQCIF_H128_X_96, // RDA; Sub-QCIF (128 x 96)
    SS_SUBSET_LOCK, // RDA; Subset lock
    SS_SUBSET_LOCK_PASSED, // RDA; Subset lock passed
    SS_SUB_BT_STEREO_WITH_BT_HEADSET_DISCONNECTED, // RDA; Sub Bluetooth stereo headset disconnected
    SS_SUCCESS, // RDA; Success
    SS_SUEZ, // RDA; Suez
    SS_SUGGESTIONS, // RDA; Suggestions
    SS_SUGGESTION_1, // RDA; Suggestion 1
    SS_SUGGESTION_2, // RDA; Suggestion 2
    SS_SUGGESTION_3, // RDA; Suggestion 3
    SS_SUGGESTION_4, // RDA; Suggestion 4
    SS_SUGGESTION_5, // RDA; Suggestion 5
    SS_SUKKUR, // RDA; Sukkur
    SS_SULAYMANIYAH, // RDA; Sulaymaniyah
    SS_SUN, // RDA; Sun
    SS_SUNDAY, // RDA; Sunday
    SS_SUNFLOWER, // RDA; Sunflower
    SS_SUNGLASSES, // RDA; Sunglasses
    SS_SUNLIGHT_MODE, // RDA; Sunlight mode
    SS_SUNSET, // RDA; Sunset
    SS_SUN_FM, // RDA; S
    SS_SUPERFINE, // RDA; Superfine
    SS_SUPERMAN_CURL, // RDA; Superman curl
    SS_SUPPLIER, // RDA; Supplier
    SS_SUR, // RDA; Sur
    SS_SURE_TO_TRANSFER, // RDA; Sure to transfer?
    SS_SURPRISE, // RDA; Surprise
    SS_SURRA, // RDA; Surra
    SS_SURROUND, // RDA; Surround
    SS_SURT, // RDA; Surt
    SS_SWAPPED, // RDA; Swapped
    SS_SWAPPED_TO_PS, // RDA; Swapped to %s
    SS_SWAP_SK_LITE, // RDA; Swap
    SS_SWEET_PINK, // RDA; Sweet pink
    SS_SWITCH, // RDA; Switch
    SS_SWITCH_TO_HEADSET, // RDA; Switch to headset
    SS_SWITCH_TO_PHONE, // RDA; Switch to phone
    SS_SWITCH_TO_PRIVATE_CALL_WITH_HEADER, // RDA; Switch to private call with
    SS_SW_WELLCOME, // RDA; Welcome to the %s setup wizard. The following screens will help you configure your phone. Continue?
    SS_SYDNEY, // RDA; Sydney
    SS_SYMBOLIC, // RDA; Symbolic
    SS_SYMBOLS, // RDA; Symbols
    SS_SYNCHRONISATION, // RDA; Synchronisation
    SS_SYNCHRONISATION_COMPLETED, // RDA; Synchronisation completed
    SS_SYNCHRONISATION_FAILED, // RDA; Synchronisation failed
    SS_SYNCHRONISE, // RDA; Synchronise
    SS_SYNCRONISER, // RDA; Synchroniser
    SS_SYNCRONIZING_ING, // RDA; Synchronising...
    SS_SYNC_CATEGORY, // RDA; Sync category
    SS_SYNC_FAILED, // RDA; Sync failed
    SS_SYNC_PROFILE, // RDA; Sync profile
    SS_SYNC_TYPE, // RDA; Sync type
    SS_SYNTAX_ERROR, // RDA; Syntax error
    SS_SYRIA, // RDA; Syria
    SS_SYSTEM_FAILURE, // RDA; System failure
    SS_S_MSATURDAY, // RDA; S
    SS_S_MSUNDAY, // RDA; S
    SS_S_SPACE, // RDA; 
    SS_T9ASSAMESE_INPUT, // RDA; T9কখগ
    SS_T9BENGALI_INPUT, // RDA; T9কখগ
    SS_T9GUJARATI_INPUT, // RDA; T9કખગ
    SS_T9HINDI_INPUT, // RDA; T9कखग
    SS_T9KANNADA_INPUT, // RDA; T9ಕಖಗ
    SS_T9MALAYALAM_INPUT, // RDA; T9കഖഗ
    SS_T9MARATHI_INPUT, // RDA; T9कखग
    SS_T9ORIYA_INPUT, // RDA; T9କଖଗ
    SS_T9PUNJABI_INPUT, // RDA; T9ਕਖਗ
    SS_T9RUSSIAN_INPUT, // RDA; T9Абв
    SS_T9RUSSIAN_LOWERINPUT, // RDA; T9абв
    SS_T9RUSSIAN_UPPERINPUT, // RDA; T9АбВ
    SS_T9TAMIL_INPUT, // RDA; T9கஙச
    SS_T9TELUGU_INPUT, // RDA; T9కఖగ
    SS_T9UKRAINIAN_INPUT, // RDA; T9yAб
    SS_T9UKRAINIAN_LOWERINPUT, // RDA; T9yaб
    SS_T9UKRAINIAN_UPPERINPUT, // RDA; T9yAБ
    SS_T9_ABC_LC, // RDA; T9 abc
    SS_T9_ABC_SHIFT, // RDA; T9 Abc
    SS_T9_ABC_UC, // RDA; T9 ABC
    SS_T9_MODE, // RDA; T9 mode
    SS_TABA, // RDA; Taba
    SS_TABARKA, // RDA; Tabarka
    SS_TABRIZ, // RDA; Tabriz
    SS_TABUK, // RDA; Tabuk
    SS_TADA, // RDA; Tada
    SS_TAFILAH, // RDA; Tafilah
    SS_TAIF, // RDA; Taif
    SS_TAIPEI, // RDA; Taipei
    SS_TAIZ, // RDA; Taiz
    SS_TAJI, // RDA; Taji
    SS_TAJOURA, // RDA; Tajura
    SS_TAKE_A_PHOTO, // RDA; Take a photo
    SS_TAKE_PHOTO, // RDA; Take photo
    SS_TAKE_PICTURE, // RDA; Take picture
    SS_TALLIN, // RDA; Tallinn
    SS_TALOQAN, // RDA; Taloqan
    SS_TAL_AFAR, // RDA; Tal Afar
    SS_TAMBOURINE, // RDA; Tambourine
    SS_TAMERZA, // RDA; Tamerza
    SS_TAMIL_INPUT, // RDA; கஙச
    SS_TANGERINE, // RDA; Tangerine
    SS_TANGIER, // RDA; Tangier
    SS_TANOMAH, // RDA; Tanomah
    SS_TANTA, // RDA; Tanta
    SS_TAN_TAN, // RDA; Tan Tan
    SS_TAP_CENTRE_OF_TARGET_REPEAT_AS_TARGET_MOVES, // RDA; Tap the centre of the target. Repeat as the target moves
    SS_TARFAYA, // RDA; Tarfaya
    SS_TARHOONA, // RDA; Tarhoona
    SS_TAROUDANT, // RDA; Taroudant
    SS_TAROUT, // RDA; Tarout
    SS_TARTUS, // RDA; Tartus
    SS_TASHKENT, // RDA; Tashkent
    SS_TASK, // RDA; Task
    SS_TASKS, // RDA; Tasks
    SS_TASK_M_APPLICATION_NAME, // RDA; Task
    SS_TASK_N_MEMO, // RDA; Task & Memo
    SS_TASK_SYNC_IN_PROGRESS, // RDA; Task sync in progress
    SS_TASK_SYNC_STARTED, // RDA; Task sync started
    SS_TAWORGHA, // RDA; Taworgha
    SS_TAYMA, // RDA; Tayma
    SS_TAZA, // RDA; Taza
    SS_TEAL_GREEN, // RDA; Teal green
    SS_TEBESSA, // RDA; Tebessa
    SS_TEBOURBA, // RDA; Tebourba
    SS_TEHERAN, // RDA; Tehran
    SS_TEHRAN, // RDA; Tehran
    SS_TELEPHONE, // RDA; Telephone
    SS_TELEPHONE_1, // RDA; Telephone 1
    SS_TELEPHONE_2, // RDA; Telephone 2
    SS_TELEPHONE_3, // RDA; Telephone 3
    SS_TELEPHONE_4, // RDA; Telephone 4
    SS_TELUGU_INPUT, // RDA; కఖగ
    SS_TEL_C, // RDA; Tel:
    SS_TEL_KEPPE, // RDA; Tel Keppe
    SS_TEMPERATURE, // RDA; Temperature
    SS_TEMPLATE, // RDA; Template
    SS_TEMPLATES, // RDA; Templates
    SS_TEMPO, // RDA; Tempo
    SS_TEMPORARY, // RDA; Temporary
    SS_TEMPORARY_FILE_SYSTEM_ERROR, // RDA; Temporary file system error
    SS_TESTOUR, // RDA; Testour
    SS_TEST_MODE, // RDA; Test mode
    SS_TETOUAN, // RDA; Tetouan
    SS_TEXT, // RDA; Text
    SS_TEXT_BROWSING, // RDA; Text browsing
    SS_TEXT_COLOUR, // RDA; Text colour
    SS_TEXT_DISPLAY, // RDA; Text display
    SS_TEXT_LANGUAGE, // RDA; Text language
    SS_TEXT_MESSAGE, // RDA; Text message
    SS_TEXT_MESSAGES, // RDA; Text messages
    SS_TEXT_MESSAGE_FAX, // RDA; Text message/Fax
    SS_TEXT_MESSAGE_MPB, // RDA; Text message
    SS_TEXT_MODE, // RDA; Text mode
    SS_TEXT_POSITION, // RDA; Text position
    SS_TEXT_SELECTION_OFF, // RDA; Text selection off
    SS_TEXT_SELECTION_ON, // RDA; Text selection on
    SS_TEXT_SIZE, // RDA; Text size
    SS_TEXT_STYLE, // RDA; Text style
    SS_TEXT_TEMPLATE, // RDA; Text template
    SS_TEXT_TEMPLATES, // RDA; Text templates
    SS_TEXT_TOO_LONG, // RDA; Text too long
    SS_THADIQ, // RDA; Thadig
    SS_THANK_YOU, // RDA; Thank you
    SS_THEME, // RDA; Theme
    SS_THEME_CREATED_ENTER_NAME, // RDA; Theme created. Enter name
    SS_THEME_NOT_CREATED_YET_END_WIZARD_Q, // RDA; Theme is not created yet. End wizard?
    SS_THERE_WILL_BE_ALERT_SOUNDS_DURING_CALL, // RDA; There will be alert sounds during call.
    SS_THERE_WILL_BE_SIGNAL_SOUNDS_EVERY_MINUTE, // RDA; There will be signal sounds every minute
    SS_THERE_WILL_NOT_BE_ALERT_SOUNDS_DURING_CALL, // RDA; There will not be alert sounds during call.
    SS_THESE_KEYS_ARE_STILL_VALID_DELETE_Q, // RDA; These keys are still valid. Delete?
    SS_THE_EVENT_WILL_BE_ON_LAST_DAY_OF_THE_MONTH, // RDA; Some months have less than 29 days. In those cases, the event will be on last day of the month
    SS_THE_NAME_FIELD_CANNOT_CONTAIN_THESE_CHARACTERS, // RDA; The name field cannot contain these characters: |, ^, {, }, [, ], ~,€ ,＼
    SS_THE_SOFTWARE_UPDATE_PROCESS_WILL_END_AND_APPLICATION_WILL_EXIT, // RDA; The software update process will end and the application will close
    SS_THE_TIME_OF_PHONE_MAY_BE_DIFFERENT_FROM_ACTUAL_LOCAL_TIME, // RDA; The time on the phone may be different from actual local time
    SS_THICKNESS, // RDA; Thickness
    SS_THIRD_PARTIES, // RDA; Third parties
    SS_THIS_ANNIVERSARY_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This anniversary overlaps with another event. Continue?
    SS_THIS_APPOINTMENT_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This appointment overlaps with another event. Continue?
    SS_THIS_BAND_NOT_OPERATE_OUTSIDE_AMERICAS, // RDA; This band may not operate outside the Americas
    SS_THIS_DAY, // RDA; This day
    SS_THIS_EMAIL_WILL_BE_SENT_FROM_PS_ACCOUNT, // RDA; This email will be sent from %s account
    SS_THIS_HOLIDAY_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This holiday overlaps with another event. Continue?
    SS_THIS_IMPORTANT_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This important overlaps with another event. Continue?
    SS_THIS_ITEM_CAN_BE_USED_FOR, // RDA; This item can be used for
    SS_THIS_ITEM_CAN_BE_USED_FOR_1_HOUR_OPEN_IT_NOW_Q, // RDA; This item can be used for 1 hour. Open it now?
    SS_THIS_ITEM_CAN_BE_USED_FOR_1_MORE_TIME_OPEN_IT_NOW_Q, // RDA; This item can be used one more time. Open?
    SS_THIS_ITEM_CAN_BE_USED_FOR_XXX_MORE_TIMES_OPEN_IT_NOW_Q, // RDA; This item can be used %d more times. Open?
    SS_THIS_ITEM_CAN_BE_USED_STARING_FROM_PS, // RDA; This item can be used starting from %s
    SS_THIS_ITEM_WILL_BE_DELETED, // RDA; This item will be deleted
    SS_THIS_KEY_IS_STILL_VALID_DELETE_Q, // RDA; This key is still valid. Delete?
    SS_THIS_MEDIA_HAS_UNLIMITED_LICENSE, // RDA; This media has unlimited licence
    SS_THIS_MISCELLANY_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This miscellany overlaps with another event. Continue?
    SS_THIS_MONTH, // RDA; This month
    SS_THIS_NETWORK_SUPPORTS_AUTOMATIC_TIMEZONE_UPDATING_CHOOSE_METHOD_C, // RDA; This network supports automatic time zone updating. Choose alternative method:
    SS_THIS_PAGE, // RDA; This page
    SS_THIS_PRIVATE_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This private overlaps with another event. Continue?
    SS_THIS_TASK_OVERLAPS_WITH_ANOTHER_EVENT_CONTINUE_Q, // RDA; This task overlaps with another event. Continue?
    SS_THIS_WEEK, // RDA; This week
    SS_THU, // RDA; Thu
    SS_THUL_HIJJAH_MEA, // RDA; Thul-Hijjah
    SS_THUL_QADAH_MEA, // RDA; Thul-Qadah
    SS_THUMBNAIL, // RDA; Thumbnail
    SS_THUMBNAILS, // RDA; Thumbnails
    SS_THUMBNAIL_VIEW, // RDA; Thumbnail view
    SS_THUMRAIT, // RDA; Thumrait
    SS_THUQBAH, // RDA; Al-Thuqbah
    SS_THURSDAY, // RDA; Thursday
    SS_THUWAL, // RDA; Thuwal
    SS_THU_FM, // RDA; T
    SS_TIARET, // RDA; Tiaret
    SS_TICK_ALL, // RDA; Tick all
    SS_TIKRIT, // RDA; Tikrit
    SS_TILT_OFF, // RDA; Tilt off
    SS_TILT_ON, // RDA; Tilt on
    SS_TIMCLICK, // RDA; TIMClick
    SS_TIME, // RDA; Time
    SS_TIMEOUT, // RDA; Timeout
    SS_TIMER, // RDA; Timer
    SS_TIMER_MCAMCORDER, // RDA; Timer
    SS_TIMER_MCAMERA, // RDA; Timer
    SS_TIMES_LC, // RDA; times
    SS_TIMES_LC_LEFT, // RDA; times left
    SS_TIME_AND_DATE, // RDA; Time and date
    SS_TIME_AND_DATE_MUST_BE_RESET_FOLLOWING_BATTERY_REMOVAL_SET_NOW_Q, // RDA; Time and date must be reset following battery removal. Set now?
    SS_TIME_C, // RDA; Time:
    SS_TIME_FORMAT, // RDA; Time format
    SS_TIME_LC_LEFT, // RDA; time left
    SS_TIME_LIMIT, // RDA; Time limit
    SS_TIME_LIMIT_MUST_BE_LONGER_THAN_0_SECONDS, // RDA; Time limit must be over 0 second
    SS_TIME_MDAY, // RDA; Time
    SS_TIME_OUT, // RDA; Time-out
    SS_TIME_ZONE, // RDA; Time zone
    SS_TIME_ZONE_UPDATING, // RDA; Time zone updating
    SS_TIME_ZONE_UPDATING_METHOD_CHANGED, // RDA; Time zone updating method changed
    SS_TIR_MEA, // RDA; Tir
    SS_TITLE, // RDA; Title
    SS_TITLE_FONT, // RDA; Title font
    SS_TITLE_MMUSIC, // RDA; Title
    SS_TIZNIT, // RDA; Tiznit
    SS_TLEMCEN, // RDA; Tlemcen
    SS_TMOBILE, // RDA; T-Mobile
    SS_TMO_SOFTWARE_UPDATE_CHARGE_BATT, // RDA; Congratulations! T-Mobile has sent you a new software update for your device. But battery life is insufficient. Please charge your phone before updating.
    SS_TMO_SOFTWARE_UPDATE_INSTALL, // RDA; Congratulations! T-Mobile has sent you a new software update for your device. To proceed with install select next.
    SS_TO, // RDA; To
    SS_TOBRUK, // RDA; Tobruk
    SS_TODAY, // RDA; Today
    SS_TOKYO, // RDA; Tokyo
    SS_TON, // RDA; ton
    SS_TONE, // RDA; Tone
    SS_TONE1, // RDA; Tone 1
    SS_TONE2, // RDA; Tone 2
    SS_TONE3, // RDA; Tone 3
    SS_TONE4, // RDA; Tone 4
    SS_TONGA, // RDA; Tonga
    SS_TONGUE_HANGING_OUT, // RDA; Tongue hanging out
    SS_TONNE, // RDA; tonne
    SS_TONNE_MCVT, // RDA; Tonne
    SS_TONS_HUK_MCVT, // RDA; Tons (UK)
    SS_TONS_HUS_MCVT, // RDA; Tons (US)
    SS_TON_HUK, // RDA; ton (UK)
    SS_TOOSHORT_PUK, // RDA; PUK too short
    SS_TOOSHORT_PUK2, // RDA; PUK2 too short
    SS_TOO_MANY_FILES, // RDA; Too many files
    SS_TOO_MANY_LISTS_ENTER_MORE_WORDS, // RDA; Too many lists. Enter more words
    SS_TOO_MANY_RECIPIENTS, // RDA; Too many recipients
    SS_TOO_MANY_USERS, // RDA; Too many users
    SS_TOP, // RDA; Top
    SS_TOP_DIR, // RDA; Top Dir
    SS_TORCH_LIGHT, // RDA; Torch light
    SS_TORCH_OFF, // RDA; Off
    SS_TORCH_ON, // RDA; On
    SS_TORONTO_CANADA, // RDA; Toronto
    SS_TOTAL, // RDA; Total
    SS_TOTAL_CALL_DURATION, // RDA; Total call duration
    SS_TOTAL_CALL_DURATION_TIME, // RDA; Total call duration time
    SS_TOTAL_COST, // RDA; Total cost
    SS_TOTAL_DIALED, // RDA; Total dialled
    SS_TOTAL_DIALLED, // RDA; Total dialled
    SS_TOTAL_MESSAGES, // RDA; Total messages
    SS_TOTAL_RECEIVED_CDATACALL, // RDA; Total received
    SS_TOTAL_RECEIVED_MCALL, // RDA; Total received
    SS_TOTAL_RECEIVED_MCALL_ABB, // RDA; Total received
    SS_TOTAL_RECEIVED_MDATACALL, // RDA; Total received
    SS_TOTAL_SENT, // RDA; Total sent
    SS_TOTAL_SENT_ABB, // RDA; Total sent
    SS_TOUGGOURT, // RDA; Touggourt
    SS_TOWNSVILLE, // RDA; Townsville
    SS_TOY, // RDA; Toy
    SS_TO_ABB_MMESSAGE, // RDA; To
    SS_TO_ACCESS_NETWORK_SERVICES_DEACTIVATE_OFFLINE_N_SELECT_ANOTHER_ONE, // RDA; To access network services, deactivate Offline profile and select another one.
    SS_TO_ACCESS_NETWORK_SERVICES_DEACTIVATE_OFFLINE_PROFILE_N_SELECT_ANOTHER_MESSAGE_SAVED_OUTBOX, // RDA; To access network services, deactivate Offline profile and select another one. Message will be saved in Outbox
    SS_TO_ACCESS_NETWORK_SERVICES_DISABLE_OFFLINE_MODE, // RDA; To access network services, disable Offline mode.
    SS_TO_C, // RDA; To:
    SS_TO_LOWER_YOUR_RISK_OF_HEARING_DAMAGE_DO_NOT_LISTEN_AT_HIGH_VOLUMES_FOR_LONG_PERIODS, // RDA; To lower your risk of hearing damage, do not listen at high volumes for long periods
    SS_TO_MRECIPENT, // RDA; To
    SS_TO_M_FROMTO, // RDA; To
    SS_TO_PHONE, // RDA; To phone
    SS_TO_SIM, // RDA; To SIM
    SS_TO_USE_FAKE_CALL_YOU_MUST_FIRST_ASSIGN_A_SHORTCUT_TO_THE_FUNCTION_IN_AN_EMERGENCY_YOU_CAN_RECEIVE_A_FAKE_CALL_AFTER_A_SET_TIME_MSG, // RDA; To use fake call, you must first assign a shortcut to the function. In an emergency, you can receive a fake call after a set time. Press and hold the volume down key to enable fake call.
    SS_TO_VIEW_THIS_ITEM_THE_RECEIVER_NEEDS_TO_BUY_THE_KEY_ONLINE, // RDA; To view this item, the receiver will need to buy a key online
    SS_TRACKS, // RDA; Tracks
    SS_TRACK_LENGTH, // RDA; Track length
    SS_TRACK_NUMBER, // RDA; Track number
    SS_TRANSFER, // RDA; Transfer
    SS_TRANSFERED, // RDA; Transfered
    SS_TRANSFERRED, // RDA; Transferred
    SS_TRANSFERRING_CALLS_ING, // RDA; Transferring calls...
    SS_TRANSFER_REQUEST_REJECTED, // RDA; Transfer request rejected
    SS_TRANSFER_TO, // RDA; Transfer to
    SS_TRANSFER_TO_BLUETOOTH_STEREO_HEADSET, // RDA; Transfer to Bluetooth stereo headset
    SS_TRANSFER_TO_DUAL_STEREO_HEADSET, // RDA; Transfer to dual stereo headset
    SS_TRANSFER_TO_PHONE, // RDA; Transfer to Phone
    SS_TRANSFER_TO_SINGLE_STEREO_HEADSET, // RDA; Transfer to single stereo headset
    SS_TRANSFORM, // RDA; Transform
    SS_TREBLEBOOSTER, // RDA; Treblebooster
    SS_TREE, // RDA; Tree
    SS_TRIPLE_ACTIVE_SIM_ACT_INFO1, // RDA; Call settings have been changed.
    SS_TRIPLE_ACTIVE_SIM_ACT_INFO2, // RDA; is out of reach, incoming calls will be forwarded to
    SS_TRIPLE_ACTIVE_SIM_ACT_INFO3, // RDA; Call forwarding in case out of reach is deactivated
    SS_TRIPLE_SIM_ACTIVE_HELP_TEXT, // RDA; Enabling Triple SIM always on lets you receive calls from both SIM cards even during calls. Triple SIM always on affects the call waiting and call forwarding settings for each SIM card if out of reach.
    SS_TRIPLE_SIM_ALWAYS_ON, // RDA; Triple SIM always on
    SS_TRIPOLI, // RDA; Tripoli
    SS_TRI_BAND, // RDA; Tri band
    SS_TRY_AGAIN, // RDA; Try again
    SS_TRY_AGAIN_Q, // RDA; Try again?
    SS_TRY_LATER, // RDA; Try later
    SS_TS_SIM1_TO_SIM2, // RDA; SIM1 to SIM2
    SS_TS_SIM2_TO_SIM3, // RDA; SIM2 to SIM3
    SS_TS_SIM3_TO_SIM1, // RDA; SIM3 to SIM1
    SS_TUE, // RDA; Tue
    SS_TUESDAY, // RDA; Tuesday
    SS_TUE_FM, // RDA; T
    SS_TUNIS, // RDA; Tunis
    SS_TUNISIA_M_COUNTRYNAME, // RDA; Tunisia
    SS_TURAIF, // RDA; Turaif
    SS_TURKEY, // RDA; Turkey
    SS_TURNOFF_HANDFREE, // RDA; Speaker off
    SS_TURNON_HANDFREE, // RDA; Speaker on
    SS_TURN_FAKE_CALL_HOT_KEY_ON_TO_ACTIVATE_THE_FAKE_CALL_KEY, // RDA; Turn Fake call hot key on to activate the fake call key
    SS_TURN_FM_RADIO_ON_Q, // RDA; Turn FM radio on?
    SS_TURN_OFF_RADIO_Q, // RDA; Turn off radio?
    SS_TV_OUT, // RDA; TV out
    SS_TV_OUT_SETTINGS, // RDA; TV-out settings
    SS_TWO_MORE_USES_OF_PS, // RDA; You have two more uses of %s
    SS_TYPE, // RDA; Type
    SS_TYPE_1, // RDA; Type 1
    SS_TYPE_2, // RDA; Type 2
    SS_TYPE_3, // RDA; Type 3
    SS_TYPE_C, // RDA; Type:
    SS_TYPE_MNOUN, // RDA; Type
    SS_T_MTHURSDAY, // RDA; T
    SS_T_MTUESDAY, // RDA; T
    SS_UAE, // RDA; UAE
    SS_UBARI, // RDA; Ubari
    SS_UDHAILIYAH, // RDA; Udhailiyah
    SS_UDISK_USB_CONNECTED, // RDA; Connected with PC
    SS_UGNO_SAHALINSK, // RDA; Ugno-Sahalinsk
    SS_UKRAINIAN_INPUT, // RDA; Абв 
    SS_UKRAINIAN_LOWERINPUT, // RDA; абв 
    SS_UKRAINIAN_UPPERINPUT, // RDA; АбВ
    SS_UMM_AL_QAIWAIN, // RDA; Umm al-Qaiwain
    SS_UMM_BAB, // RDA; Umm Bab
    SS_UMM_QASR, // RDA; Umm Qasr
    SS_UMM_SALAL_MUHAMMAD, // RDA; Umm Salal Muhammad
    SS_UNABALE_TO_CHANGE_VIEW_MODE, // RDA; Unable to change view mode
    SS_UNABLE_ADD_EMAIL_REPLACE_Q, // RDA; Unable to add more emails. Replace existing?
    SS_UNABLE_SELECT_MORE_NUMBER_REPLACE, // RDA; Unable to add more phone numbers. Replace existing number?
    SS_UNABLE_TO_ACCESS_EMAIL_WHILE_VISUAL_VOICE_MAIL_IS_RUNNING, // RDA; Unable to access Email while Visual voicemail is running
    SS_UNABLE_TO_ACCESS_FILE_SYSTEM, // RDA; Unable to access file system
    SS_UNABLE_TO_ACCESS_INTERNET_GSM_NOT_SUPPORTED_BY_WI_FI_VERIFY_BROWSER_SETTING, // RDA; Unable to access Internet. GSM bearer not supported by Wi-Fi. Verify your browser profile setting
    SS_UNABLE_TO_ACCESS_NETWORK_CHECK_APN_SETTING, // RDA; Unable to access network. Check APN setting
    SS_UNABLE_TO_ACCESS_SERVER, // RDA; Unable to access server
    SS_UNABLE_TO_ACTIVATE_BLUETOOTH, // RDA; Unable to activate Bluetooth
    SS_UNABLE_TO_ACTIVATE_BLUETOOTH_WHEN_OFFLINE_MODE_ON, // RDA; Unable to activate Bluetooth when Offline mode is on
    SS_UNABLE_TO_ACTIVATE_BT_OFFLINEMODE, // RDA; Unable to activate Bluetooth when Offline profile is on
    SS_UNABLE_TO_ACTIVATE_FDN_WHILE_USING_NETWORK, // RDA; Unable to activate FDN while using network
    SS_UNABLE_TO_ACTIVATE_IRDA, // RDA; Unable to activate IrDA
    SS_UNABLE_TO_ACTIVATE_VOICE_CLARITY_WITH_LOUDSPEAKER_ON_DEACTIVATE_Q, // RDA; Unable to activate voice clarity with loudspeaker on. Deactivate?
    SS_UNABLE_TO_ADD, // RDA; Unable to add
    SS_UNABLE_TO_ADD_DRM_FILE, // RDA; Unable to add DRM file
    SS_UNABLE_TO_ADD_ITEM_IMAGE_RESOLUTION_TOO_HIGH_CHANGE_CREATION_MODE, // RDA; Unable to add item. Image resolution too high. Change creation mode
    SS_UNABLE_TO_ADD_ITEM_ITEM_INCOMPATIBLE_CHANGE_CREATION_MODE, // RDA; Unable to add item. Item incompatible. Change creation mode
    SS_UNABLE_TO_ADD_ITEM_MAX_IMAGE_SIZE_EXCEEDED_RESIZE_Q, // RDA; Unable to add item. Max. image size exceeded. \n Resize?
    SS_UNABLE_TO_ADD_MORE_ITEMS, // RDA; Unable to add more items
    SS_UNABLE_TO_ADD_MORE_ITEMS_MAX_C_295KB, // RDA; Unable to add more items. Max: 295 KB
    SS_UNABLE_TO_ADD_MORE_PAGE, // RDA; Unable to add more page
    SS_UNABLE_TO_ADD_RATE_EXCHANGE_RATE_LIST_FULL_DELETE_SOME_ITEMS, // RDA; Unable to add rate. Exchange rate list full. Delete some items
    SS_UNABLE_TO_ADD_SLIDE_MAXIMUM_NUMBER_OF_SLIDES_HPD_REACHED, // RDA; Unable to add slide. Maximum number of slides reached
    SS_UNABLE_TO_CANCEL_DOWNLOADING_EMAILS, // RDA; Unable to cancel downloading emails
    SS_UNABLE_TO_CHANGE_COLOUR_TEXT_N_BG_COLOURS_R_THE_SAME, // RDA; Unable to change colour. Text and background colours are the same
    SS_UNABLE_TO_CLEAR_MEMORY_SOME_FILES_ARE_IN_USE, // RDA; Unable to clear memory. Some files are in use
    SS_UNABLE_TO_COMMUNICATE_TO_T_MOBLIE_SERVER, // RDA; Unable to connect to T-Moblie server
    SS_UNABLE_TO_COMPLETE_NETWORK_CONNECTION_FAILED, // RDA; Unable to complete. Network connection failed
    SS_UNABLE_TO_CONNECT_BECAUSE_BLUETOOTH_NOT_SUPPORTED_BY_PRINTER, // RDA; Unable to connect. Bluetooth not supported by printer
    SS_UNABLE_TO_CONNECT_IN_THIS_AREA, // RDA; Unable to connect in this area
    SS_UNABLE_TO_CONNECT_TO_BROWSER_CREATE_NEW_PROFILE, // RDA; Unable to connect to browser. Create profile
    SS_UNABLE_TO_CONNECT_TO_GPRS_CONNECT_TO_GSM_Q, // RDA; Unable to connect to GPRS. Connect to GSM?
    SS_UNABLE_TO_CONNECT_TO_LAN, // RDA; Unable to connect to LAN
    SS_UNABLE_TO_CONNECT_TO_SERVER, // RDA; Unable to connect to server
    SS_UNABLE_TO_COPY, // RDA; Unable to copy
    SS_UNABLE_TO_COPY_DESTINATION_FOLDER_SAME_AS_SOURCE_FOLDER, // RDA; Unable to copy. Destination folder same as source folder
    SS_UNABLE_TO_CREATE_FOLDER_TOO_MANY_SUBFOLDERS, // RDA; Unable to create folder. Too many subfolders
    SS_UNABLE_TO_DEACTIVATE_BLUETOOTH, // RDA; Unable to deactivate Bluetooth
    SS_UNABLE_TO_DEACTIVATE_IRDA, // RDA; Unable to deactivate IrDA
    SS_UNABLE_TO_DELETE, // RDA; Unable to delete
    SS_UNABLE_TO_DELETE_EMAIL, // RDA; Unable to delete email
    SS_UNABLE_TO_DELETE_FILE_IN_USE, // RDA; Unable to delete. File in use
    SS_UNABLE_TO_DISPLAY_ALL_FILES_4500FILES, // RDA; Unable to display all files. The phone can contain only 4500 files
    SS_UNABLE_TO_DISPLAY_ALL_FILES_EXCEEDING_52_CHAR, // RDA; Unable to display all files. Exceeding 52 characters are not supported in phone
    SS_UNABLE_TO_DISPLAY_PEDOMETER_WHILE_IDLE_WHEN_HOME_SCREEN_SET_TO_DYNAMIC, // RDA; Unable to display pedometer while idle when home screen is set to Dynamic
    SS_UNABLE_TO_DIVIDE_BY_ZERO, // RDA; Unable to divide by zero
    SS_UNABLE_TO_DOWNLOAD_CONTENT_TOO_LARGE, // RDA; Unable to download. Content too large
    SS_UNABLE_TO_FORWARD_MESSAGE_UNSUPPORTED_ITEM_ADDED_CHECK_CREATION_MODE, // RDA; Unable to forward message. Unsupported item added. Check creation mode
    SS_UNABLE_TO_FORWARD_N_FILE_IS_FORWARD_LOCKED, // RDA; Unable to forward.\nFile is forward locked
    SS_UNABLE_TO_HOLD, // RDA; Unable to hold
    SS_UNABLE_TO_INSTALL, // RDA; Unable to install
    SS_UNABLE_TO_INSTALL_BOOTSTRAP, // RDA; Unable to install bootstrap
    SS_UNABLE_TO_LAUNCH_APPLICATION_NOT_ENOUGH_MEMORY, // RDA; Unable to launch application. Not enough memory
    SS_UNABLE_TO_LAUNCH_JAVA_GAME, // RDA; Unable to launch Java game
    SS_UNABLE_TO_MAKE_CALL_CALL_TIME_LIMIT_SET, // RDA; Unable to make call. Call time limit set.
    SS_UNABLE_TO_MAKE_THUMBNAIL, // RDA; Unable to make thumbnail
    SS_UNABLE_TO_MOVE_DESTINATION_FOLDER_SAME_AS_SOURCE_FOLDER, // RDA; Unable to move. Destination folder same as source folder
    SS_UNABLE_TO_MOVE_FILE_IN_USE, // RDA; Unable to move. File in use
    SS_UNABLE_TO_OPEN_DATABASE, // RDA; Unable to open database
    SS_UNABLE_TO_OPEN_DUE_TO_MANY_SUBFOLDERS_ON_MEMORY_CARD, // RDA; Unable to open due to many subfolders on memory card
    SS_UNABLE_TO_OPEN_ERROR_OCCURRED, // RDA; Unable to open. Error occurred
    SS_UNABLE_TO_OPEN_FILE_DAMAGED, // RDA; Unable to open. File damaged
    SS_UNABLE_TO_OVERWRITE, // RDA; Unable to overwrite
    SS_UNABLE_TO_PAIR_WITH_PS_TRY_AGAIN_Q, // RDA; Unable to pair with %s. Try again?
    SS_UNABLE_TO_PERFORM_THIS_ACTION, // RDA; Unable to perform this action
    SS_UNABLE_TO_PLAY_FILE_WITH_BLUETOOTH_STEREO_HEADSET, // RDA; Unable to play file with Bluetooth headset
    SS_UNABLE_TO_PLAY_SOUND_DURING_CALL, // RDA; Unable to play sound during call
    SS_UNABLE_TO_PLAY_SOUND_WITH_BLUETOOTH_HEADSET_PLAY_VIA_PHONE_Q, // RDA; Unable to play sound with Bluetooth headset. Play via phone?
    SS_UNABLE_TO_PLAY_VIA_BLUETOOTH_DUAL_STEREO_HEADSET_ONLY_ONE_DEVICE_IS_SEARCHED, // RDA; Unable to play via Bluetooth dual stereo headset. Only one device is searched
    SS_UNABLE_TO_PRINT_PAGE_SIZE_TOO_BIG, // RDA; Unable to print. Page size too big
    SS_UNABLE_TO_PROCESS_REQUEST, // RDA; Unable to process request
    SS_UNABLE_TO_READ_MAX_COST, // RDA; Unable to read max. cost
    SS_UNABLE_TO_READ_PRICE_UNIT, // RDA; Unable to read Price/Unit
    SS_UNABLE_TO_READ_TOTAL_COST, // RDA; Unable to read total cost
    SS_UNABLE_TO_REBUILD_DB_AND_ACCESS_MUSIC_PLAYER, // RDA; Unable to rebuild database and access Music player
    SS_UNABLE_TO_RECEIVE, // RDA; Unable to receive
    SS_UNABLE_TO_RETRIEVE, // RDA; Unable to retrieve
    SS_UNABLE_TO_RUN_WHILE_SENDING_MMS_EMAIL, // RDA; Unable to run while sending MMS/Email
    SS_UNABLE_TO_RUN_WHILE_SENDING_RECEIVING_MESSAGE, // RDA; Unable to run while sending/receiving message
    SS_UNABLE_TO_SAVE, // RDA; Unable to save
    SS_UNABLE_TO_SAVE_DRM_FILE, // RDA; Unable to save DRM file
    SS_UNABLE_TO_SAVE_ENTER_A_NUMBER_TO_CONTINUE, // RDA; Unable to save. Enter a number to continue.
    SS_UNABLE_TO_SAVE_FILE, // RDA; Unable to save file
    SS_UNABLE_TO_SAVE_MESSAGE_EMPTY, // RDA; Unable to save. Message empty
    SS_UNABLE_TO_SAVE_PAGE, // RDA; Unable to save page
    SS_UNABLE_TO_SAVE_ZERO, // RDA; Unable to save zero
    SS_UNABLE_TO_SEND, // RDA; Unable to send
    SS_UNABLE_TO_SEND_MESSAGE, // RDA; Unable to send message
    SS_UNABLE_TO_SEND_PD_PD_MESSAGES, // RDA; Unable to send %d/%d messages
    SS_UNABLE_TO_SET_RINGTONE_IN_SILENT_PROFILE, // RDA; Unable to set ringtone in Silent profile
    SS_UNABLE_TO_START_BROWSER_WHILE_CALLING, // RDA; Unable to start browser while calling
    SS_UNABLE_TO_SWAP, // RDA; Unable to swap
    SS_UNABLE_TO_UPDATE_MYFAVES_CONTACT_ADD_PS_TO_PHONEBOOK_Q, // RDA; Unable to update MyFaves contact. Add %s to Phonebook?
    SS_UNABLE_TO_USE_FM_RADIO_WHEN_OFFLINE_PROFILE_ACTIVATE, // RDA; Unable to use FM radio when Offline profile is active
    SS_UNABLE_TO_USE_OTHER_FUNCTIONS_WHILE_SYNCHRONISING, // RDA; Unable to use other functions while synchronising
    SS_UNABLE_TO_VIEW_CERTIFICATE_OPEN_BROWSER_FIRST, // RDA; Unable to view certificate. Open Browser first
    SS_UNABLE_TO_WRITE_MEMORY_FULL, // RDA; Unable to write. Memory full
    SS_UNAUTHORISE, // RDA; Unauthorise
    SS_UNAUTHORISED, // RDA; Unauthorised
    SS_UNAUTHORISE_DEVICE, // RDA; Unauthorise device
    SS_UNAVAILABLE, // RDA; Unavailable
    SS_UNAVAILABLE_DURING_CALL, // RDA; Unavailable during call
    SS_UNAVAILABLE_IN_TV_OUT_MODE, // RDA; Unavailable in TV-out mode
    SS_UNAVAILABLE_SERVICE_IN_REMOTE_SIM_MODE_DEACTIVATE_REMOTE_SIM_MODE_Q, // RDA; Unavailable service in Remote SIM mode. Deactivate Remote SIM mode?
    SS_UNAYZAH, // RDA; Unayzah
    SS_UNBLOCKED, // RDA; Unblocked
    SS_UNCONDITIONAL_CALL_FORWARDING_ACTIVE, // RDA; Unconditional call forwarding active
    SS_UNCONDITIONAL_FORWARDING_ACTIVATED, // RDA; Unconditional forwarding activated
    SS_UNDATING_MAY_TAKE_A_FEW_MOMENTS, // RDA; Updating may take a few moments
    SS_UNDERLINE, // RDA; Underline
    SS_UNDONE, // RDA; Undone
    SS_UNDO_MVERB, // RDA; Undo
    SS_UNEXPECTED_EVENT, // RDA; Unexpected event
    SS_UNEXPIRED_ITEMS, // RDA; Unexpired items
    SS_UNICODE, // RDA; Unicode
    SS_UNIT, // RDA; Unit
    SS_UNKNOWN, // RDA; Unknown
    SS_UNKNOWN_ALBUM, // RDA; Unknown album
    SS_UNKNOWN_ARTIST, // RDA; Unknown artist
    SS_UNKNOWN_COMPOSER, // RDA; Unknown composer
    SS_UNKNOWN_ERROR, // RDA; Unknown error
    SS_UNKNOWN_FILE_SIZE, // RDA; Unknown file size
    SS_UNKNOWN_GENRE, // RDA; Unknown genre
    SS_UNKNOWN_M_CALL, // RDA; Unknown
    SS_UNKNOWN_OPERATION, // RDA; Unknown operation
    SS_UNKNOWN_PLAYLIST, // RDA; Unknown playlist
    SS_UNKNOWN_SIZE, // RDA; Unknown size
    SS_UNKNOWN_SUBSCRIBER, // RDA; Unknown subscriber
    SS_UNKNOWN_TITLE, // RDA; Unknown title
    SS_UNKNOWN_USER, // RDA; Unknown user
    SS_UNLIMITED, // RDA; Unlimited
    SS_UNLOCK, // RDA; Unlock
    SS_UNLOCKED, // RDA; Unlocked
    SS_UNLOCK_AND_THEN_OK, // RDA; Unlock and then OK
    SS_UNLOCK_LINE_Q, // RDA; Unlock line?
    SS_UNLOCK_Q, // RDA; Unlock?
    SS_UNMARK_AS_SECOND_CLOCK, // RDA; Unmark as second clock
    SS_UNMARK_SK, // RDA; Unmark
    SS_UNMUTE_KEYTONES_OPT, // RDA; Unmute keytones
    SS_UNMUTE_KEY_TONE, // RDA; Unmute keytone
    SS_UNMUTE_MCALL, // RDA; Unmute
    SS_UNPRINTABLE_DRM_FILE, // RDA; Unprintable DRM file
    SS_UNPROTECTED, // RDA; Unprotected
    SS_UNREACHABLE, // RDA; Unreachable
    SS_UNREAD, // RDA; Unread
    SS_UNREAD_MESSAGES, // RDA; Unread messages
    SS_UNRECOGNISED_ERROR, // RDA; Unrecognised error
    SS_UNREGISTERED_SIM, // RDA; Unregistered SIM
    SS_UNRESTRICTED, // RDA; Unrestricted
    SS_UNSAVED, // RDA; Unsaved
    SS_UNSAVED_VOICEMAIL_TMO, // RDA; Unsaved voicemail
    SS_UNSUPPORTED, // RDA; Unsupported
    SS_UNSUPPORTED_CHARSET, // RDA; Unsupported character
    SS_UNSUPPORTED_CONTENT, // RDA; Unsupported content
    SS_UNSUPPORTED_CONTENT_TYPE, // RDA; Unsupported content type
    SS_UNSUPPORTED_FILE_FORMAT, // RDA; Unsupported file format
    SS_UNSUPPORTED_FILE_TYPE, // RDA; Unsupported file type
    SS_UNSUPPORTED_FORMAT, // RDA; Unsupported format
    SS_UNSUPPORTED_MEDIA_TYPE, // RDA; Unsupported media type
    SS_UNSUPPORTED_MESSAGE, // RDA; Unsupported message
    SS_UNSUPPORTED_RESOLUTION_RESIZE_Q, // RDA; Unsupported resolution. Resize?
    SS_UNSUPPORTED_TIMER_FOR_CALL_DIVERTING, // RDA; Unsupported timer for call diverting
    SS_UNTICK_ALL, // RDA; Untick all
    SS_UNTIL_LC, // RDA; until
    SS_UNTITLED, // RDA; Untitled
    SS_UNTRUSTED_APPLICATION, // RDA; Untrusted application
    SS_UP, // RDA; Up
    SS_UPDATE, // RDA; Update
    SS_UPDATED, // RDA; Updated
    SS_UPDATE_BOTH, // RDA; Update both
    SS_UPDATE_COMPLETED, // RDA; Upgrade complete
    SS_UPDATE_ERROR, // RDA; Update error
    SS_UPDATE_EXISTING, // RDA; Update existing
    SS_UPDATE_FAILED_TRY_AGAIN_Q_TMO, // RDA; Update failed. Try again?
    SS_UPDATE_Q, // RDA; Update?
    SS_UPDATE_SOFTWARE_WARNING_MSG, // RDA; Software update for your phone is available from T-Mobile. This process will not delete any personal data on your phone. Update?
    SS_UPDATE_TAKES_10MIN, // RDA; Update will take approximately 10 minutes. you will not be able to use your phone during this time, including 911 Emergency call.
    SS_UPDATE_THE_TIME_TO_MATCH_NEW_TIME_ZONE_Q, // RDA; Update the time to match new time zone?
    SS_UPDATE_TO_BOTH, // RDA; Update to both
    SS_UPDATE_TO_MOBILE, // RDA; Update to mobile
    SS_UPDATE_TO_PHONE, // RDA; Update to phone
    SS_UPDATE_TO_SERVER, // RDA; Update to server
    SS_UPDATE_WILL_BE_PROCEEDED_AFTER_1_HOUR, // RDA; Update will be proceeded after 1 hour
    SS_UPDATE_WILL_BE_PROCEEDED_AFTER_3_HOURS, // RDA; Update will be proceeded after 3 hours
    SS_UPDATE_WILL_BE_PROCEEDED_AFTER_6_HOURS, // RDA; Update will be proceeded after 6 hours
    SS_UPDATING, // RDA; Updating
    SS_UPDATING_CONTACT_ING, // RDA; Updating contact...
    SS_UPDATING_ICON, // RDA; Updating icon...
    SS_UPDATING_ING, // RDA; Updating...
    SS_UPLOAD_DETAILS, // RDA; Upload details
    SS_UPWARD, // RDA; Upward
    SS_UP_TO_100KB_HALL_ITEMS_AVAILABLE, // RDA; Up to 100KB (All items available)
    SS_UP_TO_100KB_HVIDEOS_NOT_AVAILABLE, // RDA; Up to 100KB (Videos not available)
    SS_UP_TO_11_OPERATORS_AVAILABLE, // RDA; Up to 11 operators available
    SS_UP_TO_15_DIGITS_AVAILABLE, // RDA; Up to 15 digits available
    SS_UP_TO_20_OPERATORS_AVAILABLE, // RDA; Up to 20 operators available
    SS_UP_TO_300KB_HALL_ITEMS_AVAILABLE, // RDA; Up to 300KB (All items available)
    SS_UP_TO_30KB_HTEXT_ONLY, // RDA; Up to 30KB (Text only)
    SS_UP_TO_30KB_HVIDEOS_NOT_AVAILABLE, // RDA; Up to 30KB (Videos not available)
    SS_UP_TO_5_DECIMALS_AVAILABLE, // RDA; Up to 5 decimals available
    SS_URGENT, // RDA; Urgent
    SS_URL, // RDA; URL
    SS_URL_EMPTY, // RDA; URL empty
    SS_URL_NOT_FOUND, // RDA; URL not found
    SS_URMIA, // RDA; Urmia
    SS_USB, // RDA; USB
    SS_USB_CABLE_DISCONNECTED, // RDA; USB cable disconnected
    SS_USB_CHARGING_NOT_AVAILABLE, // RDA; USB charging not available
    SS_USB_IN_USE, // RDA; USB in use
    SS_USB_NOW_IN_USE, // RDA; USB now in use
    SS_USD, // RDA; USD
    SS_USED_MEDIA_NOT_DELETED, // RDA; Used media not deleted
    SS_USED_SPACE, // RDA; Used space
    SS_USED_TO_FORWARD_ETHERNET_DATA_PACKETS_BETWEEN_BLUETOOTH_PHONES_AND_TERMINALS, // RDA; Used to forward Ethernet data packets between Bluetooth phones and terminals
    SS_USED_TO_MAKE_CALLS_VIA_BLUETOOTH_CAR_KIST, // RDA; Used to make incoming and outgoing calls via Bluetooth car kits
    SS_USED_TO_MAKE_CALLS_VIA_BLUETOOTH_HEADSETS, // RDA; Used to make incoming and outgoing calls via Bluetooth headsets
    SS_USED_TO_MAKE_FAX_CALLS_VIA_A_BLUETOOTH_BASE_STATION_OR_A_PC, // RDA; Used to make fax calls via a Bluetooth base station or a PC
    SS_USED_TO_MAKE_SPEECH_CALLS_BETWEEN_BLUETOOTH_PHONES_AND_TERMINALS, // RDA; Used to make speech calls between Bluetooth phones and terminals
    SS_USED_TO_MAKE_SPEECH_CALLS_VIA_A_BLUETOOTH_BASE_STATION_OR_A_PC, // RDA; Used to make speech calls via a Bluetooth base station or a PC
    SS_USED_TO_PROVIDE_ACCESS_TO_A_SIM_CARD_VIA_BLUETOOTH, // RDA; Used to provide access to SIM card via Bluetooth
    SS_USED_TO_PROVIDE_ADVANCED_FEATURES_FOR_SENDING_RECEIVING_ARCHIVING_AND_PRINTING_IMAGES_VIA_BLUETOOTH, // RDA; Used to provide advanced features for sending, receiving, archiving and printing images via Bluetooth
    SS_USED_TO_PROVIDE_CLIENT_ACCESS_TO_A_NETWORK_ACCESS_POINT_VIA_BLUETOOTH, // RDA; Used to provide client access to a network access point via Bluetooth
    SS_USED_TO_PROVIDE_ETHERNET_BRIDGE_FEATURES_FOR_ACCESSING_NETWORK_SERVICES_VIA_BLUETOOTH, // RDA; Used to provide Ethernet bridge features for accessing network services via Bluetooth
    SS_USED_TO_PROVIDE_LAN_ACCESS_VIA_BLUETOOTH, // RDA; Used to provide LAN access via Bluetooth
    SS_USED_TO_PROVIDE_REMOTE_CONTROL_FEATURES_FOR_OTHER_DEVICES_VIA_BLUETOOTH, // RDA; Used to provide remote control features for other devices via Bluetooth
    SS_USED_TO_PROVIDE_SYNCHRONISATION_OF_PIM_DATA_BETWEEN_BLUETOOTH_PHONES_AND_TERMINALS, // RDA; Used to provide synchronisation of PIM data between Bluetooth phones and terminals
    SS_USED_TO_TRANSFER_FILES_TO_AND_FROM_BLUETOOTH_DEVICES, // RDA; Used to transfer files to and from Bluetooth devices
    SS_USER, // RDA; User
    SS_USER_AGENT, // RDA; User agent
    SS_USER_AGENT_PROFILE, // RDA; User agent profile
    SS_USER_BUSY, // RDA; User busy
    SS_USER_CREATED_GREETING, // RDA; User created greeting
    SS_USER_FAULT_ON_ILLEGAL_TRIAL, // RDA; USER FAULT ON ILLEGAL TRIAL!
    SS_USER_ID, // RDA; User ID
    SS_USER_NAME, // RDA; User name
    SS_USER_PASSWORD, // RDA; User password
    SS_USER_SELECT_STANDARD_GREETING_AFTER_REMIND_AGAIN_MSG_TMO, // RDA; You will be reminded again in a week. Otherwise you can record your message at any time through the ‘Options’ menu in Visual Voicemail
    SS_USER_SELECT_STANDARD_GREETING_MSG_TMO, // RDA; Callers will hear a standard anonymous greeting. If you later decide to record message you can do this through the ‘Options’ menu in Visual Voicemail
    SS_USE_MOBILBOX_PRO_TMO, // RDA; Use Mobilbox Pro
    SS_USE_PRESET, // RDA; Use preset
    SS_USE_PRESET_SHORTCUTS, // RDA; Use preset shortcuts
    SS_USE_THEME, // RDA; Use theme
    SS_USE_THIS_PICTURE_FOR_MYFAVES_Q, // RDA; Use this picture for MyFaves?
    SS_UYAYNA, // RDA; Uyayna
    SS_UYU, // RDA; UYU
    SS_VALIDITY, // RDA; Validity
    SS_VALIDITY_C, // RDA; Validity:
    SS_VALIDITY_PERIOD, // RDA; Validity period
    SS_VALID_LENGTH_C, // RDA; Valid length:
    SS_VANCOUVER, // RDA; Vancouver
    SS_VASANT_PANCHAMI_SWA, // RDA; Vasant panchami
    SS_VCALENDAR, // RDA; vCalendar
    SS_VCARD, // RDA; vCard
    SS_VEF, // RDA; VEF
    SS_VENDOR, // RDA; Vendor
    SS_VENDOR_C, // RDA; Vendor:
    SS_VERIFYING_ING, // RDA; Verifying...
    SS_VERSION, // RDA; Version
    SS_VERSION_C, // RDA; Version:
    SS_VERY_LOW, // RDA; Very low
    SS_VIA_EMAIL, // RDA; Via Email
    SS_VIA_MESSAGE, // RDA; Via Messages
    SS_VIA_MMS, // RDA; Via MMS
    SS_VIA_SMS, // RDA; Via SMS
    SS_VIBRATION, // RDA; Vibration
    SS_VIBRATION_1, // RDA; Vibration 1
    SS_VIBRATION_2, // RDA; Vibration 2
    SS_VIBRATION_3, // RDA; Vibration 3
    SS_VIBRATION_4, // RDA; Vibration 4
    SS_VIBRATION_5, // RDA; Vibration 5
    SS_VIBRATION_AND_INCREASING, // RDA; Vibration and increasing
    SS_VIBRATION_AND_INCREASING_MELODY, // RDA; Vibration and increasing melody
    SS_VIBRATION_AND_MELODY, // RDA; Vibration and melody
    SS_VIBRATION_FEEDBACK, // RDA; Vibration feedback
    SS_VIBRATION_INTENSITY, // RDA; Vibration intensity
    SS_VIBRATION_PATTERN, // RDA; Vibration pattern
    SS_VIBRATION_THEN_MELODY, // RDA; Vibration then melody
    SS_VIDEO, // RDA; Video
    SS_VIDEOS, // RDA; Videos
    SS_VIDEO_BASIC, // RDA; Video basic
    SS_VIDEO_CALL_RINGTONE, // RDA; Video call ringtone
    SS_VIDEO_CLIPS, // RDA; Video clips
    SS_VIDEO_M_DEFAULTFILENAME, // RDA; Video
    SS_VIDEO_PLAYER, // RDA; Video player
    SS_VIDEO_QUALITY, // RDA; Video quality
    SS_VIDEO_RECORDER, // RDA; Video recorder
    SS_VIDEO_RICH, // RDA; Video rich
    SS_VIENNA, // RDA; Vienna
    SS_VIEW, // RDA; View
    SS_VIEWFINDER, // RDA; Viewfinder
    SS_VIEWFINDER_MODE, // RDA; Viewfinder mode
    SS_VIEW_ANSWERS, // RDA; View answers
    SS_VIEW_AS, // RDA; View as
    SS_VIEW_AS_TEXT_MESSAGE, // RDA; View as text message
    SS_VIEW_AS_VIVID_MESSAGE, // RDA; View as vivid message
    SS_VIEW_ATTACHMENT, // RDA; View attachment
    SS_VIEW_BOOKMARKS, // RDA; View Bookmarks
    SS_VIEW_BY, // RDA; View by
    SS_VIEW_BY_COVER_ART, // RDA; View by cover art
    SS_VIEW_BY_LIST, // RDA; View by list
    SS_VIEW_BY_MONTH, // RDA; View by month
    SS_VIEW_BY_THUMBNAIL, // RDA; View by thumbnail
    SS_VIEW_BY_T_CALENDAR, // RDA; View by
    SS_VIEW_BY_WEEK, // RDA; View by week
    SS_VIEW_CONTACTS_FROM, // RDA; View contacts from
    SS_VIEW_CONTACT_DETAIL, // RDA; View contact detail
    SS_VIEW_CONTACT_DETAILS, // RDA; View contact details
    SS_VIEW_CONTACT_DETAILS_Q, // RDA; View contact details?
    SS_VIEW_LAST_LOG, // RDA; View last log
    SS_VIEW_MNOUN, // RDA; View
    SS_VIEW_MODE, // RDA; View mode
    SS_VIEW_MVERB, // RDA; View
    SS_VIEW_NOW_Q, // RDA; View now?
    SS_VIEW_PHONEBOOK_FROM, // RDA; View phonebook from
    SS_VIEW_RATE, // RDA; View rate
    SS_VIEW_RESULT, // RDA; View result
    SS_VIEW_SAVED_ANSWER, // RDA; View saved answer
    SS_VIEW_SAVED_BOOKMARK, // RDA; View saved bookmark
    SS_VIEW_TYPE, // RDA; View type
    SS_VIJAYA_DASHAMI_SWA, // RDA; Vijaya dashami
    SS_VILNIUS, // RDA; Vilnius
    SS_VIOLET_MTHEME, // RDA; Violet
    SS_VIRTUAL_POINTER, // RDA; Virtual pointer
    SS_VISIBILITY_DISABLED, // RDA; Visibility disabled
    SS_VISIBILITY_ENABLED, // RDA; Visibility enabled
    SS_VISIBLE, // RDA; Visible
    SS_VISUALISATION, // RDA; Visualisation
    SS_VISUAL_VOICEMAIL_BLOCKED_UNABLE_TO_USE_VISUAL_VOICEMAIL_TMO, // RDA; Visual voicemail blocked. Unable to use visual voicemail
    SS_VISUAL_VOICEMAIL_INBOX_TMO, // RDA; Visual voicemail inbox
    SS_VISUAL_VOICEMAIL_TMO, // RDA; Visual voicemail
    SS_VLADIVOSTOK, // RDA; Vladivostok
    SS_VODAFONE_LIVE_E, // RDA; Vodafone live!
    SS_VODAFONE_MAIL, // RDA; Vodafone Mail
    SS_VODAFONE_MESSENGER, // RDA; Vodafone Messenger
    SS_VOD_SETTINGS, // RDA; VOD settings
    SS_VOICE, // RDA; Voice
    SS_VOICE2, // RDA; Voice2
    SS_VOICEMAIL_ACTIVATE_MSG_TMO, // RDA; Your visual voicemail is now active. Messages will be deleted 30 days after receipt unless you ‘save’ them. Would you like to record a greeting message now? If not, your callers will hear a standard greeting instead. You can record a greeting later if you wish
    SS_VOICEMAIL_INBOX_TMO, // RDA; Voicemail inbox
    SS_VOICEMAIL_LA, // RDA; Voicemail(s)
    SS_VOICEMAIL_LIST_FULL_DELETE_SOME_ITEMS_TMO, // RDA; Voicemail list full. Delete some items
    SS_VOICEMAIL_OFF, // RDA; Voicemail off
    SS_VOICEMAIL_ON, // RDA; Voicemail on
    SS_VOICEMAIL_SETTINGS_TMO, // RDA; Voicemail settings
    SS_VOICEMAIL_TMO, // RDA; Voicemail
    SS_VOICE_1, // RDA; Voice 1
    SS_VOICE_2, // RDA; Voice 2
    SS_VOICE_3, // RDA; Voice 3
    SS_VOICE_4, // RDA; Voice 4
    SS_VOICE_CALL, // RDA; Voice call
    SS_VOICE_CALLS, // RDA; Voice calls
    SS_VOICE_CALL_RINGTONE, // RDA; Voice call ringtone
    SS_VOICE_CLARITY_OFF, // RDA; Voice clarity off
    SS_VOICE_CLARITY_ON, // RDA; Voice clarity on
    SS_VOICE_CLIPS, // RDA; Voice clips
    SS_VOICE_COMMANDS, // RDA; Voice command
    SS_VOICE_COMMAND_SETTINGS, // RDA; Voice command settings
    SS_VOICE_GREETING, // RDA; Voice greeting
    SS_VOICE_MAIL_MMESSAGE, // RDA; Voicemail
    SS_VOICE_MAIL_MSERVICE_NAME, // RDA; Voicemail
    SS_VOICE_MAIL_NUMBER, // RDA; Voicemail number
    SS_VOICE_MAIL_SERVER, // RDA; Voicemail server
    SS_VOICE_MESSAGE, // RDA; Voice message
    SS_VOICE_M_DEFAULTFILENAME, // RDA; Voice
    SS_VOICE_M_MESSAGE, // RDA; Voice
    SS_VOICE_OFF_ABB, // RDA; Voice off
    SS_VOICE_ON_ABB, // RDA; Voice on
    SS_VOICE_RECORDER, // RDA; Voice Recorder
    SS_VOICE_SERVER, // RDA; Voice server
    SS_VOICE_SERVER_NOT_ENTERED, // RDA; Voice server not entered
    SS_VOICE_SERVER_NUMBER, // RDA; Voice server number
    SS_VOLUME_1, // RDA; Volume 1
    SS_VOLUME_2, // RDA; Volume 2
    SS_VOLUME_3, // RDA; Volume 3
    SS_VOLUME_4, // RDA; Volume 4
    SS_VOLUME_5, // RDA; Volume 5
    SS_VOLUME_6, // RDA; Volume 6
    SS_VOLUME_7, // RDA; Volume 7
    SS_VOLUME_KEY, // RDA; Volume key
    SS_VOLUME_MSOUND, // RDA; Volume
    SS_VOLUME_M_SOUND, // RDA; Volume
    SS_VOLUME_SETTING, // RDA; Volume setting
    SS_VOLUME_T_CVT, // RDA; Volume
    SS_VOL_SK_M_SOUND_ABB, // RDA; Vol.
    SS_VOL_SK_M_SOUND_ABB2, // RDA; Vol.
    SS_WADI_AD_DAWASIR, // RDA; Wadi ad Dawasir
    SS_WADI_AL_SEER, // RDA; Wadi Al Seer
    SS_WAITING_FOR_RESPONSE, // RDA; Waiting for response...
    SS_WAITING_FOR_WI_FI_CONNECTION, // RDA; Waiting for Wi-Fi connection...
    SS_WAITING_ING, // RDA; Waiting...
    SS_WAITING_TIME, // RDA; Waiting time
    SS_WAIT_UNTIL_GPRS_CLOSES, // RDA; Wait until GPRS closes
    SS_WAKE_UP_ALARM, // RDA; Wake-up alarm
    SS_WALLPAPER, // RDA; Wallpaper
    SS_WALLPAPER_1, // RDA; Wallpaper 1
    SS_WALLPAPER_2, // RDA; Wallpaper 2
    SS_WALLPAPER_IMAGES, // RDA; Wallpaper images
    SS_WAP, // RDA; WAP
    SS_WAP_DISCONNECTED, // RDA; WAP disconnected
    SS_WAP_DOWNLOAD, // RDA; WAP download
    SS_WAP_DOWNLOADS, // RDA; WAP downloads
    SS_WARNING, // RDA; Warning
    SS_WARNING_MMSGCREATIONMODE, // RDA; Warning
    SS_WARP, // RDA; Warp
    SS_WARP_AT_THE_BOTTOM, // RDA; Warp at the bottom
    SS_WARP_IN_THE_CENTRE, // RDA; Warp in the centre
    SS_WARP_RANDOMLY, // RDA; Warp randomly
    SS_WARP_VERTICALLY, // RDA; Warp vertically
    SS_WARP_WITH_SPEED, // RDA; Warp with speed
    SS_WARSAW, // RDA; Warsaw
    SS_WASHIGNTON_DC, // RDA; Washington DC
    SS_WEARING_GLASSES, // RDA; Wearing glasses
    SS_WEATHER_1, // RDA; Weather 1
    SS_WEATHER_2, // RDA; Weather 2
    SS_WEB, // RDA; Web
    SS_WEBPAGE, // RDA; Webpage
    SS_WEB_N_WALK, // RDA; web'n'walk
    SS_WEB_PAGE, // RDA; Web page
    SS_WED, // RDA; Wed
    SS_WEDNESDAY, // RDA; Wednesday
    SS_WED_FM, // RDA; W
    SS_WEEK, // RDA; Week
    SS_WEEKLY, // RDA; Weekly
    SS_WEEKLY_REPEAT, // RDA; Weekly repeat
    SS_WEEK_LC, // RDA; week
    SS_WEIGHT, // RDA; Weight
    SS_WELCOME, // RDA; Welcome
    SS_WELCOME_TO_SAMSUNG_E_YOUR_DEVICE_IS_REGISTERED, // RDA; Welcome to Samsung! Your device is registered
    SS_WELLINGTON, // RDA; Wellington
    SS_WEP, // RDA; WEP
    SS_WHATS_NEW_Q, // RDA; What's new?
    SS_WHEN_PHONE_IS_CLOSED_IN_MUTE_MODE_ALERT_IS_NOT_AVAILABLE_CONTINUE_Q, // RDA; When phone is closed in Mute mode, alert is not available. Continue?
    SS_WHISPER_MODE_OFF, // RDA; Whisper mode off
    SS_WHISPER_MODE_ON, // RDA; Whisper mode on
    SS_WHITE, // RDA; White
    SS_WHITE_BALANCE, // RDA; White Balance
    SS_WHITE_MSUBJECT, // RDA; White
    SS_WHO_CALLED_Q, // RDA; Who called?
    SS_WIDE, // RDA; Wide
    SS_WIDE_SCREEN, // RDA; Wide screen
    SS_WIDTH, // RDA; Width
    SS_WIG, // RDA; Wig
    SS_WIND_BLOWN, // RDA; Wind-blown
    SS_WINE, // RDA; Wine
    SS_WITHHELD, // RDA; Withheld
    SS_WITH_ALARM_TONE_ITEMS, // RDA; With alarm tone items
    SS_WITH_CALLER_ID_ITEMS, // RDA; With caller ID items
    SS_WITH_FM_RADIO_REMINDER_TONE_ITEMS, // RDA; With FM radio reminder tone items
    SS_WITH_PLAYING_ITEM, // RDA; With playing item
    SS_WITH_PROTECTED_ITEMS, // RDA; With protected items
    SS_WITH_RINGTONE_ITEMS, // RDA; With ringtone items
    SS_WITH_SOME_SERVICE_PROVIDERS_THIS_FUNCTION_MAY_NOT_WORK_PROPERLY_OR_YOU_MAY_BE_CHARGED, // RDA; With some service providers, this function may not work properly or you may be charged
    SS_WITH_WALLPAPER_ITEM, // RDA; With wallpaper item
    SS_WIZARDSHAT, // RDA; Wizardshat
    SS_WI_FI, // RDA; Wi-Fi
    SS_WI_FI_HANDOVER_TONE, // RDA; Wi-Fi handover tone
    SS_WI_FI_PAIRING_PROCEDURE_FAILED_START_MANUAL_PAIRING_Q, // RDA; Wi-Fi pairing procedure failed. Start manual pairing?
    SS_WI_FI_SERVICE_DEACTIVATED, // RDA; Wi-Fi service deactivated
    SS_WI_FI_SERVICE_IS_NOT_ACTIVATED_ACTIVATE_WI_FI_SERVICE_Q, // RDA; Wi-Fi service is not activated. Activate Wi-Fi service?
    SS_WMLSCRIPT_ALERT, // RDA; WMLScript - Alert
    SS_WMLSCRIPT_CONFIRM, // RDA; WMLScript - Confirm
    SS_WMLSCRIPT_PROMPT, // RDA; WMLScript - Prompt
    SS_WMS_TIMCLICK_HELP, // RDA; With TIMClick you can print your photo or send it to anyone in the world.
    SS_WOLLONGONG, // RDA; Wollongong
    SS_WORD_BOOK, // RDA; Word book
    SS_WORD_NOT_FOUND, // RDA; Word not found
    SS_WORD_SUGGESTION, // RDA; Word suggestion
    SS_WORKOUT, // RDA; Workout
    SS_WORLD_CLOCK, // RDA; World clock
    SS_WORLD_CLOCK_LIST_FULL_DELETE_SOME_ITEMS, // RDA; World clock list full. Delete some items
    SS_WOW, // RDA; Wow
    SS_WPA, // RDA; WPA
    SS_WPA2, // RDA; WPA2
    SS_WRITE_USER_DATA, // RDA; Write user data
    SS_WRITING_LANGUAGE, // RDA; Writing language
    SS_WRITING_OPTIONS, // RDA; Writing options
    SS_WRONG, // RDA; Wrong
    SS_WRONG_ENTRY, // RDA; Wrong entry
    SS_WRONG_FORMAT, // RDA; Wrong format
    SS_WRONG_NUMBER, // RDA; Wrong number
    SS_WRONG_PASSWORD, // RDA; Wrong password
    SS_WRONG_PASSWORD_TRY_AGAIN_Q_TMO, // RDA; Wrong password. Try again?
    SS_WSS_DM_SESSION_RUNNING, // RDA; DM Session is running
    SS_WSS_DOWNLOAD_IN_PROGRESS, // RDA; Download in progress
    SS_W_MWEDNESDAY, // RDA; W
    SS_W_MWEEK, // RDA; W
    SS_X400, // RDA; X.400
    SS_XXX_PROFILE_ON, // RDA; XXX profile on
    SS_XYLOPHONE, // RDA; Xylophone
    SS_YAHOOSEARCH, // RDA; Yahoo! Search
    SS_YAHOO_E, // RDA; Yahoo!
    SS_YANBU, // RDA; Yanbu
    SS_YARD, // RDA; yard
    SS_YARDS2_MCVT, // RDA; Yards²
    SS_YARDS3_MCVT, // RDA; Yards³
    SS_YARDS_MCVT, // RDA; Yards
    SS_YARIM, // RDA; Yarim
    SS_YEAR, // RDA; Year
    SS_YEARLY, // RDA; Yearly
    SS_YEARLY_REPEAT, // RDA; Yearly repeat
    SS_YEARS_LC, // RDA; years
    SS_YEAR_LC, // RDA; year
    SS_YELLOW, // RDA; Yellow
    SS_YEMEN, // RDA; Yemen
    SS_YES, // RDA; Yes
    SS_YESTERDAY, // RDA; Yesterday
    SS_YES_SK, // RDA; Yes
    SS_YES_SK_ABB, // RDA; Yes
    SS_YES_TO_ALL, // RDA; Yes to all
    SS_YIFRIN, // RDA; Yifrin
    SS_YOUR_MESSAGE_DELETED_WITHOUT_READ, // RDA; Your message has been deleted without being read
    SS_YOUR_MESSAGE_READ, // RDA; Your message has been read
    SS_YOUR_MESSAGE_READ_BY, // RDA; Your message has been read by
    SS_YOUR_PHONE_WILL_NEED_TO_BE_RESTARTED_TO_APPLY_THIS_MODE, // RDA; Your phone will need to be restarted to apply this mode. Continue?
    SS_YOUR_PROFILES_ARE_LOADED_FOR_THE_NEW_SIM_CARD, // RDA; Your profiles are loaded to the new SIM card
    SS_YOUTH_PROTECTION, // RDA; Youth protection
    SS_YOU_ARE_ABOUT_TO_GO_ONLINE, // RDA; You are about to go online
    SS_YOU_ARE_REGISTERED_AS_SOS_MESSAGE_RECIPIENT, // RDA; You are registered as an SOS message recipient.
    SS_YOU_CAN_DELETE_OLD_EMAILS_AUTOMATICALLY_EXCEPT_LOCKED_ITEMS, // RDA; You can delete old emails automatically except locked items when memory exceed maximum allowed
    SS_YOU_CAN_INPUT_ONLY_17_DIGITS, // RDA; You can input only 17 digits
    SS_YOU_CAN_SEND_IMAGE_UP_TO_600KB, // RDA; You can send image up to 600KB
    SS_YOU_CAN_SEND_UP_TO_600KB_ALL_ITEMS_AVAILABLE, // RDA; You can send up to 600KB. All items available
    SS_YOU_CAN_USE_PS, // RDA; You can use %s
    SS_YOU_CAN_USE_THIS_ITEM, // RDA; You can use this item
    SS_YOU_CAN_USE_THIS_ITEM_BY_PDPDPD_PD_C_PD_C_PD, // RDA; You can use this item by %d.%d.%d %d:%d:%d
    SS_YOU_CAN_USE_THIS_ITEM_BY_PS, // RDA; You can use this item by %s
    SS_YOU_CAN_USE_THIS_ITEM_FOR_PDPDPD_PD_CPD_CPD, // RDA; You can use this item for %d.%d.%d %d:%d:%d
    SS_YOU_HAVE_SWITCHED_TO_A_CONFERENCE_CALL, // RDA; You have switched to a conference call
    SS_YOU_HAVE_THREE_MORE_USES_OF_PS, // RDA; You have three more uses of %s
    SS_YOU_NEED_TO_GET_LICENSE_FROM_PC_TO_ACTIVATE_THIS_ITEM, // RDA; Get the key from PC to activate this item
    SS_YYYY_MM_DD, // RDA; YYYY/MM/DD
    SS_YYYY_MM_DD_DOT, // RDA; YYYY.MM.DD
    SS_ZAGAZIG, // RDA; Zagazig
    SS_ZAGREB, // RDA; Zagreb
    SS_ZAHEDAN, // RDA; Zahedan
    SS_ZAHLE, // RDA; Zahle
    SS_ZAKHO, // RDA; Zakho
    SS_ZARZIS, // RDA; Zarzis
    SS_ZIP, // RDA; Zip
    SS_ZLITEN, // RDA; Zliten
    SS_ZOOM, // RDA; Zoom
    SS_ZOOM_IN, // RDA; Zoom in
    SS_ZOOM_OUT, // RDA; Zoom out
    SS_ZOOM_SOUND, // RDA; Zoom sound
    SS_ZOOM_VIEW, // RDA; Zoom view
    SS_ZUBAIR, // RDA; Az Zubayr
    SS_ZUHR, // RDA; Zuhr
    SS_ZULFI, // RDA; Al-Zilfi
    SS_ZURICH, // RDA; Zurich
    SS_ZUWARAH, // RDA; Zuwarah
    STXT_ADD, // RDA; Add
    STXT_ALARMS, // RDA; Alarms
    STXT_ANSWER, // RDA; Answer
    STXT_APPLICATIONS, // RDA; Applications
    STXT_BACKLIGHT_10M, // RDA; 10 minutes
    STXT_BACKLIGHT_10S, // RDA; 10 seconds
    STXT_BACKLIGHT_1M, // RDA; 1 minute
    STXT_BACKLIGHT_30S, // RDA; 30 seconds
    STXT_BACKLIGHT_3M, // RDA; 3 minutes
    STXT_BRIGHTNESS_LEVEL, // RDA; Level
    STXT_CALL_BARRING_CONFIRMATION, // RDA; Confirmation
    STXT_CALL_BARRING_ENTER_PASSWORD, // RDA; Enter password
    STXT_CALL_BARRING_PASSWORD_WARNING, // RDA; Password must be 4 digits long
    STXT_CALL_DIVERT_ENTER_VM_NUM_TIP, // RDA; Hold down the '1' key to connect to the voicemail server when you are not in an application or call
    STXT_CANCEL, // RDA; Cancel
    STXT_CHANGE, // RDA; Change
    STXT_CHOICE, // RDA; Select
    STXT_DELETE, // RDA; Delete
    STXT_DETAIL, // RDA; Details
    STXT_DIVERT_TO, // RDA; Divert to
    STXT_ENTER_VM, // RDA; Enter voicemail address
    STXT_EQUAL, // RDA; Equal
    STXT_EXIT, // RDA; Exit
    STXT_FM, // RDA; FM radio
    STXT_IDL_KL_EMRGCY, // RDA; Emergency call?
    STXT_LOGS, // RDA; Logs
    STXT_MAIN_CL, // RDA; Call logs
    STXT_MAIN_GAME, // RDA; Games
    STXT_MAIN_MESSAGE, // RDA; Message
    STXT_MAIN_MULTIMEDIA, // RDA; Multimedia
    STXT_MAIN_PB, // RDA; Contacts
    STXT_MAIN_PLAYER, // RDA; Player
    STXT_MAIN_SETTING, // RDA; Settings
    STXT_MARK, // RDA; Mark
    STXT_MENU, // RDA; Menu
    STXT_MESSAGES, // RDA; Messages
    STXT_MODIFY, // RDA; Modify
    STXT_OK, // RDA; OK
    STXT_OPT, // RDA; Options
    STXT_OPTION, // RDA; Options
    STXT_OPTION_SHORT, // RDA; Options
    STXT_PREVIEW, // RDA; Preview
    STXT_PROFILE_CHANGE_TO, // RDA; Profile changed to
    STXT_REJECT, // RDA; Reject
    STXT_REMOVE, // RDA; Remove
    STXT_RET, // RDA; Ret.
    STXT_RETRY, // RDA; Retry
    STXT_RETURN, // RDA; Back
    STXT_RETURNAWORD, // RDA; Return
    STXT_RF_SELECT_FALSE_PARA, // RDA; Command #*2263 Parameter out of Range
    STXT_RING1, // RDA; Over the horizon.mp3
    STXT_RING10, // RDA; Twinkling bell.mid
    STXT_RING11, // RDA; Basic bell.mid
    STXT_RING12, // RDA; Milky way.mid
    STXT_RING13, // RDA; Baroque.mid
    STXT_RING14, // RDA; Tripping.mid
    STXT_RING15, // RDA; Watercolor painting.mid
    STXT_RING16, // RDA; Song for you.mid
    STXT_RING17, // RDA; Ringing to you.mid
    STXT_RING18, // RDA; School.mid
    STXT_RING19, // RDA; Tempt.mid
    STXT_RING2, // RDA; Kshitiz.mid
    STXT_RING20, // RDA; Wine bottle.mid
    STXT_RING3, // RDA; Dil ke isharey.mid
    STXT_RING4, // RDA; Kavya mayura.mid
    STXT_RING5, // RDA; Inner Spring.mid
    STXT_RING6, // RDA; Deccan.mid
    STXT_RING7, // RDA; Punjabi beat.mid
    STXT_RING8, // RDA; Safar.mid
    STXT_RING9, // RDA; Pure tone.mid
    STXT_SAVE, // RDA; Save
    STXT_SEARCH, // RDA; Search
    STXT_SELECT, // RDA; Select
    STXT_SELECT_PRIVACY_ALL, // RDA; Select all
    STXT_SETTINGS_CALL, // RDA; Call
    STXT_SETTINGS_DISPLAY, // RDA; Display
    STXT_SETTINGS_NETWORK, // RDA; Network
    STXT_SETTINGS_PHONE, // RDA; Phone
    STXT_SETTINGS_SECURITY, // RDA; Security
    STXT_SET_ALERT_TONE, // RDA; Alert tone
    STXT_SET_AUTOMATIC, // RDA; Automatic
    STXT_SET_BACK, // RDA; Back
    STXT_SET_CALL_ALERT, // RDA; Call alert
    STXT_SET_CALL_BARRING, // RDA; Call barring
    STXT_SET_CALL_DIVERTING, // RDA; Call diverting
    STXT_SET_CALL_WAITING, // RDA; Call waiting
    STXT_SET_CANCEL, // RDA; Cancel
    STXT_SET_CLEAR, // RDA; Clear
    STXT_SET_DIALDISPLAY, // RDA; Dialling display
    STXT_SET_DIALDISPLAY_BRIGHTNESS, // RDA; Brightness
    STXT_SET_DIALDISPLAY_COLOR, // RDA; Colour
    STXT_SET_DIALDISPLAY_SIZE, // RDA; Size
    STXT_SET_DIALDISPLAY_SIZE_LARGE, // RDA; Large
    STXT_SET_DIALDISPLAY_SIZE_SMALL, // RDA; Small
    STXT_SET_DIVERTS_CANCEL_ALL, // RDA; Cancel all
    STXT_SET_FLIGHTMODE, // RDA; Flight mode
    STXT_SET_FOLDER, // RDA; Folder
    STXT_SET_FULL_SCREEN, // RDA; Full Screen
    STXT_SET_ITEM, // RDA; Item
    STXT_SET_KEYPAD_TONE, // RDA; Keypad tone
    STXT_SET_LANGUAGE, // RDA; Language
    STXT_SET_MSG_ALERT, // RDA; Message alert
    STXT_SET_OPTIONS, // RDA; Options
    STXT_SET_PREVIEW, // RDA; Preview
    STXT_SET_PRIVACY_ALL, // RDA; All
    STXT_SET_PRIVACY_DEACTIVATED, // RDA; Privacy deactivated
    STXT_SET_PROFILE_NAME, // RDA; Profile name
    STXT_SET_PROFILE_NO, // RDA; No
    STXT_SET_PROFILE_YES, // RDA; Yes
    STXT_SET_SAVE, // RDA; Save
    STXT_SET_SECURITY_ACTIVATE_PIN, // RDA; PIN check must be enabled
    STXT_SET_SECURITY_CHANGEPASSWORD, // RDA; Change Password
    STXT_SET_SECURITY_CONFIRMPASSWORD, // RDA; Confirm Password
    STXT_SET_SECURITY_CONFIRM_NEW_PIN, // RDA; Confirm new PIN
    STXT_SET_SECURITY_CONFIRM_NEW_PIN2, // RDA; Confirm new PIN2
    STXT_SET_SECURITY_CURRENTPASSWORD, // RDA; Current password
    STXT_SET_SECURITY_DIGITS, // RDA; digits]
    STXT_SET_SECURITY_ENTER_NEWPASSWORD, // RDA; Enter new password
    STXT_SET_SECURITY_ENTER_NEW_PIN, // RDA; Enter new PIN
    STXT_SET_SECURITY_ENTER_NEW_PIN2, // RDA; Enter new PIN2
    STXT_SET_SECURITY_ENTER_PIN, // RDA; Enter PIN
    STXT_SET_SECURITY_ENTER_PIN2, // RDA; Enter PIN2
    STXT_SET_SECURITY_ENTER_PUK, // RDA; Enter PUK
    STXT_SET_SECURITY_ENTER_PUK2, // RDA; Enter PUK2
    STXT_SET_SECURITY_ENTER_SIM1_PIN, // RDA; Enter SIM1 PIN
    STXT_SET_SECURITY_ENTER_SIM2_PIN, // RDA; Enter SIM2 PIN
    STXT_SET_SECURITY_ENTER_SIM3_PIN, // RDA; Enter SIM3 PIN
    STXT_SET_SECURITY_INCORRECT_PIN, // RDA; Incorrect PIN 
    STXT_SET_SECURITY_INCORRECT_PIN2, // RDA; Incorrect PIN2 
    STXT_SET_SECURITY_INCORRECT_PUK, // RDA; Incorrect PUK 
    STXT_SET_SECURITY_INCORRECT_PUK2, // RDA; Incorrect PUK2 
    STXT_SET_SECURITY_LEFT_ATTEMPTS, // RDA; attempts left
    STXT_SET_SECURITY_LOCKED, // RDA; Locked
    STXT_SET_SECURITY_NEWPASSWORD, // RDA; New password
    STXT_SET_SECURITY_PASSWORD, // RDA; Password
    STXT_SET_SECURITY_PASSWORD_CHANGED, // RDA; Password changed
    STXT_SET_SECURITY_PASSWORD_MISMATCH, // RDA; Password does not match
    STXT_SET_SECURITY_PHONEPASSWORD, // RDA; Phone password
    STXT_SET_SECURITY_PH_UNLOCKED, // RDA; Phone lock deactivated
    STXT_SET_SECURITY_PIN2_CHANGED, // RDA; PIN2 changed
    STXT_SET_SECURITY_PINPASSWORD, // RDA; Pin Password
    STXT_SET_SECURITY_PIN_CHANGED, // RDA; PIN changed
    STXT_SET_SECURITY_PIN_LOCKED, // RDA; PIN locked
    STXT_SET_SECURITY_PIN_UNLOCKED, // RDA; PIN unlocked
    STXT_SET_SECURITY_PRIVACYPASSWORD, // RDA; Privacy Password
    STXT_SET_SECURITY_SIMPASSWORD, // RDA; SIM Password
    STXT_SET_SECURITY_SIM_UNLOCKED, // RDA; SIM lock deactivated
    STXT_SET_SET, // RDA; Set
    STXT_SET_SHORTCUT, // RDA; Shortcuts
    STXT_SET_SILENT_WARNING, // RDA; Play sound in Silent profile?
    STXT_SET_SIZE, // RDA; Size
    STXT_SET_SMARTHOME, // RDA; Smart home
    STXT_SET_STANDARD_RATIO, // RDA; Standard Ratio
    STXT_SET_VIEW_BY, // RDA; View by
    STXT_SET_WALLPAPER, // RDA; Wallpaper
    STXT_SHORTCUTS_TOOLBAR_WARNING, // RDA; Home screen toolbar can contain up to 15 shortcuts
    STXT_SMS_FOCUSED_NUMBER, // RDA; Focused phone number
    STXT_SOUND_PROFILES, // RDA; Sound profiles
    STXT_STOP, // RDA; Stop
    STXT_STORE_MEDIUM, // RDA; Storage
    STXT_UP, // RDA; ^
    STXT_VIEW, // RDA; View
    STXT_VIEW_CONTACT, // RDA; View contact detail
    STXT_VM_DELETE, // RDA; Delete?
    STXT_VM_NONAME, // RDA; No name
    STXT_VM_NONUMBER, // RDA; No number
    STXT_VOICEMAIL, // RDA; Voicemail
    STXT_VOICE_CALL_RINGTONE, // RDA; Voice call ringtone
    STXT_WAIT, // RDA; Please wait
    STXT_WALLPAPER1_SET_POP, // RDA; Wallpaper 1 set as Wallpaper
    STXT_WALLPAPER1_SIZE, // RDA; 32K
    STXT_WALLPAPER2_SET_POP, // RDA; Wallpaper 2 set as Wallpaper
    STXT_WALLPAPER2_SIZE, // RDA; 38K
    STXT_WALLPAPER3_SET_POP, // RDA; Wallpaper 3 set as Wallpaper
    STXT_WALLPAPER3_SIZE, // RDA; 29K
    STXT_WALLPAPER_SLIDE_EFFECT, // RDA;  Wallpaper 3  Wallpaper 1  Wallpaper 2  Wallpaper 3  Wallpaper 1 
    TS_IN_CALL_BUTTON, // RDA; In call
    TS_MESSAGE_COUNTER, // RDA; Message counter
    TS_NEXT_BUTTON, // RDA; Next
    TS_PREVIOUS_BUTTON, // RDA; Previous
    TS_RECEIVED_MSG, // RDA; Received msg.
    TS_RESTART, // RDA; Restart
    TS_SENT_MSG, // RDA; Sent msg.
    TS_VIEW_URL_BUTTON, // RDA; View URL
    TXTSYS_WAITING, // RDA; Waiting
    TXTUSSD_UNRECOGNIZED, // RDA; Unrecognized
    TXT_ACC_CALCULATOR, // RDA; Calculator
    TXT_ACC_COUNTEDTIME, // RDA; Countdown timer
    TXT_ACC_DIVIDE_BY_ZERO, // RDA; Can't be divided by zero
    TXT_ACC_IMAGINARY, // RDA; Result is imaginary number
    TXT_ACC_OVERFLOW, // RDA; Calculation overflow
    TXT_ACC_REMOVE_DST, // RDA; Cancel daylight saving system
    TXT_ACC_SET_DST, // RDA; Set daylight saving system
    TXT_ACC_SET_LOCAL, // RDA; Set as local
    TXT_ACC_START, // RDA; Start
    TXT_ACC_STOP, // RDA; Stop
    TXT_ACC_WORLD_CLOCK, // RDA; World time
    TXT_ACC_WORLD_GMT_0, // RDA; GMT0
    TXT_ACC_WORLD_GMT_0_CASABLANCA, // RDA; Casablanca
    TXT_ACC_WORLD_GMT_0_CITY, // RDA; London, Lisbon
    TXT_ACC_WORLD_GMT_0_LISBON, // RDA; Lisbon
    TXT_ACC_WORLD_GMT_0_LONDON, // RDA; London
    TXT_ACC_WORLD_GMT_0_RABAT, // RDA; Rabat
    TXT_ACC_WORLD_GMT_10P5_P, // RDA; GMT+10:30
    TXT_ACC_WORLD_GMT_10P5_P_BRISBANE, // RDA; Brisbane
    TXT_ACC_WORLD_GMT_10_N, // RDA; GMT-10
    TXT_ACC_WORLD_GMT_10_N_CITY, // RDA; Honolulu, Hawaii
    TXT_ACC_WORLD_GMT_10_N_HONOLULU, // RDA; Honolulu
    TXT_ACC_WORLD_GMT_10_P, // RDA; GMT+10
    TXT_ACC_WORLD_GMT_10_P_CANBERRA, // RDA; Canberra
    TXT_ACC_WORLD_GMT_10_P_CITY, // RDA; Canberra, Sydney, Guam
    TXT_ACC_WORLD_GMT_10_P_GUAM, // RDA; Guam
    TXT_ACC_WORLD_GMT_10_P_SYDNEY, // RDA; Sydney
    TXT_ACC_WORLD_GMT_10_P_VLADIVOSTO, // RDA; Vladivostok
    TXT_ACC_WORLD_GMT_11P5_P, // RDA; GMT+11:30
    TXT_ACC_WORLD_GMT_11P5_P_NORFOLD_ISLAND, // RDA; Norfolk Island
    TXT_ACC_WORLD_GMT_11_N, // RDA; GMT-11
    TXT_ACC_WORLD_GMT_11_N_APIA, // RDA; Apia
    TXT_ACC_WORLD_GMT_11_N_CITY, // RDA; Samoa, Midway
    TXT_ACC_WORLD_GMT_11_P, // RDA; GMT+11
    TXT_ACC_WORLD_GMT_11_P_CITY, // RDA; Khabarovsk
    TXT_ACC_WORLD_GMT_11_P_SOLOMON, // RDA; Solomon
    TXT_ACC_WORLD_GMT_12P75_P, // RDA; GMT+12:45
    TXT_ACC_WORLD_GMT_12P75_P_TONGA, // RDA; Tonga
    TXT_ACC_WORLD_GMT_12_N, // RDA; GMT-12
    TXT_ACC_WORLD_GMT_12_N_KWAJALEIN, // RDA; Kwajalein Atoll
    TXT_ACC_WORLD_GMT_12_P, // RDA; GMT+12
    TXT_ACC_WORLD_GMT_12_P_AUCKLAND, // RDA; Auckland
    TXT_ACC_WORLD_GMT_12_P_CITY, // RDA; Magadan, Auckland, Wellington
    TXT_ACC_WORLD_GMT_12_P_WELLINGTON, // RDA; Wellington
    TXT_ACC_WORLD_GMT_1_N, // RDA; GMT-1
    TXT_ACC_WORLD_GMT_1_N_CITY, // RDA; Azores
    TXT_ACC_WORLD_GMT_1_N_PRAIA, // RDA; Praia
    TXT_ACC_WORLD_GMT_1_P, // RDA; GMT+1
    TXT_ACC_WORLD_GMT_1_P_AMSTERDAM, // RDA; Amsterdam
    TXT_ACC_WORLD_GMT_1_P_BERLIN, // RDA; Berlin
    TXT_ACC_WORLD_GMT_1_P_BRUSSELS, // RDA; Brussels
    TXT_ACC_WORLD_GMT_1_P_BUDAPEST, // RDA; Budapest
    TXT_ACC_WORLD_GMT_1_P_CITY, // RDA; Paris, Berlin, Rome, Warsaw, Budapest, Praha
    TXT_ACC_WORLD_GMT_1_P_COPENHAGEN, // RDA; Copenhagen
    TXT_ACC_WORLD_GMT_1_P_GENEVA, // RDA; Geneva
    TXT_ACC_WORLD_GMT_1_P_LUXEMBOURG, // RDA; Luxembourg
    TXT_ACC_WORLD_GMT_1_P_MADRID, // RDA; Madrid
    TXT_ACC_WORLD_GMT_1_P_PARIS, // RDA; Paris
    TXT_ACC_WORLD_GMT_1_P_PRAGUE, // RDA; Prague
    TXT_ACC_WORLD_GMT_1_P_ROME, // RDA; Rome
    TXT_ACC_WORLD_GMT_1_P_STOCKHOLM, // RDA; Stockholm
    TXT_ACC_WORLD_GMT_1_P_VIENNA, // RDA; Vienna
    TXT_ACC_WORLD_GMT_1_P_WARSAW, // RDA; Warsaw
    TXT_ACC_WORLD_GMT_1_P_ZURICH, // RDA; Zurich
    TXT_ACC_WORLD_GMT_2_N, // RDA; GMT-2
    TXT_ACC_WORLD_GMT_2_N_CENTRAL_ATLANTIC, // RDA; Central Atlantic
    TXT_ACC_WORLD_GMT_2_N_CITY, // RDA; Mid-Atlantic
    TXT_ACC_WORLD_GMT_2_P, // RDA; GMT+2
    TXT_ACC_WORLD_GMT_2_P_ATHENS, // RDA; Athens
    TXT_ACC_WORLD_GMT_2_P_BUCHAREST, // RDA; Bucharest
    TXT_ACC_WORLD_GMT_2_P_CAIRO, // RDA; Cairo
    TXT_ACC_WORLD_GMT_2_P_CAPE_TOWN, // RDA; Cape Town
    TXT_ACC_WORLD_GMT_2_P_CITY, // RDA; Athens, Helsinki, Istanbul, Cap Town
    TXT_ACC_WORLD_GMT_2_P_ISTANBUL, // RDA; Istanbul
    TXT_ACC_WORLD_GMT_2_P_JOHANNESBURG, // RDA; Johannesburg
    TXT_ACC_WORLD_GMT_2_P_KIEV, // RDA; Kiev
    TXT_ACC_WORLD_GMT_2_P_SOFIA, // RDA; Sofia
    TXT_ACC_WORLD_GMT_3P5_N, // RDA; GMT-3:30
    TXT_ACC_WORLD_GMT_3P5_N_CITY, // RDA; Newfoundland
    TXT_ACC_WORLD_GMT_3P5_N_LAPAZ, // RDA; La paz
    TXT_ACC_WORLD_GMT_3P5_P, // RDA; GMT+3:30
    TXT_ACC_WORLD_GMT_3P5_P_CITY, // RDA; Tehran
    TXT_ACC_WORLD_GMT_3P5_P_TEHRAN, // RDA; Tehran
    TXT_ACC_WORLD_GMT_3_N, // RDA; GMT-3
    TXT_ACC_WORLD_GMT_3_N_BRASILIA, // RDA; Brasilia
    TXT_ACC_WORLD_GMT_3_N_BUENOS_AIRES, // RDA; Buenos Aires
    TXT_ACC_WORLD_GMT_3_N_CITY, // RDA; Buenos Aires, Brasilia
    TXT_ACC_WORLD_GMT_3_P, // RDA; GMT+3
    TXT_ACC_WORLD_GMT_3_P_CITY, // RDA; Jeddah, Nairobi
    TXT_ACC_WORLD_GMT_3_P_MOSCOW, // RDA; Moscow
    TXT_ACC_WORLD_GMT_4P5_P, // RDA; GMT+4:30
    TXT_ACC_WORLD_GMT_4P5_P_CITY, // RDA; Kabul
    TXT_ACC_WORLD_GMT_4P5_P_KABUL, // RDA; Kabul
    TXT_ACC_WORLD_GMT_4_N, // RDA; GMT-4
    TXT_ACC_WORLD_GMT_4_N_CITY, // RDA; Caracas, La Paz
    TXT_ACC_WORLD_GMT_4_N_SANTIAGO, // RDA; Santiago
    TXT_ACC_WORLD_GMT_4_P, // RDA; GMT+4
    TXT_ACC_WORLD_GMT_4_P_ABUDHABI, // RDA; Abu Dhabi
    TXT_ACC_WORLD_GMT_4_P_CITY, // RDA; Moscow, Abu Dhabi
    TXT_ACC_WORLD_GMT_5P5_P, // RDA; GMT+5:30
    TXT_ACC_WORLD_GMT_5P5_P_BANGALORE, // RDA; Bangalore
    TXT_ACC_WORLD_GMT_5P5_P_CITY, // RDA; Mumbai, New Delhi, Colombo
    TXT_ACC_WORLD_GMT_5P5_P_KOLKATA, // RDA; Kolkata
    TXT_ACC_WORLD_GMT_5P5_P_MUMBAI, // RDA; Mumbai
    TXT_ACC_WORLD_GMT_5P5_P_NEWDELHI, // RDA; New Delhi
    TXT_ACC_WORLD_GMT_5P75_P, // RDA; GMT+5:45
    TXT_ACC_WORLD_GMT_5P75_P_CITY, // RDA; Kathmandu
    TXT_ACC_WORLD_GMT_5P75_P_KATHMANDU, // RDA; Kathmandu
    TXT_ACC_WORLD_GMT_5_N, // RDA; GMT-5
    TXT_ACC_WORLD_GMT_5_N_BOGOTA, // RDA; Bogota
    TXT_ACC_WORLD_GMT_5_N_CITY, // RDA; New York, Washington DC
    TXT_ACC_WORLD_GMT_5_N_MONTREAL, // RDA; Montreal
    TXT_ACC_WORLD_GMT_5_N_NEWYORK, // RDA; New York
    TXT_ACC_WORLD_GMT_5_P, // RDA; GMT+5
    TXT_ACC_WORLD_GMT_5_P_CITY, // RDA; Islamabad, Tashkent
    TXT_ACC_WORLD_GMT_5_P_ISLAMABAD, // RDA; Islamabad
    TXT_ACC_WORLD_GMT_6P5_P, // RDA; GMT+6:30
    TXT_ACC_WORLD_GMT_6P5_P_YANGON, // RDA; Yangon
    TXT_ACC_WORLD_GMT_6_N, // RDA; GMT-6
    TXT_ACC_WORLD_GMT_6_N_CHICAGO, // RDA; Chicago
    TXT_ACC_WORLD_GMT_6_N_CITY, // RDA; Chicago, Dallas
    TXT_ACC_WORLD_GMT_6_N_MEXICO, // RDA; Mexico
    TXT_ACC_WORLD_GMT_6_P, // RDA; GMT+6
    TXT_ACC_WORLD_GMT_6_P_CITY, // RDA; Dhaka
    TXT_ACC_WORLD_GMT_6_P_DHAKA, // RDA; Dhaka
    TXT_ACC_WORLD_GMT_7_N, // RDA; GMT-7
    TXT_ACC_WORLD_GMT_7_N_CITY, // RDA; Denver, Phoenix
    TXT_ACC_WORLD_GMT_7_N_PHOENIX, // RDA; Phoenix
    TXT_ACC_WORLD_GMT_7_P, // RDA; GMT+7
    TXT_ACC_WORLD_GMT_7_P_BANGKOK, // RDA; Bangkok
    TXT_ACC_WORLD_GMT_7_P_CITY, // RDA; Bangkok, Jakarta, Hanoi
    TXT_ACC_WORLD_GMT_7_P_HANOI, // RDA; Hanoi
    TXT_ACC_WORLD_GMT_7_P_JAKARTA, // RDA; Jakarta
    TXT_ACC_WORLD_GMT_8P5_N, // RDA; GMT-8:30
    TXT_ACC_WORLD_GMT_8P5_N_EASTER_ISLAND, // RDA; Easter Island
    TXT_ACC_WORLD_GMT_8_N, // RDA; GMT-8
    TXT_ACC_WORLD_GMT_8_N_CITY, // RDA; Los Angeles, San Francisco
    TXT_ACC_WORLD_GMT_8_N_LOSANGELES, // RDA; Los Angeles
    TXT_ACC_WORLD_GMT_8_N_SAN_FRANCISCO, // RDA; San Francisco
    TXT_ACC_WORLD_GMT_8_N_VANCOUVER, // RDA; Vancouver
    TXT_ACC_WORLD_GMT_8_P, // RDA; GMT+8
    TXT_ACC_WORLD_GMT_8_P_BEIJING, // RDA; Beijing
    TXT_ACC_WORLD_GMT_8_P_CITY, // RDA; Hong Kong, Beijing, 
    TXT_ACC_WORLD_GMT_8_P_HONGKONG, // RDA; Hong Kong
    TXT_ACC_WORLD_GMT_8_P_KUALA_LUMPUR, // RDA; Kuala Lumpur
    TXT_ACC_WORLD_GMT_8_P_MANILA, // RDA; Manila
    TXT_ACC_WORLD_GMT_8_P_SINGAPORE, // RDA; Singapore
    TXT_ACC_WORLD_GMT_8_P_TAIPEI, // RDA; Taipei
    TXT_ACC_WORLD_GMT_9P5_N, // RDA; GMT-9:30
    TXT_ACC_WORLD_GMT_9P5_N_POLYNESIA, // RDA; Polynesia
    TXT_ACC_WORLD_GMT_9P5_P, // RDA; GMT+9:30
    TXT_ACC_WORLD_GMT_9P5_P_CITY, // RDA; Adelaide
    TXT_ACC_WORLD_GMT_9P5_P_MELBOURNE, // RDA; Melbourne
    TXT_ACC_WORLD_GMT_9_N, // RDA; GMT-9
    TXT_ACC_WORLD_GMT_9_N_ANCHORAGE, // RDA; Anchorage
    TXT_ACC_WORLD_GMT_9_N_CITY, // RDA; Alaska
    TXT_ACC_WORLD_GMT_9_P, // RDA; GMT+9
    TXT_ACC_WORLD_GMT_9_P_CITY, // RDA; Seoul, Tokyo
    TXT_ACC_WORLD_GMT_9_P_TOKYO, // RDA; Tokyo
    TXT_ACC_WORLD_LOCAL, // RDA; Local
    TXT_ACC_WRONG, // RDA; Error
    TXT_ACTIVE_OPERATIVE, // RDA; Active
    TXT_ADDCONTACT, // RDA; Add to contacts
    TXT_ADDED, // RDA; Added
    TXT_ADDTOPB_EXIST, // RDA; Add to existed contact
    TXT_ADDTOPB_NEW, // RDA; Add to new contact
    TXT_ALARM_CLOCK, // RDA; Alarm
    TXT_ALARM_CLOCK1, // RDA; Alarm1
    TXT_ALARM_CLOCK2, // RDA; Alarm2
    TXT_ALARM_CLOCK3, // RDA; Alarm3
    TXT_ALARM_CLOSE, // RDA; Off
    TXT_ALARM_DELAY, // RDA; Snooze
    TXT_ALARM_DELAY_1, // RDA; Snooze1
    TXT_ALARM_DELAY_2, // RDA; Snooze2
    TXT_ALARM_DELAY_3, // RDA; Snooze3
    TXT_ALARM_DISABLE, // RDA; Off
    TXT_ALARM_EDIT, // RDA; Edit
    TXT_ALARM_EDIT_MODE, // RDA; Repeat mode
    TXT_ALARM_EDIT_NAME, // RDA; Message
    TXT_ALARM_EDIT_RING, // RDA; Ringtone
    TXT_ALARM_EDIT_TIME, // RDA; Time
    TXT_ALARM_ENABLE, // RDA; On
    TXT_ALARM_MEMORY_FULL, // RDA; Memory full
    TXT_ALARM_MODE_EVERYDAY, // RDA; Daily
    TXT_ALARM_MODE_ONCE, // RDA; Once
    TXT_ALARM_MODE_WEEK, // RDA; Weekly
    TXT_ALARM_MORE_RING, // RDA; More ringtones
    TXT_ALARM_MUST_SEL_ONE_DAY, // RDA; Select at least one day
    TXT_ALARM_NAME_EMPTY, // RDA; Name is empty
    TXT_ALARM_NO_MATCH_RECORD, // RDA; No matching records
    TXT_ALARM_SHUT_DOWN, // RDA;  seconds later
    TXT_ALARM_S_LATER_SHUTDOWN, // RDA; s later shutdown
    TXT_ALARM_WILL_TO, // RDA; will shut down
    TXT_ALL, // RDA; All
    TXT_ALREADY_EXISTS, // RDA; Already exists
    TXT_ANWSER_MASHINE, // RDA; Answer machine
    TXT_ANWSER_MASHINE_SIM1, // RDA; SIM1 Answer machine
    TXT_ANWSER_MASHINE_SIM2, // RDA; SIM2 Answer machine
    TXT_APP_CALCULATOR, // RDA; Caculator
    TXT_APP_CONVERTOR, // RDA; Convertor
    TXT_APP_MEMO, // RDA; Memo
    TXT_APP_TASK, // RDA; Task
    TXT_APP_TIMER, // RDA; Timer
    TXT_APP_TORCH_LIGHT, // RDA; Torch light
    TXT_APP_WORLD_CLOCK, // RDA; World clock
    TXT_ASP_ANWSER_WORDS, // RDA; Answering words
    TXT_ASP_START_ANWSER_TIMER, // RDA; n second later start anwser
    TXT_ASP_VOICE_MSG_LEN, // RDA; Voice msg len
    TXT_ASSIGN, // RDA; Assign
    TXT_AUTOPOWER_OFF, // RDA; Auto power off
    TXT_AUTOPOWER_ON, // RDA; Auto power on
    TXT_AUTO_POWERONOFF, // RDA; Auto power on/off
    TXT_BAIC, // RDA; Barring all incoming calls
    TXT_BAOC, // RDA; Barring all outgoing calls
    TXT_BEIJING, // RDA; Beijing
    TXT_BEIJING_010, // RDA; Beijing 010
    TXT_BICROAM, // RDA; Barring incoming calls while roaming
    TXT_BLANK, // RDA; Empty
    TXT_BLOCKED, // RDA; Blocked
    TXT_BOIC, // RDA; Barring outgoing ISD calls
    TXT_BOIC_EXHC, // RDA; Barring outgoing STD calls
    TXT_BROWSER_FILE_TITLE, // RDA; Title
    TXT_BROWSER_INQUIREDELETEALL, // RDA; Delete all?
    TXT_BTRY_CAPACITY, // RDA; Battery capacity
    TXT_BTRY_CAP_LABEL_TEXT, // RDA; Battery remains:
    TXT_BT_ACTIVATING, // RDA; Activating
    TXT_BT_AUDIO_CONNECT, // RDA; Audio connect with %s?
    TXT_BT_BLUETOOTH_DEACTIVATED, // RDA; Bluetooth Deactivated
    TXT_BT_BOUNDING, // RDA; Bonding
    TXT_BT_BOUNDING_FAILED, // RDA; Bonding failed
    TXT_BT_CANNOT_FIND_DEVICE, // RDA; Device not found
    TXT_BT_CONNECTING, // RDA; Connecting...
    TXT_BT_CONNECTING_FAILED, // RDA; Connecting Failed! 
    TXT_BT_DISCONNECTING, // RDA; Disconnecting...
    TXT_BT_EMPTY_NAME, // RDA; Empty Name
    TXT_BT_ENTER_PIN, // RDA; Enter PIN
    TXT_BT_EXCHANGE_OBJECT_WITH, // RDA; Exchange object with device ?
    TXT_BT_FET_REQ_ALERT, // RDA; Allow to get your file?
    TXT_BT_FILE_EXPLORER, // RDA; File explorer
    TXT_BT_GETTING_SERVICE_LIST, // RDA; Retrieving service list…
    TXT_BT_HEADSET_CLOSED, // RDA; Disconnected
    TXT_BT_HEADSET_OPENED, // RDA; Connected
    TXT_BT_MEMORY_FULL, // RDA; Memory full. Delete some items
    TXT_BT_NO_NEW_DEVICE_SPACE_REPLACE_Q, // RDA; No space for new device. Replace an existing device?
    TXT_BT_PAIR_AGAIN, // RDA; Pairing failed.Try again?
    TXT_BT_QUESTION_ACTIVATE_YES_NO, // RDA; Bluetooth is not activated. Activate Bluetooth?
    TXT_BT_RECEIVED, // RDA; Received
    TXT_BT_RENAME_EXISTING_FILE, // RDA; Already in use. Rename?
    TXT_BT_SAVE_TO_MEMORY_CARD, // RDA; Not enough phone memory.Save to memory card?
    TXT_BT_SAVE_TO_PHONE_MEMORY, // RDA; Not enough card memory. Save to phone?
    TXT_CALC_CLEAR, // RDA; Clear
    TXT_CALC_MCLEAR, // RDA; MC
    TXT_CALC_MPLUS, // RDA; M+
    TXT_CALC_MR, // RDA; MR
    TXT_CALC_MSUB, // RDA; M-
    TXT_CALENDAR_ALL, // RDA; All schedules
    TXT_CALENDAR_AQUARIUS, // RDA; Aquarius
    TXT_CALENDAR_ARIES, // RDA; Aries
    TXT_CALENDAR_CANCER, // RDA; Cancer
    TXT_CALENDAR_CAPRICORN, // RDA; Capricorn
    TXT_CALENDAR_CLEAR_ALL, // RDA; Clear all
    TXT_CALENDAR_CLEAR_TODAY, // RDA; Clear today's tasks
    TXT_CALENDAR_DATE, // RDA; Date:
    TXT_CALENDAR_DEADLINE, // RDA; Deadline:
    TXT_CALENDAR_DEL_CDR_DAY_PERIODSCH, // RDA; Current day has repeated schedule, delete?
    TXT_CALENDAR_DEL_THE_PERIODSCH, // RDA; This is repeated schedule, do you want to delete?
    TXT_CALENDAR_EDIT_DATE, // RDA; Date
    TXT_CALENDAR_EDIT_DEADLINE, // RDA; Deadline
    TXT_CALENDAR_EDIT_FREQ, // RDA; Type
    TXT_CALENDAR_EDIT_NAME, // RDA; Subject
    TXT_CALENDAR_EDIT_RING, // RDA; Ringtone
    TXT_CALENDAR_EDIT_TIME, // RDA; Time
    TXT_CALENDAR_EMPTY, // RDA; No record
    TXT_CALENDAR_FREQ, // RDA; Type:
    TXT_CALENDAR_GEMINI, // RDA; Gemini
    TXT_CALENDAR_LEO, // RDA; Leo
    TXT_CALENDAR_LIBRA, // RDA; Libra
    TXT_CALENDAR_LOOK, // RDA; View schedules
    TXT_CALENDAR_LUNAR, // RDA; Lunar
    TXT_CALENDAR_MEMO, // RDA; Anniversary
    TXT_CALENDAR_MONTHLY, // RDA; Monthly
    TXT_CALENDAR_NAME, // RDA; Subject:
    TXT_CALENDAR_NEW, // RDA; Add new schedule
    TXT_CALENDAR_PISCES, // RDA; Pisces
    TXT_CALENDAR_QUERY_LUNAR, // RDA; Lunar:
    TXT_CALENDAR_QUERY_SOLAR, // RDA; Date:
    TXT_CALENDAR_RING, // RDA; Ringtone:
    TXT_CALENDAR_SAGITTARIUS, // RDA; Sagittarius
    TXT_CALENDAR_SCORPIO, // RDA; Scorpio
    TXT_CALENDAR_SOLAR, // RDA; Solar
    TXT_CALENDAR_TAURUS, // RDA; Taurus
    TXT_CALENDAR_TIME, // RDA; Time:
    TXT_CALENDAR_TO_DATE, // RDA; Go to date
    TXT_CALENDAR_VIRGO, // RDA; Virgo
    TXT_CALENDAR_WEEK, // RDA; Week
    TXT_CALENDAR_WEEKLY, // RDA; Weekly
    TXT_CALENDAR_WEEK_NO, // RDA; No.
    TXT_CALL, // RDA; Call
    TXT_CALL_AUDIO, // RDA; Voice call
    TXT_CALL_BARRED, // RDA; Call barred
    TXT_CALL_BARRING, // RDA; Call barred
    TXT_CALL_FORWARD, // RDA; Call divert
    TXT_CALL_IPCALL, // RDA; IP call
    TXT_CALL_LIST, // RDA; Call list
    TXT_CALL_USING, // RDA; Busy, please try later
    TXT_CALL_WAITING, // RDA; Call waiting
    TXT_CARD_NAME, // RDA; Card name
    TXT_CC_BUILDMPTY, // RDA; Setup call conference
    TXT_CC_BUILDMPTY_ANSWER, // RDA; Setup call conference & answer
    TXT_CC_BUILTMPTYING, // RDA; Setting up call conference 
    TXT_CC_CALLING, // RDA; Calling
    TXT_CC_CALL_CONTROL_NOT_ALLOW, // RDA; SIM card not allowed
    TXT_CC_CFC_STARTUP, // RDA; Conditional divert enabled
    TXT_CC_CFU_STARTUP, // RDA; Call forwarding unconditional enabled
    TXT_CC_CHECK_LINE, // RDA; Check line options
    TXT_CC_CLOSING_CONNECTION, // RDA; Disconnecting...
    TXT_CC_COMMUN, // RDA; Connected
    TXT_CC_COMMUN_BUSY, // RDA; PS service is disabled during a call
    TXT_CC_CONNECTING, // RDA; Connecting
    TXT_CC_COST_EXCEED_LIMIT, // RDA; ACM exceeded
    TXT_CC_DIALBACK_PROMPT, // RDA; Call back by another SIM, please wait
    TXT_CC_DIAL_FAIL, // RDA; Dial failed
    TXT_CC_ECT_CONFIRM, // RDA; Call transferred
    TXT_CC_ECT_CONFIRM_REJECT, // RDA; Transferred request rejected
    TXT_CC_ECT_PROCESS, // RDA; Transfer requesting
    TXT_CC_ECT_TRANSFER, // RDA; Transfer
    TXT_CC_ECT_TRANSFERING, // RDA; Transfer in progress
    TXT_CC_ECT_TRANSFER_TO, // RDA; Transferred to
    TXT_CC_EMERGENCYONLY, // RDA; Emergency call only
    TXT_CC_END, // RDA; Call ended
    TXT_CC_ERROR_NUMBER, // RDA; Error number
    TXT_CC_EST_MPTY_FIRST, // RDA; Set call conference first
    TXT_CC_EST_SHUTTLE_FIRST, // RDA; Switch to call status first
    TXT_CC_FDN_ONLY, // RDA; Fixed dial number only
    TXT_CC_FORBID, // RDA; Call barred
    TXT_CC_HANG_OFF, // RDA; Hang up
    TXT_CC_HIDE_ID, // RDA; Check hide ID settings
    TXT_CC_HOLD, // RDA; Hold
    TXT_CC_HOLDING, // RDA; Holding
    TXT_CC_HOLD_ANSWER, // RDA; Hold & answer
    TXT_CC_INSERT_WPN, // RDA; Insert
    TXT_CC_INVALID_MES, // RDA; Invalid message
    TXT_CC_INVALID_NUM, // RDA; Invalid number
    TXT_CC_MISS_CALL, // RDA; missed call
    TXT_CC_MTCALLING, // RDA; Incoming call
    TXT_CC_MULTICALL, // RDA; Conference call
    TXT_CC_MULTICALL_VIEW, // RDA; Conference members
    TXT_CC_NO_IP_NUM, // RDA; No IP number!
    TXT_CC_NUM_N, // RDA; n
    TXT_CC_OPER_FAIL, // RDA; Failed
    TXT_CC_PAUSE_P, // RDA; Pause P
    TXT_CC_PRIVATE, // RDA; Private
    TXT_CC_PROTOCOL_ERROR, // RDA; Protocol error
    TXT_CC_PSCS_CONFLICT_QUERY, // RDA; Resource not enough, exit?
    TXT_CC_REDIALED, // RDA; Dialled
    TXT_CC_REDIALING1, // RDA; Redialing.
    TXT_CC_REDIALING2, // RDA; Redialing..
    TXT_CC_REDIALING3, // RDA; Redialing...
    TXT_CC_REDIAL_TIMES, // RDA;  time(s)
    TXT_CC_RELEASE_ACTIVE, // RDA; Release active call
    TXT_CC_RELEASE_ALL, // RDA; Release all calls
    TXT_CC_RELEASE_ANSWER, // RDA; Release & answer
    TXT_CC_RELEASE_CALL, // RDA; Release selected call
    TXT_CC_RELEASE_HOLD, // RDA; Release held call
    TXT_CC_RELEASING, // RDA; Releasing
    TXT_CC_REMOTE_BUILD_MPTY, // RDA; Multi-party call is on
    TXT_CC_REMOTE_HOLD_IND, // RDA; Call holding
    TXT_CC_REMOTE_RETRIEVED_IND, // RDA; Call retrived
    TXT_CC_RES_UNAV, // RDA; Network busy
    TXT_CC_RETRIEVING, // RDA; Retrieving
    TXT_CC_SEND_DTMF, // RDA; Send DTMF?
    TXT_CC_SERVICE_NOT_IMP, // RDA; Service not available
    TXT_CC_SERVICE_UNAV, // RDA; Service unavailable
    TXT_CC_SHUTTLE, // RDA; Switch
    TXT_CC_SHUTTLING, // RDA; Switching
    TXT_CC_SPLIT, // RDA; Split call conference
    TXT_CC_SPLITING, // RDA; Splitting call conference
    TXT_CC_UNKNOW_NUM, // RDA; Unknown number
    TXT_CC_USER_BUSY, // RDA; Number you dialed is busy
    TXT_CC_USER_NOT_RESPOND, // RDA; User you dialed isn't responding
    TXT_CC_USER_UNCONNECTED, // RDA; User is unavailable
    TXT_CC_WAIT_W, // RDA; Wait W
    TXT_CFB, // RDA; User busy
    TXT_CFNRC, // RDA; No network or power off
    TXT_CFNRY, // RDA; No answer
    TXT_CFU, // RDA; Call forwarding unconditional
    TXT_CHANGED_TO, // RDA; changed to
    TXT_CHANGE_QUERY, // RDA; Change?
    TXT_CHARGE_OVER_VOLTAGE, // RDA; Charge voltage over limit
    TXT_CHARGE_WARNING, // RDA; Low battery
    TXT_CHARGING, // RDA; Charging
    TXT_CHECKING_STATUS, // RDA; Checking status...
    TXT_CLIP, // RDA; CLIP
    TXT_CLIR, // RDA; CLIR
    TXT_CLOSE, // RDA; Off
    TXT_CLOSE_FLY_MODE_FIRST, // RDA; Please disable flight mode
    TXT_CL_ADD_TO_REJECT, // RDA; Add to reject list
    TXT_CL_ALL_LOG, // RDA; All logs
    TXT_CL_ATON, // RDA; Add to existed contacts
    TXT_CL_CALL_COSTS, // RDA; Call costs
    TXT_CL_CALL_COSTS_CURRENCY, // RDA; Currency
    TXT_CL_CALL_COSTS_PRICE, // RDA; Price
    TXT_CL_CALL_DIALLED, // RDA; Reset dialled call times?
    TXT_CL_CALL_DURATION, // RDA; Call duration
    TXT_CL_CALL_LAST, // RDA; Reset last call time?
    TXT_CL_CALL_MANAGER, // RDA; Call manager
    TXT_CL_CALL_RECEIVED, // RDA; Reset received call times?
    TXT_CL_CALL_SMSCOUNTER, // RDA; Message counter
    TXT_CL_CALL_SMSCOUNTER_RECEIVED, // RDA; Received messages
    TXT_CL_CALL_SMSCOUNTER_SENT, // RDA; Sent messages
    TXT_CL_CALL_SMSCOUNTER_TOTAL, // RDA; Total messages
    TXT_CL_CALL_TIME, // RDA; Call time
    TXT_CL_CALL_TIMERS, // RDA; Call timers
    TXT_CL_CALL_TOTAL, // RDA; Are you sure?
    TXT_CL_COPY, // RDA; Copy to dialling screen
    TXT_CL_COSTS_LIMIT_ESIT, // RDA; Set max. cost changed
    TXT_CL_COST_ALL, // RDA; Total cost
    TXT_CL_COST_LAST_CALL, // RDA; Last call
    TXT_CL_COST_LIMIT, // RDA; Cost limit
    TXT_CL_COST_MAX, // RDA; Max cost
    TXT_CL_COST_RATE, // RDA; Price per unit
    TXT_CL_COST_RECENT_RESET, // RDA; Recent reset date
    TXT_CL_COST_RESET, // RDA; Reset cost
    TXT_CL_COST_RESET_ALL, // RDA; Reset call cost
    TXT_CL_COST_SETUP_CURRENCY, // RDA; Setup currency
    TXT_CL_COST_TOTAL, // RDA; Total
    TXT_CL_DELETED, // RDA; Deleted
    TXT_CL_DEL_ALL, // RDA; Delete all
    TXT_CL_DEL_ALLLOGS, // RDA; Call history
    TXT_CL_DETAIL_SIM_HEAD, // RDA; SIM name
    TXT_CL_DIALED, // RDA; Dialled calls
    TXT_CL_DIALED_CALLS, // RDA; Dialled calls
    TXT_CL_EMPTY_NUM, // RDA; Number is null
    TXT_CL_GET_PUCT_ERROR, // RDA; Get Puct error
    TXT_CL_GPRS_FLOWRATE, // RDA; GPRS counter
    TXT_CL_INPUT_COST_MAX, // RDA; Set max cost
    TXT_CL_INPUT_COST_RATE, // RDA; Set rate
    TXT_CL_INPUT_COST_RATE_ERROR, // RDA; Input rate cost incorrect
    TXT_CL_INPUT_CURRENCY, // RDA; Input currency
    TXT_CL_INPUT_CURRENCY_ERROR, // RDA; Input currency length incorrect
    TXT_CL_LOG_MANAGER, // RDA; Log manager
    TXT_CL_LOG_TODAY, // RDA; Today
    TXT_CL_LOG_YESTODAY, // RDA; Yesterday
    TXT_CL_MISSED, // RDA; missed calls
    TXT_CL_MISSED_CALLS, // RDA; Missed calls
    TXT_CL_NOT_PRESENT, // RDA; Call cost not provided
    TXT_CL_NO_LOGS_ITEM, // RDA; No logs
    TXT_CL_RECEIVED, // RDA; Received calls
    TXT_CL_RECEIVED_CALLS, // RDA; Received calls
    TXT_CL_RECEIVED_MSG, // RDA; Received msg.
    TXT_CL_RECENT_CALLS, // RDA; Recent Contacts
    TXT_CL_REFUSED, // RDA; Rejected calls
    TXT_CL_REFUSED_CALLS, // RDA; Rejected calls
    TXT_CL_RESET, // RDA; Reset
    TXT_CL_SELECT_ALL, // RDA; Select all
    TXT_CL_SEND, // RDA; Send message
    TXT_CL_SEND_MMS, // RDA; MMS
    TXT_CL_SEND_SMS, // RDA; SMS
    TXT_CL_SENT_MSG, // RDA; Sent msg.
    TXT_CL_SET_COST_MAX, // RDA; Set max cost
    TXT_CL_SET_COST_RATE, // RDA; Set price per unit
    TXT_CL_SET_REFUSE, // RDA; Add to blacklist
    TXT_CL_SMS_RECEIVE_RESET, // RDA; Reset received messages?
    TXT_CL_SMS_RESETALL, // RDA; Reset all message counter?
    TXT_CL_SMS_SENT_RESET, // RDA; Reset sent messages?
    TXT_CL_STYLE, // RDA; Type
    TXT_CL_TIMER_DIALED_CALLS, // RDA; Dialled calls
    TXT_CL_TIMER_LAST_CALL, // RDA; Last call
    TXT_CL_TIMER_RESET, // RDA; Reset all time
    TXT_CL_TO_SEND_MMS, // RDA; Send MMS
    TXT_CL_TO_SEND_SMS, // RDA; Send message
    TXT_CL_UPDATE_EXISTING, // RDA; Update existing
    TXT_CMSBRW_FULL_NO_STORE, // RDA; Unable to save file. Memory full!
    TXT_CMSBRW_SD_FULL_CHANGE_STORE, // RDA; Not enough memory. Delete some items !
    TXT_CMSBRW_UDISK_FULL_CHANGE_STORE, // RDA; Not enough memory. Delete some items !
    TXT_CODE_ACCEPT, // RDA; Code accepted
    TXT_COLP, // RDA; COLP
    TXT_COLR, // RDA; COLR
    TXT_COMMON_CMD_NO_SUPPORT, // RDA; Command is not supported
    TXT_COMMON_CREATE, // RDA; Create
    TXT_COMMON_DEL_FAIL, // RDA; Delete failed
    TXT_COMMON_DETAIL_FILE_POS, // RDA; Storage:
    TXT_COMMON_DOWNLOAD_COMN, // RDA; Download
    TXT_COMMON_EMPTY_NUM, // RDA; Empty number
    TXT_COMMON_EXIT_WITHOUT_SAVE, // RDA; Exit without saving?
    TXT_COMMON_FOUCS, // RDA; Focus
    TXT_COMMON_IMPORTING, // RDA; Importing
    TXT_COMMON_LINK_FAILED, // RDA; 
    TXT_COMMON_MAIN_STK, // RDA; STK
    TXT_COMMON_NETWORK_CONNECT_FAIL, // RDA; Network connect failed
    TXT_COMMON_NOT_EXIST, // RDA; Not exist
    TXT_COMMON_NO_SUPPORT, // RDA; Unsupported format
    TXT_COMMON_ON, // RDA; On
    TXT_COMMON_PARAM_SETTINGS, // RDA; Parameters settings
    TXT_COMMON_PLAY_ERROR, // RDA; Play error!
    TXT_COMMON_RECEIVING, // RDA; Receiving
    TXT_COMMUNICATION_BROWSER, // RDA; Internet
    TXT_COMMUNICATION_CL, // RDA; Call logs
    TXT_COMMUNICATION_DIAL, // RDA; Dialpad
    TXT_COMMUNICATION_MESSAGE, // RDA; Message
    TXT_COMMUNICATION_PB, // RDA; Contacts
    TXT_COMMUNICATION_STK, // RDA; STK
    TXT_COMM_OPEN, // RDA; Open
    TXT_COMPLETE, // RDA; Done
    TXT_CONNECTING, // RDA; Connecting
    TXT_CONNECTION_LISTFULL, // RDA; List full
    TXT_CONNECTION_MANAGER, // RDA; Connection management
    TXT_CONTACTS_DELETED, // RDA; Contacts deleted
    TXT_CONTACT_DELETED, // RDA; Contact deleted
    TXT_CONTINUE, // RDA; Resume
    TXT_CUSTOM_TIME, // RDA; Custom time
    TXT_DATE, // RDA; Date
    TXT_DATE_IS_PAST, // RDA; Time expired
    TXT_DC_NAMEINVALID, // RDA; Invalid filename
    TXT_DC_NAMEREPEAT, // RDA; Duplicated filename
    TXT_DC_OPEN_FAIL, // RDA; Camera open failed
    TXT_DEFAULT_SIM, // RDA; Default SIM
    TXT_DELALL, // RDA; Delete all
    TXT_DELETE, // RDA; Delete
    TXT_DELETED, // RDA; Deleted
    TXT_DELETEQUERY, // RDA; Delete?
    TXT_DELETE_HEADSET_ALERT, // RDA; Bluetooth now in use. Not allowed
    TXT_DEVIDEND_INVALID, // RDA; Illegal operation
    TXT_DIAL_ADDPB, // RDA; Create contact
    TXT_DIAL_ADDTOMAN, // RDA; Update existing
    TXT_DIAL_MAXIMUM_NUM_SAVED, // RDA; Maximum 40 digits saved
    TXT_DIAL_SENDSMS, // RDA; Send message
    TXT_DISK_FILE_FULL, // RDA; Folder full
    TXT_DRM_RIGHTS_VALID_WHETHER_DELETE, // RDA; The license for this file is still valid, delete it?
    TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL, // RDA; The selected files contains unexpired drm files. Delete selected files?
    TXT_EARPHONE, // RDA; Ear-Phn
    TXT_EDIT, // RDA; Edit
    TXT_EDIT_CLIPBRD_BACK, // RDA; Back
    TXT_EDIT_CLIPBRD_CUT, // RDA; Cut
    TXT_EDIT_CLIPBRD_MARK, // RDA; Mark
    TXT_EDIT_CLIPBRD_PASTE, // RDA; Paste
    TXT_EDIT_CLIPBRD_UNMARK, // RDA; Unmark
    TXT_EMERGENCY_CALL_QUERY, // RDA; Emergency call only. Insert SIM card to access network service
    TXT_EMPTY_FILE, // RDA; Empty file
    TXT_EMPTY_LIST, // RDA; <Empty List>
    TXT_ENABLE_PIN1, // RDA; Please activate PIN
    TXT_ENABLE_PIN2, // RDA; Please activate PIN2
    TXT_ENG_ADC_CALIBRATE_INFO, // RDA; Adc Calibrate Info
    TXT_ENG_BAT_INFO, // RDA; Battery
    TXT_ENG_CMCC_BUSINESS_SERVER, // RDA; CMCC business server
    TXT_ENG_CMCC_TEST_CONFIG, // RDA; CMCC test config
    TXT_ENG_CMCC_TEST_SERVER, // RDA; CMCC test server
    TXT_ENG_DM_SETTING, // RDA; DM Settings
    TXT_ENG_FAIL, // RDA; Fail
    TXT_ENG_FULLTEST, // RDA; All phone test
    TXT_ENG_ITEMTEST, // RDA; Item test
    TXT_ENG_LANGUAGE_NOT_EXIST, // RDA; Not exist
    TXT_ENG_LANGUAGE_SET_SUCC, // RDA; Language settings succeeded
    TXT_ENG_OK, // RDA; OK
    TXT_ENG_PHONETEST, // RDA; Phone test
    TXT_ENG_PRODUCT, // RDA; Product
    TXT_ENG_RETURN, // RDA; Return
    TXT_ENG_SCREEN_LOG, // RDA; Screen Log
    TXT_ENG_TEST_BACKLIGHT, // RDA; Backlight test
    TXT_ENG_TEST_FM, // RDA; FM test
    TXT_ENG_TEST_HEADSET, // RDA; Headset test
    TXT_ENG_TEST_KEY, // RDA; Key test
    TXT_ENG_TEST_LOOPBACK, // RDA; Loopback test
    TXT_ENG_TEST_MAINLCD, // RDA; Main lcd test
    TXT_ENG_TEST_MELODY, // RDA; Melody test
    TXT_ENG_TEST_RTC, // RDA; RTC test
    TXT_ENG_TEST_VIBRATER, // RDA; Vibrater test
    TXT_ENG_UIBACKLIGHT_TITLE, // RDA; Backlight
    TXT_ENG_UICAMERA_TITLE, // RDA; Camera
    TXT_ENG_UIHEADSET_TITLE, // RDA; Headset
    TXT_ENG_UIMELODY_TITLE, // RDA; Melody
    TXT_ENG_UISD_TITLE, // RDA; MemoryCard
    TXT_ENG_UITEST_TITLE, // RDA; Mmi Ui-Test
    TXT_ENG_UIVIBRATE_TITLE, // RDA; Vibrate
    TXT_ENG_VERSION_INFO, // RDA; Version Info
    TXT_ENG_VIEWRESULT, // RDA; View phone test result
    TXT_ENTERTAIMENT_GAME, // RDA; Games
    TXT_ENTER_NEW_PIN, // RDA; Input new PIN(4-8)
    TXT_ENTER_NEW_PIN2, // RDA; Input new PIN2(4-8)
    TXT_ENTER_PIN, // RDA; Input PIN(4-8)
    TXT_ENTER_PIN2, // RDA; Input PIN2(4-8)
    TXT_ENTER_PIN2_CNF, // RDA; Confirm new PIN2(4-8)
    TXT_ENTER_PIN_CNF, // RDA; Confirm new PIN (4-8)
    TXT_ENTER_PUK, // RDA; Input PUK(4-8)
    TXT_ENTER_PUK2, // RDA; Input PUK2(4-8)
    TXT_ENTER_SIM1_PUK, // RDA; Enter SIM1 PUK
    TXT_ENTER_SIM2_PUK, // RDA; Enter SIM2 PUK
    TXT_ENTER_SIM3_PUK, // RDA; Enter SIM3 PUK
    TXT_ENTRY_EMPTY, // RDA; Entry empty
    TXT_ENTRY_IS_EMPTY, // RDA; Entry is empty
    TXT_ENVSET_ACTIVATE, // RDA; Activate
    TXT_ENVSET_ALARM_RING_TYPE, // RDA; Alarm&calendar remind
    TXT_ENVSET_CHANGE_NAME, // RDA; Rename
    TXT_ENVSET_DRIVING, // RDA; Driving
    TXT_ENVSET_EARPHONE, // RDA; Earphone
    TXT_ENVSET_EDIT, // RDA; Edit
    TXT_ENVSET_EMPTY_NAME, // RDA; Empty Name
    TXT_ENVSET_INSIDE, // RDA; Indoor
    TXT_ENVSET_MEETING, // RDA; Meeting
    TXT_ENVSET_MP_FIR, // RDA; My profile 1
    TXT_ENVSET_MP_SEC, // RDA; My profile 2
    TXT_ENVSET_NOISY, // RDA; Outdoor
    TXT_ENVSET_OFFLINE, // RDA; Offline
    TXT_ENVSET_POWER_RING_TYPE, // RDA; Power ringtone
    TXT_ENVSET_SAVE, // RDA; Save
    TXT_ENVSET_SETTING_OPT, // RDA; Customize
    TXT_ENVSET_SILENT, // RDA; Silent
    TXT_ENVSET_STANDARD, // RDA; Normal
    TXT_ERROR, // RDA; Failed
    TXT_ERROR_SS_STATUS, // RDA; Supplementary service error
    TXT_FDN_ACTIVATED, // RDA; FDN activated
    TXT_FDN_DEACTIVATED, // RDA; FDN deactivated
    TXT_FDN_DISABLED, // RDA; Fixed dial number disabled
    TXT_FDN_ENABLED, // RDA; Fixed dial number enabled
    TXT_FDN_FULL, // RDA; Fixed dialing list full
    TXT_FDN_NOT_EXIST, // RDA; Fixed dial number doesn't exist
    TXT_FDN_NUM, // RDA; Fixed dial number
    TXT_FDN_VERIFYING, // RDA; Verifying
    TXT_FILE_BROWSING_FAILED, // RDA; File Browsing Failed!
    TXT_FILE_EXIST_OVERWRITE, // RDA; Already exists
    TXT_FM, // RDA; FM radio
    TXT_FM_Add_to_LIST, // RDA; Add to
    TXT_FM_BACKGROUND_PLAY, // RDA; Background play
    TXT_FM_CANCEL_RECORD, // RDA; Cancel saving audio
    TXT_FM_CANNOT_USE, // RDA; FM is unavailable now
    TXT_FM_CHANNEL_ANME_COLON, // RDA; Channel name:
    TXT_FM_CHANNEL_AddToFav, // RDA; Add to favourites
    TXT_FM_CHANNEL_LIST, // RDA; Channel list
    TXT_FM_CHANNEL_NAME, // RDA; Channel name
    TXT_FM_CHANNEL_NOT_EXIST, // RDA; This channel doesn't exist!
    TXT_FM_CHANNEL_SelfTuning, // RDA; Self-tunning
    TXT_FM_DELETE_CHA_QUERY, // RDA; Delete this channel?
    TXT_FM_DEL_MULTI, // RDA; Delete multiple
    TXT_FM_Earphone_MODE, // RDA; Earphone on
    TXT_FM_FREQEUENCY, // RDA; Frequency(MHz)
    TXT_FM_FREQEUENCY_COLON, // RDA; Frequency(MHz):
    TXT_FM_FULL_AUTO_SEARCH, // RDA; Auto search and save
    TXT_FM_HEADSET_MODE, // RDA; Close speaker
    TXT_FM_HEADSET_PLUG_OUT, // RDA; FM radio will be automatically off without headset! Please plug in headset!
    TXT_FM_HELP, // RDA; Help
    TXT_FM_INIT_FAIL, // RDA; Initialization failed, please try again!
    TXT_FM_INPUT_HEADSET, // RDA; Please plug in headphone
    TXT_FM_INVALID_FREQ, // RDA; Invalid frequency
    TXT_FM_MANUAL_SEARCH, // RDA; Manual search
    TXT_FM_MANUAL_TUNING, // RDA; Manual tuning
    TXT_FM_NO_CHANNEL, // RDA; No channel
    TXT_FM_OPEN_ALL_LIST, // RDA; Open all channels
    TXT_FM_OPEN_Favourite_LIST, // RDA; Open Favourites
    TXT_FM_OVERWRITE_CHANNEL, // RDA; Overwrite the existed Channel?
    TXT_FM_PLAY, // RDA; Play
    TXT_FM_Programme_Reminder, // RDA; Programme reminder
    TXT_FM_RADIO, // RDA; FM radio
    TXT_FM_RECORD_DELETE_FILES, // RDA; Delete lld file list
    TXT_FM_RECORD_FILE_EXIST, // RDA; File already existed
    TXT_FM_RECORD_FILE_LIST, // RDA; Record file list
    TXT_FM_RECORD_FILE_LOCATION, // RDA; Storage
    TXT_FM_RECORD_FILE_NOT_EXIST, // RDA; Invalid filename
    TXT_FM_RECORD_NEW_NAME_INVALID, // RDA; Invalid filename
    TXT_FM_REPETITION_DAY, // RDA; Repetition days
    TXT_FM_SAVA_AS, // RDA; Save to
    TXT_FM_SAVE_CHANNEL, // RDA; Save channel
    TXT_FM_SAVE_FREQ, // RDA; Save
    TXT_FM_SD_CARD, // RDA; MemoryCard
    TXT_FM_SEARCH_CHANNEL, // RDA; Search channels...
    TXT_FM_SPEAKER_MODE, // RDA; Open speaker
    TXT_FM_SPEAKER_ON, // RDA; Speaker on
    TXT_FM_Setting, // RDA; Settings
    TXT_FM_UDISK, // RDA; Phone
    TXT_FRIDAY, // RDA; Friday
    TXT_FULL_SERVICE, // RDA; Full service
    TXT_GAME_FINISH, // RDA; Congratulations!
    TXT_GAME_GAME, // RDA; Games
    TXT_GAME_INFO, // RDA; Help
    TXT_GAME_LINK, // RDA; Link game
    TXT_GETTING_SERVICE_LIST_FAILED, // RDA; Retrieving failed!
    TXT_GUISHUDI, // RDA; Location
    TXT_GUISHUDILOCAL, // RDA; Location: local
    TXT_GUISHUDIUNKNOWN, // RDA; Location: unknown 
    TXT_GUISHUDI_NOCOMMA, // RDA; Location
    TXT_HANDFREESWITCH, // RDA; Hands-free to earpiece
    TXT_HAND_FREE, // RDA; Hands-free
    TXT_HAND_ON, // RDA; Earpiece 
    TXT_HELP, // RDA; Help
    TXT_HELP_CONTENT, // RDA; Please plug in the compatible earphone to the device and then turn on the radio. The earphone cable can be used as FM Antenna. Please adjust appropriate volume when listening to the radio. Continue to use higher volume is harmful for your ear.
    TXT_IDENTIFICATION, // RDA; Number identification
    TXT_IDLE_CALENDAR, // RDA; Calendar
    TXT_IDLE_LOCK, // RDA; Lock
    TXT_IDLE_MISSED_EVENT_DELETE, // RDA; Delete?
    TXT_IDLE_MISSED_EVENT_DELETEALL, // RDA; Delete all?
    TXT_IDLE_MISSED_EVENT_MORE, // RDA; Missed alarm events
    TXT_IDLE_MISSED_EVENT_NONE, // RDA; No event
    TXT_IDLE_MISSED_EVENT_ONE, // RDA; Missed alarm event
    TXT_IDLE_NOCALENDAR, // RDA; No schedule today
    TXT_IDLE_NOMORECALENDAR, // RDA; No more schedule today
    TXT_IDLE_NO_SIM, // RDA; Insert SIM
    TXT_IDLE_SHORT_APR, // RDA; Apr.
    TXT_IDLE_SHORT_AUG, // RDA; Aug.
    TXT_IDLE_SHORT_DEC, // RDA; Dec.
    TXT_IDLE_SHORT_FEB, // RDA; Feb.
    TXT_IDLE_SHORT_JAN, // RDA; Jan.
    TXT_IDLE_SHORT_JUL, // RDA; Jul.
    TXT_IDLE_SHORT_JUN, // RDA; Jun.
    TXT_IDLE_SHORT_MAR, // RDA; Mar.
    TXT_IDLE_SHORT_MAY, // RDA; May 
    TXT_IDLE_SHORT_NOV, // RDA; Nov.
    TXT_IDLE_SHORT_OCT, // RDA; Oct.
    TXT_IDLE_SHORT_SEP, // RDA; Sep.
    TXT_IDLE_UNLOCK, // RDA; Unlock
    TXT_IDLE_UNLOCK_TIP2, // RDA; Unlock keyboard?
    TXT_IDL_KL_DT, // RDA; Keypad is locked. Long press the * key
    TXT_IDL_KL_LOCK, // RDA; Keypad locked
    TXT_IDL_KL_SC_EMRGCY, // RDA; Emergency call
    TXT_IDL_KL_UNLOCK, // RDA; Keypad unlocked
    TXT_IMEISVN_DISPLAY, // RDA; IMEISV:
    TXT_IMEI_DISPLAY, // RDA; IMEI:
    TXT_INCOMING_FORWARED_TO_C, // RDA; Incoming call has been transferred 
    TXT_INCOMING_IS_FORWARED, // RDA; Incoming call is a forwarded call
    TXT_INPUT_ERROR, // RDA; Input error
    TXT_INPUT_NEW_BAR_PW, // RDA; Input new password(4)
    TXT_INPUT_NEW_BAR_PW_AGAIN, // RDA; Input new password again(4)
    TXT_INPUT_NEW_PASSWORD, // RDA; New password
    TXT_INPUT_NEW_PWD, // RDA; Input new password(4-8)
    TXT_INPUT_NEW_PWDAGAIN, // RDA; Confirm
    TXT_INPUT_NEW_PWD_CNF, // RDA; Confirm new password(4-8)
    TXT_INPUT_PASSWORD, // RDA; Input password(4)
    TXT_INPUT_PHONE_PASSWORD, // RDA; Please input password(4-8)
    TXT_INPUT_PWD, // RDA; Input power on password(4-8)
    TXT_INQUIRE, // RDA; Query
    TXT_INSERT_SIM, // RDA; Please insert SIM card
    TXT_INSERT_SIM_AND_INFO, // RDA; Insert SIM or switch to standby mode
    TXT_INVALID_CUSTOM_TIME, // RDA; Invalid time. Enter number between 1 to 60
    TXT_INVALID_DATE, // RDA; Invalid date
    TXT_INVALID_TIME, // RDA; Invalid time
    TXT_IQ_DATA_PROCESSING, // RDA; IQ data processing!
    TXT_JAVA, // RDA; Java
    TXT_JAVA_LCDUI_DF_APR_SHORT, // RDA; Apr
    TXT_JAVA_LCDUI_DF_AUG_SHORT, // RDA; Aug
    TXT_JAVA_LCDUI_DF_DEC_SHORT, // RDA; Dec
    TXT_JAVA_LCDUI_DF_FEB_SHORT, // RDA; Feb
    TXT_JAVA_LCDUI_DF_JAN_SHORT, // RDA; Jan
    TXT_JAVA_LCDUI_DF_JUL_SHORT, // RDA; Jul
    TXT_JAVA_LCDUI_DF_JUN_SHORT, // RDA; Jun
    TXT_JAVA_LCDUI_DF_MAR_SHORT, // RDA; Mar
    TXT_JAVA_LCDUI_DF_MAY_SHORT, // RDA; May
    TXT_JAVA_LCDUI_DF_NOV_SHORT, // RDA; Nov
    TXT_JAVA_LCDUI_DF_OCT_SHORT, // RDA; Oct
    TXT_JAVA_LCDUI_DF_SEP_SHORT, // RDA; Sep
    TXT_KL_PSW_EMERGENCY_CALL, // RDA; Emergency call
    TXT_KL_TIME_AM, // RDA; AM
    TXT_KL_TIME_PM, // RDA; PM
    TXT_KS_IMSI_REPLACEMENT_SIM1_OFF, // RDA; SIM1 IMSI Replacement Off
    TXT_KS_IMSI_REPLACEMENT_SIM1_ON, // RDA; SIM1 IMSI Replacement On
    TXT_KS_IMSI_REPLACEMENT_SIM2_OFF, // RDA; SIM1 IMSI Replacement Off
    TXT_KS_IMSI_REPLACEMENT_SIM2_ON, // RDA; SIM2 IMSI Replacement On
    TXT_LENGTH_ERROR, // RDA; Invalid length
    TXT_LOCAL, // RDA; Local
    TXT_MAINMENU_COMMUNICATION, // RDA; 1.Communication
    TXT_MAINMENU_ICON_APPS, // RDA; Activity
    TXT_MAINMENU_ICON_PASTIME, // RDA; Entertainment
    TXT_MAINMENU_ICON_SETTINGS, // RDA; Settings
    TXT_MAINMENU_ICON_TOOLS, // RDA; Tools
    TXT_MAINMENU_PASTIME, // RDA; 4. Entertainment
    TXT_MAINMENU_SETTINGS, // RDA; 2. Settings
    TXT_MAINMENU_TOOLS, // RDA; 3. Tools
    TXT_MAIN_PB, // RDA; Contacts
    TXT_MEMO, // RDA; Memo
    TXT_MEMORY_FULL, // RDA; Memory full
    TXT_MESSAGE_INFOSERVICE, // RDA; Information service
    TXT_MESSAGE_IVR, // RDA; Music talkshow
    TXT_MESSAGE_SETTING_SMS, // RDA; SMS settings
    TXT_MESSAGE_SMS, // RDA; SMS
    TXT_MMI_DISABLE, // RDA; Forbidden
    TXT_MMI_ENABLE, // RDA; Activate
    TXT_MMI_NAME, // RDA; Name
    TXT_MMI_NUMBER, // RDA; Number
    TXT_MMI_SAVESUCC, // RDA; Saved
    TXT_MMS_CREATION_MODE, // RDA; Creation mode 
    TXT_MMS_FREE, // RDA; Free
    TXT_MMS_FREE_INFO, // RDA; No size and content restrictions
    TXT_MMS_RESTRICTED, // RDA; Restricted
    TXT_MMS_RESTRICTED_INFO, // RDA; Message class will restrict size and content
    TXT_MMS_TOTAL_SIZE, // RDA; Total size
    TXT_MMS_WARNING, // RDA; Warning
    TXT_MMS_WARNING_INFO, // RDA; You will be notified when size and content exceed maximum allowed
    TXT_MODE, // RDA; Mode
    TXT_MODIFY, // RDA; Modify
    TXT_MODIFY_PASSWORD, // RDA; Change password
    TXT_MOD_DYNA_ISVALID, // RDA; The application isn't verified, do you still want to run it?
    TXT_MONDAY, // RDA; Monday
    TXT_MSD_ILLEGAL_USE, // RDA; Dailing for reporting the loss
    TXT_MSD_ILLEGAL_USE_AND_CLOSE, // RDA; Illegal use, will be powered off soon
    TXT_MSGBOX_NO_REPLY_NUMBER, // RDA; Number can't be null
    TXT_MSGBOX_NO_REPLY_TIME, // RDA; No reply time was incorrect
    TXT_MSGBOX_PASSWORD, // RDA; Incorrect digits of password
    TXT_MSGBOX_PIN2_CONFLICT, // RDA; Different PIN2 codes
    TXT_MSGBOX_PIN_CONFLICT, // RDA; Different PIN codes
    TXT_MSGBOX_PWD_CONFLICT, // RDA; Different passwords
    TXT_MTFW_ADD, // RDA; Add
    TXT_MTFW_LIST, // RDA; List
    TXT_MTFW_TITLE, // RDA; Incoming call firewall
    TXT_MULTIM_EMPTY_CONTENT, // RDA; Empty
    TXT_MULTIM_NO_EXIST, // RDA; File doesn't exist!
    TXT_MUST_SELECT_ONE, // RDA; Must select one
    TXT_NAME_EMPTY, // RDA; Name is empty
    TXT_NETWORK_CONNECT, // RDA; Network connect
    TXT_NETWORK_LIST, // RDA; Network list
    TXT_NETWORK_SEARCHING, // RDA; Network search
    TXT_NETWORK_SELECT, // RDA; Network selection
    TXT_NET_LEO, // RDA; leo™
    TXT_NEW_GAME, // RDA; New game
    TXT_NOT_PROVISION, // RDA; Don't provide
    TXT_NOT_SUPPORT_SIM, // RDA; Don't support this SIM
    TXT_NO_ACTIVATION, // RDA; Inactive
    TXT_NO_ENTRY, // RDA; No record
    TXT_NO_MESSAGE, // RDA; No message
    TXT_NO_REGISTERED, // RDA; Not registered
    TXT_NO_SD_CARD_ALERT, // RDA; No MemoryCard available!
    TXT_NO_SERVICE, // RDA; No service
    TXT_NO_SIM, // RDA; No SIM card
    TXT_OPEN, // RDA; Activated
    TXT_OPERATE, // RDA; Operate
    TXT_OPTION_MENU, // RDA; Options
    TXT_OUTGOING_FORWARED_TO_C, // RDA; Outgoing call forwarded
    TXT_OVERWRITE_Q_OTHERWISE_RECEIVING_WILL_BE_FAILED, // RDA; Overwrite? Otherwise receiving will be failed.
    TXT_PASSWORD_ERROR, // RDA; Incorrect password
    TXT_PASSWORD_INVALID, // RDA; Invalid password
    TXT_PAUSE, // RDA; Pause
    TXT_PB_ADD, // RDA; New
    TXT_PB_ADDED, // RDA; added
    TXT_PB_ADDING, // RDA; Adding
    TXT_PB_ADD_DETAILS, // RDA; Add details
    TXT_PB_ADD_ENTRY_FAX, // RDA; Fax
    TXT_PB_ADD_ENTRY_MOBILE, // RDA; Mobile
    TXT_PB_ADD_ENTRY_TEL, // RDA; Telephone
    TXT_PB_ADD_IN_NV, // RDA; Phone
    TXT_PB_ADD_IN_SIM, // RDA; SIM
    TXT_PB_ADD_MEMBER, // RDA; Add member
    TXT_PB_BIRTHDAY, // RDA; Birthday
    TXT_PB_CALLER_IMAGE, // RDA; Caller image
    TXT_PB_CHANGED, // RDA; Changed
    TXT_PB_CHANGE_TYPE, // RDA; Change type
    TXT_PB_CLOSE_SEARCH, // RDA; Close Search
    TXT_PB_CONTACTS, // RDA; contacts... 
    TXT_PB_COPIED_TO, // RDA; Copied to
    TXT_PB_COPYED, // RDA; Copy succeeded!
    TXT_PB_COPY_ALL, // RDA; Copy all
    TXT_PB_COPY_ALL_TO_NV, // RDA; From SIM to phone
    TXT_PB_COPY_ALL_TO_SIM, // RDA; Phone to SIM
    TXT_PB_COPY_FROM_NV_TO_SIM1, // RDA; Copy to SIM1
    TXT_PB_COPY_FROM_NV_TO_SIM2, // RDA; Copy to SIM2
    TXT_PB_COPY_TO_SIM, // RDA; Copy to SIM
    TXT_PB_COPY_TO_SIM_COTINUE, // RDA; continue?
    TXT_PB_COPY_TO_SIM_NOTIFY, // RDA; Some of the data might be lost.
    TXT_PB_DEFAULT_MELODIES, // RDA; Default melodies
    TXT_PB_DEFAULT_NUM, // RDA; Default numbers
    TXT_PB_DELETE_ALL_OR_NOT, // RDA; Delete all?
    TXT_PB_EMAIL_LIST_ITEM, // RDA; Empty
    TXT_PB_ENTRY_LIST, // RDA; Entry list
    TXT_PB_ERASE_ALL, // RDA; Delete all
    TXT_PB_ERASE_NV_ALL, // RDA; Phone
    TXT_PB_ERASE_OR_NOT, // RDA; Delete
    TXT_PB_ERASE_SIM_ALL, // RDA; SIM all
    TXT_PB_ERROR, // RDA; Error
    TXT_PB_FAILURE, // RDA; Failed
    TXT_PB_FAX_NUMBER, // RDA; Fax number
    TXT_PB_FDN, // RDA; FDN contacts
    TXT_PB_FDN_FULL, // RDA; FDN full
    TXT_PB_FDN_NUM, // RDA; Fixed dial number
    TXT_PB_GROUP, // RDA; Group
    TXT_PB_GROUP_ALL, // RDA; All
    TXT_PB_GROUP_BLACKLIST, // RDA; Blacklist
    TXT_PB_GROUP_CONTACTS, // RDA; Contacts
    TXT_PB_GROUP_FAVOURITES, // RDA; Favourites
    TXT_PB_GROUP_LIST, // RDA; Select group
    TXT_PB_GROUP_MEMBER_ADD, // RDA; Add name
    TXT_PB_GROUP_MEMBER_ERASE, // RDA; Delete name
    TXT_PB_GROUP_SIM, // RDA; SIM
    TXT_PB_IMPORT, // RDA; Add from Contacts
    TXT_PB_IMPORT_FROM_ALL, // RDA; All
    TXT_PB_IMPORT_FROM_PHONE, // RDA; Phone
    TXT_PB_IMPORT_FROM_SIM, // RDA; SIM
    TXT_PB_INSUFFICIENT_SPACE, // RDA; Not enough space
    TXT_PB_INVALID_MAIL, // RDA; Invalid email address
    TXT_PB_ITEM_DETAIL_COMPANY, // RDA; Company
    TXT_PB_ITEM_DETAIL_CUSTOM_IMAGE, // RDA; Image
    TXT_PB_ITEM_DETAIL_CUSTOM_MEMO, // RDA; Memo
    TXT_PB_ITEM_DETAIL_CUSTOM_RING, // RDA; Ringtone
    TXT_PB_ITEM_DETAIL_CUSTOM_SELECT_GROUP, // RDA; Group
    TXT_PB_ITEM_DETAIL_EMAIL, // RDA; Email
    TXT_PB_ITEM_DETAIL_HOME_PHONE, // RDA; Home number
    TXT_PB_ITEM_DETAIL_MOBILE_PHONE, // RDA; Mobile
    TXT_PB_ITEM_DETAIL_NAME, // RDA; Name
    TXT_PB_ITEM_DETAIL_OFFICE_PHONE, // RDA; Office number
    TXT_PB_ITEM_DETAIL_ORG, // RDA; Title
    TXT_PB_ITEM_DETAIL_WORKADR, // RDA; Company address
    TXT_PB_JOBTITLE, // RDA; Job title
    TXT_PB_LIST_MODE_SINGLE, // RDA; Single line display
    TXT_PB_MAIL, // RDA; Email
    TXT_PB_MAXIMUM_REACHED, // RDA; Maximum number of groups reached!
    TXT_PB_MENU_CHANGE_GROUP, // RDA; Change group
    TXT_PB_MORE_RING, // RDA; More ringtones
    TXT_PB_MSISDN, // RDA; Caller ID
    TXT_PB_NAME, // RDA; Name
    TXT_PB_NEED_TO_SAVE, // RDA; Save?
    TXT_PB_NEXT_STEP, // RDA; Next
    TXT_PB_NONE, // RDA; None
    TXT_PB_NOTE, // RDA; Note
    TXT_PB_NOT_READY, // RDA; Contacts not ready!
    TXT_PB_NO_ENTRY, // RDA; No entry
    TXT_PB_NO_INPUT, // RDA; No input
    TXT_PB_NO_MEMBERS, // RDA; No members
    TXT_PB_NO_MSISDN, // RDA; No caller ID
    TXT_PB_NO_PHONE, // RDA; Number is null
    TXT_PB_NUMBER_IS_NEEDED, // RDA; Number is Needed
    TXT_PB_NV_FULL, // RDA; Phone storage full
    TXT_PB_OPEN, // RDA; Open
    TXT_PB_OPTION_OTHERS, // RDA; Others
    TXT_PB_OVERWRITE_OR_NOT, // RDA; Replace?
    TXT_PB_OWNNUMBER_RESET, // RDA; Reset?
    TXT_PB_PLEASE_INPUT_GROUP, // RDA; Please input group name
    TXT_PB_POPULAR_EMAILACCONTS, // RDA; Popular email accounts
    TXT_PB_REMOVE_MULTIPLE_MEMBER, // RDA; Remove multiple
    TXT_PB_REMOVING, // RDA; Removing
    TXT_PB_RENAME_GROUP, // RDA; Rename
    TXT_PB_SAME_NAME, // RDA; Same name
    TXT_PB_SEARCH, // RDA; Search
    TXT_PB_SEARCH_OPERATE, // RDA; Operate
    TXT_PB_SEARCH_OPERATE_CALL, // RDA; Call
    TXT_PB_SEARCH_OPERATE_MODIFY, // RDA; Modify
    TXT_PB_SEARCH_OPERATE_MTFW, // RDA; Add to blacklist
    TXT_PB_SELECT, // RDA; Select SIM
    TXT_PB_SELECT_NUMBER, // RDA; Select number
    TXT_PB_SELECT_PHONE, // RDA; Select number
    TXT_PB_SEND_VCARD, // RDA; Send vCard
    TXT_PB_SETTING, // RDA; Settings
    TXT_PB_SETTING_GROUP, // RDA; Operate
    TXT_PB_SETTING_GROUP_DELETE, // RDA; Delete group
    TXT_PB_SETTING_GROUP_MEMBER, // RDA; Group members
    TXT_PB_SETTING_GROUP_MODIFY, // RDA; Change group name
    TXT_PB_SETTING_GROUP_NAME, // RDA; Name group
    TXT_PB_SETTING_GROUP_RING, // RDA; Group ringtone
    TXT_PB_SETTING_NAMECARD, // RDA; My namecard
    TXT_PB_SETTING_OWN_NUMBER, // RDA; Own numbers
    TXT_PB_SETTING_SAVE, // RDA; Save new contacts to
    TXT_PB_SETTING_SAVE_ASK, // RDA; Always ask
    TXT_PB_SETTING_VIEW_ALL, // RDA; All
    TXT_PB_SETTING_VIEW_PNONE, // RDA; Phone
    TXT_PB_SETTING_VIEW_SIM, // RDA; SIM
    TXT_PB_SIM_BLOCK, // RDA; SIM card is block
    TXT_PB_SIM_FULL, // RDA; SIM full
    TXT_PB_SOFTKEY_ADD, // RDA; Add
    TXT_PB_SPACE_MAIL, // RDA; Email
    TXT_PB_SPACE_NAME, // RDA; Name
    TXT_PB_SPACE_NO_NAME, // RDA; <No Name>
    TXT_PB_SPACE_PHONE, // RDA; <Number>
    TXT_PB_STRING_LEN_LARGE, // RDA; Too long name! It will be truncated!
    TXT_PB_SUCCESS, // RDA; Succeeded
    TXT_PB_TIPS_MODE_DOUBLE, // RDA; Double line display
    TXT_PB_TO_SAVE_CARD, // RDA; Copy to MemoryCard
    TXT_PB_UNCLASS_GROUP, // RDA; Unclassified
    TXT_PB_USED_SPACE, // RDA; Memory status
    TXT_PB_WRITE_MESSAGE, // RDA; Write message
    TXT_PHONE_CHARGE_OK, // RDA; Battery full
    TXT_PHONE_IMEI, // RDA; IMEI
    TXT_PHONE_LOCK_PASSED, // RDA; Phone lock passed
    TXT_PIN1_DISABLED, // RDA; PIN deactivated
    TXT_PIN1_ENABLED, // RDA; PIN activated
    TXT_PIN2_BLOCKED, // RDA; PIN2 blocked
    TXT_PIN2_CHANGED, // RDA; PIN2 changed
    TXT_PIN2_ERROR, // RDA; PIN2 error
    TXT_PIN_BLOCKED, // RDA; PIN blocked
    TXT_PIN_CHANGED, // RDA; PIN changed
    TXT_PIN_ERROR, // RDA; PIN error
    TXT_PIN_LOCK_PASSED, // RDA; PIN lock passed
    TXT_PIN_PUK_REMAIN, // RDA; Remain
    TXT_PIN_PUK_TIMES, // RDA; time(s)
    TXT_PLEASE_WAIT, // RDA; Please wait
    TXT_PLMN_SELECT_COMPLETE, // RDA; Network selected
    TXT_PLMN_SELECT_FAIL, // RDA; Network selection failed
    TXT_PLS_INPUT_PIN, // RDA; Please input PIN
    TXT_PLS_INPUT_PUK, // RDA; Please input PUK
    TXT_POWER_OFF, // RDA; Power off
    TXT_POWER_ON_PWD, // RDA; Power-on password
    TXT_PRIOR_STORE_MEDIUM, // RDA; Preferred storage
    TXT_PROCESS_SEARCHING_NETWORK, // RDA; Searching network...
    TXT_PROMPT_FAIL, // RDA; Fail
    TXT_PROVISION, // RDA; Provide
    TXT_PUB_DELETING_SMS, // RDA; Deleting message...
    TXT_PUB_SAVING, // RDA; Saving...
    TXT_PUK2_BLOCKED, // RDA; PUK2 blocked
    TXT_PUK2_ERROR, // RDA; PUK2 error
    TXT_PUK_BLOCKED, // RDA; PUK blocked
    TXT_PUK_ERROR, // RDA; Wrong PUK
    TXT_PWD_DISABLED, // RDA; Password deactivated
    TXT_PWD_ENABLED, // RDA; Password activated
    TXT_REGISTER_SERVICE, // RDA; Registering
    TXT_REJECT, // RDA; Rejected
    TXT_REJECT_CALL_WITH_MESSAGE1, // RDA; Call you back later
    TXT_REJECT_CALL_WITH_MESSAGE2, // RDA; I'm at meeting
    TXT_REJECT_CALL_WITH_MESSAGE3, // RDA; I'm in class
    TXT_REJECT_CALL_WITH_MESSAGE4, // RDA; I'm having a meal
    TXT_REJECT_CALL_WITH_MESSAGE5, // RDA; I'm driving
    TXT_REMOVE, // RDA; Remove
    TXT_REMOVED, // RDA; removed
    TXT_REMOVE_QUERY, // RDA; Remove?
    TXT_RING, // RDA; Ringtone
    TXT_RING1, // RDA; Ring1
    TXT_RING2, // RDA; Ring2
    TXT_RING3, // RDA; Ring3
    TXT_RING4, // RDA; Ring4
    TXT_RING5, // RDA; Ring5
    TXT_RING6, // RDA; Ring6
    TXT_RING7, // RDA; Ring7
    TXT_RING8, // RDA; Ring8
    TXT_RING_FIXED, // RDA; Fixed ringtones
    TXT_RING_SILENT, // RDA; Silent
    TXT_RING_VOLUME, // RDA; Adjust volume
    TXT_RING_VOLUME_ALARM, // RDA; Alarm&calendar ringtone
    TXT_SAMSUNG, // RDA; Under Developing
    TXT_SATURDAY, // RDA; Saturday
    TXT_SCANNING_NETWORK, // RDA; Scanning network...
    TXT_SCH_ALLSCH, // RDA; All schedules
    TXT_SCH_AWAKE_EVERYDAY, // RDA; Daily
    TXT_SCH_AWAKE_EVERYMONTH, // RDA; Monthly
    TXT_SCH_AWAKE_EVERYWEEK, // RDA; Weekly
    TXT_SCH_AWAKE_EVERYYEAR, // RDA; Yearly
    TXT_SCH_AWAKE_NO, // RDA; No reminding
    TXT_SCH_AWAKE_ONCE, // RDA; Remind once
    TXT_SCH_INPUT_DATE, // RDA; Date
    TXT_SCH_INPUT_TIME, // RDA; Time
    TXT_SCH_INVALID_DEADLINE, // RDA; Invalid deadline
    TXT_SCH_INVALID_TITLE, // RDA; Invalid title
    TXT_SCH_NEW, // RDA; New
    TXT_SCH_SCH, // RDA; Calendar
    TXT_SD_NOT_SUPPORT, // RDA; Unsupported MemoryCard, format it
    TXT_SEARCH_FOR_NUMBER, // RDA; Search for number
    TXT_SECOND, // RDA; s
    TXT_SELECT_MESSAGE, // RDA; Select message
    TXT_SELECT_SIM1, // RDA; SIM1
    TXT_SELECT_SIM2, // RDA; SIM2
    TXT_SEND, // RDA; Send
    TXT_SENDING, // RDA; Sending...
    TXT_SEND_BY_BT, // RDA; Bluetooth
    TXT_SEND_BY_SMS, // RDA; SMS
    TXT_SEND_MESSAGE, // RDA; Send message
    TXT_SEND_SMS, // RDA; SMS
    TXT_SEND_SUCCEED, // RDA; Send succeeded
    TXT_SETTINGS_APP, // RDA; Activity
    TXT_SETTINGS_APPLICATION, // RDA; Activity
    TXT_SETTINGS_CALL, // RDA; Call settings
    TXT_SETTINGS_CONTACT, // RDA; Contacts
    TXT_SETTINGS_DISPLAY, // RDA; Display settings
    TXT_SETTINGS_ENVIONMENT, // RDA; Profiles
    TXT_SETTINGS_IDLE_DUAL, // RDA; Dual clock
    TXT_SETTINGS_IDLE_ONE, // RDA; Single clock 1
    TXT_SETTINGS_IDLE_OPT_CONFIRM, // RDA; Confirm
    TXT_SETTINGS_IDLE_OPT_EDIT, // RDA; Edit shortcuts
    TXT_SETTINGS_IDLE_OPT_MOVE, // RDA; Move
    TXT_SETTINGS_IDLE_OPT_ORDER, // RDA; Change order
    TXT_SETTINGS_IDLE_SINGLE, // RDA; Single clock
    TXT_SETTINGS_IDLE_SMART, // RDA; Smart idle
    TXT_SETTINGS_IDLE_SMART_SHORTCUT, // RDA; Shortcuts toolbar
    TXT_SETTINGS_IDLE_THREE, // RDA; Single clock 3
    TXT_SETTINGS_IDLE_TWO, // RDA; Single clock 2
    TXT_SETTINGS_LINK, // RDA; Connection management
    TXT_SETTINGS_MASTER, // RDA; Master reset will reset all settings and erase all personal data Continue?
    TXT_SETTINGS_MEM, // RDA; Memory
    TXT_SETTINGS_MESSAGE, // RDA; Message
    TXT_SETTINGS_NETWORK, // RDA; Network
    TXT_SETTINGS_PHONE, // RDA; Phone settings
    TXT_SETTINGS_RESET, // RDA; Reset
    TXT_SETTINGS_RESET_MASTER, // RDA; Master reset
    TXT_SETTINGS_RESET_NONE, // RDA; Nothing selected
    TXT_SETTINGS_RESET_RESET, // RDA; Reset settings
    TXT_SETTINGS_RESET_RESET_ALL, // RDA; Select all
    TXT_SETTINGS_RESET_RESET_DISPLAY, // RDA; Display
    TXT_SETTINGS_RESET_RESET_PHONE, // RDA; Phone
    TXT_SETTINGS_RESET_RESET_SOUND, // RDA; Sound profiles
    TXT_SETTINGS_SECURITY, // RDA; Security settings
    TXT_SETTINGS_SIM_MANAGER, // RDA; SIM cards management
    TXT_SETTINGS_TD, // RDA; Time and date
    TXT_SET_ADDRESS, // RDA; Address
    TXT_SET_ALERT_ON_CALL, // RDA; Alert on call
    TXT_SET_ALERT_ON_CALL_OFF_HELP, // RDA; There will not be alert sounds during calls
    TXT_SET_ALERT_ON_CALL_ON_HELP, // RDA; There will be alert sounds during calls.
    TXT_SET_ALL_CALLS, // RDA; All Calls
    TXT_SET_ANIMATION, // RDA; Animation effect
    TXT_SET_ANIMATION_1, // RDA; Animation1
    TXT_SET_ANIMATION_2, // RDA; Animation2
    TXT_SET_ANIMATION_3, // RDA; Animation3
    TXT_SET_ANIMATION_WALLPAPER, // RDA; Animation wallpaper
    TXT_SET_ANSWERING_MODE, // RDA; Answering mode
    TXT_SET_ANSWER_ATUO_HELP, // RDA; Incoming calls will be answered automatically. Automatic answering will operate only with earpiece connected. Automatic answering is not possible when call alert type is set to mute
    TXT_SET_ANYKEY, // RDA; Any key
    TXT_SET_ANYKEY_ANSWER, // RDA; Any-key answer
    TXT_SET_ANYKEY_HELP, // RDA; Press any key to answer incoming calls
    TXT_SET_ANYKEY_REJECT_MODE, // RDA; Answer/Reject mode
    TXT_SET_AUTO, // RDA; Auto
    TXT_SET_AUTO_BOADRKEY, // RDA; Auto keypad lock
    TXT_SET_AUTO_KEYLOCK_CLOSE_TIP, // RDA; Keypad will not be locked when LCD is off
    TXT_SET_AUTO_KEYLOCK_OPEN_TIP, // RDA; Keypad will be locked when LCD is off
    TXT_SET_AUTO_REDAIL, // RDA; Auto redial
    TXT_SET_AUTO_REDIAL_OFF_HELP, // RDA; Do not redial automatically if call is unable to connect or is cut off.
    TXT_SET_AUTO_REDIAL_ON_HELP, // RDA; Redial automatically if call is unable to connect or is cut off.
    TXT_SET_AUTO_REJECT, // RDA; Auto reject
    TXT_SET_AUTO_SEL, // RDA; Automatic
    TXT_SET_BACK_LIGHT, // RDA; Backlight
    TXT_SET_BAIC, // RDA; All incoming
    TXT_SET_BAND, // RDA; Band select
    TXT_SET_BAOC, // RDA; All outgoing
    TXT_SET_BAR_ALL_MO_CALLS, // RDA; All outgoing calls
    TXT_SET_BAR_ALL_MT_CALLS, // RDA; All incoming calls
    TXT_SET_BAR_BICROAM, // RDA; Incoming while abroad
    TXT_SET_BAR_CANCEL_ALL, // RDA; Cancel all
    TXT_SET_BAR_CHANGE_PW, // RDA; Change password
    TXT_SET_BAR_MO_CALLS_INTL, // RDA; Outgoing international calls
    TXT_SET_BAR_MO_CALLS_INTL_ROAM, // RDA; International calls except home
    TXT_SET_BAR_MT_CALLS_ROAM, // RDA; Incoming calls when roaming
    TXT_SET_BOIC, // RDA; Internaltional
    TXT_SET_BOIC_EXHC, // RDA; Internaltional except home
    TXT_SET_BRIGHTNESS, // RDA; Brightness
    TXT_SET_BY_ADDRESS, // RDA; By address
    TXT_SET_BY_NETWORK, // RDA; By network
    TXT_SET_CALL, // RDA; Call settings
    TXT_SET_CALLER_INFORMATION, // RDA; Caller information
    TXT_SET_CALL_ALERT_TYPE, // RDA; Call alert type
    TXT_SET_CALL_BAR, // RDA; Call barred
    TXT_SET_CALL_CONNECT_TONE, // RDA; Call connect tone
    TXT_SET_CALL_CONNECT_TONE_HELP, // RDA; There will be a signal sound when the line is connected
    TXT_SET_CALL_DIVERTS, // RDA; Call divert
    TXT_SET_CALL_END_TONE, // RDA; Call end tone
    TXT_SET_CALL_END_TONE_HELP, // RDA; There will be a signal sound when the line is disconnected
    TXT_SET_CALL_TIME_LIMIT, // RDA; Call time limit
    TXT_SET_CALL_WAITING, // RDA; Call waiting
    TXT_SET_CFB, // RDA; Busy
    TXT_SET_CFNRC, // RDA; Unreachable
    TXT_SET_CFNRY, // RDA; No reply
    TXT_SET_CFU, // RDA; Divert always
    TXT_SET_CHANGE_PASSWORD, // RDA; Change password
    TXT_SET_CHANGE_PIN1, // RDA; Change PIN
    TXT_SET_CHANGE_PIN2, // RDA; Change PIN2
    TXT_SET_CLEAN_USER_DATA, // RDA; Clear user data?
    TXT_SET_CLIP_ID, // RDA; Show calling ID
    TXT_SET_CLIR_SEND_NUMBER, // RDA; Send number
    TXT_SET_COLP_ID, // RDA; Display all received numbers
    TXT_SET_COLR_ID, // RDA; Hide all received numbers
    TXT_SET_CONNECT_PROMPT, // RDA; Vibration when connected
    TXT_SET_CONTRAST, // RDA; Contrast
    TXT_SET_COORDINATE, // RDA; Adjust touch screen
    TXT_SET_CREATE_REJECT_NUMBER, // RDA; Create reject number
    TXT_SET_CUSTOM_WORD, // RDA; Customized word
    TXT_SET_DATA_CALL, // RDA; Data Call
    TXT_SET_DATA_NUMBER, // RDA; Information number
    TXT_SET_DATE, // RDA; Set date
    TXT_SET_DATE_DISPLAY_TYPE, // RDA; Date format
    TXT_SET_DATE_DISPLAY_TYPE_DMY, // RDA; DD/MM/YYYY
    TXT_SET_DATE_DISPLAY_TYPE_YMD, // RDA; YYYY/MM/DD
    TXT_SET_DATE_DMY, // RDA; Day-Mon-Year
    TXT_SET_DATE_HOME_ZONE, // RDA; Home zone
    TXT_SET_DATE_MDY, // RDA; Mon-Day-Year
    TXT_SET_DATE_YMD, // RDA; Year-Mon-Day
    TXT_SET_DISPLAY, // RDA; Display settings
    TXT_SET_DIVERTS_ALL_CALLS, // RDA; Always
    TXT_SET_DIVERTS_CANCEL_ALL, // RDA; Cancel all diverts
    TXT_SET_DIVERTS_NOT_REACHABLE, // RDA; If out of reach
    TXT_SET_DIVERTS_NO_ANSWER, // RDA; If no reply
    TXT_SET_DIVERTS_WHEN_BUSY, // RDA; If busy
    TXT_SET_DIVERT_CANCEL_ALL_ALERT, // RDA; All diverting voice calls inactive
    TXT_SET_DIVERT_CANCEL_ALWAYS_ALERT, // RDA; Divert always voice calls inactive
    TXT_SET_DIVERT_CANCEL_BUSY_ALERT, // RDA; Busy voice calls inactive
    TXT_SET_DIVERT_CANCEL_NOREPLY_ALERT, // RDA; No reply voice calls inactive
    TXT_SET_DIVERT_CANCEL_UNREACHABLE_ALERT, // RDA; Unreachable voice calls inactive
    TXT_SET_DIVERT_NUMBER, // RDA; Divert to number
    TXT_SET_DIVERT_TO, // RDA; Divert to
    TXT_SET_DIVERT_VOICE, // RDA; Divert to voicemail
    TXT_SET_DUAL_BAND, // RDA; Dual band
    TXT_SET_DUAL_HELP_NSK_DETAIL, // RDA; In Dual standby mode, you can change your current network by pressing the network switch key.  Indicator displays current network selected for calls and text messages
    TXT_SET_EBOOK, // RDA; eBook
    TXT_SET_EDIT_BLOCKED_ADDRESS, // RDA; Edit blocked address
    TXT_SET_EDIT_REJECT_NUMBER, // RDA; Edit reject number
    TXT_SET_EMERGENCY, // RDA; Emergency
    TXT_SET_ENHANCE_RING, // RDA; Timbre bright
    TXT_SET_ENTER_NUMBER, // RDA; Enter number
    TXT_SET_ENTRY_PASSSHORT, // RDA; Password too short
    TXT_SET_ENTRY_PASSWORD, // RDA; Enter password
    TXT_SET_FAKE_CALL, // RDA; Fake Call
    TXT_SET_FAX_NUMBER, // RDA; Fax number
    TXT_SET_FDM, // RDA; Fixed dialling mode
    TXT_SET_FDN, // RDA; Fixed dial number
    TXT_SET_FDN_SETTINGS, // RDA; FDN Settings
    TXT_SET_FINISHED, // RDA; Done
    TXT_SET_FLIGHT_MODE_QUERY, // RDA; Deactivate Flight  mode to use network services
    TXT_SET_FLIP_ANSWER, // RDA; Flip to answer
    TXT_SET_FLY_MODE, // RDA; Flight mode
    TXT_SET_FM_RADIO, // RDA; FM radio
    TXT_SET_FORBIDDEN_MOVE, // RDA; No slide
    TXT_SET_GREATING, // RDA; Greeting message
    TXT_SET_GSM, // RDA; GSM
    TXT_SET_HAND, // RDA; Manual
    TXT_SET_HELP, // RDA; Help
    TXT_SET_HIDE, // RDA; Hide
    TXT_SET_HIDE_ID, // RDA; Hide ID
    TXT_SET_HOME_ZONE_DST_OFF, // RDA; DST off
    TXT_SET_HOME_ZONE_DST_ON, // RDA; DST on
    TXT_SET_HOME_ZONE_TITLE, // RDA; Time zone
    TXT_SET_HORIZONTAL_MOVE_STYLE, // RDA; Horizontal slide
    TXT_SET_IDLE_COMMON, // RDA; Standard style
    TXT_SET_IDLE_DISPLAY, // RDA; Idle display settings
    TXT_SET_IDLE_SMART, // RDA; Smart style
    TXT_SET_IDLE_STYLE, // RDA; Switch system
    TXT_SET_INPUT_LANGUAGE, // RDA; Input language
    TXT_SET_INPUT_MODE, // RDA; Input method settings
    TXT_SET_INPUT_NEW_PRIVACY_PWD, // RDA; Input new password(4-8)
    TXT_SET_INPUT_NEW_PRIVACY_PWD_CNF, // RDA; Confirm new password(4-8)
    TXT_SET_INPUT_PRIVACY_PWD, // RDA; Input privacy password(4-8)
    TXT_SET_INSERT_SIM, // RDA; No SIM card
    TXT_SET_INVALID_CUSTOM_WORD, // RDA; Invalid customized words
    TXT_SET_IP_NUMBER, // RDA; IP shortcut key
    TXT_SET_KEYPAD_AUTOLOCK, // RDA; Keypad auto-lock
    TXT_SET_KEY_BOARD_LIGHT, // RDA; Keypad backlight time
    TXT_SET_LANGUAGE, // RDA; Select language
    TXT_SET_LINE1_NUMBER, // RDA; Line 1 number
    TXT_SET_LINE2_NUMBER, // RDA; Line 2 number
    TXT_SET_LINE_SELECT, // RDA; Select line
    TXT_SET_LINE_SELECT_LINE1, // RDA; Line 1
    TXT_SET_LINE_SELECT_LINE2, // RDA; Line 2
    TXT_SET_LISTMENU, // RDA; List menu
    TXT_SET_MAINMENU, // RDA; Main menu
    TXT_SET_MAINMENU_ICON, // RDA; Matrix
    TXT_SET_MAINMENU_OP, // RDA; Page menu
    TXT_SET_MAINMENU_STYLE, // RDA; Main menu style
    TXT_SET_MAINMENU_TITLE_ICON, // RDA; Title icon menu
    TXT_SET_MAIN_LCD, // RDA; Main display
    TXT_SET_MANUAL_SEL, // RDA; Manual
    TXT_SET_MATCH_CRITERIA, // RDA; Match criteria
    TXT_SET_MATCH_CRITERIA_ENDS, // RDA; Ends with
    TXT_SET_MATCH_CRITERIA_EXACTLY, // RDA; Exactly the same as
    TXT_SET_MATCH_CRITERIA_INCLUDES, // RDA; Includes
    TXT_SET_MATCH_CRITERIA_STARTS, // RDA; Starts with
    TXT_SET_MEMORY_MANAGEMENT, // RDA; Memory management
    TXT_SET_MENU_ANIMATION, // RDA; Main menu animation
    TXT_SET_MINUTE_ALERT, // RDA; 50 secs warning
    TXT_SET_MINUTE_MINDER, // RDA; Minute minder
    TXT_SET_MINUTE_MINDER_HELP, // RDA; There will be a signal sound  every minute
    TXT_SET_MODIFY_CODE, // RDA; Change password
    TXT_SET_MORE_RING, // RDA; More ringtones
    TXT_SET_MPEG4, // RDA; Video player
    TXT_SET_MSG_ALERT_TYPE, // RDA; Message alert type
    TXT_SET_MT, // RDA; Mobile tracker
    TXT_SET_NETWORK, // RDA; Network settings
    TXT_SET_NETWORK_CONNECT, // RDA; Network connect
    TXT_SET_NETWORK_CONNECT_FAIL, // RDA; Network connect failed
    TXT_SET_NETWORK_METHOD, // RDA; Network search method
    TXT_SET_NETWORK_NAME, // RDA; Service provider
    TXT_SET_NETWORK_REGISTERING, // RDA; Network registering
    TXT_SET_NETWORK_SETTING, // RDA; Network settings
    TXT_SET_NETWORK_TYPE, // RDA; Network type
    TXT_SET_NEW_BLOCKED_ADDRESS, // RDA; New blocked address
    TXT_SET_NEXT_STEP, // RDA; Next
    TXT_SET_NO_ANSWER_TIME, // RDA; No reply time
    TXT_SET_PCS_BAND, // RDA; PCS band
    TXT_SET_PHONE, // RDA; Phone settings
    TXT_SET_PHONE_LOCK, // RDA; Phone lock
    TXT_SET_PIC_WALLPAPER, // RDA; Picture wallpaper
    TXT_SET_PIN1, // RDA; PIN
    TXT_SET_PIN2, // RDA; PIN2
    TXT_SET_PIN_LOCK, // RDA; PIN lock
    TXT_SET_POWER_MANAGEMENT, // RDA; Power management
    TXT_SET_POWER_OFF_ANIMATION, // RDA; Power off display
    TXT_SET_POWER_ON_ANIMATION, // RDA; Power on display
    TXT_SET_POWER_ON_OFF_ANIMATION, // RDA; On/off animation
    TXT_SET_POWER_ON_OFF_ANIM_DEFAULT, // RDA; Default
    TXT_SET_POWER_ON_OFF_ANIM_MORE, // RDA; More animation
    TXT_SET_POWER_SAVING, // RDA; Power saving mode
    TXT_SET_PREFIX_DIALLING, // RDA; Prefix dialling
    TXT_SET_PREFIX_DIALLING_HELP, // RDA; When making a call, you can add frequently used prefix number automatically into dialling screen.
    TXT_SET_PREFIX_DIALLING_NUMBER, // RDA; Prefix number
    TXT_SET_PRIVACY, // RDA; Privacy lock
    TXT_SET_PROFILES, // RDA; Profiles
    TXT_SET_PROFILES_ACTIVATED, // RDA; Activated:
    TXT_SET_PROFILES_EARPHONE_PLUG_IN, // RDA; Earphone plugged in
    TXT_SET_PROFILES_EARPHONE_PLUG_OUT, // RDA; Earphone plugged out
    TXT_SET_QUERY_STATUS, // RDA; Query status
    TXT_SET_RED_KEYLOCK, // RDA; Lock screen by end-key
    TXT_SET_REJECT_CALL_WITH_MESSAGE, // RDA; Reject call with message
    TXT_SET_REJECT_CALL_WITH_MESSAGE_HELP, // RDA; You can send explanatory text message to caller when you reject their calls.
    TXT_SET_REJECT_LIST, // RDA; Reject list
    TXT_SET_REJECT_LIST_FULL, // RDA; Reject list full
    TXT_SET_RESETING, // RDA; Resetting...
    TXT_SET_RESET_COMPLETED, // RDA; Reset completed. Phone will be restarted
    TXT_SET_RESET_FACTORY, // RDA; Restore factory settings
    TXT_SET_RESET_SETTINGS_QUERY, // RDA; Reset settings?
    TXT_SET_RING_BEEP, // RDA; Beep
    TXT_SET_RING_CALL_TONE, // RDA; Call ringtone
    TXT_SET_RING_CALL_TYPE, // RDA; Call alert
    TXT_SET_RING_FIXED, // RDA; Fixed ringtones
    TXT_SET_RING_FLIP_RING, // RDA; Flip tone
    TXT_SET_RING_INCREASING_RING, // RDA; Increasing melody
    TXT_SET_RING_KEYPAD_TONE, // RDA; Keypad tone
    TXT_SET_RING_KEY_TONE, // RDA; Key tone
    TXT_SET_RING_MELODY, // RDA; Melody
    TXT_SET_RING_MSG_TONE, // RDA; Message alert tone
    TXT_SET_RING_MSG_TYPE, // RDA; Message alert
    TXT_SET_RING_MUTE, // RDA; Mute
    TXT_SET_RING_RING, // RDA; Ringtone
    TXT_SET_RING_RING1, // RDA; Ringtone1
    TXT_SET_RING_RING2, // RDA; Ringtone2
    TXT_SET_RING_RING3, // RDA; Ringtone3
    TXT_SET_RING_RING4, // RDA; Ringtone4
    TXT_SET_RING_RING5, // RDA; Ringtone5
    TXT_SET_RING_RING6, // RDA; Ringtone6
    TXT_SET_RING_SELECT_CALL, // RDA; Caller ringtone
    TXT_SET_RING_SELECT_MSG, // RDA; Message ringtone
    TXT_SET_RING_SET, // RDA; Ring settings
    TXT_SET_RING_SILENT, // RDA; Silent
    TXT_SET_RING_TKEY_TONE, // RDA; Touch tone
    TXT_SET_RING_VIBRA, // RDA; Vibrate
    TXT_SET_RING_VIBRATION, // RDA; Vibration
    TXT_SET_RING_VIBRATION_AND_RING, // RDA; Vibration and melody
    TXT_SET_RING_VIBRATION_BEFORE_RING, // RDA; Vibration then melody
    TXT_SET_RING_VIBRA_AND_INCREASING_RING, // RDA; Vibration and increasing melody
    TXT_SET_RING_VIBRA_AND_RING, // RDA; Ring and vibrate
    TXT_SET_RING_VIBRA_BEFORE_RING, // RDA; Vibrate then ring
    TXT_SET_RING_VOL, // RDA; Volume
    TXT_SET_RING_VOLTAGE_WARNING, // RDA; Battery low alert
    TXT_SET_RING_VOLUME, // RDA; Adjust volume
    TXT_SET_RING_VOLUME_COMM, // RDA; Call volume
    TXT_SET_RING_VOLUME_POWER, // RDA; Power ringtone
    TXT_SET_SAMSUNG_THEME_1, // RDA; Blue
    TXT_SET_SAMSUNG_THEME_2, // RDA; Orange
    TXT_SET_SAMSUNG_THEME_3, // RDA; Green
    TXT_SET_SECURITY, // RDA; Security settings
    TXT_SET_SELECT_ON_POWER, // RDA; Select when power on
    TXT_SET_SETTING, // RDA; Settings
    TXT_SET_SETTING_ACTIVATED, // RDA; Setting activated
    TXT_SET_SETTING_CANCELED, // RDA; Setting cancelled
    TXT_SET_SETTING_CANCEL_FAIL, // RDA; Failed to cancel
    TXT_SET_SETTING_FAIL, // RDA; Settings failed
    TXT_SET_SET_PRIVACY_PROTECT, // RDA; Privacy
    TXT_SET_SHORTCUT, // RDA; Shortcut settings
    TXT_SET_SHORTCUT_CALCULATOR, // RDA; Calculator
    TXT_SET_SHORTCUT_CALENDAR, // RDA; Calendar
    TXT_SET_SHORTCUT_CONFLICT_W_SMART_IDLE, // RDA; Navigation shortcuts cannot be used while smart idle is active
    TXT_SET_SHORTCUT_CONTACTS, // RDA; Contacts
    TXT_SET_SHORTCUT_CONVERTOR, // RDA; Convertor
    TXT_SET_SHORTCUT_CREATE_MEMO, // RDA; Create memo
    TXT_SET_SHORTCUT_CREATE_MSG, // RDA; Create message
    TXT_SET_SHORTCUT_GAME, // RDA; Games
    TXT_SET_SHORTCUT_LOGS, // RDA; Logs
    TXT_SET_SHORTCUT_MESSAGES, // RDA; Messages
    TXT_SET_SHORTCUT_SOUNDPROFILES, // RDA; Sound profiles
    TXT_SET_SHORTCUT_TAST, // RDA; Task
    TXT_SET_SHORTCUT_VOICE_MAIL, // RDA; Voicemail
    TXT_SET_SHORTCUT_WALLPAPER, // RDA; Wallpaper
    TXT_SET_SHORTCUT_WORLD_CLOCK, // RDA; World clock
    TXT_SET_SHOW_MY_NUMBER, // RDA; Show my number
    TXT_SET_SIM_LOCK, // RDA; SIM lock
    TXT_SET_SMS_REPLY, // RDA; Reply SMS after reject
    TXT_SET_SMS_RING1, // RDA; Message tone 1
    TXT_SET_SMS_RING2, // RDA; Message tone 2
    TXT_SET_SMS_RING3, // RDA; Message tone 3
    TXT_SET_SMS_RING4, // RDA; Message tone 4
    TXT_SET_SMS_RING5, // RDA; Message tone 5
    TXT_SET_SMS_RING6, // RDA; Message tone 6
    TXT_SET_SPEED_DIAL_TIME_1S, // RDA; 1 second
    TXT_SET_SPEED_DIAL_TIME_5S, // RDA; 5 seconds
    TXT_SET_STANDBY_MODE_ALERT, // RDA; Can't close all sim cards
    TXT_SET_STATE, // RDA; Status
    TXT_SET_STATUS_TONE, // RDA; Call Status Tones
    TXT_SET_SUB_LCD, // RDA; Sub display
    TXT_SET_SUCCESS, // RDA; Set succeeded
    TXT_SET_THEME, // RDA; Theme
    TXT_SET_TIME, // RDA; Set time
    TXT_SET_TIMEDATE_TIME_HH_MM, // RDA; HH:MM
    TXT_SET_TIME_10S, // RDA; 10 seconds
    TXT_SET_TIME_10m, // RDA; 10 min
    TXT_SET_TIME_12_HOUR, // RDA; 12 hours
    TXT_SET_TIME_15S, // RDA; 15 seconds
    TXT_SET_TIME_1M, // RDA; 1 min
    TXT_SET_TIME_1m, // RDA; 1 min
    TXT_SET_TIME_20S, // RDA; 20 seconds
    TXT_SET_TIME_24_HOUR, // RDA; 24 hours
    TXT_SET_TIME_25S, // RDA; 25 seconds
    TXT_SET_TIME_30S, // RDA; 30 seconds
    TXT_SET_TIME_3m, // RDA; 3 min
    TXT_SET_TIME_5M, // RDA; 5 mins
    TXT_SET_TIME_5S, // RDA; 5 seconds
    TXT_SET_TIME_AND_DATE, // RDA; Time & date
    TXT_SET_TIME_AND_DATE_TITLE, // RDA; Time and date
    TXT_SET_TIME_AUTO, // RDA; Night mode
    TXT_SET_TIME_CLOSE, // RDA; Off
    TXT_SET_TIME_DATE, // RDA; Time & date
    TXT_SET_TIME_DISPLAY_TYPE, // RDA; Time format
    TXT_SET_TIME_ON, // RDA; ON
    TXT_SET_TIME_OPEN_FOREVER, // RDA; Always on
    TXT_SET_TODAY_SCHEDULE, // RDA; Today's schedule
    TXT_SET_VERTICAL_MOVE_STYLE, // RDA; Vertical slide
    TXT_SET_VOICE_CALL, // RDA; Voice Call
    TXT_SET_WAITING, // RDA; Setting, please wait
    TXT_SET_WALLPAPER, // RDA; Wallpaper settings
    TXT_SET_WALLPAPER_1, // RDA; Wallpaper 1
    TXT_SET_WALLPAPER_2, // RDA; Wallpaper 2
    TXT_SET_WALLPAPER_3, // RDA; Wallpaper 3
    TXT_SET_WALLPAPER_4, // RDA; Wallpaper 4
    TXT_SET_WALLPAPER_5, // RDA; Wallpaper 5
    TXT_SET_WALLPAPER_MORE, // RDA; More wallpapers
    TXT_SET_WINDOW_MOVE_STYLE, // RDA; Window slide style
    TXT_SET_WRITE_MESSAGE, // RDA; Write message
    TXT_SHANGHAI, // RDA; Shanghai
    TXT_SHANGHAI_021, // RDA; Shanghai 021
    TXT_SHORT_FRIDAY, // RDA; F
    TXT_SHORT_FRIDAY_FM, // RDA; F
    TXT_SHORT_MONDAY, // RDA; M
    TXT_SHORT_MONDAY_FM, // RDA; M
    TXT_SHORT_SATURDAY, // RDA; S
    TXT_SHORT_SATURDAY_FM, // RDA; S
    TXT_SHORT_SUNDAY, // RDA; S
    TXT_SHORT_SUNDAY_FM, // RDA; S
    TXT_SHORT_THURSDAY, // RDA; T
    TXT_SHORT_THURSDAY_FM, // RDA; T
    TXT_SHORT_TUESDAY, // RDA; T
    TXT_SHORT_TUESDAY_FM, // RDA; T
    TXT_SHORT_WEDNESDAY, // RDA; W
    TXT_SHORT_WEDNESDAY_FM, // RDA; W
    TXT_SIM_AM_PM, // RDA; AM/PM
    TXT_SIM_AUTO_SWITCH, // RDA; SIM auto switch
    TXT_SIM_CORRUPT, // RDA; Please call with another SIM
    TXT_SIM_DEACTIVATED, // RDA; Deactivated
    TXT_SIM_ISNOT_READY, // RDA; SIM not ready
    TXT_SIM_LOCK_PASSED, // RDA; SIM lock passed
    TXT_SIM_MANAGE_END_TIME, // RDA; End time
    TXT_SIM_MANAGE_ICON, // RDA; Icon
    TXT_SIM_MANAGE_START_TIME, // RDA; Start time
    TXT_SIM_NOT_INIT, // RDA; SIM not initialized
    TXT_SIM_NOT_SUPPORT, // RDA; SIM card not supported
    TXT_SIM_PLUG_IN, // RDA; Detecting SIM inserted
    TXT_SIM_PLUG_OUT, // RDA; Detecting SIM removed
    TXT_SIM_PLUG_OUT_SET_STANDBY_MODE_QUERY, // RDA; The standby SIM plug out, whether modify standby mode?
    TXT_SIM_PLUG_WAITING, // RDA; SIM initializing, please waiting
    TXT_SIM_REGISTER_CARD, // RDA; Register card
    TXT_SIM_REGISTRATION_FAILED, // RDA; SIM registration failed
    TXT_SIM_REJECTED, // RDA; SIM rejected
    TXT_SIM_UNAVAILABLE, // RDA; Unavailable SIM
    TXT_SIM_UPDATE_COMPLETE, // RDA; SIM update completed
    TXT_SIM_UPDATING, // RDA; SIM card updating
    TXT_SMS_ADD_CALENDERINFO, // RDA; Calendar
    TXT_SMS_ADD_CONTACT, // RDA; Contacts
    TXT_SMS_ADD_RECIPIENT, // RDA; Add
    TXT_SMS_ADD_SYMBOLS, // RDA; Add symbols
    TXT_SMS_ADD_TASKINFO, // RDA; Task
    TXT_SMS_ADD_TEXT, // RDA; Add text
    TXT_SMS_ADD_TO_CONTACTS, // RDA; Add to contacts
    TXT_SMS_ADD_TO_CONTACTS_EXT, // RDA; Add to Contacts
    TXT_SMS_ALERT_REPETITION, // RDA; Alert repetition
    TXT_SMS_ALPHABET_8BIT, // RDA; 8bit
    TXT_SMS_ALPHABET_GSM_ALPHABET, // RDA; GSM alphabet
    TXT_SMS_ALPHABET_MANAGEMENT, // RDA; Alphabet management
    TXT_SMS_ALPHABET_UNICODE, // RDA; Unicode
    TXT_SMS_ANSWER, // RDA; Reply
    TXT_SMS_ANSWER_SMS, // RDA; SMS
    TXT_SMS_AUTO_SAVE, // RDA; Save sent messages
    TXT_SMS_Abc_INPUT, // RDA; Abc
    TXT_SMS_BEYOND_LEN, // RDA; Exceed max length
    TXT_SMS_BLOCK, // RDA; Block by address
    TXT_SMS_BOX_EMPTY, // RDA; Empty box 
    TXT_SMS_BROADCAST, // RDA; Broadcast messsge
    TXT_SMS_BUSY, // RDA; System busy, please try later
    TXT_SMS_CANCEL_MARK, // RDA; Unmark
    TXT_SMS_CANCEL_MARKALL, // RDA; Unmark all
    TXT_SMS_CANCEL_SENDING_SUCCESS, // RDA; Send has cancelled
    TXT_SMS_CB, // RDA; Broadcast message
    TXT_SMS_CB_ALL_LANG, // RDA; ALL 
    TXT_SMS_CB_ARCHIVE, // RDA; Archive
    TXT_SMS_CB_ARCHIVE_TITLE, // RDA; Archive
    TXT_SMS_CB_CHANNEL, // RDA; channels     
    TXT_SMS_CB_CHANNEL_ACTIVATION, // RDA; Activation      
    TXT_SMS_CB_CHANNEL_ADD, // RDA; Add channel
    TXT_SMS_CB_CHANNEL_ADD_FAIL, // RDA; Add failed
    TXT_SMS_CB_CHANNEL_ADD_SUCC, // RDA; Add succeeded
    TXT_SMS_CB_CHANNEL_ALL, // RDA; All channels     
    TXT_SMS_CB_CHANNEL_CHECK, // RDA; Show channel number
    TXT_SMS_CB_CHANNEL_DELETE_SUCC, // RDA; Deleted
    TXT_SMS_CB_CHANNEL_DISABLE, // RDA; Channel off
    TXT_SMS_CB_CHANNEL_EDIT_FAIL, // RDA; Edit failed
    TXT_SMS_CB_CHANNEL_EDIT_OPT, // RDA; Edit channel
    TXT_SMS_CB_CHANNEL_EDIT_SUCC, // RDA; Edit succeeded
    TXT_SMS_CB_CHANNEL_ENABLE, // RDA; Channel on
    TXT_SMS_CB_CHANNEL_ENTER_CODE, // RDA; Enter code
    TXT_SMS_CB_CHANNEL_INDEX, // RDA; Code
    TXT_SMS_CB_CHANNEL_INDEX_EMPTY, // RDA; Please input channel index
    TXT_SMS_CB_CHANNEL_INDEX_REPEAT, // RDA; Channel index repeat
    TXT_SMS_CB_CHANNEL_INDEX_TOOBIG, // RDA; Channel index too big
    TXT_SMS_CB_CHANNEL_LIST, // RDA; Channel list
    TXT_SMS_CB_CHANNEL_LIST_EMPTY, // RDA; Empty channel list
    TXT_SMS_CB_CHANNEL_LIST_FULL, // RDA; Channel list is full
    TXT_SMS_CB_CHANNEL_LIST_OPT, // RDA; Channel list options
    TXT_SMS_CB_CHANNEL_NAME, // RDA; Title
    TXT_SMS_CB_DETAIL, // RDA; Details
    TXT_SMS_CB_DISABLE, // RDA; Off
    TXT_SMS_CB_EMPTY, // RDA; Empty
    TXT_SMS_CB_ENABLE, // RDA; On
    TXT_SMS_CB_LANG, // RDA; languages
    TXT_SMS_CB_LANGUAGE, // RDA; Language
    TXT_SMS_CB_LANG_ALL, // RDA; All
    TXT_SMS_CB_LANG_ARABIC, // RDA; Arabic
    TXT_SMS_CB_LANG_CHINESE, // RDA; Chinese
    TXT_SMS_CB_LANG_CZECH, // RDA; Czech
    TXT_SMS_CB_LANG_DANISH, // RDA; Danish
    TXT_SMS_CB_LANG_DUTCH, // RDA; Dutch
    TXT_SMS_CB_LANG_ENGLISH, // RDA; English
    TXT_SMS_CB_LANG_FINNISH, // RDA; Finnish
    TXT_SMS_CB_LANG_FRENCH, // RDA; French
    TXT_SMS_CB_LANG_GERMAN, // RDA; German
    TXT_SMS_CB_LANG_GREEK, // RDA; Greek
    TXT_SMS_CB_LANG_HEBREW, // RDA; Hebrew
    TXT_SMS_CB_LANG_HUNGARIAN, // RDA; Hungarian
    TXT_SMS_CB_LANG_ICELANDIC, // RDA; Icelandic
    TXT_SMS_CB_LANG_ITALIAN, // RDA; Italian
    TXT_SMS_CB_LANG_NORWEGIAN, // RDA; Norwegian
    TXT_SMS_CB_LANG_POLISH, // RDA; Polish
    TXT_SMS_CB_LANG_PORTUGUESE, // RDA; Portuguese
    TXT_SMS_CB_LANG_RUSSIAN, // RDA; Russian
    TXT_SMS_CB_LANG_SPANISH, // RDA; Spanish
    TXT_SMS_CB_LANG_SWEDISH, // RDA; Swedish
    TXT_SMS_CB_LANG_TURKISH, // RDA; Turkish
    TXT_SMS_CB_LANG_UNSPECIFIED, // RDA; Unspecified
    TXT_SMS_CB_LIST_EMPTY, // RDA; Empty
    TXT_SMS_CB_MESSAGE_WINDOW, // RDA; Broadcast messages
    TXT_SMS_CB_MSG_CUT_ADD, // RDA; Cut address
    TXT_SMS_CB_MSG_LIST, // RDA; Cell broadcast list
    TXT_SMS_CB_MY_CHANNELS, // RDA; My Channels
    TXT_SMS_CB_ONOFF, // RDA; On/Off
    TXT_SMS_CB_PRED_TEXT, // RDA; Predictive text
    TXT_SMS_CB_READ, // RDA; Read
    TXT_SMS_CB_READ_WINDOW, // RDA; Read   
    TXT_SMS_CB_RECEIVE_NEW, // RDA; New cell broadcast
    TXT_SMS_CB_RECEIVING_CHANNELS, // RDA; Receiving Channels      
    TXT_SMS_CB_SETTING, // RDA; Broadcast settings
    TXT_SMS_CB_SETTING_FAIL, // RDA; Settings failed
    TXT_SMS_CB_SETTING_SUCC, // RDA; Set succeeded
    TXT_SMS_CB_SETTING_WINDOW, // RDA; Settings
    TXT_SMS_CB_STARTUP, // RDA; Activation
    TXT_SMS_CB_STATUS_OFF, // RDA; Off
    TXT_SMS_CB_STATUS_ON, // RDA; On
    TXT_SMS_CB_TEMPORARY, // RDA; Temporary
    TXT_SMS_CB_TEMPORARY_TITLE, // RDA; Temporary
    TXT_SMS_CB_TURN_OFF_FAIL, // RDA; Failed
    TXT_SMS_CB_TURN_OFF_SUCC, // RDA; Done
    TXT_SMS_CB_TURN_ON_FAIL, // RDA; Failed
    TXT_SMS_CB_TURN_ON_SUCC, // RDA; Done
    TXT_SMS_CONCATENATED_SMS, // RDA; Long message
    TXT_SMS_COPY_CUT, // RDA; Copy or cut
    TXT_SMS_CREATE_MESSAGE, // RDA; Create message
    TXT_SMS_CUT_ADDRESS, // RDA; Cut address
    TXT_SMS_DATA_NUMBER, // RDA; Data number
    TXT_SMS_DEL, // RDA; Delete
    TXT_SMS_DELETE_MESSAGE, // RDA; Delete message?
    TXT_SMS_DELETE_PROTECT_QUERY, // RDA; Delete all including protected messages?
    TXT_SMS_DELETE_QUERY, // RDA; Protected message   Delete?
    TXT_SMS_DETAIL, // RDA; Details
    TXT_SMS_DEUTSCH_INPUT, // RDA; Deutsch
    TXT_SMS_DEl_MULTIPLE, // RDA; Delete multiple
    TXT_SMS_DEl_MULTIPLE_ALERT, // RDA; No items selected
    TXT_SMS_DIAL, // RDA; Dial
    TXT_SMS_DRAFTBOX, // RDA; Drafts
    TXT_SMS_ECTRACT_URL_BROWSE, // RDA; Browse
    TXT_SMS_EDIT, // RDA; Edit
    TXT_SMS_EMPTY, // RDA; Empty
    TXT_SMS_EMPTY_LIST, // RDA; Empty list
    TXT_SMS_EMPTY_WORD, // RDA; <Empty>
    TXT_SMS_ENGLISH_INPUT, // RDA; English
    TXT_SMS_EXCEED_MAX_NUM, // RDA; Exceeded the limit of 100
    TXT_SMS_EXIT, // RDA; Exit
    TXT_SMS_EXIT_ABC_INPUT, // RDA; ABC
    TXT_SMS_EXIT_COMPOSER, // RDA; Exit composer
    TXT_SMS_EXIT_NO_SAVING, // RDA; Exit without saving
    TXT_SMS_EXIT_OPT_TITLE, // RDA; Exit composer
    TXT_SMS_EXTRACT_EMAIL, // RDA; Email
    TXT_SMS_EXTRACT_NUMBER, // RDA; Number
    TXT_SMS_EXTRACT_OFF, // RDA; OFF
    TXT_SMS_EXTRACT_ON, // RDA; On
    TXT_SMS_EXTRACT_URL_ADD_TO_BOOKMARK, // RDA; Add to bookmark
    TXT_SMS_EXTRACT_URL_ADRESS, // RDA; URL
    TXT_SMS_FAX_NUMBER, // RDA; Fax number
    TXT_SMS_FONT_BIG, // RDA; Big
    TXT_SMS_FONT_NORMAL, // RDA; Normal
    TXT_SMS_FONT_SMALL, // RDA; Small
    TXT_SMS_FORWARD, // RDA; Forward
    TXT_SMS_FRANCAIS_INPUT, // RDA; Francais
    TXT_SMS_FROM, // RDA; From:
    TXT_SMS_INBOX, // RDA; Inbox
    TXT_SMS_INPUT_METHOD, // RDA; Input method
    TXT_SMS_INVALID_NUMBER, // RDA; Invalid recipient number
    TXT_SMS_ITALIANO_INPUT, // RDA; Italiano
    TXT_SMS_LINE1_NUMBER, // RDA; Line 1 number
    TXT_SMS_LINE2_NUMBER, // RDA; Line 2 number
    TXT_SMS_LIST_BY_ITEM, // RDA; List by
    TXT_SMS_LOCK, // RDA; Lock
    TXT_SMS_LOST_PROMPT, // RDA; (......)
    TXT_SMS_MARK, // RDA; Mark
    TXT_SMS_MARKALL, // RDA; Mark all
    TXT_SMS_MEMORY, // RDA; Memory status
    TXT_SMS_MEMORY_FULL, // RDA; Memory full
    TXT_SMS_MEMORY_PHONE, // RDA; Phone
    TXT_SMS_MEMORY_SIM, // RDA; SIM
    TXT_SMS_MEM_FULL_ALERT_USER, // RDA; Phone SMS memory full
    TXT_SMS_MESSAGESPACE, // RDA; Message capacity
    TXT_SMS_MESSAGE_ITEM, // RDA; Message
    TXT_SMS_MESSAGE_ITEMS, // RDA; Messages
    TXT_SMS_MESSAGE_SETTINGS, // RDA; Settings
    TXT_SMS_MESSAGE_TOO_LONG, // RDA; Message too long
    TXT_SMS_ME_MEM_FULL_ALERT_USER, // RDA; Phone SMS memory full
    TXT_SMS_MOVED, // RDA; Moved
    TXT_SMS_MOVE_TO, // RDA; Move to
    TXT_SMS_MOVE_TO_MYFOLDER, // RDA; Move to my folders
    TXT_SMS_MOVE_TO_PHONE, // RDA; Phone
    TXT_SMS_MOVE_TO_SIM, // RDA; Sim
    TXT_SMS_MULTIPLE, // RDA; Multiple
    TXT_SMS_MY_FOLDERS, // RDA; My folders
    TXT_SMS_NAME_SORT, // RDA; Contacts
    TXT_SMS_NEDERLANDS_INPUT, // RDA; Nederlands
    TXT_SMS_NEW_MSG, // RDA;  new message
    TXT_SMS_NEW_MSGS, // RDA;  new messages
    TXT_SMS_NEW_SMS, // RDA; New SMS
    TXT_SMS_NOT_SAVE, // RDA; Don't save
    TXT_SMS_NO_MESSAGES, // RDA; No messages
    TXT_SMS_NO_SC, // RDA; No service center
    TXT_SMS_NO_SIM_ALERT, // RDA; Insert SIM card
    TXT_SMS_NUMBER_IS_IN_PB, // RDA; Already in use.View contact details?
    TXT_SMS_NUMERIC_INPUT, // RDA; Numeric
    TXT_SMS_NUM_DEL, // RDA; Delete
    TXT_SMS_NUM_EDIT, // RDA; Edit
    TXT_SMS_OPTION, // RDA; Options
    TXT_SMS_OUTBOX, // RDA; Outbox
    TXT_SMS_PASTE, // RDA; Paste
    TXT_SMS_PIECE_SAVED_IN_NV, // RDA; Phone
    TXT_SMS_PIECE_SAVED_IN_SIM, // RDA; SIM
    TXT_SMS_POLSKI_INPUT, // RDA; Polski
    TXT_SMS_PREDICTIVE_OFF, // RDA; Predictive text off
    TXT_SMS_PREDICTIVE_ON, // RDA; Predictive text on
    TXT_SMS_PREFERRED_CONNECTION, // RDA; Preferred connection
    TXT_SMS_PREFERRED_GPRS, // RDA; Prefer GPRS
    TXT_SMS_PREFERRED_GPRS_ONLY, // RDA; GPRS only
    TXT_SMS_PREFERRED_GSM, // RDA; Prefer GSM
    TXT_SMS_PREFERRED_GSM_ONLY, // RDA; GSM only
    TXT_SMS_PREVIEW_EXTRACT_OPEN_OR_CLOSE, // RDA; Extract subject
    TXT_SMS_PROMPT, // RDA; Prompt user
    TXT_SMS_PROMPT_DEL, // RDA; Delete
    TXT_SMS_PROMPT_FAIL, // RDA; Failed
    TXT_SMS_PROMPT_NO_SPACE, // RDA; Not enough space
    TXT_SMS_PROMPT_SAVING, // RDA; Saving
    TXT_SMS_PROMPT_SENDING, // RDA; Sending message
    TXT_SMS_PROMPT_SUCC, // RDA; Succeeded
    TXT_SMS_PROTECTION_OFF, // RDA; Protection off
    TXT_SMS_PROTECTION_ON, // RDA; Protection on
    TXT_SMS_QUERY_CONTINUE, // RDA; Not enough space, continue?
    TXT_SMS_QUERY_RESEND, // RDA;  item(s) failed,
    TXT_SMS_QUERY_RESEND2, // RDA; Resend?
    TXT_SMS_READ_ERR, // RDA; Message deleted
    TXT_SMS_READ_SMS, // RDA; Read message
    TXT_SMS_READ_SORT, // RDA; Read
    TXT_SMS_RECEIVER, // RDA; Recipients
    TXT_SMS_RECEIVE_VCARD_MSG, // RDA; Received vCard
    TXT_SMS_RECIPIENT_SORT, // RDA; Recipient
    TXT_SMS_RECIP_EXCEED_ALERT, // RDA; Too many recipients
    TXT_SMS_REGISTERED, // RDA; Registered
    TXT_SMS_REGISTERED_NOT_ALLOWED, // RDA; Not allowed
    TXT_SMS_REPLY_PATH, // RDA; Reply path
    TXT_SMS_RESEND, // RDA; Resend
    TXT_SMS_SAMENUM_DEL, // RDA; Delete repeated numbers
    TXT_SMS_SAVE, // RDA; Save
    TXT_SMS_SAVED, // RDA; Saved
    TXT_SMS_SAVE_AS_DRAFT, // RDA; Save as draft
    TXT_SMS_SAVE_AS_TEMPLATE, // RDA; Save as template
    TXT_SMS_SAVE_IN_DRAFT, // RDA; Drafts
    TXT_SMS_SAVE_IN_DRAFTS, // RDA; Save in Drafts
    TXT_SMS_SAVE_IN_FOLDER, // RDA; My folders
    TXT_SMS_SAVE_IN_OPT, // RDA; Save in
    TXT_SMS_SAVE_SEND, // RDA; Send and save
    TXT_SMS_SAVE_TO, // RDA; Save to
    TXT_SMS_SAVE_TO_ANNIVERSARY, // RDA; Anniversary
    TXT_SMS_SAVE_TO_APPOINTMENT, // RDA; Appointment
    TXT_SMS_SEARCH_OPERATE, // RDA; Operate
    TXT_SMS_SEND, // RDA; Send
    TXT_SMS_SENDED, // RDA; Message sent
    TXT_SMS_SENDER_SORT, // RDA; Sender
    TXT_SMS_SENDOPTION, // RDA; Send options
    TXT_SMS_SEND_ONLY, // RDA; Send only
    TXT_SMS_SEND_QUERY_HEADR, // RDA; This message will be sent as
    TXT_SMS_SEND_QUERY_TAIL, // RDA; separate messages
    TXT_SMS_SENTBOX, // RDA; Sentbox
    TXT_SMS_SENT_AGAIN, // RDA; Send again
    TXT_SMS_SETTING, // RDA; Message settings
    TXT_SMS_SETTING_ALERT, // RDA; Alert repetition
    TXT_SMS_SETTING_ALERT_ONCE, // RDA; Once
    TXT_SMS_SETTING_ALERT_TIME1, // RDA; Every 2 minutes
    TXT_SMS_SETTING_ALERT_TIME2, // RDA; Every 5 minutes
    TXT_SMS_SETTING_ALERT_TIME3, // RDA; Every 10 minutes
    TXT_SMS_SETTING_BM, // RDA; Broadcast messages
    TXT_SMS_SETTING_BN, // RDA; Block number
    TXT_SMS_SETTING_BOLOCK, // RDA; Block messages
    TXT_SMS_SETTING_CS, // RDA; Character support
    TXT_SMS_SETTING_DELETE, // RDA; Delete by folder
    TXT_SMS_SETTING_DELIVERY_REPORT, // RDA; Request delivery report
    TXT_SMS_SETTING_GS, // RDA; General settings
    TXT_SMS_SETTING_KEEP_COPY, // RDA; Keep a copy
    TXT_SMS_SETTING_MS, // RDA; Memory status
    TXT_SMS_SETTING_MT, // RDA; Message type
    TXT_SMS_SETTING_OFF, // RDA; Off
    TXT_SMS_SETTING_ON, // RDA; On
    TXT_SMS_SETTING_RP, // RDA; Reply path
    TXT_SMS_SETTING_SAMSUNG_SC, // RDA; SIM options
    TXT_SMS_SETTING_SAVE_SEND, // RDA; Save sent message
    TXT_SMS_SETTING_SC, // RDA; Message center
    TXT_SMS_SETTING_SC_TITLE, // RDA; Message center
    TXT_SMS_SETTING_SERVICE_CENTRE, // RDA; Service centre
    TXT_SMS_SETTING_SMS, // RDA; SMS settings
    TXT_SMS_SETTING_SO, // RDA; Sending options
    TXT_SMS_SETTING_SOS, // RDA; SOS messages
    TXT_SMS_SETTING_STATUS, // RDA; Status settings
    TXT_SMS_SETTING_TS, // RDA; Text messages
    TXT_SMS_SETTING_TS_MS, // RDA; Text message
    TXT_SMS_SETTING_VMN, // RDA; Voicemail number
    TXT_SMS_SETTING_VP, // RDA; Message validity period
    TXT_SMS_SETTING_VP_TITLE, // RDA; Message validity period
    TXT_SMS_SETTING_V_P, // RDA; Validity period
    TXT_SMS_SET_FONT, // RDA; Font size
    TXT_SMS_SIM_MEM_FULL_ALERT_USER, // RDA; SIM SMS memory full
    TXT_SMS_SIM_SET_OPT, // RDA; SIM options
    TXT_SMS_SIM_STORAGE_SMS, // RDA; SIM storage SMS
    TXT_SMS_SIM_VOICEMAIL_EMPTY, // RDA; Voicemail number unset
    TXT_SMS_SMS_SPACE, // RDA; SMS memory:
    TXT_SMS_SORT, // RDA; Sort
    TXT_SMS_SPACE, // RDA; Size
    TXT_SMS_SPAMBOX, // RDA; Spambox
    TXT_SMS_SR_DELAY, // RDA; Send later
    TXT_SMS_SR_DISABLE, // RDA; Off
    TXT_SMS_SR_ENABLE, // RDA; On
    TXT_SMS_SR_FAIL, // RDA; Send failed
    TXT_SMS_SR_INVAILD, // RDA; Invaild report
    TXT_SMS_SR_NO_CONF, // RDA; Can't confirm
    TXT_SMS_SR_REPLACE, // RDA; Replaced
    TXT_SMS_SR_SUCC, // RDA; Delivered
    TXT_SMS_STATUSREPORT, // RDA; Delivery report
    TXT_SMS_STORAGE_PHONE_SORT, // RDA; Phone message
    TXT_SMS_STORAGE_SIM_SORT, // RDA; SIM message
    TXT_SMS_SUBJECT_SORT, // RDA; Subject
    TXT_SMS_TEMPLATES, // RDA; Templates
    TXT_SMS_TIME_SORT, // RDA; Time
    TXT_SMS_TO, // RDA; To:
    TXT_SMS_TYPE_SORT, // RDA; Type
    TXT_SMS_UNKNOW, // RDA; (Unknown)
    TXT_SMS_UNLOADED_SMS, // RDA; SMS unloaded
    TXT_SMS_UNLOCK, // RDA; Unlock
    TXT_SMS_UNREAD_SORT, // RDA; Unread
    TXT_SMS_UPDATED, // RDA; Message updated
    TXT_SMS_USE_NUM, // RDA; Extract number
    TXT_SMS_VM_CALLVM, // RDA; Retrieve message
    TXT_SMS_VM_CONNECT, // RDA; Connect to voice server
    TXT_SMS_VM_NUM, // RDA; Voice server number
    TXT_SMS_VM_SETVM, // RDA; Number settings
    TXT_SMS_VOICEMAIL, // RDA; Voicemail server
    TXT_SMS_VOICEMAIL_EMPTY, // RDA; Voicemail empty
    TXT_SMS_VP_1HOUR, // RDA; 1 hour
    TXT_SMS_VP_1WEEK, // RDA; 1 week
    TXT_SMS_VP_24HOUR, // RDA; 24 hours
    TXT_SMS_VP_6HOUR, // RDA; 6 hours
    TXT_SMS_VP_72HOUR, // RDA; 72 hours
    TXT_SMS_VP_MAX, // RDA; Maximum
    TXT_SMS_V_P_1HOUR, // RDA; 1 hour
    TXT_SMS_V_P_1WEEK, // RDA; 1 week
    TXT_SMS_V_P_24HOUR, // RDA; 24 hours
    TXT_SMS_V_P_6HOUR, // RDA; 6 hours
    TXT_SMS_V_P_MAX, // RDA; Maximum
    TXT_SMS_WRITE, // RDA; Write message
    TXT_SMS_WRITE_MESSAGE, // RDA; Write message
    TXT_SMS_WRITE_OPT, // RDA; Options
    TXT_SMS_WRITING_LANGUAGE, // RDA; Writing language
    TXT_SMS_WRITING_OPT, // RDA; Writing options
    TXT_SMS_abc_INPUT, // RDA; abc
    TXT_SOS, // RDA; SOS
    TXT_SOS_ONLY, // RDA; Emergency call only
    TXT_SPEEDDIAL_VIEW_NAME, // RDA; Name:
    TXT_SPEEDDIAL_VIEW_PHONENUM, // RDA; Number:
    TXT_SS, // RDA; Supplementary service
    TXT_STARTUP, // RDA; Power on
    TXT_STARTUP_OR_NOT, // RDA; Do you want to power on?
    TXT_STK_CALL_MODIFIED, // RDA; Call modified
    TXT_STK_HELP_MENU, // RDA; Help
    TXT_STK_NO, // RDA; No
    TXT_STK_OPEN_CHANNEL_QUERY, // RDA; Whether originating connection
    TXT_STK_OPEN_CHANNEL_RECEIVING, // RDA; Data receiving
    TXT_STK_OPEN_CHANNEL_SENDING, // RDA; Data sending
    TXT_STK_OVERTIME_RETURN, // RDA; Overtime return
    TXT_STK_SENDING, // RDA; Sending
    TXT_STK_SEND_FAIL, // RDA; Send failed
    TXT_STK_SETUP_CALL, // RDA; Setup call
    TXT_STK_TRY_AGAIN, // RDA; Try again
    TXT_STK_YES, // RDA; Yes
    TXT_STOPWATCH, // RDA; Stopwatch
    TXT_SUCCESS, // RDA; Succeeded
    TXT_SUNDAY, // RDA; Sunday
    TXT_SYS_WAITING_AND_TRY_LATER, // RDA; Please try again
    TXT_TAIWAN, // RDA; Taiwan
    TXT_TAIWAN_00886, // RDA; Taiwan 00886
    TXT_THURSDAY, // RDA; Thursday
    TXT_TIME, // RDA; Time
    TXT_TIME_MINUTE, // RDA; Minute
    TXT_TOOLS_ALARM, // RDA; Alarm
    TXT_TOOLS_CALCULATOR, // RDA; Calculator
    TXT_TOOLS_CALENDAR, // RDA; Calendar
    TXT_TOOLS_COUNTEDTIME, // RDA; Timer
    TXT_TOOLS_COUNTEDTIME_ARRIVE, // RDA; Timer arrives
    TXT_TOOLS_LOCATION, // RDA; Location
    TXT_TOOLS_OTHERS, // RDA; Others
    TXT_TOOLS_WORLDTIME, // RDA; World clock
    TXT_TUESDAY, // RDA; Tuesday
    TXT_TURNOFF_HANDFREE, // RDA; Turn off hands-free
    TXT_TURNOFF_MUTE, // RDA; Cancel mute
    TXT_TURNOFF_SPEAKER, // RDA; Close speaker
    TXT_TURNON_HANDFREE, // RDA; Hands-free
    TXT_TURNON_MUTE, // RDA; Mute
    TXT_TURNON_SPEAKER, // RDA; Turn on speaker
    TXT_TURN_ON_OR_NOT, // RDA; Bluetooth is not activated. Activate Bluetooth?
    TXT_UDISK_UCOM_BUSY, // RDA; COM port is busy
    TXT_UDISK_USB_LOG, // RDA; USB log
    TXT_UDISK_VUART, // RDA; PC connecting
    TXT_UDISK_WAIT_START_UDISK, // RDA; Starting U Disk
    TXT_UDISK_WAIT_STOP_UDISK, // RDA; Stopping U Disk
    TXT_UNABLE_TO_ADD, // RDA; Unable to add
    TXT_UNABLE_TO_SAVE, // RDA; Unable to save
    TXT_UNKNOWN, // RDA; Unknown
    TXT_UPDATETOPB_EXIST, // RDA; Update
    TXT_USB_PLUGOUT, // RDA; USB cable is removed
    TXT_USSD_BUSY, // RDA; USSD busy
    TXT_USSD_NOTIFICATION, // RDA; Notification
    TXT_USSD_REPLY, // RDA; Reply
    TXT_USSD_REQUEST, // RDA; Requesting
    TXT_USSD_REQUEST_SERVICE, // RDA; Requesting service...
    TXT_USSD_REQUEST_UNCOMPLETED, // RDA; Request failed
    TXT_VC_DELETESUCC, // RDA; Deleted
    TXT_VIEW, // RDA; View
    TXT_VOICEMAIL, // RDA; Voicemail
    TXT_VP_LOADING, // RDA; Loading...
    TXT_WAITING_TIME, // RDA; Waiting time
    TXT_WEDNESDAY, // RDA; Wednesday
    TXT_WITHOUT_RESULT, // RDA; No result
    TXT_WRONG_PASSWORD, // RDA; Wrong password
    XT_PB_TO_SAVE_CARD_T_UDISK, // RDA; Copy to U Disk
    TXT_ALARMS_POWER_ON,
    STR_DIAL_EMERGENCY_NUMBER,
    STR_ID_AUDPLY_PLAYTITLE_NONE,
    STR_ID_TITLE_RTC_TIME,
    STR_INITIALIZE_MEMO_TEST,
    STR_GET_STATUS_MEMO_TEST,
    STR_MAKE_MAXFILE_MEMO_TEST,
    STR_MAKE_BC_TEST_FILES_MEMO_TEST,
    STR_TITLE_MEMO_TEST,
    STR_NETWORK_LOCK,
    STR_SUBSET_LOCK,
    STR_SP_LOCK,
    STR_CP_LOCK,
    STR_SIM_LOCK,
    STR_ACTIVE_LOCK,
    STR_AUTO_NETWORK_LOCK,
    STR_AUTO_SUBSET_LOCK,
    STR_AUTO_SP_LOCK,
    STR_AUTO_CP_LOCK,
    STR_AUTO_SIM_LOCK,
    STR_INTERROGATE,
    STR_SETTINGS_RINGTONE_1,
    STR_ID_LAUNCHER_MENU,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_INPUTBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_SPINBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_CHECKBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_TEXTBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_SCROLLBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_IMAGEBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_LISTUSERBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_FOCUS_INPUT_POPUP,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_MENUBOX,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_FOCUS_TAP,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_MULT_FOCUS,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_VAR_SAMPLE,
    SS_HIDDEN_KEY_FOCUS_SAMPLE_DROP_MENU_FOCUS,
    SS_HIDDEN_KEY_MENU_SAMPLE_NORMAL_MENU_1_LINE,
    SS_HIDDEN_KEY_MENU_SAMPLE_NORMAL_MENU_2_LINE,
    SS_HIDDEN_KEY_MENU_SAMPLE_MATRIX_MENU,
    SS_HIDDEN_KEY_MENU_SAMPLE_FREE,
    SS_HIDDEN_KEY_MENU_SAMPLE_POPUP_MENU,
    SS_HIDDEN_KEY_MENU_SAMPLE_VAR_POPUP_MENU,
    SS_HIDDEN_KEY_MENU_SAMPLE_RICHPANEL_TEST,
    SS_HIDDEN_KEY_MENU_SAMPLE_GLOBAL_UX_TEST,
    SS_HIDDEN_KEY_TEST_MODE_PRODUCTION,
    SS_HIDDEN_KEY_TEST_MODE_TEXT_MSG,
    SS_HIDDEN_KEY_TEST_MODE_SAT_TEST,
    SS_HIDDEN_KEY_TEST_MODE_VERSION,
    SS_HIDDEN_KEY_TEST_MODE_HW_VERSION,
    SS_HIDDEN_KEY_TEST_MODE_LIB_VERSION,
    SS_HIDDEN_KEY_TEST_MODE_TFS_VERSION,
    SS_HIDDEN_KEY_TEST_MODE_VIB_TEST,
    SS_HIDDEN_KEY_TEST_MODE_AUDIO_SETTINGS,
    SS_HIDDEN_KEY_TEST_MODE_BATT_INFO,
    SS_HIDDEN_KEY_LOOK_TEST,
    SS_HIDDEN_KEY_LOOK_TEST_BITMAP_COMP,
    SS_HIDDEN_KEY_LOOK_TEST_CHK_SMO,
    SS_HIDDEN_KEY_LOOK_TEST_EXT,
    SS_HIDDEN_KEY_LOOK_TEST_FOCUS,
    SS_HIDDEN_KEY_LOOK_TEST_FONT_TEST,
    SS_HIDDEN_KEY_LOOK_TEST_LCD_DUMP,
    SS_HIDDEN_KEY_LOOK_TEST_MAIN_INDICATOR,
    SS_HIDDEN_KEY_LOOK_TEST_MEM_POOL,
    SS_HIDDEN_KEY_LOOK_TEST_MENU,
    SS_HIDDEN_KEY_LOOK_TEST_POPUP_SAMPLES,
    SS_HIDDEN_KEY_LOOK_TEST_RESET_PHN_PSWD,
    SS_HIDDEN_KEY_LOOK_TEST_TITLE,
    SS_HIDDEN_KEY_LOOK_TEST_VECTOR_CLIP_TEST,
    SS_HIDDEN_KEY_LOOK_TEST_XML_LOADING,
    SS_HIDDEN_KEY_LOOK_TEST_LOW_BATT_SETTING,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CFU,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CFB,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CFNRY,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CFNRC,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CT,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_BOAC,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_BOIC,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_BOIC_EXHC,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_BAIC,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_BIC_ROAM,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_MPTY,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CUG,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_AOC,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_PREF_GUG,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CUG_OA,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_HOLD,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CW,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CCBS,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_SM_MT,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_SM_MO,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_SM_CB,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_REPLY_PATH,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_DEL_CONF,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_PROTOCOL_ID,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_VALIDITY_PERIOD,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_ALS,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CLIP,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_COLR,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_COLP,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_MCI,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CLI_SEND,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_CLI_BLOCK,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_PLMN_MODE,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_VPS,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_SM_MO_PAGING,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_SM_MO_EMAIL,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_FAX,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_DATA,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_LANGUAGE,
    SS_HIDDEN_KEY_SIM_INFO_CPHS_INFO_NUMBER,
    SS_HIDDE_KEY_SIM_INFO_SST,
    SS_HIDDE_KEY_SIM_INFO_CPHS,
    SS_HIDDE_KEY_SIM_INFO_CONTACTS,
    SS_HIDDE_KEY_SIM_INFO_EXT,
    SS_HIDDE_KEY_SIM_INFO_TEXT_MESSAGE,
    SS_HIDDE_KEY_SIM_INFO_LANGUAGE,
    SS_HIDDE_KEY_SIM_INFO_IMSI,
    STRING_MTPNP_CARD1_CLOSED,
    STRING_MTPNP_CARD2_CLOSED,
    STRING_MTPNP_CARD3_CLOSED,
    STRING_MTPNP_CARD4_CLOSED,
    STRING_MTPNP_CARD1ABSENT,
    STRING_MTPNP_CARD2ABSENT,
    SS_SOS_ONLY,
    SS_CB_IDLE,
    STR_ID_FM_ADC,
    STR_ID_FM_KEYPAD,
    STR_FMGR_REM_DEVICE,
    STR_ID_CALCULATOR_LST,
    STR_MID_DEFAULT_ALARM,
    STR_PROFILES_RING_1,
    STR_ID_PLX_BROW_GUICB_ADDPHONEBOOK_ERR_NAMETOOLONG,
    STR_ID_ADDPHONEBOOK_ERR_NUMBERTOOLONG,
    SS_TO_PHONE2,
    SS_TO_SIM2,
    STR_ID_PHB_EXTRA_NUMBER,
    STR_MEMORY_STATUS_PHONE,
    STR_MEMORY_STATUS_SIM,
    STR_MEMORY_FULL,
    STR_SMS_MEMORY_FULL,
    STR_PHB_MEMORY_FULL,
    STR_CALLLOG_MEMORY_FULL,
    STR_MEMO_MEMORY_FULL,
    STR_CALENDAR_MEMORY_FULL,
    SS_CHECK_HIDE_ID_SETTING,
    STR_CALL_POP_SSCSTRING_FACTORY_RESET_TITLE,
    STR_CALL_POP_SSCSTRING_FACTORY_RESET,
    STR_ID_SHOW_IMEI,
    STR_ID_SHOW_ENGINEER_MODE,
    STR_ID_SHOW_FACTORY_MODE,
    STR_ID_SHOW_VERSION_DISP,
    STR_ID_MEM_FULL_TEST,
    STR_ID_MEM_FACTORY_RESET,
    STR_ID_AUTO_RESET_MODE,
    STR_ID_ENCRYPT_SSCSTRING,
    STR_ID_FACTORY_MEMORY_CHECK_START,
    STR_ID_FACTORY_MEMORY_CHECK_SHOW,
    STR_ID_FACTORY_CLUSTER_CHECK_SHOW,
    STR_ID_FACTORY_RESTORE_USERDATA,
    STR_ID_FACTORY_AUTO_TEST_KEYPRESS_RECORD_START,
    STR_ID_FACTORY_AUTO_TEST_START,
    STR_ID_FACTORY_FTA_SW_VERSION,
    STR_ID_FACTORY_FTA_HW_VERSION,
    STR_ID_FACTORY_SW_HW_VERSION,
    STR_ID_RTC_TIME,
    STR_ID_MEMO_TEST,
    STR_ID_BACKLIGHT_TIME_MORMAL,
    STR_ID_BACKLIGHT_TIME_ALWAYSON,
    STR_ID_LCD_CONTRAST,
    STR_ID_AUTOANSWER,
    STR_ID_SELF_CHECKUP,
    STR_ID_RESET_PHONE_PWD_FLAG,
    STR_ID_VIB_TEST,
    STR_ID_BATTERY_INFO,
    STR_ID_JIG_TEST,
    STR_ID_ERROR_STACK,
    STR_HW_SW_SLAVE,
    STR_ID_INTERROGATE,
    STR_ID_BAND_ST,
    STR_ID_LOOK_TEST,
    STR_ID_LOOPBACK_TEST,
    STR_ID_USB_TRACE,
    STR_ID_SET_RAMDUMP,
    STR_ID_IMSI_ON_OFF,
    STR_ID_BAND_SELECTION,
    STR_ID_BAND_SELECTION_850,
    STR_ID_BAND_SELECTION_GSM,
    STR_ID_BAND_SELECTION_EGSM,
    STR_ID_BAND_SELECTION_DCS,
    STR_ID_BAND_SELECTION_PCS,
    STR_ID_CSC_SETTINE,
    STR_ID_CSC_TEST,
    STR_ID_CSC_DEVELOPER_MENU,
    SS_DATA_SCREEN,
    SS_AUTO_FFS_TEST,
    STR_ID_UNIQUE_NUM,
    STR_ID_SERIAL_NO,
    STR_ID_SIM_INFO,
    STR_ID_SPACE_INFO,
    STR_ID_BLUE_SCREEN,
    STR_ID_INTERNAL_MENU,
    STR_ID_DEBUG_SCREEN,
    STR_ID_BAUD_RATE,
    STR_ID_BAUD_RATE_57600,
    STR_ID_BAUD_RATE_115200,
    STR_ID_BAUD_RATE_230400,
    STR_ID_BAUD_RATE_460800,
    STR_ID_BAUD_RATE_921600,
    STR_ID_15_FACTORY_MODE,
    STR_ID_TRACE_LEVEL,
    STR_ID_SW_CFG,
    STR_ID_SW_CFG_TEST,
    STR_ID_SW_CFG_DEV,
    STR_ID_FAIL_HISTORY,
    STR_SETTING_RESTORE_PROCESSING,
    STR_SCR8069_CAPTION1,
    STR_ID_PHB_OPTION_IP_DIAL,
    STR_ID_IP_DIAL_IP_NUM,
    STR_SMS_NEEDREPLY1,
    STR_ID_MMS_MSG_RESTRICTED,
    STR_SMS_FAILURE_MSG_LEN_EXCEEDED,
    STR_MESSAGE_INCOMPLETE,
    SERVICES_SS_NAME_MSENDER,
    STR_FMGR_FOLDER_CREATE,
    STR_FMGR_BT,
    FMGR_FS_DISK_FULL_TEXT,
    STR_FMGR_WALLPAPER,
    SS_COPYING,
    SS_FORMAT_MNOUNING,
    SS_MEMORY_STATUS_PHONE,
    STR_ID_FMGR_DRV_TOTAL_SPACE,
    STR_ID_FMGR_DRV_FREE_SPACE,
    STR_ID_FMGR_FILE_SIZE,
    STR_NFY_CALL_FWDNG_CONDITIONAL,
    STR_NFY_CALL_FWDNG_UNCONDITIONAL,
    SS_CALL_FORWARDED_UNCONDITIONAL,
    SS_CALL_FORWARDED_CONDITIONAL,
    STR_NFY_CALL_BUSY_FORWARDED,
    STR_NFY_CALL_ALL_OUTGOING_BARRED,
    STR_NFY_CALL_ALL_INCOMING_BARRED,
    STR_NFY_CALL_CUG,
    STR_NFY_CALL_CTFAC,
    STR_GLOBAL_UNFINISHED,
    SS_SOSREGISTERED,
    SS_MEMORY_STATUS_SIM,
    STR_SMS_NUM_CANNOT_BE_EMPTY,
    MAX_NL10N_STRID,
};

#endif /* _NL10N_STRING_IDS_H_ */

