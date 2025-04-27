#pragma once
#include <afx.h>
#include <vector>
#include <string>

struct LogEntry {
    std::wstring PID;
    CTime time;
	std::wstring timeStr;
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
	bool searchByCommand;
	bool searchByPID;
	std::wstring commandName;
	std::wstring PID;
	CTime dateTill;
public:
	bool isEverySame(AppState& state) const {
		return isLogsLoaded == state.isLogsLoaded && isWarningsHidden == state.isWarningsHidden && isInfoHidden == state.isWarningsHidden &&
			searchByProc == state.searchByProc && state.dateFrom == dateFrom && state.dateTill == dateTill && state.searchByDate == searchByDate && procName == state.procName &&searchByCommand == state.searchByCommand && searchByPID == state.searchByPID && commandName == state.commandName && PID == state.PID;
	}
};