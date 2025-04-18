#include "pch.h"
#include "Security.h" // Замените на имя вашего проекта
#include "afxdialogex.h"
#include "CInfoList.h"
#include "Structures.h"
#include "Globals.h"

IMPLEMENT_DYNAMIC(CInfoList, CDialogEx)

CInfoList::CInfoList(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG1, pParent)
{
}

CInfoList::~CInfoList()
{
}

void CInfoList::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST3, main_list); // Используем main_list
}

BEGIN_MESSAGE_MAP(CInfoList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CInfoList::OnLvnItemchangedList3)
END_MESSAGE_MAP()

std::wstring CTimeToWString(const CTime&);

BOOL CInfoList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Настройка ListCtrl
    main_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Добавляем колонки
    main_list.InsertColumn(0, _T("ID процесса"), LVCFMT_LEFT, 100);
    main_list.InsertColumn(1, _T("Время исполнения"), LVCFMT_LEFT, 100);
    main_list.InsertColumn(2, _T("Название процесса"), LVCFMT_LEFT, 300);
    main_list.InsertColumn(3, _T("Имя команды"), LVCFMT_LEFT, 120);
    main_list.InsertColumn(4, _T("Уровень подозрительности"), LVCFMT_LEFT, 250);

    // Заполняем список данными из m_logs
    for (size_t i = 0; i < copied_logs.size(); ++i)
    {
        const LogEntry& logEntry = copied_logs[i];
        std::wstring string_time = CTimeToWString(logEntry.time);
        // Вставляем строку и заполняем колонки
        auto nItem = main_list.InsertItem(i, logEntry.PID.c_str()); // ID процесса
        main_list.SetItemText(nItem, 1, string_time.c_str()); //Время
        main_list.SetItemText(nItem, 2, logEntry.process.c_str());       // Процесс
        main_list.SetItemText(nItem, 3, logEntry.command.c_str());     // Команда
        main_list.SetItemText(nItem, 4, logEntry.details.c_str());     // Детали
    }

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CInfoList::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: добавьте свой код обработчика уведомлений
    *pResult = 0;
}

std::wstring CTimeToWString(const CTime& time) {
    // Форматируем дату через CTime::Format()
    CString formattedTime = time.Format(L"%T");

    // Конвертируем CString в std::wstring
    return std::wstring(formattedTime.GetString());
}