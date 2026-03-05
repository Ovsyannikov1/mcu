#include "protocol-task.h"
#include <stdio.h>
#include <string.h>

static api_t* api = NULL;
static int commands_count = 0;

void protocol_task_init(api_t* device_api)
{
    api = device_api;
    
    // Подсчет количества команд (до первой NULL-записи)
    commands_count = 0;
    while (api[commands_count].command_name != NULL) {
        commands_count++;
    }
    
    printf("Protocol initialized with %d commands\n", commands_count);
}

void protocol_task_handle(char* command_string)
{
    // Проверка на NULL
    if (!command_string)
    {
        return;  // Строка команды еще не получена
    }
    
    // Делим строку на команду и аргументы
    const char* command_name = command_string;
    const char* command_args = NULL;
    
    // Ищем пробел, разделяющий команду и аргументы
    char* space_symbol = strchr(command_string, ' ');
    
    if (space_symbol)
    {
        *space_symbol = '\0';        // Разделяем строку
        command_args = space_symbol + 1;  // Аргументы - после пробела
    }
    else
    {
        command_args = "";  // Аргументов нет
    }
    
    // Отладка: выводим разобранную команду
    printf("Parsed command: '%s', args: '%s'\n", command_name, command_args);
    
    // Поиск команды в таблице
    for (int i = 0; i < commands_count; i++)
    {
        if (strcmp(command_name, api[i].command_name) == 0)
        {
            // Команда найдена - вызываем callback
            printf("Executing command: %s\n", api[i].command_name);
            api[i].command_callback(command_args);
            return;
        }
    }
    
    // Команда не найдена
    printf("Unknown command: '%s'\n", command_name);
    printf("Type 'help' for available commands\n");
}