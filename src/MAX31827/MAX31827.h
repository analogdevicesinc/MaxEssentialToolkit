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


#define MAX31827_DRV_VERSION          "v0.0.1"

#define MAX31827_ERR_UNKNOWN            (-1)
#define MAX31827_ERR_CRC_MISMATCH       (-2)


class MAX31827 {
    public:
        typedef enum {
            PERIOD_0_SHUTDOWN  = 0,
            PERIOD_1_DIV_64SEC = 1,
            PERIOD_1_DIV_32SEC = 2,
            PERIOD_1_DIV_16SEC = 3,
            PERIOD_1_DIV_4SEC  = 4,
            PERIOD_1_DIV_1SEC  = 5,
            PERIOD_4_DIV_1SEC  = 6,
            PERIOD_8_DIV_1SEC  = 7,
            // One Shut moe
            PERIOD_ONE_SHOT,
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
            FAULT_NUMBER_1   = 0,
            FAULT_NUMBER_2   = 1,
            FAULT_NUMBER_4   = 2,
            FAULT_NUMBER_8   = 3
        } fault_t;

        typedef struct {
            bool temp_ready; // true: ready, false: not ready
            bool pec_err;    // true: PEC error occur, false: No error
            bool temp_low;   // under temperature status
            bool temp_high;  // over temperature status
        } status_t;

        typedef union {
            uint16_t raw;
            struct {
                uint8_t oneshot         : 1; // 
                uint8_t conversion_rate : 3; // conv_period_t
                uint8_t pec_enable      : 1; // 
                uint8_t timeout         : 1; // 
                uint8_t resolution      : 2; // resolution_t
                uint8_t alarm_polarity  : 1; // 
                uint8_t comp_int        : 1; // mode_t
                uint8_t fault_queue     : 2; // fault_t
                uint8_t                 : 1; // not used
                uint8_t pec_error       : 1; // 
                uint8_t under_temp_stat : 1; // 
                uint8_t over_temp_stat  : 1; // 
            } bits;
        } reg_cfg_t;


        /**
        * @brief        Constructer to driver sensor over i2c interface
        *
        * @param[in]    i2c: I2C instance address
        * @param[in]    i2c_addr: I2C addr
        */
        MAX31827(TwoWire *i2c, uint8_t i2c_addr);

        /**
        * @brief        Must be call one time before using class methods
        *
        */
        void begin(void);
        
        /**
        * @brief        Read status flags of target  
        *               There is not status register on MAX31827 
        *               this function read configuration register and 
        *               provide status related flag in status_t structure
        *
        * @param[out]   status_t
        *
        * @return       0 on success, error code on failure
        */
        int get_status(status_t &stat);
        
        /**
        * @brief        Read configuration register of target. 
        *
        * @param[out]   reg_cfg_t
        *
        * @return       0 on success, error code on failure
        */
        int get_configuration(reg_cfg_t &cfg);

        /**
        * @brief        Set configuration register of target.
        *
        * @param[in]    reg_cfg_t
        *
        * @return       0 on success, error code on failure
        */
        int set_configuration(reg_cfg_t cfg);
        
        /**
        * @brief        Set alarm low and alarm high. 
        *
        * @param[in]    temp_low: Low alarm temperature
        * @param[in]    temp_hig: High alarm temperature
        * 
        * @return       0 on success, error code on failure
        */ 
        int set_alarm(float temp_low, float temp_high);
        
        /**
        * @brief        Get existing alarm low and alarm high values. 
        *
        * @param[out]   temp_low: Existing low alarm temperature
        * @param[out]   temp_high: Existing high alarm temperature
        * 
        * @return       0 on success, error code on failure
        */ 
        int get_alarm(float &temp_low, float &temp_high);

        /**
        * @brief        Set alarm hysteresis. 
        *
        * @param[in]    tl_hyst: low hysteresis
        * @param[in]    th_hyst: high hysteresis
        * 
        * @return       0 on success, error code on failure
        */ 
        int set_alarm_hyst(float tl_hyst, float th_hyst);

        /**
        * @brief        Get existing alarm hysteresis
        *
        * @param[out]   tl_hyst: low hysteresis
        * @param[out]   th_hyst: high hysteresis
        * 
        * @return       0 on success, error code on failure
        */         
        int get_alarm_hyst(float &tl_hyst, float &th_hyst);

        /**
        * @brief        Start temperature conversion.
        *
        * @param[in]    conv_period_t
        * 
        * @return       0 on success, error code on failure
        */
        int start_temp_conversion(conv_period_t period = PERIOD_ONE_SHOT);

        /**
        * @brief        Read temperature value. 
        *
        * @param[out]   tmp: Temperature value that measured
        * @param[in]    register: Temperature register address
        *               please take a look MAX31827_registers.h file
        * 
        * @return       0 on success, error code on failure
        */
        int get_temp(float &temp, uint8_t register = MAX31827_R_TEMPERATURE);
        
        // Register direct access function
        int read_register(uint8_t register, uint16_t &val);
        int write_register(uint8_t register, uint16_t val);

    private:
        TwoWire *m_i2c;
        uint8_t  m_slave_addr;
        bool     m_pec_status;

        int set_temp(float temp, uint8_t register);
};

#endif /* _MAX31827_H_ */
