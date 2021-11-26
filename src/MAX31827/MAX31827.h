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

#ifndef _MAX31827_H_
#define _MAX31827_H_


#include <MAX31827/MAX31827_registers.h>

#include "Arduino.h"
#include <Wire.h>


/*
 *
 * MAX31827 Temperature Sensor Class
 * 
 */
class MAX31827 {
    public:
        typedef struct {
            bool temp_ready; // true: ready, false: not ready
            bool pec_err;    // true: PEC error occur, false: No error
            bool temp_low;
            bool temp_high; 
        } status_t;

        typedef enum {
            PERIOD_0_SHUTDOWN  = MAX31827_S_CFG_CONV_RATE_SHUTDOWN,
            PERIOD_1_DIV_64SEC = MAX31827_S_CFG_CONV_RATE_1_DIV_64SEC,
            PERIOD_1_DIV_32SEC = MAX31827_S_CFG_CONV_RATE_1_DIV_32SEC,
            PERIOD_1_DIV_16SEC = MAX31827_S_CFG_CONV_RATE_1_DIV_16SEC,
            PERIOD_1_DIV_4SEC  = MAX31827_S_CFG_CONV_RATE_1_DIV_4SEC,
            PERIOD_1_DIV_1SEC  = MAX31827_S_CFG_CONV_RATE_1_DIV_1SEC,
            PERIOD_4_DIV_1SEC  = MAX31827_S_CFG_CONV_RATE_4_DIV_1SEC,
            PERIOD_8_DIV_1SEC  = MAX31827_S_CFG_CONV_RATE_8_DIV_1SEC,
            // One Shut moe
            PERIOD_ONE_SHOT,
        } conv_period_t;

        typedef enum {
            RESOLUTION_8_BIT   = MAX31827_S_CFG_RESOLUTION_8_BIT,
            RESOLUTION_9_BIT   = MAX31827_S_CFG_RESOLUTION_9_BIT,
            RESOLUTION_10_BIT  = MAX31827_S_CFG_RESOLUTION_10_BIT,
            RESOLUTION_12_BIT  = MAX31827_S_CFG_RESOLUTION_12_BIT
        } resolution_t;

        typedef enum {
            MODE_COMPARE   = 0,
            MODE_INTERRUPT = 1
        } mode_t;

        typedef enum {
            FAULT_NUMBER_1   = MAX31827_S_CFG_FAULT_QUE_1,
            FAULT_NUMBER_2   = MAX31827_S_CFG_FAULT_QUE_2,
            FAULT_NUMBER_4   = MAX31827_S_CFG_FAULT_QUE_4,
            FAULT_NUMBER_8   = MAX31827_S_CFG_FAULT_QUE_8
        } fault_t;

        // constructer
        MAX31827(TwoWire *i2c, uint8_t i2c_addr);
        //
        void begin(void);
        //
        int get_status(status_t &stat);
        
        //
        int set_temp(float temp, uint8_t reg);
        int set_alarm(float temp_low, float temp_high);
        int get_alarm(float &temp_low, float &temp_high);
        int set_alarm_hyst(float tl_hyst, float th_hyst);
        int get_alarm_hyst(float &tl_hyst, float &th_hyst);

        int start_meas(conv_period_t period);
        int get_temp(float &temp, uint8_t reg=MAX31827_R_TEMPERATURE);
        //
        int set_pec_status(bool enable);
        int set_timeout_status(bool enable);
        int set_resolution(resolution_t res);
        int set_alarm_polarity(bool high);
        int set_cmp_int_mode(mode_t mode);
        int set_fault_number(fault_t fault);

    private:
        uint8_t m_slave_addr;
        TwoWire *m_i2c;
        bool m_pec_status;

        int read_register(uint8_t reg, uint16_t &val);
        int write_register(uint8_t reg, uint16_t val);
};

#endif /* _MAX31827_H_ */
