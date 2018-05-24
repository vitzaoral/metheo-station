# Solar powered meteo station with ESP8266 Wemos D1 Mini Pro
IoT meteo station system based on ESP8266 Wemos D1 Mini Pro. Checks temperature, humidity and pressure. Data are sent to Blynk and ThingSpeak channel every minute, then is used deep sleep. Project is free to use, coded in C++, created in Visual Code with [PlatfomIO IDE](http://docs.platformio.org/en/latest/ide/vscode.html).

> To build a project, you need to download all the necessary libraries and create the *settings.cpp* file in the *src* folder:
```c++
// Meteo station project settings
struct Settings
{
    const char *thingSpeakWriteApiKey = "Your API Key";
    const unsigned long thingSpeakChannelId = channelId;
    const char *ssid = "WIFI ssid";
    const char *password = "WIFI password";
    const char *blynkAuth = "You Blynk Auth";
};
```

### Powering:
Wemos D1 Mini Pro is powered by NCR18650 3.7V 3400mAh Li-ion battery (with PCB protection). Battery is charged by six 6V 1W solar panels via TP4056 Lithium Battery Charger Module (with protection). Between solars panels and TP4056 is need to insert Step Down Voltage Supply Module (Buck converter), which set voltage to 5V because TP4056 does not work on higher voltage than 6V. On direct sun solars panel can get more than 6 volts so charging is not working because TP4056 automatically turn off itself on higher voltage.

Output from TP4056 is connected to 5V/GND pins on Wemos D1 Mini Pro. It works even if battery has voltage about 2.7V (then is battery automatically turn of by PCB protection). There can be [more effective ways](https://github.com/z2amiller/sensorboard/blob/master/PowerSaving.md) but this solution works well. Without charging device can works about 7 days (data are sent every minute, so for example for 10 minutes limit device will work about one month - it's depend on temperature too). Device checks battery voltage via A0 pin and sent it to the Blynk.

### Currents list:
* [Wemos D1 Mini Pro with antenna](https://www.aliexpress.com/item/WEMOS-D1-Mini-Pro-16M-Bytes-External-Antenna-Connector-ESP8266-WIFI-IoT-Board/32747247131.html)
* [TP4056 Lithium Battery Charger Module Charging Board](https://www.aliexpress.com/item/Automatic-Protection-2PCS-Micro-USB-5V-1A-18650-TP4056-Lithium-Battery-Charger-Module-Charging-Board-With/32216803000.html)
* [6x 6V 1W solar panel](https://www.aliexpress.com/item/Solar-Panel-Portable-Mini-6V-1W-Sunpower-DIY-Module-Panel-System-For-Solar-Lamp-Battery-Toys/32819872730.html)
* [NCR18650B 3400mah Rechargeable battery 3.7V with PCB](https://www.aliexpress.com/item/2-PCS-lot-New-Protected-Original-Rechargeable-battery-18650-NCR18650B-3400mah-with-PCB-3-7V-Free/32628168837.html)
* [18650 Power Battery Storage Case Plastic Box Holder](https://www.aliexpress.com/item/1pcs-18650-Power-Battery-Storage-Case-Plastic-Box-Holder-With-Leads-Oct24/32756928086.html)
* [5819 1A 40V SCHOTTKY DIODE](https://www.aliexpress.com/item/50Pcs-1N5819-5819-1A-40V-SCHOTTKY-DIODE-NEW/32216810320.html)
* [DC-DC 12-24V To 5V 3A Step Down Power Supply Module Voltage](https://www.aliexpress.com/item/5Pcs-TL081-TL081CP-TI-IC-JFET-Input-Operational-Amplifiers-DIP-8-NEW-AL/32629679397.html)
* [Prototype Paper PCB](https://www.aliexpress.com/item/Hot-Free-shipping-1Pcs-5-7-PCB-5x7-PCB-5cm-7cm-DIY-Prototype-Paper-PCB-Universal/32774676055.html)
* [Waterproof Plastic Electronic Project Box](https://www.aliexpress.com/item/115x90x55mm-Waterproof-Plastic-Electronic-Project-Box-Enclosure-Cover-CASE-L057-New-hot/32824079176.html)
* [Plastic humidity sensor project box](https://www.aliexpress.com/item/szomk-plastic-humidity-sensor-project-box-2-pcs-84-27-16mm-diy-wall-mounting-plastic-junction/32586666805.htmll)

### Schema:
![Schema](https://github.com/vitzaoral/metheo-station/blob/master/schema/meteo_schema.jpg)

### PCB circuit:


### Blynk:
