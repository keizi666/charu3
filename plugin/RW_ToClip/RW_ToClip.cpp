
/*----------------------------------------------------------
Toclip用データ読み書きDLL
									2002/09/08 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"
#include "RW_Plugin.h"

#pragma data_seg(".shared")
//アプリケーション名と拡張しを指定
const TCHAR *szName = _T("ToClip");
const TCHAR *szExtn = _T("dat");

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

	CFile MyFile,titleFile;
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
	char *szReadBuff;
	CString strText,strFolder,strRirekiFileName;
	list<STRING_DATA> lstFolder;

	//まずは履歴ファイルを読み込み
	int nFound = strFileName.ReverseFind('\\');
	if(nFound >= 0) strFolder = strFileName.Left(nFound) + _T("\\");
	else 			strFolder = _T(".\\");
	strRirekiFileName = strFolder + _T("clphistry.dat");
	int nDataSize,nCount,i,nFolderID;
	//ファイルを開く
	if(MyFile.Open(strRirekiFileName,CFile::modeRead,NULL))	{
		if(!MyFile.Read(&nCount,4)) goto error;//件数

		Data.m_nParentID = -1;
		Data.m_cKIND = KIND_RIREKI;//種別
		Data.m_nMyID = nID++;
		nFolderID = Data.m_nMyID;

		Data.m_strTitle = _T("@r履歴");
		Data.m_strMacro = _T("count=50\r\n");
		pData->insert(pData->end(),Data);
		Data.m_strMacro = _T("");

		for(i = 0; i < nCount; i++) {
			if(!MyFile.Read(&nDataSize,sizeof(nDataSize))) break;//データサイズを読み込み
			Data.m_nParentID = nFolderID;
			Data.m_nMyID = nID++;
			
			Data.m_cKIND = KIND_LOCK;//種別
			
			//文章を読み込み
			szReadBuff = new char[nDataSize + 1];
			MyFile.Read(szReadBuff,nDataSize);
			szReadBuff[nDataSize] = NULL;
			Data.m_strData = szReadBuff;
			delete [] szReadBuff;
			Data.m_strTitle = Data.m_strData.Left(32);
			Data.m_strTitle.Replace(_T("\n"),_T("\\n"));
			Data.m_strTitle.Replace(_T("\t"),_T(""));

			pData->insert(pData->end(),Data);
		}
error:
		MyFile.Close();
	}

	//定型文を読み込み
	int nCountT,nCountD,nDataSizeT,nDataSizeD;
	CString strTitleFileName,strDataFileName;

	strTitleFileName = strFolder + _T("tctitle.dat");
	strDataFileName  = strFolder + _T("tctext.dat");
	//ファイルを開く
	if(!MyFile.Open(strDataFileName,CFile::modeRead,NULL)) goto error_end;
	if(!titleFile.Open(strTitleFileName,CFile::modeRead,NULL)) {
		MyFile.Close();
		goto error_end;
	}
	if(!titleFile.Read(&nCountT,4)) goto error2;//件数
	if(!MyFile.Read(&nCountD,4)) goto error2;//件数
	if(nCountT != nCountD) goto error2;

	Data.m_nParentID = -1;
	Data.m_cKIND = KIND_LOCK;//種別
	Data.m_strMacro = _T("");

	for(i = 0; i < nCountT; i++) {
		if(!titleFile.Read(&nDataSizeT,sizeof(nDataSizeT))) break;//データサイズを読み込み
		if(!MyFile.Read(&nDataSizeD,sizeof(nDataSizeD))) break;//データサイズを読み込み
		Data.m_nMyID = nID++;
		
		//タイトルを読み込み
		szReadBuff = new char[nDataSizeT + 1];
		titleFile.Read(szReadBuff,nDataSizeT);
		szReadBuff[nDataSizeT] = NULL;
		Data.m_strTitle = szReadBuff;
		delete [] szReadBuff;

		//文章を読み込み
		szReadBuff = new char[nDataSizeD + 1];
		MyFile.Read(szReadBuff,nDataSizeD);
		szReadBuff[nDataSizeD] = NULL;
		Data.m_strData = szReadBuff;
		delete [] szReadBuff;

		Data.m_strTitle.Replace(_T("\n"),_T("\\n"));
		Data.m_strTitle.Replace(_T("\t"),_T(""));

		if(i > 0)	pData->insert(pData->end(),Data);
	}
error2:
	MyFile.Close();
	titleFile.Close();

error_end:	
	return pData;
}


//---------------------------------------------------
//関数名	writeDataFile(CString strFileName,CList<STRING_DATA,STRING_DATA> *pList)
//機能		リスト構造体をファイルに書き出し
//---------------------------------------------------
extern "C" __declspec (dllexport) bool writeDataFile(CString strFileName,list<STRING_DATA> *pList)
{
	bool isRet = false;
	CString strFolder,strRirekiFileName;

	int nFound = strFileName.ReverseFind('\\');
	if(nFound >= 0) strFolder = strFileName.Left(nFound) + _T("\\");
	else 			strFolder = _T(".\\");

	//履歴ファイルを書き出す
	strRirekiFileName = strFolder + _T("clphistry.dat");
	list<STRING_DATA>::iterator it;
	STRING_DATA Data;
	int nDataSize,nRirekiID = -1;;
	int nRirekiChild = 0;

	//履歴フォルダのIDを保存
	for(it = pList->begin(); it != pList->end(); it++) {
		if(it->m_cKIND & KIND_RIREKI) {
			nRirekiID = it->m_nMyID;
			break;
		}
	}
	if(nRirekiID < 0) return isRet;
	//履歴の子供を数える
	for(it = pList->begin(); it != pList->end(); it++) {
		if(nRirekiID == it->m_nParentID) nRirekiChild++;
	}
	CFile MyFile(strRirekiFileName,CFile::modeWrite | CFile::modeCreate);
	MyFile.Write(&nRirekiChild,sizeof(nRirekiChild));

#ifdef _UNICODE
	int nLength;
	char *szWriteBuff;
#else
	LPTSTR lpStr;
#endif
	//データを保存する
	for(it = pList->begin(); it != pList->end(); it++) {
		if(it->m_nParentID == nRirekiID) {//履歴データを判定
			//文章を保存
#ifdef _UNICODE
			nLength = ::WideCharToMultiByte(CP_ACP, 0, it->m_strData,-1,NULL,0,NULL,NULL);
			szWriteBuff = new char[nLength];
			if(szWriteBuff) {
				::WideCharToMultiByte(CP_ACP,0,it->m_strData,-1,szWriteBuff,nLength,"",NULL);
				nDataSize = nLength - 1;
				MyFile.Write(&nDataSize,sizeof(nDataSize));
				MyFile.Write(szWriteBuff,nDataSize);
				delete szWriteBuff;
			}
#else
			nDataSize = it->m_strData.GetLength();
			MyFile.Write(&nDataSize,sizeof(nDataSize));
			lpStr = (char*)LPCTSTR(it->m_strData);
			MyFile.Write(lpStr,nDataSize);
#endif
		}
	}
	MyFile.Close();

	//定型文ファイル
	int nDataCount = 0;
	//データ件数を数える
	for(it = pList->begin(); it != pList->end(); it++) {
		if(it->m_nParentID == ROOT) nDataCount++;
	}
	CString strTitleFileName,strDataFileName;

	strTitleFileName = strFolder + _T("tctitle.dat");
	strDataFileName  = strFolder + _T("tctext.dat");
	//ファイルを開く
	CFile titleFile(strTitleFileName,CFile::modeWrite | CFile::modeCreate);
	CFile dataFile(strDataFileName,CFile::modeWrite | CFile::modeCreate);
	titleFile.Write(&nDataCount,sizeof(nDataCount));
	dataFile.Write(&nDataCount,sizeof(nDataCount));

	//データを保存する
	for(it = pList->begin(); it != pList->end(); it++) {
		if(it->m_nParentID == -1) {
			STRING_DATA Data;
			Data = *it;
			//履歴データ
			if(Data.m_cKIND & KIND_RIREKI) {
				Data.m_strTitle = _T("%ClipHistory");
				Data.m_strData = _T("");
			}
			//タイトルを保存
#ifdef _UNICODE
			nLength = ::WideCharToMultiByte(CP_ACP, 0, Data.m_strTitle,-1,NULL,0,NULL,NULL);
			szWriteBuff = new char[nLength];
			if(szWriteBuff) {
				::WideCharToMultiByte(CP_ACP,0,Data.m_strTitle,-1,szWriteBuff,nLength,"",NULL);
				nDataSize = nLength - 1;
				titleFile.Write(&nDataSize,sizeof(nDataSize));
				titleFile.Write(szWriteBuff,nDataSize);
				delete szWriteBuff;
			}
#else
			nDataSize = Data.m_strTitle.GetLength();
			titleFile.Write(&nDataSize,sizeof(nDataSize));
			lpStr = (char*)LPCTSTR(Data.m_strTitle);
			titleFile.Write(lpStr,nDataSize);
#endif

			//文章を保存
#ifdef _UNICODE
			nLength = ::WideCharToMultiByte(CP_ACP, 0, Data.m_strData,-1,NULL,0,NULL,NULL);
			szWriteBuff = new char[nLength];
			if(szWriteBuff) {
				::WideCharToMultiByte(CP_ACP,0,Data.m_strData,-1,szWriteBuff,nLength,"",NULL);
				nDataSize = nLength - 1;
				dataFile.Write(&nDataSize,sizeof(nDataSize));
				dataFile.Write(szWriteBuff,nDataSize);
				delete szWriteBuff;
			}
#else
			nDataSize = Data.m_strData.GetLength();
			dataFile.Write(&nDataSize,sizeof(nDataSize));
			lpStr = (char*)LPCTSTR(Data.m_strData);
			dataFile.Write(lpStr,nDataSize);
#endif
		}
	}
	titleFile.Close();
	dataFile.Close();

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
