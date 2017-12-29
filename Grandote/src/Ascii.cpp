/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Ascii class.
 *
 *  @see    Ascii
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/target.h"
#include "com/diag/grandote/Ascii.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/Platform.h"


namespace com { namespace diag { namespace grandote {


//
//  ASCII table.
//
const Ascii::Entry Ascii::table[] = {
    { "NUL", "NUL", "\\0", "\\0" },
    { "SOH", "SOH", "\\1", "\\x1" },
    { "STX", "STX", "\\2", "\\x2" },
    { "ETX", "ETX", "\\3", "\\x3" },
    { "EOT", "EOT", "\\4", "\\x4" },
    { "ENQ", "ENQ", "\\5", "\\x5" },
    { "ACK", "ACK", "\\6", "\\x6" },
    { "BEL", "BEL", "\\a", "\\a" },
    { "BS", "BS", "\\b", "\\b" },
    { "HT", "HT", "\\t", "\\t" },
    { "LF", "NL", "\\n", "\\n" },
    { "VT", "VT", "\\v", "\\v" },
    { "FF", "NP", "\\f", "\\f" },
    { "CR", "CR", "\\r", "\\r" },
    { "SO", "SO", "\\16", "\\xe" },
    { "SI", "SI", "\\17", "\\xf" },
    { "DLE", "DLE", "\\20", "\\x10" },
    { "DC1", "DC1", "\\21", "\\x11" },
    { "DC2", "DC2", "\\22", "\\x12" },
    { "DC3", "DC3", "\\23", "\\x13" },
    { "DC4", "DC4", "\\24", "\\x14" },
    { "NAK", "NAK", "\\25", "\\x15" },
    { "SYN", "SYN", "\\26", "\\x16" },
    { "ETB", "ETB", "\\27", "\\x17" },
    { "CAN", "CAN", "\\30", "\\x18" },
    { "EM", "EM", "\\31", "\\x19" },
    { "SUB", "SUB", "\\32", "\\x1a" },
    { "ESC", "ESC", "\\33", "\\x1b" },
    { "FS", "FS", "\\34", "\\x1c" },
    { "GS", "GS", "\\35", "\\x1d" },
    { "RS", "RS", "\\36", "\\x1e" },
    { "US", "US", "\\37", "\\x1f" },
    { "SP", "SP", "\\40", "\\x20" },
    { "!", "!", "!", "!" },
    { "\"", "\"", "\"", "\"" },
    { "#", "#", "#", "#" },
    { "$", "$", "$", "$" },
    { "%", "%", "%", "%" },
    { "&", "&", "&", "&" },
    { "'", "'", "'", "'" },
    { "(", "(", "(", "(" },
    { ")", ")", ")", ")" },
    { "*", "*", "*", "*" },
    { "+", "+", "+", "+" },
    { ",", ",", ",", "," },
    { "-", "-", "-", "-" },
    { ".", ".", ".", "." },
    { "/", "/", "/", "/" },
    { "0", "0", "0", "0" },
    { "1", "1", "1", "1" },
    { "2", "2", "2", "2" },
    { "3", "3", "3", "3" },
    { "4", "4", "4", "4" },
    { "5", "5", "5", "5" },
    { "6", "6", "6", "6" },
    { "7", "7", "7", "7" },
    { "8", "8", "8", "8" },
    { "9", "9", "9", "9" },
    { ":", ":", ":", ":" },
    { ";", ";", ";", ";" },
    { "<", "<", "<", "<" },
    { "=", "=", "=", "=" },
    { ">", ">", ">", ">" },
    { "?", "?", "?", "?" },
    { "@", "@", "@", "@" },
    { "A", "A", "A", "A" },
    { "B", "B", "B", "B" },
    { "C", "C", "C", "C" },
    { "D", "D", "D", "D" },
    { "E", "E", "E", "E" },
    { "F", "F", "F", "F" },
    { "G", "G", "G", "G" },
    { "H", "H", "H", "H" },
    { "I", "I", "I", "I" },
    { "J", "J", "J", "J" },
    { "K", "K", "K", "K" },
    { "L", "L", "L", "L" },
    { "M", "M", "M", "M" },
    { "N", "N", "N", "N" },
    { "O", "O", "O", "O" },
    { "P", "P", "P", "P" },
    { "Q", "Q", "Q", "Q" },
    { "R", "R", "R", "R" },
    { "S", "S", "S", "S" },
    { "T", "T", "T", "T" },
    { "U", "U", "U", "U" },
    { "V", "V", "V", "V" },
    { "W", "W", "W", "W" },
    { "X", "X", "X", "X" },
    { "Y", "Y", "Y", "Y" },
    { "Z", "Z", "Z", "Z" },
    { "[", "[", "[", "[" },
    { "\\", "\\", "\\\\", "\\\\" },
    { "]", "]", "]", "]" },
    { "^", "^", "^", "^" },
    { "_", "_", "_", "_" },
    { "`", "`", "`", "`" },
    { "a", "a", "a", "a" },
    { "b", "b", "b", "b" },
    { "c", "c", "c", "c" },
    { "d", "d", "d", "d" },
    { "e", "e", "e", "e" },
    { "f", "f", "f", "f" },
    { "g", "g", "g", "g" },
    { "h", "h", "h", "h" },
    { "i", "i", "i", "i" },
    { "j", "j", "j", "j" },
    { "k", "k", "k", "k" },
    { "l", "l", "l", "l" },
    { "m", "m", "m", "m" },
    { "n", "n", "n", "n" },
    { "o", "o", "o", "o" },
    { "p", "p", "p", "p" },
    { "q", "q", "q", "q" },
    { "r", "r", "r", "r" },
    { "s", "s", "s", "s" },
    { "t", "t", "t", "t" },
    { "u", "u", "u", "u" },
    { "v", "v", "v", "v" },
    { "w", "w", "w", "w" },
    { "x", "x", "x", "x" },
    { "y", "y", "y", "y" },
    { "z", "z", "z", "z" },
    { "{", "{", "{", "{" },
    { "|", "|", "|", "|" },
    { "}", "}", "}", "}" },
    { "~", "~", "~", "~" },
    { "DEL", "DEL", "\\177", "\\x7f" },
    { "\\200", "\\200", "\\200", "\\x80" },
    { "\\201", "\\201", "\\201", "\\x81" },
    { "\\202", "\\202", "\\202", "\\x82" },
    { "\\203", "\\203", "\\203", "\\x83" },
    { "\\204", "\\204", "\\204", "\\x84" },
    { "\\205", "\\205", "\\205", "\\x85" },
    { "\\206", "\\206", "\\206", "\\x86" },
    { "\\207", "\\207", "\\207", "\\x87" },
    { "\\210", "\\210", "\\210", "\\x88" },
    { "\\211", "\\211", "\\211", "\\x89" },
    { "\\212", "\\212", "\\212", "\\x8a" },
    { "\\213", "\\213", "\\213", "\\x8b" },
    { "\\214", "\\214", "\\214", "\\x8c" },
    { "\\215", "\\215", "\\215", "\\x8d" },
    { "\\216", "\\216", "\\216", "\\x8e" },
    { "\\217", "\\217", "\\217", "\\x8f" },
    { "\\220", "\\220", "\\220", "\\x90" },
    { "\\221", "\\221", "\\221", "\\x91" },
    { "\\222", "\\222", "\\222", "\\x92" },
    { "\\223", "\\223", "\\223", "\\x93" },
    { "\\224", "\\224", "\\224", "\\x94" },
    { "\\225", "\\225", "\\225", "\\x95" },
    { "\\226", "\\226", "\\226", "\\x96" },
    { "\\227", "\\227", "\\227", "\\x97" },
    { "\\230", "\\230", "\\230", "\\x98" },
    { "\\231", "\\231", "\\231", "\\x99" },
    { "\\232", "\\232", "\\232", "\\x9a" },
    { "\\233", "\\233", "\\233", "\\x9b" },
    { "\\234", "\\234", "\\234", "\\x9c" },
    { "\\235", "\\235", "\\235", "\\x9d" },
    { "\\236", "\\236", "\\236", "\\x9e" },
    { "\\237", "\\237", "\\237", "\\x9f" },
    { "\\240", "\\240", "\\240", "\\xa0" },
    { "\\241", "\\241", "\\241", "\\xa1" },
    { "\\242", "\\242", "\\242", "\\xa2" },
    { "\\243", "\\243", "\\243", "\\xa3" },
    { "\\244", "\\244", "\\244", "\\xa4" },
    { "\\245", "\\245", "\\245", "\\xa5" },
    { "\\246", "\\246", "\\246", "\\xa6" },
    { "\\247", "\\247", "\\247", "\\xa7" },
    { "\\250", "\\250", "\\250", "\\xa8" },
    { "\\251", "\\251", "\\251", "\\xa9" },
    { "\\252", "\\252", "\\252", "\\xaa" },
    { "\\253", "\\253", "\\253", "\\xab" },
    { "\\254", "\\254", "\\254", "\\xac" },
    { "\\255", "\\255", "\\255", "\\xad" },
    { "\\256", "\\256", "\\256", "\\xae" },
    { "\\257", "\\257", "\\257", "\\xaf" },
    { "\\260", "\\260", "\\260", "\\xb0" },
    { "\\261", "\\261", "\\261", "\\xb1" },
    { "\\262", "\\262", "\\262", "\\xb2" },
    { "\\263", "\\263", "\\263", "\\xb3" },
    { "\\264", "\\264", "\\264", "\\xb4" },
    { "\\265", "\\265", "\\265", "\\xb5" },
    { "\\266", "\\266", "\\266", "\\xb6" },
    { "\\267", "\\267", "\\267", "\\xb7" },
    { "\\270", "\\270", "\\270", "\\xb8" },
    { "\\271", "\\271", "\\271", "\\xb9" },
    { "\\272", "\\272", "\\272", "\\xba" },
    { "\\273", "\\273", "\\273", "\\xbb" },
    { "\\274", "\\274", "\\274", "\\xbc" },
    { "\\275", "\\275", "\\275", "\\xbd" },
    { "\\276", "\\276", "\\276", "\\xbe" },
    { "\\277", "\\277", "\\277", "\\xbf" },
    { "\\300", "\\300", "\\300", "\\xc0" },
    { "\\301", "\\301", "\\301", "\\xc1" },
    { "\\302", "\\302", "\\302", "\\xc2" },
    { "\\303", "\\303", "\\303", "\\xc3" },
    { "\\304", "\\304", "\\304", "\\xc4" },
    { "\\305", "\\305", "\\305", "\\xc5" },
    { "\\306", "\\306", "\\306", "\\xc6" },
    { "\\307", "\\307", "\\307", "\\xc7" },
    { "\\310", "\\310", "\\310", "\\xc8" },
    { "\\311", "\\311", "\\311", "\\xc9" },
    { "\\312", "\\312", "\\312", "\\xca" },
    { "\\313", "\\313", "\\313", "\\xcb" },
    { "\\314", "\\314", "\\314", "\\xcc" },
    { "\\315", "\\315", "\\315", "\\xcd" },
    { "\\316", "\\316", "\\316", "\\xce" },
    { "\\317", "\\317", "\\317", "\\xcf" },
    { "\\320", "\\320", "\\320", "\\xd0" },
    { "\\321", "\\321", "\\321", "\\xd1" },
    { "\\322", "\\322", "\\322", "\\xd2" },
    { "\\323", "\\323", "\\323", "\\xd3" },
    { "\\324", "\\324", "\\324", "\\xd4" },
    { "\\325", "\\325", "\\325", "\\xd5" },
    { "\\326", "\\326", "\\326", "\\xd6" },
    { "\\327", "\\327", "\\327", "\\xd7" },
    { "\\330", "\\330", "\\330", "\\xd8" },
    { "\\331", "\\331", "\\331", "\\xd9" },
    { "\\332", "\\332", "\\332", "\\xda" },
    { "\\333", "\\333", "\\333", "\\xdb" },
    { "\\334", "\\334", "\\334", "\\xdc" },
    { "\\335", "\\335", "\\335", "\\xdd" },
    { "\\336", "\\336", "\\336", "\\xde" },
    { "\\337", "\\337", "\\337", "\\xdf" },
    { "\\340", "\\340", "\\340", "\\xe0" },
    { "\\341", "\\341", "\\341", "\\xe1" },
    { "\\342", "\\342", "\\342", "\\xe2" },
    { "\\343", "\\343", "\\343", "\\xe3" },
    { "\\344", "\\344", "\\344", "\\xe4" },
    { "\\345", "\\345", "\\345", "\\xe5" },
    { "\\346", "\\346", "\\346", "\\xe6" },
    { "\\347", "\\347", "\\347", "\\xe7" },
    { "\\350", "\\350", "\\350", "\\xe8" },
    { "\\351", "\\351", "\\351", "\\xe9" },
    { "\\352", "\\352", "\\352", "\\xea" },
    { "\\353", "\\353", "\\353", "\\xeb" },
    { "\\354", "\\354", "\\354", "\\xec" },
    { "\\355", "\\355", "\\355", "\\xed" },
    { "\\356", "\\356", "\\356", "\\xee" },
    { "\\357", "\\357", "\\357", "\\xef" },
    { "\\360", "\\360", "\\360", "\\xf0" },
    { "\\361", "\\361", "\\361", "\\xf1" },
    { "\\362", "\\362", "\\362", "\\xf2" },
    { "\\363", "\\363", "\\363", "\\xf3" },
    { "\\364", "\\364", "\\364", "\\xf4" },
    { "\\365", "\\365", "\\365", "\\xf5" },
    { "\\366", "\\366", "\\366", "\\xf6" },
    { "\\367", "\\367", "\\367", "\\xf7" },
    { "\\370", "\\370", "\\370", "\\xf8" },
    { "\\371", "\\371", "\\371", "\\xf9" },
    { "\\372", "\\372", "\\372", "\\xfa" },
    { "\\373", "\\373", "\\373", "\\xfb" },
    { "\\374", "\\374", "\\374", "\\xfc" },
    { "\\375", "\\375", "\\375", "\\xfd" },
    { "\\376", "\\376", "\\376", "\\xfe" },
    { "\\377", "\\377", "\\377", "\\xff" }
};


//
//  Destructor.
//
Ascii::~Ascii() {
}


//
//  Show this object on the output object.
//
void Ascii::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


} } }
