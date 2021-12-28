/*----------------------------------------------------------
	EditDialogクラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_EDITDIALOG_H__B4B68363_82C1_4D62_AC66_00D698DF05A4__INCLUDED_)
#define AFX_EDITDIALOG_H__B4B68363_82C1_4D62_AC66_00D698DF05A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDialog.h : ヘッダー ファイル
//
#include "MyEditCtrl.h"
//---------------------------------------------------
// CEditDialog ダイアログ
//---------------------------------------------------
class CEditDialog : public CDialog
{
// コンストラクション
public:
	CEditDialog(CWnd* pParent,STRING_DATA data);   // 標準のコンストラクタ
	STRING_DATA m_data;
	void setMacroTempate(vector<MACRO_STRUCT> *pMacro,vector<MACRO_STRUCT> *pDataMacro){
		m_vctMacro = pMacro;
		m_vctDataMacro = pDataMacro;
	}
// ダイアログ データ
	//{{AFX_DATA(CEditDialog)
	enum { IDD = IDD_EDIT };
	CMyEditCtrl	m_ctrlDataEdit;
	CMyEditCtrl	m_ctrlMacroEdit;
	CComboBox	m_ctrlKindCombo;
	CComboBox	m_ctrlIconCombo;
	CButton	m_ctrlRirekiChk;
	CComboBox	m_ctrlMacro;
	CComboBox	m_ctrlDataMacro;
	CString	m_strDataName;
	CString	m_strDataMacro;
	CString	m_strData;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	vector<MACRO_STRUCT>  *m_vctMacro;
	vector<MACRO_STRUCT>  *m_vctDataMacro;
	void pasteMacro(int nCtrlID,CString strString);
	CFont *m_cFontEdit,*m_cOlgFontEdit;
	CFont *m_cFontTitle,*m_cOlgFontTitle;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditDialog)
	afx_msg void OnEditPasteFile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeEditMacroCombo();
	afx_msg void OnSelchangeEditDataMacroCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITDIALOG_H__B4B68363_82C1_4D62_AC66_00D698DF05A4__INCLUDED_)
