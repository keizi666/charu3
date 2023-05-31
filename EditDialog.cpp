/*----------------------------------------------------------
	EditDialogクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "EditDialog.h"
#include "ClipBord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	CEditDialog
//機能		コンストラクタ
//---------------------------------------------------
CEditDialog::CEditDialog(CWnd* pParent /*=NULL*/,STRING_DATA data) : CDialog(CEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDialog)
	m_strDataName = _T("");
	m_strDataMacro = _T("");
	m_strData = _T("");
	//}}AFX_DATA_INIT
	m_data = data;
	m_vctDataMacro = NULL;
	m_vctDataMacro = NULL;
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDialog)
	
	DDX_Control(pDX, IDC_EDIT_DATA, m_ctrlDataEdit);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_ctrlMacroEdit);
	if(GetDlgItem(IDC_KIND_COMBO))
		DDX_Control(pDX, IDC_KIND_COMBO, m_ctrlKindCombo);
	if(GetDlgItem(IDC_ICON_COMBO))
		DDX_Control(pDX, IDC_ICON_COMBO, m_ctrlIconCombo);
	if(GetDlgItem(IDC_RIREKI_CHECK))
		DDX_Control(pDX, IDC_RIREKI_CHECK, m_ctrlRirekiChk);
	if(GetDlgItem(IDC_EDIT_MACRO_COMBO))
		DDX_Control(pDX, IDC_EDIT_MACRO_COMBO, m_ctrlMacro);
	if(GetDlgItem(IDC_EDIT_DATA_MACRO_COMBO))
		DDX_Control(pDX, IDC_EDIT_DATA_MACRO_COMBO, m_ctrlDataMacro);
	if(GetDlgItem(IDC_NAME))
		DDX_Text(pDX, IDC_NAME, m_strDataName);
	if(GetDlgItem(IDC_EDIT_MACRO))
		DDX_Text(pDX, IDC_EDIT_MACRO, m_strDataMacro);
	if(GetDlgItem(IDC_EDIT_DATA))
		DDX_Text(pDX, IDC_EDIT_DATA, m_strData);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CEditDialog, CDialog)
	//{{AFX_MSG_MAP(CEditDialog)
	ON_BN_CLICKED(IDC_EDIT_PASTE_FILE, OnEditPasteFile)
	ON_CBN_SELCHANGE(IDC_EDIT_MACRO_COMBO, OnSelchangeEditMacroCombo)
	ON_CBN_SELCHANGE(IDC_EDIT_DATA_MACRO_COMBO, OnSelchangeEditDataMacroCombo)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// CEditDialog メッセージ ハンドラ
//---------------------------------------------------

void CEditDialog::OnEditPasteFile() 
{
	CFileDialog *pFileDialog;
	CString strRes;
	strRes.LoadString(APP_INF_FILE_FILTER6);
	pFileDialog = new CFileDialog(TRUE,_T("*.*"),NULL,NULL,strRes);

	if(pFileDialog) {
		if(IDOK == pFileDialog->DoModal()) {
			pasteMacro(IDC_EDIT_DATA,pFileDialog->GetPathName());
		}	
		delete pFileDialog;		
	}
}

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		ダイアログの初期化
//---------------------------------------------------
BOOL CEditDialog::OnInitDialog() 
{
	m_strData = m_data.m_strData;
	m_strDataName = m_data.m_strTitle;
	m_strDataMacro = m_data.m_strMacro;

	m_cOlgFontEdit = GetDlgItem(IDC_EDIT_DATA)->GetFont();
	m_cFontEdit = new CFont;
	if(m_cFontEdit) {
		m_cFontEdit->CreatePointFont(theApp.m_ini.m_visual.m_nFontSize,theApp.m_ini.m_visual.m_strFontName);
		GetDlgItem(IDC_EDIT_DATA)->SetFont(m_cFontEdit,TRUE);
		GetDlgItem(IDC_EDIT_MACRO)->SetFont(m_cFontEdit,TRUE);
	}
	m_cOlgFontTitle = GetDlgItem(IDC_NAME)->GetFont();
	m_cFontTitle = new CFont;
	if(m_cFontTitle) {
		m_cFontTitle->CreatePointFont(TEXTBOX_FONT_SIZE,theApp.m_ini.m_visual.m_strFontName);
		GetDlgItem(IDC_NAME)->SetFont(m_cFontTitle,TRUE);
	}

	CDialog::OnInitDialog();
	
	CString strSelText;
	if(m_data.m_cKind & KIND_FOLDER_ALL) {
		if(m_data.m_cKind & KIND_RIREKI) 	m_ctrlRirekiChk.SetCheck(TRUE);
		m_ctrlIconCombo.EnableWindow(FALSE);
		m_ctrlIconCombo.ShowWindow(SW_HIDE);
		m_ctrlKindCombo.EnableWindow(FALSE);
		m_ctrlKindCombo.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_KIND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ICON)->ShowWindow(SW_HIDE);
	}
	else {
		m_ctrlRirekiChk.EnableWindow(FALSE);
		m_ctrlRirekiChk.ShowWindow(SW_HIDE);
		if(m_data.m_cKind & KIND_ONETIME)	m_ctrlKindCombo.SetCurSel(0);
		else								m_ctrlKindCombo.SetCurSel(1);
		m_ctrlIconCombo.SetCurSel(m_data.m_cIcon+1);
	}

	//マクロテンプレートを初期化
	vector<MACRO_STRUCT>::iterator it;
	int i;
	if(m_vctMacro) {
		for(it = m_vctMacro->begin(),i = 0; it != m_vctMacro->end(); it++,i++){
			if(m_data.m_cKind & it->m_cKind) {
				m_ctrlMacro.AddString(it->m_strName);
				m_ctrlMacro.SetItemData(m_ctrlMacro.GetCount()-1,i);
			}
		}
	}
	if(m_vctDataMacro) {
		m_ctrlDataMacro.Clear();
		for(it = m_vctDataMacro->begin(),i = 0; it != m_vctDataMacro->end(); it++,i++){
			if(m_data.m_cKind & it->m_cKind) {
				m_ctrlDataMacro.AddString(it->m_strName);
				m_ctrlDataMacro.SetItemData(m_ctrlDataMacro.GetCount()-1,i);
			}
		}
	}
	if(m_ctrlMacro.GetCount() == 0) m_ctrlMacro.EnableWindow(FALSE);
	if(m_ctrlDataMacro.GetCount() == 0) m_ctrlDataMacro.EnableWindow(FALSE);
	return TRUE;
}

