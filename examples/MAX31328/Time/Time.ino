#include <max31328.h>


Max31328 rtc(&Wire);

void setup() {
  Serial.begin(115200);
}

void print_current_date_time(){
  max31328_time_t time;
  max31328_calendar_t date;

  // Get Time and date from MAX31328
  if(rtc.get_time(&time) || rtc.get_calendar(&date)){
    Serial.println("ERROR: Cannot get time.");
    return;
  }

  Serial.print("RTC Time: ");

  // Print date
  Serial.print(date.month);
  Serial.print("/");
  Serial.print(date.day);
  Serial.print("/");
  Serial.print(date.year + 2000);
  Serial.print(" - ");
  // Print time
  Serial.print(time.hours);
  Serial.print(":");
  Serial.print(time.minutes);
  Serial.print(":");
  Serial.println(time.seconds);
}

void loop() {
  long current_millis = millis();
  static long previous_millis = current_millis-5000;

  // Print date/time once every five seconds
  if( current_millis - previous_millis > 5000){
    print_current_date_time();
    previous_millis = current_millis;
  }
}
