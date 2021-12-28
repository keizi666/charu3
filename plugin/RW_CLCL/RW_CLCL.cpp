
/*----------------------------------------------------------
CLCL用データ読み書きDLL
									2003/04/30 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"
#include "RW_Plugin.h"

#pragma data_seg(".shared")
//アプリケーション名と拡張しを指定
const TCHAR *szName = _T("CLCL");
const TCHAR *szExtn = _T("dat");

//データ保管用リスト
list<STRING_DATA> *pData;
#pragma data_seg()
//#pragma comment(linker, "/SECTION:.shared,RWS")


bool readCLCLData(CFile *pMyFile,STRING_DATA *pData);
bool readUntilZero(CFile *pMyFile,char *szReadBuff,int nSize);

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
//関数名	getFormatName(char *szBuff,int nSize)
//機能		形式名称を取得
//---------------------------------------------------
extern "C" __declspec (dllexport) void getFormatName(TCHAR *szBuff,int nSize)
{
	if(sizeof(szName) > nSize) return;
	_tcscpy(szBuff,szName);
}

//---------------------------------------------------
//関数名	getExtension(char *szBuff,int nSize)
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
	Data.m_cIcon = 0;
	Data.m_timeCreate = 0;
	Data.m_timeEdit = 0;

	if(!pData) pData = new list<STRING_DATA>;
	pData->clear();

	int nID = 0;
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

	//--------------------------------ここから変更------------------------------------
	//データ読み込み
	CString strText,strFolder,strRirekiFileName;
	list<STRING_DATA> lstFolder;
	list<int> lstParent;

	//まずは履歴ファイルを読み込み
	int nFound = strFileName.ReverseFind('\\');
	if(nFound >= 0) strFolder = strFileName.Left(nFound) + _T("\\");
	else 			strFolder = _T(".\\");
	strRirekiFileName = strFolder + _T("history.dat");
	int nFolderID;

	//ファイルを開く
	if(MyFile.Open(strRirekiFileName,CFile::modeRead,NULL))	{

		Data.m_nParentID = -1;
		Data.m_cKIND = KIND_RIREKI;//種別
		Data.m_nMyID = nID++;
		nFolderID = Data.m_nMyID;

		Data.m_strTitle = _T("@h履歴");
		Data.m_strMacro = _T("count=200\r\nClassHistory=20\r\n");
		pData->insert(pData->end(),Data);
		Data.m_strMacro = _T("");

		Data.m_cIcon = 0;
		Data.m_strMacro = "";
		Data.m_nParentID = nFolderID;
		while(1) {
			Data.m_nMyID = nID++;
			//データ読み込み
			if(readCLCLData(&MyFile,&Data)) {
				if(Data.m_cKIND == -2) continue;
				else if(Data.m_cKIND == -1) {
					//1階層上がる
					list<int>::iterator it;
					it = lstParent.end();
					it--;
					Data.m_nParentID = *it;
					lstParent.pop_back(); 
				}
				else {
					if(Data.m_strTitle == "") {
						Data.m_strTitle = Data.m_strData.Left(32);
						Data.m_strTitle.Replace(_T("\n"),_T(""));
						Data.m_strTitle.Replace(_T("\t"),_T(""));
					}
					pData->insert(pData->end(),Data);
					if(Data.m_cKIND & KIND_FOLDER) {
						lstParent.push_back(Data.m_nParentID);
						Data.m_nParentID = Data.m_nMyID;
					}
				}
			}
			else break;
		}
		MyFile.Close();
	}
//regist.dat
	strRirekiFileName = strFolder + _T("regist.dat");
	lstParent.clear();

	//ファイルを開く
	if(MyFile.Open(strRirekiFileName,CFile::modeRead,NULL))	{

		Data.m_nParentID = -1;
		Data.m_cKIND = KIND_FOLDER;//種別
		Data.m_nMyID = nID++;
		nFolderID = Data.m_nMyID;

		Data.m_strTitle = _T("@t登録アイテム");
		Data.m_strMacro = _T("");
		pData->insert(pData->end(),Data);
		Data.m_strMacro = _T("");

		Data.m_cIcon = 0;
		Data.m_strMacro = "";
		Data.m_nParentID = nFolderID;
		while(1) {
			Data.m_nMyID = nID++;
			//データ読み込み
			if(readCLCLData(&MyFile,&Data)) {
				if(Data.m_cKIND == -2) continue;
				else if(Data.m_cKIND == -1) {
					//1階層上がる
					list<int>::iterator it;
					it = lstParent.end();
					it--;
					Data.m_nParentID = *it;
					lstParent.pop_back(); 
				}
				else {
					if(Data.m_strTitle == "") {
						Data.m_strTitle = Data.m_strData.Left(32);
						Data.m_strTitle.Replace(_T("\n"),_T(""));
						Data.m_strTitle.Replace(_T("\t"),_T(""));
					}
					pData->insert(pData->end(),Data);
					if(Data.m_cKIND & KIND_FOLDER) {
						lstParent.push_back(Data.m_nParentID);
						Data.m_nParentID = Data.m_nMyID;
					}
				}
			}
			else break;
		}
		MyFile.Close();
	}

	return pData;
}

//---------------------------------------------------
//関数名	readCLCLData(CFile *pMyFile,STRING_DATA *pData)
//機能		CLCLデータ1件を読み込み
//---------------------------------------------------
bool readCLCLData(CFile *pMyFile,STRING_DATA *pData)
{
	bool isRet = false;
	char cRead,strReadBuff[65535];
	CString strTmp;
	//データ種別
	if(!pMyFile->Read(&cRead,1)) return isRet;
	if(cRead == 01) {//データ
		pData->m_cKIND = KIND_LOCK;
	}
	else if(cRead == 04) {//フォルダ
		pData->m_cKIND = KIND_FOLDER;
	}
	else if(cRead == 05) {//1階層上がる
		pData->m_cKIND = -1;
		return true;
	}
	else if(cRead <= 10) return isRet;
	else {
		pMyFile->Seek(-1,CFile::current);
		goto data_section;
	}
	//タイトル
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	pData->m_strTitle = strReadBuff;
	if(pData->m_cKIND & KIND_FOLDER) {//フォルダ
		if(pData->m_strTitle == "") pData->m_strTitle = _T("フォルダ");
		return true;
	}
	//更新日時
	DWORD dwTime;
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	sscanf(strReadBuff,"%x",&dwTime);
	pData->m_timeCreate = dwTime;
	pData->m_timeEdit = dwTime;
	//無視 ウィンドウ名 0まで
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
/*	//無視 ツール用文字列 0まで
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	//無視 ツール用long 0まで atoiする
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	//無視 オプション 0まで
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
*/	//無視 ヘッダ開始位置までスキップ 02まで
	do {
		if(!pMyFile->Read(&cRead,1)) return isRet;
	} while(cRead != 0x02);