//---------------------------------------------------
//関数名	OnOK()
//機能		OK処理
//---------------------------------------------------
void CEditDialog::OnOK() 
{
	CString strBuff;
	GetDlgItem(IDC_NAME)->GetWindowText(strBuff);
	if (strBuff == _T("")) {
		CString strRes;
		strRes.LoadString(APP_MES_NAME_EMPTY);
		AfxMessageBox(strRes);
		return;
	}
	m_data.m_strTitle = strBuff;
	GetDlgItem(IDC_EDIT_MACRO)->GetWindowText(strBuff);
	m_data.m_strMacro = strBuff;
	GetDlgItem(IDC_EDIT_DATA)->GetWindowText(strBuff);
	m_data.m_strData = strBuff;

	int nCursel;
	if(m_data.m_cKind & KIND_FOLDER_ALL) {
		if(!m_ctrlRirekiChk.GetCheck())	m_data.m_cKind = KIND_FOLDER;
		else							m_data.m_cKind = KIND_RIREKI;
		m_data.m_cIcon = KIND_DEFAULT;
	}
	else {
		nCursel = m_ctrlKindCombo.GetCurSel();
		if(nCursel == 0)	m_data.m_cKind = KIND_ONETIME;
		else				m_data.m_cKind = KIND_LOCK;
		m_data.m_cIcon = m_ctrlIconCombo.GetCurSel() - 1;
	}

	CDialog::OnOK();
}

//---------------------------------------------------
//関数名	OnEditchangeEditMacroCombo() 
//機能		マクロコンボ変更処理
//---------------------------------------------------
void CEditDialog::OnSelchangeEditMacroCombo() 
{
	if(m_vctMacro) {
		int nSelect = m_ctrlMacro.GetItemData(m_ctrlMacro.GetCurSel());
		if(nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctMacro->at(nSelect);
			pasteMacro(IDC_EDIT_DATA,macro.m_strMacro);
		}
	}	
}

//---------------------------------------------------
//関数名	OnEditchangeEditDataMacroCombo() 
//機能		マクロコンボ変更処理
//---------------------------------------------------
void CEditDialog::OnSelchangeEditDataMacroCombo() 
{
	if(m_vctDataMacro) {
		int nSelect = m_ctrlDataMacro.GetItemData(m_ctrlDataMacro.GetCurSel());
		if(nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctDataMacro->at(nSelect);
			pasteMacro(IDC_EDIT_MACRO,macro.m_strMacro);
		}
	}	
}

//---------------------------------------------------
//関数名	pasteMacro(int nCtrlID,CString strString)
//機能		コントロール指定貼り付け
//---------------------------------------------------
void CEditDialog::pasteMacro(int nCtrlID,CString strString)
{
	CString strBkup;
	CClipBord clip;
	clip.getClipboardText(strBkup);
	clip.setClipboardText(strString);

	GetDlgItem(nCtrlID)->SendMessage(WM_PASTE,0,0);
	GetDlgItem(nCtrlID)->SetFocus();
	clip.setClipboardText(strBkup);
}


void CEditDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	theApp.m_pTreeDlg->DrawBorder(); // The border somehow disappears, so redraw it as a workaround
}

BOOL CEditDialog::DestroyWindow() 
{
	theApp.m_pTreeDlg->DrawBorder(); // The border somehow disappears, so redraw it as a workaround

	GetDlgItem(IDC_EDIT_DATA)->SetFont(m_cOlgFontEdit,FALSE);	
	GetDlgItem(IDC_EDIT_MACRO)->SetFont(m_cOlgFontEdit,TRUE);
	if(m_cFontEdit) delete m_cFontEdit;	

	GetDlgItem(IDC_NAME)->SetFont(m_cOlgFontTitle,TRUE);
	if(m_cFontTitle) delete m_cFontTitle;
	
	return CDialog::DestroyWindow();
}
