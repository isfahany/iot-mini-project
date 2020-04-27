#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <DHT.h>

#define Env_Service_UUID        "0000181A-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID     "00002A1C-0000-1000-8000-00805F9B34FB"

float temperature = 0.0;
BLECharacteristic TemperatureCharacteristic(BLEUUID(CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

bool clientConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect (BLEServer * pServer) {
    clientConnected = true;
  };
  void onDisconnect (BLEServer * pServer) {
  clientConnected = false;
  }
};

void InitBLE() {
  BLEDevice::init("BLE Temperature");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pTemperature = pServer->createService(Env_Service_UUID);

  TemperatureCharacteristic.addDescriptor(new BLE2902());

  pServer->getAdvertising()->addServiceUUID(Env_Service_UUID);

  pTemperature->start();
  // Start advertising
  pServer->getAdvertising()->start();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Temperature Indicator - BLE");
  InitBLE();
}

void loop () {
  if (clientConnected) {
     uint8_t buf[6];
     buf[0] = 0x06;
     temperature = random(240, 270) / 10.0;
     buf[1] = temperature;
     TemperatureCharacteristic.setValue(buf, sizeof(buf));
     TemperatureCharacteristic.notify();
     Serial.println(temperature);
     delay(50000);
  }
}
