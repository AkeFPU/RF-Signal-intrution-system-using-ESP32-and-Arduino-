#define RXD2 16
#define TXD2 17
#define ONBOARD_LED 2

// 7-Segment Pin Definitions
int segA = 13; int segB = 14; int segC = 25; int segD = 26;
int segE = 27; int segF = 32; int segG = 33;

int segments[] = {segA, segB, segC, segD, segE, segF, segG};

// Binary map for numbers 0-9 (1 = ON, 0 = OFF)
byte numbers[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(ONBOARD_LED, OUTPUT);
  
  for (int i = 0; i < 7; i++) {
    pinMode(segments[i], OUTPUT);
  }
  
  Serial.println("🛡️ ESP32 TELEMETRY ENGINE ONLINE 🛡️");
  displayNumber(0); // Default to Safe (0)
}

void loop() {
  if (Serial2.available()) {
    String incomingMessage = Serial2.readStringUntil('\n');
    incomingMessage.trim();

    // Check if the message starts with our new "HITS:" prefix
    if (incomingMessage.startsWith("HITS:")) {
      String valueString = incomingMessage.substring(5);
      int hitCount = valueString.toInt();
      
      // Map the hit count to a Threat Level (Max is 9)
      int threatLevel = hitCount;
      if (threatLevel > 9) threatLevel = 9;
      
      displayNumber(threatLevel);

      // ALARM CONDITION: If hits exceed our threshold (12), freak out!
      if (hitCount >= 12) {
        Serial.println("🚨 MAXIMUM THREAT LEVEL REACHED! 🚨");
        
        // Flash everything for 3 seconds
        for(int i = 0; i < 15; i++) {
          digitalWrite(ONBOARD_LED, HIGH);
          displayNumber(9); 
          delay(100);
          
          digitalWrite(ONBOARD_LED, LOW);
          clearDisplay(); // Flash the 7-segment off
          delay(100);
        }
        
        // Flush buffer so we don't get stuck in a loop
        while(Serial2.available() > 0) { Serial2.read(); }
      }
    }
  }
}

// Function to draw the number on the display
void displayNumber(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments[i], numbers[num][i]);
  }
}

// Function to turn off all segments
void clearDisplay() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments[i], LOW);
  }
}