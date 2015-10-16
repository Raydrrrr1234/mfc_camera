// OpencvUIDlgDlg.h : header file
//

#if !defined(AFX_OPENCVUIDLGDLG_H__72E775A8_F37D_4609_9E50_EAF58019261A__INCLUDED_)
#define AFX_OPENCVUIDLGDLG_H__72E775A8_F37D_4609_9E50_EAF58019261A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COpencvUIDlgDlg dialog

class COpencvUIDlgDlg : public CDialog
{
// Construction
public:
	COpencvUIDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COpencvUIDlgDlg)
	enum { IDD = IDD_OPENCVUIDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpencvUIDlgDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COpencvUIDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOpencamera();
	afx_msg void OnClosecamera();
	afx_msg void OnGrabframe();
	afx_msg void OnSavemovie();
	afx_msg void OnStopsavemovie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENCVUIDLGDLG_H__72E775A8_F37D_4609_9E50_EAF58019261A__INCLUDED_)
