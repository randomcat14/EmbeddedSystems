#include "ina219.h"

// Register Access


HAL_StatusTypeDef INA219_WriteRegister(INA219_t *dev, uint8_t reg, uint16_t value)
{
    uint8_t data[3];
    data[0] = reg;
    data[1] = (value >> 8) & 0xFF;
    data[2] = value & 0xFF;

    return HAL_I2C_Master_Transmit(dev->i2c, dev->address, data, 3, HAL_MAX_DELAY);
}

HAL_StatusTypeDef INA219_ReadRegister(INA219_t *dev, uint8_t reg, uint16_t *value)
{
    uint8_t data[2];

    if (HAL_I2C_Master_Transmit(dev->i2c, dev->address, &reg, 1, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    if (HAL_I2C_Master_Receive(dev->i2c, dev->address, data, 2, HAL_MAX_DELAY) != HAL_OK)
        return HAL_ERROR;

    *value = (data[0] << 8) | data[1];
    return HAL_OK;
}

void INA219_Init(INA219_t *dev)
{
    dev->current_lsb = CURRENT_LSB;
    dev->power_lsb   = POWER_LSB;

    uint16_t config =
        (1 << 13) |  // BRNG 32V
        (3 << 11) |  // PG 320mV
        (3 << 7)  |  // BADC 12-bit
        (3 << 3)  |  // SADC 12-bit
        7;           // Mode: continuous

    INA219_WriteRegister(dev, INA219_REG_CONFIG, config);
    INA219_WriteRegister(dev, INA219_REG_CALIBRATION, INA219_CALIBRATION_VALUE);
}


// Reading Functions

float INA219_ReadBusVoltage_V(INA219_t *dev)
{
    uint16_t raw;
    INA219_ReadRegister(dev, INA219_REG_BUS_VOLTAGE, &raw);
    raw >>= 3;
    return raw * 0.004f;
}

float INA219_ReadShuntVoltage_mV(INA219_t *dev)
{
    uint16_t raw;
    INA219_ReadRegister(dev, INA219_REG_SHUNT_VOLTAGE, &raw);
    return (int16_t)raw * 0.01f;
}

float INA219_ReadCurrent_mA(INA219_t *dev)
{
    uint16_t raw;
    INA219_ReadRegister(dev, INA219_REG_CURRENT, &raw);
    return (int16_t)raw * (dev->current_lsb * 1000.0f);
}

float INA219_ReadPower_mW(INA219_t *dev)
{
    uint16_t raw;
    INA219_ReadRegister(dev, INA219_REG_POWER, &raw);
    return raw * (dev->power_lsb * 1000.0f);
}
