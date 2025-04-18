#pragma once
#include "Structures.h"
#include <map>
#include <vector>

extern std::vector<LogEntry> logs;
extern std::vector<LogEntry> copied_logs;
extern CString logPath;
extern AppState recentState;
extern AppState state;
extern int cpl_l;
extern int cpl_m;
extern int cpl_h;
extern std::map<std::wstring, std::wstring> processes;
extern std::map<std::wstring, std::wstring> commandRisk;