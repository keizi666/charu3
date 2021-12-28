/*----------------------------------------------------------
	行選択
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

void getLine(TCHAR *strResult,TCHAR *strSrc);

extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{
	bool isRet = false;
	TCHAR *strTmp,*strTmp2;
	int nLine;
	strTmp = new TCHAR[nSize];
	strTmp2 = strTmp;
	_stscanf(strSource,_T("%d,%s"),&nLine,strTmp);
	nLine--;
	for(;*strSource != NULL; strSource++) {
		//行を切り出し
		if(*strSource == ','){
			strSource++;
			break;
		}
	}
	_tcscpy(strTmp,strSource);	
	*strResult = NULL;
	for(;*strTmp != NULL; strTmp++) {
		//行を切り出し
		if(nLine <= 0){
			getLine(strResult,strTmp);
			break;
		}
		if(*strTmp == 0x0a) nLine--;
	}
	delete [] strTmp2;	
	isRet = true;
	return isRet;
}

void getLine(TCHAR *strResult,TCHAR *strSrc)
{
	for(;*strSrc != NULL; strSrc++) {
		//行を切り出し
		if(*strSrc == 0x0a) break;
		if(*strSrc != 0x0d){
			*strResult = *strSrc;//コピー
			strResult++;
		}
	}
	*strResult = NULL;
}
