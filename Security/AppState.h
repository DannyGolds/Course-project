#pragma once
#include <string>
struct AppState {
	//������ �������� �����
	bool logDownloaded;
	//���������� �� ��������������
	bool warningsDown;
	//���������� �� ������
	bool errorsDown;
	//��������� ����� �� ��������
	bool searchByProc;
	//��� ��������
	std::string procName;
	//��������� ����� �� ����
	bool searchByDate;
};