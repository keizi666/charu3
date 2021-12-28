/*----------------------------------------------------------
	履歴FIFO設定クラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "OptFifo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	COptFifo
//機能		コンストラクタ
//---------------------------------------------------
COptFifo::COptFifo(CWnd* pParent /*=NULL*/) : CDialog(COptFifo::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptFifo)
	//}}AFX_DATA_INIT
}


//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptFifo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptFifo)
	//}}AFX_DATA_MAP
	if(GetDlgItem(IDC_OPT_WAVE_NAME))
		DDX_Text(pDX, IDC_OPT_WAVE_NAME, theApp.m_ini.m_fifo.m_strWaveFile);
	if(GetDlgItem(IDC_OPT_FIFO_NON))
		DDX_Radio(pDX, IDC_OPT_FIFO_NON,theApp.m_ini.m_fifo.m_nFifo);
	if(GetDlgItem(IDC_OPT_FIFO_WAVE))
		DDX_Check(pDX,IDC_OPT_FIFO_WAVE,theApp.m_ini.m_fifo.m_nCopySound);
	if(GetDlgItem(IDC_OPT_CLEAR_ONETIME))
		DDX_Check(pDX,IDC_OPT_CLEAR_ONETIME,theApp.m_ini.m_fifo.m_nOffClear);
	if(GetDlgItem(IDC_OPT_NO_CLEAR_ONETIME))
		DDX_Check(pDX,IDC_OPT_NO_CLEAR_ONETIME,theApp.m_ini.m_fifo.m_nNoClear);
	if(GetDlgItem(IDC_OPT_PASTE_SOUND))
		DDX_Check(pDX,IDC_OPT_PASTE_SOUND,theApp.m_ini.m_fifo.m_nPasteSound);
	if(GetDlgItem(IDC_ALL_DEL_OFF))
		DDX_Check(pDX,IDC_ALL_DEL_OFF,theApp.m_ini.m_fifo.m_nAllClearOff);
	if(GetDlgItem(IDC_OPT_DUPLICATION))
		DDX_Check(pDX,IDC_OPT_DUPLICATION,theApp.m_ini.m_fifo.m_nDuplication);
}


BEGIN_MESSAGE_MAP(COptFifo, CDialog)
	//{{AFX_MSG_MAP(COptFifo)
	ON_BN_CLICKED(IDC_FILE, OnFile)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptFifo メッセージ ハンドラ
//---------------------------------------------------


//---------------------------------------------------
//関数名	OnFile()
//機能		参照ボタン
//---------------------------------------------------
void COptFifo::OnFile() 
{
	CString strRes;
	strRes.LoadString(APP_INF_FILE_FILTER4);
	CFileDialog openFileDialog(TRUE,_T("wav"),NULL,OFN_FILEMUSTEXIST,strRes);

	openFileDialog.m_ofn.lpstrInitialDir = theApp.m_ini.m_strAppPath;
	if(openFileDialog.DoModal() == IDOK)
		SetDlgItemText(IDC_OPT_WAVE_NAME,openFileDialog.GetPathName());		
}

BOOL COptFifo::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0){
		::PostMessage(::GetParent(this->m_hWnd),pMsg->message,VK_PRIOR,pMsg->lParam);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void COptFifo::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)	GetDlgItem(IDC_OPT_FIFO_NON)->SetFocus();
	
}
