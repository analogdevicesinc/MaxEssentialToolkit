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

#include <MAX31343/MAX31343.h>
#include <stdarg.h>

#define pr_err(fmt, ...) if(1) printf(fmt " (%s:%d)\r\n", ## __VA_ARGS__, __func__, __LINE__)
#define pr_debug(fmt, ...) if(0) printf(fmt " (%s:%d)\r\n", ## __VA_ARGS__, __func__, __LINE__)

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)
#define BIN2BCD(val) ((((val) / 10) << 4) + (val) % 10)

#define POST_INTR_WORK_SIGNAL_ID			0x1

#define TRICKLE_ENABLE_CODE					0x5

/*
	typedef struct {
		uint8_t config_reg1;
		uint8_t config_reg2;
		uint8_t int_ploarity_config;
		uint8_t timer_config;
		uint8_t int_en_reg;
		uint8_t int_status_reg;
		uint8_t seconds;
		uint8_t minutes;
		uint8_t hours;
		uint8_t day;
		uint8_t date;
		uint8_t month;
		uint8_t year;
		uint8_t alm1_sec;
		uint8_t alm1_min;
		uint8_t alm1_hrs;
		uint8_t alm1day_date;
		uint8_t alm1_mon;
		uint8_t alm1_year;
		uint8_t alm2_min;
		uint8_t alm2_hrs;
		uint8_t alm2day_date;
		uint8_t timer_count;
		uint8_t timer_init;
		uint8_t ram_start;
		uint8_t ram_end;
		uint8_t pwr_mgmt_reg;
		uint8_t trickle_reg;
		uint8_t clock_sync_delay;
		uint8_t temp_msb;
		uint8_t temp_lsb;
		uint8_t ts_config;
	} regmap_t;
*/

MAX31343::MAX31343(TwoWire *i2c, uint8_t i2c_addr)
{
	if (i2c == NULL) {
		pr_err("i2c object is invalid!");
		while (1);
	}
	i2c_handler = i2c;
	m_slave_addr = i2c_addr;
}

void MAX31343::begin(void)
{
	i2c_handler->begin();

	sw_reset_release();

	rtc_start();

	irq_disable_all();
}

int MAX31343::read_register(uint8_t reg, uint8_t *value, uint8_t len)
{
	int counter = 0;

	if (value == NULL) {
		pr_err("value is invalid!");
		return -1;
	}

	i2c_handler->beginTransmission(m_slave_addr);
	i2c_handler->write(reg);
	i2c_handler->endTransmission(false);
	i2c_handler->requestFrom(m_slave_addr, len);

	while (i2c_handler->available()) { // slave may send less than requested
		value[counter++] = i2c_handler->read(); // receive a byte as character
	}

	if(counter == len) {
		return 0;
	} else {
		pr_err("i2c write failed.");
		return -1;
	}
}

int MAX31343::write_register(uint8_t reg, const uint8_t *value, uint8_t len)
{
	int ret;
	uint8_t *buffer;

	if (value == NULL) {
		pr_err("value is invalid!");
		return -1;
	}

	i2c_handler->beginTransmission(m_slave_addr);
	i2c_handler->write(reg);
	i2c_handler->write(value,len);
	ret = i2c_handler->endTransmission();

	if (ret != 0) {
		pr_err("i2c write failed with %d!", ret);
	}

	return ret;
}

int MAX31343::rtc_regs_to_time(struct tm *time, const rtc_time_regs_t *regs)
{
	/* tm_sec seconds [0,61] */
	time->tm_sec = BCD2BIN(regs->seconds.bcd.value);

	/* tm_min minutes [0,59] */
	time->tm_min = BCD2BIN(regs->minutes.bcd.value);

	/* tm_hour hour [0,23] */
	time->tm_hour = BCD2BIN(regs->hours.bcd.value);

	/* tm_wday day of week [0,6] (Sunday = 0) */
	time->tm_wday = BCD2BIN(regs->day.bcd.value) - 1;

	/* tm_mday day of month [1,31] */
	time->tm_mday = BCD2BIN(regs->date.bcd.value);

	/* tm_mon month of year [0,11] */
	time->tm_mon = BCD2BIN(regs->month.bcd.value) - 1;

	/* tm_year years since 2000 */
	if (regs->month.bits.century) {
		time->tm_year = BCD2BIN(regs->year.bcd.value) + 200;
	} else {
		time->tm_year = BCD2BIN(regs->year.bcd.value) + 100;
	}

	/* tm_yday day of year [0,365] */
	time->tm_yday = 0; /* TODO */

	/* tm_isdst daylight savings flag */
	time->tm_isdst = 0; /* TODO */

	return 0;
}

