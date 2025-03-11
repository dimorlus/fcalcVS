#include "pch.h"
#include "framework.h"
#include "fcalc.h"
#include "fcalcDlg.h"
#include "afxdialogex.h"
#include "sfmts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* CfcalcDlg::REG_KEY = _T("Software\\MyCalcApp");

CfcalcDlg* CfcalcDlg::g_pDialog = nullptr;

void showmenu(int param) {
    if (CfcalcDlg::g_pDialog) {
        if (param == 0) {
            CfcalcDlg::g_pDialog->SetMenu(nullptr);
        }
        else {
            CMenu menu;
            menu.LoadMenu(IDR_MENU1);
            CfcalcDlg::g_pDialog->SetMenu(&menu);
            menu.Detach();
        }
        CfcalcDlg::g_pDialog->DrawMenuBar();
    }
}

CfcalcDlg::CfcalcDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_FCALC_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    ccalc = new calculator(PAS | SCI | UPCASE);
    g_pDialog = this;
    opts.options = PAS | SCI | UPCASE;
    m_hEditBrush = CreateSolidBrush(RGB(255, 255, 255));
}

CfcalcDlg::~CfcalcDlg()
{
    if (m_hEditBrush)
        DeleteObject(m_hEditBrush);
    if (ccalc)
        delete ccalc;
}

void CfcalcDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_EXPR, m_comboExpr);
    DDX_Control(pDX, IDC_EDIT_RESULT, m_editResult);
    if (pDX->m_bSaveAndValidate && !m_comboExpr.m_hWnd) {
        TRACE(_T("m_comboExpr is not initialized!\n"));
    }
}

