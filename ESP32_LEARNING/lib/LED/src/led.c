
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define NUM_PIN 2 // Cambia al pin que estés usando
uint8_t led_state = 0;
void blink_task(void *pvParameter)
{
    while (1)
    {
        led_state = !led_state;
        gpio_set_level((gpio_num_t)NUM_PIN, led_state);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Cambia el tiempo de parpadeo si es necesario
    }
}
esp_err_t init_led(void)
{

    gpio_reset_pin((gpio_num_t)NUM_PIN);
    gpio_set_direction((gpio_num_t)NUM_PIN, GPIO_MODE_OUTPUT);
    return ESP_OK;
}