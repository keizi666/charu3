#if !defined(AFX_INPUTDLG_H__FB55BF38_6D3D_455A_94DB_A1708708618D__INCLUDED_)
#define AFX_INPUTDLG_H__FB55BF38_6D3D_455A_94DB_A1708708618D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : ヘッダー ファイル
//
#include "resource.h"       // メイン シンボル
#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////
// CInputDlg ダイアログ

class CInputDlg : public CDialog
{
// コンストラクション
public:
	CInputDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_DIALOG };
	CString	m_strEdit;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CInputDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INPUTDLG_H__FB55BF38_6D3D_455A_94DB_A1708708618D__INCLUDED_)
