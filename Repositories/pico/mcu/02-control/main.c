#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "Pico-Control"
#define DEVICE_VRSN "v2.0.0"

// ============ ПРОТОТИПЫ ============
void version_callback(const char* args);
void help_callback(const char* args);
void led_on_callback(const char* args);
void led_off_callback(const char* args);
void led_blink_callback(const char* args);
void led_blink_set_period_ms_callback(const char* args);
void mem_read_callback(const char* args);   // Новая команда
void mem_write_callback(const char* args);  // Новая команда

// ============ МАССИВ КОМАНД ============
api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"help",    help_callback,    "show this help"},
    {"on",      led_on_callback,  "turn LED on"},
    {"off",     led_off_callback, "turn LED off"},
    {"blink",   led_blink_callback, "make LED blink"},
    {"set_period", led_blink_set_period_ms_callback, "set blink period in ms"},
    {"mem",     mem_read_callback,    "mem <hex_addr> - read 32-bit value from address"},
    {"wmem",    mem_write_callback,   "wmem <hex_addr> <hex_val> - write 32-bit value to address"},
    {NULL, NULL, NULL}
};

// ============ РЕАЛИЗАЦИИ ============

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", 
           DEVICE_NAME, DEVICE_VRSN);
}

void help_callback(const char* args)
{
    printf("\n========================================\n");
    printf("        Available Commands\n");
    printf("========================================\n");
    
    for (int i = 0; device_api[i].command_name != NULL; i++)
    {
        printf("  %-12s - %s\n", 
               device_api[i].command_name, 
               device_api[i].command_help);
    }
    
    printf("========================================\n\n");
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
    printf("LED turned ON\n");
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED turned OFF\n");
}

void led_blink_callback(const char* args)
{
    led_task_state_set(LED_STATE_BLINK);
    printf("LED blinking started\n");
}

void led_blink_set_period_ms_callback(const char* args)
{
    uint period_ms = 0;
    int parsed = sscanf(args, "%u", &period_ms);
    
    if (parsed != 1 || period_ms == 0) {
        printf("Error: invalid period. Usage: set_period <ms>\n");
        return;
    }
    
    led_task_set_blink_period_ms(period_ms);
}

// ============ НОВЫЕ КОМАНДЫ ============

// Команда mem - чтение памяти по адресу (hex)
void mem_read_callback(const char* args)
{
    uint32_t addr = 0;
    
    // Парсим адрес в hex формате
    int parsed = sscanf(args, "%x", &addr);
    
    if (parsed != 1) {
        printf("Error: invalid address. Usage: mem <hex_addr>\n");
        printf("Example: mem 0x20000000\n");
        return;
    }
    
    // Читаем значение по адресу
    uint32_t value = *(volatile uint32_t*)addr;
    
    printf("Address: 0x%08X | Value: 0x%08X (%u)\n", addr, value, value);
}

// Команда wmem - запись значения по адресу (hex)
void mem_write_callback(const char* args)
{
    uint32_t addr = 0;
    uint32_t value = 0;
    
    // Парсим адрес и значение в hex формате
    int parsed = sscanf(args, "%x %x", &addr, &value);
    
    if (parsed != 2) {
        printf("Error: invalid arguments. Usage: wmem <hex_addr> <hex_val>\n");
        printf("Example: wmem 0x20000000 0x12345678\n");
        return;
    }
    
    // Записываем значение по адресу
    *(volatile uint32_t*)addr = value;
    
    // Читаем обратно для проверки
    uint32_t read_back = *(volatile uint32_t*)addr;
    
    printf("Written: 0x%08X to address 0x%08X\n", value, addr);
    printf("Read back: 0x%08X\n", read_back);
}

// ============ MAIN ============
int main() {
    stdio_usb_init();
    sleep_ms(3000);
    stdio_init_all();
    
    stdio_task_init();
    led_task_init();
    protocol_task_init(device_api);
    
    printf("\n\n========================================\n");
    printf("   02-control Memory Debug Commands\n");
    printf("========================================\n");
    printf("Type 'help' to see all available commands\n");
    printf("Commands 'mem' and 'wmem' use HEX format\n");
    printf("----------------------------------------\n\n");
    
    while (1) {
        char* command = stdio_task_handle();
        
        if (command != NULL) {
            protocol_task_handle(command);
        }
        
        led_task_handle();
        sleep_ms(1);
    }
}