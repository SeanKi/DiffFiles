
// DiffFilesDlg.h : header file
//

#pragma once


// CDiffFilesDlg dialog
class CDiffFilesDlg : public CDialogEx
{
// Construction
public:
	CDiffFilesDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIFFFILES_DIALOG };
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
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_sFileExp;
	void CompareFiles(const CString& fileExp, int nNum1, int nNum2);
	afx_msg	void OnBnClickedBtnGo();
//	CString m_sNum1;
//	CString m_sNum2;
	int m_nNum1;
	int m_nNum2;
};
