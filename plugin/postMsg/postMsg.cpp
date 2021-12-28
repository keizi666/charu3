/*----------------------------------------------------------
	テキスト色々変換
									2001/08/04 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"

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

struct FOCUS_INFO {
	HWND m_hActiveWnd;
	HWND m_hFocusWnd;
};

//---------------------------------------------------
//関数名	CharuPlugIn
//機能		プラグイン関数
//---------------------------------------------------
extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{
	bool isRet = false;
	*strResult = NULL;
	DWORD wMsg,dwWParam,dwLParam;
	TCHAR cPost,cTarget;
	
	//---------------ここからコードを書くといいです---------------------
	if(_stscanf(strSource,_T("%c,%c,%x,%x,%x"),&cPost,&cTarget,&wMsg,&dwWParam,&dwLParam) == 5) {
		FOCUS_INFO *focusInfo;
		focusInfo = (FOCUS_INFO*)pVoid;
		HWND hWnd;
		if(cTarget == _T('c'))	hWnd = focusInfo->m_hFocusWnd;
			else				hWnd = focusInfo->m_hActiveWnd;
		if(cPost == _T('s')) {
			::SendMessage(hWnd,wMsg,dwWParam,dwLParam);
		}
		else if(cPost == _T('p')) {
			::PostMessage(hWnd,wMsg,dwWParam,dwLParam);
		}
	}
	//---------------ここまでコードを書くといいです---------------------
	isRet = true;
	return isRet;
}

