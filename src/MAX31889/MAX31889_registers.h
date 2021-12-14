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


#ifndef _MAX31889_REGISTERS_H_
#define _MAX31889_REGISTERS_H_


/*
 *  MAX31889 Registers
 */
#define MAX31889_R_STATUS                     0x00
#define MAX31889_R_INT_EN                     0x01
#define MAX31889_R_FIFO_WR_PTR                0x04
#define MAX31889_R_FIFO_RD_PTR                0x05
#define MAX31889_R_FIFO_OVF_CNT               0x06
#define MAX31889_R_FIFO_DATA_COUNTER          0x07
#define MAX31889_R_FIFO_DATA                  0x08
#define MAX31889_R_FIFO_CONF                  0x09
#define MAX31889_R_FIFO_CONF2                 0x0A
#define MAX31889_R_SYS_CTRL                   0x0C
#define MAX31889_R_ALARM_HI_MSB               0x10
#define MAX31889_R_ALARM_HI_LSB               0x11
#define MAX31889_R_ALARM_LO_MSB               0x12
#define MAX31889_R_ALARM_LO_LSB               0x13
#define MAX31889_R_TEMP_SENS_SETUP            0x14
#define MAX31889_R_GPIO_SETUP                 0x20
#define MAX31889_R_GPIO_CTRL                  0x21
#define MAX31889_R_ROM_ID_1                   0x30
#define MAX31889_R_ROM_ID_2                   0x31
#define MAX31889_R_ROM_ID_3                   0x32
#define MAX31889_R_ROM_ID_4                   0x33
#define MAX31889_R_ROM_ID_5                   0x34
#define MAX31889_R_ROM_ID_6                   0x35
#define MAX31889_R_ROM_ID_7                   0x36
#define MAX31889_R_PART_IDF                   0xFF

/*
 *  Status Register
 */
#define MAX31889_F_STATUS_TEMP_RDY_POS         (0)
#define MAX31889_F_STATUS_TEMP_RDY             (1<<MAX31889_F_STATUS_TEMP_RDY_POS)
#define MAX31889_F_STATUS_TEMP_HI_POS          (1)
#define MAX31889_F_STATUS_TEMP_HI              (1<<MAX31889_F_STATUS_TEMP_HI_POS)
#define MAX31889_F_STATUS_TEMP_LO_POS          (2)
#define MAX31889_F_STATUS_TEMP_LO              (1<<MAX31889_F_STATUS_TEMP_LO_POS)
#define MAX31889_F_STATUS_A_FULL_POS           (7)
#define MAX31889_F_STATUS_A_FULL               (1<<MAX31889_F_STATUS_A_FULL_POS)

/*
 *  Interrupt Register
 */
#define MAX31889_F_INT_EN_TEMP_RDY_POS         (0)
#define MAX31889_F_INT_EN_TEMP_RDY             (1<<MAX31889_F_INT_EN_TEMP_RDY_POS)
#define MAX31889_F_INT_EN_TEMP_HI_POS          (1)
#define MAX31889_F_INT_EN_TEMP_HI              (1<<MAX31889_F_INT_EN_TEMP_HI_POS)
#define MAX31889_F_INT_EN_TEMP_LO_POS          (2)
#define MAX31889_F_INT_EN_TEMP_LO              (1<<MAX31889_F_INT_EN_TEMP_LO_POS)
#define MAX31889_F_INT_EN_A_FULL_POS           (7)
#define MAX31889_F_INT_EN_A_FULL               (1<<MAX31889_F_INT_EN_A_FULL_POS)

/*
 *  FIFO Registers
 */
#define MAX31889_F_FIFO_WR_PTR_POS             (0)
#define MAX31889_F_FIFO_WR_PTR                 (0x1F<<MAX31889_F_FIFO_WR_PTR_POS)
#define MAX31889_F_FIFO_RD_PTR_POS             (0)
#define MAX31889_F_FIFO_RD_PTR                 (0x1F<<MAX31889_F_FIFO_RD_PTR_POS)
#define MAX31889_F_FIFO_OVF_CNT_POS            (0)
#define MAX31889_F_FIFO_OVF_CNT                (0x1F<<MAX31889_F_FIFO_OVF_CNT_POS)
#define MAX31889_F_FIFO_DATA_COUNTER_POS       (0)
#define MAX31889_F_FIFO_DATA_COUNTER           (0x3F<<MAX31889_F_FIFO_DATA_COUNTER_POS)
#define MAX31889_F_FIFO_CONF_FIFO_A_FULL_POS   (0)
#define MAX31889_F_FIFO_CONF_FIFO_A_FULL       (0x1F<<MAX31889_F_FIFO_CONF_FIFO_A_FULL_POS)


#define MAX31889_F_FIFO_CONF2_FIFO_RO_POS       (1)
#define MAX31889_F_FIFO_CONF2_FIFO_RO           (1<<MAX31889_F_FIFO_CONF2_FIFO_RO_POS)
#define MAX31889_F_FIFO_CONF2_A_FULL_TYPE_POS   (2)
#define MAX31889_F_FIFO_CONF2_A_FULL_TYPE       (1<<MAX31889_F_FIFO_CONF2_A_FULL_TYPE_POS)
#define MAX31889_F_FIFO_CONF2_FIFO_STAT_CLR_POS (3)
#define MAX31889_F_FIFO_CONF2_FIFO_STAT_CLR     (1<<MAX31889_F_FIFO_CONF2_FIFO_STAT_CLR_POS)
#define MAX31889_F_FIFO_CONF2_FLUSH_FIFO_POS    (4)
#define MAX31889_F_FIFO_CONF2_FLUSH_FIFO        (1<<MAX31889_F_FIFO_CONF2_FLUSH_FIFO_POS)

/*
 *  SYSTEM
 */
#define MAX31889_F_SYS_CTRL_RESET_POS           (0)
#define MAX31889_F_SYS_CTRL_RESET               (1<<MAX31889_BIT_SYS_CTRL_RESET_POS)

/*
 *  TEMPRATURE
 */
#define MAX31889_F_TEMP_SENS_SETUP_CONVERT_T_POS    (0)
#define MAX31889_F_TEMP_SENS_SETUP_CONVERT_T        (1<<MAX31889_F_TEMP_SENS_SETUP_CONVERT_T_POS)


/*
 *  GPIO
 */
#define MAX31889_F_GPIO_SETUP_GPIO0_MODE_POS        (0)
#define MAX31889_F_GPIO_SETUP_GPIO0_MODE            (3<<MAX31889_F_GPIO_SETUP_GPIO0_MODE_POS)
#define MAX31889_F_GPIO_SETUP_GPIO1_MODE_POS        (6)
#define MAX31889_F_GPIO_SETUP_GPIO1_MODE            (3<<MAX31889_F_GPIO_SETUP_GPIO1_MODE_POS)

//
#define MAX31889_F_GPIO_CTRL_GPIO0_LL_POS           (0)
#define MAX31889_F_GPIO_CTRL_GPIO0_LL               (1<<MAX31889_F_GPIO_CTRL_GPIO0_LL_POS)

#define MAX31889_F_GPIO_CTRL_GPIO1_LL_POS           (3)
#define MAX31889_F_GPIO_CTRL_GPIO1_LL               (1<<MAX31889_F_GPIO_CTRL_GPIO1_LL_POS)


#endif /* _MAX31889_REGISTERS_H_ */
