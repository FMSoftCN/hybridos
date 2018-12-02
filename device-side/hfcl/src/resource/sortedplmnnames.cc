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

#include <stdlib.h>
#include <string.h>

#include "ngux.h"
#include "ngux_string_ids.h"

NAMESPACE_BEGIN

typedef struct _SORTED_PLMN_NAME {
    const char* plmn;
    const char* name;
    int l10n_id;
} SORTED_PLMN_NAME;

static SORTED_PLMN_NAME _sorted_plmn_names [] = {
    { "20201", "C-OTE", 
#ifdef STRID_COMMON_PLMN_20201
        STRID_COMMON_PLMN_20201, 
#else
        0,
#endif
    },
    { "20205", "voda GR", 
#ifdef STRID_COMMON_PLMN_20205
        STRID_COMMON_PLMN_20205, 
#else
        0,
#endif
    },
    { "20209", "WIND", 
#ifdef STRID_COMMON_PLMN_20209
        STRID_COMMON_PLMN_20209, 
#else
        0,
#endif
    },
    { "20210", "WIND", 
#ifdef STRID_COMMON_PLMN_20210
        STRID_COMMON_PLMN_20210, 
#else
        0,
#endif
    },
    { "20402", "Tele2", 
#ifdef STRID_COMMON_PLMN_20402
        STRID_COMMON_PLMN_20402, 
#else
        0,
#endif
    },
    { "20404", "voda NL", 
#ifdef STRID_COMMON_PLMN_20404
        STRID_COMMON_PLMN_20404, 
#else
        0,
#endif
    },
    { "20408", "NL KPN", 
#ifdef STRID_COMMON_PLMN_20408
        STRID_COMMON_PLMN_20408, 
#else
        0,
#endif
    },
    { "20412", "NL Tlfrt", 
#ifdef STRID_COMMON_PLMN_20412
        STRID_COMMON_PLMN_20412, 
#else
        0,
#endif
    },
    { "20416", "TMO NL", 
#ifdef STRID_COMMON_PLMN_20416
        STRID_COMMON_PLMN_20416, 
#else
        0,
#endif
    },
    { "20601", "Proximus", 
#ifdef STRID_COMMON_PLMN_20601
        STRID_COMMON_PLMN_20601, 
#else
        0,
#endif
    },
    { "20610", "OBE", 
#ifdef STRID_COMMON_PLMN_20610
        STRID_COMMON_PLMN_20610, 
#else
        0,
#endif
    },
    { "20620", "BASE", 
#ifdef STRID_COMMON_PLMN_20620
        STRID_COMMON_PLMN_20620, 
#else
        0,
#endif
    },
    { "20801", "Orange", 
#ifdef STRID_COMMON_PLMN_20801
        STRID_COMMON_PLMN_20801, 
#else
        0,
#endif
    },
    { "20802", "Contact", 
#ifdef STRID_COMMON_PLMN_20802
        STRID_COMMON_PLMN_20802, 
#else
        0,
#endif
    },
    { "20809", "SFR", 
#ifdef STRID_COMMON_PLMN_20809
        STRID_COMMON_PLMN_20809, 
#else
        0,
#endif
    },
    { "20810", "SFR", 
#ifdef STRID_COMMON_PLMN_20810
        STRID_COMMON_PLMN_20810, 
#else
        0,
#endif
    },
    { "20811", "SFRFEMTO", 
#ifdef STRID_COMMON_PLMN_20811
        STRID_COMMON_PLMN_20811, 
#else
        0,
#endif
    },
    { "20813", "Contact", 
#ifdef STRID_COMMON_PLMN_20813
        STRID_COMMON_PLMN_20813, 
#else
        0,
#endif
    },
    { "20815", "Free", 
#ifdef STRID_COMMON_PLMN_20815
        STRID_COMMON_PLMN_20815, 
#else
        0,
#endif
    },
    { "20820", "BYTEL", 
#ifdef STRID_COMMON_PLMN_20820
        STRID_COMMON_PLMN_20820, 
#else
        0,
#endif
    },
    { "20888", "Contact", 
#ifdef STRID_COMMON_PLMN_20888
        STRID_COMMON_PLMN_20888, 
#else
        0,
#endif
    },
    { "21201", "vala", 
#ifdef STRID_COMMON_PLMN_21201
        STRID_COMMON_PLMN_21201, 
#else
        0,
#endif
    },
    { "21210", "MONACO", 
#ifdef STRID_COMMON_PLMN_21210
        STRID_COMMON_PLMN_21210, 
#else
        0,
#endif
    },
    { "21303", "M-AND", 
#ifdef STRID_COMMON_PLMN_21303
        STRID_COMMON_PLMN_21303, 
#else
        0,
#endif
    },
    { "21401", "voda ES", 
#ifdef STRID_COMMON_PLMN_21401
        STRID_COMMON_PLMN_21401, 
#else
        0,
#endif
    },
    { "21403", "ESPRT", 
#ifdef STRID_COMMON_PLMN_21403
        STRID_COMMON_PLMN_21403, 
#else
        0,
#endif
    },
    { "21404", "YOIGO", 
#ifdef STRID_COMMON_PLMN_21404
        STRID_COMMON_PLMN_21404, 
#else
        0,
#endif
    },
    { "21407", "Movistar", 
#ifdef STRID_COMMON_PLMN_21407
        STRID_COMMON_PLMN_21407, 
#else
        0,
#endif
    },
    { "21601", "TelenorH", 
#ifdef STRID_COMMON_PLMN_21601
        STRID_COMMON_PLMN_21601, 
#else
        0,
#endif
    },
    { "21603", "DIGI-HUN", 
#ifdef STRID_COMMON_PLMN_21603
        STRID_COMMON_PLMN_21603, 
#else
        0,
#endif
    },
    { "21630", "THU", 
#ifdef STRID_COMMON_PLMN_21630
        STRID_COMMON_PLMN_21630, 
#else
        0,
#endif
    },
    { "21670", "voda HU", 
#ifdef STRID_COMMON_PLMN_21670
        STRID_COMMON_PLMN_21670, 
#else
        0,
#endif
    },
    { "21803", "HTERONET", 
#ifdef STRID_COMMON_PLMN_21803
        STRID_COMMON_PLMN_21803, 
#else
        0,
#endif
    },
    { "21805", "m:tel", 
#ifdef STRID_COMMON_PLMN_21805
        STRID_COMMON_PLMN_21805, 
#else
        0,
#endif
    },
    { "21890", "BHMOBILE", 
#ifdef STRID_COMMON_PLMN_21890
        STRID_COMMON_PLMN_21890, 
#else
        0,
#endif
    },
    { "21901", "HT HR", 
#ifdef STRID_COMMON_PLMN_21901
        STRID_COMMON_PLMN_21901, 
#else
        0,
#endif
    },
    { "21902", "Tele2 HR", 
#ifdef STRID_COMMON_PLMN_21902
        STRID_COMMON_PLMN_21902, 
#else
        0,
#endif
    },
    { "21910", "VIP", 
#ifdef STRID_COMMON_PLMN_21910
        STRID_COMMON_PLMN_21910, 
#else
        0,
#endif
    },
    { "22001", "Telenor", 
#ifdef STRID_COMMON_PLMN_22001
        STRID_COMMON_PLMN_22001, 
#else
        0,
#endif
    },
    { "22003", "MTS", 
#ifdef STRID_COMMON_PLMN_22003
        STRID_COMMON_PLMN_22003, 
#else
        0,
#endif
    },
    { "22005", "Vip SRB", 
#ifdef STRID_COMMON_PLMN_22005
        STRID_COMMON_PLMN_22005, 
#else
        0,
#endif
    },
    { "22201", "TIM", 
#ifdef STRID_COMMON_PLMN_22201
        STRID_COMMON_PLMN_22201, 
#else
        0,
#endif
    },
    { "22210", "voda IT", 
#ifdef STRID_COMMON_PLMN_22210
        STRID_COMMON_PLMN_22210, 
#else
        0,
#endif
    },
    { "22288", "I WIND", 
#ifdef STRID_COMMON_PLMN_22288
        STRID_COMMON_PLMN_22288, 
#else
        0,
#endif
    },
    { "22299", "3 ITA", 
#ifdef STRID_COMMON_PLMN_22299
        STRID_COMMON_PLMN_22299, 
#else
        0,
#endif
    },
    { "22601", "VF-RO", 
#ifdef STRID_COMMON_PLMN_22601
        STRID_COMMON_PLMN_22601, 
#else
        0,
#endif
    },
    { "22603", "TELEKOM", 
#ifdef STRID_COMMON_PLMN_22603
        STRID_COMMON_PLMN_22603, 
#else
        0,
#endif
    },
    { "22605", "Digi", 
#ifdef STRID_COMMON_PLMN_22605
        STRID_COMMON_PLMN_22605, 
#else
        0,
#endif
    },
    { "22610", "ORANGE", 
#ifdef STRID_COMMON_PLMN_22610
        STRID_COMMON_PLMN_22610, 
#else
        0,
#endif
    },
    { "22801", "Swisscom", 
#ifdef STRID_COMMON_PLMN_22801
        STRID_COMMON_PLMN_22801, 
#else
        0,
#endif
    },
    { "22802", "Sunrise", 
#ifdef STRID_COMMON_PLMN_22802
        STRID_COMMON_PLMN_22802, 
#else
        0,
#endif
    },
    { "22803", "Salt", 
#ifdef STRID_COMMON_PLMN_22803
        STRID_COMMON_PLMN_22803, 
#else
        0,
#endif
    },
    { "23001", "TMO CZ", 
#ifdef STRID_COMMON_PLMN_23001
        STRID_COMMON_PLMN_23001, 
#else
        0,
#endif
    },
    { "23002", "O2 - CZ", 
#ifdef STRID_COMMON_PLMN_23002
        STRID_COMMON_PLMN_23002, 
#else
        0,
#endif
    },
    { "23003", "Vodafone", 
#ifdef STRID_COMMON_PLMN_23003
        STRID_COMMON_PLMN_23003, 
#else
        0,
#endif
    },
    { "23101", "Orange", 
#ifdef STRID_COMMON_PLMN_23101
        STRID_COMMON_PLMN_23101, 
#else
        0,
#endif
    },
    { "23102", "Telekom", 
#ifdef STRID_COMMON_PLMN_23102
        STRID_COMMON_PLMN_23102, 
#else
        0,
#endif
    },
    { "23103", "SWAN SK", 
#ifdef STRID_COMMON_PLMN_23103
        STRID_COMMON_PLMN_23103, 
#else
        0,
#endif
    },
    { "23106", "O2 - SK", 
#ifdef STRID_COMMON_PLMN_23106
        STRID_COMMON_PLMN_23106, 
#else
        0,
#endif
    },
    { "23201", "A1", 
#ifdef STRID_COMMON_PLMN_23201
        STRID_COMMON_PLMN_23201, 
#else
        0,
#endif
    },
    { "23203", "TMA", 
#ifdef STRID_COMMON_PLMN_23203
        STRID_COMMON_PLMN_23203, 
#else
        0,
#endif
    },
    { "23205", "3 AT", 
#ifdef STRID_COMMON_PLMN_23205
        STRID_COMMON_PLMN_23205, 
#else
        0,
#endif
    },
    { "23207", "telering", 
#ifdef STRID_COMMON_PLMN_23207
        STRID_COMMON_PLMN_23207, 
#else
        0,
#endif
    },
    { "23210", "3 AT", 
#ifdef STRID_COMMON_PLMN_23210
        STRID_COMMON_PLMN_23210, 
#else
        0,
#endif
    },
    { "23403", "ATL-VOD", 
#ifdef STRID_COMMON_PLMN_23403
        STRID_COMMON_PLMN_23403, 
#else
        0,
#endif
    },
    { "23408", "BTOP", 
#ifdef STRID_COMMON_PLMN_23408
        STRID_COMMON_PLMN_23408, 
#else
        0,
#endif
    },
    { "23410", "O2 - UK", 
#ifdef STRID_COMMON_PLMN_23410
        STRID_COMMON_PLMN_23410, 
#else
        0,
#endif
    },
    { "23415", "voda UK", 
#ifdef STRID_COMMON_PLMN_23415
        STRID_COMMON_PLMN_23415, 
#else
        0,
#endif
    },
    { "23416", "TalkTalk", 
#ifdef STRID_COMMON_PLMN_23416
        STRID_COMMON_PLMN_23416, 
#else
        0,
#endif
    },
    { "23420", "3 UK", 
#ifdef STRID_COMMON_PLMN_23420
        STRID_COMMON_PLMN_23420, 
#else
        0,
#endif
    },
    { "23428", "Marathon", 
#ifdef STRID_COMMON_PLMN_23428
        STRID_COMMON_PLMN_23428, 
#else
        0,
#endif
    },
    { "23430", "EE", 
#ifdef STRID_COMMON_PLMN_23430
        STRID_COMMON_PLMN_23430, 
#else
        0,
#endif
    },
    { "23433", "EE", 
#ifdef STRID_COMMON_PLMN_23433
        STRID_COMMON_PLMN_23433, 
#else
        0,
#endif
    },
    { "23450", "JT", 
#ifdef STRID_COMMON_PLMN_23450
        STRID_COMMON_PLMN_23450, 
#else
        0,
#endif
    },
    { "23455", "Sure", 
#ifdef STRID_COMMON_PLMN_23455
        STRID_COMMON_PLMN_23455, 
#else
        0,
#endif
    },
    { "23458", "MANX", 
#ifdef STRID_COMMON_PLMN_23458
        STRID_COMMON_PLMN_23458, 
#else
        0,
#endif
    },
    { "23801", "TDC", 
#ifdef STRID_COMMON_PLMN_23801
        STRID_COMMON_PLMN_23801, 
#else
        0,
#endif
    },
    { "23802", "TelenoDK", 
#ifdef STRID_COMMON_PLMN_23802
        STRID_COMMON_PLMN_23802, 
#else
        0,
#endif
    },
    { "23806", "3 DK", 
#ifdef STRID_COMMON_PLMN_23806
        STRID_COMMON_PLMN_23806, 
#else
        0,
#endif
    },
    { "23820", "Telia", 
#ifdef STRID_COMMON_PLMN_23820
        STRID_COMMON_PLMN_23820, 
#else
        0,
#endif
    },
    { "23866", "TT DK", 
#ifdef STRID_COMMON_PLMN_23866
        STRID_COMMON_PLMN_23866, 
#else
        0,
#endif
    },
    { "23877", "TelenoDK", 
#ifdef STRID_COMMON_PLMN_23877
        STRID_COMMON_PLMN_23877, 
#else
        0,
#endif
    },
    { "24001", "TELIA", 
#ifdef STRID_COMMON_PLMN_24001
        STRID_COMMON_PLMN_24001, 
#else
        0,
#endif
    },
    { "24002", "3 SE", 
#ifdef STRID_COMMON_PLMN_24002
        STRID_COMMON_PLMN_24002, 
#else
        0,
#endif
    },
    { "24004", "SWE", 
#ifdef STRID_COMMON_PLMN_24004
        STRID_COMMON_PLMN_24004, 
#else
        0,
#endif
    },
    { "24005", "Sweden3G", 
#ifdef STRID_COMMON_PLMN_24005
        STRID_COMMON_PLMN_24005, 
#else
        0,
#endif
    },
    { "24007", "Tele2 SE", 
#ifdef STRID_COMMON_PLMN_24007
        STRID_COMMON_PLMN_24007, 
#else
        0,
#endif
    },
    { "24008", "TelenorS", 
#ifdef STRID_COMMON_PLMN_24008
        STRID_COMMON_PLMN_24008, 
#else
        0,
#endif
    },
    { "24024", "MobileS", 
#ifdef STRID_COMMON_PLMN_24024
        STRID_COMMON_PLMN_24024, 
#else
        0,
#endif
    },
    { "24201", "TELENOR", 
#ifdef STRID_COMMON_PLMN_24201
        STRID_COMMON_PLMN_24201, 
#else
        0,
#endif
    },
    { "24202", "NetCom", 
#ifdef STRID_COMMON_PLMN_24202
        STRID_COMMON_PLMN_24202, 
#else
        0,
#endif
    },
    { "24214", "ice.net", 
#ifdef STRID_COMMON_PLMN_24214
        STRID_COMMON_PLMN_24214, 
#else
        0,
#endif
    },
    { "24403", "DNA", 
#ifdef STRID_COMMON_PLMN_24403
        STRID_COMMON_PLMN_24403, 
#else
        0,
#endif
    },
    { "24405", "elisa", 
#ifdef STRID_COMMON_PLMN_24405
        STRID_COMMON_PLMN_24405, 
#else
        0,
#endif
    },
    { "24412", "DNA", 
#ifdef STRID_COMMON_PLMN_24412
        STRID_COMMON_PLMN_24412, 
#else
        0,
#endif
    },
    { "24414", "FI AMT", 
#ifdef STRID_COMMON_PLMN_24414
        STRID_COMMON_PLMN_24414, 
#else
        0,
#endif
    },
    { "24421", "elisa", 
#ifdef STRID_COMMON_PLMN_24421
        STRID_COMMON_PLMN_24421, 
#else
        0,
#endif
    },
    { "24436", "SuomenYV", 
#ifdef STRID_COMMON_PLMN_24436
        STRID_COMMON_PLMN_24436, 
#else
        0,
#endif
    },
    { "24491", "SONERA", 
#ifdef STRID_COMMON_PLMN_24491
        STRID_COMMON_PLMN_24491, 
#else
        0,
#endif
    },
    { "24601", "OMT", 
#ifdef STRID_COMMON_PLMN_24601
        STRID_COMMON_PLMN_24601, 
#else
        0,
#endif
    },
    { "24602", "BITE", 
#ifdef STRID_COMMON_PLMN_24602
        STRID_COMMON_PLMN_24602, 
#else
        0,
#endif
    },
    { "24603", "Tele2 LT", 
#ifdef STRID_COMMON_PLMN_24603
        STRID_COMMON_PLMN_24603, 
#else
        0,
#endif
    },
    { "24701", "LMT", 
#ifdef STRID_COMMON_PLMN_24701
        STRID_COMMON_PLMN_24701, 
#else
        0,
#endif
    },
    { "24702", "Tele2 LV", 
#ifdef STRID_COMMON_PLMN_24702
        STRID_COMMON_PLMN_24702, 
#else
        0,
#endif
    },
    { "24705", "BITE LV", 
#ifdef STRID_COMMON_PLMN_24705
        STRID_COMMON_PLMN_24705, 
#else
        0,
#endif
    },
    { "24801", "Telia", 
#ifdef STRID_COMMON_PLMN_24801
        STRID_COMMON_PLMN_24801, 
#else
        0,
#endif
    },
    { "24802", "ELISA", 
#ifdef STRID_COMMON_PLMN_24802
        STRID_COMMON_PLMN_24802, 
#else
        0,
#endif
    },
    { "24803", "Tele2 EE", 
#ifdef STRID_COMMON_PLMN_24803
        STRID_COMMON_PLMN_24803, 
#else
        0,
#endif
    },
    { "25001", "MTS RUS", 
#ifdef STRID_COMMON_PLMN_25001
        STRID_COMMON_PLMN_25001, 
#else
        0,
#endif
    },
    { "25002", "MegaFon", 
#ifdef STRID_COMMON_PLMN_25002
        STRID_COMMON_PLMN_25002, 
#else
        0,
#endif
    },
    { "25007", "SMARTS", 
#ifdef STRID_COMMON_PLMN_25007
        STRID_COMMON_PLMN_25007, 
#else
        0,
#endif
    },
    { "25020", "Tele2", 
#ifdef STRID_COMMON_PLMN_25020
        STRID_COMMON_PLMN_25020, 
#else
        0,
#endif
    },
    { "25027", "LETAI", 
#ifdef STRID_COMMON_PLMN_25027
        STRID_COMMON_PLMN_25027, 
#else
        0,
#endif
    },
    { "25028", "voda", 
#ifdef STRID_COMMON_PLMN_25028
        STRID_COMMON_PLMN_25028, 
#else
        0,
#endif
    },
    { "25035", "MOTIV", 
#ifdef STRID_COMMON_PLMN_25035
        STRID_COMMON_PLMN_25035, 
#else
        0,
#endif
    },
    { "25099", "Beeline", 
#ifdef STRID_COMMON_PLMN_25099
        STRID_COMMON_PLMN_25099, 
#else
        0,
#endif
    },
    { "25501", "VODAFONE", 
#ifdef STRID_COMMON_PLMN_25501
        STRID_COMMON_PLMN_25501, 
#else
        0,
#endif
    },
    { "25502", "Beeline", 
#ifdef STRID_COMMON_PLMN_25502
        STRID_COMMON_PLMN_25502, 
#else
        0,
#endif
    },
    { "25503", "UA-KS", 
#ifdef STRID_COMMON_PLMN_25503
        STRID_COMMON_PLMN_25503, 
#else
        0,
#endif
    },
    { "25506", "life:)", 
#ifdef STRID_COMMON_PLMN_25506
        STRID_COMMON_PLMN_25506, 
#else
        0,
#endif
    },
    { "25507", "TriMob", 
#ifdef STRID_COMMON_PLMN_25507
        STRID_COMMON_PLMN_25507, 
#else
        0,
#endif
    },
    { "25701", "VELCOM", 
#ifdef STRID_COMMON_PLMN_25701
        STRID_COMMON_PLMN_25701, 
#else
        0,
#endif
    },
    { "25702", "MTS BY", 
#ifdef STRID_COMMON_PLMN_25702
        STRID_COMMON_PLMN_25702, 
#else
        0,
#endif
    },
    { "25704", "life:)BY", 
#ifdef STRID_COMMON_PLMN_25704
        STRID_COMMON_PLMN_25704, 
#else
        0,
#endif
    },
    { "25901", "Orange", 
#ifdef STRID_COMMON_PLMN_25901
        STRID_COMMON_PLMN_25901, 
#else
        0,
#endif
    },
    { "25902", "Moldcell", 
#ifdef STRID_COMMON_PLMN_25902
        STRID_COMMON_PLMN_25902, 
#else
        0,
#endif
    },
    { "25905", "UNITE", 
#ifdef STRID_COMMON_PLMN_25905
        STRID_COMMON_PLMN_25905, 
#else
        0,
#endif
    },
    { "26001", "PLUS", 
#ifdef STRID_COMMON_PLMN_26001
        STRID_COMMON_PLMN_26001, 
#else
        0,
#endif
    },
    { "26002", "TM PL", 
#ifdef STRID_COMMON_PLMN_26002
        STRID_COMMON_PLMN_26002, 
#else
        0,
#endif
    },
    { "26003", "Orange", 
#ifdef STRID_COMMON_PLMN_26003
        STRID_COMMON_PLMN_26003, 
#else
        0,
#endif
    },
    { "26006", "Play", 
#ifdef STRID_COMMON_PLMN_26006
        STRID_COMMON_PLMN_26006, 
#else
        0,
#endif
    },
    { "26201", "TDG", 
#ifdef STRID_COMMON_PLMN_26201
        STRID_COMMON_PLMN_26201, 
#else
        0,
#endif
    },
    { "26202", "Vodafone", 
#ifdef STRID_COMMON_PLMN_26202
        STRID_COMMON_PLMN_26202, 
#else
        0,
#endif
    },
    { "26203", "E-Plus", 
#ifdef STRID_COMMON_PLMN_26203
        STRID_COMMON_PLMN_26203, 
#else
        0,
#endif
    },
    { "26207", "o2 - de", 
#ifdef STRID_COMMON_PLMN_26207
        STRID_COMMON_PLMN_26207, 
#else
        0,
#endif
    },
    { "26208", "o2 - de", 
#ifdef STRID_COMMON_PLMN_26208
        STRID_COMMON_PLMN_26208, 
#else
        0,
#endif
    },
    { "26601", "GIBTEL", 
#ifdef STRID_COMMON_PLMN_26601
        STRID_COMMON_PLMN_26601, 
#else
        0,
#endif
    },
    { "26801", "voda P", 
#ifdef STRID_COMMON_PLMN_26801
        STRID_COMMON_PLMN_26801, 
#else
        0,
#endif
    },
    { "26803", "NOS", 
#ifdef STRID_COMMON_PLMN_26803
        STRID_COMMON_PLMN_26803, 
#else
        0,
#endif
    },
    { "26806", "MEO", 
#ifdef STRID_COMMON_PLMN_26806
        STRID_COMMON_PLMN_26806, 
#else
        0,
#endif
    },
    { "27001", "POST", 
#ifdef STRID_COMMON_PLMN_27001
        STRID_COMMON_PLMN_27001, 
#else
        0,
#endif
    },
    { "27077", "TANGO", 
#ifdef STRID_COMMON_PLMN_27077
        STRID_COMMON_PLMN_27077, 
#else
        0,
#endif
    },
    { "27099", "Orange", 
#ifdef STRID_COMMON_PLMN_27099
        STRID_COMMON_PLMN_27099, 
#else
        0,
#endif
    },
    { "27201", "voda IE", 
#ifdef STRID_COMMON_PLMN_27201
        STRID_COMMON_PLMN_27201, 
#else
        0,
#endif
    },
    { "27202", "3", 
#ifdef STRID_COMMON_PLMN_27202
        STRID_COMMON_PLMN_27202, 
#else
        0,
#endif
    },
    { "27203", "METEOR", 
#ifdef STRID_COMMON_PLMN_27203
        STRID_COMMON_PLMN_27203, 
#else
        0,
#endif
    },
    { "27205", "3", 
#ifdef STRID_COMMON_PLMN_27205
        STRID_COMMON_PLMN_27205, 
#else
        0,
#endif
    },
    { "27401", "SIMINN", 
#ifdef STRID_COMMON_PLMN_27401
        STRID_COMMON_PLMN_27401, 
#else
        0,
#endif
    },
    { "27402", "Vodafone", 
#ifdef STRID_COMMON_PLMN_27402
        STRID_COMMON_PLMN_27402, 
#else
        0,
#endif
    },
    { "27404", "Viking", 
#ifdef STRID_COMMON_PLMN_27404
        STRID_COMMON_PLMN_27404, 
#else
        0,
#endif
    },
    { "27408", "OnWaves", 
#ifdef STRID_COMMON_PLMN_27408
        STRID_COMMON_PLMN_27408, 
#else
        0,
#endif
    },
    { "27411", "NOVA", 
#ifdef STRID_COMMON_PLMN_27411
        STRID_COMMON_PLMN_27411, 
#else
        0,
#endif
    },
    { "27412", "TAL", 
#ifdef STRID_COMMON_PLMN_27412
        STRID_COMMON_PLMN_27412, 
#else
        0,
#endif
    },
    { "27601", "T.al", 
#ifdef STRID_COMMON_PLMN_27601
        STRID_COMMON_PLMN_27601, 
#else
        0,
#endif
    },
    { "27602", "Voda AL", 
#ifdef STRID_COMMON_PLMN_27602
        STRID_COMMON_PLMN_27602, 
#else
        0,
#endif
    },
    { "27603", "ATmobile", 
#ifdef STRID_COMMON_PLMN_27603
        STRID_COMMON_PLMN_27603, 
#else
        0,
#endif
    },
    { "27604", "PLUS AL", 
#ifdef STRID_COMMON_PLMN_27604
        STRID_COMMON_PLMN_27604, 
#else
        0,
#endif
    },
    { "27801", "voda MT", 
#ifdef STRID_COMMON_PLMN_27801
        STRID_COMMON_PLMN_27801, 
#else
        0,
#endif
    },
    { "27821", "gomobile", 
#ifdef STRID_COMMON_PLMN_27821
        STRID_COMMON_PLMN_27821, 
#else
        0,
#endif
    },
    { "27877", "Melita", 
#ifdef STRID_COMMON_PLMN_27877
        STRID_COMMON_PLMN_27877, 
#else
        0,
#endif
    },
    { "28001", "CytaVoda", 
#ifdef STRID_COMMON_PLMN_28001
        STRID_COMMON_PLMN_28001, 
#else
        0,
#endif
    },
    { "28010", "MTN", 
#ifdef STRID_COMMON_PLMN_28010
        STRID_COMMON_PLMN_28010, 
#else
        0,
#endif
    },
    { "28020", "PrimeTel", 
#ifdef STRID_COMMON_PLMN_28020
        STRID_COMMON_PLMN_28020, 
#else
        0,
#endif
    },
    { "28201", "GCELL", 
#ifdef STRID_COMMON_PLMN_28201
        STRID_COMMON_PLMN_28201, 
#else
        0,
#endif
    },
    { "28202", "MAGTI", 
#ifdef STRID_COMMON_PLMN_28202
        STRID_COMMON_PLMN_28202, 
#else
        0,
#endif
    },
    { "28204", "BEE", 
#ifdef STRID_COMMON_PLMN_28204
        STRID_COMMON_PLMN_28204, 
#else
        0,
#endif
    },
    { "28301", "Beeline", 
#ifdef STRID_COMMON_PLMN_28301
        STRID_COMMON_PLMN_28301, 
#else
        0,
#endif
    },
    { "28305", "MTS Arm", 
#ifdef STRID_COMMON_PLMN_28305
        STRID_COMMON_PLMN_28305, 
#else
        0,
#endif
    },
    { "28310", "Ucom", 
#ifdef STRID_COMMON_PLMN_28310
        STRID_COMMON_PLMN_28310, 
#else
        0,
#endif
    },
    { "28401", "Mtel", 
#ifdef STRID_COMMON_PLMN_28401
        STRID_COMMON_PLMN_28401, 
#else
        0,
#endif
    },
    { "28403", "Vivacom", 
#ifdef STRID_COMMON_PLMN_28403
        STRID_COMMON_PLMN_28403, 
#else
        0,
#endif
    },
    { "28405", "Telenor", 
#ifdef STRID_COMMON_PLMN_28405
        STRID_COMMON_PLMN_28405, 
#else
        0,
#endif
    },
    { "28601", "TCELL", 
#ifdef STRID_COMMON_PLMN_28601
        STRID_COMMON_PLMN_28601, 
#else
        0,
#endif
    },
    { "28602", "VF-TR", 
#ifdef STRID_COMMON_PLMN_28602
        STRID_COMMON_PLMN_28602, 
#else
        0,
#endif
    },
    { "28603", "AVEA", 
#ifdef STRID_COMMON_PLMN_28603
        STRID_COMMON_PLMN_28603, 
#else
        0,
#endif
    },
    { "28801", "FT-GSM", 
#ifdef STRID_COMMON_PLMN_28801
        STRID_COMMON_PLMN_28801, 
#else
        0,
#endif
    },
    { "28802", "VODAFONE", 
#ifdef STRID_COMMON_PLMN_28802
        STRID_COMMON_PLMN_28802, 
#else
        0,
#endif
    },
    { "29001", "TELE GRL", 
#ifdef STRID_COMMON_PLMN_29001
        STRID_COMMON_PLMN_29001, 
#else
        0,
#endif
    },
    { "29201", "SMT", 
#ifdef STRID_COMMON_PLMN_29201
        STRID_COMMON_PLMN_29201, 
#else
        0,
#endif
    },
    { "29340", "A1 SI", 
#ifdef STRID_COMMON_PLMN_29340
        STRID_COMMON_PLMN_29340, 
#else
        0,
#endif
    },
    { "29341", "MOBITEL", 
#ifdef STRID_COMMON_PLMN_29341
        STRID_COMMON_PLMN_29341, 
#else
        0,
#endif
    },
    { "29364", "T-2", 
#ifdef STRID_COMMON_PLMN_29364
        STRID_COMMON_PLMN_29364, 
#else
        0,
#endif
    },
    { "29370", "TELEMACH", 
#ifdef STRID_COMMON_PLMN_29370
        STRID_COMMON_PLMN_29370, 
#else
        0,
#endif
    },
    { "29401", "TMO MK", 
#ifdef STRID_COMMON_PLMN_29401
        STRID_COMMON_PLMN_29401, 
#else
        0,
#endif
    },
    { "29403", "Vip MKD", 
#ifdef STRID_COMMON_PLMN_29403
        STRID_COMMON_PLMN_29403, 
#else
        0,
#endif
    },
    { "29501", "Swiss FL", 
#ifdef STRID_COMMON_PLMN_29501
        STRID_COMMON_PLMN_29501, 
#else
        0,
#endif
    },
    { "29502", "Salt.li", 
#ifdef STRID_COMMON_PLMN_29502
        STRID_COMMON_PLMN_29502, 
#else
        0,
#endif
    },
    { "29505", "FL1", 
#ifdef STRID_COMMON_PLMN_29505
        STRID_COMMON_PLMN_29505, 
#else
        0,
#endif
    },
    { "29701", "Telenor", 
#ifdef STRID_COMMON_PLMN_29701
        STRID_COMMON_PLMN_29701, 
#else
        0,
#endif
    },
    { "29702", "T.me", 
#ifdef STRID_COMMON_PLMN_29702
        STRID_COMMON_PLMN_29702, 
#else
        0,
#endif
    },
    { "29703", "MTEL", 
#ifdef STRID_COMMON_PLMN_29703
        STRID_COMMON_PLMN_29703, 
#else
        0,
#endif
    },
    { "302220", "TELUS", 
#ifdef STRID_COMMON_PLMN_302220
        STRID_COMMON_PLMN_302220, 
#else
        0,
#endif
    },
    { "302270", "Eastlink", 
#ifdef STRID_COMMON_PLMN_302270
        STRID_COMMON_PLMN_302270, 
#else
        0,
#endif
    },
    { "302320", "Mobilici", 
#ifdef STRID_COMMON_PLMN_302320
        STRID_COMMON_PLMN_302320, 
#else
        0,
#endif
    },
    { "302340", "ETI", 
#ifdef STRID_COMMON_PLMN_302340
        STRID_COMMON_PLMN_302340, 
#else
        0,
#endif
    },
    { "302370", "Fido", 
#ifdef STRID_COMMON_PLMN_302370
        STRID_COMMON_PLMN_302370, 
#else
        0,
#endif
    },
    { "302380", "KNET", 
#ifdef STRID_COMMON_PLMN_302380
        STRID_COMMON_PLMN_302380, 
#else
        0,
#endif
    },
    { "302490", "Freedom", 
#ifdef STRID_COMMON_PLMN_302490
        STRID_COMMON_PLMN_302490, 
#else
        0,
#endif
    },
    { "302500", "CANVT", 
#ifdef STRID_COMMON_PLMN_302500
        STRID_COMMON_PLMN_302500, 
#else
        0,
#endif
    },
    { "302520", "CANVT", 
#ifdef STRID_COMMON_PLMN_302520
        STRID_COMMON_PLMN_302520, 
#else
        0,
#endif
    },
    { "302610", "Bell", 
#ifdef STRID_COMMON_PLMN_302610
        STRID_COMMON_PLMN_302610, 
#else
        0,
#endif
    },
    { "302620", "Ice", 
#ifdef STRID_COMMON_PLMN_302620
        STRID_COMMON_PLMN_302620, 
#else
        0,
#endif
    },
    { "302660", "MTS", 
#ifdef STRID_COMMON_PLMN_302660
        STRID_COMMON_PLMN_302660, 
#else
        0,
#endif
    },
    { "302720", "ROGERS", 
#ifdef STRID_COMMON_PLMN_302720
        STRID_COMMON_PLMN_302720, 
#else
        0,
#endif
    },
    { "302780", "SaskTel", 
#ifdef STRID_COMMON_PLMN_302780
        STRID_COMMON_PLMN_302780, 
#else
        0,
#endif
    },
    { "302880", "FastRoam", 
#ifdef STRID_COMMON_PLMN_302880
        STRID_COMMON_PLMN_302880, 
#else
        0,
#endif
    },
    { "302940", "Wightman", 
#ifdef STRID_COMMON_PLMN_302940
        STRID_COMMON_PLMN_302940, 
#else
        0,
#endif
    },
    { "30801", "AMERIS", 
#ifdef STRID_COMMON_PLMN_30801
        STRID_COMMON_PLMN_30801, 
#else
        0,
#endif
    },
    { "310020", "UnionTel", 
#ifdef STRID_COMMON_PLMN_310020
        STRID_COMMON_PLMN_310020, 
#else
        0,
#endif
    },
    { "310030", "AT&T", 
#ifdef STRID_COMMON_PLMN_310030
        STRID_COMMON_PLMN_310030, 
#else
        0,
#endif
    },
    { "310032", "IT&E", 
#ifdef STRID_COMMON_PLMN_310032
        STRID_COMMON_PLMN_310032, 
#else
        0,
#endif
    },
    { "310050", "GCI", 
#ifdef STRID_COMMON_PLMN_310050
        STRID_COMMON_PLMN_310050, 
#else
        0,
#endif
    },
    { "310100", "PLATEAU", 
#ifdef STRID_COMMON_PLMN_310100
        STRID_COMMON_PLMN_310100, 
#else
        0,
#endif
    },
    { "310110", "IT&E", 
#ifdef STRID_COMMON_PLMN_310110
        STRID_COMMON_PLMN_310110, 
#else
        0,
#endif
    },
    { "310120", "Sprint", 
#ifdef STRID_COMMON_PLMN_310120
        STRID_COMMON_PLMN_310120, 
#else
        0,
#endif
    },
    { "310140", "GTA", 
#ifdef STRID_COMMON_PLMN_310140
        STRID_COMMON_PLMN_310140, 
#else
        0,
#endif
    },
    { "310150", "AT&T", 
#ifdef STRID_COMMON_PLMN_310150
        STRID_COMMON_PLMN_310150, 
#else
        0,
#endif
    },
    { "310160", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310160
        STRID_COMMON_PLMN_310160, 
#else
        0,
#endif
    },
    { "310170", "AT&T", 
#ifdef STRID_COMMON_PLMN_310170
        STRID_COMMON_PLMN_310170, 
#else
        0,
#endif
    },
    { "310180", "WCW", 
#ifdef STRID_COMMON_PLMN_310180
        STRID_COMMON_PLMN_310180, 
#else
        0,
#endif
    },
    { "310190", "D-HARBOR", 
#ifdef STRID_COMMON_PLMN_310190
        STRID_COMMON_PLMN_310190, 
#else
        0,
#endif
    },
    { "310200", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310200
        STRID_COMMON_PLMN_310200, 
#else
        0,
#endif
    },
    { "310210", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310210
        STRID_COMMON_PLMN_310210, 
#else
        0,
#endif
    },
    { "310220", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310220
        STRID_COMMON_PLMN_310220, 
#else
        0,
#endif
    },
    { "310230", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310230
        STRID_COMMON_PLMN_310230, 
#else
        0,
#endif
    },
    { "310240", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310240
        STRID_COMMON_PLMN_310240, 
#else
        0,
#endif
    },
    { "310250", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310250
        STRID_COMMON_PLMN_310250, 
#else
        0,
#endif
    },
    { "310260", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310260
        STRID_COMMON_PLMN_310260, 
#else
        0,
#endif
    },
    { "310270", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310270
        STRID_COMMON_PLMN_310270, 
#else
        0,
#endif
    },
    { "310280", "AT&T", 
#ifdef STRID_COMMON_PLMN_310280
        STRID_COMMON_PLMN_310280, 
#else
        0,
#endif
    },
    { "310300", "Big Sky", 
#ifdef STRID_COMMON_PLMN_310300
        STRID_COMMON_PLMN_310300, 
#else
        0,
#endif
    },
    { "310310", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310310
        STRID_COMMON_PLMN_310310, 
#else
        0,
#endif
    },
    { "310320", "Cell", 
#ifdef STRID_COMMON_PLMN_310320
        STRID_COMMON_PLMN_310320, 
#else
        0,
#endif
    },
    { "310340", "WestLink", 
#ifdef STRID_COMMON_PLMN_310340
        STRID_COMMON_PLMN_310340, 
#else
        0,
#endif
    },
    { "310370", "DOCOMOPA", 
#ifdef STRID_COMMON_PLMN_310370
        STRID_COMMON_PLMN_310370, 
#else
        0,
#endif
    },
    { "310380", "AT&T", 
#ifdef STRID_COMMON_PLMN_310380
        STRID_COMMON_PLMN_310380, 
#else
        0,
#endif
    },
    { "310410", "AT&T", 
#ifdef STRID_COMMON_PLMN_310410
        STRID_COMMON_PLMN_310410, 
#else
        0,
#endif
    },
    { "310450", "NECCI", 
#ifdef STRID_COMMON_PLMN_310450
        STRID_COMMON_PLMN_310450, 
#else
        0,
#endif
    },
    { "310460", "USA1L", 
#ifdef STRID_COMMON_PLMN_310460
        STRID_COMMON_PLMN_310460, 
#else
        0,
#endif
    },
    { "310480", "iConnect", 
#ifdef STRID_COMMON_PLMN_310480
        STRID_COMMON_PLMN_310480, 
#else
        0,
#endif
    },
    { "310490", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310490
        STRID_COMMON_PLMN_310490, 
#else
        0,
#endif
    },
    { "310530", "IWS", 
#ifdef STRID_COMMON_PLMN_310530
        STRID_COMMON_PLMN_310530, 
#else
        0,
#endif
    },
    { "310570", "Cell One", 
#ifdef STRID_COMMON_PLMN_310570
        STRID_COMMON_PLMN_310570, 
#else
        0,
#endif
    },
    { "310630", "AMERLINK", 
#ifdef STRID_COMMON_PLMN_310630
        STRID_COMMON_PLMN_310630, 
#else
        0,
#endif
    },
    { "310660", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310660
        STRID_COMMON_PLMN_310660, 
#else
        0,
#endif
    },
    { "310690", "LMUSA", 
#ifdef STRID_COMMON_PLMN_310690
        STRID_COMMON_PLMN_310690, 
#else
        0,
#endif
    },
    { "310710", "ASTAC", 
#ifdef STRID_COMMON_PLMN_310710
        STRID_COMMON_PLMN_310710, 
#else
        0,
#endif
    },
    { "310730", "USCC", 
#ifdef STRID_COMMON_PLMN_310730
        STRID_COMMON_PLMN_310730, 
#else
        0,
#endif
    },
    { "310740", "OTZ", 
#ifdef STRID_COMMON_PLMN_310740
        STRID_COMMON_PLMN_310740, 
#else
        0,
#endif
    },
    { "310770", "IWS", 
#ifdef STRID_COMMON_PLMN_310770
        STRID_COMMON_PLMN_310770, 
#else
        0,
#endif
    },
    { "310800", "T-Mobile", 
#ifdef STRID_COMMON_PLMN_310800
        STRID_COMMON_PLMN_310800, 
#else
        0,
#endif
    },
    { "310840", "telna", 
#ifdef STRID_COMMON_PLMN_310840
        STRID_COMMON_PLMN_310840, 
#else
        0,
#endif
    },
    { "310880", "ACSIAC", 
#ifdef STRID_COMMON_PLMN_310880
        STRID_COMMON_PLMN_310880, 
#else
        0,
#endif
    },
    { "310990", "Evolve", 
#ifdef STRID_COMMON_PLMN_310990
        STRID_COMMON_PLMN_310990, 
#else
        0,
#endif
    },
    { "311030", "Indigo", 
#ifdef STRID_COMMON_PLMN_311030
        STRID_COMMON_PLMN_311030, 
#else
        0,
#endif
    },
    { "311040", "Commnet", 
#ifdef STRID_COMMON_PLMN_311040
        STRID_COMMON_PLMN_311040, 
#else
        0,
#endif
    },
    { "311080", "PINECell", 
#ifdef STRID_COMMON_PLMN_311080
        STRID_COMMON_PLMN_311080, 
#else
        0,
#endif
    },
    { "311090", "SXLP", 
#ifdef STRID_COMMON_PLMN_311090
        STRID_COMMON_PLMN_311090, 
#else
        0,
#endif
    },
    { "311190", "C1ECI", 
#ifdef STRID_COMMON_PLMN_311190
        STRID_COMMON_PLMN_311190, 
#else
        0,
#endif
    },
    { "311240", "CWCI", 
#ifdef STRID_COMMON_PLMN_311240
        STRID_COMMON_PLMN_311240, 
#else
        0,
#endif
    },
    { "311270", "Verizon", 
#ifdef STRID_COMMON_PLMN_311270
        STRID_COMMON_PLMN_311270, 
#else
        0,
#endif
    },
    { "311330", "BTW", 
#ifdef STRID_COMMON_PLMN_311330
        STRID_COMMON_PLMN_311330, 
#else
        0,
#endif
    },
    { "311370", "GCI", 
#ifdef STRID_COMMON_PLMN_311370
        STRID_COMMON_PLMN_311370, 
#else
        0,
#endif
    },
    { "311480", "Verizon", 
#ifdef STRID_COMMON_PLMN_311480
        STRID_COMMON_PLMN_311480, 
#else
        0,
#endif
    },
    { "311530", "USANW", 
#ifdef STRID_COMMON_PLMN_311530
        STRID_COMMON_PLMN_311530, 
#else
        0,
#endif
    },
    { "311540", "ProxMo", 
#ifdef STRID_COMMON_PLMN_311540
        STRID_COMMON_PLMN_311540, 
#else
        0,
#endif
    },
    { "311580", "USCC", 
#ifdef STRID_COMMON_PLMN_311580
        STRID_COMMON_PLMN_311580, 
#else
        0,
#endif
    },
    { "311660", "MPCS", 
#ifdef STRID_COMMON_PLMN_311660
        STRID_COMMON_PLMN_311660, 
#else
        0,
#endif
    },
    { "311710", "NEWN", 
#ifdef STRID_COMMON_PLMN_311710
        STRID_COMMON_PLMN_311710, 
#else
        0,
#endif
    },
    { "311730", "ProxMo", 
#ifdef STRID_COMMON_PLMN_311730
        STRID_COMMON_PLMN_311730, 
#else
        0,
#endif
    },
    { "311740", "TCI", 
#ifdef STRID_COMMON_PLMN_311740
        STRID_COMMON_PLMN_311740, 
#else
        0,
#endif
    },
    { "311810", "BW", 
#ifdef STRID_COMMON_PLMN_311810
        STRID_COMMON_PLMN_311810, 
#else
        0,
#endif
    },
    { "312060", "CvgCo", 
#ifdef STRID_COMMON_PLMN_312060
        STRID_COMMON_PLMN_312060, 
#else
        0,
#endif
    },
    { "312180", "LMUSA", 
#ifdef STRID_COMMON_PLMN_312180
        STRID_COMMON_PLMN_312180, 
#else
        0,
#endif
    },
    { "312230", "SRTW", 
#ifdef STRID_COMMON_PLMN_312230
        STRID_COMMON_PLMN_312230, 
#else
        0,
#endif
    },
    { "312260", "USACL", 
#ifdef STRID_COMMON_PLMN_312260
        STRID_COMMON_PLMN_312260, 
#else
        0,
#endif
    },
    { "312280", "CNP", 
#ifdef STRID_COMMON_PLMN_312280
        STRID_COMMON_PLMN_312280, 
#else
        0,
#endif
    },
    { "312480", "Nemont", 
#ifdef STRID_COMMON_PLMN_312480
        STRID_COMMON_PLMN_312480, 
#else
        0,
#endif
    },
    { "312630", "NetGenui", 
#ifdef STRID_COMMON_PLMN_312630
        STRID_COMMON_PLMN_312630, 
#else
        0,
#endif
    },
    { "312870", "GigSky", 
#ifdef STRID_COMMON_PLMN_312870
        STRID_COMMON_PLMN_312870, 
#else
        0,
#endif
    },
    { "330110", "Claro", 
#ifdef STRID_COMMON_PLMN_330110
        STRID_COMMON_PLMN_330110, 
#else
        0,
#endif
    },
    { "330120", "Open Mob", 
#ifdef STRID_COMMON_PLMN_330120
        STRID_COMMON_PLMN_330120, 
#else
        0,
#endif
    },
    { "334020", "TELCEL", 
#ifdef STRID_COMMON_PLMN_334020
        STRID_COMMON_PLMN_334020, 
#else
        0,
#endif
    },
    { "33403", "TEMM", 
#ifdef STRID_COMMON_PLMN_33403
        STRID_COMMON_PLMN_33403, 
#else
        0,
#endif
    },
    { "334050", "AT&T", 
#ifdef STRID_COMMON_PLMN_334050
        STRID_COMMON_PLMN_334050, 
#else
        0,
#endif
    },
    { "33490", "AT&T", 
#ifdef STRID_COMMON_PLMN_33490
        STRID_COMMON_PLMN_33490, 
#else
        0,
#endif
    },
    { "338050", "Digicel", 
#ifdef STRID_COMMON_PLMN_338050
        STRID_COMMON_PLMN_338050, 
#else
        0,
#endif
    },
    { "338180", "FLOW", 
#ifdef STRID_COMMON_PLMN_338180
        STRID_COMMON_PLMN_338180, 
#else
        0,
#endif
    },
    { "34001", "Orange", 
#ifdef STRID_COMMON_PLMN_34001
        STRID_COMMON_PLMN_34001, 
#else
        0,
#endif
    },
    { "34002", "SFR", 
#ifdef STRID_COMMON_PLMN_34002
        STRID_COMMON_PLMN_34002, 
#else
        0,
#endif
    },
    { "34020", "DigicelF", 
#ifdef STRID_COMMON_PLMN_34020
        STRID_COMMON_PLMN_34020, 
#else
        0,
#endif
    },
    { "342600", "FLOW", 
#ifdef STRID_COMMON_PLMN_342600
        STRID_COMMON_PLMN_342600, 
#else
        0,
#endif
    },
    { "342750", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_342750
        STRID_COMMON_PLMN_342750, 
#else
        0,
#endif
    },
    { "344030", "imobile", 
#ifdef STRID_COMMON_PLMN_344030
        STRID_COMMON_PLMN_344030, 
#else
        0,
#endif
    },
    { "344920", "FLOW", 
#ifdef STRID_COMMON_PLMN_344920
        STRID_COMMON_PLMN_344920, 
#else
        0,
#endif
    },
    { "344930", "Cingular", 
#ifdef STRID_COMMON_PLMN_344930
        STRID_COMMON_PLMN_344930, 
#else
        0,
#endif
    },
    { "346140", "FLOW", 
#ifdef STRID_COMMON_PLMN_346140
        STRID_COMMON_PLMN_346140, 
#else
        0,
#endif
    },
    { "348170", "FLOW", 
#ifdef STRID_COMMON_PLMN_348170
        STRID_COMMON_PLMN_348170, 
#else
        0,
#endif
    },
    { "348570", "CCTBVI", 
#ifdef STRID_COMMON_PLMN_348570
        STRID_COMMON_PLMN_348570, 
#else
        0,
#endif
    },
    { "350000", "CELLONE", 
#ifdef STRID_COMMON_PLMN_350000
        STRID_COMMON_PLMN_350000, 
#else
        0,
#endif
    },
    { "350010", "Cingular", 
#ifdef STRID_COMMON_PLMN_350010
        STRID_COMMON_PLMN_350010, 
#else
        0,
#endif
    },
    { "352030", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_352030
        STRID_COMMON_PLMN_352030, 
#else
        0,
#endif
    },
    { "352110", "FLOW", 
#ifdef STRID_COMMON_PLMN_352110
        STRID_COMMON_PLMN_352110, 
#else
        0,
#endif
    },
    { "354860", "FLOW", 
#ifdef STRID_COMMON_PLMN_354860
        STRID_COMMON_PLMN_354860, 
#else
        0,
#endif
    },
    { "356110", "FLOW", 
#ifdef STRID_COMMON_PLMN_356110
        STRID_COMMON_PLMN_356110, 
#else
        0,
#endif
    },
    { "358050", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_358050
        STRID_COMMON_PLMN_358050, 
#else
        0,
#endif
    },
    { "358110", "FLOW", 
#ifdef STRID_COMMON_PLMN_358110
        STRID_COMMON_PLMN_358110, 
#else
        0,
#endif
    },
    { "360070", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_360070
        STRID_COMMON_PLMN_360070, 
#else
        0,
#endif
    },
    { "360110", "FLOW", 
#ifdef STRID_COMMON_PLMN_360110
        STRID_COMMON_PLMN_360110, 
#else
        0,
#endif
    },
    { "36251", "Telcell", 
#ifdef STRID_COMMON_PLMN_36251
        STRID_COMMON_PLMN_36251, 
#else
        0,
#endif
    },
    { "36269", "Digicel", 
#ifdef STRID_COMMON_PLMN_36269
        STRID_COMMON_PLMN_36269, 
#else
        0,
#endif
    },
    { "36278", "Kla", 
#ifdef STRID_COMMON_PLMN_36278
        STRID_COMMON_PLMN_36278, 
#else
        0,
#endif
    },
    { "36291", "CHIPPIE", 
#ifdef STRID_COMMON_PLMN_36291
        STRID_COMMON_PLMN_36291, 
#else
        0,
#endif
    },
    { "36301", "SETAR", 
#ifdef STRID_COMMON_PLMN_36301
        STRID_COMMON_PLMN_36301, 
#else
        0,
#endif
    },
    { "36302", "Digicel", 
#ifdef STRID_COMMON_PLMN_36302
        STRID_COMMON_PLMN_36302, 
#else
        0,
#endif
    },
    { "36439", "BTC", 
#ifdef STRID_COMMON_PLMN_36439
        STRID_COMMON_PLMN_36439, 
#else
        0,
#endif
    },
    { "36449", "aliv", 
#ifdef STRID_COMMON_PLMN_36449
        STRID_COMMON_PLMN_36449, 
#else
        0,
#endif
    },
    { "365840", "FLOW", 
#ifdef STRID_COMMON_PLMN_365840
        STRID_COMMON_PLMN_365840, 
#else
        0,
#endif
    },
    { "366020", "Cingular", 
#ifdef STRID_COMMON_PLMN_366020
        STRID_COMMON_PLMN_366020, 
#else
        0,
#endif
    },
    { "366110", "FLOW", 
#ifdef STRID_COMMON_PLMN_366110
        STRID_COMMON_PLMN_366110, 
#else
        0,
#endif
    },
    { "36801", "CUBACEL", 
#ifdef STRID_COMMON_PLMN_36801
        STRID_COMMON_PLMN_36801, 
#else
        0,
#endif
    },
    { "37001", "Orange", 
#ifdef STRID_COMMON_PLMN_37001
        STRID_COMMON_PLMN_37001, 
#else
        0,
#endif
    },
    { "37002", "ClaroDOM", 
#ifdef STRID_COMMON_PLMN_37002
        STRID_COMMON_PLMN_37002, 
#else
        0,
#endif
    },
    { "37004", "Viva", 
#ifdef STRID_COMMON_PLMN_37004
        STRID_COMMON_PLMN_37004, 
#else
        0,
#endif
    },
    { "37203", "Natcom", 
#ifdef STRID_COMMON_PLMN_37203
        STRID_COMMON_PLMN_37203, 
#else
        0,
#endif
    },
    { "37412", "TSTT", 
#ifdef STRID_COMMON_PLMN_37412
        STRID_COMMON_PLMN_37412, 
#else
        0,
#endif
    },
    { "374130", "Digicel", 
#ifdef STRID_COMMON_PLMN_374130
        STRID_COMMON_PLMN_374130, 
#else
        0,
#endif
    },
    { "376350", "FLOW", 
#ifdef STRID_COMMON_PLMN_376350
        STRID_COMMON_PLMN_376350, 
#else
        0,
#endif
    },
    { "40001", "Azercell", 
#ifdef STRID_COMMON_PLMN_40001
        STRID_COMMON_PLMN_40001, 
#else
        0,
#endif
    },
    { "40002", "BAKCELL", 
#ifdef STRID_COMMON_PLMN_40002
        STRID_COMMON_PLMN_40002, 
#else
        0,
#endif
    },
    { "40004", "Nar", 
#ifdef STRID_COMMON_PLMN_40004
        STRID_COMMON_PLMN_40004, 
#else
        0,
#endif
    },
    { "40101", "Beeline", 
#ifdef STRID_COMMON_PLMN_40101
        STRID_COMMON_PLMN_40101, 
#else
        0,
#endif
    },
    { "40102", "Kcell", 
#ifdef STRID_COMMON_PLMN_40102
        STRID_COMMON_PLMN_40102, 
#else
        0,
#endif
    },
    { "40107", "ALTEL", 
#ifdef STRID_COMMON_PLMN_40107
        STRID_COMMON_PLMN_40107, 
#else
        0,
#endif
    },
    { "40177", "Tele2", 
#ifdef STRID_COMMON_PLMN_40177
        STRID_COMMON_PLMN_40177, 
#else
        0,
#endif
    },
    { "40211", "B-Mobile", 
#ifdef STRID_COMMON_PLMN_40211
        STRID_COMMON_PLMN_40211, 
#else
        0,
#endif
    },
    { "40277", "TASHICEL", 
#ifdef STRID_COMMON_PLMN_40277
        STRID_COMMON_PLMN_40277, 
#else
        0,
#endif
    },
    { "40401", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40401
        STRID_COMMON_PLMN_40401, 
#else
        0,
#endif
    },
    { "40402", "airtel", 
#ifdef STRID_COMMON_PLMN_40402
        STRID_COMMON_PLMN_40402, 
#else
        0,
#endif
    },
    { "40403", "airtel", 
#ifdef STRID_COMMON_PLMN_40403
        STRID_COMMON_PLMN_40403, 
#else
        0,
#endif
    },
    { "404030", "Vodafone", 
#ifdef STRID_COMMON_PLMN_404030
        STRID_COMMON_PLMN_404030, 
#else
        0,
#endif
    },
    { "40404", "IDEA", 
#ifdef STRID_COMMON_PLMN_40404
        STRID_COMMON_PLMN_40404, 
#else
        0,
#endif
    },
    { "40405", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40405
        STRID_COMMON_PLMN_40405, 
#else
        0,
#endif
    },
    { "40407", "IDEA", 
#ifdef STRID_COMMON_PLMN_40407
        STRID_COMMON_PLMN_40407, 
#else
        0,
#endif
    },
    { "40409", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40409
        STRID_COMMON_PLMN_40409, 
#else
        0,
#endif
    },
    { "40410", "airtel", 
#ifdef STRID_COMMON_PLMN_40410
        STRID_COMMON_PLMN_40410, 
#else
        0,
#endif
    },
    { "40411", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40411
        STRID_COMMON_PLMN_40411, 
#else
        0,
#endif
    },
    { "40412", "IDEA", 
#ifdef STRID_COMMON_PLMN_40412
        STRID_COMMON_PLMN_40412, 
#else
        0,
#endif
    },
    { "40413", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40413
        STRID_COMMON_PLMN_40413, 
#else
        0,
#endif
    },
    { "40414", "IDEA", 
#ifdef STRID_COMMON_PLMN_40414
        STRID_COMMON_PLMN_40414, 
#else
        0,
#endif
    },
    { "40415", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40415
        STRID_COMMON_PLMN_40415, 
#else
        0,
#endif
    },
    { "40416", "airtel", 
#ifdef STRID_COMMON_PLMN_40416
        STRID_COMMON_PLMN_40416, 
#else
        0,
#endif
    },
    { "40417", "Aircel", 
#ifdef STRID_COMMON_PLMN_40417
        STRID_COMMON_PLMN_40417, 
#else
        0,
#endif
    },
    { "40418", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40418
        STRID_COMMON_PLMN_40418, 
#else
        0,
#endif
    },
    { "40419", "IDEA", 
#ifdef STRID_COMMON_PLMN_40419
        STRID_COMMON_PLMN_40419, 
#else
        0,
#endif
    },
    { "40420", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40420
        STRID_COMMON_PLMN_40420, 
#else
        0,
#endif
    },
    { "40422", "IDEA", 
#ifdef STRID_COMMON_PLMN_40422
        STRID_COMMON_PLMN_40422, 
#else
        0,
#endif
    },
    { "40424", "IDEA", 
#ifdef STRID_COMMON_PLMN_40424
        STRID_COMMON_PLMN_40424, 
#else
        0,
#endif
    },
    { "40425", "Aircel", 
#ifdef STRID_COMMON_PLMN_40425
        STRID_COMMON_PLMN_40425, 
#else
        0,
#endif
    },
    { "40427", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40427
        STRID_COMMON_PLMN_40427, 
#else
        0,
#endif
    },
    { "40428", "Aircel", 
#ifdef STRID_COMMON_PLMN_40428
        STRID_COMMON_PLMN_40428, 
#else
        0,
#endif
    },
    { "40429", "Aircel", 
#ifdef STRID_COMMON_PLMN_40429
        STRID_COMMON_PLMN_40429, 
#else
        0,
#endif
    },
    { "40431", "airtel", 
#ifdef STRID_COMMON_PLMN_40431
        STRID_COMMON_PLMN_40431, 
#else
        0,
#endif
    },
    { "40433", "Aircel", 
#ifdef STRID_COMMON_PLMN_40433
        STRID_COMMON_PLMN_40433, 
#else
        0,
#endif
    },
    { "40434", "CellOne", 
#ifdef STRID_COMMON_PLMN_40434
        STRID_COMMON_PLMN_40434, 
#else
        0,
#endif
    },
    { "40435", "Aircel", 
#ifdef STRID_COMMON_PLMN_40435
        STRID_COMMON_PLMN_40435, 
#else
        0,
#endif
    },
    { "40436", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40436
        STRID_COMMON_PLMN_40436, 
#else
        0,
#endif
    },
    { "40437", "Aircel", 
#ifdef STRID_COMMON_PLMN_40437
        STRID_COMMON_PLMN_40437, 
#else
        0,
#endif
    },
    { "40438", "CellOne", 
#ifdef STRID_COMMON_PLMN_40438
        STRID_COMMON_PLMN_40438, 
#else
        0,
#endif
    },
    { "40440", "airtel", 
#ifdef STRID_COMMON_PLMN_40440
        STRID_COMMON_PLMN_40440, 
#else
        0,
#endif
    },
    { "40441", "Aircel", 
#ifdef STRID_COMMON_PLMN_40441
        STRID_COMMON_PLMN_40441, 
#else
        0,
#endif
    },
    { "40442", "Aircel", 
#ifdef STRID_COMMON_PLMN_40442
        STRID_COMMON_PLMN_40442, 
#else
        0,
#endif
    },
    { "40443", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40443
        STRID_COMMON_PLMN_40443, 
#else
        0,
#endif
    },
    { "40444", "IDEA", 
#ifdef STRID_COMMON_PLMN_40444
        STRID_COMMON_PLMN_40444, 
#else
        0,
#endif
    },
    { "40445", "airtel", 
#ifdef STRID_COMMON_PLMN_40445
        STRID_COMMON_PLMN_40445, 
#else
        0,
#endif
    },
    { "40446", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40446
        STRID_COMMON_PLMN_40446, 
#else
        0,
#endif
    },
    { "40449", "airtel", 
#ifdef STRID_COMMON_PLMN_40449
        STRID_COMMON_PLMN_40449, 
#else
        0,
#endif
    },
    { "40450", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40450
        STRID_COMMON_PLMN_40450, 
#else
        0,
#endif
    },
    { "40451", "CellOne", 
#ifdef STRID_COMMON_PLMN_40451
        STRID_COMMON_PLMN_40451, 
#else
        0,
#endif
    },
    { "40452", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40452
        STRID_COMMON_PLMN_40452, 
#else
        0,
#endif
    },
    { "40453", "CellOne", 
#ifdef STRID_COMMON_PLMN_40453
        STRID_COMMON_PLMN_40453, 
#else
        0,
#endif
    },
    { "40454", "CellOne", 
#ifdef STRID_COMMON_PLMN_40454
        STRID_COMMON_PLMN_40454, 
#else
        0,
#endif
    },
    { "40455", "CellOne", 
#ifdef STRID_COMMON_PLMN_40455
        STRID_COMMON_PLMN_40455, 
#else
        0,
#endif
    },
    { "40456", "IDEA", 
#ifdef STRID_COMMON_PLMN_40456
        STRID_COMMON_PLMN_40456, 
#else
        0,
#endif
    },
    { "40457", "CellOne", 
#ifdef STRID_COMMON_PLMN_40457
        STRID_COMMON_PLMN_40457, 
#else
        0,
#endif
    },
    { "40458", "CellOne", 
#ifdef STRID_COMMON_PLMN_40458
        STRID_COMMON_PLMN_40458, 
#else
        0,
#endif
    },
    { "40459", "CellOne", 
#ifdef STRID_COMMON_PLMN_40459
        STRID_COMMON_PLMN_40459, 
#else
        0,
#endif
    },
    { "40460", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40460
        STRID_COMMON_PLMN_40460, 
#else
        0,
#endif
    },
    { "40462", "CellOne", 
#ifdef STRID_COMMON_PLMN_40462
        STRID_COMMON_PLMN_40462, 
#else
        0,
#endif
    },
    { "40464", "CellOne", 
#ifdef STRID_COMMON_PLMN_40464
        STRID_COMMON_PLMN_40464, 
#else
        0,
#endif
    },
    { "40466", "CellOne", 
#ifdef STRID_COMMON_PLMN_40466
        STRID_COMMON_PLMN_40466, 
#else
        0,
#endif
    },
    { "40467", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40467
        STRID_COMMON_PLMN_40467, 
#else
        0,
#endif
    },
    { "40468", "DOLPHIN", 
#ifdef STRID_COMMON_PLMN_40468
        STRID_COMMON_PLMN_40468, 
#else
        0,
#endif
    },
    { "40469", "DOLPHIN", 
#ifdef STRID_COMMON_PLMN_40469
        STRID_COMMON_PLMN_40469, 
#else
        0,
#endif
    },
    { "40470", "airtel", 
#ifdef STRID_COMMON_PLMN_40470
        STRID_COMMON_PLMN_40470, 
#else
        0,
#endif
    },
    { "40471", "CellOne", 
#ifdef STRID_COMMON_PLMN_40471
        STRID_COMMON_PLMN_40471, 
#else
        0,
#endif
    },
    { "40472", "CellOne", 
#ifdef STRID_COMMON_PLMN_40472
        STRID_COMMON_PLMN_40472, 
#else
        0,
#endif
    },
    { "40473", "CellOne", 
#ifdef STRID_COMMON_PLMN_40473
        STRID_COMMON_PLMN_40473, 
#else
        0,
#endif
    },
    { "40474", "CellOne", 
#ifdef STRID_COMMON_PLMN_40474
        STRID_COMMON_PLMN_40474, 
#else
        0,
#endif
    },
    { "40475", "CellOne", 
#ifdef STRID_COMMON_PLMN_40475
        STRID_COMMON_PLMN_40475, 
#else
        0,
#endif
    },
    { "40476", "CellOne", 
#ifdef STRID_COMMON_PLMN_40476
        STRID_COMMON_PLMN_40476, 
#else
        0,
#endif
    },
    { "40477", "CellOne", 
#ifdef STRID_COMMON_PLMN_40477
        STRID_COMMON_PLMN_40477, 
#else
        0,
#endif
    },
    { "40478", "IDEA", 
#ifdef STRID_COMMON_PLMN_40478
        STRID_COMMON_PLMN_40478, 
#else
        0,
#endif
    },
    { "40479", "CellOne", 
#ifdef STRID_COMMON_PLMN_40479
        STRID_COMMON_PLMN_40479, 
#else
        0,
#endif
    },
    { "40480", "CellOne", 
#ifdef STRID_COMMON_PLMN_40480
        STRID_COMMON_PLMN_40480, 
#else
        0,
#endif
    },
    { "40481", "CellOne", 
#ifdef STRID_COMMON_PLMN_40481
        STRID_COMMON_PLMN_40481, 
#else
        0,
#endif
    },
    { "40482", "IDEA", 
#ifdef STRID_COMMON_PLMN_40482
        STRID_COMMON_PLMN_40482, 
#else
        0,
#endif
    },
    { "40483", "Reliance", 
#ifdef STRID_COMMON_PLMN_40483
        STRID_COMMON_PLMN_40483, 
#else
        0,
#endif
    },
    { "40484", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40484
        STRID_COMMON_PLMN_40484, 
#else
        0,
#endif
    },
    { "40485", "RELIANCE", 
#ifdef STRID_COMMON_PLMN_40485
        STRID_COMMON_PLMN_40485, 
#else
        0,
#endif
    },
    { "40486", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40486
        STRID_COMMON_PLMN_40486, 
#else
        0,
#endif
    },
    { "40487", "IDEA", 
#ifdef STRID_COMMON_PLMN_40487
        STRID_COMMON_PLMN_40487, 
#else
        0,
#endif
    },
    { "40488", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40488
        STRID_COMMON_PLMN_40488, 
#else
        0,
#endif
    },
    { "40489", "IDEA", 
#ifdef STRID_COMMON_PLMN_40489
        STRID_COMMON_PLMN_40489, 
#else
        0,
#endif
    },
    { "40490", "airtel", 
#ifdef STRID_COMMON_PLMN_40490
        STRID_COMMON_PLMN_40490, 
#else
        0,
#endif
    },
    { "40491", "Aircel", 
#ifdef STRID_COMMON_PLMN_40491
        STRID_COMMON_PLMN_40491, 
#else
        0,
#endif
    },
    { "40492", "airtel", 
#ifdef STRID_COMMON_PLMN_40492
        STRID_COMMON_PLMN_40492, 
#else
        0,
#endif
    },
    { "40493", "airtel", 
#ifdef STRID_COMMON_PLMN_40493
        STRID_COMMON_PLMN_40493, 
#else
        0,
#endif
    },
    { "40494", "airtel", 
#ifdef STRID_COMMON_PLMN_40494
        STRID_COMMON_PLMN_40494, 
#else
        0,
#endif
    },
    { "40495", "airtel", 
#ifdef STRID_COMMON_PLMN_40495
        STRID_COMMON_PLMN_40495, 
#else
        0,
#endif
    },
    { "40496", "airtel", 
#ifdef STRID_COMMON_PLMN_40496
        STRID_COMMON_PLMN_40496, 
#else
        0,
#endif
    },
    { "40497", "airtel", 
#ifdef STRID_COMMON_PLMN_40497
        STRID_COMMON_PLMN_40497, 
#else
        0,
#endif
    },
    { "40498", "airtel", 
#ifdef STRID_COMMON_PLMN_40498
        STRID_COMMON_PLMN_40498, 
#else
        0,
#endif
    },
    { "40501", "Reliance", 
#ifdef STRID_COMMON_PLMN_40501
        STRID_COMMON_PLMN_40501, 
#else
        0,
#endif
    },
    { "405025", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405025
        STRID_COMMON_PLMN_405025, 
#else
        0,
#endif
    },
    { "405027", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405027
        STRID_COMMON_PLMN_405027, 
#else
        0,
#endif
    },
    { "405029", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405029
        STRID_COMMON_PLMN_405029, 
#else
        0,
#endif
    },
    { "405030", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405030
        STRID_COMMON_PLMN_405030, 
#else
        0,
#endif
    },
    { "405031", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405031
        STRID_COMMON_PLMN_405031, 
#else
        0,
#endif
    },
    { "405032", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405032
        STRID_COMMON_PLMN_405032, 
#else
        0,
#endif
    },
    { "405034", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405034
        STRID_COMMON_PLMN_405034, 
#else
        0,
#endif
    },
    { "405035", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405035
        STRID_COMMON_PLMN_405035, 
#else
        0,
#endif
    },
    { "405036", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405036
        STRID_COMMON_PLMN_405036, 
#else
        0,
#endif
    },
    { "405037", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405037
        STRID_COMMON_PLMN_405037, 
#else
        0,
#endif
    },
    { "405038", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405038
        STRID_COMMON_PLMN_405038, 
#else
        0,
#endif
    },
    { "405039", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405039
        STRID_COMMON_PLMN_405039, 
#else
        0,
#endif
    },
    { "405041", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405041
        STRID_COMMON_PLMN_405041, 
#else
        0,
#endif
    },
    { "405042", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405042
        STRID_COMMON_PLMN_405042, 
#else
        0,
#endif
    },
    { "405043", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405043
        STRID_COMMON_PLMN_405043, 
#else
        0,
#endif
    },
    { "405044", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405044
        STRID_COMMON_PLMN_405044, 
#else
        0,
#endif
    },
    { "405045", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405045
        STRID_COMMON_PLMN_405045, 
#else
        0,
#endif
    },
    { "405046", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405046
        STRID_COMMON_PLMN_405046, 
#else
        0,
#endif
    },
    { "405047", "TATA DO", 
#ifdef STRID_COMMON_PLMN_405047
        STRID_COMMON_PLMN_405047, 
#else
        0,
#endif
    },
    { "40505", "Reliance", 
#ifdef STRID_COMMON_PLMN_40505
        STRID_COMMON_PLMN_40505, 
#else
        0,
#endif
    },
    { "40506", "Reliance", 
#ifdef STRID_COMMON_PLMN_40506
        STRID_COMMON_PLMN_40506, 
#else
        0,
#endif
    },
    { "40507", "Reliance", 
#ifdef STRID_COMMON_PLMN_40507
        STRID_COMMON_PLMN_40507, 
#else
        0,
#endif
    },
    { "40509", "Reliance", 
#ifdef STRID_COMMON_PLMN_40509
        STRID_COMMON_PLMN_40509, 
#else
        0,
#endif
    },
    { "40510", "Reliance", 
#ifdef STRID_COMMON_PLMN_40510
        STRID_COMMON_PLMN_40510, 
#else
        0,
#endif
    },
    { "40511", "Reliance", 
#ifdef STRID_COMMON_PLMN_40511
        STRID_COMMON_PLMN_40511, 
#else
        0,
#endif
    },
    { "40513", "Reliance", 
#ifdef STRID_COMMON_PLMN_40513
        STRID_COMMON_PLMN_40513, 
#else
        0,
#endif
    },
    { "40515", "Reliance", 
#ifdef STRID_COMMON_PLMN_40515
        STRID_COMMON_PLMN_40515, 
#else
        0,
#endif
    },
    { "40518", "Reliance", 
#ifdef STRID_COMMON_PLMN_40518
        STRID_COMMON_PLMN_40518, 
#else
        0,
#endif
    },
    { "40519", "Reliance", 
#ifdef STRID_COMMON_PLMN_40519
        STRID_COMMON_PLMN_40519, 
#else
        0,
#endif
    },
    { "40520", "Reliance", 
#ifdef STRID_COMMON_PLMN_40520
        STRID_COMMON_PLMN_40520, 
#else
        0,
#endif
    },
    { "40521", "Reliance", 
#ifdef STRID_COMMON_PLMN_40521
        STRID_COMMON_PLMN_40521, 
#else
        0,
#endif
    },
    { "40522", "Reliance", 
#ifdef STRID_COMMON_PLMN_40522
        STRID_COMMON_PLMN_40522, 
#else
        0,
#endif
    },
    { "40551", "airtel", 
#ifdef STRID_COMMON_PLMN_40551
        STRID_COMMON_PLMN_40551, 
#else
        0,
#endif
    },
    { "40552", "airtel", 
#ifdef STRID_COMMON_PLMN_40552
        STRID_COMMON_PLMN_40552, 
#else
        0,
#endif
    },
    { "40553", "airtel", 
#ifdef STRID_COMMON_PLMN_40553
        STRID_COMMON_PLMN_40553, 
#else
        0,
#endif
    },
    { "40554", "airtel", 
#ifdef STRID_COMMON_PLMN_40554
        STRID_COMMON_PLMN_40554, 
#else
        0,
#endif
    },
    { "40555", "airtel", 
#ifdef STRID_COMMON_PLMN_40555
        STRID_COMMON_PLMN_40555, 
#else
        0,
#endif
    },
    { "40556", "airtel", 
#ifdef STRID_COMMON_PLMN_40556
        STRID_COMMON_PLMN_40556, 
#else
        0,
#endif
    },
    { "40566", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40566
        STRID_COMMON_PLMN_40566, 
#else
        0,
#endif
    },
    { "40567", "Vodafone", 
#ifdef STRID_COMMON_PLMN_40567
        STRID_COMMON_PLMN_40567, 
#else
        0,
#endif
    },
    { "40570", "IDEA", 
#ifdef STRID_COMMON_PLMN_40570
        STRID_COMMON_PLMN_40570, 
#else
        0,
#endif
    },
    { "405750", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405750
        STRID_COMMON_PLMN_405750, 
#else
        0,
#endif
    },
    { "405751", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405751
        STRID_COMMON_PLMN_405751, 
#else
        0,
#endif
    },
    { "405752", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405752
        STRID_COMMON_PLMN_405752, 
#else
        0,
#endif
    },
    { "405753", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405753
        STRID_COMMON_PLMN_405753, 
#else
        0,
#endif
    },
    { "405754", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405754
        STRID_COMMON_PLMN_405754, 
#else
        0,
#endif
    },
    { "405755", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405755
        STRID_COMMON_PLMN_405755, 
#else
        0,
#endif
    },
    { "405756", "Vodafone", 
#ifdef STRID_COMMON_PLMN_405756
        STRID_COMMON_PLMN_405756, 
#else
        0,
#endif
    },
    { "405799", "IDEA", 
#ifdef STRID_COMMON_PLMN_405799
        STRID_COMMON_PLMN_405799, 
#else
        0,
#endif
    },
    { "405800", "Aircel", 
#ifdef STRID_COMMON_PLMN_405800
        STRID_COMMON_PLMN_405800, 
#else
        0,
#endif
    },
    { "405801", "Aircel", 
#ifdef STRID_COMMON_PLMN_405801
        STRID_COMMON_PLMN_405801, 
#else
        0,
#endif
    },
    { "405802", "Aircel", 
#ifdef STRID_COMMON_PLMN_405802
        STRID_COMMON_PLMN_405802, 
#else
        0,
#endif
    },
    { "405803", "Aircel", 
#ifdef STRID_COMMON_PLMN_405803
        STRID_COMMON_PLMN_405803, 
#else
        0,
#endif
    },
    { "405804", "Aircel", 
#ifdef STRID_COMMON_PLMN_405804
        STRID_COMMON_PLMN_405804, 
#else
        0,
#endif
    },
    { "405805", "Aircel", 
#ifdef STRID_COMMON_PLMN_405805
        STRID_COMMON_PLMN_405805, 
#else
        0,
#endif
    },
    { "405806", "Aircel", 
#ifdef STRID_COMMON_PLMN_405806
        STRID_COMMON_PLMN_405806, 
#else
        0,
#endif
    },
    { "405807", "Aircel", 
#ifdef STRID_COMMON_PLMN_405807
        STRID_COMMON_PLMN_405807, 
#else
        0,
#endif
    },
    { "405808", "Aircel", 
#ifdef STRID_COMMON_PLMN_405808
        STRID_COMMON_PLMN_405808, 
#else
        0,
#endif
    },
    { "405809", "Aircel", 
#ifdef STRID_COMMON_PLMN_405809
        STRID_COMMON_PLMN_405809, 
#else
        0,
#endif
    },
    { "405810", "Aircel", 
#ifdef STRID_COMMON_PLMN_405810
        STRID_COMMON_PLMN_405810, 
#else
        0,
#endif
    },
    { "405811", "Aircel", 
#ifdef STRID_COMMON_PLMN_405811
        STRID_COMMON_PLMN_405811, 
#else
        0,
#endif
    },
    { "405812", "Aircel", 
#ifdef STRID_COMMON_PLMN_405812
        STRID_COMMON_PLMN_405812, 
#else
        0,
#endif
    },
    { "405818", "Telenor", 
#ifdef STRID_COMMON_PLMN_405818
        STRID_COMMON_PLMN_405818, 
#else
        0,
#endif
    },
    { "405819", "Telenor", 
#ifdef STRID_COMMON_PLMN_405819
        STRID_COMMON_PLMN_405819, 
#else
        0,
#endif
    },
    { "405823", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405823
        STRID_COMMON_PLMN_405823, 
#else
        0,
#endif
    },
    { "405824", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405824
        STRID_COMMON_PLMN_405824, 
#else
        0,
#endif
    },
    { "405825", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405825
        STRID_COMMON_PLMN_405825, 
#else
        0,
#endif
    },
    { "405827", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405827
        STRID_COMMON_PLMN_405827, 
#else
        0,
#endif
    },
    { "405828", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405828
        STRID_COMMON_PLMN_405828, 
#else
        0,
#endif
    },
    { "405829", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405829
        STRID_COMMON_PLMN_405829, 
#else
        0,
#endif
    },
    { "405830", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405830
        STRID_COMMON_PLMN_405830, 
#else
        0,
#endif
    },
    { "405831", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405831
        STRID_COMMON_PLMN_405831, 
#else
        0,
#endif
    },
    { "405832", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405832
        STRID_COMMON_PLMN_405832, 
#else
        0,
#endif
    },
    { "405833", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405833
        STRID_COMMON_PLMN_405833, 
#else
        0,
#endif
    },
    { "405834", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405834
        STRID_COMMON_PLMN_405834, 
#else
        0,
#endif
    },
    { "405835", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405835
        STRID_COMMON_PLMN_405835, 
#else
        0,
#endif
    },
    { "405836", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405836
        STRID_COMMON_PLMN_405836, 
#else
        0,
#endif
    },
    { "405837", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405837
        STRID_COMMON_PLMN_405837, 
#else
        0,
#endif
    },
    { "405838", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405838
        STRID_COMMON_PLMN_405838, 
#else
        0,
#endif
    },
    { "405839", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405839
        STRID_COMMON_PLMN_405839, 
#else
        0,
#endif
    },
    { "405840", "JIO", 
#ifdef STRID_COMMON_PLMN_405840
        STRID_COMMON_PLMN_405840, 
#else
        0,
#endif
    },
    { "405841", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405841
        STRID_COMMON_PLMN_405841, 
#else
        0,
#endif
    },
    { "405842", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405842
        STRID_COMMON_PLMN_405842, 
#else
        0,
#endif
    },
    { "405843", "VIDEOCON", 
#ifdef STRID_COMMON_PLMN_405843
        STRID_COMMON_PLMN_405843, 
#else
        0,
#endif
    },
    { "405845", "IDEA", 
#ifdef STRID_COMMON_PLMN_405845
        STRID_COMMON_PLMN_405845, 
#else
        0,
#endif
    },
    { "405846", "IDEA", 
#ifdef STRID_COMMON_PLMN_405846
        STRID_COMMON_PLMN_405846, 
#else
        0,
#endif
    },
    { "405848", "IDEA", 
#ifdef STRID_COMMON_PLMN_405848
        STRID_COMMON_PLMN_405848, 
#else
        0,
#endif
    },
    { "405849", "IDEA", 
#ifdef STRID_COMMON_PLMN_405849
        STRID_COMMON_PLMN_405849, 
#else
        0,
#endif
    },
    { "405850", "IDEA", 
#ifdef STRID_COMMON_PLMN_405850
        STRID_COMMON_PLMN_405850, 
#else
        0,
#endif
    },
    { "405852", "IDEA", 
#ifdef STRID_COMMON_PLMN_405852
        STRID_COMMON_PLMN_405852, 
#else
        0,
#endif
    },
    { "405853", "IDEA", 
#ifdef STRID_COMMON_PLMN_405853
        STRID_COMMON_PLMN_405853, 
#else
        0,
#endif
    },
    { "405854", "JIO", 
#ifdef STRID_COMMON_PLMN_405854
        STRID_COMMON_PLMN_405854, 
#else
        0,
#endif
    },
    { "405855", "JIO", 
#ifdef STRID_COMMON_PLMN_405855
        STRID_COMMON_PLMN_405855, 
#else
        0,
#endif
    },
    { "405856", "JIO", 
#ifdef STRID_COMMON_PLMN_405856
        STRID_COMMON_PLMN_405856, 
#else
        0,
#endif
    },
    { "405857", "JIO", 
#ifdef STRID_COMMON_PLMN_405857
        STRID_COMMON_PLMN_405857, 
#else
        0,
#endif
    },
    { "405858", "JIO", 
#ifdef STRID_COMMON_PLMN_405858
        STRID_COMMON_PLMN_405858, 
#else
        0,
#endif
    },
    { "405859", "JIO", 
#ifdef STRID_COMMON_PLMN_405859
        STRID_COMMON_PLMN_405859, 
#else
        0,
#endif
    },
    { "405860", "JIO", 
#ifdef STRID_COMMON_PLMN_405860
        STRID_COMMON_PLMN_405860, 
#else
        0,
#endif
    },
    { "405861", "JIO", 
#ifdef STRID_COMMON_PLMN_405861
        STRID_COMMON_PLMN_405861, 
#else
        0,
#endif
    },
    { "405862", "JIO", 
#ifdef STRID_COMMON_PLMN_405862
        STRID_COMMON_PLMN_405862, 
#else
        0,
#endif
    },
    { "405863", "JIO", 
#ifdef STRID_COMMON_PLMN_405863
        STRID_COMMON_PLMN_405863, 
#else
        0,
#endif
    },
    { "405864", "JIO", 
#ifdef STRID_COMMON_PLMN_405864
        STRID_COMMON_PLMN_405864, 
#else
        0,
#endif
    },
    { "405865", "JIO", 
#ifdef STRID_COMMON_PLMN_405865
        STRID_COMMON_PLMN_405865, 
#else
        0,
#endif
    },
    { "405866", "JIO", 
#ifdef STRID_COMMON_PLMN_405866
        STRID_COMMON_PLMN_405866, 
#else
        0,
#endif
    },
    { "405867", "JIO", 
#ifdef STRID_COMMON_PLMN_405867
        STRID_COMMON_PLMN_405867, 
#else
        0,
#endif
    },
    { "405868", "JIO", 
#ifdef STRID_COMMON_PLMN_405868
        STRID_COMMON_PLMN_405868, 
#else
        0,
#endif
    },
    { "405869", "JIO", 
#ifdef STRID_COMMON_PLMN_405869
        STRID_COMMON_PLMN_405869, 
#else
        0,
#endif
    },
    { "405870", "JIO", 
#ifdef STRID_COMMON_PLMN_405870
        STRID_COMMON_PLMN_405870, 
#else
        0,
#endif
    },
    { "405871", "JIO", 
#ifdef STRID_COMMON_PLMN_405871
        STRID_COMMON_PLMN_405871, 
#else
        0,
#endif
    },
    { "405872", "JIO", 
#ifdef STRID_COMMON_PLMN_405872
        STRID_COMMON_PLMN_405872, 
#else
        0,
#endif
    },
    { "405873", "JIO", 
#ifdef STRID_COMMON_PLMN_405873
        STRID_COMMON_PLMN_405873, 
#else
        0,
#endif
    },
    { "405874", "JIO", 
#ifdef STRID_COMMON_PLMN_405874
        STRID_COMMON_PLMN_405874, 
#else
        0,
#endif
    },
    { "405875", "Telenor", 
#ifdef STRID_COMMON_PLMN_405875
        STRID_COMMON_PLMN_405875, 
#else
        0,
#endif
    },
    { "405876", "Telenor", 
#ifdef STRID_COMMON_PLMN_405876
        STRID_COMMON_PLMN_405876, 
#else
        0,
#endif
    },
    { "405879", "Telenor", 
#ifdef STRID_COMMON_PLMN_405879
        STRID_COMMON_PLMN_405879, 
#else
        0,
#endif
    },
    { "405927", "Telenor", 
#ifdef STRID_COMMON_PLMN_405927
        STRID_COMMON_PLMN_405927, 
#else
        0,
#endif
    },
    { "405929", "Telenor", 
#ifdef STRID_COMMON_PLMN_405929
        STRID_COMMON_PLMN_405929, 
#else
        0,
#endif
    },
    { "41001", "Jazz", 
#ifdef STRID_COMMON_PLMN_41001
        STRID_COMMON_PLMN_41001, 
#else
        0,
#endif
    },
    { "41003", "UFONE", 
#ifdef STRID_COMMON_PLMN_41003
        STRID_COMMON_PLMN_41003, 
#else
        0,
#endif
    },
    { "41004", "ZONG", 
#ifdef STRID_COMMON_PLMN_41004
        STRID_COMMON_PLMN_41004, 
#else
        0,
#endif
    },
    { "41005", "SCOM", 
#ifdef STRID_COMMON_PLMN_41005
        STRID_COMMON_PLMN_41005, 
#else
        0,
#endif
    },
    { "41006", "TELENOR", 
#ifdef STRID_COMMON_PLMN_41006
        STRID_COMMON_PLMN_41006, 
#else
        0,
#endif
    },
    { "41007", "WaridTel", 
#ifdef STRID_COMMON_PLMN_41007
        STRID_COMMON_PLMN_41007, 
#else
        0,
#endif
    },
    { "41201", "AWCC", 
#ifdef STRID_COMMON_PLMN_41201
        STRID_COMMON_PLMN_41201, 
#else
        0,
#endif
    },
    { "41220", "ROSHAN", 
#ifdef STRID_COMMON_PLMN_41220
        STRID_COMMON_PLMN_41220, 
#else
        0,
#endif
    },
    { "41240", "MTN", 
#ifdef STRID_COMMON_PLMN_41240
        STRID_COMMON_PLMN_41240, 
#else
        0,
#endif
    },
    { "41250", "Etisalat", 
#ifdef STRID_COMMON_PLMN_41250
        STRID_COMMON_PLMN_41250, 
#else
        0,
#endif
    },
    { "41301", "Mobitel", 
#ifdef STRID_COMMON_PLMN_41301
        STRID_COMMON_PLMN_41301, 
#else
        0,
#endif
    },
    { "41302", "DIALOG", 
#ifdef STRID_COMMON_PLMN_41302
        STRID_COMMON_PLMN_41302, 
#else
        0,
#endif
    },
    { "41303", "Etisalat", 
#ifdef STRID_COMMON_PLMN_41303
        STRID_COMMON_PLMN_41303, 
#else
        0,
#endif
    },
    { "41305", "Airtel", 
#ifdef STRID_COMMON_PLMN_41305
        STRID_COMMON_PLMN_41305, 
#else
        0,
#endif
    },
    { "41308", "Hutch", 
#ifdef STRID_COMMON_PLMN_41308
        STRID_COMMON_PLMN_41308, 
#else
        0,
#endif
    },
    { "41401", "MPTGSM", 
#ifdef STRID_COMMON_PLMN_41401
        STRID_COMMON_PLMN_41401, 
#else
        0,
#endif
    },
    { "41405", "Ooredoo", 
#ifdef STRID_COMMON_PLMN_41405
        STRID_COMMON_PLMN_41405, 
#else
        0,
#endif
    },
    { "41406", "TNM", 
#ifdef STRID_COMMON_PLMN_41406
        STRID_COMMON_PLMN_41406, 
#else
        0,
#endif
    },
    { "41409", "MYTEL", 
#ifdef STRID_COMMON_PLMN_41409
        STRID_COMMON_PLMN_41409, 
#else
        0,
#endif
    },
    { "41501", "alfa", 
#ifdef STRID_COMMON_PLMN_41501
        STRID_COMMON_PLMN_41501, 
#else
        0,
#endif
    },
    { "41503", "touch", 
#ifdef STRID_COMMON_PLMN_41503
        STRID_COMMON_PLMN_41503, 
#else
        0,
#endif
    },
    { "41505", "OM", 
#ifdef STRID_COMMON_PLMN_41505
        STRID_COMMON_PLMN_41505, 
#else
        0,
#endif
    },
    { "41601", "Zain JO", 
#ifdef STRID_COMMON_PLMN_41601
        STRID_COMMON_PLMN_41601, 
#else
        0,
#endif
    },
    { "41603", "UMNIAH", 
#ifdef STRID_COMMON_PLMN_41603
        STRID_COMMON_PLMN_41603, 
#else
        0,
#endif
    },
    { "41677", "OrangeJO", 
#ifdef STRID_COMMON_PLMN_41677
        STRID_COMMON_PLMN_41677, 
#else
        0,
#endif
    },
    { "41701", "SYRIATEL", 
#ifdef STRID_COMMON_PLMN_41701
        STRID_COMMON_PLMN_41701, 
#else
        0,
#endif
    },
    { "41702", "MTN", 
#ifdef STRID_COMMON_PLMN_41702
        STRID_COMMON_PLMN_41702, 
#else
        0,
#endif
    },
    { "41805", "ASIACELL", 
#ifdef STRID_COMMON_PLMN_41805
        STRID_COMMON_PLMN_41805, 
#else
        0,
#endif
    },
    { "41820", "zain IQ", 
#ifdef STRID_COMMON_PLMN_41820
        STRID_COMMON_PLMN_41820, 
#else
        0,
#endif
    },
    { "41830", "IRAQNA", 
#ifdef STRID_COMMON_PLMN_41830
        STRID_COMMON_PLMN_41830, 
#else
        0,
#endif
    },
    { "41840", "KOREK", 
#ifdef STRID_COMMON_PLMN_41840
        STRID_COMMON_PLMN_41840, 
#else
        0,
#endif
    },
    { "41902", "zain KW", 
#ifdef STRID_COMMON_PLMN_41902
        STRID_COMMON_PLMN_41902, 
#else
        0,
#endif
    },
    { "41903", "Ooredoo", 
#ifdef STRID_COMMON_PLMN_41903
        STRID_COMMON_PLMN_41903, 
#else
        0,
#endif
    },
    { "41904", "VIVA", 
#ifdef STRID_COMMON_PLMN_41904
        STRID_COMMON_PLMN_41904, 
#else
        0,
#endif
    },
    { "42001", "STC", 
#ifdef STRID_COMMON_PLMN_42001
        STRID_COMMON_PLMN_42001, 
#else
        0,
#endif
    },
    { "42003", "Mobily", 
#ifdef STRID_COMMON_PLMN_42003
        STRID_COMMON_PLMN_42003, 
#else
        0,
#endif
    },
    { "42004", "Zain KSA", 
#ifdef STRID_COMMON_PLMN_42004
        STRID_COMMON_PLMN_42004, 
#else
        0,
#endif
    },
    { "42101", "SABAFON", 
#ifdef STRID_COMMON_PLMN_42101
        STRID_COMMON_PLMN_42101, 
#else
        0,
#endif
    },
    { "42102", "MTN", 
#ifdef STRID_COMMON_PLMN_42102
        STRID_COMMON_PLMN_42102, 
#else
        0,
#endif
    },
    { "42104", "Y", 
#ifdef STRID_COMMON_PLMN_42104
        STRID_COMMON_PLMN_42104, 
#else
        0,
#endif
    },
    { "42202", "Omantel", 
#ifdef STRID_COMMON_PLMN_42202
        STRID_COMMON_PLMN_42202, 
#else
        0,
#endif
    },
    { "42203", "Ooredoo", 
#ifdef STRID_COMMON_PLMN_42203
        STRID_COMMON_PLMN_42203, 
#else
        0,
#endif
    },
    { "42402", "ETISALAT", 
#ifdef STRID_COMMON_PLMN_42402
        STRID_COMMON_PLMN_42402, 
#else
        0,
#endif
    },
    { "42403", "du", 
#ifdef STRID_COMMON_PLMN_42403
        STRID_COMMON_PLMN_42403, 
#else
        0,
#endif
    },
    { "42501", "Partner", 
#ifdef STRID_COMMON_PLMN_42501
        STRID_COMMON_PLMN_42501, 
#else
        0,
#endif
    },
    { "42502", "Cellcom", 
#ifdef STRID_COMMON_PLMN_42502
        STRID_COMMON_PLMN_42502, 
#else
        0,
#endif
    },
    { "42503", "PCL", 
#ifdef STRID_COMMON_PLMN_42503
        STRID_COMMON_PLMN_42503, 
#else
        0,
#endif
    },
    { "42505", "JAWWAL", 
#ifdef STRID_COMMON_PLMN_42505
        STRID_COMMON_PLMN_42505, 
#else
        0,
#endif
    },
    { "42506", "WM", 
#ifdef STRID_COMMON_PLMN_42506
        STRID_COMMON_PLMN_42506, 
#else
        0,
#endif
    },
    { "42507", "HOT IL", 
#ifdef STRID_COMMON_PLMN_42507
        STRID_COMMON_PLMN_42507, 
#else
        0,
#endif
    },
    { "42528", "Israel", 
#ifdef STRID_COMMON_PLMN_42528
        STRID_COMMON_PLMN_42528, 
#else
        0,
#endif
    },
    { "42601", "BATELCO", 
#ifdef STRID_COMMON_PLMN_42601
        STRID_COMMON_PLMN_42601, 
#else
        0,
#endif
    },
    { "42602", "Zain BH", 
#ifdef STRID_COMMON_PLMN_42602
        STRID_COMMON_PLMN_42602, 
#else
        0,
#endif
    },
    { "42604", "VIVA BH", 
#ifdef STRID_COMMON_PLMN_42604
        STRID_COMMON_PLMN_42604, 
#else
        0,
#endif
    },
    { "42701", "Ooredoo", 
#ifdef STRID_COMMON_PLMN_42701
        STRID_COMMON_PLMN_42701, 
#else
        0,
#endif
    },
    { "42702", "vodafone", 
#ifdef STRID_COMMON_PLMN_42702
        STRID_COMMON_PLMN_42702, 
#else
        0,
#endif
    },
    { "42801", "Skytel", 
#ifdef STRID_COMMON_PLMN_42801
        STRID_COMMON_PLMN_42801, 
#else
        0,
#endif
    },
    { "42888", "UNTLMN", 
#ifdef STRID_COMMON_PLMN_42888
        STRID_COMMON_PLMN_42888, 
#else
        0,
#endif
    },
    { "42899", "MobiCom", 
#ifdef STRID_COMMON_PLMN_42899
        STRID_COMMON_PLMN_42899, 
#else
        0,
#endif
    },
    { "42901", "NT", 
#ifdef STRID_COMMON_PLMN_42901
        STRID_COMMON_PLMN_42901, 
#else
        0,
#endif
    },
    { "42902", "Ncell", 
#ifdef STRID_COMMON_PLMN_42902
        STRID_COMMON_PLMN_42902, 
#else
        0,
#endif
    },
    { "43211", "IR-TCI", 
#ifdef STRID_COMMON_PLMN_43211
        STRID_COMMON_PLMN_43211, 
#else
        0,
#endif
    },
    { "43220", "IRN 20", 
#ifdef STRID_COMMON_PLMN_43220
        STRID_COMMON_PLMN_43220, 
#else
        0,
#endif
    },
    { "43232", "Taliya", 
#ifdef STRID_COMMON_PLMN_43232
        STRID_COMMON_PLMN_43232, 
#else
        0,
#endif
    },
    { "43235", "Irancell", 
#ifdef STRID_COMMON_PLMN_43235
        STRID_COMMON_PLMN_43235, 
#else
        0,
#endif
    },
    { "43404", "Beeline", 
#ifdef STRID_COMMON_PLMN_43404
        STRID_COMMON_PLMN_43404, 
#else
        0,
#endif
    },
    { "43405", "Ucell", 
#ifdef STRID_COMMON_PLMN_43405
        STRID_COMMON_PLMN_43405, 
#else
        0,
#endif
    },
    { "43407", "UMS", 
#ifdef STRID_COMMON_PLMN_43407
        STRID_COMMON_PLMN_43407, 
#else
        0,
#endif
    },
    { "43408", "UzMobile", 
#ifdef STRID_COMMON_PLMN_43408
        STRID_COMMON_PLMN_43408, 
#else
        0,
#endif
    },
    { "43601", "TCELL", 
#ifdef STRID_COMMON_PLMN_43601
        STRID_COMMON_PLMN_43601, 
#else
        0,
#endif
    },
    { "43602", "TCELL", 
#ifdef STRID_COMMON_PLMN_43602
        STRID_COMMON_PLMN_43602, 
#else
        0,
#endif
    },
    { "43603", "MegaFon", 
#ifdef STRID_COMMON_PLMN_43603
        STRID_COMMON_PLMN_43603, 
#else
        0,
#endif
    },
    { "43604", "Babilon", 
#ifdef STRID_COMMON_PLMN_43604
        STRID_COMMON_PLMN_43604, 
#else
        0,
#endif
    },
    { "43605", "BEELINE", 
#ifdef STRID_COMMON_PLMN_43605
        STRID_COMMON_PLMN_43605, 
#else
        0,
#endif
    },
    { "43701", "Beeline", 
#ifdef STRID_COMMON_PLMN_43701
        STRID_COMMON_PLMN_43701, 
#else
        0,
#endif
    },
    { "43705", "MegaCom", 
#ifdef STRID_COMMON_PLMN_43705
        STRID_COMMON_PLMN_43705, 
#else
        0,
#endif
    },
    { "43709", "O!", 
#ifdef STRID_COMMON_PLMN_43709
        STRID_COMMON_PLMN_43709, 
#else
        0,
#endif
    },
    { "43801", "MTS TM", 
#ifdef STRID_COMMON_PLMN_43801
        STRID_COMMON_PLMN_43801, 
#else
        0,
#endif
    },
    { "43802", "TM CELL", 
#ifdef STRID_COMMON_PLMN_43802
        STRID_COMMON_PLMN_43802, 
#else
        0,
#endif
    },
    { "44010", "DOCOMO", 
#ifdef STRID_COMMON_PLMN_44010
        STRID_COMMON_PLMN_44010, 
#else
        0,
#endif
    },
    { "44020", "SoftBank", 
#ifdef STRID_COMMON_PLMN_44020
        STRID_COMMON_PLMN_44020, 
#else
        0,
#endif
    },
    { "44050", "KDDI", 
#ifdef STRID_COMMON_PLMN_44050
        STRID_COMMON_PLMN_44050, 
#else
        0,
#endif
    },
    { "44051", "KDDI", 
#ifdef STRID_COMMON_PLMN_44051
        STRID_COMMON_PLMN_44051, 
#else
        0,
#endif
    },
    { "45005", "SKT", 
#ifdef STRID_COMMON_PLMN_45005
        STRID_COMMON_PLMN_45005, 
#else
        0,
#endif
    },
    { "45006", "LG U+", 
#ifdef STRID_COMMON_PLMN_45006
        STRID_COMMON_PLMN_45006, 
#else
        0,
#endif
    },
    { "45008", "KT", 
#ifdef STRID_COMMON_PLMN_45008
        STRID_COMMON_PLMN_45008, 
#else
        0,
#endif
    },
    { "45201", "Mobifone", 
#ifdef STRID_COMMON_PLMN_45201
        STRID_COMMON_PLMN_45201, 
#else
        0,
#endif
    },
    { "45202", "GPC", 
#ifdef STRID_COMMON_PLMN_45202
        STRID_COMMON_PLMN_45202, 
#else
        0,
#endif
    },
    { "45204", "VIETTEL", 
#ifdef STRID_COMMON_PLMN_45204
        STRID_COMMON_PLMN_45204, 
#else
        0,
#endif
    },
    { "45205", "VNMOBILE", 
#ifdef STRID_COMMON_PLMN_45205
        STRID_COMMON_PLMN_45205, 
#else
        0,
#endif
    },
    { "45207", "Gmobile", 
#ifdef STRID_COMMON_PLMN_45207
        STRID_COMMON_PLMN_45207, 
#else
        0,
#endif
    },
    { "45400", "CSL", 
#ifdef STRID_COMMON_PLMN_45400
        STRID_COMMON_PLMN_45400, 
#else
        0,
#endif
    },
    { "45402", "CSL", 
#ifdef STRID_COMMON_PLMN_45402
        STRID_COMMON_PLMN_45402, 
#else
        0,
#endif
    },
    { "45403", "3", 
#ifdef STRID_COMMON_PLMN_45403
        STRID_COMMON_PLMN_45403, 
#else
        0,
#endif
    },
    { "45404", "3", 
#ifdef STRID_COMMON_PLMN_45404
        STRID_COMMON_PLMN_45404, 
#else
        0,
#endif
    },
    { "45406", "SMC HK", 
#ifdef STRID_COMMON_PLMN_45406
        STRID_COMMON_PLMN_45406, 
#else
        0,
#endif
    },
    { "45410", "CSL", 
#ifdef STRID_COMMON_PLMN_45410
        STRID_COMMON_PLMN_45410, 
#else
        0,
#endif
    },
    { "45412", "CMHK", 
#ifdef STRID_COMMON_PLMN_45412
        STRID_COMMON_PLMN_45412, 
#else
        0,
#endif
    },
    { "45413", "CMHK", 
#ifdef STRID_COMMON_PLMN_45413
        STRID_COMMON_PLMN_45413, 
#else
        0,
#endif
    },
    { "45415", "SMC HK", 
#ifdef STRID_COMMON_PLMN_45415
        STRID_COMMON_PLMN_45415, 
#else
        0,
#endif
    },
    { "45416", "CSL", 
#ifdef STRID_COMMON_PLMN_45416
        STRID_COMMON_PLMN_45416, 
#else
        0,
#endif
    },
    { "45417", "SMC HK", 
#ifdef STRID_COMMON_PLMN_45417
        STRID_COMMON_PLMN_45417, 
#else
        0,
#endif
    },
    { "45418", "CSL", 
#ifdef STRID_COMMON_PLMN_45418
        STRID_COMMON_PLMN_45418, 
#else
        0,
#endif
    },
    { "45419", "CSL", 
#ifdef STRID_COMMON_PLMN_45419
        STRID_COMMON_PLMN_45419, 
#else
        0,
#endif
    },
    { "45420", "CSL", 
#ifdef STRID_COMMON_PLMN_45420
        STRID_COMMON_PLMN_45420, 
#else
        0,
#endif
    },
    { "45500", "SMC MAC", 
#ifdef STRID_COMMON_PLMN_45500
        STRID_COMMON_PLMN_45500, 
#else
        0,
#endif
    },
    { "45501", "CTM", 
#ifdef STRID_COMMON_PLMN_45501
        STRID_COMMON_PLMN_45501, 
#else
        0,
#endif
    },
    { "45503", "3 Macau", 
#ifdef STRID_COMMON_PLMN_45503
        STRID_COMMON_PLMN_45503, 
#else
        0,
#endif
    },
    { "45504", "CTM", 
#ifdef STRID_COMMON_PLMN_45504
        STRID_COMMON_PLMN_45504, 
#else
        0,
#endif
    },
    { "45601", "Cellcard", 
#ifdef STRID_COMMON_PLMN_45601
        STRID_COMMON_PLMN_45601, 
#else
        0,
#endif
    },
    { "45604", "CADCOMMS", 
#ifdef STRID_COMMON_PLMN_45604
        STRID_COMMON_PLMN_45604, 
#else
        0,
#endif
    },
    { "45605", "SMART", 
#ifdef STRID_COMMON_PLMN_45605
        STRID_COMMON_PLMN_45605, 
#else
        0,
#endif
    },
    { "45606", "SMART", 
#ifdef STRID_COMMON_PLMN_45606
        STRID_COMMON_PLMN_45606, 
#else
        0,
#endif
    },
    { "45608", "Metfone", 
#ifdef STRID_COMMON_PLMN_45608
        STRID_COMMON_PLMN_45608, 
#else
        0,
#endif
    },
    { "45611", "SEATEL", 
#ifdef STRID_COMMON_PLMN_45611
        STRID_COMMON_PLMN_45611, 
#else
        0,
#endif
    },
    { "45701", "LAO GSM", 
#ifdef STRID_COMMON_PLMN_45701
        STRID_COMMON_PLMN_45701, 
#else
        0,
#endif
    },
    { "45702", "ETLMNW", 
#ifdef STRID_COMMON_PLMN_45702
        STRID_COMMON_PLMN_45702, 
#else
        0,
#endif
    },
    { "45703", "UNITEL", 
#ifdef STRID_COMMON_PLMN_45703
        STRID_COMMON_PLMN_45703, 
#else
        0,
#endif
    },
    { "45708", "BEELINE", 
#ifdef STRID_COMMON_PLMN_45708
        STRID_COMMON_PLMN_45708, 
#else
        0,
#endif
    },
    { "46000", "CMCC", 
#ifdef STRID_COMMON_PLMN_46000
        STRID_COMMON_PLMN_46000, 
#else
        0,
#endif
    },
    { "46001", "UNICOM", 
#ifdef STRID_COMMON_PLMN_46001
        STRID_COMMON_PLMN_46001, 
#else
        0,
#endif
    },
    { "46002", "CMCC", 
#ifdef STRID_COMMON_PLMN_46002
        STRID_COMMON_PLMN_46002, 
#else
        0,
#endif
    },
    { "46003", "CT", 
#ifdef STRID_COMMON_PLMN_46003
        STRID_COMMON_PLMN_46003, 
#else
        0,
#endif
    },
    { "46004", "CMCC", 
#ifdef STRID_COMMON_PLMN_46004
        STRID_COMMON_PLMN_46004, 
#else
        0,
#endif
    },
    { "46007", "CMCC", 
#ifdef STRID_COMMON_PLMN_46007
        STRID_COMMON_PLMN_46007, 
#else
        0,
#endif
    },
    { "46008", "CMCC", 
#ifdef STRID_COMMON_PLMN_46008
        STRID_COMMON_PLMN_46008, 
#else
        0,
#endif
    },
    { "46009", "UNICOM", 
#ifdef STRID_COMMON_PLMN_46009
        STRID_COMMON_PLMN_46009, 
#else
        0,
#endif
    },
    { "46011", "CT", 
#ifdef STRID_COMMON_PLMN_46011
        STRID_COMMON_PLMN_46011, 
#else
        0,
#endif
    },
    { "46601", "FET", 
#ifdef STRID_COMMON_PLMN_46601
        STRID_COMMON_PLMN_46601, 
#else
        0,
#endif
    },
    { "46605", "APT", 
#ifdef STRID_COMMON_PLMN_46605
        STRID_COMMON_PLMN_46605, 
#else
        0,
#endif
    },
    { "46688", "KGT", 
#ifdef STRID_COMMON_PLMN_46688
        STRID_COMMON_PLMN_46688, 
#else
        0,
#endif
    },
    { "46689", "T Star", 
#ifdef STRID_COMMON_PLMN_46689
        STRID_COMMON_PLMN_46689, 
#else
        0,
#endif
    },
    { "46692", "Chunghwa", 
#ifdef STRID_COMMON_PLMN_46692
        STRID_COMMON_PLMN_46692, 
#else
        0,
#endif
    },
    { "46693", "TW MOB", 
#ifdef STRID_COMMON_PLMN_46693
        STRID_COMMON_PLMN_46693, 
#else
        0,
#endif
    },
    { "46697", "TWM", 
#ifdef STRID_COMMON_PLMN_46697
        STRID_COMMON_PLMN_46697, 
#else
        0,
#endif
    },
    { "46699", "TWM TAT", 
#ifdef STRID_COMMON_PLMN_46699
        STRID_COMMON_PLMN_46699, 
#else
        0,
#endif
    },
    { "46705", "KL-DPRK", 
#ifdef STRID_COMMON_PLMN_46705
        STRID_COMMON_PLMN_46705, 
#else
        0,
#endif
    },
    { "470001", "Grameen", 
#ifdef STRID_COMMON_PLMN_470001
        STRID_COMMON_PLMN_470001, 
#else
        0,
#endif
    },
    { "47002", "robi", 
#ifdef STRID_COMMON_PLMN_47002
        STRID_COMMON_PLMN_47002, 
#else
        0,
#endif
    },
    { "47003", "BL", 
#ifdef STRID_COMMON_PLMN_47003
        STRID_COMMON_PLMN_47003, 
#else
        0,
#endif
    },
    { "47004", "bMobile", 
#ifdef STRID_COMMON_PLMN_47004
        STRID_COMMON_PLMN_47004, 
#else
        0,
#endif
    },
    { "47007", "Airtel", 
#ifdef STRID_COMMON_PLMN_47007
        STRID_COMMON_PLMN_47007, 
#else
        0,
#endif
    },
    { "47009", "BIEL 4G", 
#ifdef STRID_COMMON_PLMN_47009
        STRID_COMMON_PLMN_47009, 
#else
        0,
#endif
    },
    { "47201", "DHIRAAGU", 
#ifdef STRID_COMMON_PLMN_47201
        STRID_COMMON_PLMN_47201, 
#else
        0,
#endif
    },
    { "47202", "Ooredoo", 
#ifdef STRID_COMMON_PLMN_47202
        STRID_COMMON_PLMN_47202, 
#else
        0,
#endif
    },
    { "50212", "MY MAXIS", 
#ifdef STRID_COMMON_PLMN_50212
        STRID_COMMON_PLMN_50212, 
#else
        0,
#endif
    },
    { "50213", "CELCOM", 
#ifdef STRID_COMMON_PLMN_50213
        STRID_COMMON_PLMN_50213, 
#else
        0,
#endif
    },
    { "502152", "Yes 4G", 
#ifdef STRID_COMMON_PLMN_502152
        STRID_COMMON_PLMN_502152, 
#else
        0,
#endif
    },
    { "502153", "webe", 
#ifdef STRID_COMMON_PLMN_502153
        STRID_COMMON_PLMN_502153, 
#else
        0,
#endif
    },
    { "502156", "ALTEL", 
#ifdef STRID_COMMON_PLMN_502156
        STRID_COMMON_PLMN_502156, 
#else
        0,
#endif
    },
    { "50216", "Digi 4G+", 
#ifdef STRID_COMMON_PLMN_50216
        STRID_COMMON_PLMN_50216, 
#else
        0,
#endif
    },
    { "50218", "U MOBILE", 
#ifdef STRID_COMMON_PLMN_50218
        STRID_COMMON_PLMN_50218, 
#else
        0,
#endif
    },
    { "50219", "CELCOM", 
#ifdef STRID_COMMON_PLMN_50219
        STRID_COMMON_PLMN_50219, 
#else
        0,
#endif
    },
    { "50501", "Telstra", 
#ifdef STRID_COMMON_PLMN_50501
        STRID_COMMON_PLMN_50501, 
#else
        0,
#endif
    },
    { "50502", "Optus", 
#ifdef STRID_COMMON_PLMN_50502
        STRID_COMMON_PLMN_50502, 
#else
        0,
#endif
    },
    { "50503", "voda AU", 
#ifdef STRID_COMMON_PLMN_50503
        STRID_COMMON_PLMN_50503, 
#else
        0,
#endif
    },
    { "50506", "3TELSTRA", 
#ifdef STRID_COMMON_PLMN_50506
        STRID_COMMON_PLMN_50506, 
#else
        0,
#endif
    },
    { "50510", "NT", 
#ifdef STRID_COMMON_PLMN_50510
        STRID_COMMON_PLMN_50510, 
#else
        0,
#endif
    },
    { "50571", "Telstra", 
#ifdef STRID_COMMON_PLMN_50571
        STRID_COMMON_PLMN_50571, 
#else
        0,
#endif
    },
    { "50572", "Telstra", 
#ifdef STRID_COMMON_PLMN_50572
        STRID_COMMON_PLMN_50572, 
#else
        0,
#endif
    },
    { "51001", "INDOSAT", 
#ifdef STRID_COMMON_PLMN_51001
        STRID_COMMON_PLMN_51001, 
#else
        0,
#endif
    },
    { "51008", "Axis", 
#ifdef STRID_COMMON_PLMN_51008
        STRID_COMMON_PLMN_51008, 
#else
        0,
#endif
    },
    { "51009", "SF", 
#ifdef STRID_COMMON_PLMN_51009
        STRID_COMMON_PLMN_51009, 
#else
        0,
#endif
    },
    { "51010", "T-SEL", 
#ifdef STRID_COMMON_PLMN_51010
        STRID_COMMON_PLMN_51010, 
#else
        0,
#endif
    },
    { "51011", "XL", 
#ifdef STRID_COMMON_PLMN_51011
        STRID_COMMON_PLMN_51011, 
#else
        0,
#endif
    },
    { "51021", "INDOSAT", 
#ifdef STRID_COMMON_PLMN_51021
        STRID_COMMON_PLMN_51021, 
#else
        0,
#endif
    },
    { "51028", "SF", 
#ifdef STRID_COMMON_PLMN_51028
        STRID_COMMON_PLMN_51028, 
#else
        0,
#endif
    },
    { "51088", "Bolt-ID", 
#ifdef STRID_COMMON_PLMN_51088
        STRID_COMMON_PLMN_51088, 
#else
        0,
#endif
    },
    { "51089", "3", 
#ifdef STRID_COMMON_PLMN_51089
        STRID_COMMON_PLMN_51089, 
#else
        0,
#endif
    },
    { "51401", "TCEL", 
#ifdef STRID_COMMON_PLMN_51401
        STRID_COMMON_PLMN_51401, 
#else
        0,
#endif
    },
    { "51402", "TT", 
#ifdef STRID_COMMON_PLMN_51402
        STRID_COMMON_PLMN_51402, 
#else
        0,
#endif
    },
    { "51502", "GLOBE", 
#ifdef STRID_COMMON_PLMN_51502
        STRID_COMMON_PLMN_51502, 
#else
        0,
#endif
    },
    { "51503", "SMART", 
#ifdef STRID_COMMON_PLMN_51503
        STRID_COMMON_PLMN_51503, 
#else
        0,
#endif
    },
    { "51505", "SUN", 
#ifdef STRID_COMMON_PLMN_51505
        STRID_COMMON_PLMN_51505, 
#else
        0,
#endif
    },
    { "52000", "TH 3G+", 
#ifdef STRID_COMMON_PLMN_52000
        STRID_COMMON_PLMN_52000, 
#else
        0,
#endif
    },
    { "52001", "AIS", 
#ifdef STRID_COMMON_PLMN_52001
        STRID_COMMON_PLMN_52001, 
#else
        0,
#endif
    },
    { "52003", "AIS", 
#ifdef STRID_COMMON_PLMN_52003
        STRID_COMMON_PLMN_52003, 
#else
        0,
#endif
    },
    { "52004", "TRUE-H", 
#ifdef STRID_COMMON_PLMN_52004
        STRID_COMMON_PLMN_52004, 
#else
        0,
#endif
    },
    { "52005", "dtac", 
#ifdef STRID_COMMON_PLMN_52005
        STRID_COMMON_PLMN_52005, 
#else
        0,
#endif
    },
    { "52015", "TOT3G", 
#ifdef STRID_COMMON_PLMN_52015
        STRID_COMMON_PLMN_52015, 
#else
        0,
#endif
    },
    { "52018", "DTAC", 
#ifdef STRID_COMMON_PLMN_52018
        STRID_COMMON_PLMN_52018, 
#else
        0,
#endif
    },
    { "52099", "TRUE-H", 
#ifdef STRID_COMMON_PLMN_52099
        STRID_COMMON_PLMN_52099, 
#else
        0,
#endif
    },
    { "52501", "Singtel", 
#ifdef STRID_COMMON_PLMN_52501
        STRID_COMMON_PLMN_52501, 
#else
        0,
#endif
    },
    { "52502", "Singtel", 
#ifdef STRID_COMMON_PLMN_52502
        STRID_COMMON_PLMN_52502, 
#else
        0,
#endif
    },
    { "52503", "M1", 
#ifdef STRID_COMMON_PLMN_52503
        STRID_COMMON_PLMN_52503, 
#else
        0,
#endif
    },
    { "52505", "StarHub", 
#ifdef STRID_COMMON_PLMN_52505
        STRID_COMMON_PLMN_52505, 
#else
        0,
#endif
    },
    { "52507", "CallZone", 
#ifdef STRID_COMMON_PLMN_52507
        STRID_COMMON_PLMN_52507, 
#else
        0,
#endif
    },
    { "52508", "StarHub", 
#ifdef STRID_COMMON_PLMN_52508
        STRID_COMMON_PLMN_52508, 
#else
        0,
#endif
    },
    { "52802", "PCSB", 
#ifdef STRID_COMMON_PLMN_52802
        STRID_COMMON_PLMN_52802, 
#else
        0,
#endif
    },
    { "52811", "DSTCom", 
#ifdef STRID_COMMON_PLMN_52811
        STRID_COMMON_PLMN_52811, 
#else
        0,
#endif
    },
    { "53001", "voda NZ", 
#ifdef STRID_COMMON_PLMN_53001
        STRID_COMMON_PLMN_53001, 
#else
        0,
#endif
    },
    { "53005", "Spark NZ", 
#ifdef STRID_COMMON_PLMN_53005
        STRID_COMMON_PLMN_53005, 
#else
        0,
#endif
    },
    { "53024", "2degrees", 
#ifdef STRID_COMMON_PLMN_53024
        STRID_COMMON_PLMN_53024, 
#else
        0,
#endif
    },
    { "53701", "bmobile", 
#ifdef STRID_COMMON_PLMN_53701
        STRID_COMMON_PLMN_53701, 
#else
        0,
#endif
    },
    { "53702", "TM", 
#ifdef STRID_COMMON_PLMN_53702
        STRID_COMMON_PLMN_53702, 
#else
        0,
#endif
    },
    { "53703", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_53703
        STRID_COMMON_PLMN_53703, 
#else
        0,
#endif
    },
    { "53901", "U-CALL", 
#ifdef STRID_COMMON_PLMN_53901
        STRID_COMMON_PLMN_53901, 
#else
        0,
#endif
    },
    { "53988", "Digicel", 
#ifdef STRID_COMMON_PLMN_53988
        STRID_COMMON_PLMN_53988, 
#else
        0,
#endif
    },
    { "54002", "bmobile", 
#ifdef STRID_COMMON_PLMN_54002
        STRID_COMMON_PLMN_54002, 
#else
        0,
#endif
    },
    { "54101", "SMILE", 
#ifdef STRID_COMMON_PLMN_54101
        STRID_COMMON_PLMN_54101, 
#else
        0,
#endif
    },
    { "54105", "Digicel", 
#ifdef STRID_COMMON_PLMN_54105
        STRID_COMMON_PLMN_54105, 
#else
        0,
#endif
    },
    { "54201", "VODAFONE", 
#ifdef STRID_COMMON_PLMN_54201
        STRID_COMMON_PLMN_54201, 
#else
        0,
#endif
    },
    { "54202", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_54202
        STRID_COMMON_PLMN_54202, 
#else
        0,
#endif
    },
    { "54301", "Manuia", 
#ifdef STRID_COMMON_PLMN_54301
        STRID_COMMON_PLMN_54301, 
#else
        0,
#endif
    },
    { "544110", "BLUESKY", 
#ifdef STRID_COMMON_PLMN_544110
        STRID_COMMON_PLMN_544110, 
#else
        0,
#endif
    },
    { "54601", "MOBNCL", 
#ifdef STRID_COMMON_PLMN_54601
        STRID_COMMON_PLMN_54601, 
#else
        0,
#endif
    },
    { "54715", "VODAFONE", 
#ifdef STRID_COMMON_PLMN_54715
        STRID_COMMON_PLMN_54715, 
#else
        0,
#endif
    },
    { "54720", "VINI", 
#ifdef STRID_COMMON_PLMN_54720
        STRID_COMMON_PLMN_54720, 
#else
        0,
#endif
    },
    { "54801", "KOKANET", 
#ifdef STRID_COMMON_PLMN_54801
        STRID_COMMON_PLMN_54801, 
#else
        0,
#endif
    },
    { "54900", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_54900
        STRID_COMMON_PLMN_54900, 
#else
        0,
#endif
    },
    { "54927", "Bluesky", 
#ifdef STRID_COMMON_PLMN_54927
        STRID_COMMON_PLMN_54927, 
#else
        0,
#endif
    },
    { "55001", "FSM Tele", 
#ifdef STRID_COMMON_PLMN_55001
        STRID_COMMON_PLMN_55001, 
#else
        0,
#endif
    },
    { "55101", "NTA", 
#ifdef STRID_COMMON_PLMN_55101
        STRID_COMMON_PLMN_55101, 
#else
        0,
#endif
    },
    { "55201", "PalauCel", 
#ifdef STRID_COMMON_PLMN_55201
        STRID_COMMON_PLMN_55201, 
#else
        0,
#endif
    },
    { "55301", "FSM Tele", 
#ifdef STRID_COMMON_PLMN_55301
        STRID_COMMON_PLMN_55301, 
#else
        0,
#endif
    },
    { "55401", "TTOK", 
#ifdef STRID_COMMON_PLMN_55401
        STRID_COMMON_PLMN_55401, 
#else
        0,
#endif
    },
    { "60201", "Orange", 
#ifdef STRID_COMMON_PLMN_60201
        STRID_COMMON_PLMN_60201, 
#else
        0,
#endif
    },
    { "60202", "voda EG", 
#ifdef STRID_COMMON_PLMN_60202
        STRID_COMMON_PLMN_60202, 
#else
        0,
#endif
    },
    { "60203", "Etisalat", 
#ifdef STRID_COMMON_PLMN_60203
        STRID_COMMON_PLMN_60203, 
#else
        0,
#endif
    },
    { "60204", "te", 
#ifdef STRID_COMMON_PLMN_60204
        STRID_COMMON_PLMN_60204, 
#else
        0,
#endif
    },
    { "60301", "Mobilis", 
#ifdef STRID_COMMON_PLMN_60301
        STRID_COMMON_PLMN_60301, 
#else
        0,
#endif
    },
    { "60302", "Djezzy", 
#ifdef STRID_COMMON_PLMN_60302
        STRID_COMMON_PLMN_60302, 
#else
        0,
#endif
    },
    { "60303", "Ooredoo", 
#ifdef STRID_COMMON_PLMN_60303
        STRID_COMMON_PLMN_60303, 
#else
        0,
#endif
    },
    { "60400", "Orange", 
#ifdef STRID_COMMON_PLMN_60400
        STRID_COMMON_PLMN_60400, 
#else
        0,
#endif
    },
    { "60401", "IAM", 
#ifdef STRID_COMMON_PLMN_60401
        STRID_COMMON_PLMN_60401, 
#else
        0,
#endif
    },
    { "60402", "inwi", 
#ifdef STRID_COMMON_PLMN_60402
        STRID_COMMON_PLMN_60402, 
#else
        0,
#endif
    },
    { "60501", "Orange", 
#ifdef STRID_COMMON_PLMN_60501
        STRID_COMMON_PLMN_60501, 
#else
        0,
#endif
    },
    { "60502", "TUNTEL", 
#ifdef STRID_COMMON_PLMN_60502
        STRID_COMMON_PLMN_60502, 
#else
        0,
#endif
    },
    { "60503", "TUNISIAN", 
#ifdef STRID_COMMON_PLMN_60503
        STRID_COMMON_PLMN_60503, 
#else
        0,
#endif
    },
    { "60600", "Libyana", 
#ifdef STRID_COMMON_PLMN_60600
        STRID_COMMON_PLMN_60600, 
#else
        0,
#endif
    },
    { "60601", "Al Madar", 
#ifdef STRID_COMMON_PLMN_60601
        STRID_COMMON_PLMN_60601, 
#else
        0,
#endif
    },
    { "60701", "GAMCEL", 
#ifdef STRID_COMMON_PLMN_60701
        STRID_COMMON_PLMN_60701, 
#else
        0,
#endif
    },
    { "60702", "AFRICELL", 
#ifdef STRID_COMMON_PLMN_60702
        STRID_COMMON_PLMN_60702, 
#else
        0,
#endif
    },
    { "60703", "GMCOMIUM", 
#ifdef STRID_COMMON_PLMN_60703
        STRID_COMMON_PLMN_60703, 
#else
        0,
#endif
    },
    { "60704", "QC", 
#ifdef STRID_COMMON_PLMN_60704
        STRID_COMMON_PLMN_60704, 
#else
        0,
#endif
    },
    { "60801", "ALIZE", 
#ifdef STRID_COMMON_PLMN_60801
        STRID_COMMON_PLMN_60801, 
#else
        0,
#endif
    },
    { "60802", "SENTEL", 
#ifdef STRID_COMMON_PLMN_60802
        STRID_COMMON_PLMN_60802, 
#else
        0,
#endif
    },
    { "60803", "Expresso", 
#ifdef STRID_COMMON_PLMN_60803
        STRID_COMMON_PLMN_60803, 
#else
        0,
#endif
    },
    { "60901", "MATTEL", 
#ifdef STRID_COMMON_PLMN_60901
        STRID_COMMON_PLMN_60901, 
#else
        0,
#endif
    },
    { "60902", "EMR", 
#ifdef STRID_COMMON_PLMN_60902
        STRID_COMMON_PLMN_60902, 
#else
        0,
#endif
    },
    { "60910", "MAURITEL", 
#ifdef STRID_COMMON_PLMN_60910
        STRID_COMMON_PLMN_60910, 
#else
        0,
#endif
    },
    { "61001", "MALITEL", 
#ifdef STRID_COMMON_PLMN_61001
        STRID_COMMON_PLMN_61001, 
#else
        0,
#endif
    },
    { "61002", "OML", 
#ifdef STRID_COMMON_PLMN_61002
        STRID_COMMON_PLMN_61002, 
#else
        0,
#endif
    },
    { "61101", "ORANGEGN", 
#ifdef STRID_COMMON_PLMN_61101
        STRID_COMMON_PLMN_61101, 
#else
        0,
#endif
    },
    { "61102", "LAGUI", 
#ifdef STRID_COMMON_PLMN_61102
        STRID_COMMON_PLMN_61102, 
#else
        0,
#endif
    },
    { "61104", "GNAreeba", 
#ifdef STRID_COMMON_PLMN_61104
        STRID_COMMON_PLMN_61104, 
#else
        0,
#endif
    },
    { "61105", "Cellcom", 
#ifdef STRID_COMMON_PLMN_61105
        STRID_COMMON_PLMN_61105, 
#else
        0,
#endif
    },
    { "61202", "MOOV", 
#ifdef STRID_COMMON_PLMN_61202
        STRID_COMMON_PLMN_61202, 
#else
        0,
#endif
    },
    { "61203", "Orange", 
#ifdef STRID_COMMON_PLMN_61203
        STRID_COMMON_PLMN_61203, 
#else
        0,
#endif
    },
    { "61204", "KoZ", 
#ifdef STRID_COMMON_PLMN_61204
        STRID_COMMON_PLMN_61204, 
#else
        0,
#endif
    },
    { "61205", "MTN CI", 
#ifdef STRID_COMMON_PLMN_61205
        STRID_COMMON_PLMN_61205, 
#else
        0,
#endif
    },
    { "61301", "ONATEL", 
#ifdef STRID_COMMON_PLMN_61301
        STRID_COMMON_PLMN_61301, 
#else
        0,
#endif
    },
    { "61302", "Orange", 
#ifdef STRID_COMMON_PLMN_61302
        STRID_COMMON_PLMN_61302, 
#else
        0,
#endif
    },
    { "61402", "Airtel", 
#ifdef STRID_COMMON_PLMN_61402
        STRID_COMMON_PLMN_61402, 
#else
        0,
#endif
    },
    { "61403", "ETISALAT", 
#ifdef STRID_COMMON_PLMN_61403
        STRID_COMMON_PLMN_61403, 
#else
        0,
#endif
    },
    { "61404", "Orange", 
#ifdef STRID_COMMON_PLMN_61404
        STRID_COMMON_PLMN_61404, 
#else
        0,
#endif
    },
    { "61501", "TGCELL", 
#ifdef STRID_COMMON_PLMN_61501
        STRID_COMMON_PLMN_61501, 
#else
        0,
#endif
    },
    { "61503", "ETISALAT", 
#ifdef STRID_COMMON_PLMN_61503
        STRID_COMMON_PLMN_61503, 
#else
        0,
#endif
    },
    { "61602", "ETISALAT", 
#ifdef STRID_COMMON_PLMN_61602
        STRID_COMMON_PLMN_61602, 
#else
        0,
#endif
    },
    { "61603", "MTNBENIN", 
#ifdef STRID_COMMON_PLMN_61603
        STRID_COMMON_PLMN_61603, 
#else
        0,
#endif
    },
    { "61604", "BBCOM", 
#ifdef STRID_COMMON_PLMN_61604
        STRID_COMMON_PLMN_61604, 
#else
        0,
#endif
    },
    { "61605", "GloBenin", 
#ifdef STRID_COMMON_PLMN_61605
        STRID_COMMON_PLMN_61605, 
#else
        0,
#endif
    },
    { "61701", "CELL +", 
#ifdef STRID_COMMON_PLMN_61701
        STRID_COMMON_PLMN_61701, 
#else
        0,
#endif
    },
    { "61703", "MTML", 
#ifdef STRID_COMMON_PLMN_61703
        STRID_COMMON_PLMN_61703, 
#else
        0,
#endif
    },
    { "61710", "EMTEL", 
#ifdef STRID_COMMON_PLMN_61710
        STRID_COMMON_PLMN_61710, 
#else
        0,
#endif
    },
    { "61801", "LoneStar", 
#ifdef STRID_COMMON_PLMN_61801
        STRID_COMMON_PLMN_61801, 
#else
        0,
#endif
    },
    { "61804", "Novafone", 
#ifdef STRID_COMMON_PLMN_61804
        STRID_COMMON_PLMN_61804, 
#else
        0,
#endif
    },
    { "61807", "Orange L", 
#ifdef STRID_COMMON_PLMN_61807
        STRID_COMMON_PLMN_61807, 
#else
        0,
#endif
    },
    { "61901", "Airtel", 
#ifdef STRID_COMMON_PLMN_61901
        STRID_COMMON_PLMN_61901, 
#else
        0,
#endif
    },
    { "61905", "Africell", 
#ifdef STRID_COMMON_PLMN_61905
        STRID_COMMON_PLMN_61905, 
#else
        0,
#endif
    },
    { "62001", "MTN", 
#ifdef STRID_COMMON_PLMN_62001
        STRID_COMMON_PLMN_62001, 
#else
        0,
#endif
    },
    { "62002", "Vodafone", 
#ifdef STRID_COMMON_PLMN_62002
        STRID_COMMON_PLMN_62002, 
#else
        0,
#endif
    },
    { "62003", "MIC", 
#ifdef STRID_COMMON_PLMN_62003
        STRID_COMMON_PLMN_62003, 
#else
        0,
#endif
    },
    { "62006", "Airtel", 
#ifdef STRID_COMMON_PLMN_62006
        STRID_COMMON_PLMN_62006, 
#else
        0,
#endif
    },
    { "62120", "Airtel", 
#ifdef STRID_COMMON_PLMN_62120
        STRID_COMMON_PLMN_62120, 
#else
        0,
#endif
    },
    { "62130", "MTN-NG", 
#ifdef STRID_COMMON_PLMN_62130
        STRID_COMMON_PLMN_62130, 
#else
        0,
#endif
    },
    { "62140", "ntel", 
#ifdef STRID_COMMON_PLMN_62140
        STRID_COMMON_PLMN_62140, 
#else
        0,
#endif
    },
    { "62150", "glo", 
#ifdef STRID_COMMON_PLMN_62150
        STRID_COMMON_PLMN_62150, 
#else
        0,
#endif
    },
    { "62160", "9mobile", 
#ifdef STRID_COMMON_PLMN_62160
        STRID_COMMON_PLMN_62160, 
#else
        0,
#endif
    },
    { "62201", "Airtel", 
#ifdef STRID_COMMON_PLMN_62201
        STRID_COMMON_PLMN_62201, 
#else
        0,
#endif
    },
    { "62203", "Tigo", 
#ifdef STRID_COMMON_PLMN_62203
        STRID_COMMON_PLMN_62203, 
#else
        0,
#endif
    },
    { "62301", "ETISALAT", 
#ifdef STRID_COMMON_PLMN_62301
        STRID_COMMON_PLMN_62301, 
#else
        0,
#endif
    },
    { "62302", "Telecel", 
#ifdef STRID_COMMON_PLMN_62302
        STRID_COMMON_PLMN_62302, 
#else
        0,
#endif
    },
    { "62303", "Orange", 
#ifdef STRID_COMMON_PLMN_62303
        STRID_COMMON_PLMN_62303, 
#else
        0,
#endif
    },
    { "62401", "MTN CAM", 
#ifdef STRID_COMMON_PLMN_62401
        STRID_COMMON_PLMN_62401, 
#else
        0,
#endif
    },
    { "62402", "Orange", 
#ifdef STRID_COMMON_PLMN_62402
        STRID_COMMON_PLMN_62402, 
#else
        0,
#endif
    },
    { "62404", "Nexttel", 
#ifdef STRID_COMMON_PLMN_62404
        STRID_COMMON_PLMN_62404, 
#else
        0,
#endif
    },
    { "62501", "CVMOVEL", 
#ifdef STRID_COMMON_PLMN_62501
        STRID_COMMON_PLMN_62501, 
#else
        0,
#endif
    },
    { "62502", "UnitelT+", 
#ifdef STRID_COMMON_PLMN_62502
        STRID_COMMON_PLMN_62502, 
#else
        0,
#endif
    },
    { "62601", "CSTmovel", 
#ifdef STRID_COMMON_PLMN_62601
        STRID_COMMON_PLMN_62601, 
#else
        0,
#endif
    },
    { "62602", "Unitel", 
#ifdef STRID_COMMON_PLMN_62602
        STRID_COMMON_PLMN_62602, 
#else
        0,
#endif
    },
    { "62703", "Muni-GQ", 
#ifdef STRID_COMMON_PLMN_62703
        STRID_COMMON_PLMN_62703, 
#else
        0,
#endif
    },
    { "62801", "LIBERTIS", 
#ifdef STRID_COMMON_PLMN_62801
        STRID_COMMON_PLMN_62801, 
#else
        0,
#endif
    },
    { "62802", "ETISALAT", 
#ifdef STRID_COMMON_PLMN_62802
        STRID_COMMON_PLMN_62802, 
#else
        0,
#endif
    },
    { "62803", "Airtel", 
#ifdef STRID_COMMON_PLMN_62803
        STRID_COMMON_PLMN_62803, 
#else
        0,
#endif
    },
    { "62804", "AZUR", 
#ifdef STRID_COMMON_PLMN_62804
        STRID_COMMON_PLMN_62804, 
#else
        0,
#endif
    },
    { "62901", "Airtel", 
#ifdef STRID_COMMON_PLMN_62901
        STRID_COMMON_PLMN_62901, 
#else
        0,
#endif
    },
    { "62910", "MTN-CG", 
#ifdef STRID_COMMON_PLMN_62910
        STRID_COMMON_PLMN_62910, 
#else
        0,
#endif
    },
    { "63001", "VODACOM", 
#ifdef STRID_COMMON_PLMN_63001
        STRID_COMMON_PLMN_63001, 
#else
        0,
#endif
    },
    { "63002", "Airtel", 
#ifdef STRID_COMMON_PLMN_63002
        STRID_COMMON_PLMN_63002, 
#else
        0,
#endif
    },
    { "63086", "Orange", 
#ifdef STRID_COMMON_PLMN_63086
        STRID_COMMON_PLMN_63086, 
#else
        0,
#endif
    },
    { "63089", "TIGO", 
#ifdef STRID_COMMON_PLMN_63089
        STRID_COMMON_PLMN_63089, 
#else
        0,
#endif
    },
    { "63090", "AFRICELL", 
#ifdef STRID_COMMON_PLMN_63090
        STRID_COMMON_PLMN_63090, 
#else
        0,
#endif
    },
    { "63102", "UNITEL", 
#ifdef STRID_COMMON_PLMN_63102
        STRID_COMMON_PLMN_63102, 
#else
        0,
#endif
    },
    { "63104", "AGOMV", 
#ifdef STRID_COMMON_PLMN_63104
        STRID_COMMON_PLMN_63104, 
#else
        0,
#endif
    },
    { "63202", "MTN", 
#ifdef STRID_COMMON_PLMN_63202
        STRID_COMMON_PLMN_63202, 
#else
        0,
#endif
    },
    { "63203", "Orange B", 
#ifdef STRID_COMMON_PLMN_63203
        STRID_COMMON_PLMN_63203, 
#else
        0,
#endif
    },
    { "63301", "C&W SEY", 
#ifdef STRID_COMMON_PLMN_63301
        STRID_COMMON_PLMN_63301, 
#else
        0,
#endif
    },
    { "63310", "Airtel", 
#ifdef STRID_COMMON_PLMN_63310
        STRID_COMMON_PLMN_63310, 
#else
        0,
#endif
    },
    { "63401", "Zain SD", 
#ifdef STRID_COMMON_PLMN_63401
        STRID_COMMON_PLMN_63401, 
#else
        0,
#endif
    },
    { "63402", "MTN", 
#ifdef STRID_COMMON_PLMN_63402
        STRID_COMMON_PLMN_63402, 
#else
        0,
#endif
    },
    { "63510", "MTN", 
#ifdef STRID_COMMON_PLMN_63510
        STRID_COMMON_PLMN_63510, 
#else
        0,
#endif
    },
    { "63513", "TIGO RW", 
#ifdef STRID_COMMON_PLMN_63513
        STRID_COMMON_PLMN_63513, 
#else
        0,
#endif
    },
    { "63514", "Airtel", 
#ifdef STRID_COMMON_PLMN_63514
        STRID_COMMON_PLMN_63514, 
#else
        0,
#endif
    },
    { "63601", "ET-MTN", 
#ifdef STRID_COMMON_PLMN_63601
        STRID_COMMON_PLMN_63601, 
#else
        0,
#endif
    },
    { "63701", "Tele", 
#ifdef STRID_COMMON_PLMN_63701
        STRID_COMMON_PLMN_63701, 
#else
        0,
#endif
    },
    { "63704", "SOMAFONE", 
#ifdef STRID_COMMON_PLMN_63704
        STRID_COMMON_PLMN_63704, 
#else
        0,
#endif
    },
    { "63730", "Golis", 
#ifdef STRID_COMMON_PLMN_63730
        STRID_COMMON_PLMN_63730, 
#else
        0,
#endif
    },
    { "63750", "Hormuud", 
#ifdef STRID_COMMON_PLMN_63750
        STRID_COMMON_PLMN_63750, 
#else
        0,
#endif
    },
    { "63801", "EVATIS", 
#ifdef STRID_COMMON_PLMN_63801
        STRID_COMMON_PLMN_63801, 
#else
        0,
#endif
    },
    { "63902", "SAF-COM", 
#ifdef STRID_COMMON_PLMN_63902
        STRID_COMMON_PLMN_63902, 
#else
        0,
#endif
    },
    { "63903", "Airtel", 
#ifdef STRID_COMMON_PLMN_63903
        STRID_COMMON_PLMN_63903, 
#else
        0,
#endif
    },
    { "63907", "GSMTKL", 
#ifdef STRID_COMMON_PLMN_63907
        STRID_COMMON_PLMN_63907, 
#else
        0,
#endif
    },
    { "63910", "FAIBA ", 
#ifdef STRID_COMMON_PLMN_63910
        STRID_COMMON_PLMN_63910, 
#else
        0,
#endif
    },
    { "64002", "TIGO", 
#ifdef STRID_COMMON_PLMN_64002
        STRID_COMMON_PLMN_64002, 
#else
        0,
#endif
    },
    { "64003", "ZANTEL", 
#ifdef STRID_COMMON_PLMN_64003
        STRID_COMMON_PLMN_64003, 
#else
        0,
#endif
    },
    { "64004", "VodaCom", 
#ifdef STRID_COMMON_PLMN_64004
        STRID_COMMON_PLMN_64004, 
#else
        0,
#endif
    },
    { "64005", "Airtel", 
#ifdef STRID_COMMON_PLMN_64005
        STRID_COMMON_PLMN_64005, 
#else
        0,
#endif
    },
    { "64101", "Airtel", 
#ifdef STRID_COMMON_PLMN_64101
        STRID_COMMON_PLMN_64101, 
#else
        0,
#endif
    },
    { "64110", "MTN-UG", 
#ifdef STRID_COMMON_PLMN_64110
        STRID_COMMON_PLMN_64110, 
#else
        0,
#endif
    },
    { "64111", "UTL", 
#ifdef STRID_COMMON_PLMN_64111
        STRID_COMMON_PLMN_64111, 
#else
        0,
#endif
    },
    { "64114", "AUL", 
#ifdef STRID_COMMON_PLMN_64114
        STRID_COMMON_PLMN_64114, 
#else
        0,
#endif
    },
    { "64118", "suretel", 
#ifdef STRID_COMMON_PLMN_64118
        STRID_COMMON_PLMN_64118, 
#else
        0,
#endif
    },
    { "64122", "Airtel", 
#ifdef STRID_COMMON_PLMN_64122
        STRID_COMMON_PLMN_64122, 
#else
        0,
#endif
    },
    { "64203", "ONATEL", 
#ifdef STRID_COMMON_PLMN_64203
        STRID_COMMON_PLMN_64203, 
#else
        0,
#endif
    },
    { "64207", "SMART", 
#ifdef STRID_COMMON_PLMN_64207
        STRID_COMMON_PLMN_64207, 
#else
        0,
#endif
    },
    { "64282", "BDITL", 
#ifdef STRID_COMMON_PLMN_64282
        STRID_COMMON_PLMN_64282, 
#else
        0,
#endif
    },
    { "64301", "mCel", 
#ifdef STRID_COMMON_PLMN_64301
        STRID_COMMON_PLMN_64301, 
#else
        0,
#endif
    },
    { "64303", "MOVITEL", 
#ifdef STRID_COMMON_PLMN_64303
        STRID_COMMON_PLMN_64303, 
#else
        0,
#endif
    },
    { "64304", "VodaCom", 
#ifdef STRID_COMMON_PLMN_64304
        STRID_COMMON_PLMN_64304, 
#else
        0,
#endif
    },
    { "64501", "Airtel", 
#ifdef STRID_COMMON_PLMN_64501
        STRID_COMMON_PLMN_64501, 
#else
        0,
#endif
    },
    { "64502", "MTN ZM", 
#ifdef STRID_COMMON_PLMN_64502
        STRID_COMMON_PLMN_64502, 
#else
        0,
#endif
    },
    { "64601", "Airtel", 
#ifdef STRID_COMMON_PLMN_64601
        STRID_COMMON_PLMN_64601, 
#else
        0,
#endif
    },
    { "64602", "OrangeMG", 
#ifdef STRID_COMMON_PLMN_64602
        STRID_COMMON_PLMN_64602, 
#else
        0,
#endif
    },
    { "64604", "TELMA", 
#ifdef STRID_COMMON_PLMN_64604
        STRID_COMMON_PLMN_64604, 
#else
        0,
#endif
    },
    { "64605", "Bip", 
#ifdef STRID_COMMON_PLMN_64605
        STRID_COMMON_PLMN_64605, 
#else
        0,
#endif
    },
    { "64700", "Orange", 
#ifdef STRID_COMMON_PLMN_64700
        STRID_COMMON_PLMN_64700, 
#else
        0,
#endif
    },
    { "64701", "MMOBILE", 
#ifdef STRID_COMMON_PLMN_64701
        STRID_COMMON_PLMN_64701, 
#else
        0,
#endif
    },
    { "64702", "ONLY", 
#ifdef STRID_COMMON_PLMN_64702
        STRID_COMMON_PLMN_64702, 
#else
        0,
#endif
    },
    { "64703", "FREE RE", 
#ifdef STRID_COMMON_PLMN_64703
        STRID_COMMON_PLMN_64703, 
#else
        0,
#endif
    },
    { "64801", "NETONE", 
#ifdef STRID_COMMON_PLMN_64801
        STRID_COMMON_PLMN_64801, 
#else
        0,
#endif
    },
    { "64803", "TELECEL", 
#ifdef STRID_COMMON_PLMN_64803
        STRID_COMMON_PLMN_64803, 
#else
        0,
#endif
    },
    { "64804", "ECONET", 
#ifdef STRID_COMMON_PLMN_64804
        STRID_COMMON_PLMN_64804, 
#else
        0,
#endif
    },
    { "64901", "MTCNAM", 
#ifdef STRID_COMMON_PLMN_64901
        STRID_COMMON_PLMN_64901, 
#else
        0,
#endif
    },
    { "64903", "tnmobile", 
#ifdef STRID_COMMON_PLMN_64903
        STRID_COMMON_PLMN_64903, 
#else
        0,
#endif
    },
    { "65001", "TNM", 
#ifdef STRID_COMMON_PLMN_65001
        STRID_COMMON_PLMN_65001, 
#else
        0,
#endif
    },
    { "65010", "Airtel", 
#ifdef STRID_COMMON_PLMN_65010
        STRID_COMMON_PLMN_65010, 
#else
        0,
#endif
    },
    { "65101", "VODACOM", 
#ifdef STRID_COMMON_PLMN_65101
        STRID_COMMON_PLMN_65101, 
#else
        0,
#endif
    },
    { "65102", "ETL", 
#ifdef STRID_COMMON_PLMN_65102
        STRID_COMMON_PLMN_65102, 
#else
        0,
#endif
    },
    { "65201", "MASCOM", 
#ifdef STRID_COMMON_PLMN_65201
        STRID_COMMON_PLMN_65201, 
#else
        0,
#endif
    },
    { "65202", "Orange", 
#ifdef STRID_COMMON_PLMN_65202
        STRID_COMMON_PLMN_65202, 
#else
        0,
#endif
    },
    { "65204", "beMOBILE", 
#ifdef STRID_COMMON_PLMN_65204
        STRID_COMMON_PLMN_65204, 
#else
        0,
#endif
    },
    { "65302", "Swazi Mo", 
#ifdef STRID_COMMON_PLMN_65302
        STRID_COMMON_PLMN_65302, 
#else
        0,
#endif
    },
    { "65310", "SwaziMTN", 
#ifdef STRID_COMMON_PLMN_65310
        STRID_COMMON_PLMN_65310, 
#else
        0,
#endif
    },
    { "65401", "HURI", 
#ifdef STRID_COMMON_PLMN_65401
        STRID_COMMON_PLMN_65401, 
#else
        0,
#endif
    },
    { "65402", "COMTM", 
#ifdef STRID_COMMON_PLMN_65402
        STRID_COMMON_PLMN_65402, 
#else
        0,
#endif
    },
    { "65501", "Vodacom", 
#ifdef STRID_COMMON_PLMN_65501
        STRID_COMMON_PLMN_65501, 
#else
        0,
#endif
    },
    { "65502", "TelkomSA", 
#ifdef STRID_COMMON_PLMN_65502
        STRID_COMMON_PLMN_65502, 
#else
        0,
#endif
    },
    { "65507", "Cell C", 
#ifdef STRID_COMMON_PLMN_65507
        STRID_COMMON_PLMN_65507, 
#else
        0,
#endif
    },
    { "65510", "MTN", 
#ifdef STRID_COMMON_PLMN_65510
        STRID_COMMON_PLMN_65510, 
#else
        0,
#endif
    },
    { "65801", "Sure", 
#ifdef STRID_COMMON_PLMN_65801
        STRID_COMMON_PLMN_65801, 
#else
        0,
#endif
    },
    { "65902", "MTN", 
#ifdef STRID_COMMON_PLMN_65902
        STRID_COMMON_PLMN_65902, 
#else
        0,
#endif
    },
    { "65904", "Vivacell", 
#ifdef STRID_COMMON_PLMN_65904
        STRID_COMMON_PLMN_65904, 
#else
        0,
#endif
    },
    { "65906", "ZAIN SS", 
#ifdef STRID_COMMON_PLMN_65906
        STRID_COMMON_PLMN_65906, 
#else
        0,
#endif
    },
    { "70267", "BTL", 
#ifdef STRID_COMMON_PLMN_70267
        STRID_COMMON_PLMN_70267, 
#else
        0,
#endif
    },
    { "70269", "Smart", 
#ifdef STRID_COMMON_PLMN_70269
        STRID_COMMON_PLMN_70269, 
#else
        0,
#endif
    },
    { "70401", "CLAROGTM", 
#ifdef STRID_COMMON_PLMN_70401
        STRID_COMMON_PLMN_70401, 
#else
        0,
#endif
    },
    { "70402", "TIGO", 
#ifdef STRID_COMMON_PLMN_70402
        STRID_COMMON_PLMN_70402, 
#else
        0,
#endif
    },
    { "70403", "Movistar", 
#ifdef STRID_COMMON_PLMN_70403
        STRID_COMMON_PLMN_70403, 
#else
        0,
#endif
    },
    { "70601", "ClaroSLV", 
#ifdef STRID_COMMON_PLMN_70601
        STRID_COMMON_PLMN_70601, 
#else
        0,
#endif
    },
    { "70602", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_70602
        STRID_COMMON_PLMN_70602, 
#else
        0,
#endif
    },
    { "70603", "Tigo", 
#ifdef STRID_COMMON_PLMN_70603
        STRID_COMMON_PLMN_70603, 
#else
        0,
#endif
    },
    { "70604", "Movistar", 
#ifdef STRID_COMMON_PLMN_70604
        STRID_COMMON_PLMN_70604, 
#else
        0,
#endif
    },
    { "708001", "ClaroHND", 
#ifdef STRID_COMMON_PLMN_708001
        STRID_COMMON_PLMN_708001, 
#else
        0,
#endif
    },
    { "70802", "TIGO", 
#ifdef STRID_COMMON_PLMN_70802
        STRID_COMMON_PLMN_70802, 
#else
        0,
#endif
    },
    { "708030", "HT - 200", 
#ifdef STRID_COMMON_PLMN_708030
        STRID_COMMON_PLMN_708030, 
#else
        0,
#endif
    },
    { "71021", "ClaroNIC", 
#ifdef STRID_COMMON_PLMN_71021
        STRID_COMMON_PLMN_71021, 
#else
        0,
#endif
    },
    { "710300", "Movistar", 
#ifdef STRID_COMMON_PLMN_710300
        STRID_COMMON_PLMN_710300, 
#else
        0,
#endif
    },
    { "71073", "ClaroNIC", 
#ifdef STRID_COMMON_PLMN_71073
        STRID_COMMON_PLMN_71073, 
#else
        0,
#endif
    },
    { "71201", "I.C.E.", 
#ifdef STRID_COMMON_PLMN_71201
        STRID_COMMON_PLMN_71201, 
#else
        0,
#endif
    },
    { "71202", "I.C.E.", 
#ifdef STRID_COMMON_PLMN_71202
        STRID_COMMON_PLMN_71202, 
#else
        0,
#endif
    },
    { "71203", "CLARO CR", 
#ifdef STRID_COMMON_PLMN_71203
        STRID_COMMON_PLMN_71203, 
#else
        0,
#endif
    },
    { "71204", "Movistar", 
#ifdef STRID_COMMON_PLMN_71204
        STRID_COMMON_PLMN_71204, 
#else
        0,
#endif
    },
    { "71401", "PANCW", 
#ifdef STRID_COMMON_PLMN_71401
        STRID_COMMON_PLMN_71401, 
#else
        0,
#endif
    },
    { "714020", "Movistar", 
#ifdef STRID_COMMON_PLMN_714020
        STRID_COMMON_PLMN_714020, 
#else
        0,
#endif
    },
    { "71403", "CLARO PA", 
#ifdef STRID_COMMON_PLMN_71403
        STRID_COMMON_PLMN_71403, 
#else
        0,
#endif
    },
    { "71404", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_71404
        STRID_COMMON_PLMN_71404, 
#else
        0,
#endif
    },
    { "71606", "MOVISTAR", 
#ifdef STRID_COMMON_PLMN_71606
        STRID_COMMON_PLMN_71606, 
#else
        0,
#endif
    },
    { "71610", "ClaroPER", 
#ifdef STRID_COMMON_PLMN_71610
        STRID_COMMON_PLMN_71610, 
#else
        0,
#endif
    },
    { "71615", "VTP", 
#ifdef STRID_COMMON_PLMN_71615
        STRID_COMMON_PLMN_71615, 
#else
        0,
#endif
    },
    { "71617", "Entel", 
#ifdef STRID_COMMON_PLMN_71617
        STRID_COMMON_PLMN_71617, 
#else
        0,
#endif
    },
    { "722010", "TEFMVNO", 
#ifdef STRID_COMMON_PLMN_722010
        STRID_COMMON_PLMN_722010, 
#else
        0,
#endif
    },
    { "72207", "Movistar", 
#ifdef STRID_COMMON_PLMN_72207
        STRID_COMMON_PLMN_72207, 
#else
        0,
#endif
    },
    { "722310", "CLARO AR", 
#ifdef STRID_COMMON_PLMN_722310
        STRID_COMMON_PLMN_722310, 
#else
        0,
#endif
    },
    { "72234", "AR TP", 
#ifdef STRID_COMMON_PLMN_72234
        STRID_COMMON_PLMN_72234, 
#else
        0,
#endif
    },
    { "72236", "AR TP", 
#ifdef STRID_COMMON_PLMN_72236
        STRID_COMMON_PLMN_72236, 
#else
        0,
#endif
    },
    { "72402", "TIM", 
#ifdef STRID_COMMON_PLMN_72402
        STRID_COMMON_PLMN_72402, 
#else
        0,
#endif
    },
    { "72403", "TIM", 
#ifdef STRID_COMMON_PLMN_72403
        STRID_COMMON_PLMN_72403, 
#else
        0,
#endif
    },
    { "72404", "TIM", 
#ifdef STRID_COMMON_PLMN_72404
        STRID_COMMON_PLMN_72404, 
#else
        0,
#endif
    },
    { "72405", "Claro", 
#ifdef STRID_COMMON_PLMN_72405
        STRID_COMMON_PLMN_72405, 
#else
        0,
#endif
    },
    { "72406", "VIVO", 
#ifdef STRID_COMMON_PLMN_72406
        STRID_COMMON_PLMN_72406, 
#else
        0,
#endif
    },
    { "72410", "VIVO", 
#ifdef STRID_COMMON_PLMN_72410
        STRID_COMMON_PLMN_72410, 
#else
        0,
#endif
    },
    { "72411", "VIVO", 
#ifdef STRID_COMMON_PLMN_72411
        STRID_COMMON_PLMN_72411, 
#else
        0,
#endif
    },
    { "72415", "SCTL", 
#ifdef STRID_COMMON_PLMN_72415
        STRID_COMMON_PLMN_72415, 
#else
        0,
#endif
    },
    { "72416", "Oi", 
#ifdef STRID_COMMON_PLMN_72416
        STRID_COMMON_PLMN_72416, 
#else
        0,
#endif
    },
    { "72423", "VIVO", 
#ifdef STRID_COMMON_PLMN_72423
        STRID_COMMON_PLMN_72423, 
#else
        0,
#endif
    },
    { "72424", "Oi", 
#ifdef STRID_COMMON_PLMN_72424
        STRID_COMMON_PLMN_72424, 
#else
        0,
#endif
    },
    { "72431", "Oi", 
#ifdef STRID_COMMON_PLMN_72431
        STRID_COMMON_PLMN_72431, 
#else
        0,
#endif
    },
    { "72432", "Algar", 
#ifdef STRID_COMMON_PLMN_72432
        STRID_COMMON_PLMN_72432, 
#else
        0,
#endif
    },
    { "72433", "Algar", 
#ifdef STRID_COMMON_PLMN_72433
        STRID_COMMON_PLMN_72433, 
#else
        0,
#endif
    },
    { "72434", "Algar", 
#ifdef STRID_COMMON_PLMN_72434
        STRID_COMMON_PLMN_72434, 
#else
        0,
#endif
    },
    { "72439", "Nextel", 
#ifdef STRID_COMMON_PLMN_72439
        STRID_COMMON_PLMN_72439, 
#else
        0,
#endif
    },
    { "73001", "ENTEL", 
#ifdef STRID_COMMON_PLMN_73001
        STRID_COMMON_PLMN_73001, 
#else
        0,
#endif
    },
    { "73002", "Movistar", 
#ifdef STRID_COMMON_PLMN_73002
        STRID_COMMON_PLMN_73002, 
#else
        0,
#endif
    },
    { "73003", "ClaroCHL", 
#ifdef STRID_COMMON_PLMN_73003
        STRID_COMMON_PLMN_73003, 
#else
        0,
#endif
    },
    { "73007", "Movistar", 
#ifdef STRID_COMMON_PLMN_73007
        STRID_COMMON_PLMN_73007, 
#else
        0,
#endif
    },
    { "73009", "WOM", 
#ifdef STRID_COMMON_PLMN_73009
        STRID_COMMON_PLMN_73009, 
#else
        0,
#endif
    },
    { "73010", "ENTEL", 
#ifdef STRID_COMMON_PLMN_73010
        STRID_COMMON_PLMN_73010, 
#else
        0,
#endif
    },
    { "732101", "Claro", 
#ifdef STRID_COMMON_PLMN_732101
        STRID_COMMON_PLMN_732101, 
#else
        0,
#endif
    },
    { "732103", "TIGO", 
#ifdef STRID_COMMON_PLMN_732103
        STRID_COMMON_PLMN_732103, 
#else
        0,
#endif
    },
    { "732111", "TIGO", 
#ifdef STRID_COMMON_PLMN_732111
        STRID_COMMON_PLMN_732111, 
#else
        0,
#endif
    },
    { "732123", "Movistar", 
#ifdef STRID_COMMON_PLMN_732123
        STRID_COMMON_PLMN_732123, 
#else
        0,
#endif
    },
    { "732130", "Avantel", 
#ifdef STRID_COMMON_PLMN_732130
        STRID_COMMON_PLMN_732130, 
#else
        0,
#endif
    },
    { "732187", "ETB", 
#ifdef STRID_COMMON_PLMN_732187
        STRID_COMMON_PLMN_732187, 
#else
        0,
#endif
    },
    { "73402", "DIGITEL", 
#ifdef STRID_COMMON_PLMN_73402
        STRID_COMMON_PLMN_73402, 
#else
        0,
#endif
    },
    { "73404", "Movistar", 
#ifdef STRID_COMMON_PLMN_73404
        STRID_COMMON_PLMN_73404, 
#else
        0,
#endif
    },
    { "73406", "Movilnet", 
#ifdef STRID_COMMON_PLMN_73406
        STRID_COMMON_PLMN_73406, 
#else
        0,
#endif
    },
    { "73601", "VIVA", 
#ifdef STRID_COMMON_PLMN_73601
        STRID_COMMON_PLMN_73601, 
#else
        0,
#endif
    },
    { "73602", "EMOVIL", 
#ifdef STRID_COMMON_PLMN_73602
        STRID_COMMON_PLMN_73602, 
#else
        0,
#endif
    },
    { "73603", "TIGO", 
#ifdef STRID_COMMON_PLMN_73603
        STRID_COMMON_PLMN_73603, 
#else
        0,
#endif
    },
    { "738002", "GTT+", 
#ifdef STRID_COMMON_PLMN_738002
        STRID_COMMON_PLMN_738002, 
#else
        0,
#endif
    },
    { "73801", "Digicel", 
#ifdef STRID_COMMON_PLMN_73801
        STRID_COMMON_PLMN_73801, 
#else
        0,
#endif
    },
    { "74000", "Movistar", 
#ifdef STRID_COMMON_PLMN_74000
        STRID_COMMON_PLMN_74000, 
#else
        0,
#endif
    },
    { "74001", "CLARO", 
#ifdef STRID_COMMON_PLMN_74001
        STRID_COMMON_PLMN_74001, 
#else
        0,
#endif
    },
    { "74002", "CNT", 
#ifdef STRID_COMMON_PLMN_74002
        STRID_COMMON_PLMN_74002, 
#else
        0,
#endif
    },
    { "74401", "HPGYSA", 
#ifdef STRID_COMMON_PLMN_74401
        STRID_COMMON_PLMN_74401, 
#else
        0,
#endif
    },
    { "74402", "CLARO PY", 
#ifdef STRID_COMMON_PLMN_74402
        STRID_COMMON_PLMN_74402, 
#else
        0,
#endif
    },
    { "74404", "TIGO", 
#ifdef STRID_COMMON_PLMN_74404
        STRID_COMMON_PLMN_74404, 
#else
        0,
#endif
    },
    { "74405", "Personal", 
#ifdef STRID_COMMON_PLMN_74405
        STRID_COMMON_PLMN_74405, 
#else
        0,
#endif
    },
    { "74602", "TeleG", 
#ifdef STRID_COMMON_PLMN_74602
        STRID_COMMON_PLMN_74602, 
#else
        0,
#endif
    },
    { "74603", "DIGICEL", 
#ifdef STRID_COMMON_PLMN_74603
        STRID_COMMON_PLMN_74603, 
#else
        0,
#endif
    },
    { "74801", "Antel", 
#ifdef STRID_COMMON_PLMN_74801
        STRID_COMMON_PLMN_74801, 
#else
        0,
#endif
    },
    { "74807", "Movistar", 
#ifdef STRID_COMMON_PLMN_74807
        STRID_COMMON_PLMN_74807, 
#else
        0,
#endif
    },
    { "74810", "CLARO UY", 
#ifdef STRID_COMMON_PLMN_74810
        STRID_COMMON_PLMN_74810, 
#else
        0,
#endif
    },
    { "750001", "Sure", 
#ifdef STRID_COMMON_PLMN_750001
        STRID_COMMON_PLMN_750001, 
#else
        0,
#endif
    },
    { "90111", "INX", 
#ifdef STRID_COMMON_PLMN_90111
        STRID_COMMON_PLMN_90111, 
#else
        0,
#endif
    },
    { "90112", "TelenorM", 
#ifdef STRID_COMMON_PLMN_90112
        STRID_COMMON_PLMN_90112, 
#else
        0,
#endif
    },
    { "90114", "AeroMob", 
#ifdef STRID_COMMON_PLMN_90114
        STRID_COMMON_PLMN_90114, 
#else
        0,
#endif
    },
    { "90115", "OnAir", 
#ifdef STRID_COMMON_PLMN_90115
        STRID_COMMON_PLMN_90115, 
#else
        0,
#endif
    },
    { "90126", "TIM@sea", 
#ifdef STRID_COMMON_PLMN_90126
        STRID_COMMON_PLMN_90126, 
#else
        0,
#endif
    },
    { "90145", "AISatSea", 
#ifdef STRID_COMMON_PLMN_90145
        STRID_COMMON_PLMN_90145, 
#else
        0,
#endif
    },
    { "90150", "EML", 
#ifdef STRID_COMMON_PLMN_90150
        STRID_COMMON_PLMN_90150, 
#else
        0,
#endif
    },
};

const char* GetPLMNDisplayName (const char* plmn)
{
    int count = sizeof (_sorted_plmn_names)/sizeof (SORTED_PLMN_NAME);
    int idx_s = 0;
    int idx_e = count - 1;
    SORTED_PLMN_NAME* plmn_name = NULL;

    while (idx_e >= idx_s) {
        int idx = (idx_s + idx_e)/2;
        int cmp;

        plmn_name = _sorted_plmn_names + idx;
        cmp = strcmp (plmn, plmn_name->plmn);
        if (cmp == 0)
            break;

        plmn_name = NULL;
        if (cmp > 0) {
            idx_s = idx + 1;
        }
        else {
            idx_e = idx - 1;
        }
    }
    
    if (plmn_name) {
        if (plmn_name->l10n_id)
            return GetTextRes (plmn_name->l10n_id);
        else
            return plmn_name->name;
    }

    return NULL;
}

NAMESPACE_END
