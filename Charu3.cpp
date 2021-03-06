/*----------------------------------------------------------
	Charu3
	アプリケーション用クラスの機能定義	'2002.11.15 (c)C+ Factory
----------------------------------------------------------*/
#include "stdafx.h"
#include "Charu3.h"
#include "MainFrm.h"
#include "AddDialog.h"
#include "OptMainDialog.h"
#include "MyFileDialog.h"
#include "StringWork.h"
#include <MULTIMON.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _ME_NAME_
#undef _ME_NAME_
#endif
#define _ME_NAME_ _T("Charu3.cpp")

//---------------------------------------------------
//関数名	MonitorEnumFunc
//機能		モニター列挙コールバック
//---------------------------------------------------
BOOL CALLBACK MonitorEnumFunc(HMONITOR hMonitor,HDC hdc,LPRECT rect,LPARAM lParam)
{
    MONITORINFOEX MonitorInfoEx;
    static int count=0;

	CArray<RECT,RECT> *pArray;
	pArray = (CArray<RECT,RECT>*)lParam;
    count++;
    MonitorInfoEx.cbSize=sizeof(MonitorInfoEx);
    if (!GetMonitorInfo(hMonitor,&MonitorInfoEx)) {
         return FALSE;
    }
	pArray->Add(*rect);
    return TRUE;
}

//---------------------------------------------------
// CCharu3App
//---------------------------------------------------
BEGIN_MESSAGE_MAP(CCharu3App, CWinApp)
	//{{AFX_MSG_MAP(CCharu3App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_HELP, OnHelp)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_OPTION, OnOption)
	ON_COMMAND(IDM_DATA_SAVE, OnDataSave)
	ON_COMMAND(IDM_ONETIME_CLEAR, OnOnetimeClear)
	ON_COMMAND(IDM_ALL_LOCK, OnAllLock)
	ON_COMMAND(IDM_CHANG_DATA, OnChangData)
	ON_COMMAND(IDM_ADD_DATA, OnAddData)
	ON_COMMAND(IDM_ICON_DECIDE, OnIconDecide)
	ON_COMMAND(IDM_BBS_OPEN, OnBbsOpen)
	ON_COMMAND(IDM_STOCK_STOP, OnStockStop)
	ON_COMMAND(IDM_VISUAL_FILE, OnVisualFile)
	ON_COMMAND(IDM_RESET_TREE, OnResetTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
//関数名	CCharu3App()
//機能		CCharu3Appクラスのコンストラクタ
//---------------------------------------------------
CCharu3App::CCharu3App()// : m_treeDlg(&m_tree)
{
	m_isCloseKey = false;
	m_isStockMode = false;
	m_isCornerPopup = false;
	m_isWordBugClipCheck = false;
	m_nPhase = PHASE_START;
	m_dwDoubleKeyPopTime = 0;
	m_dwDoubleKeyFifoTime = 0;
	m_hSelectItemBkup = NULL;
	m_focusInfo.m_hActiveWnd  = NULL;
	m_focusInfo.m_hFocusWnd = NULL;

	int i,nCount = 0,nSize;

	int stKeycode[] = {	0x08,0x09,0x0c,0x0d,0x10,0x11,0x12,0x13,
						0x14,0x1B,0x20,0x21,0x22,0x23,0x24,0x25,
						0x26,0x27,0x28,0x29,0x2b,0x2c,0x2d,0x2e,
						0x2f,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x90,
						0x91,0x92,0xBB,0xBC,};

	char *stKeyName[] = {"BS","TAB","CLEAR","ENTER","SHIFT","CTRL","ALT","PAUSE",
						"CAPSLOCK","ESC","SPACE","PAGEUP","PAGEDOWN","END","HOME","LEFT",
						"UP","RIGHT","DOWN","SELECT","EXECUTE","PRINTSCREEN","INSERT","DEL",
						"HELP","NUM*","NUM+","NUMSEP","NUM-","NUM.","NUM/","NUMLOCK",
						"SCROLLLOCK","NUM=","SEMICOLON","COMMA",};

	//テンキーの数字
	for(i = 0x60; i <= 0x69; i++,nCount++) {
		m_keyStruct[nCount].nKeyCode = i;
		m_keyStruct[nCount].strName.Format(_T("NUM%d"),i - 0x60);
	}
	//ファンクションキー
	for(i = 0x70; i <= 0x87; i++,nCount++) {
		m_keyStruct[nCount].nKeyCode = i;
		m_keyStruct[nCount].strName.Format(_T("F%d"),i - 0x6f);
	}

	nSize = sizeof(stKeycode) / 4;
	//拡張キー
	for(i = 0; i < nSize; i++,nCount++) {
		m_keyStruct[nCount].nKeyCode = stKeycode[i];
		m_keyStruct[nCount].strName = stKeyName[i];
	}
	m_keyStruct[nCount].nKeyCode = -1;

	m_pTreeDlg = new CMyTreeDialog;
	m_pTree = new CCharu3Tree;
}
//---------------------------------------------------
//関数名	~CCharu3App()
//機能		CCharu3Appクラスのデストラクタ
//---------------------------------------------------
CCharu3App::~CCharu3App()
{
	if(m_pTreeDlg)	delete m_pTreeDlg;
	if(m_pTree)		delete m_pTree;
	if(m_hMutex)	CloseHandle(m_hMutex);
}

//---------------------------------------------------
// 唯一の CCharu3App オブジェクト
//---------------------------------------------------
CCharu3App theApp;

//---------------------------------------------------
//関数名	InitInstance()
//機能		CCharu3App クラスの初期化
//---------------------------------------------------
BOOL CCharu3App::InitInstance()
{
	//重複起動阻止
	LPTSTR pszMutexObjectName = _T("Charu3");			//重複起動防止名
	m_hMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, pszMutexObjectName);
//	HWND hActiveWnd = ::GetForegroundWindow();
	CGeneral::getFocusInfo(&m_focusInfo);

	if(m_hMutex) {
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return FALSE;
	}
	m_hMutex = CreateMutex( FALSE, 0, pszMutexObjectName);

#ifdef _AFXDLL
	Enable3dControls();		// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	//言語リソースを設定
	m_hLangDll = LoadLibrary(_T("c3language.dll"));
	if(m_hLangDll) {
		AfxSetResourceHandle(m_hLangDll);
	}

	//メインフォームの作成
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	// フレームをリソースからロードして作成します
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,	NULL);

	// メイン ウィンドウは最小化して隠しておきます
	pFrame->ShowWindow(SW_MINIMIZE);
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();
	m_pMainFrame = pFrame;

	HWND hWndTop;
	m_hSelfWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);

	m_pTreeDlg->setTree(m_pTree);
    m_pTreeDlg->Create(IDD_POPUP_DIALOG,this->m_pMainWnd);
//	::SetForegroundWindow(hActiveWnd);

	pFrame->setMenu();
	init();//初期化
	CGeneral::setFocusInfo(&m_focusInfo);
//	setAppendKeyInit(m_focusInfo.m_hActiveWnd,&m_keySet);//キー設定を変更

	m_nPhase = PHASE_IDOL;

	pFrame->ShowWindow(SW_MINIMIZE);
	pFrame->ShowWindow(SW_HIDE);
	return TRUE;
}

int CCharu3App::ExitInstance() 
{
	if(m_hLangDll) {
		FreeLibrary(m_hLangDll);
	}
	return CWinApp::ExitInstance();
}
//---------------------------------------------------
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ
//---------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	m_strVersion = ABOUT_NAME;
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSION_NAME, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_WEB, OnWeb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CCharu3App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
void CAboutDlg::OnWeb() 
{
	ShellExecute(NULL,NULL,_T("http://keijiweb.com/"),NULL,NULL,SW_SHOWNORMAL);
	EndDialog(IDOK);
}

//---------------------------------------------------
//関数名	init()
//機能		初期化
//---------------------------------------------------
void CCharu3App::init()
{
	CString StringBuff,StringBuff2;

	//設定クラスを初期化
	m_ini.initialize();

	//一旦初期INIを読む
	StringBuff = m_ini.m_strAppPath + INI_FILE;
	m_ini.setIniFileName(StringBuff);
	m_ini.readAllInitData();
	//設定ファイルの名前を設定
	if(m_ini.m_etc.m_nMulchUser == 1) {
		StringBuff = m_ini.m_strAppPath + m_ini.m_strUserName + "\\" + INI_FILE;
		if (!FILEEXIST(StringBuff)) {
			StringBuff2 = m_ini.m_strAppPath + m_ini.m_strUserName;

			CreateDirectory(StringBuff2,NULL);
			StringBuff2 = m_ini.m_strAppPath + INI_FILE;
			CopyFile(StringBuff2,StringBuff,TRUE);
		}
		m_ini.setIniFileName(StringBuff);
	}

	//設定ファイル読み込み
	m_ini.readAllInitData();
	m_ini.m_strDebugLog = m_ini.m_strAppPath + m_ini.m_strDebugLog;

	m_pTree->setImageList(theApp.m_ini.m_IconSize,theApp.m_ini.m_visual.m_strResourceName,m_ini.m_strAppPath);
	m_pTree->setInitInfo(&m_ini.m_nTreeID,&m_ini.m_nSelectID,&m_ini.m_nRecNumber);//ID初期値を設定

	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("start \"%s\"\n"),ABOUT_NAME);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);

		strText.Format(_T("Windows version %d.%d  %d\n"),m_ini.m_osVersion.dwMajorVersion,m_ini.m_osVersion.dwMinorVersion,m_ini.m_osVersion.dwPlatformId);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);

		strText.Format(_T("ini file name \"%s\"\n"),StringBuff);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	//データ互換プラグインを初期化
	if(!m_pTree->setPlugin(m_ini.m_strRwPluginFolder)){
		m_ini.m_strRwPluginFolder = m_ini.m_strAppPath + "rw_plugin";
		m_pTree->setPlugin(m_ini.m_strRwPluginFolder);
		m_ini.writeEnvInitData();
	}

	//データ読み込み
	if(!m_pTree->loadDataFileDef(m_ini.m_strDataFile,m_ini.m_strPluginName)) {
		if(m_ini.m_etc.m_nMulchUser) {
			m_ini.m_strDataFile = m_ini.m_strAppPath + m_ini.m_strUserName + "\\" + DAT_FILE;
		}
		else {
			m_ini.m_strDataFile = m_ini.m_strAppPath + DAT_FILE;
		}
		m_ini.m_strPluginName = DAT_FORMAT;
		m_ini.writeEnvInitData();

		if(!m_pTree->loadDataFileDef(m_ini.m_strDataFile,m_ini.m_strPluginName)) {
			m_pTree->loadDataFileDef(m_ini.m_strAppPath + DAT_FILE,m_ini.m_strPluginName);
		}
		m_pTree->saveDataFile(m_ini.m_strDataFile,m_ini.m_strPluginName,NULL);	
	}
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("read data file\"%s\" %s\n"),m_ini.m_strDataFile,m_ini.m_strPluginName);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	//クリップボードクラスの初期化 変更検知を設定(メインフレームでメッセージ処理をしてます)
	m_clipbord.getClipboardText(m_strlClipBackup);
	m_clipbord.setParent(this->m_pMainWnd->m_hWnd);
	m_keySet = m_ini.m_key.m_defKeySet;

	TCHAR strKeyLayoutName[256];
	GetKeyboardLayoutName(strKeyLayoutName);
	m_ini.m_keyLayout = LoadKeyboardLayout(strKeyLayoutName,KLF_REPLACELANG);
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("KeyLayoutName \"%s\"\n"),strKeyLayoutName);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	//自己診断タイマーセット
	if(m_ini.m_etc.m_nSelfDiagnosisTime)
		SetTimer(m_pMainWnd->m_hWnd,TIMER_SELF_DIAGNOSIS,m_ini.m_etc.m_nSelfDiagnosisTime,NULL);

	//マウス監視タイマー
	if(m_ini.m_pop.m_nCornerPopup)
		SetTimer(m_pMainWnd->m_hWnd,TIMER_MOUSE,m_ini.m_pop.m_nCornerPopupTime,NULL);

	m_ini.setHookKey(m_hSelfWnd);

	//ホットキーを設定
	registerHotkey();
	setAppendHotKey();
}

