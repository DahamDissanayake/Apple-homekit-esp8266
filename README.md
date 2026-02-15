# Apple HomeKit ESP8266 Smart Switch

This project enables you to control 4 switches (relays or LEDs) using Apple HomeKit on an ESP8266 implementation. It uses the [Arduino-HomeKit-ESP8266](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266) library to create a HomeKit accessory.

## Features

- **HomeKit Integration**: Natively control devices via the Apple Home app and Siri.
- **4-Channel Control**: Supports up to 4 distinct switches/relays.
- **Wi-Fi Connectivity**: Connects to your existing 2.4GHz Wi-Fi network.

## Hardware Requirements

- **ESP8266 Development Board**: Such as NodeMCU or Wemos D1 Mini.
- **Relay Module (4-Channel)**: To control high-voltage appliances (optional, for real-world usage).
- **Jumper Wires**: For connections.

### Default Pin Mapping

The code is currently configured to control the following GPIO pins:

| Switch   | GPIO Pin | NodeMCU/WeMos Pin |
| :------- | :------- | :---------------- |
| Switch 1 | GPIO 2   | D4                |
| Switch 2 | GPIO 5   | D1                |
| Switch 3 | GPIO 4   | D2                |
| Switch 4 | GPIO 0   | D3                |

> **Note**: These pins can be customized in `DAMA_homekit/DAMA_homekit.ino`.

## Software Requirements

- **Arduino IDE**: [Download here](https://www.arduino.cc/en/software).
- **ESP8266 Board Package**: Install via Arduino IDE Board Manager.
- **Library**: `Arduino-HomeKit-ESP8266` by Mixiaoxiao.

## Installation & Setup

1. **Clone the Repository**

   ```bash
   git clone https://github.com/DahamDissanayake/Apple-homekit-esp8266.git
   ```

2. **Open the Project**
   - Open `DAMA_homekit/DAMA_homekit.ino` in the Arduino IDE.

3. **Configure Wi-Fi**
   - Open the `wifi_info.h` tab.
   - Update `ssid` and `password` with your Wi-Fi credentials:
     ```c
     const char *ssid = "YOUR_WIFI_SSID";
     const char *password = "YOUR_WIFI_PASSWORD";
     ```

4. **Upload to ESP8266**
   - Connect your ESP8266 to your computer.
   - Select the correct Board and Port in Arduino IDE.
   - Click **Upload**.

   > **Troubleshooting**: If you need to reset the HomeKit pairing, uncomment the line `homekit_storage_reset();` in `DAMA_homekit.ino`, upload, run it once, then comment it out and re-upload.

## Usage

1. **Monitor Serial Output**
   - Open the Serial Monitor (Baud Rate: 115200).
   - Wait for the "WiFi connected" message and the "HomeKit: Started" logs.

2. **Pair with Home App**
   - Open the **Home** app on your iPhone/iPad.
   - Tap **Add Accessory**.
   - Tap **More options...** slightly above the camera view if the accessory doesn't appear immediately.
   - Select **SmartR_Sys** (or the name defined in `my_accessory.c`).
   - Enter the Setup Code:

     **`123-41-234`**

3. **Control Devices**
   - You should now see 4 switches in your Home app.
   - Toggling them in the app will toggle the corresponding pins on the ESP8266.

## Additional Notes

- **Correct code.txt**: The root directory contains a file named `Correct code.txt`. This appears to be an enhanced version of the code that includes support for **physical toggle switches** with debouncing and synchronization. If you plan to use physical wall switches alongside HomeKit, consider migrating the logic from this file to your main sketch.

## License

This project is based on the [Arduino-HomeKit-ESP8266](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266) library.
