#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// https://lastminuteengineers.com/oled-display-esp32-tutorial/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin


class DisplayManager{
    public:
        DisplayManager();

        bool begin();
        void main_screen();
        
        void change_pump();
    
    private:
        Adafruit_SSD1306 display;
        
        const uint8_t pump1_line = 3;
        const uint8_t pump2_line = 4;
        uint8_t selected_pump_line = 3;
};


//extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//void hello_world(Adafruit_SSD1306 &display);


#endif // DISPLAY_H