//---------------------------------------------------
//関数名	registerHotkey()
//機能		ホットキーを設定する
//---------------------------------------------------
void CCharu3App::registerHotkey()
{
	UINT uMod = 0,uVK = 0;
	switch(m_ini.m_pop.m_nDoubleKeyPOP) {
		case 1:	uMod = MOD_SHIFT;break;
		case 2:	uMod = MOD_CONTROL;break;
		case 3:	uMod = MOD_ALT;break;
		default:
		case 0:
			uMod = m_ini.m_pop.m_uMod_Pouup;
			uVK  = m_ini.m_pop.m_uVK_Pouup;
			break;
	}
	if(!RegisterHotKey(NULL,HOTKEY_POPUP,uMod,uVK)) {//ポップアップキー
		CString strRes;
		strRes.LoadString(APP_MES_FAILURE_HOTKEY);
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CGeneral::writeLog(m_ini.m_strDebugLog,_T("False hotkey setting popup\n"),_ME_NAME_,__LINE__);
		}
//		AfxMessageBox(strRes + _T("Popup Key"),MB_ICONEXCLAMATION,0);
	}

	uMod = 0,uVK = 0;
	switch(m_ini.m_pop.m_nDoubleKeyFIFO) {
		case 1:	uMod = MOD_SHIFT;break;
		case 2:	uMod = MOD_CONTROL;break;
		case 3:	uMod = MOD_ALT;break;
		default:
		case 0:
			uMod = m_ini.m_pop.m_uMod_Fifo;
			uVK  = m_ini.m_pop.m_uVK_Fifo;
			break;
	}
	if(!RegisterHotKey(NULL,HOTKEY_FIFO,uMod,uVK)) {//履歴FIFOキー
		CString strRes;
		strRes.LoadString(APP_MES_FAILURE_HOTKEY);
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CGeneral::writeLog(m_ini.m_strDebugLog,_T("False hotkey setting stuckmode\n"),_ME_NAME_,__LINE__);
		}
//		AfxMessageBox(strRes + _T("Stock mode Key"),MB_ICONEXCLAMATION,0);
	}
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CGeneral::writeLog(m_ini.m_strDebugLog,_T("registerHotkey\n"),_ME_NAME_,__LINE__);
	}
}

//---------------------------------------------------
//関数名	stopHotkey()
//機能		ホットキーを止める
//---------------------------------------------------
void CCharu3App::stopHotkey()
{
	UnregisterHotKey(NULL,HOTKEY_POPUP);
	UnregisterHotKey(NULL,HOTKEY_FIFO);
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CGeneral::writeLog(m_ini.m_strDebugLog,_T("stopHotkey\n"),_ME_NAME_,__LINE__);
	}
}

//---------------------------------------------------
//関数名	setAppendHotKey()
//機能		追加ホットキーを設定
//---------------------------------------------------
void CCharu3App::setAppendHotKey() 
{
	HOT_KEY_CODE keyData;
	list<STRING_DATA>::iterator it;

	m_hotkeyVector.clear();

	CString strKey;
	STRING_DATA data;
	HTREEITEM hRet = NULL;

	if(m_isStockMode)
		RegisterHotKey(NULL,HOTKEY_PASTE,m_keySet.m_uMod_Paste,m_keySet.m_uVK_Paste);//ペーストキー

	int nSize = m_pTree->m_MyStringList.size();
	HTREEITEM hTreeItem = m_pTree->GetRootItem();
	for(int i = 0; i < nSize && hTreeItem; hTreeItem = m_pTree->getTrueNextItem(hTreeItem),i++) {
		if(hTreeItem) {
			data = m_pTree->getData(hTreeItem);
			strKey = m_pTree->getDataOptionStr(data.m_strMacro,EXMACRO_HOT_KEY);
			if(strKey != "") {
				//ホットキー設定を変換
				convHotKeyConf(strKey,&keyData.m_uModKey,&keyData.m_uVkCode,&keyData.m_isDoubleClick);
				keyData.m_strMacroName = EXMACRO_HOT_KEY;
				keyData.m_nDataID = data.m_nMyID;
				keyData.m_hItem = hTreeItem;
				keyData.m_dwDoubleKeyTime = 0;
				m_hotkeyVector.insert(m_hotkeyVector.end(),keyData);//設定アレイに追加
				int nret = RegisterHotKey(NULL,HOT_ITEM_KEY+m_hotkeyVector.size()-1,keyData.m_uModKey,keyData.m_uVkCode);//ホットキーをレジスト
				//デバッグログ処理
				if(m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("setAppendHotKey hotkey \"%s\" %d\n"),strKey,nret);
					CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}
			}
			strKey = m_pTree->getDataOptionStr(data.m_strMacro,EXMACRO_DIRECT_COPY);
			if(strKey != "") {
				//ホットキー設定を変換
				convHotKeyConf(strKey,&keyData.m_uModKey,&keyData.m_uVkCode,&keyData.m_isDoubleClick);
				keyData.m_strMacroName = EXMACRO_DIRECT_COPY;
				keyData.m_nDataID = data.m_nMyID;
				keyData.m_hItem = hTreeItem;
				keyData.m_dwDoubleKeyTime = 0;
				m_hotkeyVector.insert(m_hotkeyVector.end(),keyData);//設定アレイに追加
				int nret = RegisterHotKey(NULL,HOT_ITEM_KEY+m_hotkeyVector.size()-1,keyData.m_uModKey,keyData.m_uVkCode);//ホットキーをレジスト
				//デバッグログ処理
				if(m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("setAppendHotKey directcopy \"%s\" %d\n"),strKey,nret);
					CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}
			}
		}
	}
}
//---------------------------------------------------
//関数名	stopAppendHotKey()
//機能		追加ホットキーをすべて止める
//---------------------------------------------------
void CCharu3App::stopAppendHotKey() 
{
	UnregisterHotKey(NULL,HOTKEY_PASTE);
	int nSize = m_hotkeyVector.size();
	for(int i = 0; i < nSize; i++) {
		UnregisterHotKey(NULL,HOT_ITEM_KEY+i);
	}
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CGeneral::writeLog(m_ini.m_strDebugLog,_T("stopAppendHotKey\n"),_ME_NAME_,__LINE__);
	}
}

//---------------------------------------------------
//関数名	convHotKeyConf()
//機能		文字列からホットキーの設定を解読
//---------------------------------------------------
void CCharu3App::convHotKeyConf(CString strKey,UINT *pMod,UINT *pVK,bool *isDoubleClick)
{
	int nFound;
	CString strVK,strMod;
	strKey.MakeUpper();
	nFound = strKey.ReverseFind('+');
	*isDoubleClick = false;

	if(nFound < 0) {//+で区切られてない場合
		*pMod = NULL;
		strVK = strKey;
		strVK.TrimLeft();
		strVK.TrimRight();
		if(strVK == "SHIFT*2") {
			*pMod = MOD_SHIFT;
		}
		else if(strVK == "CTRL*2") {
			*pMod = MOD_CONTROL;
		}
		else if(strVK == "ALT*2") {
			*pMod = MOD_ALT;
		}
		if(*pMod) {
			*isDoubleClick = true;
			*pVK = NULL;
			return;
		}
	}
	else {//+で区切られてる場合
		strVK  = strKey.Right(strKey.GetLength() - nFound - 1);
		strVK.TrimLeft();
		strVK.TrimRight();

		*pMod = 0;
		if(strKey.Find(_T("SHIFT")) >= 0) {
			*pMod = *pMod | MOD_SHIFT;
		}
		if(strKey.Find(_T("CTRL")) >= 0) {
			*pMod = *pMod | MOD_CONTROL;
		}
		if(strKey.Find(_T("ALT")) >= 0) {
			*pMod = *pMod | MOD_ALT;
		}
	}
	*pVK = getKeycode(strVK.GetBuffer(strVK.GetLength()));
}
//---------------------------------------------------
//関数名	getKeycode(char *szKeyName)
//機能		キー名からコードを得る
//---------------------------------------------------
int CCharu3App::getKeycode(TCHAR *szKeyName)
{
	CString strTmp;
	strTmp = szKeyName;
	strTmp.MakeUpper();

	int i,nRet = 0;

	if(strTmp.Find(_T("0X")) >= 0) 
		_stscanf(szKeyName,_T("0x%2x"),&nRet);
	
	if(!nRet) {
		if(_tcsclen(szKeyName) == 1) {
			SHORT code = VkKeyScanEx(*szKeyName,m_ini.m_keyLayout);
			nRet = code & 0xff;
		}
		else {
			for(i = 0; m_keyStruct[i].nKeyCode > 0 && i < 256; i++) {
				if(m_keyStruct[i].strName == strTmp) {
					nRet = m_keyStruct[i].nKeyCode;
					break;
				}
			}
		}
	}
	return nRet;
}

//---------------------------------------------------
//関数名	setAppendKeyInit(HWND hTopWindow)
//機能		追加キー設定
//---------------------------------------------------
bool CCharu3App::setAppendKeyInit(HWND hTopWindow,COPYPASTE_KEY *keySet)
{
	TCHAR strWindowName[1024];
	*strWindowName = (TCHAR)NULL;
	CString strWinName;
	bool isRet = false;

	if(!hTopWindow || m_hActiveKeyWnd == hTopWindow) return isRet;//前と同じハンドルなら帰る
	if(!GetWindowText(hTopWindow,strWindowName,sizeof(strWindowName))) return isRet;//キャプションを取得

	strWinName = strWindowName;
	*keySet = m_ini.getAppendKeyInit(strWinName);
	if(keySet->m_nMessage <= -1)	*keySet = m_ini.m_key.m_defKeySet;
	m_hActiveKeyWnd = hTopWindow;

	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("setAppendKeyInit %s %d %d %d %d %d\n"),strWinName,keySet->m_uMod_Copy,keySet->m_uVK_Copy,keySet->m_uMod_Paste,keySet->m_uVK_Paste,keySet->m_nMessage);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
	
	isRet = true;
	return isRet;
}

//---------------------------------------------------
//関数名	popupTreeWindow(POINT pos)
//機能		ポップアップを表示
//---------------------------------------------------
void CCharu3App::popupTreeWindow(POINT pos,int nSelect,HTREEITEM hOpenItem)
{
	if(m_nPhase != PHASE_IDOL) return;
	m_nPhase = PHASE_POPUP;
	m_ini.unHookKey();
	stopAppendHotKey();//追加ホットキーを停止

//	CGeneral::getFocusInfo(&m_focusInfo);

	if(m_focusInfo.m_hActiveWnd == this->m_pMainFrame->m_hWnd) {
		m_nPhase = PHASE_IDOL;
		return;
	}

	if(m_isStockMode)	KillTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE);//監視タイマー停止
	//IMEをOFFにする
	if(m_ini.m_pop.m_nAutoImeOff) {
		HIMC hIMC = ImmGetContext(m_pTreeDlg->m_hWnd);
		m_isImeStatus = ImmGetOpenStatus(hIMC);
		if(m_isImeStatus) ImmSetOpenStatus(hIMC,FALSE);
	}
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("popupTreeWindow %d %d %d\n"),pos.x,pos.y,nSelect);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	CGeneral::setAbsoluteForegroundWindow(m_pMainWnd->m_hWnd);//自分をアクティブに設定
	if(nSelect)
		m_pTreeDlg->showWindowPos(pos,m_ini.m_DialogSize,SW_SHOW,true,hOpenItem);//ダイアログを表示
	else
		m_pTreeDlg->showWindowPos(pos,m_ini.m_DialogSize,SW_SHOW,false,hOpenItem);//ダイアログを表示
}

//---------------------------------------------------
//関数名	popupTreeWinMC(HWND hForeground)
//機能		ポップアップをマウスカーソル位置に表示
//---------------------------------------------------
void CCharu3App::popupTreeWinMC(HWND hForeground)
{
	POINT pos;
	GetCursorPos(&pos);
	pos.x -=  m_ini.m_DialogSize.x;
	pos.y -=  m_ini.m_DialogSize.y;
	if(m_ini.m_pop.m_nOutRevice)	reviseWindowPos(&pos);
//	CGeneral::getFocusInfo(&theApp.m_focusInfo,hForeground);
	popupTreeWindow(pos,m_ini.m_pop.m_nSelectSW);//ポップアップ
}
//---------------------------------------------------
//関数名	reviseWindowPos(POINT pos)
//機能		ポップアップ位置を補正
//---------------------------------------------------
void CCharu3App::reviseWindowPos(POINT *pos)
{
	RECT DeskTopSize;
	int nMonCount = GetSystemMetrics(SM_CMONITORS);

	if(nMonCount <= 1) {
		//デスクトップのサイズを取得
		int nDektopWidth = GetSystemMetrics( SM_CXSCREEN);
		int nDesktopHeight = GetSystemMetrics( SM_CYFULLSCREEN );
		//解像度の取得
		int nScreenWidth = GetSystemMetrics( SM_CXSCREEN );
		int nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("reviseWindowPos %d %d %d %d\n"),nDektopWidth,nDesktopHeight,nScreenWidth,nScreenHeight);
			CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
		HWND hDeskTop = GetDesktopWindow();
		if(hDeskTop) ::GetWindowRect(hDeskTop,&DeskTopSize);
		else return;
	}
	else {
		CArray<RECT,RECT> arrayRect;
		arrayRect.RemoveAll();
		EnumDisplayMonitors(NULL,NULL,(MONITORENUMPROC)MonitorEnumFunc,(LPARAM)&arrayRect);

		int nSize = arrayRect.GetSize(),nCurrentMon = 0,nWidth,nHeight;
		CString strBuff;
		double dDistance,dMinDistance = -1.0f;
		for(int i = 0; i < nSize; i++) {
			strBuff.Format(_T("left:%d top:%d right:%d bottom:%d"),arrayRect[i].left,arrayRect[i].top,arrayRect[i].right,arrayRect[i].bottom);
			nWidth = abs((arrayRect[i].left + arrayRect[i].right) / 2 - pos->x);
			nHeight = abs((arrayRect[i].top + arrayRect[i].bottom) / 2 - pos->y);

			dDistance = sqrt((nWidth * nWidth) + (nHeight * nHeight));
			if(dDistance < dMinDistance || i == 0) {
				dMinDistance = dDistance;
				nCurrentMon = i;
			}
		}
		DeskTopSize = arrayRect[nCurrentMon];
	}
	//ウィンドウ位置を補正
	if(pos->y + m_ini.m_DialogSize.y > DeskTopSize.bottom)
		pos->y -= (pos->y + m_ini.m_DialogSize.y) - DeskTopSize.bottom;
	if(pos->x + m_ini.m_DialogSize.x > DeskTopSize.right)
		pos->x -= (pos->x + m_ini.m_DialogSize.x) - DeskTopSize.right;

	if(pos->y < DeskTopSize.top)
		pos->y = DeskTopSize.top;
	if(pos->x < DeskTopSize.left)
		pos->x = DeskTopSize.left;


}

