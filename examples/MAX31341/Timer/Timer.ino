#include <MaxEssentialToolkit.h>

MAX31341 rtc(&Wire, MAX31341_I2C_ADDRESS);

void setup() {
    int ret;

    Serial.begin(115200);
    Serial.println("---------------------");
    Serial.println("MAX31341 timer use case example:");
    Serial.println("MAX31341 timer feature will be configured and timer count value will be display");
    Serial.println(" ");

    rtc.begin();
        
    bool repeat = true;
    MAX31341::timer_freq_t freq = MAX31341::TIMER_FREQ_16HZ;
    uint8_t value = 0xFF;

    ret = rtc.timer_init(value, repeat, freq);
    if (ret) {
        Serial.println("Timer init failed!");
    }

    ret = rtc.timer_start();
    if (ret) {
        Serial.println("Timer start failed!");
    }
}

void loop()  {

    delay(10); // wait a little

    uint8_t counter = rtc.timer_get();
    Serial.print("TMR Conter: ");
    Serial.println(counter, HEX);
}
