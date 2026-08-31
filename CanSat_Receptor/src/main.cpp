/** 
 * @file main.cpp
 * @brief Main file for the CanSat Receptor project
 * @author Your Name
 * @date 2023-04-01
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include "structures.h"

//Configuracion de los pines para el receptor LoRa
namespace ReceptorPINS {
constexpr uint8_t NSS = D1; // Pin de selección de chip (NSS) para LoRa
constexpr uint8_t RESET = D2; // Pin de reinicio para LoRa
constexpr uint8_t DIO0 = D3; // Pin de interrupción para LoRa
constexpr uint8_t SPI_SCK = D8; // Pin de reloj SPI
constexpr uint8_t SPI_MISO = D9; // Pin de entrada de datos SPI
constexpr uint8_t SPI_MOSI = D10; // Pin de salida de datos SPI
constexpr long FREQUENCY = 433E6; // Frecuencia de operación de LoRa (433 MHz)
}

SensorData rxTelemetry; // Estructura para almacenar los datos recibidos

void setup () {

  Serial.begin(115200);
  while (!Serial && millis() < 5000); // Espera a que el puerto serie esté listo

  //Configuracion del bus SPI para LoRa
  SPI.begin (
    ReceptorPINS :: SPI_SCK,
    ReceptorPINS :: SPI_MISO,
    ReceptorPINS :: SPI_MOSI,
    ReceptorPINS :: NSS
  );

  LoRa.setPins(ReceptorPINS::NSS, ReceptorPINS::RESET, ReceptorPINS::DIO0); // Configura los pines de LoRa

  if (!LoRa.begin(ReceptorPINS::FREQUENCY)) {
    Serial.println("Error al inicializar LoRa");
    while (1);
  }

  //Configuracion a la del emisor para que los datos se reciban correctamente
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
}

void loop () {
  int packetSize = LoRa.parsePacket ();

  //Validar el paquete recibido.
  if (packetSize == sizeof (SensorData)){
    
  /// Leer el buffer binario entrante.
  LoRa.readBytes((uint8_t*) & rxTelemetry, sizeof (SensorData));

  Serial.printf("%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
    rxTelemetry.timestamp,
    rxTelemetry.imu.accel_x, rxTelemetry.imu.accel_y, rxTelemetry.imu.accel_z,
    rxTelemetry.imu.gyro_x, rxTelemetry.imu.gyro_y, rxTelemetry.imu.gyro_z,
    rxTelemetry.imu.mag_x, rxTelemetry.imu.mag_y, rxTelemetry.imu.mag_z,
    rxTelemetry.imu.pressure, rxTelemetry.imu.altitude,
    rxTelemetry.env.temperature, rxTelemetry.env.humidity, rxTelemetry.env.gas_res_bme,
    rxTelemetry.power.bus_voltage, rxTelemetry.power.current_mA, rxTelemetry.power.power_mW
  );
  }
}