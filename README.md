# CanSat Tepochtli

<div align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32%20%7C%20ESP32-C6-1d4ed8" alt="Platform" />
  <img src="https://img.shields.io/badge/Protocol-LoRa%20433MHz-22c55e" alt="LoRa" />
  <img src="https://img.shields.io/badge/Stack-Arduino%20%2F%20PlatformIO-f59e0b" alt="Stack" />
  <img src="https://img.shields.io/badge/Status-Flight%20Telemetry-informational" alt="Status" />
</div>

## Descripción técnica

El proyecto implementa una arquitectura de telemetría para CanSat compuesta por dos nodos:

- Nodo emisor: integrado a bordo del CanSat, adquiere mediciones de sensores y transmite datos por LoRa.
- Nodo receptor: instalado en la estación terrestre, recibe paquetes radio y los expone en puerto serial para monitoreo y análisis.

La solución está desarrollada en Arduino con PlatformIO y utiliza multiplexación I2C/analógica para gestionar varios sensores sobre un mismo bus eléctrico y lógico.

---

## Arquitectura del sistema

### 1. Emisor embarcado

El módulo emisor se ejecuta sobre una ESP32-C6 y realiza las siguientes funciones:

- Inicialización de UART y bus I2C.
- Configuración de multiplexores para selección de canales.
- Adquisición de parámetros ambientales, inerciales y de potencia.
- Serialización de la estructura `SensorData`.
- Transmisión de paquetes LoRa a 433 MHz.

#### Sensores y periféricos

| Componente | Función | Interfaz |
|---|---|---|
| MPU9250 | Aceleración, giroscopio y magnetómetro | I2C |
| BME680 | Temperatura, humedad y resistencia de gas | I2C |
| ENS160 | Calidad del aire, eCO2, TVOC | I2C |
| INA219 | Voltaje, corriente y potencia | I2C |
| MICS-6814 | CO, NO2, NH3 | Analógica multiplexada |
| RA02 LoRa | Telemetría por radio | SPI + LoRa |
| GPS | Puntos de conexión previstos para navegación | UART |

#### Lógica de operación

El flujo principal se gestiona en `CanSat_Emisor/src/main.cpp`:

- `setup()` inicializa:
  - Serial a 115200 baudios
  - I2C con pines `SDA = 21`, `SCL = 22`
  - sensores mediante `SensorManager::initSensors()`
  - módulo LoRa mediante `LoRaManager::begin()`

- `loop()` ejecuta una lógica basada en temporizadores:
  - `FAST_INTERVAL = 1000 ms`: actualización de sensores
  - `SLOW_INTERVAL = 5000 ms`: envío de telemetría

El paquete transmitido corresponde a la estructura `SensorData` definida en `structures.h`:

- `imu`: aceleración, giroscopio, magnetómetro y campos de presión/altitud
- `env`: temperatura, humedad, gases y calidad del aire
- `power`: voltaje, corriente y potencia
- `timestamp`: marca de tiempo en milisegundos

---

### 2. Receptor terrestre

El receptor usa una placa compatible con ESP32-C6 y configura la radio LoRa con parámetros coherentes con el emisor. El proceso implementado incluye:

- inicialización del bus SPI,
- configuración de pines del módulo LoRa,
- llamada a `LoRa.begin(433E6)`,
- ajuste del spreading factor, bandwidth y coding rate,
- recepción del paquete, validación de tamaño y despliegue serial.

La recepción se realiza en `CanSat_Receptor/src/main.cpp`, donde se valida que `packetSize == sizeof(SensorData)` antes de leer el contenido del buffer en memoria.

---

## Estructura del repositorio

```text
CanSat_Tepochtli/
├── README.md
├── CanSat_Emisor/
│   ├── platformio.ini
│   ├── include/
│   │   ├── LoRa_manager.h
│   │   ├── mux_driver.h
│   │   ├── pines.h
│   │   ├── sensor_manager.h
│   │   └── structures.h
│   ├── src/
│   │   └── main.cpp
│   └── test/
│       └── README
├── CanSat_Receptor/
│   ├── platformio.ini
│   ├── include/
│   │   └── structures.h
│   ├── src/
│   │   └── main.cpp
│   └── test/
│       └── README
└──
```

---

## Módulos principales

### Emisor

- `CanSat_Emisor/include/pines.h`: definición de pines para sensores, multiplexores, GPS y LoRa.
- `CanSat_Emisor/include/mux_driver.h`: controlador de multiplexores TCA9548A y CD74HC4067M.
- `CanSat_Emisor/include/sensor_manager.h`: encapsula la inicialización y lectura de sensores.
- `CanSat_Emisor/include/LoRa_manager.h`: gestión del PHY LoRa y envío de paquetes.
- `CanSat_Emisor/include/structures.h`: modelado de telemetría del sistema.
- `CanSat_Emisor/src/main.cpp`: punto de entrada principal del nodo emisor.

### Receptor

- `CanSat_Receptor/src/main.cpp`: recepción y parseo de paquetes radio.
- `CanSat_Receptor/include/structures.h`: estructura de datos utilizada en el lado receptor.

---

## Dependencias de compilación

La configuración de PlatformIO del emisor incluye:

- `adafruit/Adafruit Unified Sensor`
- `adafruit/Adafruit BME680 Library`
- `LoRa`
- `sciosense/ScioSense_ENS16x`
- `hideakitai/MPU9250`
- `adafruit/Adafruit INA219`
- `adafruit/ENS160 - Adafruit Fork`

El receptor incluye:

- `sandeepmistry/LoRa`

---

## Configuración de hardware

### Nodo emisor

| Parámetro | Valor |
|---|---|
| Microcontrolador | ESP32-C6 |
| Bus I2C | TCA9548A |
| Mux analógico | CD74HC4067M |
| Frecuencia LoRa | 433 MHz |
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| LoRa NSS | GPIO 13 |
| LoRa SCK | GPIO 19 |
| LoRa MOSI | GPIO 20 |
| LoRa MISO | GPIO 18 |
| LoRa RST | GPIO 15 |
| LoRa DIO0 | GPIO 2 |

### Nodo receptor

| Parámetro | Valor |
|---|---|
| Microcontrolador | Seeed XIAO ESP32C6 |
| Frecuencia LoRa | 433 MHz |
| NSS | D1 |
| RESET | D2 |
| DIO0 | D3 |
| SPI_SCK | D8 |
| SPI_MISO | D9 |
| SPI_MOSI | D10 |

---

## Flujo de operación

1. El emisor inicializa el puerto serial, el bus I2C y el módulo LoRa.
2. Se seleccionan los canales de sensores mediante multiplexación.
3. Se actualizan valores del IMU, ambiente y consumo energético cada 1 s.
4. Cada 5 s se empaqueta la telemetría en `SensorData` y se transmite.
5. El receptor valida el tamaño del paquete y despliega los datos por serial.

---

## Compilación y carga

### Emisor

```bash
cd CanSat_Tepochtli/CanSat_Emisor
pio run
pio run --target upload
```

### Receptor

```bash
cd CanSat_Tepochtli/CanSat_Receptor
pio run
pio run --target upload
```

---

## Observaciones técnicas

La implementación actual corresponde a una base funcional de telemetría CanSat con las siguientes capacidades:

- adquisición multi-sensor,
- multiplexación de periféricos,
- serialización compacta de datos,
- comunicación inalámbrica vía LoRa,
- recepción y monitoreo en estación terrestre.

El sistema está preparado para validación experimental, depuración de hardware y extensión funcional.

---

## Autor

Proyecto desarrollado por el equipo de programación del CanSat Tepochtli.
