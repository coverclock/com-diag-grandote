/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
    Boston, MA 02111-1307 USA, or http://www.gnu.org/copyleft/lesser.txt.



*******************************************************************************/


/**
 *  @file
 *
 *  Implements the ascii command line tool.
 *
 *  @see    ascii
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


/******************************************************************************

man ascii (Solaris 5.8):

|000 nul|001 soh|002 stx|003 etx|004 eot|005 enq|006 ack|007 bel|
|010 bs |011 ht |012 nl |013 vt |014 np |015 cr |016 so |017 si |
|020 dle|021 dc1|022 dc2|023 dc3|024 dc4|025 nak|026 syn|027 etb|
|030 can|031 em |032 sub|033 esc|034 fs |035 gs |036 rs |037 us |
|040 sp |041  ! |042  " |043  # |044  $ |045  % |046  & |047  ' |
|050  ( |051  ) |052  * |053  + |054  , |055  - |056  . |057  / |
|060  0 |061  1 |062  2 |063  3 |064  4 |065  5 |066  6 |067  7 |
|070  8 |071  9 |072  : |073  ; |074  < |075  = |076  > |077  ? |
|100  @ |101  A |102  B |103  C |104  D |105  E |106  F |107  G |
|110  H |111  I |112  J |113  K |114  L |115  M |116  N |117  O |
|120  P |121  Q |122  R |123  S |124  T |125  U |126  V |127  W |
|130  X |131  Y |132  Z |133  [ |134  \ |135  ] |136  ^ |137  _ |
|140  ` |141  a |142  b |143  c |144  d |145  e |146  f |147  g |
|150  h |151  i |152  j |153  k |154  l |155  m |156  n |157  o |
|160  p |161  q |162  r |163  s |164  t |165  u |166  v |167  w |
|170  x |171  y |172  z |173  { |174  | |175  } |176  ~ |177 del|

| 00 nul| 01 soh| 02 stx| 03 etx| 04 eot| 05 enq| 06 ack| 07 bel|
| 08 bs | 09 ht | 0a nl | 0b vt | 0c np | 0d cr | 0e so | 0f si |
| 10 dle| 11 dc1| 12 dc2| 13 dc3| 14 dc4| 15 nak| 16 syn| 17 etb|
| 18 can| 19 em | 1a sub| 1b esc| 1c fs | 1d gs | 1e rs | 1f us |
| 20 sp | 21  ! | 22  " | 23  # | 24  $ | 25  % | 26  & | 27  ' |
| 28  ( | 29  ) | 2a  * | 2b  + | 2c  , | 2d  - | 2e  . | 2f  / |
| 30  0 | 31  1 | 32  2 | 33  3 | 34  4 | 35  5 | 36  6 | 37  7 |
| 38  8 | 39  9 | 3a  : | 3b  ; | 3c  < | 3d  = | 3e  > | 3f  ? |
| 40  @ | 41  A | 42  B | 43  C | 44  D | 45  E | 46  F | 47  G |
| 48  H | 49  I | 4a  J | 4b  K | 4c  L | 4d  M | 4e  N | 4f  O |
| 50  P | 51  Q | 52  R | 53  S | 54  T | 55  U | 56  V | 57  W |
| 58  X | 59  Y | 5a  Z | 5b  [ | 5c  \ | 5d  ] | 5e  ^ | 5f  _ |
| 60  ` | 61  a | 62  b | 63  c | 64  d | 65  e | 66  f | 67  g |
| 68  h | 69  i | 6a  j | 6b  k | 6c  l | 6d  m | 6e  n | 6f  o |
| 70  p | 71  q | 72  r | 73  s | 74  t | 75  u | 76  v | 77  w |
| 78  x | 79  y | 7a  z | 7b  { | 7c  | | 7d  } | 7e  ~ | 7f del|

man ascii (Linux 2.4.18-14):

       Oct   Dec   Hex   Char           Oct   Dec   Hex   Char
       ------------------------------------------------------------
       000   0     00    NUL '\0'       100   64    40    @
       001   1     01    SOH            101   65    41    A
       002   2     02    STX            102   66    42    B
       003   3     03    ETX            103   67    43    C
       004   4     04    EOT            104   68    44    D
       005   5     05    ENQ            105   69    45    E
       006   6     06    ACK            106   70    46    F
       007   7     07    BEL '\a'       107   71    47    G
       010   8     08    BS  '\b'       110   72    48    H
       011   9     09    HT  '\t'       111   73    49    I
       012   10    0A    LF  '\n'       112   74    4A    J
       013   11    0B    VT  '\v'       113   75    4B    K
       014   12    0C    FF  '\f'       114   76    4C    L
       015   13    0D    CR  '\r'       115   77    4D    M
       016   14    0E    SO             116   78    4E    N
       017   15    0F    SI             117   79    4F    O
       020   16    10    DLE            120   80    50    P
       021   17    11    DC1            121   81    51    Q
       022   18    12    DC2            122   82    52    R
       023   19    13    DC3            123   83    53    S
       024   20    14    DC4            124   84    54    T
       025   21    15    NAK            125   85    55    U
       026   22    16    SYN            126   86    56    V
       027   23    17    ETB            127   87    57    W
       030   24    18    CAN            130   88    58    X
       031   25    19    EM             131   89    59    Y
       032   26    1A    SUB            132   90    5A    Z
       033   27    1B    ESC            133   91    5B    [
       034   28    1C    FS             134   92    5C    \   '\\'
       035   29    1D    GS             135   93    5D    ]
       036   30    1E    RS             136   94    5E    ^
       037   31    1F    US             137   95    5F    _
       040   32    20    SPACE          140   96    60    `
       041   33    21    !              141   97    61    a
       042   34    22    "              142   98    62    b
       043   35    23    #              143   99    63    c
       044   36    24    $              144   100   64    d
       045   37    25    %              145   101   65    e
       046   38    26    &              146   102   66    f
       047   39    27    '              147   103   67    g
       050   40    28    (              150   104   68    h
       051   41    29    )              151   105   69    i
       052   42    2A    *              152   106   6A    j
       053   43    2B    +              153   107   6B    k
       054   44    2C    ,              154   108   6C    l
       055   45    2D    -              155   109   6D    m
       056   46    2E    .              156   110   6E    n
       057   47    2F    /              157   111   6F    o
       060   48    30    0              160   112   70    p
       061   49    31    1              161   113   71    q
       062   50    32    2              162   114   72    r
       063   51    33    3              163   115   73    s
       064   52    34    4              164   116   74    t
       065   53    35    5              165   117   75    u
       066   54    36    6              166   118   76    v
       067   55    37    7              167   119   77    w
       070   56    38    8              170   120   78    x
       071   57    39    9              171   121   79    y
       072   58    3A    :              172   122   7A    z
       073   59    3B    ;              173   123   7B    {
       074   60    3C    <              174   124   7C    |
       075   61    3D    =              175   125   7D    }
       076   62    3E    >              176   126   7E    ~
       077   63    3F    ?              177   127   7F    DEL

man ascii (SunOS 5.1 maybe):

| 00 NUL| 01 SOH| 02 STX| 03 ETX|
| 04 EOT| 05 ENQ| 06 ACK| 07 BEL|
| 08 BS | 09 HT | 0A NL | 0B VT |
| 0C NP | 0D CR | 0E SO | 0F SI |
| 10 DLE| 11 DC1| 12 DC2| 13 DC3|
| 14 DC4| 15 NAK| 16 SYN| 17 ETB|
| 18 CAN| 19 EM | 1A SUB| 1B ESC|
| 1C FS | 1D GS | 1E RS | 1F US |
| 20 SP | 21  ! | 22  " | 23  # |
| 24  $ | 25  % | 26  & | 27  ' |
| 28  ( | 29  ) | 2A  * | 2B  + |
| 2C  , | 2D  - | 2E  . | 2F  / |
| 30  0 | 31  1 | 32  2 | 33  3 |
| 34  4 | 35  5 | 36  6 | 37  7 |
| 38  8 | 39  9 | 3A  : | 3B  ; |
| 3C  < | 3D  = | 3E  > | 3F  ? |
| 40  @ | 41  A | 42  B | 43  C |
| 44  D | 45  E | 46  F | 47  G |
| 48  H | 49  I | 4A  J | 4B  K |
| 4C  L | 4D  M | 4E  N | 4F  O |
| 50  P | 51  Q | 52  R | 53  S |
| 54  T | 55  U | 56  V | 57  W |
| 58  X | 59  Y | 5A  Z | 5B  [ |
| 5C  \ | 5D  ] | 5E  ^ | 5F  _ |
| 60  ` | 61  a | 62  b | 63  c |
| 64  d | 65  e | 66  f | 67  g |
| 68  h | 69  i | 6A  j | 6B  k |
| 6C  l | 6D  m | 6E  n | 6F  o |
| 70  p | 71  q | 72  r | 73  s |
| 74  t | 75  u | 76  v | 77  w |
| 78  x | 79  y | 7A  z | 7B  { |
| 7C  | | 7D  } | 7E  ~ | 7F DEL|

*******************************************************************************/

