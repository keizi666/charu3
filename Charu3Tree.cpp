/*----------------------------------------------------------
	Charu3Treeクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "Charu3Tree.h"
#include "General.h"
#include "StringWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _ME_NAME_
#undef _ME_NAME_
#endif
#define _ME_NAME_ _T("Charu3Tree.cpp")

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	CCharu3Tree
//機能		コンストラクタ
//---------------------------------------------------
CCharu3Tree::CCharu3Tree()
{
	m_MyStringList.clear();
	m_ImageList = NULL;
	m_nMaxID = 0;
	m_nSelectID = 0;
	m_isDrag = false;
	m_hPrevTarget = NULL;
}

//---------------------------------------------------
//関数名	~CCharu3Tree
//機能		デストラクタ
//---------------------------------------------------
CCharu3Tree::~CCharu3Tree()
{
	if(m_ImageList)	delete m_ImageList;
}


BEGIN_MESSAGE_MAP(CCharu3Tree, CTreeCtrl)
	//{{AFX_MSG_MAP(CCharu3Tree)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//---------------------------------------------------
//関数名	setPlugin(CString strPath)
//機能		プラグイン情報を設定する
//---------------------------------------------------
bool CCharu3Tree::setPlugin(CString strPath)
{
	bool isRet = false;

	readDataFile pReadData;
	HMODULE hDLL;
	initDLL pInitDLL;
	endDLL	pEndDLL;
	writeDataFile pWriteData;
	getFormatName pGetFormatName;
	getExtension  pGetExtension;
	isUnicode pIsUnicode;
	RW_PLUGIN plugin;
	CString strFileName;

	CFileFind filefind;
	strFileName = strPath + "\\*.dll";
	BOOL isSuccess = filefind.FindFile(strFileName);
	if (!isSuccess) return isRet;

	m_rwPlugin.reserve(20);//とりあえず20個分確保
	m_rwPlugin.clear();
	while(isSuccess == TRUE) {
		isSuccess = filefind.FindNextFile();
		if(filefind.IsDots() == TRUE);				//カレントか親
		else if(filefind.IsDirectory() == TRUE);	//フォルダ
		else {
			hDLL = LoadLibrary(filefind.GetFilePath());//DLLをロード
			if(hDLL){
				//関数ポインタを取得
				pIsUnicode = (isUnicode)GetProcAddress(hDLL,"isUnicode");
#ifdef _UNICODE
				if(pIsUnicode && pIsUnicode()) {
#else
				if(pIsUnicode && !pIsUnicode()) {
#endif
					pReadData = (readDataFile)GetProcAddress(hDLL,"readDataFile");
					pInitDLL = (initDLL)GetProcAddress(hDLL,"initDLL");
					pEndDLL = (endDLL)GetProcAddress(hDLL,"endDLL");
					pWriteData = (writeDataFile)GetProcAddress(hDLL,"writeDataFile");
					pGetFormatName = (getFormatName)GetProcAddress(hDLL,"getFormatName");
					pGetExtension = (getExtension)GetProcAddress(hDLL,"getExtension");

					if(pInitDLL && pEndDLL && pReadData && pWriteData && pGetFormatName && pGetExtension) {
						TCHAR strBuff[256];
						int a = sizeof(strBuff);
						plugin.m_strPluginName = filefind.GetFilePath();
						pGetExtension(strBuff,sizeof(strBuff));
						plugin.m_strExtension = strBuff;
						pGetFormatName(strBuff,sizeof(strBuff));
						plugin.m_strSoftName = strBuff;

						//デバッグログ処理
						if(theApp.m_ini.m_nDebug) {
							CString strText;
							strText.Format(_T("plugin setting \"%s\"\n"),strBuff);
							CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
						}
						
						m_rwPlugin.push_back(plugin);
					}
				}
				FreeLibrary(hDLL);
			}
		}
	}
	isRet = true;
	return isRet;
}

//---------------------------------------------------
//関数名	setImageList(POINT posSize)
//機能		イメージリストを作る
//---------------------------------------------------
void CCharu3Tree::setImageList(POINT posSize,CString strFileName,CString strPath)
{
	//アイコンのイメージリストを作る
	if(m_ImageList == NULL) {
		::SetCurrentDirectory(strPath);

		CBitmap Bitmap;
		m_ImageList = new CImageList;

		if(strFileName == "" || !CGeneral::loadBitmapFile(strFileName,&Bitmap))
			Bitmap.LoadBitmap(IDB_ICON_BITMAP);//ビットマップをロード

		BITMAP bitMap;
		Bitmap.GetBitmap(&bitMap);

		m_ImageList->Create(bitMap.bmHeight,bitMap.bmHeight,ILC_COLOR16 | ILC_MASK,22,2);

		m_ImageList->Add(&Bitmap,RGB(0,255,0));
		Bitmap.DeleteObject();		
		SetImageList(m_ImageList,TVSIL_NORMAL);
	}
}	

//---------------------------------------------------
//関数名	checkRedundancyID(int nID)
//機能		重複IDをチェック
//---------------------------------------------------
bool CCharu3Tree::checkRedundancyID(int nID)
{
	bool isRet = false;
	list<STRING_DATA>::iterator it;

	for (it = m_MyStringList.begin(); it != m_MyStringList.end(); it++) {
		if(it->m_nMyID == nID) {
			isRet = true;
			break;
		}
	}
	return isRet;
}

//---------------------------------------------------
//関数名	checkMyChild(HTREEITEM hChildItem,int nMyID)
//機能		自分の子孫フォルダか調べる
//---------------------------------------------------
bool CCharu3Tree::checkMyChild(HTREEITEM hMeItem,HTREEITEM hChildItem)
{
	bool isRet = false;

	if(!hChildItem || !hMeItem) return isRet;

	while(hChildItem) {
		hChildItem = GetParentItem(hChildItem);//親を取得
		if(hMeItem == hChildItem) {//自分の子孫でした
			isRet = true;
			break;
		}
	}

	return isRet;
}

//---------------------------------------------------
//関数名	serchMyRoots(HTREEITEM hStartItem)
//機能		一番上の親を探す
//---------------------------------------------------
HTREEITEM CCharu3Tree::serchMyRoots(HTREEITEM hStartItem)
{
	HTREEITEM hRet = NULL,hTmp;

	hTmp = hStartItem;
	while(hTmp) {
		hRet = hTmp;
		hTmp = GetParentItem(hTmp);
		if(hTmp) Expand(hTmp,TVE_COLLAPSE);
	}
	return hRet;
}

//---------------------------------------------------
//関数名	serchParentOption(HTREEITEM hStartItem,CString strOption)
//機能		親のオプションを捜していく
//---------------------------------------------------
HTREEITEM CCharu3Tree::serchParentOption(HTREEITEM hStartItem,CString strOption)
{
	HTREEITEM hRet = NULL;
	STRING_DATA data;
	int nFound;
	
	strOption.MakeLower();
	hRet = hStartItem;
	while(hRet) {
		if(hRet) {
			data = getData(hRet);

			data.m_strMacro.MakeLower();
			nFound = data.m_strMacro.Find(strOption);
			if(nFound >= 0) {
				nFound = data.m_strMacro.Find(_T("="),nFound);
				if(nFound >= 0) {
					break;
				}
			}
		}
		hRet = GetParentItem(hRet);
	}
	return hRet;
}

//---------------------------------------------------
//関数名	saveDataFile(CString strFileName,CString strPlugin)
//機能		データを保存する
//---------------------------------------------------
bool CCharu3Tree::saveDataFile(CString strFileName,CString strPlugin,HTREEITEM hStartItem)
{
	bool isRet = false;

	if(theApp.m_ini.m_etc.m_nDontSave) return true;

	list<STRING_DATA> tmplist;
	list<STRING_DATA>::iterator it;
	STRING_DATA Data;

	//全データを出力
	if(!hStartItem) {
		hStartItem = GetRootItem();
		//保存データをリストに変換（データの順番はツリーで持っているため）
		tree2List(hStartItem,&tmplist);
	}
	//エクスポートの場合
	else {
		Data = getData(hStartItem,&it);
		Data.m_nParentID = ROOT;
		tmplist.insert(tmplist.end(),Data);
		if(ItemHasChildren(hStartItem)) tree2List(GetChildItem(hStartItem),&tmplist);
	}


	if(strPlugin != DAT_FORMAT && strPlugin != DAT_FORMAT2) {
		isRet = saveDataFilePlugin(strFileName,strPlugin,&tmplist);
		return isRet;
	}

	int nDataSize;
	FILE *fFile;
	CString strTmp;

	//データを保存する
	if(!(fFile = _tfopen(strFileName,_T("wb"))))	return isRet;//ファイルを開く

	#ifdef _UNICODE
		fwrite(DAT_FORMAT2,strlen(DAT_FORMAT2),1,fFile);//データ識別子
	#else
		fwrite(DAT_FORMAT,strlen(DAT_FORMAT),1,fFile);//データ識別子
	#endif
	for (it = tmplist.begin(); it != tmplist.end(); it++) {
		
		if(it->m_cKind & KIND_ONETIME) {//ノーマル項目以外を保存
			continue;
		}
		fwrite(&it->m_cKind,sizeof(it->m_cKind),1,fFile);//データの種類
		fwrite(&it->m_cIcon,sizeof(it->m_cIcon),1,fFile);//アイコン種別
		fwrite(&it->m_nMyID,sizeof(it->m_nMyID),1,fFile);//自分のID
		fwrite(&it->m_nParentID,sizeof(it->m_nParentID),1,fFile);//親のID
		fwrite(&it->m_timeCreate,sizeof(it->m_timeCreate),1,fFile);//作成日時
		fwrite(&it->m_timeEdit,sizeof(it->m_timeEdit),1,fFile);//変更日時

		//タイトル
		nDataSize = it->m_strTitle.GetLength();
#ifdef _UNICODE
		nDataSize = nDataSize * sizeof(WCHAR);
#endif
		fwrite(&nDataSize,sizeof(nDataSize),1,fFile);
		strTmp = it->m_strTitle;
		fwrite(LPCTSTR(strTmp),nDataSize,1,fFile);

		//文章
		nDataSize = it->m_strData.GetLength();
#ifdef _UNICODE
		nDataSize = nDataSize * sizeof(WCHAR);
#endif
		fwrite(&nDataSize,sizeof(nDataSize),1,fFile);
		fwrite(LPCTSTR(it->m_strData),nDataSize,1,fFile);

		//拡張用文字列データ
		nDataSize = it->m_strMacro.GetLength();
#ifdef _UNICODE
		nDataSize = nDataSize * sizeof(WCHAR);
#endif
		fwrite(&nDataSize,sizeof(nDataSize),1,fFile);
		fwrite(LPCTSTR(it->m_strMacro),nDataSize,1,fFile);
	}
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("wrriten charu3 data \"%s\"\n"),strFileName);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
	fclose(fFile);

	isRet = true;
	return isRet;
}

//---------------------------------------------------
//関数名	saveDataFilePlugin(CString strFileName,CString strPlugin)
//機能		プラグインを使ってデータを保存する
//---------------------------------------------------
bool CCharu3Tree::saveDataFilePlugin(CString strFileName,CString strPlugin,list<STRING_DATA> *tmplist)
{
	bool isRet = false;
	RW_PLUGIN plugin;

	if(!getPlugin(strPlugin,&plugin)) {
		CString strRes;
		strRes.LoadString(APP_MES_NOT_FOUND_WRITEPLUGIN);
		AfxMessageBox(strRes,MB_ICONEXCLAMATION,0);
		return isRet;
	}

	HMODULE hDLL;
	writeDataFile pWriteData;

	hDLL = LoadLibrary(plugin.m_strPluginName);//DLLをロード
	if(hDLL){
		//関数ポインタを取得
		pWriteData = (writeDataFile)GetProcAddress(hDLL,"writeDataFile");

		if(pWriteData) {
			isRet = pWriteData(strFileName,tmplist);
		}
		FreeLibrary(hDLL);
	}
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("wrriten plugin data \"%s\" %s %d\n"),strFileName,strPlugin,isRet);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
	return isRet;
}

//---------------------------------------------------
//関数名	loadDataFileDef(CString strFileName,CString strPlugin)
//機能		デフォルトリストにデータを読み込む
//---------------------------------------------------
bool CCharu3Tree::loadDataFileDef(CString strFileName,CString strPlugin) {
	DeleteAllItems();
	bool isRet = loadDataFile(strFileName,strPlugin,&m_MyStringList);
	CWnd::LockWindowUpdate();
	copyData(ROOT,TVI_ROOT,&m_MyStringList);//ツリーにデータをセット
	CWnd::UnlockWindowUpdate();

	return isRet;
}	

//---------------------------------------------------
//関数名	loadDataFile(CString strFileName,CString strPlugin)
//機能		データを読み込む
//---------------------------------------------------
bool CCharu3Tree::loadDataFile(CString strFileName,CString strPlugin,list<STRING_DATA> *tmplist)
{
	bool isRet = false;

	if(strPlugin != DAT_FORMAT) {
		//プラグインを使ってファイルから読み込む
		return loadDataFilePlugin(strFileName,strPlugin,tmplist);
	}

	STRING_DATA data;
	initStringData(&data);
	int nDataSize;
	FILE *fFile;
/*
	識別子
	データ種別		1byte
	アイコン種別	1byte
	ID				4byte
	親のID			4byte
	作成日時		4byte
	タイトルサイズ	4byte
	タイトル
	本文サイズ		4byte
	本文
	マクロサイズ	4byte
	マクロ
*/
	//ファイルの拡張子を検査
	char  *szReadBuff;
	char strMBCS[256];
	wchar_t *szUniReadBuff;
	int  nVersion = 0;
	CString strExtention;
	strExtention = strFileName.Right(3);
	strExtention.MakeLower();
	list<STRING_DATA> readList;//データリスト

	if(strExtention != DAT_EXT) return isRet;

	if(!(fFile = _tfopen(strFileName,_T("rb"))))	return isRet;//ファイルを開く
	if(!fread(strMBCS,strlen(DAT_FORMAT),1,fFile)) {;//データ識別子を取得
		fclose(fFile);
		return isRet;
	}

	strMBCS[strlen(DAT_FORMAT)] = NULL;
	
	if(strcmp(strMBCS,DAT_FORMAT) != 0) {
		nVersion = 1;
		if(strcmp(strMBCS,DAT_FORMAT2) != 0) {
			fclose(fFile);
			return isRet;
		}
	}

	if(feof(fFile)) {
		fclose(fFile);
		return isRet;
	}

	readList.clear();

	//仮リストに読み込み
	while(!feof(fFile)) {
		if(!fread(&data.m_cKind,sizeof(data.m_cKind),1,fFile)) break;//データの種類を読み込み
		if(!fread(&data.m_cIcon,sizeof(data.m_cIcon),1,fFile)) break;//アイコン種別
		if(!fread(&data.m_nMyID,sizeof(data.m_nMyID),1,fFile)) break;//自分のIDを読み込み
		if(!data.m_nMyID) data.m_nMyID = makeNewID();
		if(!fread(&data.m_nParentID,sizeof(data.m_nParentID),1,fFile)) break;//親のIDを読み込み
		if(!fread(&data.m_timeCreate,sizeof(data.m_timeCreate),1,fFile)) break;//作成日時を読み込み
		if(!fread(&data.m_timeEdit,sizeof(data.m_timeEdit),1,fFile)) break;//変更日時を読み込み

		//文字列データ読み込み MBCS
		if(nVersion == 0) {
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

			//拡張用文字列データ
			if(!fread(&nDataSize,sizeof(nDataSize),1,fFile)) break;
			szReadBuff = new char[nDataSize+1];
			fread(szReadBuff,nDataSize,1,fFile);
			szReadBuff[nDataSize] = NULL;
			data.m_strMacro = szReadBuff;
			delete [] szReadBuff;
		}
		//文字列データ読み込み　Unicode
		else if(nVersion == 1) {
			//タイトルを読み込み
			if(!fread(&nDataSize,sizeof(nDataSize),1,fFile)) break;
			szUniReadBuff = new wchar_t[nDataSize+1];
			fread(szUniReadBuff,nDataSize,1,fFile);
			szUniReadBuff[nDataSize / sizeof(WCHAR)] = NULL;
#ifdef _UNICODE
			data.m_strTitle = szUniReadBuff;
#else
			data.m_strTitle = CGeneral::wideCharToCString(szUniReadBuff);
#endif
			delete [] szUniReadBuff;
			//文章を読み込み
			if(!fread(&nDataSize,sizeof(nDataSize),1,fFile)) break;
			szUniReadBuff = new wchar_t[nDataSize+1];
			fread(szUniReadBuff,nDataSize,1,fFile);
			szUniReadBuff[nDataSize / sizeof(WCHAR)] = NULL;
#ifdef _UNICODE
			data.m_strData = szUniReadBuff;
#else
			data.m_strData = CGeneral::wideCharToCString(szUniReadBuff);
#endif
			delete [] szUniReadBuff;

			//拡張用文字列データ
			if(!fread(&nDataSize,sizeof(nDataSize),1,fFile)) break;
			szUniReadBuff = new wchar_t[nDataSize+1];
			fread(szUniReadBuff,nDataSize,1,fFile);
			szUniReadBuff[nDataSize / sizeof(WCHAR)] = NULL;
#ifdef _UNICODE
			data.m_strMacro = szUniReadBuff;
#else
			data.m_strMacro = CGeneral::wideCharToCString(szUniReadBuff);
#endif
			delete [] szUniReadBuff;
		}

		if(data.m_cIcon < KIND_DEFAULT) data.m_cIcon = KIND_DEFAULT;
		if(data.m_timeCreate < 0) data.m_timeCreate = 0;
		if(data.m_timeEdit < data.m_timeCreate) data.m_timeEdit = data.m_timeCreate;

		readList.insert(readList.end(),data);//リストに追加
	}

	//本リストに登録	
	tmplist->clear();
	list<STRING_DATA>::iterator stit;

	for(stit = readList.begin(); stit != readList.end(); stit++) {
		data = *stit;
		if(checkRedundancyID(data.m_nMyID)) {
			int nOldID = data.m_nMyID;
			data.m_nMyID = makeNewID();
			list<STRING_DATA>::iterator newit;
			for(newit = readList.begin(); newit != readList.end(); newit++) {
				if(newit->m_nParentID == nOldID) newit->m_nParentID = data.m_nMyID;
			}
		}
		tmplist->insert(tmplist->end(),data);//リストに追加
	}
	fclose(fFile);
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("read charu3 data \"%s\"\n"),strFileName);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	isRet = true;
	return isRet;
}

//---------------------------------------------------
//関数名	getPlugin(CString strName,RW_PLUGIN* pPlugin)
//機能		プラグインを検索
//---------------------------------------------------
bool CCharu3Tree::getPlugin(CString strName,RW_PLUGIN* pPlugin)
{
	bool isRet = false;
	vector<RW_PLUGIN>::iterator it;

	for(it = m_rwPlugin.begin(); it != m_rwPlugin.end(); it++) {
		if(it->m_strSoftName == strName) {
			isRet = true;
			*pPlugin = *it;
			break;
		}
	}
	return isRet;
}

//---------------------------------------------------
//関数名	loadDataFilePlugin(CString strFileName,CString strPlugin)
//機能		プラグインでデータを読み込み
//---------------------------------------------------
bool CCharu3Tree::loadDataFilePlugin(CString strFileName,CString strPlugin,list<STRING_DATA> *tmplist)
{
	bool isRet = false;
	RW_PLUGIN plugin;

	if(!getPlugin(strPlugin,&plugin)) {
		CString strRes;
		strRes.LoadString(APP_MES_NOT_FOUND_READPLUGIN);
		AfxMessageBox(strRes,MB_ICONEXCLAMATION,0);
		return false;
	}

	HMODULE hDLL;
	readDataFile pReadData;
	initDLL pInitDLL;
	endDLL	pEndDLL;
	time_t	timeCreate;	//作成日時
	time(&timeCreate);

	list<STRING_DATA> *pData;//読み込みリスト
	hDLL = LoadLibrary(plugin.m_strPluginName);//DLLをロード
	if(hDLL){
		//関数ポインタを取得
		pReadData = (readDataFile)GetProcAddress(hDLL,"readDataFile");
		pInitDLL = (initDLL)GetProcAddress(hDLL,"initDLL");
		pEndDLL = (endDLL)GetProcAddress(hDLL,"endDLL");

		if(pInitDLL && pEndDLL && pReadData) {
			pInitDLL();
			pData = pReadData(strFileName,false);//読み込み関数を呼び出し

			if(!pData) {
				pEndDLL();
				FreeLibrary(hDLL);
				return isRet;
			}
			tmplist->clear();
			TCHAR *szBuff;
			STRING_DATA data,data2;

			list<STRING_DATA> readList;//データリスト
			list<STRING_DATA>::iterator stit;
			//仮リストに登録
			for(stit = pData->begin(); stit != pData->end(); stit++) {
				if(stit->m_strTitle != "") {
					szBuff = new TCHAR[stit->m_strTitle.GetLength()+1];
					_tcscpy(szBuff,LPCTSTR(stit->m_strTitle));
					data.m_strTitle = szBuff;
					delete [] szBuff;
				}
				else	data.m_strTitle = "";

				if(stit->m_strData != "") {
					szBuff = new TCHAR[stit->m_strData.GetLength()+1];
					_tcscpy(szBuff,LPCTSTR(stit->m_strData));
					data.m_strData = szBuff;
					delete [] szBuff;
				}
				else	data.m_strData = "";

				if(stit->m_strMacro != "") {
					szBuff = new TCHAR[stit->m_strMacro.GetLength()+1];
					_tcscpy(szBuff,LPCTSTR(stit->m_strMacro));
					data.m_strMacro = szBuff;
					delete [] szBuff;
				}
				else	data.m_strMacro = "";

				data.m_cKind = stit->m_cKind;
				data.m_nMyID = stit->m_nMyID;
				data.m_nParentID = stit->m_nParentID;

				data.m_cIcon = stit->m_cIcon;
				if(data.m_cIcon < KIND_DEFAULT) data.m_cIcon = KIND_DEFAULT;

				data.m_timeCreate = stit->m_timeCreate;
				if(data.m_timeCreate <= 0) data.m_timeCreate = timeCreate;
				data.m_timeEdit = stit->m_timeEdit;
				if(data.m_timeEdit <= 0) data.m_timeEdit = timeCreate;

				readList.insert(readList.end(),data);
			}
			pEndDLL();//メモリを開放

			//本リストに登録	
			tmplist->clear();
			normalizationID(&readList,ROOT);//IDを正規化
			for(stit = readList.begin(); stit != readList.end(); stit++) {
				tmplist->insert(tmplist->end(),*stit);//リストに追加
			}
		}

		FreeLibrary(hDLL);
	}
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("read plugin data \"%s\" %s\n"),strFileName,strPlugin);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
	isRet = true;
	return isRet;
}

//---------------------------------------------------
//関数名	normalizationID(list<STRING_DATA>* pList,int nParentID)
//機能		IDを正規化(ほとんどCharu2Proのデータ専用)
//---------------------------------------------------
void CCharu3Tree::normalizationID(list<STRING_DATA>* pList,int nParentID)
{
	list<STRING_DATA>::iterator it;

	for(it = pList->begin(); it != pList->end(); it++) {
		//親のIDを比較
		if(it->m_nParentID == nParentID) {
			if(checkRedundancyID(it->m_nMyID)) {//IDの重複をチェック
				int nOldID = it->m_nMyID;
				it->m_nMyID = makeNewID();//新しいIDを振る
				list<STRING_DATA>::iterator newit;
				for(newit = pList->begin(); newit != pList->end(); newit++) {//自分の子の親IDを変える
					if(newit->m_nParentID == nOldID) newit->m_nParentID = it->m_nMyID;
				}
			}
			//自分の子供達を再帰処理
			if(it->m_cKind & KIND_FOLDER_ALL)	normalizationID(pList,it->m_nMyID);
		}
	}
}

//---------------------------------------------------
//関数名	convertMacroPlugin(CString strSourceData)
//機能		マクロ文字列を置換
//---------------------------------------------------
bool CCharu3Tree::convertMacroPlugin(STRING_DATA *SourceData,CString *strRet,CString strSelect,CString strClip,CString strSoftName)
{
	RW_PLUGIN plugin;
	TCHAR *szRet;
	bool isRet = false;

	bool isFound = getPlugin(strSoftName,&plugin);

	if(!isFound) {
		CString strRes;
		strRes.LoadString(APP_MES_NOT_FOUND_MACROPLUGIN);
		AfxMessageBox(strRes,MB_ICONEXCLAMATION,0);
		return isRet;
	}

	HMODULE hDLL;
	convertMacro pConvertMacro;

	hDLL = LoadLibrary(plugin.m_strPluginName);//DLLをロード
	if(hDLL){
		//関数ポインタを取得
		pConvertMacro = (convertMacro)GetProcAddress(hDLL,"convertMacro");

		if(pConvertMacro) {
			int nSize = SourceData->m_strData.GetLength()*10+10240;
			szRet = NULL;
			szRet = new TCHAR[nSize];
			if(szRet) {
				isRet = pConvertMacro((TCHAR*)LPCTSTR(SourceData->m_strData),szRet,nSize,
					(TCHAR*)LPCTSTR(strSelect),(TCHAR*)LPCTSTR(strClip));
				*strRet = szRet;
				delete [] szRet;
			}
		}
		FreeLibrary(hDLL);
	}
	return isRet;
}

//---------------------------------------------------
//関数名	copyData
//引数		int nParentID 親のID
//          HTREEITEM hParentTreeItem 親のポインタ
//機能		文字列データリストをフォルダ表示する
//---------------------------------------------------
void CCharu3Tree::copyData(int nParentID,HTREEITEM hParentTreeItem,list<STRING_DATA> *pList)
{
	int nRet = 0;
	HTREEITEM hTreeItem;
	list<STRING_DATA>::iterator it;

	//ツリーデータ
	TV_INSERTSTRUCT TreeCtrlItem;
	
	//データを処理
	for (it = pList->begin(); it != pList->end(); it++) {
		//親IDと同じのだけ処理
		if(it->m_nParentID == nParentID) {
			data2TreeStruct(&TreeCtrlItem,it);//データを写す
			TreeCtrlItem.hParent = hParentTreeItem;//親を設定

			hTreeItem = InsertItem(&TreeCtrlItem);//インサート

			if(*m_nSelectID == it->m_nMyID) SelectItem(hTreeItem);

			//フォルダなら再帰呼び出し
			if(it->m_cKind & KIND_FOLDER_ALL) {
				copyData(it->m_nMyID,hTreeItem,pList);
				if(it->m_cKind & FOLDER_OPEN)	Expand(hTreeItem,TVE_EXPAND);
				else							Expand(hTreeItem,TVE_COLLAPSE);
			}
		}
	}
}

//---------------------------------------------------
//関数名	moveChildren(HTREEITEM hFromItem,HTREEITEM hToItem)
//機能		フォルダからフォルダに子供たちを移動
//---------------------------------------------------
void CCharu3Tree::moveChildren(HTREEITEM hFromItem,HTREEITEM hToItem)
{
	if(! hFromItem || !hToItem) return;
	list<STRING_DATA>::iterator it;
	hFromItem = GetChildItem(hFromItem);
	do {
		//データを取得
		STRING_DATA dataF;
		dataF = getData(hFromItem);

		//ツリーデータを取得
		TV_ITEM TreeCtrlItemFrom;
		ZeroMemory(&TreeCtrlItemFrom,sizeof(TreeCtrlItemFrom));
		TreeCtrlItemFrom.hItem = hFromItem;
		TreeCtrlItemFrom.mask = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		GetItem(&TreeCtrlItemFrom);

		//追加データを作成
		TV_INSERTSTRUCT AddTreeCtrlItem;

		ZeroMemory(&AddTreeCtrlItem,sizeof(AddTreeCtrlItem));
		AddTreeCtrlItem.hInsertAfter = hToItem;
		AddTreeCtrlItem.item.mask = TreeCtrlItemFrom.mask;
		AddTreeCtrlItem.item.iImage = TreeCtrlItemFrom.iImage;
		AddTreeCtrlItem.item.iSelectedImage = TreeCtrlItemFrom.iSelectedImage;
		AddTreeCtrlItem.item.pszText = dataF.m_strTitle.GetBuffer(dataF.m_strTitle.GetLength());
		AddTreeCtrlItem.item.lParam = TreeCtrlItemFrom.lParam;
		AddTreeCtrlItem.hParent = hToItem;//フォルダ

		//同じデータを挿入
		HTREEITEM hInsertItem = InsertItem(&AddTreeCtrlItem);
		//元にフォルダ発見
		if(ItemHasChildren(hFromItem)) {
			//ターゲットを決めて再帰コピー
			moveChildren(hFromItem,hInsertItem);
		}
		hFromItem = GetNextItem(hFromItem,TVGN_NEXT);


	}while(hFromItem);
}
//---------------------------------------------------
//関数名	copyChildren(HTREEITEM hFromItem,HTREEITEM hToItem)
//機能		フォルダからフォルダに子供たちをコピー
//---------------------------------------------------
void CCharu3Tree::copyChildren(HTREEITEM hFromItem,HTREEITEM hToItem)
{
	if(! hFromItem || !hToItem) return;
	list<STRING_DATA>::iterator it;
	hFromItem = GetChildItem(hFromItem);
	HTREEITEM hAddtItem = NULL;
	do {
		//データを取得
		STRING_DATA dataF;
		dataF = getData(hFromItem);

		//同じデータを挿入
		if(hAddtItem)
			hAddtItem = addData(hAddtItem,dataF);
		else
			hAddtItem = addData(hToItem,dataF,true,true);
		//元にフォルダ発見
		if(ItemHasChildren(hFromItem)) {
			//ターゲットを決めて再帰コピー
			copyChildren(hFromItem,hAddtItem);
		}
		hFromItem = GetNextItem(hFromItem,TVGN_NEXT);
	}while(hFromItem);
}


//---------------------------------------------------
//関数名	addNewFolder(HTREEITEM hTreeItem)
//機能		新しいフォルダを追加する
//---------------------------------------------------
HTREEITEM CCharu3Tree::addNewFolder(HTREEITEM hTreeItem,CString strName)
{
	STRING_DATA Data;
	initStringData(&Data);

	Data.m_cKind = KIND_FOLDER;
	Data.m_strData = "";
	Data.m_strTitle = strName;
	HTREEITEM hAddItem = addData(hTreeItem,Data);
	SelectItem(hAddItem);
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("add new folder \"%s\"\n"),strName);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	return hAddItem;
}

