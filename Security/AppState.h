#pragma once
#include <string>
struct AppState {
	//Статус загрузки логов
	bool isLogsLoaded;
	//Путь к логам
	std::string logsPath;
	//Показывать ли предупреждения
	bool isWarningsShowed;
	//Показывать ли ошибки
	bool isFailsShowed;
	//Разрешить поиск по процессу
	bool searchByProc;
	//Имя процесса
	std::string procName;
	//Разрешить поиск по дате
	bool searchByDate;
	//Начальная дата поиска
	CTime dateFrom;
	//Конечная дата поиска
	CTime dateTill;
	AppState(bool logs, std::string logsPath, bool warns, bool fails, bool byProc, std::string procName, bool byDate, CTime from, CTime till) :
		isLogsLoaded(logs), logsPath(logsPath),
		isWarningsShowed(warns), isFailsShowed(fails),
		searchByProc(byProc), procName(procName),
		searchByDate(byDate),dateFrom(from), dateTill(till) {};
	AppState() :
		isLogsLoaded(false), logsPath(""),
		isWarningsShowed(false), isFailsShowed(false),
		searchByProc(false), procName(""),
		searchByDate(false), dateFrom(CTime::GetCurrentTime()), dateTill(CTime::GetCurrentTime()) {
	}
};