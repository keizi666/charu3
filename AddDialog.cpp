/*----------------------------------------------------------
	AddDialogクラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "AddDialog.h"
#include "ClipBord.h"

#include <vector>

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
CAddDialog::CAddDialog(CWnd* pParent, STRING_DATA* pData, bool newData)
    : CDialog(CAddDialog::IDD, pParent)
    , m_pData(pData)
    , m_bNewData(newData)
    , m_cFontEdit(NULL)
    , m_cOlgFontEdit(NULL)
    , m_cFontTitle(NULL)
    , m_cOlgFontTitle(NULL)
{
	//{{AFX_DATA_INIT(CAddDialog)
	//}}AFX_DATA_INIT
	m_vctMacro = &theApp.m_ini.m_vctMacro;
	m_vctDataMacro = &theApp.m_ini.m_vctDataMacro;
}

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void CAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDialog)
	if (GetDlgItem(IDC_EDIT_MACRO_COMBO))
		DDX_Control(pDX, IDC_EDIT_MACRO_COMBO, m_ctrlMacro);
	if (GetDlgItem(IDC_EDIT_DATA_MACRO_COMBO))
		DDX_Control(pDX, IDC_EDIT_DATA_MACRO_COMBO, m_ctrlDataMacro);
	if (GetDlgItem(IDC_ICON_COMBO))
		DDX_Control(pDX, IDC_ICON_COMBO, m_ctrlIcon);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddDialog, CDialog)
	//{{AFX_MSG_MAP(CAddDialog)
	ON_BN_CLICKED(IDC_PASTE_FILE, OnPasteFile)
	ON_CBN_SELCHANGE(IDC_EDIT_MACRO_COMBO, OnSelchangeMacroCombo)
	ON_CBN_SELCHANGE(IDC_EDIT_DATA_MACRO_COMBO, OnSelchangeDataMacroCombo)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
// CAddDialog メッセージ ハンドラ
//---------------------------------------------------

//---------------------------------------------------
//関数名	OnInitDialog()
//機能		ダイアログの初期化
//---------------------------------------------------
BOOL CAddDialog::OnInitDialog()
{
	m_cOlgFontEdit = GetDlgItem(IDC_EDIT_DATA)->GetFont();
	m_cFontEdit = new CFont;
	if (m_cFontEdit) {
		m_cFontEdit->CreatePointFont(theApp.m_ini.m_visual.m_nFontSize, theApp.m_ini.m_visual.m_strFontName);
		GetDlgItem(IDC_EDIT_DATA)->SetFont(m_cFontEdit, TRUE);
		GetDlgItem(IDC_EDIT_MACRO)->SetFont(m_cFontEdit, TRUE);
	}
	m_cOlgFontTitle = GetDlgItem(IDC_EDIT_NAME)->GetFont();
	m_cFontTitle = new CFont;
	if (m_cFontTitle) {
		m_cFontTitle->CreatePointFont(TEXTBOX_FONT_SIZE, theApp.m_ini.m_visual.m_strFontName);
		GetDlgItem(IDC_EDIT_NAME)->SetFont(m_cFontTitle, TRUE);
	}

	CDialog::OnInitDialog();

	m_ctrlIcon.SetCurSel(0);

	//マクロテンプレートを初期化
	std::vector<MACRO_STRUCT>::iterator it;
	int i;
	int filter = m_bNewData ? KIND_DATA_ALL : m_pData->m_cKind;
	if (m_vctMacro) {
		for (it = m_vctMacro->begin(), i = 0; it != m_vctMacro->end(); it++, i++) {
			if (it->m_cKind & filter) {
				m_ctrlMacro.AddString(it->m_strName);
				m_ctrlMacro.SetItemData(m_ctrlMacro.GetCount() - 1, i);
			}
		}
	}
	if (m_vctDataMacro) {
		for (it = m_vctDataMacro->begin(), i = 0; it != m_vctDataMacro->end(); it++, i++) {
			if (it->m_cKind & filter) {
				m_ctrlDataMacro.AddString(it->m_strName);
				m_ctrlDataMacro.SetItemData(m_ctrlDataMacro.GetCount() - 1, i);
			}
		}
	}
	if (m_ctrlMacro.GetCount() == 0) m_ctrlMacro.EnableWindow(FALSE);
	if (m_ctrlDataMacro.GetCount() == 0) m_ctrlDataMacro.EnableWindow(FALSE);
	return TRUE;
}

BOOL CAddDialog::DestroyWindow()
{
	GetDlgItem(IDC_EDIT_DATA)->SetFont(m_cOlgFontEdit, FALSE);
	GetDlgItem(IDC_EDIT_MACRO)->SetFont(m_cOlgFontEdit, TRUE);
	if (m_cFontEdit) delete m_cFontEdit;

	GetDlgItem(IDC_EDIT_NAME)->SetFont(m_cOlgFontTitle, TRUE);
	if (m_cFontTitle) delete m_cFontTitle;

	return CDialog::DestroyWindow();
}

//---------------------------------------------------
//関数名	OnOK()
//機能		追加ボタン押下処理
//---------------------------------------------------
void CAddDialog::OnOK()
{
	CString strBuff;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strBuff);
	m_pData->m_strTitle = strBuff;
	GetDlgItem(IDC_EDIT_DATA)->GetWindowText(strBuff);
	m_pData->m_strData = strBuff;
	GetDlgItem(IDC_EDIT_MACRO)->GetWindowText(strBuff);
	m_pData->m_strMacro = strBuff;

	if (m_pData->m_strTitle == _T("") && m_pData->m_strData == _T("")) {
		CString strRes;
		strRes.LoadString(APP_MES_ALL_EMPTY);
		AfxMessageBox(strRes);
		return;
	}

	if (m_pData->m_strTitle == _T("")) {//タイトルが空の場合はタイトルを作る
		m_pData->m_strTitle = theApp.m_pTree->makeTitle(m_pData->m_strData);
	}

	m_pData->m_cKind = KIND_LOCK;

	int icon = m_ctrlIcon.GetCurSel();
	if (icon > 0) {
		m_pData->m_cIcon = icon - 1;
	}
	else {
		m_pData->m_cIcon = theApp.m_pTree->decideIcon(m_pData->m_strData);
	}

	CDialog::OnOK();
}

//---------------------------------------------------
//関数名	OnPasteFile()
//機能		ファイル名貼り付け処理
//---------------------------------------------------
void CAddDialog::OnPasteFile()
{
	CFileDialog* pFileDialog;
	CString strRes;
	strRes.LoadString(APP_INF_FILE_FILTER6);
	pFileDialog = new CFileDialog(TRUE, _T("*.*"), NULL, NULL, strRes);
	if (pFileDialog) {
		if (IDOK == pFileDialog->DoModal()) {
			PasteMacro(IDC_EDIT_DATA, pFileDialog->GetPathName());
		}
		delete pFileDialog;
	}
}

//---------------------------------------------------
//関数名	OnSelchangeMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CAddDialog::OnSelchangeMacroCombo() 
{
	if(m_vctMacro) {
		int nSelect = m_ctrlMacro.GetItemData(m_ctrlMacro.GetCurSel());
		if(nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctMacro->at(nSelect);
			PasteMacro(IDC_EDIT_DATA,macro.m_strMacro);
		}
	}	
}

//---------------------------------------------------
//関数名	OnSelchangeDataMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CAddDialog::OnSelchangeDataMacroCombo() 
{
	if(m_vctDataMacro) {
		int nSelect = m_ctrlDataMacro.GetItemData(m_ctrlDataMacro.GetCurSel());
		if(nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctDataMacro->at(nSelect);
			PasteMacro(IDC_EDIT_MACRO,macro.m_strMacro);
		}
	}	
}

//---------------------------------------------------
//関数名	PasteMacro(int nCtrlID,CString strString)
//機能		コントロール指定貼り付け
//---------------------------------------------------
void CAddDialog::PasteMacro(int nCtrlID, CString strString)
{
	CString strBkup;
	CClipBoard clip;
	clip.getClipboardText(strBkup, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);
	clip.setClipboardText(strString, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);

	GetDlgItem(nCtrlID)->SendMessage(WM_PASTE, 0, 0);
	GetDlgItem(nCtrlID)->SetFocus();
	clip.setClipboardText(strBkup, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);
}
