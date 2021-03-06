:
# vi: set ts=4:

###############################################################################
#
#   Copyright 2005-2013 Digital Aggregates Corporation, Colorado, USA.
#   This file is part of the Digital Aggregates Grandote library.
#   
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Lesser General Public
#   License as published by the Free Software Foundation; either
#   version 2.1 of the License, or (at your option) any later version.
#
#   As a special exception, if other files instantiate templates or
#   use macros or inline functions from this file, or you compile
#   this file and link it with other works to produce a work based on
#   this file, this file does not by itself cause the resulting work
#   to be covered by the GNU Lesser General Public License. However
#   the source code for this file must still be made available in
#   accordance with the GNU Lesser General Public License.
#
#   This exception does not invalidate any other reasons why a work
#   based on this file might be covered by the GNU Lesser General
#   Public License.
#
#   Alternative commercial licensing terms are available from the copyright
#   holder. Contact Digital Aggregates Corporation for more information.
#
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General
#   Public License along with this library; if not, write to the
#   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
#   Boston, MA 02111-1307 USA, or http://www.gnu.org/copyleft/lesser.txt.
#
###############################################################################

CMD="`basename $0`"
ROOT=$(mktemp -d ${TMPDIR:-"/tmp"}/${CMD}.XXXXXXXXXX)
trap "rm -rf ${ROOT}" HUP INT TERM
FILE="lesser.txt"
SOURCE=$(dirname $(dirname $(dirname $(dirname $(dirname $(dirname $(which $0)))))))/${FILE}
PGM="unittestInputOutput"
ERRORS=0

echo "${CMD}: begin"

for OBJECT in D F N P S B; do
    for METHOD in c l s f b; do
        for IO in x s f; do

            INPUT="${ROOT}/${FILE}"
            OUTPUT="${ROOT}/${OBJECT}${METHOD}${IO}.txt"
            ERROR="${ROOT}/${OBJECT}${METHOD}${IO}.log"
			cp /dev/null ${ERROR}

            if [ "${IO}" = "s" ]; then
                OUT="1> ${OUTPUT}"
                IN="< ${INPUT}"
                cat ${SOURCE} > ${INPUT}
            elif [ "${IO}" = "f" ]; then
                OUT="-o ${OUTPUT}"
                IN="${INPUT}"
                cp ${SOURCE} ${INPUT}
            else
                OUT="1> ${OUTPUT}"
                IN="< ${INPUT}"
                cp /dev/null ${INPUT}
            fi
            ERR="2>&1 | tee -a ${ERROR}"

            echo "${PGM} ${DEBUG} ${VERBOSE} -${OBJECT} -${METHOD} ${OUT} ${IN} ${ERR}"
            eval ${PGM} ${DEBUG} ${VERBOSE} -${OBJECT} -${METHOD} ${OUT} ${IN} ${ERR}
            RC=$?
            if [ ${RC} -ne 0 ]; then
                echo "${CMD}: ${PGM} ${OBJECT} ${METHOD} ${IO} ${RC}!"
            fi
            ERRORS=`expr ${ERRORS} + ${RC}`

            if [ "${OBJECT}" != "N" ]; then
                ls -l ${INPUT} ${OUTPUT} 2>&1 | tee -a ${ERROR}
                diff -q ${INPUT} ${OUTPUT} 2>&1 | tee -a ${ERROR}
                RC=$?
                if [ ${RC} -ne 0 ]; then
                    echo "${CMD}: diff ${OBJECT} ${METHOD} ${IO} ${RC} ${INPUT} ${OUTPUT}!"
                    diff ${INPUT} ${OUTPUT}
                fi
                ERRORS=`expr ${ERRORS} + ${RC}`
            fi
        done
    done
done

grep 'errors=[^0]' ${ROOT}/*.log
echo "$CMD: end errors="${ERRORS}

rm -rf ${ROOT}

exit ${ERRORS}
