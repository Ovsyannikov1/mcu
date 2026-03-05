#pragma once

// Тип указателя на функцию-обработчик команды
typedef void(*command_callback_t)(const char* args);

// Структура команды
typedef struct
{
    const char* command_name;      // Имя команды (например, "version")
    command_callback_t command_callback;  // Указатель на функцию обработки
    const char* command_help;       // Описание команды для справки
} api_t;

// Прототипы функций модуля
void protocol_task_init(api_t* device_api);
void protocol_task_handle(char* command_string);