#include <MaxEssential.h>

MAX40080 sensor(&Wire, MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM, 0.05);

// Pin 44 is P5.4 for MAX32630FTHR Board: https://os.mbed.com/platforms/MAX32630FTHR/
// MAX40080 ALERT pin connects to MAX32630FTHR P5.4
int pin_alert = 44;

MAX40080::reg_status_t   g_stat;
MAX40080::reg_cfg_t      g_cfg;
MAX40080::reg_fifo_cfg_t g_fifo_cfg;

void setup() {
    int ret = 0;

    Serial.begin(115200);
    Serial.println("--------------------------------");
    Serial.println("MAX40080 alarm use case example:");
    Serial.println("Please note that: MAX40080 sensor I2C pins need to be driven by 1.8V");
    Serial.println("Be sure your host board able to drive I2C pins with 1.8V");
    Serial.println(" ");

    // Configure alarm pin as input
    pinMode(pin_alert, INPUT);
    
    sensor.begin();

#if defined(ARDUINO_MAXIM)
  /*
   *  Default Wire is #1 for MAX32630FTHR board
   *  If you change Wire number (while creating MAX40080 instance)
   *  you need to update below pins too, to match them with new Wire number
   */
  #if defined(MAX32630)
    useVDDIO(pin_alert); // To MAX32630FTHR board drive ALERT pin with 1.8V
  
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

    g_fifo_cfg.store_iv = MAX40080::MEAS_CURRENT_AND_VOLTAGE;
    g_fifo_cfg.overflow_warning = 8;
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

    g_cfg.mode = MAX40080::OP_MODE_4SPS;
    g_cfg.digital_filter = MAX40080::AVERAGE_1_SAMPLE;
    
    ret = sensor.set_configuration(g_cfg);
    if (ret) {
        Serial.println("Set configuration failed!");
    }

    // enable interrupt
    ret = sensor.set_interrupt_status(MAX40080::INTR_ID_OVERFLOW_WARNING, true);
    if (ret) {
        Serial.println("Set interrupt failed!");
    }
}

void loop()  {
    int ret = 0;

    int pin_state = digitalRead(pin_alert);
    
    if (pin_state == LOW) {
        //
        sensor.clear_interrupts();

        int fifo_data_count = 0;
        float voltage;
        float current;
        
        ret = sensor.get_status(g_stat);
        if (ret) {
            Serial.println("Status read failed!");
            return;
        }

        fifo_data_count = g_stat.fifo_data_count;
    
        if ( (fifo_data_count == 0) && (g_stat.fifo_overflow == 1) ) {
            fifo_data_count = 64;
        }

        Serial.print("Number of sample in fifo: ");
        Serial.println(fifo_data_count);
        
        for (int i=0; i < fifo_data_count; i++) {
            
            voltage = 0.0f;
            current = 0.0f;

            ret = sensor.get_current_and_voltage(current, voltage);        
            if (ret == 0) {
                Serial.print("Measured values: ");
                Serial.print(voltage, 4);
                Serial.print(" (V)");
                Serial.print(" - ");
                Serial.print(current, 4);
                Serial.println(" (A)");
            } else if ( ret == MAX40080_ERR_DATA_NOT_VALID) {
              Serial.println("Read failed (Not Valid Data)!");
            } else {
              Serial.println("Read failed!");
            }
        }
    } 
}
