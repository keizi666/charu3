// HotkeyMgr.h: CHotkeyMgr クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOTKEYMGR_H__037D6F91_6895_4E1A_8C4D_0A874F9BC77B__INCLUDED_)
#define AFX_HOTKEYMGR_H__037D6F91_6895_4E1A_8C4D_0A874F9BC77B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHotkeyMgr  
{
public:
	CHotkeyMgr(HWND m_hWnd,);
	virtual ~CHotkeyMgr();

protected:
	HWND m_hWnd;

};

#endif // !defined(AFX_HOTKEYMGR_H__037D6F91_6895_4E1A_8C4D_0A874F9BC77B__INCLUDED_)
