#pragma once
#include <string>
struct AppState {
	//������ �������� �����
	bool isLogsLoaded;
	//���� � �����
	std::string logsPath;
	//���������� �� ��������������
	bool isWarningsHidden;
	//���������� �� ������
	bool isInfoHidden;
	//��������� ����� �� ��������
	bool searchByProc;
	//��� ��������
	std::wstring procName;
	//��������� ����� �� ����
	bool searchByDate;
	//��������� ���� ������
	CTime dateFrom;
	//�������� ���� ������
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