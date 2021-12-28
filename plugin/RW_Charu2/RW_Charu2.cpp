
/*----------------------------------------------------------
Charu2用データ読み書きDLL
									2002/09/02 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"
#include "RW_Plugin.h"

#pragma data_seg(".shared")
//アプリケーション名と拡張しを指定
const TCHAR *szName = _T("Charu2");
const TCHAR *szExtn = _T("dat");

//データ保管用リスト
list<STRING_DATA> *pData;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

#define C2KIND_NORMAL 0	//ノーマル文字列
#define C2KIND_LOCK   1	//ロック項目
#define C2KIND_FOLDER 2	//フォルダ項目
#define C2KIND_RIREKI 3	//履歴フォルダ
//---------------------------------------------------
//関数名	GetRirekiOption(CString strData,CString strKind)
//機能		履歴設定を解読
//---------------------------------------------------
int GetFolderOption(CString strData,CString strKind)
{
	int nRet = 0,nFound,nFoundEnd;
	CString strBuff;

	strData.MakeLower();
	nFound = strData.Find(strKind);
	if(nFound >= 0) {
		nFound = strData.Find(_T("="),nFound);
		if(nFound >= 0) {
			nFoundEnd = strData.Find(_T("\n"),nFound);
			if(nFoundEnd <= 0) nFoundEnd = strData.GetLength();
			strBuff = strData.Mid(nFound+1,nFoundEnd - (nFound+1));
			strBuff.TrimRight();
			strBuff.TrimLeft();
			if(strBuff == _T("yes")) nRet = 1;
			else if(strBuff == _T("no")) nRet = 0;
			else if(strBuff == _T("")) nRet = -1;
			else nRet = _ttoi(strBuff.GetBuffer(strBuff.GetLength()));
		}
		else nRet = -1;
	}
	else nRet = -1;
	return nRet;
}

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
	if(!pData) pData = new list<STRING_DATA>;
	pData->clear();
	int nID = 0;
	time_t lTime;
	nID = time(&lTime); 

	STRING_DATA data;
	int nDataSize;
	int nBuff;
	char cBuff;
	FILE *fFile;

	data.m_cIcon = 0;
	data.m_timeCreate = 0;
	data.m_timeEdit = 0;
/*
	識別子
	データ種別		4byte
	フォルダ開閉	1byte
	アイコン種別	1byte
	未使用			1byte
	ID				4byte
	親のID			4byte
	タイトルサイズ	4byte
	タイトル
	本文サイズ		4byte
	本文
*/
	//ファイルの拡張子を検査
	char *szReadBuff;
	CString strExtention;
	strExtention = strFileName.Right(3);
	strExtention.MakeLower();
	if(strExtention != "dat" && strExtention != "cdt") return NULL;

	if(!(fFile = _tfopen(strFileName,_T("rb"))))	return NULL;//ファイルを開く
	if(feof(fFile)) return NULL;

	if(!fread(&nBuff,sizeof(nBuff),1,fFile)) return NULL;//データ数を読み込み
	while(!feof(fFile)) {
		if(!fread(&data.m_cKind,sizeof(data.m_cKind),1,fFile)) break;//データの種類を読み込み
		if(data.m_cKind == C2KIND_NORMAL)
			data.m_cKind = KIND_ONETIME;
		else if(data.m_cKind == C2KIND_LOCK)
			data.m_cKind = KIND_LOCK;
		else if(data.m_cKind == C2KIND_FOLDER)
			data.m_cKind = KIND_FOLDER;

		if(!fread(&cBuff,sizeof(cBuff),1,fFile)) break;//フォルダ開閉
		if(cBuff)	data.m_cKind = data.m_cKind | FOLDER_OPEN;
		if(!fread(&data.m_cIcon,sizeof(data.m_cIcon),1,fFile)) break;//アイコン種別
		if(data.m_cIcon == (char)200)	data.m_cKind = KIND_RIREKI;
		if(!fread(&cBuff,sizeof(cBuff),1,fFile)) break;//未使用
		if(!fread(&data.m_nMyID,sizeof(data.m_nMyID),1,fFile)) break;//自分のIDを読み込み
		if(!data.m_nMyID) data.m_nMyID = nID++;
		if(!fread(&data.m_nParentID,sizeof(data.m_nParentID),1,fFile)) break;//親のIDを読み込み
		//タイトルを読み込み
		if(!fread(&nDataSize,sizeof(nDataSize),1,fFile)) break;
		szReadBuff = new char[nDataSize+1];
		fread(szReadBuff,nDataSize,1,fFile);
		szReadBuff[nDataSize] = NULL;
		data.m_strTitle = szReadBuff;
		delete [] szReadBuff;
		
		//文章を読み込み
		if(!fread(&nDataSize,sizeof(nDataSize),1,fFile)) break;
		szReadBuff = new char[nDataSize+1];
		fread(szReadBuff,nDataSize,1,fFile);
		szReadBuff[nDataSize] = NULL;
		data.m_strData = szReadBuff;
		delete [] szReadBuff;

		if(data.m_cIcon < 0) data.m_cIcon = 0;
		if(data.m_cKind & KIND_FOLDER_ALL) {
			data.m_strMacro = data.m_strData;
		}
		pData->insert(pData->end(),data);
	}
	fclose(fFile);
	return pData;
}


