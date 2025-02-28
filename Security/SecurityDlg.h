﻿
// SecurityDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CSecurityDlg
class CSecurityDlg : public CDialogEx
{
// Создание
public:
	CSecurityDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECURITY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnOpen();
	afx_msg void OnApplyFilter();
	afx_msg void OnFullCheck();
	CEdit show_log;
	afx_msg void OnSaveAs();
	afx_msg void OnBnClickedButton3();
};