BEGIN_MESSAGE_MAP(CfcalcDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_CBN_EDITCHANGE(IDC_COMBO_EXPR, &CfcalcDlg::OnCbnEditChangeComboExpr)
    ON_CBN_SELENDOK(IDC_COMBO_EXPR, &CfcalcDlg::OnCbnSelendokComboExpr)
    ON_WM_SIZE()
    ON_COMMAND(ID_CALC_PASSTYLE, &CfcalcDlg::OnCalcPasStyle)
    ON_COMMAND(ID_CALC_CASESENSETIVE, &CfcalcDlg::OnCalcCaseSensetive)
    ON_COMMAND(ID_CALC_FORCEDFLOAT, &CfcalcDlg::OnCalcForcedFloat)
    ON_COMMAND(ID_FORMAT_SCIENTIFIC, &CfcalcDlg::OnFormatScientific)
    ON_WM_CTLCOLOR()
    ON_WM_NCPAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CLOSE()
    ON_WM_KEYDOWN()
    ON_BN_CLICKED(IDOK, &CfcalcDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CfcalcDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    CFont font;
    font.CreateFont(
        20,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN,
        _T("Courier New")
    );
    m_comboExpr.SetFont(&font);
    m_editResult.SetFont(&font);

    CFont* pFont = m_editResult.GetFont();
    LOGFONT lf;
    pFont->GetLogFont(&lf);
    HDC hDC = ::GetDC(m_hWnd);
    HFONT hFont = CreateFontIndirect(&lf);
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    TEXTMETRIC tm;
    GetTextMetrics(hDC, &tm);
    m_lineHeight = tm.tmHeight + tm.tmExternalLeading;
    SelectObject(hDC, hOldFont);
    DeleteObject(hFont);
    ::ReleaseDC(m_hWnd, hDC);

    m_comboExpr.SetWindowText(_T(""));
    m_comboExpr.SetFocus(); // Устанавливаем фокус на комбо-бокс при старте
    ccalc->addfn("menu", showmenu);

    LoadFromRegistry();

    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->ModifyMenu(ID_CALC_PASSTYLE, MF_BYCOMMAND | MF_STRING, ID_CALC_PASSTYLE,
            opts.pas ? _T("Pas style") : _T("C style"));
        pMenu->CheckMenuItem(ID_CALC_CASESENSETIVE, MF_BYCOMMAND | (opts.upcase ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_CALC_FORCEDFLOAT, MF_BYCOMMAND | (opts.ffloat ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_SCIENTIFIC, MF_BYCOMMAND | (opts.scf ? MF_CHECKED : MF_UNCHECKED));
    }

    UpdateResult();
    return TRUE;
}

BOOL CfcalcDlg::PreTranslateMessage(MSG* pMsg)
{
    TRACE(_T("PreTranslateMessage: message = %d, nChar = %d\n"), pMsg->message, pMsg->wParam);
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN) {
            CString expr;
            m_comboExpr.GetWindowText(expr);
            TRACE(_T("PreTranslateMessage: expr = '%s', IsEmpty = %d\n"), expr, expr.IsEmpty());
            if (!expr.IsEmpty()) {
                int index = m_comboExpr.FindStringExact(-1, expr);
                TRACE(_T("PreTranslateMessage: FindStringExact returned %d\n"), index);
                if (index == CB_ERR) {
                    m_comboExpr.InsertString(0, expr);
                    if (m_comboExpr.GetCount() > 100) m_comboExpr.DeleteString(100); // Лимит 100
                    TRACE(_T("PreTranslateMessage: Added expr '%s' to history, count = %d\n"), expr, m_comboExpr.GetCount());
                }
            }
            UpdateResult(); // Обновляем результат
            return TRUE; // Перехватываем сообщение
        }
        else if (pMsg->wParam == VK_DELETE) {
            TRACE(_T("PreTranslateMessage: VK_DELETE detected, dropped state = %d\n"), m_comboExpr.GetDroppedState());
            if (m_comboExpr.GetDroppedState()) {
                int selIndex = m_comboExpr.GetCurSel();
                if (selIndex != CB_ERR) {
                    m_comboExpr.DeleteString(selIndex);
                    TRACE(_T("PreTranslateMessage: Deleted item at index %d, new count = %d\n"), selIndex, m_comboExpr.GetCount());
                }
                return TRUE; // Перехватываем сообщение
            }
            // Если список закрыт, передаём событие дальше для редактирования
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void CfcalcDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CfcalcDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CfcalcDlg::OnCbnEditChangeComboExpr()
{
    // Get current cursor position (start and end of selection)
    DWORD sel = m_comboExpr.GetEditSel();
    int start = LOWORD(sel);
    int end = HIWORD(sel);

    // Update the result
    UpdateResult();

    // Restore cursor position
    m_comboExpr.SetEditSel(start, start);
}
void CfcalcDlg::OnCbnSelendokComboExpr()
{
    int nLength = m_comboExpr.GetWindowTextLength();
    m_comboExpr.SetEditSel(nLength, nLength);
    // Убираем добавление в историю при выборе из списка
}

void CfcalcDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_editResult.m_hWnd && m_comboExpr.m_hWnd) {
        CRect comboRect;
        m_comboExpr.GetWindowRect(&comboRect);
        ScreenToClient(&comboRect);
        int comboHeight = comboRect.Height();
        m_comboExpr.MoveWindow(0, 0, cx, comboHeight);
        m_editResult.MoveWindow(0, comboHeight + 1, cx, cy - comboHeight - 1);
    }
}

void CfcalcDlg::UpdateResult()
{
    if (!m_comboExpr.m_hWnd) {
        TRACE(_T("m_comboExpr is not valid in UpdateResult!\n"));
        return;
    }

    CString expr;
    m_comboExpr.GetWindowText(expr);
    TRACE(_T("UpdateResult: Input expression = '%s'\n"), expr);
    if (expr.IsEmpty()) {
        expr = _T("");
        sprintf_s(strings[0], 80, "%66.66s ", " ");
    }

    SetWindowText(expr);

    int64_t iVal = 0;
    float__t fVal = 0;
    char exprBuf[2048];
    memset(strings, 0, sizeof(strings));
    strncpy_s(exprBuf, CStringA(expr), sizeof(exprBuf) - 1);
    exprBuf[sizeof(exprBuf) - 1] = '\0';
    TRACE(_T("UpdateResult: Converted to exprBuf = '%hs'\n"), exprBuf);

    ccalc->syntax(opts.options);
    TRACE(_T("UpdateResult: Before evaluate, opts.options = %d\n"), opts.options);
    fVal = ccalc->evaluate(exprBuf, &iVal);
    TRACE(_T("UpdateResult: After evaluate, fVal = %Lf, iVal = %lld\n"), fVal, iVal);

    int scfg = ccalc->issyntax();
    int n = 0;

    if (isnan(fVal)) {
        if (ccalc->error()[0]) {
            TRACE(_T("UpdateResult: Error detected: %s\n"), ccalc->error());
            if (ccalc->errps() < 64) {
                char binstr[80];
                memset(binstr, '-', sizeof(binstr));
                binstr[ccalc->errps()] = '^';
                binstr[ccalc->errps() + 1] = '\0';
                sprintf_s(strings[n++], 80, "%-64s", binstr);
                sprintf_s(strings[n++], 80, "%66.66s ", ccalc->error());
            }
            else {
                sprintf_s(strings[n++], 80, "%66.66s ", ccalc->error());
            }
        }
        else {
            if (!expr.IsEmpty())
                sprintf_s(strings[n++], 80, "%66.66s ", "NaN");
            else
                sprintf_s(strings[n++], 80, "%66.66s ", " ");
        }
    }
    else {
        sprintf_s(strings[n++], 80, "Result: %.16Lg", fVal); // Формат %.16Lg
        if (iVal != 0) {
            for (int i = 0; i < ((iVal < 19) ? iVal : 19); i++)
                sprintf_s(strings[n++], 80, "Int Value gy%d: %lld", i, iVal);
        }
    }

    opts.options = ccalc->issyntax();
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->CheckMenuItem(ID_CALC_CASESENSETIVE, MF_BYCOMMAND | (opts.upcase ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_CALC_FORCEDFLOAT, MF_BYCOMMAND | (opts.ffloat ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_SCIENTIFIC, MF_BYCOMMAND | (opts.scf ? MF_CHECKED : MF_UNCHECKED));
    }

    CString resultText;
    int lineCount = 0;
    for (int i = 0; i < 20 && strings[i][0] != '\0'; i++) {
        resultText += CString(strings[i]) + _T("\r\n");
        lineCount++;
    }
    m_editResult.SetWindowText(resultText);
    TRACE(_T("UpdateResult: SetWindowText completed with %d lines\n"), lineCount);

    int margin = 0;
    int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
    int menuHeight = GetMenu() ? GetSystemMetrics(SM_CYMENU) : 0;
    int borderHeight = GetSystemMetrics(SM_CYFRAME) * 2;
    int comboHeight = 40;

    int totalExtraHeight = titleBarHeight + menuHeight + borderHeight + margin + comboHeight;
    int newHeight = totalExtraHeight + 4 + (m_lineHeight * lineCount);
    SetWindowPos(nullptr, 0, 0, 520, newHeight, SWP_NOMOVE | SWP_NOZORDER);
}

void CfcalcDlg::OnCalcPasStyle()
{
    opts.pas = !opts.pas;
    ccalc->syntax(opts.options);
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->ModifyMenu(ID_CALC_PASSTYLE, MF_BYCOMMAND | MF_STRING, ID_CALC_PASSTYLE,
            opts.pas ? _T("Pas style") : _T("C style"));
    }
    UpdateResult();
}

void CfcalcDlg::OnCalcCaseSensetive()
{
    opts.upcase = !opts.upcase;
    ccalc->syntax(opts.options);
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->CheckMenuItem(ID_CALC_CASESENSETIVE, MF_BYCOMMAND | (opts.upcase ? MF_CHECKED : MF_UNCHECKED));
    }
    UpdateResult();
}

