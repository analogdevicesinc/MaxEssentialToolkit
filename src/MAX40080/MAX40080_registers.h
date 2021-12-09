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
#define MAX40080_R_THRSHLD_OVER_CURRENT     0x04 
#define MAX40080_R_THRSHLD_OVER_VOLTAGE     0x05 
#define MAX40080_R_THRSHLD_UNDER_VOLTAGE    0x06 
#define MAX40080_R_WAKEUP_CURRENT           0x07 
#define MAX40080_R_MAX_PEAK_CURRENT         0x08
#define MAX40080_R_FIFO_CFG                 0x0A
#define MAX40080_R_CURRENT                  0x0C
#define MAX40080_R_VOLTAGE                  0x0E
#define MAX40080_R_CURRENT_AND_VOLTAGE      0x10
#define MAX40080_R_INT_EN                   0x14


/*
 *  Status Registers
 */
#define MAX40080_F_STATUS_WAKEUP_POS                0
#define MAX40080_F_STATUS_WAKEUP                    (1<<MAX40080_F_STATUS_WAKEUP_POS)
#define MAX40080_F_STATUS_CONV_READY_POS            1
#define MAX40080_F_STATUS_CONV_READY                (1<<MAX40080_F_STATUS_CONV_READY_POS)
#define MAX40080_F_STATUS_OVERFLOW_I_POS            2
#define MAX40080_F_STATUS_OVERFLOW_I                (1<<MAX40080_F_STATUS_OVERFLOW_I_POS)
#define MAX40080_F_STATUS_OVERFLOW_V_POS            3
#define MAX40080_F_STATUS_OVERFLOW_V                (1<<MAX40080_F_STATUS_OVERFLOW_V_POS)
#define MAX40080_F_STATUS_UNDERFLOW_V_POS           4
#define MAX40080_F_STATUS_UNDERFLOW_V               (1<<MAX40080_F_STATUS_UNDERFLOW_V_POS)
#define MAX40080_F_STATUS_I2C_TIMEOUT_POS           5
#define MAX40080_F_STATUS_I2C_TIMEOUT               (1<<MAX40080_F_STATUS_I2C_TIMEOUT_POS)
#define MAX40080_F_STATUS_FIFO_ALARM_POS            6
#define MAX40080_F_STATUS_FIFO_ALARM                (1<<MAX40080_F_STATUS_FIFO_ALARM_POS)
#define MAX40080_F_STATUS_FIFO_OVERFLOW_POS         7
#define MAX40080_F_STATUS_FIFO_OVERFLOW             (1<<MAX40080_F_STATUS_FIFO_OVERFLOW_POS)
#define MAX40080_F_STATUS_FIFO_DATA_COUNT_POS       8
#define MAX40080_F_STATUS_FIFO_DATA_COUNT           (0x3F<<MAX40080_F_STATUS_FIFO_DATA_COUNT_POS)


/*
 *  Configuration Registers
 */
#define MAX40080_F_CFG_MODE_POS                     0
#define MAX40080_F_CFG_MODE                         (0x07<<MAX40080_F_CFG_MODE_POS)
#define MAX40080_F_CFG_I2C_TIMEOUT_POS              3
#define MAX40080_F_CFG_I2C_TIMEOUT                  (1<<MAX40080_F_CFG_I2C_TIMEOUT_POS)
#define MAX40080_F_CFG_ALERT_POS                    4
#define MAX40080_F_CFG_ALERT                        (1<<MAX40080_F_CFG_ALERT_POS)
#define MAX40080_F_CFG_PEC_POS                      5
#define MAX40080_F_CFG_PEC                          (1<<MAX40080_F_CFG_PEC_POS)
#define MAX40080_F_CFG_INPUT_RANGE_POS              6
#define MAX40080_F_CFG_INPUT_RANGE                  (1<<MAX40080_F_CFG_INPUT_RANGE_POS)
#define MAX40080_F_CFG_STAY_HS_MODE_POS             7
#define MAX40080_F_CFG_STAY_HS_MODE                 (1<<MAX40080_F_CFG_STAY_HS_MODE_POS)
#define MAX40080_F_CFG_ADC_SAMPLE_RATE_POS          8
#define MAX40080_F_CFG_ADC_SAMPLE_RATE              (0x0f<<MAX40080_F_CFG_ADC_SAMPLE_RATE_POS)
#define MAX40080_F_CFG_DIGITAL_FILTER_POS           12
#define MAX40080_F_CFG_DIGITAL_FILTER               (0x07<<MAX40080_F_CFG_DIGITAL_FILTER_POS)

/*
 *  FIFO Configuration Registers
 */
#define MAX40080_F_FIFO_CFG_STORE_IV_POS             0
#define MAX40080_F_FIFO_CFG_STORE_IV                 (0x03<<MAX40080_F_FIFO_CFG_STORE_IV_POS)
#define MAX40080_F_FIFO_CFG_OVERFLOW_WARNING_POS     8
#define MAX40080_F_FIFO_CFG_OVERFLOW_WARNING         (0x3F<<MAX40080_F_FIFO_CFG_OVERFLOW_WARNING_POS)
#define MAX40080_F_FIFO_CFG_ROLLOVER_POS             14
#define MAX40080_F_FIFO_CFG_ROLLOVER                 (1<<MAX40080_F_FIFO_CFG_ROLLOVER_POS)
#define MAX40080_F_FIFO_CFG_FLUSH_POS                15
#define MAX40080_F_FIFO_CFG_FLUSH                    (1<<MAX40080_F_FIFO_CFG_FLUSH_POS)



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
