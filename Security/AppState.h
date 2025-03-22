#pragma once
#include <string>
struct AppState {
	//������ �������� �����
	bool isLogsLoaded;
	//���� � �����
	std::string logsPath;
	//���������� �� ��������������
	bool isWarningsShowed;
	//���������� �� ������
	bool isFailsShowed;
	//��������� ����� �� ��������
	bool searchByProc;
	//��� ��������
	std::string procName;
	//��������� ����� �� ����
	bool searchByDate;
	//��������� ���� ������
	CTime dateFrom;
	//�������� ���� ������
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