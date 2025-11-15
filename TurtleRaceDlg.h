
// TurtleRaceDlg.h : header file
//

#pragma once
#include "Game.h"

// CTurtleRaceDlg dialog
class CTurtleRaceDlg : public CDialogEx
{
// Construction
public:
	CTurtleRaceDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TURTLERACE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnOK() {};
	afx_msg void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CGame			m_game_;
public:
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuNewgame();
	afx_msg void OnMenuJoingame();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
