#include <MAX31889.h>


MAX31889 temp_sensor(&Wire);
// Pin 43 is P5.3 (for MAX32630FTHR Board)
// More detail here: https://os.mbed.com/platforms/MAX32630FTHR/
int pin_alarm = 43; // MAX31889 GPIO0 pin connects to MAX32630FTHR P5.3

void print_sensor_info()  {
    int ret;
    char buf[32];
    max31889_id_t id;
    
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
    int ret;

    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31889 use case example:");
    Serial.println("The temperature conversion will be trigger by MAX31889 register");
    Serial.println("Convertaion decision will be done by checking MAX31889 interrupt pin");
    Serial.println("------------------------------------------------------------");

    temp_sensor.begin();
    print_sensor_info();

    ret = temp_sensor.config_gpio(GPIO_NUM_0, GPIO_MODE_3);// GPIO0 mode INTB
    if (ret) {
        Serial.println("GPIO configuration failed!");
    }

    // Attach interrupt on INT pin of MAX31889.
    pinMode(pin_alarm, INPUT);

    ret = temp_sensor.set_interrupt(INT_TEMP_RDY, true);
    if (ret) {
        Serial.println("Interrupt initialization failed!");
    }

    Serial.println("---------------------");
    Serial.println("Measurement Started");
}

void loop()  { 
    int ret;

    ret = temp_sensor.start_meas();
    if (ret) {
        Serial.println("Start measurement failed!");
        return;
    }

    delay(500); // wait a little

    if ( LOW == digitalRead(pin_alarm) ) {
        float temp = 0;

        ret = temp_sensor.read_samples(&temp, 1);
        if (ret) {
            Serial.println("Temprature read failed!");
        } else {
            Serial.print("Temperature (Celsius): ");
            Serial.println(temp, 4);
        }

        temp_sensor.clear_flags();
    }
}
