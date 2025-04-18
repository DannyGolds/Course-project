﻿#pragma once
#include "CInfoList.h"
#include "Structures.h"
#include <vector>

class CSecurityDlg : public CDialogEx
{
public:
    CSecurityDlg(CWnd* pParent = nullptr);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    virtual void OnSysCommand(UINT nID, LPARAM lParam);
    virtual void OnPaint();
    virtual HCURSOR OnQueryDragIcon();

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChangeProcName();
    afx_msg void OnOpen();
    afx_msg void OnApplyFilter();
    afx_msg void OnFullCheck();
    afx_msg void OnSearchingByDate();
    afx_msg void OnSearchingByProc();
    afx_msg void OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult);

private:
    CEdit show_log;
    CEdit edit_process;
    CButton check_warns;
    CButton check_info;
    CButton check_process;
    CButton btn_check;
    CButton check_date;
    CButton btn_apply;
    CDateTimeCtrl date_from;
    CDateTimeCtrl date_till;
    std::vector<LogEntry> logs;
    AppState state;
public:
    afx_msg void OnHideInfo();
    afx_msg void OnHideWarns();
    afx_msg void OnSave();
};