#include <MaxEssentialToolkit.h>

/*
    DS2482 is I2C to OneWire converter, 
    This example use DS2482 to drive MAX31825 temperature sensor
    MAX31825EVKIT default has DS2482 converter.
    Webpage: 
        https://www.maximintegrated.com/en/products/sensors/MAX31825EVKIT.html
        https://www.maximintegrated.com/en/products/sensors/MAX31825EVSYS1.html
*/
MAX31825 temp_sensor(&Wire, DS2482_I2C_ADDR_AD_00);

void setup() {
    int ret;
    byte serial[8];

    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31825 oneshot use case example:");
    Serial.println("The MAX31825 temperature sensor will be driven over DS2482 (I2C to Onewire converter)");
    Serial.println("Addressing mode can be changed by set_addressing_mode function");
    Serial.println("------------------------------------------------------------");
    
    temp_sensor.begin();

    ret = temp_sensor.read_rom(serial);
    if (ret) {
        Serial.println("Read rom failed!");
    } else {
        char buf[64];
        sprintf(buf, "ROM Code:%02X %02X %02X %02X %02X %02X %02X %02X", 
              serial[0], serial[1], serial[2], serial[3],
              serial[4], serial[5], serial[6], serial[7]);
        Serial.println(buf);
    }
    
    //ret = temp_sensor.set_addressing_mode(MAX31825::ADDRESSING_USE_ADD1_ADD0, MAX31825_ADDR_FOR_56_6_KOHM);
    //ret = temp_sensor.set_addressing_mode(MAX31825::ADDRESSING_USE_ROM, 0x00, serial);
    ret = temp_sensor.set_addressing_mode(MAX31825::ADDRESSING_NONE);
    if (ret) {
         Serial.println("Set addressing mode failed!");
    }

    Serial.println("---------------------");
    Serial.println("Measurement Started");
}

void loop()  {
    int ret;
    float temp;
    
    ret = temp_sensor.start_temp_conversion();
    if (ret) {
        Serial.println("start_temp_conversion failed!");
    }
    
    delay(1000); // wait a little

    ret = temp_sensor.get_temp(temp);
    if (ret) {
        Serial.println("get_temp failed!");
    } else {
        Serial.print("Temperature (Celsius): ");
        Serial.println(temp, 4);
    }
}
