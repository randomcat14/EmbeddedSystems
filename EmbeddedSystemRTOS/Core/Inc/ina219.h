#ifndef INA219_H_
#define INA219_H_

#include "stm32f4xx_hal.h"

//Default address (shifted for HAL)
#define INA219_ADDRESS       (0x40 << 1)

// Registers
#define INA219_REG_CONFIG         0x00
#define INA219_REG_SHUNT_VOLTAGE  0x01
#define INA219_REG_BUS_VOLTAGE    0x02
#define INA219_REG_POWER          0x03
#define INA219_REG_CURRENT        0x04
#define INA219_REG_CALIBRATION    0x05

// Calibration values
#define INA219_CALIBRATION_VALUE  0x6A0B
#define CURRENT_LSB               0.000015f
#define POWER_LSB                 0.0003f

// Multiple i2c instance structure
typedef struct {
    I2C_HandleTypeDef *i2c;
    uint8_t address;
    float current_lsb;
    float power_lsb;
} INA219_t;

//Pointers are used to account for multiple instances
void INA219_Init(INA219_t *dev);

float INA219_ReadBusVoltage_V(INA219_t *dev);
float INA219_ReadShuntVoltage_mV(INA219_t *dev);
float INA219_ReadCurrent_mA(INA219_t *dev);
float INA219_ReadPower_mW(INA219_t *dev);

HAL_StatusTypeDef INA219_ReadRegister(INA219_t *dev, uint8_t reg, uint16_t *value);
HAL_StatusTypeDef INA219_WriteRegister(INA219_t *dev, uint8_t reg, uint16_t value);

#endif