//---------------------------------------------------
//関数名	margeTreeData(HTREEITEM hTreeItem,list<STRING_DATA> *pList)
//機能		リストをマージしてツリーに反映させる
//---------------------------------------------------
HTREEITEM CCharu3Tree::mergeTreeData(HTREEITEM hTreeItem,list<STRING_DATA> *pList,bool isRoot)
{
	if(pList->size() > 0) {
		STRING_DATA folder;
		int nParentID;
		CString strRes;
		if(!isRoot) {//インポートフォルダを作る
			strRes.LoadString(APP_INF_IMPORT_FOLDERNAME);
			hTreeItem = addNewFolder(hTreeItem,strRes);//親フォルダを作る
			if(!hTreeItem) return NULL;
			folder = getData(hTreeItem);
			nParentID = folder.m_nMyID;
		}
		else {//ルートに展開する
			nParentID = ROOT;
		}
		mergeList(&m_MyStringList,pList,nParentID);
		//親のいない子は消す
		list<STRING_DATA>::iterator it,itNext;
		int i;
		for (i = 0,it = m_MyStringList.begin(); it != m_MyStringList.end(); i++,it++) {
			STRING_DATA data;
			data = *it;
			if(it->m_nParentID != ROOT && !checkRedundancyID(it->m_nParentID)) {
				//親がいないのは消しちゃう
				itNext = it;
				if(i > 0) itNext--;
				m_MyStringList.erase(it);
				if(i > 0) it = itNext;
				else it = m_MyStringList.begin();
			}
		}
		CWnd::LockWindowUpdate();
		if(!isRoot){
			copyData(nParentID,hTreeItem,&m_MyStringList);
		}
		else {
			DeleteAllItems();
			copyData(nParentID,TVI_ROOT,&m_MyStringList);
			hTreeItem = this->GetRootItem();
		}
		CWnd::UnlockWindowUpdate();
	}
	else hTreeItem = NULL;
	return hTreeItem;
}

//---------------------------------------------------
//関数名	mergeList(list<STRING_DATA> *pMainList,list<STRING_DATA> *pList,int nParent)
//機能		リストをマージする
//---------------------------------------------------
int CCharu3Tree::mergeList(list<STRING_DATA> *pMainList,list<STRING_DATA> *pList,int nParent)
{
	int nRet = 0,nBeginID = 0;
	list<STRING_DATA>::iterator it;
	it = pList->begin();
	nBeginID = ROOT;

	for(it = pList->begin(); it != pList->end(); it++) {
		if(it->m_nParentID == nBeginID)	it->m_nParentID = nParent;
		pMainList->insert(pMainList->end(),*it);
		nRet++;
	}

	return nRet;
}

//---------------------------------------------------
//関数名	addData(HTREEITEM hTreeItem,STRING_DATA Data)
//機能		データを追加する
//---------------------------------------------------
HTREEITEM CCharu3Tree::addData(HTREEITEM hTreeItem,STRING_DATA data,bool isNewID/* = true*/,bool isChild /*fasle*/)
{
	list<STRING_DATA>::iterator it;

	if(isNewID)	data.m_nMyID = makeNewID();
	data.m_nParentID = ROOT;
	//NULLでくると親はROOTになる
	if(hTreeItem) {
		HTREEITEM hParentItem;
		if(isChild)	hParentItem = hTreeItem;
		else		hParentItem = GetParentItem(hTreeItem);
		if(hParentItem) {
			getData(hParentItem,&it);
			data.m_nParentID = it->m_nMyID;
		}
	}

	if(data.m_strTitle == "") data.m_strTitle = makeTitle(data.m_strData);

	time(&data.m_timeCreate);
	time(&data.m_timeEdit);
	it = m_MyStringList.insert(m_MyStringList.end(),data);//リストに追加

	//ツリーデータ作成
	TV_INSERTSTRUCT TreeCtrlItem;
	data2TreeStruct(&TreeCtrlItem,it);

	if(hTreeItem) {
		if(isChild) {//子供にする場合はフォルダの先頭に入れる
			TreeCtrlItem.hParent = hTreeItem;
			TreeCtrlItem.hInsertAfter = TVI_FIRST;//どこに入れるか指定
		}
		else {//兄弟の場合は自分の下に入れる
			TreeCtrlItem.hInsertAfter = hTreeItem;
			TreeCtrlItem.hParent = GetParentItem(hTreeItem);
		}
	}
	else {//ルートに入れる場合は先頭に入れる
		TreeCtrlItem.hInsertAfter = TVI_FIRST;//どこに入れるか指定
		TreeCtrlItem.hParent = TVI_ROOT;//親を設定
	}

	//ツリーに追加
	hTreeItem = InsertItem(&TreeCtrlItem);
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("add data %s %s %d\n"),data.m_strTitle,data.m_strData,data.m_cKind);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
	return hTreeItem;
}

//---------------------------------------------------
//関数名	tree2List(HTREEITEM hStartItem,list<STRING_DATA> *list)
//機能		ツリー構造体からリストを作成
//---------------------------------------------------
void CCharu3Tree::tree2List(HTREEITEM hStartItem,list<STRING_DATA> *tmplist,bool isAll/*=false*/)
{
	if(!hStartItem) return;
	list<STRING_DATA>::iterator it;
	STRING_DATA data;
	HTREEITEM hItem = hStartItem;
	do {
		data = getData(hItem,&it);

		tmplist->insert(tmplist->end(),data);
		if(ItemHasChildren(hItem) && (getDataOption(data.m_strMacro,"clearrec") != 1 || isAll)) tree2List(GetChildItem(hItem),tmplist);
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}while(hItem);
}

//---------------------------------------------------
//関数名	data2TreeStruct(TV_INSERTSTRUCT *pTreeCtrlItem,list<STRING_DATA>::iterator *pit)
//機能		ツリー構造体にデータを格納
//---------------------------------------------------
void CCharu3Tree::data2TreeStruct(TV_INSERTSTRUCT *pTreeCtrlItem,list<STRING_DATA>::iterator it)
{
	//ツリーデータ作成
	pTreeCtrlItem->hInsertAfter = TVI_LAST;
	pTreeCtrlItem->item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	STRING_DATA data;
	data.m_strTitle = it->m_strTitle;
	
	//アイコンを設定
	int nIcon = getIconNumber(it->m_cKind,it->m_cIcon);
	pTreeCtrlItem->item.iImage = nIcon;
	pTreeCtrlItem->item.iSelectedImage = nIcon + 1;

	//名前を設定
	pTreeCtrlItem->item.pszText = (TCHAR*)LPCTSTR(it->m_strTitle);
	//データのアドレスを設定
	memcpy((void*)&pTreeCtrlItem->item.lParam,(void*)&it,sizeof(it));
}

//---------------------------------------------------
//関数名	getIconNumber(char cKind,char cIcon)
//機能		アイコン番号を取得
//---------------------------------------------------
int CCharu3Tree::getIconNumber(char cKind,char cIcon)
{
	int nRet = 0;
	if(cKind & KIND_FOLDER) {
		nRet = ICON_FOLDER;
	}
	else if(cKind & KIND_RIREKI) {
		nRet = ICON_RIREKI_FOLDER;
	}
	else if(cKind & KIND_LOCK){//ロック項目の場合
		if(cIcon > 0) {
			int nIcon = cIcon * 2;
			nRet = ICON_KEY + nIcon;
		}
		else {
			nRet = ICON_KEY;
		}
	}
	else {//一時項目の場合
		nRet = ICON_ONETIME;
	}
	return nRet;
}


//---------------------------------------------------
//関数名	deleteData(HTREEITEM hTreeItem)
//機能		データを削除する
//---------------------------------------------------
void CCharu3Tree::deleteData(HTREEITEM hTreeItem)
{
	list<STRING_DATA>::iterator it;
	//データのアドレスを設定
	getData(hTreeItem,&it);

	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("delete data \"%s\" %d\n"),it->m_strTitle,it->m_cKind);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	if(it->m_cKind & KIND_FOLDER_ALL) {
		//フォルダを再帰で削除
		clearFolder(GetChildItem(hTreeItem));//フォルダの子を削除
		m_MyStringList.erase(it);
		checkOut(hTreeItem);
		DeleteItem(hTreeItem);
	}
	else {
		m_MyStringList.erase(it);
		checkOut(hTreeItem);
		this->DeleteItem(hTreeItem);
	}
}

//---------------------------------------------------
//関数名	void clearFolder(HTREEITEM hStartItem)
//機能		指定データ以下を削除する
//---------------------------------------------------
void CCharu3Tree::clearFolder(HTREEITEM hStartItem)
{
	if(!hStartItem) return;
	list<STRING_DATA>::iterator it;
	HTREEITEM hItem = hStartItem,hPrevItem;
	do {
		if(ItemHasChildren(hItem)) {
			clearFolder(GetChildItem(hItem));//再帰で削除
		}
		getData(hItem,&it);
		hPrevItem = hItem;
		hItem = GetNextItem(hItem,TVGN_NEXT);
		m_MyStringList.erase(it);
		checkOut(hPrevItem);
		this->DeleteItem(hPrevItem);
	}while(hItem);
}

//---------------------------------------------------
//関数名	void closeFolder(HTREEITEM hStartItem)
//機能		指定データ以下を閉じる
//---------------------------------------------------
void CCharu3Tree::closeFolder(HTREEITEM hStartItem)
{
	if(!hStartItem) return;
	list<STRING_DATA>::iterator it;
	HTREEITEM hItem = hStartItem;
	do {
		if(ItemHasChildren(hItem)) {
			getData(hItem,&it);
			Expand(hItem,TVE_COLLAPSE);
			it->m_cKind = it->m_cKind & (FOLDER_OPEN ^ 0xff); 
			closeFolder(GetChildItem(hItem));//再帰で閉じる
		}
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}while(hItem);
}

