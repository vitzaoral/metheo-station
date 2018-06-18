#include "PowerController.h"

Adafruit_INA219 ina219;

// Initialize and get metheorological data
PowerController::PowerController()
{
    pinMode(A0, INPUT);
    // Initialize the INA219.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    ina219.begin();
    // To use a slightly lower 32V, 1A range (higher precision on amps):
    //ina219.setCalibration_32V_1A();
    // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
    ina219.setCalibration_16V_400mA();
}

/*
 * Voltage divider of 100k+220k over 100k
 * gives 100/420k
 * ergo 4.2V -> 1Volt
 * Max input on A0=1Volt ->1023
 * 4.2*(Raw/1023)=Vbat
 */
void PowerController::setBatteryVoltage()
{
    int raw = analogRead(A0);
    float volt = raw / 1023.0;
    batteryVoltage = volt * 4.2;

    Serial.print("Battery Voltage:   ");
    Serial.print(batteryVoltage);
    Serial.println(" V");
}

void PowerController::setData(void)
{
    shuntVoltage = ina219.getShuntVoltage_mV();
    busVoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadVoltage = busVoltage + (shuntVoltage / 1000);

    Serial.print("Bus Voltage:   ");
    Serial.print(busVoltage);
    Serial.println(" V");
    Serial.print("Shunt Voltage: ");
    Serial.print(shuntVoltage);
    Serial.println(" mV");
    Serial.print("Load Voltage:  ");
    Serial.print(loadVoltage);
    Serial.println(" V");
    Serial.print("Current:       ");
    Serial.print(current_mA);
    Serial.println(" mA");
    Serial.print("Power:         ");
    Serial.print(power_mW);
    Serial.println(" mW");

    setBatteryVoltage();
}