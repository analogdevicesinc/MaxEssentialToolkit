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

#include <MAX31341/MAX31341.h>
   
#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)
#define BIN2BCD(val) ((((val) / 10) << 4) + (val) % 10)

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
	    uint8_t rev_id;
	} regmap_t;
*/

MAX31341::MAX31341(TwoWire *i2c, uint8_t i2c_addr)
{
	if (i2c == NULL) {
		while (1) {
            ;
        }
	}
	m_i2c = i2c;
	m_slave_addr = i2c_addr;
}

int MAX31341::get_version(uint8_t &version)
{
    int ret;
    reg_rev_id_t id;

	ret = read_register(MAX31341_REG_REV_ID, (uint8_t *)&id);
    version = id.bits.rev_id;

    return ret;
}

void MAX31341::begin(void)
{
    m_i2c->begin();
    
    //
    sw_reset_release();
    //
    rtc_start();
    //
    irq_disable_all();
}

int MAX31341::read_register(uint8_t reg, uint8_t *buf, uint8_t len/*=1*/)
{
    int ret;
    int counter = 0;

    m_i2c->beginTransmission(m_slave_addr);
    m_i2c->write(reg);

    /*
        stop = true, sends a stop message after transmission, releasing the I2C bus.
        
        stop = false, sends a restart message after transmission. 
          The bus will not be released, which prevents another master device from transmitting between messages. 
          This allows one master device to send multiple transmissions while in control.
    */
    ret = m_i2c->endTransmission(false);
    /*
        0:success
        1:data too long to fit in transmit buffer
        2:received NACK on transmit of address
        3:received NACK on transmit of data
        4:other error
    */
    if (ret != 0) {
        m_i2c->begin(); // restart
        return -1;
    }

    // Read    
    m_i2c->requestFrom((char)m_slave_addr, (char)len, false);

    while (m_i2c->available()) { // slave may send less than requested
        buf[counter++] = m_i2c->read(); // receive a byte as character
    }
    
    m_i2c->endTransmission();

    //
    if (counter != len) {
        m_i2c->begin(); // restart
        ret = -1;
    }

    return ret;
}

int MAX31341::write_register(uint8_t reg, const uint8_t *buf, uint8_t len/*=1*/)
{
    int ret;

    m_i2c->beginTransmission(m_slave_addr);
    m_i2c->write(reg);
    m_i2c->write(buf, len);
    ret = m_i2c->endTransmission();
    /*
        0:success
        1:data too long to fit in transmit buffer
        2:received NACK on transmit of address
        3:received NACK on transmit of data
        4:other error
    */

    if (ret != 0) {
        m_i2c->begin(); // restart
    }

    return ret;
}

int MAX31341::rtc_regs_to_time(struct tm *time, const regs_rtc_time_t *regs)
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

int MAX31341::time_to_rtc_regs(regs_rtc_time_t *regs, const struct tm *time)
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
		//Invalid set date!
		return -1;
	}

	return 0;
}

int MAX31341::get_time(struct tm *time)
{
    int ret;
	regs_rtc_time_t time_regs;

	if (time == NULL) {
		return -1;
	}

    ret = read_register(MAX31341_REG_SECONDS, (uint8_t *) &time_regs, sizeof(time_regs));
	if (ret) {
		return -1;
	}

	ret = rtc_regs_to_time(time, &time_regs);

    return ret;
}

int MAX31341::set_rtc_time()
{
    int ret;
	reg_config2_t reg;

	/* Toggle Set_RTC bit to set RTC date */

    ret = read_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);
	if (ret) {
		return -1;
	}

	reg.bits.set_rtc = CONFIG_REG2_SET_RTC_RTCPRGM;
	ret = write_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);
    if (ret) {
		return -1;
	}

	/* SET_RTC bit should be kept high at least 10ms */
    delay(10);
    
	reg.bits.set_rtc = CONFIG_REG2_SET_RTC_RTCRUN;
	ret = write_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);

	return ret;
}

int MAX31341::set_time(const struct tm *time)
{
    int ret;
	regs_rtc_time_t time_regs;

	if (time == NULL) {
		return -1;
	}

	time_to_rtc_regs(&time_regs, time);

    ret = write_register(MAX31341_REG_SECONDS, (const uint8_t *) &time_regs, sizeof(time_regs));
	if (ret < 0) {
		return -1;
	}

	ret = set_rtc_time();
    
    return ret;
}

