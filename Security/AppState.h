#pragma once
#include <string>
struct AppState {
	//Статус загрузки логов
	bool isLogsLoaded;
	//Путь к логам
	std::string logsPath;
	//Показывать ли предупреждения
	bool isWarningsHidden;
	//Показывать ли ошибки
	bool isInfoHidden;
	//Разрешить поиск по процессу
	bool searchByProc;
	//Имя процесса
	std::wstring procName;
	//Разрешить поиск по дате
	bool searchByDate;
	//Начальная дата поиска
	CTime dateFrom;
	//Конечная дата поиска
	CTime dateTill;
	AppState(bool logs, std::string logsPath, bool warns, bool info, bool byProc, std::wstring procName, bool byDate, CTime from, CTime till) :
		isLogsLoaded(logs), logsPath(logsPath),
		isWarningsHidden(warns), isInfoHidden(info),
		searchByProc(byProc), procName(procName),
		searchByDate(byDate),dateFrom(from), dateTill(till) {};
	AppState() :
		isLogsLoaded(false), logsPath(""),
		isWarningsHidden(false), isInfoHidden(false),
		searchByProc(false), procName(L""),
		searchByDate(false), dateFrom(CTime::GetCurrentTime()), dateTill(CTime::GetCurrentTime()) {
	}
};