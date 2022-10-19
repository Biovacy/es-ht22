#include <driver/i2c.h>

void initMPU6050();

uint32_t getAccelerationMagnitude();

int16_t readLowAndHighRegisters(uint8_t registerL, uint8_t registerH);