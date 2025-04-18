#include "pch.h"
#include "Globals.h"
#include "Structures.h"
#include <map>
#include <vector>
#include <string>

std::vector<LogEntry> logs;
std::vector<LogEntry> copied_logs;
AppState state;
CString logPath;
std::map<std::wstring, std::wstring> processes;
std::map<std::wstring, std::wstring> commandRisk = {
       {L"execve", L"������� - ������ ������������ ��������"},
       {L"ptrace", L"������� - �������� �� ����������"},
       {L"access", L"������ - �������� ����������� ������"},
       {L"mmap", L"������� - ��������� ��������� ����"},
       {L"connect", L"������� - �������� �������� �����������"},
       {L"unlink", L"������� - �������� ������"},
       {L"write", L"������� - ������ � ����"},
       {L"close", L"������ - �������� �����"},
       {L"openat", L"������� - �������� ����� ���. ��������"},
       {L"newfwstat", L"������ - ��������� ���������� �� �����"},
       {L"read", L"������� - ������ ������(��������, ����������������)"},
       {L"chmod", L"������� - ��������� ���� �� ����"},
       {L"chown", L"������� - ����� ��������� �����"},
       {L"arch_prctl", L"������� - �������������� �����"},
       {L"getpid", L"������� - ��������� proc_id"},
       {L"fork", L"������� - �������� ������ ��������"},
       {L"kill", L"������� - ���������� ���������"},
       {L"brk", L"������ - ��������� ���. ������"},
};