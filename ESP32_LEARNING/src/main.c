#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "led.h"

static const char *TAG = "WIFI_SNIFFER";

void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type)
{
  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
  const uint8_t *payload = ppkt->payload;

  // SSID está en el beacon frame después del byte 36 (aproximadamente)
  // Esto es simplificado, puede fallar con marcos truncados
  int ssid_len = payload[37];
  if (ssid_len > 32 || ssid_len <= 0)
    return;

  char ssid[33] = {0};
  memcpy(ssid, &payload[38], ssid_len);

  printf("RSSI: %d | MAC: %02x:%02x:%02x:%02x:%02x:%02x | SSID: %s\n",
         ppkt->rx_ctrl.rssi,
         payload[10], payload[11], payload[12],
         payload[13], payload[14], payload[15],
         ssid);
}

void wifi_sniffer_init()
{
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
  ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE)); // Puedes cambiar el canal
}

void app_main()
{
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  printf("Iniciando sniffer Wi-Fi...\n");

  wifi_sniffer_init();

  init_led();
  xTaskCreate(&blink_task, "blink_task", 2048, NULL, 5, NULL);

  while (true)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
