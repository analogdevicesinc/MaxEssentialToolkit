#include <MaxEssentialToolkit.h>

// I2C Addr depend on the R_SEL resistor
MAX31827 temp_sensor(&Wire, MAX31827_I2C_ADDR_FOR_640_0_KOHM);
MAX31827::status_t  status;

void setup()  {
    int ret;

    Serial.begin(115200);
    
    temp_sensor.begin();

    Serial.println("---------------------");
    Serial.println("Measurement Started");
}

void loop()  {
    int ret;

    ret = temp_sensor.start_meas(MAX31827::PERIOD_ONE_SHOT);
    if (ret) {
        Serial.println("Start measurement failed!");
        return;
    }

    delay(500); // wait a little

    ret = temp_sensor.get_status(status);
    if (ret) {
        Serial.println("Status read failed!");
        return;
    }
    
    if (status.temp_ready) {
        float temp = 0;

        ret = temp_sensor.get_temp(temp);
        if (ret) {
            Serial.println("Temperature read failed!");
        } else {
            Serial.print("Temperature (Celsius): ");
            Serial.println(temp, 4);
        }
    }
}
