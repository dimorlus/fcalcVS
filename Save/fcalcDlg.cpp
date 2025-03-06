// fcalcDlg.cpp : implementation file
//
#include "pch.h"
#include "framework.h"
#include "fcalc.h"
#include "fcalcDlg.h"
#include "afxdialogex.h"
#include "sfmts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
    ON_WM_SIZE()
    ON_COMMAND(ID_CALC_PASSTYLE, &CfcalcDlg::OnCalcPasStyle)
    ON_COMMAND(ID_CALC_CASESENSETIVE, &CfcalcDlg::OnCalcCaseSensetive)
    ON_COMMAND(ID_CALC_FORCEDFLOAT, &CfcalcDlg::OnCalcForcedFloat)
    ON_COMMAND(ID_FORMAT_SCIENTIFIC, &CfcalcDlg::OnFormatScientific)
END_MESSAGE_MAP()

BOOL CfcalcDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // Устанавливаем моноширинный шрифт
    CFont font;
    font.CreateFont(
        20,                        // Высота шрифта
        0,                         // Ширина
        0,                         // Угол наклона
        0,                         // Ориентация
        FW_NORMAL,                 // Вес
        FALSE,                     // Курсив
        FALSE,                     // Подчёркивание
        FALSE,                     // Зачёркивание
        ANSI_CHARSET,              // Кодировка
        OUT_DEFAULT_PRECIS,        // Точность вывода
        CLIP_DEFAULT_PRECIS,       // Точность обрезки
        DEFAULT_QUALITY,           // Качество
        FIXED_PITCH | FF_MODERN,   // Моноширинный шрифт
        _T("Courier New")          // Название шрифта
    );
    m_comboExpr.SetFont(&font);
    m_editResult.SetFont(&font);

    m_comboExpr.SetWindowText(_T(""));
    ccalc->addfn("menu", showmenu);

    CMenu* pMenu = GetMenu();
    if (pMenu) {
        pMenu->ModifyMenu(ID_CALC_PASSTYLE, MF_BYCOMMAND | MF_STRING, ID_CALC_PASSTYLE, _T("Pas style"));
        pMenu->CheckMenuItem(ID_CALC_CASESENSETIVE, MF_BYCOMMAND | (opts.upcase ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_CALC_FORCEDFLOAT, MF_BYCOMMAND | (opts.ffloat ? MF_CHECKED : MF_UNCHECKED));
        pMenu->CheckMenuItem(ID_FORMAT_SCIENTIFIC, MF_BYCOMMAND | (opts.scf ? MF_CHECKED : MF_UNCHECKED));
    }

    UpdateResult();
    return TRUE;
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
    UpdateResult();
}

void CfcalcDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_editResult.m_hWnd && m_comboExpr.m_hWnd) {
        m_comboExpr.MoveWindow(7, 7, 358, 50);    // Начальные координаты ComboBox
        m_editResult.MoveWindow(7, 40, 358, cy - 50 - 7);  // Начальные координаты Edit
    }
}

void CfcalcDlg::UpdateResult()
{
    CString expr;
    m_comboExpr.GetWindowText(expr);
    if (expr.IsEmpty()) {
        expr = _T("");
        sprintf_s(strings[0], 80, "%66.66s ", " "); // Пустая строка при запуске
    }

    SetWindowText(expr);

    int64_t iVal = 0;
    float__t fVal = 0;
    char exprBuf[2048];
    memset(strings, 0, sizeof(strings));
    strncpy_s(exprBuf, CStringA(expr), sizeof(exprBuf) - 1);
    exprBuf[sizeof(exprBuf) - 1] = '\0';

    ccalc->syntax(opts.options);
    fVal = ccalc->evaluate(exprBuf, &iVal);

    int scfg = ccalc->issyntax();
    int n = 0;

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
        sprintf_s(strings[n++], 80, "Result gy: %.2Lf", fVal);
        if (iVal != 0) {
            sprintf_s(strings[n++], 80, "Int Value gy1: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy2: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy3: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy4: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy5: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy6: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy7: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy8: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy9: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy10: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy11: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy12: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy13: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy14: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy15: %lld", iVal);
            sprintf_s(strings[n++], 80, "Int Value gy16: %lld", iVal);

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

    int index = m_comboExpr.FindStringExact(-1, expr);
    if (index == CB_ERR) {
        m_comboExpr.InsertString(0, expr);
        if (m_comboExpr.GetCount() > 20) m_comboExpr.DeleteString(20);
    }
    // Адаптируем высоту окна по количеству строк
    CFont* pFont = m_editResult.GetFont();
    LOGFONT lf;
    pFont->GetLogFont(&lf);
    HDC hDC = ::GetDC(m_hWnd);
    HFONT hFont = CreateFontIndirect(&lf);
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    TEXTMETRIC tm;
    GetTextMetrics(hDC, &tm);
    int lineHeight = tm.tmHeight + tm.tmExternalLeading; // Высота строки
    SelectObject(hDC, hOldFont);
    DeleteObject(hFont);
    ::ReleaseDC(m_hWnd, hDC);


    int margin = 0; // Убираем лишний запас, чтобы убрать "мёртвую полосу"
    int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
    int menuHeight = GetMenu() ? GetSystemMetrics(SM_CYMENU) : 0;
    int borderHeight = GetSystemMetrics(SM_CYFRAME) * 2;

    lineHeight = 14;
    int totalExtraHeight = titleBarHeight + menuHeight + borderHeight + margin + 50; // Учитываем ComboBox (50)
    int newHeight = totalExtraHeight +30 + (lineHeight * (lineCount ? lineCount : 1));
    SetWindowPos(nullptr, 0, 0, 388, newHeight, SWP_NOMOVE | SWP_NOZORDER);
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