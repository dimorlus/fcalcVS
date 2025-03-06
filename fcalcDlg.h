// fcalcDlg.h
#pragma once
#include "afxwin.h"
#include "scalc.h"

class CfcalcDlg : public CDialogEx
{
public:
    CfcalcDlg(CWnd* pParent = nullptr);
    static CfcalcDlg* g_pDialog;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_FCALC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnCbnEditChangeComboExpr();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCalcPasStyle();
    afx_msg void OnCalcCaseSensetive();
    afx_msg void OnCalcForcedFloat();
    afx_msg void OnFormatScientific();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnNcPaint(); // Для рисования рамки ComboBox
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    CComboBox m_comboExpr;
    CEdit m_editResult;
    calculator* ccalc;
    toptions opts;
    char strings[20][80];
    HBRUSH m_hEditBrush;
    void UpdateResult();
};