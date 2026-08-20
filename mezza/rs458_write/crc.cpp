/* -------------------------------------------------------------------------- */
/* FILENAME:    
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc / maidnl74@gmail.com)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION: 
   LICENSE:     GNU Lesser General Public License (See the end of this file for
                details)
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "crc.h"

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i;        /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_lo ^ *buffer++; /* calculate the CRC  */
        crc_lo = crc_hi ^ table_crc_hi[i];
        crc_hi = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}


/* -------------------------------------------------------------------------- */
/* LICENSE 
   -------   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA                                                                    */
/* -------------------------------------------------------------------------- */