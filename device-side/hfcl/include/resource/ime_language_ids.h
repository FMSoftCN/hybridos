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

#ifndef _IME_LANGUAGE_IDS_H_
#define _IME_LANGUAGE_IDS_H_

/*----------------------------------------------------------------------------
 *  Primary language identifiers
 *----------------------------------------------------------------------------*/
#define IMELANGIDMASK         (0x00FF)
#define IMELANGIDNone         (0x0000)

#define IMELANGIDArabic       (0x0001)
#define IMELANGIDBulgarian    (0x0002)
#define IMELANGIDCatalan      (0x0003)
#define IMELANGIDChinese      (0x0004)
#define IMELANGIDCzech        (0x0005)
#define IMELANGIDDanish       (0x0006)
#define IMELANGIDGerman       (0x0007)
#define IMELANGIDGreek        (0x0008)
#define IMELANGIDEnglish      (0x0009)
#define IMELANGIDSpanish      (0x000A)
#define IMELANGIDFinnish      (0x000B)
#define IMELANGIDFrench       (0x000C)
#define IMELANGIDHebrew       (0x000D)
#define IMELANGIDHungarian    (0x000E)
#define IMELANGIDIcelandic    (0x000F)
#define IMELANGIDItalian      (0x0010)
#define IMELANGIDJapanese     (0x0011)
#define IMELANGIDKorean       (0x0012)
#define IMELANGIDDutch        (0x0013)
#define IMELANGIDNorwegian    (0x0014)
#define IMELANGIDPolish       (0x0015)
#define IMELANGIDPortuguese   (0x0016)
#define IMELANGIDRhaetoRomance (0x0017)
#define IMELANGIDRomanian     (0x0018)
#define IMELANGIDRussian      (0x0019)
#define IMELANGIDSerboCroatian (0x001A)
#define IMELANGIDSlovak       (0x001B)
#define IMELANGIDAlbanian     (0x001C)
#define IMELANGIDSwedish      (0x001D)
#define IMELANGIDThai         (0x001E)
#define IMELANGIDTurkish      (0x001F)
#define IMELANGIDUrdu         (0x0020)
#define IMELANGIDIndonesian   (0x0021)
#define IMELANGIDUkrainian    (0x0022)
#define IMELANGIDBelarusian   (0x0023)
#define IMELANGIDSlovenian    (0x0024)
#define IMELANGIDEstonian     (0x0025)
#define IMELANGIDLatvian      (0x0026)
#define IMELANGIDLithuanian   (0x0027)
#define IMELANGIDMaori        (0x0028)
#define IMELANGIDFarsi        (0x0029)
#define IMELANGIDVietnamese   (0x002A)
#define IMELANGIDLao          (0x002B)
#define IMELANGIDKhmer        (0x002C)
#define IMELANGIDBasque       (0x002D)
#define IMELANGIDSorbian      (0x002E)
#define IMELANGIDMacedonian   (0x002F)
#define IMELANGIDSutu         (0x0030)
#define IMELANGIDTsonga       (0x0031)
#define IMELANGIDTswana       (0x0032)
#define IMELANGIDVenda        (0x0033)
#define IMELANGIDXhosa        (0x0034)
#define IMELANGIDZulu         (0x0035)
#define IMELANGIDAfrikaans    (0x0036)
#define IMELANGIDFaeroese     (0x0038)
#define IMELANGIDHindi        (0x0039)
#define IMELANGIDMaltese      (0x003A)
#define IMELANGIDSami         (0x003B)
#define IMELANGIDScotsGaelic  (0x003C)
#define IMELANGIDMalay        (0x003E)
#define IMELANGIDSwahili      (0x0041)
#define IMELANGIDAfar         (0x0042)
#define IMELANGIDAbkhazian    (0x0043)
#define IMELANGIDAmharic      (0x0044)
#define IMELANGIDAssamese     (0x0045)
#define IMELANGIDAymara       (0x0046)
#define IMELANGIDAzerbaijani  (0x0047)
#define IMELANGIDBashkir      (0x0048)
#define IMELANGIDBihari       (0x0049)
#define IMELANGIDBislama      (0x004a)
#define IMELANGIDBengali      (0x004b)
#define IMELANGIDTibetan      (0x004c)
#define IMELANGIDBreton       (0x004d)
#define IMELANGIDCorsican     (0x004e)
#define IMELANGIDWelsh        (0x004f)
#define IMELANGIDBhutani      (0x0050)
#define IMELANGIDEsperanto    (0x0051)
#define IMELANGIDFiji         (0x0052)
#define IMELANGIDFrisian      (0x0053)
#define IMELANGIDIrish        (0x0054)
#define IMELANGIDGalician     (0x0055)
#define IMELANGIDGuarani      (0x0056)
#define IMELANGIDGujarati     (0x0057)
#define IMELANGIDHausa        (0x0058)
#define IMELANGIDCroatian     (0x0059)
#define IMELANGIDArmenian     (0x005a)
#define IMELANGIDInterlingua  (0x005b)
#define IMELANGIDInterlingue  (0x005c)
#define IMELANGIDInupiak      (0x005d)
#define IMELANGIDInuktitut    (0x005e)
#define IMELANGIDJavanese     (0x005f)
#define IMELANGIDGeorgian     (0x0060)
#define IMELANGIDKazakh       (0x0061)
#define IMELANGIDGreenlandic  (0x0062)
#define IMELANGIDKannada      (0x0063)
#define IMELANGIDKashmiri     (0x0064)
#define IMELANGIDKurdish      (0x0065)
#define IMELANGIDKirghiz      (0x0066)
#define IMELANGIDLatin        (0x0067)
#define IMELANGIDLingala      (0x0068)
#define IMELANGIDMalagasy     (0x0069)
#define IMELANGIDMalayalam    (0x006a)
#define IMELANGIDMongolian    (0x006b)
#define IMELANGIDMoldavian    (0x006c)
#define IMELANGIDMarathi      (0x006d)
#define IMELANGIDBurmese      (0x006e)
#define IMELANGIDNauru        (0x006f)
#define IMELANGIDNepali       (0x0070)
#define IMELANGIDOccitan      (0x0071)
#define IMELANGIDOromo        (0x0072)
#define IMELANGIDOriya        (0x0073)
#define IMELANGIDPunjabi      (0x0074)
#define IMELANGIDPashto       (0x0075)
#define IMELANGIDQuechua      (0x0076)
#define IMELANGIDKirundi      (0x0077)
#define IMELANGIDKiyarwanda   (0x0078)
#define IMELANGIDSanskrit     (0x0079)
#define IMELANGIDSindhi       (0x007a)
#define IMELANGIDSangho       (0x007b)
#define IMELANGIDSinhala      (0x007c)
#define IMELANGIDSamoan       (0x007d)
#define IMELANGIDShona        (0x007e)
#define IMELANGIDSomali       (0x007f)
#define IMELANGIDSerbian      (0x0080)
#define IMELANGIDSiswati      (0x0081)
#define IMELANGIDSesotho      (0x0082)
#define IMELANGIDSudanese     (0x0083)
#define IMELANGIDTamil        (0x0084)
#define IMELANGIDTelugu       (0x0085)
#define IMELANGIDTajik        (0x0086)
#define IMELANGIDTigrinya     (0x0087)
#define IMELANGIDTurkmen      (0x0088)
#define IMELANGIDTagalog      (0x0089)
#define IMELANGIDSetswana     (0x008a)
#define IMELANGIDTonga        (0x008b)
#define IMELANGIDTatar        (0x008c)
#define IMELANGIDTwi          (0x008d)
#define IMELANGIDUyghur       (0x008e)
#define IMELANGIDUzbek        (0x008f)
#define IMELANGIDVolapuk      (0x0090)
#define IMELANGIDWolof        (0x0091)
#define IMELANGIDYiddish      (0x0092)
#define IMELANGIDYoruba       (0x0093)
#define IMELANGIDZhuang       (0x0094)
#define IMELANGIDIgbo         (0x0095)
#define IMELANGIDTamazight    (0x0096)
#define IMELANGIDBosnian      (0x0097)
#define IMELANGIDDari         (0x0098)
#define IMELANGIDSundanese    (0x0099)
#define IMELANGIDCebuano      (0x009a)
#define IMELANGIDNavajo       (0x009b)
#define IMELANGIDHawaiian     (0x009c)

