
// SecurityDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Security.h"
#include "SecurityDlg.h"
#include "afxdialogex.h"
#include "CInfoList.h"
#include "LogStructure.h"
#include "AppState.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CAboutDlg::OnChoiceDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CAboutDlg::OnChoiceDateTill)
END_MESSAGE_MAP()


// Диалоговое окно CSecurityDlg



CSecurityDlg::CSecurityDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SECURITY_DIALOG, pParent) { m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);}

void CSecurityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, show_log);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER2, time_from);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER1, time_till);
	DDX_Control(pDX, IDC_EDIT2, edit_process);
	DDX_Control(pDX, IDC_CHECK3, check_warns);
	DDX_Control(pDX, IDC_CHECK4, check_fails);
	DDX_Control(pDX, IDC_CHECK1, check_process);
	DDX_Control(pDX, IDC_CHECK2, check_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, date_from);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, date_till);
}

BEGIN_MESSAGE_MAP(CSecurityDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CSecurityDlg::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_EDIT2, &CSecurityDlg::OnEnChangeEdit2)
	ON_COMMAND(ID_Open, &CSecurityDlg::OnOpen)
	ON_BN_CLICKED(IDC_BUTTON1, &CSecurityDlg::OnApplyFilter)
	ON_BN_CLICKED(IDC_BUTTON3, &CSecurityDlg::OnFullCheck)
	ON_COMMAND(ID_32775, &CSecurityDlg::OnSaveAs)
	ON_BN_CLICKED(IDC_CHECK2, &CSecurityDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CSecurityDlg::OnBnClickedCheck1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CSecurityDlg::OnChoiceDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CSecurityDlg::OnChoiceDateTill)
END_MESSAGE_MAP()


// Обработчики сообщений CSecurityDlg

BOOL CSecurityDlg::OnInitDialog()
{
	setlocale(LC_ALL, "RU");
	CDialogEx::OnInitDialog();
	CFont LogFont;
	LogFont.CreateFontW(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, DEFAULT_PITCH | FF_SWISS, _T("Montserrat"));
	show_log.SetFont(&LogFont);
	show_log.SetWindowTextW(_T("Файл не выбран."));
	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
	date_from.EnableWindow(FALSE); // Отключаем элемент
	date_till.EnableWindow(FALSE); // Отключаем элемент
	edit_process.EnableWindow(FALSE);
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CSecurityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSecurityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSecurityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSecurityDlg::OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}

void CSecurityDlg::OnEnChangeEdit2()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}

//Function templates 
template<typename T>
void readFile(T& file, std::vector<LogEntry>& logs);

void setCheckState(CButton&, CWnd&);
//End of Function templates


//Global variables
std::vector<LogEntry> logs;
AppState state;
CString logPath;
//
void CSecurityDlg::OnOpen()
{
	CFileDialog fileDialog(TRUE, NULL, L"*.txt;*.log;*.xml;*.csv");
	if (fileDialog.DoModal() == IDOK)
	{
		logPath = fileDialog.GetPathName();
		CStdioFile file;
		if (file.Open(logPath, CFile::modeRead)) // Проверка открытия файла
		{
			show_log.SetWindowTextW(logPath);
			logs.clear(); // Очищаем вектор перед чтением
			readFile(file, logs); //Вызов шаблонной функции
			file.Close(); // Закрываем файл после использования
		}
		else
		{
			TRACE(_T("Не удалось открыть файл!"));
		}
	}
}

void CSecurityDlg::OnSaveAs()
{
	CFileDialog fileDialog(FALSE, NULL, L"Конфигурация системы.log");
	if (fileDialog.DoModal() == IDOK)
	{
		auto logToSave = fileDialog.GetPathName();
		CStdioFile fileToSave;
		if (fileToSave.Open(logToSave, CFile::modeCreate | CFile::modeWrite)) //Проверка открытия
		{
			CString data = _T("Логи загружены!\r\n"); // Добавляем перевод строки
			fileToSave.Write(data, data.GetLength() * sizeof(TCHAR));
			fileToSave.Close();
		}
		else
		{
			TRACE(_T("Не удалось создать файл для сохранения!"));
		}
	}
}


void CSecurityDlg::OnApplyFilter()
{
	TRACE(state.dateFrom.Format("%d.%m.%Y"));
	TRACE(state.dateTill.Format("%d.%m.%Y"));
}

void CSecurityDlg::OnFullCheck()
{
	if (logPath != L"") {
		CInfoList InfoList;
		InfoList.SetLogs(logs);
		InfoList.DoModal();
	}
	else {
		AfxMessageBox(L"Вы не выбрали ни одного лог-файла. Проверка невозможна!");
	}
	
}

void CSecurityDlg::OnBnClickedCheck2()
{
	if (check_date.GetCheck() == BST_CHECKED) {
		state.searchByDate = true;
	}
	else {
		state.searchByDate = false;
	}
	setCheckState(check_date, date_from);
	setCheckState(check_date, date_till);
}


void CSecurityDlg::OnBnClickedCheck1()
{
	if (check_process.GetCheck() == BST_CHECKED) {
		state.searchByProc = true;
	}
	else {
		state.searchByProc = false;
	}
	setCheckState(check_process, edit_process);
}


//Start of support functions

// Start of support functions
template<typename T>
void readFile(T& file, std::vector<LogEntry>& logs)
{
	CString line;
	while (file.ReadString(line))
	{
		CStringArray tokens;
		CString token;
		int iStart = 0;

		while ((token = line.Tokenize(_T(","), iStart)) != _T(""))
		{
			tokens.Add(token);
		}

		// Создаем LogEntry только если у нас достаточно токенов (минимум 5)
		if (tokens.GetSize() >= 5)
		{
			LogEntry logEntry(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
			logs.push_back(logEntry);
		}
		else
		{
			AfxMessageBox(_T("Файл не соответствует требованиям! Чтение завершено с ошибкой.\n"));
		}
	}
}

void setCheckState(CButton& checkBox, CWnd& elem)
{
	if (checkBox.GetCheck() == BST_CHECKED)
	{
		elem.EnableWindow(TRUE); // Включаем элемент
	}
	else if (checkBox.GetCheck() == BST_UNCHECKED)
	{
		elem.EnableWindow(FALSE); // Отключаем элемент
	}
}

//End of support functions
void CSecurityDlg::OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	*pResult = 0;

	CTime time; 
	date_from.GetTime(time); // Проверяем, что дата успешно получена
	state.dateFrom = time; // Сохраняем дату 
}

void CSecurityDlg::OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
	CTime time;
	date_till.GetTime(time); // Проверяем, что дата успешно получена
	state.dateTill = time; // Сохраняем дату в state
	
}


void CAboutDlg::OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}

void CAboutDlg::OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}