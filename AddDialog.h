/*----------------------------------------------------------
	AddDialogクラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_ADDDIALOG_H__9B7892F6_DF6E_4022_8FFA_45EF89D77EC9__INCLUDED_)
#define AFX_ADDDIALOG_H__9B7892F6_DF6E_4022_8FFA_45EF89D77EC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

//---------------------------------------------------
// CAddDialog ダイアログ
//---------------------------------------------------
class CAddDialog : public CDialog
{
// コンストラクション
public:
	CAddDialog(CWnd* pParent, STRING_DATA* pData, bool newData);

// ダイアログ データ
	//{{AFX_DATA(CAddDialog)
	enum { IDD = IDD_ADD_DATA };
private:
	CComboBox	m_ctrlMacro;
	CComboBox	m_ctrlDataMacro;
	CComboBox	m_ctrlIcon;
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
	std::vector<MACRO_STRUCT>  *m_vctMacro;
	std::vector<MACRO_STRUCT>  *m_vctDataMacro;
	CFont *m_cFontEdit,*m_cOlgFontEdit;
	CFont *m_cFontTitle,*m_cOlgFontTitle;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAddDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPasteFile();
	afx_msg void OnSelchangeMacroCombo();
	afx_msg void OnSelchangeDataMacroCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void PasteMacro(int nCtrlID, CString strString);

	STRING_DATA* m_pData;
	bool m_bNewData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ADDDIALOG_H__9B7892F6_DF6E_4022_8FFA_45EF89D77EC9__INCLUDED_)
