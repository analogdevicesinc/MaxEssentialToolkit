#include <MaxEssentialToolkit.h>


MAX31889 temp_sensor(&Wire, MAX31889_DEFAULT_I2C_ADDR);
// Pin 43 is P5.3 
// Pin 46 is P5.6 
// (MAX32630FTHR Board): https://os.mbed.com/platforms/MAX32630FTHR/
int pin_alarm = 43;  // Connects to GPIO0 pin of MAX31889
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
    Serial.println("MAX31889 use case example:");
    Serial.println("The temperature conversion will be trigger by GPIO control");
    Serial.println("Convertaion decision will be done by checking MAX31889 interrupt pin");
    Serial.println("------------------------------------------------------------");
    
    temp_sensor.begin();
    print_sensor_info();

    ret = temp_sensor.config_gpio(MAX31889::GPIO_NUM_0, MAX31889::GPIO_MODE_3);// GPIO0 mode INTB
    if (ret) {
        Serial.println("GPIO0 configure failed");
    }

    ret = temp_sensor.config_gpio(MAX31889::GPIO_NUM_1, MAX31889::GPIO_MODE_3);// GPIO1 mode CONVERT Trigger
    if (ret) {
        Serial.println("GPIO1 configure failed");
    }

    // Connects to MAX31889 GPIO1 pin 
    pinMode(pin_trigger, OUTPUT);
    
    // Connects to MAX31889 GPIO0 pin
    pinMode(pin_alarm, INPUT);

    ret = temp_sensor.set_interrupt(MAX31889::INT_TEMP_RDY, true);
    if (ret) {
        Serial.println("Interrupt initialization failed!");
    }

    Serial.println("---------------------");
    Serial.println("Measurement Started");
}

void loop()  {
    
    trigger_meas();

    delay(500); // wait a little
    
    if ( LOW == digitalRead(pin_alarm) ) {
        int ret;
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
