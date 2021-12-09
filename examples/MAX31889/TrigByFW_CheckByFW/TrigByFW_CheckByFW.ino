#include <MaxEssentialToolkit.h>

MAX31889 temp_sensor(&Wire, MAX31889_DEFAULT_I2C_ADDR);
MAX31889::status_t sensor_status;

void print_sensor_info()  {
    int ret;
    char buf[32];
    MAX31889::id_t id;
    
    ret = temp_sensor.get_id(id);
    if (ret) {
        Serial.println("Get ID failed!");
    }
    sprintf(buf, "ROM  ID (HEX):%02X-%02X-%02X-%02X-%02X-%02X", 
                id.rom_id[0], id.rom_id[1], id.rom_id[2],  
                id.rom_id[3], id.rom_id[4], id.rom_id[5]);
    Serial.println(buf);
    
    sprintf(buf, "PART ID (HEX):%02X\n", id.part_id);
    Serial.println(buf);
    Serial.println(" ");
}

void setup()  {

    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31889 use case example:");
    Serial.println("The temperature conversion will be trigger by MAX31889 register");
    Serial.println("Convertaion decision will be done by reading MAX31889 status register");
    Serial.println("------------------------------------------------------------");

    temp_sensor.begin();
    print_sensor_info();

    Serial.println("---------------------");
    Serial.println("Measurement Started");
}

void loop()  { 
    int ret;

    ret = temp_sensor.start_meas();
    if (ret) {
        Serial.println("Restart measurement failed!");
        return;
    }

    delay(500);  // wait a little
    
    ret = temp_sensor.get_status(sensor_status);
    if (ret) {
        Serial.println("Status read failed!");
        return;
    }

    if (sensor_status.temp_ready) {
        float temp = 0;

        ret = temp_sensor.read_samples(&temp, 1);
        if (ret) {
            Serial.println("Temperature read failed!");
        } else {
            Serial.print("Temperature (Celsius): ");
            Serial.println(temp, 4);
        }
    }
}
