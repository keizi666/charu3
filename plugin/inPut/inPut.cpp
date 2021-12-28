// input.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "input.h"
#include "InputDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	メモ!
//
//		この DLL が MFC DLL に対して動的にリンクされる場合、
//		MFC 内で呼び出されるこの DLL からエクスポートされた
//		どの関数も関数の最初に追加される AFX_MANAGE_STATE 
//		マクロを含んでいなければなりません。
//
//		例:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 通常関数の本体はこの位置にあります
//		}
//
//		このマクロが各関数に含まれていること、MFC 内の
//		どの呼び出しより優先することは非常に重要です。
//		これは関数内の最初のステートメントでなければな
//		らないことを意味します、コンストラクタが MFC 
//		DLL 内への呼び出しを行う可能性があるので、オブ
//		ジェクト変数の宣言よりも前でなければなりません。
//
//		詳細については MFC テクニカル ノート 33 および
//		58 を参照してください。
//

/////////////////////////////////////////////////////////////////////////////
// CInputApp

BEGIN_MESSAGE_MAP(CInputApp, CWinApp)
	//{{AFX_MSG_MAP(CInputApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputApp の構築

CInputApp::CInputApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CInputApp オブジェクト

CInputApp theApp;


extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{
	bool isRet = false;
	*strResult = NULL;
	CInputDialog dlgInput(strSource);
	CString strTmp;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//---------------ここからコードを書くといいです---------------------
	dlgInput.m_strInput = _T("");
	dlgInput.DoModal();
	strTmp = dlgInput.m_strInput;

	_tcscpy(strResult,strTmp.GetBuffer(strTmp.GetLength()));
	strResult[strTmp.GetLength()] = NULL;
	//---------------ここまでコードを書くといいです---------------------
	isRet = true;
	return isRet;
}
