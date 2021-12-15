/*******************************************************************************
* Copyright (C) 2021 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/

#ifndef _MAX31889_H_
#define _MAX31889_H_


#include <MAX31889/MAX31889_registers.h>

#include <Arduino.h>
#include <Wire.h>


#define MAX31889_FIFO_DEPTH         32   // Max Sample count in fifo
#define MAX31889_DEFAULT_I2C_ADDR   0x50 // 7 bit address


class MAX31889
{
    public:
        typedef enum {
            GPIO_NUM_0 = 0, // GPIO0
            GPIO_NUM_1,     // GPIO1
            GPIO_NUM_MAX
        } gpio_t;

        typedef enum {
            GPIO_MODE_0 = 0,  // Digital input (HiZ)
            GPIO_MODE_1,      // Digital output (open-drain).
            GPIO_MODE_2,      // Digital Input with 1M Ohm pulldown.
            GPIO_MODE_3,      // if GPIO1: Convert Temperature Input (active low)
                              // if GPII0: INTB (open-drain, active low) 
            GPIO_MODE_MAX
        } gpio_mode_t;

        typedef enum {
            INTR_ID_TEMP_RDY         = MAX31889_F_INT_EN_TEMP_RDY,
            INTR_ID_TEMP_HIGH        = MAX31889_F_INT_EN_TEMP_HI,
            INTR_ID_TEMP_LOW         = MAX31889_F_INT_EN_TEMP_LO,
            INTR_ID_FIFO_ALMOST_FULL = MAX31889_F_INT_EN_A_FULL,
            INTR_ID_ALL              = 0xFF
        } intr_id_t;

        typedef union {
            uint8_t raw;
            struct {
                uint8_t temp_rdy  : 1; // temperature ready
                uint8_t temp_high : 1; // temp high
                uint8_t temp_low  : 1; // temp low
                uint8_t           : 4; // not used
                uint8_t a_full    : 1; // almost full
            } bits;
        } reg_status_t;

        typedef union {
            uint8_t raw;
            struct {
                uint8_t               : 1; // not used
                uint8_t fifo_ro       : 1; // fifo roolover
                uint8_t a_full_type   : 1; // almost full type
                uint8_t fifo_stat_clr : 1; // fifo stat clear
                uint8_t flush_fifo    : 1; // 
                uint8_t               : 3; // not used
            } bits;
        } reg_fifo_cfg_t;

        typedef struct {
            uint8_t rom_id[6];
            uint8_t part_id; 
        } id_t;


        /**
        * @brief    Constructer to driver sensor over i2c interface
        *
        * @param[in]    i2c: I2C instance
        * @param[in]    i2c_addr: slave address
        */
        MAX31889(TwoWire *i2c, uint8_t i2c_addr = MAX31889_DEFAULT_I2C_ADDR);

        /**
        * @brief    Must be call one time before using class methods
        *
        */
        void begin(void);

        /**
        * @brief        Read targver version and part id
        * 
        * @param[out]   id_t
        *
        * @return       0 on success, error code on failure
        */
        int get_id(id_t &id);

        /**
        * @brief        To get status register of target. 
        *
        * @param[out]   stat: Decoded status register
        *
        * @return       0 on success, error code on failure
        */        
        int get_status(reg_status_t &stat);

        /**
        * @brief        Enable interrupt
        *
        * @param[in]    id Interrupt id, one of INTR_ID_*
        *
        * @return       0 on success, error code on failure
        */
        int irq_enable(intr_id_t id);

        /**
        * @brief        Disable interrupt
        *
        * @param[in]    id Interrupt id, one of INTR_ID_*
        *
        * @return       0 on success, error code on failure
        */
        int irq_disable(intr_id_t id);
        
        /**
        * @brief    Clear the interrupt flags, 
        *
        * @return   0 on success, error code on failure
        */
        int irq_clear_flag(intr_id_t id = INTR_ID_ALL);

        /**
        * @brief        Configure gpio, select GPIO0/1 working mechanism 
        *
        * @param[in]    gpio_t, gpio number
        * @param[in]    gpio_mode_t 
        *
        * @return       0 on success, error code on failure
        */
        int config_gpio(gpio_t gpio, gpio_mode_t mode);

        /**
        * @brief        Read current gpio state 
        *
        * @param[in]    gpio_t, gpio number
        *
        * @return       1 on high, 0 on low, others on error
        */
        int get_gpio_state(gpio_t gpio);

        /**
        * @brief        Set gpio value 
        *
        * @param[in]    gpio_t, gpio number
        * @param[in]    1 to high, 0 to low
        *
        * @return       0 on success, error code on failure
        */
        int set_gpio_state(gpio_t gpio, int state);
     
        /**
        * @brief        Get existing alarm low and alarm high values. 
        *
        * @param[out]   temp_low: Existing low alarm temperature
        * @param[out]   temp_hig: Existing high alarm temperature
        * 
        * @return       0 on success, error code on failure
        */
        int get_alarm(float &temp_low, float &temp_high);

        /**
        * @brief        Set alarm low and alarm high. 
        *
        * @param[in]    temp_low: Low alarm temperature
        * @param[in]    temp_hig: High alarm temperature
        * 
        * @return       0 on success, error code on failure
        */ 
        int set_alarm(float temp_low=-40.0, float temp_high=125.0);

        /**
        * @brief        Start temperature conversion.
        *
        * 
        * @return       0 on success, error code on failure
        */
        int start_temp_conversion(void);

         /**
        * @brief        Get number of samples in fifo.
        *
        * 
        * @return       -1 on failure, others number of samples
        */       
        int get_num_of_sample(void);

        /**
        * @brief        Read temperature value. 
        *
        * @param[out]   tmp: Temperature value that measured
        * @param[in]    num_of_samples: Number of samples that would like to read
        * 
        * @return       0 on success, error code on failure
        */        
        int get_temp(float *temp, int num_of_samples=1);
        
        /**
        * @brief        Flush fifo. 
        * 
        * @return       0 on success, error code on failure
        */  
        int flush_fifo();

        /**
        * @brief        Read configuration register of sensor.
        *
        * @param[out]   cfg: Decoded fifo configuration register
        *
        * @return       0 on success, error code on failure
        */       
        int get_fifo_configuration(reg_fifo_cfg_t &cfg);
        
        /**
        * @brief        Set configuration register of sensor.
        *
        * @param[in]    fifo_configuration register
        *
        * @return       0 on success, error code on failure
        */
        int set_fifo_configuration(reg_fifo_cfg_t cfg);
        
        /**
        * @brief        Set allmost full interrupt depth. 
        *
        * @param[in]    number of sample
        *
        * @return       0 on success, error code on failure
        */
        int set_almost_full_depth(unsigned int num_of_samples);

        /**
        * @brief        Reset all sensor register 
        *
        * @return       0 on success, error code on failure
        */
        int reset_registers(void);

        // Register access function
        int read_register(uint8_t reg, uint8_t *data, int len=1);
        int write_register(uint8_t reg, const uint8_t *data, int len=1);

    private:
        TwoWire *m_i2c;
        uint8_t  m_slave_addr;

        float    convert_count_2_temp(uint16_t count);
        uint16_t convert_temp_2_count(float temp);   
};

#endif /* _MAX31889_H_ */
