/*Libreria para la gestion de sensores*/

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include "structures.h"
#include "mux_driver.h"
#include "pines.h"

/// Libreria de los sensores.
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <ScioSense_ENS160.h>
#include <Adafruit_INA219.h>
#include <MPU9250.h>

class SensorManager{
private:
    HARDWARE_MUX _mux; // Objeto para el control de los multiplexores.

    Adafruit_BME680 _bme;  // Objeto para el sensor BME680.
    ScioSense_ENS160 _ens; // Objeto para el sensor ENS160.
    MPU9250 _mpu;          // Objeto para el sensor MPU9250.
    Adafruit_INA219 _ina;  // Objeto para el sensor INA219.

public:
    SensorManager() : _ens(ENS160_I2CADDR_0) {} // Constructor de la clase SensorManager.
    bool initSensors()
    {                       // Inicializa los sensores y el multiplexor. Retorna true si todos los sensores estan conectados y funcionando correctamente, false si alguno de los sensores no esta conectado o no funciona correctamente.
        _mux.begin();       // Inicializa el multiplexor.
        bool status = true; // Estado de los sensores, true si todos los sensores estan conectados y funcionando correctamente, false si alguno de los sensores no esta conectado o no funciona correctamente.

        // Para GY-91 (MPU9250 + BMP280)
        _mux.selectI2CChannel(PINOUT::I2C_Channel::GY91); // Selecciona el canal del multiplexor I2C para el sensor GY-91.
        if (_bme.begin() < 0)
            status = false; // Si el sensor no se inicializa correctamente, cambia el estado a false.

        /// Para BME680
        _mux.selectI2CChannel(PINOUT::I2C_Channel::BME680); // Selecciona el canal del multiplexor I2C para el sensor BME680.
        if (!_bme.begin(0x76))
            status = false; // Si el sensor no se inicializa correctamente, cambia el estado a false.

        /// Para ENS160
        _mux.selectI2CChannel(PINOUT::I2C_Channel::ENS160); // Selecciona el canal del multiplexor I2C para el sensor ENS160.
        if (!_ina.begin())
            status = false; // Si el sensor no se inicializa correctamente, cambia el estado a false.

        return status;
    }

    void updateALL(SensorData & data)
    {
        data.timestamp = millis(); // Actualiza el timestamp de la estructura de datos.
        // Actualiza los datos de los sensores.
        _mux.selectI2CChannel(PINOUT::I2C_Channel::GY91); // Selecciona el canal del multiplexor I2C para el sensor GY-91.
        if (_mpu.update())
        { // Si la lectura del sensor es correcta, actualiza los datos de aceleración, giroscopio y magnetómetro.
            data.imu.accel_x = _mpu.getAccX(); // Actualiza los datos de aceleración en m/s^2 o g.
            data.imu.accel_y = _mpu.getAccY(); // Actualiza los datos de aceleración en m/s^2 o g.
            data.imu.accel_z = _mpu.getAccZ(); // Actualiza los datos de aceleración en m/s^2 o g.

            data.imu.gyro_x = _mpu.getGyroX(); // Actualiza los datos de giroscopio en deg/s o rad/s.
            data.imu.gyro_y = _mpu.getGyroY(); // Actualiza los datos de giroscopio en deg/s o rad/s.
            data.imu.gyro_z = _mpu.getGyroZ(); // Actualiza los datos de giroscopio en deg/s o rad/s.

            data.imu.mag_x = _mpu.getMagX(); // Actualiza los datos de magnetómetro en uT (Microteslas).
            data.imu.mag_y = _mpu.getMagY(); // Actualiza los datos de magnetómetro en uT (Microteslas).
            data.imu.mag_z = _mpu.getMagZ(); // Actualiza los datos de magnetómetro en uT (Microteslas).
        }

        _mux.selectI2CChannel(PINOUT::I2C_Channel::BME680); // Selecciona el canal del multiplexor I2C para el sensor BME680.
        if (_bme.performReading())
        {                                            // Si la lectura del sensor es correcta, actualiza los datos de presión, altitud, temperatura y humedad.
            data.env.temperature = _bme.temperature; // Actualiza los datos de temperatura en °C.
            data.env.humidity = _bme.humidity;       // Actualiza los datos de humedad en %RH.
        }

        _mux.selectI2CChannel(PINOUT::I2C_Channel::ENS160); // Selecciona el canal del multiplexor I2C para el sensor ENS160.
        data.power.bus_voltage = _ina.getBusVoltage_V();   // Actualiza los datos de voltaje del bus en V.

        /// Lecturas analogicas de los sensores de gas MICS-6814
        uint16_t raw_co = _mux.readAnalogChannel(PINOUT::ANALOG_MUX::MISC_CO);   // Lee el canal del multiplexor analógico para el sensor de CO.
        uint16_t raw_no2 = _mux.readAnalogChannel(PINOUT::ANALOG_MUX::MISC_NO2); // Lee el canal del multiplexor analógico para el sensor de NO2.
        uint16_t raw_nh3 = _mux.readAnalogChannel(PINOUT::ANALOG_MUX::MISC_NH3); // Lee el canal del multiplexor analógico para el sensor de

        data.env.gas_co = (raw_co * 3.3f) / 4095.0f;   // Actualiza los datos de CO en PPM.
        data.env.gas_no2 = (raw_no2 * 3.3f) / 4095.0f; // Actualiza los datos de NO2 en PPM.
        data.env.gas_nh3 = (raw_nh3 * 3.3f) / 4095.0f; // Actualiza los datos de NH3 en PPM.
    }
};

#endif // SENSOR_MANAGER_H