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

#ifndef _MAX31341_H_
#define _MAX31341_H_

#include <MAX31341_regs.h>

#include "Arduino.h"
#include <Wire.h>
#include <time.h>


/**
* @brief	Mode of the comparator
*/
typedef enum {
    POW_MGMT_MODE_COMPARATOR,		/**< Comparator */
    POW_MGMT_MODE_POWER_MANAGEMENT,	/**< Power Management / Trickle Charger Mode */
} power_mgmt_mode_t;

/**
* @brief	Analog comparator threshold voltage
*/
typedef enum {
    COMP_THRESH_1V3,	/**< 1.3V */
    COMP_THRESH_1V7,	/**< 1.7V */
    COMP_THRESH_2V0,	/**< 2.0V */
    COMP_THRESH_2V2,	/**< 2.2V */
} comp_thresh_t;

/**
* @brief	Supply voltage select.
*/
typedef enum {
    POW_MGMT_SUPPLY_SEL_AUTO,	/**< Circuit decides whether to use VCC or VBACKUP */
    POW_MGMT_SUPPLY_SEL_VCC,	/**< Use VCC as supply */
    POW_MGMT_SUPPLY_SEL_AIN,	/**< Use AIN as supply */
} power_mgmt_supply_t;

/**
* @brief	Selection of charging path's resistor value
*/
typedef enum {
    TRICKLE_CHARGER_NO_CONNECTION = 0,   /**< No connect */
    TRICKLE_CHARGER_3K_S  = 0x08,   /**< 3Kohm in series with a Schottky diode. */
    TRICKLE_CHARGER_6K_S  = 0x0A,   /**< 6Kohm in series with a Schottky diode. */
    TRICKLE_CHARGER_11K_S = 0x0B,   /**< 11Kohm in series with a Schottky diode. */
    TRICKLE_CHARGER_3K_S_2  = 0x0C,   /**< 3Kohm in series with a diode in series with a Schottky diode. */
    TRICKLE_CHARGER_6K_S_2  = 0x0E,   /**< 6Kohm in series with a diode in series with a Schottky diode. */
    TRICKLE_CHARGER_11K_S_2 = 0x0F,   /**< 11Kohm in series with a diode in series with a Schottky diode. */
} trickle_charger_ohm_t;

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
* @brief	CLKIN frequency selection
*/
typedef enum {
    CLKIN_FREQ_1HZ,		/**< 1Hz*/
    CLKIN_FREQ_50HZ,	/**< 50Hz */
    CLKIN_FREQ_60HZ,	/**< 60Hz */
    CLKIN_FREQ_32768HZ,	/**< 32.768kHz */
} clkin_freq_t;

/**
* @brief	Square wave output frequency selection on CLKOUT pin
*/
typedef enum {
    SQUARE_WAVE_OUT_FREQ_1HZ,		/**< 1Hz */
    SQUARE_WAVE_OUT_FREQ_4096HZ,	/**< 4.098kHz */
    SQUARE_WAVE_OUT_FREQ_8192HZ,	/**< 8.192kHz */
    SQUARE_WAVE_OUT_FREQ_32768HZ,	/**< 32.768kHz */
} square_wave_out_freq_t;

/**
* @brief	Selection of interrupt ids
*/
typedef enum {
    INTR_ID_ALARM1,		/**< Alarm1 flag */
    INTR_ID_ALARM2,		/**< Alarm2 flag */
    INTR_ID_TIMER,		/**< Timer interrupt flag */
    INTR_ID_RESERVED,
    INTR_ID_EXTERNAL,	/**< External interrupt flag for DIN1 */
    INTR_ID_ANALOG,		/**< Analog Interrupt flag / Power fail flag  */
    INTR_ID_OSF,		/**< Oscillator stop flag */
    INTR_ID_LOS,		/**< Loss of signal */
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
} alarm_period_t;

/**
* @brief	Selection of INTA/CLKIN pin function
*/
typedef enum {
    CONFIGURE_PIN_AS_INTA,	/**< Configure pin as interrupt out */
    CONFIGURE_PIN_AS_CLKIN,	/**< Configure pin as external clock in  */
} config_inta_clkin_pin_t;

/**
* @brief	Selection of INTB/CLKOUT pin function
*/
typedef enum {
    CONFIGURE_PIN_AS_CLKOUT,	/**< Output is square wave */
    CONFIGURE_PIN_AS_INTB,		/**< Output is interrupt */
} config_intb_clkout_pin_t;

/**
 * @brief	Selection of sync delay
 */
typedef enum {
    SYNC_DLY_LESS_THAN_1SEC = 0,	/**<  Sync delay less than 1 second, recommended for external 1Hz clock */
    SYNC_DLY_LESS_THAN_100MS,		/**<  Sync delay less than 100 msec, recommended for external 50Hz/60Hz/32KHz clock */
    SYNC_DLY_LESS_THAN_10MS,		/**<  Sync delay less than 10 msec, recommended for internal clock */
} sync_delay_t;


/**
 * @brief	RTC base driver class for Maxim Max31341 RTC series.
 */
class MAX31341
{
public:
	/**
	* @brief	Base class constructor.
	*
	* @param[in]	regmap Pointer to device register mappings.
	* @param[in]	i2c Pointer to I2C bus object for this device.
	* @param[in]	inta_pin MCU's pin number that device's INTA pin connected
	* @param[in]	intb_pin MCU's pin number that device's INTB pin connected
	*/
	MAX31341(TwoWire *i2c, uint8_t i2c_addr);
    
    void begin(void);

    /**
	* @brief		Get Revision ID of sensor
	*
	* @param[out]	version Pointer to save version number.
	*
	* @returns		0 on success, negative error code on failure.
	*/
    int get_version(uint8_t &version);
	
