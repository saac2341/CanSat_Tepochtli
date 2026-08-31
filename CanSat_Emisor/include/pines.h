/*
 * Definicion de pines de la ESP32_C6  hacia los sensores y multiplexores.
 */

#ifndef DEFAULT_H
#define DEFAULT_H

#include <stdint.h>

/// @brief Espacio de nombres para los pines de la ESP32_C6.
namespace PINOUT
{
    /// MUltiplexor CD74HC4067M
    namespace MuxAnalog
    {
        constexpr uint8_t S0_MUX_ANG = 9;
        constexpr uint8_t S1_MUX_ANG = 10;
        constexpr uint8_t S2_MUX_ANG = 11;
        constexpr uint8_t S3_MUX_ANG = 12;
        constexpr uint8_t ANG_MUX_LECTURA = 3; /// Pin de lectura al ESP32_C6 desde el multiplexor analógico.
    }

    /// Multiplexor TCA9548A
    namespace I2C_MUX
    {
        constexpr uint8_t MUX_I2C_SCL = 22;
        constexpr uint8_t MUX_I2C_SDA = 21;
        /// constexpr uint8_t RCT_I2C = 8; ///Pin de reset del multiplexor I2C.
    }

    // Gestor de canales de los multiplexores, tanto I2C como analógico, para los sensores del CanSat.
    namespace I2C_Channel
    {
        constexpr uint8_t GY91 = 0;  /// Canal del multiplexor I2C para el sensor GY-91.
        constexpr uint8_t BME680 = 1; /// Canal del multiplexor I2C para el sensor BME680.
        constexpr uint8_t ENS160 = 2; /// Canal del multiplexor I2C para el sensor ENS160.
        constexpr uint8_t INA219 = 3; /// Canal del multiplexor I2C para el sensor INA219.
    }

    namespace ANALOG_MUX
    {
        constexpr uint8_t MISC_CO = 0;  /// Canal del multiplexor analógico para el sensor de CO.
        constexpr uint8_t MISC_NO2 = 1; /// Canal del multiplexor analógico para el sensor de NO2.
        constexpr uint8_t MISC_NH3 = 2;  /// Canal del multiplexor analógico para el sensor de gas NH3.
    }
    /// Definición de pines para la comunicación LoRa y GPS.
    namespace RA02
    {
        constexpr uint8_t NSS = 13;
        constexpr uint8_t SCK = 19;
        constexpr uint8_t MOSI = 20;
        constexpr uint8_t MISO = 18;
        constexpr uint8_t RST = 15;
        constexpr uint8_t DIO0 = 2;
        constexpr long FREQUENCY = 433E6; /// Frecuencia de comunicación LoRa.
    }

    /// Definición de pines para la comunicación LoRa y GPS.
    namespace GPS
    {
        constexpr uint8_t RX = 16;
        constexpr uint8_t TX = 17;
    }
}
#endif // DEFAULT_H