//---------------------------------------------------
//関数名	checkFolder(HTREEITEM hStartItem,bool isCheck)
//機能		指定データ以下にチェックを入れる
//---------------------------------------------------
void CCharu3Tree::checkFolder(HTREEITEM hStartItem,bool isCheck,list<HTREEITEM> *listItem)
{
	if(!hStartItem) return;
	list<STRING_DATA>::iterator it;
	SetCheck(hStartItem,isCheck);
	HTREEITEM hItem = GetChildItem(hStartItem);
	do {
		if(ItemHasChildren(hItem)) {
			checkFolder(hItem,isCheck,listItem);
		}
		SetCheck(hItem,isCheck);
		if(isCheck) {//リストに追加
			listItem->insert(listItem->end(),hItem);
		}
		else {//リストから削除
			list<HTREEITEM>::iterator it;
			for(it = listItem->begin(); it != listItem->end(); it++) {
				if(*it == hItem) {
					listItem->erase(it);
					break;
				}
			}
		}
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}while(hItem);
}

//---------------------------------------------------
//関数名	void clearOneTime(HTREEITEM hStartItem)
//機能		一時項目を削除する
//---------------------------------------------------
void CCharu3Tree::clearOneTime(HTREEITEM hStartItem,int nKind/*KIND_LOCKだと削除でなくロック処理*/)
{
	if(!hStartItem) return;
	list<STRING_DATA>::iterator it;
	HTREEITEM hItem = hStartItem,hPrevItem;
	do {
		if(ItemHasChildren(hItem)) {
			clearOneTime(GetChildItem(hItem),nKind);//再帰で処理
		}
		getData(hItem,&it);
		hPrevItem = hItem;
		hItem = GetNextItem(hItem,TVGN_NEXT);
		if(it->m_cKind & KIND_ONETIME) {
			if(nKind == KIND_LOCK) {//ロック処理の場合
				STRING_DATA data = getData(hPrevItem);
				data.m_cKind = KIND_LOCK;
				editData(hPrevItem,data);
			}
			else {//クリア処理に場合
				m_MyStringList.erase(it);
				checkOut(hPrevItem);
				DeleteItem(hPrevItem);
			}
		}
	}while(hItem);
}


//---------------------------------------------------
//関数名	editData(HTREEITEM hTreeItem,STRING_DATA Data)
//機能		データを変更する
//---------------------------------------------------
void CCharu3Tree::editData(HTREEITEM hTreeItem,STRING_DATA Data)
{
	list<STRING_DATA>::iterator it;

	//データのアドレスを設定
	getData(hTreeItem,&it);
	time(&Data.m_timeEdit);

	*it = Data;
	int nIcon = getIconNumber(Data.m_cKind,Data.m_cIcon);

	SetItemImage(hTreeItem,nIcon,nIcon + 1);
	SetItemText(hTreeItem,Data.m_strTitle);
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("edit data \"%s\" %d\n"),it->m_strTitle,it->m_cKind);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
}
void CCharu3Tree::changeIcon(HTREEITEM hTreeItem,int nID) 
{
	STRING_DATA data = getData(hTreeItem);
	data.m_cIcon = nID;
	editData(hTreeItem,data);
}

//---------------------------------------------------
//関数名	getData(HTREEITEM hTreeItem)
//機能		ツリーハンドルからデータを取得
//---------------------------------------------------
STRING_DATA CCharu3Tree::getData(HTREEITEM hTreeItem)
{
	STRING_DATA retData;
	list<STRING_DATA>::iterator it;

	//データのアドレスを設定
	getData(hTreeItem,&it);

	retData = *it;
	
	return retData;
}
STRING_DATA CCharu3Tree::getData(HTREEITEM hTreeItem,list<STRING_DATA>::iterator *it)
{
	STRING_DATA retData;

	//データのアドレスを設定
	long lParam = GetItemData(hTreeItem);
	memcpy((void*)it,(void*)&lParam,sizeof(lParam));

	retData = **it;
	
	
	return retData;
}

//---------------------------------------------------
//関数名	serchItem()
//機能		指定のアイテムを検索し、選択する
//---------------------------------------------------
HTREEITEM CCharu3Tree::serchItem(int nKind,int nLogic,CString strKey,HTREEITEM hStartItem)
{
	vector<CString> m_keyVector;//検索キー
	vector<CString>::iterator keyIT;
	int nEnd;
	bool isTitleFound = false,isDataFound = false,isFound = false;
	STRING_DATA data,parentData;
	HTREEITEM hTreeItem;

	//余分を切り取り
	strKey.TrimLeft();
	strKey.TrimRight();

	//キーワードをスペースで切り出し
	CString strRes;
	strRes.LoadString(APP_INF_2BYTESPACE);
	strKey.Replace(strRes,_T(" "));
	while(1) {
		nEnd = strKey.Find(_T(" "));//空白で区切る
		if(nEnd == -1) break;
		m_keyVector.insert(m_keyVector.end(),strKey.Left(nEnd));//配列に追加
		strKey = strKey.Right(strKey.GetLength() - nEnd);//追加した文を切り取り
		strKey.TrimLeft();
	}
	m_keyVector.insert(m_keyVector.end(),strKey);//配列に追加

	//検索
	if(!hStartItem) {
		hStartItem = GetRootItem();
	}
	hTreeItem = getTrueNextItem(hStartItem);
	while(hTreeItem != hStartItem) {
		m_hSerchItem = hTreeItem;
		data = getData(hTreeItem);
		data.m_strData = data.m_strData + data.m_strMacro;//マクロも検索対象にする
		if(!hTreeItem)	break;
		//タイトル検索
		if(nKind == SERCH_KIND_TITLE || nKind == SERCH_KIND_ALL) {
			if(nLogic == SERCH_LOGIC_AND)	isTitleFound = true;
			else							isTitleFound = false;
			for(keyIT = m_keyVector.begin(); keyIT != m_keyVector.end(); keyIT++) {
				//AND検索
				if(data.m_strTitle.Find(*keyIT) == -1 && nLogic == SERCH_LOGIC_AND) {
					isTitleFound = false;
					break;
				}
				//OR検索
				else if(data.m_strTitle.Find(*keyIT) != -1 && nLogic == SERCH_LOGIC_OR) {
					isTitleFound = true;
					break;
				}
			}
			isFound = isTitleFound;
			if(isFound) break;
		}
		//データ検索
		if((nKind == SERCH_KIND_TEXT || nKind == SERCH_KIND_ALL) && !isFound) {
			if(nLogic == SERCH_LOGIC_AND)	isDataFound = true;
			else							isDataFound = false;
			for(keyIT = m_keyVector.begin(); keyIT != m_keyVector.end(); keyIT++) {
				//AND検索
				if(data.m_strData.Find(*keyIT) == -1 && nLogic == SERCH_LOGIC_AND) {
					isDataFound = false;
					break;
				}
				//OR検索
				else if(data.m_strData.Find(*keyIT) != -1 && nLogic == SERCH_LOGIC_OR) {
					isDataFound = true;
					break;
				}
			}
			isFound = isDataFound;
			if(isFound) break;
		}
		hTreeItem = getTrueNextItem(hTreeItem);
	}
	if(isFound && hTreeItem) {
		SelectItem(hTreeItem);//見つかったら選択
	}
	else hTreeItem = NULL;
	return hTreeItem;
}

//---------------------------------------------------
//関数名	HTREEITEM serchTitle(HTREEITEM hStartItem,CString strKey)
//機能		タイトル検索(前方一致)
//---------------------------------------------------
HTREEITEM CCharu3Tree::serchTitle(HTREEITEM hStartItem,CString strKey,int isLower)
{
	HTREEITEM hRetItem = NULL;

	if(!hStartItem) return NULL;
	list<STRING_DATA>::iterator it;
	STRING_DATA data;
	HTREEITEM hItem = hStartItem;
	hItem = getTrueNextItem(hItem);
	while(hItem && hStartItem != hItem) {
		data = getData(hItem);
		if(isLower) data.m_strTitle.MakeLower();
		if(data.m_strTitle.Find(strKey) == 0) {//発見
			hRetItem = hItem;
			break;
		}
		hItem = getTrueNextItem(hItem);
	}
	if(hStartItem == hItem)	hRetItem = hItem;

	return hRetItem;
}
//---------------------------------------------------
//関数名	getTrueNextItem(HTREEITEM hTreeItem)
//機能		次のアイテムを取得(色々処理してます)
//---------------------------------------------------
HTREEITEM CCharu3Tree::getTrueNextItem(HTREEITEM hTreeItem)
{
	HTREEITEM hRetTreeItem,hParentItem;

	if(!hTreeItem) {		//ルートを設定
		hRetTreeItem = GetRootItem();
	}
	else if(!GetChildItem(hTreeItem)) {//子供がいるか調べる
		hRetTreeItem = GetNextSiblingItem(hTreeItem);//兄弟ハンドルを取得
		if(!hRetTreeItem) {//兄弟がもういない時は親の兄弟
			hParentItem = hTreeItem;
			do {
				hParentItem = GetParentItem(hParentItem );
				if(!hParentItem) {
					hRetTreeItem = NULL;
					break;
				}
				hRetTreeItem = GetNextSiblingItem(hParentItem);//兄弟ハンドルを取得
			}while(hParentItem != GetRootItem() && !hRetTreeItem);
		}
	}
	else {
		hRetTreeItem = GetChildItem(hTreeItem);//子供のハンドルを取得
	}
	if(!hRetTreeItem){
		hRetTreeItem = GetRootItem();//再ループで必要なので戻る
	}
	return hRetTreeItem;
}

//---------------------------------------------------
//関数名	getLastVisibleItem()
//機能		最後の可視アイテムを取得
//---------------------------------------------------
HTREEITEM CCharu3Tree::getLastVisibleItem()
{
	HTREEITEM hRetTreeItem;
	HTREEITEM hTreeItemTmp;
	hRetTreeItem = GetRootItem();
	while(hRetTreeItem) {
		hTreeItemTmp = hRetTreeItem;
		hRetTreeItem = GetNextVisibleItem(hRetTreeItem);//最後の可視アイテムを取得
	}
	if(hTreeItemTmp)	hRetTreeItem = hTreeItemTmp;
	return hRetTreeItem;
}

//---------------------------------------------------
//関数名	allIconCheck()
//機能		全てのアイコンを検査して変更する
//---------------------------------------------------
void CCharu3Tree::allIconCheck()
{
	HTREEITEM hTreeItem;
	char cOrg;
	STRING_DATA data;
	hTreeItem = GetRootItem();
	int nSize = m_MyStringList.size();
	for(int i = 0; i < nSize; hTreeItem = getTrueNextItem(hTreeItem),i++) {
		if(hTreeItem) {
			data = getData(hTreeItem);
			cOrg = data.m_cIcon;
			data.m_cIcon = decideIcon(data.m_strData);
			if(cOrg != data.m_cIcon) editData(hTreeItem,data);
		}
	}
}


