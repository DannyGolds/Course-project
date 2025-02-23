#pragma once
#include <string>
struct AppState {
	//Статус загрузки логов
	bool logDownloaded;
	//Показывать ли предупреждения
	bool warningsDown;
	//Показывать ли ошибки
	bool errorsDown;
	//Разрешить поиск по процессу
	bool searchByProc;
	//Имя процесса
	std::string procName;
	//Разрешить поиск по дате
	bool searchByDate;
};