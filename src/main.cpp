#include <Arduino.h>
#include <Config.h>

#if ENABLE_WIFI
  #include <WiFi.h>
#endif

/**
 * @brief Initialiser la connexion WiFi
 */
void setupWiFi() {
    #if ENABLE_WIFI
        DPRINTLN("\n[WiFi] Démarrage de la connexion...");
        DPRINTF("[WiFi] Connexion à SSID: %s\n", WIFI_SSID);

        WiFi.mode(WIFI_MODE);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        unsigned long startAttempt = millis();
        int dotCount = 0;

        // Boucle d'attente avec timeout (non-bloquante avec retours)
        while (WiFi.status() != WL_CONNECTED &&
               millis() - startAttempt < WIFI_CONNECT_TIMEOUT_MS) {
            DPRINT(".");
            dotCount++;
            if (dotCount % 40 == 0) DPRINTLN(""); // Nouvelle ligne tous les 40 points
            delay(500);
        }
        DPRINTLN("");

        if (WiFi.status() == WL_CONNECTED) {
            DPRINTLN("[WiFi] ✅ Connecté avec succès !");
            DPRINT("[WiFi] IP Address: ");
            DPRINTLN(WiFi.localIP());
            DPRINTF("[WiFi] Signal (RSSI): %d dBm\n", WiFi.RSSI());
            DPRINTF("[WiFi] MAC Address: %s\n", WiFi.macAddress().c_str());
        } else {
            DPRINTLN("[WiFi] ❌ Échec de connexion (Timeout)");
            DPRINTLN("[WiFi] Mode hors-ligne / Retry actif");
        }
    #else
        DPRINTLN("[WiFi] Module désactivé (ENABLE_WIFI=0 dans config.h)");
    #endif
}

/**
 * @brief Initialisation du système
 */
void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000); // Laisser le temps au moniteur série de s'ouvrir

    // Afficher les infos système (fonction dans config.h)
    printSystemInfo();

    // Configurer la LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Lancer le WiFi
    setupWiFi();

    DPRINTLN("[Setup] Initialisation terminée ✅\n");
}

/**
 * @brief Boucle principale
 */
void loop() {
    // Faire clignoter la LED pour montrer que le programme tourne
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

    // Vérification périodique du statut WiFi
    #if ENABLE_WIFI
        static unsigned long lastCheck = 0;
        if (millis() - lastCheck > 10000) { // Vérifier toutes les 10 secondes
            lastCheck = millis();

            if (WiFi.status() == WL_CONNECTED) {
                DPRINTF("[Loop] WiFi OK - IP: %s | RSSI: %d dBm\n",
                        WiFi.localIP().toString().c_str(), WiFi.RSSI());
            } else {
                DPRINTLN("[Loop] ⚠️ WiFi Déconnecté - Tentative de reconnexion...");
                WiFi.reconnect(); // Essayer de reconnecter automatiquement
            }
        }
    #endif

    // Afficher l'état de la mémoire périodiquement
    static unsigned long lastHeapCheck = 0;
    if (millis() - lastHeapCheck > HEAP_MONITOR_INTERVAL_MS) {
        lastHeapCheck = millis();
        DPRINTF("[Memory] Free SRAM: %s\n", formatBytes(ESP.getFreeHeap()).c_str());
    }
}
