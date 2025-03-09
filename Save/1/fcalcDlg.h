#pragma once
#include "afxwin.h"
#include "scalc.h"

class CfcalcDlg : public CDialogEx
{
public:
    CfcalcDlg(CWnd* pParent = nullptr);
    ~CfcalcDlg();
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
    afx_msg void OnBnClickedOk(); // Обработчик для IDOK
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    int m_lineHeight;
    CComboBox m_comboExpr;
    CEdit m_editResult;
    calculator* ccalc;
    toptions opts;
    char strings[20][80];
    HBRUSH m_hEditBrush;
    void UpdateResult();

    void SaveToRegistry();
    void LoadFromRegistry();
    static const TCHAR* REG_KEY;
};