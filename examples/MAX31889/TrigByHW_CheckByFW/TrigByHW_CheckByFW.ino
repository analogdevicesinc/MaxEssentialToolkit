#include <MaxEssentialToolkit.h>


MAX31889 temp_sensor(&Wire, MAX31889_DEFAULT_I2C_ADDR);
MAX31889::status_t sensor_status;
// Pin 46 is P5.6 (for MAX32630FTHR Board)
// More detail here: https://os.mbed.com/platforms/MAX32630FTHR/
int pin_trigger = 46; // Connects to GPIO1 pin of MAX31889

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

void trigger_meas(void)  {
    digitalWrite(pin_trigger, LOW);
    delay(1); // 1ms
    digitalWrite(pin_trigger, HIGH);
}

void setup()  {
    int ret;

    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31889 Usecase example:");
    Serial.println("The temperature conversion will be trigger by GPIO control");
    Serial.println("Convertaion decision will be done by reading MAX31889 status register");
    Serial.println("------------------------------------------------------------");
    
    temp_sensor.begin();
    print_sensor_info();

    // Configure GPIO1 as CONVERT Trigger
    ret = temp_sensor.config_gpio(MAX31889::GPIO_NUM_1, MAX31889::GPIO_MODE_3);
    if (ret) {
        Serial.println("GPIO1 configuration failed!");
    }
    //
    pinMode(pin_trigger, OUTPUT);
    digitalWrite(pin_trigger, HIGH);
    
    Serial.println("---------------------");
    Serial.println("Measurement Started");
}

void loop() {
    int ret;

    trigger_meas();

    delay(500); // wait a little
    
    ret = temp_sensor.get_status(sensor_status);
    if (ret) {
        Serial.println("Status read failed!");
        return;
    }

    if (sensor_status.temp_ready) {
        float temp = 0;
        
        ret = temp_sensor.read_samples(&temp, 1);
        if (ret) {
            Serial.println("Temprature read failed!");
        } else {
            Serial.print("Temperature (Celsius): ");
            Serial.println(temp, 4);
        }
    }
}
