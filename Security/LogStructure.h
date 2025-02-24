#pragma once

struct LogEntry {
    CString timestamp;     // Время события (UNIX timestamp)
    CString level;            // Уровень важности: INFO, WARNING, ERROR, CRITICAL
    CString process;    // Имя файла, где произошло событие
    CString message;       // Текст сообщения
    CString details;       // Дополнительные сведения (опционально)

    LogEntry(CString timestamp,     // Время события (UNIX timestamp)
        CString level,            // Уровень важности: INFO, WARNING, ERROR, CRITICAL
        CString process,    // Имя файла, где произошло событие
        CString message,       // Текст сообщения
        CString details) : timestamp(timestamp), level(level), process(process), message(message), details(details) {
    };
};