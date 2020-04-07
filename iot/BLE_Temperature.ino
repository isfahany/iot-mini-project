#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <DHT.h>

#define TemperatureService BLEUUID((uint16_t)0x181A) 
BLECharacteristic HumidityCharacteristic(BLEUUID((uint16_t)0x2A6F), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY); 
BLECharacteristic TemperatureCharacteristic(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor TemperatureDescriptor(BLEUUID((uint16_t)0x2901))
BLEDescriptor HumidityDescriptor(BLEUUID((uint16_t)0x2901))

bool clientConnected = false;

#define DHTTYPE    DHT11     // DHT 11

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
  BLEService *pTemperature = pServer->createService(TemperatureService);

  pTemperature->addCharacteristic(&TemperatureCharacteristic);
  pTemperature->addCharacteristic(&HumidityCharacteristic);
  TemperatureDescriptor.setValue("Temperature -40-60°C");
  HumidityDescriptor.setValue("Humidity 0 to 100%");
  TemperatureCharacteristic.addDescriptor(&TemperatureDescriptor);
  HumidityCharacteristic.addDescriptor(&HumidityDescriptor);
  HumidityCharacteristic.addDescriptor(new BLE2902());
  TemperatureCharacteristic.addDescriptor(new BLE2902());;

  pServer->getAdvertising()->addServiceUUID(TemperatureService);

  pBattery->start();
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
     uint8_t level = 57;
     BatteryLevelCharacteristic.setValue(&level, 1);
     BatteryLevelCharacteristic.notify();
     delay(5000);

     level++; 
     if (int(level)>=100){
      level=100;
     }
     Serial.println(int(level));
  }
}
