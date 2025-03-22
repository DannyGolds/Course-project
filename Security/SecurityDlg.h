#pragma once
#include "CInfoList.h"
#include "LogStructure.h"
#include "AppState.h"
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
    afx_msg void OnEnChangeEdit2();
    afx_msg void OnOpen();
    afx_msg void OnApplyFilter();
    afx_msg void OnFullCheck();
    afx_msg void OnSaveAs();
    afx_msg void OnBnClickedCheck2();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnChoiceDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChoiceDateTill(NMHDR* pNMHDR, LRESULT* pResult);

private:
    CEdit show_log;
    CEdit edit_process;
    CButton check_warns;
    CButton check_fails;
    CButton check_process;
    CButton check_date;
    CDateTimeCtrl date_from;
    CDateTimeCtrl date_till;

    std::vector<LogEntry> logs;
    AppState state;

    //template<typename T>
    //void readFile(T& file, std::vector<LogEntry>& logs);
    //void setCheckState(CButton& checkBox, CWnd& elem);
};