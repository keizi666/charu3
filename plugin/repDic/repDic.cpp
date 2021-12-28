/*----------------------------------------------------------
	置換
									2003/03/24 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"
#include "StringWork.h"

#include <list>
using namespace std;

//---------------------------------------------------
// データ構造体
//---------------------------------------------------
struct STRING_DATA
{
	char	m_cKind;		//データ種別
	char	m_cIcon;		//アイコン種別
	int		m_nMyID;		//データのID
	int		m_nParentID;	//親データのID
	time_t	m_timeCreate;	//作成日時
	time_t	m_timeEdit;		//変更日時
	CString  m_strTitle;	//設定データタイトル
	CString  m_strData;		//設定データ文字列
	CString  m_strMacro;	//拡張用文字列データ
};

struct DIC_DATA
{
	CString strSrc;
	CString strDst;
};

extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{

	bool isRet = false;
	*strResult = NULL;
	CString strTmp;
	strTmp = strSource;
	//---------------ここからコードを書くといいです---------------------

	list<DIC_DATA> lstDic;
	FILE *InFile;
	TCHAR strBuff[65535];//,*szSplit1,*szSplit2;
	CString strFileName,strReadBuff,strSplit1,strSplit2;

	int nFound = strTmp.Find(_T(","));
	if(nFound >= 0 && nFound < 256 && nFound < strTmp.GetLength()) {
		strFileName = strTmp.Left(nFound);
		strTmp = strTmp.Right(strTmp.GetLength() - nFound - 1);

		//辞書読み込み
		DIC_DATA dicData;
		if((InFile = _tfopen(strFileName,_T("r"))) == NULL)	return isRet;
		while(!feof(InFile)) {
			if(!_fgetts(strBuff,sizeof(strBuff),InFile)) break;//読む
AfxMessageBox(strBuff);
			strReadBuff = strBuff;
			nFound = strReadBuff.Find(_T("\t"));
			strSplit1 = strReadBuff.Left(nFound); 
			strSplit2 = strReadBuff.Right(strReadBuff.GetLength() - nFound - 1);
			if(strSplit1 != "" && strSplit2 != "") {
//			if(UStringWork::splitString(strBuff,'\t',&szSplit1,&szSplit2,NULL) == 2) {
				strReadBuff = strSplit1;
				strReadBuff.Replace(_T("\\n"),_T("\n"));
				strReadBuff.Replace(_T("\\t"),_T("\t"));
				strReadBuff.Replace(_T("\\\\"),_T("\\"));
				dicData.strSrc = strReadBuff;
				strReadBuff = strSplit2;
				strReadBuff.Replace(_T("\n"),_T(""));
				strReadBuff.Replace(_T("\\n"),_T("\n"));
				strReadBuff.Replace(_T("\\t"),_T("\t"));
				strReadBuff.Replace(_T("\\\\"),_T("\\"));
				dicData.strDst = strReadBuff;
				lstDic.push_back(dicData);
			}
		}
		//置換処理
		list<DIC_DATA>::iterator it;
		for(it = lstDic.begin(); lstDic.end() != it; it++) {
			strTmp.Replace(it->strSrc,it->strDst);
		}

		_tcscpy(strResult,strTmp.GetBuffer(strTmp.GetLength()));	
		isRet = true;
	}
	//---------------ここまでコードを書くといいです---------------------
	return isRet;
}


