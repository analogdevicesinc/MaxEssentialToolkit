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

#ifndef _MAX31889_H_
#define _MAX31889_H_


#include <MAX31889/MAX31889_registers.h>

#include <Arduino.h>
#include <Wire.h>


#define MAX31889_FIFO_DEPTH         32   // Max Sample count in fifo
#define MAX31889_DEFAULT_I2C_ADDR   0x50 // 7 bit address

/*
 *
 * MAX31889 Temperature Sensor
 * 
 */
class MAX31889
{
    public:
        typedef enum {
            GPIO_NUM_0 = 0, // GPIO0
            GPIO_NUM_1,     // GPIO1
            GPIO_NUM_MAX
        } gpio_t;

        typedef enum {
            GPIO_MODE_0 = 0,  // Digital input (HiZ)
            GPIO_MODE_1,      // Digital output (open-drain).
            GPIO_MODE_2,      // Digital Input with 1M Ohm pulldown.
            GPIO_MODE_3,      // if GPIO1: Convert Temperature Input (active low)
                              // if GPII0: INTB (open-drain, active low) 
            GPIO_MODE_MAX
        } gpio_mode_t;

        typedef enum {
            INT_TEMP_RDY         = MAX31889_F_INT_EN_TEMP_RDY,
            INT_TEMP_HIGH        = MAX31889_F_INT_EN_TEMP_HI,
            INT_TEMP_LOW         = MAX31889_F_INT_EN_TEMP_LO,
            INT_FIFO_ALMOST_FULL = MAX31889_F_INT_EN_A_FULL
        } int_mode_t;

        typedef union {
            uint8_t raw;
            struct {
                uint8_t temp_rdy  : 1; // temperature ready
                uint8_t temp_high : 1; // temp high
                uint8_t temp_low  : 1; // temp low
                uint8_t           : 4; // not used
                uint8_t a_full    : 1; // almost full
            } bits;
        } status_t;

        typedef union {
            uint8_t raw;
            struct {
                uint8_t               : 1; // not used
                uint8_t fifo_ro       : 1; // fifo roolover
                uint8_t a_full_type   : 1; // almost full type
                uint8_t fifo_stat_clr : 1; // fifo stat clear
                uint8_t flush_fifo    : 1; // 
                uint8_t               : 3; // not used
            } bits;
        } fifo_cfg_t;

        typedef struct {
            uint8_t rom_id[6];
            uint8_t part_id; 
        } id_t;


        // constructer
        MAX31889(TwoWire *i2c, uint8_t i2c_addr = MAX31889_DEFAULT_I2C_ADDR);
        void begin(void);
        // identifier
        int get_id(id_t &id);
        //
        int clear_flags(void);
        int get_status(status_t &stat);
        int set_interrupt(int_mode_t interrupt, bool is_enable);
        // gpio functions
        int config_gpio(gpio_t gpio, gpio_mode_t mode);
        int get_gpio_state(gpio_t gpio); // get 1 or 0
        int set_gpio_state(gpio_t gpio, int state);// state 1 or 0
        //
        int get_alarm_temp(float &temp_low, float &temp_high);
        int set_alarm_temp(float temp_low=-40.0, float temp_high=125.0);

        int start_temp_conversion(void);
        int get_num_of_sample(void);
        int get_temp(float *temp, int num_of_samples=1);
        
        // fifo functions
        int flush_fifo();
        int get_fifo_cfg(fifo_cfg_t &cfg);
        int set_fifo_cfg(fifo_cfg_t cfg);
        int set_almost_full_depth(unsigned int num_of_samples);

        // generic functions
        int reset_registers(void);

        // Register access function
        int read_register(uint8_t reg, uint8_t *data, int len=1);
        int write_register(uint8_t reg, const uint8_t *data, int len=1);

    private:
        TwoWire *m_i2c;
        uint8_t  m_slave_addr;

        float    convert_count_2_temp(uint16_t count);
        uint16_t convert_temp_2_count(float temp);   
};

#endif /* _MAX31889_H_ */