//---------------------------------------------------
//関数名	closeTreeWindow(int nRet)
//機能		ポップアップ隠蔽処理
//---------------------------------------------------
void CCharu3App::closeTreeWindow(int nRet)
{
	m_pTreeDlg->ShowWindow(SW_HIDE);
	STRING_DATA data;
	bool isPaste = true;
	if(::GetAsyncKeyState(VK_SHIFT) < 0) isPaste = false;

	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("closeTreeWindow %d\n"),nRet);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	//アクティブウィンドウを復帰
	if(nRet == IDOK)  {
		CString strClip,strSelect;
		m_clipbord.getClipboardText(strClip);//クリップボードを保存

		setAppendKeyInit(m_focusInfo.m_hActiveWnd,&m_keySet);//キー設定を変更
		//キーが離されるのを待つ
		while(::GetAsyncKeyState(VK_MENU) < 0 || ::GetAsyncKeyState(VK_CONTROL) < 0 ||
			::GetAsyncKeyState(VK_SHIFT) < 0 || ::GetAsyncKeyState(VK_RETURN) < 0) Sleep(50);
		CGeneral::setFocusInfo(&m_focusInfo);//ターゲットをフォーカス
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("setFocusInfo %x %x\n"),m_focusInfo.m_hActiveWnd,m_focusInfo.m_hActiveWnd);
			CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
		//貼り付け処理
		if(m_pTree->m_ltCheckItems.size() > 0) {//複数選択データがある
			strSelect = getSelectString(m_keySet,m_focusInfo.m_hFocusWnd);//選択テキスト取得
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("closeTreeWindow sel:%s clip:%s\n"),strSelect,strClip);
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}

			list<HTREEITEM>::iterator it;
			for(it = m_pTree->m_ltCheckItems.begin(); it != m_pTree->m_ltCheckItems.end(); it++) {
				if(m_pTree->GetItemState(*it,TVIF_HANDLE)) {
					data = m_pTree->getData(*it);
					//デバッグログ処理
					if(m_ini.m_nDebug) {
						CString strText;
						strText.Format(_T("closeTreeWindow check paste %s\n"),data.m_strTitle);
						CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
					}
					playData(data,strClip,strSelect,isPaste,false);
				}
			}
			//一時項目は消す
			for(it = m_pTree->m_ltCheckItems.begin(); it != m_pTree->m_ltCheckItems.end(); it++) {
				if(m_pTree->GetItemState(*it,TVIF_HANDLE)) {
					HTREEITEM hItem = *it;
					*it = NULL;
					data = m_pTree->getData(hItem);
					if(data.m_cKind & KIND_ONETIME) {
						m_hSelectItemBkup = NULL;
					}
				}
			}
		}
		else if(m_pTreeDlg->m_selectIT != NULL) {//通常選択データ
			strSelect = getSelectString(m_keySet,m_focusInfo.m_hFocusWnd);//選択テキスト取得
			data = *(m_pTreeDlg->m_selectIT);
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("closeTreeWindow sel:%s clip:%s title:\n"),strSelect,strClip,data.m_strTitle);
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}
			playData(data,strClip,strSelect,isPaste);
		}
	}
	else if(::GetForegroundWindow() == m_focusInfo.m_hActiveWnd){
		CGeneral::setFocusInfo(&m_focusInfo);
	}
	if(m_ini.m_fifo.m_nAllClearOff && !m_pTree->getOneTimeText(m_ini.m_fifo.m_nFifo) && m_isStockMode) toggleStockMode();
	if(m_hSelectItemBkup) {
		m_pTree->SelectItem(m_hSelectItemBkup);
		m_hSelectItemBkup = NULL;
	}

	
	//データを保存
	if(!m_pTree->saveDataFile(m_ini.m_strDataFile,m_ini.m_strPluginName,NULL)) {	
		CString strRes;
		strRes.LoadString(APP_MES_FAILURE_DATA_SAVE);
		AfxMessageBox(strRes,MB_ICONEXCLAMATION,0);
	}
	m_ini.writeEnvInitData();//環境設定を保存

	//監視タイマーセット
	if(m_isStockMode && m_ini.m_etc.m_nActiveTime)
		SetTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE,m_ini.m_etc.m_nActiveTime,NULL);
	
	setAppendHotKey();//追加ホットキーを設定
	m_ini.setHookKey(m_hSelfWnd);

	//チェックボックス付きだった場合はチェックをクリア
	if(m_pTree->GetStyle() & TVS_CHECKBOXES) {
		if(m_ini.m_visual.m_nResetTree) {
			resetTreeDialog();
			CGeneral::setFocusInfo(&m_focusInfo);//ターゲットをフォーカス
		}
		else {
			m_pTree->removeCheck();
		}
	}
	//IMEをONにする
	if(m_ini.m_pop.m_nAutoImeOff) {
		HIMC hIMC = ImmGetContext(m_pTreeDlg->m_hWnd);
		if(m_isImeStatus)	ImmSetOpenStatus(hIMC,m_isImeStatus);
	}
	if(m_ini.m_etc.m_nMinimization) {
		m_pMainFrame->ShowWindow(SW_HIDE);
		m_pMainFrame->ShowWindow(SW_MINIMIZE);
		m_pMainFrame->ShowWindow(SW_HIDE);
	}
	ASSERT(m_nPhase == PHASE_POPUP);
	m_nPhase = PHASE_IDOL;
}

//---------------------------------------------------
//関数名	playData(STRING_DATA data,CString strClip)
//機能		データの展開処理をする
//---------------------------------------------------
void CCharu3App::playData(STRING_DATA data,CString strClip,CString strSelect,bool isPaste,bool isChange)
{
	CString strMacro,strPaste;
	list<STRING_DATA>::iterator itMacro;
	int nIsMove;

	//マクロ処理
	HTREEITEM hSelectItem = m_pTree->GetSelectedItem();
	HTREEITEM hMacroItem = NULL;
	if(hSelectItem) hMacroItem = m_pTree->serchParentOption(hSelectItem,_T("macro"));//一番近い親か自分のマクロを調べる
	if(hMacroItem) {
		m_pTree->getData(hMacroItem,&itMacro);
		strMacro = m_pTree->getDataOptionStr(itMacro->m_strMacro,_T("macro"));
	}
	else strMacro = m_ini.m_strMacroPluginName;
	strPaste = convertMacro(&data,strSelect,strClip,strMacro);

	//テキストを貼り付け処理
	if(isPaste) {
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("playData active:%x focus:%x\n"),m_focusInfo.m_hActiveWnd,m_focusInfo.m_hFocusWnd);
			CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
		execData(strPaste,m_keySet,hSelectItem,m_focusInfo.m_hFocusWnd);

		//クリップボード復帰
		if(!m_ini.m_etc.m_nToClip) {
			m_strlClipBackup = strClip;
			m_clipbord.setClipboardText(strClip);
		}
		if(isChange) {
			//貼り付けデータをフォルダの先頭に移動
			if(hSelectItem)	hMacroItem  = m_pTree->serchParentOption(hSelectItem,_T("move"));
			else			hMacroItem = NULL;
			if(hMacroItem) {
				m_pTree->getData(hMacroItem,&itMacro);
				nIsMove = m_pTree->getDataOption(itMacro->m_strMacro,_T("move"));
				if(nIsMove) { 
					hSelectItem = m_pTree->moveFolderTop(hSelectItem);
					if(hSelectItem) m_pTree->SelectItem(hSelectItem);
				}
			}
			//一時項目は消す
			if(m_pTree->getDatakind(hSelectItem) & KIND_ONETIME) {
				m_pTree->deleteData(hSelectItem);
				m_hSelectItemBkup = NULL;
			}
		}
	}
	else {
		m_clipbord.setClipboardText(strPaste);
	}
}

//---------------------------------------------------
//関数名	playHotItem(nTarget)
//機能		ホットアイテム処理
//---------------------------------------------------
void CCharu3App::playHotItem(int nTarget)
{
	if(nTarget >= 0 && nTarget < m_hotkeyVector.size()) {
		HOT_KEY_CODE keyData;
		keyData = m_hotkeyVector.at(nTarget);
		if(!keyData.m_uVkCode) {
			DWORD dwTime = timeGetTime();
			if((dwTime - keyData.m_dwDoubleKeyTime) >  m_ini.m_pop.m_nDCKeyPopTime) {
				m_hotkeyVector[nTarget].m_dwDoubleKeyTime = dwTime;
				return;
			}
		}
		STRING_DATA data;//テキストデータ

		if(keyData.m_hItem) {
			data = m_pTree->getData(keyData.m_hItem);
			CString strMacroData;
			CString strSelect,strMacro;
			//フォルダの場合
			if(data.m_cKind & KIND_FOLDER_ALL) {
				POINT pos;
				//ダイレクトコピー
				if(keyData.m_strMacroName == EXMACRO_DIRECT_COPY) {
					m_nPhase = PHASE_LOCK;
					stopAppendHotKey();//追加ホットキーを停止
					STRING_DATA dataChild;
					CGeneral::getCaretPos(&pos,&m_focusInfo);//キャレット位置を取得(ハンドルを取るため)

					//キーが離されるのを待つ
					while(::GetAsyncKeyState(VK_MENU) < 0 || ::GetAsyncKeyState(VK_CONTROL) < 0 ||
						::GetAsyncKeyState(VK_SHIFT) < 0 || ::GetAsyncKeyState(keyData.m_uVkCode) < 0) Sleep(50);
					keyUpDown(keyData.m_uModKey,keyData.m_uVkCode,KEY_UP);//キーを離す処理（これが無いと選択テキスト取得で失敗）

					setAppendKeyInit(m_focusInfo.m_hActiveWnd,&m_keySet);//キー設定を変更
					strSelect = getSelectString(m_keySet,m_focusInfo.m_hFocusWnd);//選択文字取得
					if(strSelect != "") {
						dataChild.m_cKind = KIND_LOCK;
						dataChild.m_nParentID = data.m_nMyID;
						dataChild.m_strData = strSelect;
						int nTitleLength = m_pTree->getDataOption(data.m_strMacro,_T("titlelen"));//タイトルの文字数
						if(nTitleLength < 1 || nTitleLength > 256) nTitleLength = 32;
						dataChild.m_strTitle = m_pTree->makeTitle(strSelect,nTitleLength);
						//デバッグログ処理
						if(m_ini.m_nDebug) {
							CString strText;
							strText.Format(_T("Direct copy folder %s\n"),dataChild.m_strTitle);
							CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
						}
						m_pTree->addData(keyData.m_hItem,dataChild,true,true);
					}
					setAppendHotKey();//追加ホットキーを設定
					m_nPhase = PHASE_IDOL;
				}
				//ホットキー
				else if(keyData.m_strMacroName == EXMACRO_HOT_KEY) {
					m_hSelectItemBkup = m_pTree->GetSelectedItem();

					m_pTree->SelectItem(keyData.m_hItem);
					getPopupPos(&pos,m_ini.m_pop.m_nPopupPos);//ポップアップ位置を取得
					if(m_ini.m_pop.m_nOutRevice)	reviseWindowPos(&pos);
					popupTreeWindow(pos,true,keyData.m_hItem);//ポップアップ
				}
			}
			else {//定形文は一発貼り付け
				m_nPhase = PHASE_LOCK;
				stopAppendHotKey();//追加ホットキーを停止

				CString strClip,strPaste;
				strPaste = data.m_strData;
				m_clipbord.getClipboardText(strClip);//クリップボードを保存

				POINT pos;

				//キーが離されるのを待つ
				while(::GetAsyncKeyState(VK_MENU) < 0 || ::GetAsyncKeyState(VK_CONTROL) < 0 ||
					::GetAsyncKeyState(VK_SHIFT) < 0 || ::GetAsyncKeyState(keyData.m_uVkCode) < 0) Sleep(50);
				keyUpDown(keyData.m_uModKey,keyData.m_uVkCode,KEY_UP);//キーを離す処理（これが無いと選択テキスト取得で失敗）

				HTREEITEM hMacroItem;
				list<STRING_DATA>::iterator itMacro;
				int nIsMove;

				//ダイレクトコピー
				if(keyData.m_strMacroName == EXMACRO_DIRECT_COPY) {
					CGeneral::getCaretPos(&pos,&m_focusInfo);//キャレット位置を取得
					setAppendKeyInit(m_focusInfo.m_hActiveWnd,&m_keySet);//キー設定を変更
					strSelect = getSelectString(m_keySet,m_focusInfo.m_hFocusWnd);//選択文字取得
					data.m_strData = strSelect;
					m_pTree->editData(keyData.m_hItem,data);
					//デバッグログ処理
					if(m_ini.m_nDebug) {
						CString strText;
						strText.Format(_T("Direct copy data %s\n"),data.m_strTitle);
						CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
					}
				}
				//ホットキー
				else if(keyData.m_strMacroName == EXMACRO_HOT_KEY) {
					//マクロ処理
					hMacroItem = m_pTree->serchParentOption(keyData.m_hItem,_T("macro"));//一番近い親か自分のマクロを調べる
					if(hMacroItem) {
						m_pTree->getData(hMacroItem,&itMacro);
						strMacro = m_pTree->getDataOptionStr(itMacro->m_strMacro,_T("macro"));
					}
					else strMacro = m_ini.m_strMacroPluginName;

					CGeneral::getCaretPos(&pos,&m_focusInfo);//キャレット位置を取得
					setAppendKeyInit(m_focusInfo.m_hActiveWnd,&m_keySet);//キー設定を変更

					strSelect = getSelectString(m_keySet,m_focusInfo.m_hFocusWnd);//選択文字取得
					strPaste = convertMacro(&data,strSelect,strClip,strMacro);//マクロ変換
					
					//デバッグログ処理
					if(m_ini.m_nDebug) {
						CString strText;
						strText.Format(_T("Direct paste data %s active:%x focus:%x\n"),strPaste,m_focusInfo.m_hActiveWnd,m_focusInfo.m_hFocusWnd);
						CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
					}

					//テキストを貼り付け処理
					execData(strPaste,m_keySet,keyData.m_hItem,m_focusInfo.m_hFocusWnd);

					//貼り付けデータをフォルダの先頭に移動
					hMacroItem  = m_pTree->serchParentOption(keyData.m_hItem,_T("move"));
					if(hMacroItem) {
						m_pTree->getData(hMacroItem,&itMacro);
						nIsMove = m_pTree->getDataOption(itMacro->m_strMacro,_T("move"));
						if(nIsMove) {
							keyData.m_hItem = m_pTree->moveFolderTop(keyData.m_hItem);
						}
					}
					//クリップボード復帰
					if(!m_ini.m_etc.m_nToClip) {
						m_clipbord.setClipboardText(strClip);
					}
					//一時項目は消す
					if(m_pTree->getDatakind(keyData.m_hItem) & KIND_ONETIME) {
						m_pTree->deleteData(keyData.m_hItem);
					}
				}
				setAppendHotKey();//追加ホットキーを設定
				m_nPhase = PHASE_IDOL;
			}
		}
	}
	return;
}

