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

#ifndef _MAX31343_H_
#define _MAX31343_H_

#include <MAX31343/MAX31343_registers.h>

#include <time.h>
#include <Wire.h>
#include "Arduino.h"


/**
 * @brief	RTC base driver class for Maxim Max3134x RTC series.
 */
class MAX31343
{
public:
    /**
    * @brief	Uncompensated clock output frequency selection
    */
    typedef enum {
        CLKO_FREQ_1HZ,		/**< 1Hz */
        CLKO_FREQ_2HZ,		/**< 2Hz */
        CLKO_FREQ_4HZ,		/**< 4Hz */
        CLKO_FREQ_8HZ,		/**< 8Hz */
        CLKO_FREQ_16HZ,		/**< 16Hz */
        CLKO_FREQ_32HZ,		/**< 32Hz */
        CLKO_FREQ_64HZ,		/**< 64Hz */
        CLKO_FREQ_128HZ,	/**< 128Hz */
        CLKO_FREQ_32000HZ,	/**< 32KHz */
    } clko_freq_t;

    /**
    * @brief	Square wave output frequency selection
    */
    typedef enum {
        SQUARE_WAVE_OUT_FREQ_1HZ,	/**<  1Hz */
        SQUARE_WAVE_OUT_FREQ_2HZ,	/**<  2Hz */
        SQUARE_WAVE_OUT_FREQ_4HZ,	/**<  4Hz */
        SQUARE_WAVE_OUT_FREQ_8HZ,	/**<  8Hz */
        SQUARE_WAVE_OUT_FREQ_16HZ,	/**< 16Hz */
        SQUARE_WAVE_OUT_FREQ_32HZ,	/**< 32Hz */
    } square_wave_out_freq_t;

    /**
    * @brief	Power fail threshold voltage
    */
    typedef enum {
        COMP_THRESH_1V85 = 1,	/**< 1.85V */
        COMP_THRESH_2V15,		/**< 2.15V */
        COMP_THRESH_2V40,		/**< 2.40V */
    } comp_thresh_t;

    /**
    * @brief	Supply voltage select.
    */
    typedef enum {
        POW_MGMT_SUPPLY_SEL_AUTO,	/**< Circuit decides whether to use VCC or VBACKUP */
        POW_MGMT_SUPPLY_SEL_VCC,	/**< Use VCC as supply */
        POW_MGMT_SUPPLY_SEL_VBACK,	/**< Use VBACKUP as supply */
    } power_mgmt_supply_t;

    /**
    * @brief	Selection of charging path's resistor value
    */
    typedef enum {
        TRICKLE_CHARGER_3K_S,   /**< 3Kohm in series with a Schottky diode  */
        TRICKLE_CHARGER_3K_2_S, /**< 3Kohm in series with a Schottky diode  */
        TRICKLE_CHARGER_6K_S,	/**< 6Kohm in series with a Schottky diode */
        TRICKLE_CHARGER_11K_S,    /**<  11Kohm in series with a Schottky diode */
        TRICKLE_CHARGER_3K_D_S,   /**< 3Kohm in series with a diode+Schottky diode  */
        TRICKLE_CHARGER_3K_2_D_S,   /**< 3Kohm in series with a diode+Schottky diode */
        TRICKLE_CHARGER_6K_D_S,   /**< 6Kohm in series with a diode+Schottky diode */
        TRICKLE_CHARGER_11K_D_S,  /**< 11Kohm in series with a diode+Schottky diode */
        TRICKLE_CHARGER_NO_CONNECTION,   /**< No Connection  */
    } trickle_charger_path_t;

    /**
    * @brief	Timer frequency selection
    */
    typedef enum {
        TIMER_FREQ_1024HZ,	/**< 1024Hz */
        TIMER_FREQ_256HZ,	/**< 256Hz */
        TIMER_FREQ_64HZ,	/**< 64Hz */
        TIMER_FREQ_16HZ,	/**< 16Hz */
    } timer_freq_t;

