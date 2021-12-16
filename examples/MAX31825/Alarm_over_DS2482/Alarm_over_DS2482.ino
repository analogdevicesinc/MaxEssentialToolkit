#include <MaxEssentialToolkit.h>

#define ALARM_LOW    (22.0f)
#define ALARM_HIGH   (26.0f)

/*
    DS2482 is I2C to OneWire converter, 
    This example use DS2482 to drive MAX31825 temperature sensor
    MAX31825EVKIT default has DS2482 converter.
    Webpage: 
        https://www.maximintegrated.com/en/products/sensors/MAX31825EVKIT.html
        https://www.maximintegrated.com/en/products/sensors/MAX31825EVSYS1.html
*/

MAX31825 temp_sensor(&Wire, DS2482_I2C_ADDR_AD_00);

// MAX31825 ALARM pin connects to Pin 9
int pin_alarm = 9;

void setup()  {
    int ret;
    byte serial[8];
    
    Serial.begin(115200);
    //
    Serial.println(" ");
    Serial.println("MAX31825 alarm use case example:");
    Serial.println("The MAX31825 temperature sensor will be driven over DS2482 (I2C to Onewire converter)");
    Serial.println("Addressing mode can be changed by set_addressing_mode function");
    Serial.println("------------------------------------------------------------");

    // Configure alarm pin as input
    pinMode(pin_alarm, INPUT);

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

    ret = temp_sensor.set_alarm(ALARM_LOW, ALARM_HIGH);
    if (ret) {
        Serial.println("Set alarm failed!");
    }
    
    float temp_low = 0.0;
    float temp_high = 0.0;

    delay(1);
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

    delay(1);

    MAX31825::reg_cfg_t cfg;
    
    ret = temp_sensor.get_configuration(cfg);
    if (ret) {
        Serial.println("Get configuration failed!");
    }

    // Set configuration
    cfg.bits.comp_int = MAX31825::MODE_COMPARE;

    ret = temp_sensor.set_configuration(cfg);
    if (ret) {
        Serial.println("Set configuration failed!");
    } else {
        Serial.println("--------- Configuration Register (BIN Format) ---------");
        Serial.print("Convertion Rate: ");      Serial.println(cfg.bits.conversion_rate, BIN);
        Serial.print("COMP/INT       : ");      Serial.println(cfg.bits.comp_int, BIN);
        Serial.print("Resolution     : ");      Serial.println(cfg.bits.resolution, BIN);
        Serial.print("Format         : ");      Serial.println(cfg.bits.format, BIN);
    }

    Serial.println("---------------------");
    Serial.println("Measurement Started"); 
}

void loop()  {

    int ret = temp_sensor.start_temp_conversion();
    if (ret) {
        Serial.println("Start measurement failed!");
    }
    
    delay(500); // wait a little
    
    int pin_state = digitalRead(pin_alarm);
    
    if (pin_state == LOW) {

        MAX31825::reg_status_t  status;
        float temp = 0;

        ret = temp_sensor.get_status(status);
        if (ret) {
            Serial.println("Status read failed!");
            return;
        }
    
        if (status.bits.th_fault) {
            Serial.println("---High temperature alarm detected.---");
        }
    
        if (status.bits.tl_fault) {
            Serial.println("---Low temperature alarm detected.---");
        }
          
        ret = temp_sensor.get_temp(temp);
        if (ret) {
            Serial.println("Temperature read failed!");
        } else {
            Serial.print("Temperature (Celsius): ");
            Serial.println(temp, 4);
        }
    }
}
