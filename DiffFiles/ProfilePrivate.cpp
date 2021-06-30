// ProfilePrivate.cpp: implementation of the CProfilePrivate class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ProfilePrivate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 2001.7.1
/*	GetStringToEditBox_Range, WriteStringFromEditBox_Range �Լ� �߰� - �������� Edit������ ���� �����ϰ� ã��
*/
// 2001.6.29
/*	GetStringList, WriteStringList�Լ� �߰�
	GetStringListToComboBox, ~ToListBox �� WriteStringList~ �Լ� ��Ʈ �߰�
	AryFromComboBox, AryToComboBox �� ~ListBox�Լ� ��Ʈ �߰� - CStringAry <-> ��Ʈ���� ���ڿ�����Ʈ ��ȯ
*/
// 2001.6.28 �迭�� �а� ���� ���� �Լ� �߰�
/*	static int AryFromComboBox(CStringArray &ary, CComboBox &cbo);
	static int AryFromListBox(CStringArray &ary, CListBox &lb);
	static int AryToListBox(CStringArray &ary, CListBox &lb);
	static int AryToComboBox(CStringArray &ary, CComboBox &cbo);
	BOOL WriteStringListFromListBox(LPCTSTR szSection, CListBox &lb);
	BOOL GetStringListToListBox(LPCTSTR szSection, CListBox &lb);
	BOOL GetStringListToComboBox(CComboBox &cbo);
	BOOL WriteStringListFromComboBox(CComboBox &cbo);
	BOOL GetStringListToComboBox(LPCTSTR szSection, CComboBox &cbo);
	BOOL WriteStringListFromComboBox(LPCTSTR szSection, CComboBox &cbo);
	BOOL GetStringList(LPCTSTR szSection, CStringArray &ary);
	BOOL WriteStringList(LPCTSTR szSection, CStringArray &ary);
*/
// 2000.11.7
// GetInt�� �߰�


#define ASSERTSTRING(sz) ASSERT(sz != "")
//#define ASSERT1 ASSERTSTRING(m_sSection);ASSERTSTRING(m_sFilename)
#define ASSERT1 ASSERTSTRING(m_sFilename)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfilePrivate::CProfilePrivate()
{

}

CProfilePrivate::~CProfilePrivate()
{

}


//���ϰ��� �޴� ���� ũ��
BOOL CProfilePrivate::GetString(LPCTSTR szEntry, CString &sVal, LPCTSTR szDefault)
{
	ASSERT1;
	//sVal = AfxGetApp()->GetProfileString(m_sSection, szEntry, szDefault);
	TCHAR szReturnedString[1024];
	DWORD nSize=1024;
	int nRSize =
	GetPrivateProfileString(
		m_sSection,        // points to section name
		szEntry,        // points to key name
		szDefault,        // points to default string
		szReturnedString,  // points to destination buffer
		nSize,              // size of destination buffer
		m_sFilename        // points to initialization filename
	);
	sVal = szReturnedString;
	return nRSize;
}

BOOL CProfilePrivate::WriteInt(LPCTSTR szSection, LPCTSTR szEntry, UINT nVal)
{
	ASSERT1;
	CString sVal;
	sVal.Format(_T("%d"), nVal);
	return	WriteString(szSection, szEntry, sVal);
}

void CProfilePrivate::SetSection(LPCTSTR sz)
{
	m_sSection = sz;
}

void CProfilePrivate::SetFilename(LPCTSTR sz)
{
	m_sFilename = sz;
}

UINT CProfilePrivate::GetInt(LPCTSTR szEntry, UINT nDefault)
{
	return
		GetPrivateProfileInt(
			m_sSection,        // points to section name
			szEntry,        // points to key name
			nDefault,        // default Int(KeyName�� �߰ߵ��� ���� ��)
			   // size of destination buffer
			m_sFilename        // points to initialization filename
		);
}
//Ư�� ���ǿ� ����Ʈ�� ��´�. ����Ʈ�� ����""�� �ν�
BOOL CProfilePrivate::WriteStringList(LPCTSTR szSection, CStringArray &ary)
{
	ASSERT1;
	CString sEntry;
	int i;
	for (i=0; i<ary.GetSize();i++) {
		sEntry.Format(_T("%d"), i);
		if (!WritePrivateProfileString(
			szSection,        // points to section name
			sEntry,        // points to key name
			ary[i],
			m_sFilename  // pointer to initialization filename
			))
			return FALSE;
	}
	sEntry.Format(_T("%d"), i);
	return WritePrivateProfileString( //������ �׸� ""�� �߰������ν� ���� ǥ��
			szSection,        // points to section name
			sEntry,        // points to key name
			_T(""),
			m_sFilename);
}

BOOL CProfilePrivate::GetStringList(LPCTSTR szSection, CStringArray &ary)
{
	ASSERT1;
	CString sEntry;
	TCHAR szReturnedString[1024];
	DWORD nSize=1024;
	int i;
	for(i=0;TRUE;i++) {
		sEntry.Format(_T("%d"), i);
		int nRSize =GetPrivateProfileString(
			szSection,        // points to section name
			sEntry,        // points to key name
			_T(""),        // points to default string
			szReturnedString,  // points to destination buffer
			nSize,              // size of destination buffer
			m_sFilename        // points to initialization filename
		);
		if (nRSize <= 0)
			break;
		ary.Add(szReturnedString);
	}
	return i;
}

BOOL CProfilePrivate::WriteStringListFromComboBox(LPCTSTR szSection, CComboBox &cbo)
{
	CStringArray ary;
	CString rStr;
	for(int i=0;i<cbo.GetCount();i++) {
		cbo.GetLBText(i, rStr);
		ary.Add(rStr);
	}
	WriteStringList(szSection, ary);
	return ary.GetSize();
}

