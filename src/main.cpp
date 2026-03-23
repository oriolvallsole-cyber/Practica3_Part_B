#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {

    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
      Serial.print("Recibido: ");
      Serial.println(value.c_str());
    }
  }
};

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Dispositivo conectado");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Dispositivo desconectado");
    BLEDevice::startAdvertising(); // 🔥 importante
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