//---------------------------------------------------
//関数名	getSelectString()
//機能		選択文字列を取る
//---------------------------------------------------
CString CCharu3App::getSelectString(COPYPASTE_KEY key,HWND hWnd)
{
	m_clipbord.setClipboardText(_T(""));
	if(!hWnd)	hWnd = m_focusInfo.m_hFocusWnd;
	CString strSelect;

	TCHAR strWindowName[1024];
	*strWindowName = (TCHAR)NULL;
	CString strWinName;
	int i;
	COPYPASTE_KEY keySet;

	GetWindowText(m_focusInfo.m_hActiveWnd,strWindowName,sizeof(strWindowName));
	strWinName = strWindowName;

	keySet.m_nMessage = 0;

	for(i = 0; keySet.m_nMessage > -1 ;i++) {
		keySet = m_ini.getAppendKeyInit(strWinName,i);
		if(keySet.m_nMessage <= -1 && i == 0)	keySet = key;
		if(keySet.m_nMessage > -1) {
			if(keySet.m_nMessage == 0) {//イベント方式
				keyUpDown(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_DOWN);
				keyUpDown(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_UP);
				Sleep(keySet.m_nCopyWait);//ウェイト
				keyUpDownMessage(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_UP,hWnd);
			}
			else if(keySet.m_nMessage == 1) {//Charu2Pro方式
				keyUpDownC2(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_DOWN);
				keyUpDownC2(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_UP);
				Sleep(keySet.m_nCopyWait);//ウェイト
				keyUpDownMessage(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_DOWN,hWnd);
				Sleep(keySet.m_nCopyWait/4);//ウェイト
				keyUpDownMessage(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_DOWN,hWnd);
				keyUpDownMessage(keySet.m_uMod_Copy,keySet.m_uVK_Copy,KEY_UP,hWnd);
				Sleep(keySet.m_nCopyWait);//ウェイト
			}
			else if(keySet.m_nMessage == 2) {
				::PostMessage(hWnd,key.m_copyMessage.Msg,key.m_copyMessage.wParam,key.m_copyMessage.lParam);//メッセージ方式
//				::SendMessage(hWnd,WM_COPY,NULL,NULL);//メッセージ方式
			}
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("getSelectString %d %s\n"),keySet.m_nMessage,strSelect);
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}
		}
	}
	for(i = 0; i <= m_ini.m_etc.m_nCopyWaitCnt; i++) {
		if(m_clipbord.getClipboardText(strSelect)) break;
		Sleep(key.m_nCopyWait);//ウェイト
	}
	return strSelect;
}

//---------------------------------------------------
//関数名	execKeyMacro(CString strKeyMacro)
//機能		キーボードマクロを実行
//---------------------------------------------------
void CCharu3App::execKeyMacro(CString strKeyMacro)
{
	TCHAR *szKeyMacro,strKeyCode[256],*szSplit;
	int nCount,nSleep,nKey[16],i,nKeyCount;
	szKeyMacro  = strKeyMacro.GetBuffer(strKeyMacro.GetLength());

	//貼り付け後のスリープを取得
	szSplit = UStringWork::awk(szKeyMacro,strKeyCode,sizeof(strKeyCode),3,_T(';'));
	if(szSplit)  {
		nSleep = _ttoi(strKeyCode);
		if(nSleep > 10000) nSleep = 10000;
	}
	else         nSleep = 0;

	//貼り付け回数を取得
	szSplit = UStringWork::awk(szKeyMacro,strKeyCode,sizeof(strKeyCode),2,_T(';'));
	if(szSplit)  {
		nCount = _ttoi(strKeyCode);
		if(nCount > 256) nCount = 256;
		*szSplit = NULL;
	}
	else         nCount = 1;

	//キー配列を取得　最大で16キー
	for(i = 0; i <= 15; i++) {
		UStringWork::awk(szKeyMacro,strKeyCode,sizeof(strKeyCode),i+1,_T(','));
		if(_tcsclen(strKeyCode) > 0) nKey[i] = getKeycode(strKeyCode);
		else break;
	}
	nKeyCount = i - 1;//キーの個数

	//キーを押す
	for(;nCount > 0; nCount--) {
		for(i = 0;i <= nKeyCount; i++) {
			keybd_event(nKey[i],0,KEYEVENTF_EXTENDEDKEY,0);//押す
		}
	}
	for(i = 0;i <= nKeyCount; i++) {
		keybd_event(nKey[i],0,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,0);//キーを離す
	}
	Sleep(nSleep);
}

//---------------------------------------------------
//関数名	execData()
//機能		キーボードマクロを実行しながらデータを貼り付ける
//---------------------------------------------------
void CCharu3App::execData(CString strPaste,COPYPASTE_KEY key,HTREEITEM hTargetItem,HWND hWnd)
{
	HTREEITEM hMacroItem;
	list<STRING_DATA>::iterator itMacro;
	CString strCut,strKeyMacro;
	int nStart = 0,nFoundStart,nFoundEnd,nMacroLenS,nMacroLenE;

	//ビフォアキー処理
	if(hTargetItem)	hMacroItem  = m_pTree->serchParentOption(hTargetItem,_T("beforkey"));
	else			hMacroItem = NULL;
	if(hMacroItem) {
		m_pTree->getData(hMacroItem,&itMacro);
		strKeyMacro = m_pTree->getDataOptionStr(itMacro->m_strMacro,_T("beforkey"));
		if(strKeyMacro != "") execKeyMacro(strKeyMacro);
	}
	
	//マクロを置換
	strPaste.Replace(_T("<charu2MACRO_KEY>"),MACRO_START_KEY);
	strPaste.Replace(_T("</charu2MACRO_KEY>"),MACRO_END_KEY);

	nMacroLenS = _tcsclen(MACRO_START_KEY);
	nMacroLenE = _tcsclen(MACRO_END_KEY);

	do {
		nFoundStart = strPaste.Find(MACRO_START_KEY,nStart);
		strKeyMacro = "";
		//キーマクロを含む
		if(nFoundStart >= 0) {
			nFoundEnd = strPaste.Find(MACRO_END_KEY,nFoundStart);
			strCut = strPaste.Mid(nStart,nFoundStart - nStart);
			if(nFoundEnd > nFoundStart)	{
				nStart = nFoundEnd + nMacroLenE;
					strKeyMacro = strPaste.Mid(nFoundStart + nMacroLenS,nFoundEnd - nFoundStart - nMacroLenS);
			}
			else	nStart = nFoundStart + nMacroLenS;
		}
		else 	strCut = strPaste.Mid(nStart,strPaste.GetLength() - nStart);
		//個別に貼り付け処理
		if(strCut != _T("")) {
//			pasteData(strCut,key,hWnd);//貼り付け
//			m_strlClipBackup = strCut;
			
			TCHAR strWindowName[1024];
			*strWindowName = (char)NULL;
			CString strWinName;
			int i;
			COPYPASTE_KEY keySet;

			GetWindowText(m_focusInfo.m_hActiveWnd,strWindowName,sizeof(strWindowName));
			strWinName = strWindowName;

			keySet.m_nMessage = 0;
			for(i = 0; keySet.m_nMessage > -1 ;i++) {
				keySet = m_ini.getAppendKeyInit(strWinName,i);
				if(keySet.m_nMessage <= -1 && i == 0)	keySet = key;
				if(keySet.m_nMessage > -1) {
					pasteData(strCut,keySet,hWnd);//貼り付け
				}
			}
			m_strlClipBackup = strCut;
		}
		//キーマクロを実行
		if(strKeyMacro != _T("")) {
			execKeyMacro(strKeyMacro);
		}
	}while(nFoundStart >= 0);

	//アフターキー処理
	if(hTargetItem)	hMacroItem  = m_pTree->serchParentOption(hTargetItem,_T("afterkey"));
	else			hMacroItem = NULL;
	if(hMacroItem) {
		m_pTree->getData(hMacroItem,&itMacro);
		strKeyMacro = m_pTree->getDataOptionStr(itMacro->m_strMacro,_T("afterkey"));
		if(strKeyMacro != _T("")) execKeyMacro(strKeyMacro);
	}
}

//---------------------------------------------------
//関数名	pasteData()
//機能		データを貼り付ける
//---------------------------------------------------
void CCharu3App::pasteData(CString strPaste,COPYPASTE_KEY key,HWND hWnd)
{
	if(m_isStockMode) m_strlClipBackup = strPaste;
	m_clipbord.setClipboardText(strPaste);

	if(key.m_nMessage == 0) {//イベント方式
		keyUpDown(key.m_uMod_Paste,key.m_uVK_Paste,KEY_DOWN);
		keyUpDown(key.m_uMod_Paste,key.m_uVK_Paste,KEY_UP);
		Sleep(key.m_nPasteWait);
	}
	else if(key.m_nMessage == 1) {//Charu2Pro方式
		keyUpDownC2(key.m_uMod_Paste,key.m_uVK_Paste,KEY_DOWN);
		keyUpDownC2(key.m_uMod_Paste,key.m_uVK_Paste,KEY_UP);
		Sleep(key.m_nPasteWait);
	}
	else if(key.m_nMessage == 2) {
		::PostMessage(hWnd,key.m_pasteMessage.Msg,key.m_pasteMessage.wParam,key.m_pasteMessage.lParam);//メッセージ方式
//		::PostMessage(hWnd,WM_PASTE,NULL,NULL);//メッセージ方式
	}
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("pasteData %d %s %x %x active:%x focus:%x\n"),key.m_nMessage,strPaste,key.m_uMod_Paste,key.m_uVK_Paste,m_focusInfo.m_hActiveWnd,m_focusInfo.m_hFocusWnd);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
}

