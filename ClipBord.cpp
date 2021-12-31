/*----------------------------------------------------------
	ClipBordクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "ClipBord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//---------------------------------------------------
//関数名	ClipBord
//機能		コンストラクタ
//---------------------------------------------------
CClipBord::CClipBord()
{
	m_hParentWnd = NULL;
}

//---------------------------------------------------
//関数名	~ClipBord
//機能		デストラクタ
//---------------------------------------------------
CClipBord::~CClipBord()
{

}

//---------------------------------------------------
//関数名	setClipboardText(const CString& sData)
//機能		クリップボードにテキストを設定
//---------------------------------------------------
BOOL CClipBord::setClipboardText(const TCHAR *szString)
{
	HGLOBAL hMem;
	int nStrLength=_tcsclen(szString)+1;
	LPTSTR  pPtr;
	bool isRet = false;
	UINT uFormat;
	#ifdef _UNICODE
		uFormat = CF_UNICODETEXT;
	#else
		uFormat = CF_TEXT;
	#endif

	if(!::OpenClipboard(m_hParentWnd)) return isRet;

	hMem = ::GlobalAlloc(GHND,nStrLength * sizeof(WCHAR));
	pPtr=(TCHAR *)::GlobalLock(hMem);
	_tcscpy_s(pPtr, nStrLength, szString);
	::GlobalUnlock(hMem);

	::EmptyClipboard();
	HANDLE hRet = ::SetClipboardData(uFormat,hMem);
	::CloseClipboard();

	OutputDebugString(_T("setClipboardText\n"));

	isRet = true;
	return isRet;
}

//---------------------------------------------------
//関数名	getClipboardText(CString& sData)
//機能		クリップボードのテキストを取得
//---------------------------------------------------
BOOL CClipBord::getClipboardText(CString& sData)
{
	TCHAR *pPtr;
	bool isRet = false;

	UINT uFormat;
	#ifdef _UNICODE
		uFormat = CF_UNICODETEXT;
	#else
		uFormat = CF_TEXT;
	#endif

	if(::IsClipboardFormatAvailable(uFormat)==FALSE) return isRet;
	if(!::OpenClipboard(m_hParentWnd)) return isRet;

	HGLOBAL hGet=::GetClipboardData(uFormat);
	if(!hGet) {
		::CloseClipboard();
		return isRet;
	}
	pPtr=(TCHAR *)::GlobalLock(hGet);
	sData=pPtr;
	::GlobalUnlock(hGet);

	::CloseClipboard();

	isRet = true;
	return isRet;
}

