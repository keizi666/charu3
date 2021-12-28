/*----------------------------------------------------------
	ClipBordクラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_CLIPBORD_H__582AEB30_7E55_42E1_8D77_A28FAD0F7294__INCLUDED_)
#define AFX_CLIPBORD_H__582AEB30_7E55_42E1_8D77_A28FAD0F7294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------
// クラス定義
//---------------------------------------------------
class CClipBord  
{
public:
	CClipBord();
	virtual ~CClipBord();
	void setParent(HWND hWnd){
		m_hParentWnd = hWnd;
		m_hNextClipbord = ::SetClipboardViewer(hWnd);
	}
	BOOL getClipboardText(CString& sData);
	BOOL setClipboardText(const TCHAR *szString);

	void delClipView(HWND hWnd){
	    ::ChangeClipboardChain(hWnd,getNextCb());
	}
	void resetClipView(HWND hWnd){
		if(GetClipboardViewer() != hWnd){
			::ChangeClipboardChain(hWnd,getNextCb());
			m_hNextClipbord = NULL;
			m_hNextClipbord = ::SetClipboardViewer(hWnd);
		}
	}

	HWND getNextCb() {return m_hNextClipbord;};
	void setNextCb(HWND hWnd) {m_hNextClipbord = hWnd;};
protected:
	HWND m_hParentWnd;
	HWND m_hNextClipbord;
};

#endif // !defined(AFX_CLIPBORD_H__582AEB30_7E55_42E1_8D77_A28FAD0F7294__INCLUDED_)
