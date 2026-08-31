/** @file structures.h
 *  @brief Definición de estructuras de datos para el CanSat.
 *  @author [Cansat Team]
 *  @date [2024-06-15]
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h>

// 1. Datos del IMU + Barómetro (GY-91 = MPU9250 + BMP280)
struct InertialBaroData
{
    // MPU9250
    float accel_x, accel_y, accel_z; // m/s^2 o g
    float gyro_x, gyro_y, gyro_z;    // deg/s o rad/s
    float mag_x, mag_y, mag_z;       // uT (Microteslas)

    // BMP280 / BME688
    float pressure; // hPa
    float altitude; // m
};

// 2. Calidad de Aire y Gases (ENS160 + MICS-6814 + BME688)
struct EnvironmentalGasData
{
    // BME688
    float temperature; // °C
    float humidity;    // %RH
    float gas_res_bme; // Ohms (Resistencia de la capa de gas)

    // ENS160
    uint16_t eco2; // ppm (eCO2)
    uint16_t tvoc; // ppb (TVOC)
    uint8_t aqi;   // Air Quality Index (1-5)

    // MICS-6814 (Lecturas analógicas/resistencia)
    float gas_co;  // CO (PPM / Resistencia)
    float gas_no2; // NO2 (PPM / Resistencia)
    float gas_nh3; // NH3 (PPM / Resistencia)
};

// 3. Monitoreo de Energía (INA219)
struct PowerData
{
    float bus_voltage;   // V
    float current_mA;    // mA
    float power_mW;      // mW
    float battery_level; // %
};

// 4. Estructura General de Sensores
struct SensorData
{
    InertialBaroData imu;
    EnvironmentalGasData env;
    PowerData power;
    uint32_t timestamp; // ms transcurridos desde inicio
};

// 5. Estructura para Estado/Gestión de Multiplexores (TCA9548A)
struct MultiplexerData
{
    uint8_t active_channel;          // Canal I2C actualmente activo (0 - 7)
    uint8_t connected_channels_mask; // Máscara de bits para canales detectados (ej. 0b00000101)
    bool channel_status[8];          // Estado de salud o presencia de dispositivo por canal
};

#endif // STRUCTURES_H