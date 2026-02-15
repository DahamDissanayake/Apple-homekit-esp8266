#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// Board switches and Physical Switches Defining
#define PIN_SWITCH1 5     // Switch 1 connected to GPIO 5 (D1)
#define PHY_SWITCH1 14    // Physical Switch 1 connected to GPIO 14 (D5)

#define PIN_SWITCH2 4     // Switch 2 connected to GPIO 4 (D2)
#define PHY_SWITCH2 12    // Physical Switch 2 connected to GPIO 12 (D6)

#define PIN_SWITCH3 0 
    // Switch 3 connected to GPIO 0 (D3)
#define PHY_SWITCH3 13    // Physical Switch 3 connected to GPIO 13 (D7)

#define PIN_SWITCH4 2     // Switch 4 connected to GPIO 2 (D4)
#define PHY_SWITCH4 15    // Physical Switch 4 connected to GPIO 15 (D8)

#define DEBUG_SW 1

// Previous states for debounce and synchronization
int switch_ON_Flag1_Previous = 0;
int switch_ON_Flag2_Previous = 0;
int switch_ON_Flag3_Previous = 0;
int switch_ON_Flag4_Previous = 0;

// Debounce-related variables
const int debounceDelay = 50;  // 50ms debounce delay
uint32_t lastDebounceTime1 = 0, lastDebounceTime2 = 0, lastDebounceTime3 = 0, lastDebounceTime4 = 0;

// Function Prototypes
void switch_feedback_function();
void my_homekit_setup();
void my_homekit_loop();

void setup() {
    Serial.begin(115200);
    wifi_connect(); // in wifi_info.h
    my_homekit_setup();
}

