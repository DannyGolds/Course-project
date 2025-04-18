#pragma once
#include "Structures.h"
#include <vector>
#include <fstream>
#include <string>

std::wstring UTF8ToUTF16(const std::string& utf8);
bool WStringToCTime(std::wstring& timeStr, CTime& outTime);
std::vector<std::wstring> splitLine(const std::wstring& line);
void filterByParameters(const AppState state, const std::vector<LogEntry>& logs, std::vector<LogEntry>& result);
void setCheckState(CButton& checkBox, CWnd& elem);
void readFile(std::ifstream& file, std::vector<LogEntry>& logs);
double calculateSuspicionPercentage(const std::vector<LogEntry>& logs)