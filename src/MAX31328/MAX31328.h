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


#ifndef _MAX31328_H_
#define _MAX31328_H_


#include <MAX31328/MAX31328_registers.h>

#include "Arduino.h"
#include <Wire.h>
#include <time.h>


typedef struct {
    uint32_t seconds;  // Use decimal value. Member fx's convert to BCD
    uint32_t minutes;  // Use decimal value. Member fx's convert to BCD
    uint32_t hours;    // Use decimal value. Member fx's convert to BCD
    bool am_pm;        // TRUE for PM, same logic as datasheet
    bool mode;         // TRUE for 12 hour, same logic as datasheet
}max31328_time_t;

typedef struct {
    uint32_t day;       // Use decimal value. Member fx's convert to BCD
    uint32_t date;      // Use decimal value. Member fx's convert to BCD
    uint32_t month;     // Use decimal value. Member fx's convert to BCD
    uint32_t year;      // Use decimal value. Member fx's convert to BCD
}max31328_calendar_t;

typedef struct {
    //Seconds and am1 not used for alarm2
    uint32_t seconds;   // Use decimal value. Member fx's convert to BCD
    uint32_t minutes;   // Use decimal value. Member fx's convert to BCD
    uint32_t hours;     // Use decimal value. Member fx's convert to BCD
    uint32_t day;       // Use decimal value. Member fx's convert to BCD
    uint32_t date;      // Use decimal value. Member fx's convert to BCD
    bool am1;           // Flag for setting alarm rate
    bool am2;           // Flag for setting alarm rate
    bool am3;           // Flag for setting alarm rate
    bool am4;           // Flag for setting alarm rate
    bool am_pm;         // TRUE for PM, same logic as datasheet
    bool mode;          // TRUE for 12 hour, same logic as datasheet
    bool dy_dt;         // TRUE for Day, same logic as datasheet
}max31328_alrm_t;

typedef struct {
    uint8_t control;  // Binary data for read/write of control register 
    uint8_t status;   // Binary data  for read/write of status register
}max31328_cntl_stat_t;
        
                
/******************************************************************//**
* MAX31328 Class
**********************************************************************/
class MAX31328
{
    public:
        MAX31328(TwoWire *i2c, uint8_t i2c_addr=MAX3128_I2C_ADDRESS);
        
        void begin(void);
        
