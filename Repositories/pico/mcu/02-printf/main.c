#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

uint32_t global_variable = 0;
const uint32_t constant_variable = 42;

int main() {
    stdio_usb_init();    
    sleep_ms(3000);
    // Инициализация stdio (USB)
    stdio_init_all();
    
    // Задержка для установки USB соединения
    sleep_ms(2000);
    
    printf("\n\n=== 02-printf Project Started ===\n");
    
    while (1) {
        printf("\n--- Loop ---\n");
        
        // 4. Вывод строковой константы
        printf("Hello World!\n");
        
        // 5. Вывод строки с аргументами
        printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
        
        // 6. Вывод системного таймера
        uint64_t timestamp = time_us_64();
        printf("system timestamp: %llu us\n", timestamp);
        
        // 7. Переменная на стеке
        uint32_t stack_variable = 8888;
        printf("stack variable | addr = 0x%X | value = %u\n", &stack_variable, *(&stack_variable));
        printf("stack variable | addr = 0x%X | value = %X\n", &stack_variable, *(&stack_variable));
        printf("stack variable | addr = 0x%X | value = 0x%X\n", &stack_variable, *(&stack_variable));
        
        // 8. Глобальная переменная (инкремент)
        global_variable++;
        printf("global variable | addr = 0x%X | value = %u\n", &global_variable, *(&global_variable));
        
        // 9. Переменная на куче (СПЕЦИАЛЬНО БЕЗ FREE - будет утечка памяти)
        uint32_t* heap_variable = (uint32_t*)malloc(sizeof(uint32_t));
        *heap_variable = 5555;
        printf("heap variable | addr = 0x%X | value = %u\n", heap_variable, *heap_variable);
        
        // 10. Константная переменная
        printf("constant variable | addr = 0x%X | value = %u\n", &constant_variable, *(&constant_variable));
        
        // 11. Строковая константа (DEVICE_NAME)
        printf("constant string | addr = 0x%X | value = 0x%X, [%s]\n", 
               DEVICE_NAME, *((uint32_t*)DEVICE_NAME), DEVICE_NAME);
        
        // 12. Регистр RP2040 (CHIP_ID)
        printf("reg chip id | addr = 0x%X | value = 0x%X\n", 
               0x40000000, *((uint32_t*)0x40000000));
        
        // 13. Глобальная переменная по прямому адресу (адрес может отличаться!)
        // Посмотрите адрес global_variable из вывода выше и подставьте его
        printf("var by addr | addr = 0x%X | value = %u\n", 
               0x20002278, *((uint32_t*)0x20002278));
        
        // 14. Адрес функции main и первые команды
        printf("main function | addr = 0x%X | value = 0x%X\n", 
               main, *((uint32_t*)main));
        
        // 15. Задержка 1 секунда
        sleep_ms(1000);
    }
}
