# Kemvin-


---

# **ESP32 Universal Multitasker **

## **Project Overview**
This project uses an **ESP32** microcontroller to create a multi-functional device with a sliding UI interface displayed on an **OLED screen**. The device is equipped with multiple operational modes, including:
- **Wi-Fi Network Scanning**
- **IR Signal Reception and Transmission**
- **PWM Control**
- **Sensor Sensing**
- **Analog Sensing**

The user can navigate through these modes using three buttons (**Up**, **Down**, and **Select**) while viewing a graphical interface that slides left and right between modes. Each mode is represented by its own logo displayed on an OLED screen. Additionally, UART communication is used to exchange data with another device, such as an **Arduino Uno**. The project also features a buzzer to provide audio feedback.

## **Hardware Requirements**
- **ESP32 Development Board**
- **128x64 OLED Display**
- **IR Receiver (TSOP)**
- **IR Transmitter**
- **3 Push Buttons**
- **Potentiometers** (for PWM Control)
- **External EEPROM** (optional, if required for data storage)
- **Buzzer**
- **Resistors and Wires**
- **Battery Monitoring Circuit** (optional, for low battery interrupts)

## **Features**
### **1. User Interface (UI) with Slide Transitions**
- The main interface consists of **sliding tabs**, each representing a different operational mode. Users can navigate through the tabs by pressing the **Up** and **Down** buttons, and select a mode using the **Select** button.
- Each tab contains its own graphical **logo**, which slides in and out during transitions.

### **2. Operational Modes**
#### **Wi-Fi Network Scanning**
- Scans nearby Wi-Fi networks and displays them on the OLED screen.
  
#### **IR Remote Control Emulator**
- Receives and emulates IR remote control signals using an **IR Receiver** and **IR Transmitter**.

#### **PWM Control**
- Uses potentiometers to adjust the **duty cycle** (0-100%) and **frequency** (20Hz to 20kHz) of a **PWM signal**.

#### **Sensor Sensing (UART Communication)**
- Exchanges data with another microcontroller (e.g., Arduino Uno) over **UART**.
  
#### **Analog Sensing**
- Reads analog signals from a sensor and plots the values as a graph on the OLED screen.

### **3. Low Battery Interrupt**
- An interrupt triggers when the battery voltage drops below a certain threshold, ensuring the system can handle low-power conditions.

## **Pin Configuration**
| **Component**          | **ESP32 Pin**    |
|------------------------|------------------|
| OLED SDA               | GPIO 21          |
| OLED SCL               | GPIO 22          |
| IR Receiver (TSOP)     | GPIO 15          |
| PWM Output Pin         | GPIO 23          |
| Potentiometer (Duty)   | GPIO 33          |
| Potentiometer (Freq)   | GPIO 32          |
| Analog Sense Input     | GPIO 34          |
| Button (Select)        | GPIO 27          |
| Button (Up)            | GPIO 14          |
| Button (Down)          | GPIO 12          |
| Buzzer                 | GPIO 18          |
| UART RX                | GPIO 16          |
| UART TX                | GPIO 17          |

## **Installation**
### **1. Clone the Repository**
```bash
git clone https://github.com/YourUsername/ESP32-Multimode-UI.git
cd ESP32-Multimode-UI
```

### **2. Hardware Setup**
1. Connect the **OLED display** to the ESP32 using I2C pins (SDA & SCL).
2. Connect the **IR Receiver** and **IR Transmitter** to the designated GPIO pins.
3. Connect the **PWM control potentiometers** to the ESP32.
4. Set up the **push buttons** for navigation and selection.
5. Optionally, set up a **battery monitoring circuit** if you're handling low battery detection.

### **3. Install Dependencies**
Ensure the following libraries are installed in your Arduino IDE:
- **WiFi.h**
- **SPIFFS.h**
- **Wire.h**
- **Adafruit_SSD1306.h** (for OLED Display)
- **IRremote.h** (for IR communication)

### **4. Upload the Code**
1. Open the `ESP32_Multimode_UI.ino` file in the Arduino IDE.
2. Select your ESP32 board and the correct port in the **Tools** menu.
3. Upload the code.

## **Usage Instructions**
1. **Power on the device**. Upon startup, the device will display a welcome screen for 2 seconds.
2. **Navigate between tabs** using the **Up** and **Down** buttons. Each tab will slide into view, displaying its logo and name.
3. **Select a tab** using the **Select** button to enter its mode:
   - **Wi-Fi Mode**: Scans for Wi-Fi networks.
   - **IR Emulator**: Receives and emulates IR signals.
   - **PWM Control**: Adjusts duty cycle and frequency using potentiometers.
   - **Sensor Sense (UART)**: Exchanges data with a connected device via UART.
   - **Analog Sense**: Reads and plots analog sensor data.
4. **Low Battery Detection**: If the battery voltage drops below a certain threshold, an interrupt will trigger, alerting the user.

## **Code Structure**
- **`ESP32_Multimode_UI.ino`**: Main code handling the UI, navigation, and mode selection.
- **`WiFi.h` & `IRremote.h`**: Libraries used for Wi-Fi scanning and IR communication.
- **`Adafruit_SSD1306.h`**: Used to handle the OLED display.
- **`UART()`**: Manages UART communication for the Sensor Sense tab.

## **Future Improvements**
- Add more modes such as **Bluetooth connectivity** or **data logging**.
- Implement **EEPROM storage** to save user preferences across power cycles.
- Improve **battery monitoring** by providing real-time updates on battery voltage.

## **License**
This project is licensed under the MIT License. Feel free to modify and use the code for your own projects.

## **Contributing**
Contributions are welcome! Feel free to submit a pull request or open an issue for any bug reports or feature requests.

---

This **README** provides clear information about your project, its functionality, installation process, and usage, making it easy for anyone to understand and contribute to the project.
