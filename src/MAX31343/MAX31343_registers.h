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

#ifndef _MAX31343_REGISTERS_H_
#define _MAX31343_REGISTERS_H_

#define MAX31343_I2C_ADDRESS                      0x68

#define MAX31343_R_INT_STATUS_REG_ADDR	        ( 0x00 )
#define MAX31343_R_INT_EN_REG_ADDR		        ( 0x01 )
#define MAX31343_R_RTC_RESET_ADDR			    ( 0x02 )
#define MAX31343_R_CONFIG_REG1_ADDR		        ( 0x03 )
#define MAX31343_R_CONFIG_REG2_ADDR		        ( 0x04 )
#define MAX31343_R_TIMER_CONFIG_ADDR		    ( 0x05 )
#define MAX31343_R_SECONDS_ADDR			        ( 0x06 )
#define MAX31343_R_MINUTES_ADDR			        ( 0x07 )
#define MAX31343_R_HOURS_ADDR				    ( 0x08 )
#define MAX31343_R_DAY_ADDR				        ( 0x09 )
#define MAX31343_R_DATE_ADDR				    ( 0x0A )
#define MAX31343_R_MONTH_ADDR				    ( 0x0B )
#define MAX31343_R_YEAR_ADDR				    ( 0x0C )
#define MAX31343_R_ALM1_SEC_ADDR			    ( 0x0D )
#define MAX31343_R_ALM1_MIN_ADDR			    ( 0x0E )
#define MAX31343_R_ALM1_HRS_ADDR			    ( 0x0F )
#define MAX31343_R_ALM1DAY_DATE_ADDR		    ( 0x10 )
#define MAX31343_R_ALM1_MON_ADDR			    ( 0x11 )
#define MAX31343_R_ALM1_YEAR_ADDR			    ( 0x12 )
#define MAX31343_R_ALM2_MIN_ADDR			    ( 0x13 )
#define MAX31343_R_ALM2_HRS_ADDR			    ( 0x14 )
#define MAX31343_R_ALM2DAY_DATE_ADDR		    ( 0x15 )
#define MAX31343_R_TIMER_COUNT_ADDR		        ( 0x16 )
#define MAX31343_R_TIMER_INIT_ADDR		        ( 0x17 )
#define MAX31343_R_PWR_MGMT_REG_ADDR		    ( 0x18 )
#define MAX31343_R_TRICKLE_REG_ADDR		        ( 0x19 )
#define MAX31343_R_TEMP_MSB_ADDR			    ( 0x1A )
#define MAX31343_R_TEMP_LSB_ADDR			    ( 0x1B )
#define MAX31343_R_TS_CONFIG_ADDR			    ( 0x1C )


#endif /* _MAX31343_REGISTERS_H_ */
