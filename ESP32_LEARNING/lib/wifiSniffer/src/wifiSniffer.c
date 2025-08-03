#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "WIFI_SNIFFER";

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
    const uint8_t *addr = ppkt->payload;

    const uint8_t *src_mac = addr + 10;
    int rssi = ppkt->rx_ctrl.rssi;

    ESP_LOGI(TAG, "RSSI: %d dBm - SRC MAC: %02X:%02X:%02X:%02X:%02X:%02X",
             rssi,
             src_mac[0], src_mac[1], src_mac[2],
             src_mac[3], src_mac[4], src_mac[5]);
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
