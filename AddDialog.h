/*----------------------------------------------------------
	AddDialogクラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_ADDDIALOG_H__9B7892F6_DF6E_4022_8FFA_45EF89D77EC9__INCLUDED_)
#define AFX_ADDDIALOG_H__9B7892F6_DF6E_4022_8FFA_45EF89D77EC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Charu3.h"
#include "Charu3Tree.h"
#include <list>
using namespace std;

//---------------------------------------------------
// CAddDialog ダイアログ
//---------------------------------------------------
class CAddDialog : public CDialog
{
// コンストラクション
public:
	CAddDialog(CWnd* pParent,HTREEITEM hTreeItem);   // 標準のコンストラクタ
	void setMacroTempate(vector<MACRO_STRUCT> *pMacro,vector<MACRO_STRUCT> *pDataMacro){
		m_vctMacro = pMacro;
		m_vctDataMacro = pDataMacro;
	}

// ダイアログ データ
	//{{AFX_DATA(CAddDialog)
	enum { IDD = IDD_ADD_DATA };
	CComboBox	m_ctrlDataMacro;
	CComboBox	m_ctrlMacro;
	CComboBox	m_comboIcon;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAddDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HTREEITEM m_hTreeItem;
	vector<MACRO_STRUCT>  *m_vctMacro;
	vector<MACRO_STRUCT>  *m_vctDataMacro;
	void pasteMacro(int nCtrlID,CString strString);
	CFont *m_cFontEdit,*m_cOlgFontEdit;
	CFont *m_cFontTitle,*m_cOlgFontTitle;
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAddDialog)
	afx_msg void OnAddOk();
	afx_msg void OnAddClose();
	afx_msg void OnAddPasteFile();
	afx_msg void OnMacroPaste();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAddMacroCombo();
	afx_msg void OnSelchangeEditDataMacroCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ADDDIALOG_H__9B7892F6_DF6E_4022_8FFA_45EF89D77EC9__INCLUDED_)
