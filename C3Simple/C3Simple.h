// C3Simple.h : C3SIMPLE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_C3SIMPLE_H__476A0EC9_18E7_4BD5_9AB5_F0A87E921083__INCLUDED_)
#define AFX_C3SIMPLE_H__476A0EC9_18E7_4BD5_9AB5_F0A87E921083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CC3SimpleApp
// このクラスの動作の定義に関しては C3Simple.cpp ファイルを参照してください。
//

class CC3SimpleApp : public CWinApp
{
public:
	CC3SimpleApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CC3SimpleApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC3SimpleApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_C3SIMPLE_H__476A0EC9_18E7_4BD5_9AB5_F0A87E921083__INCLUDED_)
