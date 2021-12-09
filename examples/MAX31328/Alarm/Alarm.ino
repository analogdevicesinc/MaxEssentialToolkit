#include <MaxEssential.h>


MAX31328 rtc(&Wire, MAX3128_I2C_ADDRESS);
int pin_interrupt = 2; // // INT pin that connect to MAX31328
volatile bool interrupt_occured = false;

void rtc_interrupt_handler() {
    interrupt_occured = true;
}

void setup() {
    // Setup
    Serial.begin(115200);
    pinMode(pin_interrupt, INPUT);

    // Attach interrupt on INT pin of MAX31328.
    attachInterrupt(digitalPinToInterrupt(pin_interrupt), rtc_interrupt_handler, FALLING);

    rtc.begin();

    // Configure alarm to fire once per second
    max31328_alrm_t alarm = {0};
    alarm.am1 = 1;
    alarm.am2 = 1;
    alarm.am3 = 1;
    alarm.am4 = 1;
    alarm.date = 1;
    alarm.day = 1;
  
    max31328_cntl_stat_t regs;
    if ( rtc.get_cntl_stat_reg(&regs) ) {
        Serial.println("ERROR: Cannot read control register.");
    }

    // Enable Alarm 1 Interrupt and set Interrupt Control
    regs.control |= MAX31328_F_CTRL_INTCN | MAX31328_F_CTRL_A1IE;
    regs.status = 0;
    if ( rtc.set_cntl_stat_reg(regs) ) {
        Serial.println("ERROR: Cannot set control register.");
    }

    // Set Alarm 1
    if ( rtc.set_alarm(alarm, true) ) {
        Serial.println("ERROR: Cannot set alarm 1.");
    }
}

void print_current_time() {
    max31328_time_t time;
    // Get current time from MAX31328
    if ( rtc.get_time(&time) ) {
        Serial.println("ERROR: Cannot get time.");
        return;
    }

    // Print time
    Serial.print("RTC Time: ");
    Serial.print(time.hours);
    Serial.print(":");
    Serial.print(time.minutes);
    Serial.print(":");
    Serial.println(time.seconds);
}

void loop() {
  
    // Check if the interrupt has occured
    if (interrupt_occured) {

        interrupt_occured = false;
        Serial.print("1 Second has passed! ");

        // Print time
        print_current_time();

        // Clear Interrupt Status
        max31328_cntl_stat_t regs;
        rtc.get_cntl_stat_reg(&regs);
        regs.status = 0;
        rtc.set_cntl_stat_reg(regs);
    }
}
