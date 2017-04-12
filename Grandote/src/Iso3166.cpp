/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.
    
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
 *  Implements the Iso3166 class.
 *
 *  @see    Iso3166
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Iso3166.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Country-code table.
//
const Iso3166::Entry Iso3166::table[] = {
    { "AFGHANISTAN", "AF", "AFG", 4 },
    { "ALBANIA", "AL", "ALB", 8 },
    { "ALGERIA", "DZ", "DZA", 12 },
    { "AMERICAN SAMOA", "AS", "ASM", 16 },
    { "ANDORRA", "AD", "AND", 20 },
    { "ANGOLA", "AO", "AGO", 24 },
    { "ANGUILLA", "AI", "AIA", 660 },
    { "ANTARCTICA", "AQ", "ATA", 10 },
    { "ANTIGUA AND BARBUDA", "AG", "ATG", 28 },
    { "ARGENTINA", "AR", "ARG", 32 },
    { "ARMENIA", "AM", "ARM", 51 },
    { "ARUBA", "AW", "ABW", 533 },
    { "AUSTRALIA", "AU", "AUS", 36 },
    { "AUSTRIA", "AT", "AUT", 40 },
    { "AZERBAIJAN", "AZ", "AZE", 31 },
    { "BAHAMAS", "BS", "BHS", 44 },
    { "BAHRAIN", "BH", "BHR", 48 },
    { "BANGLADESH", "BD", "BGD", 50 },
    { "BARBADOS", "BB", "BRB", 52 },
    { "BELARUS", "BY", "BLR", 112 },
    { "BELGIUM", "BE", "BEL", 56 },
    { "BELIZE", "BZ", "BLZ", 84 },
    { "BENIN", "BJ", "BEN", 204 },
    { "BERMUDA", "BM", "BMU", 60 },
    { "BHUTAN", "BT", "BTN", 64 },
    { "BOLIVIA", "BO", "BOL", 68 },
    { "BOSNIA AND HERZEGOVINA", "BA", "BIH", 70 },
    { "BOTSWANA", "BW", "BWA", 72 },
    { "BOUVET ISLAND", "BV", "BVT", 74 },
    { "BRAZIL", "BR", "BRA", 76 },
    { "BRITISH INDIAN OCEAN TERRITORY", "IO", "IOT", 86 },
    { "BRUNEI DARUSSALAM", "BN", "BRN", 96 },
    { "BULGARIA", "BG", "BGR", 100 },
    { "BURKINA FASO", "BF", "BFA", 854 },
    { "BURUNDI", "BI", "BDI", 108 },
    { "CAMBODIA", "KH", "KHM", 116 },
    { "CAMEROON", "CM", "CMR", 120 },
    { "CANADA", "CA", "CAN", 124 },
    { "CAPE VERDE", "CV", "CPV", 132 },
    { "CAYMAN ISLANDS", "KY", "CYM", 136 },
    { "CENTRAL AFRICAN REPUBLIC", "CF", "CAF", 140 },
    { "CHAD", "TD", "TCD", 148 },
    { "CHILE", "CL", "CHL", 152 },
    { "CHINA", "CN", "CHN", 156 },
    { "CHRISTMAS ISLAND", "CX", "CXR", 162 },
    { "COCOS (KEELING) ISLANDS", "CC", "CCK", 166 },
    { "COLOMBIA", "CO", "COL", 170 },
    { "COMOROS", "KM", "COM", 174 },
    { "CONGO", "CG", "COG", 178 },
    { "CONGO, THE DEMOCRATIC REPUBLIC OF THE", "CD", "COD", 180 },
    { "COOK ISLANDS", "CK", "COK", 184 },
    { "COSTA RICA", "CR", "CRI", 188 },
    { "COTE D'IVOIRE", "CI", "CIV", 384 },
    { "CROATIA", "HR", "HRV", 191 },
    { "CUBA", "CU", "CUB", 192 },
    { "CYPRUS", "CY", "CYP", 196 },
    { "CZECH REPUBLIC", "CZ", "CZE", 203 },
    { "DENMARK", "DK", "DNK", 208 },
    { "DJIBOUTI", "DJ", "DJI", 262 },
    { "DOMINICA", "DM", "DMA", 212 },
    { "DOMINICAN REPUBLIC", "DO", "DOM", 214 },
    { "ECUADOR", "EC", "ECU", 218 },
    { "EGYPT", "EG", "EGY", 818 },
    { "EL SALVADOR", "SV", "SLV", 222 },
    { "EQUATORIAL GUINEA", "GQ", "GNQ", 226 },
    { "ERITREA", "ER", "ERI", 232 },
    { "ESTONIA", "EE", "EST", 233 },
    { "ETHIOPIA", "ET", "ETH", 231 },
    { "FALKLAND ISLANDS (MALVINAS)", "FK", "FLK", 238 },
    { "FAROE ISLANDS", "FO", "FRO", 234 },
    { "FIJI", "FJ", "FJI", 242 },
    { "FINLAND", "FI", "FIN", 246 },
    { "FRANCE", "FR", "FRA", 250 },
    { "FRENCH GUIANA", "GF", "GUF", 254 },
    { "FRENCH POLYNESIA", "PF", "PYF", 258 },
    { "FRENCH SOUTHERN TERRITORIES", "TF", "ATF", 260 },
    { "GABON", "GA", "GAB", 266 },
    { "GAMBIA", "GM", "GMB", 270 },
    { "GEORGIA", "GE", "GEO", 268 },
    { "GERMANY", "DE", "DEU", 276 },
    { "GHANA", "GH", "GHA", 288 },
    { "GIBRALTAR", "GI", "GIB", 292 },
    { "GREECE", "GR", "GRC", 300 },
    { "GREENLAND", "GL", "GRL", 304 },
    { "GRENADA", "GD", "GRD", 308 },
    { "GUADELOUPE", "GP", "GLP", 312 },
    { "GUAM", "GU", "GUM", 316 },
    { "GUATEMALA", "GT", "GTM", 320 },
    { "GUINEA", "GN", "GIN", 324 },
    { "GUINEA-BISSAU", "GW", "GNB", 624 },
    { "GUYANA", "GY", "GUY", 328 },
    { "HAITI", "HT", "HTI", 332 },
    { "HEARD ISLAND AND MCDONALD ISLANDS", "HM", "HMD", 334 },
    { "HOLY SEE (VATICAN CITY STATE)", "VA", "VAT", 336 },
    { "HONDURAS", "HN", "HND", 340 },
    { "HONG KONG", "HK", "HKG", 344 },
    { "HUNGARY", "HU", "HUN", 348 },
    { "ICELAND", "IS", "ISL", 352 },
    { "INDIA", "IN", "IND", 356 },
    { "INDONESIA", "ID", "IDN", 360 },
    { "IRAN, ISLAMIC REPUBLIC OF", "IR", "IRN", 364 },
    { "IRAQ", "IQ", "IRQ", 368 },
    { "IRELAND", "IE", "IRL", 372 },
    { "ISRAEL", "IL", "ISR", 376 },
    { "ITALY", "IT", "ITA", 380 },
    { "JAMAICA", "JM", "JAM", 388 },
    { "JAPAN", "JP", "JPN", 392 },
    { "JORDAN", "JO", "JOR", 400 },
    { "KAZAKHSTAN", "KZ", "KAZ", 398 },
    { "KENYA", "KE", "KEN", 404 },
    { "KIRIBATI", "KI", "KIR", 296 },
    { "KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF", "KP", "PRK", 408 },
    { "KOREA, REPUBLIC OF", "KR", "KOR", 410 },
    { "KUWAIT", "KW", "KWT", 414 },
    { "KYRGYZSTAN", "KG", "KGZ", 417 },
    { "LAO PEOPLE'S DEMOCRATIC REPUBLIC", "LA", "LAO", 418 },
    { "LATVIA", "LV", "LVA", 428 },
    { "LEBANON", "LB", "LBN", 422 },
    { "LESOTHO", "LS", "LSO", 426 },
    { "LIBERIA", "LR", "LBR", 430 },
    { "LIBYAN ARAB JAMAHIRIYA", "LY", "LBY", 434 },
    { "LIECHTENSTEIN", "LI", "LIE", 438 },
    { "LITHUANIA", "LT", "LTU", 440 },
    { "LUXEMBOURG", "LU", "LUX", 442 },
    { "MACAO", "MO", "MAC", 446 },
    { "MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF", "MK", "MKD", 807 },
    { "MADAGASCAR", "MG", "MDG", 450 },
    { "MALAWI", "MW", "MWI", 454 },
    { "MALAYSIA", "MY", "MYS", 458 },
    { "MALDIVES", "MV", "MDV", 462 },
    { "MALI", "ML", "MLI", 466 },
    { "MALTA", "MT", "MLT", 470 },
    { "MARSHALL ISLANDS", "MH", "MHL", 584 },
    { "MARTINIQUE", "MQ", "MTQ", 474 },
    { "MAURITANIA", "MR", "MRT", 478 },
    { "MAURITIUS", "MU", "MUS", 480 },
    { "MAYOTTE", "YT", "MYT", 175 },
    { "MEXICO", "MX", "MEX", 484 },
    { "MICRONESIA, FEDERATED STATES OF", "FM", "FSM", 583 },
    { "MOLDOVA, REPUBLIC OF", "MD", "MDA", 498 },
    { "MONACO", "MC", "MCO", 492 },
    { "MONGOLIA", "MN", "MNG", 496 },
    { "MONTSERRAT", "MS", "MSR", 500 },
    { "MOROCCO", "MA", "MAR", 504 },
    { "MOZAMBIQUE", "MZ", "MOZ", 508 },
    { "MYANMAR", "MM", "MMR", 104 },
    { "NAMIBIA", "NA", "NAM", 516 },
    { "NAURU", "NR", "NRU", 520 },
    { "NEPAL", "NP", "NPL", 524 },
    { "NETHERLANDS", "NL", "NLD", 528 },
    { "NETHERLANDS ANTILLES", "AN", "ANT", 530 },
    { "NEW CALEDONIA", "NC", "NCL", 540 },
    { "NEW ZEALAND", "NZ", "NZL", 554 },
    { "NICARAGUA", "NI", "NIC", 558 },
    { "NIGER", "NE", "NER", 562 },
    { "NIGERIA", "NG", "NGA", 566 },
    { "NIUE", "NU", "NIU", 570 },
    { "NORFOLK ISLAND", "NF", "NFK", 574 },
    { "NORTHERN MARIANA ISLANDS", "MP", "MNP", 580 },
    { "NORWAY", "NO", "NOR", 578 },
    { "OMAN", "OM", "OMN", 512 },
    { "PAKISTAN", "PK", "PAK", 586 },
    { "PALAU", "PW", "PLW", 585 },
    { "PALESTINIAN TERRITORY, OCCUPIED", "PS", "PSE", 275 },
    { "PANAMA", "PA", "PAN", 591 },
    { "PAPUA NEW GUINEA", "PG", "PNG", 598 },
    { "PARAGUAY", "PY", "PRY", 600 },
    { "PERU", "PE", "PER", 604 },
    { "PHILIPPINES", "PH", "PHL", 608 },
    { "PITCAIRN", "PN", "PCN", 612 },
    { "POLAND", "PL", "POL", 616 },
    { "PORTUGAL", "PT", "PRT", 620 },
    { "PUERTO RICO", "PR", "PRI", 630 },
    { "QATAR", "QA", "QAT", 634 },
    { "REUNION", "RE", "REU", 638 },
    { "ROMANIA", "RO", "ROU", 642 },
    { "RUSSIAN FEDERATION", "RU", "RUS", 643 },
    { "RWANDA", "RW", "RWA", 646 },
    { "SAINT HELENA", "SH", "SHN", 654 },
    { "SAINT KITTS AND NEVIS", "KN", "KNA", 659 },
    { "SAINT LUCIA", "LC", "LCA", 662 },
    { "SAINT PIERRE AND MIQUELON", "PM", "SPM", 666 },
    { "SAINT VINCENT AND THE GRENADINES", "VC", "VCT", 670 },
    { "SAMOA", "WS", "WSM", 882 },
    { "SAN MARINO", "SM", "SMR", 674 },
    { "SAO TOME AND PRINCIPE", "ST", "STP", 678 },
    { "SAUDI ARABIA", "SA", "SAU", 682 },
    { "SENEGAL", "SN", "SEN", 686 },
    { "SERBIA AND MONTENEGRO", "CS", "SCG", 891 },
    { "SEYCHELLES", "SC", "SYC", 690 },
    { "SIERRA LEONE", "SL", "SLE", 694 },
    { "SINGAPORE", "SG", "SGP", 702 },
    { "SLOVAKIA", "SK", "SVK", 703 },
    { "SLOVENIA", "SI", "SVN", 705 },
    { "SOLOMON ISLANDS", "SB", "SLB", 90 },
    { "SOMALIA", "SO", "SOM", 706 },
    { "SOUTH AFRICA", "ZA", "ZAF", 710 },
    { "SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS", "GS", "SGS", 239 },
    { "SPAIN", "ES", "ESP", 724 },
    { "SRI LANKA", "LK", "LKA", 144 },
    { "SUDAN", "SD", "SDN", 736 },
    { "SURINAME", "SR", "SUR", 740 },
    { "SVALBARD AND JAN MAYEN", "SJ", "SJM", 744 },
    { "SWAZILAND", "SZ", "SWZ", 748 },
    { "SWEDEN", "SE", "SWE", 752 },
    { "SWITZERLAND", "CH", "CHE", 756 },
    { "SYRIAN ARAB REPUBLIC", "SY", "SYR", 760 },
    { "TAIWAN, PROVINCE OF CHINA", "TW", "TWN", 158 },
    { "TAJIKISTAN", "TJ", "TJK", 762 },
    { "TANZANIA, UNITED REPUBLIC OF", "TZ", "TZA", 834 },
    { "THAILAND", "TH", "THA", 764 },
    { "TIMOR-LESTE", "TL", "TLS", 626 },
    { "TOGO", "TG", "TGO", 768 },
    { "TOKELAU", "TK", "TKL", 772 },
    { "TONGA", "TO", "TON", 776 },
    { "TRINIDAD AND TOBAGO", "TT", "TTO", 780 },
    { "TUNISIA", "TN", "TUN", 788 },
    { "TURKEY", "TR", "TUR", 792 },
    { "TURKMENISTAN", "TM", "TKM", 795 },
    { "TURKS AND CAICOS ISLANDS", "TC", "TCA", 796 },
    { "TUVALU", "TV", "TUV", 798 },
    { "UGANDA", "UG", "UGA", 800 },
    { "UKRAINE", "UA", "UKR", 804 },
    { "UNITED ARAB EMIRATES", "AE", "ARE", 784 },
    { "UNITED KINGDOM", "GB", "GBR", 826 },
    { "UNITED STATES", "US", "USA", 840 },
    { "UNITED STATES MINOR OUTLYING ISLANDS", "UM", "UMI", 581 },
    { "URUGUAY", "UY", "URY", 858 },
    { "UZBEKISTAN", "UZ", "UZB", 860 },
    { "VANUATU", "VU", "VUT", 548 },
    { "VENEZUELA", "VE", "VEN", 862 },
    { "VIET NAM", "VN", "VNM", 704 },
    { "VIRGIN ISLANDS, BRITISH", "VG", "VGB", 92 },
    { "VIRGIN ISLANDS, U.S.", "VI", "VIR", 850 },
    { "WALLIS AND FUTUNA", "WF", "WLF", 876 },
    { "WESTERN SAHARA", "EH", "ESH", 732 },
    { "YEMEN", "YE", "YEM", 887 },
    { "ZAMBIA", "ZM", "ZMB", 894 },
    { "ZIMBABWE", "ZW", "ZWE", 716 }
};


//
//  Destructor.
//
Iso3166::~Iso3166() {
}


//
//  Show this object on the output object.
//
void Iso3166::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"