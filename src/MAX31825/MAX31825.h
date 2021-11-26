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

#ifndef _MAX31825_H_
#define _MAX31825_H_


#include <MAX31825/MAX31825_registers.h>
#include <MAX31825/DS2482/DS2482.h>

#include "Arduino.h"
#include <Wire.h>
#include <OneWire.h>


/*
 *
 * MAX31825 Temperature Sensor Class
 * 
 */
class MAX31825 {
    public:
        typedef struct {
            bool th_fault;
            bool tl_fault;
            byte addr;
        } status_t;

        typedef enum {
            PERIOD_0_SHUTDOWN  = MAX31825_S_CFG_CONV_RATE_SHUTDOWN,
            PERIOD_1_DIV_64SEC = MAX31825_S_CFG_CONV_RATE_1_DIV_64SEC,
            PERIOD_1_DIV_32SEC = MAX31825_S_CFG_CONV_RATE_1_DIV_32SEC,
            PERIOD_1_DIV_16SEC = MAX31825_S_CFG_CONV_RATE_1_DIV_16SEC,
            PERIOD_1_DIV_4SEC  = MAX31825_S_CFG_CONV_RATE_1_DIV_4SEC,
            PERIOD_1_DIV_1SEC  = MAX31825_S_CFG_CONV_RATE_1_DIV_1SEC,
            PERIOD_4_DIV_1SEC  = MAX31825_S_CFG_CONV_RATE_4_DIV_1SEC,
            PERIOD_8_DIV_1SEC  = MAX31825_S_CFG_CONV_RATE_8_DIV_1SEC,
        } conv_period_t;

        typedef enum {
            RESOLUTION_8_BIT   = MAX31825_S_CFG_RESOLUTION_8_BIT,
            RESOLUTION_9_BIT   = MAX31825_S_CFG_RESOLUTION_9_BIT,
            RESOLUTION_10_BIT  = MAX31825_S_CFG_RESOLUTION_10_BIT,
            RESOLUTION_12_BIT  = MAX31825_S_CFG_RESOLUTION_12_BIT
        } resolution_t;

        typedef enum {
            MODE_COMPARE   = 0,
            MODE_INTERRUPT = 1
        } mode_t;

        typedef enum {
            ADDRESSING_NONE,         // Pass rom addr and A5:A0 addr
            ADDRESSING_USE_ROM,      // Use rom code to addressing target 
            ADDRESSING_USE_ADD1_ADD0 // Use resistor that connected to ADDR0
        } addressing_mode_t;

        MAX31825(OneWire *onewire);
        MAX31825(TwoWire *i2c, byte i2c_addr);
        //
        void begin(void);
        int  set_addressing_mode(addressing_mode_t addr_mode, byte location=0x00, byte rom_code[8]={0,} );
        int  read_rom(byte (&rom_code)[8]);
        //
        int get_status(status_t &stat);
        //
        int start_meas();
        int get_temp(float &tmp);
        int set_alarm(float temp_low, float temp_high);
        int get_alarm(float &temp_low, float &temp_high);
        //
        int set_conv_rate(conv_period_t period);
        int set_extend_mode(bool enable);
        int set_resolution(resolution_t res);
        int set_cmp_int_mode(mode_t mode);
        //
        int get_conf_reg(byte &cfg);
        
    private:
        DS2482   *m_ds2482;
        OneWire  *m_onewire;
        int  m_interface;
        byte m_serial[8];
        byte m_scratchpad[8];
        addressing_mode_t m_addr_mode;
        byte m_target_addr; // target addr for onewire

        //
        uint16_t convert_temp_2_count(float temp);
        float convert_count_2_temp(uint16_t count);
        
        int  onewire_reset(void);
        int  onewire_write_byte(byte byt);
        int  onewire_read_byte(byte &byt);
        
        int write_cmd(byte cmd);
        int read_scratchpad();
        int write_scratchpad();
};

#endif /* _MAX31825_H_ */
