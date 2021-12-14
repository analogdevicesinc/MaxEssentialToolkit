#include <MaxEssentialToolkit.h>

// I2C Addr depend on the R_SEL resistor
MAX31827 temp_sensor(&Wire, MAX31827_I2C_ADDR_FOR_640_0_KOHM);
MAX31827::status_t  status;

void setup()  {
    int ret;

    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31827 oneshot mode use case example:");
    Serial.println("Temperature conversion will be triggered  by oneshot mode command");
    Serial.println("Conversion status will be checked by reading status register");
    Serial.println("---------------------------------------------------------------");
    
    temp_sensor.begin();

    MAX31827::reg_cfg_t cfg;
    ret = temp_sensor.get_configuration(cfg);
    if (ret) {
        Serial.println("Get configuration failed!");    
    }

    switch (cfg.bits.resolution) {
        case MAX31827::RESOLUTION_8_BIT :  Serial.println("Current Resolution  8 bits");   break;
        case MAX31827::RESOLUTION_9_BIT :  Serial.println("Current Resolution  9 bits");   break;
        case MAX31827::RESOLUTION_10_BIT:  Serial.println("Current Resolution 10 bits");   break;
        case MAX31827::RESOLUTION_12_BIT:  Serial.println("Current Resolution 12 bits");   break;
    }
    
    if (cfg.bits.resolution != MAX31827::RESOLUTION_12_BIT) { // Update resolution if it is not 12bits
        ret = temp_sensor.set_resolution(MAX31827::RESOLUTION_12_BIT);
        if (ret) {
            Serial.println("Resolution set failed!");    
        } else {
            Serial.println("Resolution is updated to 12 bits");
        }
    }
    
    Serial.println("Measurement Started");
    Serial.println(" ");
}

void loop()  {
    int ret;

    ret = temp_sensor.start_temp_conversion(MAX31827::PERIOD_ONE_SHOT);
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
    } else {
        Serial.println("Conversion not complete yet!");
    }
}
