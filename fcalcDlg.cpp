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
    g_pDialog = this;
    opts.options = PAS + SCI + UPCASE + FFLOAT + SCF + NRM + CMP + IGR + UNS + HEX + CHR + OCT + fBIN + DAT + DEG + STR + ALL + MNU + FRC + FRI;
    ccalc = new calculator(opts.options);
    m_hEditBrush = CreateSolidBrush(RGB(255, 255, 255));
    fVal = 0; // Initialize fVal
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
    ON_COMMAND(ID_FORMAT_ALL, &CfcalcDlg::OnFormatAll) // Исправлено: ID_FORMAT_ALL
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
    ccalc->addfn("menu", showmenu);

    LoadFromRegistry();

    UpdateMenuFromOptions();

    UpdateResult();
    return TRUE;
}

void CfcalcDlg::UpdateMenuFromOptions()
{
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->ModifyMenu(ID_CALC_PASSTYLE, MF_BYCOMMAND | MF_STRING, ID_CALC_PASSTYLE,
            (opts.options & PAS) ? _T("Pas style") : _T("C style"));
        pMenu->CheckMenuItem(ID_CALC_CASESENSETIVE, MF_BYCOMMAND | ((opts.options & UPCASE) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_CALC_FORCEDFLOAT, MF_BYCOMMAND | ((opts.options & FFLOAT) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_SCIENTIFIC, MF_BYCOMMAND | ((opts.options & SCI) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_NORMALIZED, MF_BYCOMMAND | ((opts.options & NRM) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_FRACTION, MF_BYCOMMAND | ((opts.options & FRC) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_COMPUTING, MF_BYCOMMAND | ((opts.options & CMP) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_INTEGER, MF_BYCOMMAND | ((opts.options & IGR) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_UNSIGNED, MF_BYCOMMAND | ((opts.options & UNS) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_HEX, MF_BYCOMMAND | ((opts.options & HEX) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_OCTAL, MF_BYCOMMAND | ((opts.options & OCT) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_BINARY, MF_BYCOMMAND | ((opts.options & FBIN) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_CHAR, MF_BYCOMMAND | ((opts.options & CHR) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_WIDECHAR, MF_BYCOMMAND | ((opts.options & WCH) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_DATETIME, MF_BYCOMMAND | ((opts.options & DAT) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_UNIXTIME, MF_BYCOMMAND | ((opts.options & UTM) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_DEGREESE, MF_BYCOMMAND | ((opts.options & DEG) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_STRING, MF_BYCOMMAND | ((opts.options & STR) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_INCH, MF_BYCOMMAND | ((opts.options & FRI) ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_AUTO, MF_BYCOMMAND | ((opts.options & AUT) ? MF_CHECKED : MF_UNCHECKED));
    }
}

void CfcalcDlg::UpdateOptions()
{
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        // Получаем текущее состояние опций
        UINT state;
        // Pas style (переключаем только текст, галочка всегда активна)
        state = pMenu->GetMenuState(ID_CALC_PASSTYLE, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= PAS; // Устанавливаем бит, если галочка
        }
        else {
            opts.options &= ~PAS; // Снимаем бит, если нет галочки
        }
        // Case Sensitive
        state = pMenu->GetMenuState(ID_CALC_CASESENSETIVE, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options &= ~UPCASE; // Инвертируем: галочка = case sensitive (UPCASE = 0)
        }
        else {
            opts.options |= UPCASE;
        }
        // Forced Float
        state = pMenu->GetMenuState(ID_CALC_FORCEDFLOAT, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= FFLOAT;
        }
        else {
            opts.options &= ~FFLOAT;
        }
        // Scientific
        state = pMenu->GetMenuState(ID_FORMAT_SCIENTIFIC, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= SCI;
        }
        else {
            opts.options &= ~SCI;
        }
        // Normalized
        state = pMenu->GetMenuState(ID_FORMAT_NORMALIZED, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= NRM;
        }
        else {
            opts.options &= ~NRM;
        }
        // Fraction
        state = pMenu->GetMenuState(ID_FORMAT_FRACTION, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= FRC;
        }
        else {
            opts.options &= ~FRC;
        }
        // Computing
        state = pMenu->GetMenuState(ID_FORMAT_COMPUTING, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= CMP;
        }
        else {
            opts.options &= ~CMP;
        }
        // Integer
        state = pMenu->GetMenuState(ID_FORMAT_INTEGER, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= IGR;
        }
        else {
            opts.options &= ~IGR;
        }
        // Unsigned
        state = pMenu->GetMenuState(ID_FORMAT_UNSIGNED, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= UNS;
        }
        else {
            opts.options &= ~UNS;
        }
        // Hex
        state = pMenu->GetMenuState(ID_FORMAT_HEX, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= HEX;
        }
        else {
            opts.options &= ~HEX;
        }
        // Octal
        state = pMenu->GetMenuState(ID_FORMAT_OCTAL, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= OCT;
        }
        else {
            opts.options &= ~OCT;
        }
        // Binary
        state = pMenu->GetMenuState(ID_FORMAT_BINARY, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= FBIN;
        }
        else {
            opts.options &= ~FBIN;
        }
        // Char
        state = pMenu->GetMenuState(ID_FORMAT_CHAR, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= CHR;
        }
        else {
            opts.options &= ~CHR;
        }
        // Wide Char
        state = pMenu->GetMenuState(ID_FORMAT_WIDECHAR, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= WCH;
        }
        else {
            opts.options &= ~WCH;
        }
        // DateTime
        state = pMenu->GetMenuState(ID_FORMAT_DATETIME, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= DAT;
        }
        else {
            opts.options &= ~DAT;
        }
        // UnixTime
        state = pMenu->GetMenuState(ID_FORMAT_UNIXTIME, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= UTM;
        }
        else {
            opts.options &= ~UTM;
        }
        // Degrees
        state = pMenu->GetMenuState(ID_FORMAT_DEGREESE, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= DEG;
        }
        else {
            opts.options &= ~DEG;
        }
        // String
        state = pMenu->GetMenuState(ID_FORMAT_STRING, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= STR;
        }
        else {
            opts.options &= ~STR;
        }
        // Inch
        state = pMenu->GetMenuState(ID_FORMAT_INCH, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= FRI;
        }
        else {
            opts.options &= ~FRI;
        }
        // Auto
        state = pMenu->GetMenuState(ID_FORMAT_AUTO, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= AUT;
        }
        else {
            opts.options &= ~AUT;
        }

        ccalc->syntax(opts.options);
        UpdateMenuFromOptions();
        UpdateResult();
    }
}

void CfcalcDlg::UpdateOptionsAll()
{
    CMenu* pMenu = GetMenu();
    if (pMenu) {
        UINT state = pMenu->GetMenuState(ID_FORMAT_ALL, MF_BYCOMMAND);
        if (state & MF_CHECKED) {
            opts.options |= (SCI + NRM + FRC + CMP + IGR + UNS + HEX + OCT + fBIN + CHR + DAT + UTM + DEG + STR + FRI);
        }
        else {
            opts.options &= ~(SCI + NRM + FRC + CMP + IGR + UNS + HEX + OCT + fBIN + CHR + DAT + UTM + DEG + STR + FRI);
        }
        ccalc->syntax(opts.options);
        UpdateMenuFromOptions();
        UpdateResult();
    }
}

BOOL CfcalcDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
        CString expr;
        m_comboExpr.GetWindowText(expr);
        if (!expr.IsEmpty()) {
            int index = m_comboExpr.FindStringExact(-1, expr);
            if (index == CB_ERR) {
                m_comboExpr.InsertString(0, expr);
                if (m_comboExpr.GetCount() > 100) m_comboExpr.DeleteString(100);
            }
        }
        UpdateResult();
        CString fValStr;
        fValStr.Format(_T("%.16Lg"), fVal);
        m_comboExpr.SetWindowText(fValStr);
        int nLength = m_comboExpr.GetWindowTextLength();
        m_comboExpr.SetEditSel(nLength, nLength);
        m_comboExpr.RedrawWindow();
        return TRUE;
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
    DWORD sel = m_comboExpr.GetEditSel();
    int start = LOWORD(sel);
    int end = HIWORD(sel);

    UpdateResult();

    m_comboExpr.SetEditSel(start, start);
}

void CfcalcDlg::OnCbnSelendokComboExpr()
{
    int nLength = m_comboExpr.GetWindowTextLength();
    m_comboExpr.SetEditSel(nLength, nLength);
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
    CString expr;
    m_comboExpr.GetWindowText(expr);
    if (expr.IsEmpty()) {
        expr = _T("");
        sprintf_s(strings[0], 80, "%66.66s ", " ");
    }

    SetWindowText(expr);

    int64_t iVal = 0;
    // Убрано: fVal = 0; // Не переинициализируем fVal
    char exprBuf[2048];
    memset(strings, 0, sizeof(strings));
    strncpy_s(exprBuf, CStringA(expr), sizeof(exprBuf) - 1);
    exprBuf[sizeof(exprBuf) - 1] = '\0';

    ccalc->syntax(opts.options);
    fVal = ccalc->evaluate(exprBuf, &iVal);

    int scfg = ccalc->issyntax();
    int n = 0;
    n = format_out(opts.options, scfg, 64, n, fVal, iVal, exprBuf, strings, ccalc);
/*
    if (isnan(fVal)) {
        if (ccalc->error()[0]) {
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
        sprintf_s(strings[n++], 80, "Result: %.16Lg", fVal);
        if (iVal != 0) {
            for (int i = 0; i < ((iVal < 19) ? iVal : 19); i++)
                sprintf_s(strings[n++], 80, "Int Value gy%d: %lld", i, iVal);
        }
    }
*/
    if (n == 0) n = 1;

    opts.options = ccalc->issyntax();
    UpdateMenuFromOptions();

    CString resultText;
    int lineCount = 0;
    for (int i = 0; i < n && strings[i][0]; i++) {
        resultText += CString(strings[i]) + _T("\r\n");
        lineCount++;
    }
    m_editResult.SetWindowText(resultText);

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
    UpdateOptions();
}

void CfcalcDlg::OnCalcCaseSensetive()
{
    UpdateOptions();
}

void CfcalcDlg::OnCalcForcedFloat()
{
    UpdateOptions();
}

void CfcalcDlg::OnFormatScientific()
{
    UpdateOptions();
}

void CfcalcDlg::OnFormatAll()
{
    UpdateOptionsAll();
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

        DWORD options = FFLOAT + SCF + NRM + CMP + IGR + UNS + HEX + CHR + OCT + fBIN + DAT + DEG + STR + ALL + MNU + FRC + FRI;
        if (ERROR_SUCCESS == regKey.QueryDWORDValue(_T("Options"), options))
        {
            opts.options = options;
            ccalc->syntax(opts.options);
        }
    }
    regKey.Close();
}

void CfcalcDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_RETURN) {
        CString expr;
        m_comboExpr.GetWindowText(expr);
        if (!expr.IsEmpty()) {
            int index = m_comboExpr.FindStringExact(-1, expr);
            if (index == CB_ERR) {
                m_comboExpr.InsertString(0, expr);
                if (m_comboExpr.GetCount() > 100) m_comboExpr.DeleteString(100);
            }
        }
    }
    else if (nChar == VK_DELETE) {
        int index = m_comboExpr.GetCurSel();
        if (index != CB_ERR) {
            m_comboExpr.DeleteString(index);
            if (index < m_comboExpr.GetCount())
                m_comboExpr.SetCurSel(index);
            else if (m_comboExpr.GetCount())
                m_comboExpr.SetCurSel(m_comboExpr.GetCount() - 1);
            return;
        }
    }
    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CfcalcDlg::OnBnClickedOk()
{
    CDialogEx::OnOK();
}