        /**********************************************************//**
        * Sets the time on MAX31328
        * Struct data is in integer format, not BCD.  Fx will convert
        * to BCD for you.
        *
        * On Entry:
        *     @param[in] time - struct cotaining time data; 
        *
        * On Exit:
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //time = 12:00:00 AM 12hr mode
        * max31328_time_t time = {12, 0, 0, 0, 1}
        * uint16_t rtn_val;
        *
        * rtn_val = rtc.set_time(time);
        *
        * @endcode
        **************************************************************/
        uint16_t set_time(max31328_time_t time);
        
        
        /**********************************************************//**
        * Sets the calendar on MAX31328
        *
        * On Entry:
        *     @param[in] calendar - struct cotaining calendar data 
        *
        * On Exit:
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //see datasheet for calendar format
        * max31328_calendar_t calendar = {1, 1, 1, 0}; 
        * uint16_t rtn_val;
        *
        * rtn_val = rtc.set_calendar(calendar);
        *
        * @endcode
        **************************************************************/
        uint16_t set_calendar(max31328_calendar_t calendar);
        
        
        /**********************************************************//**
        * Set either Alarm1 or Alarm2 of MAX31328
        *
        * On Entry:
        *     @param[in] alarm - struct cotaining alarm data 
        *                        
        *     @param[in] one_r_two - TRUE for Alarm1 and FALSE for 
        *                            Alarm2
        *
        * On Exit:
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //see max31328.h for .members and datasheet for alarm format
        * max31328_alrm_t alarm; 
        * uint16_t rtn_val;
        *
        * rtn_val = rtc.set_alarm(alarm, FALSE);
        *
        * @endcode
        **************************************************************/
        uint16_t set_alarm(max31328_alrm_t alarm, bool one_r_two);
        
        
        /**********************************************************//**
        * Set control and status registers of MAX31328
        *
        * On Entry:
        *     @param[in] data - Struct containing control and status 
        *                       register data
        *
        * On Exit:
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //do not use 0xAA, see datasheet for appropriate data 
        * max31328_cntl_stat_t data = {0xAA, 0xAA}; 
        *
        * rtn_val = rtc.set_cntl_stat_reg(data);
        *
        * @endcode
        **************************************************************/
        uint16_t set_cntl_stat_reg(max31328_cntl_stat_t data);
        
        
        /**********************************************************//**
        * Gets the time on MAX31328
        *
        * On Entry:
        *     @param[in] time - pointer to struct for storing time data
        *
        * On Exit:
        *     @param[out] time - contains current integrer rtc time 
        *                        data
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //time = 12:00:00 AM 12hr mode
        * max31328_time_t time = {12, 0, 0, 0, 1} 
        * uint16_t rtn_val;
        *
        * rtn_val = rtc.get_time(&time);
        *
        * @endcode
        **************************************************************/
        uint16_t get_time(max31328_time_t* time);
        
        
        /**********************************************************//**
        * Gets the calendar on MAX31328
        *
        * On Entry:
        *     @param[in] calendar - pointer to struct for storing 
        *                           calendar data
        *
        * On Exit:
        *     @param[out] calendar - contains current integer rtc 
        *                            calendar data
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //see datasheet for calendar format
        * max31328_calendar_t calendar = {1, 1, 1, 0}; 
        * uint16_t rtn_val;
        *
        * rtn_val = rtc.get_calendar(&calendar);
        *
        * @endcode
        **************************************************************/
        uint16_t get_calendar(max31328_calendar_t* calendar);
        
        
        /**********************************************************//**
        * Get either Alarm1 or Alarm2 of MAX31328
        *
        * On Entry:
        *     @param[in] alarm - pointer to struct for storing alarm 
        *                        data; 
        *
        *     @param[in] one_r_two - TRUE for Alarm1 and FALSE for 
        *                            Alarm2
        *
        * On Exit:
        *     @param[out] alarm - contains integer alarm data
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //see max31328.h for .members and datasheet for alarm format
        * max31328_alrm_t alarm; 
        * uint16_t rtn_val;
        *
        * rtn_val = rtc.get_alarm(&alarm, FALSE);
        *
        * @endcode
        **************************************************************/
        uint16_t get_alarm(max31328_alrm_t* alarm, bool one_r_two);
        
        
        /**********************************************************//**
        * Get control and status registers of MAX31328
        *
        * On Entry:
        *     @param[in] data - pointer to struct for storing control 
        *                       and status register data
        *
        * On Exit:
        *     @param[out] data - contains control and status registers
        *                        data
        *     @return return value = 0 on success, non-0 on failure
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * //do not use 0xAA, see datasheet for appropriate data 
        * max31328_cntl_stat_t data = {0xAA, 0xAA}; 
        *
        * rtn_val = rtc.get_cntl_stat_reg(&data);
        *
        * @endcode
        **************************************************************/
        uint16_t get_cntl_stat_reg(max31328_cntl_stat_t* data);
        
        
        /**********************************************************//**
        * Get temperature data of MAX31328
        *
        * On Entry:
        *
        * On Exit:
        *     @return return value = raw temperature data
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * uint16_t temp; 
        *
        * temp = rtc.get_temperature();
        *
        * @endcode
        **************************************************************/
        uint16_t get_temperature(void);
        
        
        /**********************************************************//**
        * Get epoch time based on current RTC time and date.  
        * MAX31328 must be configured and running before this fx is 
        * called
        *
        * On Entry:
        *
        * On Exit:
        *     @return return value = epoch time
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * MAX31328 rtc(&Wire); 
        * 
        * uint32_t epoch_time; 
        *
        * epoch_time = rtc.get_epoch();
        *
        * @endcode
        **************************************************************/
        time_t get_epoch(void);

    private:
        TwoWire *m_i2c;
        uint8_t  m_slave_addr;

        uint16_t uchar_2_bcd(uint8_t data);
        uint8_t bcd_2_uchar(uint8_t bcd);

        int read_register(uint8_t reg, uint8_t *buf, uint8_t len=1);
        int write_register(uint8_t reg, const uint8_t *buf, uint8_t len=1);
};
#endif /* _MAX31328_H_ */
