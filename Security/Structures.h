#pragma once
#include <afx.h>
#include <vector>
#include <string>

struct LogEntry {
    std::wstring PID;
    CTime time;
    std::wstring process;
    std::wstring command;
    std::wstring details;
};

struct AppState {
    bool isLogsLoaded;
	std::string logsPath;
	bool isWarningsHidden;
	bool isInfoHidden;
	bool searchByProc;
	std::wstring procName;
	bool searchByDate;
	CTime dateFrom;
	CTime dateTill;
};