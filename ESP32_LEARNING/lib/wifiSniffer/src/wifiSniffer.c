#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "string.h"

static const char *TAG = "SNIFFER";

typedef struct
{
    uint8_t frame_control[2];
    uint8_t duration[2];
    uint8_t addr1[6];
    uint8_t addr2[6]; // Source MAC
    uint8_t addr3[6];
    uint8_t sequence_ctrl[2];
    uint8_t timestamp[8];
    uint16_t beacon_interval;
    uint16_t capability_info;
    // Following is tagged parameters (SSID, Supported rates, etc)
} wifi_ieee80211_packet_t;

void wifi_sniffer_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

// --- Tarea 1: Logger
void logger_task(void *pvParameter)
{
    while (1)
    {
        ESP_LOGI("LOG", "esplog: Sistema activo");
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 segundos
    }
}

// --- Callback para el modo promiscuo (aún vacío)

static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type)
{
    const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
    const uint8_t *payload = ppkt->payload;
    int rssi = ppkt->rx_ctrl.rssi;

    // Solo paquetes management tipo beacon (subtype 8)
    // frame_control byte 0: bits 4-7 subtype, bits 0-3 type
    uint8_t frame_ctrl = payload[0];
    uint8_t frame_type = (frame_ctrl >> 2) & 0x3;
    uint8_t frame_subtype = (frame_ctrl >> 4) & 0xF;

    if (frame_type == 0 && frame_subtype == 8)
    { // management beacon
        // Dirección MAC de origen (addr2)
        const uint8_t *src_mac = payload + 10;

        // La SSID suele estar después de 36 bytes desde el inicio del payload (cabeceras)
        // Estructura de etiquetas:
        // offset 36: tag_number (0=SSID), tag_length, tag_data
        const uint8_t *tags = payload + 36;
        uint8_t tag_number = tags[0];
        uint8_t tag_length = tags[1];
        char ssid[33] = {0};

        if (tag_number == 0 && tag_length <= 32)
        {
            memcpy(ssid, &tags[2], tag_length);
            ESP_LOGI(TAG, "Beacon RSSI:%d MAC:%02X:%02X:%02X:%02X:%02X:%02X SSID:%s",
                     rssi,
                     src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5],
                     ssid);
        }
    }
}

// --- Tarea 2: Sniffer
void sniffer_task(void *pvParameter)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    esp_wifi_set_promiscuous_rx_cb(wifi_sniffer_packet_handler);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera activa
    }
}
