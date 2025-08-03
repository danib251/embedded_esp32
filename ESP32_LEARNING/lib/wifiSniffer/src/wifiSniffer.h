#ifndef WIFI_SNIFFER_H
#define WIFI_SNIFFER_H

#include "esp_err.h"

/**
 * @brief Inicializa el subsistema de sniffer Wi-Fi (modo promiscuo).
 * Debe llamarse una sola vez antes de empezar a capturar.
 */
void wifi_sniffer_init(void);

void logger_task(void *pvParameter);

void sniffer_task(void *pvParameter);

#endif // WIFI_SNIFFER_H
