#include <MaxEssential.h>

MAX40080 sensor(&Wire, MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM, 0.05);

void setup() {
    int ret;

    Serial.begin(115200);
    Serial.println("---------------------");
    Serial.println("MAX40080 Alarm use case example:");
    Serial.println(" ");

    sensor.begin();
}

void loop()  {
    int ret;
    delay(1000);

    MAX40080::reg_status_t stat;

    ret = sensor.get_status(stat);
    if (ret) {
        Serial.println("Read status failed!");
    } else {
        Serial.print("Read status SUCCESS: ");
        Serial.println(stat.raw, HEX);
    }

    float current = 0.9;

    ret = sensor.set_threshold_over_current(current);
    if (ret) {
        Serial.println("set_threshold_over_current failed!");
    }

    current = 0;
    ret = sensor.get_threshold_over_current(current);
    if (ret) {
        Serial.println("get_threshold_over_current failed!");
    } else {
        Serial.print("Threshold over current: ");
        Serial.println(current, 2);
    }    
}
