/*----------------------------------------------------------
	一般設定クラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTETC_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_)
#define AFX_OPTETC_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptEtc.h : ヘッダー ファイル

//---------------------------------------------------
// COptEtc ダイアログ
//---------------------------------------------------
class COptEtc : public CDialog
{
// コンストラクション
public:
	COptEtc(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(COptEtc)
	enum { IDD = IDD_OPT_ETC };
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COptEtc)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COptEtc)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTETC_H__0D4DFA1E_8391_4DA0_A8C2_1C86D2C0C460__INCLUDED_)
