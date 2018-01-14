#ifndef __OledDisplay_H
#define __OledDisplay_H

#include <Adafruit_SSD1306.h>
#include <MetheoData.h>

#define OLED_RESET 0 // GPIO0

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

class OledDisplay
{
  public:
    OledDisplay();
    void printMetheoDataToDisplay(MetheoData data);

  private:
    void prepareDisplay(void);
};

#endif
