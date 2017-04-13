#ifndef _COM_DIAG_GRANDOTE_ISO3166_H_
#define _COM_DIAG_GRANDOTE_ISO3166_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Grandote library.
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    As a special exception, if other files instantiate templates or
    use macros or inline functions from this file, or you compile
    this file and link it with other works to produce a work based on
    this file, this file does not by itself cause the resulting work
    to be covered by the GNU Lesser General Public License. However
    the source code for this file must still be made available in
    accordance with the GNU Lesser General Public License.

    This exception does not invalidate any other reasons why a work
    based on this file might be covered by the GNU Lesser General
    Public License.

    Alternative commercial licensing terms are available from the copyright
    holder. Contact Digital Aggregates Corporation for more information.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
    Boston, MA 02111-1307 USA, or http://www.gnu.org/copyleft/lesser.txt.



******************************************************************************/


/**
 *  @file
 *
 *  Declares the Iso3166 class.
 *
 *  @see    Iso3166
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/Object.h"
#include "com/diag/grandote/Output.h"


#include "com/diag/grandote/Begin.h"

/**
 *  Contains a table of country names, abbreviations, and country numbers
 *  conformant with the ISO 3166-1:1997 standard. Data provided
 *  via the web by the RIPE Network Coordination Centre from raw data
 *  from the ISO 3166 Maintenance Agency. Data last updated by RIPE on
 *  Wednesday 10 October 2003, 16:40 CET.
 *
 *  @see    ISO 3166/MA, <I>Codes for the representation of names
 *          of countries and their subdivisions</I>, ISO3166-1:1997,
 *          ISO 3166 Maintenance Agency, International Organization for
 *          Standardization, Geneva, Switzerland, 2003-09-05,
 *          http://www.iso.ch/iso/en/prods-services/iso3166ma/index.html
 *
 *  @see    RIPE, <I>Some codes from ISO3166-1</I>, RIPE Network
 *          Coordination Centre, Re'seaux IP Europe'ens, Amsterdam, The
 *          Netherlands, 2003-10-10,
 *          ftp://ftp.ripe.net/iso3166-countrycodes.txt
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Iso3166 : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Iso3166();

    /**
     *  Destructor.
     */
    virtual ~Iso3166();

    /**
     *  This describes a single entry in the country-code table.
     */
    struct Entry {

        /**
         *  This is the country name. It is guaranteed to be NUL
         *  terminated. The length of this field may change in the
         *  future to accomodate larger country names.
         */
        char name[44 + 1];

        /**
         *  This is the two-letter alphabetic code for the country. It
         *  is guaranteed to be NUL terminated. This is the two-letter
         *  code that is used in Internet domain names.
         */
        char a2[2 + 1];

        /**
         *  This is the three-letter alphabetic code for the country. It
         *  is guaranteed to be NUL terminated.
         */
        char a3[3 + 1];

        /**
         *  This it the three-digit number for the country. It bears
         *  no numerical relationship to (for example) the country's
         *  ITU-T E.164 telephony country code used for international
         *  dialing.
         */
        int number;

    };

    /**
     *  This enumeration defines indices into the country-code table.
     *  Each country has an index made up of its name, its two-letter
     *  alphabetic code, its three-letter alphabetic code, and its
     *  number. Note that the enumeration symbols made up of the
     *  two- and three-letter alphabetic codes must be prefixed to
     *  avoid a collision with a potential future two- or three-letter
     *  country name (as unlikely as that may be).
     */
    enum Index {
        MINIMUM = 0,
        AFGHANISTAN = 0,
        A2_AF = 0,
        A3_AFG = 0,
        N_004 = 0,
        ALBANIA = 1,
        A2_AL = 1,
        A3_ALB = 1,
        N_008 = 1,
        ALGERIA = 2,
        A2_DZ = 2,
        A3_DZA = 2,
        N_012 = 2,
        AMERICAN_SAMOA = 3,
        A2_AS = 3,
        A3_ASM = 3,
        N_016 = 3,
        ANDORRA = 4,
        A2_AD = 4,
        A3_AND = 4,
        N_020 = 4,
        ANGOLA = 5,
        A2_AO = 5,
        A3_AGO = 5,
        N_024 = 5,
        ANGUILLA = 6,
        A2_AI = 6,
        A3_AIA = 6,
        N_660 = 6,
        ANTARCTICA = 7,
        A2_AQ = 7,
        A3_ATA = 7,
        N_010 = 7,
        ANTIGUA_AND_BARBUDA = 8,
        A2_AG = 8,
        A3_ATG = 8,
        N_028 = 8,
        ARGENTINA = 9,
        A2_AR = 9,
        A3_ARG = 9,
        N_032 = 9,
        ARMENIA = 10,
        A2_AM = 10,
        A3_ARM = 10,
        N_051 = 10,
        ARUBA = 11,
        A2_AW = 11,
        A3_ABW = 11,
        N_533 = 11,
        AUSTRALIA = 12,
        A2_AU = 12,
        A3_AUS = 12,
        N_036 = 12,
        AUSTRIA = 13,
        A2_AT = 13,
        A3_AUT = 13,
        N_040 = 13,
        AZERBAIJAN = 14,
        A2_AZ = 14,
        A3_AZE = 14,
        N_031 = 14,
        BAHAMAS = 15,
        A2_BS = 15,
        A3_BHS = 15,
        N_044 = 15,
        BAHRAIN = 16,
        A2_BH = 16,
        A3_BHR = 16,
        N_048 = 16,
        BANGLADESH = 17,
        A2_BD = 17,
        A3_BGD = 17,
        N_050 = 17,
        BARBADOS = 18,
        A2_BB = 18,
        A3_BRB = 18,
        N_052 = 18,
        BELARUS = 19,
        A2_BY = 19,
        A3_BLR = 19,
        N_112 = 19,
        BELGIUM = 20,
        A2_BE = 20,
        A3_BEL = 20,
        N_056 = 20,
        BELIZE = 21,
        A2_BZ = 21,
        A3_BLZ = 21,
        N_084 = 21,
        BENIN = 22,
        A2_BJ = 22,
        A3_BEN = 22,
        N_204 = 22,
        BERMUDA = 23,
        A2_BM = 23,
        A3_BMU = 23,
        N_060 = 23,
        BHUTAN = 24,
        A2_BT = 24,
        A3_BTN = 24,
        N_064 = 24,
        BOLIVIA = 25,
        A2_BO = 25,
        A3_BOL = 25,
        N_068 = 25,
        BOSNIA_AND_HERZEGOVINA = 26,
        A2_BA = 26,
        A3_BIH = 26,
        N_070 = 26,
        BOTSWANA = 27,
        A2_BW = 27,
        A3_BWA = 27,
        N_072 = 27,
        BOUVET_ISLAND = 28,
        A2_BV = 28,
        A3_BVT = 28,
        N_074 = 28,
        BRAZIL = 29,
        A2_BR = 29,
        A3_BRA = 29,
        N_076 = 29,
        BRITISH_INDIAN_OCEAN_TERRITORY = 30,
        A2_IO = 30,
        A3_IOT = 30,
        N_086 = 30,
        BRUNEI_DARUSSALAM = 31,
        A2_BN = 31,
        A3_BRN = 31,
        N_096 = 31,
        BULGARIA = 32,
        A2_BG = 32,
        A3_BGR = 32,
        N_100 = 32,
        BURKINA_FASO = 33,
        A2_BF = 33,
        A3_BFA = 33,
        N_854 = 33,
        BURUNDI = 34,
        A2_BI = 34,
        A3_BDI = 34,
        N_108 = 34,
        CAMBODIA = 35,
        A2_KH = 35,
        A3_KHM = 35,
        N_116 = 35,
        CAMEROON = 36,
        A2_CM = 36,
        A3_CMR = 36,
        N_120 = 36,
        CANADA = 37,
        A2_CA = 37,
        A3_CAN = 37,
        N_124 = 37,
        CAPE_VERDE = 38,
        A2_CV = 38,
        A3_CPV = 38,
        N_132 = 38,
        CAYMAN_ISLANDS = 39,
        A2_KY = 39,
        A3_CYM = 39,
        N_136 = 39,
        CENTRAL_AFRICAN_REPUBLIC = 40,
        A2_CF = 40,
        A3_CAF = 40,
        N_140 = 40,
        CHAD = 41,
        A2_TD = 41,
        A3_TCD = 41,
        N_148 = 41,
        CHILE = 42,
        A2_CL = 42,
        A3_CHL = 42,
        N_152 = 42,
        CHINA = 43,
        A2_CN = 43,
        A3_CHN = 43,
        N_156 = 43,
        CHRISTMAS_ISLAND = 44,
        A2_CX = 44,
        A3_CXR = 44,
        N_162 = 44,
        COCOS_KEELING_ISLANDS = 45,
        A2_CC = 45,
        A3_CCK = 45,
        N_166 = 45,
        COLOMBIA = 46,
        A2_CO = 46,
        A3_COL = 46,
        N_170 = 46,
        COMOROS = 47,
        A2_KM = 47,
        A3_COM = 47,
        N_174 = 47,
        CONGO = 48,
        A2_CG = 48,
        A3_COG = 48,
        N_178 = 48,
        CONGO_THE_DEMOCRATIC_REPUBLIC_OF_THE = 49,
        A2_CD = 49,
        A3_COD = 49,
        N_180 = 49,
        COOK_ISLANDS = 50,
        A2_CK = 50,
        A3_COK = 50,
        N_184 = 50,
        COSTA_RICA = 51,
        A2_CR = 51,
        A3_CRI = 51,
        N_188 = 51,
        COTE_D_IVOIRE = 52,
        A2_CI = 52,
        A3_CIV = 52,
        N_384 = 52,
        CROATIA = 53,
        A2_HR = 53,
        A3_HRV = 53,
        N_191 = 53,
        CUBA = 54,
        A2_CU = 54,
        A3_CUB = 54,
        N_192 = 54,
        CYPRUS = 55,
        A2_CY = 55,
        A3_CYP = 55,
        N_196 = 55,
        CZECH_REPUBLIC = 56,
        A2_CZ = 56,
        A3_CZE = 56,
        N_203 = 56,
        DENMARK = 57,
        A2_DK = 57,
        A3_DNK = 57,
        N_208 = 57,
        DJIBOUTI = 58,
        A2_DJ = 58,
        A3_DJI = 58,
        N_262 = 58,
        DOMINICA = 59,
        A2_DM = 59,
        A3_DMA = 59,
        N_212 = 59,
        DOMINICAN_REPUBLIC = 60,
        A2_DO = 60,
        A3_DOM = 60,
        N_214 = 60,
        ECUADOR = 61,
        A2_EC = 61,
        A3_ECU = 61,
        N_218 = 61,
        EGYPT = 62,
        A2_EG = 62,
        A3_EGY = 62,
        N_818 = 62,
        EL_SALVADOR = 63,
        A2_SV = 63,
        A3_SLV = 63,
        N_222 = 63,
        EQUATORIAL_GUINEA = 64,
        A2_GQ = 64,
        A3_GNQ = 64,
        N_226 = 64,
        ERITREA = 65,
        A2_ER = 65,
        A3_ERI = 65,
        N_232 = 65,
        ESTONIA = 66,
        A2_EE = 66,
        A3_EST = 66,
        N_233 = 66,
        ETHIOPIA = 67,
        A2_ET = 67,
        A3_ETH = 67,
        N_231 = 67,
        FALKLAND_ISLANDS_MALVINAS = 68,
        A2_FK = 68,
        A3_FLK = 68,
        N_238 = 68,
        FAROE_ISLANDS = 69,
        A2_FO = 69,
        A3_FRO = 69,
        N_234 = 69,
        FIJI = 70,
        A2_FJ = 70,
        A3_FJI = 70,
        N_242 = 70,
        FINLAND = 71,
        A2_FI = 71,
        A3_FIN = 71,
        N_246 = 71,
        FRANCE = 72,
        A2_FR = 72,
        A3_FRA = 72,
        N_250 = 72,
        FRENCH_GUIANA = 73,
        A2_GF = 73,
        A3_GUF = 73,
        N_254 = 73,
        FRENCH_POLYNESIA = 74,
        A2_PF = 74,
        A3_PYF = 74,
        N_258 = 74,
        FRENCH_SOUTHERN_TERRITORIES = 75,
        A2_TF = 75,
        A3_ATF = 75,
        N_260 = 75,
        GABON = 76,
        A2_GA = 76,
        A3_GAB = 76,
        N_266 = 76,
        GAMBIA = 77,
        A2_GM = 77,
        A3_GMB = 77,
        N_270 = 77,
        GEORGIA = 78,
        A2_GE = 78,
        A3_GEO = 78,
        N_268 = 78,
        GERMANY = 79,
        A2_DE = 79,
        A3_DEU = 79,
        N_276 = 79,
        GHANA = 80,
        A2_GH = 80,
        A3_GHA = 80,
        N_288 = 80,
        GIBRALTAR = 81,
        A2_GI = 81,
        A3_GIB = 81,
        N_292 = 81,
        GREECE = 82,
        A2_GR = 82,
        A3_GRC = 82,
        N_300 = 82,
        GREENLAND = 83,
        A2_GL = 83,
        A3_GRL = 83,
        N_304 = 83,
        GRENADA = 84,
        A2_GD = 84,
        A3_GRD = 84,
        N_308 = 84,
        GUADELOUPE = 85,
        A2_GP = 85,
        A3_GLP = 85,
        N_312 = 85,
        GUAM = 86,
        A2_GU = 86,
        A3_GUM = 86,
        N_316 = 86,
        GUATEMALA = 87,
        A2_GT = 87,
        A3_GTM = 87,
        N_320 = 87,
        GUINEA = 88,
        A2_GN = 88,
        A3_GIN = 88,
        N_324 = 88,
        GUINEA_BISSAU = 89,
        A2_GW = 89,
        A3_GNB = 89,
        N_624 = 89,
        GUYANA = 90,
        A2_GY = 90,
        A3_GUY = 90,
        N_328 = 90,
        HAITI = 91,
        A2_HT = 91,
        A3_HTI = 91,
        N_332 = 91,
        HEARD_ISLAND_AND_MCDONALD_ISLANDS = 92,
        A2_HM = 92,
        A3_HMD = 92,
        N_334 = 92,
        HOLY_SEE_VATICAN_CITY_STATE = 93,
        A2_VA = 93,
        A3_VAT = 93,
        N_336 = 93,
        HONDURAS = 94,
        A2_HN = 94,
        A3_HND = 94,
        N_340 = 94,
        HONG_KONG = 95,
        A2_HK = 95,
        A3_HKG = 95,
        N_344 = 95,
        HUNGARY = 96,
        A2_HU = 96,
        A3_HUN = 96,
        N_348 = 96,
        ICELAND = 97,
        A2_IS = 97,
        A3_ISL = 97,
        N_352 = 97,
        INDIA = 98,
        A2_IN = 98,
        A3_IND = 98,
        N_356 = 98,
        INDONESIA = 99,
        A2_ID = 99,
        A3_IDN = 99,
        N_360 = 99,
        IRAN_ISLAMIC_REPUBLIC_OF = 100,
        A2_IR = 100,
        A3_IRN = 100,
        N_364 = 100,
        IRAQ = 101,
        A2_IQ = 101,
        A3_IRQ = 101,
        N_368 = 101,
        IRELAND = 102,
        A2_IE = 102,
        A3_IRL = 102,
        N_372 = 102,
        ISRAEL = 103,
        A2_IL = 103,
        A3_ISR = 103,
        N_376 = 103,
        ITALY = 104,
        A2_IT = 104,
        A3_ITA = 104,
        N_380 = 104,
        JAMAICA = 105,
        A2_JM = 105,
        A3_JAM = 105,
        N_388 = 105,
        JAPAN = 106,
        A2_JP = 106,
        A3_JPN = 106,
        N_392 = 106,
        JORDAN = 107,
        A2_JO = 107,
        A3_JOR = 107,
        N_400 = 107,
        KAZAKHSTAN = 108,
        A2_KZ = 108,
        A3_KAZ = 108,
        N_398 = 108,
        KENYA = 109,
        A2_KE = 109,
        A3_KEN = 109,
        N_404 = 109,
        KIRIBATI = 110,
        A2_KI = 110,
        A3_KIR = 110,
        N_296 = 110,
        KOREA_DEMOCRATIC_PEOPLE_S_REPUBLIC_OF = 111,
        A2_KP = 111,
        A3_PRK = 111,
        N_408 = 111,
        KOREA_REPUBLIC_OF = 112,
        A2_KR = 112,
        A3_KOR = 112,
        N_410 = 112,
        KUWAIT = 113,
        A2_KW = 113,
        A3_KWT = 113,
        N_414 = 113,
        KYRGYZSTAN = 114,
        A2_KG = 114,
        A3_KGZ = 114,
        N_417 = 114,
        LAO_PEOPLE_S_DEMOCRATIC_REPUBLIC = 115,
        A2_LA = 115,
        A3_LAO = 115,
        N_418 = 115,
        LATVIA = 116,
        A2_LV = 116,
        A3_LVA = 116,
        N_428 = 116,
        LEBANON = 117,
        A2_LB = 117,
        A3_LBN = 117,
        N_422 = 117,
        LESOTHO = 118,
        A2_LS = 118,
        A3_LSO = 118,
        N_426 = 118,
        LIBERIA = 119,
        A2_LR = 119,
        A3_LBR = 119,
        N_430 = 119,
        LIBYAN_ARAB_JAMAHIRIYA = 120,
        A2_LY = 120,
        A3_LBY = 120,
        N_434 = 120,
        LIECHTENSTEIN = 121,
        A2_LI = 121,
        A3_LIE = 121,
        N_438 = 121,
        LITHUANIA = 122,
        A2_LT = 122,
        A3_LTU = 122,
        N_440 = 122,
        LUXEMBOURG = 123,
        A2_LU = 123,
        A3_LUX = 123,
        N_442 = 123,
        MACAO = 124,
        A2_MO = 124,
        A3_MAC = 124,
        N_446 = 124,
        MACEDONIA_THE_FORMER_YUGOSLAV_REPUBLIC_OF = 125,
        A2_MK = 125,
        A3_MKD = 125,
        N_807 = 125,
        MADAGASCAR = 126,
        A2_MG = 126,
        A3_MDG = 126,
        N_450 = 126,
        MALAWI = 127,
        A2_MW = 127,
        A3_MWI = 127,
        N_454 = 127,
        MALAYSIA = 128,
        A2_MY = 128,
        A3_MYS = 128,
        N_458 = 128,
        MALDIVES = 129,
        A2_MV = 129,
        A3_MDV = 129,
        N_462 = 129,
        MALI = 130,
        A2_ML = 130,
        A3_MLI = 130,
        N_466 = 130,
        MALTA = 131,
        A2_MT = 131,
        A3_MLT = 131,
        N_470 = 131,
        MARSHALL_ISLANDS = 132,
        A2_MH = 132,
        A3_MHL = 132,
        N_584 = 132,
        MARTINIQUE = 133,
        A2_MQ = 133,
        A3_MTQ = 133,
        N_474 = 133,
        MAURITANIA = 134,
        A2_MR = 134,
        A3_MRT = 134,
        N_478 = 134,
        MAURITIUS = 135,
        A2_MU = 135,
        A3_MUS = 135,
        N_480 = 135,
        MAYOTTE = 136,
        A2_YT = 136,
        A3_MYT = 136,
        N_175 = 136,
        MEXICO = 137,
        A2_MX = 137,
        A3_MEX = 137,
        N_484 = 137,
        MICRONESIA_FEDERATED_STATES_OF = 138,
        A2_FM = 138,
        A3_FSM = 138,
        N_583 = 138,
        MOLDOVA_REPUBLIC_OF = 139,
        A2_MD = 139,
        A3_MDA = 139,
        N_498 = 139,
        MONACO = 140,
        A2_MC = 140,
        A3_MCO = 140,
        N_492 = 140,
        MONGOLIA = 141,
        A2_MN = 141,
        A3_MNG = 141,
        N_496 = 141,
        MONTSERRAT = 142,
        A2_MS = 142,
        A3_MSR = 142,
        N_500 = 142,
        MOROCCO = 143,
        A2_MA = 143,
        A3_MAR = 143,
        N_504 = 143,
        MOZAMBIQUE = 144,
        A2_MZ = 144,
        A3_MOZ = 144,
        N_508 = 144,
        MYANMAR = 145,
        A2_MM = 145,
        A3_MMR = 145,
        N_104 = 145,
        NAMIBIA = 146,
        A2_NA = 146,
        A3_NAM = 146,
        N_516 = 146,
        NAURU = 147,
        A2_NR = 147,
        A3_NRU = 147,
        N_520 = 147,
        NEPAL = 148,
        A2_NP = 148,
        A3_NPL = 148,
        N_524 = 148,
        NETHERLANDS = 149,
        A2_NL = 149,
        A3_NLD = 149,
        N_528 = 149,
        NETHERLANDS_ANTILLES = 150,
        A2_AN = 150,
        A3_ANT = 150,
        N_530 = 150,
        NEW_CALEDONIA = 151,
        A2_NC = 151,
        A3_NCL = 151,
        N_540 = 151,
        NEW_ZEALAND = 152,
        A2_NZ = 152,
        A3_NZL = 152,
        N_554 = 152,
        NICARAGUA = 153,
        A2_NI = 153,
        A3_NIC = 153,
        N_558 = 153,
        NIGER = 154,
        A2_NE = 154,
        A3_NER = 154,
        N_562 = 154,
        NIGERIA = 155,
        A2_NG = 155,
        A3_NGA = 155,
        N_566 = 155,
        NIUE = 156,
        A2_NU = 156,
        A3_NIU = 156,
        N_570 = 156,
        NORFOLK_ISLAND = 157,
        A2_NF = 157,
        A3_NFK = 157,
        N_574 = 157,
        NORTHERN_MARIANA_ISLANDS = 158,
        A2_MP = 158,
        A3_MNP = 158,
        N_580 = 158,
        NORWAY = 159,
        A2_NO = 159,
        A3_NOR = 159,
        N_578 = 159,
        OMAN = 160,
        A2_OM = 160,
        A3_OMN = 160,
        N_512 = 160,
        PAKISTAN = 161,
        A2_PK = 161,
        A3_PAK = 161,
        N_586 = 161,
        PALAU = 162,
        A2_PW = 162,
        A3_PLW = 162,
        N_585 = 162,
        PALESTINIAN_TERRITORY_OCCUPIED = 163,
        A2_PS = 163,
        A3_PSE = 163,
        N_275 = 163,
        PANAMA = 164,
        A2_PA = 164,
        A3_PAN = 164,
        N_591 = 164,
        PAPUA_NEW_GUINEA = 165,
        A2_PG = 165,
        A3_PNG = 165,
        N_598 = 165,
        PARAGUAY = 166,
        A2_PY = 166,
        A3_PRY = 166,
        N_600 = 166,
        PERU = 167,
        A2_PE = 167,
        A3_PER = 167,
        N_604 = 167,
        PHILIPPINES = 168,
        A2_PH = 168,
        A3_PHL = 168,
        N_608 = 168,
        PITCAIRN = 169,
        A2_PN = 169,
        A3_PCN = 169,
        N_612 = 169,
        POLAND = 170,
        A2_PL = 170,
        A3_POL = 170,
        N_616 = 170,
        PORTUGAL = 171,
        A2_PT = 171,
        A3_PRT = 171,
        N_620 = 171,
        PUERTO_RICO = 172,
        A2_PR = 172,
        A3_PRI = 172,
        N_630 = 172,
        QATAR = 173,
        A2_QA = 173,
        A3_QAT = 173,
        N_634 = 173,
        REUNION = 174,
        A2_RE = 174,
        A3_REU = 174,
        N_638 = 174,
        ROMANIA = 175,
        A2_RO = 175,
        A3_ROU = 175,
        N_642 = 175,
        RUSSIAN_FEDERATION = 176,
        A2_RU = 176,
        A3_RUS = 176,
        N_643 = 176,
        RWANDA = 177,
        A2_RW = 177,
        A3_RWA = 177,
        N_646 = 177,
        SAINT_HELENA = 178,
        A2_SH = 178,
        A3_SHN = 178,
        N_654 = 178,
        SAINT_KITTS_AND_NEVIS = 179,
        A2_KN = 179,
        A3_KNA = 179,
        N_659 = 179,
        SAINT_LUCIA = 180,
        A2_LC = 180,
        A3_LCA = 180,
        N_662 = 180,
        SAINT_PIERRE_AND_MIQUELON = 181,
        A2_PM = 181,
        A3_SPM = 181,
        N_666 = 181,
        SAINT_VINCENT_AND_THE_GRENADINES = 182,
        A2_VC = 182,
        A3_VCT = 182,
        N_670 = 182,
        SAMOA = 183,
        A2_WS = 183,
        A3_WSM = 183,
        N_882 = 183,
        SAN_MARINO = 184,
        A2_SM = 184,
        A3_SMR = 184,
        N_674 = 184,
        SAO_TOME_AND_PRINCIPE = 185,
        A2_ST = 185,
        A3_STP = 185,
        N_678 = 185,
        SAUDI_ARABIA = 186,
        A2_SA = 186,
        A3_SAU = 186,
        N_682 = 186,
        SENEGAL = 187,
        A2_SN = 187,
        A3_SEN = 187,
        N_686 = 187,
        SERBIA_AND_MONTENEGRO = 188,
        A2_CS = 188,
        A3_SCG = 188,
        N_891 = 188,
        SEYCHELLES = 189,
        A2_SC = 189,
        A3_SYC = 189,
        N_690 = 189,
        SIERRA_LEONE = 190,
        A2_SL = 190,
        A3_SLE = 190,
        N_694 = 190,
        SINGAPORE = 191,
        A2_SG = 191,
        A3_SGP = 191,
        N_702 = 191,
        SLOVAKIA = 192,
        A2_SK = 192,
        A3_SVK = 192,
        N_703 = 192,
        SLOVENIA = 193,
        A2_SI = 193,
        A3_SVN = 193,
        N_705 = 193,
        SOLOMON_ISLANDS = 194,
        A2_SB = 194,
        A3_SLB = 194,
        N_090 = 194,
        SOMALIA = 195,
        A2_SO = 195,
        A3_SOM = 195,
        N_706 = 195,
        SOUTH_AFRICA = 196,
        A2_ZA = 196,
        A3_ZAF = 196,
        N_710 = 196,
        SOUTH_GEORGIA_AND_THE_SOUTH_SANDWICH_ISLANDS = 197,
        A2_GS = 197,
        A3_SGS = 197,
        N_239 = 197,
        SPAIN = 198,
        A2_ES = 198,
        A3_ESP = 198,
        N_724 = 198,
        SRI_LANKA = 199,
        A2_LK = 199,
        A3_LKA = 199,
        N_144 = 199,
        SUDAN = 200,
        A2_SD = 200,
        A3_SDN = 200,
        N_736 = 200,
        SURINAME = 201,
        A2_SR = 201,
        A3_SUR = 201,
        N_740 = 201,
        SVALBARD_AND_JAN_MAYEN = 202,
        A2_SJ = 202,
        A3_SJM = 202,
        N_744 = 202,
        SWAZILAND = 203,
        A2_SZ = 203,
        A3_SWZ = 203,
        N_748 = 203,
        SWEDEN = 204,
        A2_SE = 204,
        A3_SWE = 204,
        N_752 = 204,
        SWITZERLAND = 205,
        A2_CH = 205,
        A3_CHE = 205,
        N_756 = 205,
        SYRIAN_ARAB_REPUBLIC = 206,
        A2_SY = 206,
        A3_SYR = 206,
        N_760 = 206,
        TAIWAN_PROVINCE_OF_CHINA = 207,
        A2_TW = 207,
        A3_TWN = 207,
        N_158 = 207,
        TAJIKISTAN = 208,
        A2_TJ = 208,
        A3_TJK = 208,
        N_762 = 208,
        TANZANIA_UNITED_REPUBLIC_OF = 209,
        A2_TZ = 209,
        A3_TZA = 209,
        N_834 = 209,
        THAILAND = 210,
        A2_TH = 210,
        A3_THA = 210,
        N_764 = 210,
        TIMOR_LESTE = 211,
        A2_TL = 211,
        A3_TLS = 211,
        N_626 = 211,
        TOGO = 212,
        A2_TG = 212,
        A3_TGO = 212,
        N_768 = 212,
        TOKELAU = 213,
        A2_TK = 213,
        A3_TKL = 213,
        N_772 = 213,
        TONGA = 214,
        A2_TO = 214,
        A3_TON = 214,
        N_776 = 214,
        TRINIDAD_AND_TOBAGO = 215,
        A2_TT = 215,
        A3_TTO = 215,
        N_780 = 215,
        TUNISIA = 216,
        A2_TN = 216,
        A3_TUN = 216,
        N_788 = 216,
        TURKEY = 217,
        A2_TR = 217,
        A3_TUR = 217,
        N_792 = 217,
        TURKMENISTAN = 218,
        A2_TM = 218,
        A3_TKM = 218,
        N_795 = 218,
        TURKS_AND_CAICOS_ISLANDS = 219,
        A2_TC = 219,
        A3_TCA = 219,
        N_796 = 219,
        TUVALU = 220,
        A2_TV = 220,
        A3_TUV = 220,
        N_798 = 220,
        UGANDA = 221,
        A2_UG = 221,
        A3_UGA = 221,
        N_800 = 221,
        UKRAINE = 222,
        A2_UA = 222,
        A3_UKR = 222,
        N_804 = 222,
        UNITED_ARAB_EMIRATES = 223,
        A2_AE = 223,
        A3_ARE = 223,
        N_784 = 223,
        UNITED_KINGDOM = 224,
        A2_GB = 224,
        A3_GBR = 224,
        N_826 = 224,
        UNITED_STATES = 225,
        A2_US = 225,
        A3_USA = 225,
        N_840 = 225,
        UNITED_STATES_MINOR_OUTLYING_ISLANDS = 226,
        A2_UM = 226,
        A3_UMI = 226,
        N_581 = 226,
        URUGUAY = 227,
        A2_UY = 227,
        A3_URY = 227,
        N_858 = 227,
        UZBEKISTAN = 228,
        A2_UZ = 228,
        A3_UZB = 228,
        N_860 = 228,
        VANUATU = 229,
        A2_VU = 229,
        A3_VUT = 229,
        N_548 = 229,
        VENEZUELA = 230,
        A2_VE = 230,
        A3_VEN = 230,
        N_862 = 230,
        VIET_NAM = 231,
        A2_VN = 231,
        A3_VNM = 231,
        N_704 = 231,
        VIRGIN_ISLANDS_BRITISH = 232,
        A2_VG = 232,
        A3_VGB = 232,
        N_092 = 232,
        VIRGIN_ISLANDS_U_S = 233,
        A2_VI = 233,
        A3_VIR = 233,
        N_850 = 233,
        WALLIS_AND_FUTUNA = 234,
        A2_WF = 234,
        A3_WLF = 234,
        N_876 = 234,
        WESTERN_SAHARA = 235,
        A2_EH = 235,
        A3_ESH = 235,
        N_732 = 235,
        YEMEN = 236,
        A2_YE = 236,
        A3_YEM = 236,
        N_887 = 236,
        ZAMBIA = 237,
        A2_ZM = 237,
        A3_ZMB = 237,
        N_894 = 237,
        ZIMBABWE = 238,
        A2_ZW = 238,
        A3_ZWE = 238,
        N_716 = 238,
        MAXIMUM = 238
    };

    /**
     *  Returns a reference to the indexed entry in the ISO3166 table.
     *
     *  @param  index   is an index into the ISO3166 table.
     *
     *  @return a reference to the indexed entry in the ISO3166 table.
     */
    const Entry& operator[] (Index index) const;

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param display  points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;

private:

    /**
     *  This is the country-code table.
     */
    static const Entry table[239];

};


//
// Constructor
//
inline Iso3166::Iso3166() {
}


//
//  Return a reference to the indexed entry in the country code table.
//
inline const Iso3166::Entry& Iso3166::operator[] (Index index)
        const {
    return this->table[index];
}

#include "com/diag/grandote/End.h"


#if defined(GRANDOTE_HAS_UNITTESTS)
#include "com/diag/grandote/cxxcapi.h"
/**
 *  Run the Iso3166 unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestIso3166(void);
#endif


#endif
