/*Controlador de la comunicación LoRa*/

#ifndef LORA_MANAGER_H
#define LORA_MANAGER_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "structures.h"
#include "pines.h"

/// Definición de la clase LoRaManager para gestionar la comunicación LoRa.
class LoRaManager {
    public:
        LoRaManager() {} // Constructor de la clase LoRaManager.

    bool begin() {
        // Inicializa la comunicación SPI y LoRa con los pines definidos en pines.h.
        SPI.begin(
            PINOUT::RA02::SCK,  // SCK
            PINOUT::RA02::MISO, // MISO
            PINOUT::RA02::MOSI, // MOSI
            PINOUT::RA02::NSS   // NSS
        );
        // Configura los pines de LoRa y la frecuencia de comunicación.
        LoRa.setPins(
            PINOUT::RA02::NSS,  // NSS
            PINOUT::RA02::RST,  // RST
            PINOUT::RA02::DIO0  // DIO0
        );
        if (!LoRa.begin(PINOUT::RA02::FREQUENCY)) {
            return false; // Si la inicialización de LoRa falla, retorna false.
        }

        // Paraetros de configuracion de telemetria LoRa.
        LoRa.setTxPower(20); // Establece el poder de transmisión a 14 dBm.
        LoRa.setSpreadingFactor(7); // Establece el factor de propagación a 7.
        LoRa.setSignalBandwidth(125E3); // Establece el ancho de banda de señal a 125 kHz.
        LoRa.setCodingRate4(5); // Establece la tasa de codificación a 4/5.
        return true; // Si la inicialización de LoRa es exitosa, retorna true.
    }
    /// @brief Envía los datos de telemetría a través de LoRa.
    bool sendTelemetry(const SensorData &data) {
        // Inicia un nuevo paquete LoRa.
        if(LoRa.beginPacket ()) {
            return false; // Si no se puede iniciar un paquete, retorna false.
        }
        LoRa.write((const uint8_t *)&data, sizeof(SensorData)); // Escribe los datos de telemetría en el paquete.
        return LoRa.endPacket(); // Finaliza el paquete y lo envía. Retorna true si el envío es exitoso, false si falla.
    }
};

#endif // LORA_MANAGER_H