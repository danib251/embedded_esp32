
#ifndef LED_H
#define LED_H

#include "esp_err.h"

// Inicializa el pin como salida
esp_err_t init_led(void);

// Tarea que hace parpadear el LED
void blink_task(void *pvParameter);

#endif // LED_H
