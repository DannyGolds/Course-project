#pragma once
#include <string>
struct LogEntry {
    CTime timestamp;
    std::wstring level;
    std::wstring process;
    std::wstring message;
    std::wstring details;
    LogEntry(const CTime& timestamp, const std::wstring& level, const std::wstring& process, const std::wstring& message, const std::wstring& details)
        : timestamp(timestamp), level(level), process(process), message(message), details(details) {
    }
};