//---------------------------------------------------
//関数名	keyUpDown()
//機能		仮想キーを押す
//---------------------------------------------------
void CCharu3App::keyUpDown(UINT uMod,UINT uVKCode,int nFlag)
{
	if(nFlag & KEY_DOWN) {
		if(uMod & MOD_ALT)
			keybd_event(VK_MENU, (BYTE)MapVirtualKey(VK_MENU, 0), KEYEVENTF_EXTENDEDKEY, 0);
		if(uMod & MOD_CONTROL)
			keybd_event(VK_CONTROL, (BYTE)MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_EXTENDEDKEY, 0);
		if(uMod & MOD_SHIFT)
			keybd_event(VK_SHIFT, (BYTE)MapVirtualKey(VK_SHIFT, 0), KEYEVENTF_EXTENDEDKEY, 0);
		
		keybd_event(uVKCode,0,KEYEVENTF_EXTENDEDKEY,0);
	}
	if(nFlag & KEY_UP) {
		keybd_event(uVKCode,0,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,0);//キーを離す

		if(uMod & MOD_ALT)
			keybd_event(VK_MENU, (BYTE)MapVirtualKey(VK_MENU, 0), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		if(uMod & MOD_CONTROL)
			keybd_event(VK_CONTROL, (BYTE)MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		if(uMod & MOD_SHIFT)
			keybd_event(VK_SHIFT, (BYTE)MapVirtualKey(VK_SHIFT, 0), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}

//---------------------------------------------------
//関数名	keyUpDown()
//機能		仮想キーを押す
//---------------------------------------------------
void CCharu3App::keyUpDownC2(UINT uMod,UINT uVKCode,int nFlag)
{
	if(nFlag & KEY_DOWN) {
		if(uMod & MOD_ALT)		keybd_event(VK_MENU,0,NULL,0);
		if(uMod & MOD_CONTROL)	keybd_event(VK_CONTROL,0,NULL,0);
		if(uMod & MOD_SHIFT)	keybd_event(VK_SHIFT,0,NULL,0);
		
		keybd_event(uVKCode,0,NULL,0);
	}
	else if(nFlag & KEY_UP) {
		keybd_event(uVKCode,0,KEYEVENTF_KEYUP,0);//キーを離す

		if(uMod & MOD_ALT)		keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
		if(uMod & MOD_CONTROL)	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
		if(uMod & MOD_SHIFT)	keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
	}
}

//---------------------------------------------------
//関数名	keyUpDownMessage
//機能		仮想キーを押す
//---------------------------------------------------
void CCharu3App::keyUpDownMessage(UINT uMod,UINT uVKCode,int nFlag,HWND hWnd)
{
	if(nFlag & KEY_DOWN) {
		if(uMod & MOD_ALT)		::SendMessage(hWnd,WM_KEYDOWN,VK_MENU,1);
		if(uMod & MOD_CONTROL)	::SendMessage(hWnd,WM_KEYDOWN,VK_CONTROL,1);
		if(uMod & MOD_SHIFT)	::SendMessage(hWnd,WM_KEYDOWN,VK_SHIFT,1);

		::SendMessage(hWnd,WM_KEYDOWN,uVKCode,1);
	}
	if(nFlag & KEY_UP) {
		::SendMessage(hWnd,WM_KEYUP,uVKCode,1);

		if(uMod & MOD_ALT)		::SendMessage(hWnd,WM_KEYUP,VK_MENU,1);
		if(uMod & MOD_CONTROL)	::SendMessage(hWnd,WM_KEYUP,VK_CONTROL,1);
		if(uMod & MOD_SHIFT)	::SendMessage(hWnd,WM_KEYUP,VK_SHIFT,1);
	}
}
//---------------------------------------------------
//関数名	convertMacro(CString strSourceData)
//機能		マクロ文字列を置換
//---------------------------------------------------
CString CCharu3App::convertMacro(STRING_DATA *SourceData,CString strSelect,CString strClip,CString strSoftName)
{
	SetCurrentDirectory(m_ini.m_strAppPath);

	if(strSoftName == _T("no")) return SourceData->m_strData;

	CString strSourceData;
	if(strSoftName != DAT_FORMAT) {
		if(m_pTree->convertMacroPlugin(SourceData,&strSourceData,strSelect,strClip,strSoftName))
			return strSourceData;
	}
	
	int nStart,nEnd,nMove,nTagStart,nTagEnd;

	CString strTag,strTagEnd,strBuff,strBuff2,strTime;
	CString strShellStart = _T("$SHELL<"),strShellEnd = _T(">$SHELL");
	CString strRelateStart = _T("$RELATE<"),strRelateEnd = _T(">$RELATE");
	CString strPlugStart = _T("$PLUG-IN<"),strPlugEnd = _T(">$PLUG-IN");

	//マクロ誤処理対策
	TCHAR strMark[2];
	strMark[0] = 0x7f;
	strMark[1] = NULL;
	bool isSelect = false,isClip = false;

	CString strRepBuff1,strRepBuff2,strRepBuff3,strRepBuff4,strRepBuff5;
	strRepBuff1 = _T("<charuMACR@c3mO@>");
	strRepBuff2 = _T("</charuMACR@c3mO@>");
	strRepBuff3 = _T("@c3m$@PLUG-IN<");
	strRepBuff4 = _T(">@c3m$@PLUG-IN");
	strRepBuff5 = _T("@c3m$@CLIP");

	if(SourceData->m_strData.Find(_T("$SEL")) != -1) {
		strSelect.Replace(MACRO_START,strRepBuff1);
		strSelect.Replace(MACRO_END,strRepBuff2);
		strSelect.Replace(strPlugStart,strRepBuff3);
		strSelect.Replace(strPlugEnd,strRepBuff4);
		strSelect.Replace(_T("$CLIP"),strRepBuff5);
		isSelect = true;
	}
	if(SourceData->m_strData.Find(_T("$CLIP")) != -1) {
		strClip.Replace(MACRO_START,strRepBuff1);
		strClip.Replace(MACRO_END,strRepBuff2);
		strClip.Replace(strPlugStart,strRepBuff3);
		strClip.Replace(strPlugEnd,strRepBuff4);
		isClip = true;
	}

	CTime cTime = CTime::GetCurrentTime();
	strSourceData = SourceData->m_strData;

	strSourceData.Replace(_T("<charu2MACRO>"),MACRO_START);
	strSourceData.Replace(_T("</charu2MACRO>"),MACRO_END);
	strTag = MACRO_START;
	strTagEnd = MACRO_END;
	nEnd = 0;
	while(1) {
 		nStart = strSourceData.Find(strTag,nEnd);//タグの先頭を取得
		if(nStart == -1) break;
		nEnd = strSourceData.Find(strTagEnd,nStart);//タグの終わりを取得
		if(nEnd == -1) nEnd = strSourceData.GetLength();
		if(nStart > nEnd) break;
		nEnd += strTagEnd.GetLength();

		strBuff = strSourceData.Mid(nStart,nEnd-nStart);//タグ部分切り出し
		strBuff2 = strBuff;
		strBuff2.Replace(strTag,_T(""));
		strBuff2.Replace(strTagEnd,_T(""));

		if(strBuff2.Find(_T("$Y")) != -1) {//2000 西暦4桁
			strTime.Format(_T("%04d"),cTime.GetYear());
			strBuff2.Replace(_T("$Y"),strTime);
		}
		if(strBuff2.Find(_T("$y")) != -1) {//00 西暦2桁
			strTime.Format(_T("%02d"),cTime.GetYear() - 2000);
			strBuff2.Replace(_T("$y"),strTime);
		}
		if(strBuff2.Find(_T("$WH")) != -1) {//00 和暦2桁
			strTime.Format(_T("%02d"),cTime.GetYear() - 1988);
			strBuff2.Replace(_T("$WH"),strTime);
		}

		if(strBuff2.Find(_T("$MM")) != -1) {//May 月英語表記
			CString strMonth[] = {_T("Jan"),_T("Feb"),_T("Mar"),_T("Apr"),_T("May"),_T("Jun"),_T("Jul"),_T("Aug"),_T("Sep"),_T("Oct"),_T("Nov"),_T("Dec")};
			strBuff2.Replace(_T("$MM"),strMonth[cTime.GetMonth() - 1]);
		}
		if(strBuff2.Find(_T("$M")) != -1) {//05 月2桁
			strTime.Format(_T("%02d"),cTime.GetMonth());
			strBuff2.Replace(_T("$M"),strTime);
		}

		if(strBuff2.Find(_T("$D")) != -1) {//06 日付数字2桁
			strTime.Format(_T("%02d"),cTime.GetDay());
			strBuff2.Replace(_T("$D"),strTime);
		}
		if(strBuff2.Find(_T("$ddd")) != -1) {//Saturday 曜日英語
			CString strYoubi[] = {_T("Sunday"),_T("Monday"),_T("Tuesday"),_T("Wednesday"),_T("Thursday"),_T("Friday"),_T("Saturday")};
			strBuff2.Replace(_T("$ddd"),strYoubi[cTime.GetDayOfWeek() - 1]);
		}
		if(strBuff2.Find(_T("$dd")) != -1) {//Sat 曜日英語簡易表記
			CString strYoubi[] = {_T("Sun"),_T("Mon"),_T("Tue"),_T("Wed"),_T("Thu"),_T("Fri"),_T("Sat")};
			strBuff2.Replace(_T("$dd"),strYoubi[cTime.GetDayOfWeek() - 1]);
		}
		if(strBuff2.Find(_T("$d")) != -1) {//土曜 曜日
			CString strYoubi[] = {_T("日"),_T("月"),_T("火"),_T("水"),_T("木"),_T("金"),_T("土")};
			strBuff2.Replace(_T("$d"),strYoubi[cTime.GetDayOfWeek() - 1]);
		}

		if(strBuff2.Find(_T("$H")) != -1) {//21 時間24時間表記
			strTime.Format(_T("%02d"),cTime.GetHour());
			strBuff2.Replace(_T("$H"),strTime);
		}
		if(strBuff2.Find(_T("$h")) != -1) {//09 時間
			int nHours = cTime.GetHour();
			if(nHours > 11) nHours -= 12;
			strTime.Format(_T("%02d"),nHours);
			strBuff2.Replace(_T("$h"),strTime);
		}
		if(strBuff2.Find(_T("$m")) != -1) {//46 分
			strTime.Format(_T("%02d"),cTime.GetMinute());
			strBuff2.Replace(_T("$m"),strTime);
		}
		if(strBuff2.Find(_T("$s")) != -1) {//02 秒
			strTime.Format(_T("%02d"),cTime.GetSecond());
			strBuff2.Replace(_T("$s"),strTime);
		}

		if(strBuff2.Find(_T("$ampm")) != -1) {//午後
			int nHours = cTime.GetHour();
			if(nHours > 11) strTime = _T("午後");
			else			strTime = _T("午前");
			strBuff2.Replace(_T("$ampm"),strTime);
		}
		if(strBuff2.Find(_T("$AMPM")) != -1) {//PM
			int nHours = cTime.GetHour();
			if(nHours > 11) strTime = _T("PM");
			else			strTime = _T("AM");
			strBuff2.Replace(_T("$AMPM"),strTime);
		}
		if(strBuff2.Find(_T("$SEL")) != -1) {//選択部分
			strBuff2.Replace(_T("$SEL"),strSelect);
		}
		if(strBuff2.Find(_T("$CLIP")) != -1) {//選択部分
			strBuff2.Replace(_T("$CLIP"),strClip);
		}
		if(strBuff2.Find(strPlugStart) != -1) {//プラグイン
			int nPlugLength = strPlugStart.GetLength();
			int nPlugEndLength = strPlugEnd.GetLength();

			nTagStart = strBuff2.Find(strPlugStart);
			nTagEnd = strBuff2.Find(strPlugEnd);

			if(nTagEnd < nTagStart) {
				strBuff2.Delete(nTagStart,nPlugLength);
			}
			else {

				HMODULE hDLL;
				CharuPlugIn pPlugIn;

				while(strBuff2.Find(strPlugStart) != -1) {
					int nPlugStart = strBuff2.Find(strPlugStart) + nPlugLength;//開始位置取得
					int nPlugEnd = strBuff2.Find(strPlugEnd,nPlugStart);//終わり位置取得
					if(nPlugEnd == -1) nPlugEnd = strBuff2.GetLength();

					CString strPlug,strDllName,strTmp;
					TCHAR *strRet;
					strPlug = strBuff2.Mid(nPlugStart,nPlugEnd - nPlugStart);
					int nComma = strPlug.Find(_T(","),0);
					if(nComma >= 0){
						strDllName = strPlug.Left(nComma);
						strTmp = strPlug.Right(strPlug.GetLength() - nComma - 1);
					}
					else{
						strDllName = _T("");
						strTmp = strPlug;
					}

					//DLLをロード
					hDLL = LoadLibrary(strDllName);
					if(hDLL){
						pPlugIn = (CharuPlugIn)GetProcAddress(hDLL,"CharuPlugIn");
						if(pPlugIn) {
							int nSize = strTmp.GetLength() * 5;
							if(nSize < 10240) nSize = 10240;
							strRet = new TCHAR[nSize];
							if(strRet) {
								CString strPluginRet;
								int nTmpLen = strTmp.GetLength()+1;
								if(strTmp.Find(_T("@c3m")) >= 0) {
									strTmp.Replace(_T("@c3mO@"),_T("O"));
									strTmp.Replace(_T("@c3m$@"),_T("$"));
								}
//								int nBuffSize = nSize * sizeof(TCHAR);

								pPlugIn(strTmp.GetBuffer(strTmp.GetLength()+1),strRet,nSize,SourceData,(void*)&m_focusInfo);
								strPluginRet = strRet;
								strPluginRet.Replace(MACRO_START,strRepBuff1);
								strPluginRet.Replace(MACRO_END,strRepBuff2);
								strPluginRet.Replace(strPlugStart,strRepBuff3);
								strPluginRet.Replace(strPlugEnd,strRepBuff4);

								strBuff2.Delete(nPlugStart,strDllName.GetLength()+nTmpLen);
								strBuff2.Insert(nPlugStart,strPluginRet);//置換
								delete []strRet;
							}
						}
						else{
							CString strRes;
							strRes.LoadString(APP_MES_NO_ENTRY_FUNC);
							AfxMessageBox(strRes,MB_ICONEXCLAMATION | MB_SYSTEMMODAL,0);
						}
						FreeLibrary(hDLL);
					}
					else{
						CString strMessage,strRes;
						strRes.LoadString(APP_MES_NOT_FOUND);
						strMessage.Format(strRes,strDllName);
						AfxMessageBox(strMessage,MB_ICONEXCLAMATION | MB_SYSTEMMODAL,0);
					}
					strBuff2.Delete(strBuff2.Find(strPlugStart),nPlugLength);
					strBuff2.Delete(strBuff2.Find(strPlugEnd),nPlugEndLength);
				}
			}
		}
		if(strBuff2.Find(strRelateStart) != -1) {//関連付け
			int nRelateLength = strRelateStart.GetLength();
			nTagStart = strBuff2.Find(strRelateStart);
			nTagEnd = strBuff2.Find(strRelateEnd);

			if(nTagEnd < nTagStart) {
				strBuff2.Delete(nTagStart,nRelateLength);
			}
			else {
				while(strBuff2.Find(strRelateStart) != -1) {
					int nMacroStart = strBuff2.Find(strRelateStart);
					int nRelateStart = nMacroStart + nRelateLength;//開始位置取得
					int nRelateEnd = strBuff2.Find(strRelateEnd,nRelateStart);//終わり位置取得
					if(nRelateEnd == -1) nRelateEnd = strBuff2.GetLength();

					CString strRelate;
					strRelate = strBuff2.Mid(nRelateStart,nRelateEnd - nRelateStart);//コマンドライン切り出し

					ShellExecute(NULL,NULL,strRelate,NULL,NULL,SW_SHOWNORMAL);
					strBuff2.Delete(nMacroStart,nRelateLength + nRelateEnd  - nMacroStart);//タグを削除
				}
			}
		}
		if(strBuff2.Find(strShellStart) != -1) {//シェル
			STARTUPINFO sinf;
			PROCESS_INFORMATION pri;
			ZeroMemory(&sinf,sizeof(sinf));
			int nShellLength = strShellStart.GetLength();
			int nRelateLength = strRelateStart.GetLength();

			nTagStart = strBuff2.Find(strShellStart);
			nTagEnd = strBuff2.Find(strShellEnd);

			if(nTagEnd < nTagStart) {
				strBuff2.Delete(nTagStart,nShellLength);
			}
			else {
				while(strBuff2.Find(strShellStart) != -1) {
					int nMacroStart = strBuff2.Find(strShellStart);
					int nShellStart = nMacroStart + nShellLength;//開始位置取得
					int nShellEnd = strBuff2.Find(strShellEnd,nShellStart);//終わり位置取得
					if(nShellEnd == -1) nShellEnd = strBuff2.GetLength();

					CString strShell;
					strShell = strBuff2.Mid(nShellStart,nShellEnd - nShellStart);//コマンドライン切り出し

					CreateProcess(NULL,strShell.GetBuffer(strShell.GetLength()),NULL,NULL,FALSE,0,NULL,NULL,&sinf,&pri);//起動
					WaitForInputIdle(pri.hProcess,INFINITE);
					strBuff2.Delete(nMacroStart,nShellLength + nShellEnd  - nMacroStart);//タグを削除
				}
			}
		}

		
		nMove = strBuff2.GetLength();// - strBuff.GetLength();
		strSourceData.Delete(nStart,strBuff.GetLength());
		strSourceData.Insert(nStart,strBuff2);
		nEnd = nStart + nMove;
	}
	if(strSourceData.Find(_T("@c3m")) >= 0) {
		strSourceData.Replace(_T("@c3mO@"),_T("O"));
		strSourceData.Replace(_T("@c3m$@"),_T("$"));
	}
/*	strSourceData.Replace(strRepBuff2,MACRO_END);
	strSourceData.Replace(strRepBuff3,strPlugStart);
	strSourceData.Replace(strRepBuff4,strPlugEnd);*/

	return strSourceData;
}

//---------------------------------------------------
//関数名	changeClipBord()
//機能		メインフレームでクリップボードの変更を検知、
//          クリップボードの内容をリストに追加する
//---------------------------------------------------
void CCharu3App::changeClipBord(CString strClipBord)
{ 
	if(m_isWordBugClipCheck) {
		m_isWordBugClipCheck = false;
		return;
	}
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("changeClipBord \"%s\"\n"),strClipBord);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}
	//連続で空のクリップボード更新イベントが起こるので対策
	//2007/10/27-20:20:19-------------------
	static int nEmptyCnt = 0;
	if(strClipBord == "")  {
/*		if(nEmptyCnt > 5) {
			Sleep(3000);
			nEmptyCnt = 0;
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("changeClipBord Sleep \"%s\"\n"),strClipBord);
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}
		}
		else {
			nEmptyCnt++;
		}
*/		return;
	}
	nEmptyCnt = 0;
	CString strText;

	//サイズ確認
	int nLimit;
	CString strTitle;
	nLimit = m_ini.m_key.m_nHistoryLimit;

	//タイトル別に処理してみる
	strTitle = CGeneral::getWindowTitle(::GetForegroundWindow());
	if(strTitle != "") {
		CHANGE_KEY key;
		key = m_ini.getAppendKeyInit2(strTitle);
		if(key.m_nMatch >= 0) nLimit = key.m_nHistoryLimit;
	}
	nLimit = nLimit * 1024;//KBなので1024倍
	if(nLimit >= 0 && nLimit < strClipBord.GetLength()) return;//上限より大きかったら抜ける


	if(m_nPhase == PHASE_IDOL && /*strClipBord != m_strlClipBackup &&*/ strClipBord != "") {
		m_nPhase = PHASE_LOCK;
		STRING_DATA data;
		m_pTree->initStringData(&data);

		data.m_cKind    = KIND_ONETIME;
		data.m_strData  = strClipBord;
		data.m_strTitle = "";
		data.m_cIcon = KIND_DEFAULT;
		//タイマー停止
		KillTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE);
		KillTimer(m_pMainWnd->m_hWnd,TIMER_SELF_DIAGNOSIS);
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("clipboard record \"%s\"\n"),strClipBord);
			CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
		//ストックモード処理
		if(m_isStockMode) {
			if((m_ini.m_fifo.m_nDuplication && strClipBord != m_strlClipBackup) || !m_ini.m_fifo.m_nDuplication) {
				if(m_ini.m_fifo.m_nCopySound) PlaySound(m_ini.m_fifo.m_strWaveFile, NULL, SND_ASYNC|SND_FILENAME);
				m_pTree->addData(NULL,data);//リストに追加
			}
		}
		//履歴フォルダ処理
		m_pTree->addDataToRecordFolder(data,m_strlClipBackup);
		if(m_isStockMode)
			SetTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE,m_ini.m_etc.m_nActiveTime,NULL);
		//自己診断タイマーセット
		if(m_ini.m_etc.m_nSelfDiagnosisTime)
			SetTimer(m_pMainWnd->m_hWnd,TIMER_SELF_DIAGNOSIS,m_ini.m_etc.m_nSelfDiagnosisTime,NULL);
	
		m_nPhase = PHASE_IDOL;
	}
	m_strlClipBackup = strClipBord;
}

