
/*----------------------------------------------------------
Quui用データ読み書きDLL
									2002/09/25 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"
#include "RW_Plugin.h"

#pragma data_seg(".shared")
//アプリケーション名と拡張しを指定
const TCHAR *szName = _T("Quui");
const TCHAR *szExtn = _T("qui");

//データ保管用リスト
list<STRING_DATA> *pData;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

//---------------------------------------------------
//関数名	void initDLL()
//機能		初期化処理
//---------------------------------------------------
extern "C" __declspec (dllexport) void initDLL()
{
	pData = NULL;
}

//---------------------------------------------------
//関数名	void endDLL()
//機能		解放処理
//---------------------------------------------------
extern "C" __declspec (dllexport) void endDLL()
{
	if(pData) {
		pData->clear();
		delete pData;
	}
}

//---------------------------------------------------
//関数名	bool isUnicode()
//機能		unicode判定
//---------------------------------------------------
extern "C" __declspec (dllexport) bool isUnicode()
{
#ifdef _UNICODE
	return true;
#else
	return false;
#endif
}

//---------------------------------------------------
//関数名	getFormatName(TCHAR *szBuff,int nSize)
//機能		形式名称を取得
//---------------------------------------------------
extern "C" __declspec (dllexport) void getFormatName(TCHAR *szBuff,int nSize)
{
	if(sizeof(szName) > nSize) return;
	_tcscpy(szBuff,szName);
}

//---------------------------------------------------
//関数名	getExtension(TCHAR *szBuff,int nSize)
//機能		拡張子を取得
//---------------------------------------------------
extern "C" __declspec (dllexport) void getExtension(TCHAR *szBuff,int nSize)
{
	if(sizeof(szExtn) > nSize) return;
	_tcscpy(szBuff,szExtn);
}


//---------------------------------------------------
//関数名	readDataFile(CString strFileName,bool isImport)
//機能		リスト構造体にファイルを読み込み
//---------------------------------------------------
extern "C" __declspec (dllexport) list<STRING_DATA>* readDataFile(CString strFileName,bool isImport)
{
	STRING_DATA Data;

	if(!pData) pData = new list<STRING_DATA>;
	pData->clear();
	int nID ;
	time_t lTime;

	CFile MyFile;
	//ファイルを開く
	if(MyFile.Open(strFileName,CFile::modeRead,NULL))	{
		CFileStatus fileStatus;
		MyFile.GetStatus(fileStatus);
		lTime = fileStatus.m_ctime.GetTime();
		nID = (int)lTime;
		MyFile.Close();
	}
	else return NULL;

	Data.m_cIcon = 0;		//アイコン種別
	Data.m_timeCreate = lTime;	//作成日時
	Data.m_timeEdit = lTime;		//変更日時

	//--------------------------------ここから変更------------------------------------
	vector<CString> vecTitle;
	vector<CString> vecText;

	//履歴フォルダ作成
	int nParent2;
	Data.m_nParentID = ROOT;
	Data.m_cKIND = KIND_RIREKI;//種別
	Data.m_nMyID = nID++;

	Data.m_strTitle = _T("履歴フォルダ");
	Data.m_strMacro = _T("count=100\r\n");
	pData->insert(pData->end(),Data);

	//データ読み込み
	CFile QuuiFile;
	char *strTmp,strTmpSt[64000],cBuff[3];
	ZeroMemory(&strTmpSt,sizeof(strTmpSt));
	CString strBuff;
	strBuff = _T("");
	int nSeq = 1;

	//ファイルを開く
	if(!QuuiFile.Open(strFileName,CFile::modeRead,NULL)) {
		return NULL;
	}
	strTmp = strTmpSt;

	//タイトルは0x0d0a区切りで、そのあとにNULL区切りで本文
	if(QuuiFile.Read(&cBuff,3) < 3) {
		return NULL;
	}
	while(QuuiFile.Read(&cBuff,1) >= 1) {
		if(nSeq == 1) {//タイトル
			if(*cBuff == 0x0d) {//改行
				*strTmp = (char)NULL;
				strBuff = strTmpSt;
				vecTitle.insert(vecTitle.end(),strBuff);
				strTmp = strTmpSt;//ポインタ復帰
				QuuiFile.Read(&cBuff,1);
			}
			else if(*cBuff == NULL) {//タイトル終わり
				nSeq = 2;
				strTmp = strTmpSt;//ポインタ復帰
			}
			else {
				*strTmp = *cBuff;
				strTmp++;
			}
		}
		else if(nSeq == 2) {//本文
			if(*cBuff == NULL) {//本文終わり
				*strTmp = NULL;
				strBuff = strTmpSt;
				vecText.insert(vecText.end(),strBuff);
				strTmp = strTmpSt;//ポインタ復帰
			}
			else {
				*strTmp = *cBuff;
				strTmp++;
			}
		}
	}
	QuuiFile.Close();
	//データを追加

	int i;
	for(i = 0; i < vecTitle.size() && i < vecText.size(); i++) {
		if((i % 20) == 0) {
			Data.m_strTitle.Format(_T("%d_Quuiデータ"),i/20+1);
			Data.m_strData.Format(_T("%d_Quuiデータ"),i/20+1);
			Data.m_cKIND = KIND_FOLDER;//種別
			Data.m_nMyID = nID++;
			nParent2 = Data.m_nMyID;
			Data.m_nParentID = ROOT;//nParent;
			pData->insert(pData->end(),Data);
		}
		Data.m_cKIND = KIND_LOCK;
		Data.m_nMyID = nID++;
		Data.m_nParentID = nParent2;
		Data.m_strTitle = vecTitle[i];
		Data.m_strData = vecText[i];
		pData->insert(pData->end(),Data);
	}
	
	return pData;
}


//---------------------------------------------------
//関数名	writeDataFile(CString strFileName,CList<STRING_DATA,STRING_DATA> *pList)
//機能		リスト構造体をファイルに書き出し
//---------------------------------------------------
extern "C" __declspec (dllexport) bool writeDataFile(CString strFileName,list<STRING_DATA> *pList)
{
	list<STRING_DATA>::iterator it;

	STRING_DATA Data;
	CString strBuff;

	CFile MyFile(strFileName,CFile::modeWrite | CFile::modeCreate);

	char strTmp[256];
	strTmp[0] = (char)0xe7;
	strTmp[1] = (char)0x23;
	strTmp[2] = (char)0x00;
	MyFile.Write(&strTmp,3);
#ifdef _UNICODE
	char *szWriteBuff;
	int nLength;
#else
	LPTSTR lpStr;
#endif

	for(it = pList->begin(); it != pList->end(); it++) {
		Data = *it;
		if(!(Data.m_cKIND & KIND_FOLDER_ALL)) {
			strBuff = Data.m_strTitle + "\r\n";
#ifdef _UNICODE
			nLength = ::WideCharToMultiByte(CP_ACP, 0, strBuff,-1,NULL,0,NULL,NULL);
			szWriteBuff = new char[nLength];
			::WideCharToMultiByte(CP_ACP,0,strBuff,-1,szWriteBuff,nLength,"",NULL);
			MyFile.Write(szWriteBuff,nLength-1);
			delete szWriteBuff;
#else
			lpStr = (LPTSTR)LPCTSTR(strBuff);
			MyFile.Write(lpStr,strBuff.GetLength());
#endif
		}
	}
	strTmp[0] = (char)0x00;
	strTmp[1] = (char)0x00;
	MyFile.Write(&strTmp,1);
	for(it = pList->begin(); it != pList->end(); it++) {
		Data = *it;
		if(!(Data.m_cKIND & KIND_FOLDER_ALL)) {
#ifdef _UNICODE
			nLength = ::WideCharToMultiByte(CP_ACP, 0, Data.m_strData,-1,NULL,0,NULL,NULL);
			szWriteBuff = new char[nLength];
			::WideCharToMultiByte(CP_ACP,0,Data.m_strData,-1,szWriteBuff,nLength,"",NULL);
			MyFile.Write(szWriteBuff,nLength);
			delete szWriteBuff;
#else
			lpStr = (LPTSTR)LPCTSTR(Data.m_strData);
			MyFile.Write(lpStr,Data.m_strData.GetLength());
			MyFile.Write(&strTmp,1);
#endif
		}
	}
	strTmp[0] = (char)0x00;
	strTmp[1] = (char)0xe7;
	strTmp[2] = (char)0x23;
	strTmp[3] = (char)0x01;
	MyFile.Write(&strTmp,4);
	
	MyFile.Close();

	return true;
}


//---------------------------------------------------
//関数名	convertMacro(TCHAR *szSource,TCHAR *szRet,int nSize,TCHAR *szSelect,TCHAR *szClip)
//機能		マクロ処理プラグイン
//---------------------------------------------------
extern "C" __declspec (dllexport) bool convertMacro(TCHAR *szSource,TCHAR *szRet,int nSize,TCHAR *szSelect,TCHAR *szClip)
{
	bool isRet = false;
	return isRet;
}
