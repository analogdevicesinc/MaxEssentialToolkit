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

#ifndef _DS2482_H_
#define _DS2482_H_


#include <DS2482/DS2482_registers.h>

#include "Arduino.h"
#include <Wire.h>


/*
 *
 * DS2482 I2C to OneWire Converter
 * 
 */
class DS2482 {
    public:
        DS2482(TwoWire *i2c, byte i2c_addr);
        void begin(void);
        int  update_cfg(byte cfg);
        int  device_reset(void);
        int  onewire_reset();
        int  onewire_write_single_bit(byte bit);
        int  onewire_write_byte(byte byt);
        int  onewire_read_byte(byte &byt);
        int  onewire_write_triplet(byte byt);
    private:
        byte m_slave_addr;
        TwoWire *m_i2c;

        int read_byte(byte &byt);
        int write_byte(byte byt, bool stop=true);
        int write_register(byte reg, byte val, bool stop=true);

        int check_status(int max_delay=10);
};

#endif /* _DS2482_H_ */