void CfcalcDlg::OnCalcForcedFloat()
{
    opts.ffloat = !opts.ffloat;
    ccalc->syntax(opts.options);
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->CheckMenuItem(ID_CALC_FORCEDFLOAT, MF_BYCOMMAND | (opts.ffloat ? MF_CHECKED : MF_UNCHECKED));
    }
    UpdateResult();
}

void CfcalcDlg::OnFormatScientific()
{
    opts.scf = !opts.scf;
    ccalc->syntax(opts.options);
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->CheckMenuItem(ID_FORMAT_SCIENTIFIC, MF_BYCOMMAND | (opts.scf ? MF_CHECKED : MF_UNCHECKED));
    }
    UpdateResult();
}

void CfcalcDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    TRACE(_T("OnKeyDown: nChar = %d\n"), nChar);
    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags); // Передаём событие дальше
}

void CfcalcDlg::OnBnClickedOk()
{
    UpdateResult();
}

HBRUSH CfcalcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if (pWnd->GetDlgCtrlID() == IDC_EDIT_RESULT) {
        pDC->SetBkMode(OPAQUE);
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 0));
        return m_hEditBrush;
    }
    if (nCtlColor == CTLCOLOR_DLG) {
        pDC->SetBkColor(RGB(255, 255, 255));
        return m_hEditBrush;
    }
    return hbr;
}

void CfcalcDlg::OnNcPaint()
{
    CDialogEx::OnNcPaint();
}

BOOL CfcalcDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    pDC->FillSolidRect(&rect, RGB(255, 255, 255));
    return TRUE;
}

void CfcalcDlg::OnClose()
{
    SaveToRegistry();
    CDialogEx::OnClose();
}

void CfcalcDlg::SaveToRegistry()
{
    if (!m_comboExpr.m_hWnd) {
        TRACE(_T("m_comboExpr is not valid in SaveToRegistry!\n"));
        return;
    }

    CRegKey regKey;
    if (ERROR_SUCCESS == regKey.Create(HKEY_CURRENT_USER, REG_KEY))
    {
        int count = m_comboExpr.GetCount();
        regKey.SetDWORDValue(_T("HistoryCount"), min(count, 100));
        for (int i = 0; i < min(count, 100); i++)
        {
            CString item;
            m_comboExpr.GetLBText(i, item);
            CString keyName;
            keyName.Format(_T("History%d"), i);
            regKey.SetStringValue(keyName, item);
        }

        regKey.SetDWORDValue(_T("Options"), opts.options);
    }
    regKey.Close();
}

void CfcalcDlg::LoadFromRegistry()
{
    CRegKey regKey;
    if (ERROR_SUCCESS == regKey.Open(HKEY_CURRENT_USER, REG_KEY, KEY_READ))
    {
        DWORD count = 0;
        regKey.QueryDWORDValue(_T("HistoryCount"), count);
        for (DWORD i = 0; i < min(count, 100); i++)
        {
            CString keyName;
            keyName.Format(_T("History%d"), i);
            CString item;
            ULONG len = 256;
            if (ERROR_SUCCESS == regKey.QueryStringValue(keyName, item.GetBuffer(len), &len))
            {
                item.ReleaseBuffer();
                m_comboExpr.AddString(item);
            }
        }

        DWORD options = 0;
        if (ERROR_SUCCESS == regKey.QueryDWORDValue(_T("Options"), options))
        {
            opts.options = options;
            ccalc->syntax(opts.options);
        }
    }
    regKey.Close();
}