void loop() {
    my_homekit_loop();
    delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// Access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on1;
extern "C" homekit_characteristic_t cha_switch_on2;
extern "C" homekit_characteristic_t cha_switch_on3;
extern "C" homekit_characteristic_t cha_switch_on4;

// Setter functions called when iOS Home App changes the switch value
void cha_switch_on_setter1(const homekit_value_t value) {
    bool on = value.bool_value;
    cha_switch_on1.value.bool_value = on;  // sync the value
    LOG_D("Switch1: %s", on ? "ON" : "OFF");
    digitalWrite(PIN_SWITCH1, on ? LOW : HIGH);
}

void cha_switch_on_setter2(const homekit_value_t value) {
    bool on = value.bool_value;
    cha_switch_on2.value.bool_value = on;
    LOG_D("Switch2: %s", on ? "ON" : "OFF");
    digitalWrite(PIN_SWITCH2, on ? LOW : HIGH);
}

void cha_switch_on_setter3(const homekit_value_t value) {
    bool on = value.bool_value;
    cha_switch_on3.value.bool_value = on;
    LOG_D("Switch3: %s", on ? "ON" : "OFF");
    digitalWrite(PIN_SWITCH3, on ? LOW : HIGH);
}

void cha_switch_on_setter4(const homekit_value_t value) {
    bool on = value.bool_value;
    cha_switch_on4.value.bool_value = on;
    LOG_D("Switch4: %s", on ? "ON" : "OFF");
    digitalWrite(PIN_SWITCH4, on ? LOW : HIGH);
}

void my_homekit_setup() {
    // Configure pins
    pinMode(PIN_SWITCH1, OUTPUT);
    pinMode(PHY_SWITCH1, INPUT_PULLUP);
    digitalWrite(PIN_SWITCH1, HIGH);

    pinMode(PIN_SWITCH2, OUTPUT);
    pinMode(PHY_SWITCH2, INPUT_PULLUP);
    digitalWrite(PIN_SWITCH2, HIGH);

    pinMode(PIN_SWITCH3, OUTPUT);
    pinMode(PHY_SWITCH3, INPUT_PULLUP);
    digitalWrite(PIN_SWITCH3, HIGH);

    pinMode(PIN_SWITCH4, OUTPUT);
    pinMode(PHY_SWITCH4, INPUT_PULLUP);
    digitalWrite(PIN_SWITCH4, HIGH); // Set to HIGH by default to avoid issues on start

    // Assign setter functions
    cha_switch_on1.setter = cha_switch_on_setter1;
    cha_switch_on2.setter = cha_switch_on_setter2;
    cha_switch_on3.setter = cha_switch_on_setter3;
    cha_switch_on4.setter = cha_switch_on_setter4;

    arduino_homekit_setup(&config);
}

void my_homekit_loop() {
    arduino_homekit_loop();
    switch_feedback_function();

    static uint32_t next_heap_millis = 0;
    const uint32_t t = millis();
    if (t > next_heap_millis) {
        // Show heap info every 5 seconds
        next_heap_millis = t + 5000;
        LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
    }
}

// Handle feedback from physical switches
void switch_feedback_function() {
    // Physical Switch 1
    int currentSwitchState1 = digitalRead(PHY_SWITCH1);
    if (currentSwitchState1 == LOW && switch_ON_Flag1_Previous == 0 && (millis() - lastDebounceTime1 > debounceDelay)) {
        digitalWrite(PIN_SWITCH1, LOW);
        cha_switch_on1.value.bool_value = true;
        homekit_characteristic_notify(&cha_switch_on1, cha_switch_on1.value);
        switch_ON_Flag1_Previous = 1;
        lastDebounceTime1 = millis();
        if (DEBUG_SW) Serial.println("Switch1 - ON");
    } else if (currentSwitchState1 == HIGH && switch_ON_Flag1_Previous == 1 && (millis() - lastDebounceTime1 > debounceDelay)) {
        digitalWrite(PIN_SWITCH1, HIGH);
        cha_switch_on1.value.bool_value = false;
        homekit_characteristic_notify(&cha_switch_on1, cha_switch_on1.value);
        switch_ON_Flag1_Previous = 0;
        lastDebounceTime1 = millis();
        if (DEBUG_SW) Serial.println("Switch1 - OFF");
    }

    // Physical Switch 2
    int currentSwitchState2 = digitalRead(PHY_SWITCH2);
    if (currentSwitchState2 == LOW && switch_ON_Flag2_Previous == 0 && (millis() - lastDebounceTime2 > debounceDelay)) {
        digitalWrite(PIN_SWITCH2, LOW);
        cha_switch_on2.value.bool_value = true;
        homekit_characteristic_notify(&cha_switch_on2, cha_switch_on2.value);
        switch_ON_Flag2_Previous = 1;
        lastDebounceTime2 = millis();
        if (DEBUG_SW) Serial.println("Switch2 - ON");
    } else if (currentSwitchState2 == HIGH && switch_ON_Flag2_Previous == 1 && (millis() - lastDebounceTime2 > debounceDelay)) {
        digitalWrite(PIN_SWITCH2, HIGH);
        cha_switch_on2.value.bool_value = false;
        homekit_characteristic_notify(&cha_switch_on2, cha_switch_on2.value);
        switch_ON_Flag2_Previous = 0;
        lastDebounceTime2 = millis();
        if (DEBUG_SW) Serial.println("Switch2 - OFF");
    }

    // Physical Switch 3
    int currentSwitchState3 = digitalRead(PHY_SWITCH3);
    if (currentSwitchState3 == LOW && switch_ON_Flag3_Previous == 0 && (millis() - lastDebounceTime3 > debounceDelay)) {
        digitalWrite(PIN_SWITCH3, LOW);
        cha_switch_on3.value.bool_value = true;
        homekit_characteristic_notify(&cha_switch_on3, cha_switch_on3.value);
        switch_ON_Flag3_Previous = 1;
        lastDebounceTime3 = millis();
        Serial.println("Switch3 - ON");
    } else if (currentSwitchState3 == HIGH && switch_ON_Flag3_Previous == 1 && (millis() - lastDebounceTime3 > debounceDelay)) {
        digitalWrite(PIN_SWITCH3, HIGH);
        cha_switch_on3.value.bool_value = false;
        homekit_characteristic_notify(&cha_switch_on3, cha_switch_on3.value);
        switch_ON_Flag3_Previous = 0;
        lastDebounceTime3 = millis();
        Serial.println("Switch3 - OFF");
    }

}