//---------------------------------------------------
//関数名	decideIcon(CString strData)
//機能		アイコンを決定
//---------------------------------------------------
char CCharu3Tree::decideIcon(CString strData)
{
	char cIcon = -1;

	//キーマクロ
	if(strData.Find(_T("<charu2MACRO_KEY>"),0) > -1 || strData.Find(_T("<charuMACRO_KEY>"),0) > -1){
			cIcon = KIND_KEY;
	}
	//マクロが含まれている
	else if(strData.Find(_T("<charu2MACRO>"),0) > -1 || strData.Find(_T("<charuMACRO>"),0) > -1) {
		//プラグイン
		if(strData.Find(_T("$PLUG-IN<"),0) > -1){
			cIcon = KIND_PLUG;
		}
		//シェル
		else if(strData.Find(_T("$SHELL<"),0) > -1){
			cIcon = KIND_EXE;
		}
		//関連付け
		else if(strData.Find(_T("$RELATE<"),0) > -1){
			cIcon = KIND_RELATE;
		}
		//選択テキスト
		else if(strData.Find(_T("$SEL"),0) > -1){
			cIcon = KIND_SELECT;
		}
		//クリップボード
		else if(strData.Find(_T("$CLIP"),0) > -1){
			cIcon = KIND_CLIP;
		}
		//日付
		else if(strData.Find(_T("$Y"),0) > -1 || strData.Find(_T("$y"),0) > -1 || strData.Find(_T("$WH"),0) > -1
			|| strData.Find(_T("$MM"),0) > -1 || strData.Find(_T("$M"),0) > -1 || strData.Find(_T("$D"),0) > -1
			|| strData.Find(_T("$ddd"),0) > -1 || strData.Find(_T("$dd"),0) > -1 || strData.Find(_T("$d"),0) > -1
			|| strData.Find(_T("$H"),0) > -1 || strData.Find(_T("$h"),0) > -1 || strData.Find(_T("$m"),0) > -1
			|| strData.Find(_T("$s"),0) > -1 || strData.Find(_T("$ampm"),0) > -1 || strData.Find(_T("$AMPM"),0) > -1){
			cIcon = KIND_DATE;
		}
	}
	return cIcon;
}


//---------------------------------------------------
//関数名	makeTitle(CString strData)
//機能		保存文字列からタイトルを作る
//---------------------------------------------------
CString CCharu3Tree::makeTitle(CString strData,int nTitleLength)
{
	CString strBuff;
	strData = strData.Left(256);
	strData.Replace(_T("　"),_T(""));
	strData.Replace(_T("\r\n"),_T(""));
	strData.Replace(_T("\r"),_T(""));
	strData.Replace(_T("\n"),_T(""));
	strData.Replace(_T("\t"),_T(" "));
	UStringWork::compressionSpace((TCHAR*)(LPCTSTR)strData);
	strData.TrimLeft();
	strData.TrimRight();
	if(nTitleLength > strData.GetLength())	strBuff = strData;
	else	strBuff = strData.Left(nTitleLength);

	return strBuff;
}


//---------------------------------------------------
//関数名	getOneTimeText(bool isFirst)
//機能		最初か最後の一時項目を取得
//---------------------------------------------------
HTREEITEM CCharu3Tree::getOneTimeText(int nType)
{
	CString strRet;
	STRING_DATA data;
	HTREEITEM hRet = NULL;

	//先入れ先出しの場合 FIFO 一番下の一時項目を探す
	int nSize = m_MyStringList.size();
	HTREEITEM hTreeItem = GetRootItem();
	for(int i = 0; i < nSize; hTreeItem = GetNextItem(hTreeItem,TVGN_NEXT),i++) {
		if(hTreeItem) {
			data = getData(hTreeItem);
			if(data.m_cKind & KIND_ONETIME) {
				//デバッグログ処理
				if(theApp.m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("getOneTimeText \"%s\" %s %d\n"),data.m_strTitle,data.m_strData,data.m_cKind);
					CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}

				strRet = data.m_strData;
				hRet = hTreeItem;
				if(nType == 2) break;
			}
		}
	}
	return hRet;
}

//---------------------------------------------------
//関数名	getDataOption(CString strData,CString strKind)
//機能		データ補助設定を解読
//---------------------------------------------------
int CCharu3Tree::getDataOption(CString strData,CString strKind)
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
			else if(strBuff == _T( "")) nRet = -1;
			else nRet = _ttoi(strBuff.GetBuffer(strBuff.GetLength()));
		}
		else nRet = -1;
		//デバッグログ処理
		if(theApp.m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("getDataOption \"%s\" %d\n"),strKind,nRet);
			CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
	}
	else nRet = -1;

	return nRet;
}

//---------------------------------------------------
//関数名	getDataOptionHex(CString strData,CString strKind)
//機能		データ補助設定を解読
//---------------------------------------------------
DWORD CCharu3Tree::getDataOptionHex(CString strData,CString strKind)
{
	DWORD dwRet = -1;
	int nFound,nFoundEnd;
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
			_stscanf(strBuff,_T("%x"),&dwRet);
		}
		//デバッグログ処理
		if(theApp.m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("getDataOptionHex \"%s\" 0x%x\n"),strKind,dwRet);
			CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
	}

	return dwRet;
}

//---------------------------------------------------
//関数名	getDataOptionStr(CString strData,CString strKind)
//機能		データ補助設定を解読
//---------------------------------------------------
CString CCharu3Tree::getDataOptionStr(CString strData,CString strKind)
{
	int nFound,nFoundEnd;
	CString strBuff,strRet,strDataCopy;
	strRet = _T("");

	strDataCopy = strData;
	strData.MakeLower();
	nFound = strData.Find(strKind);
	if(nFound >= 0) {
		nFound = strData.Find(_T("="),nFound);
		if(nFound >= 0) {
			nFoundEnd = strData.Find(_T("\n"),nFound);
			if(nFoundEnd <= 0) nFoundEnd = strData.GetLength();
			strBuff = strDataCopy.Mid(nFound+1,nFoundEnd - (nFound+1));
			strBuff.TrimRight();
			strBuff.TrimLeft();
			strRet = strBuff;
		}
		//デバッグログ処理
		if(theApp.m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("getDataOptionHex \"%s\" %s\n"),strKind,strRet);
			CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
	}
	return strRet;
}

//---------------------------------------------------
//関数名	addDataToRecordFolder(STRING_DATA data)
//機能		履歴処理をする
//---------------------------------------------------
void CCharu3Tree::addDataToRecordFolder(STRING_DATA data,CString strClipBkup)
{
	int nSize = m_MyStringList.size(),nRirekiCount,nIsLock,i,nDoDuplication;
	int nNumber = *m_nRecNumber;
	list<STRING_DATA>::iterator it;

	STRING_DATA parentData;
	HTREEITEM hTreeItem,hStart = NULL;
	//デバッグログ処理
	if(theApp.m_ini.m_nDebug) {
		CString strText;
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,_T("addDataToRecordFolder\n"),_ME_NAME_,__LINE__);
		strText.Format(_T("hRootItem:%d TreeSize:%d\n"),GetRootItem(),nSize);
		CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
//	hTreeItem = (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0);
	for(hTreeItem = GetRootItem(),i = 0; i < nSize && hTreeItem && hTreeItem != hStart;i++,hTreeItem = getTrueNextItem(hTreeItem)) {
		parentData = getData(hTreeItem,&it);
		hStart = GetRootItem();
		//履歴フォルダか判別
		if(parentData.m_cKind & KIND_RIREKI) {
			nDoDuplication = getDataOption(parentData.m_strMacro,_T("duplicationcheck"));
			if((nDoDuplication && strClipBkup != data.m_strData) || !nDoDuplication) {
				if(*m_nRecNumber == nNumber)	nNumber++;
				//デバッグログ処理
				if(theApp.m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("number:%d handle:%d title:%s ID:%d\n"),i,hTreeItem,parentData.m_strTitle,parentData.m_nMyID);
					CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}
				nIsLock = getDataOption(parentData.m_strMacro,_T("lock"));//履歴種別を取得
				if(!nIsLock)	data.m_cKind = KIND_ONETIME;
				else			data.m_cKind = KIND_LOCK;

				nRirekiCount = getDataOption(parentData.m_strMacro,_T("count"));//履歴数を取得
				if(nRirekiCount <= 0) nRirekiCount = 2;
				int nDeleteCount = nRirekiCount;
				deleteExcessChildren(GetChildItem(hTreeItem),&nDeleteCount);//余計な子を消す

				int nTitleLength = getDataOption(parentData.m_strMacro,_T("titlelen"));//タイトルの文字数
				CString strTitle;
				if(nTitleLength > 0 && nTitleLength < 256) {
					strTitle = makeTitle(data.m_strData,nTitleLength);
				}
				else
					strTitle = makeTitle(data.m_strData);
				data.m_strTitle = strTitle;

				if(getDataOption(parentData.m_strMacro,_T("number"))) {//番号をつけるかどうか
					data.m_strTitle.Format(_T("%02d : %s"),*m_nRecNumber,strTitle);
					if(nNumber > nRirekiCount) nNumber = 0;
				}

				//階層履歴処理
				nRirekiCount = getDataOption(parentData.m_strMacro,_T("classhistory"));//階層履歴数を取得
				if(nRirekiCount > 0) {
					classHistoryFolder(hTreeItem,nRirekiCount);
				}
				//デバッグログ処理
				if(theApp.m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("addDataToRecordFolder \"%s\" %s %d\n"),data.m_strTitle,data.m_strData,nNumber);
					CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}

				addData(hTreeItem,data,true,true);//追加
				nSize = m_MyStringList.size();
			}
		}
	}
	*m_nRecNumber = nNumber;
}

