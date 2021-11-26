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


#ifndef _MAX31827_REGISTERS_H_
#define _MAX31827_REGISTERS_H_


/*
 *  MAX31827 Registers
 */
#define MAX31827_R_TEMPERATURE      0x00
#define MAX31827_R_CFG              0x02
#define MAX31827_R_TH               0x04
#define MAX31827_R_TL               0x06
#define MAX31827_R_TH_HYST          0x08
#define MAX31827_R_TL_HYST          0x0A


/*
 *  Temperature Register
 */
#define MAX31827_F_TEMPERATURE_MASK 0x0FFF

/*
 *  Configuration Register
 */
#define MAX31827_F_CFG_ONESHOT_POS             (0)
#define MAX31827_F_CFG_ONESHOT                 (1<<MAX31827_F_CFG_ONESHOT_POS)

#define MAX31827_F_CFG_CONV_RATE_POS           (1)
#define MAX31827_F_CFG_CONV_RATE               (7<<MAX31827_F_CFG_CONV_RATE_POS)
//
#define MAX31827_S_CFG_CONV_RATE_SHUTDOWN      (0<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_1_DIV_64SEC   (1<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_1_DIV_32SEC   (2<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_1_DIV_16SEC   (3<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_1_DIV_4SEC    (4<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_1_DIV_1SEC    (5<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_4_DIV_1SEC    (6<<MAX31827_F_CFG_CONV_RATE_POS)
#define MAX31827_S_CFG_CONV_RATE_8_DIV_1SEC    (7<<MAX31827_F_CFG_CONV_RATE_POS)

//
#define MAX31827_F_CFG_PEC_ENABLE_POS          (4)
#define MAX31827_F_CFG_PEC_ENABLE              (1<<MAX31827_F_CFG_PEC_ENABLE_POS)

#define MAX31827_F_CFG_TIMEOUT_POS             (5)
#define MAX31827_F_CFG_TIMEOUT                 (1<<MAX31827_F_CFG_TIMEOUT_POS)

#define MAX31827_F_CFG_RESOLUTION_POS          (6)
#define MAX31827_F_CFG_RESOLUTION              (3<<MAX31827_F_CFG_RESOLUTION_POS)
//
#define MAX31827_S_CFG_RESOLUTION_8_BIT        (0<<MAX31827_F_CFG_RESOLUTION_POS)
#define MAX31827_S_CFG_RESOLUTION_9_BIT        (1<<MAX31827_F_CFG_RESOLUTION_POS)
#define MAX31827_S_CFG_RESOLUTION_10_BIT       (2<<MAX31827_F_CFG_RESOLUTION_POS)
#define MAX31827_S_CFG_RESOLUTION_12_BIT       (3<<MAX31827_F_CFG_RESOLUTION_POS)

#define MAX31827_F_CFG_ALARM_POL_POS           (8)
#define MAX31827_F_CFG_ALARM_POL               (1<<MAX31827_F_CFG_ALARM_POL_POS)

#define MAX31827_F_CFG_CMP_INT_POS             (9)
#define MAX31827_F_CFG_CMP_INT                 (1<<MAX31827_F_CFG_CMP_INT_POS)

#define MAX31827_F_CFG_FAULT_QUE_POS           (10)
#define MAX31827_F_CFG_FAULT_QUE               (3<<MAX31827_F_CFG_FAULT_QUE_POS)
#define MAX31827_S_CFG_FAULT_QUE_1             (0<<MAX31827_F_CFG_FAULT_QUE_POS)
#define MAX31827_S_CFG_FAULT_QUE_2             (1<<MAX31827_F_CFG_FAULT_QUE_POS)
#define MAX31827_S_CFG_FAULT_QUE_4             (2<<MAX31827_F_CFG_FAULT_QUE_POS)
#define MAX31827_S_CFG_FAULT_QUE_8             (3<<MAX31827_F_CFG_FAULT_QUE_POS)

#define MAX31827_F_CFG_PEC_ERR_POS             (13)
#define MAX31827_F_CFG_PEC_ERR                 (1<<MAX31827_F_CFG_PEC_ERR_POS)
#define MAX31827_F_CFG_UNDER_TEMP_STAT_POS     (14)
#define MAX31827_F_CFG_UNDER_TEMP_STAT         (1<<MAX31827_F_CFG_UNDER_TEMP_STAT_POS)
#define MAX31827_F_CFG_OVER_TEMP_STAT_POS      (15)
#define MAX31827_F_CFG_OVER_TEMP_STAT          (1<<MAX31827_F_CFG_OVER_TEMP_STAT_POS)


// I2C ADDR
#define MAX31827_I2C_ADDR_FOR_4_2_KOHM          (0x5F)
#define MAX31827_I2C_ADDR_FOR_5_0_KOHM          (0x5E)
#define MAX31827_I2C_ADDR_FOR_5_9_KOHM          (0x5D)
#define MAX31827_I2C_ADDR_FOR_7_1_KOHM          (0x5C)
#define MAX31827_I2C_ADDR_FOR_8_4_KOHM          (0x5B)
#define MAX31827_I2C_ADDR_FOR_10_0_KOHM         (0x5A)
#define MAX31827_I2C_ADDR_FOR_11_9_KOHM         (0x59)
#define MAX31827_I2C_ADDR_FOR_14_1_KOHM         (0x58)
#define MAX31827_I2C_ADDR_FOR_16_8_KOHM         (0x57)
#define MAX31827_I2C_ADDR_FOR_20_0_KOHM         (0x56)
#define MAX31827_I2C_ADDR_FOR_23_8_KOHM         (0x55)
#define MAX31827_I2C_ADDR_FOR_28_3_KOHM         (0x54)
#define MAX31827_I2C_ADDR_FOR_33_6_KOHM         (0x53)
#define MAX31827_I2C_ADDR_FOR_40_0_KOHM         (0x52)
#define MAX31827_I2C_ADDR_FOR_47_6_KOHM         (0x51)
#define MAX31827_I2C_ADDR_FOR_56_6_KOHM         (0x50)
#define MAX31827_I2C_ADDR_FOR_67_3_KOHM         (0x4F)
#define MAX31827_I2C_ADDR_FOR_80_0_KOHM         (0x4E)
#define MAX31827_I2C_ADDR_FOR_95_1_KOHM         (0x4D)
#define MAX31827_I2C_ADDR_FOR_113_1_KOHM        (0x4C)
#define MAX31827_I2C_ADDR_FOR_134_5_KOHM        (0x4B)
#define MAX31827_I2C_ADDR_FOR_160_0_KOHM        (0x4A)
#define MAX31827_I2C_ADDR_FOR_190_3_KOHM        (0x49)
#define MAX31827_I2C_ADDR_FOR_226_3_KOHM        (0x48)
#define MAX31827_I2C_ADDR_FOR_269_1_KOHM        (0x47)
#define MAX31827_I2C_ADDR_FOR_320_0_KOHM        (0x46)
#define MAX31827_I2C_ADDR_FOR_380_5_KOHM        (0x45)
#define MAX31827_I2C_ADDR_FOR_452_5_KOHM        (0x44)
#define MAX31827_I2C_ADDR_FOR_538_2_KOHM        (0x43)
#define MAX31827_I2C_ADDR_FOR_640_0_KOHM        (0x42)
#define MAX31827_I2C_ADDR_FOR_761_1_KOHM        (0x41)
#define MAX31827_I2C_ADDR_FOR_905_1_KOHM        (0x40)



#endif /* _MAX31827_REGISTERS_H_ */
