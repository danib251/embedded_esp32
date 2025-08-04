#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "wifiSniffer.h"
// #include "udpSender.h"
#include "webserver.h"
#include "esp_log.h"

void inicialications()
{
  // Aquí puedes agregar otras inicializaciones si es necesario
  ESP_LOGI("main_task", "Iniciando el sistema...");
  init_led();
  // wifi_sniffer_init();
}
void app_main()
{

  inicialications();
  webserver_main();

  xTaskCreate(&blink_task, "blink_task", 2048, NULL, 5, NULL);
  // xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);

  // xTaskCreate(&sniffer_task, "sniffer_task", 4096, NULL, 5, NULL);
  while (true)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
