#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// Define UUIDs for the BLE service and characteristic
#define SERVICE_UUID "242b8f43-9b57-4268-94e3-ff2c618acdc2"
#define CHARACTERISTIC_UUID "5fb35310-9b56-4ddb-97de-9cfc615c0f92"

// Define the GPIO pin for the button
const int buttonPin = 12; 

// Variables to track the timer's state and button press status
bool timerRunning = false; // Tracks whether the timer is currently running
unsigned long startTime = 0; // Start time of the timer
unsigned long endTime = 0;   // End time of the timer
bool buttonPressed = false; // Tracks the button press state to debounce and prevent multiple presses

// Declare BLE characteristic pointer
BLECharacteristic *pCharacteristic;

// BLE server callback class for connection events
class MyServerCallbacks: public BLEServerCallbacks {
  // Called when a device connects
  void onConnect(BLEServer* pServer) {
    Serial.println("Device connected");
  }

  // Called when a device disconnects
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Device disconnected");
    BLEDevice::startAdvertising(); // Restart advertising to allow reconnections
  }
};

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Configure the button pin as input with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize BLE and set the device name
  BLEDevice::init("Stop10 Controller");

  // Create a BLE server and set the connection callback class
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create a BLE service with the specified UUID
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE characteristic with read and notify properties
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                    );

  // Add a descriptor to allow notifications on this characteristic
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising the BLE service to allow devices to find it
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  Serial.println("BLE advertising started");
}

void loop() {
  // Read the current state of the button (LOW when pressed, HIGH when released)
  int buttonState = digitalRead(buttonPin);

  // Check for a button press event (button goes from HIGH to LOW)
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true; // Mark the button as pressed to prevent multiple triggers
    delay(50); // Small debounce delay to avoid false triggering

    // If the timer is not already running, start it
    if (!timerRunning) {
      startTime = millis(); // Capture the current time as the start time
      timerRunning = true;  // Mark the timer as running
      Serial.println("Timer started");

      // Notify the p5.js client that the timer has started
      pCharacteristic->setValue("start"); // Send "start" as the notification message
      pCharacteristic->notify();
    } else { 
      // If the timer is already running, stop it
      endTime = millis(); // Capture the current time as the end time
      timerRunning = false; // Mark the timer as stopped

      // Calculate the elapsed time in seconds
      float elapsedTime = (endTime - startTime) / 1000.0;

      // Convert the elapsed time to a string with 3 decimal places
      String timeString = String(elapsedTime, 3);

      // Notify the p5.js client with the elapsed time
      pCharacteristic->setValue(timeString.c_str()); // Send the elapsed time
      pCharacteristic->notify();
      Serial.println("Timer stopped: " + timeString + " seconds");
    }
  } 
  // Check for a button release event (button goes from LOW to HIGH)
  else if (buttonState == HIGH && buttonPressed) {
    buttonPressed = false; // Reset button state to allow next press
    delay(50); // Additional debounce delay
  }
}