int MAX31341::nvram_write(const uint8_t *buffer, int offset, int length)
{
    int ret;
	int totlen;

	totlen = (MAX31341_REG_RAM_END - MAX31341_REG_RAM_START) + 1;

	if ((offset + length) > totlen) {
		return -1;
	}

	if (length == 0) {
		return 0;
	}

    ret = write_register(MAX31341_REG_RAM_START + offset, buffer, length);

	return ret;
}

int MAX31341::nvram_read(uint8_t *buffer, int offset, int length)
{
    int ret;
	int totlen;

	totlen = (MAX31341_REG_RAM_END - MAX31341_REG_RAM_START) + 1;

	if ((offset + length) > totlen) {
		return -1;
	}

	if (length == 0) {
		return -1;
	}

    ret = read_register(MAX31341_REG_RAM_START + offset, buffer, length);

	return ret;
}

int MAX31341::nvram_size()
{
	return (MAX31341_REG_RAM_END - MAX31341_REG_RAM_START) + 1;
}

int MAX31341::time_to_alarm_regs(regs_alarm_t &regs, const struct tm *alarm_time)
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
	//regs.mon.bcd.value = BIN2BCD(alarm_time->tm_mon);

	return 0;
}

int MAX31341::alarm_regs_to_time(struct tm *alarm_time, const regs_alarm_t *regs)
{
	alarm_time->tm_sec = BCD2BIN(regs->sec.bcd.value);
	alarm_time->tm_min = BCD2BIN(regs->min.bcd.value);
	alarm_time->tm_hour = BCD2BIN(regs->hrs.bcd.value);

	if (regs->day_date.bits.dy_dt == 0) { /* date */
		alarm_time->tm_mday = BCD2BIN(regs->day_date.bcd_date.value);
	} else { /* day */
		alarm_time->tm_wday = BCD2BIN(regs->day_date.bcd_day.value);
	}

	//alarm_time->tm_mon = BCD2BIN(regs->mon.bcd.value) - 1;
	//alarm_time->tm_year = BCD2BIN(regs->year.bcd.value) + 100;	/* XXX no century bit */

	return 0;
}

int MAX31341::set_alarm_period(alarm_no_t alarm_no, regs_alarm_t &regs, alarm_period_t period)
{
	if ((alarm_no == ALARM2) && (period == ALARM_PERIOD_EVERYSECOND)) {
		return -1; /* Alarm2 does not support "once per second" alarm*/
	}

	regs.sec.bits.axm1 = 1;
	regs.min.bits.axm2 = 1;
	regs.hrs.bits.axm3 = 1;
	regs.day_date.bits.axm4 = 1;
	//regs.mon.bits.axm5 = 1;
	//regs.mon.bits.axm6 = 1;
	regs.day_date.bits.dy_dt = 1;


	switch (period) {
		case ALARM_PERIOD_EVERYSECOND:
			// Do nothing
			break;
		case ALARM_PERIOD_EVERYMINUTE:
			regs.sec.bits.axm1 = 0;
			break;
		case ALARM_PERIOD_HOURLY:
			regs.sec.bits.axm1 = 0;
			regs.min.bits.axm2 = 0;
			break;
		case ALARM_PERIOD_DAILY:
			regs.sec.bits.axm1 = 0;
			regs.min.bits.axm2 = 0;
			regs.hrs.bits.axm3 = 0;
			break;
		case ALARM_PERIOD_WEEKLY:
			regs.sec.bits.axm1 = 0;
			regs.min.bits.axm2 = 0;
			regs.hrs.bits.axm3 = 0;
			regs.day_date.bits.axm4 = 0;
			break;
		case ALARM_PERIOD_MONTHLY:
			regs.sec.bits.axm1 = 0;
			regs.min.bits.axm2 = 0;
			regs.hrs.bits.axm3 = 0;
			regs.day_date.bits.axm4 = 0;
			regs.day_date.bits.dy_dt = 0;
			break;
		//case ALARM_PERIOD_YEARLY:
		//...
		//	break;
		//case ALARM_PERIOD_ONETIME:
		//...
		//	break;
		default:
			return -1;
	}

	return 0;
}

