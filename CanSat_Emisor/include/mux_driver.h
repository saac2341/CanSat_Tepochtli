/*Controladodor de los multiplexores*/

#ifndef MUX_DRIVER_H
#define MUX_DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include "pines.h"

#define TCA9548A_ADDRESS 0x70 /// Dirección del multiplexor I2C TCA9548A.

class HARDWARE_MUX
{
public:
    HARDWARE_MUX() {}

    /// @brief Inicializa el controlador de multiplexores.
    void begin()
    {
        /// Configuracion de pines para el multiplexor analógico.
        pinMode(PINOUT::MuxAnalog::S0_MUX_ANG, OUTPUT);
        pinMode(PINOUT::MuxAnalog::S1_MUX_ANG, OUTPUT);
        pinMode(PINOUT::MuxAnalog::S2_MUX_ANG, OUTPUT);
        pinMode(PINOUT::MuxAnalog::S3_MUX_ANG, OUTPUT);
        pinMode(PINOUT::MuxAnalog::ANG_MUX_LECTURA, INPUT);

        /* Configuracion de pines para el multiplexor I2C.
        pinMode(PINOUT::I2C_MUX::MUX_I2C_SCL, OUTPUT);     
        pinMode(PINOUT::I2C_MUX::MUX_I2C_SDA, OUTPUT);
        */
    }

    /// @brief Selecciona un canal del multiplexor I2C.
    /// @param channel
    void selectI2CChannel(uint8_t channel)
    {
        if (channel > 7)
            return; // Valid channels are 0-7
        Wire.beginTransmission(TCA9548A_ADDRESS);
        Wire.write(1 << channel); // Select the channel
        Wire.endTransmission();
    }
    /// @brief Lee un canal del multiplexor analógico.
    /// @param channel
    /// @return
    uint16_t readAnalogChannel(uint8_t channel)
    {
        digitalWrite(PINOUT::MuxAnalog::S0_MUX_ANG, bitRead(channel, 0));
        digitalWrite(PINOUT::MuxAnalog::S1_MUX_ANG, bitRead(channel, 1));
        digitalWrite(PINOUT::MuxAnalog::S2_MUX_ANG, bitRead(channel, 2));
        digitalWrite(PINOUT::MuxAnalog::S3_MUX_ANG, bitRead(channel, 3));

        delayMicroseconds(10); // Allow time for the channel to settle
        return analogRead(PINOUT::MuxAnalog::ANG_MUX_LECTURA);
    }
};

#endif // MUX_DRIVER_H