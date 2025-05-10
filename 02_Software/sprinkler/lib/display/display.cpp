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

    display.setCursor(0, this->time_line * 10);
    display.print("Time:");

    display.setCursor(0, this->pump1_line * 10);
    display.print(">");

    display.setCursor(10, this->pump1_line * 10);
    display.print("Pump1");

    display.setCursor(10, this->pump2_line * 10);
    display.print("Pump2");

    display.setCursor(this->start_stop_position, this->pump1_line * 10);
    display.print("STOP");

    display.setCursor(this->start_stop_position, this->pump2_line * 10);
    display.print("STOP");

    display.display();
    delay(100);
}

void DisplayManager::change_pump()
{
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.fillRect(0, this->selected_pump_line * 10, 6, 8, SSD1306_BLACK);

    this->selected_pump_line = (this->selected_pump_line == this->pump1_line) ? this->pump2_line : this->pump1_line;

    display.setCursor(0, this->selected_pump_line * 10);
    display.print(">");

    this->selected_pump = this->selected_pump_line - 2; // 2 is the offeset between the line and the pump selected

    display.display();
    delay(100);
}

void DisplayManager::running_pump1()
{
    display.fillRect(this->start_stop_position, this->selected_pump_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(this->start_stop_position, this->selected_pump_line * 10);
    display.print("START");
    display.display();
    delay(100);
}

void DisplayManager::running_pump2()
{
    display.fillRect(this->start_stop_position, this->selected_pump_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(this->start_stop_position, this->selected_pump_line * 10);
    display.print("START");
    display.display();
    delay(100);
}

void DisplayManager::stop_pump1()
{
    display.fillRect(this->start_stop_position, this->selected_pump_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(this->start_stop_position, this->selected_pump_line * 10);
    display.print("STOP");
    display.display();
    delay(100);
}

void DisplayManager::stop_pump2()
{
    display.fillRect(this->start_stop_position, this->selected_pump_line * 10, 30, 8, SSD1306_BLACK);
    display.setCursor(this->start_stop_position, this->selected_pump_line * 10);
    display.print("STOP");
    display.display();
    delay(100);
}
