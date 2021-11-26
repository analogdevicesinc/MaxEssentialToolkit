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

#include <MAX31825/DS2482/DS2482.h>

/************************************ Private Functions ***********************/
int DS2482::read_byte(byte &byt)
{
    int ret = -1;

    m_i2c->requestFrom((char)m_slave_addr, (char)1, true);
    //
    while (m_i2c->available()) {
        byt = m_i2c->read();
        ret = 0;
    }

    if (ret != 0) {
       m_i2c->begin(); // restart
    }

    return ret;
}

int DS2482::write_byte(byte byt, bool stop/*=true*/)
{
    int ret;

    m_i2c->beginTransmission(m_slave_addr);
    //
    m_i2c->write(byt);
    //
    ret = m_i2c->endTransmission(stop);
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

int DS2482::write_register(byte reg, byte val, bool stop/*=true*/)
{
    int ret;

    m_i2c->beginTransmission(m_slave_addr);
    //
    m_i2c->write(reg);
    m_i2c->write(val);
    //
    ret = m_i2c->endTransmission(stop);
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

int DS2482::check_status(int max_delay)
{
    int ret = -1;
    byte stat;

    for (int i=0; i < max_delay; i++) {
        //
        m_i2c->requestFrom((char)m_slave_addr, (char)1, false);
        //
        if (m_i2c->available()) {
            stat = m_i2c->read();
            
            if ( 0 == (stat & DS2482_F_STAT_1WB)) {
                ret = 0;
                break;
            }
        }
        delay(1);
    }

    /*
        0:success
        1:data too long to fit in transmit buffer
        2:received NACK on transmit of address
        3:received NACK on transmit of data
        4:other error
    */

    if ( 0 != m_i2c->endTransmission() ) {
        ret = -1;
        m_i2c->begin(); // restart
    }

    return ret;
}

/************************************ Public Functions ***********************/
DS2482::DS2482(TwoWire *i2c, byte i2c_addr)
{
    if (i2c == NULL) {
        while (1) {
            ;
        }
    }
    
    m_slave_addr = i2c_addr;
    m_i2c  = i2c;
}

void DS2482::begin(void)
{
    m_i2c->begin();
}

int DS2482::update_cfg(byte cfg)
{
    int ret;
    byte rsp;

    cfg &= 0x0f;
    cfg |= ((~cfg) & 0x0f)  << 4;
    ret = write_register(DS2482_CMD_WCFG, cfg, false);
    if (ret) {
        return -1;
    }

    delay(5);
    ret = read_byte(rsp);

    return ret;
}

int DS2482::device_reset(void)
{
    int ret;
    byte rsp = 0;

    ret = write_byte(DS2482_CMD_DRST, false);
    if (ret) {
        return -1;
    }
    
    //delay(1);
    ret = read_byte(rsp);
    if (ret == 0) {
        if (0 == (rsp & DS2482_F_STAT_RST)) {
            return -1; // Reset not applied
        }
    }

    return ret;
}

int DS2482::onewire_reset()
{
    int ret;
    
#if 1
    byte rsp = 0;

    ret = write_byte(DS2482_CMD_1WRST);
    if (ret) {
        return -1;
    }

    delay(5);
    ret = read_byte(rsp);
    if (ret == 0) {
        if (rsp & DS2482_F_STAT_1WB) {
            return -1; // Device is busy
        }
    }
#else  
    ret = write_byte(DS2482_CMD_1WRST, false);
    if (ret == 0) {
        ret = check_status();
    }
#endif

    return ret;
}

int DS2482::onewire_write_single_bit(byte bit)
{
    int ret;

#if 1
    byte rsp = 0;
    
    ret = write_register(DS2482_CMD_1WWSB, bit);
    if (ret) {
        return -1;
    }
    
    delay(5);
    ret = read_byte(rsp);
    if (ret == 0) {
        if (rsp & DS2482_F_STAT_1WB) {
            return -1; // Device is busy
        }
    }  
#else
    ret = write_register(DS2482_CMD_1WWSB, bit, false);
    if (ret == 0) {
        ret = check_status();
    }
#endif

    return ret;
}

int DS2482::onewire_write_byte(byte byt)
{
    int ret;
    
#if 1
    ret = write_register(DS2482_CMD_1WWB, byt);  
#else
    ret = write_register(DS2482_CMD_1WWB, byt, false);
    if (ret == 0) {
        ret = check_status();
    }
#endif
    
    return ret;
}

int DS2482::onewire_read_byte(byte &byt)
{
    int ret;

    ret = write_byte(DS2482_CMD_1WRB);
    if ( ret ) {
        return -1;
    }

    delay(1);
    ret = write_register(DS2482_CMD_SRP, DS2482_CMD_SRP, false);
    if ( ret ) {
        return -1;
    }

    ret = read_byte(byt);
    if ( ret ) {
        return -1;
    }
    
    return ret;
}

int DS2482::onewire_write_triplet(byte byt)
{
    int ret;

    byt &= 0x80; // ignore other bits
#if 1    
    ret = write_register(DS2482_CMD_1WT, byt);  
    if ( ret ) {
        return -1;
    }

    delay(1);
    ret = read_byte(byt);
    if ( ret ) {
        return -1;
    }
 #else
    ret = write_register(DS2482_CMD_1WT, byt, false);
    if (ret == 0) {
        ret = check_status();
    }
#endif

    return ret;
}