//---------------------------------------------------
//関数名	fifoClipbord()
//機能		ストックモード処理
//---------------------------------------------------
void CCharu3App::fifoClipbord()
{
	if(!m_isStockMode || m_nPhase != PHASE_IDOL) return;
	UnregisterHotKey(NULL,HOTKEY_PASTE);//ホットキー解除
	STRING_DATA data;
	HTREEITEM hTarget;

	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CGeneral::writeLog(m_ini.m_strDebugLog,_T("fifoClipbord\n"),_ME_NAME_,__LINE__);
	}
	m_nPhase = PHASE_LOCK;
	//先入れ処理
	if(m_ini.m_fifo.m_nFifo) {
		hTarget = m_pTree->getOneTimeText(m_ini.m_fifo.m_nFifo);
		if(hTarget)	{
			data = m_pTree->getData(hTarget);
			m_pTree->deleteData(hTarget);
		}
		if(data.m_strData != "" || !m_ini.m_fifo.m_nNoClear) {
			for(int i = 0; i <= m_ini.m_etc.m_nPasteWaitCnt; i++) {
				//デバッグログ処理
				if(m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("fifoClipbord text:%s\n"),data.m_strData);
					CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}
				if(m_clipbord.setClipboardText(data.m_strData))	break;//クリップボードにセット
				Sleep(50);//ウェイト
			}
			m_strlClipBackup = data.m_strData;		//比較用文字列にセット
		}
	}
	if(m_ini.m_fifo.m_nPasteSound) {//効果音を鳴らす
		PlaySound(m_ini.m_fifo.m_strWaveFile, NULL, SND_ASYNC|SND_FILENAME);
	}
	//仮想的にCtrl+Vを押す
	UINT uVkCode = NULL;
	if(m_keySet.m_nMessage == 1) {//Charu2Pro方式イベント方式
		keyUpDownC2(m_keySet.m_uMod_Paste,m_keySet.m_uVK_Paste,KEY_DOWN);

	}
	else {
		keyUpDown(m_keySet.m_uMod_Paste,m_keySet.m_uVK_Paste,KEY_DOWN);
	}
	//キーを離す処理をする
	if(::GetAsyncKeyState(VK_RCONTROL) < 0 || ::GetAsyncKeyState(VK_LCONTROL) < 0 || ::GetAsyncKeyState(VK_CONTROL) < 0)
		uVkCode |= VK_CONTROL;
	if(::GetAsyncKeyState(VK_RSHIFT) < 0 || ::GetAsyncKeyState(VK_LSHIFT) < 0 || ::GetAsyncKeyState(VK_SHIFT) < 0)
		uVkCode |= VK_SHIFT;
	if(::GetAsyncKeyState(VK_RMENU) < 0 || ::GetAsyncKeyState(VK_LMENU) < 0 || ::GetAsyncKeyState(VK_MENU) < 0)
		uVkCode |= VK_MENU;

	keybd_event(uVkCode, (BYTE)MapVirtualKey(uVkCode, 0), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

	RegisterHotKey(NULL,HOTKEY_PASTE,m_keySet.m_uMod_Paste,m_keySet.m_uVK_Paste);//ペーストキー
	//一時項目が無かったらストックモードOFF
	if(m_ini.m_fifo.m_nAllClearOff && !m_pTree->getOneTimeText(m_ini.m_fifo.m_nFifo)) toggleStockMode();
	m_nPhase = PHASE_IDOL;
}

//---------------------------------------------------
//関数名	toggleStockMode()
//機能		ストックモード切替
//---------------------------------------------------
void CCharu3App::toggleStockMode()
{
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CString strText;
		strText.Format(_T("toggleStockMode %d\n"),!m_isStockMode);
		CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
	}

	if(m_isStockMode) {
		m_isStockMode = false;
		KillTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE);
		UnregisterHotKey(NULL,HOTKEY_PASTE);
		m_pMainFrame->changeTrayIcon(m_isStockMode);
		if(m_ini.m_fifo.m_nOffClear)	m_pTree->clearOneTime(m_pTree->GetRootItem());//一時項目クリア
	}
	else {
		m_isStockMode = true;
		m_strlClipBackup = "";
		setAppendKeyInit(::GetForegroundWindow(),&m_keySet);
		RegisterHotKey(NULL,HOTKEY_PASTE,theApp.m_keySet.m_uMod_Paste,theApp.m_keySet.m_uVK_Paste);//ペーストキー
		//監視タイマーセット
		if(m_ini.m_etc.m_nActiveTime)
			SetTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE,m_ini.m_etc.m_nActiveTime,NULL);
		m_pMainFrame->changeTrayIcon(m_isStockMode);
	}
}

