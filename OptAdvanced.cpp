/*----------------------------------------------------------
	Setting Dialog - Advanced
----------------------------------------------------------*/

#include "stdafx.h"
#include "OptAdvanced.h"
#include "Charu3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCharu3App theApp;

//---------------------------------------------------
//関数名	DoDataExchange(CDataExchange* pDX)
//機能		データエクスチェンジ
//---------------------------------------------------
void COptAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(COptEtc)
	if (GetDlgItem(IDC_OPT_CLIPBOARD_OPEN_DELAY)) {
		DDX_Text(pDX, IDC_OPT_CLIPBOARD_OPEN_DELAY, theApp.m_ini.m_nClipboardOpenDelay);
		// Not worked correctly
		//DDV_MinMaxInt(pDX, theApp.m_ini.m_nClipboardOpenDelay, 0, 4000);
	}
	if (GetDlgItem(IDC_OPT_CLIPBOARD_RETRY_INTERVAL)) {
		DDX_Text(pDX, IDC_OPT_CLIPBOARD_RETRY_INTERVAL, theApp.m_ini.m_nClipboardRetryInterval);
		// Not worked correctly
		//DDV_MinMaxInt(pDX, theApp.m_ini.m_nClipboardRetryInterval, 1, 1000);
	}
	if (GetDlgItem(IDC_OPT_CLIPBOARD_RETRY_TIMES)) {
		DDX_Text(pDX, IDC_OPT_CLIPBOARD_RETRY_TIMES, theApp.m_ini.m_nClipboardRetryTimes);
		// Not worked correctly
		//DDV_MinMaxInt(pDX, theApp.m_ini.m_nClipboardRetryTimes, 0, 100);
	}
	if (GetDlgItem(IDC_OPT_WINDOW_CHECK_INTERVAL)) {
		DDX_Text(pDX, IDC_OPT_WINDOW_CHECK_INTERVAL, theApp.m_ini.m_nWindowCheckInterval);
		// Not worked correctly
		//DDV_MinMaxInt(pDX, theApp.m_ini.m_nWindowCheckInterval, 1, 2000);
	}
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptAdvanced, CDialog)
	//{{AFX_MSG_MAP(COptAdvanced)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
