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


#include <MAX31328/MAX31328.h>
#include <stdarg.h>


#define pr_err(msg) Serial.println("max31328.cpp: " msg)

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
* Max31328 rtc(D14, D15); 
*
* @endcode
**************************************************************/
MAX31328::MAX31328(TwoWire *i2c, uint8_t i2c_addr)
{
  if (i2c == NULL) {
    pr_err("i2c object is invalid!");
    while (1);
  }
  m_i2c = i2c;
  m_slave_addr = i2c_addr;

}

void MAX31328::begin(void)
{
    m_i2c->begin();
}

int MAX31328::write(const char *data, int length)
{
  int ret;

  m_i2c->beginTransmission(m_slave_addr);
  m_i2c->write(data,length);
  ret = m_i2c->endTransmission();

  if (ret != 0) {
    pr_err("i2c write failed");
  }

  return ret;
}

int MAX31328::read(char *data, int length)
{
  int counter = 0;

  m_i2c->requestFrom((uint8_t)m_slave_addr, (uint8_t)length);

  while (m_i2c->available()) { // slave may send less than requested
    data[counter++] = m_i2c->read(); // receive a byte as character
  }

  if(counter == length) {
    return 0;
  } else {
    pr_err("i2c write failed.");
    return -1;
  }
}

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
* MAX31328 rtc(D14, D15); 
* 
* //time = 12:00:00 AM 12hr mode
* max31328_time_t time = {12, 0, 0, 0, 1}
* uint16_t rtn_val;
*
* rtn_val = rtc.set_time(time);
*
* @endcode
**************************************************************/
uint16_t MAX31328::set_time(max31328_time_t time)
{
    uint8_t data[] = {0,0,0,0};
    uint8_t data_length = 0;
    uint8_t max_hour = 24;
    
    data[data_length++] = MAX31328_R_SECONDS;
    data[data_length++] = uchar_2_bcd(time.seconds);
    data[data_length++] = uchar_2_bcd(time.minutes);
    
    //format Hours register
    data[data_length] = uchar_2_bcd(time.hours);
    if(time.mode)
    {
        max_hour = max_hour/2;
        
        data[data_length] |= MAX31328_F_HOURS_F24_12;
        if(time.am_pm)
        {
            data[data_length] |= MAX31328_F_HOURS_AM_PM;
        }
        
    }
    else
    {
        max_hour = max_hour - 1;
    }
    data_length++;
    
    //Make sure data is within range.  
    if((time.seconds > 59) || (time.minutes > 59) || (time.hours > max_hour))
    {
        return(1);
    }
    else
    {
        return(write((const char*) data, data_length));
    }
}


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
* MAX31328 rtc(D14, D15); 
* 
* //see datasheet for calendar format
* max31328_calendar_t calendar = {1, 1, 1, 0}; 
* uint16_t rtn_val;
*
* rtn_val = rtc.set_calendar(calendar);
*
* @endcode
**************************************************************/
uint16_t MAX31328::set_calendar(max31328_calendar_t calendar)
{
    uint8_t data[] = {0,0,0,0,0};
    uint8_t data_length = 0;
    
    data[data_length++] = MAX31328_R_DAY;
    data[data_length++] = uchar_2_bcd(calendar.day);
    data[data_length++] = uchar_2_bcd(calendar.date);
    data[data_length++] = uchar_2_bcd(calendar.month);
    data[data_length++] = uchar_2_bcd(calendar.year);

    //Make sure data is within range.  
    if(((calendar.day < 1) || (calendar.day > 7)) || 
       ((calendar.date < 1) || (calendar.date > 31)) || 
       ((calendar.month < 1) || (calendar.month > 12)) || 
       (calendar.year > 99))
    {
        return(1);
    }
    else
    {
        return(write((const char*) data, data_length));
    }
}


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
* MAX31328 rtc(D14, D15); 
* 
* //see max31328.h for .members and datasheet for alarm format
* max31328_alrm_t alarm; 
* uint16_t rtn_val;
*
* rtn_val = rtc.set_alarm(alarm, FALSE);
*
* @endcode
**************************************************************/
uint16_t MAX31328::set_alarm(max31328_alrm_t alarm, bool one_r_two)
{
    uint8_t data[] = {0,0,0,0,0};
    uint8_t data_length = 0;
    uint8_t max_hour = 24;
    uint8_t mask_var = 0;
    
    //setting alarm 1 or 2?
    if(one_r_two)
    {
        data[data_length++] = MAX31328_R_ALRM1_SECONDS;
        
        //config seconds register
        if(alarm.am1)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        data[data_length++] =  (mask_var | uchar_2_bcd(alarm.seconds));
        mask_var = 0;
        
        //config minutes register
        if(alarm.am2)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        data[data_length++] =  (mask_var | uchar_2_bcd(alarm.minutes));
        mask_var = 0;
        
        //config hours register
        if(alarm.am3)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        if(alarm.mode)
        {
            max_hour = max_hour/2;
            mask_var |= MAX31328_F_HOURS_F24_12;
            if(alarm.am_pm)
            {
                mask_var |= MAX31328_F_HOURS_AM_PM;
            }
        } 
        else
        {
            max_hour = max_hour - 1;
        }
        data[data_length++] =  (mask_var | uchar_2_bcd(alarm.hours));
        mask_var = 0;
        
        //config day/date register
        if(alarm.am4)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        if(alarm.dy_dt)
        {
            mask_var |= MAX31328_F_ALRMX_DY_DT;
            data[data_length++] =  (mask_var | uchar_2_bcd(alarm.day));
        } 
        else
        {
            data[data_length++] =  (mask_var | uchar_2_bcd(alarm.date));
        }
        mask_var = 0;
    }
    else
    {
        data[data_length++] = MAX31328_R_ALRM2_MINUTES;
        
        //config minutes register
        if(alarm.am2)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        data[data_length++] =  (mask_var | uchar_2_bcd(alarm.minutes));
        mask_var = 0;
        
        //config hours register
        if(alarm.am3)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        if(alarm.mode)
        {
            max_hour = max_hour/2;
            mask_var |= MAX31328_F_HOURS_F24_12;
            if(alarm.am_pm)
            {
                mask_var |= MAX31328_F_HOURS_AM_PM;
            }
        } 
        else
        {
            max_hour = max_hour - 1;
        }
        data[data_length++] =  (mask_var | uchar_2_bcd(alarm.hours));
        mask_var = 0;
        
        //config day/date register
        if(alarm.am4)
        {
           mask_var |= MAX31328_F_ALRMX_ALRM_MASK;
        }
        if(alarm.dy_dt)
        {
            mask_var |= MAX31328_F_ALRMX_DY_DT;
            data[data_length++] =  (mask_var | uchar_2_bcd(alarm.day));
        } 
        else
        {
            data[data_length++] =  (mask_var | uchar_2_bcd(alarm.date));
        }
        mask_var = 0;
    }  
    
    //Make sure data is within range.
    if((alarm.seconds > 59) || (alarm.minutes > 59) || (alarm.hours > max_hour) || 
       ((alarm.day < 1) || (alarm.day > 7)) || 
       ((alarm.date < 1) || (alarm.date > 31)))
    {
        return(1);
    }
    else
    {
        return(write((const char*) data, data_length));
    }
}


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
* MAX31328 rtc(D14, D15);  
* 
* //do not use 0xAA, see datasheet for appropriate data 
* max31328_cntl_stat_t data = {0xAA, 0xAA}; 
*
* rtn_val = rtc.set_cntl_stat_reg(data);
*
* @endcode
**************************************************************/
uint16_t MAX31328::set_cntl_stat_reg(max31328_cntl_stat_t data)
{
    uint8_t local_data[] = {0,0,0};
    uint8_t data_length = 0;
    
    local_data[data_length++] = MAX31328_R_CONTROL;
    local_data[data_length++] = data.control;
    local_data[data_length++] = data.status;

    //users responsibility to make sure data is logical
    return(write((const char*) local_data, data_length));
}


