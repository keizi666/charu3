/*----------------------------------------------------------
	Charu3Treeクラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_CHARU3TREE_H__246081ED_82BD_4EFE_AA98_067AC36E58E6__INCLUDED_)
#define AFX_CHARU3TREE_H__246081ED_82BD_4EFE_AA98_067AC36E58E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Charu3Tree.h : ヘッダー ファイル
//

#include "Charu3.h"


#include <list>
#include <vector>
using namespace std;


//---------------------------------------------------
// プラグイン構造体
//---------------------------------------------------
struct RW_PLUGIN
{
	CString m_strPluginName;
	CString m_strSoftName;
	CString m_strExtension;
};

//---------------------------------------------------
// データ構造体
//---------------------------------------------------
struct STRING_DATA
{
	char	m_cKind;		//データ種別
	char	m_cIcon;		//アイコン種別
	int		m_nMyID;		//データのID
	int		m_nParentID;	//親データのID
	time_t	m_timeCreate;	//作成日時 32bit
	time_t	m_timeEdit;		//変更日時 32bit
	CString  m_strTitle;	//設定データタイトル
	CString  m_strData;		//設定データ文字列
	CString  m_strMacro;	//拡張用文字列データ
};

typedef bool (*writeDataFile)(CString strFileName,list<STRING_DATA> *pList);
typedef list<STRING_DATA> * (*readDataFile)(CString strFileName,bool isImport);
typedef bool (*convertMacro)(TCHAR *szSource,TCHAR *szRet,int nSize,TCHAR *szSelect,TCHAR *szClip);
typedef void (*getExtension)(TCHAR *szBuff,int nSize);
typedef void (*getFormatName)(TCHAR *szBuff,int nSize);
typedef void (*endDLL)();
typedef void (*initDLL)();
typedef bool (*isUnicode)();

//---------------------------------------------------
// CCharu3Tree ウィンドウ
//---------------------------------------------------
class CCharu3Tree : public CTreeCtrl
{
// コンストラクション
public:
	CCharu3Tree();
	virtual ~CCharu3Tree();

public:
	bool setPlugin(CString strPath);
	bool getPlugin(CString strName,RW_PLUGIN* pPlugin);
	void setImageList(POINT posSize,CString strFileName,CString strPath);
	HTREEITEM addData(HTREEITEM hTreeItem,STRING_DATA Data,bool isNewID = true,bool isChild = false);
	HTREEITEM addNewFolder(HTREEITEM hTreeItem,CString strName);

	void deleteData(HTREEITEM hTreeItem);
	void clearFolder(HTREEITEM hStartItem);
	void closeFolder(HTREEITEM hStartItem);
	void checkFolder(HTREEITEM hStartItem,bool isCheck,list<HTREEITEM> *listItem);
	void clearOneTime(HTREEITEM hStartItem,int nKind = 0);
	void removeCheck();

	void changeIcon(HTREEITEM hTreeItem,int nID);
	void editData(HTREEITEM hTreeItem,STRING_DATA Data);
	void tree2List(HTREEITEM hStartItem,list<STRING_DATA> *tmplist,bool isAll=false);
	void data2TreeStruct(TV_INSERTSTRUCT *pTreeCtrlItem,list<STRING_DATA>::iterator it);
	list<STRING_DATA>::iterator findData(STRING_DATA* dataPtr);

	STRING_DATA getData(HTREEITEM hTreeItem);
	STRING_DATA* getDataPtr(HTREEITEM hTreeItem);

	HTREEITEM serchItem(int nKind,int nLogic,CString strKey,HTREEITEM hStartItem);
	HTREEITEM serchTitle(HTREEITEM hStartItem,CString strKey,int isLower = 0);
	HTREEITEM getTrueNextItem(HTREEITEM hTreeItem);
	HTREEITEM getLastVisibleItem();
	HTREEITEM getFirstFolder(HTREEITEM hStartItem);
	HTREEITEM getLastChild(HTREEITEM hStartItem);

	void copyChildren(HTREEITEM hFromItem,HTREEITEM hToItem);
	void moveChildren(HTREEITEM hFromItem,HTREEITEM hToItem);

	int  getIconNumber(char cKind,char cIcon);
	int  getChildCount(HTREEITEM hTreeItem,bool isBrotherOnly = false);
	char getDatakind(HTREEITEM hTreeItem){
		char cRet = 0;
		if(hTreeItem) {
			STRING_DATA data = getData(hTreeItem);
			cRet = data.m_cKind;
		}
		return cRet;
	}

	void deleteExcessChildren(HTREEITEM hTreeItem,int *nCount);
	void copyData(int nParentID,HTREEITEM hParentTreeItem,list<STRING_DATA> *pList);
	HTREEITEM mergeTreeData(HTREEITEM hTreeItem,list<STRING_DATA> *pList,bool isRoot);
	int  mergeList(list<STRING_DATA> *pMainList,list<STRING_DATA> *pList,int nParent);

	bool loadDataFileDef(CString strFileName,CString strPlugin);
	bool loadDataFilePlugin(CString strFileName,CString strPlugin,list<STRING_DATA> *tmplist);
	bool loadDataFile(CString strFileName,CString strPlugin,list<STRING_DATA> *tmplist);
	void normalizationID(list<STRING_DATA>* pList,int nParentID);
	bool convertMacroPlugin(STRING_DATA *SourceData,CString *strRet,CString strSelect,CString strClip,CString strSoftName);

	bool saveDataFilePlugin(CString strFileName,CString strPlugin,list<STRING_DATA> *tmplist);
	bool saveDataFile(CString strFileName,CString strPlugin,HTREEITEM hStartItem);
	int  makeNewID(){
		(*m_nMaxID)++;
		while(checkRedundancyID(*m_nMaxID)) {
			(*m_nMaxID) += 10;
		}
		return *m_nMaxID;
	}
	void initStringData(STRING_DATA *pData){
		pData->m_cKind = 0;		//データ種別
		pData->m_cIcon = 0;	//アイコン種別
		pData->m_nMyID = 0;		//データのID
		pData->m_nParentID = 0;	//親データのID
		pData->m_timeCreate = 0;	//作成日時
		pData->m_timeEdit = 0;		//変更日時
		pData->m_strTitle = "";	//設定データタイトル
		pData->m_strData = "";		//設定データ文字列
		pData->m_strMacro = "";	//拡張用文字列データ
	}
	
		
	void setInitInfo(int *nMaxID,int *nSelectID,int *nRecNumber){
		m_nMaxID = nMaxID;
		m_nSelectID = nSelectID;
		m_nRecNumber = nRecNumber;
	}
	void setSelectID(int nID){*m_nSelectID = nID;}
	
	bool checkRedundancyID(int nID);
	bool checkMyChild(HTREEITEM hMeItem,HTREEITEM hChildItem);
	HTREEITEM serchMyRoots(HTREEITEM hStartItem);
	HTREEITEM serchParentOption(HTREEITEM hStartItem,CString strOption);

	void allIconCheck();
	char decideIcon(CString strData);
	CString makeTitle(CString strData,int nTitleLength = 32);
	HTREEITEM getOneTimeText(int nType);

	int getDataOption(CString strData,CString strKind);
	DWORD getDataOptionHex(CString strData,CString strKind);
	CString getDataOptionStr(CString strData,CString strKind);
	void addDataToRecordFolder(STRING_DATA data,CString strClipBkup);
	HTREEITEM moveFolderTop(HTREEITEM hTreeItem);
	void classHistoryFolder(HTREEITEM hTreeItem,int nRirekiCount);
	void checkItem(HTREEITEM hItem);
	void checkOut(HTREEITEM hItem);

	list<STRING_DATA> m_MyStringList;//データリスト
	vector<RW_PLUGIN>  m_rwPlugin;
	list<HTREEITEM> m_ltCheckItems;
	void setScrollBar();
	bool isDrag() { return m_isDrag;}

protected:
	int *m_nMaxID;
	int *m_nRecNumber;
	bool m_isDrag;
	CImageList* m_pDragImage;
	HTREEITEM m_hDragItem,m_hDragTarget,m_hSerchItem;
	HTREEITEM m_hPrevTarget;

	// オペレーション
public:
	CWnd *m_pParent;
	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCharu3Tree)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL


	// 生成されたメッセージ マップ関数
protected:
	CImageList *m_ImageList;//イメージリスト
	int *m_nSelectID;

	//{{AFX_MSG(CCharu3Tree)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHARU3TREE_H__246081ED_82BD_4EFE_AA98_067AC36E58E6__INCLUDED_)
