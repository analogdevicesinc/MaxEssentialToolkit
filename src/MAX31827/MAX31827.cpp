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

#include <MAX31827/MAX31827.h>

// 
#define TEMP_RESOLUTION_FOR_12_BIT      (0.0625f)
#define TEMP_RESOLUTION_FOR_10_BIT      (0.25f)
#define TEMP_RESOLUTION_FOR_9_BIT       (0.5f)
#define TEMP_RESOLUTION_FOR_8_BIT       (1.0f)

#define POLY    0x06 // # 0x106 = x^8 + x^2 + x^1  (0x06 -> 0x106)

/************************************ Private Functions ***********************/
static uint8_t calc_crc8(uint8_t crc, uint8_t *buf, int len)
{
    int i;
 
    while (len--) {
        crc = crc ^ *buf++;

        // Eight rounds of 1-bit
        for(i=0; i<8; i++)  {
            if (crc & 0x80) {
                crc = (crc << 1) ^ POLY;
            } else {
                crc = (crc << 1); // Left Shifting
            }
        }
    }
 
    return crc;
}

int MAX31827::read_register(uint8_t reg, uint16_t &val)
{
    int ret;
    int counter = 0;
    uint8_t buf[3] = {0,};

    m_i2c->beginTransmission(m_slave_addr);
    m_i2c->write(reg);

    /*
        stop = true, sends a stop message after transmission, releasing the I2C bus.
        
        stop = false, sends a restart message after transmission. 
          The bus will not be released, which prevents another master device from transmitting between messages. 
          This allows one master device to send multiple transmissions while in control.
    */
    ret = m_i2c->endTransmission(false);
    /*
        0:success
        1:data too long to fit in transmit buffer
        2:received NACK on transmit of address
        3:received NACK on transmit of data
        4:other error
    */
    if (ret != 0) {
        m_i2c->begin(); // restart
        return -1;
    }

    // Read
    int r_Len = m_pec_status ? 3: 2; // +1 for PEC byte
    
    m_i2c->requestFrom((char)m_slave_addr, (char)r_Len, false);

    while (m_i2c->available()) { // slave may send less than requested
        buf[counter++] = m_i2c->read(); // receive a byte as character
    }
    
    m_i2c->endTransmission();

    //
    if (counter == r_Len) {
        val = (buf[0] << 8) | buf[1];

        // CHECK CRC if PEC enabled
        if (m_pec_status) {
            uint8_t crc = 0;
            crc = calc_crc8(crc, &m_slave_addr, 1);
            crc = calc_crc8(crc, &reg, 1);
            crc = calc_crc8(crc, buf,  2);
            
            if (crc != buf[2]) {
                // CRC deos not match with PEC byte
                //return -2;
            }
        }

    } else {
        m_i2c->begin(); // restart
        ret = -1;
    }


    return ret;
}

int MAX31827::write_register(uint8_t reg, uint16_t val)
{
    int ret;
    uint8_t buf[2];

    m_i2c->beginTransmission(m_slave_addr);
    //
    m_i2c->write(reg);
    buf[0] = (uint8_t)(val>>8);
    buf[1] = (uint8_t)val;
    
    m_i2c->write(buf, 2);

    if (m_pec_status) {
        uint8_t crc = 0;

        crc = calc_crc8(crc, &m_slave_addr, 1);
        crc = calc_crc8(crc, &reg, 1);
        crc = calc_crc8(crc, buf,  2);
        
        m_i2c->write(crc);
    }

    //
    ret = m_i2c->endTransmission();
    /*
        0:success
        1:data too long to fit in transmit buffer
        2:received NACK on transmit of address
        3:received NACK on transmit of data
        4:other error
    */

    if (ret != 0) {
        m_i2c->begin(); // restart
    }

    return ret;
}

/************************************ Public Functions ***********************/
MAX31827::MAX31827(TwoWire *i2c, uint8_t i2c_addr)
{
    if (i2c == NULL) {
        while (1) {
            ;
        }
    }
    
    m_slave_addr = i2c_addr;
    m_pec_status = false;
    m_i2c = i2c;
}

void MAX31827::begin(void)
{
    m_i2c->begin();
}

int MAX31827::get_status(status_t &stat)
{
    int ret = 0;
    uint16_t cfg;

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    stat.temp_ready = (cfg & MAX31827_F_CFG_ONESHOT) ? false: true;
    stat.pec_err    = (cfg & MAX31827_F_CFG_PEC_ERR) ? true: false;
    stat.temp_low   = (cfg & MAX31827_F_CFG_UNDER_TEMP_STAT) ? true: false;
    stat.temp_high  = (cfg & MAX31827_F_CFG_OVER_TEMP_STAT) ? true: false;

    return ret;
}

int MAX31827::set_alarm(float temp_low, float temp_high)
{
    int ret = 0;

    ret = set_temp(temp_low, MAX31827_R_TL);
    if (ret) {
        return -1;
    }

    ret = set_temp(temp_high, MAX31827_R_TH);
    if (ret) {
        return -1;
    }

    return ret;
}

int MAX31827::get_alarm(float &temp_low, float &temp_high)
{
    int ret = 0;

    ret = get_temp(temp_low, MAX31827_R_TL);
    if (ret) {
        return -1;
    }

    ret = get_temp(temp_high, MAX31827_R_TH);
    if (ret) {
        return -1;
    }

    return ret;
}

