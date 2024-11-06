let myBLE; // BLE instance to handle Bluetooth connection
let myCharacteristic; // BLE characteristic for data communication
let isConnected = false; // Tracks if BLE is connected
let startTime; // Records the start time of the timer
let timeDiff = 0; // Stores the elapsed time
let isRunning = false; // Tracks if the timer is running
const goalTime = 10; // Set the target time to reach 10 seconds
let message = ""; // Message to display win/loss status

// UUID for the BLE service and characteristic
const serviceUuid = "242b8f43-9b57-4268-94e3-ff2c618acdc2";

function setup() {
  // Setup the canvas and text formatting
  createCanvas(400, 400);
  textSize(24);
  textAlign(CENTER, CENTER);
  fill(255);

  // Initialize the BLE instance
  myBLE = new p5ble();

  // Create a "Connect" button for initiating the BLE connection
  const connectButton = createButton('Connect');
  connectButton.mousePressed(connectToBLE);
}

function draw() {
  // Clear the background and set up display elements
  background(0);
  
  // Display the goal time for the user at the top of the screen
  text("Goal: " + goalTime + " seconds", width / 2, 40);

  // Check if the timer is currently running
  if (isRunning) {
    // Calculate the elapsed time since the timer started
    let currentTime = millis();
    timeDiff = (currentTime - startTime) / 1000;

    // Display the running timer in the center of the screen
    text(timeDiff.toFixed(3) + " seconds", width / 2, height / 2);
  } else {
    // If the timer is stopped, display the final time and win/loss message
    if (timeDiff > 0) {
      text("Stopped at: " + timeDiff.toFixed(3) + " seconds", width / 2, height / 2 - 20);
      text(message, width / 2, height / 2 + 40);

      // Prompt to try again, showing at the bottom of the screen
      text("Press button on ESP32 to try again", width / 2, height - 40);
    } else {
      // Initial message to prompt the user to start the timer
      text("Press the button on ESP32 to start", width / 2, height / 2);
    }
  }
}

function connectToBLE() {
  // Connect to the BLE device if not already connected
  if (!isConnected) {
    console.log("Attempting to connect...");
    myBLE.connect(serviceUuid, gotCharacteristics, onDisconnected);
  } else {
    console.log("Already connected.");
  }
}

function gotCharacteristics(error, characteristics) {
  // Handle BLE connection and notification setup
  if (error) {
    console.error("Error: ", error);
    return;
  }

  // Store the characteristic to allow data exchange
  myCharacteristic = characteristics[0];

  // Start listening for notifications from the ESP32
  myBLE.startNotifications(myCharacteristic, gotValue, "string");
  isConnected = true;
  console.log("Connected and notifications started");
}

function onDisconnected() {
  // Handle device disconnection
  console.log("Device got disconnected.");
  isConnected = false;
}

function gotValue(value) {
  // Handle incoming BLE data from ESP32
  if (value === "start") {
    // When the timer starts
    timeDiff = 0; // Reset the time difference
    isRunning = true; // Set timer to running
    startTime = millis(); // Record start time
    message = ""; // Clear any previous message
    console.log("Timer started");
  } else {
    // When the timer stops, process the elapsed time
    timeDiff = parseFloat(value); // Convert received value to a number
    isRunning = false; // Stop timer display

    // Determine win/loss status based on the final time
    if (timeDiff <= goalTime) {
      if (goalTime - timeDiff <= 0.5) { 
        message = "You Win!"; // Win if within 0.5 seconds below the goal
      } else {
        message = "You Lose! Try Again."; // Lose if too far below the goal
      }
    } else {
      message = "You Lose! Try Again."; // Lose if time exceeds goal
    }

    console.log("Timer stopped at: " + timeDiff + " seconds");
  }
}