#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Desperado.h"

static const char *ascii[]={
    "NUL","SOH","STX","ETX",
    "EOT","ENQ","ACK","BEL",
    "BS", "HT", "LF", "VT",
    "FF", "CR", "SO", "SI",
    "DLE","DC1","DC2","DC3",
    "DC4","NAK","SYN","ETB",
    "CAN","EM", "SUB","ESC",
    "FS", "GS", "RS", "US",
    "SP", "DEL"
};

static const char *c[]={
    "NUL","SOH","STX","ETX",
    "EOT","ENQ","ACK","BEL",
    "BS", "HT", "NL", "VT",
    "NP", "CR", "SO", "SI",
    "DLE","DC1","DC2","DC3",
    "DC4","NAK","SYN","ETB",
    "CAN","EM", "SUB","ESC",
    "FS", "GS", "RS", "US",
    "SP", "DEL"
};

static const char *octal[]={
    "\\\\0",  "\\\\001","\\\\002","\\\\003",
    "\\\\004","\\\\005","\\\\006","\\\\a",
    "\\\\b",  "\\\\t",  "\\\\n",  "\\\\v",
    "\\\\f",  "\\\\r",  "\\\\016","\\\\017",
    "\\\\020","\\\\021","\\\\022","\\\\023",
    "\\\\024","\\\\025","\\\\026","\\\\027",
    "\\\\030","\\\\031","\\\\032","\\\\033",
    "\\\\034","\\\\035","\\\\036","\\\\037",
    "\\\\040","\\\\177"
};

