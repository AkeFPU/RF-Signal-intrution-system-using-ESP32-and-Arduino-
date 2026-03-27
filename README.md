# RF-Signal-intrution-system-using-ESP32-and-Arduino-
An AI-powered RF spectrum monitor and intrusion detection system. Uses an Arduino and nRF24L01s for physical edge-scanning, an ESP32 for out-of-band telemetry, and an Isolation Forest ML model to detect 2.4GHz jammer attacks


# 🛡️ Hybrid AI-Powered RF Intrusion Detection System


An enterprise-grade RF spectrum monitor and intrusion detection system. This project merges hardware edge-computing with Python machine learning to detect physical-layer 2.4GHz jammer attacks, featuring independent out-of-band telemetry failovers.


---

## ✨ Key Features
* **Hybrid Edge/Cloud Architecture:** Hardware-level anomaly detection runs in parallel with a PC-based Machine Learning model.
* **Unsupervised Machine Learning:** Utilizes an `Isolation Forest` AI model to learn the room's baseline radiation and flag unnatural spectrum floods mathematically.
* **Out-of-Band (OOB) Telemetry:** The Arduino streams live threat data to a dedicated ESP32 graphics processor, ensuring the alarm system works even if the primary Wi-Fi network is completely jammed.
* **Dynamic Hardware Sensitivity Dial:** A physical 10k potentiometer allows security personnel to manually tune the edge-computing threshold on the fly to adapt to crowded RF environments.
* **Hardware Override:** Includes a physical panic button for manual system-wide lockdown, triggering local red strobe LEDs and the ESP32 threat display.

---

## 🧠 System Architecture

This system is divided into three completely independent "brains":

1. **The Scanner (Arduino Uno + 2x nRF24L01):**
   Physically sweeps 128 channels of the 2.4GHz spectrum using dual radios. It reads the raw physical energy (RSSI) to detect broad-band floods (jammers) or narrow-band spikes (Wi-Fi/Bluetooth).
2. **The Telemetry Engine (ESP32 + 7-Segment Display):**
   Acts as an isolated security console. It receives raw threat-level data from the Arduino and acts as a graphics driver for a 10-pin 7-segment display, showing the active threat level (0-9).
3. **The AI Brain (Python + Streamlit):**
   Ingests the 128-channel array via USB. An Isolation Forest algorithm evaluates the morphological shape of the radio spectrum to filter out normal background noise and trigger an interactive dashboard alert.

---

## 🛠️ Hardware Requirements

* 1x Arduino Uno
* 1x ESP32 DevKit V1 (36-pin)
* 2x nRF24L01 Radio Modules (with PA/LNA antennas recommended)
* 1x 10-Pin Single Digit 7-Segment Display
* 1x 10k Trimpot (Potentiometer)
* 1x Tactile Push Button (4-pin)
* 2x LEDs (1x Red, 1x White)
* Resistors (220Ω for LEDs and 7-segment display)
* Breadboards and Jumper Wires


---

## 💻 Software & Library Requirements

### Python Environment
* `streamlit`
* `pandas`
* `scikit-learn`
* `pyserial`
* `altair`

### Arduino IDE Libraries
* `RF24` by TMRh20
* `SoftwareSerial` (Built-in)

---

## 🚀 Installation & Setup

### 1. Flash the Hardware
1. Open the `Arduino_Scanner` folder and upload the `.ino` file to your Arduino Uno.
2. Open the `ESP32_Telemetry` folder and upload the `.ino` file to your ESP32.
3. Wire the components according to the pin definitions at the top of each script.

### 2. Setup the Python AI
1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/YOUR_USERNAME/RF-Signal-Intrusion-System.git](https://github.com/YOUR_USERNAME/RF-Signal-Intrusion-System.git)
