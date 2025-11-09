
// TurtleRaceDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TurtleRace.h"
#include "TurtleRaceDlg.h"
#include "afxdialogex.h"

#include "CreateGameDlg.h"
#include "JoinGameDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTurtleRaceDlg dialog



CTurtleRaceDlg::CTurtleRaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TURTLERACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTurtleRaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTurtleRaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_QUIT, &CTurtleRaceDlg::OnMenuQuit)
	ON_COMMAND(ID_MENU_NEWGAME, &CTurtleRaceDlg::OnMenuNewgame)
	ON_COMMAND(ID_MENU_JOINGAME, &CTurtleRaceDlg::OnMenuJoingame)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CTurtleRaceDlg message handlers

BOOL CTurtleRaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetTimer(IDT_TIMER, 20, NULL);

	{
		CRect		client;
		GetClientRect(client);
		client.NormalizeRect();

		m_game_.set_dialog(this);
		m_game_.init_client_rectangle(client);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTurtleRaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTurtleRaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		CDialogEx::OnPaint();
		CPaintDC	paint_dc(this);	// контекст устройства для рисования
		CDC			dcMem;
		CBitmap		bitmap;
		CRect		client;

		GetClientRect(client);
		client.NormalizeRect();

		dcMem.CreateCompatibleDC(&paint_dc);
		bitmap.CreateCompatibleBitmap(&paint_dc, client.Width(), client.Height());

		CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);
		CPen* pOldPen = dcMem.GetCurrentPen();
		CFont* pOldFont = dcMem.GetCurrentFont();

		// Здесь рисуем все, что должно быть на экране
		m_game_.Draw(&dcMem, client);

		paint_dc.BitBlt(client.left, client.top, client.Width(), client.Height(), &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pOldBitmap);
		dcMem.SelectObject(pOldPen);
		dcMem.SelectObject(pOldFont);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTurtleRaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTurtleRaceDlg::OnMenuQuit()
{
	if (m_game_.game_in_progress())
	{
		if (MessageBox("Вход в игру выполнен.\nТочно хотите выйти?", "Вопрос", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
			return;
	}

	KillTimer(IDT_TIMER);
	CDialogEx::OnCancel();
}

void CTurtleRaceDlg::OnCancel()
{
	OnMenuQuit();
}

void CTurtleRaceDlg::OnMenuNewgame()
{
	// TODO: Add your command handler code here
	if (!m_game_.game_in_progress())
	{
		CreateGameDlg	dlg_create_game;
		dlg_create_game.gamer_ = m_game_.get_gamer();

		if (dlg_create_game.DoModal() == IDOK)
		{
			m_game_.create_game(dlg_create_game.gamer_);
		}
	}
	else
		MessageBox("Команда отменена:\nИгра уже начата", "Не выполнено", MB_ICONERROR);
}

void CTurtleRaceDlg::OnMenuJoingame()
{
	// TODO: Add your command handler code here
	if (!m_game_.game_in_progress())
	{
		JoinGameDlg	dlg_join_game;
		dlg_join_game.gamer_ = m_game_.get_gamer();

		if (dlg_join_game.DoModal() == IDOK)
		{
			m_game_.join_game(dlg_join_game.gamer_);
		}
	}
	else
		MessageBox("Команда отменена:\nИгра уже начата", "Не выполнено", MB_ICONERROR);
}

void CTurtleRaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_game_.on_timer();

	CDialogEx::OnTimer(nIDEvent);
}

void CTurtleRaceDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_game_.on_mouse_move(point);

	CDialogEx::OnMouseMove(nFlags, point);
}

void CTurtleRaceDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_game_.on_mouse_click(point);

	CDialogEx::OnLButtonDown(nFlags, point);
}
