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

#ifndef _MAX31341_REGS_H_
#define _MAX31341_REGS_H_

#include "Arduino.h"

#define MAX31341_I2C_ADDRESS                       0x69

// Registers
#define  MAX31341_REG_CONFIG_1                    ( 0x00 )
#define  MAX31341_REG_CONFIG_2                    ( 0x01 )
#define  MAX31341_REG_INT_POLARITY_CONFIG         ( 0x02 )
#define  MAX31341_REG_TIMER_CONFIG                ( 0x03 )
#define  MAX31341_REG_INT_EN                      ( 0x04 )
#define  MAX31341_REG_INT_STATUS                  ( 0x05 )
#define  MAX31341_REG_SECONDS                     ( 0x06 )
#define  MAX31341_REG_MINUTES                     ( 0x07 )
#define  MAX31341_REG_HOURS                       ( 0x08 )
#define  MAX31341_REG_DAY                         ( 0x09 )
#define  MAX31341_REG_DATE                        ( 0x0A )
#define  MAX31341_REG_MONTH                       ( 0x0B )
#define  MAX31341_REG_YEAR                        ( 0x0C )
#define  MAX31341_REG_ALM1_SEC                    ( 0x0D )
#define  MAX31341_REG_ALM1_MIN                    ( 0x0E )
#define  MAX31341_REG_ALM1_HRS                    ( 0x0F )
#define  MAX31341_REG_ALM1DAY_DATE                ( 0x10 )
#define  MAX31341_REG_ALM2_MIN                    ( 0x11 )
#define  MAX31341_REG_ALM2_HRS                    ( 0x12 )
#define  MAX31341_REG_ALM2DAY_DATE                ( 0x13 )
#define  MAX31341_REG_TIMER_COUNT                 ( 0x14 )
#define  MAX31341_REG_TIMER_INIT                  ( 0x15 )
#define  MAX31341_REG_RAM_START                   ( 0x16 )
#define  MAX31341_REG_RAM_END                     ( 0x55 )
#define  MAX31341_REG_PWR_MGMT                    ( 0x56 )
#define  MAX31341_REG_TRICKLE                     ( 0x57 )
#define  MAX31341_REG_CLOCK_SYNC                  ( 0x58 )
#define  MAX31341_REG_REV_ID                      ( 0x59 )


typedef struct {
    union {
        uint8_t raw;
        struct {
            uint8_t seconds : 4;    /**< RTC seconds value. */
            uint8_t sec_10  : 3;    /**< RTC seconds in multiples of 10 */
            uint8_t         : 1;
        } bits;
        struct {
            uint8_t value   : 7;
            uint8_t         : 1;
        } bcd;
    } seconds;

    union {
        uint8_t raw;
        struct {
            uint8_t minutes : 4;    /**< RTC minutes value */
            uint8_t min_10  : 3;    /**< RTC minutes in multiples of 10 */
            uint8_t         : 1;
        } bits;
        struct {
            uint8_t value   : 7;
            uint8_t         : 1;
        } bcd;
    } minutes;

    union {
        uint8_t raw;
        struct {
            uint8_t hour       : 4; /**< RTC hours value */
            uint8_t hr_10      : 2; /**< RTC hours in multiples of 10 */
            uint8_t            : 2;
        } bits;
        struct {
            uint8_t value      : 6;
            uint8_t            : 2;
        } bcd;
    } hours;

    union {
        uint8_t raw;
        struct {
            uint8_t day : 3;    /**< RTC days */
            uint8_t     : 5;
        } bits;
        struct {
            uint8_t value : 3;
            uint8_t       : 5;
        } bcd;
    } day;

    union {
        uint8_t raw;
        struct {
            uint8_t date    : 4;    /**< RTC date */
            uint8_t date_10 : 2;    /**< RTC date in multiples of 10 */
            uint8_t         : 2;
        } bits;
        struct {
            uint8_t value   : 6;
            uint8_t         : 2;
        } bcd;
    } date;

    union {
        uint8_t raw;
        struct {
            uint8_t month    : 4;   /**< RTC months */
            uint8_t month_10 : 1;   /**< RTC month in multiples of 10 */
            uint8_t          : 2;
            uint8_t century  : 1;   /**< Century bit */
        } bits;
        struct {
            uint8_t value   : 5;
            uint8_t         : 3;
        } bcd;
    } month;

    union {
        uint8_t raw;
        struct {
            uint8_t year    : 4;    /**< RTC years */
            uint8_t year_10 : 4;    /**< RTC year multiples of 10 */
        } bits;
        struct {
            uint8_t value   : 8;
        } bcd;
    } year;
} regs_rtc_time_t;


