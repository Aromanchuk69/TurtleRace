#pragma once
#include "afxdialogex.h"
#include "Game.h"

// JoinGameDlg dialog

class JoinGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(JoinGameDlg)

public:
	JoinGameDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~JoinGameDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOINGAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl	m_cIPAddress;
	int				m_iPort;
	CString			m_sLogin;
	std::string		m_sIpAddress;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
