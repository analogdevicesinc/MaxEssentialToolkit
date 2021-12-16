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

#include <MAX31825/MAX31825.h>

#define INTF_I2C    0
#define INTF_1W     1

// 
#define TEMP_RESOLUTION_FOR_12_BIT      (0.0625f)
#define TEMP_RESOLUTION_FOR_10_BIT      (0.25f)
#define TEMP_RESOLUTION_FOR_9_BIT       (0.5f)
#define TEMP_RESOLUTION_FOR_8_BIT       (1.0f)

// ROM Commands
#define MAX31825_CMD_ROM_READ           0x33
#define MAX31825_CMD_ROM_MATCH          0x55
#define MAX31825_CMD_ROM_SEARCH         0xF0
#define MAX31825_CMD_ROM_SKIP           0xCC

// Function Commands
#define MAX31825_CMD_CONVERT_T          0x44  // Initiates temperature conversion.
#define MAX31825_CMD_READ_SCRATCHPAD    0xBE  // Reads the 9-byte scratchpad including the CRC byte.
#define MAX31825_CMD_WRITE_SCRATCHPAD   0x4E  // Writes bytes 3 through 7 (Configuration and thresholds) to the scratchpad.
#define MAX31825_CMD_DETECT_ADDR        0x88  // Loads location bits
#define MAX31825_CMD_SELECT_ADDR        0x70  // Selects device with location bits that match transmitted bits. Follow with a Convert, Read, or Write command.

#define GET_BIT_VAL(val, pos, mask)     ( ( (val) & mask) >> pos )
#define SET_BIT_VAL(val, pos, mask)     ( ( ((int)val) << pos) & mask )

#define SCRATCHPAD_GET_TEMP_COUNT(scpth)   ( (scpth[MAX31825_R_TEMP_MSB]<<8) | scpth[MAX31825_R_TEMP_LSB] )
#define SCRATCHPAD_GET_STATUS(scpth)       (  scpth[MAX31825_R_STATUS] )
#define SCRATCHPAD_GET_CFG(scpth)          (  scpth[MAX31825_R_CFG] )
#define SCRATCHPAD_GET_TH_COUNT(scpth)     ( (scpth[MAX31825_R_TH_MSB]<<8) | scpth[MAX31825_R_TH_LSB] )
#define SCRATCHPAD_GET_TL_COUNT(scpth)     ( (scpth[MAX31825_R_TL_MSB]<<8) | scpth[MAX31825_R_TL_LSB] )


#define POLY    0x31 // # 0x131 = x^8 + x^5 + x^4 + 1  (0x31 -> 0x131)

static byte crc_table[256] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

/************************************ Private Functions ***********************/
static byte calc_crc8(byte *buf, int len)
{
    int i;
    byte crc = 0;
    
    for (i = 0; i < len; i++) {
        crc = crc_table[crc ^ *buf++];
    }
 
    return crc;
}

uint16_t MAX31825::convert_temp_2_count(float temp)
{
    uint16_t count;

    if (temp < 0) {
        temp  = 0 - temp;
        count = temp / TEMP_RESOLUTION_FOR_12_BIT;
        count = (count - 1) ^ 0xFFFF;
    } else {
        count = temp / TEMP_RESOLUTION_FOR_12_BIT;
    }

    return count;
}

float MAX31825::convert_count_2_temp(uint16_t count)
{
    float temp;

    // 16th bit is sign bit
    if (count & (1<<15) ) {
        count = (count ^ 0xFFFF) + 1;
        temp  = count * TEMP_RESOLUTION_FOR_12_BIT;
        temp  = 0 - temp; // convert to negative
    } else {
        temp = count * TEMP_RESOLUTION_FOR_12_BIT;
    }

    return temp;
}

int MAX31825::onewire_reset(void)
{
    int ret = 0;
    
    if (m_interface == INTF_I2C) {
        ret = m_ds2482->onewire_reset();
    } else {
        ret = m_onewire->reset();
        if (ret == 1) { // 1 means success
            ret = 0; // means success
        } else {
            ret = -1; // means fail
        }
    }
    
    return ret;
}

int MAX31825::onewire_write_byte(byte byt)
{
    int ret = 0;
    
    if (m_interface == INTF_I2C) {
        ret = m_ds2482->onewire_write_byte(byt);
    } else {
        m_onewire->write(byt);
    }
    
    return ret;
}

int MAX31825::onewire_read_byte(byte &byt)
{
    int ret = 0;
    
    if (m_interface == INTF_I2C) {
        ret = m_ds2482->onewire_read_byte(byt);
    } else {
        byt = m_onewire->read();
        ret = 0; // assume success
    }
    
    return ret;
}

