#include <Arduino.h>

// Pin Definitions
const int HOPPER_FORWARD_PIN = 22;  // Rack and pinion forward
const int HOPPER_BACKWARD_PIN = 24; // Rack and pinion backward
const int IRIS_OPEN_PIN = 28;       // Iris mechanism open
const int IRIS_CLOSE_PIN = 26;      // Iris mechanism close
const int BASE_MOTOR_PIN = 30;      // Base rotation motor
const int ROLLER_UP_PIN = 34;       // Roller DC motor up
const int ROLLER_DOWN_PIN = 32;     // Roller DC motor down

// Constants
const String REFILL_COMMAND = "refill";
const String START_COMMAND = "start";
const String STOP_COMMAND = "stop";

// Timing constants for 3.5RPM DC motor
const int ROLLER_MOVEMENT_TIME = 3000;  // Time for roller to move up/down (adjust based on testing)
const int REFILL_MOVEMENT_TIME = 9000;  // Time for hopper to move up during refill (3 seconds)

// Global Variables
bool isProcessing = false;
bool systemRunning = false;

void moveRollerUp() {
  Serial.println("Moving roller up");
  // First ensure roller is stopped
  digitalWrite(ROLLER_UP_PIN, HIGH);
  digitalWrite(ROLLER_DOWN_PIN, HIGH);
  delay(500);  // Wait for motor to stop completely
  
  // Move up
  digitalWrite(ROLLER_DOWN_PIN, HIGH);  // Ensure other direction is off
  digitalWrite(ROLLER_UP_PIN, LOW);     // Activate up movement
  delay(ROLLER_MOVEMENT_TIME);
  digitalWrite(ROLLER_UP_PIN, HIGH);    // Stop movement
  delay(500);        // Allow for any momentum to settle
  Serial.println("Roller in up position");
}

void moveRollerDown() {
  Serial.println("Moving roller down");
  // First ensure roller is stopped
  digitalWrite(ROLLER_UP_PIN, HIGH);
  digitalWrite(ROLLER_DOWN_PIN, HIGH);
  delay(500);  // Wait for motor to stop completely
  
  // Move down
  digitalWrite(ROLLER_UP_PIN, HIGH);    // Ensure other direction is off
  digitalWrite(ROLLER_DOWN_PIN, LOW);   // Activate down movement
  delay(ROLLER_MOVEMENT_TIME);
  digitalWrite(ROLLER_DOWN_PIN, HIGH);  // Stop movement
  delay(500);        // Allow for any momentum to settle
  Serial.println("Roller in down position");
}

void initializeSystem() {
  // Initialize all relays to OFF
  digitalWrite(HOPPER_FORWARD_PIN, HIGH);
  digitalWrite(HOPPER_BACKWARD_PIN, HIGH);
  digitalWrite(IRIS_OPEN_PIN, HIGH);
  digitalWrite(IRIS_CLOSE_PIN, HIGH);
  digitalWrite(BASE_MOTOR_PIN, HIGH);
  digitalWrite(ROLLER_UP_PIN, HIGH);
  digitalWrite(ROLLER_DOWN_PIN, HIGH);
  
  Serial.println("System initialized");
}

void setup() {
  Serial.begin(9600);
  
  pinMode(HOPPER_FORWARD_PIN, OUTPUT);
  pinMode(HOPPER_BACKWARD_PIN, OUTPUT);
  pinMode(IRIS_OPEN_PIN, OUTPUT);
  pinMode(IRIS_CLOSE_PIN, OUTPUT);
  pinMode(BASE_MOTOR_PIN, OUTPUT);
  pinMode(ROLLER_UP_PIN, OUTPUT);
  pinMode(ROLLER_DOWN_PIN, OUTPUT);
  
  initializeSystem();
  Serial.println("System ready - Send 'start' to begin cycle");
}

void extrudeDough() {
  Serial.println("Extruding dough");
  digitalWrite(HOPPER_FORWARD_PIN, LOW);
  delay(3000);  // Adjust time as needed
  digitalWrite(HOPPER_FORWARD_PIN, HIGH);
}

void cutDough() {
  Serial.println("Cutting dough");
  // Close iris
  digitalWrite(IRIS_CLOSE_PIN, LOW);
  delay(1200);
  digitalWrite(IRIS_CLOSE_PIN, HIGH);
  
  delay(1100);
  
  // Open iris
  digitalWrite(IRIS_OPEN_PIN, LOW);
  delay(1200);
  digitalWrite(IRIS_OPEN_PIN, HIGH);
}

void flattenDough() {
  Serial.println("Flattening dough");
  
  // Lower roller
  moveRollerDown();
  
  // Start base rotation
  digitalWrite(BASE_MOTOR_PIN, LOW);
  delay(5000);  // Allow time for base rotation
  
  // Stop base rotation
  digitalWrite(BASE_MOTOR_PIN, HIGH);
  
  // Raise roller
  moveRollerUp();
  
  Serial.println("Flattening complete");
}

void refillHopper() {
  systemRunning = false;  // Stop any ongoing operations
  Serial.println("Moving hopper to refill position");
  digitalWrite(HOPPER_BACKWARD_PIN, LOW);  // Move hopper up
  delay(REFILL_MOVEMENT_TIME);            // Run for specified time
  digitalWrite(HOPPER_BACKWARD_PIN, HIGH); // Stop hopper movement
  Serial.println("Hopper in refill position. Add dough and send 'start' when ready.");
}

void checkSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove any whitespace
    
    if (command.equals("refill")) {
      if (!systemRunning) {
        refillHopper();
      } else {
        Serial.println("Cannot refill while system is running");
      }
    }
    else if (command.equals("start")) {
      if (!systemRunning) {
        Serial.println("Starting system...");
        initializeSystem();
        moveRollerUp();
        systemRunning = true;
      } else {
        Serial.println("System already running. Wait for cycle to complete");
      }
    }
    else if (command.equals("stop")) {
      systemRunning = false;
      // Stop all motors
      digitalWrite(HOPPER_FORWARD_PIN, HIGH);
      digitalWrite(HOPPER_BACKWARD_PIN, HIGH);
      digitalWrite(IRIS_OPEN_PIN, HIGH);
      digitalWrite(IRIS_CLOSE_PIN, HIGH);
      digitalWrite(BASE_MOTOR_PIN, HIGH);
      digitalWrite(ROLLER_UP_PIN, HIGH);
      digitalWrite(ROLLER_DOWN_PIN, HIGH);
      
      moveRollerDown();
      Serial.println("System stopped");
    }
  }
}

void loop() {
  checkSerialCommands();  // Check for serial commands first
  
  if (systemRunning) {
    // Execute one complete cycle
    extrudeDough();
    delay(1000);
    
    cutDough();
    delay(1000);
    
    flattenDough();
    
    // Wait after flattening
    Serial.println("Waiting for 5 seconds...");
    delay(5000);
    
    // Return to home position
    moveRollerDown();
    
    systemRunning = false;  // Stop system until next start command
    Serial.println("Cycle complete - Send 'start' to begin next cycle");
  }
}