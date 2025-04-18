#include "pch.h"
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
    CFileDialog fileDialog(TRUE, _T("strace.log"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Файлы журналирования |*.log;*.txt||"));
    if (fileDialog.DoModal() == IDOK)
    {
        logPath = fileDialog.GetPathName();
        std::ifstream file(logPath, std::ios::in);
        try {
                show_log.SetWindowTextW(logPath);
                readFile(file, logs);
                file.close();
                btn_check.EnableWindow(TRUE);
                check_date.EnableWindow(TRUE);
                check_info.EnableWindow(TRUE);
                check_process.EnableWindow(TRUE);
                check_warns.EnableWindow(TRUE);
                btn_apply.EnableWindow(TRUE);
        }
        catch(...) {
            AfxMessageBox(_T("Ошибка открытия файла"));
        }
    }
}

void CSecurityDlg::OnApplyFilter()
{
    filterByParameters(state, logs, copied_logs);
}

void CSecurityDlg::OnSearchingByDate()
{
    state.searchByDate = (check_date.GetCheck() == BST_CHECKED);
    setCheckState(check_date, date_from);
    setCheckState(check_date, date_till);
}

void CSecurityDlg::OnSearchingByProc()
{
    state.searchByProc = (check_process.GetCheck() == BST_CHECKED);
    setCheckState(check_process, edit_process);
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

void CSecurityDlg::OnChangeProcName()
{
    CString str;
    edit_process.GetWindowTextW(str);
    state.procName = std::wstring(str.GetString());
}

void CSecurityDlg::OnHideInfo()
{
    state.isInfoHidden = (check_info.GetCheck() == BST_CHECKED);
}

void CSecurityDlg::OnHideWarns() {
    state.isWarningsHidden = (check_warns.GetCheck() == BST_CHECKED);
}

void setCheckState(CButton& checkBox, CWnd& elem)
{
    elem.EnableWindow(checkBox.GetCheck() == BST_CHECKED);
}

void CSecurityDlg::OnFullCheck()
{
    CInfoList list;
    list.DoModal();
}

void CSecurityDlg::OnSave()
{
    CFileDialog fileDialog(FALSE, NULL, L"Конфигурация системы.log");
    if (fileDialog.DoModal() == IDOK)
    {
        auto logToSave = fileDialog.GetPathName();
        std::ofstream fileToSave(logToSave, std::ios::app);
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

