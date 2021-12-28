/*----------------------------------------------------------
	ポップアップ設定クラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "OptPopup.h"
#include "General.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	COptPopup
//機能		コンストラクタ
//---------------------------------------------------
COptPopup::COptPopup(CWnd* pParent /*=NULL*/) : CDialog(COptPopup::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptPopup)
	m_nPopupCombo = -1;
	//}}AFX_DATA_INIT
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptPopup)
	//}}AFX_DATA_MAP
	if(GetDlgItem(IDC_OPT_FIFO_KEY))
		DDX_Control(pDX, IDC_OPT_FIFO_KEY, m_ctrlFifoKey);
	if(GetDlgItem(IDC_OPT_POPUPKEY))
		DDX_Control(pDX, IDC_OPT_POPUPKEY, m_ctrlPopupKey);

	if(GetDlgItem(IDC_OPT_POPUP_POS))
		DDX_CBIndex(pDX,IDC_OPT_POPUP_POS,theApp.m_ini.m_pop.m_nPopupPos);

	if(GetDlgItem(IDC_OPT_SELECT_SAVE))
		DDX_Check(pDX,IDC_OPT_SELECT_SAVE,theApp.m_ini.m_pop.m_nSelectSW);

	if(GetDlgItem(IDC_OPT_FOLDER_OPEN))
		DDX_Check(pDX,IDC_OPT_FOLDER_OPEN,theApp.m_ini.m_pop.m_nFolderSW);

	if(GetDlgItem(IDC_OPT_KEYRADIO_POP))
		DDX_Radio(pDX,IDC_OPT_KEYRADIO_POP,theApp.m_ini.m_pop.m_nDoubleKeyPOP);

	if(GetDlgItem(IDC_OPT_KEYRADIO_FIFO))
		DDX_Radio(pDX,IDC_OPT_KEYRADIO_FIFO,theApp.m_ini.m_pop.m_nDoubleKeyFIFO);

	if(GetDlgItem(IDC_OPT_IME_AUTO_OFF))
		DDX_Check(pDX,IDC_OPT_IME_AUTO_OFF,theApp.m_ini.m_pop.m_nAutoImeOff);

	if(GetDlgItem(IDC_OPT_QUICK_ACCERSS))
		DDX_Check(pDX,IDC_OPT_QUICK_ACCERSS,theApp.m_ini.m_pop.m_nQuickEnter);

	if(GetDlgItem(IDC_OPT_QUICK_ACCERSS_CHAR))
		DDX_Check(pDX,IDC_OPT_QUICK_ACCERSS_CHAR,theApp.m_ini.m_pop.m_nQuickChar);

	if(GetDlgItem(IDC_QUICK_TIME))
		DDX_Text(pDX,IDC_QUICK_TIME, theApp.m_ini.m_pop.m_nQuickTime);

	if(GetDlgItem(IDC_OPT_HoseiX))
		DDX_Text(pDX,IDC_OPT_HoseiX, theApp.m_ini.m_pop.m_posCaretHosei.x);

	if(GetDlgItem(IDC_OPT_HoseiY))
		DDX_Text(pDX,IDC_OPT_HoseiY, theApp.m_ini.m_pop.m_posCaretHosei.y);

	if(GetDlgItem(IDC_OPT_OUT_HOSEI))
		DDX_Check(pDX,IDC_OPT_OUT_HOSEI, theApp.m_ini.m_pop.m_nOutRevice);

	if(GetDlgItem(IDC_OPT_SINGLE_EXPAND))
		DDX_Check(pDX,IDC_OPT_SINGLE_EXPAND, theApp.m_ini.m_pop.m_nSingleExpand);

}


BEGIN_MESSAGE_MAP(COptPopup, CDialog)
	//{{AFX_MSG_MAP(COptPopup)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP , hotkeyEnablePOP)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP2, hotkeyDisablePOP)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO , hotkeyEnableFIFO)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO2, hotkeyDisableFIFO)
	ON_BN_CLICKED(IDC_OPT_POPUP_EDGE, OnOptPopupEdge)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP3, hotkeyDisablePOP)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_POP4, hotkeyDisablePOP)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO3, hotkeyDisableFIFO)
	ON_BN_CLICKED(IDC_OPT_KEYRADIO_FIFO4, hotkeyDisableFIFO)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// COptPopup メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		初期化
