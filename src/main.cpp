#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <MetheoData.h>

#define OLED_RESET 0 // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

MetheoData metheoData;

void printMetheoDataToDisplay()
{
    // temperature SHT
    display.print("Ts:");
    display.print(metheoData.shtTemperature);
    display.println(" C");
    // temperature BMP
    display.print("Tb:");
    display.print(metheoData.bmpTemperature);
    display.println(" C");
    // humidity
    display.print("H:");
    display.print(metheoData.shtHumidity);
    display.println(" %");
    // presure
    display.print("P:");
    display.print(metheoData.bmpPresure / 100);
    display.println(" h");
    // altitude
    display.print("A:");
    display.print(metheoData.bmpRealAltitude);
    display.println(" m/n");
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


void setup()
{
    Serial.begin(9600);
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
    display.display();
}


void loop()
{
    metheoData.setData();
    printToDisplay();
    delay(3000);
    Serial.println("LOOP");
}