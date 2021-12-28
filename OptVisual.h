/*----------------------------------------------------------
	ビジュアル設定クラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_OPTVISUAL_H__CE6482E8_6118_46DF_B05A_C898A00E9122__INCLUDED_)
#define AFX_OPTVISUAL_H__CE6482E8_6118_46DF_B05A_C898A00E9122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptVisual.h : ヘッダー ファイル
//
#include "PaletStatic.h"

#define	MAX_FONT 256
//---------------------------------------------------
// COptVisual ダイアログ
//---------------------------------------------------
class COptVisual : public CDialog
{
// コンストラクション
public:
	COptVisual(CWnd* pParent = NULL);   // 標準のコンストラクタ
	friend int CALLBACK EnumFontProc(ENUMLOGFONT *lpelf,NEWTEXTMETRIC *lpntm,int FontType,LPARAM _dwDmy);

// ダイアログ データ
	//{{AFX_DATA(COptVisual)
	enum { IDD = IDD_OPT_VISUAL };
	CPaletStatic m_ctrlBorderPal;
	CPaletStatic m_ctrlTextPal;
	CPaletStatic m_ctrlBackPal;
	CComboBox	m_ctrlFontCombo;
	CString	m_strBackColor;
	CString	m_strTextColor;
	CString	m_strBorderColor;
	int		m_n;
	int		m_nScrollH;
	int		m_nScrollV;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COptVisual)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void setPalet(CEdit *edit,CPaletStatic *stPal);
	void setTextToPalet(CEdit *edit,CPaletStatic *stPal);
	int m_nMaxFont; 

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COptVisual)
	afx_msg void OnOptBackColorPal();
	afx_msg void OnOptBorderColorPal();
	afx_msg void OnOptTextColorPal();
	virtual BOOL OnInitDialog();
	afx_msg void OnOptVsBrows();
	afx_msg void OnChangeOptTextColor();
	afx_msg void OnChangeOptBorderColor();
	afx_msg void OnChangeOptBackColor();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPTVISUAL_H__CE6482E8_6118_46DF_B05A_C898A00E9122__INCLUDED_)
