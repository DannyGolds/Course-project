// CInfoList.h
#pragma once
#include "afxdialogex.h"

// Диалоговое окно CInfoList

class CInfoList : public CDialogEx
{
    DECLARE_DYNAMIC(CInfoList) // ***ДОБАВЬ ЭТУ СТРОКУ***

public:
    CInfoList(CWnd* pParent = nullptr);   // стандартный конструктор
    virtual ~CInfoList();

    enum { IDD = IDD_DIALOG1 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
    virtual BOOL OnInitDialog(); // Обязательно добавьте эту строку

    DECLARE_MESSAGE_MAP()

public:
    CListCtrl main_list;
    afx_msg void OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult);
    CString error_count;
    CString warning_count;
    CString system_state;
};