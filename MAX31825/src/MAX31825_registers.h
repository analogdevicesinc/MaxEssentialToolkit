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

#ifndef _MAX31825_REGISTERS_H_
#define _MAX31825_REGISTERS_H_


/*
 *  MAX31825 Registers
 */
#define MAX31825_R_TEMP_LSB      0x00
#define MAX31825_R_TEMP_MSB      0x01
#define MAX31825_R_STATUS        0x02
#define MAX31825_R_CFG           0x03
#define MAX31825_R_TH_MSB        0x04
#define MAX31825_R_TH_LSB        0x05
#define MAX31825_R_TL_LSB        0x06
#define MAX31825_R_TL_MSB        0x07
#define MAX31825_R_CRC           0x08


/*
 *  Status Register
 */
#define MAX31825_F_STATUS_ADDR_POS          (0)
#define MAX31825_F_STATUS_ADDR              (0x3F<<MAX31825_F_STATUS_ADDR_POS)
#define MAX31825_F_STATUS_TL_FAULT_POS      (6)
#define MAX31825_F_STATUS_TL_FAULT          (1<<MAX31825_F_STATUS_TL_FAULT_POS)
#define MAX31825_F_STATUS_TH_FAULT_POS      (7)
#define MAX31825_F_STATUS_TH_FAULT          (1<<MAX31825_F_STATUS_TH_FAULT_POS)


/*
 *  Configuration Register
 */
#define MAX31825_F_CFG_CONV_RATE_POS           (0)
#define MAX31825_F_CFG_CONV_RATE               (7<<MAX31825_F_CFG_CONV_RATE_POS)
// Values
#define MAX31825_V_CFG_CONV_RATE_SHUTDOWN      (0)
#define MAX31825_S_CFG_CONV_RATE_SHUTDOWN      (MAX31825_V_CFG_CONV_RATE_SHUTDOWN<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_1_DIV_64SEC   (1)
#define MAX31825_S_CFG_CONV_RATE_1_DIV_64SEC   (MAX31825_V_CFG_CONV_RATE_1_DIV_64SEC<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_1_DIV_32SEC   (2)
#define MAX31825_S_CFG_CONV_RATE_1_DIV_32SEC   (MAX31825_V_CFG_CONV_RATE_1_DIV_32SEC<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_1_DIV_16SEC   (3)
#define MAX31825_S_CFG_CONV_RATE_1_DIV_16SEC   (MAX31825_V_CFG_CONV_RATE_1_DIV_16SEC<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_1_DIV_4SEC    (4)
#define MAX31825_S_CFG_CONV_RATE_1_DIV_4SEC    (MAX31825_V_CFG_CONV_RATE_1_DIV_4SEC<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_1_DIV_1SEC    (5)
#define MAX31825_S_CFG_CONV_RATE_1_DIV_1SEC    (MAX31825_V_CFG_CONV_RATE_1_DIV_1SEC<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_4_DIV_1SEC    (6)
#define MAX31825_S_CFG_CONV_RATE_4_DIV_1SEC    (MAX31825_V_CFG_CONV_RATE_4_DIV_1SEC<<MAX31825_F_CFG_CONV_RATE_POS)
#define MAX31825_V_CFG_CONV_RATE_8_DIV_1SEC    (7)
#define MAX31825_S_CFG_CONV_RATE_8_DIV_1SEC    (MAX31825_V_CFG_CONV_RATE_8_DIV_1SEC<<MAX31825_F_CFG_CONV_RATE_POS)

#define MAX31825_F_CFG_CMP_INT_POS             (4)
#define MAX31825_F_CFG_CMP_INT                 (1<<MAX31825_F_CFG_CMP_INT_POS)

