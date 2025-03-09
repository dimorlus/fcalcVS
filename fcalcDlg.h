// fcalcDlg.h
#pragma once
#include "afxwin.h"
#include "scalc.h"

#define _AFXDLL

class CfcalcDlg : public CDialogEx
{
public:
    CfcalcDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_FCALC_DIALOG };
    static CfcalcDlg* g_pDialog;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    HICON m_hIcon;
    calculator* ccalc;
    struct { int options; bool pas, upcase, ffloat, scf; } opts;
    HBRUSH m_hEditBrush;
    int m_lineHeight;
    CComboBox m_comboExpr;
    CEdit m_editResult;
    char strings[20][80];
    static const TCHAR* REG_KEY; // Добавляем static

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnCbnEditChangeComboExpr();
    afx_msg void OnCbnSelendokComboExpr();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCalcPasStyle();
    afx_msg void OnCalcCaseSensetive();
    afx_msg void OnCalcForcedFloat();
    afx_msg void OnFormatScientific();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnNcPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnClose();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnBnClickedOk();

    void UpdateResult();
    void SaveToRegistry();
    void LoadFromRegistry();
    ~CfcalcDlg();
};