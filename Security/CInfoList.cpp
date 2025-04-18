#include "pch.h"
#include "Security.h" // Замените на имя вашего проекта
#include "afxdialogex.h"
#include "CInfoList.h"
#include "Structures.h"
#include "Utils.h"  
#include "Globals.h"
#define WM_UPDATE_LOGS (WM_USER + 1)
IMPLEMENT_DYNAMIC(CInfoList, CDialogEx)

CInfoList::CInfoList(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG1, pParent)
	, cpl_l_edit(_T(""))
	, cpl_m_edit(_T(""))
	, cpl_h_edit(_T(""))
{
}

CInfoList::~CInfoList()
{
}


void CInfoList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, main_list); // Используем main_list
	DDX_Text(pDX, IDC_EDIT1, cpl_l_edit);
	DDX_Text(pDX, IDC_EDIT2, cpl_m_edit);
	DDX_Text(pDX, IDC_EDIT3, cpl_h_edit);
}

BEGIN_MESSAGE_MAP(CInfoList, CDialogEx)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CInfoList::OnLvnItemchangedList3)
END_MESSAGE_MAP()

std::wstring CTimeToWString(const CTime&);

BOOL CInfoList::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    ModifyStyleEx(WS_EX_TOPMOST, 0);
    ModifyStyle(WS_CHILD | WS_POPUP, WS_OVERLAPPEDWINDOW);
    // Настройка ListCtrl
    main_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    main_list.InsertColumn(0, _T("ID процесса"), LVCFMT_LEFT, 100);
    main_list.InsertColumn(1, _T("Время исполнения"), LVCFMT_LEFT, 180);
    main_list.InsertColumn(2, _T("Название процесса"), LVCFMT_LEFT, 400);
    main_list.InsertColumn(3, _T("Имя команды"), LVCFMT_LEFT, 120);
    main_list.InsertColumn(4, _T("Уровень подозрительности"), LVCFMT_LEFT, 260);
    this->FillList(copied_logs, logs);
	this->ShowMessageRes();
    // Заполняем список данными из m_logs
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CInfoList::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: добавьте свой код обработчика уведомлений
    *pResult = 0;
}

void CInfoList::PostNcDestroy()
{
    CDialogEx::PostNcDestroy();
}

void CInfoList::FillList(const std::vector<LogEntry>& lg, const std::vector<LogEntry>& logs_p)
{
	main_list.DeleteAllItems(); // Очистка списка перед заполнением

	for (size_t i = 0; i < lg.size(); ++i)
	{
		const LogEntry& log = lg[i];
		main_list.InsertItem(i, log.PID.c_str());
		main_list.SetItemText(i, 1, log.timeStr.c_str());
		main_list.SetItemText(i, 2, log.process.c_str());
		main_list.SetItemText(i, 3, log.command.c_str());
		main_list.SetItemText(i, 4, log.details.c_str());
		if (log.details.find(L"Низк") != std::wstring::npos) {
			cpl_l++;
		}
		else if (log.details.find(L"Сред") != std::wstring::npos) {
			cpl_m++;
		}
		else if (log.details.find(L"Выс") != std::wstring::npos) {
			cpl_h++;
		}
	}

	cpl_l_edit.Format(_T("%d"), cpl_l);
	cpl_m_edit.Format(_T("%d"), cpl_m);
	cpl_h_edit.Format(_T("%d"), cpl_h);
	UpdateData(FALSE); // Обновляем отображение данных в диалоговом окне
}

void CInfoList::ShowMessageRes()
{
	auto resOfWork = calculateSuspicionPercentage(cpl_l, cpl_m, cpl_h);
	CString message;
	message.Format(L"При данных настройках действия подозрительны на %.2f%%", resOfWork);
	AfxMessageBox(message);
}