int MAX31343::time_to_rtc_regs(rtc_time_regs_t *regs, const struct tm *time)
{
	/*********************************************************
	 * +----------+------+---------------------------+-------+
	 * | Member   | Type | Meaning                   | Range |
	 * +----------+------+---------------------------+-------+
	 * | tm_sec   | int  | seconds after the minute  | 0-61* |
	 * | tm_min   | int  | minutes after the hour    | 0-59  |
	 * | tm_hour  | int  | hours since midnight      | 0-23  |
	 * | tm_mday  | int  | day of the month          | 1-31  |
	 * | tm_mon   | int  | months since January      | 0-11  |
	 * | tm_year  | int  | years since 1900          |       |
	 * | tm_wday  | int  | days since Sunday         | 0-6   |
	 * | tm_yday  | int  | days since January 1      | 0-365 |
	 * | tm_isdst | int  | Daylight Saving Time flag |       |
	 * +----------+------+---------------------------+-------+
	 * * tm_sec is generally 0-59. The extra range is to accommodate for leap
	 *   seconds in certain systems.
	 *********************************************************/
	regs->seconds.bcd.value = BIN2BCD(time->tm_sec);

	regs->minutes.bcd.value = BIN2BCD(time->tm_min);

	regs->hours.bcd.value= BIN2BCD(time->tm_hour);

	regs->day.bcd.value = BIN2BCD(time->tm_wday + 1);

	regs->date.bcd.value = BIN2BCD(time->tm_mday);

	regs->month.bcd.value = BIN2BCD(time->tm_mon + 1);

	if (time->tm_year >= 200) {
		regs->month.bits.century = 1;
		regs->year.bcd.value = BIN2BCD(time->tm_year - 200);
	} else if (time->tm_year >= 100) {
		regs->month.bits.century = 0;
		regs->year.bcd.value = BIN2BCD(time->tm_year - 100);
	} else {
		pr_err("Invalid set year!");
		return -1;
	}

	return 0;
}

int MAX31343::get_time(struct tm *time)
{
	rtc_time_regs_t time_regs;

	if (time == NULL) {
		pr_err("rtc_ctime is invalid!");
		return -1;
	}

	if (read_register(MAX31343_R_SECONDS_ADDR, (uint8_t *) &time_regs,
			sizeof(time_regs)) < 0) {
		pr_err("read time registers failed!");
		return -1;
	}

	return rtc_regs_to_time(time, &time_regs);
}

int MAX31343::set_time(const struct tm *time)
{
	rtc_time_regs_t time_regs;

	if (time == NULL) {
		pr_err("rtc_ctime is invalid!");
		return -1;
	}

	time_to_rtc_regs(&time_regs, time);

	if (write_register(MAX31343_R_SECONDS_ADDR, (const uint8_t *) &time_regs,
			sizeof(time_regs)) < 0) {
		pr_err("read time registers failed!");
		return -1;
	}

	return 0;
}