/* VincentWei: the following languages are not defined by T9 IME */
#define IMELANGIDPersian      (0x009d)
#define IMELANGIDPushto       (0x009e)

/* language ID from 0x00D0 through 00DF are hybrid languages */
#define IMELANGIDHinglish	  (0x00D0)
#define IMELANGIDSpanglish	  (0x00D1)
/* language ID 0x00E0 up are reserved to match Chinese XT9 lang ids */
#define IMELANGIDNull         (0x00FF)

/*----------------------------------------------------------------------------
 *  Secondary language identifiers
 *----------------------------------------------------------------------------*/
#define IMELANGSIDMASK                         (0xFF00)
#define IMELANGSIDNone                         (0x0000)
#define IMELANGSIDDEFAULT                      (0x0100)

#define IMELANGSIDChineseTraditional           (0x0100)
#define IMELANGSIDChineseSimplified            (0x0200)

#define IMELANGSIDChineseTraditionalPTI        (0x1100)
#define IMELANGSIDChineseSimplifiedPTI         (0x1200)

#define IMELANGSIDKoreanSyllable               (0x0100)
#define IMELANGSIDKoreanChun                   (0x0300)

/*----------------------------------------------------------------------------
 *  Primary identifiers for special languages.
 *----------------------------------------------------------------------------*/
#define IMELANGIDChineseTraditional           (IMELANGIDChinese | IMELANGSIDChineseTraditional)
#define IMELANGIDChineseSimplified            (IMELANGIDChinese | IMELANGSIDChineseSimplified)

#define IMELANGIDKoreanSyllable               (IMELANGIDKorean | IMELANGSIDKoreanSyllable)
#define IMELANGIDKoreanChun                   (IMELANGIDKorean | IMELANGSIDKoreanChun)

#endif /* _IME_LANGUAGE_IDS_H_ */

