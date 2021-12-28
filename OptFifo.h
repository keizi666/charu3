/*----------------------------------------------------------
	履歴FIFO設定クラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTFIFO_H__63AED525_A07B_429F_B89E_2A8964C05622__INCLUDED_)
#define AFX_OPTFIFO_H__63AED525_A07B_429F_B89E_2A8964C05622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptFifo.h : ヘッダー ファイル
//

//---------------------------------------------------
// COptFifo ダイアログ
//---------------------------------------------------

class COptFifo : public CDialog
{
// コンストラクション
public:
	COptFifo(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(COptFifo)
	enum { IDD = IDD_OPT_FIFO };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COptFifo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COptFifo)
	afx_msg void OnFile();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTFIFO_H__63AED525_A07B_429F_B89E_2A8964C05622__INCLUDED_)