    /**
    * @brief	Selection of interrupt ids
    */
    typedef enum {
        INTR_ID_ALARM1 = 0,		/**< Alarm1 flag */
        INTR_ID_ALARM2 = 1,		/**< Alarm2 flag */
        INTR_ID_TIMER  = 2,		/**< Timer interrupt flag */
        INTR_ID_TEMP   = 3,		/**< Temp sense ready flag */
        INTR_ID_PFAIL  = 5,		/**< Power fail interrupt enable */
        INTR_ID_DOSF   = 6,		/**< Disable Oscillator flag */
        INTR_ID_END,
    } intr_id_t;

    /**
    * @brief	Alarm number selection
    */
    typedef enum {
        ALARM1,	/**< Alarm number 1 */
        ALARM2,	/**< Alarm number 2 */
    } alarm_no_t;

    /**
    * @brief	Alarm periodicity selection
    */
    typedef enum {
        ALARM_PERIOD_EVERYSECOND,	/**< Once per second */
        ALARM_PERIOD_EVERYMINUTE,	/**< Second match / Once per minute */
        ALARM_PERIOD_HOURLY,		/**< Second and Minute match */
        ALARM_PERIOD_DAILY,			/**< Hour, Minute and Second match*/
        ALARM_PERIOD_WEEKLY,		/**< Day and Time match */
        ALARM_PERIOD_MONTHLY,		/**< Date and Time match */
        ALARM_PERIOD_YEARLY,		/**< Month, Date and Time match (Max31342 only) */
        ALARM_PERIOD_ONETIME,		/**< Year, Month, Date and Time match (Max31342 only) */
    } alarm_period_t;

    /**
    * @brief	Selection of INTB/CLKOUT pin function
    */
    typedef enum {
        CONFIGURE_PIN_AS_CLKOUT,	/**< Output is square wave */
        CONFIGURE_PIN_AS_INTB,		/**< Output is interrupt */
    } config_intb_clkout_pin_t;

    /**
     * @brief	Temperature measurement interval for automatic mode
     */
    typedef enum {
        TTS_INTERNAL_1SEC = 0,
        TTS_INTERNAL_2SEC,
        TTS_INTERNAL_4SEC,
        TTS_INTERNAL_8SEC,
        TTS_INTERNAL_16SEC,
        TTS_INTERNAL_32SEC,
        TTS_INTERNAL_64SEC,
        TTS_INTERNAL_128SEC,
    } ttsint_t;

	/**
	* @brief	Base class constructor.
	*
	* @param[in]	i2c Pointer to I2C bus object for this device.
	* @param[in]	7 bit i2c addr
	*/
	MAX31343(TwoWire *i2c, uint8_t i2c_addr);

	/**
	* @brief		Read from a register.
	*
	* @param[in]	reg Address of a register to be read.
	* @param[out]	value Pointer to save result value.
	* @param[in]	len Size of result to be read.
	*
	* @returns		0 on success, negative error code on failure.
	*/
	int read_register(uint8_t reg, uint8_t *value, uint8_t len);

	/**
	* @brief		Write to a register.
	*
	* @param[in]	reg Address of a register to be written.
	* @param[out]	value Pointer of value to be written to register.
	* @param[in]	len Size of result to be written.
	*
	* @returns		0 on success, negative error code on failure.
	*/
	int write_register(uint8_t reg, const uint8_t *value, uint8_t len);

	/**
	* @brief		Read time info from RTC.
	*
	* @param[out]	rtc_time Time info from RTC.
	*
	* @returns		0 on success, negative error code on failure.
	*/
	int get_time(struct tm *rtc_ctime);

	/**
	* @brief		Set time info to RTC.
	*
	* @param[in]	rtc_time Time info to be written to RTC.
	*
	* @returns		0 on success, negative error code on failure.
	*/
	int set_time(const struct tm *rtc_ctime);

