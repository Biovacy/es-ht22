#include <stdio.h>
#include <driver/i2c.h>
#include <math.h>
#include <esp_pm.h>

#include "MPU6050.h"

/*
Example program that reads data from MPU6050
Connections:
Vcc -> 3.3V
GND -> GND
SCL -> 32
SDA -> 33
AD0 -> GND
*/

/*
From the Datasheet, page 33:
The slave address of the MPU-60X0 is b110100X which is 7 bits long.
The LSB bit of the 7 bit address is determined by the logic level on pin AD0.
This allows two MPU-60X0s to be connected to the same I2C bus.
*/

// If AD0 is LOW, the I2C address of the board will be 0x68. Otherwise, the address will be 0x69.
#define MPU6050_ADDR 0x68
/*
From Datasheet, page 40:
Register (Hex): 6B
Bit7 DEVICE_RESET
Bit6 SLEEP
Bit5 CYCLE
Bit4 -
Bit3 TEMP_DIS
Bit2, Bit1, Bit0 CLKSEL[2:0]
*/
#define MPU6050_PWR_MGMT_1 0x6B

/*
Register (Hex) 19
Bit7, Bit6, Bit5, Bit4, Bit3, Bit2, Bit1, Bit0: SMPLRT_DIV[7:0]
*/
#define MPU6050_SMPLRT_DIV 0x19

/*
From Datasheet, page 45:
Register (Hex): 75
Bit7 -
Bit6, Bit5, Bit4, Bit3, Bit2, Bit1: WHO_AM_I[6:1]
Bit0: -
*/
#define MPU6050_WHO_AM_I 0x75

/*
Define all axis, datasheet page 29
*/
#define MPU6050_X_OUT_H 0x3b
#define MPU6050_X_OUT_L 0x3c
#define MPU6050_Y_OUT_H 0x3d
#define MPU6050_Y_OUT_L 0x3e
#define MPU6050_Z_OUT_H 0x3f
#define MPU6050_Z_OUT_L 0x40

/**
 * Define the pins being used.
*/
#define SDA_PIN 33
#define SCL_PIN 32

void initMPU6050()
{

    // configure and install driver
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;             // ESP32 acts as master
    conf.sda_io_num = SDA_PIN;                    // pin used for SDA
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE; // The SDA and SCL lines are active low, so they should be pulled up with resistors
    conf.scl_io_num = SCL_PIN;                    // SCL pin number
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE; // enables pullup on SDA
    conf.master.clk_speed = 100000;          // Standard mode (100 Kbit/s)
    conf.clk_flags = 0;

    // configure I2C controller 0
    esp_err_t res = i2c_param_config(I2C_NUM_0, &conf);
    //ESP_ERROR_CHECK(res);

    // install driver, no buffers needed in master mode nor special interrupts config
    res = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    //ESP_ERROR_CHECK(res);

    // configure power mode
    // here we set all bits of the PWR_MGMT_1 register to 0
    // create command
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // start command
    res = i2c_master_start(cmd);
    //ESP_ERROR_CHECK(res);

    // set address + write and check for ack, might not be used, resets the Accel
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1);
    //ESP_ERROR_CHECK(res);

    // write the register address and check for ack set the cycle
    res = i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
    //ESP_ERROR_CHECK(res);

    // write value of the register: 0, and check for ack
    res = i2c_master_write_byte(cmd, 0x00, 1);
    //ESP_ERROR_CHECK(res);

    // end of command
    res = i2c_master_stop(cmd);
    //ESP_ERROR_CHECK(res);

    // send the command, 1 second timeout
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    //ESP_ERROR_CHECK(res);

    // delete command now that it's not needed
    i2c_cmd_link_delete(cmd);

    // set the sampling frequency
    // the sampling freq is gyro sampling freq / (1 + divider)
    // setting divider to 250 leads to sampling freq. of 32 Hz
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    //ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    //ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_SMPLRT_DIV, 1); // write to SMPLRT_DIV
    //ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, 250, 1); // set SMPLRT_DIV to 250
    //ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    //ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    // ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}

uint32_t getAccelerationMagnitude()
{
    // Read all the accelerations in all the axies
    double doubleX = readLowAndHighRegisters(MPU6050_X_OUT_L, MPU6050_X_OUT_H);
    double doubleY = readLowAndHighRegisters(MPU6050_Y_OUT_L, MPU6050_Y_OUT_H);
    double doubleZ = readLowAndHighRegisters(MPU6050_Z_OUT_L, MPU6050_Z_OUT_H);

    // Calculate the magnitude of the acceleration
    double accMagnitude = sqrt(pow(doubleX, 2) + pow(doubleY, 2) + pow(doubleZ, 2));
    
    /**
     * Question from lab:
     * "When can [accMagnitude] be zero?"
     * 
     * It can zero if you lift the acceleromiter at the same
     * acceleration as gravity. 
    */

    return (uint32_t) accMagnitude;
}

int16_t readLowAndHighRegisters(uint8_t lowRegister, uint8_t highRegister)
{
    // The returning value
    uint16_t value;
    
    // The temporary buffer that reads from the I2C
    uint8_t buffer;

    // Request low bits
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    // send the register number and activate desired bit to use accel
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);

    // Read low bits
    res = i2c_master_write_byte(cmd, lowRegister, 1); // read low first
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

    // Wait a little
    vTaskDelay(10 / portTICK_RATE_MS);

    // now read the answer
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ, 1); // READ bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

    // Assign the low bits to the value
    value = buffer;

    // Request high bits
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, highRegister, 1); // read high
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(10 / portTICK_RATE_MS);

    // Read high bits
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ, 1); // READ bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

    // Append the high bit to the value
    return value |= ((int16_t)buffer) << 8;
}