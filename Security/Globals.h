#pragma once
#include "Structures.h"
#include <map>
#include <vector>

extern std::vector<LogEntry> logs;
extern CString logPath;
extern AppState state;
extern std::map<std::wstring, std::wstring> processes;
extern std::map<std::wstring, std::wstring> commandRisk;
extern std::vector<LogEntry> copied_logs;