int MAX31341::set_alarm_regs(alarm_no_t alarm_no, const regs_alarm_t *regs)
{
    int ret;
	uint8_t *ptr_regs = (uint8_t *)regs;
	uint8_t len = sizeof(regs_alarm_t);

	if (alarm_no == ALARM1) {
		ret = write_register(MAX31341_REG_ALM1_SEC, &ptr_regs[0], len);
	} else {
		/* Discard sec starts from min register */
		len -= 1;
		ret = write_register(MAX31341_REG_ALM2_MIN, &ptr_regs[1], len);
	}
    
    return ret;
}

int MAX31341::get_alarm_regs(alarm_no_t alarm_no, regs_alarm_t *regs)
{
    int ret;
	uint8_t *ptr_regs = (uint8_t *)regs;
	uint8_t len = sizeof(regs_alarm_t);

	if (alarm_no == ALARM1) {
		ret = read_register(MAX31341_REG_ALM1_SEC, &ptr_regs[0], len);
	} else {
		regs->sec.raw = 0;	/* zeroise second register for alarm2 */
		/* starts from min register (no sec register) */
		len -= 1;	/* discard mon & sec registers */
		ret = read_register(MAX31341_REG_ALM2_MIN, &ptr_regs[1], len);
	}

    return ret;
}

int MAX31341::set_alarm(alarm_no_t alarm_no, const struct tm *alarm_time, alarm_period_t period)
{
	int ret;
	regs_alarm_t regs;

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

	return ret;
}

int MAX31341::get_alarm(alarm_no_t alarm_no, struct tm *alarm_time, alarm_period_t *period, bool *is_enabled)
{
	int ret;
	regs_alarm_t regs;
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

	*period = (alarm_no == ALARM1) ? ALARM_PERIOD_EVERYSECOND : ALARM_PERIOD_EVERYMINUTE;

	if ((alarm_no == ALARM1) && (regs.sec.bits.axm1 == 0)) *period = ALARM_PERIOD_EVERYMINUTE;
	if (regs.min.bits.axm2 == 0) *period = ALARM_PERIOD_HOURLY;
	if (regs.hrs.bits.axm3 == 0) *period = ALARM_PERIOD_DAILY;
	if (regs.day_date.bits.axm4 == 0) *period = ALARM_PERIOD_WEEKLY;
 	if (regs.day_date.bits.dy_dt == 0) *period = ALARM_PERIOD_MONTHLY;
	//if ((alarm_no == ALARM1) && (regs.mon.bits.axm5 == 0)) *period = ALARM_PERIOD_YEARLY;
	//if ((alarm_no == ALARM1) && (regs.mon.bits.axm6 == 0)) *period = ALARM_PERIOD_ONETIME;

	ret = read_register(MAX31341_REG_INT_EN, (uint8_t *)&reg, 1);
	if (ret) {
		return ret;
	}

	if (alarm_no == ALARM1) {
		*is_enabled = (reg & (1 << INTR_ID_ALARM1)) != 0;
	} else {
		*is_enabled = (reg & (1 << INTR_ID_ALARM2)) != 0;
	}

	return ret;
}

int MAX31341::set_power_mgmt_mode(power_mgmt_mode_t mode)
{
	int ret;
	reg_pwr_mgmt_t reg;

	ret = read_register(MAX31341_REG_PWR_MGMT, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.d_mode = mode;
	ret = write_register(MAX31341_REG_PWR_MGMT, (uint8_t *)&reg);

	return ret;
}

int MAX31341::comparator_threshold_level(comp_thresh_t th)
{
	int ret;
	reg_config2_t reg;

	ret = read_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.bref = th;
	ret = write_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);

	return ret;
}

