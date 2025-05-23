﻿#include "pch.h"
#include "framework.h"
#include "Security.h"
#include "SecurityDlg.h"
#include "afxdialogex.h"
#include "CInfoList.h"
#include "Structures.h"
#include "Globals.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <map>

#include <thread>
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
    DDX_Control(pDX, IDC_CHECK4, check_info);
    DDX_Control(pDX, IDC_CHECK1, check_process);
    DDX_Control(pDX, IDC_CHECK2, check_date);
    DDX_Control(pDX, IDC_DATETIMEPICKER2, date_from);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, date_till);
    DDX_Control(pDX, IDC_BUTTON3, btn_check);
    DDX_Control(pDX, IDC_BUTTON1, btn_apply);
    DDX_Control(pDX, IDC_EDIT8, PID_edit);
    DDX_Control(pDX, IDC_EDIT7, command_edit);
    DDX_Control(pDX, IDC_CHECK5, pid_check);
    DDX_Control(pDX, IDC_CHECK6, command_check);
}

BEGIN_MESSAGE_MAP(CSecurityDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CSecurityDlg::OnNMCustomdrawProgress1)
    ON_EN_CHANGE(IDC_EDIT2, &CSecurityDlg::OnChangeProcName)
    ON_COMMAND(ID_Open, &CSecurityDlg::OnOpen)
    ON_BN_CLICKED(IDC_BUTTON1, &CSecurityDlg::OnApplyFilter)
    ON_BN_CLICKED(IDC_BUTTON3, &CSecurityDlg::OnFullCheck)
    ON_BN_CLICKED(IDC_CHECK2, &CSecurityDlg::OnSearchingByDate)
    ON_BN_CLICKED(IDC_CHECK1, &CSecurityDlg::OnSearchingByProc)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CSecurityDlg::OnChoiceDateFrom)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CSecurityDlg::OnChoiceDateTill)
    ON_BN_CLICKED(IDC_CHECK4, &CSecurityDlg::OnHideInfo)
    ON_BN_CLICKED(IDC_CHECK3, &CSecurityDlg::OnHideWarns)
    ON_COMMAND(ID_32778, &CSecurityDlg::OnSave)
    ON_BN_CLICKED(IDC_CHECK5, &CSecurityDlg::OnBnClickedCheck5)
    ON_BN_CLICKED(IDC_CHECK6, &CSecurityDlg::OnBnClickedCheck6)
    ON_EN_CHANGE(IDC_EDIT8, &CSecurityDlg::OnEnChangeEdit8)
    ON_EN_CHANGE(IDC_EDIT7, &CSecurityDlg::OnEnChangeEdit7)
    ON_COMMAND(ID_32779, &CSecurityDlg::OnSaveFiltered)
END_MESSAGE_MAP()


BOOL CSecurityDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    show_log.SetWindowTextW(_T("Файл не выбран."));
    if (takePath != L"") { OnOpen();
    };
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    return TRUE;
}

void CSecurityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
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

void CSecurityDlg::OnOpen()
{
    // Если takePath задан, используем его для открытия файла
    if (!takePath.IsEmpty()) {
        logPath = takePath; // Устанавливаем путь из командной строки
    }
    else {
        // Если takePath не задан, открываем диалоговое окно
        CFileDialog fileDialog(TRUE, _T("strace.log"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Файлы журналирования |*.log;*.txt||"));
        if (fileDialog.DoModal() == IDOK) {
            logPath = fileDialog.GetPathName(); // Получаем путь из диалогового окна
        }
    }

    // Открываем файл по указанному пути
    std::ifstream file(logPath);
    AfxMessageBox(logPath);
    try {
        if (file.is_open()) {
            logs.clear();
            copied_logs.clear();
            show_log.SetWindowTextW(logPath); // Отображаем путь в интерфейсе
            readFile(file, logs); // Читаем файл
            file.close();
            if (!savePath.IsEmpty()) {
                OnSave();
            }

            // Активируем элементы управления
            btn_check.EnableWindow(TRUE);
            check_date.EnableWindow(TRUE);
            check_info.EnableWindow(TRUE);
            check_process.EnableWindow(TRUE);
            check_warns.EnableWindow(TRUE);
            btn_apply.EnableWindow(TRUE);
            pid_check.EnableWindow(TRUE);
            command_check.EnableWindow(TRUE);
        }
        else {
            AfxMessageBox(_T("Не удалось открыть файл."));
        }
    }
    catch (...) {
        AfxMessageBox(_T("Ошибка при чтении файла."));
    }
}


void CSecurityDlg::OnApplyFilter()
{
    if (state.isEverySame(recentState)) {
        return;
    }
    filterByParameters(state, logs, copied_logs);
}

void CSecurityDlg::OnSearchingByDate()
{   
	recentState.searchByDate = state.searchByDate;
    state.searchByDate = (check_date.GetCheck() == BST_CHECKED);
    setCheckState(check_date, date_from);
    setCheckState(check_date, date_till);
}

void CSecurityDlg::OnSearchingByProc()
{
    recentState.searchByProc = state.searchByProc;
    state.searchByProc = (check_process.GetCheck() == BST_CHECKED);
    setCheckState(check_process, edit_process);
}

void CSecurityDlg::OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
    *pResult = 0;
    recentState.dateFrom = state.dateFrom;
    CTime time;
    date_from.GetTime(time);
    state.dateFrom = time;
}

void CSecurityDlg::OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
    *pResult = 0;
	recentState.dateTill = state.dateTill;
    CTime time;
    date_till.GetTime(time);
    state.dateTill = time;
}

