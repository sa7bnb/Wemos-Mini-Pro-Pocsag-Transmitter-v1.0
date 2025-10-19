// RadioLib POCSAG Transmitter for CC1101 with frequency offset
#include <RadioLib.h>

// CC1101 connections for Wemos D1 mini Pro
// CS pin:    15 (instead of 5 in example)
// GDO0 pin:  4  (keep same as example) 
// RST pin:   NC (not connected)
// GDO2 pin:  2  (keep same as example)
// Note: SPI pins (SCK=14, MISO=12, MOSI=13) are handled automatically

CC1101 radio = new Module(15, 4, RADIOLIB_NC, 2);

// LED pin
#define LED_PIN 5

// FREQUENCY SETTINGS - ADJUST THESE
float BASE_FREQUENCY = 433.92;     // Nominal frequency (MHz)
float FREQUENCY_OFFSET = 0.015; // Offset in MHz (15 kHz in your case)
                                   // Positive = higher frequency
                                   // Negative = lower frequency

// create Pager client instance using the FSK module
PagerClient pager(&radio);

void setup() {
  Serial.begin(115200);
  Serial.println("RadioLib POCSAG Transmitter Starting...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Calculate actual frequency
  float actualFrequency = BASE_FREQUENCY + FREQUENCY_OFFSET;
  
  Serial.printf("Base frequency: %.5f MHz\n", BASE_FREQUENCY);
  Serial.printf("Frequency offset: %.5f MHz\n", FREQUENCY_OFFSET);
  Serial.printf("Actual transmit frequency: %.5f MHz\n", actualFrequency);

  // initialize CC1101 with default settings
  Serial.print(F("[CC1101] Initializing ... "));
  int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize Pager client with adjusted frequency
  Serial.print(F("[Pager] Initializing ... "));
  state = pager.begin(actualFrequency, 1200);  // Use adjusted frequency
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
    
    // Set maximum output power for amateur radio use
    Serial.print(F("[CC1101] Setting maximum power ... "));
    state = radio.setOutputPower(10);  // +10 dBm (approximately 10mW)
    if(state == RADIOLIB_ERR_NONE) {
      Serial.println(F("MAX POWER SET!"));
    } else {
      Serial.print(F("power setting failed, code "));
      Serial.println(state);
    }
    
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  Serial.println("RadioLib POCSAG ready at maximum power!");
  Serial.printf("Transmitting on: %.5f MHz\n", actualFrequency);
}

void loop() {
  Serial.println("\n=== POCSAG Message Sender ===");
  Serial.println("Format: RIC::Message (e.g., '123456::Hello World')");
  Serial.println("Or just: Message (uses default RIC 123456)");
  Serial.printf("TX Frequency: %.5f MHz\n", BASE_FREQUENCY + FREQUENCY_OFFSET);
  Serial.print("Enter command: ");
  
  // Wait for user input
  String userInput = "";
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      
      if (c == '\n' || c == '\r') {
        // Enter pressed - process command if not empty
        if (userInput.length() > 0) {
          Serial.println(); // New line after input
          break;
        }
      } else if (c >= 32 && c <= 126) {
        // Printable character
        userInput += c;
        Serial.print(c); // Echo character
      } else if (c == 8 || c == 127) {
        // Backspace
        if (userInput.length() > 0) {
          userInput.remove(userInput.length() - 1);
          Serial.print("\b \b"); // Erase character on screen
        }
      }
    }
    delay(10);
  }
  
  // Parse input: Check if format is "RIC::Message"
  userInput.trim();
  long ricNumber = 123456; // Default RIC
  String message = "";
  
  int separatorPos = userInput.indexOf("::");
  if (separatorPos > 0) {
    // Format: RIC::Message
    String ricString = userInput.substring(0, separatorPos);
    message = userInput.substring(separatorPos + 2);
    
    // Validate and convert RIC
    ricNumber = ricString.toInt();
    if (ricNumber <= 0 || ricNumber > 2097151) { // POCSAG RIC limit (21 bits)
      Serial.printf("Error: Invalid RIC '%s'. Must be 1-2097151. Using default 123456.\n", ricString.c_str());
      ricNumber = 123456;
    }
  } else {
    // Format: Just message (use default RIC)
    message = userInput;
  }
  
  // Validate message
  if (message.length() == 0) {
    Serial.println("Error: Empty message. Please try again.");
    delay(2000);
    return;
  }
  
  if (message.length() > 40) {
    Serial.println("Warning: Message truncated to 40 characters");
    message = message.substring(0, 40);
  }
  
  digitalWrite(LED_PIN, LOW);
  
  Serial.printf("Sending: '%s' to RIC %ld\n", message.c_str(), ricNumber);
  Serial.print(F("[Pager] Transmitting ... "));

  // Send user's alphanumeric message to specified RIC
  int state = pager.transmit(message.c_str(), ricNumber, RADIOLIB_PAGER_ASCII);

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("SUCCESS!"));
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Message sent successfully!");
  } else {
    Serial.print(F("FAILED, code "));
    Serial.println(state);
  }
  
  delay(2000); // Brief pause before next prompt
}
