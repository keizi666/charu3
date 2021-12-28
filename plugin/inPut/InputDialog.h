#if !defined(AFX_INPUTDIALOG_H__24765F68_3A2C_11D6_B778_0004AC583E0D__INCLUDED_)
#define AFX_INPUTDIALOG_H__24765F68_3A2C_11D6_B778_0004AC583E0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CInputDialog ダイアログ

class CInputDialog : public CDialog
{
// コンストラクション
public:
	CInputDialog(TCHAR *szTitle,CWnd* pParent = NULL);   // 標準のコンストラクタ
	TCHAR *m_szTitle;
// ダイアログ データ
	//{{AFX_DATA(CInputDialog)
	enum { IDD = IDD_DIALOG };
	CString	m_strInput;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInputDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CInputDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INPUTDIALOG_H__24765F68_3A2C_11D6_B778_0004AC583E0D__INCLUDED_)
