// HostGet.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"

bool getHostName(TCHAR *pHostName,int nDstSize, TCHAR *pIpAddr)
{	
	struct hostent	*host;
	unsigned long	addr;
	bool isRet = true;
	char *szMBCS;

	
	#ifdef _UNICODE
	int nSize =_tcsclen(pIpAddr) + 1;
	szMBCS = new char[nSize];
	if(!szMBCS) return false;
	WideCharToMultiByte(CP_ACP,0,pIpAddr,nSize,szMBCS,nSize,"",NULL);
	addr = inet_addr(szMBCS);
	#else
	addr = inet_addr(pIpAddr);
	#endif

	host = gethostbyaddr((const char *)&addr, 4, 2);

	if (host == NULL) {
		_tcscpy(pHostName,_T("not found!"));
		isRet = false;
	}
	else {
		#ifdef _UNICODE
		MultiByteToWideChar(CP_ACP,0,host->h_name,strlen(host->h_name),pHostName,nDstSize);
		pHostName[strlen(host->h_name)] = NULL;
		#else
		_tcscpy(pHostName,host->h_name);
		#endif

	}

	#ifdef _UNICODE
	if(szMBCS) delete szMBCS;
	#endif

	return isRet;
}

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
	WSADATA		WsaData;		//アドレス情報構造体

	ZeroMemory(&WsaData, sizeof(WsaData));

	int			nRetValue;
	WORD		wRequired;

	wRequired = MAKEWORD(2,0);
	nRetValue = WSAStartup(wRequired,&WsaData);

	//バージョン初期化
	if( WsaData.wVersion != wRequired || nRetValue != 0) {
		WSACleanup();
		return isRet;
	}	
	
	getHostName(strResult,nSize,strSource);
	
	WSACleanup();
	isRet = true;
	return isRet;
}

