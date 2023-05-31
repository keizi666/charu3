/*----------------------------------------------------------
	AddDialogクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "AddDialog.h"
#include "ClipBord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	CAddDialog					[public]
//機能		コンストラクタ
//---------------------------------------------------
CAddDialog::CAddDialog(CWnd* pParent,HTREEITEM hTreeItem) : CDialog(CAddDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddDialog)
	//}}AFX_DATA_INIT
	m_hTreeItem = hTreeItem;

}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void CAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDialog)
	if(GetDlgItem(IDC_EDIT_DATA_MACRO_COMBO))
		DDX_Control(pDX, IDC_EDIT_DATA_MACRO_COMBO, m_ctrlDataMacro);
	if(GetDlgItem(IDC_ADD_MACRO_COMBO))
		DDX_Control(pDX, IDC_ADD_MACRO_COMBO, m_ctrlMacro);
	if(GetDlgItem(IDC_ICON_COMBO))
		DDX_Control(pDX, IDC_ICON_COMBO, m_comboIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddDialog, CDialog)
	//{{AFX_MSG_MAP(CAddDialog)
	ON_BN_CLICKED(ID_ADD_OK, OnAddOk)
	ON_BN_CLICKED(ID_ADD_CLOSE, OnAddClose)
	ON_BN_CLICKED(IDC_ADD_PASTE_FILE, OnAddPasteFile)
	ON_CBN_SELCHANGE(IDC_ADD_MACRO_COMBO, OnSelchangeAddMacroCombo)
	ON_CBN_SELCHANGE(IDC_EDIT_DATA_MACRO_COMBO, OnSelchangeEditDataMacroCombo)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// CAddDialog メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	OnAddOk()
//機能		追加ボタン押下処理
//---------------------------------------------------
void CAddDialog::OnAddOk()
{
	STRING_DATA Data;
	theApp.m_pTree->initStringData(&Data);

	CString strBuff;

	//追加データ作成
	GetDlgItem(IDC_ADD_EDIT)->GetWindowText(strBuff);
	Data.m_strData = strBuff;
	GetDlgItem(IDC_ADD_NAME)->GetWindowText(strBuff);
	Data.m_strTitle = strBuff;
	GetDlgItem(IDC_ADD_MACRO)->GetWindowText(strBuff);
	Data.m_strMacro = strBuff;

	if(Data.m_strTitle == _T("") && Data.m_strData == _T("")) {
		CString strRes;
		strRes.LoadString(APP_MES_ALL_EMPTY);
		AfxMessageBox(strRes);
		return;
	}
	if(Data.m_strTitle == _T("")) {//タイトルが空の場合はタイトルを作る
		Data.m_strTitle = theApp.m_pTree->makeTitle(Data.m_strData);
	}
	Data.m_cKind = KIND_LOCK;
	int nCursel = m_comboIcon.GetCurSel();
	if(nCursel > 0)			Data.m_cIcon = nCursel - 1;
	else if(nCursel == 0)	Data.m_cIcon = theApp.m_pTree->decideIcon(Data.m_strData);

	theApp.m_pTree->addData(m_hTreeItem,Data);
	CString strMessage;
	CString strRes;
	strRes.LoadString(APP_MES_ADD_OK2);
	strMessage.Format(strRes,Data.m_strTitle);
	AfxMessageBox(strMessage,MB_OK | MB_ICONINFORMATION);
	//エディットボックスをクリア
	GetDlgItem(IDC_ADD_MACRO)->SetWindowText(_T(""));
	GetDlgItem(IDC_ADD_EDIT)->SetWindowText(_T(""));
	GetDlgItem(IDC_ADD_NAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_ADD_NAME)->SetFocus();
}

//---------------------------------------------------
//関数名	OnAddClose()
//機能		閉じるボタン処理
//---------------------------------------------------
void CAddDialog::OnAddClose()
{
	EndDialog(IDCANCEL);
}

//---------------------------------------------------
//関数名	OnAddPasteFile()
//機能		ファイル名貼り付け処理
//---------------------------------------------------
void CAddDialog::OnAddPasteFile() 
{
	CFileDialog *pFileDialog;

	CString strRes;
	strRes.LoadString(APP_INF_FILE_FILTER6);
	pFileDialog = new CFileDialog(TRUE,_T("*.*"),NULL,NULL,strRes);

	if(IDOK == pFileDialog->DoModal()) {
		pasteMacro(IDC_ADD_EDIT,pFileDialog->GetPathName());
	}	
	if(pFileDialog)	delete pFileDialog;		
}

//---------------------------------------------------
//関数名	OnInitDialog() 
//機能		初期化
//---------------------------------------------------
BOOL CAddDialog::OnInitDialog() 
{
	m_cOlgFontEdit = GetDlgItem(IDC_ADD_DATA)->GetFont();
	m_cFontEdit = new CFont;
	if(m_cFontEdit) {
		m_cFontEdit->CreatePointFont(theApp.m_ini.m_visual.m_nFontSize,theApp.m_ini.m_visual.m_strFontName);
		GetDlgItem(IDC_ADD_DATA)->SetFont(m_cFontEdit,TRUE);
		GetDlgItem(IDC_ADD_MACRO)->SetFont(m_cFontEdit,TRUE);
	}
	m_cOlgFontTitle = GetDlgItem(IDC_ADD_NAME)->GetFont();
	m_cFontTitle = new CFont;
	if(m_cFontTitle) {
		m_cFontTitle->CreatePointFont(TEXTBOX_FONT_SIZE,theApp.m_ini.m_visual.m_strFontName);
		GetDlgItem(IDC_ADD_NAME)->SetFont(m_cFontTitle,TRUE);
	}

	CDialog::OnInitDialog();
	
	m_comboIcon.SetCurSel(0);

	//マクロテンプレートを初期化
	vector<MACRO_STRUCT>::iterator it;
	int i;
	if(m_vctMacro) {
		for(it = m_vctMacro->begin(),i = 0; it != m_vctMacro->end(); it++,i++){
			if(KIND_DATA_ALL & it->m_cKind) {
				m_ctrlMacro.AddString(it->m_strName);
				m_ctrlMacro.SetItemData(m_ctrlMacro.GetCount()-1,i);
			}
		}
	}
	if(m_vctDataMacro) {
		for(it = m_vctDataMacro->begin(),i = 0; it != m_vctDataMacro->end(); it++,i++){
			if(KIND_DATA_ALL & it->m_cKind) {
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
//関数名	pasteMacro(int nCtrlID,CString strString)
//機能		コントロール指定貼り付け
//---------------------------------------------------
void CAddDialog::pasteMacro(int nCtrlID,CString strString)
{
	CString strBkup;
	CClipBord clip;
	clip.getClipboardText(strBkup);
	clip.setClipboardText(strString);

	GetDlgItem(nCtrlID)->SendMessage(WM_PASTE,0,0);
	GetDlgItem(nCtrlID)->SetFocus();
	clip.setClipboardText(strBkup);
}

//---------------------------------------------------
//関数名	OnSelchangeAddMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CAddDialog::OnSelchangeAddMacroCombo() 
{
	if(m_vctMacro) {
		int nSelect = m_ctrlMacro.GetItemData(m_ctrlMacro.GetCurSel());
		if(nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctMacro->at(nSelect);
			pasteMacro(IDC_ADD_DATA,macro.m_strMacro);
		}
	}	
}

//---------------------------------------------------
//関数名	OnSelchangeEditDataMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CAddDialog::OnSelchangeEditDataMacroCombo() 
{
	if(m_vctDataMacro) {
		int nSelect = m_ctrlDataMacro.GetItemData(m_ctrlDataMacro.GetCurSel());
		if(nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctDataMacro->at(nSelect);
			pasteMacro(IDC_ADD_MACRO,macro.m_strMacro);
		}
	}	
}

void CAddDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	theApp.m_pTreeDlg->DrawBorder(); // The border somehow disappears, so redraw it as a workaround
}

BOOL CAddDialog::DestroyWindow() 
{
	theApp.m_pTreeDlg->DrawBorder(); // The border somehow disappears, so redraw it as a workaround

	GetDlgItem(IDC_ADD_DATA)->SetFont(m_cOlgFontEdit,FALSE);	
	GetDlgItem(IDC_ADD_MACRO)->SetFont(m_cOlgFontEdit,TRUE);
	if(m_cFontEdit) delete m_cFontEdit;	

	GetDlgItem(IDC_ADD_NAME)->SetFont(m_cOlgFontTitle,TRUE);
	if(m_cFontTitle) delete m_cFontTitle;
	
	return CDialog::DestroyWindow();
}
