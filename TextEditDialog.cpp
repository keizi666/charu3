#include "stdafx.h"
#include "Charu3.h"
#include "TextEditDialog.h"

#include <vector>

extern CCharu3App theApp; 

CTextEdit::CTextEdit(CWnd* pParent, bool newData, int kind) : CDialog(CTextEdit::IDD, pParent), m_pParent(pParent)
{
	m_vctMacro = &theApp.m_ini.m_vctMacro;
}

void CTextEdit::DoDataExchange(CDataExchange* pDX)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT_DATA, m_ctrlDataEdit);
	if (GetDlgItem(IDC_EDIT_MACRO_COMBO))
		DDX_Control(pDX, IDC_EDIT_MACRO_COMBO, m_ctrlMacro);
}

BEGIN_MESSAGE_MAP(CTextEdit, CDialog)
	//{{AFX_MSG_MAP(CEditDialog)
	ON_BN_CLICKED(IDC_PASTE_FILE, OnPasteFile)
	ON_CBN_SELCHANGE(IDC_EDIT_MACRO_COMBO, OnSelchangeMacroCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTextEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::vector<MACRO_STRUCT>::iterator it;
	int i;
	//int filter = m_bNewData ? KIND_DATA_ALL : m_nKind;
	int filter = KIND_DATA_ALL;
	if (m_vctMacro) {
		for (it = m_vctMacro->begin(), i = 0; it != m_vctMacro->end(); it++, i++) {
			if (it->m_cKind & filter) {
				m_ctrlMacro.AddString(it->m_strName);
				m_ctrlMacro.SetItemData(m_ctrlMacro.GetCount() - 1, i);
			}
		}
	}
	if (m_ctrlMacro.GetCount() == 0) m_ctrlMacro.EnableWindow(FALSE);

	if (GetDlgItem(IDC_TEXT)) {
		GetDlgItem(IDC_TEXT)->SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CTextEdit::OnCancel()
{
	CDialog::OnCancel();
	m_pParent->SendMessage(WM_CLOSE);
}

//---------------------------------------------------
//関数名	OnPasteFile()
//機能		ファイル名貼り付け処理
//---------------------------------------------------
void CTextEdit::OnPasteFile()
{
	CFileDialog* pFileDialog;
	CString strRes;
	strRes.LoadString(APP_INF_FILE_FILTER6);
	pFileDialog = new CFileDialog(TRUE, _T("*.*"), NULL, NULL, strRes);
	if (pFileDialog) {
		if (IDOK == pFileDialog->DoModal()) {
			Paste(IDC_TEXT, pFileDialog->GetPathName());
		}
		delete pFileDialog;
	}
}

//---------------------------------------------------
//関数名	OnSelchangeMacroCombo()
//機能		マクロコンボボックス変更
//---------------------------------------------------
void CTextEdit::OnSelchangeMacroCombo()
{
	if (m_vctMacro) {
		int nSelect = m_ctrlMacro.GetItemData(m_ctrlMacro.GetCurSel());
		if (nSelect >= 0) {
			MACRO_STRUCT macro;
			macro = m_vctMacro->at(nSelect);
			PasteMacro(IDC_TEXT, macro.m_strMacro);
		}
	}
}

//---------------------------------------------------
//関数名	PasteMacro(int nCtrlID,CString strString)
//機能		コントロール指定貼り付け
//---------------------------------------------------
void CTextEdit::PasteMacro(int nCtrlID, CString strString)
{
	CString strBkup;
	CClipBoard clip;
	clip.getClipboardText(strBkup, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);
	clip.setClipboardText(strString, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);

	GetDlgItem(nCtrlID)->SendMessage(WM_PASTE, 0, 0);
	GetDlgItem(nCtrlID)->SetFocus();
	clip.setClipboardText(strBkup, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);
}

void CTextEdit::Paste(int nCtrlID, CString strString)
{
	CString strBkup;
	CClipBoard clip;
	clip.getClipboardText(strBkup, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);
	clip.setClipboardText(strString, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);

	GetDlgItem(nCtrlID)->SendMessage(WM_PASTE, 0, 0);
	GetDlgItem(nCtrlID)->SetFocus();
	clip.setClipboardText(strBkup, theApp.m_ini.m_nClipboardRetryTimes, theApp.m_ini.m_nClipboardRetryInterval);
}
