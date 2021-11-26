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


#include <MAX31889_registers.h>

#include "Arduino.h"
#include <Wire.h>


#define MAX31889_FIFO_DEPTH         32   // Max Sample count in fifo
#define MAX31889_DEFAULT_I2C_ADDR   0x50 // 7 bit address

/*
 *
 * MAX31889 Temperature Sensor Class
 * 
 */
class MAX31889
{
    public:
        typedef struct {
            uint8_t fifo_almost_full;
            uint8_t temp_low;
            uint8_t temp_high;
            uint8_t temp_ready; 
        } status_t;

        typedef enum {
            GPIO_NUM_0 = 0,
            GPIO_NUM_1,
            GPIO_NUM_MAX
        } gpio_t;

        typedef enum {
            GPIO_MODE_0 = 0,
            GPIO_MODE_1,
            GPIO_MODE_2,
            GPIO_MODE_3,
            GPIO_MODE_MAX
        } gpio_mode_t;

        typedef enum {
            INT_TEMP_RDY         = MAX31889_F_INT_EN_TEMP_RDY,
            INT_TEMP_HIGH        = MAX31889_F_INT_EN_TEMP_HI,
            INT_TEMP_LOW         = MAX31889_F_INT_EN_TEMP_LO,
            INT_FIFO_ALMOST_FULL = MAX31889_F_INT_EN_A_FULL
        } int_mode_t;

        typedef struct {
            uint8_t rom_id[6];
            uint8_t part_id; 
        } id_t;

        typedef struct {
            uint8_t roolover;
            uint8_t allmost_full_type;
            uint8_t stat_clear; 
        } fifo_cfg_t;

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
        int get_gpio_state(gpio_t gpio);
        int set_gpio_state(gpio_t gpio, int state);
        //
        int get_alarm_temp(float &temp_low, float &temp_high);
        int set_alarm_temp(float temp_low=-40.0, float temp_high=125.0);

        int start_meas(void);
        int get_num_of_sample(void);
        int read_samples(float *temp, int num_of_samples);
        
        // fifo functions
        int flush_fifo();
        int get_fifo_cfg(fifo_cfg_t &cfg);
        int set_fifo_cfg(fifo_cfg_t cfg);
        int set_almost_full_depth(unsigned int num_of_samples);

        // generic functions
        int reset_registers(void);

    private:
        uint8_t m_slave_addr;
        TwoWire *m_i2c;

        int read_register(uint8_t reg, uint8_t *data, int len=1);
        int write_register(uint8_t reg, const uint8_t *data, int len=1);

        float    convert_count_2_temp(uint16_t count);
        uint16_t convert_temp_2_count(float temp);   
};

#endif /* _MAX31889_H_ */
