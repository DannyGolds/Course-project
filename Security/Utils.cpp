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

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;

public:
    ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this]() { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
                });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
        using returnType = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<returnType> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    };
};

void filterByParameters(const AppState currentState, const std::vector<LogEntry>& logs, std::vector<LogEntry>& copied_logs) {
    copied_logs.clear();
    ThreadPool pool(std::thread::hardware_concurrency()); // Количество потоков = числу ядер процессора
    std::vector<std::future<void>> futures;
    std::mutex mtx;
    copied_logs = logs;
    if (currentState.isInfoHidden) {
        futures.emplace_back(pool.enqueue([&]() {
            std::lock_guard<std::mutex> lock(mtx);
            copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
                [](const LogEntry& log) { return !log.details.find(L"Низк"); }),
                copied_logs.end());
            }));
    }
    if (currentState.isWarningsHidden) {
        futures.emplace_back(pool.enqueue([&]() {
			std::lock_guard<std::mutex> lock(mtx);
			copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
				[](const LogEntry& log) { return !log.details.find(L"Средн"); }),
				copied_logs.end());
			}));
    }
    if (currentState.searchByProc) {
        futures.emplace_back(pool.enqueue([&]() {
			std::lock_guard<std::mutex> lock(mtx);
			copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
				[currentState](const LogEntry& log) { return !log.process.find(currentState.procName); }),
				copied_logs.end());
			}));
    }
    if (currentState.searchByDate) {
        futures.emplace_back(pool.enqueue([&]() {
			std::lock_guard<std::mutex> lock(mtx);
			copied_logs.erase(std::remove_if(copied_logs.begin(), copied_logs.end(),
				[currentState](const LogEntry& log) { return !(log.time.GetTime() <= currentState.dateTill.GetTime() && log.time.GetTime() >= currentState.dateFrom.GetTime()); }),
				copied_logs.end());
			}));
    }
	for (auto& future : futures) {
        future.get();
	}
}

void readFile(std::ifstream& file, std::vector<LogEntry>& logs) {
    std::string line;
    logs.reserve(20000);
    ThreadPool pool(std::thread::hardware_concurrency()); // Количество потоков = числу ядер процессора
    std::vector<std::future<void>> futures;
    std::mutex mtx;
    while (getline(file, line)) {
        futures.emplace_back(pool.enqueue([&logs, &line, &mtx]() {
            std::wstring utf16Line = UTF8ToUTF16(line);
            auto vec = splitLine(utf16Line);
            if (vec[3] != L"") {
                LogEntry log;
                log.PID = vec[0];
                CTime time;
                if (WStringToCTime(vec[1], time)) {
                    log.time = time;
                }
                else {
                    log.time = CTime::GetCurrentTime();
                }
                log.process = vec[2];
                log.command = vec[3];
                log.details = vec[4];
                static std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                logs.emplace_back(log);
            }
            }));
        
    };
    for (auto& future : futures) {
        future.get();
    }
    if (copied_logs.empty()) {
        copied_logs.swap(logs);
    }
}

std::vector<std::wstring> splitLine(const std::wstring& line) {
    std::wstringstream splited(line);
    std::wstring token;
    std::mutex mute;
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
        size_t end = ssplited.find(_T(","), start);
        result[2] = ssplited.substr(start, end - start); // Добавление команды в результат
        processes[result[0]] = result[2]; // Добавление PID в список процессов
    }
    else {
        result[2] = processes[result[0]]; // Добавление команды в результат, если нет процесса
    }

    return result;
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
    int hours = 0, minutes = 0, seconds = 0;

    // Попытка распарсить строку времени
    if (swscanf_s(timeStr.c_str(), L"%d:%d:%d", &hours, &minutes, &seconds) != 3) {
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