int MAX31341::supply_select(power_mgmt_supply_t supply)
{
	int ret;
	reg_pwr_mgmt_t reg;

	ret = read_register(MAX31341_REG_PWR_MGMT, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	switch (supply) {
	case POW_MGMT_SUPPLY_SEL_VCC:
		reg.bits.d_man_sel = 1;
		reg.bits.d_vback_sel = 0;
		break;
	case POW_MGMT_SUPPLY_SEL_AIN:
		reg.bits.d_man_sel = 1;
		reg.bits.d_vback_sel = 1;
		break;
	case POW_MGMT_SUPPLY_SEL_AUTO:
	default:
		reg.bits.d_man_sel = 0;
		break;
	}

	ret = write_register(MAX31341_REG_PWR_MGMT, (uint8_t *)&reg);

	return ret;
}

int MAX31341::trickle_charger_enable(trickle_charger_ohm_t res)
{
	int ret;
	struct {
		unsigned char d_trickle : 4;
		unsigned char           : 4;
	} reg;

	reg.d_trickle = res;
	ret = write_register(MAX31341_REG_TRICKLE, (uint8_t *)&reg);

	return ret;
}

int MAX31341::trickle_charger_disable()
{
	int ret;
	uint8_t reg;

	reg = 0;
	ret = write_register(MAX31341_REG_TRICKLE, &reg);

	return ret;
}


int MAX31341::set_output_square_wave_frequency(square_wave_out_freq_t freq)
{
	int ret;
	reg_config1_t reg;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.rs = freq;
	ret = write_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg, 1);

	return ret;
}

int MAX31341::set_clock_sync_delay(sync_delay_t delay)
{
	int ret;
	reg_clock_sync_t reg;

	ret = read_register(MAX31341_REG_CLOCK_SYNC, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.sync_delay = delay;
	ret = write_register(MAX31341_REG_CLOCK_SYNC, (uint8_t *)&reg);
    
	return ret;
}

int MAX31341::set_clkin_frequency(clkin_freq_t freq)
{
	int ret;
	reg_config1_t reg;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.clksel = freq;

	ret = write_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	if (freq == CLKIN_FREQ_1HZ) {
		ret = set_clock_sync_delay(SYNC_DLY_LESS_THAN_1SEC);
	} else {
		ret = set_clock_sync_delay(SYNC_DLY_LESS_THAN_100MS);
	}

	return ret;
}

int MAX31341::configure_intb_clkout_pin(config_intb_clkout_pin_t sel)
{
	int ret;
	reg_config1_t reg;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.intcn = (sel == CONFIGURE_PIN_AS_INTB) ? 1 : 0;
	ret = write_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);

	return ret;
}

int MAX31341::configure_inta_clkin_pin(config_inta_clkin_pin_t sel)
{
	int ret;
	reg_config1_t reg;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.eclk = (sel == CONFIGURE_PIN_AS_CLKIN) ? 1 : 0;

	ret = write_register(MAX31341_REG_CONFIG_1, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	if (sel == CONFIGURE_PIN_AS_CLKIN) {
		/* Default synchronization delay for external clock mode */
		ret = set_clock_sync_delay(SYNC_DLY_LESS_THAN_1SEC);
	} else {
		/* Synchronization delay for internal oscillator mode */
		ret = set_clock_sync_delay(SYNC_DLY_LESS_THAN_10MS);
	}

	return ret;
}

int MAX31341::timer_init(uint8_t value, bool repeat, timer_freq_t freq)
{
	int ret;
	reg_timer_config_t reg_cfg;

	ret = read_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg_cfg);
	if (ret) {
		return ret;
	}

	reg_cfg.bits.te = 0; /* timer is reset */
	reg_cfg.bits.tpause = 1; /* timer is paused */
	reg_cfg.bits.trpt = repeat ? 1 : 0;	/* Timer repeat mode */
	reg_cfg.bits.tfs = freq;	/* Timer frequency */

	ret = write_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg_cfg);
	if (ret) {
		return ret;
	}

	ret = write_register(MAX31341_REG_TIMER_INIT, (uint8_t *)&value);

	return ret;
}

uint8_t MAX31341::timer_get()
{
	int ret;
	uint8_t reg;

	ret = read_register(MAX31341_REG_TIMER_COUNT, (uint8_t *)&reg);
	if (ret) {
		return (uint8_t) - 1;
	}

	return reg;
}

int MAX31341::timer_start()
{
	int ret;
	reg_timer_config_t reg;

	ret = read_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.te = 1;
	reg.bits.tpause = 0;

	ret = write_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);

	return ret;
}

int MAX31341::timer_pause()
{
	int ret;
	reg_timer_config_t reg;

	ret = read_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.te = 1;
	reg.bits.tpause = 1;

	ret = write_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);

	return ret;
}

int MAX31341::timer_continue()
{
	int ret;
	reg_timer_config_t reg;

	ret = read_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.te = 1;
	reg.bits.tpause = 0;
	ret = write_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);

	return ret;
}