int MAX31825::write_cmd(byte cmd)
{
    int ret = 0;

    if (m_interface == INTF_I2C) {
        ret = m_ds2482->device_reset();
        if (ret) {
            return ret;
        }
    }
        
    ret = onewire_reset();
    if (ret) {
        return ret;
    }
    
    switch (m_addr_mode) {
        case ADDRESSING_USE_ROM:
            /* Select ROM */
            ret = onewire_write_byte(MAX31825_CMD_ROM_MATCH);
            if (ret) {
               return ret;
            }
            
            for (int i = 0; i < sizeof(m_serial); i++) {
                delay(1);
                ret = onewire_write_byte( m_serial[i] );
                if (ret) {
                    return ret;
                }
            }
            break;
        case ADDRESSING_USE_ADD1_ADD0:            
            /* Select Location */
            ret = onewire_write_byte(MAX31825_CMD_SELECT_ADDR);
            if (ret) {
               return ret;
            }
            
            delay(1);
            ret = onewire_write_byte( m_target_addr );
            if (ret) {
                return ret;
            }
            break;
        default:
            /* Pass ROM */
            ret = onewire_write_byte(MAX31825_CMD_ROM_SKIP);
            if (ret) {
               return ret;
            }
            break;
    }
    
    delay(1);
    ret = onewire_write_byte(cmd);
    
    return ret;
}

int MAX31825::read_scratchpad(void)
{
    int ret;
    int i;
    
    ret = write_cmd(MAX31825_CMD_READ_SCRATCHPAD);
    if (ret) {
       return ret;
    } 
    
    delay(1);
    for (i=0; i < sizeof(m_scratchpad); i++) {        
        ret = onewire_read_byte(m_scratchpad[i]);
        if (ret) {
            return ret;
        }
    }
    
    return ret;
}

int MAX31825::write_scratchpad(void)
{
    int ret;
    int  i;

    ret = write_cmd(MAX31825_CMD_WRITE_SCRATCHPAD);
    if (ret) {
       return ret;
    } 
    
    // Write CFG, TH, TL
    for (i=3; i < sizeof(m_scratchpad); i++) {   
        delay(1);    
        ret = onewire_write_byte(m_scratchpad[i]);
        if (ret) {
            return ret;
        }
    }
    
    return ret;
}

/************************************ Public Functions ***********************/
MAX31825::MAX31825(TwoWire *i2c, byte i2c_addr)
{
    if (i2c == NULL) {
        while (1) {
            ;
        }
    }
    
    m_interface = INTF_I2C;
    m_ds2482 = new DS2482(i2c, i2c_addr);
}

MAX31825::MAX31825(OneWire *onewire)
{
    if (onewire == NULL) {
        while (1) {
            ;
        }
    }
    
    m_interface = INTF_1W;
    m_onewire = onewire;
}

void MAX31825::begin(void)
{
    m_addr_mode = ADDRESSING_NONE;

    if (m_interface == INTF_I2C) {
        m_ds2482->begin();
        //
        m_ds2482->device_reset();
    } else {
        //...
    }
}

int MAX31825::set_addressing_mode(addressing_mode_t addr_mode, byte location/*=0x00*/, byte rom_code[8]/*{0,}*/ )
{
    int ret = 0;
    
    switch (addr_mode) {
        case ADDRESSING_USE_ROM:
            break;
        case ADDRESSING_USE_ADD1_ADD0:
            ret = write_cmd(MAX31825_CMD_DETECT_ADDR);
            break;
        default:
            break;
    }
    
    if (ret == 0) {
        m_addr_mode = addr_mode;
        m_target_addr = location;
        
        for (int i=0; i < sizeof(m_serial); i++) {
            m_serial[i] = rom_code[i];
        }        
    }
    
    return ret;
}

int MAX31825::read_rom(byte (&code)[8])
{
    int ret = -1;

    if ( onewire_reset() ) {
        return -1;
    }
        
    if (m_interface == INTF_I2C) {
        
        if ( onewire_write_byte(MAX31825_CMD_ROM_READ) ) {
            return -1;
        }
        
        for (int i=0; i < MAX31825_ROMCODE_SIZE; i++) {
            delay(1);
            if ( onewire_read_byte(code[i]) ) {
                return -1;
            }
        }
        
        byte crc = calc_crc8(code, 7);
        if (crc == code[7]) {
            if (MAX31825_FAMILY_CODE == code[0]) {
                ret = 0; // Found MAX31825 sensor
            }
        }
        
    } else {
        
        m_onewire->reset_search();
        while( m_onewire->search(code) ) {
        
            byte crc = calc_crc8(code, 7);
            if (crc == code[7]) {
                if (MAX31825_FAMILY_CODE == code[0]) {
                    ret = 0;
                    break; // Found MAX31825 sensor
                }
            }
        }
    }
    
    return ret;
}