data_section:
	//サイズ0まで atoiする 10進数
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	int nSize = atoi(strReadBuff);
	//形式 0まで
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	if(strcmp(strReadBuff,"TEXT") != 0) pData->m_cKIND = -2;
/*	//無視 ツール用文字列 0まで
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	//無視 ツール用long 0まで atoiする
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
	//無視 オプション 0まで
	if(!readUntilZero(pMyFile,strReadBuff,sizeof(strReadBuff))) return isRet;
*/	//03 データ開始位置までスキップ 03まで
	do {
		if(!pMyFile->Read(&cRead,1)) return isRet;
	} while(cRead != 0x03);
	//サイズ分データ 0含む
	char *pDataBuff;
	pDataBuff = new char[nSize];
	if(!pMyFile->Read(pDataBuff,nSize)) return isRet;
	pData->m_strData = pDataBuff;
	delete pDataBuff;

	isRet = true;
	return isRet;

/*
データの種類  1byte 01ふつう 04フォルダ

フォルダ 0までタイトル

データ
タイトル      0まで
更新日時      0まで 16進数
無視 ウィンドウ名 0まで
無視 ツール用文字列 0まで
無視 ツール用long 0まで atoiする
無視 オプション 0まで
無視 ヘッダ開始位置までスキップ 02まで
サイズ0まで atoiする 10進数
TEXT 形式 0まで
無視 ツール用文字列 0まで
無視 ツール用long 0まで atoiする
無視 オプション 0まで
03 データ開始位置までスキップ 03まで
サイズ分データ 0含む
*/

}

//---------------------------------------------------
//関数名	readUntilZero(CFile *pMyFile,char *szReadBuff,int nSize)
//機能		0まで文字列を読み込み
//---------------------------------------------------
bool readUntilZero(CFile *pMyFile,char *szReadBuff,int nSize)
{
	int i = 0;
	bool isRet = false;
	char cRead;
	do {
		if(!pMyFile->Read(&cRead,1)) return isRet;
		*szReadBuff = cRead;
		szReadBuff++;i++;
	} while(cRead && i < nSize);
	*szReadBuff = NULL;

	isRet = true;
	return isRet;
}


//---------------------------------------------------
//関数名	writeDataFile(CString strFileName,CList<STRING_DATA,STRING_DATA> *pList)
//機能		リスト構造体をファイルに書き出し
//---------------------------------------------------
extern "C" __declspec (dllexport) bool writeDataFile(CString strFileName,list<STRING_DATA> *pList)
{
	bool isRet = false;
	isRet = true;
	return isRet;

}



//---------------------------------------------------
//関数名	convertMacro(char *szSource,char *szRet,int nSize,char *szSelect,char *szClip)
//機能		マクロ処理プラグイン
//---------------------------------------------------
extern "C" __declspec (dllexport) bool convertMacro(TCHAR *szSource,TCHAR *szRet,int nSize,TCHAR *szSelect,TCHAR *szClip)
{
	bool isRet = false;
	return isRet;
}
