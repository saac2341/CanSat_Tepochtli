#include <Arduino.h>
#include "pines.h"
#include "sensor_manager.h"
#include "LoRa_manager.h"
#include "structures.h"
#include <Wire.h>

/// @brief Objeto para la gestión de sensores.
SensorManager sensors;    // Objeto para la gestión de sensores.
LoRaManager radio;        // Objeto para la gestión de la comunicación LoRa.
SensorData telemetryData; // Estructura para almacenar los datos de los sensores.

/// Configuracion de temporizadores para la lectura de sensores y el envío de telemetría.
unsigned long lastFastTask = 0; // Última vez que se ejecutó la tarea rápida (lectura de sensores).
unsigned long lastSlowTask = 0; // Última vez que se ejecutó la tarea lenta (envío de telemetría).

const unsigned long FAST_INTEVALO = 1000; // Intervalo de tiempo para la tarea rápida (lectura de sensores) en milisegundos.
const unsigned long SLOW_INTEVALO = 5000; // Intervalo de tiempo para la tarea lenta (envío de telemetría) en milisegundos.

/// Comfiguracion de la programa principal.
void setup()
{
  Serial.begin(115200); // Inicializa la comunicación serial a 115200 baudios

  // Inicializacion de la comunicaicon I2c
  Wire.begin(
      PINOUT::I2C_MUX::MUX_I2C_SDA, // Pin SDA
      PINOUT::I2C_MUX::MUX_I2C_SCL  // Pin SCL
  );

  Serial.println("Telemetria de satelite CanSat encendida ...");
  Serial.println("============================================");

  if (!sensors.initSensors())
  { // Inicializa los sensores y verifica si todos están conectados y funcionando correctamente.
    Serial.println("Error: No se pudieron inicializar todos los sensores.");
  }
  else
  {
    Serial.println("[OK] Sensores inicializados correctamente.");
  }

  if (!radio.begin())
  { // Inicializa la comunicación LoRa y verifica si se pudo inicializar correctamente.
    Serial.println("Error: No se pudo inicializar la comunicación LoRa.");
  }
  else
  {
    Serial.println("[OK] Comunicación LoRa inicializada correctamente a 433MHz.");
  }
}

/// @brief Bucle principal del programa.
void loop()
{
  unsigned long currentMilis = millis(); // Obtiene el tiempo actual en milisegundos desde que se encendió el microcontrolador.

  // Lectura de los sensores desde el inicio.
  if (currentMilis - lastFastTask >= FAST_INTEVALO)
  {
    lastFastTask = currentMilis;      // Actualiza el tiempo de la última tarea rápida.
    sensors.updateALL(telemetryData); // Actualiza los datos de todos los sensores.

    // Transmision de los datos por LoRa.
    bool txSuccess = radio.sendTelemetry(telemetryData); // Envía los datos de telemetría a través de LoRa.

    /// Impresion de los datos de telemetría en la consola serial.
    Serial.printf("[%lu ms] TX LoRa: %s | Tamaño: %u bytes | ",
                  telemetryData.timestamp, txSuccess ? "OK" : "Error", sizeof(telemetryData));

    Serial.printf(" -> AccZ: %.2f | Temp: %.2f | CO: %.2f | NO2: %.2f\n",
                  telemetryData.imu.accel_z,
                  telemetryData.env.temperature,
                  telemetryData.env.gas_co,
                  telemetryData.env.gas_no2);
  }
}