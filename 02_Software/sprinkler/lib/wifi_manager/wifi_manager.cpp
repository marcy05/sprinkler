#include <wifi_manager.h>
#include <hw_abstraction.h>
#include <logger.h>


WiFiManager::WiFiManager() : server(80) {}

void WiFiManager::begin() {
    preferences.begin("wifi", false);
    turn_off_wifi_led();

    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");

    if (ssid.length() > 0 && password.length() > 0) {
        ssid.toCharArray(storedSSID, sizeof(storedSSID));
        password.toCharArray(storedPassword, sizeof(storedPassword));
        connectToWiFi();
    } else {
        debugln("No Wi-Fi credentials found. Starting AP mode.");
        startAPMode();
    }
}

void WiFiManager::connectToWiFi() {
    WiFi.begin(storedSSID, storedPassword);
    debug("Connecting to Wi-Fi: ");
    debugln(storedSSID);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 20) {
        turn_on_wifi_led();
        delay(250);
        turn_off_wifi_led();
        delay(250);
        debug(".");
        attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        debugln("\nWi-Fi Connected!");
        debug("IP Address: ");
        debugln(WiFi.localIP());
        turn_on_wifi_led();
    } else {
        debugln("\nWi-Fi Failed. Starting AP mode.");
        debugln("Resetting Wi-Fi credentials...");
        debugln("Discarding saved information...");
        preferences.begin("wifi", false);
        preferences.remove("ssid");     // Remove stored SSID
        preferences.remove("password"); // Remove stored Password
        preferences.end();
        debugln("Information cancelled.");
        turn_off_wifi_led();
        startAPMode();
    }
}

void WiFiManager::startAPMode() {
    WiFi.softAP("ESP32_Config", "12345678"); // Creates an Access Point
    IPAddress IP = WiFi.softAPIP();
    debug("AP Mode IP: ");
    debugln(IP);

    server.on("/", std::bind(&WiFiManager::handleRoot, this));
    server.on("/save", HTTP_POST, std::bind(&WiFiManager::handleSave, this));
    server.begin();
}

void WiFiManager::handleRoot() {
    int numNetworks = WiFi.scanNetworks(); // Scan for Wi-Fi networks
    Serial.println("Wi-Fi scan complete.");
    
    String html = "<html><head><title>Wi-Fi Setup</title></head><body>";
    html += "<h1>Configure Wi-Fi</h1>";
    html += "<form action='/save' method='POST'>";
    html += "SSID: <select name='ssid'>";

    if (numNetworks == 0) {
        html += "<option>No networks found</option>";
    } else {
        for (int i = 0; i < numNetworks; i++) {
            html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + "dBm)</option>";
        }
    }

    html += "</select><br>";
    html += "Password: <input type='password' name='password'><br>";
    html += "<input type='submit' value='Save'>";
    html += "</form></body></html>";

    server.send(200, "text/html", html);
}

void WiFiManager::handleSave() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    if (ssid.length() > 0 && password.length() > 0) {
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        server.send(200, "text/html", "<h1>Saved! Restarting...</h1>");
        delay(2000);
        ESP.restart();
    } else {
        server.send(400, "text/html", "<h1>Error: Missing SSID or Password</h1>");
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::handleClient() {
    server.handleClient();
}

void WiFiManager::resetWiFi()
{
    debugln("Resetting Wi-Fi credentials...");

    preferences.begin("wifi", false);
    preferences.remove("ssid");     // Remove stored SSID
    preferences.remove("password"); // Remove stored Password
    preferences.end();

    debugln("Wi-Fi credentials erased. Restarting...");
    delay(2000);
    ESP.restart();
}

WiFiManager wifiManager;

