// CInfoList.h
#pragma once
#include "afxdialogex.h"
#include "Structures.h"
#include <vector>
#include "Utils.h"
#include <string>

// Диалоговое окно CInfoList

class CInfoList : public CDialogEx
{
    DECLARE_DYNAMIC(CInfoList) // ***ДОБАВЬ ЭТУ СТРОКУ***

public:
    CInfoList(CWnd* pParent = nullptr);   // стандартный конструктор
    enum { IDD = IDD_DIALOG1 };
    virtual void PostNcDestroy() override;
    virtual ~CInfoList();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
    virtual BOOL OnInitDialog(); // Обязательно добавьте эту строку
    DECLARE_MESSAGE_MAP()

public:
    CListCtrl main_list;
    afx_msg void FillList(const std::vector<LogEntry>&, const std::vector<LogEntry>&); 
    afx_msg void ShowMessageRes(const int& cpl_l, const int& cpl_m, const int& cpl_h);// Метод для заполнения списка данными
    afx_msg void OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult);
    CString cpl_l_edit;
    CString cpl_m_edit;
    CString cpl_h_edit;
    afx_msg void OnShowSummary();
    afx_msg void OnListClose();
};