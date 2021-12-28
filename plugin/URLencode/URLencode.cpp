/*----------------------------------------------------------
	SJISをEUCに変換
									2001/08/04 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"

#define SJIS1(c)	((c) >= 0x81 && (c) <= 0x9F || (c) >= 0xE0 && (c) <= 0xF0)
#define SJIS2(c2)	((c2) >= 0x40 && (c2) <= 0xFC && (c2) != 0x7F)
void URLEncode(char *szSource,char *szResult,int nSize);

//---------------------------------------------------
// データ構造体
//---------------------------------------------------
struct STRING_DATA
{
	TCHAR	m_cKind;		//データ種別
	TCHAR	m_cIcon;		//アイコン種別
	int		m_nMyID;		//データのID
	int		m_nParentID;	//親データのID
	time_t	m_timeCreate;	//作成日時
	time_t	m_timeEdit;		//変更日時
	CString  m_strTitle;	//設定データタイトル
	CString  m_strData;		//設定データ文字列
	CString  m_strMacro;	//拡張用文字列データ
};

//--------------------------------------------------
//関数名	CharuPlugIn
//機能		プラグイン関数本体
//--------------------------------------------------
extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{
	bool isRet = false;
	*strResult = NULL;
	//---------------ここからコードを書くといいです---------------------
	char *strTmpSource,*strTmpResult;
	int nLength,nBuffSize;
	nBuffSize = _tcslen(strSource);
	nBuffSize = (nBuffSize + 1) * sizeof(TCHAR); 

	strTmpSource = new char[nBuffSize];
	strTmpResult = new char[nBuffSize * 3];

	//UnicodeをS-JISに変換する
	#ifdef _UNICODE
		ZeroMemory(strTmpSource,nBuffSize);
		nLength = ::WideCharToMultiByte(CP_ACP, 0, strSource,_tcsclen(strSource),NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP,0,strSource,nLength,strTmpSource,nBuffSize,"",NULL);
	#else
		strcpy(strTmpSource,strSource);
	#endif
	
	//URLエンコード処理
	URLEncode(strTmpSource,strTmpResult,nBuffSize * 3);

	//元にもどす	
	#ifdef _UNICODE
		ZeroMemory(strResult,nSize);
		nLength = MultiByteToWideChar(CP_ACP,0,strTmpResult,-1,strResult,nSize);
	#else
		strncpy(strResult,strTmpResult,nBuffSize * 3);
	#endif
	
	delete strTmpSource;
	delete strTmpResult;
		
	//---------------ここまでコードを書くといいです---------------------
	isRet = true;
	return isRet;
}

//--------------------------------------------------
//関数名	URLEncode(char *szSource,char *szResult,int nSize)
//機能		URL変換をする
//--------------------------------------------------
void URLEncode(char *szSource,char *szResult,int nSize)
{
	ZeroMemory(szResult,nSize);
	char strHex[4];
	while(*szSource && nSize > 0) {
		sprintf(strHex,"%%%02x",(int)(*szSource) & 0xff);
		strcat(szResult,strHex);
		nSize-=3;
		szSource++;
	}
}

