#include "OledDisplay.h"

Adafruit_SSD1306 display(OLED_RESET);

// Initialize OLED Shield 64x48
OledDisplay::OledDisplay()
{
   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
    display.display();
}

void OledDisplay::printMetheoDataToDisplay(MetheoData data) {
    prepareDisplay();
    
    // temperature SHT
    display.print("Ts:");
    display.print(data.shtTemperature);
    display.println(" C");
    // temperature BMP
    display.print("Tb:");
    display.print(data.bmpTemperature);
    display.println(" C");
    // humidity
    display.print("H:");
    display.print(data.shtHumidity);
    display.println(" %");
    // presure
    display.print("P:");
    display.print(data.bmpPresure / 100);
    display.println(" h");
    // altitude
    display.print("A:");
    display.print(data.bmpRealAltitude);
    display.println(" m/n");

    display.display();
}

// clear display, set size and cursor
void OledDisplay::prepareDisplay(void)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
}