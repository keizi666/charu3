/*----------------------------------------------------------
	キー設定クラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTKEYSET_H__A388CCDA_003B_4044_99AF_3B563BF12272__INCLUDED_)
#define AFX_OPTKEYSET_H__A388CCDA_003B_4044_99AF_3B563BF12272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptKeySet.h : ヘッダー ファイル
//
#include "MyHotkeyCtrl.h"

//---------------------------------------------------
// COptKeySet ダイアログ
//---------------------------------------------------
class COptKeySet : public CDialog
{
// コンストラクション
public:
	COptKeySet(CWnd* pParent = NULL);   // 標準のコンストラクタ
	CHANGE_KEY windouToKeyOption();
// ダイアログ データ
	//{{AFX_DATA(COptKeySet)
	enum { IDD = IDD_OPT_KEYSET };
	CEdit	m_ctrlCopyWaitEdit;
	CEdit	m_ctrlPasteWaitEdit;
	CMyHotkeyCtrl	m_ctrlPasteKey;
	CListCtrl	m_ctrlIniList;
	CMyHotkeyCtrl	m_ctrlCopyKey;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COptKeySet)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void SetListData(CHANGE_KEY sKeyData,list<CHANGE_KEY>::iterator it,bool isSet,int nLine);

	list<CHANGE_KEY> m_MyKeyList;//キー設定データリスト
	list<CHANGE_KEY>::iterator m_itSelect;
	int m_nSelItem;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COptKeySet)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptKeyAdd();
	afx_msg void OnOptKeyDalete();
	afx_msg void OnOptKeyOk();
	afx_msg void OnItemchangedOptKeyIniList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnOptKeyEdit();
	afx_msg void OnDblclkOptKeyIniList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTKEYSET_H__A388CCDA_003B_4044_99AF_3B563BF12272__INCLUDED_)
