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


    std::time_t timestamp;     // ����� ������� (UNIX timestamp)
    LogLevel level;            // ������� ��������: INFO, WARNING, ERROR, CRITICAL
    std::string process;    // ��� �����, ��� ��������� �������
    std::string message;       // ����� ���������
    std::string details;       // �������������� �������� (�����������)

    // �����������
    LogEntry(LogLevel lvl, const std::string& file, int line, const std::string& msg, const std::string& det = "")
        : timestamp(std::time(nullptr)), level(lvl), process(file), message(msg), details(det) {
    }

    // ������� ��� �������������� ������ ���� � ������ (������)
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