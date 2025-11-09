// JoingameDlg.cpp : implementation file
//

#include "pch.h"
#include "TurtleRace.h"
#include "afxdialogex.h"
#include "JoinGameDlg.h"


// JoingameDlg dialog

IMPLEMENT_DYNAMIC(JoinGameDlg, CDialogEx)

JoinGameDlg::JoinGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JOINGAME_DIALOG, pParent)
	, m_iPort(0)
	, m_sLogin(_T(""))
{
}

JoinGameDlg::~JoinGameDlg()
{
}

void JoinGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REMOTEIPADDRESS, m_cIPAddress);
	DDX_Text(pDX, IDC_EPORT, m_iPort);
	DDX_Text(pDX, IDC_ELOGIN, m_sLogin);
}


BEGIN_MESSAGE_MAP(JoinGameDlg, CDialogEx)
END_MESSAGE_MAP()


// JoinGameDlg message handlers

BOOL JoinGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_iPort = gamer_.port_;
	m_sLogin = gamer_.login_.c_str();
	m_cIPAddress.SetWindowText(gamer_.ip_address_.c_str());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void JoinGameDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();

	gamer_.port_ = m_iPort;
	gamer_.login_ = LPCTSTR(m_sLogin);

	CString		sAddress;
	m_cIPAddress.GetWindowTextA(sAddress);
	gamer_.ip_address_ = LPCSTR(sAddress);


	if (gamer_.login_.empty())
	{
		MessageBox("Не задано имя", "Неверно", MB_ICONERROR);
		return;
	}

	if (gamer_.login_.length() > 12)
	{
		MessageBox("Ох, и имечко!\nВыберите покороче, пожалуйста.\nНе более 12 символов", "Неверно", MB_ICONERROR);
		return;
	}

	if (gamer_.port_ < 10000 || gamer_.port_ > 65535)
	{
		MessageBox("Выберите другой порт", "Неверно", MB_ICONERROR);
		return;
	}

	if (gamer_.ip_address_.empty())
	{
		MessageBox("Не задан IP адрес для соединения", "Неверно", MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}
