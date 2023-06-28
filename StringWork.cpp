/*----------------------------------------------------------
	文字列処理クラス
									2002/11/20 (c)keizi
----------------------------------------------------------*/

#include "StringWork.h"

//---------------------------------------------------
//関数名	trim(char *strSource)		[public]
//機能		文字列の前後のスペースを取る
//---------------------------------------------------
int UStringWork::trim(TCHAR *strSource)
{
	int nRet = 0;

	nRet = trimBack(strSource);
	nRet = trimFront(strSource);

	return nRet;
}


//---------------------------------------------------
//関数名	trimFront(char *strSource)	[public]
//機能		文字列の前のスペースを取る
//---------------------------------------------------
int UStringWork::trimFront(TCHAR *strSource)
{
	int nRet = 0;
	bool isSpace = false;
	TCHAR *szStart = strSource;

	for( ; *strSource != (TCHAR)NULL; strSource++) {
		if(!isSpace && *strSource != _T(' ') && *strSource != (TCHAR)0x0d && *strSource != (TCHAR)0x0a) {
			isSpace = true;
		}
		if(isSpace) {
			*szStart = *strSource;
			szStart++;
			nRet++;
		}
	}
	*szStart = (TCHAR)NULL;
	return nRet;
}

//---------------------------------------------------
//関数名	trimBack(char *strSource)	[public]
//機能		文字列の後ろのスペースを取る
//---------------------------------------------------
int UStringWork::trimBack(TCHAR *strSource)
{
	int nRet = 0;
	bool isSpace = false;
	TCHAR *szEnd = strSource + _tcsclen(strSource)-1;

	for(; *szEnd == _T(' ') || *szEnd == (TCHAR)0x0d || *szEnd == (TCHAR)0x0a; szEnd--) {
		if(szEnd == strSource) break;
	}
	szEnd++;
	*szEnd = (TCHAR)NULL;
	return _tcsclen(szEnd);
}

//---------------------------------------------------
//関数名	splitString(char split, ...)	[public]
//機能		指定文字で文字列を分割する
//---------------------------------------------------
int UStringWork::splitString(TCHAR *strSource,TCHAR split, ...)
{
	va_list marker;
	TCHAR **strTarget;
	int nRet = 0;

	va_start(marker,split);//変数リストの初期化
	strTarget = va_arg( marker, TCHAR**);//先頭をコピー

	while(strTarget != (TCHAR**)NULL) {//変数リストの最後まで
		if(*strSource)	nRet++;
		*strTarget = *strSource ? strSource: (TCHAR*)NULL;
		for(; *strSource; strSource++) {
			if(isSJIS(strSource)) {
				*strSource++;
				continue;
			}
			if(*strSource == split) {//分割文字列まで
				*strSource = (TCHAR)NULL;//分割文字列にNULLを入れてポインタを進める
				*strSource++;
				break;
			}
		}
		strTarget = va_arg( marker, TCHAR**);//次の変数リストを取得
	}
	va_end( marker );

	return nRet;
}

//---------------------------------------------------
//関数名	splitStringSame(char split, ...)[public]
//機能		指定文字で文字列を分割する
//---------------------------------------------------
int UStringWork::splitStringSame(TCHAR *strSource,TCHAR split, ...)
{
	va_list marker;
	TCHAR **strTarget;
	int nRet = 0;
	bool isSame = false;

	va_start(marker,split);//変数リストの初期化
	strTarget = va_arg( marker, TCHAR**);//先頭をコピー

	while(strTarget != (TCHAR**)NULL) {//変数リストの最後まで
		if(*strSource)	nRet++;
		*strTarget = *strSource ? strSource: (TCHAR*)NULL;
		for(; *strSource; strSource++) {
			if(isSJIS(strSource)) {
				*strSource++;
				continue;
			}
			if(*strSource == split && !isSame) {//分割文字列まで
				*strSource = (TCHAR)NULL;//分割文字列にNULLを入れてポインタを進める
				*strSource++;
				isSame = true;
				break;
			}
			else if(isSame && *strSource != split) isSame = false;
		}
		strTarget = va_arg( marker, TCHAR**);//次の変数リストを取得
	}
	va_end( marker );

	return nRet;
}

