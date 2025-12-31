#ifndef CONFIG_H
#define CONFIG_H

/**
 * =====================================================
 * Configuration centrale ESP32-WROOM-32E
 * =====================================================
 * Benoît Bremaud | MIT License
 */

// ===== DEBUG & VERBOSE =====
#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#define DPRINT(x) Serial.print(x)
#define DPRINTLN(x) Serial.println(x)
#define DPRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
#define DPRINT(x)
#define DPRINTLN(x)
#define DPRINTF(fmt, ...)
#endif

// ===== MODULE VERSIONS =====
#define MODULE_NAME "esp32-embedded-framework"
#define MODULE_VERSION "0.1.0"
#define MODULE_AUTHOR "Benoit Bremaud"

// ===== COMPILATION FLAGS =====
#define ENABLE_WIFI 1           // ✅ Activé pour Phase 2
#define ENABLE_BLE 0            // À faire en Phase 3
#define ENABLE_I2C 1            // Test local possible
#define ENABLE_ADC 1            // Test local possible
#define ENABLE_DEEP_SLEEP 0     // Désactivé en développement

// ===== COMMUNICATION SÉRIE =====
#define SERIAL_BAUD 115200      // Standard ESP32
#define SERIAL_TIMEOUT_MS 1000

// ===== WIFI CONFIGURATION (Quand module disponible) =====
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define WIFI_CONNECT_TIMEOUT_MS 20000
#define WIFI_MODE WIFI_STA

// ===== BLE CONFIGURATION =====
#define BLE_DEVICE_NAME "ESP32-IOT-Hub"
#define BLE_POWER_LEVEL ESP_PWR_LVL_P7  // Maximum power

// ===== GPIO PINOUT (Reference ESP32-WROOM-32E) =====
// ─────────────────────────────────────────────────────

// Sortie LED
#define LED_BUILTIN 2           // GPIO2 (LED interne)
#define LED_PIN 2
#define LED_ACTIVE_HIGH 1

// Entrée Bouton
#define BUTTON_PIN 4            // GPIO4 (Button + Touch input T0)
#define BUTTON_PULLUP 1
#define BUTTON_ACTIVE_LOW 1

// I2C (Flexible GPIO Matrix)
#define I2C_SDA_PIN 21          // GPIO21 (SDA)
#define I2C_SCL_PIN 22          // GPIO22 (SCL)
#define I2C_FREQUENCY 400000    // 400 kHz standard
#define I2C_TIMEOUT_MS 1000

// UART2 (Flexible GPIO Matrix)
#define UART2_RX_PIN 16         // GPIO16 (RX)
#define UART2_TX_PIN 17         // GPIO17 (TX)
#define UART2_BAUD 9600

// SPI (VSPI = SPI3 - Recommandé)
#define SPI_CLK_PIN 18          // GPIO18 (CLK)
#define SPI_MOSI_PIN 23         // GPIO23 (D/MOSI)
#define SPI_MISO_PIN 19         // GPIO19 (Q/MISO)
#define SPI_CS_PIN 5            // GPIO5 (CS)
#define SPI_FREQUENCY 10000000  // 10 MHz

// Analogique (ADC1 = pas d'interference WiFi)
#define ADC_PIN 35              // GPIO35 (ADC1_CH7, input-only)
#define ADC_CHANNEL ADC1_CHANNEL_7
#define ADC_RESOLUTION 12       // 12-bit (0-4095)
#define ADC_ATTENUATION ADC_ATTEN_DB_11  // Full range 0-3.3V
#define ADC_SAMPLES 10          // Nombre de samples pour moyenne

// Temperature Sensor (RTC)
#define TEMP_SENSOR_ENABLED 1

// ===== MEMORY CONFIGURATION =====
#define HEAP_MONITOR_INTERVAL_MS 10000  // Afficher free heap tous les 10s
#define STACK_SIZE_BYTES 4096

// ===== POWER MANAGEMENT =====
#define CPU_FREQ_MHZ 240        // 240 MHz (max), peut être réduit
#define SLEEP_SECONDS 10        // Deep sleep interval
#define WAKE_THRESHOLD_VOLTAGE 2.8  // Seuil de batterie faible (volts)

// ===== TIMING & TASKS =====
#define LOOP_DELAY_MS 100       // Main loop sleep
#define HEARTBEAT_INTERVAL_MS 5000

// ===== BUFFER SIZES =====
#define BUFFER_SIZE_SMALL 64
#define BUFFER_SIZE_MEDIUM 256
#define BUFFER_SIZE_LARGE 1024

// ===== COMPILER ASSERTIONS =====
#if DEBUG
#define ASSERT(x) do { if(!(x)) { DPRINTLN("ASSERT FAILED: " #x); while(1); } } while(0)
#else
#define ASSERT(x)
#endif

// ===== INLINE UTILITIES =====

/**
 * @brief Format une taille de mémoire de manière lisible
 * @param bytes Nombre de bytes
 * @return String formatée (ex: "1.23 KB")
 */
static inline String formatBytes(uint32_t bytes) {
    if (bytes < 1024) return String(bytes) + " B";
    else if (bytes < 1024 * 1024) return String(bytes / 1024.0, 2) + " KB";
    else return String(bytes / (1024.0 * 1024), 2) + " MB";
}

/**
 * @brief Afficher les infos système au démarrage
 */
static inline void printSystemInfo() {
    DPRINTLN("\n╔════════════════════════════════════════╗");
    DPRINTLN("║       ESP32-WROOM-32E System Info      ║");
    DPRINTLN("╚════════════════════════════════════════╝");

    DPRINTF("Module:       %s v%s\n", MODULE_NAME, MODULE_VERSION);
    DPRINTF("Author:       %s\n", MODULE_AUTHOR);
    DPRINTF("Compile Date: %s %s\n", __DATE__, __TIME__);
    DPRINTF("CPU Freq:     %d MHz\n", getCpuFrequencyMhz());
    DPRINTF("Total SRAM:   %s\n", formatBytes(ESP.getHeapSize()).c_str());
    DPRINTF("Free SRAM:    %s\n", formatBytes(ESP.getFreeHeap()).c_str());
    DPRINTF("Flash:        %s\n", formatBytes(ESP.getFlashChipSize()).c_str());
    DPRINTF("Chip ID:      0x%X\n", (uint32_t)ESP.getEfuseMac());

    #if ENABLE_WIFI
    DPRINTLN("WiFi:         ENABLED");
    #else
    DPRINTLN("WiFi:         DISABLED");
    #endif

    #if ENABLE_BLE
    DPRINTLN("BLE:          ENABLED");
    #else
    DPRINTLN("BLE:          DISABLED");
    #endif

    DPRINTLN("╚════════════════════════════════════════╝\n");
}

#endif // CONFIG_H
