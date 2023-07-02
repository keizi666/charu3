/*----------------------------------------------------------
	文字列処理クラスヘッダ
									2002/11/20 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
#define AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_

#include <stdarg.h>
#include <tchar.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct KANSUUJI
{
	char strKANJI[3];
	int  nNumber;
};

//---------------------------------------------------
//　クラス宣言
//---------------------------------------------------
class UStringWork
{
public:
	static int splitString(TCHAR *strSource,TCHAR split, ...);
	static int splitStringSame(TCHAR *strSource,TCHAR split, ...);
	static int trimFront(TCHAR *strSource);
	static int trimBack(TCHAR *strSource);
	static int trim(TCHAR *strSource);
	static TCHAR* awk(TCHAR *strSource,TCHAR *strRet,int nSize,int nAwk,TCHAR cSplit = 0x20);
	static bool isSJIS(TCHAR *szSource);
	static int compressionSpace(TCHAR *szSource);
	static int moveForward(TCHAR *szMovePos,int nMove);
};

#endif // !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