int MAX31343::time_to_alarm_regs(alarm_regs_t &regs, const struct tm *alarm_time)
{
	regs.sec.bcd.value = BIN2BCD(alarm_time->tm_sec);
	regs.min.bcd.value = BIN2BCD(alarm_time->tm_min);

	regs.hrs.bcd.value = BIN2BCD(alarm_time->tm_hour);

	if (regs.day_date.bits.dy_dt == 0) {
		/* Date match */
		regs.day_date.bcd_date.value = BIN2BCD(alarm_time->tm_mday);
	} else {
		/* Day match */
		regs.day_date.bcd_day.value = BIN2BCD(alarm_time->tm_wday);
	}
	regs.mon.bcd.value = BIN2BCD(alarm_time->tm_mon+1);
    
    if (alarm_time->tm_year >= 200) {
		regs.year.bcd.value = BIN2BCD(alarm_time->tm_year - 200);
	} else if (alarm_time->tm_year >= 100) {
		regs.year.bcd.value = BIN2BCD(alarm_time->tm_year - 100);
	} else {
		pr_err("Invalid set year!");
		return -1;
	}

	return 0;
}

int MAX31343::alarm_regs_to_time(struct tm *alarm_time, const alarm_regs_t *regs)
{
	alarm_time->tm_sec = BCD2BIN(regs->sec.bcd.value);
	alarm_time->tm_min = BCD2BIN(regs->min.bcd.value);
	alarm_time->tm_hour = BCD2BIN(regs->hrs.bcd.value);

	if (regs->day_date.bits.dy_dt == 0) { /* date */
		alarm_time->tm_mday = BCD2BIN(regs->day_date.bcd_date.value);
	} else { /* day */
		alarm_time->tm_wday = BCD2BIN(regs->day_date.bcd_day.value);
	}

	alarm_time->tm_mon = BCD2BIN(regs->mon.bcd.value) - 1;

	alarm_time->tm_year = BCD2BIN(regs->year.bcd.value) + 100;	/* XXX no century bit */

	return 0;
}

int MAX31343::set_alarm_period(alarm_no_t alarm_no, alarm_regs_t &regs, alarm_period_t period)
{
	switch (period) {
	case ALARM_PERIOD_ONETIME:
		if (alarm_no == ALARM2) { /* not supported! */
			return -1;
		}
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 0;
		regs.hrs.bits.a1m3 = 0;
		regs.day_date.bits.a1m4 = 0;
		regs.mon.bits.a1m5 = 0;
		regs.mon.bits.a1m6 = 0;
		regs.day_date.bits.dy_dt = 0;
		break;
	case ALARM_PERIOD_YEARLY:
		if (alarm_no == ALARM2) { /* not supported! */
			return -1;
		}
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 0;
		regs.hrs.bits.a1m3 = 0;
		regs.day_date.bits.a1m4 = 0;
		regs.mon.bits.a1m5 = 0;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 0;
		break;
	case ALARM_PERIOD_MONTHLY:
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 0;
		regs.hrs.bits.a1m3 = 0;
		regs.day_date.bits.a1m4 = 0;
		regs.mon.bits.a1m5 = 1;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 0;
		break;
	case ALARM_PERIOD_WEEKLY:
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 0;
		regs.hrs.bits.a1m3 = 0;
		regs.day_date.bits.a1m4 = 0;
		regs.mon.bits.a1m5 = 1;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 1;
		break;
	case ALARM_PERIOD_DAILY:
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 0;
		regs.hrs.bits.a1m3 = 0;
		regs.day_date.bits.a1m4 = 1;
		regs.mon.bits.a1m5 = 1;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 0;
		break;
	case ALARM_PERIOD_HOURLY:
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 0;
		regs.hrs.bits.a1m3 = 1;
		regs.day_date.bits.a1m4 = 1;
		regs.mon.bits.a1m5 = 1;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 0;
		break;
	case ALARM_PERIOD_EVERYMINUTE:
		regs.sec.bits.a1m1 = 0;
		regs.min.bits.a1m2 = 1;
		regs.hrs.bits.a1m3 = 1;
		regs.day_date.bits.a1m4 = 1;
		regs.mon.bits.a1m5 = 1;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 0;
		break;
	case ALARM_PERIOD_EVERYSECOND:
		if ((alarm_no == ALARM2) && (period == ALARM_PERIOD_EVERYSECOND)) {
			return -1; /* Alarm2 does not support "once per second" alarm*/
		}
		regs.sec.bits.a1m1 = 1;
		regs.min.bits.a1m2 = 1;
		regs.hrs.bits.a1m3 = 1;
		regs.day_date.bits.a1m4 = 1;
		regs.mon.bits.a1m5 = 1;
		regs.mon.bits.a1m6 = 1;
		regs.day_date.bits.dy_dt = 0;
		break;
	default:
		return -1;
	}

	return 0;
}

