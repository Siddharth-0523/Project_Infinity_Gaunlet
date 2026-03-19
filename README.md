# Project Infinity Gauntlet 🧤

Project Infinity Gauntlet is an assistive smart glove designed to convert hand signs into audible speech.
The goal of this project is to help people with speech or hearing disabilities communicate more easily with others.

The glove uses **flex sensors** (and later motion sensors) to detect hand gestures and convert them into meaningful speech output.

> ⚠️ Project Status: Early prototype / Work in progress

---

## Hardware & Tools Used

* ESP32 microcontroller
* Flex sensors (testing phase)
* Mobile phone (for Wi-Fi connection and Text-to-Speech)
* Arduino IDE
* Web browser (Google Chrome recommended)

---

## Current Progress

The initial phase focused on testing the **Wi-Fi capabilities of the ESP32**, which has been successfully implemented.

At the current stage:

* ESP32 operates in **Access Point (AP) mode**
* A mobile phone connects directly to the ESP32 via Wi-Fi
* Text entered in the **Serial Monitor** is sent to the phone
* The phone reads the text aloud using **Text-to-Speech (TTS)** through a web interface

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

* Flex sensors are connected to the ESP32 using analog input pins
* A voltage divider circuit is used for stable readings
* Raw sensor values are read and displayed on the Serial Monitor
* Bending the sensor produces a clear and repeatable change in values

This testing phase confirms that the flex sensors can reliably detect finger bending,
which is essential for accurate hand gesture recognition.

The flex sensor checking code can be found here:
firmware/flex_sensor_test/flex_sensor_test.ino

---

## Flex Sensor Calibration Improvement

During testing, it was observed that **flex sensor baseline values change slightly every time the glove is worn**.
This happens due to:

* Slight differences in finger position
* Mechanical stress on the sensor
* Sensor placement variations
* Temperature and environmental conditions

Initially, fixed threshold values were used to detect finger bending.
However, this caused inconsistent gesture detection.

### Solution Implemented

To solve this issue, a **dynamic calibration system** was introduced.

When the device starts:

1. The user keeps all fingers **straight for 5 seconds**
2. The ESP32 continuously reads values from all flex sensors
3. The **average value of each sensor is calculated**
4. This average is stored as the **normal (unbent) value** for that sensor

Each flex sensor therefore gets its **own baseline value** instead of relying on fixed thresholds.

### Gesture Detection Logic

After calibration:

If the difference between the current reading and the stored baseline value exceeds a **tolerance of 30**, the finger is considered **bent**.

Example logic:

```
if |current_value - normal_value| > tolerance
    finger_state = 1
else
    finger_state = 0
```

The system outputs finger states like:

Index : 1
Middle : 0
Ring : 0
Little : 1

This produces a **binary representation of hand gestures**, which can later be mapped to specific words or phrases.

---

## MPU6050 Motion Sensor Testing

The MPU6050 motion sensor has been successfully tested and is working as expected.

Current implementation:

1. MPU6050 is interfaced with the ESP32 using I2C communication
2. Raw accelerometer values (X, Y, Z) are read correctly
3. Roll angle is calculated using accelerometer data

Hand orientation is classified as:

* UP
* NEUTRAL
* DOWN

Initial issues were faced when using high-level libraries.

This was resolved by using **low-level I2C register access**, which provided reliable sensor readings.

---

## Challenges Faced

Challenges Faced -

1. Random and noisy flex sensor readings
2. MPU6050 not detected using certain libraries
3. Incorrect tilt detection due to sensor orientation
4. Power instability when using a battery supply
5. Flex sensor values changing between different uses of the glove
6. During full integration , the sensor's were not sensitive enough.

Solutions Implemented -

1. Added a **10 µF capacitor** to stabilize flex sensor readings
2. Used **direct I2C register-level communication** for the MPU6050
3. Identified the correct tilt axis based on sensor mounting
4. Used a **buck converter** to provide stable power to the ESP32
5. Implemented **automatic 5-second calibration to dynamically determine normal flex sensor values**
6.By reducing the sensitive (tolerance) the gloves was made to react to even smaller bending's

---

## Next Steps

* Combine flex sensor data with MPU6050 orientation data
* Define hand gesture patterns
* Map gestures to predefined speech outputs
* Integrate gesture detection with the Wi-Fi Text-to-Speech system
