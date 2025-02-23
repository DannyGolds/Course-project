#pragma once

#include <ctime>
#include <string>
#include <format>

struct LogEntry {
    enum class LogLevel {
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };


    std::time_t timestamp;     // Время события (UNIX timestamp)
    LogLevel level;            // Уровень важности: INFO, WARNING, ERROR, CRITICAL
    std::string process;    // Имя файла, где произошло событие
    std::string message;       // Текст сообщения
    std::string details;       // Дополнительные сведения (опционально)

    // Конструктор
    LogEntry(LogLevel lvl, const std::string& file, int line, const std::string& msg, const std::string& det = "")
        : timestamp(std::time(nullptr)), level(lvl), process(file), message(msg), details(det) {
    }

    // Функция для форматирования записи лога в строку (пример)
    std::string ToString() const {
        char buffer[26];
        std::tm* timeinfo;
        timeinfo = std::localtime(&timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        std::string levelString;
        switch (level) {
        case LogLevel::INFO:     levelString = "INFO";     break;
        case LogLevel::WARNING:  levelString = "WARNING";  break;
        case LogLevel::ERROR:    levelString = "ERROR";    break;
        case LogLevel::CRITICAL: levelString = "CRITICAL"; break;
        default:                 levelString = "UNKNOWN";  break;
        }

        std::string logString = std::format("{} {} {} {}", buffer, levelString, process, message);
        if (!details.empty()) {
            logString += " - Details: " + details;
        }
        return logString;
    };
};