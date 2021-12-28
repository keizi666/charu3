/*----------------------------------------------------------
	文字列処理クラスヘッダ
									2001/11/20 (c)Wizard
----------------------------------------------------------*/

#if !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
#define AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <vector>

using namespace std;

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
	static int splitString(char *strSource,char split, ...);
	static int splitStringSame(char *strSource,char split, ...);
	static time_t strtime2TimeT_S(char *strSource);
	static time_t strtime2TimeT_A(char *strTime);
	static int trimFront(char *strSource);
	static int trimBack(char *strSource);
	static int trim(char *strSource);
	static char* awk(char *strSource,char *strRet,int nSize,int nAwk,char cSplit = 0x20);
	static char* serchString(char *strSource,char *strSerch,int *nFoundSize,int nStart = 0);
	static bool isSJIS(char *szSource);

	static void convertKansuuji(char *strSource,char *strDst,int nSize);

	static char* GetIPv4String(char* overSize16Buffer, int nIPv4);
	static char* GetIPv4MaskString(char* overSize16Buffer, int under32);
	static int URLEncode(const char *pszSrc,int nSrcLength,char *pszBuf);
	static int URLDecode(const char *pszSrc,char *pszBuf);
	static int HexToDec(char c);

protected:
	static char* addNumberString(int nNum,char *szDst);




};

#endif // !defined(AFX_STRINGWORK_H__E26F2A4C_01CC_4E2E_8A31_1E4C6D507C1E__INCLUDED_)
