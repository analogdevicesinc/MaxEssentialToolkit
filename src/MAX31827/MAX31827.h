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

#include <Arduino.h>
#include <Wire.h>


#define MAX31827_ERR_UNKNOWN            (-1)
#define MAX31827_ERR_CRC_MISMATCH       (-2)

/*
 *
 * MAX31827 Temperature Sensor
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
            PERIOD_0_SHUTDOWN  = 0<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_1_DIV_64SEC = 1<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_1_DIV_32SEC = 2<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_1_DIV_16SEC = 3<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_1_DIV_4SEC  = 4<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_1_DIV_1SEC  = 5<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_4_DIV_1SEC  = 6<<MAX31827_F_CFG_CONV_RATE_POS,
            PERIOD_8_DIV_1SEC  = 7<<MAX31827_F_CFG_CONV_RATE_POS,
            // One Shut moe
            PERIOD_ONE_SHOT,
        } conv_period_t;

        typedef enum {
            RESOLUTION_8_BIT   = 0<<MAX31827_F_CFG_RESOLUTION_POS,
            RESOLUTION_9_BIT   = 1<<MAX31827_F_CFG_RESOLUTION_POS,
            RESOLUTION_10_BIT  = 2<<MAX31827_F_CFG_RESOLUTION_POS,
            RESOLUTION_12_BIT  = 3<<MAX31827_F_CFG_RESOLUTION_POS
        } resolution_t;

        typedef enum {
            MODE_COMPARE   = 0,
            MODE_INTERRUPT = 1
        } mode_t;

        typedef enum {
            FAULT_NUMBER_1   = 0<<MAX31827_F_CFG_FAULT_QUE_POS,
            FAULT_NUMBER_2   = 1<<MAX31827_F_CFG_FAULT_QUE_POS,
            FAULT_NUMBER_4   = 2<<MAX31827_F_CFG_FAULT_QUE_POS,
            FAULT_NUMBER_8   = 3<<MAX31827_F_CFG_FAULT_QUE_POS
        } fault_t;

        // constructer
        MAX31827(TwoWire *i2c, uint8_t i2c_addr);
        //
        void begin(void);
        //
        int get_status(status_t &stat);
        
        //
        int set_alarm(float temp_low, float temp_high);
        int get_alarm(float &temp_low, float &temp_high);
        int set_alarm_hyst(float tl_hyst, float th_hyst);
        int get_alarm_hyst(float &tl_hyst, float &th_hyst);
        int set_temp(float temp, uint8_t register);

        int start_meas(conv_period_t period);
        int get_temp(float &temp, uint8_t register=MAX31827_R_TEMPERATURE);
        //
        int set_timeout_status(bool enable);
        int set_resolution(resolution_t res);
        int set_alarm_polarity(bool high);
        int set_cmp_int_mode(mode_t mode);
        int set_fault_number(fault_t fault);
        int set_pec_status(bool enable);

        // Register direct access function
        int read_register(uint8_t register, uint16_t &val);
        int write_register(uint8_t register, uint16_t val);

    private:
        TwoWire *m_i2c;
        uint8_t  m_slave_addr;
        bool     m_pec_status;
};

#endif /* _MAX31827_H_ */
