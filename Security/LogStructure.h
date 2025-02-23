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


    std::time_t timestamp;     // ����� ������� (UNIX timestamp)
    LogLevel level;            // ������� ��������: INFO, WARNING, ERROR, CRITICAL
    std::string process;    // ��� �����, ��� ��������� �������
    std::string message;       // ����� ���������
    std::string details;       // �������������� �������� (�����������)
};