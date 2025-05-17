#include <display.h>
#include <logger.h>

DisplayManager::DisplayManager()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

bool DisplayManager::begin()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // default I2C address
        return false;
    }
    display.clearDisplay();
    display.display();
    return true;
}

void DisplayManager::main_screen()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Sprinker v0.1");

    display.setCursor(10, time_line * 10);
    display.print("Time:");

    display.setCursor(0, time_line * 10);
    display.print(">");

    display.setCursor(10, pump1_line * 10);
    display.print("Pump1");

    display.setCursor(10, pump2_line * 10);
    display.print("Pump2");

    display.setCursor(firts_column_pos, pump1_line * 10);
    display.print("STOP");

    display.setCursor(firts_column_pos, pump2_line * 10);
    display.print("STOP");

    display.display();
    delay(100);
}

void DisplayManager::main_screen(DateTime now)
{

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Sprinker v0.1");

    display.setCursor(0, time_line * 10);
    display.print(">");

    display.setCursor(10, time_line * 10);
    display.print("Time:");

    display.setCursor(firts_column_pos, time_line * 10);
    String hour = String(now.hour());
    String min = String(now.minute());
    String time = hour + ":" + min;
    display.print(time);

    display.setCursor(10, pump1_line * 10);
    display.print("Pump1");

    display.setCursor(10, pump2_line * 10);
    display.print("Pump2");

    display.setCursor(firts_column_pos, pump1_line * 10);
    display.print("STOP");

    display.setCursor(firts_column_pos, pump2_line * 10);
    display.print("STOP");

    display.display();
    delay(100);
}

void DisplayManager::change_line()
{
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.fillRect(0, selected_line * 10, 6, 8, SSD1306_BLACK);

    if (selected_line == time_line){
        selected_line = pump1_line;
    } else if (selected_line == pump1_line){
        selected_line = pump2_line;
    } else if (selected_line == pump2_line){
        selected_line = time_line;
    }

    display.setCursor(0, selected_line * 10);
    display.print(">");


    selected_pump = selected_line - 2; // 2 is the offeset between the line and the pump selected


    display.display();
    delay(100);
}

void DisplayManager::running_pump1()
{
    display.fillRect(firts_column_pos, selected_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, selected_line * 10);
    display.print("START");
    display.display();
    delay(100);
}

void DisplayManager::running_pump2()
{
    display.fillRect(firts_column_pos, selected_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, selected_line * 10);
    display.print("START");
    display.display();
    delay(100);
}

void DisplayManager::stop_pump1()
{
    display.fillRect(firts_column_pos, selected_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, selected_line * 10);
    display.print("STOP");
    display.display();
    delay(100);
}

void DisplayManager::stop_pump2()
{
    display.fillRect(firts_column_pos, selected_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, selected_line * 10);
    display.print("STOP");
    display.display();
    delay(100);
}

void DisplayManager::update_time(DateTime now)
{
    display.fillRect(firts_column_pos, time_line * 10, 6 * 5, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, time_line * 10);
    String hour = String(now.hour());
    String min = String(now.minute());
    String time = hour + ":" + min;
    display.print(time);
    display.display();
    delay(100);
}

void DisplayManager::sleep_screen()
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Deep sleep.");
    display.display();
    delay(100);
}
