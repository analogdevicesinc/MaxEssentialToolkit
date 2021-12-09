/*
 * This is an example for MAX31343 RTC
 * 
 * Designed to show time-of-day alarm feature of MAX31343 with interrupt.
 * MAX31343 has 2 time-of-day alarms. 
 * Alarm1 event occurs when date, hour, minute and seconds match. 
 * Alarm2 event occurs when date, hour, minute match. 
 * 
 */

#include <MaxEssentialToolkit.h>

MAX31343 rtc = MAX31343(&Wire, MAX31343_I2C_ADDRESS);
uint8_t pin_interrupt = 2;// interrupt pins that connects to MAX31343

char str_time[40];
struct tm _rtc_ctime;
struct tm current_time;
struct tm alarm1;
struct tm alarm2;

/*Variables to handle RTC interrupts*/
bool MAX31343_interrupt_flag = false;
bool MAX31343_interrupt_handled = false;
unsigned long MAX31343_interrupt_time;


void MAX31343_interrupt_handler() {

    MAX31343_interrupt_flag = true;
    MAX31343_interrupt_handled = false;
    MAX31343_interrupt_time = millis();
}

void setup() {

    Serial.begin(115200);
    pinMode(pin_interrupt, INPUT_PULLUP);

    rtc.begin();

    current_time.tm_sec = 50;
    current_time.tm_min = 2;
    current_time.tm_hour = 15;
    current_time.tm_mday = 19;
    current_time.tm_wday = 4;
    current_time.tm_mon = 2;
    current_time.tm_year = 120;
    current_time.tm_yday = 78;

    alarm1.tm_sec = 55;
    alarm1.tm_min = 2;
    alarm1.tm_hour = 15;
    alarm1.tm_mday = 19;
    alarm1.tm_wday = 4;
    alarm1.tm_mon = 2;
    alarm1.tm_year = 120;
    alarm1.tm_yday = 78;

    alarm2.tm_sec = 9;
    alarm2.tm_min = 8;
    alarm2.tm_hour = 15;
    alarm2.tm_mday = 19;
    alarm2.tm_wday = 4;
    alarm2.tm_mon = 2;
    alarm2.tm_year = 120;
    alarm2.tm_yday = 78;

    rtc.set_time(&current_time);
    rtc.irq_disable_all();
    rtc.irq_enable(MAX31343::INTR_ID_ALARM1);
    rtc.irq_enable(MAX31343::INTR_ID_ALARM2);
    rtc.set_alarm(MAX31343::ALARM1, &alarm1, MAX31343::ALARM_PERIOD_EVERYMINUTE);
    rtc.set_alarm(MAX31343::ALARM2, &alarm2, MAX31343::ALARM_PERIOD_HOURLY);
    rtc.clear_interrupt();

    attachInterrupt(digitalPinToInterrupt(pin_interrupt), MAX31343_interrupt_handler, FALLING);

    Serial.println("RTC setup is done.");
}

void loop() {

    if (MAX31343_interrupt_handled && millis() > (MAX31343_interrupt_time+1100)) {
        Serial.println("rtc interrupt cleared!");
        MAX31343_interrupt_handled = false;
        rtc.clear_interrupt();

        rtc.get_time(&_rtc_ctime);
        strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S %A", &_rtc_ctime);
        Serial.println(str_time);
    }
  
    if (MAX31343_interrupt_flag) {
        int intr_source = rtc.get_interrupt_source();

        if (intr_source == MAX31343::INTR_ID_ALARM1) {
            Serial.println("Alarm1 interrupt handled!");
        } else if (intr_source == MAX31343::INTR_ID_ALARM2) {
            Serial.println("Alarm2 interrupt handled!");
        } else {
            Serial.println("Unhandled alarm!");
        }
            
        MAX31343_interrupt_handled = true;
        MAX31343_interrupt_flag = false;

        rtc.get_time(&_rtc_ctime);
        strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S %A", &_rtc_ctime);
        Serial.println(str_time);
    }  
}
