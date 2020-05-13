#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

#define Service_UUID        "0000180F-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "00002A19-0000-1000-8000-00805F9B34FB"
                    
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
  Serial.begin(115200);
  BLEDevice::init("BLE Battery");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pBattery = pServer->createService(Service_UUID);

  pCharacteristic = pBattery->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                    
  pCharacteristic->addDescriptor(new BLE2902());

  pBattery->start();
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(Service_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Battery Level Indicator - BLE");
  InitBLE();
}

uint8_t level = 57;

void loop () {
  if (clientConnected) {
     pCharacteristic->setValue(&level, 1);
     pCharacteristic->notify();
     delay(5000);

     level++; 
     if (int(level)>=100){
      level=100;
     }
     Serial.println(int(level));
  }
}