//---------------------------------------------------
//関数名	writeDataFile(CString strFileName,CList<STRING_DATA,STRING_DATA> *pList)
//機能		リスト構造体をファイルに書き出し
//---------------------------------------------------
extern "C" __declspec (dllexport) bool writeDataFile(CString strFileName,list<STRING_DATA> *pList)
{
	bool isRet = false;
	STRING_DATA Data;
	FILE *fFile;
	vector<int> arrayEndClearParent;
	int i,nDataSize,nDataCount;
	char cBuff;
#ifdef _UNICODE
	char *lpStr;
#else
	LPTSTR lpStr;
#endif
	list<STRING_DATA> listData;
	listData = *pList;
	list<STRING_DATA>::iterator it,prevIT;

	//履歴フォルダクリアを探す
	for (it = listData.begin(); it != listData.end(); it++) {
		if((it->m_cKind & KIND_RIREKI) && GetFolderOption(it->m_strData,_T("endclear")) == 1) {
			arrayEndClearParent.insert(arrayEndClearParent.end(),it->m_nMyID);
		}
		else if(it->m_cKind & KIND_ONETIME) {
			prevIT = it;
			it++;
			listData.erase(prevIT);
			it--;
		}
		else if(it->m_cKind & KIND_LOCK) {
			for(i = 0; i < arrayEndClearParent.size(); i++) {
				if(arrayEndClearParent[i] == it->m_nParentID) {
					prevIT = it;
					it++;
					listData.erase(prevIT);
					it--;
					break;
				}
			}
		}
	}
	int nEndClearParentCount = arrayEndClearParent.size();

	//保存するデータの数を調べる
	nDataCount = listData.size();
	
	//データを保存する
	if(!(fFile = _tfopen(strFileName,_T("wb"))))	return isRet;//ファイルを開く
	fwrite(&nDataCount,sizeof(nDataCount),1,fFile);//データ数
	
	//データを保存する
	for (it = listData.begin(); it != listData.end(); it++) {
		if(it->m_cKind != KIND_FOLDER) {
			for(i = 0; i < nEndClearParentCount; i++) {
				if(arrayEndClearParent[i] == Data.m_nParentID) {
					break;
				}
			}
		}
		Data = *it;
		if(Data.m_cKind & KIND_FOLDER)		cBuff = C2KIND_FOLDER;
		else if(Data.m_cKind & KIND_RIREKI)	cBuff = C2KIND_FOLDER;
		else if(Data.m_cKind & KIND_LOCK)	cBuff = C2KIND_LOCK;
		fwrite(&cBuff,sizeof(cBuff),1,fFile);//データの種類
		if(Data.m_cKind & FOLDER_OPEN)		cBuff = 1;
		else cBuff = 0;
		fwrite(&cBuff,sizeof(cBuff),1,fFile);//開いてるか
		if(Data.m_cKind & KIND_RIREKI)	cBuff = (char)200;
		else cBuff = Data.m_cIcon;
		fwrite(&cBuff,sizeof(cBuff),1,fFile);//アイコン種別
		fwrite(&cBuff,sizeof(cBuff),1,fFile);//未使用
		fwrite(&Data.m_nMyID,sizeof(Data.m_nMyID),1,fFile);//自分のID
		fwrite(&Data.m_nParentID,sizeof(Data.m_nParentID),1,fFile);//親のID

#ifdef _UNICODE
		//タイトルを保存
		nDataSize = ::WideCharToMultiByte(CP_ACP, 0, Data.m_strTitle,-1,NULL,0,NULL,NULL);
		lpStr = new char[nDataSize + 1];
		if(lpStr) {
			::WideCharToMultiByte(CP_ACP,0,Data.m_strTitle,-1,lpStr,nDataSize + 1,"",NULL);
			fwrite(&nDataSize,sizeof(nDataSize),1,fFile);//サイズ
			fwrite(lpStr,nDataSize,1,fFile);
			delete lpStr;
		}
		//文章を保存
		nDataSize = ::WideCharToMultiByte(CP_ACP, 0, Data.m_strData,-1,NULL,0,NULL,NULL);
		lpStr = new char[nDataSize + 1];
		if(lpStr) {
			::WideCharToMultiByte(CP_ACP,0,Data.m_strData,-1,lpStr,nDataSize + 1,"",NULL);
			fwrite(&nDataSize,sizeof(nDataSize),1,fFile);//サイズ
			fwrite(lpStr,nDataSize,1,fFile);
			delete lpStr;
		}
#else
		//タイトルを保存
		nDataSize = Data.m_strTitle.GetLength() + 1;
		fwrite(&nDataSize,sizeof(nDataSize),1,fFile);//サイズ
		lpStr = (char*)LPCTSTR(Data.m_strTitle);
		fwrite(lpStr,nDataSize,1,fFile);
		//文章を保存
		nDataSize = Data.m_strData.GetLength() + 1;
		fwrite(&nDataSize,sizeof(nDataSize),1,fFile);//サイズ
		lpStr = (char*)LPCTSTR(Data.m_strData);
		fwrite(lpStr,nDataSize,1,fFile);
#endif
	}
	fclose(fFile);
	isRet = true;
	return isRet;
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