    /**
	* @brief		Read from a register.
	*
	* @param[in]	reg Address of a register to be read.
	* @param[out]	value Pointer to save result value.
	* @param[in]	len Size of result to be read.
	*
	* @returns		0 on success, negative error code on failure.
	*/
	int read_register(uint8_t reg, uint8_t *value, uint8_t len=1);

	/**
	* @brief		Write to a register.
	*
	* @param[in]	reg Address of a register to be written.
	* @param[out]	value Pointer of value to be written to register.
	* @param[in]	len Size of result to be written.
	*
	* @returns		0 on success, negative error code on failure.
	*/
	int write_register(uint8_t reg, const uint8_t *value, uint8_t len=1);

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
	* @brief		Non-volatile memory write
	*
	* @param[out]	buffer Pointer to the data to be written
	* @param[in]	offset Offset of location in NVRAM
	* @param[in]	length Number of bytes to write
	*
	* @return		0 on success, error code on failure
	*/
	int nvram_write(const uint8_t *buffer, int offset, int length);

	/**
	* @brief		Non-volatile memory read
	*
	* @param[in]	buffer Buffer to read in to
	* @param[in]	offset Offset of location in NVRAM
	* @param[in]	length Number of bytes to read
	*
	* @return		0 on success, error code on failure
	*/
	int nvram_read(uint8_t *buffer, int offset, int length);

	/**
	* @brief		NVRAM size of the part
	*
	* @return		0 if part does not have a NVRAM, otherwise returns size
	*/
	int nvram_size();

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
	* @brief		Select power management mode of operation
	*
	* @param[in]	mode Mode selection, one of COMP_MODE_*
	*
	* @return		0 on success, error code on failure
	*/
	int set_power_mgmt_mode(power_mgmt_mode_t mode);

	/**
	* @brief		Set comparator threshold
	*
	* @param[in]	th Set Analog Comparator Threshold level, one of COMP_THRESH_*
	*
	* @return		0 on success, error code on failure
	*/
	int comparator_threshold_level(comp_thresh_t th);

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
	* @param[in]	res Value of resister
	*
	* @return		0 on success, error code on failure
	*/
	int trickle_charger_enable(trickle_charger_ohm_t res);

	/**
	* @brief		Disable trickle charger
	*
	* @return		0 on success, error code on failure
	*/
	int trickle_charger_disable();

	/**
	* @brief		Select square wave output frequency selection
	*
	* @param[in]	freq Clock frequency, one of CLKOUT_FREQ_*
	*
	* @return		0 on success, error code on failure
	*/
	int set_output_square_wave_frequency(square_wave_out_freq_t freq);

	/**
	* @brief		Select external clock input frequency
	*
	* @param[in]	freq Clock frequency, one of CLKIN_FREQ_*
	*
	* @return		0 on success, error code on failure
	*/
	int set_clkin_frequency(clkin_freq_t freq);

	/**
	* @brief		Select direction of INTB/CLKOUT pin
	*
	* @param[in]	sel Pin function, one of CONFIGURE_PIN_B3_AS_INTB or CONFIGURE_PIN_B3_AS_CLKOUT
	*
	* @return		0 on success, error code on failure
	*/
	int configure_intb_clkout_pin(config_intb_clkout_pin_t sel);

	/**
	* @brief		Select direction of INTA/CLKIN pin
	*
	* @param[in]	sel Pin function, one of CONFIGURE_PIN_B3_AS_INTA or CONFIGURE_PIN_B3_AS_CLKIN
	*
	* @return		0 on success, error code on failure
	*/
	int configure_inta_clkin_pin(config_inta_clkin_pin_t sel);

	/**
	* @brief		Initialize timer
	*
	* @param[in]	value Timer initial value
	* @param[in]	repeat Timer repeat mode enable/disable
	* @param[in]	freq Timer frequency, one of TIMER_FREQ_*
	* @param[in]	mode Timer mode, 0 or 1
	*
	* @return		0 on success, error code on failure
	*
	* @note			\p mode controls the countdown timer interrupt function
	* along with \p repeat.
	* 	Pulse interrupt when \p mode = 0, irrespective of \p repeat (true or false)
	* 	Pulse interrupt when \p mode = 1 and \p repeat = true
	* 	Level interrupt when \p mode = 1 and \p repeat = false
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

private:
	TwoWire *m_i2c;
	uint8_t m_slave_addr;

	enum config_reg2_set_rtc {
		CONFIG_REG2_SET_RTC_RTCRUN = 0,	/**< Setting this bit to zero doesn't allow to write into the RTC */
		CONFIG_REG2_SET_RTC_RTCPRGM,	/**< This bit must be set to one, before writing into the RTC. i.e to set the initial time for the RTC this bit must be high. */
	};

	int tm_hour_to_rtc_hr12(int hours, bool *is_am);

	int rtc_hr12_to_tm_hour(int hours, bool is_am);

	int rtc_regs_to_time(struct tm *time, const regs_rtc_time_t *regs);

	int time_to_rtc_regs(regs_rtc_time_t *regs, const struct tm *time);

	int set_alarm_regs(alarm_no_t alarm_no, const regs_alarm_t *regs);

	int get_alarm_regs(alarm_no_t alarm_no, regs_alarm_t *regs);

	int time_to_alarm_regs(regs_alarm_t &regs, const struct tm *alarm_time);

	int alarm_regs_to_time(struct tm *alarm_time, const regs_alarm_t *regs);

	int set_alarm_period(alarm_no_t alarm_no, regs_alarm_t &regs, alarm_period_t period);

	int set_rtc_time();

	int data_retention_mode_config(int state);

	int i2c_timeout_config(int enable);

	int set_clock_sync_delay(sync_delay_t delay);
};

#endif /* _MAX31341_H_ */
