# RF-Signal-intrution-system-using-ESP32-and-Arduino-
An AI-powered RF spectrum monitor and intrusion detection system. Uses an Arduino and nRF24L01s for physical edge-scanning, an ESP32 for out-of-band telemetry, and an Isolation Forest ML model to detect 2.4GHz jammer attacks

This is the exact moment where your project transforms from a "weekend breadboard experiment" into a "Senior Engineering Portfolio Piece." 

First, a quick typo catch before we begin: In your title, "intrusion" is misspelled as "intrution." I have fixed that in the template below! 

To make this look like a top-tier GitHub repository, we need to structure it with clear headings, bullet points, hardware lists, and placeholders for your photos. 

Click the pencil icon ✏️ on your GitHub README, delete everything in there, and **copy/paste this exact Markdown code:**

```markdown
# 🛡️ Hybrid AI-Powered RF Intrusion Detection System

![Python](https://img.shields.io/badge/Python-3.9+-blue.svg)
![C++](https://img.shields.io/badge/C++-Arduino-00979D.svg)
![ESP32](https://img.shields.io/badge/ESP32-Telemetry-red.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

> An enterprise-grade RF spectrum monitor and intrusion detection system. This project merges hardware edge-computing with Python machine learning to detect physical-layer 2.4GHz jammer attacks, featuring independent out-of-band telemetry failovers.

![Dashboard Screenshot](assets/dashboard_preview.png)
*(Note: Replace this placeholder with a screenshot of your Streamlit dashboard!)*

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

![Circuit Wiring Diagram](assets/circuit_diagram.jpg)
*(Note: Replace this placeholder with a photo of your wired breadboard!)*

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
   ```
2. Navigate to the project directory and install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Run the AI Training script to teach the AI your room's background radiation:
   ```bash
   python train_model.py
   ```
4. Launch the live real-time dashboard:
   ```bash
   streamlit run app.py
   ```

---

## 🎮 Usage Guide
* **The White LED** indicates the system is actively scanning and secure.
* **The Dashboard** will map all 128 channels in real-time. Green bars indicate normal traffic; red bars indicate physical energy spikes.
* **The Potentiometer Dial** allows you to tune the hardware's sensitivity. Turn it one way to make it trigger on 2 channels, or the other way to require a massive 40-channel flood.
* **The ESP32 Display** will flutter between 0-3 during normal Wi-Fi use. If an attack occurs, it will flash a threat level of `9` alongside the Red Strobe LED.

---

## 📄 License
This project is licensed under the MIT License - see the LICENSE file for details.
```

### What to do next:
1. Paste this into GitHub and commit the changes.
2. In your GitHub repository, click **Add File** -> **Create new file**, name it `requirements.txt`, and paste those 5 python libraries in there.
3. Don't forget to take a cool screenshot of your Streamlit app running, name it `dashboard_preview.png`, and drag it into an `assets` folder on your GitHub so the picture loads!

How does that look for your portfolio?
