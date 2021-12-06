#include <MaxEssential.h>

MAX40080 sensor(&Wire, MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM, 0.05);

MAX40080::reg_status_t   g_stat;
MAX40080::reg_cfg_t      g_cfg;
MAX40080::reg_fifo_cfg_t g_fifo_cfg;

void setup() {
    int ret = 0;

    Serial.begin(115200);
    Serial.println("--------------------------------");
    Serial.println("MAX40080 read voltage use case example:");
    Serial.println("Please note that: MAX40080 sensor I2C pins need to be driven by 1.8V");
    Serial.println("Be sure your host board able to drive I2C pins with 1.8V");
    Serial.println(" ");

    sensor.begin();

#if defined(ARDUINO_MAXIM)
  /*
   *  Default Wire is #1 for MAX32630FTHR board
   *  If you change Wire number (while creating MAX40080 instance)
   *  you need to update below pins too, to match them with new Wire number
   */
  #if defined(MAX32630)
    useVDDIO(PIN_WIRE1_SDA); // To MAX32630FTHR board drive SDA line with 1.8V
    useVDDIO(PIN_WIRE1_SCL); // To MAX32630FTHR board drive SCL line with 1.8V
  #endif
#endif

    /*
     *  Set store type
     */
    ret = sensor.get_fifo_configuration(g_fifo_cfg);
    if (ret) {
        Serial.println("Read fifo configuration failed!");
    }

    g_fifo_cfg.store_iv = MAX40080::MEAS_VOLTAGE_ONLY;
    ret = sensor.set_fifo_configuration(g_fifo_cfg);
    if (ret) {
        Serial.println("Set fifo configuration failed!");
    }

    /*
     *  Set operation mode
     */
    ret = sensor.get_configuration(g_cfg);
    if (ret) {
        Serial.println("Read configuration failed!");
    }

    g_cfg.mode = MAX40080::OP_MODE_SINGLE_CONVERTION;
    g_cfg.digital_filter = MAX40080::AVERAGE_1_SAMPLE;
    
    ret = sensor.set_configuration(g_cfg);
    if (ret) {
        Serial.println("Set configuration failed!");
    }

    // For single convertion quick command need to be send
    sensor.send_quick_command();
}

void loop()  {
    int ret = 0;
    
    delay(500); // wait a liitle
    
    ret = sensor.get_status(g_stat);
    if (ret == 0) {
      if (g_stat.conv_ready) {
          float voltage = 0.0f;
          
          ret = sensor.get_voltage(voltage);
          if (ret) {
              Serial.println("Read voltage failed!");
          } else {
              Serial.print("Measured voltage (V): ");
              Serial.println(voltage, 4);
          }

          // Resend conversion command
          sensor.send_quick_command();    
      } else {
          Serial.println("Conversion not ready!");
      }
    }
}