int MAX31827::set_alarm_hyst(float tl_hyst, float th_hyst)
{
    int ret = 0;

    ret = set_temp(tl_hyst, MAX31827_R_TL_HYST);
    if (ret) {
        return -1;
    }

    ret = set_temp(th_hyst, MAX31827_R_TH_HYST);
    if (ret) {
        return -1;
    }

    return ret;
}

int MAX31827::get_alarm_hyst(float &tl_hyst, float &th_hyst)
{
    int ret = 0;

    ret = get_temp(tl_hyst, MAX31827_R_TL_HYST);
    if (ret) {
        return -1;
    }

    ret = get_temp(th_hyst, MAX31827_R_TH_HYST);
    if (ret) {
        return -1;
    }

    return ret;
}

int MAX31827::set_pec_status(bool enable)
{
    int  ret = 0;
    uint16_t cfg;

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    if (enable) {
        cfg |= MAX31827_F_CFG_PEC_ENABLE;
    } else {
        cfg &= ~MAX31827_F_CFG_PEC_ENABLE;
    }
    
    ret = write_register(MAX31827_R_CFG, cfg);

    if (ret == 0) {
        m_pec_status = enable;
    }

    return ret;
}

int MAX31827::set_timeout_status(bool enable)
{
    int  ret = 0;
    uint16_t cfg;

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    if (enable) {
        cfg |= MAX31827_F_CFG_TIMEOUT;
    } else {
        cfg &= ~MAX31827_F_CFG_TIMEOUT;
    }
    
    ret = write_register(MAX31827_R_CFG, cfg);

    return ret;
}

int MAX31827::set_resolution(resolution_t resolution)
{
    int  ret = 0;
    uint16_t cfg;
    uint16_t val = (uint16_t)resolution;

    // mask other bits
    val &= (MAX31827_F_CFG_RESOLUTION);

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    // to switch shutdown mode
    cfg &= ~MAX31827_F_CFG_CONV_RATE;
    //
    cfg = (cfg & ~MAX31827_F_CFG_RESOLUTION) | val;
    ret = write_register(MAX31827_R_CFG, cfg);

    return ret;
}

int MAX31827::set_alarm_polarity(bool high)
{
    int  ret = 0;
    uint16_t cfg;

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    if (high) {
        cfg |= MAX31827_F_CFG_ALARM_POL;
    } else {
        cfg &= ~MAX31827_F_CFG_ALARM_POL;
    }
    
    ret = write_register(MAX31827_R_CFG, cfg);

    return ret;
}

int MAX31827::set_cmp_int_mode(mode_t mode)
{
    int  ret = 0;
    uint16_t cfg;

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    if (mode == MODE_INTERRUPT) {
        cfg |= MAX31827_F_CFG_CMP_INT;
    } else {
        cfg &= ~MAX31827_F_CFG_CMP_INT;
    }
    
    ret = write_register(MAX31827_R_CFG, cfg);

    return ret;
}

int MAX31827::set_fault_number(fault_t fault)
{
    int  ret = 0;
    uint16_t cfg;
    uint16_t val = (uint16_t)fault;

    // mask other bits
    val &= (MAX31827_F_CFG_FAULT_QUE);

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    cfg = (cfg & ~MAX31827_F_CFG_FAULT_QUE) | val;
    ret = write_register(MAX31827_R_CFG, cfg);

    return ret;
}

int MAX31827::start_meas(conv_period_t period)
{
    int  ret = 0;
    uint16_t cfg;
    uint16_t val = (uint16_t)period;

    // mask other bits
    val &= (MAX31827_F_CFG_CONV_RATE);

    ret = read_register(MAX31827_R_CFG, cfg);
    if (ret) {
        return -1;
    }

    // shutdown conv rate
    cfg &= ~MAX31827_F_CFG_CONV_RATE;

    if (period == PERIOD_ONE_SHOT) {
        cfg |= MAX31827_F_CFG_ONESHOT;
    } else {
        cfg |= val;
    }
    ret  = write_register(MAX31827_R_CFG, cfg);

    return ret;
}

int MAX31827::get_temp(float &temp, uint8_t reg)
{
    int ret = 0;
    uint16_t count;

    ret = read_register(reg, count);
    if (ret) {
        return -1;
    }

    // convert count to temperature
    if (count & (1<<15) ) {
        count = (count ^ 0xFFFF) + 1;
        temp  = count * TEMP_RESOLUTION_FOR_12_BIT;
        temp  = 0 - temp; // convert to negative
    } else {
        count &= MAX31827_F_TEMPERATURE_MASK;
        temp  = count * TEMP_RESOLUTION_FOR_12_BIT;
    }

    return ret;
}

int MAX31827::set_temp(float temp, uint8_t reg)
{
    int ret = 0;
    uint16_t count;

    // Switch to shutdown mode before changing register
    ret = start_meas(PERIOD_0_SHUTDOWN);
    if (ret) {
        return -1;
    }

    // convert temperature to count
    if (temp < 0) {
        temp  = 0 - temp;
        count = temp / TEMP_RESOLUTION_FOR_12_BIT;
        count = (count - 1) ^ 0xFFFF;
    } else {
        count  = temp / TEMP_RESOLUTION_FOR_12_BIT;
        count &= MAX31827_F_TEMPERATURE_MASK;
    }

    // set register
    ret = write_register(reg, count);

    return ret;
}