	/**
	* @brief		Set an alarm condition
	*
	* @param[in]	alarm_no Alarm number, ALARM1 or ALARM2
	* @param[in]	alarm_time Pointer to alarm time to be set
	* @param[in]	period Alarm periodicity, one of ALARM_PERIOD_*
	*
	* @return		0 on success, error code on failure
	*/
	int set_alarm(alarm_no_t alarm_no, const struct tm *alarm_time, alarm_period_t period);

	/**
	* @brief		Get alarm data & time
	*
	* @param[in]	alarm_no Alarm number, ALARM1 or ALARM2
	* @param[out]	alarm_time Pointer to alarm time to be filled in
	* @param[out]	period Pointer to the period of alarm, one of ALARM_PERIOD_*
	* @param[out]	is_enabled Pointer to the state of alarm
	*
	* @return		0 on success, error code on failure
	*/
	int get_alarm(alarm_no_t alarm_no, struct tm *alarm_time, alarm_period_t *period, bool *is_enabled);

	/**
	* @brief		Set power fail threshold voltage
	*
	* @param[in]	th Set Analog Comparator Threshold level, one of COMP_THRESH_*
	*
	* @return		0 on success, error code on failure
	*/
	int powerfail_threshold_level(comp_thresh_t th);

	/**
	* @brief		Select device power source
	*
	* @param[in]	supply Supply selection, one of POW_MGMT_SUPPLY_SEL_*
	*
	* @return		0 on success, error code on failure
	*/
	int supply_select(power_mgmt_supply_t supply);

    /**
    * @brief		Configure trickle charger charging path, also enable it
    *
    * @param[in]	path Value of resister + diodes
    *
    * @return		0 on success, error code on failure
    */
    int trickle_charger_enable(trickle_charger_path_t path);

	/**
	* @brief		Disable trickle charger
	*
	* @return		0 on success, error code on failure
	*/
	int trickle_charger_disable();

	/**
	* @brief		Select square wave output frequency selection
	*
	* @param[in]	freq Clock frequency, one of SQUARE_WAVE_OUT_FREQ_*
	*
	* @return		0 on success, error code on failure
	*/
	int set_output_square_wave_frequency(square_wave_out_freq_t freq);

	/**
	* @brief		Enable CLKO
	*
	* @param[in]	freq Clock frequency, one of CLKO_FREQ_*
	*
	* @return		0 on success, error code on failure
	*/
	int clko_enable(clko_freq_t freq);

	/**
	* @brief		Disable CLKO
	*
	* @return		0 on success, error code on failure
	*/
	int clko_disable();


	/**
	* @brief		Initialize timer
	*
	* @param[in]	value Timer initial value
	* @param[in]	repeat Timer repeat mode enable/disable
	* @param[in]	freq Timer frequency, one of TIMER_FREQ_*
	*
	* @return		0 on success, error code on failure
	*
	*/
	int timer_init(uint8_t value, bool repeat, timer_freq_t freq);

	/**
	* @brief	Read timer value
	*
	* @return	0 on success, error code on failure
	*/
	uint8_t timer_get();

	/**
	* @brief	Enable timer
	*
	* @return	0 on success, error code on failure
	*/
	int timer_start();

	/**
	* @brief	Pause timer, timer value is preserved
	*
	* @return	0 on success, error code on failure
	*/
	int timer_pause();

	/**
	* @brief	Start timer from the paused value
	*
	* @return	0 on success, error code on failure
	*/
	int timer_continue();

	/**
	* @brief	Disable timer
	*
	* @return	0 on success, error code on failure
	*/
	int timer_stop();

	/**
	* @brief	Put device into data retention mode
	*
	* @return	0 on success, error code on failure
	*/
	int data_retention_mode_enter();

	/**
	* @brief	Remove device from data retention mode
	*
	* @return	0 on success, error code on failure
	*/
	int data_retention_mode_exit();

	/**
	* @brief	Enable I2C bus timeout mechanism
	*
	* @return	0 on success, error code on failure
	*/
	int i2c_timeout_enable();

