#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_SSD1306.h>

SHT3X sht30(0x45);
Adafruit_BMP085 bmp;

#define OLED_RESET 0 // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

// check wright library https://github.com/mcauser/Adafruit_SSD1306/tree/esp8266-64x48
#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

float shtTemperature = 0;
float shtHumidity = 0;
float bmpPresure = 0;
float bmpTemperature = 0;
float bmpRealAltitude = 0;

void setup()
{
    Serial.begin(9600);
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
    display.display();
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1)
        {
        }
    }
}

void printMetheoDataToDisplay()
{
  // temperature SHT
  display.print("Ts:");
  display.print(shtTemperature);
  display.println(" C");
  // temperature BMP
  display.print("Tb:");
  display.print(bmpTemperature);
  display.println(" C");
  // humidity
  display.print("H:");
  display.print(shtHumidity);
  display.println(" %");
  // presure
  display.print("P:");
  display.print(bmpPresure / 100);
  display.println(" h");
  // altitude
  display.print("A:");
  display.print(bmpRealAltitude);
  display.println(" m/n");
}

void getMetheoData() {
  shtTemperature = 0;
  shtHumidity = 0;
  bmpPresure = bmp.readPressure();
  bmpTemperature = bmp.readTemperature();
  bmpRealAltitude = bmp.readAltitude(101500);

  if (sht30.get() == 0)
    {
        shtTemperature = sht30.cTemp;
        shtHumidity = sht30.humidity;
    }
    else
    {
        Serial.println("Error!");
    }
}

void printToDisplay()
{
  // set display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  printMetheoDataToDisplay();

  // display all
  display.display();
}

void loop()
{

    getMetheoData();
    printToDisplay();
    delay(3000);
}