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

#ifndef _DS2482_REGISTERS_H_
#define _DS2482_REGISTERS_H_


/*
 *  DS2482 Registers
 */
#define DS2482_R_STATUS               0xF0
#define DS2482_R_READ_DATA            0xE1
#define DS2482_R_CFG                  0xC3


/*
 *  Status Register
 */
#define DS2482_F_STAT_1WB_POS              (0)
#define DS2482_F_STAT_1WB                  (1<<DS2482_F_STAT_1WB_POS)
#define DS2482_F_STAT_PPD_POS              (1)
#define DS2482_F_STAT_PPD                  (1<<DS2482_F_STAT_PPD_POS)
#define DS2482_F_STAT_SD_POS               (2)
#define DS2482_F_STAT_SD                   (1<<DS2482_F_STAT_SD_POS)
#define DS2482_F_STAT_LL_POS               (3)
#define DS2482_F_STAT_LL                   (1<<DS2482_F_STAT_LL_POS)
#define DS2482_F_STAT_RST_POS              (4)
#define DS2482_F_STAT_RST                  (1<<DS2482_F_STAT_RST_POS)
#define DS2482_F_STAT_SBR_POS              (5)
#define DS2482_F_STAT_SBR                  (1<<DS2482_F_STAT_SBR_POS)
#define DS2482_F_STAT_TSB_POS              (6)
#define DS2482_F_STAT_TSB                  (1<<DS2482_F_STAT_TSB_POS)
#define DS2482_F_STAT_DIR_POS              (7)
#define DS2482_F_STAT_DIR                  (1<<DS2482_F_STAT_DIR_POS)

/*
 *  Configuration Register
 */
#define DS2482_F_CFG_APU_POS              (0)                         // Active Pullup
#define DS2482_F_CFG_APU                  (1<<DS2482_F_CFG_APU_POS)
#define DS2482_F_CFG_SPU_POS              (2)                         // Active Pullup
#define DS2482_F_CFG_SPU                  (1<<DS2482_F_CFG_SPU_POS)
#define DS2482_F_CFG_1WS_POS              (3)                         // 1 Wire Speed
#define DS2482_F_CFG_1WS                  (1<<DS2482_F_CFG_1WS_POS)


/*
    Commands
*/
#define DS2482_CMD_DRST             0xF0
#define DS2482_CMD_SRP              0xE1
#define DS2482_CMD_WCFG             0xD2
#define DS2482_CMD_1WRST            0xB4
#define DS2482_CMD_1WWSB            0x87
#define DS2482_CMD_1WWB             0xA5
#define DS2482_CMD_1WRB             0x96
#define DS2482_CMD_1WT              0x78

/*
    I2C Addr
*/
#define DS2482_I2C_ADDR_AD_00             0x18  // AD1:0  AD0:0
#define DS2482_I2C_ADDR_AD_01             0x19  // AD1:0  AD0:1
#define DS2482_I2C_ADDR_AD_10             0x1A  // AD1:1  AD0:0
#define DS2482_I2C_ADDR_AD_11             0x1B  // AD1:1  AD0:1

#endif /* _DS2482_REGISTERS_H_ */
