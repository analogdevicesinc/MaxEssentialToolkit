/*******************************************************************************
* Copyright (C) 2021 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/

#ifndef _MAX31341_REGS_H_
#define _MAX31341_REGS_H_


#define MAX31341_I2C_ADDRESS                       0x69

// Registers
#define  MAX31341_REG_CONFIG_1                    ( 0x00 )
#define  MAX31341_REG_CONFIG_2                    ( 0x01 )
#define  MAX31341_REG_INT_POLARITY_CONFIG         ( 0x02 )
#define  MAX31341_REG_TIMER_CONFIG                ( 0x03 )
#define  MAX31341_REG_INT_EN                      ( 0x04 )
#define  MAX31341_REG_INT_STATUS                  ( 0x05 )
#define  MAX31341_REG_SECONDS                     ( 0x06 )
#define  MAX31341_REG_MINUTES                     ( 0x07 )
#define  MAX31341_REG_HOURS                       ( 0x08 )
#define  MAX31341_REG_DAY                         ( 0x09 )
#define  MAX31341_REG_DATE                        ( 0x0A )
#define  MAX31341_REG_MONTH                       ( 0x0B )
#define  MAX31341_REG_YEAR                        ( 0x0C )
#define  MAX31341_REG_ALM1_SEC                    ( 0x0D )
#define  MAX31341_REG_ALM1_MIN                    ( 0x0E )
#define  MAX31341_REG_ALM1_HRS                    ( 0x0F )
#define  MAX31341_REG_ALM1DAY_DATE                ( 0x10 )
#define  MAX31341_REG_ALM2_MIN                    ( 0x11 )
#define  MAX31341_REG_ALM2_HRS                    ( 0x12 )
#define  MAX31341_REG_ALM2DAY_DATE                ( 0x13 )
#define  MAX31341_REG_TIMER_COUNT                 ( 0x14 )
#define  MAX31341_REG_TIMER_INIT                  ( 0x15 )
#define  MAX31341_REG_RAM_START                   ( 0x16 )
#define  MAX31341_REG_RAM_END                     ( 0x55 )
#define  MAX31341_REG_PWR_MGMT                    ( 0x56 )
#define  MAX31341_REG_TRICKLE                     ( 0x57 )
#define  MAX31341_REG_CLOCK_SYNC                  ( 0x58 )
#define  MAX31341_REG_REV_ID                      ( 0x59 )


#endif /* _MAX31341_REGS_H_ */
