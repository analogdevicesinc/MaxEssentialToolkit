#include <MAX31341.h>

MAX31341 max3134x(&Wire, MAX31341_I2C_ADDRESS);

void setup() {
    int ret;

    Serial.begin(115200);
    Serial.println("---------------------");
    Serial.println("MAX31341 timer use case example:");
    Serial.println("MAX31341 timer feature will be configured and timer count value will be display");
    Serial.println(" ");

    max3134x.begin();
        
    bool repeat = true;
    timer_freq_t freq = TIMER_FREQ_16HZ;
    uint8_t value = 0xFF;

    ret = max3134x.timer_init(value, repeat, freq);
    if (ret) {
        Serial.println("Timer init failed!");
    }

    ret = max3134x.timer_start();
    if (ret) {
        Serial.println("Timer start failed!");
    }
}

void loop()  {

    delay(10); // wait a little

    uint8_t counter = max3134x.timer_get();
    Serial.print("TMR Conter: ");
    Serial.println(counter, HEX);
}
