/*----------------------------------------------------------
	置換
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

extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{

	bool isRet = false;
	*strResult = NULL;
	//---------------ここからコードを書くといいです---------------------
	CString strTmp;
	strTmp = strSource;
	strTmp.Replace(_T("\r"),_T("<br>\r"));
	if(strTmp.Find(_T("\r")) < 0) {
		strTmp.Replace(_T("\n"),_T("<br>\n"));
	}
	_tcscpy(strResult,strTmp.GetBuffer(strTmp.GetLength()));	
	//---------------ここまでコードを書くといいです---------------------
	isRet = true;
	return isRet;
}


