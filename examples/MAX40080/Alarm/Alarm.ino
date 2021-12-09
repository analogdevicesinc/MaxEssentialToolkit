#include <MaxEssentialToolkit.h>

MAX40080 sensor(&Wire, MAX40080_DEFAULT_I2C_ADDR_FOR_100_KOHM, 0.05);

// MAX32630FTHR P5.4 pin number is 44, https://os.mbed.com/platforms/MAX32630FTHR/
// MAX40080 ALERT pin connects to MAX32630FTHR P5.4
int pin_alert = 44;

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

    // enable interrupt
    sensor.set_interrupt_status(MAX40080::INTR_ID_CONV_READY, true);

    // For single convertion quick command need to be send
    sensor.send_quick_command();
}

void loop()  {
    int ret = 0;
    
    delay(500); // wait a liitle

    int pin_state = digitalRead(pin_alert);
    
    if (pin_state == LOW) {
        float voltage = 0.0f;
          
        ret = sensor.get_voltage(voltage);
        if (ret) {
            Serial.println("Read voltage failed!");
        } else {
            Serial.print("Measured voltage (V): ");
            Serial.println(voltage, 4);
        }

        sensor.clear_interrupt_flag(MAX40080::INTR_ID_CONV_READY);
        
        // Resend conversion command
        sensor.send_quick_command();   
    } 
}
