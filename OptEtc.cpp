/*----------------------------------------------------------
	一般設定クラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "OptEtc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	COptEtc
//機能		コンストラクタ
//---------------------------------------------------
COptEtc::COptEtc(CWnd* pParent /*=NULL*/) : CDialog(COptEtc::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptEtc)
	//}}AFX_DATA_INIT
}


//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptEtc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptEtc)
	//}}AFX_DATA_MAP
	if(GetDlgItem(IDC_OPT_SELF_TIME)) {
		DDX_Text(pDX, IDC_OPT_SELF_TIME, theApp.m_ini.m_etc.m_nSelfDiagnosisTime);
		DDV_MinMaxUInt(pDX, theApp.m_ini.m_etc.m_nSelfDiagnosisTime, 0, 1000000);
	}
	if(GetDlgItem(IDC_OPT_ICON_POP))
		DDX_Radio(pDX, IDC_OPT_ICON_POP,theApp.m_ini.m_etc.m_nIconClick);
	if(GetDlgItem(IDC_OPT_TOCLIP))
		DDX_Check(pDX,IDC_OPT_TOCLIP,theApp.m_ini.m_etc.m_nToClip);
	if(GetDlgItem(IDC_OPT_TOOLTIP_TITLE))
		DDX_Check(pDX,IDC_OPT_TOOLTIP_TITLE,theApp.m_ini.m_etc.m_nToolTip);
	if(GetDlgItem(IDC_OPT_WORD_BUG))
		DDX_Check(pDX,IDC_OPT_WORD_BUG,theApp.m_ini.m_etc.m_nWordBugSW);
	if(GetDlgItem(IDC_OPT_MINIMIZ))
		DDX_Check(pDX,IDC_OPT_MINIMIZ,theApp.m_ini.m_etc.m_nMinimization);
}


BEGIN_MESSAGE_MAP(COptEtc, CDialog)
	//{{AFX_MSG_MAP(COptEtc)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptEtc メッセージ ハンドラ
//---------------------------------------------------

BOOL COptEtc::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0){
		::PostMessage(::GetParent(this->m_hWnd),pMsg->message,VK_PRIOR,pMsg->lParam);
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void COptEtc::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)	GetDlgItem(IDC_OPT_TOCLIP)->SetFocus();
}
