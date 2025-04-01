#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>


class WiFiManager {
public:
    WiFiManager();
    void begin();
    bool isConnected();
    void handleClient();
    void resetWiFi();

private:
    char storedSSID[32];
    char storedPassword[64];

    Preferences preferences;
    WebServer server;

    void startAPMode();
    void connectToWiFi();
    void handleRoot();
    void handleSave();
};



extern WiFiManager wifiManager;



#endif
