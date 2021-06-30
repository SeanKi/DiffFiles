
// DiffFilesDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DiffFiles.h"
#include "DiffFilesDlg.h"
#include "afxdialogex.h"

#include <filesystem>
#ifndef INLINE_CONSOLE_H
#include <iostream>
#endif

//  determine filesystem namespace dependends on compiler
#ifdef _FSPFX
namespace  fs = std::experimental::filesystem;      //  toolset v140, v141
#else
#if !defined(_HAS_CXX17) || _HAS_CXX17 == 0
#error "Error: in order to use <filesystem> with v142 toolset set compiler flag /std:c++17"
#endif
namespace  fs = std::filesystem;                    //  toolset v142
#endif // _FSPFX

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <sstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>
using std::ios;
using std::ifstream;

#include <exception>
using std::exception;

#include <cstring>
#include <cstdlib>
using std::exit;
using std::memcmp;

bool equalFiles(ifstream& in1, ifstream& in2)
{
	ifstream::pos_type size1, size2;

	size1 = in1.seekg(0, ifstream::end).tellg();
	in1.seekg(0, ifstream::beg);

	size2 = in2.seekg(0, ifstream::end).tellg();
	in2.seekg(0, ifstream::beg);

	if (size1 != size2)
		return false;

	static const size_t BLOCKSIZE = 4096 * 2;
	size_t remaining = size1;

	while (remaining)
	{
		char buffer1[BLOCKSIZE], buffer2[BLOCKSIZE];
		size_t size = min(BLOCKSIZE, remaining);

		in1.read(buffer1, size);
		in2.read(buffer2, size);

		if (0 != memcmp(buffer1, buffer2, size))
			return false;

		remaining -= size;
	}

	return true;
}

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


// CDiffFilesDlg dialog



CDiffFilesDlg::CDiffFilesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIFFFILES_DIALOG, pParent)
	, m_sFileExp(_T(""))
	, m_nNum1(0)
	, m_nNum2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDiffFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_FILEEXP, m_sFileExp);
	//  DDX_Text(pDX, IDC_ED_NUM1, m_sNum1);
	//  DDX_Text(pDX, IDC_ED_NUM2, m_sNum2);
	DDX_Text(pDX, IDC_ED_NUM1, m_nNum1);
	DDX_Text(pDX, IDC_ED_NUM2, m_nNum2);
}

BEGIN_MESSAGE_MAP(CDiffFilesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_GO, &CDiffFilesDlg::OnBnClickedBtnGo)
END_MESSAGE_MAP()


// CDiffFilesDlg message handlers

BOOL CDiffFilesDlg::OnInitDialog()
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
	m_sFileExp = "Scan%d.raw";
	m_nNum1 = 1;
	m_nNum2 = 100;

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDiffFilesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDiffFilesDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDiffFilesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDiffFilesDlg::CompareFiles(const CString& fileExp, int nNum1, int nNum2)
{
	std::ofstream logFile("comp_result.txt", ios::app);
	bool bContinue;
	int nCount = nNum1;
	do {
		CString fname, fname2;
		if (nCount > nNum2) {
			TRACE(_T("End of File Numbers\n"));
			logFile << "End of File numbers" << endl;
			break;
		}
		fname.Format(m_sFileExp, nCount);
		nCount++;
		fname2.Format(m_sFileExp, nCount);
        try {
            ifstream in1(fname, ios::binary);
            ifstream in2(fname2, ios::binary);

            bContinue = true;
            std::stringstream result;
            if (!in1.is_open()) {
                result << CT2CA(fname) << " is not exist" << endl;
                bContinue = false;
            }
            else if (!in2.is_open()) {
                result << CT2CA(fname2) << " is not exist" << endl;
                bContinue = false;
            }
            else {
                if (equalFiles(in1, in2)) {
                    result << "Files are equal - "
                        << CT2CA(fname) << " = " << CT2CA(fname2) << endl;
                }
                else
                {
                    result << "Files are not equal - "
                        << CT2CA(fname) << " != " << CT2CA(fname2) << endl;

                }
            }
			
			TRACE(CA2CT(result.str().c_str()));
			logFile << result.str();
            nCount++;
        }
        catch (const exception& ex) {
            cerr << ex.what() << endl;
        }
	} while (bContinue);
	logFile.close();
}

void CDiffFilesDlg::OnBnClickedBtnGo()
{
	UpdateData(TRUE);
	
	CompareFiles(m_sFileExp, m_nNum1, m_nNum2);
	// TODO: Add your control notification handler code here
}