//---------------------------------------------------
//関数名	classHistoryFolder(HTREEITEM hTreeItem)
//機能		階層履歴処理
//---------------------------------------------------
void CCharu3Tree::classHistoryFolder(HTREEITEM hTreeItem,int nRirekiCount)
{
	int nChildCount = getChildCount(hTreeItem,true);//履歴フォルダ直下のアイテム数を取得
	if(nChildCount >= nRirekiCount) {//階層履歴数を超えてたら
		HTREEITEM hFirstFolder = getFirstFolder(hTreeItem);
		if(!hFirstFolder || (getChildCount(hFirstFolder) >= nRirekiCount)) {//階層履歴数を超えてるか、フォルダが無い場合は新しいフォルダを作る
			if(hFirstFolder) {//挿入フォルダ位置を決める
				hFirstFolder = GetNextItem(hFirstFolder,TVGN_PREVIOUS);
				if(!hFirstFolder) hFirstFolder = hTreeItem;
			}
			else {//最初の階層フォルダの場合 最後の子アイテムを取得
				hFirstFolder = getLastChild(hTreeItem);
			}
			STRING_DATA data;
			initStringData(&data);

			data.m_cKind = KIND_FOLDER;
			data.m_strData = "";
			data.m_strTitle.LoadString(APP_INF_CLASS_HISTORY);
			addData(hFirstFolder,data);//階層履歴フォルダを作成
			hFirstFolder = getFirstFolder(hTreeItem);//改めてフォルダ位置を取得
		}
		if(hFirstFolder) {//あぶれ分を移動
			HTREEITEM hLastChild = getLastChild(hTreeItem);

			//データを取得
			STRING_DATA data,dataTarget;
			data       = getData(hLastChild);
			dataTarget = getData(hFirstFolder);

			//元データを取得
			TV_ITEM TreeCtrlItem;
			ZeroMemory(&TreeCtrlItem,sizeof(TreeCtrlItem));
			TreeCtrlItem.hItem = hLastChild;
			TreeCtrlItem.mask = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			this->GetItem(&TreeCtrlItem);

			//追加データを作成
			TV_INSERTSTRUCT AddTreeCtrlItem;

			ZeroMemory(&AddTreeCtrlItem,sizeof(AddTreeCtrlItem));
			AddTreeCtrlItem.item.mask = TreeCtrlItem.mask;
			AddTreeCtrlItem.item.iImage = TreeCtrlItem.iImage;
			AddTreeCtrlItem.item.iSelectedImage = TreeCtrlItem.iSelectedImage;
			AddTreeCtrlItem.item.pszText = data.m_strTitle.GetBuffer(data.m_strTitle.GetLength());
			AddTreeCtrlItem.item.lParam = TreeCtrlItem.lParam;
			AddTreeCtrlItem.hInsertAfter = TVI_FIRST;
			AddTreeCtrlItem.hParent = hFirstFolder;
	
			data.m_nParentID = dataTarget.m_nMyID;
			editData(hLastChild,data);
			InsertItem(&AddTreeCtrlItem);
			checkOut(hLastChild);
			DeleteItem(hLastChild);
		}
	}
}

//---------------------------------------------------
//関数名	HTREEITEM getFirstFolder(HTREEITEM hStartItem)
//機能		最初のフォルダを探す
//---------------------------------------------------
HTREEITEM CCharu3Tree::getFirstFolder(HTREEITEM hStartItem)
{
	if(!hStartItem) return NULL;
	list<STRING_DATA>::iterator it;
	HTREEITEM hItem = hStartItem;
	hItem = GetChildItem(hItem);
	do {
		getData(hItem,&it);
		STRING_DATA data = *it;
		if(it->m_cKind & KIND_FOLDER_ALL) 	break;
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}while(hItem);
	return hItem;
}

//---------------------------------------------------
//関数名	HTREEITEM getLastChild(HTREEITEM hStartItem)
//機能		子アイテムの最後を探す
//---------------------------------------------------
HTREEITEM CCharu3Tree::getLastChild(HTREEITEM hStartItem)
{
	if(!hStartItem) return NULL;
	list<STRING_DATA>::iterator it;
	HTREEITEM hItem = hStartItem,hPrevItem;
	hItem = GetChildItem(hItem);
	do {
		getData(hItem,&it);
		if(it->m_cKind & KIND_DATA_ALL) hPrevItem = hItem;
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}while(hItem);
	return hPrevItem;
}

//---------------------------------------------------
//関数名	deleteExcessChildren(HTREEITEM hTreeItem,int nCount)
//機能		余計な子の数を消す
//---------------------------------------------------
void CCharu3Tree::deleteExcessChildren(HTREEITEM hTreeItem,int *nCount)
{
	if(!hTreeItem) return;
	list<STRING_DATA>::iterator it;
	HTREEITEM hPrevItem;
	do {
		getData(hTreeItem,&it);
		if(ItemHasChildren(hTreeItem)) {
			deleteExcessChildren(GetChildItem(hTreeItem),nCount);
		}
		hPrevItem = hTreeItem;
		hTreeItem = GetNextItem(hTreeItem,TVGN_NEXT);
		if(*nCount <= 1 && !ItemHasChildren(hPrevItem)) {//子を消していきます
			deleteData(hPrevItem);
		}
		else if(it->m_cKind & KIND_DATA_ALL)	(*nCount)--;
	}while(hTreeItem);
	return;
}

//---------------------------------------------------
//関数名	getChildCount(HTREEITEM hTreeItem)
//機能		子の数を数える
//---------------------------------------------------
int CCharu3Tree::getChildCount(HTREEITEM hTreeItem,bool isBrotherOnly)
{
	int nChildren = 0;
	if(hTreeItem) hTreeItem = GetChildItem(hTreeItem);
	if(!hTreeItem) return nChildren;
	list<STRING_DATA>::iterator it;
	do {
		getData(hTreeItem,&it);
		if(!isBrotherOnly && ItemHasChildren(hTreeItem)) {
			nChildren += getChildCount(hTreeItem);
		}
		if(it->m_cKind & KIND_DATA_ALL)	nChildren++;
		hTreeItem = GetNextItem(hTreeItem,TVGN_NEXT);
	}while(hTreeItem);



	return nChildren;
}

//---------------------------------------------------
//関数名	moveFolderTop(HTREEITEM hTreeItem)
//機能		データをフォルダの先頭に移動
//---------------------------------------------------
HTREEITEM CCharu3Tree::moveFolderTop(HTREEITEM hTreeItem)
{
	HTREEITEM hRet = NULL;
	if(!hTreeItem) return hRet;

	STRING_DATA dataF;
	HTREEITEM hParent = GetParentItem(hTreeItem);

	if(!hParent) hParent = GetRootItem();
	dataF = getData(hTreeItem);

	//ツリーデータを取得
	TV_ITEM TreeCtrlItemFrom;
	ZeroMemory(&TreeCtrlItemFrom,sizeof(TreeCtrlItemFrom));
	TreeCtrlItemFrom.hItem = hTreeItem;
	TreeCtrlItemFrom.mask = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	GetItem(&TreeCtrlItemFrom);

	//追加データを作成
	TV_INSERTSTRUCT AddTreeCtrlItem;

	ZeroMemory(&AddTreeCtrlItem,sizeof(AddTreeCtrlItem));
	AddTreeCtrlItem.hInsertAfter = TVI_FIRST;
	AddTreeCtrlItem.item.mask = TreeCtrlItemFrom.mask;
	AddTreeCtrlItem.item.iImage = TreeCtrlItemFrom.iImage;
	AddTreeCtrlItem.item.iSelectedImage = TreeCtrlItemFrom.iSelectedImage;
	AddTreeCtrlItem.item.pszText = dataF.m_strTitle.GetBuffer(dataF.m_strTitle.GetLength());
	AddTreeCtrlItem.item.lParam = TreeCtrlItemFrom.lParam;
	AddTreeCtrlItem.hParent = hParent;//フォルダ

	//同じデータを挿入
	hRet = InsertItem(&AddTreeCtrlItem);
	checkOut(hTreeItem);
	DeleteItem(hTreeItem);
	return hRet;
}

//---------------------------------------------------
//関数名	removeCheck()
//機能		チェックを外す
//---------------------------------------------------
void CCharu3Tree::removeCheck()
{
	int nSize = m_MyStringList.size(),i;
	HTREEITEM hTreeItem;
	for(hTreeItem = GetRootItem(),i = 0; i < nSize && hTreeItem;i++,hTreeItem = getTrueNextItem(hTreeItem)) {
		SetCheck(hTreeItem,false);
	}
}

//---------------------------------------------------
//関数名	checkItem(HTREEITEM hItem)
//機能		データをチェックする
//---------------------------------------------------
void CCharu3Tree::checkItem(HTREEITEM hItem)
{
	ModifyStyle(NULL,TVS_CHECKBOXES,NULL);
	if(ItemHasChildren(hItem)) {
		if(GetCheck(hItem)) {
			checkFolder(hItem,false,&m_ltCheckItems);
		}
		else {
			checkFolder(hItem,false,&m_ltCheckItems);
			checkFolder(hItem,true,&m_ltCheckItems);
		}
	}
	else if(hItem) {
		SetCheck(hItem,!GetCheck(hItem));
		if(GetCheck(hItem)) {//リストに追加
			m_ltCheckItems.insert(m_ltCheckItems.end(),hItem);
		}
		else {//リストから削除
			list<HTREEITEM>::iterator it;
			for(it = m_ltCheckItems.begin(); it != m_ltCheckItems.end(); it++) {
				if(*it == hItem) {
					m_ltCheckItems.erase(it);
					break;
				}
			}
		}
	}
}

//---------------------------------------------------
//関数名	checkOut(HTREEITEM hItem)
//機能		チェックを外す
//---------------------------------------------------
void CCharu3Tree::checkOut(HTREEITEM hItem)
{
	if(ItemHasChildren(hItem)) {
		checkFolder(hItem,false,&m_ltCheckItems);
	}
	else if(hItem) {
		SetCheck(hItem,false);
		list<HTREEITEM>::iterator it;
		for(it = m_ltCheckItems.begin(); it != m_ltCheckItems.end(); it++) {
			if(*it == hItem) {
				m_ltCheckItems.erase(it);
				break;
			}
		}
	}
}

//---------------------------------------------------
// CCharu3Tree メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//機能		クリエイト処理
//---------------------------------------------------
BOOL CCharu3Tree::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL isRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	return isRet;
}