BOOL CProfilePrivate::GetStringListToComboBox(LPCTSTR szSection, CComboBox &cbo)
{
	CStringArray ary;
	GetStringList(szSection, ary);
	AryToComboBox(ary, cbo);
	return ary.GetSize();
}

BOOL CProfilePrivate::WriteStringListFromComboBox(CComboBox &cbo)
{
	CStringArray ary;
	CString rStr;
	for(int i=0;i<cbo.GetCount();i++) {
		cbo.GetLBText(i, rStr);
		ary.Add(rStr);
	}
	CString stmp;
	stmp.Format(_T("COMBOBOX_%d"),cbo.GetDlgCtrlID());
	WriteStringList(stmp, ary);
	return ary.GetSize();
}

BOOL CProfilePrivate::GetStringListToComboBox(CComboBox &cbo)
{
	CStringArray ary;
	CString stmp;
	stmp.Format(_T("COMBOBOX_%d"),cbo.GetDlgCtrlID());
	GetStringList(stmp, ary);
	for(int i=0;i<ary.GetSize();i++) {
		cbo.AddString(ary[i]);
	}
	return ary.GetSize();
}
BOOL CProfilePrivate::WriteStringListFromListBox(LPCTSTR szSection, CListBox &lb)
{
	CStringArray ary;
	CString rStr;
	for(int i=0;i<lb.GetCount();i++) {
		lb.GetText(i, rStr);
		ary.Add(rStr);
	}
	WriteStringList(szSection, ary);
	return ary.GetSize();
}

BOOL CProfilePrivate::GetStringListToListBox(LPCTSTR szSection, CListBox &lb)
{
	CStringArray ary;
	GetStringList(szSection, ary);
	for(int i=0;i<ary.GetSize();i++) {
		lb.AddString(ary[i]);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryToComboBox(CStringArray &ary, CComboBox &cbo)
{
	cbo.ResetContent();
	for(int i=0;i<ary.GetSize();i++) {
		cbo.AddString(ary[i]);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryToListBox(CStringArray &ary, CListBox &lb)
{
	lb.ResetContent();
	for(int i=0;i<ary.GetSize();i++) {
		lb.AddString(ary[i]);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryFromListBox(CStringArray &ary, CListBox &lb)
{
	CString rStr;
	ary.RemoveAll();
	for(int i=0;i<lb.GetCount();i++) {
		lb.GetText(i, rStr);
		ary.Add(rStr);
	}
	return ary.GetSize();
}

int CProfilePrivate::AryFromComboBox(CStringArray &ary, CComboBox &cbo)
{
	CString rStr;
	ary.RemoveAll();
	for(int i=0;i<cbo.GetCount();i++) {
		cbo.GetLBText(i, rStr);
		ary.Add(rStr);
	}
	return ary.GetSize();
}

// New since 2001.7.1
//
//////////////////////

BOOL CProfilePrivate::GetString(LPCTSTR szSection, LPCTSTR szEntry, CString &sVal, LPCTSTR szDefault)
{
	ASSERT1;
	//sVal = AfxGetApp()->GetProfileString(m_sSection, szEntry, szDefault);
	TCHAR szReturnedString[1024];
	DWORD nSize=1024;
	int nRSize =
	GetPrivateProfileString(
		szSection,        // points to section name
		szEntry,        // points to key name
		szDefault,        // points to default string
		szReturnedString,  // points to destination buffer
		nSize,              // size of destination buffer
		m_sFilename        // points to initialization filename
	);
	sVal = szReturnedString;
	return nRSize;

}

BOOL CProfilePrivate::WriteString(LPCTSTR szSection, LPCTSTR szEntry, CString sVal)
{
	ASSERT1;
	return WritePrivateProfileString(
			szSection,        // points to section name
			szEntry,        // points to key name
			sVal,
			m_sFilename  // pointer to initialization filename
			);

}

BOOL CProfilePrivate::WriteStringFromEditBox_Range(int RESOURCEfrom, int RESOURCEto, CWnd *pwnd)
{
	CString stmp;
	CString sVal;
	const TCHAR szSection[]= _T("EDITBOX_LIST");
	for (int i=RESOURCEfrom; i<=RESOURCEto; i++) {
//		CWnd *lpwnd = pwnd->GetDlgItem(i);
		//if(!lpwnd->IsKindOf( RUNTIME_CLASS( CEdit ) ) ) continue; //EditBox�� �͸� ó���ϵ���
		stmp.Format(_T("ID_%d"), i);
		pwnd->GetDlgItemText(i, sVal);
//		lpwnd->GetWindowText(sVal);
		WriteString(szSection, stmp, sVal);
	}
	return TRUE;
}
BOOL CProfilePrivate::GetStringToEditBox_Range(int RESOURCEfrom, int RESOURCEto, CWnd *pwnd)
{
	CString stmp;
	CString sVal;
	const TCHAR szSection[]= _T("EDITBOX_LIST");
	for (int i=RESOURCEfrom; i<=RESOURCEto; i++) {
//		CWnd *lpwnd = pwnd->GetDlgItem(i);
//		if(!lpwnd->IsKindOf( RUNTIME_CLASS( CEdit ) ) ) continue; //EditBox�� �͸� ó���ϵ���
		stmp.Format(_T("ID_%d"), i);
		GetString(szSection, stmp, sVal, _T(""));
	//	lpwnd->SetWindowText(sVal);
		pwnd->SetDlgItemText(i, sVal);
	}
	return TRUE;
}
