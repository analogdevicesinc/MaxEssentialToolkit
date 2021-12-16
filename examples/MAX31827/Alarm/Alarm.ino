#include <MaxEssentialToolkit.h>

#define ALARM_LOW    (21.0f)
#define ALARM_HIGH   (22.5f)

// I2C Addr depend on the R_SEL resistor
MAX31827 temp_sensor(&Wire, MAX31827_I2C_ADDR_FOR_640_0_KOHM);

// Pin 43 is P5.3 for MAX32630FTHR Board: https://os.mbed.com/platforms/MAX32630FTHR/
// MAX31827 ALARM pin connects to MAX32630FTHR P5.3
int pin_alarm = 43;

void setup()  {
    int ret;

    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31827 alarm mode use case example:");
    Serial.println("------------------------------------------");

    // Configure alarm pin as input
    pinMode(pin_alarm, INPUT);

    temp_sensor.begin();

    MAX31827::reg_cfg_t cfg;
    ret = temp_sensor.get_configuration(cfg);
    if (ret) {
        Serial.println("Get configuration failed!");    
    }

    // Update resolution to 12bits
    cfg.bits.resolution = MAX31827::RESOLUTION_12_BIT; 
    ret = temp_sensor.set_configuration(cfg);
    if (ret) {
        Serial.println("Set configuration failed!");    
    }

    ret = temp_sensor.set_alarm(ALARM_LOW, ALARM_HIGH);
    if (ret) {
        Serial.println("Set alarm failed!");
    }
    
    float temp_low = 0.0;
    float temp_high = 0.0;
    
    ret = temp_sensor.get_alarm(temp_low, temp_high);
    if (ret) {
        Serial.println("Get alarm failed!");
    } else {
        Serial.print("Alarm configured for ");
        Serial.print("Temp Low: ");
        Serial.print(temp_low, 2);
        Serial.print("  Temp High: ");
        Serial.println(temp_high, 2);
    }

    ret = temp_sensor.start_temp_conversion(MAX31827::PERIOD_4_DIV_1SEC);
    if (ret) {
        Serial.println("Start measurement failed!");
    } else {
        Serial.println("Measurement Started");
    }
}

void loop()  {
    
    int pin_state = digitalRead(pin_alarm);
    
    if (pin_state == HIGH) {
        int ret;
        MAX31827::status_t  status;
        float temp = 0;

        ret = temp_sensor.get_status(status);
        if (ret) {
            Serial.println("Status read failed!");
            return;
        }
    
        if (status.temp_high) {
            Serial.println("---High temperature alarm detected.---");
        }
    
        if (status.temp_low) {
            Serial.println("---Low temperature alarm detected.---");
        }
          
        ret = temp_sensor.get_temp(temp);
        if (ret) {
            Serial.println("Temperature read failed!");
        } else {
            Serial.print("Temperature (Celsius): ");
            Serial.println(temp, 4);
        }
        
        delay(500); // wait a little
    }
}
