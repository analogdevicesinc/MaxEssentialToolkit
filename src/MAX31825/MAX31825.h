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

#include <Arduino.h>
#include <Wire.h>
#include <OneWire.h>


/*
 *
 * MAX31825 Temperature Sensor
 * 
 */
class MAX31825 {
    public:
        typedef enum {
            PERIOD_0_SHUTDOWN  = 0,
            PERIOD_1_DIV_64SEC = 1,
            PERIOD_1_DIV_32SEC = 2,
            PERIOD_1_DIV_16SEC = 3,
            PERIOD_1_DIV_4SEC  = 4,
            PERIOD_1_DIV_1SEC  = 5,
            PERIOD_4_DIV_1SEC  = 6,
            PERIOD_8_DIV_1SEC  = 7
        } conv_period_t;

        typedef enum {
            RESOLUTION_8_BIT   = 0,
            RESOLUTION_9_BIT   = 1,
            RESOLUTION_10_BIT  = 2,
            RESOLUTION_12_BIT  = 3
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

        typedef union {
            uint8_t raw;
            struct {
                uint8_t addr     : 6; // Address information which selected by the resistor value
                uint8_t tl_fault : 1; // 
                uint8_t th_fault : 1; //
            } bits;
        } status_t;

        typedef union {
            uint8_t raw;
            struct {
                uint8_t conversion_rate : 3; // conv_period_t
                uint8_t                 : 1; // 
                uint8_t comp_int        : 1; // mode_t
                uint8_t resolution      : 2; // resolution_t
                uint8_t format          : 1; // 
            } bits;
        } reg_cfg_t;


        MAX31825(OneWire *onewire);
        MAX31825(TwoWire *i2c, byte i2c_addr);
        //
        void begin(void);
        int  set_addressing_mode(addressing_mode_t addr_mode, byte location=0x00, byte rom_code[8]={0,} );
        int  read_rom(byte (&rom_code)[8]);
        //
        int get_status(status_t &stat);
        int get_configuration(reg_cfg_t &cfg);
        int set_configuration(reg_cfg_t cfg);

        //
        int start_temp_conversion();
        int get_temp(float &tmp);
        int set_alarm(float temp_low, float temp_high);
        int get_alarm(float &temp_low, float &temp_high);
        //
        int set_conv_rate(conv_period_t period);
        int set_extend_mode(bool enable);
        int set_resolution(resolution_t res);
        int set_cmp_int_mode(mode_t mode);
        
    private:
        DS2482   *m_ds2482;
        OneWire  *m_onewire;
        int  m_interface;
        byte m_serial[MAX31825_ROMCODE_SIZE];
        byte m_scratchpad[MAX31825_SCRATCHPAD_SIZE];
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
