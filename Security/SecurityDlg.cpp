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

// Предварительные объявления шаблонных функций
template<typename T>
void readFile(T& file, std::vector<LogEntry>& logs);

void setCheckState(CButton&, CWnd&);

// Глобальные переменные
std::vector<LogEntry> logs;
AppState state;
CString logPath;

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
    : CDialogEx(IDD_SECURITY_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CSecurityDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, show_log);
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

BOOL CSecurityDlg::OnInitDialog()
{
    setlocale(LC_ALL, "RU");
    CDialogEx::OnInitDialog();
    CFont LogFont;
    LogFont.CreateFontW(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, DEFAULT_PITCH | FF_SWISS, _T("Montserrat"));
    show_log.SetFont(&LogFont);
    show_log.SetWindowTextW(_T("Файл не выбран."));

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    date_from.EnableWindow(FALSE);
    date_till.EnableWindow(FALSE);
    edit_process.EnableWindow(FALSE);

    return TRUE;
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

void CSecurityDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CSecurityDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CSecurityDlg::OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
}

void CSecurityDlg::OnEnChangeEdit2()
{
    // Обработка изменения текста
}

void CSecurityDlg::OnOpen()
{
    CFileDialog fileDialog(TRUE, NULL, L"*.txt;*.log;*.xml;*.csv");
    if (fileDialog.DoModal() == IDOK)
    {
        logPath = fileDialog.GetPathName();
        std::ifstream file(logPath, std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            show_log.SetWindowTextW(logPath);
            readFile(file, logs);
            file.close();
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
        std::ofstream fileToSave(logToSave, std::ios::out | std::ios::binary);
        if (fileToSave.is_open())
        {
            std::string data = "Логи загружены!";
            fileToSave << data;
        }
        else
        {
            TRACE(_T("Не удалось создать файл для сохранения!"));
        }
        fileToSave.close();
    }
}

void CSecurityDlg::OnApplyFilter()
{
    TRACE(state.dateFrom.Format(L"%d.%m.%Y"));
    TRACE(state.dateTill.Format(L"%d.%m.%Y"));
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
    state.searchByDate = (check_date.GetCheck() == BST_CHECKED);
    setCheckState(check_date, date_from);
    setCheckState(check_date, date_till);
}

void CSecurityDlg::OnBnClickedCheck1()
{
    state.searchByProc = (check_process.GetCheck() == BST_CHECKED);
    setCheckState(check_process, edit_process);
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

// Реализация шаблонной функции readFile
template<typename T>
void readFile(T& file, std::vector<LogEntry>& logs)
{
    CString errorMsg = _T("");
    std::string line;
    while (std::getline(file, line))
    {
        // Конвертируем строку из UTF-8 в UTF-16
        std::wstring utf16Line = UTF8ToUTF16(line);

        // Разделяем строку на токены
        std::vector<std::wstring> tokens;
        size_t start = 0;
        size_t end = utf16Line.find(L',');

        while (end != std::wstring::npos)
        {
            tokens.push_back(utf16Line.substr(start, end - start));
            start = end + 1;
            end = utf16Line.find(L',', start);
        }
        tokens.push_back(utf16Line.substr(start, end));

        // Если токенов достаточно, создаем LogEntry
        if (tokens.size() >= 4)
        {
            LogEntry logEntry(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
            logs.push_back(logEntry);
        }
        else
        {
            continue;
            errorMsg = _T("Некоторые или все строки файла не соответствуют требованиям!");
        }
    }
    if (errorMsg.GetLength() > 0) AfxMessageBox(errorMsg);
}

// Реализация функции setCheckState
void setCheckState(CButton& checkBox, CWnd& elem)
{
    elem.EnableWindow(checkBox.GetCheck() == BST_CHECKED);
}

void CSecurityDlg::OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
    *pResult = 0;

    CTime time;
    date_from.GetTime(time);
    state.dateFrom = time;
}

void CSecurityDlg::OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
    *pResult = 0;

    CTime time;
    date_till.GetTime(time);
    state.dateTill = time;
}

void CAboutDlg::OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
}

void CAboutDlg::OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
}

void filterByParameters(AppState currentState) {

}