//---------------------------------------------------
//関数名	getPopupPos(POINT *pPos,int nPodType,HWND hTarget)
//機能		設定に応じたポップアップ位置を取得
//---------------------------------------------------
void CCharu3App::getPopupPos(POINT *pPos,int nPosType)
{
	RECT rectDeskTop;

	//ターゲットのハンドルを取るために実行
	CGeneral::getCaretPos(pPos,&m_focusInfo);//キャレット位置を取得

	//デスクトップのサイズを取得
	if(nPosType > POPUP_POS_MOUSE) {
		HWND hDeskTop = GetDesktopWindow();
		::GetWindowRect(hDeskTop,&rectDeskTop);
	}
	switch(nPosType) {
	case POPUP_POS_MOUSE:
		GetCursorPos(pPos);
		break;
	case POPUP_POS_LEFT_U:
		pPos->x = 0;
		pPos->y = 0;
		break;
	case POPUP_POS_RIGHT_U:
		pPos->x = rectDeskTop.right - m_ini.m_DialogSize.x;
		pPos->y = 0;
		break;
	case POPUP_POS_CENTOR:
		pPos->x = rectDeskTop.right / 2 - m_ini.m_DialogSize.x / 2;
		pPos->y = rectDeskTop.bottom / 2 - m_ini.m_DialogSize.y / 2;
		break;
	case POPUP_POS_LEFT_D:
		pPos->x = 0;
		pPos->y = rectDeskTop.bottom - m_ini.m_DialogSize.y;
		break;
	case POPUP_POS_RIGHT_D:
		pPos->x = rectDeskTop.right - m_ini.m_DialogSize.x;
		pPos->y = rectDeskTop.bottom - m_ini.m_DialogSize.y;
		break;
	case POPUP_POS_CARET:
		pPos->x += m_ini.m_pop.m_posCaretHosei.x;
		pPos->y += m_ini.m_pop.m_posCaretHosei.y;
		break;
	}
}

//---------------------------------------------------
//関数名	cornerPopup()
//機能		デスクトップ隅でのポップアップ処理
//---------------------------------------------------
bool CCharu3App::cornerPopup()
{
	//デスクトップのサイズを取得
	RECT rectDeskTop;
	HWND hDeskTop = GetDesktopWindow();
	::GetWindowRect(hDeskTop,&rectDeskTop);
	POINT pos;
	GetCursorPos(&pos);
	int nPosType = 0;
	if(m_ini.m_pop.m_nCornerPopup & 0x02) //左上
		if(pos.x  <= m_ini.m_pop.m_nCornerPopupPix && pos.y <= m_ini.m_pop.m_nCornerPopupPix) nPosType = POPUP_POS_LEFT_U;

	if(m_ini.m_pop.m_nCornerPopup & 0x04) //右上
		if(pos.x >= rectDeskTop.right - m_ini.m_pop.m_nCornerPopupPix && pos.y <= m_ini.m_pop.m_nCornerPopupPix) nPosType = POPUP_POS_RIGHT_U;
		
	if(m_ini.m_pop.m_nCornerPopup & 0x08) //左下
		if(pos.x  <= m_ini.m_pop.m_nCornerPopupPix && pos.y >= rectDeskTop.bottom - m_ini.m_pop.m_nCornerPopupPix) nPosType = POPUP_POS_LEFT_D;
		
	if(m_ini.m_pop.m_nCornerPopup & 0x10) //右下
		if(pos.x >= rectDeskTop.right - m_ini.m_pop.m_nCornerPopupPix && pos.y >= rectDeskTop.bottom - m_ini.m_pop.m_nCornerPopupPix) nPosType = POPUP_POS_RIGHT_D;
		
	if(nPosType) {
		if(!m_isCornerPopup) {
			m_isCornerPopup = true;
			return false;
		}
		getPopupPos(&pos,nPosType);
		popupTreeWindow(pos,m_ini.m_pop.m_nSelectSW);//ポップアップ
		m_isCornerPopup = false;
		return true;
	}
	else m_isCornerPopup = false;
	return false;
}

//---------------------------------------------------
//関数名	resetTreeDialog()
//機能		ツリーダイアログを再構築
//---------------------------------------------------
void  CCharu3App::resetTreeDialog()
{
	list<STRING_DATA> stringList;//データリスト

	//データの順番を正規化してコピー
	HTREEITEM hStartItem = m_pTree->GetRootItem();
	m_pTree->tree2List(hStartItem,&stringList,true);

	if(m_pTree)		delete m_pTree;
	if(m_pTreeDlg)	delete m_pTreeDlg;

	m_pTreeDlg = NULL;
	m_pTree = NULL;

	m_pTreeDlg = new CMyTreeDialog;
	m_pTree = new CCharu3Tree;
	if(m_pTree) {
		m_pTreeDlg->setTree(m_pTree);
		m_pTreeDlg->Create(IDD_POPUP_DIALOG,this->m_pMainWnd);

		m_pTree->setImageList(m_ini.m_IconSize,theApp.m_ini.m_visual.m_strResourceName,m_ini.m_strAppPath);
		m_pTree->setInitInfo(&m_ini.m_nTreeID,&m_ini.m_nSelectID,&m_ini.m_nRecNumber);//ID初期値を設定

		//データ互換プラグインを初期化
		if(!m_pTree->setPlugin(m_ini.m_strRwPluginFolder)){
			m_ini.m_strRwPluginFolder = m_ini.m_strAppPath + "rw_plugin";
			m_pTree->setPlugin(m_ini.m_strRwPluginFolder);
			m_ini.writeEnvInitData();
		}
		m_pTree->m_MyStringList = stringList;
		m_pTree->CWnd::LockWindowUpdate();
		m_pTree->copyData(ROOT,TVI_ROOT,&m_pTree->m_MyStringList);//ツリーにデータをセット
		m_pTree->CWnd::UnlockWindowUpdate();

		setAppendHotKey();
	}
}

//---------------------------------------------------
// CCharu3App メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL CCharu3App::PreTranslateMessage(MSG* pMsg) 
{
	//キーフック処理
	if(pMsg->message == WM_KEY_HOOK) {
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("WM_KEY_HOOK %d %d\n"),pMsg->wParam,pMsg->lParam);
			CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
		UINT uMod = 0;
		if(m_ini.m_pop.m_nDoubleKeyPOP) {
			switch(m_ini.m_pop.m_nDoubleKeyPOP) {
				case 1:	uMod = MOD_SHIFT;break;
				case 2:	uMod = MOD_CONTROL;break;
				case 3:	uMod = MOD_ALT;break;
			}
			if(CGeneral::mod2VK(uMod) == pMsg->wParam) {
				pMsg->message = WM_HOTKEY;
				pMsg->wParam  = HOTKEY_POPUP;
			}
		}
		if(m_ini.m_pop.m_nDoubleKeyFIFO) {
			switch(m_ini.m_pop.m_nDoubleKeyFIFO) {
				case 1:	uMod = MOD_SHIFT;break;
				case 2:	uMod = MOD_CONTROL;break;
				case 3:	uMod = MOD_ALT;break;
			}
			if(CGeneral::mod2VK(uMod) == pMsg->wParam) {
				pMsg->message = WM_HOTKEY;
				pMsg->wParam  = HOTKEY_FIFO;
			}
		}
		//フォルダホットキー処理
		int nSize = m_hotkeyVector.size();
		for(int i = 0; i < nSize; i++) {
			if(m_hotkeyVector[i].m_isDoubleClick) {
				if(CGeneral::mod2VK(m_hotkeyVector[i].m_uModKey) == pMsg->wParam) {
					pMsg->message = WM_HOTKEY;
					pMsg->wParam  = HOT_ITEM_KEY + i;
					break;
				}
			}
		}
	}
	
	if(m_nPhase == PHASE_IDOL && pMsg->message == WM_TIMER) {
		//監視タイマー処理
		if(pMsg->wParam == TIMER_ACTIVE && m_isStockMode) {
			if(setAppendKeyInit(::GetForegroundWindow(),&m_keySet)) {
				UnregisterHotKey(NULL,HOTKEY_PASTE);
				RegisterHotKey(NULL,HOTKEY_PASTE,theApp.m_keySet.m_uMod_Paste,theApp.m_keySet.m_uVK_Paste);
			}
		}
		//自己診断タイマー
		else if(pMsg->wParam == TIMER_SELF_DIAGNOSIS) {
			m_clipbord.resetClipView(this->m_pMainWnd->m_hWnd);
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CGeneral::writeLog(m_ini.m_strDebugLog,_T("TIMER_SELF_DIAGNOSIS\n"),_ME_NAME_,__LINE__);
			}
		}
		//マウス監視タイマー
		else if(pMsg->wParam == TIMER_MOUSE) {
			if(cornerPopup()) return FALSE;
		}
	}
	//ホットキー処理
	if(pMsg->message == WM_HOTKEY) {
		switch(pMsg->wParam) {
		case HOTKEY_POPUP://ポップアップ
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CGeneral::writeLog(m_ini.m_strDebugLog,_T("HOTKEY_POPUP\n"),_ME_NAME_,__LINE__);
			}
			if(m_ini.m_pop.m_nDoubleKeyPOP) {//ダブルキークリック
				DWORD dwTime = timeGetTime();
				if((dwTime - m_dwDoubleKeyPopTime) > m_ini.m_pop.m_nDCKeyPopTime) {
					m_dwDoubleKeyPopTime = dwTime;
					return FALSE;
				}
			}
			if(m_nPhase == PHASE_IDOL) {
				POINT pos;
				getPopupPos(&pos,m_ini.m_pop.m_nPopupPos);//ポップアップ位置を取得
				if(m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("getPopupPos %d %d\n"),pos.x,pos.y);
					CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}
				if(m_ini.m_pop.m_nOutRevice)	reviseWindowPos(&pos);
				if(m_ini.m_nDebug) {
					CString strText;
					strText.Format(_T("reviseWindowPos %d %d\n"),pos.x,pos.y);
					CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
				}
				popupTreeWindow(pos,m_ini.m_pop.m_nSelectSW);//ポップアップ
			}
			else	m_isCloseKey = true;//閉じるスイッチを入れておく
			return FALSE;
			break;
		case HOTKEY_FIFO://ストックモード切り替え
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CGeneral::writeLog(m_ini.m_strDebugLog,_T("HOTKEY_FIFO\n"),_ME_NAME_,__LINE__);
			}
			if(m_ini.m_pop.m_nDoubleKeyFIFO) {//ダブルキークリック
				DWORD dwTime = timeGetTime();
				if((dwTime - m_dwDoubleKeyFifoTime) > m_ini.m_pop.m_nDCKeyFifoTime) {
					m_dwDoubleKeyFifoTime = dwTime;
					return FALSE;
				}
			}
			toggleStockMode();
			return FALSE;
			break;
		case HOTKEY_PASTE://履歴FIFO処理
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CGeneral::writeLog(m_ini.m_strDebugLog,_T("HOTKEY_PASTE\n"),_ME_NAME_,__LINE__);
			}
			if(m_ini.m_etc.m_nWordBugSW && !m_ini.m_fifo.m_nDuplication) m_isWordBugClipCheck = true;

			fifoClipbord();
			return FALSE;
			break;
		//ホットアイテム
		default:
			if(m_nPhase == PHASE_IDOL) {
				int nTarget = pMsg->wParam - HOT_ITEM_KEY;
				playHotItem(nTarget);
				return FALSE;
			}
			break;
		}
	}
	//閉じるスイッチが入ってて、キーアップの時にポップアップを閉じる
	//(ALTの時に対象アプリのメニューが開いちゃうのを防ぐ)
	else if((pMsg->message == WM_KEYUP || pMsg->message == WM_SYSKEYUP) && m_isCloseKey) {
		m_pTreeDlg->PostMessage(WM_TREE_CLOSE,IDCANCEL,NULL);
		m_isCloseKey = false;
	}
	//ポップアップから閉じる命令が来たので閉じる
	if(pMsg->message == WM_TREE_CLOSE && m_nPhase == PHASE_POPUP) {
		closeTreeWindow(pMsg->wParam);
		return FALSE;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

//---------------------------------------------------
//関数名	OnAbout()
//機能		Aboutダイアログ
//---------------------------------------------------
void CCharu3App::OnAbout() 
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//---------------------------------------------------
//関数名	OnHelp()
//機能		ヘルプ
//---------------------------------------------------
void CCharu3App::OnHelp() 
{
	CString strFileName;
	strFileName = m_ini.m_strAppPath + HELP_FILE;
	ShellExecute(NULL,NULL,strFileName,NULL,NULL,SW_SHOWNORMAL);
}

//---------------------------------------------------
//関数名	OnExit()
//機能		終了処理
//---------------------------------------------------
void CCharu3App::OnExit() 
{
	stopAppendHotKey();//追加ホットキーを停止
	stopHotkey();
	//タイマー停止
	KillTimer(m_pMainWnd->m_hWnd,TIMER_SELF_DIAGNOSIS);
	KillTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE);
	KillTimer(m_pMainWnd->m_hWnd,TIMER_MOUSE);

	if(m_hLangDll)	FreeLibrary(m_hLangDll);
	m_ini.unHookKey();
	m_pTree->saveDataFile(m_ini.m_strDataFile,m_ini.m_strPluginName,NULL);
	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CGeneral::writeLog(m_ini.m_strDebugLog,_T("OnExit\n"),_ME_NAME_,__LINE__);
	}
}

