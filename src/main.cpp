#include <Arduino.h>
#include <Config.h>

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    // Initialiser LED
    pinMode(LED_PIN, OUTPUT);
    
    // Afficher les infos système
    printSystemInfo();
    
    DPRINTLN("✅ Setup complete!");
}

void loop() {
    // Blink LED
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    
    // Afficher la mémoire libre tous les 10s
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > HEAP_MONITOR_INTERVAL_MS) {
        DPRINTF("Free Heap: %s\n", formatBytes(ESP.getFreeHeap()).c_str());
        lastCheck = millis();
    }
}
