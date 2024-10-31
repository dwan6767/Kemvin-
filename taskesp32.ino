#include <WiFi.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define IR_RECEIVE_PIN 15
#define IR_SEND_PIN 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Kakbus";  
const char* password = "Shikaridwan6@";  

const int pinSelect = 27;
const int pinUp = 14;
const int pinDown = 12;
const int buzz = 18;
int selectedPin = 15;    
int blinkFrequency = 1;  
int programOption = 0;   
int currentTab = 0;
int option = 0;
bool inTab = false;
bool wifiConnected = false;

unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
unsigned long lastDebounceTimeSelect = 0;
const unsigned long debounceDelay = 200;  

String tabs[] = {"Configure", "Attacks", "Program Upload", "Wifi Finder"};
IRrecv irrecv(IR_RECEIVE_PIN);  // IR receiver object
IRsend irsend;                  // IR sender object
decode_results results;         // Object to store received IR codes


void setup() {
  pinMode(pinSelect, INPUT_PULLDOWN);
  pinMode(pinUp, INPUT_PULLDOWN);
  pinMode(pinDown, INPUT_PULLDOWN);
  pinMode(buzz,OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  showIntroduction();  

 
  
 delay(2000);
  
  updateDisplay();
}

void loop() {
  handleNavigation();
}

void handleNavigation() {
 
  if (digitalRead(pinUp) == HIGH) {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTimeUp > debounceDelay) {
      navigateUp();
      lastDebounceTimeUp = currentTime;
    }
  }

  
  if (digitalRead(pinDown) == HIGH) {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTimeDown > debounceDelay) {
      navigateDown();
      lastDebounceTimeDown = currentTime;
    }
  }

 
  if (digitalRead(pinSelect) == HIGH) {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTimeSelect > debounceDelay) {
      if (inTab) {
        backToMenu();
      } else {
        enterTab();
      }
      lastDebounceTimeSelect = currentTime;
    }
  }
}

void navigateUp() {
  if (!inTab) {
    currentTab--;
    if (currentTab < 0) currentTab = 3;  
    updateDisplay();
     simpletone();

  } else {
    option--;
    if (option < 0) option = 2; 
    updateDetailsTab();
  }
}

void navigateDown() {
  if (!inTab) {
    currentTab++;
    if (currentTab > 3) currentTab = 0;  // wrap around
    updateDisplay();
     simpletone();
  } else {
    option++;
    if (option > 2) option = 0;  // adjust as per the number of options inside the tab
    updateDetailsTab();
  }
}

void enterTab() {
  inTab = true;
  if (currentTab == 0) {
    // Connect to WiFi when "Configure" is selected
    connectToWiFi();
  } else if (currentTab == 1) {  // Games Tab
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Infrared Attack:");
     irrecv.enableIRIn();
    display.println("Waiting for IR signal...");
    display.display();
    simpletone();
    
    // Wait for an IR signal to be received
    if (irrecv.decode(&results)) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("IR signal received!");

      // Display the received value on the screen
      display.setCursor(0, 10);
      display.print("Code: 0x");
      display.println(results.value, HEX);
      display.display();

      // Save the received code to replay later
      unsigned long receivedCode = results.value;

      // Emulate the received IR code
      delay(2000);  // Give time for the user to see the code
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("Sending IR signal...");
      delay(200);
      
      display.display();

      irsend.sendNEC(receivedCode, 32);  // Send the captured IR signal
      delay(500);  // Short delay to avoid continuous sending

      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("IR signal sent!");
      display.display();
      simpletone();
    }

    irrecv.resume();  // Receive the next value
  }
   else if (currentTab == 2) {  // Program Upload Tab
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("click to open terminal");
     simpletone();
  
    display.display();
    // Placeholder for upload handling
    pinSelectionMode();  // Enter pin selection mode
  } else if (currentTab == 3) {  // Entertainment Tab
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Wi-Fi Search:");
    display.display();
     simpletone();
    wifiSearch();
  }
}

void backToMenu() {
  inTab = false;
  updateDisplay();
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);  // Ensure normal text size
  display.println("Choose your mode");
  for (int i = 0; i < 4; i++) {
    if (i == currentTab) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.println(tabs[i]);
     
  }
   simpletone();
  display.display();
}

void updateDetailsTab() {
  if (currentTab == 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);  // Ensure normal text size in details tab
    switch (option) {
      case 0:
        display.println("WiFi SSID: ");
        if (wifiConnected) {
          display.setCursor(0, 10);
          display.println(WiFi.SSID());
           simpletone();
        } else {
          display.println("Not Connected");
           simpletone();
        }
        break;
      case 1:
      display.setCursor(0, 10);
        display.println("Chip: ESP32-D0WD-V3");
        display.println(" ");
        display.println("MAC: 2c:bc:bb:0c:61:c8");
         simpletone();
        break;
      case 2:
      display.setCursor(0, 10);
        display.println("Flash Memory: ");
        display.println("");
        display.println(ESP.getFlashChipSize());
         simpletone();
        break;
      case 3:
        if (!wifiConnected) {
          display.println("Retry WiFi");
          connectToWiFi();  // Retry Wi-Fi connection if requested
           simpletone();
        }
        break;
    }
    display.display();
  }
}





