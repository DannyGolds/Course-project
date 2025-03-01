// CInfoList.cpp
#include "pch.h"
#include "Security.h" //  Замените на имя вашего проекта
#include "afxdialogex.h"
#include "CInfoList.h"

#pragma execution_character_set("utf-8")

// Диалоговое окно CInfoList

IMPLEMENT_DYNAMIC(CInfoList, CDialogEx)

CInfoList::CInfoList(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG1, pParent)
    , error_count(_T("0"))
    , warning_count(_T("0"))
    , system_state(_T("Нормальное"))
{
}

CInfoList::~CInfoList()
{
}

void CInfoList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST3, main_list); // Используем main_list
    DDX_Text(pDX, IDC_EDIT1, error_count);
    DDX_Text(pDX, IDC_EDIT2, warning_count);
    DDX_Text(pDX, IDC_EDIT3, system_state);
}

BEGIN_MESSAGE_MAP(CInfoList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CInfoList::OnLvnItemchangedList3)
END_MESSAGE_MAP()

// Обработчики сообщений CInfoList
void CInfoList::SetLogs(const std::vector<LogEntry>& logs)
{
    m_logs = logs;
}

BOOL CInfoList::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CFont font;
    font.CreateFont(14, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, _T("Calibri"));
    // Настройка ListCtrl
    main_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    main_list.InsertColumn(0, _T("Время"), LVCFMT_LEFT, 150);
    main_list.InsertColumn(1, _T("Уровень"), LVCFMT_LEFT, 80);
    main_list.InsertColumn(2, _T("Процесс"), LVCFMT_LEFT, 120);
    main_list.InsertColumn(3, _T("Сообщение"), LVCFMT_LEFT, 250);
    main_list.InsertColumn(4, _T("Детали"), LVCFMT_LEFT, 300);

    for (size_t i = 0; i < m_logs.size(); ++i)
    {
        const LogEntry& logEntry = m_logs[i];

        int nItem = main_list.InsertItem(i, logEntry.timestamp); // Вставляем первую колонку
        main_list.SetItemText(nItem, 1, logEntry.level);
        main_list.SetItemText(nItem, 2, logEntry.process);
        main_list.SetItemText(nItem, 3, logEntry.message);
        main_list.SetItemText(nItem, 4, logEntry.details);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CInfoList::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: добавьте свой код обработчика уведомлений
    *pResult = 0;
}