int MAX31825::start_temp_conversion(void)
{
    int ret = 0;
    
    ret = write_cmd(MAX31825_CMD_CONVERT_T);
    
    return ret;
}

int MAX31825::get_status(reg_status_t &stat)
{
    int ret = 0;
    byte val8;
    
    ret = read_scratchpad();
    if (ret) {
        return ret;
    }
    val8 = SCRATCHPAD_GET_STATUS(m_scratchpad);
    
    stat.bits.addr     = GET_BIT_VAL(val8, MAX31825_F_STATUS_ADDR_POS,     MAX31825_F_STATUS_ADDR);
    stat.bits.tl_fault = GET_BIT_VAL(val8, MAX31825_F_STATUS_TL_FAULT_POS, MAX31825_F_STATUS_TL_FAULT);
    stat.bits.th_fault = GET_BIT_VAL(val8, MAX31825_F_STATUS_TH_FAULT_POS, MAX31825_F_STATUS_TH_FAULT);

    return ret;
}

int MAX31825::get_configuration(reg_cfg_t &cfg)
{
    int  ret = 0;
    uint8_t val8;

    ret = read_scratchpad();
    if (ret) {
        return ret;
    }
    val8 = SCRATCHPAD_GET_CFG(m_scratchpad);

    cfg.bits.conversion_rate = GET_BIT_VAL(val8, MAX31825_F_CFG_CONV_RATE_POS,  MAX31825_F_CFG_CONV_RATE);
    cfg.bits.comp_int        = GET_BIT_VAL(val8, MAX31825_F_CFG_CMP_INT_POS,    MAX31825_F_CFG_CMP_INT);
    cfg.bits.resolution      = GET_BIT_VAL(val8, MAX31825_F_CFG_RESOLUTION_POS, MAX31825_F_CFG_RESOLUTION);  
    cfg.bits.format          = GET_BIT_VAL(val8, MAX31825_F_CFG_FORMAT_POS,     MAX31825_F_CFG_FORMAT);

    return ret;
}

int MAX31825::set_configuration(reg_cfg_t cfg)
{
    int  ret = 0;
    uint8_t val8;

    /* m_scratchpad already includes current values of sensor 
     * to increase performance no need to read it again
     */
    //ret = read_scratchpad();
    //if (ret) {
    //    return ret;
    //}

    val8 = 0;
    val8 |= SET_BIT_VAL(cfg.bits.conversion_rate, MAX31825_F_CFG_CONV_RATE_POS,  MAX31825_F_CFG_CONV_RATE);
    val8 |= SET_BIT_VAL(cfg.bits.comp_int,        MAX31825_F_CFG_CMP_INT_POS,    MAX31825_F_CFG_CMP_INT);
    val8 |= SET_BIT_VAL(cfg.bits.resolution,      MAX31825_F_CFG_RESOLUTION_POS, MAX31825_F_CFG_RESOLUTION);  
    val8 |= SET_BIT_VAL(cfg.bits.format,          MAX31825_F_CFG_FORMAT_POS,     MAX31825_F_CFG_FORMAT);

    m_scratchpad[MAX31825_R_CFG] = val8;
    ret = write_scratchpad();

    return ret;
}

int MAX31825::get_temp(float &tmp)
{
    int ret;
    
    ret = read_scratchpad();
    if (ret) {
        return ret;
    }
    tmp = convert_count_2_temp( SCRATCHPAD_GET_TEMP_COUNT(m_scratchpad) );
    
    return ret;
}

int MAX31825::set_alarm(float temp_low, float temp_high)
{
    int ret;
    uint16_t count;
    
    ret = read_scratchpad();
    if (ret) {
        return ret;
    }
    
    count = convert_temp_2_count( temp_low );
    m_scratchpad[MAX31825_R_TL_LSB] = count & 0xff;
    m_scratchpad[MAX31825_R_TL_MSB] = (count>>8) & 0xff;
    
    count = convert_temp_2_count( temp_high );
    m_scratchpad[MAX31825_R_TH_LSB] = count & 0xff;
    m_scratchpad[MAX31825_R_TH_MSB] = (count>>8) & 0xff; 
    
    ret = write_scratchpad();
    
    return ret;
}

int MAX31825::get_alarm(float &temp_low, float &temp_high)
{
    int ret;
    
    ret = read_scratchpad();
    if (ret) {
        return ret;
    }
    temp_low = convert_count_2_temp( SCRATCHPAD_GET_TL_COUNT(m_scratchpad) );
    temp_high = convert_count_2_temp( SCRATCHPAD_GET_TH_COUNT(m_scratchpad) );
    
    return ret;
}