/**********************************************************//**
* Gets the time on MAX31328
*
* On Entry:
*     @param[in] time - pointer to struct for storing time data
*
* On Exit:
*     @param[out] time - contains current integer rtc time 
*                        data
*     @return return value = 0 on success, non-0 on failure
*
* Example:
* @code
* 
* //instantiate rtc object
* MAX31328 rtc(D14, D15); 
* 
* //time = 12:00:00 AM 12hr mode
* max31328_time_t time = {12, 0, 0, 0, 1} 
* uint16_t rtn_val;
*
* rtn_val = rtc.get_time(&time);
*
* @endcode
**************************************************************/
uint16_t MAX31328::get_time(max31328_time_t* time)
{
    uint16_t rtn_val = 1;
    uint8_t data[3];
    
    data[0] = MAX31328_R_SECONDS;
    rtn_val = write((const char*) data, 1);
    
    if(!rtn_val)
    {
        rtn_val = read((char *) data, 3);
        
        time->seconds = bcd_2_uchar(data[0]);
        time->minutes = bcd_2_uchar(data[1]);
        time->am_pm = (data[2] & MAX31328_F_HOURS_AM_PM);
        time->mode = (data[2] & MAX31328_F_HOURS_F24_12);
        
        if(time->mode)
        {
            time->hours = bcd_2_uchar((data[2]&0x1F));
        }
        else
        {
            time->hours = bcd_2_uchar((data[2]&0x3F));
        }  
    } 
  
    return(rtn_val);
}


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
* MAX31328 rtc(D14, D15); 
* 
* //see datasheet for calendar format
* max31328_calendar_t calendar = {1, 1, 1, 0}; 
* uint16_t rtn_val;
*
* rtn_val = rtc.get_calendar(&calendar);
*
* @endcode
**************************************************************/
uint16_t MAX31328::get_calendar(max31328_calendar_t* calendar)
{
    uint16_t rtn_val = 1;
    uint8_t data[4];
    
    data[0] = MAX31328_R_DAY;
    rtn_val = write((const char*) data, 1);
    
    if(!rtn_val)
    {
        rtn_val = read((char *) data, 4);
        
        calendar->day = bcd_2_uchar(data[0]);
        calendar->date = bcd_2_uchar(data[1]);
        calendar->month = bcd_2_uchar((data[2]&0x1F));
        calendar->year = bcd_2_uchar(data[3]);
    } 
  
    return(rtn_val);
}


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
* MAX31328 rtc(D14, D15); 
* 
* //see max31328.h for .members and datasheet for alarm format
* max31328_alrm_t alarm; 
* uint16_t rtn_val;
*
* rtn_val = rtc.get_alarm(&alarm, FALSE);
*
* @endcode
**************************************************************/
uint16_t MAX31328::get_alarm(max31328_alrm_t* alarm, bool one_r_two)
{
    uint16_t rtn_val = 1;
    uint8_t data[4];
    
    if(one_r_two)
    {
        data[0] = MAX31328_R_ALRM1_SECONDS;
        rtn_val = write((const char*) data, 1);
        
        if(!rtn_val)
        {
            rtn_val = read((char *) data, 4);
            
            alarm->seconds = bcd_2_uchar(data[0]&0x7F);
            alarm->am1 = (data[0] & MAX31328_F_ALRMX_ALRM_MASK);
            alarm->minutes = bcd_2_uchar(data[1]&0x7F);
            alarm->am2 = (data[1] & MAX31328_F_ALRMX_ALRM_MASK);
            alarm->am3 = (data[2] & MAX31328_F_ALRMX_ALRM_MASK);
            alarm->am_pm = (data[2] & MAX31328_F_HOURS_AM_PM);
            alarm->mode = (data[2] & MAX31328_F_HOURS_F24_12);
            
            if(alarm->mode)
            {
                alarm->hours = bcd_2_uchar((data[2]&0x1F));
            }
            else
            {
                alarm->hours = bcd_2_uchar((data[2]&0x3F));
            }  
            
            if(data[3] & MAX31328_F_ALRMX_DY_DT)
            {
                alarm->dy_dt = 1;
                alarm->day = bcd_2_uchar(data[3]&0x0F);
            }
            else
            {
                alarm->date = bcd_2_uchar(data[3]&0x3F);
            }
            alarm->am4 = (data[3] & MAX31328_F_ALRMX_ALRM_MASK);
        } 
    }
    else
    {
        data[0] = MAX31328_R_ALRM2_MINUTES;
        rtn_val = write((const char*) data, 1);
        
        if(!rtn_val)
        {
            rtn_val = read((char *) data, 4);
            
            alarm->minutes = bcd_2_uchar(data[0]&0x7F);
            alarm->am2 = (data[0] & MAX31328_F_ALRMX_ALRM_MASK);
            alarm->am3 = (data[1] & MAX31328_F_ALRMX_ALRM_MASK);
            alarm->am_pm = (data[1] & MAX31328_F_HOURS_AM_PM);
            alarm->mode = (data[1] & MAX31328_F_HOURS_F24_12);
            
            if(alarm->mode)
            {
                alarm->hours = bcd_2_uchar((data[2]&0x1F));
            }
            else
            {
                alarm->hours = bcd_2_uchar((data[2]&0x3F));
            }  
            
            if(data[2] & MAX31328_F_ALRMX_DY_DT)
            {
                alarm->dy_dt = 1;
                alarm->day = bcd_2_uchar(data[2]&0x0F);
            }
            else
            {
                alarm->date = bcd_2_uchar(data[2]&0x3F);
            }
            alarm->am4 = (data[2] & MAX31328_F_ALRMX_ALRM_MASK);
        } 
    }
    
    return(rtn_val);
}


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
* MAX31328 rtc(D14, D15);  
* 
* //do not use 0xAA, see datasheet for appropriate data 
* max31328_cntl_stat_t data = {0xAA, 0xAA}; 
*
* rtn_val = rtc.get_cntl_stat_reg(&data);
*
* @endcode
**************************************************************/
uint16_t MAX31328::get_cntl_stat_reg(max31328_cntl_stat_t* data)
{
    uint16_t rtn_val = 1;
    uint8_t local_data[2];
    
    local_data[0] = MAX31328_R_CONTROL;
    rtn_val = write((const char*) local_data, 1);
    
    if(!rtn_val)
    {
        rtn_val = read((char *) local_data, 2);
        
        data->control = local_data[0];
        data->status = local_data[1];
    } 
  
    return(rtn_val);
}


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
* MAX31328 rtc(D14, D15); 
* 
* uint16_t temp; 
*
* temp = rtc.get_temperature();
*
* @endcode
**************************************************************/
uint16_t MAX31328::get_temperature(void)
{
    uint16_t rtn_val = 1;
    uint8_t data[2];
    
    data[0] = MAX31328_R_MSB_TEMP;
    rtn_val = write((const char*) data, 1);
    
    if(!rtn_val)
    {
        read((char *) data, 2);
        
        rtn_val = data[0] << 8;
        rtn_val |= data[1];
    } 
  
    return(rtn_val);    
}


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
* MAX31328 rtc(D14, D15); 
* 
* time_t epoch_time; 
*
* epoch_time = rtc.get_epoch();
*
* @endcode
**************************************************************/
time_t MAX31328::get_epoch(void)
{
    //system vars
    struct tm sys_time;
    
    //RTC vars
    max31328_time_t rtc_time = {0,0,0,0,0};
    max31328_calendar_t rtc_calendar = {0,0,0,0};
    
    get_calendar(&rtc_calendar);
    get_time(&rtc_time);
    
    sys_time.tm_wday = rtc_calendar.day - 1;
    sys_time.tm_mday = rtc_calendar.date;
    sys_time.tm_mon = rtc_calendar.month - 1;
    sys_time.tm_year = rtc_calendar.year + 100;
    
    //check for 12hr or 24hr mode
    if(rtc_time.mode)
    {
        //check am/pm
        if(rtc_time.am_pm  && (rtc_time.hours != 12))
        {
            sys_time.tm_hour = rtc_time.hours + 12;
        }
        else
        {
            sys_time.tm_hour = rtc_time.hours;
        }
        
    }
    else
    {
        //24hr mode
        sys_time.tm_hour = rtc_time.hours;
    }
    
    sys_time.tm_min = rtc_time.minutes;
    sys_time.tm_sec = rtc_time.seconds;
    
    //make epoch time
    return(mktime(&sys_time));
}


/**********************************************************//**
* Private mmber fx, converts unsigned char to BCD
*
* On Entry:
*     @param[in] data - 0-255
*
* On Exit:
*     @return bcd_result = BCD representation of data
*
**************************************************************/
uint16_t MAX31328::uchar_2_bcd(uint8_t data)
{
   uint16_t bcd_result = 0;
   
   //Get hundreds
   bcd_result |= ((data/100) << 8);
   data = (data - (data/100)*100);
   
   //Get tens
   bcd_result |= ((data/10) << 4);
   data = (data - (data/10)*10);
   
   //Get ones
   bcd_result |= data;
   
   return(bcd_result);   
}


/**********************************************************//**
* Private mmber fx, converts BCD to a uint8_t
*
* On Entry:
*     @param[in] bcd - 0-99
*
* On Exit:
*     @return rtn_val = integer rep. of BCD
*
**************************************************************/
uint8_t MAX31328::bcd_2_uchar(uint8_t bcd)
{
    uint8_t rtn_val = 0;

    rtn_val += ((bcd&0xf0)>>4)*10;
    rtn_val += (bcd&0x000f);   

    return rtn_val;
}
