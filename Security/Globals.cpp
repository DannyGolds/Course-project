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
       {L"execve", L"Высокая - запуск произвольных программ"},
       {L"ptrace", L"Высокая - слежение за процессами"},
       {L"access", L"Низкая - проверка доступности файлов"},
       {L"mmap", L"Высокая - возможное внедрение кода"},
       {L"connect", L"Средняя - создание сетевого подключения"},
       {L"unlink", L"Высокая - удаление файлов"},
       {L"write", L"Средняя - запись в файл"},
       {L"close", L"Низкая - закрытие файла"},
       {L"openat", L"Средняя - открытие файла отн. каталога"},
       {L"newfwstat", L"Низкая - выявление статистики по файлу"},
       {L"read", L"Средняя - чтение данных(возможно, конфиденциальных)"},
       {L"chmod", L"Высокая - изменение прав на файл"},
       {L"chown", L"Высокая - смена владельца файла"},
       {L"arch_prctl", L"Средняя - переименование файла"},
       {L"getpid", L"Средняя - получение proc_id"},
       {L"fork", L"Средняя - создание нового процесса"},
       {L"kill", L"Средняя - завершение процессов"},
       {L"brk", L"Низкая - выделение доп. памяти"},
};