//---------------------------------------------------
//関数名	awk(char *strSource,int nAwk,char cSplit)
//機能		指定文字区切りで文字列を切り出す
//---------------------------------------------------
TCHAR* UStringWork::awk(TCHAR *strSource,TCHAR *strRet,int nSize,int nAwk,TCHAR cSplit /* = 0x20*/)
{
	bool isSpace = false;
	TCHAR *szStart = NULL;

	if(nSize < 1 || nAwk < 1 || *strSource == NULL) return strSource;
	while(*strSource != NULL) {

		if(!isSJIS(strSource)) {
			if(!isSpace) {
				if(*strSource == cSplit) {//スプリッタが見つかった
					isSpace = true;
				}
			}
			else if(isSpace) {
				if(*strSource != cSplit) {//スプリッタじゃ無くなった
					nAwk--;//カウンタを減らす
					isSpace = false;
				}
			}
		}
		else if(isSpace){
			nAwk--;//カウンタを減らす
			isSpace = false;
		}
		if(nAwk == 1){
			if(!szStart)	szStart = strSource - 1;

			if(*strSource == 0x0a || *strSource == 0x0d || *strSource == cSplit ||
				*strSource == 0x1a|| *strSource == NULL) {
				break;
			}
			if(nSize <= 1) break;
			*strRet = *strSource;
			if(isSJIS(strSource)) {
				strSource++;//ポインタを進める
				strRet++;
				*strRet = *strSource;
			}
			nSize--;
			strRet++;
		}
		else if(isSJIS(strSource))	strSource++;//ポインタを進める
		strSource++;//ポインタを進める
	}
	*strRet = (TCHAR)NULL;

	return szStart;
}

//---------------------------------------------------
//関数名	isSJIS(char *szSource)
//機能		S-JISかどうかを判別
//---------------------------------------------------
bool UStringWork::isSJIS(TCHAR *szSource)
{
	bool isRet = false;
#ifdef _UNICODE
	return false;
#endif
	if(*szSource == NULL) return isRet;
	if(((unsigned char)*szSource >= 0x81 && (unsigned char)*szSource <= 0x9f) ||
		((unsigned char)*szSource >= 0xe0 && (unsigned char)*szSource <= 0xef)) 
		if(((unsigned char)*(szSource + 1) >= 0x40 && (unsigned char)*(szSource + 1) <= 0x7e) ||
			((unsigned char)*szSource >= 0x80 && (unsigned char)*szSource <= 0xfc)) 
			isRet = true;

	return isRet;
}

//---------------------------------------------------
//関数名	compressionSpace(TCHAR *szSource)
//機能		複数のスペースを一つに纏める
//---------------------------------------------------
int UStringWork::compressionSpace(TCHAR *szSource)
{
	int i;
	for(i = 0; *szSource ; i++,szSource++) {
		if(*szSource == _T(' ')) {
			TCHAR *szSpaceEnd;
			szSpaceEnd = szSource;
			int j;
			for(j = 0; *szSpaceEnd && *szSpaceEnd == _T(' ') ; j++,szSpaceEnd++);
			UStringWork::moveForward(szSpaceEnd,j-1);
			szSource++;
		}
	}
	return i;
}

//---------------------------------------------------
//関数名	moveForward(TCHAR *szMovePos,int nMove)
//機能		文字列を前に移動(メモリ範囲に注意)
//---------------------------------------------------
int UStringWork::moveForward(TCHAR *szMovePos,int nMove)
{
	//文字列移動
	int i;
	for(i = 0; *szMovePos ; i++,szMovePos++) {
		*(szMovePos - nMove) = *szMovePos;
	}
	*(szMovePos - nMove) = NULL;
	return i;
}

