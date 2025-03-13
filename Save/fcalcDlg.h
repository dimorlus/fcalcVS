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
    afx_msg void OnFormatNormalized();
    afx_msg void OnFormatFraction();
    afx_msg void OnFormatComputing();
    afx_msg void OnFormatInteger();
    afx_msg void OnFormatUnsigned();
    afx_msg void OnFormatHex();
    afx_msg void OnFormatOctal();
    afx_msg void OnFormatBinary();
    afx_msg void OnFormatChar();
    afx_msg void OnFormatWideChar();
    afx_msg void OnFormatDateTime();
    afx_msg void OnFormatUnixTime();
    afx_msg void OnFormatDegrees();
    afx_msg void OnFormatString();
    afx_msg void OnFormatInch();
    afx_msg void OnFormatAuto();
    afx_msg void OnFormatAll();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnNcPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnClose();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnBnClickedOk();
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
    float__t fVal;
    void UpdateResult();

    void SaveToRegistry();
    void LoadFromRegistry();
    static const TCHAR* REG_KEY;

    void UpdateMenuFromOptions();
    void UpdateOptions();
    void UpdateOptionsAll();
    void UpdateOptionFromMenu(UINT menuId, UINT optionBit); // Новая функция

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};