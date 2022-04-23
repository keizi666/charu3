/*----------------------------------------------------------
	MainFrmクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "General.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
// CMainFrame
//---------------------------------------------------

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_WM_TIMER()
	ON_WM_QUERYENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//---------------------------------------------------
//関数名	CMainFrame
//機能		コンストラクタ
//---------------------------------------------------
CMainFrame::CMainFrame()
{
	//メニューを原則有効化
	CFrameWnd::m_bAutoMenuEnable = FALSE;
	
}

//---------------------------------------------------
//関数名	~CMainFrame
//機能		デストラクタ
//---------------------------------------------------
CMainFrame::~CMainFrame()
{
}

//---------------------------------------------------
//関数名	OnCreate(LPCREATESTRUCT lpCreateStruct)
//機能		初期化
//---------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// フレームのクライアント領域全体を占めるビューを作成します。
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	//アイコン処理
#if true
	m_hIcon = (HICON)LoadImage(theApp.m_hInstance,MAKEINTRESOURCE(IDI_RUN256),IMAGE_ICON,16,16,0);//スモールアイコン
	m_hStopIcon  = (HICON)LoadImage(theApp.m_hInstance,MAKEINTRESOURCE(IDI_STOP256),IMAGE_ICON,16,16,0);//スモールアイコン
#else
	m_hIcon = (HICON)LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDI_RUN), IMAGE_ICON, 16, 16, 0);//スモールアイコン
	m_hStopIcon = (HICON)LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(IDI_STOP), IMAGE_ICON, 16, 16, 0);//スモールアイコン
#endif
	//タスクトレイアイコンを設定する
	m_nIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_nIcon.uID = 4;
	m_nIcon.hWnd = m_hWnd;
	m_nIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nIcon.hIcon = m_hStopIcon;
	m_nIcon.uCallbackMessage = WM_TASKTRAY;

	_tcscpy_s(m_nIcon.szTip,_T("Charu3"));
	Shell_NotifyIcon(NIM_ADD,&m_nIcon);

	return 0;
}

//---------------------------------------------------
//関数名	checkTrayPos()
//機能		カーソルがタスクトレイに重なってるか判定
//---------------------------------------------------
bool CMainFrame::checkTrayPos()
{
	HWND hTaskBarWnd;
	HWND hTrayNotifyWnd;
	RECT rtTrayNotify;
	POINT ptCursolPos;
	bool isRet = false;

	GetCursorPos(&ptCursolPos);

	// タスクバーのインジケータ領域の矩形を得る
	hTaskBarWnd = ::FindWindow(_T(SZTASKBAR_CLASS),NULL);
	hTrayNotifyWnd = FindWindowEx(hTaskBarWnd,NULL,_T(SZTRAY_CLASS),NULL)->GetSafeHwnd();
	if (hTrayNotifyWnd == NULL)	return FALSE;

	ZeroMemory(&rtTrayNotify,sizeof(rtTrayNotify));
	::GetWindowRect(hTrayNotifyWnd ,&rtTrayNotify);

	if(ptCursolPos.x > rtTrayNotify.left && ptCursolPos.x < rtTrayNotify.right
		&& ptCursolPos.y > rtTrayNotify.top && ptCursolPos.y < rtTrayNotify.bottom)
		isRet = true;

	return isRet;
}

//---------------------------------------------------
//関数名	setMenu()
//機能		メニュー初期化
//---------------------------------------------------
void CMainFrame::setMenu()
{
	m_PopupMenu.LoadMenu(MAKEINTRESOURCE(IDR_MAINFRAME));//メニュークラスにメニューを読む
	//マクロプラグインメニューを構築
	if(theApp.m_ini.m_strMacroPluginName == DAT_FORMAT)
		m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU,MF_CHECKED);
	else
		m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU,MF_UNCHECKED);

	int nMax = theApp.m_pTree->m_rwPlugin.size();//データ数を取得
	for(int i = 0; i < nMax; i++) {
		m_PopupMenu.InsertMenu (IDM_MACRO_CHARU,MF_STRING,IDM_MACRO_CHARU + i + 1,theApp.m_pTree->m_rwPlugin[i].m_strSoftName);
		if(theApp.m_ini.m_strMacroPluginName == theApp.m_pTree->m_rwPlugin[i].m_strSoftName) {
			m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU + i + 1,MF_CHECKED);
		}
	}
	m_PopupMenu.InsertMenu (IDM_MACRO_CHARU,MF_SEPARATOR);
}

//---------------------------------------------------
//関数名	PreCreateWindow(CREATESTRUCT& cs)
//機能		初期化前処理
//---------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )	return FALSE;

	cs.cx = 185;
	cs.cy = 256;
	cs.x = -500;
	cs.y = -500;

	cs.style = WS_TILED | WS_MINIMIZEBOX | WS_SYSMENU;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	
	return TRUE;
}

//---------------------------------------------------
//関数名	changeClip()
//機能		クリップボードの変更処理
//---------------------------------------------------
void CMainFrame::changeClip()
{
	CString strClipBord;
	theApp.m_clipbord.getClipboardText(strClipBord);//クリップボードの内容を取得
	theApp.changeClipBord(strClipBord);//本体にクリップボードの変更を通知

	if(!theApp.m_ini.m_etc.m_nToolTip) {
		//タスクトレイのツールヒントを変更
		strClipBord = strClipBord.Left(1024);
		strClipBord.Replace(_T("	"),_T("･"));//タブを置換
		_tcscpy_s(m_nIcon.szTip,strClipBord.Left(63));
		if(!Shell_NotifyIcon(NIM_MODIFY,&m_nIcon)){
			Shell_NotifyIcon(NIM_ADD,&m_nIcon);
		}
	}

}

//---------------------------------------------------
//関数名	changeTrayIcon(bool isStock)
//機能		トレイアイコン切り替え
//---------------------------------------------------
void CMainFrame::changeTrayIcon(bool isStock)
{
	if(isStock) m_nIcon.hIcon = m_hIcon;
	else		m_nIcon.hIcon = m_hStopIcon;
	if(!Shell_NotifyIcon(NIM_MODIFY,&m_nIcon))	Shell_NotifyIcon(NIM_ADD,&m_nIcon);
}

//---------------------------------------------------
// CMainFrame クラスの診断
//---------------------------------------------------

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

//---------------------------------------------------
//関数名	OnSetFocus(CWnd* pOldWnd)
//機能		フォーカスを与える
//---------------------------------------------------
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// ビュー ウィンドウにフォーカスを与えます。
	m_wndView.SetFocus();
}

//---------------------------------------------------
//関数名	OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//機能		コマンド処理
//---------------------------------------------------
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 処理されなかった場合にはデフォルトの処理を行います。
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//---------------------------------------------------
//関数名	WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
//機能		メッセージ処理
//---------------------------------------------------
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	//タスクトレイ処理
	if(message == WM_TASKTRAY && theApp.getPhase() == PHASE_IDOL) {
		if(lParam == WM_RBUTTONUP) {//メニューを出す
			//マクロプラグインメニューを構築
			int nMax = theApp.m_pTree->m_rwPlugin.size();//データ数を取得
			if(theApp.m_ini.m_strMacroPluginName == DAT_FORMAT)
				m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU,MF_CHECKED);
			else
				m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU,MF_UNCHECKED);
			for(int i = 0; i < nMax; i++) {
				if(theApp.m_ini.m_strMacroPluginName == theApp.m_pTree->m_rwPlugin[i].m_strSoftName)
					m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU + i + 1,MF_CHECKED);
				else
					m_PopupMenu.CheckMenuItem(IDM_MACRO_CHARU + i + 1,MF_UNCHECKED);
			}

			POINT point;
			GetCursorPos(&point);
			this->SetForegroundWindow();

			m_PopupMenu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, this);
			PostMessage(0,0,0);
			return FALSE;//タスクトレイのメニューも出ちゃうので追加
		}
		//左クリック
		else if(lParam == WM_LBUTTONDOWN) {
			if(theApp.m_ini.m_etc.m_nIconClick == 0) {
				if(theApp.getPhase() == PHASE_IDOL) {
					theApp.popupTreeWinMC(m_hActive);//ポップアップを出す
				}
			}
			else
				theApp.toggleStockMode();//ストックモード切替

		}
		else if(checkTrayPos()) {//カーソルがアイコンに重なった
			m_hActive = ::GetForegroundWindow();
			if(m_hActive != theApp.m_focusInfo.m_hActiveWnd) {
				CGeneral::getFocusInfo(&theApp.m_focusInfo);
			}
		}
	}
	//クリップボードチェインの変更を検知
  	if(message == WM_CHANGECBCHAIN) {
		//デバッグログ処理
		if(theApp.m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("WM_CHANGECBCHAIN wParam:%x lParam:%x nowNext:%x\n"),wParam,lParam,theApp.m_clipbord.getNextCb());
			CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}

		if(theApp.m_clipbord.getNextCb() == (HWND)wParam){
			theApp.m_clipbord.setNextCb((HWND)lParam);

		}
		else if(m_hWnd != theApp.m_clipbord.getNextCb() && theApp.m_clipbord.getNextCb())
			::SendMessage(theApp.m_clipbord.getNextCb(),message,wParam,lParam);
	}
	//クリップボードの変更を検知
 	if(message == WM_DRAWCLIPBOARD) {
		//デバッグログ処理
		if(theApp.m_ini.m_nDebug) {
			CString strText;
			strText.Format(_T("WM_DRAWCLIPBOARD wParam:%x lParam:%x nowNext:%x\n"),wParam,lParam,theApp.m_clipbord.getNextCb());
			CGeneral::writeLog(theApp.m_ini.m_strDebugLog,strText,_ME_NAME_,__LINE__);
		}
		//Wordバグ回避
		if(theApp.m_ini.m_etc.m_nWordBugSW)	{
			PostMessage(WM_WORD_CLIP,NULL,NULL);
		}
		else 								changeClip();
		if(m_hWnd != theApp.m_clipbord.getNextCb() && theApp.m_clipbord.getNextCb()) {//次のチェインにメッセージを渡す
			::SendMessage(theApp.m_clipbord.getNextCb(), message, wParam, lParam);
		}
	}
	//ワードバグ用メッセージハンドラ
	if(message == WM_WORD_CLIP) {
		Sleep(150);
		changeClip();
	}
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

//---------------------------------------------------
//関数名	OnExit()
//機能		終了処理
//---------------------------------------------------
void CMainFrame::OnExit() 
{
	theApp.m_clipbord.delClipView(m_hWnd);
	theApp.OnExit();
	Shell_NotifyIcon(NIM_DELETE,&m_nIcon);
	CFrameWnd::DestroyWindow();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	if(nIDEvent == TIMER_SELF_DIAGNOSIS){
		if(!Shell_NotifyIcon(NIM_MODIFY,&m_nIcon)){
			Shell_NotifyIcon(NIM_ADD,&m_nIcon);
		}
	}	
	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	
	theApp.m_clipbord.delClipView(m_hWnd);
	theApp.OnExit();
	Shell_NotifyIcon(NIM_DELETE,&m_nIcon);
	
	return TRUE;
}