void CSecurityDlg::OnChangeProcName()
{
	recentState.procName = state.procName;
    CString str;
    edit_process.GetWindowTextW(str);
    state.procName = std::wstring(str.GetString());
}

void CSecurityDlg::OnHideInfo()
{
	recentState.isInfoHidden = state.isInfoHidden;
    state.isInfoHidden = (check_info.GetCheck() == BST_CHECKED);
}

void CSecurityDlg::OnHideWarns() {
	recentState.isWarningsHidden = state.isWarningsHidden;
    state.isWarningsHidden = (check_warns.GetCheck() == BST_CHECKED);
}

void setCheckState(CButton& checkBox, CWnd& elem)
{
    elem.EnableWindow(checkBox.GetCheck() == BST_CHECKED);
}

void CSecurityDlg::OnFullCheck()
{
    if (m_pInfoList)
    {
        m_pInfoList->DestroyWindow(); // Уничтожаем старое окно
        delete m_pInfoList;          // Удаляем объект
        m_pInfoList = nullptr;
    }

    m_pInfoList = new CInfoList();
    m_pInfoList->Create(IDD_DIALOG1, nullptr);
    m_pInfoList->ShowWindow(SW_SHOW);
    clearInfoListInputs();
}

void CSecurityDlg::OnSave()
{
    CString logToSave;
    // Если savePath задан, используем его для сохранения файла
    if (!savePath.IsEmpty()) {
        logToSave = savePath;
        AfxMessageBox(L"Файлы трассировки были приняты, обработаны и сохранены по указанному пути!");
    }
    else {
        // Если savePath не задан, открываем диалоговое окно
        CFileDialog fileDialog(FALSE, NULL, L"NotOfS.log", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Лог-файлы (*.log)|*.log|Все файлы (*.*)|*.*||"));
        if (fileDialog.DoModal() == IDOK) {
            logToSave = fileDialog.GetPathName();
        }
        else {
            return; // Если пользователь закрыл диалоговое окно, выходим из метода
        }
    }
    // Сохраняем лог в указанный файл
    std::ofstream fileToSave(logToSave);
    if (fileToSave.is_open()) {
        fileToSave << "PID TIME PROCESS COMMAND DETAILS" << std::endl;
        for (auto& log : logs) {
            std::string pid = WStoS(log.PID);
            std::string time = WStoS(log.timeStr);
            std::string proc = WStoS(log.process);
            std::string command = WStoS(log.command);
            std::string details = WStoS(log.details);
            std::string data = pid + " " + time + " " + "\"" + proc + "\"" + " " + command + " " + "\"" + details + "\"";
            fileToSave << data << std::endl;
        }
        fileToSave.close();
    }
    else {
        AfxMessageBox(_T("Не удалось создать файл для сохранения!"));
    }
}


void CSecurityDlg::OnSaveFiltered()
{
    CFileDialog fileDialog(FALSE, NULL, L"NotOfS_filtered.log");
    if (logPath == "") AfxMessageBox(_T("Файл не выбран!"));
    else if (fileDialog.DoModal() == IDOK)
    {
        auto logToSave = fileDialog.GetPathName();
        std::ofstream fileToSave(logToSave);
        if (fileToSave.is_open())
        {
            fileToSave << "PID TIME PROCESS COMMAND DETAILS" << std::endl;
            for (auto& log : copied_logs) {
                std::string pid = WStoS(log.PID);
                std::string time = WStoS(log.timeStr);
                std::string proc = WStoS(log.process);
                std::string command = WStoS(log.command);
                std::string details = WStoS(log.details);
                std::string data = pid + " " + time + " " + "\"" + proc + "\"" + " " + command + " " + "\"" + details + "\"";
                fileToSave << data << std::endl;
            };
        }
        else
        {
            TRACE(_T("Не удалось создать файл для сохранения!"));
        }
        fileToSave.close();
    }
}

void CSecurityDlg::OnBnClickedCheck5()//pid
{
    recentState.searchByPID = state.searchByPID;
    state.searchByPID = (pid_check.GetCheck() == BST_CHECKED);
    setCheckState(pid_check, PID_edit);
}

void CSecurityDlg::OnBnClickedCheck6()//command
{
    recentState.searchByCommand = state.searchByCommand;
    state.searchByCommand = (command_check.GetCheck() == BST_CHECKED);
    setCheckState(command_check, command_edit);
}

void CSecurityDlg::OnEnChangeEdit8()//pid
{
    recentState.PID = state.PID;
    CString str;
    PID_edit.GetWindowTextW(str);
    state.PID = std::wstring(str.GetString());
}

void CSecurityDlg::OnEnChangeEdit7()//command
{
    recentState.commandName = state.commandName;
    CString str;
    command_edit.GetWindowTextW(str);
    state.commandName = std::wstring(str.GetString());
}