#define MAX31825_F_CFG_RESOLUTION_POS          (5)
#define MAX31825_F_CFG_RESOLUTION              (3<<MAX31825_F_CFG_RESOLUTION_POS)
// Values
#define MAX31825_V_CFG_RESOLUTION_8_BIT        (0)
#define MAX31825_S_CFG_RESOLUTION_8_BIT        (MAX31825_V_CFG_RESOLUTION_8_BIT<<MAX31825_F_CFG_RESOLUTION_POS)
#define MAX31825_V_CFG_RESOLUTION_9_BIT        (1)
#define MAX31825_S_CFG_RESOLUTION_9_BIT        (MAX31825_V_CFG_RESOLUTION_9_BIT<<MAX31825_F_CFG_RESOLUTION_POS)
#define MAX31825_V_CFG_RESOLUTION_10_BIT       (2)
#define MAX31825_S_CFG_RESOLUTION_10_BIT       (MAX31825_V_CFG_RESOLUTION_10_BIT<<MAX31825_F_CFG_RESOLUTION_POS)
#define MAX31825_V_CFG_RESOLUTION_12_BIT       (3)
#define MAX31825_S_CFG_RESOLUTION_12_BIT       (MAX31825_V_CFG_RESOLUTION_12_BIT<<MAX31825_F_CFG_RESOLUTION_POS)

#define MAX31825_F_CFG_FORMAT_POS              (7)
#define MAX31825_F_CFG_FORMAT                  (1<<MAX31825_F_CFG_FORMAT_POS)

// ADDR  A4:A0
#define MAX31825_ADDR_FOR_4_2_KOHM          (0x1F)
#define MAX31825_ADDR_FOR_5_0_KOHM          (0x1E)
#define MAX31825_ADDR_FOR_5_9_KOHM          (0x1D)
#define MAX31825_ADDR_FOR_7_1_KOHM          (0x1C)
#define MAX31825_ADDR_FOR_8_4_KOHM          (0x1B)
#define MAX31825_ADDR_FOR_10_0_KOHM         (0x1A)
#define MAX31825_ADDR_FOR_11_9_KOHM         (0x19)
#define MAX31825_ADDR_FOR_14_1_KOHM         (0x18)
#define MAX31825_ADDR_FOR_16_8_KOHM         (0x17)
#define MAX31825_ADDR_FOR_20_0_KOHM         (0x16)
#define MAX31825_ADDR_FOR_23_8_KOHM         (0x15)
#define MAX31825_ADDR_FOR_28_3_KOHM         (0x14)
#define MAX31825_ADDR_FOR_33_6_KOHM         (0x13)
#define MAX31825_ADDR_FOR_40_0_KOHM         (0x12)
#define MAX31825_ADDR_FOR_47_6_KOHM         (0x11)
#define MAX31825_ADDR_FOR_56_6_KOHM         (0x10)
#define MAX31825_ADDR_FOR_67_3_KOHM         (0x0F)
#define MAX31825_ADDR_FOR_80_0_KOHM         (0x0E)
#define MAX31825_ADDR_FOR_95_1_KOHM         (0x0D)
#define MAX31825_ADDR_FOR_113_1_KOHM        (0x0C)
#define MAX31825_ADDR_FOR_134_5_KOHM        (0x0B)
#define MAX31825_ADDR_FOR_160_0_KOHM        (0x0A)
#define MAX31825_ADDR_FOR_190_3_KOHM        (0x09)
#define MAX31825_ADDR_FOR_226_3_KOHM        (0x08)
#define MAX31825_ADDR_FOR_269_1_KOHM        (0x07)
#define MAX31825_ADDR_FOR_320_0_KOHM        (0x06)
#define MAX31825_ADDR_FOR_380_5_KOHM        (0x05)
#define MAX31825_ADDR_FOR_452_5_KOHM        (0x04)
#define MAX31825_ADDR_FOR_538_2_KOHM        (0x03)
#define MAX31825_ADDR_FOR_640_0_KOHM        (0x02)
#define MAX31825_ADDR_FOR_761_1_KOHM        (0x01)
#define MAX31825_ADDR_FOR_905_1_KOHM        (0x00)

#define MAX31825_FAMILY_CODE    0x3B


#endif /* _MAX31825_REGISTERS_H_ */
