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

#ifndef _MAX40080_H_
#define _MAX40080_H_


#include <MAX40080/MAX40080_registers.h>

#include "Arduino.h"
#include <Wire.h>

#define MAX40080_FULL_RANGE_VOLTAGE (36.0f) // full Range 36.0V
#define MAX40080_VOLTAGE_STEP_PER_BIT (MAX40080_FULL_RANGE_VOLTAGE/4095.0f) // 12bit ADC count (4095)

#define MAX40080_ERR_UNKNOWN            (-1)
#define MAX40080_ERR_CRC_MISMATCH       (-2)
#define MAX40080_ERR_DATA_NOT_VALID     (-3)

/*
 *
 * MAX40080 
 * 
 */
class MAX40080 {
    public:
        typedef enum {
            MEAS_CURRENT_ONLY        = 0,
            MEAS_VOLTAGE_ONLY        = 1,
            MEAS_CURRENT_AND_VOLTAGE = 2,
            MEAS_NONE,         
        } measure_type_t;

        typedef enum {
            INTR_ID_WAKEUP           = 1<<0,    
            INTR_ID_CONV_READY       = 1<<1,
            INTR_ID_OVER_I           = 1<<2,    
            INTR_ID_OVER_V           = 1<<3,
            INTR_ID_UNDER_V          = 1<<4,   
            INTR_ID_I2C_TIMEOUT      = 1<<5, 
            INTR_ID_OVERFLOW_WARNING = 1<<6, 
            INTR_ID_OVERFLOW         = 1<<7
        } intr_id_t;

        typedef enum {
            OP_MODE_STANDBY           = 0, // Default mode at device power up   
            OP_MODE_LOW_POWER         = 1, // 
            OP_MODE_SINGLE_CONVERTION = 2, // Oneshot mode  
            OP_MODE_CONTINUOUS        = 3, // Continuous measurement
            OP_MODE_4SPS              = 4, // Fixed sample rate at 4sps 
            OP_MODE_1SPS              = 5, // Fixed sample rate at 1sps
            OP_MODE_1_DIV_4SPS        = 6, // Fixed sample rate at 0.25sps
            OP_MODE_1_DIV_16SPS       = 7  // Fixed sample rate at 0.0625sps
        } operation_mode_t;

        typedef enum {
            INPUT_RANGE_50mV,
            INPUT_RANGE_10mV 
        } input_range_t;

        typedef enum { // D stands for Dot
            SAMPLE_RATE_15_KSPS           = 0, //
            SAMPLE_RATE_23D45_KSPS        = 2, //
            SAMPLE_RATE_30_KSPS           = 3, //
            SAMPLE_RATE_37D5_KSPS         = 4, //
            SAMPLE_RATE_47D1_KSPS         = 5, //
            SAMPLE_RATE_60_KSPS           = 6, //
            SAMPLE_RATE_93D5_KSPS         = 7, //
            SAMPLE_RATE_120_KSPS          = 8, //
            SAMPLE_RATE_150_KSPS          = 9, //
            SAMPLE_RATE_234D5_KSPS        = 10, //
            SAMPLE_RATE_375_KSPS          = 11, //
            SAMPLE_RATE_468D5_KSPS        = 12, //
            SAMPLE_RATE_750_KSPS          = 13, //
            SAMPLE_RATE_1000_KSPS         = 14, //
            SAMPLE_RATE_0D5_KSPS          = 15, //
         } adc_sample_rate_t;

        typedef enum {
            AVERAGE_1_SAMPLE       = 0, // No average
            AVERAGE_8_SAMPLES      = 1, // Average among   8 samples
            AVERAGE_16_SAMPLES     = 2, // Average among  16 samples
            AVERAGE_32_SAMPLES     = 3, // Average among  32 samples
            AVERAGE_64_SAMPLES     = 4, // Average among  64 samples
            AVERAGE_128_SAMPLES    = 5  // Average among 128 samples
         } digital_filter_t;

        typedef struct {
            bool    wakeup;         // Wakeup current reached
            bool    conv_ready;     // Indicated ADC convertion completed
            bool    over_i;         //  
            bool    over_v;         //  
            bool    under_v;        //  
            bool    i2c_timeout;    //  
            bool    fifo_alarm;     //  
            bool    fifo_overflow;  // When set to 1 it indicates that the FIFO is completely full with 64 data on it.
            uint8_t fifo_data_count;// 6-bit counter that indicates the number of data that are currently inside the FIFO. Range is from 0 to 63.
        } reg_status_t;

        typedef struct {
            operation_mode_t   mode;
            bool               i2c_timeout;
            bool               alert;
            bool               pec;
            input_range_t      input_range;
            bool               stay_hs_mode;
            adc_sample_rate_t  adc_sample_rate;
            digital_filter_t   digital_filter;
        } reg_cfg_t;
        
        typedef struct {
            measure_type_t store_iv; // These two bits determine whether the device measures and stores into the FIFO either current or voltage or both current and voltage.
            uint8_t overflow_warning;// Overflow threshold
            bool rollover;           // Roll over
            bool flush;              // flush fifo               
        } reg_fifo_cfg_t;
        
        // constructer
        MAX40080(TwoWire *i2c, uint8_t i2c_addr, float shuntResistor);
        //
        void begin(void);
        
        int get_status(reg_status_t &stat);
        int flush_fifo(void);
        int set_interrupt_status(intr_id_t interrupt, bool status);
        int clear_interrupts(void);
        int clear_interrupt_flag(intr_id_t interrupt);

        int get_configuration(reg_cfg_t &cfg);
        int set_configuration(reg_cfg_t  cfg);
        int get_fifo_configuration(reg_fifo_cfg_t &cfg);
        int set_fifo_configuration(reg_fifo_cfg_t  cfg);

        int get_voltage(float &voltage);
        int get_current(float  &current);
        int get_current_and_voltage(float &current, float &voltage);

        int get_threshold_over_current(float  &current);
        int get_threshold_over_voltage(float  &voltage);
        int get_threshold_under_voltage(float  &voltage);
        int get_wakeup_current(float  &current);
        int get_max_peak_current(float  &current);
        //
        int set_threshold_over_current(float  current);
        int set_threshold_over_voltage(float  voltage);
        int set_threshold_under_voltage(float voltage);
        int set_wakeup_current(float  current);

        int send_quick_command(void);
        
    private:
        TwoWire *m_i2c;
        uint8_t m_slave_addr;
        reg_cfg_t m_reg_cfg;
        float m_shuntResistor;
        
        int write_register(uint8_t reg, const uint8_t *buf, uint8_t len=1);
        int read_register(uint8_t reg, uint8_t *buf, uint8_t len=1);
        
        uint16_t convert_voltage_2_count(float voltage, int resolution);
        float convert_count_2_voltage(uint16_t count, int resolution);
        
        uint16_t convert_current_2_count(float current);
        float convert_count_2_current(uint16_t count);

};

#endif /* _MAX40080_H_ */