int MAX31341::timer_stop()
{
	int ret;
	reg_timer_config_t reg;

	ret = read_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.te = 0;
	reg.bits.tpause = 1;

	ret = write_register(MAX31341_REG_TIMER_CONFIG, (uint8_t *)&reg);

	return ret;
}

int MAX31341::data_retention_mode_config(int state)
{
	int ret;
	reg_config1_t cfg1;
	reg_config2_t cfg2;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *)&cfg1);
	if (ret) {
		return ret;
	}

	ret = read_register(MAX31341_REG_CONFIG_2, (uint8_t *)&cfg2);
	if (ret) {
		return ret;
	}

	if (state) {
		cfg1.bits.osconz = 1;
		cfg2.bits.data_reten = 1;
	} else {
		cfg1.bits.osconz = 0;
		cfg2.bits.data_reten = 0;
	}

	ret = write_register(MAX31341_REG_CONFIG_1, (uint8_t *)&cfg1);
	if (ret) {
		return ret;
	}

	ret = write_register(MAX31341_REG_CONFIG_2, (uint8_t *)&cfg2);

	return ret;
}

int MAX31341::data_retention_mode_enter()
{
	return data_retention_mode_config(1);
}

int MAX31341::data_retention_mode_exit()
{
	return data_retention_mode_config(0);
}

int MAX31341::i2c_timeout_config(int enable)
{
	int ret;
	reg_config2_t reg;

	ret = read_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);
	if (ret) {
		return ret;
	}

	reg.bits.i2c_timeout = (enable) ? 1 : 0;

	ret = write_register(MAX31341_REG_CONFIG_2, (uint8_t *)&reg);

	return ret;
}

int MAX31341::i2c_timeout_enable()
{
	return i2c_timeout_config(1);
}

int MAX31341::i2c_timeout_disable()
{
	return i2c_timeout_config(0);
}

int MAX31341::irq_enable(intr_id_t id)
{
	int ret;
	uint8_t reg;

	ret = read_register(MAX31341_REG_INT_EN, &reg);
	if (ret) {
		return ret;
	}

	reg |= (1 << id);

	ret = write_register(MAX31341_REG_INT_EN, &reg);

	return ret;
}

int MAX31341::irq_disable(intr_id_t id)
{
	int ret;
	uint8_t reg;

	ret = read_register(MAX31341_REG_INT_EN, &reg);
	if (ret) {
		return ret;
	}

	reg &= ~(1 << id);
	ret = write_register(MAX31341_REG_INT_EN, &reg);

	return ret;
}

int MAX31341::irq_disable_all()
{
	int ret;
	uint8_t reg = 0;

	ret = write_register(MAX31341_REG_INT_EN, &reg);

	return ret;
}

int MAX31341::sw_reset_assert()
{
	int ret;
	reg_config1_t config_reg1;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *) &config_reg1);
	if (ret) {
		return ret;
	}

	config_reg1.bits.swrstn = 0;	/* Put device in reset state */

	ret = write_register(MAX31341_REG_CONFIG_1, (const uint8_t *) &config_reg1);

	return ret;
}

int MAX31341::sw_reset_release()
{
	int ret;
	reg_config1_t config_reg1;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *) &config_reg1);
	if (ret) {
		return ret;
	}

	config_reg1.bits.swrstn = 1;	/* Remove device from reset state */
	ret = write_register(MAX31341_REG_CONFIG_1, (const uint8_t *) &config_reg1);

	return ret;
}

int MAX31341::rtc_start()
{
	int ret;
	reg_config1_t config_reg1;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *) &config_reg1);
	if (ret) {
		return ret;
	}

	config_reg1.bits.osconz = 0;	/* Enable the oscillator */
	ret = write_register(MAX31341_REG_CONFIG_1, (const uint8_t *) &config_reg1);

	return ret;
}

int MAX31341::rtc_stop()
{
	int ret;
	reg_config1_t config_reg1;

	ret = read_register(MAX31341_REG_CONFIG_1, (uint8_t *) &config_reg1);
	if (ret) {
		return ret;
	}

	config_reg1.bits.osconz = 1;	/* Disable the oscillator */
	ret = write_register(MAX31341_REG_CONFIG_1, (const uint8_t *) &config_reg1);

	return ret;
}
