#include <display.h>

DisplayManager::DisplayManager()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

bool DisplayManager::begin()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
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

    // Time line
    display.setCursor(0, time_line * 10);
    display.print(">");

    display.setCursor(10, time_line * 10);
    display.print("Time:");

    display.setCursor(firts_column_pos, time_line * 10);
    String hour = String(now.hour());
    String min = String(now.minute());
    String time = hour + ":" + min;
    display.print(time);

    // Pump1 line
    display.setCursor(10, pump1_line * 10);
    display.print("Pump1");

    display.setCursor(firts_column_pos, pump1_line * 10);
    display.print("STOP");

    // Pump2 line
    display.setCursor(10, pump2_line * 10);
    display.print("Pump2");

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

    if (selected_line == time_line)
    {
        selected_line = pump1_line;
    }
    else if (selected_line == pump1_line)
    {
        selected_line = pump2_line;
    }
    else if (selected_line == pump2_line)
    {
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
    display.fillRect(firts_column_pos, pump1_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, pump1_line * 10);
    display.print("START");
    display.display();
    delay(100);
}

void DisplayManager::running_pump2()
{
    display.fillRect(firts_column_pos, pump2_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, pump2_line * 10);
    display.print("START");
    display.display();
    delay(100);
}

void DisplayManager::stop_pump1()
{
    display.fillRect(firts_column_pos, pump1_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, pump1_line * 10);
    display.print("STOP");
    display.display();
    delay(100);
}

void DisplayManager::stop_pump2()
{
    display.fillRect(firts_column_pos, pump2_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, pump2_line * 10);
    display.print("STOP");
    display.display();
    delay(100);
}

void DisplayManager::update_time(DateTime now)
{
    String hour = "";
    String min = "";
    String sec = "";
    if (now.hour() < 10)
    {
        hour = "0" + String(now.hour());
    }
    else
    {
        hour = String(now.hour());
    }
    if (now.minute() < 10)
    {
        min = "0" + String(now.minute());
    }
    else
    {
        min = String(now.minute());
    }
    if (now.second() < 10)
    {
        sec = "0" + String(now.second());
    }
    else
    {
        sec = now.second();
    }
    display.fillRect(firts_column_pos, time_line * 10, 6 * 8, 8, SSD1306_BLACK);
    display.setCursor(firts_column_pos, time_line * 10);

    String time = hour + ":" + min + ":" + sec;
    display.print(time);
    display.display();
    delay(100);
}

void DisplayManager::sleep_screen()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Deep sleep.");
    display.display();
    delay(100);
}

void DisplayManager::update_pump_timer(uint8_t pump_numb, DailyTimer &timer)
{
    debugln("DSP-Update pump timer.");
    String hour_str = "";
    String min_str = "";

    if (timer.get_hour() < 10)
    {
        hour_str = "0" + String(timer.get_hour());
    }
    else
    {
        hour_str = String(timer.get_hour());
    }

    if (timer.get_min() < 10)
    {
        min_str = "0" + String(timer.get_min());
    }
    else
    {
        min_str = String(timer.get_min());
    }

    if (pump_numb == 1)
    {
        display.fillRect(second_column_pos, selected_line * 10, 30, 8, SSD1306_BLACK);
        display.setCursor(second_column_pos, pump1_line * 10);
        display.print(hour_str + ":" + min_str);
    }
    else if (pump_numb == 2)
    {
        display.fillRect(second_column_pos, selected_line * 10, 30, 8, SSD1306_BLACK);
        display.setCursor(second_column_pos, pump2_line * 10);
        display.print(hour_str + ":" + min_str);
    }

    display.display();
    delay(100);
}
