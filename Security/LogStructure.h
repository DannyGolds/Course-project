#pragma once

struct LogEntry {
    CString timestamp;     // ����� ������� (UNIX timestamp)
    CString level;            // ������� ��������: INFO, WARNING, ERROR, CRITICAL
    CString process;    // ��� �����, ��� ��������� �������
    CString message;       // ����� ���������
    CString details;       // �������������� �������� (�����������)

    LogEntry(CString timestamp,     // ����� ������� (UNIX timestamp)
        CString level,            // ������� ��������: INFO, WARNING, ERROR, CRITICAL
        CString process,    // ��� �����, ��� ��������� �������
        CString message,       // ����� ���������
        CString details) : timestamp(timestamp), level(level), process(process), message(message), details(details) {
    };
};