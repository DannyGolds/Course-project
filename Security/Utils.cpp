#include "pch.h"
#include "Utils.h"
#include "Globals.h"
#include <sstream>
#include <fstream>
#include <mutex>
#include <thread>
#include <future>
#include <queue>
#include <condition_variable>
#include <string>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <Windows.h>

void clearInfoListInputs() {
	cpl_l = 0;
	cpl_m = 0;
	cpl_h = 0;
}

std::string WStoS(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();

    // Определяем размер буфера для строки UTF-8
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (sizeNeeded <= 0) return std::string();

    // Преобразуем UTF-16 в UTF-8
    std::string utf8Str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &utf8Str[0], sizeNeeded, nullptr, nullptr);

    return utf8Str;
}


std::vector<std::wstring> splitLine(const std::wstring& line) {
    std::wstringstream splited(line);
    std::wstring token;
    auto ssplited = splited.str();
    std::vector<std::wstring> result(5);
    splited >> result[0] >> result[1] >> token;
    for (auto const& [cmd, lev] : commandRisk) {
        if (token.find(cmd) != std::wstring::npos) {
            result[3] = cmd;
            result[4] = lev; // Добавление уровня подозрительности в результат
            break;
        }
        else {
            result[3] = L"";
            result[4] = L"";
        }
    };
    if (result[3] == L"execve") {
        size_t start = ssplited.find(_T('"')) + 1;
        size_t end = ssplited.find(_T(","), start) - 1;
        result[2] = ssplited.substr(start, end - start); // Добавление команды в результат
        processes[result[0]] = result[2]; // Добавление PID в список процессов
    }
    else {
        result[2] = processes[result[0]];
    }
    return result;
}
void filterByParameters(const AppState currentState, const std::vector<LogEntry>& logs, std::vector<LogEntry>& copied_logs) {

    if (currentState.isEverySame(recentState)) {
        return;
    }
    else {
        copied_logs = logs;
        if (currentState.isInfoHidden) {
            copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
                [](const LogEntry& log) { return !log.details.find(L"Низк"); }),
                copied_logs.end());
        }

        if (currentState.isWarningsHidden) {
            copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
                [](const LogEntry& log) { return !log.details.find(L"Средн"); }),
                copied_logs.end());
        }

        if (currentState.searchByProc && !currentState.procName.empty()) {
            copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
                [currentState](const LogEntry& log) {
                    return log.process.find(currentState.procName) == std::wstring::npos;
                }),
                copied_logs.end());
        }

        if (currentState.searchByDate) {
            copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
                [currentState](const LogEntry& log) {
                    int logTimeInSeconds = log.time.GetHour() * 3600 + log.time.GetMinute() * 60 + log.time.GetSecond();
                    int fromTimeInSeconds = currentState.dateFrom.GetHour() * 3600 + currentState.dateFrom.GetMinute() * 60 + currentState.dateFrom.GetSecond();
                    int tillTimeInSeconds = currentState.dateTill.GetHour() * 3600 + currentState.dateTill.GetMinute() * 60 + currentState.dateTill.GetSecond();
                    if (fromTimeInSeconds <= tillTimeInSeconds) {
                        // Обычный случай: диапазон в пределах одного дня
                        return logTimeInSeconds < fromTimeInSeconds || logTimeInSeconds > tillTimeInSeconds;
                    }
                    else {
                        // Диапазон пересекает границу суток
                        return logTimeInSeconds > tillTimeInSeconds && logTimeInSeconds < fromTimeInSeconds;
                    }; }),
                copied_logs.end());
        }
    }
}
void readFile(std::ifstream& file, std::vector<LogEntry>& logs) {
    std::string line;
    while (getline(file, line)) {
        std::wstring utf16Line = UTF8ToUTF16(line);
        auto vec = splitLine(utf16Line);
        if (vec[3] != L"") {
            LogEntry log;
            log.PID = vec[0];
            log.timeStr = vec[1];
            CTime time;
            if (WStringToCTime(vec[1], time)) {
                log.time = time;
            } else {
                log.time = CTime::GetCurrentTime();
            }
            log.process = vec[2];
            log.command = vec[3];
            log.details = vec[4];
            logs.emplace_back(log);
        }
    }

    if (copied_logs.empty()) {
        copied_logs = logs;
    }
}
std::wstring UTF8ToUTF16(const std::string& utf8) {
    if (utf8.empty()) return std::wstring();

    // Находим длину строки до первого нулевого символа
    size_t length = utf8.find('\0');
    if (length == std::string::npos) {
        length = utf8.size(); // Если нулевой символ не найден, берём всю строку
    }

    // Вычисляем размер буфера для UTF-16
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(length), nullptr, 0);
    if (size == 0) return std::wstring();

    // Преобразуем UTF-8 в UTF-16
    std::wstring utf16(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(length), &utf16[0], size);

    return utf16;
}
bool WStringToCTime(std::wstring& timeStr, CTime& outTime) {
    int hours = 0, minutes = 0, seconds, ms = 0;

    // Попытка распарсить строку времени
    if (swscanf_s(timeStr.c_str(), L"%d:%d:%d.%d", &hours, &minutes, &seconds, &ms) != 4) {
        return false;  // Ошибка парсинга
    }

    try {
        // Используем текущую дату для создания CTime
        CTime currentDate = CTime::GetCurrentTime();
        outTime = CTime(
            currentDate.GetYear(),
            currentDate.GetMonth(),
            currentDate.GetDay(),
            hours,
            minutes,
            seconds
        );
        return true;
    }
    catch (CException* e) {
        e->Delete();  // Удаляем исключение
        return false;  // Некорректное время
    }
}
std::wstring CTimeToWString(const CTime& time) {
    // Форматируем дату через CTime::Format()
    CString formattedTime = time.Format(L"%T");

    // Конвертируем CString в std::wstring
    return std::wstring(formattedTime.GetString());
}

double calculateSuspicionPercentage(const int& low, const int& medium, const int& high) {
    // Проверяем, есть ли данные для расчета
    int total = high + medium + low;
    if (total == 0) {
        return 0.0; // Если нет данных, возвращаем 0%
    }

    // Весовые коэффициенты для каждого уровня подозрительности
    const double highWeight = 1.0;   // Высокая подозрительность
    const double mediumWeight = 0.5; // Средняя подозрительность
    const double lowWeight = 0.2;    // Низкая подозрительность

    // Рассчитываем общий "вес" подозрительности
    double weightedSum = (high * highWeight) + (medium * mediumWeight) + (low * lowWeight);

    // Рассчитываем процент подозрительности
    double suspicionPercentage = (weightedSum / total) * 100.0;

    return suspicionPercentage;
}
