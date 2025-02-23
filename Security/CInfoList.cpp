// CInfoList.cpp: файл реализации
//

#include "pch.h"
#include "Security.h"
#include "afxdialogex.h"
#include "CInfoList.h"

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
    DDX_Control(pDX, IDC_LIST3, main_list);
    DDX_Text(pDX, IDC_EDIT1, error_count);
    DDX_Text(pDX, IDC_EDIT2, warning_count);
    DDX_Text(pDX, IDC_EDIT3, system_state);
}

BEGIN_MESSAGE_MAP(CInfoList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CInfoList::OnLvnItemchangedList3)
END_MESSAGE_MAP()

// Обработчики сообщений CInfoList

BOOL CInfoList::OnInitDialog()
{
    BOOL bResult = CDialogEx::OnInitDialog(); // Сохраняем возвращаемое значение

    // ВАЖНО: Устанавливаем расширенные стили для отображения сетки и выделения всей строки
    main_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Получаем ширину List Control (используем client rect)
    CRect rect;
    main_list.GetClientRect(&rect);
    int width = rect.Width();

    // Рассчитываем ширину столбцов
    int col1Width = width / 5;
    int col2Width = width / 5;
    int col3Width = width / 5;
    int col4Width = width / 5;
    int col5Width = width - col1Width - col2Width - col3Width - col4Width; // Компенсируем округление

    // Добавляем столбцы
    main_list.InsertColumn(0, _T("Время события"), LVCFMT_LEFT, col1Width);
    main_list.InsertColumn(1, _T("Уровень важности"), LVCFMT_LEFT, col2Width + 25);
    main_list.InsertColumn(2, _T("Процесс"), LVCFMT_LEFT, col3Width - 25);
    main_list.InsertColumn(2, _T("Сообщение"), LVCFMT_LEFT, col4Width);
    main_list.InsertColumn(2, _T("Детали"), LVCFMT_LEFT, col5Width);

    return bResult; // Возвращаем сохраненное значение
}
void CInfoList::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: добавьте свой код обработчика уведомлений
    *pResult = 0;
}
