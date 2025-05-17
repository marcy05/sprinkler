#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <daily_timer.h>

// https://lastminuteengineers.com/oled-display-esp32-tutorial/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin


class DisplayManager{
    public:
        DisplayManager();

        bool begin();
        void main_screen();
        void main_screen(DateTime now);
        
        void change_line();

        void running_pump1();
        void running_pump2();

        void stop_pump1();
        void stop_pump2();

        void update_time(DateTime now);

        void sleep_screen();

        void update_pump_timer(uint8_t pump_numb, DailyTimer &timer);
        
        uint8_t selected_pump = 0;

        bool pump1_last_status = 0;
        bool pump2_last_status = 0;

        const uint8_t time_line = 2;
        const uint8_t pump1_line = 3;
        const uint8_t pump2_line = 4;
        uint8_t selected_line = 2;
    
    private:
        Adafruit_SSD1306 display;
        
        const uint8_t firts_column_pos = 50;

        const uint8_t second_column_pos = 90;
};


//extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//void hello_world(Adafruit_SSD1306 &display);


#endif // DISPLAY_H
