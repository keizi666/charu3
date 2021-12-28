// MyFileDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "charu3.h"
#include "MyFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd,bool isDispCheck) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_nCheck = FALSE;
	m_strCheckString = "";
	m_isAutoMacro = false;
	m_isDispCheck = isDispCheck;
	if(isDispCheck)
		SetTemplate(0,IDD_CHECKBOX);
}


BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CMyFileDialog)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();


	if(m_isDispCheck) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MACRO);
		if(pButton) {
			pButton->SetCheck(m_nCheck);
			if(m_nCheck)	m_isAutoMacro = true;
			else			m_isAutoMacro = false;
			pButton->SetWindowText(m_strCheckString);
		}
	}
	return TRUE;
}

void CMyFileDialog::OnDestroy() 
{
	if(m_isDispCheck) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MACRO);
		if(pButton) {
			if(pButton->GetCheck()) m_isAutoMacro = true;
			else m_isAutoMacro = false;
		}
	}
	CFileDialog::OnDestroy();
	
}