void showIntroduction() {
  display.clearDisplay();
  display.setTextSize(3);  // Make the "KEMVIN" bold and large
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("KEMVIN");
  display.display();
  playRandomTones() ;
  delay(100);  // Show the "KEMVIN" introduction for 2 seconds
  display.setTextSize(1);  // Reset text size back to normal after introduction

}

void pinSelectionMode() {
  while (true) {
    // Check UP and DOWN buttons to change pin selection
    if (digitalRead(pinUp) == LOW) {
      if (selectedPin > 15) {
        selectedPin--;
        updatePinSelectionDisplay();
        simpletone();
      }
      delay(200);  // Debounce delay
    } else if (digitalRead(pinDown) == LOW) {
      if (selectedPin < 18) {
        selectedPin++;
        updatePinSelectionDisplay();
        simpletone();
      }
      delay(200);  // Debounce delay
    }

    // If SELECT button is pressed, move on to frequency setting
    if (digitalRead(pinSelect) == HIGH) {
      selectFrequency();
      break;  // Exit the loop after selecting a pin
    }
  }
}

void updatePinSelectionDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Select GPIO Pin:");
  for (int i = 15; i <= 18; i++) {
    if (i == selectedPin) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.print("GPIO ");
    display.println(i);
  }
  display.display();
}

void selectFrequency() {
  while (true) {
    // Check UP and DOWN buttons to change frequency
    if (digitalRead(pinUp) == LOW) {
      if (blinkFrequency < 1000) {
        blinkFrequency++;
        updateFrequencyDisplay();
      }
      delay(200);  // Debounce delay
    } else if (digitalRead(pinDown) == LOW) {
      if (blinkFrequency > 1) {
        blinkFrequency--;
        updateFrequencyDisplay();
      }
      delay(200);  // Debounce delay
    }

    // If SELECT button is pressed, start blinking
    if (digitalRead(pinSelect) == HIGH) {
      startBlinking(selectedPin, blinkFrequency);
      break;  // Exit the loop after starting the blink
    }
  }
}

void updateFrequencyDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Select Frequency:");
  display.print(blinkFrequency);
  display.println(" Hz");
  display.display();
}

void startBlinking(int pin, int frequency) {
  pinMode(pin, OUTPUT);
  int interval = 1000 / (2 * frequency);  // Calculate interval in milliseconds
  while (true) {
    digitalWrite(pin, HIGH);
    delay(interval);
    digitalWrite(pin, LOW);
    delay(interval);

    // Check if SELECT button is pressed to exit blinking
    if (digitalRead(pinSelect) == HIGH) {
      break;  // Exit blinking
    }
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Connecting to WiFi...");
  display.display();
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  wifiConnected = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Connected to WiFi");
  display.display();
  simpletone();
  simpletone();
}

void wifiSearch() {
  int n = WiFi.scanNetworks();
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.println("Networks found:");
  for (int i = 0; i < n; i++) {
    display.print(WiFi.SSID(i));
    display.print(" (");
    display.print(WiFi.RSSI(i));  // Display dBm value
    display.println(" dBm)");
    if (i == 4) break;  // Show up to 5 networks to fit screen
  }
  display.display();
  simpletone();
}

void simpletone(){
digitalWrite(buzz,HIGH);
delay(100);
digitalWrite(buzz,LOW);
delay(30);
}
void simpletone1(){
digitalWrite(buzz,HIGH);
delay(80);
digitalWrite(buzz,LOW);
delay(30);
}
void simpletone2(){
digitalWrite(buzz,HIGH);
delay(120);
digitalWrite(buzz,LOW);
delay(30);
}
void simpletone3(){
digitalWrite(buzz,HIGH);
delay(50);
digitalWrite(buzz,LOW);
delay(30);
}
void playRandomTones() {
  unsigned long startingMillis = millis();
  
  while (millis() - startingMillis < 2000) {  
    int randomFrequency = random(200, 3000);      
    int delayTimeu = 1000000 / randomFrequency / 2;  
    
   
    for (int i = 0; i < random(100, 300); i++) {  
      digitalWrite(buzz, HIGH);
      delayMicroseconds(delayTimeu);
      digitalWrite(buzz, LOW);
      delayMicroseconds(delayTimeu);
    }
  }
}