	/**
	* @brief	Disable I2C bus timeout mechanism
	*
	* @return	0 on success, error code on failure
	*/
	int i2c_timeout_disable();

	/**
	* @brief		Read Temperature
	*
	* @param[out]	temp temperature value
	*
	* @return		0 on success, error code on failure
	*/
	int read_temp(float *temp);

	/**
	* @brief		Configure temperature sensor
	*
	* @param[in]	automode temperature value
	* @param[in]	interval temperature read interval for automode, one of
	* 				TTS_INTERNAL_*
	*
	* @return		0 on success, error code on failure
	*/
	int temp_sensor_config(bool automode, ttsint_t interval = TTS_INTERNAL_1SEC);

	/**
	* @brief		Enable interrupt
	*
	* @param[in]	id Interrupt id, one of INTR_ID_*
	*
	* @return		0 on success, error code on failure
	*/
	int irq_enable(intr_id_t id);

	/**
	* @brief		Disable interrupt
	*
	* @param[in]	id Interrupt id, one of INTR_ID_*
	*
	* @return		0 on success, error code on failure
	*/
	int irq_disable(intr_id_t id);

	/**
	* @brief	Disable all interrupts
	*
	* @return	0 on success, error code on failure
	*/
	int irq_disable_all();

    /**
    * @brief    Get the interrupt source
    *
    * @return   0 on success, error code on failure
    */
    int get_interrupt_source();
    
    /**
    * @brief    Clear the interrupt
    *
    * @return   0 on success, error code on failure
    */
    int clear_interrupt();

	/**
	* @brief	Put device into reset state
	*
	* @return 	0 on success, error code on failure
	*/
	int sw_reset_assert();

	/**
	* @brief	Release device from state state
	*
	* @return	0 on success, error code on failure
	*/
	int sw_reset_release();

	/**
	* @brief	Enable the RTC oscillator
	*
	* @return	0 on success, error code on failure
	*/
	int rtc_start();

	/**
	* @brief	Disable the RTC oscillator
	*
	* @return	0 on success, error code on failure
	*/
	int rtc_stop();

	/**
	* @brief	Base class destructor.
	*/
	~MAX31343();

protected:
	typedef struct {
		union {
			unsigned char raw;
			struct {
				unsigned char seconds : 4;	/**< RTC seconds value. */
				unsigned char sec_10  : 3;	/**< RTC seconds in multiples of 10 */
				unsigned char         : 1;
			} bits;
			struct {
				unsigned char value   : 7;
				unsigned char         : 1;
			} bcd;
		} seconds;

		union {
			unsigned char raw;
			struct {
				unsigned char minutes : 4;	/**< RTC minutes value */
				unsigned char min_10  : 3;	/**< RTC minutes in multiples of 10 */
				unsigned char         : 1;
			} bits;
			struct {
				unsigned char value   : 7;
				unsigned char         : 1;
			} bcd;
		} minutes;

		union {
			unsigned char raw;
			struct {
				unsigned char hour       : 4;	/**< RTC hours value */
				unsigned char hr_10      : 2;	/**< RTC hours in multiples of 10 */
				unsigned char            : 2;
			} bits;
			struct {
				unsigned char value      : 6;
				unsigned char            : 2;
			} bcd;
		} hours;

		union {
			unsigned char raw;
			struct {
				unsigned char day : 3;	/**< RTC days */
				unsigned char     : 5;
			} bits;
			struct {
				unsigned char value : 3;
				unsigned char       : 5;
			} bcd;
		} day;

		union {
			unsigned char raw;
			struct {
				unsigned char date    : 4;	/**< RTC date */
				unsigned char date_10 : 2;	/**< RTC date in multiples of 10 */
				unsigned char         : 2;
			} bits;
			struct {
				unsigned char value   : 6;
				unsigned char         : 2;
			} bcd;
		} date;

		union {
			unsigned char raw;
			struct {
				unsigned char month    : 4;	/**< RTC months */
				unsigned char month_10 : 1;	/**< RTC month in multiples of 10 */
				unsigned char          : 2;
				unsigned char century  : 1;	/**< Century bit */
			} bits;
			struct {
				unsigned char value   : 5;
				unsigned char         : 3;
			} bcd;
		} month;

		union {
			unsigned char raw;
			struct {
				unsigned char year    : 4;	/**< RTC years */
				unsigned char year_10 : 4;	/**< RTC year multiples of 10 */
			} bits;
			struct {
				unsigned char value   : 8;
			} bcd;
		} year;
	} rtc_time_regs_t;

