#include <MaxEssential.h>

MAX40080 sensor(&Wire, MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM, 0.05);

void setup() {
    int ret;

    Serial.begin(115200);
    Serial.println("---------------------");
    Serial.println("MAX40080 dump configuration example:");
    Serial.println(" ");

    sensor.begin();
}

void loop()  {
    
    delay(1000);
    Serial.println("**********************************************");

    int ret;

    MAX40080::reg_status_t   stat;

    ret = sensor.get_status(stat);
    if (ret) {
        Serial.println("Read status failed!");
    } else {
        Serial.println("----------- Status Register -----------");
        Serial.print("Wakeup: ");            Serial.println(stat.wakeup);
        Serial.print("Conversion Ready: ");  Serial.println(stat.conv_ready);
        Serial.print("Overflow I: ");        Serial.println(stat.over_i);
        Serial.print("Overflow V: ");        Serial.println(stat.over_v);
        Serial.print("Underflow V: ");       Serial.println(stat.under_v);
        Serial.print("I2C Timeout: ");       Serial.println(stat.i2c_timeout);
        Serial.print("FIFO Alarm: ");        Serial.println(stat.fifo_alarm);
        Serial.print("FIFO Overflow: ");     Serial.println(stat.fifo_overflow);
        Serial.print("FIFO Data Count: ");   Serial.println(stat.fifo_data_count);
    }

    MAX40080::reg_cfg_t  cfg;
    
    ret = sensor.get_configuration(cfg);
    if (ret) {
        Serial.println("Read configuration failed!");
    } else {
        Serial.println("----------- Configuration Register -----------");
        Serial.print("Mode: ");            Serial.println(cfg.mode);
        Serial.print("I2C Timeout: ");     Serial.println(cfg.i2c_timeout);
        Serial.print("Alert: ");           Serial.println(cfg.alert);
        Serial.print("PEC: ");             Serial.println(cfg.pec);
        Serial.print("Input Range: ");     Serial.println(cfg.input_range);
        Serial.print("Stay HS Mode: ");    Serial.println(cfg.stay_hs_mode);
        Serial.print("ADC Sample Rate: "); Serial.println(cfg.adc_sample_rate);
        Serial.print("Digital Filter: ");  Serial.println(cfg.digital_filter);
    }

    MAX40080::reg_fifo_cfg_t fifo_cfg;
    
    ret = sensor.get_fifo_configuration(fifo_cfg);
    if (ret) {
        Serial.println("Read fifo configuration failed!");
    } else {
       Serial.println("----------- FIFO Configuration Register -----------");
       Serial.print("Store_IV: ");          Serial.println(fifo_cfg.store_iv);
       Serial.print("Overflow Warning: ");  Serial.println(fifo_cfg.overflow_warning);
       Serial.print("Rollover: ");          Serial.println(fifo_cfg.rollover);
       Serial.print("Flush: ");             Serial.println(fifo_cfg.flush);
    } 
}
