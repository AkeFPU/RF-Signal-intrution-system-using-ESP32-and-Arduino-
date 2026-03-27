#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

// --- PIN DEFINITIONS ---
#define CE1_PIN 9
#define CSN1_PIN 10
#define CE2_PIN 7
#define CSN2_PIN 6
#define ALERT_LED_PIN 8   // The Red "Threat" LED
#define SAFE_LED_PIN 5    // The White "System Safe" LED
#define BUTTON_PIN 4      // The Manual Demo/Panic Button

// --- HARDWARE INITIALIZATION ---
RF24 radio1(CE1_PIN, CSN1_PIN);
RF24 radio2(CE2_PIN, CSN2_PIN);
SoftwareSerial espSerial(2, 3); 

// --- ALERT TIMERS & SENSITIVITY ---
unsigned long alertTimer = 0;
bool isAlertActive = false;
// Note: JAMMER_THRESHOLD is gone! We now use the dynamic dial in the loop.

// --- BLINKING VARIABLES ---
unsigned long blinkTimer = 0;
bool alertLedState = false;
const int BLINK_INTERVAL = 80; // Fast strobe effect for Red LED

void setup() {
  Serial.begin(115200);   
  espSerial.begin(9600);  
  
  // Setup the LEDs and Button
  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(SAFE_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use Arduino's internal resistor for the switch
  
  // Default State: System is Safe
  digitalWrite(ALERT_LED_PIN, LOW);
  digitalWrite(SAFE_LED_PIN, HIGH); 

  // Initialize Radios
  radio1.begin();
  radio1.setAutoAck(false);
  radio1.setPALevel(RF24_PA_MAX);
  
  radio2.begin();
  radio2.setAutoAck(false);
  radio2.setPALevel(RF24_PA_MAX);
}

void loop() {
  int hitCount = 0; 

  // --- DYNAMIC SENSITIVITY DIAL ---
  // Read the 10k potentiometer on Analog Pin A0 (0 to 1023)
  int potValue = analogRead(A0); 
  // Map the dial to require between 2 and 40 channel hits to trigger
  int dynamicThreshold = map(potValue, 0, 1023, 2, 40); 

  // --- SWEEP LOWER BAND ---
  for (int i = 0; i < 64; i++) {
    radio1.setChannel(i);
    radio1.startListening();
    delayMicroseconds(200); 
    bool carrier = radio1.testCarrier();
    radio1.stopListening();
    
    if (carrier) {
      Serial.print(i); Serial.println(",-40"); 
      hitCount++;
    } else {
      Serial.print(i); Serial.println(",-105");
    }
  }

  // --- SWEEP UPPER BAND ---
  for (int i = 64; i < 128; i++) {
    radio2.setChannel(i);
    radio2.startListening();
    delayMicroseconds(200); 
    bool carrier = radio2.testCarrier();
    radio2.stopListening();
    
    if (carrier) {
      Serial.print(i); Serial.println(",-40");
      hitCount++; 
    } else {
      Serial.print(i); Serial.println(",-105");
    }
  }

  // --- COMMAND & BUTTON HANDLING ---
  char command = ' ';
  while (Serial.available() > 0) {
    command = Serial.read(); // Grab commands from Python/Streamlit
  }

  // Read the physical button (LOW means it is being pressed)
  bool isButtonPressed = (digitalRead(BUTTON_PIN) == LOW);

  // Stream data to ESP32: If Button is pressed OR Streamlit sends 'A', fake a massive hit
  if (command == 'A' || isButtonPressed) {
    espSerial.print("HITS:");
    espSerial.println(99); 
  } else {
    espSerial.print("HITS:");
    espSerial.println(hitCount);
  }

  // TRIGGER LOCAL ALARM IF: Streamlit Alert OR local flood passes our DYNAMIC DIAL OR button press
  if (command == 'A' || hitCount >= dynamicThreshold || isButtonPressed) {
    alertTimer = millis(); 
    if (!isAlertActive) {
      digitalWrite(SAFE_LED_PIN, LOW);    
      isAlertActive = true;
    }
  }

  // --- ASYNCHRONOUS BLINKING & RESET LOGIC ---
  if (isAlertActive) {
    if (millis() - blinkTimer >= BLINK_INTERVAL) {
      blinkTimer = millis();
      alertLedState = !alertLedState; 
      digitalWrite(ALERT_LED_PIN, alertLedState);
    }

    if (millis() - alertTimer > 3000) {
      isAlertActive = false;
      digitalWrite(ALERT_LED_PIN, LOW);    
      digitalWrite(SAFE_LED_PIN, HIGH);    
    }
  }
}