	typedef struct {
		union {
			unsigned char raw;
			struct {
				unsigned char seconds : 4;	/**< Alarm1 seconds */
				unsigned char sec_10  : 3;	/**< Alarm1 seconds in multiples of 10 */
				unsigned char a1m1    : 1;	/**< Alarm1 mask bit for minutes */
			} bits;
			struct {
				unsigned char value   : 7;
				unsigned char         : 1;
			} bcd;
		} sec;

		union {
			unsigned char raw;
			struct {
				unsigned char minutes : 4;	/**< Alarm1 minutes */
				unsigned char min_10  : 3;	/**< Alarm1 minutes in multiples of 10 */
				unsigned char a1m2    : 1;	/**< Alarm1 mask bit for minutes */
			} bits;
			struct {
				unsigned char value   : 7;
				unsigned char         : 1;
			} bcd;
		} min;

		union {
			unsigned char raw;
			struct {
				unsigned char hour       : 4;	/**< Alarm1 hours */
				unsigned char hr_10      : 2;	/**< Alarm1 hours in multiples of 10 */
				unsigned char            : 1;
				unsigned char a1m3       : 1;	/**< Alarm1 mask bit for hours */
			} bits;
			struct {
				unsigned char value      : 6;
				unsigned char            : 2;
			} bcd;
		} hrs;

		union {
			unsigned char raw;
			struct {
				unsigned char day_date : 4;	/**< Alarm1 day/date */
				unsigned char date_10  : 2;	/**< Alarm1 date in multiples of 10 */
				unsigned char dy_dt    : 1;
				unsigned char a1m4     : 1;	/**< Alarm1 mask bit for day/date */
			} bits;
			struct {
				unsigned char value   : 3;
				unsigned char         : 5;
			} bcd_day;
			struct {
				unsigned char value   : 6;
				unsigned char         : 2;
			} bcd_date;
		} day_date;

		union {
			unsigned char raw;
			struct {
				unsigned char month    : 4;	/**< Alarm1 months */
				unsigned char month_10 : 1;	/**< Alarm1 months in multiples of 10 */
				unsigned char          : 1;
				unsigned char a1m6     : 1;	/**< Alarm1 mask bit for year */
				unsigned char a1m5     : 1;	/**< Alarm1 mask bit for month */
			} bits;
			struct {
				unsigned char value   : 5;
				unsigned char         : 3;
			} bcd;
		} mon;

		union {
			unsigned char raw;
			struct {
				unsigned char year    : 4;	/* Alarm1 years */
				unsigned char year_10 : 4;	/* Alarm1 multiples of 10 */
			} bits;
			struct {
				unsigned char value   : 8;
			} bcd;
		} year;
	} alarm_regs_t;

	typedef union {
		unsigned char raw;
		struct {
			unsigned char swrst       : 1;	/**< Active high software reset bit */
			unsigned char             : 7;
		} bits;
	} rtc_reset_reg_t;

    typedef union {
        unsigned char raw;
        struct {
            unsigned char             : 1;	/**< Reserved has to be set to 0. */
            unsigned char enosc       : 1;	/**< Oscillator is on when set to 0. Oscillator is off when set to 1. */
            unsigned char             : 1;
            unsigned char i2c_timeout : 1;	/**< I2C timeout Enable */
            unsigned char data_reten  : 1;	/**< Sets the circuit into data retention mode. */
            unsigned char             : 3;
        } bits;
    } config_reg1_t;