//---------------------------------------------------
BOOL COptPopup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nPopupCombo = theApp.m_ini.m_pop.m_nPopupPos;

	CButton *chkbox;
	if(theApp.m_ini.m_pop.m_nCornerPopup) {
		chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE);
		chkbox->SetCheck(TRUE);
		if(theApp.m_ini.m_pop.m_nCornerPopup & 0x02) {
			chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE1);
			chkbox->SetCheck(TRUE);
		}
		if(theApp.m_ini.m_pop.m_nCornerPopup & 0x04) {
			chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE2);
			chkbox->SetCheck(TRUE);
		}
		if(theApp.m_ini.m_pop.m_nCornerPopup & 0x08) {
			chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE3);
			chkbox->SetCheck(TRUE);
		}
		if(theApp.m_ini.m_pop.m_nCornerPopup & 0x10) {
			chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE4);
			chkbox->SetCheck(TRUE);
		}
	}
	OnOptPopupEdge();

	//ホットキー
	if(theApp.m_ini.m_pop.m_nDoubleKeyPOP  != 0)	m_ctrlPopupKey.EnableWindow (false);
	if(theApp.m_ini.m_pop.m_nDoubleKeyFIFO != 0)	m_ctrlFifoKey.EnableWindow (false);

	UINT uPopKey,uPopMod,uRirekiKey,uRirekiMod;
	theApp.m_ini.getHotKey(&uPopKey,&uPopMod,&uRirekiKey,&uRirekiMod);

	//コントロールにセット
	uPopMod = CGeneral::mod2Hotkey(uPopMod);
	m_ctrlPopupKey.SetHotKey(uPopKey,uPopMod);
	uRirekiMod = CGeneral::mod2Hotkey(uRirekiMod);
	m_ctrlFifoKey.SetHotKey(uRirekiKey,uRirekiMod);
	
	return TRUE;
}

//---------------------------------------------------
//関数名	PreTranslateMessage(MSG* pMsg)
//機能		メッセージ前処理
//---------------------------------------------------
BOOL COptPopup::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB && ::GetKeyState(VK_CONTROL) < 0){
		::PostMessage(::GetParent(this->m_hWnd),pMsg->message,VK_PRIOR,pMsg->lParam);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//---------------------------------------------------
//関数名	hotkeyEnablePOP()
//機能		ホットキー有効
//---------------------------------------------------
void COptPopup::hotkeyEnablePOP()
{
	m_ctrlPopupKey.EnableWindow (true);
}
void COptPopup::hotkeyEnableFIFO()
{
	m_ctrlFifoKey.EnableWindow (true);
}


//---------------------------------------------------
//関数名	hotkeyDisablePOP()
//機能		ホットキー無効
//---------------------------------------------------
void COptPopup::hotkeyDisablePOP()
{
	m_ctrlPopupKey.EnableWindow (false);
}
void COptPopup::hotkeyDisableFIFO()
{
	m_ctrlFifoKey.EnableWindow (false);
}

//---------------------------------------------------
//関数名	DestroyWindow()
//機能		設定を反映
//---------------------------------------------------
BOOL COptPopup::DestroyWindow() 
{
	WORD wVkCodeP,wModP;
	WORD wVkCodeF,wModF;

	//ホットキー設定を取得、設定
	m_ctrlPopupKey.GetHotKey(wVkCodeP,wModP);
	wModP = CGeneral::hotkey2MOD(wModP);

	m_ctrlFifoKey.GetHotKey(wVkCodeF,wModF);
	wModF = CGeneral::hotkey2MOD(wModF);
	
	theApp.m_ini.setHotkey(wVkCodeP,wModP,wVkCodeF,wModF);

	//角ポップアップの設定	
	CButton *chkbox;
	chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE);
	if(chkbox->GetCheck()) {
		theApp.m_ini.m_pop.m_nCornerPopup = 1;
		chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE1);
		if(chkbox->GetCheck()) {
			theApp.m_ini.m_pop.m_nCornerPopup = theApp.m_ini.m_pop.m_nCornerPopup | 0x02;
		}
		chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE2);
		if(chkbox->GetCheck()) {
			theApp.m_ini.m_pop.m_nCornerPopup = theApp.m_ini.m_pop.m_nCornerPopup | 0x04;
		}
		chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE3);
		if(chkbox->GetCheck()) {
			theApp.m_ini.m_pop.m_nCornerPopup = theApp.m_ini.m_pop.m_nCornerPopup | 0x08;
		}
		chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE4);
		if(chkbox->GetCheck()) {
			theApp.m_ini.m_pop.m_nCornerPopup = theApp.m_ini.m_pop.m_nCornerPopup | 0x10;
		}
	}
	else theApp.m_ini.m_pop.m_nCornerPopup = 0;

	CComboBox *cmbBox;
	cmbBox = (CComboBox*)GetDlgItem(IDC_OPT_POPUP_POS);
	theApp.m_ini.m_pop.m_nPopupPos = cmbBox->GetCurSel();

	return CDialog::DestroyWindow();
}

//---------------------------------------------------
//関数名	OnOptPopupEdge()
//機能		角ポップアップの設定イベント
//---------------------------------------------------
void COptPopup::OnOptPopupEdge() 
{
	CButton *chkbox;
	chkbox = (CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE);
	if(chkbox->GetCheck()) {
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE1)->EnableWindow (true);
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE2)->EnableWindow (true);
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE3)->EnableWindow (true);
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE4)->EnableWindow (true);
	}
	else {
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE1)->EnableWindow (false);
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE2)->EnableWindow (false);
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE3)->EnableWindow (false);
		(CButton*)GetDlgItem(IDC_OPT_POPUP_EDGE4)->EnableWindow (false);
	}
}


void COptPopup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)	GetDlgItem(IDC_OPT_SELECT_SAVE)->SetFocus();	
	
}
