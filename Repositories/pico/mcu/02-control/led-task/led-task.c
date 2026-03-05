#include "led-task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdint.h>  // Для uint32_t

// Константы
const uint LED_PIN = 25;
static uint LED_BLINK_PERIOD_US = 500000;  // 500 ms (теперь static)

// Переменные состояния
static uint64_t led_ts = 0;
static led_state_t led_state = LED_STATE_OFF;

void led_task_init()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    led_state = LED_STATE_OFF;
    gpio_put(LED_PIN, 0);
    led_ts = 0;
    
    printf("LED Task initialized (period: %d ms)\n", LED_BLINK_PERIOD_US / 1000);
}

void led_task_state_set(led_state_t state)
{
    led_state = state;
    printf("LED state set to: %d\n", state);
    
    if (state == LED_STATE_ON) {
        gpio_put(LED_PIN, 1);
    } else if (state == LED_STATE_OFF) {
        gpio_put(LED_PIN, 0);
    }
}

// Новая функция для установки периода мигания
void led_task_set_blink_period_ms(uint32_t period_ms)
{
    if (period_ms < 10) {  // Защита от слишком маленького периода
        printf("Period too small, minimum 10 ms\n");
        return;
    }
    
    LED_BLINK_PERIOD_US = period_ms * 1000;  // Переводим в микросекунды
    printf("LED blink period set to: %d ms (%d us)\n", period_ms, LED_BLINK_PERIOD_US);
    
    // Сбрасываем таймер, чтобы не было скачков
    led_ts = 0;
}

void led_task_handle()
{
    switch (led_state)
    {
        case LED_STATE_OFF:
            gpio_put(LED_PIN, 0);
            break;
            
        case LED_STATE_ON:
            gpio_put(LED_PIN, 1);
            break;
            
        case LED_STATE_BLINK:
            if (time_us_64() > led_ts)
            {
                // Переключаем светодиод
                gpio_put(LED_PIN, !gpio_get(LED_PIN));
                
                // Устанавливаем следующее время переключения (полпериода)
                led_ts = time_us_64() + (LED_BLINK_PERIOD_US / 2);
            }
            break;
            
        default:
            break;
    }
}