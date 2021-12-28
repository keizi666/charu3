/*----------------------------------------------------------
	テキスト色々変換
									2001/08/04 (c)C+ Factory
----------------------------------------------------------*/

#include "stdafx.h"
#include "..\PsTxCvtL115\PsTxCvtL.h"

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

extern "C" __declspec (dllexport) bool CharuPlugIn
	(TCHAR *strSource,TCHAR *strResult,int nSize,STRING_DATA *data,void *pVoid)
{
	bool isRet = false;
	*strResult = NULL;
	
	//---------------ここからコードを書くといいです---------------------
	int outFile;
	char *strTmp,*strTmp2,*szCommma;;
	int nType;
	strTmp = new char[nSize];
	int nLength;

	#ifdef _UNICODE
		ZeroMemory(strTmp,sizeof(strTmp));
		nLength = ::WideCharToMultiByte(CP_ACP, 0, strSource,_tcsclen(strSource),NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP,0,strSource,nLength,strTmp,nSize,"",NULL);
	#else
		strcpy(strTmp,strSource);
	#endif

	if(sscanf(strTmp,"%d,%s",&nType) != 2) {
		delete [] strTmp;
		return isRet;
	}
	bool isCamma = false;
	szCommma = strTmp;
	strTmp2 = strTmp;
	for(;*szCommma != NULL; szCommma++) {
		if(isCamma) {
			*strTmp = *szCommma;
			strTmp++;
		}
		if(!isCamma && *szCommma == ','){
			isCamma = true;
		}
	}
	*strTmp = NULL;
	strTmp = strTmp2;
	
	if(nType <111 || nType > 416) {
		delete [] strTmp;
		AfxMessageBox(_T("変換タイプが不正です。"));
		return isRet;
	}

	if((outFile = _topen(_T("convTempIn.$$$"), _O_CREAT | _O_BINARY | _O_WRONLY)) == -1) {
		delete [] strTmp;
		AfxMessageBox(_T("ファイルI/Oエラー create"));
		return isRet;
	}
	nLength = strlen(strTmp);
	_write(outFile,strTmp,nLength);
	close(outFile);
	
	Pfc_TxCvt("convTempIn.$$$","convTempOut.$$$",nType);

	CFile fileIn;
	try {
		fileIn.Open(_T("convTempOut.$$$"),CFile::modeRead);
	}
	catch (CFileException *e){
		delete [] strTmp;
		AfxMessageBox(_T("ファイルI/Oエラー read"));
		e->Delete();
		return isRet;
	}
	#ifdef _UNICODE
		fileIn.Read(strTmp,fileIn.GetLength());
		strTmp[fileIn.GetLength()] = NULL;
		ZeroMemory(strResult,sizeof(strResult));
		nLength = MultiByteToWideChar(CP_ACP,0,strTmp,-1,strResult,nSize);
	#else
		fileIn.Read(strResult,fileIn.GetLength());
		strResult[fileIn.GetLength()] = NULL;
	#endif

	fileIn.Close();

	SetFileAttributes(_T("convTempIn.$$$"),0x20);
	SetFileAttributes(_T("convTempOut.$$$"),0x20);
	DeleteFile(_T("convTempIn.$$$"));
	DeleteFile(_T("convTempOut.$$$"));
	delete [] strTmp;
	//---------------ここまでコードを書くといいです---------------------
	isRet = true;
	return isRet;
}

