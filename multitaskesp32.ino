/* This project is a try to make an universal multitasker using esp32 microcontroller , in future we hope to develop a fully universal multitasking tool for enginners and geeks
/*
    ESP32 Multitasker  Project
//    Copyright (c) https://github.com/dwan6767/

    
*/





#include <WiFi.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h> // installing additional libraries for simplicity

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define IR_RECEIVE_PIN 15 // in ir remote library there is an in-built pin for this
#define analogSensePin 34 
#define potDutyPin 33  
#define potFreqPin 32  
#define pwmPin 23     
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUM_SAMPLES 128
#define SAMPLE_RATE 10000 
int samples[NUM_SAMPLES];
unsigned long lastSampleTime = 0;
unsigned int sampleInterval = 1000000 / SAMPLE_RATE;   // sample rate for oscillscope interface 
int verticalOffset = 0;
const int pinSelect = 27;
const int pinUp = 14;
const int pinDown = 12;
float k =0.3;
const int buzz = 18;
int programOption = 0;   
int currentTab = 0;
int option = 0;
bool inTab=false;
bool wifiConnected = false;
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;// Debouncing is the phenomena when even if you press the buttons , it may not be completely on or off there is a mechanical jitter ness , so some delay is taken in account
unsigned long lastDebounceTimeSelect = 0;
const unsigned long debounceDelay = 230; 

String tabs[] = {"Networks", "IR EMULATOR", "PWM", "Sensor-Sense","Analog-Sense",};// one can change the tabs as of their preference


IRrecv irrecv(IR_RECEIVE_PIN);  // IR receiver object
IRsend irsend;                  // IR sender object
decode_results results;         // Object to store received IR codes


void setup() {
  pinMode(pinSelect, INPUT_PULLDOWN);
  pinMode(pinUp, INPUT_PULLDOWN);// PULLING DOWN THE PIN USING INTERNAL PULL DOWN RESISTORS SO NO FLOATING VALUES
  pinMode(pinDown, INPUT_PULLDOWN);
  pinMode(buzz,OUTPUT);
  pinMode(analogSensePin,INPUT);
  pinMode(potDutyPin, INPUT);  
  pinMode(potFreqPin, INPUT);  
  pinMode(pwmPin, OUTPUT);     
  ledcSetup(0, 5000, 8);   // in built pwm control libarary
  ledcAttachPin(pwmPin, 0); 
Serial.begin(9600);
 irrecv.enableIRIn();
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
if(currentTab==4 ) {
analogSense();
val();}
if(currentTab==2 )
pwm();
 
}

void handleNavigation() {
 
  if (digitalRead(pinUp) == HIGH) {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTimeUp > debounceDelay) {   // in all naviogation   Debouncing delay time (200 ms) has considered one can use external capacitors so delay can be minimized in code
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
    if (currentTab < 0) currentTab = 4;  
    updateDisplay();
     simpletone();

  } else {
    option--;
    if (option < 0) option = 3; 
    updateDetailsTab();
  }
}

void navigateDown() {
  if (!inTab) {
    currentTab++;
    if (currentTab > 4) currentTab = 0;  // wrap around
    updateDisplay();
     simpletone();
  } else {
    option++;
    if (option > 3) option = 0;  // adjust as per the number of options inside the tab
    updateDetailsTab();
  }
}

void enterTab() {
  inTab = true;
  if (currentTab == 0) {
    updateDetailsTab();
   
  }  else if (currentTab == 1) {
   
   executeIRAttack();
  
  
}

   else if (currentTab == 2) {  // Pwm output tab
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("pwm outputs");
     simpletone();
  
    display.display();
   
   
  } else if (currentTab == 3) {  
    display.clearDisplay();
    display.setCursor(0, 0); // currently due to limited knowledge i have only been able to apply uart protocol for data
    display.setTextSize(1);
    display.println("UART..");
    
    display.display();
  
     simpletone();
     UART();
    
  }
  
}

void backToMenu() {
  inTab = false;
  
  updateDisplay();
}

void updateDisplay() {
  display.clearDisplay(); // the indicator will move through the tabs
  display.setCursor(0, 10);
  display.setTextSize(1);  /
  display.println("Choose your mode");
  for (int i = 0; i < 5; i++) {
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
    display.setCursor(30, 20);
    display.setTextSize(1);  
    switch (option) {
      case 0:
        display.println(" WELCOME ");
        display.setCursor(10,40);
        display.print("Wifi ");
        display.setCursor(0,40);
        display.print("<");
        display.setCursor(120,40);
        display.print(">");
        display.setCursor(90,40);
        display.print("Chip");
        
         
        simpletone();
       
        
        break;
      case 1:
      display.setCursor(0, 10);
        display.println("Chip: ESP32-D0WD-V3");
        display.println(" ");
        display.println("MAC: 2c:bc:bb:0c:61:c8");// you can get the mac id
        display.setCursor(120,40);
        
        display.print(">");
        display.setCursor(80,40);
        display.print("Memory");
        
         simpletone();
        break;
      case 2:
      display.setCursor(0, 10);
        display.println("Flash Memory: ");
        display.println("");
        display.println(ESP.getFlashChipSize()); // this is in bytes or 8 times in bit 
         simpletone();
          display.setCursor(120,40);
         display.print(">");
        display.setCursor(75,40);
        display.print("Wifi ");
        break;
      case 3:
      display.setCursor(0,20);
      display.println("Searching wifi..");
      delay(1000);
        wifiSearch();
        break;
        
    }
    display.display();
  }
}





