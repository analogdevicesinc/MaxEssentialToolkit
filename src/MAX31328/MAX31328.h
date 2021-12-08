/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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


#ifndef MAX31328_H
#define MAX31328_H


#include "Arduino.h"
#include <Wire.h>
#include <time.h>
#include <stdarg.h>


#define MAX31328_I2C_ADRS 0x68 
#define I2C_WRITE 0
#define I2C_READ  1

#define AM_PM     (1 << 5) 
#define MODE      (1 << 6)
#define DY_DT     (1 << 6)
#define ALRM_MASK (1 << 7)

//control register bit masks
#define A1IE  (1 << 0)
#define A2IE  (1 << 1)
#define INTCN (1 << 2)
#define RS1   (1 << 3)
#define RS2   (1 << 4)
#define CONV  (1 << 5)
#define BBSQW (1 << 6)
#define EOSC  (1 << 7)

//status register bit masks
#define A1F     (1 << 0)
#define A2F     (1 << 1)
#define BSY     (1 << 2)
#define EN32KHZ (1 << 3)
#define OSF     (1 << 7)
  

/**
* max31328_time_t - Struct for containing time data.
* 
* Members:
*
* - uint32_t seconds - Use decimal value. Member fx's convert to BCD
*
* - uint32_t minutes - Use decimal value. Member fx's convert to BCD
*
* - uint32_t hours   - Use decimal value. Member fx's convert to BCD
*
* - bool am_pm      - TRUE for PM, same logic as datasheet
*
* - bool mode       - TRUE for 12 hour, same logic as datasheet
*/
typedef struct
{
    uint32_t seconds; 
    uint32_t minutes; 
    uint32_t hours; 
    bool am_pm; 
    bool mode; 
}max31328_time_t;


/**
* max31328_calendar_t - Struct for containing calendar data.
* 
* Members:
*
* - uint32_t day   - Use decimal value. Member fx's convert to BCD
*
* - uint32_t date  - Use decimal value. Member fx's convert to BCD
*
* - uint32_t month - Use decimal value. Member fx's convert to BCD
*
* - uint32_t year  - Use decimal value. Member fx's convert to BCD
*/
typedef struct
{
    uint32_t day; 
    uint32_t date; 
    uint32_t month; 
    uint32_t year;
}max31328_calendar_t;


/**
* max31328_alrm_t - Struct for containing alarm data.
* 
* Members:
*
* - uint32_t seconds - Use decimal value. Member fx's convert to BCD 
*
* - uint32_t minutes - Use decimal value. Member fx's convert to BCD 
*
* - uint32_t hours   - Use decimal value. Member fx's convert to BCD 
*
* - uint32_t day     - Use decimal value. Member fx's convert to BCD 
*
* - uint32_t date    - Use decimal value. Member fx's convert to BCD 
*
* - bool am1        - Flag for setting alarm rate
*
* - bool am2        - Flag for setting alarm rate
*
* - bool am3        - Flag for setting alarm rate
*
* - bool am4        - Flag for setting alarm rate
*
* - bool am_pm      - TRUE for PM, same logic as datasheet
*
* - bool mode       - TRUE for 12 hour, same logic as datasheet
*
* - bool dy_dt      - TRUE for Day, same logic as datasheet
*/
typedef struct
{
    //Seconds and am1 not used for alarm2
    uint32_t seconds; 
    uint32_t minutes; 
    uint32_t hours; 
    uint32_t day; 
    uint32_t date; 
    bool am1; 
    bool am2;
    bool am3;
    bool am4;
    bool am_pm; 
    bool mode; 
    bool dy_dt;
}max31328_alrm_t;


/**
* max31328_cntl_stat_t - Struct for containing control and status 
* register data.
* 
* Members:
*
* - uint8_t control - Binary data for read/write of control register 
*
* - uint8_t status  - Binary data  for read/write of status register 
*/
typedef struct
{
    uint8_t control; 
    uint8_t status; 
}max31328_cntl_stat_t;
        
                
/******************************************************************//**
* Max31328 Class
**********************************************************************/
class Max31328
{
    uint8_t w_adrs = MAX31328_I2C_ADRS, r_adrs = MAX31328_I2C_ADRS;

    TwoWire *i2c;

    /**********************************************************//**
    * Private member fx, converts unsigned char to BCD
    *
    * On Entry:
    *     @param[in] data - 0-255
    *
    * On Exit:
    *     @return bcd_result = BCD representation of data
    *
    **************************************************************/
    uint16_t uchar_2_bcd(uint8_t data);
    
    
    /**********************************************************//**
    * Private member fx, converts BCD to a uint8_t
    *
    * On Entry:
    *     @param[in] bcd - 0-99
    *
    * On Exit:
    *     @return rtn_val = integer rep. of BCD
    *
    **************************************************************/
    uint8_t bcd_2_uchar(uint8_t bcd);


    /**********************************************************//**
    * Private member fx, Writes data into i2c bus.
    *
    * On Entry:
    *     @param[in] address    I2C Address
    *     @param[in] data       Pointer to data array
    *     @param[in] length     Length of data array
    *
    * On Exit:
    *     @return 0 on success, negative error code on failure.
    *
    **************************************************************/
    int write(int address, const char *data, int length);


    /**********************************************************//**
    * Private member fx, Reads data from i2c bus.
    *
    * On Entry:
    *     @param[in] address    I2C Address
    *     @param[in] buffer     Pointer to data buffer
    *     @param[in] length     Number of bytes to be read
    *
    * On Exit:
    *     @return 0 on success, negative error code on failure.
    *
    **************************************************************/
    int read(int address, char *buffer, int length);
    

    public:
    
        /**
        * max31328_regs_t - enumerated MAX31328 registers 
        */
        typedef enum
        {
            SECONDS,
            MINUTES,
            HOURS,
            DAY,
            DATE,
            MONTH,
            YEAR,
            ALRM1_SECONDS,
            ALRM1_MINUTES,
            ALRM1_HOURS,
            ALRM1_DAY_DATE,
            ALRM2_MINUTES,
            ALRM2_HOURS,
            ALRM2_DAY_DATE,
            CONTROL,
            STATUS,
            AGING_OFFSET, //don't touch this register
            MSB_TEMP,
            LSB_TEMP
        }max31328_regs_t;
    
        
        /**********************************************************//**
        * Constructor for Max31328 Class
        *
        * On Entry:
        *     @param[in] i2c Pointer to I2C bus object for this device.
        *
        * On Exit:
        *    @return none
        *
        * Example:
        * @code
        * 
        * //instantiate rtc object
        * Max31328 rtc(&Wire); 
        *
        * @endcode
        **************************************************************/
        Max31328(TwoWire *i2c);
        
        
        /**********************************************************//**
        * Sets the time on MAX31328
        * Struct data is in integrer format, not BCD.  Fx will convert
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
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
        * Max31328 rtc(&Wire); 
        * 
        * uint32_t epoch_time; 
        *
        * epoch_time = rtc.get_epoch();
        *
        * @endcode
        **************************************************************/
        time_t get_epoch(void);
        
};
#endif /* MAX31328_H*/
