# Project Infinity Gauntlet 🧤

Project Infinity Gauntlet is an assistive smart glove designed to convert hand signs into audible speech.  
The aim of this project is to help people with speech or hearing disabilities communicate more easily with others.

The glove will use flex sensors (and later motion sensors) to detect hand gestures and convert them into meaningful speech output.

---

## Current Progress

The project was started by testing the Wi-Fi capabilities of the ESP32, which was successfully implemented.

At the current stage:
- The ESP32 works in **Access Point (AP) mode**
- A mobile phone connects directly to the ESP32 via Wi-Fi
- Text entered in the **Serial Monitor** is sent to the phone
- The phone reads the text aloud using **Text-to-Speech (TTS)** through a web interface

---

## How the Wi-Fi Text-to-Speech Demo Works

1. ESP32 creates a Wi-Fi network (Access Point mode)
2. Phone connects to the ESP32 Wi-Fi using the credentials defined in the code
3. ESP32 hosts a local webpage at: http://192.168.4.1/
4. User types a message in the Serial Monitor
5. When the webpage is opened/refreshed in Google Chrome, the message is spoken aloud

The Wi-Fi TTS testing code can be found here:
/firmware/wifi_tts/wifi_tts.ino