void showIntroduction() {
  display.clearDisplay();
  display.setTextSize(3);  
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("KEMVIN");
  display.display();
  playRandomTones() ;
  delay(100);  
  display.setTextSize(1);  

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
digitalWrite(buzz,LOW);    // this tones every time i press or execute any button (i have called this function many times in code)
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
      delayMicroseconds(delayTimeu);   // currently i am playting random tones 
      digitalWrite(buzz, LOW);
      delayMicroseconds(delayTimeu);
    }
  }
}
void executeIRAttack() {
  while(!irrecv.decode(&results)){
    display.clearDisplay();
    
    display.setCursor(0, 20);
    display.setTextSize(1);
    display.println("Press your Remote Key...");
    display.display();
    simpletone();
    
  }
   
   delay(100);
 
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
      display.println("ATTACK in 5sec.. ");
      display.display();

      delay(5000);
      
      irsend.sendNEC(receivedCode, 32);  // Send the captured IR signal
      delay(1500);  // Short delay to avoid continuous sending

      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("ATTACK Successful!");
      display.display();
      simpletone();
      
      irrecv.resume();  // Prepare to receive the next signal
    }
   
}
void val(){
  int h = analogRead(analogSensePin);
  float d=3.3/4095*h;
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(d,2);
  display.display();
}
 void analogSense(){
 

  verticalOffset = constrain(verticalOffset, -30, 30);

 
  for (int i = 0; i < NUM_SAMPLES; i++) {
    while (micros() - lastSampleTime < sampleInterval) {
      
    }
    samples[i] = k*analogRead(analogSensePin);  
    lastSampleTime = micros();
  }

 
  display.clearDisplay();

  
  for (int i = 1; i < NUM_SAMPLES; i++) {
    int x0 = map(i - 1, 0, NUM_SAMPLES - 1, 0, SCREEN_WIDTH - 1);
    int y0 = map(samples[i - 1], 0, 1023, SCREEN_HEIGHT - 1, 0) + verticalOffset;     // maping the values in both x and y direction i have taken offset zero may be in further process external feedback will be added for 
    // for more smoother control over the screen 
    int x1 = map(i, 0, NUM_SAMPLES - 1, 0, SCREEN_WIDTH - 1);
    int y1 = map(samples[i], 0, 1023, SCREEN_HEIGHT - 1, 0) + verticalOffset;

    // Ensure y values are within screen bounds
    y0 = constrain(y0, 0, SCREEN_HEIGHT - 1);
    y1 = constrain(y1, 0, SCREEN_HEIGHT - 1);

    display.drawLine(x0, y0, x1, y1, SSD1306_WHITE);
  }

 
  display.display();

 
  delay(5);
   
   
  for (int j = 0; j < 50; j++) {
    handleNavigation();  // Keep checking for button presses during the delay
    delay(1);  // Small delay to avoid blocking other processes
  }
  
 }
 void pwm() {
  int potValueDuty = analogRead(potDutyPin);  // Read potentiometer for duty cycle
  int potValueFreq = analogRead(potFreqPin);  // Read potentiometer for frequency

  // Map the potentiometer values
  int dutyCycle = map(potValueDuty, 0, 4095, 0, 255);  // Map to 0-255 for 8-bit duty cycle
  int frequency = map(potValueFreq, 0, 4095, 20, 20000);  // Map to 100Hz - 10kHz range for frequency

  // Set the PWM frequency dynamically
  ledcSetup(0, frequency, 8);  // Update frequency (duty cycle stays 8-bit)
  ledcWrite(0, dutyCycle);     // Update the duty cycle (0-255)

  // Display on OLED (optional, you can adjust for your display)
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("PWM Control:");
  display.print("Duty Cycle: ");
  display.print(map(potValueDuty, 0, 4095, 0, 100));  // Display duty cycle percentage
  display.println("%");
  display.print("Frequency: ");
  display.print(frequency);  // Display the current frequency
  display.println(" Hz");
  display.display();

  delay(100);  // Small delay to smoothen the display updates
}
void UART() {
 
  String message = "Hello from ESP32!";
  Serial.println(message);  
  
  // Optional: If using additional UART pins, use Serial1
  // Serial1.println(message);

  if (Serial.available()) {
    String incomingData = Serial.readString();  
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(1);
    display.println("Received: ");
    display.println(incomingData);
    display.display();
  }
 
  delay(1000);  
}