//---------------------------------------------------
//関数名	OnOption()
//機能		設定
//---------------------------------------------------
void CCharu3App::OnOption() 
{
	if(m_nPhase != PHASE_IDOL && m_nPhase != PHASE_POPUP) return;
	int nPhase = m_nPhase;
	m_nPhase = PHASE_OPTION;

	//タイマー停止
	KillTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE);
	KillTimer(m_pMainWnd->m_hWnd,TIMER_SELF_DIAGNOSIS);
	KillTimer(m_pMainWnd->m_hWnd,TIMER_MOUSE);

	COptMainDialog dlgOption(NULL,m_ini.m_nOptionPage);
	CInit iniBkup;

	iniBkup = m_ini;

	if(nPhase == PHASE_IDOL)	m_ini.unHookKey();
	stopAppendHotKey();//追加ホットキーを停止
	stopHotkey();

	//デバッグログ処理
	if(m_ini.m_nDebug) {
		CGeneral::writeLog(m_ini.m_strDebugLog,_T("OnOption\n"),_ME_NAME_,__LINE__);
	}
	int ret = dlgOption.DoModal();
	if(ret == IDOK) {
		m_ini.writeAllInitData();
		//デバッグログ処理
		if(m_ini.m_nDebug) {
			CGeneral::writeLog(m_ini.m_strDebugLog,_T("writeAllInitData\n"),_ME_NAME_,__LINE__);
		}
	}
	else m_ini = iniBkup;

	if(m_ini.m_visual.m_nResetTree && nPhase != PHASE_POPUP && iniBkup.m_visual.m_strResourceName != m_ini.m_visual.m_strResourceName)
		resetTreeDialog();//ツリー再構築

	registerHotkey();
	setAppendHotKey();//追加ホットキーを設定
	if(nPhase == PHASE_IDOL)	m_ini.setHookKey(m_hSelfWnd);

	//自己診断タイマーセット
	if(m_ini.m_etc.m_nSelfDiagnosisTime)
		SetTimer(m_pMainWnd->m_hWnd,TIMER_SELF_DIAGNOSIS,m_ini.m_etc.m_nSelfDiagnosisTime,NULL);
	//監視タイマーセット アイドル状態でストックモード中なら
	if(m_nPhase == PHASE_IDOL && m_isStockMode && m_ini.m_etc.m_nActiveTime)
		SetTimer(m_pMainWnd->m_hWnd,TIMER_ACTIVE,m_ini.m_etc.m_nActiveTime,NULL);

	//マウス監視タイマー
	if(m_ini.m_pop.m_nCornerPopup)
		SetTimer(m_pMainWnd->m_hWnd,TIMER_MOUSE,m_ini.m_pop.m_nCornerPopupTime,NULL);

	m_hActiveKeyWnd = NULL;
	m_nPhase = nPhase;
}

//---------------------------------------------------
//関数名	OnDataSave()
//機能		別名保存
//---------------------------------------------------
void CCharu3App::OnDataSave() 
{
	CFileDialog *pFileDialog;
	CString strFilter;
	strFilter.LoadString(APP_INF_FILE_FILTER);
	pFileDialog = new CFileDialog(FALSE,_T("*.c3d"),NULL,NULL,strFilter + _T("||"));

	if(pFileDialog) {
		if(IDOK == pFileDialog->DoModal()) {
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("OnDataSave \"%s\"\n"),pFileDialog->GetPathName());
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}
			if(!m_pTree->saveDataFile(pFileDialog->GetPathName(),DAT_FORMAT,NULL)) {
				CString strRes;
				strRes.LoadString(APP_MES_FAILURE_DATA_SAVE);
				AfxMessageBox(strRes,MB_ICONEXCLAMATION,0);
			}
		}
		delete pFileDialog;
	}
}

//---------------------------------------------------
//関数名	OnOnetimeClear() 
//機能		一時項目をクリア
//---------------------------------------------------
void CCharu3App::OnOnetimeClear() 
{
	m_pTree->clearOneTime(m_pTree->GetRootItem());	
}

//---------------------------------------------------
//関数名	OnAllLock()
//機能		一時項目を全てロック
//---------------------------------------------------
void CCharu3App::OnAllLock() 
{
	m_pTree->clearOneTime(m_pTree->GetRootItem(),KIND_LOCK);	
}

//---------------------------------------------------
//関数名	OnChangData()
//機能		データファイル切り替え
//---------------------------------------------------
void CCharu3App::OnChangData() 
{
	CString strFilter,strBuff;

	strFilter = "";
	RW_PLUGIN plugin;
	vector<RW_PLUGIN>::iterator it;
	CString strRes;
	for(it = m_pTree->m_rwPlugin.begin(); it != m_pTree->m_rwPlugin.end(); it++) {
		strRes.LoadString(APP_INF_FILE_FILTER2);
		strBuff.Format(strRes,it->m_strSoftName,it->m_strExtension,it->m_strExtension);
		strFilter = strFilter + strBuff;
	}

	CMyFileDialog *pFileDialog;

	CString strFilterC3;
	strFilterC3.LoadString(APP_INF_FILE_FILTER);
	pFileDialog = new CMyFileDialog(TRUE,_T("c3d"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilterC3 + _T("|") + strFilter,NULL,true);

	if(pFileDialog) {
		pFileDialog->m_ofn.lpstrInitialDir = m_ini.m_strAppPath;
		strRes.LoadString(APP_INF_CHECKBOX_MACRO);
		if(IDOK == pFileDialog->DoModal(TRUE,strRes)) {
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("OnChangData \"%s\"\n"),pFileDialog->GetPathName());
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}
			
			CString strNameBkup,strPlugBkup;
			strNameBkup = m_ini.m_strDataFile;
			m_ini.m_strDataFile = pFileDialog->GetPathName();

			//適合プラグインを検索
			bool isFound = false;
			OPENFILENAME fileName;
			fileName = pFileDialog->getFileName();

			//ソフト名を設定
			strPlugBkup = m_ini.m_strPluginName;
			if(fileName.nFilterIndex == 1)
				m_ini.m_strPluginName = DAT_FORMAT;
			else
				m_ini.m_strPluginName = m_pTree->m_rwPlugin[fileName.nFilterIndex - 2].m_strSoftName;

			if(m_pTree->loadDataFileDef(m_ini.m_strDataFile,m_ini.m_strPluginName)) {	
				//マクロプラグイン処理
				if(pFileDialog->m_isAutoMacro) 	m_ini.m_strMacroPluginName = m_ini.m_strPluginName;
			}
			else {
				strRes.LoadString(APP_MES_UNKNOWN_FORMAT);
				AfxMessageBox(strRes,MB_ICONEXCLAMATION,0);
				m_ini.m_strDataFile = strNameBkup;
				m_ini.m_strPluginName = strPlugBkup;
				m_pTree->loadDataFileDef(m_ini.m_strDataFile,m_ini.m_strPluginName);
			}
			//状態を保存
			m_ini.writeEnvInitData();
		}
		delete pFileDialog;
	}	
}

//---------------------------------------------------
//関数名	OnAddData()
//機能		データ追加
//---------------------------------------------------
void CCharu3App::OnAddData() 
{
	CAddDialog addDialog(NULL,NULL);
	addDialog.setMacroTempate(&m_ini.m_vctMacro,&m_ini.m_vctDataMacro);
	//追加ダイアログを開く
	int nRet = addDialog.DoModal();
}

//---------------------------------------------------
//関数名	OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
//機能		すでに割り当てられているメッセージがない場合
//---------------------------------------------------
BOOL CCharu3App::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(pHandlerInfo == NULL) {	//　すでに割り当てられているメッセージがない場合……
		if(nCode == CN_COMMAND) {
			int nMax = m_pTree->m_rwPlugin.size();//データ数を取得
			for(int i = 0; i <= nMax; i++) {
				if(nID == (UINT)(IDM_MACRO_CHARU + i)) {//適合プラグインを設定
					if(i == 0) m_ini.m_strMacroPluginName = DAT_FORMAT;
					else {
						m_ini.m_strMacroPluginName = m_pTree->m_rwPlugin[i-1].m_strSoftName;
					}
					m_ini.writeEnvInitData();
					break;
				}
			}
		}
	}
	
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//---------------------------------------------------
//関数名	OnIconDecide()  
//機能		アイコンを自動判別
//---------------------------------------------------
void CCharu3App::OnIconDecide() 
{
	CString strRes;
	strRes.LoadString(APP_MES_DECIDE_ICONS);
	int nRet = AfxMessageBox(strRes,MB_YESNO|MB_ICONEXCLAMATION|MB_APPLMODAL);

	if(nRet == IDYES) {
		m_pTree->allIconCheck();
	}		
}

//---------------------------------------------------
//関数名	OnBbsOpen()
//機能		サポート掲示板を開く
//---------------------------------------------------
void CCharu3App::OnBbsOpen() 
{
	ShellExecute(NULL,NULL,m_ini.m_strBBS,NULL,NULL,SW_SHOWNORMAL);
	
}

//---------------------------------------------------
//関数名	OnStockStop()
//機能		ストックモード切替
//---------------------------------------------------
void CCharu3App::OnStockStop() 
{
	toggleStockMode();
}

//---------------------------------------------------
//関数名	OnVisualFile() 
//機能		ビジュアル設定を読み込み
//---------------------------------------------------
void CCharu3App::OnVisualFile() 
{
	CString strFileName,strIniData;

	CFileDialog *pFileDialog;
	CString strRes;
	strRes.LoadString(APP_INF_FILE_FILTER3);
	pFileDialog = new CFileDialog(TRUE,_T("ini"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strRes,NULL);

	CInit iniBkup;
	iniBkup = m_ini;

	if(pFileDialog) {
		pFileDialog->m_ofn.lpstrInitialDir = m_ini.m_strAppPath;
		if(IDOK == pFileDialog->DoModal()) {
			//デバッグログ処理
			if(m_ini.m_nDebug) {
				CString strText;
				strText.Format(_T("OnVisualFile \"%s\"\n"),pFileDialog->GetPathName());
				CGeneral::writeLog(m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
			}
			strFileName = pFileDialog->GetPathName();

			TCHAR strBuff[256];
			FILE *inPut;

			if((inPut = _tfopen(strFileName, _T("r"))) != NULL) {
				strIniData = _T("");
				while(!feof(inPut)) {
					_fgetts(strBuff,sizeof(strBuff),inPut);
					strIniData = strIniData + strBuff;
				}
				fclose(inPut);

				CString strTmp;
				int nValue;
				strTmp = m_pTree->getDataOptionStr(strIniData,_T("iconfile"));//アイコンファイル名
				if(strTmp != "") m_ini.m_visual.m_strResourceName = strTmp;

				strTmp = m_pTree->getDataOptionStr(strIniData,_T("fontname"));//フォント名
				if(strTmp != "") m_ini.m_visual.m_strFontName = strTmp;
				nValue = m_pTree->getDataOption(strIniData,_T("fontsize"));//フォントサイズ
				if(nValue > -1) m_ini.m_visual.m_nFontSize = nValue;

				nValue = m_pTree->getDataOptionHex(strIniData,_T("backcolor"));
				if(nValue > -1) m_ini.m_visual.m_nBackColor = nValue;
				nValue = m_pTree->getDataOptionHex(strIniData,_T("bordercolor"));
				if(nValue > -1) m_ini.m_visual.m_nBorderColor = nValue;
				nValue = m_pTree->getDataOptionHex(strIniData,_T("textcolor"));
				if(nValue > -1) m_ini.m_visual.m_nTextColor = nValue;
				m_ini.writeAllInitData();
			}
		}
		delete pFileDialog;

		if(m_ini.m_visual.m_nResetTree && iniBkup.m_visual.m_strResourceName != m_ini.m_visual.m_strResourceName)
			resetTreeDialog();//ツリー再構築
	}
}

//---------------------------------------------------
//関数名	resetTreeDialog()
//機能		ツリー再構築
//---------------------------------------------------
void CCharu3App::OnResetTree() 
{
	resetTreeDialog();//ツリー再構築
}