typedef struct {
    union {
        uint8_t raw;
        struct {
            uint8_t seconds : 4;    /**< Alarm1 seconds */
            uint8_t sec_10  : 3;    /**< Alarm1 seconds in multiples of 10 */
            uint8_t axm1    : 1;    /**< Alarm1 mask bit for minutes */
        } bits;
        struct {
            uint8_t value   : 7;
            uint8_t         : 1;
        } bcd;
    } sec;

    union {
        uint8_t raw;
        struct {
            uint8_t minutes : 4;    /**< Alarm1 minutes */
            uint8_t min_10  : 3;    /**< Alarm1 minutes in multiples of 10 */
            uint8_t axm2    : 1;    /**< Alarm1 mask bit for minutes */
        } bits;
        struct {
            uint8_t value   : 7;
            uint8_t         : 1;
        } bcd;
    } min;

    union {
        uint8_t raw;
        struct {
            uint8_t hour       : 4; /**< Alarm1 hours */
            uint8_t hr_10      : 2; /**< Alarm1 hours in multiples of 10 */
            uint8_t            : 1;
            uint8_t axm3       : 1; /**< Alarm1 mask bit for hours */
        } bits;
        struct {
            uint8_t value      : 6;
            uint8_t            : 2;
        } bcd;
    } hrs;

    union {
        uint8_t raw;
        struct {
            uint8_t day_date : 4;   /**< Alarm1 day/date */
            uint8_t date_10  : 2;   /**< Alarm1 date in multiples of 10 */
            uint8_t dy_dt    : 1;
            uint8_t axm4     : 1;   /**< Alarm1 mask bit for day/date */
        } bits;
        struct {
            uint8_t value   : 3;
            uint8_t         : 5;
        } bcd_day;
        struct {
            uint8_t value   : 6;
            uint8_t         : 2;
        } bcd_date;
    } day_date;
} regs_alarm_t;


typedef union {
    uint8_t raw;
    struct {
        uint8_t swrstn : 1; /**< Software reset */
        uint8_t rs     : 2; /**< Square wave output frequency selection on CLKOUT pin */
        uint8_t osconz : 1; /**< Oscillator is on when set to 0. Oscillator is off when set to 1. */
        uint8_t clksel : 2; /**< Selects the CLKIN frequency */
        uint8_t intcn  : 1; /**< Interrupt control bit. Selects the direction of INTB/CLKOUT */
        uint8_t eclk   : 1; /**< Enable external clock input */
    } bits;
} reg_config1_t;

typedef union {
    uint8_t raw;
    struct {
        uint8_t             : 1;
        uint8_t set_rtc     : 1;    /**< Set RTC */
        uint8_t rd_rtc      : 1;    /**< Read RTC. */
        uint8_t i2c_timeout : 1;    /**< I2C timeout Enable */
        uint8_t bref        : 2;    /**< BREF sets the analog comparator threshold voltage. */
        uint8_t data_reten  : 1;    /**< Sets the circuit into data retention mode. */
        uint8_t             : 1;
    } bits;
} reg_config2_t;

typedef union {
    uint8_t raw;
    struct {
        uint8_t tfs    : 2; /**< Timer frequency selection */
        uint8_t trpt   : 1; /**< Timer repeat mode. It controls the timer interrupt function along with TM. */
        uint8_t        : 1;
        uint8_t te     : 1; /**< Timer enable */
        uint8_t tpause : 1; /**< Timer Pause.*/
        uint8_t        : 2;
    } bits;
} reg_timer_config_t;

typedef union {
    uint8_t raw;
    struct {
        uint8_t d_mode      : 2;    /**< Sets the mode of the comparator to one of the two following: comparator mode, and power management mode/trickle charger mode. */
        uint8_t d_man_sel   : 1;    /**< Default low. When this bit is low, input control block decides which supply to use. When this bit is high, user can manually select whether to use V<sub>CC</sub> or VBACKUP as supply. */
        uint8_t d_vback_sel : 1;    /**< : Default low. When this bit is low, and D_MANUAL_SEL is high, V<sub>CC</sub> is switched to supply. When this bit is high, and D_MANUAL_SEL is high, V<sub>BACKUP</sub> is switched to supply. */
        uint8_t             : 4;
    } bits;
} reg_pwr_mgmt_t;

typedef union {
    uint8_t raw;
    struct {
        uint8_t sync_delay : 2; /* Sync delay to take for the internal countdown chain to reset after the rising edge of Set_RTC */
        uint8_t            : 6;
    } bits;
} reg_clock_sync_t;

typedef union {
    uint8_t raw;
    struct {
        uint8_t            : 4; 
        uint8_t rev_id     : 4; /* Revision Identification Register */
    } bits;
} reg_rev_id_t;

#endif /* _MAX31341_REGS_H_ */
