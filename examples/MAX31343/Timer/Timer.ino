/*
 * This is an example for MAX31343 RTC
 *
 * Designed to show timer feature of MAX31343 with interrupt.
 *
 */

#include <MaxEssential.h>
#include <Wire.h>
#include <time.h>


MAX31343 *rtc;
char _time_char_buffer[40];
struct tm _rtc_ctime;
struct tm current_time;

bool RTC_interrupt_flag = false;
bool RTC_interrupt_handled = false;
unsigned long RTC_interrupt_time;
unsigned long interrupt_clear_offset_ms = 1000;

#define RTC_INT_PIN 2

void RTC_interrupt_handler()
{
  RTC_interrupt_flag = true;
  RTC_interrupt_handled = false;
  RTC_interrupt_time = millis();
}

void setup() {

  pinMode(RTC_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RTC_INT_PIN), RTC_interrupt_handler, FALLING);

  Serial.begin(115200);

  rtc = new MAX31343(&Wire, MAX31343_I2C_ADDRESS);

  current_time.tm_sec = 50;
  current_time.tm_min = 58;
  current_time.tm_hour = 14;
  current_time.tm_mday = 19;
  current_time.tm_wday = 4;
  current_time.tm_mon = 2;
  current_time.tm_year = 120;
  current_time.tm_yday = 78;

  rtc->set_time(&current_time);
  rtc->irq_disable_all();
  rtc->clear_interrupt();

  rtc->timer_stop();
  rtc->timer_init( 64,true, MAX31343::TIMER_FREQ_16HZ);

  //wait time until interrupt event cleared
  interrupt_clear_offset_ms = 1+pow(4 ,MAX31343::TIMER_FREQ_16HZ );
  rtc->irq_enable(MAX31343::INTR_ID_TIMER);
  rtc->timer_start();
  Serial.println("RTC setup is done.");
}

void loop() {

  if(RTC_interrupt_handled && millis()>(RTC_interrupt_time+interrupt_clear_offset_ms)){
    Serial.println("rtc interrupt cleared!");
    RTC_interrupt_handled = false;
    rtc->clear_interrupt();

    rtc->get_time(&_rtc_ctime);
    strftime(_time_char_buffer, 40, "%Y-%m-%d %H:%M:%S %A", &_rtc_ctime);
    Serial.println(_time_char_buffer);
  }

  if (RTC_interrupt_flag){
    int intr_source = rtc->get_interrupt_source();

    if(intr_source == MAX31343::INTR_ID_TIMER){
      Serial.println("Timer interrupt handled!");
    }else{
      Serial.println("Unhandled alarm!");
    }

    RTC_interrupt_handled = true;
    RTC_interrupt_flag = false;

    rtc->get_time(&_rtc_ctime);
    strftime(_time_char_buffer, 40, "%Y-%m-%d %H:%M:%S %A", &_rtc_ctime);
    Serial.println(_time_char_buffer);
  }
}
