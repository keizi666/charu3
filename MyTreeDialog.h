/*----------------------------------------------------------
	MyTreeDialogクラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_MYTREEDIALOG_H__00B921AA_8B84_46EB_8705_D1A7BDA5A401__INCLUDED_)
#define AFX_MYTREEDIALOG_H__00B921AA_8B84_46EB_8705_D1A7BDA5A401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTreeDialog.h : ヘッダー ファイル
#include "Charu3Tree.h"

//---------------------------------------------------
// CMyTreeDialog ダイアログ
//---------------------------------------------------
class CMyTreeDialog : public CDialog
{
// コンストラクション
public:
	CMyTreeDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CMyTreeDialog() {
		if(m_hDLL)	::FreeLibrary(m_hDLL);
		m_PopupMenu.DestroyMenu();
	}
	void setTree(CCharu3Tree *pTreeCtrl) {
		m_pTreeCtrl = pTreeCtrl;
	}
	BOOL showWindowPos(POINT pos,POINT size,int nCmdShow,bool isSelect,HTREEITEM hOpenItem = NULL);
	void enterData(list<STRING_DATA>::iterator it);
	void closePopup();

	list<STRING_DATA>::iterator m_selectIT;
// ダイアログ データ
	//{{AFX_DATA(CMyTreeDialog)
	enum { IDD = IDD_POPUP_DIALOG };
	CCharu3Tree	*m_pTreeCtrl;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyTreeDialog)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	int static m_stKeycode[];
	char static *m_stKeyName[];
	char static *m_stKeyNameShift[];

	HMODULE m_hDLL;
	typedef DWORD (WINAPI *PFUNC)(HWND,DWORD,BYTE,DWORD);
	PFUNC m_pExStyle;
	CFont *m_cFont,*m_cOlgFont;

	CToolTipCtrl m_toolTip;//ツールチップ
	bool m_isInitOK,m_isModal;
	bool m_isAltDown;
	list<STRING_DATA>::iterator m_itDbClick;

	CBrush m_brBack;
	CMenu m_PopupMenu;
	HTREEITEM m_hCopyData;
	COLORREF m_colFrameL,m_colFrame,m_colFrameD;

	DWORD m_dwStartTime;
	CString m_strQuickKey;
	HTREEITEM m_hQuickItem;

	void changeTipString(STRING_DATA data);	
	void drawFrame(CDC* pDC, CRect& rect);
	void drawLline(CDC* pDC, CPoint* point, COLORREF col);

	void pouupMenu(CPoint point);
	bool quickAccess(UINT uKeyCode);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMyTreeDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnChangeOnetime();
	afx_msg void OnChangeLock();
	afx_msg void OnCopyData();
	afx_msg void OnDataPaste();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnExport();
	afx_msg void OnFolderClear();
	afx_msg void OnIconClip();
	afx_msg void OnIconDate();
	afx_msg void OnIconExe();
	afx_msg void OnIconKey();
	afx_msg void OnIconKeymacro();
	afx_msg void OnIconPlugin();
	afx_msg void OnIconRelate();
	afx_msg void OnIconSelect();
	afx_msg void OnImport();
	afx_msg void OnListSerch();
	afx_msg void OnNewFolder();
	afx_msg void OnOption();
	afx_msg void OnRclickMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnNcPaint(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBeginlabeleditMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckItem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYTREEDIALOG_H__00B921AA_8B84_46EB_8705_D1A7BDA5A401__INCLUDED_)
