/*----------------------------------------------------------
	Charu3
	検索ダイアログ
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "SerchDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// アプリケーションオブジェクト
extern	CCharu3App theApp;

//---------------------------------------------------
// CSerchDialog ダイアログ
//---------------------------------------------------
BEGIN_MESSAGE_MAP(CSearchDialog, CDialog)
	//{{AFX_MSG_MAP(CSerchDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------
//コンストラクタ
//---------------------------------------------------
CSearchDialog::CSearchDialog(CWnd* pParent /*=NULL*/)	: CDialog(CSearchDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerchDialog)
	m_nSearchByName = (theApp.m_ini.m_nSearchTarget & SEARCH_TARGET_NAME) ? 1 : 0;
	m_nSearchByData = (theApp.m_ini.m_nSearchTarget & SEARCH_TARGET_DATA) ? 1 : 0;
	m_nSearchLogic = theApp.m_ini.m_nSearchLogic;
	m_strSearchKeywords = _T("");
	//}}AFX_DATA_INIT
}

//---------------------------------------------------
//データエクスチェンジ
//---------------------------------------------------
void CSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerchDialog)
	if(GetDlgItem(IDC_SEARCH_NAME))
		DDX_Check(pDX, IDC_SEARCH_NAME, m_nSearchByName);
	if (GetDlgItem(IDC_SEARCH_DATA))
		DDX_Check(pDX, IDC_SEARCH_DATA, m_nSearchByData);
	if(GetDlgItem(IDC_RADIO_AND))
		DDX_Radio(pDX, IDC_RADIO_AND, m_nSearchLogic);
	if(GetDlgItem(IDC_SEARCH_TEXT))
		DDX_Text(pDX, IDC_SEARCH_TEXT, m_strSearchKeywords);
	//}}AFX_DATA_MAP
}

int CSearchDialog::GetTarget()
{
	return (m_nSearchByName ? SEARCH_TARGET_NAME : 0) | (m_nSearchByData ? SEARCH_TARGET_DATA : 0);
}

int CSearchDialog::GetSearchLogic()
{
	return m_nSearchLogic;
}