int MAX31343::set_alarm_regs(alarm_no_t alarm_no, const alarm_regs_t *regs)
{
	uint8_t *ptr_regs = (uint8_t *)regs;
	uint8_t off = 0;
	uint8_t dev_ba;
	uint8_t len = sizeof(alarm_regs_t);

	if (alarm_no == ALARM1) {
		dev_ba = MAX31343_R_ALM1_SEC_ADDR;
	} else {
		dev_ba = MAX31343_R_ALM2_MIN_ADDR;
		off = 1;	/* starts from min register */
		len -= 3;	/* XXX discard min, mon & sec registers */
	}

	return write_register(dev_ba, &ptr_regs[off], len);
}

int MAX31343::get_alarm_regs(alarm_no_t alarm_no, alarm_regs_t *regs)
{
	uint8_t *ptr_regs = (uint8_t *)regs;
	uint8_t off = 0;
	uint8_t dev_ba;
	uint8_t len = sizeof(alarm_regs_t);

	if (alarm_no == ALARM1) {
		dev_ba = MAX31343_R_ALM1_SEC_ADDR;
	} else {
		regs->sec.raw = 0;	/* zeroise second register for alarm2 */
		dev_ba = MAX31343_R_ALM2_MIN_ADDR;
		off = 1;	/* starts from min register (no sec register) */
		len -= 2;	/* XXX discard mon & sec registers */
	}

	return read_register(dev_ba, &ptr_regs[off], len);
}

