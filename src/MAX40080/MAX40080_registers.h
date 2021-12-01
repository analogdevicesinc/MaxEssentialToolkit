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


#ifndef _MAX40080_REGISTERS_H_
#define _MAX40080_REGISTERS_H_


/*
 *  MAX40080 Registers
 */
#define MAX40080_R_CFG                      0x00
#define MAX40080_R_STATUS                   0x02
#define MAX40080_R_THRSHLD_OVER_CURRENT     0x04 // Over-current threshold. When the measured current is higher than this value an alert is issued on ALERT_ and one status register bit is set
#define MAX40080_R_THRSHLD_OVER_VOLTAGE     0x05 // Over-voltage threshold. When the measured voltage is higher than this value an alert is issued on ALERT_ and one status register bit is set
#define MAX40080_R_THRSHLD_UNDER_VOLTAGE    0x06 // Under-voltage threshold. When the measured voltage is lower than this value an alert is issued on ALERT_ and one status register bit is set
#define MAX40080_R_WAKEUP_CURRENT           0x07 // Wake-up current threshold when in Low-Power Mode. When the measured current is higher than this value the device will switch into Active Mode. Additionally, an alert is issued on ALERT_ and one status register bit is set
#define MAX40080_R_MAX_PEAK_CURRENT         0x08
#define MAX40080_R_FIFO_CFG                 0x0A
#define MAX40080_R_CURRENT                  0x0C
#define MAX40080_R_VOLTAGE                  0x0E
#define MAX40080_R_CURRENT_AND_VOLTAGE      0x10
#define MAX40080_R_INT_EN                   0x14



// I2C ADDR, A6 A5: 01
#define MAX40080_DEFAULT_I2C_ADDR_FOR_115_KOHM          (0x20)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM          (0x21)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_86_6_KOHM         (0x22)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_75_KOHM           (0x23)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_64_9_KOHM         (0x24)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_56_2_KOHM         (0x25)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_48_7_KOHM         (0x26)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_42_2_KOHM         (0x27)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_36_5_KOHM         (0x28)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_30_9_KOHM         (0x29)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_26_1_KOHM         (0x2A)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_21_5_KOHM         (0x2B)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_16_9_KOHM         (0x2C)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_12_4_KOHM         (0x2D)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_8_06_KOHM         (0x2E)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_3_74_KOHM         (0x2F)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_2_87_KOHM         (0x30)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_2_49_KOHM         (0x31)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_2_15_KOHM         (0x32)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_1_87_KOHM         (0x33)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_1_62_KOHM         (0x34)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_1_4_KOHM          (0x35)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_1_21_KOHM         (0x36)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_1_05_KOHM         (0x37)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_909_KOHM        (0x38)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_768_KOHM        (0x39)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_649_KOHM        (0x3A)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_536_KOHM        (0x3B)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_422_KOHM        (0x3C)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_309_KOHM        (0x3D)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_200_KOHM        (0x3E)
#define MAX40080_DEFAULT_I2C_ADDR_FOR_0_0953_KOHM       (0x3F)



#endif /* _MAX40080_REGISTERS_H_ */
