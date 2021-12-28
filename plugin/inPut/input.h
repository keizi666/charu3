// input.h : INPUT アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_INPUT_H__24765F61_3A2C_11D6_B778_0004AC583E0D__INCLUDED_)
#define AFX_INPUT_H__24765F61_3A2C_11D6_B778_0004AC583E0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CInputApp
// このクラスの動作の定義に関しては input.cpp ファイルを参照してください。
//
//---------------------------------------------------
// データ構造体
//---------------------------------------------------
struct STRING_DATA
{
	char	m_cKind;		//データ種別
	char	m_cIcon;		//アイコン種別
	int		m_nMyID;		//データのID
	int		m_nParentID;	//親データのID
	time_t	m_timeCreate;	//作成日時
	time_t	m_timeEdit;		//変更日時
	CString  m_strTitle;	//設定データタイトル
	CString  m_strData;		//設定データ文字列
	CString  m_strMacro;	//拡張用文字列データ
};

class CInputApp : public CWinApp
{
public:
	CInputApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInputApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CInputApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INPUT_H__24765F61_3A2C_11D6_B778_0004AC583E0D__INCLUDED_)