int MAX31343::set_alarm(alarm_no_t alarm_no, const struct tm *alarm_time, alarm_period_t period)
{
	int ret;
	alarm_regs_t regs;

	ret = set_alarm_period(alarm_no, regs, period);
	if (ret) {
		return ret;
	}

	/* Convert time structure to alarm registers */
	ret = time_to_alarm_regs(regs, alarm_time);
	if (ret) {
		return ret;
	}

	ret = set_alarm_regs(alarm_no, &regs);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::get_alarm(alarm_no_t alarm_no, struct tm *alarm_time, alarm_period_t *period, bool *is_enabled)
{
	int ret;
	alarm_regs_t regs;
	uint8_t reg;

	ret = get_alarm_regs(alarm_no, &regs);
	if (ret) {
		return ret;
	}

	/* Convert alarm registers to time structure */
	ret = alarm_regs_to_time(alarm_time, &regs);
	if (ret) {
		return ret;
	}

    if(alarm_no == ALARM1) {
        int alarm = regs.sec.bits.a1m1 | (regs.min.bits.a1m2<<1)
                    | (regs.hrs.bits.a1m3<<2) | (regs.day_date.bits.a1m4<<3)
                    | (regs.mon.bits.a1m5<<4) | (regs.mon.bits.a1m6<<5)
                    | (regs.day_date.bits.dy_dt<<6);

        switch(alarm) {
            case 0b1111111:
            case 0b0111111:
                *period = ALARM_PERIOD_EVERYSECOND;
                break;
            case 0b1111110:
            case 0b0111110:
                *period = ALARM_PERIOD_EVERYMINUTE;
                break;
            case 0b1111100:
            case 0b0111100:
                *period = ALARM_PERIOD_HOURLY;
                break;
            case 0b0111000:
            case 0b1111000:
                *period = ALARM_PERIOD_DAILY;
                break;
            case 0b0110000:
                *period = ALARM_PERIOD_MONTHLY;
                break;
            case 0b0100000:
                *period = ALARM_PERIOD_YEARLY;
                break;
            case 0b0000000:
                *period = ALARM_PERIOD_ONETIME;
                break;
            case 0b1110000:
                *period = ALARM_PERIOD_WEEKLY;
        }
    } else {
        int alarm = (regs.min.bits.a1m2) | (regs.hrs.bits.a1m3<<1)
                    | (regs.day_date.bits.a1m4<<2)
                    | (regs.day_date.bits.dy_dt<<3);

        switch(alarm) {
            case 0b1111:
            case 0b0111:
                *period = ALARM_PERIOD_EVERYMINUTE;
                break;
            case 0b1110:
            case 0b0110:
                *period = ALARM_PERIOD_HOURLY;
                break;
            case 0b1100:
            case 0b0100:
                *period = ALARM_PERIOD_DAILY;
                break;
            case 0b0000:
                *period = ALARM_PERIOD_MONTHLY;
                break;
            case 0b1000:
                *period = ALARM_PERIOD_WEEKLY;
        }
    }

	ret = read_register(MAX31343_R_INT_EN_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	if (alarm_no == ALARM1) {
		*is_enabled = (reg & (1 << INTR_ID_ALARM1)) != 0;
	} else {
		*is_enabled = (reg & (1 << INTR_ID_ALARM2)) != 0;
	}

	return 0;
}

int MAX31343::powerfail_threshold_level(comp_thresh_t th)
{
	int ret;
	pwr_mgmt_reg_t reg;

	ret = read_register(MAX31343_R_PWR_MGMT_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.bits.pfvt = th;

	ret = write_register(MAX31343_R_PWR_MGMT_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::supply_select(power_mgmt_supply_t supply)
{
	int ret;
	pwr_mgmt_reg_t reg;

	ret = read_register(MAX31343_R_PWR_MGMT_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	switch (supply) {
	case POW_MGMT_SUPPLY_SEL_VCC:
		reg.bits.d_man_sel = 1;
		reg.bits.d_vback_sel = 0;
		break;
	case POW_MGMT_SUPPLY_SEL_VBACK:
		reg.bits.d_man_sel = 1;
		reg.bits.d_vback_sel = 1;
		break;
	case POW_MGMT_SUPPLY_SEL_AUTO:
	default:
		reg.bits.d_man_sel = 0;
		break;
	}

	ret = write_register(MAX31343_R_PWR_MGMT_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::trickle_charger_enable(trickle_charger_path_t path)
{
    int ret;
    struct {
        unsigned char path     : 4;
        unsigned char tche     : 4;
    } reg;

	ret = read_register(MAX31343_R_TRICKLE_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

    reg.tche = TRICKLE_ENABLE_CODE;
    reg.path = path;

	ret = write_register(MAX31343_R_TRICKLE_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::trickle_charger_disable()
{
    int ret;
    struct {
        unsigned char path     : 4;
        unsigned char tche     : 4;
    } reg;


	ret = read_register(MAX31343_R_TRICKLE_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.tche = 0;

	ret = write_register(MAX31343_R_TRICKLE_REG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::set_output_square_wave_frequency(square_wave_out_freq_t freq)
{
	int ret;
	config_reg2_t reg;

	ret = read_register(MAX31343_R_CONFIG_REG2_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.bits.sqw_hz = freq;

	ret = write_register(MAX31343_R_CONFIG_REG2_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::clko_enable(clko_freq_t freq)
{
	int ret;
	config_reg2_t cfg2;

	ret = read_register(MAX31343_R_CONFIG_REG2_ADDR, (uint8_t *)&cfg2, 1);
	if (ret) {
		return ret;
	}

	cfg2.bits.enclko = 1;
	cfg2.bits.clko_hz = freq;

	ret = write_register(MAX31343_R_CONFIG_REG2_ADDR, (uint8_t *)&cfg2, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::clko_disable()
{
	int ret;
	config_reg2_t cfg2;

	ret = read_register(MAX31343_R_CONFIG_REG2_ADDR, (uint8_t *)&cfg2, 1);
	if (ret) {
		return ret;
	}

	cfg2.bits.enclko = 0;

	ret = write_register(MAX31343_R_CONFIG_REG2_ADDR, (uint8_t *)&cfg2, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::timer_init(uint8_t value, bool repeat, timer_freq_t freq)
{
	int ret;
	timer_config_t reg_cfg;

	ret = read_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg_cfg, 1);
	if (ret) {
		return ret;
	}

	reg_cfg.bits.te = 0; /* timer is reset */
	reg_cfg.bits.tpause = 1; /* timer is paused */
	reg_cfg.bits.trpt = repeat ? 1 : 0;	/* Timer repeat mode */
	reg_cfg.bits.tfs = freq;	/* Timer frequency */

	ret = write_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg_cfg, 1);
	if (ret) {
		return ret;
	}

	ret = write_register(MAX31343_R_TIMER_INIT_ADDR, (uint8_t *)&value, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

uint8_t MAX31343::timer_get()
{
	int ret;
	uint8_t reg;

	ret = read_register(MAX31343_R_TIMER_COUNT_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return (uint8_t) - 1;
	}

	return reg;
}

int MAX31343::timer_start()
{
	int ret;
	timer_config_t reg;

	ret = read_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.bits.te = 1;
	reg.bits.tpause = 0;

	ret = write_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::timer_pause()
{
	int ret;
	timer_config_t reg;

	ret = read_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.bits.te = 1;
	reg.bits.tpause = 1;

	ret = write_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::timer_continue()
{
	int ret;
	timer_config_t reg;

	ret = read_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.bits.te = 1;
	reg.bits.tpause = 0;

	ret = write_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::timer_stop()
{
	int ret;
	timer_config_t reg;

	ret = read_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	reg.bits.te = 0;
	reg.bits.tpause = 1;

	ret = write_register(MAX31343_R_TIMER_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::data_retention_mode_config(int state)
{
	int ret;
	config_reg1_t cfg1;

	ret = read_register(MAX31343_R_CONFIG_REG1_ADDR, (uint8_t *)&cfg1, 1);
	if (ret) {
		return ret;
	}

	if (state) {
		cfg1.bits.enosc = 0;
		cfg1.bits.data_reten = 1;
	} else {
		cfg1.bits.enosc = 1;
		cfg1.bits.data_reten = 0;
	}

	ret = write_register(MAX31343_R_CONFIG_REG1_ADDR, (uint8_t *)&cfg1, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::data_retention_mode_enter()
{
	return data_retention_mode_config(1);
}

int MAX31343::data_retention_mode_exit()
{
	return data_retention_mode_config(0);
}

int MAX31343::i2c_timeout_config(int enable)
{
	int ret;
	config_reg1_t cfg1;

	ret = read_register(MAX31343_R_CONFIG_REG1_ADDR, (uint8_t *)&cfg1, 1);
	if (ret) {
		return ret;
	}

	cfg1.bits.i2c_timeout = (enable) ? 1 : 0;

	ret = write_register(MAX31343_R_CONFIG_REG1_ADDR, (uint8_t *)&cfg1, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::i2c_timeout_enable()
{
	return i2c_timeout_config(1);
}

int MAX31343::i2c_timeout_disable()
{
	return i2c_timeout_config(0);
}

int MAX31343::read_temp(float *temp)
{
	int ret;
	uint8_t regs[2];
	ts_config_t cfg_reg;
	int itemp;

	ret = read_register(MAX31343_R_TS_CONFIG_ADDR, (uint8_t *)&cfg_reg, 1);
	if (ret) {
		return ret;
	}

	if (cfg_reg.bits.automode) { /* manual mode */
		/* trigger temperature reading */
		cfg_reg.bits.oneshotmode = 1;

		ret = write_register(MAX31343_R_TS_CONFIG_ADDR, (uint8_t *)&cfg_reg, 1);
		if (ret) {
			return ret;
		}

		while (cfg_reg.bits.oneshotmode) {
			ret = read_register(MAX31343_R_TS_CONFIG_ADDR, (uint8_t *)&cfg_reg, 1);
			if (ret) {
				return ret;
			}
		}
	}

	ret = read_register(MAX31343_R_TEMP_MSB_ADDR, regs, 2);
	if (ret) {
		return ret;
	}

	itemp = ((regs[0] << 2) | (regs[1] >> 6));
	if (regs[0] & 0x80) { /* if negative */
		itemp |= -(1 << 10); /* sign extend */
	}

	*temp = (float)itemp / 4; /* LSB resolution: 0.25 C */

	return 0;
}

int MAX31343::temp_sensor_config(bool automode, ttsint_t interval)
{
	int ret;
	ts_config_t reg;

	ret = read_register(MAX31343_R_TS_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	if (automode) {
		reg.bits.automode = 1;
		reg.bits.ttint = interval;
	} else {
		reg.bits.automode = 0;
	}

	ret = write_register(MAX31343_R_TS_CONFIG_ADDR, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::irq_enable(intr_id_t id)
{
	int ret;
	uint8_t reg;

	ret = read_register(MAX31343_R_INT_EN_REG_ADDR, &reg, 1);
	if (ret) {
		return ret;
	}

	reg |= (1 << id);

	ret = write_register(MAX31343_R_INT_EN_REG_ADDR, &reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::irq_disable(intr_id_t id)
{
	int ret;
	uint8_t reg;

	ret = read_register(MAX31343_R_INT_EN_REG_ADDR, &reg, 1);
	if (ret) {
		return ret;
	}

	reg &= ~(1 << id);

	ret = write_register(MAX31343_R_INT_EN_REG_ADDR, &reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::irq_disable_all()
{
	int ret;
	uint8_t reg = 0;

	ret = write_register(MAX31343_R_INT_EN_REG_ADDR, &reg, 1);
	if (ret) {
		return ret;
	}

	return 0;
}

int MAX31343::clear_interrupt()
{
  return get_interrupt_source();
}

int MAX31343::get_interrupt_source()
{
  int ret;
  uint8_t reg;
  ret = read_register(MAX31343_R_INT_STATUS_REG_ADDR, &reg, 1);
  if (ret) {
    pr_err("Read interrupt status failed!");
  }


  uint8_t counter = 0;
  while(counter<8){
      if(reg & 0x01){
         return counter;
      }else{
        reg = reg >> 1;
      }
      counter++;
  }

  return 0;
}

int MAX31343::sw_reset_assert()
{
	int ret;
	rtc_reset_reg_t rst_reg;

	ret = read_register(MAX31343_R_RTC_RESET_ADDR, (uint8_t *) &rst_reg, 1);
	if (ret < 0) {
		pr_err("read_register failed!");
		return ret;
	}

	rst_reg.bits.swrst = 1;	/* Put device in reset state */

	ret = write_register(MAX31343_R_RTC_RESET_ADDR, (const uint8_t *) &rst_reg, 1);
	if (ret < 0) {
		pr_err("read_register failed!");
		return ret;
	}

	return 0;
}

int MAX31343::sw_reset_release()
{
	int ret;
	rtc_reset_reg_t rst_reg;

	ret = read_register(MAX31343_R_RTC_RESET_ADDR, (uint8_t *) &rst_reg, 1);
	if (ret < 0) {
		pr_err("read_register failed!");
		return ret;
	}

	rst_reg.bits.swrst = 0;	/* Remove device from reset state */

	ret = write_register(MAX31343_R_RTC_RESET_ADDR, (const uint8_t *) &rst_reg, 1);
	if (ret < 0) {
		pr_err("read_register failed!");
		return ret;
	}

	return 0;
}

int MAX31343::rtc_start()
{
	int ret;
	config_reg1_t cfg1;

	ret = read_register(MAX31343_R_CONFIG_REG1_ADDR, (uint8_t *) &cfg1, 1);
	if (ret < 0) {
		return ret;
	}

	cfg1.bits.enosc = 1;	/* Enable the oscillator */

	ret = write_register(MAX31343_R_CONFIG_REG1_ADDR, (const uint8_t *) &cfg1, 1);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int MAX31343::rtc_stop()
{
	int ret;
	config_reg1_t cfg1;

	ret = read_register(MAX31343_R_CONFIG_REG1_ADDR, (uint8_t *) &cfg1, 1);
	if (ret < 0) {
		return ret;
	}

	cfg1.bits.enosc = 0;	/* Disable the oscillator */

	ret = write_register(MAX31343_R_CONFIG_REG1_ADDR, (const uint8_t *) &cfg1, 1);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

