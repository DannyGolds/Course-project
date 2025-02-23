#pragma once

#include <ctime>
#include <string>

struct LogEntry {
    enum class LogLevel {
        INFO,
        WARNING,
        ERR,
        CRITICAL
    };


    std::time_t timestamp;     // Время события (UNIX timestamp)
    LogLevel level;            // Уровень важности: INFO, WARNING, ERROR, CRITICAL
    std::string process;    // Имя файла, где произошло событие
    std::string message;       // Текст сообщения
    std::string details;       // Дополнительные сведения (опционально)
};