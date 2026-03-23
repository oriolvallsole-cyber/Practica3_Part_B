# Práctica 3 – WiFi y Bluetooth con ESP32

## Parte B – Comunicación Bluetooth (BLE)

---

# Descripción

En esta parte de la práctica se implementa una **comunicación inalámbrica mediante Bluetooth** entre una ESP32 y un dispositivo móvil.

Debido a que la placa utilizada es una **ESP32-S3**, que no dispone de Bluetooth clásico, se ha utilizado **Bluetooth Low Energy (BLE)** para realizar la comunicación.

El sistema permite que un dispositivo móvil envíe datos a la ESP32, los cuales se muestran por el **monitor serie**.

---

# Objetivo

* Establecer una conexión Bluetooth entre la ESP32 y un móvil
* Recibir datos enviados desde el móvil
* Mostrar los datos recibidos en el monitor serie

---

# Tecnologías utilizadas

* ESP32-S3
* Bluetooth Low Energy (BLE)
* Framework Arduino
* PlatformIO
* Monitor Serie

---

# Funcionamiento del sistema

1. La ESP32 inicia el módulo BLE con el nombre:

```
ESP32_BLE
```

2. Se crea un servicio y una característica BLE.

3. El dispositivo móvil se conecta a la ESP32.

4. Cuando el usuario envía datos desde el móvil:

   * La ESP32 los recibe
   * Los muestra en el monitor serie

---

# Código principal

```cpp
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;

// Callback cuando se reciben datos
class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {

    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      Serial.print("Recibido: ");
      Serial.println(value.c_str());
    }
  }
};

// Callback de conexión
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Dispositivo conectado");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Dispositivo desconectado");
    BLEDevice::startAdvertising();
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando BLE...");

  BLEDevice::init("ESP32_BLE");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService("1234");

  pCharacteristic = pService->createCharacteristic(
                      "5678",
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("1234");
  BLEDevice::startAdvertising();

  Serial.println("BLE listo");
}

void loop() {
}
```

---

# Configuración de PlatformIO

Archivo `platformio.ini`:

```ini
[env:esp32-s3-devkitm-1]
platform = espressif32
board = esp32-s3-devkitm-1
framework = arduino
monitor_speed = 115200
```

---

# Ejecución

## 1. Subir el programa

Compilar y cargar el código en la ESP32 desde PlatformIO.

## 2. Abrir el monitor serie

Velocidad:

```
115200
```

Salida esperada:

```
Iniciando BLE...
BLE listo
```

---

## 3. Conexión desde el móvil

Aplicaciones recomendadas:

* nRF Connect
* LightBlue
* Serial BLE Terminal

Pasos:

1. Buscar dispositivos BLE
2. Seleccionar:

```
ESP32_BLE
```

3. Conectarse
4. Enviar datos

---

# Resultado

Cuando se envían datos desde el móvil, en el monitor serie se obtiene:

```
Recibido: hola
```

---

# Conclusión

Se ha conseguido establecer una comunicación Bluetooth entre la ESP32 y un dispositivo móvil utilizando **BLE**.

Este método permite la transmisión de datos de forma inalámbrica, cumpliendo el objetivo de la práctica.

---

# Autor

-Oriol Vall Solé
