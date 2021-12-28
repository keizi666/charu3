#if !defined(AFX_MYFILEDIALOG_H__E770EA4C_C3F7_418C_B0C9_F20ED1D68BCE__INCLUDED_)
#define AFX_MYFILEDIALOG_H__E770EA4C_C3F7_418C_B0C9_F20ED1D68BCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog ダイアログ

class CMyFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDialog)

public:
	bool m_isDispCheck;
	bool m_isAutoMacro;
	CMyFileDialog(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		bool isDispCheck = false);

	int DoModal(int nCheck,CString strCheckString) {
		m_strCheckString = strCheckString;
		m_nCheck = nCheck;

		return CFileDialog::DoModal();
	}
	OPENFILENAME getFileName(){return m_ofn;}
protected:
	//{{AFX_MSG(CMyFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nCheck;
	CString m_strCheckString;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYFILEDIALOG_H__E770EA4C_C3F7_418C_B0C9_F20ED1D68BCE__INCLUDED_)
