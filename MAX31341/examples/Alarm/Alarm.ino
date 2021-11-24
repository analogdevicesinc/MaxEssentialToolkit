#include <MAX31341.h>

MAX31341 max3134x(&Wire, MAX31341_I2C_ADDRESS);
int inta_pin = 3;

char time_char_buffer[40];
struct tm rtc_ctime;

void alarm1_irq_handler(void) {
  
    max3134x.get_time(&rtc_ctime);

    strftime(time_char_buffer, sizeof(time_char_buffer), "%Y-%m-%d %H:%M:%S", &rtc_ctime);
    Serial.println(time_char_buffer);
}

void clear_irq_flag(void) {
    int ret;
    uint8_t reg;
    
    // reading status register will clear flags
    ret = max3134x.read_register(MAX31341_REG_INT_STATUS, &reg);
     if (ret) {
        Serial.println("Read Status failed!");
    }
}

void setup() {
    int ret;

    Serial.begin(115200);
    Serial.println("---------------------");
    Serial.println("MAX31341 Alarm1 use case example:");
    Serial.println("MAX31341 will be configured to generate periodic alarm");
    Serial.println(" ");

    max3134x.begin();
    
    rtc_ctime.tm_year = 121; // years since 1900
    rtc_ctime.tm_mon  = 10;  // 0-11
    rtc_ctime.tm_mday = 24;  // 1-31
    rtc_ctime.tm_hour = 15;  // 0-23
    rtc_ctime.tm_min  = 10;  // 0-59
    rtc_ctime.tm_sec  = 0;   // 0-61
    //
    rtc_ctime.tm_yday  = 0;  // 0-365
    rtc_ctime.tm_wday  = 0;  // 0-6
    rtc_ctime.tm_isdst = 0;  // Daylight saving flag
    
    ret = max3134x.set_time(&rtc_ctime);
    if (ret) {
        Serial.println("Set time failed!");
    }

    // Set alarm pin as input
    pinMode(inta_pin, INPUT);

    ret = max3134x.configure_inta_clkin_pin(CONFIGURE_PIN_AS_INTA);
    if (ret) {
        Serial.println("Configure inta failed!");
    }

    ret = max3134x.set_alarm(ALARM1, &rtc_ctime, ALARM_PERIOD_EVERYSECOND);
    if (ret) {
        Serial.println("Set alarm failed!");
    }

    ret = max3134x.irq_enable(INTR_ID_ALARM1);
    if (ret) {
        Serial.println("IRQ enable failed!");
    }

    clear_irq_flag();
}

void loop()  {

    //Serial.println("Tick");

    int pin_state = digitalRead(inta_pin);
    
    if (pin_state == LOW) {
        alarm1_irq_handler();
        clear_irq_flag();    
    }
}