//---------------------------------------------------
//関数名	OnMouseMove(UINT nFlags, CPoint point)
//機能		マウスカーソル移動
//---------------------------------------------------
void CCharu3Tree::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_isDrag) {
		CPoint ptTree(point);

		MapWindowPoints(this,&ptTree,1);
		m_pDragImage->DragMove(point);
		m_pDragImage->DragLeave(this);
		UINT uHitTest = TVHT_ONITEM;
		m_hDragTarget = this->HitTest(point,&uHitTest);
		if(m_hDragTarget) {
			STRING_DATA data;
			data = getData(m_hDragTarget);
			//フォルダで子供がいなければ選択、それ以外は挿入マーク
			if(data.m_cKind & KIND_FOLDER_ALL && !GetChildItem(m_hDragTarget)) {
				SetInsertMark(NULL);
				SelectItem(m_hDragTarget);
			}
			else {
				SetInsertMark(m_hDragTarget,TRUE);
				SelectItem(m_hDragTarget);
			}
		}
		else {
			if(point.y <= 0) {
				m_hDragTarget = GetFirstVisibleItem();
				m_hDragTarget = GetNextItem(m_hDragTarget,TVGN_PREVIOUSVISIBLE);
			}
			m_pDragImage->DragLeave(this);
			SelectItem(m_hDragTarget);
			return;
		}
		this->UpdateWindow();
		m_pDragImage->DragEnter(this, point);
	}
	else {
		CPoint ptTree(point);

		MapWindowPoints(this,&ptTree,1);
		m_pDragImage->DragMove(point);
		m_pDragImage->DragLeave(this);
		UINT uHitTest = TVHT_ONITEMLABEL ;
		HTREEITEM hTarget = this->HitTest(point,&uHitTest);
		if(hTarget && m_hPrevTarget != hTarget && uHitTest & 0x06) {
			::PostMessage(this->GetParent()->m_hWnd,WM_TIPS_CHANGE,(unsigned int)hTarget,0);
			m_hPrevTarget = hTarget;
		}
		else if(m_hPrevTarget != hTarget){
			::PostMessage(this->GetParent()->m_hWnd,WM_TIPS_CHANGE,NULL,0);
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

//---------------------------------------------------
//関数名	OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
//機能		ドラッグ開始
//---------------------------------------------------
void CCharu3Tree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	m_hDragItem = pNMTreeView->itemNew.hItem;

	CPoint ptImage,ptHotSpot;

	RECT rSelItem;
	this->GetItemRect(m_hDragItem, &rSelItem,NULL);
	m_pDragImage = this->CreateDragImage(m_hDragItem);

	m_pDragImage->DragShowNolock(TRUE);
	ptHotSpot.x = pNMTreeView->ptDrag.x + rSelItem.right;
	ptHotSpot.y = pNMTreeView->ptDrag.y + rSelItem.bottom;
	m_pDragImage->SetDragCursorImage(0,ptHotSpot);
	CPoint ptCursorPos;
	GetCursorPos(&ptCursorPos);
	m_pDragImage->BeginDrag(0,CPoint(0,0));
	m_pDragImage->DragEnter(this,pNMTreeView->ptDrag);
	SetCapture();

//	m_nDragPosY = pNMTreeView->ptDrag.y;
	m_isDrag = true;

	*pResult = 0;
}

//---------------------------------------------------
//関数名	OnLButtonUp(UINT nFlags, CPoint point) 
//機能		ボタンを離した
//---------------------------------------------------
void CCharu3Tree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_isDrag) return;

	m_isDrag = false;
	m_pDragImage->DragLeave(this);
	m_pDragImage->EndDrag();
	delete m_pDragImage;
	ReleaseCapture();
	SetInsertMark(NULL);

	//アイテムの移動処理をする
	if(m_hDragTarget && m_hDragTarget != m_hDragItem) {
		//自分の子孫には移動できない
		if(!checkMyChild(m_hDragItem,m_hDragTarget)) {
			AfxGetApp()->DoWaitCursor(1);

			//データを取得
			STRING_DATA data,dataTarget;
			data       = getData(m_hDragItem);
			dataTarget = getData(m_hDragTarget);

			//ツリーデータを取得
			TV_ITEM TreeCtrlItem;
			ZeroMemory(&TreeCtrlItem,sizeof(TreeCtrlItem));
			TreeCtrlItem.hItem = m_hDragItem;
			TreeCtrlItem.mask = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			this->GetItem(&TreeCtrlItem);

			//追加データを作成
			TV_INSERTSTRUCT AddTreeCtrlItem;

			ZeroMemory(&AddTreeCtrlItem,sizeof(AddTreeCtrlItem));
			AddTreeCtrlItem.hInsertAfter = m_hDragTarget;
			AddTreeCtrlItem.item.mask = TreeCtrlItem.mask;
			AddTreeCtrlItem.item.iImage = TreeCtrlItem.iImage;
			AddTreeCtrlItem.item.iSelectedImage = TreeCtrlItem.iSelectedImage;
			AddTreeCtrlItem.item.pszText = data.m_strTitle.GetBuffer(data.m_strTitle.GetLength());
			AddTreeCtrlItem.item.lParam = TreeCtrlItem.lParam;

			//親を設定
			if(dataTarget.m_cKind & KIND_FOLDER_ALL && !ItemHasChildren(m_hDragTarget)) {
				AddTreeCtrlItem.hInsertAfter = TVI_FIRST;
				AddTreeCtrlItem.hParent = m_hDragTarget;//フォルダ
				data.m_nParentID = dataTarget.m_nMyID;
				editData(m_hDragItem,data);
			}
			else {
				AddTreeCtrlItem.hParent = GetParentItem(m_hDragTarget);//それ以外
				data.m_nParentID = dataTarget.m_nParentID;
				editData(m_hDragItem,data);
			}
			//データを移動
			if(data.m_cKind & KIND_FOLDER_ALL && ItemHasChildren(m_hDragItem)) {
				copyChildren(m_hDragItem,InsertItem(&AddTreeCtrlItem));
				checkOut(m_hDragItem);
				clearFolder(GetChildItem(m_hDragItem));
				DeleteItem(m_hDragItem);
			}
			else {
				SelectItem(InsertItem(&AddTreeCtrlItem));
				checkOut(m_hDragItem);
				DeleteItem(m_hDragItem);
			}

			AfxGetApp()->DoWaitCursor(0);
		}
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

//---------------------------------------------------
//関数名	OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
//機能		フォルダ開閉処理
//---------------------------------------------------
void CCharu3Tree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	list<STRING_DATA>::iterator it;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if(hItem) {
		getData(hItem,&it);
		//閉じる
		if(pNMTreeView->action == 1)
			it->m_cKind = it->m_cKind & (FOLDER_OPEN ^ 0xff); 
		//開く
		else if(pNMTreeView->action == 2)
			it->m_cKind = it->m_cKind | FOLDER_OPEN;
	}
	*pResult = 0;
}

void CCharu3Tree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	list<STRING_DATA>::iterator it;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if(hItem) {
		getData(hItem,&it);
		setSelectID(it->m_nMyID);
	}
	*pResult = 0;
}

void CCharu3Tree::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if(lpwndpos) {
		CTreeCtrl::OnWindowPosChanging(lpwndpos);
	}
}

//---------------------------------------------------
//関数名	setScrollBar
//機能		スクロールバー設定
//---------------------------------------------------
void CCharu3Tree::setScrollBar()
{
	//スクロールバーの状態を変える時だけ動くようにしとくように

	
	int nPageH,nPageV;
	SCROLLINFO infoHorz,infoVert;
	static SCROLLINFO stInfoHorz,stInfoVert;
	infoHorz.cbSize = sizeof(SCROLLINFO);
	infoVert.cbSize = sizeof(SCROLLINFO);
	if(theApp.m_ini.m_visual.m_nScrollbar == SB_BOTH) {//両方付ける
		this->GetScrollInfo(SB_HORZ,&infoHorz,SIF_ALL);
		nPageH = infoHorz.nPage;
		this->GetScrollInfo(SB_VERT,&infoVert,SIF_ALL);
		nPageV = infoVert.nPage;

//		if(memcmp(&infoHorz,&stInfoHorz,sizeof(SCROLLINFO)) != 0 || memcmp(&infoVert,&stInfoVert,sizeof(SCROLLINFO)) != 0) {
			if(nPageH > 1 && nPageV > 1) this->ShowScrollBar(SB_BOTH,TRUE);
			if(nPageH > 1)	this->ShowScrollBar(SB_HORZ,TRUE);
			if(nPageV > 1)	this->ShowScrollBar(SB_VERT,TRUE);

			stInfoHorz = infoHorz;
			stInfoVert = infoVert;
//		}
	}
	//水平だけ
	else if(theApp.m_ini.m_visual.m_nScrollbar == SB_HORZ) {
		this->GetScrollInfo(SB_HORZ,&infoHorz,SIF_ALL);
		nPageH = infoHorz.nPage;
//		if(memcmp(&infoHorz,&stInfoHorz,sizeof(SCROLLINFO)) != 0) {
			if(nPageH > 1)	this->ShowScrollBar(SB_HORZ,TRUE);
			this->ShowScrollBar(SB_VERT,FALSE);

			stInfoHorz = infoHorz;
//		}
	}
	//垂直だけ
	else if(theApp.m_ini.m_visual.m_nScrollbar == SB_VERT) {
		this->GetScrollInfo(SB_VERT,&infoVert,SIF_ALL);
		nPageV = infoVert.nPage;
//		if(memcmp(&infoVert,&stInfoVert,sizeof(SCROLLINFO)) != 0) {
			if(nPageV > 1)	this->ShowScrollBar(SB_VERT,TRUE);
			this->ShowScrollBar(SB_HORZ,FALSE);

			stInfoVert = infoVert;
//		}
	}
	else{
		this->ShowScrollBar(SB_BOTH,FALSE);
	}

}

//---------------------------------------------------
//関数名	OnMouseWheel
//機能		ホイール処理
//---------------------------------------------------
BOOL CCharu3Tree::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(theApp.m_ini.m_visual.m_nScrollbar != SB_BOTH && theApp.m_ini.m_visual.m_nScrollbar != SB_VERT) {
		if(zDelta < 0)
			SendMessage(WM_KEYDOWN,VK_DOWN,0);
		else
			SendMessage(WM_KEYDOWN,VK_UP,0);
	}
	return CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CCharu3Tree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	setScrollBar();
	
	*pResult = 0;
}
