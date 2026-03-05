#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"

#define LED_PIN 25
#define DEVICE_NAME "Pico-Echo"
#define DEVICE_VERSION "v1.0.0"

int main() {
    // Инициализация USB
    stdio_usb_init();
    sleep_ms(3000);
    
    stdio_init_all();
    
    // Инициализация светодиода
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);  // Начинаем с выключенным
    
    printf("\n\n================================\n");
    printf("   02-echo Device Control\n");
    printf("================================\n");
    printf("Commands:\n");
    printf("  'e' - Enable LED  (turn ON)\n");
    printf("  'd' - Disable LED (turn OFF)\n");
    printf("  'v' - Show device version\n");
    printf("  Any other key - show ASCII code\n");
    printf("--------------------------------\n\n");
    
    while (1) {
        // Блокирующее считывание символа из USB
        char symbol = getchar();
        
        // Обработка команд
        switch(symbol) {
            case 'e':  // Enable LED
                gpio_put(LED_PIN, true);
                printf("[CMD] LED ENABLED (ON)\n");
                break;
                
            case 'd':  // Disable LED
                gpio_put(LED_PIN, false);
                printf("[CMD] LED DISABLED (OFF)\n");
                break;
                
            case 'v':  // Show version
                printf("[CMD] Device: %s, Version: %s\n", 
                       DEVICE_NAME, DEVICE_VERSION);
                break;
                
            default:   // Show ASCII info for any other key
                printf("received char: '%c' [ ASCII code: %d (0x%X) ]\n", 
                       symbol, symbol, symbol);
                break;
        }
    }
}