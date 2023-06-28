// MyFileDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
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
                             DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, bool isDispCheck)
    : CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_nCheck = FALSE;
	m_strCheckString = "";
	m_isAutoMacro = false;
	m_isDispCheck = isDispCheck;
	if (isDispCheck) {
#if (NTDDI_VERSION < NTDDI_VISTA)
		SetTemplate(0, IDD_CHECKBOX);
#else
#if false
// This is not works
		IFileDialogCustomize* pfdc = GetIFileDialogCustomize();
		pfdc->AddCheckButton(IDD_CHECKBOX, _T("マクロプラグインを自動切換えする"), false);
#endif
#endif
	}
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
#if (NTDDI_VERSION < NTDDI_VISTA)
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MACRO);
		if(pButton) {
			pButton->SetCheck(m_nCheck);
			if(m_nCheck)	m_isAutoMacro = true;
			else			m_isAutoMacro = false;
			pButton->SetWindowText(m_strCheckString);
		}
#endif
	}
	return TRUE;
}

void CMyFileDialog::OnDestroy() 
{
	if(m_isDispCheck) {
#if (NTDDI_VERSION < NTDDI_VISTA)
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MACRO);
		if(pButton) {
			if(pButton->GetCheck()) m_isAutoMacro = true;
			else m_isAutoMacro = false;
		}
#endif
	}
	CFileDialog::OnDestroy();
	
}
