#include <MaxEssentialToolkit.h>

MAX40080 sensor(&Wire, MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM, 0.05);

MAX40080::reg_status_t    g_stat;
MAX40080::reg_cfg_t       g_cfg;
MAX40080::reg_fifo_cfg_t  g_fifo_cfg;

void setup() {
    int ret;

    Serial.begin(115200);
    Serial.println("---------------------");
    Serial.println("MAX40080 dump configuration example:");
    Serial.println(" ");

    sensor.begin();
}

void loop()  {
    int ret;

    delay(1000);
    Serial.println("**********************************************");

    ret = sensor.get_status(g_stat);
    if (ret) {
        Serial.println("Read status failed!");
    } else {
        Serial.println("----------- Status Register -----------");
        Serial.print("Wakeup          : ");  Serial.println(g_stat.bits.wakeup);
        Serial.print("Conversion Ready: ");  Serial.println(g_stat.bits.conv_ready);
        Serial.print("Overflow I      : ");  Serial.println(g_stat.bits.over_i);
        Serial.print("Overflow V      : ");  Serial.println(g_stat.bits.over_v);
        Serial.print("Underflow V     : ");  Serial.println(g_stat.bits.under_v);
        Serial.print("I2C Timeout     : ");  Serial.println(g_stat.bits.i2c_timeout);
        Serial.print("FIFO Alarm      : ");  Serial.println(g_stat.bits.fifo_alarm);
        Serial.print("FIFO Overflow   : ");  Serial.println(g_stat.bits.fifo_overflow);
        Serial.print("FIFO Data Count : ");  Serial.println(g_stat.bits.fifo_data_count);
    }

    ret = sensor.get_configuration(g_cfg);
    if (ret) {
        Serial.println("Read configuration failed!");
    } else {
        Serial.println("----------- Configuration Register -----------");
        Serial.print("Mode            : ");   Serial.println(g_cfg.bits.mode);
        Serial.print("I2C Timeout     : ");   Serial.println(g_cfg.bits.i2c_timeout);
        Serial.print("Alert           : ");   Serial.println(g_cfg.bits.alert);
        Serial.print("PEC             : ");   Serial.println(g_cfg.bits.pec);
        Serial.print("Input Range     : ");   Serial.println(g_cfg.bits.input_range);
        Serial.print("Stay HS Mode    : ");   Serial.println(g_cfg.bits.stay_hs_mode);
        Serial.print("ADC Sample Rate : ");   Serial.println(g_cfg.bits.adc_sample_rate);
        Serial.print("Digital Filter  : ");   Serial.println(g_cfg.bits.digital_filter);
    }
    
    ret = sensor.get_fifo_configuration(g_fifo_cfg);
    if (ret) {
        Serial.println("Read fifo configuration failed!");
    } else {
       Serial.println("----------- FIFO Configuration Register -----------");
       Serial.print("Store_IV        : ");   Serial.println(g_fifo_cfg.bits.store_iv);
       Serial.print("Overflow Warning: ");   Serial.println(g_fifo_cfg.bits.overflow_warning);
       Serial.print("Rollover        : ");   Serial.println(g_fifo_cfg.bits.rollover);
       Serial.print("Flush           : ");   Serial.println(g_fifo_cfg.bits.flush);
    } 
}
