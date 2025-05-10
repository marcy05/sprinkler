#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

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

        void running_pump1();
        void running_pump2();

        void stop_pump1();
        void stop_pump2();
        
        uint8_t selected_pump = 1;

        bool pump1_last_status = 0;
        bool pump2_last_status = 0;
    
    private:
        Adafruit_SSD1306 display;
        
        const uint8_t time_line = 2;
        const uint8_t pump1_line = 3;
        const uint8_t pump2_line = 4;
        uint8_t selected_pump_line = 3;

        const uint8_t start_stop_position = 50;
};


//extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//void hello_world(Adafruit_SSD1306 &display);


#endif // DISPLAY_H