static const char *hexadecimal[]={
    "\\\\0",   "\\\\x001","\\\\x002","\\\\x003",
    "\\\\x004","\\\\x005","\\\\x006","\\\\a",
    "\\\\b",   "\\\\t",   "\\\\n",   "\\\\v",
    "\\\\f",   "\\\\r",   "\\\\x00e","\\\\x00f",
    "\\\\x010","\\\\x011","\\\\x012","\\\\x013",
    "\\\\x014","\\\\x015","\\\\x016","\\\\x017",
    "\\\\x018","\\\\x019","\\\\x01a","\\\\x01b",
    "\\\\x01c","\\\\x01d","\\\\x01e","\\\\x01f",
    "\\\\x020","\\\\x07f"
};

int main() {
    Print printf(Platform::instance().output());

    int ch;
    const char *n;
    const char *a;
    const char *o;
    const char *h;
    char s[2];

    s[1] = '\0';
    for (ch = 0x00; 0xff >= ch; ++ch) {
        if ('\\' == ch) {
            n = "\\\\";
            a = "\\\\";
            o = "\\\\\\\\";
            h = "\\\\\\\\";
        } else if ('?' == ch) {
            n = "?";
            a = "?";
            o = "\\\\?";
            h = "\\\\?";
        } else if ('\'' == ch) {
            n = "'";
            a = "'";
            o = "\\\\'";
            h = "\\\\'";
        } else if ('"' == ch) {
            n = "\\\"";
            a = "\\\"";
            o = "\\\\\\\"";
            h = "\\\\\\\"";
        } else if (0x7f == ch) {
            n = ascii[0x21];
            a = c[0x21];
            o = octal[0x21];
            h = hexadecimal[0x21];
        } else if (' ' >= ch) {
            n = ascii[ch];
            a = c[ch];
            o = octal[ch];
            h = hexadecimal[ch];
        } else if ('~' >= ch) {
            s[0] = ch;
            n = s;
            a = s;
            o = s;
            h = s;
        } else {
            n = 0;
            a = 0;
            o = 0;
            h = 0;
        }

        if (0 == n) {
            printf("{ \"\\\\%03o\", \"\\\\%03o\", \"\\\\%03o\", \"\\\\x%03x\" }"
                ",\n",
                ch, ch, ch, ch);
        } else {
            printf("{ \"%s\", \"%s\", \"%s\", \"%s\" },\n",
                n, a, o, h);
        }
    }

    return 0;
}