	typedef union {
		unsigned char raw;
		struct {
			unsigned char sqw_hz  : 3;	/**< When INTCN=0, set output clock on SQW to specified frequency */
			unsigned char clko_hz : 4;	/**< Specify uncompensated clock frequency output on pin CLKO. */
			unsigned char enclko  : 1;	/**< CLKO enable */
		} bits;
	} config_reg2_t;

	typedef union {
		unsigned char raw;
		struct {
			unsigned char tfs    : 2;	/**< Timer frequency selection */
			unsigned char trpt   : 1;	/**< Timer repeat mode. It controls the timer interrupt function along with TM. */
			unsigned char tpause : 1;	/**< Timer Pause.*/
			unsigned char te     : 1;	/**< Timer enable */
			unsigned char        : 3;
		} bits;
	} timer_config_t;

	typedef union {
		unsigned char raw;
		struct {
			unsigned char             : 2;
			unsigned char d_man_sel   : 1;	/**< Default low. When this bit is low, input control block decides which supply to use. When this bit is high, user can manually select whether to use V<sub>CC</sub> or VBACKUP as supply. */
			unsigned char d_vback_sel : 1;	/**< Default low. When this bit is low, and D_MANUAL_SEL is high, V<sub>CC</sub> is switched to supply. When this bit is high, and D_MANUAL_SEL is high, V<sub>BACKUP</sub> is switched to supply. */
			unsigned char pfvt        : 2;	/**< Power fail threshold voltage. Sets analog comparator threshold voltage. Require D_MAN_SEL='0' for this setting to have effect. */
			unsigned char             : 2;
		} bits;
	} pwr_mgmt_reg_t;

	typedef union {
		unsigned char raw;
		struct {
			unsigned char sync_delay : 2; /* Sync delay to take for the internal countdown chain to reset after the rising edge of Set_RTC */
			unsigned char            : 6;
		} bits;
	} clock_sync_reg_t;

    typedef union {
        unsigned char raw;
        struct {
            unsigned char             : 3;
            unsigned char ttint       : 3;	/**< Set temp measurement interval to specified time for auto mode */
            unsigned char oneshotmode : 1;	/**< One-shot user requested temp measurement in real-time.
            AUTOMODE must be 0 in one-shot measurement mode. */
            unsigned char automode    : 1;	/**< Automatic mode of temperature measurement.
            This mode is valid only when ONESHOTMODE=0. */
        } bits;
    } ts_config_t;

private:
	/* PRIVATE TYPE DECLARATIONS */

	/* PRIVATE VARIABLE DECLARATIONS */
	TwoWire *i2c_handler;
	uint8_t m_slave_addr;

	enum config_reg2_set_rtc {
		CONFIG_REG2_SET_RTC_RTCRUN = 0,	/**< Setting this bit to zero doesn't allow to write into the RTC */
		CONFIG_REG2_SET_RTC_RTCPRGM,	/**< This bit must be set to one, before writing into the RTC. i.e to set the initial time for the RTC this bit must be high. */
	};

	/* PRIVATE FUNCTION DECLARATIONS */

	int rtc_regs_to_time(struct tm *time, const rtc_time_regs_t *regs);

	int time_to_rtc_regs(rtc_time_regs_t *regs, const struct tm *time);

	int set_alarm_regs(alarm_no_t alarm_no, const alarm_regs_t *regs);

	int get_alarm_regs(alarm_no_t alarm_no, alarm_regs_t *regs);

	int time_to_alarm_regs(alarm_regs_t &regs, const struct tm *alarm_time);

	int alarm_regs_to_time(struct tm *alarm_time, const alarm_regs_t *regs);

	int set_alarm_period(alarm_no_t alarm_no, alarm_regs_t &regs, alarm_period_t period);

	int set_rtc_time();

	int data_retention_mode_config(int state);

	int i2c_timeout_config(int enable);
};

#endif /* _MAX31343_H_ */
