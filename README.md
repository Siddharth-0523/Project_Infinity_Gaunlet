# Project Infinity Gauntlet 🧤

Project Infinity Gauntlet is an assistive smart glove designed to convert hand signs into audible speech.  
The goal of this project is to help people with speech or hearing disabilities communicate more easily with others.

The glove uses **flex sensors** (and later motion sensors) to detect hand gestures and convert them into meaningful speech output.

> ⚠️ Project Status: Early prototype / Work in progress

---

## Hardware & Tools Used

- ESP32 microcontroller
- Flex sensors (testing phase)
- Mobile phone (for Wi-Fi connection and Text-to-Speech)
- Arduino IDE
- Web browser (Google Chrome recommended)

---

## Current Progress

The initial phase focused on testing the **Wi-Fi capabilities of the ESP32**, which has been successfully implemented.

At the current stage:
- ESP32 operates in **Access Point (AP) mode**
- A mobile phone connects directly to the ESP32 via Wi-Fi
- Text entered in the **Serial Monitor** is sent to the phone
- The phone reads the text aloud using **Text-to-Speech (TTS)** through a web interface

---

## How the Wi-Fi Text-to-Speech Demo Works

1. ESP32 creates a Wi-Fi network (Access Point mode)
2. Phone connects to the ESP32 Wi-Fi using the credentials defined in the code
3. ESP32 hosts a local webpage at: http://192.168.4.1/
4. User types a message in the Serial Monitor
5. When the webpage is opened or refreshed, the message is spoken aloud

---

## Flex Sensor Testing

Flex sensors have been successfully tested and are working as expected.

Current implementation:
- Flex sensors are connected to the ESP32 using analog input pins
- A voltage divider circuit is used for stable readings
- Raw sensor values are read and displayed on the Serial Monitor
- Bending the sensor produces a clear and repeatable change in values

This testing phase confirms that the flex sensors can reliably detect finger bending,  
which is essential for accurate hand gesture recognition.

The flex sensor checking code can be found here:
firmware/flex_sensor_test/flex_sensor_test.ino


