// c3language.h : C3LANGUAGE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_C3LANGUAGE_H__C3AA07A6_3A7E_11D7_B866_000629AB8F8F__INCLUDED_)
#define AFX_C3LANGUAGE_H__C3AA07A6_3A7E_11D7_B866_000629AB8F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CC3languageApp
// このクラスの動作の定義に関しては c3language.cpp ファイルを参照してください。
//

class CC3languageApp : public CWinApp
{
public:
	CC3languageApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CC3languageApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC3languageApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_C3LANGUAGE_H__C3AA07A6_3A7E_11D7_B866_000629AB8F8F__INCLUDED_)
