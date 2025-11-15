#pragma once
#include "afxdialogex.h"
#include "Game.h"


// CreateGameDlg dialog

class CreateGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateGameDlg)

public:
	CreateGameDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CreateGameDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWGAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int		m_iPort;
	CString m_sLogin;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
