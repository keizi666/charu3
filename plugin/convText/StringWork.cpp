/*----------------------------------------------------------
	文字列処理クラス
									2001/11/20 (c)Wizard
----------------------------------------------------------*/

#include "stdafx.h"
#include "StringWork.h"

//---------------------------------------------------
//関数名	trim(char *strSource)		[public]
//機能		文字列の前後のスペースを取る
//---------------------------------------------------
int UStringWork::trim(char *strSource)
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
int UStringWork::trimFront(char *strSource)
{
	int nRet = 0;
	bool isSpace = false;
	char *szStart = strSource;

	for( ; *strSource != (char)NULL; strSource++) {
		if(!isSpace && *strSource != ' ' && *strSource != (char)0x0d && *strSource != (char)0x0a) {
			isSpace = true;
		}
		if(isSpace) {
			*szStart = *strSource;
			szStart++;
			nRet++;
		}
	}
	*szStart = (char)NULL;
	return nRet;
}

//---------------------------------------------------
//関数名	trimBack(char *strSource)	[public]
//機能		文字列の後ろのスペースを取る
//---------------------------------------------------
int UStringWork::trimBack(char *strSource)
{
	int nRet = 0;
	bool isSpace = false;
	char *szEnd = strSource + strlen(strSource)-1;

	for(; *szEnd == ' ' || *szEnd == (char)0x0d || *szEnd == (char)0x0a; szEnd--) {
		if(szEnd == strSource) break;
	}
	szEnd++;
	*szEnd = (char)NULL;
	return strlen(szEnd);
}

//---------------------------------------------------
//関数名	splitString(char split, ...)	[public]
//機能		指定文字で文字列を分割する
//---------------------------------------------------
int UStringWork::splitString(char *strSource,char split, ...)
{
	va_list marker;
	char **strTarget;
	int nRet = 0;

	va_start(marker,split);//変数リストの初期化
	strTarget = va_arg( marker, char**);//先頭をコピー

	while(strTarget != (char**)NULL) {//変数リストの最後まで
		if(*strSource)	nRet++;
		*strTarget = *strSource ? strSource: (char*)NULL;
		for(; *strSource; strSource++) {
			if(isSJIS(strSource)) {
				*strSource++;
				continue;
			}
			if(*strSource == split) {//分割文字列まで
				*strSource = (char)NULL;//分割文字列にNULLを入れてポインタを進める
				*strSource++;
				break;
			}
		}
		strTarget = va_arg( marker, char**);//次の変数リストを取得
	}
	va_end( marker );

	return nRet;
}

//---------------------------------------------------
//関数名	splitStringSame(char split, ...)[public]
//機能		指定文字で文字列を分割する
//---------------------------------------------------
int UStringWork::splitStringSame(char *strSource,char split, ...)
{
	va_list marker;
	char **strTarget;
	int nRet = 0;
	bool isSame = false;

	va_start(marker,split);//変数リストの初期化
	strTarget = va_arg( marker, char**);//先頭をコピー

	while(strTarget != (char**)NULL) {//変数リストの最後まで
		if(*strSource)	nRet++;
		*strTarget = *strSource ? strSource: (char*)NULL;
		for(; *strSource; strSource++) {
			if(isSJIS(strSource)) {
				*strSource++;
				continue;
			}
			if(*strSource == split && !isSame) {//分割文字列まで
				*strSource = (char)NULL;//分割文字列にNULLを入れてポインタを進める
				*strSource++;
				isSame = true;
				break;
			}
			else if(isSame && *strSource != split) isSame = false;
		}
		strTarget = va_arg( marker, char**);//次の変数リストを取得
	}
	va_end( marker );

	return nRet;
}

//---------------------------------------------------
//関数名	strtime2TimeT_S(char *strSource)[public]
//機能		文字列の時間を変換
//---------------------------------------------------
time_t UStringWork::strtime2TimeT_S(char *strSource)
{
	//YYYY/MM/DD-HH:MM:SS形式を変換
	tm timeptr;
	time_t timet = (time_t)NULL;
	char *szDate,*szTime;
	char *szTmp[3];

	memset(&timeptr,0x00,sizeof(timeptr));

	if(UStringWork::splitString(strSource,'-',&szDate,&szTime,NULL) != 2) return timet;

	//日付を変換
	if(splitString(szDate,'/',&szTmp[0],&szTmp[1],&szTmp[2],NULL) != 3) return timet;
	timeptr.tm_year = atoi(szTmp[0]);
	timeptr.tm_mon  = atoi(szTmp[1]);
	timeptr.tm_mday = atoi(szTmp[2]);

	//時間を変換
	if(splitString(szTime,':',&szTmp[0],&szTmp[1],&szTmp[2],NULL) != 3) return timet;
	timeptr.tm_hour = atoi(szTmp[0]);
	timeptr.tm_min  = atoi(szTmp[1]);
	timeptr.tm_sec  = atoi(szTmp[2]);

	timeptr.tm_year -= 1900;
	timet = mktime(&timeptr);

	return timet;
}

//---------------------------------------------------
//関数名	time_t strtime2TimeT_A(char *strTime)
//機能		時間を変換
//---------------------------------------------------
time_t UStringWork::strtime2TimeT_A(char *strTime)
{
	//Oct 15 15:49:38形式を変換
	char *strMonthList[] = {"Jan","Feb","Mar","Apr","May","Jun",
							"Jul","Aug","Sep","Oct","Nov","Dec"};
	tm timeptr,*nowTm;
	time_t timet,nowTime;
	char *szMonth,*szDay,*szTime;
	char *szTmp[3];

	//現在の年を取る
	time(&nowTime);
	nowTm = localtime(&nowTime);

	memset(&timeptr,0x00,sizeof(timeptr));
	memset(&timet,0x00,sizeof(timet));

	//日付を分割
	if(splitString(strTime,' ',&szMonth,&szDay,&szTime,NULL) != 3) return timet;

	//時間を分割
	if(splitString(szTime,':',&szTmp[0],&szTmp[1],&szTmp[2],NULL) != 3) return timet;

	timeptr.tm_mday = atoi(szDay);

	timeptr.tm_hour = atoi(szTmp[0]);
	timeptr.tm_min  = atoi(szTmp[1]);
	timeptr.tm_sec  = atoi(szTmp[2]);

	int nMonth = -1;
	for(int i = 0; i <= 12; i++){
		if(strcmp(strMonthList[i],szMonth) == 0) {
			nMonth = i+1;
			break;
		}
	}
	if(nMonth == -1) {
		return -1;
	};
	timeptr.tm_mon = nMonth;
	timeptr.tm_year = nowTm->tm_year;
	timet = mktime(&timeptr);

	return timet;
}

//---------------------------------------------------
//関数名	awk(char *strSource,int nAwk,char cSplit)
//機能		指定文字区切りで文字列を切り出す
//---------------------------------------------------
char* UStringWork::awk(char *strSource,char *strRet,int nSize,int nAwk,char cSplit /* = 0x20*/)
{
	bool isSpace = false;

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
			if(*strSource == 0x0a || *strSource == 0x0d || *strSource == cSplit ||
				*strSource == 0x1a|| *strSource == NULL) {
				break;
			}
			*strRet = *strSource;
			if(isSJIS(strSource)) {
				strSource++;//ポインタを進める
				strRet++;
				*strRet = *strSource;
			}
			strRet++;
		}
		else if(isSJIS(strSource))	strSource++;//ポインタを進める
		strSource++;//ポインタを進める
	}
	*strRet = (char)NULL;

	return strSource;
}

//---------------------------------------------------
//関数名	serchString(char *strSerch,char *strFound,int nFoundSize)
//機能		ワイルドカード付き文字列検索
//			*なんでもOK ?なんでも1文字 {xxx}括弧内の文字列なら
//---------------------------------------------------
char* UStringWork::serchString(char *strSource,char *strSerch,int *nFoundSize,int nStart)
{
	char *szRet = NULL,*szSerch;

	szRet = NULL;
	*nFoundSize = 0;
	szSerch = strSerch;

	strSource += (char)nStart;
	bool isFirstSame = false;

	while(*strSource && *strSerch && strcmp(strSerch,"*")){
		//検索語が全角
		if(UStringWork::isSJIS(strSerch)){
			if(strncmp(strSource,strSerch,2) == 0) {//一致した
				if(!szRet)	szRet = strSource;//発見位置を記録
				strSource += 2;
				strSerch += 2;
				(*nFoundSize) += 2;
			}
			else {//違った
				if(UStringWork::isSJIS(strSource)) strSource += 2;
				else strSource++;//ソースのポインタを進める
				strSerch = szSerch;//検索後を最初に戻す
				szRet = NULL;
				(*nFoundSize) = 0;
				continue;
			}
		}
		//検索語が半角
		else if(*strSerch == '*' || *strSerch == '?' || *strSerch == '{'){//ワイルドカード
			if(!szRet)	szRet = strSource;//発見位置を記録
			if(!UStringWork::isSJIS(strSerch) && *strSerch == '{') {//文字クラスなので、クラス内で照合
				char *szClass;
				bool isSame = false;

				szClass = strSerch + 1;
				while(*szClass && *szClass != '}') {// ]に当たるか、文字列の最後まで検索
					//文字クラス、ソースのどちらかが全角なら2バイト照合
					if((UStringWork::isSJIS(strSource) || UStringWork::isSJIS(szClass))
						&& strncmp(strSource,szClass,2) == 0)
						isSame = true;
					//両方半角なら1バイト照合
					else if(!UStringWork::isSJIS(strSource) && !UStringWork::isSJIS(szClass) && *strSource == *szClass)	
						isSame = true;

					//文字クラスのポインタを進める
					if(UStringWork::isSJIS(szClass))	szClass += 2;	//全角
					else								szClass ++;		//半角
				}
				//照合失敗
				if(!isSame) {
					//文字クラスの中で、1回も照合成功しなかった
					if(!isFirstSame) {
						szRet = NULL;//ﾊｹｰﾝデータを消去
						*nFoundSize = 0;
						strSerch = szSerch;//検索語を最初に戻す
						if(UStringWork::isSJIS(strSource)) strSource += 2;
						else strSource++;//ソースのポインタを進める
					}
					else strSerch = szClass + 1;
					isFirstSame = false;
					continue;
				}
				isFirstSame = true;//少なくとも1文字は照合成功
				//ソースのポインタを進める
				if(UStringWork::isSJIS(strSource)){
					if(!szRet)	szRet = strSource;//発見位置を記録
					strSource += 2;//ソースのポインタを進める
					if(!*strSource)	strSerch = szClass + 1;//ソースが終わりなら検索語も進める
					(*nFoundSize) += 2;
				}
				else {
					if(!szRet)	szRet = strSource;//発見位置を記録
					strSource++;//ソースのポインタを進める
					(*nFoundSize)++;
				}
				continue;
			}
			//?の場合
			if(!UStringWork::isSJIS(strSerch) && *strSerch == '?') {
				strSerch++;//検索語を1文字進める
				if(UStringWork::isSJIS(strSource)) {//対象を1文字進める
					(*nFoundSize) += 2;
					strSource += 2;
				}
				else {
					(*nFoundSize)++;
					strSource ++;
				}
			}
			//*で、ソースが半角で*範囲が終わりの場合は検索語を進める
			else if(strlen(strSerch) > 1 && !UStringWork::isSJIS(strSerch) && *strSerch == '*' && !UStringWork::isSJIS(strSource) && *(strSerch+1) == *strSource) {
				strSerch++;
			}
			//*で、ソースが全角で*範囲が終わりの場合は検索語を進める
			else if(strlen(strSerch) > 1 && !UStringWork::isSJIS(strSerch) && *strSerch == '*' && UStringWork::isSJIS(strSource) && strncmp(strSerch+1,strSource,2) == 0) {
				strSerch++;
			}
			else if(UStringWork::isSJIS(strSource)) {
					strSource += 2;
					if(szRet)	(*nFoundSize) += 2;
			}
			else {
					strSource++;//ソースのポインタを進める
					if(szRet)	(*nFoundSize)++;
			}
		}
		else if(*strSource == *strSerch && !UStringWork::isSJIS(strSource)){//半角文字列で一致
			if(!szRet)	szRet = strSource;//発見位置を記録
			strSource++;
			strSerch++;
			(*nFoundSize)++;
		}
		else {//全然違う
			if(UStringWork::isSJIS(strSource)) strSource += 2;
			else strSource++;//ソースのポインタを進める
			strSerch = szSerch;//検索語を最初に戻す
			szRet = NULL;		//ﾊｹｰﾝ情報をクリア
			*nFoundSize = 0;
			continue;
		}
	}
	if(strcmp(strSerch,"*") == 0)	*nFoundSize = strlen(szRet);//最後が*だった場合は文字数を再カウント
	else if(strlen(strSerch) > 0)	szRet = NULL;//検索語が終りきってない場合は不一致

	return szRet;
}

//---------------------------------------------------
//関数名	isSJIS(char *szSource)
//機能		S-JISかどうかを判別
//---------------------------------------------------
bool UStringWork::isSJIS(char *szSource)
{
	bool isRet = false;
	
	if(((unsigned char)*szSource >= 0x81 && (unsigned char)*szSource <= 0x9f) ||
		((unsigned char)*szSource >= 0xe0 && (unsigned char)*szSource <= 0xef)) 
		if(((unsigned char)*(szSource + 1) >= 0x40 && (unsigned char)*(szSource + 1) <= 0x7e) ||
			((unsigned char)*szSource >= 0x80 && (unsigned char)*szSource <= 0xfc)) 
			isRet = true;

	return isRet;
}

//---------------------------------------------------
//関数名	convertKansuuji(char *strSource,char *strDst,int nSize)
//機能		漢数字を変換
//---------------------------------------------------
void UStringWork::convertKansuuji(char *strSource,char *strDst,int nSize)
{
	memset(strDst,0x00,nSize);

	static vector<KANSUUJI> kanjiData;
	KANSUUJI kanji;

	if(kanjiData.size() < 1) {
		kanjiData.reserve(20);
		//変換用データを設定
		strcpy(kanji.strKANJI,"一");kanji.nNumber = 1;			kanjiData[0] = kanji;
		strcpy(kanji.strKANJI,"壱");							kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"二");kanji.nNumber = 2;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"弐");							kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"三");kanji.nNumber = 3;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"参");							kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"四");kanji.nNumber = 4;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"五");kanji.nNumber = 5;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"六");kanji.nNumber = 6;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"七");kanji.nNumber = 7;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"八");kanji.nNumber = 8;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"九");kanji.nNumber = 9;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"十");kanji.nNumber = 10;			kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"拾");							kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"百");kanji.nNumber = 100;		kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"千");kanji.nNumber = 1000;		kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"万");kanji.nNumber = 10000;		kanjiData.push_back(kanji);
		strcpy(kanji.strKANJI,"億");kanji.nNumber = 100000000;	kanjiData.push_back(kanji);
	}
	int nNumber = 0;
	int nTmpNum = 0;
	bool isNumber = false,isNumberMode = false;

	char *sz;
	sz = strDst;

	while(*strSource) {
		vector<KANSUUJI>::iterator it;
		isNumber = false;
		for( it = kanjiData.begin(); it != kanjiData.end(); it++) {
			if(strncmp(strSource,it->strKANJI,2) == 0) {//漢数字を探す
				isNumber = true;
				strSource += 2;
				break;
			}
		}
		if(isNumber) {
			if(it->nNumber > 9) {//10以上の数字の場合は桁処理をする
				if(nTmpNum == 0) nTmpNum = 1;
				nNumber += nTmpNum * it->nNumber;//掛ける
				nTmpNum = 0;
			}
			else				nTmpNum = nTmpNum + it->nNumber;//10未満の場合は足すだけ
			isNumberMode = true;
		}
		else if(!isNumber && isNumberMode) {//処理中の漢数字が終ったよ
			if(nTmpNum) nNumber += nTmpNum;//数字が10未満の場合の処理
			strDst = UStringWork::addNumberString(nNumber,strDst);
			nNumber = 0;
			nTmpNum = 0;
			isNumberMode = false;
		}
		else {				//漢数字じゃない文字
			*strDst = *strSource;
			strDst++; strSource++;
		}
	}
	if(isNumberMode) {//数字処理中に終ったので、最後の数字を処理
		strDst = UStringWork::addNumberString(nNumber+nTmpNum,strDst);
	}

	*strDst = (char)NULL;
}


//---------------------------------------------------
//関数名	addNumberString(int nNum,char *szDst)
//機能		半角数字を全角にして文字列に追加
//---------------------------------------------------
char* UStringWork::addNumberString(int nNum,char *szDst)
{
	char strBuff[32],strBuffZen[64],*szZenPos;
	const char szZenSuu[] = "０１２３４５６７８９";
	
	itoa(nNum,strBuff,10);

	//全角数字にする
	memset(strBuffZen,0x00,sizeof(strBuffZen));
	szZenPos = strBuffZen;
	for(int i = 0; i < strlen(strBuff); i++) {
		strncpy(szZenPos,&szZenSuu[((strBuff[i] - '0') * 2)],2);
		szZenPos += 2;
	}

	//文字列を連結
	strcat(szDst,strBuffZen);
	szDst += strlen(strBuffZen);

	return szDst;
}

//---------------------------------------------------
//関数名	GetIPv4String(char* overSize16Buffer, int nIPv4)
//機能		ユーザ定義のバッファ(16バイト以上必要)に
//			xxx.xxx.xxx.xxx形式の文字列を入れる。
//			nIPv4 は !(32以下 0以上) の数字
//---------------------------------------------------
char* UStringWork::GetIPv4String(char* overSize16Buffer, int nIPv4)
{
	BYTE* b = (BYTE*)&nIPv4;
	_snprintf(overSize16Buffer, 16, "%d.%d.%d.%d", b[3], b[2], b[1], b[0]);
	return overSize16Buffer;
}


//---------------------------------------------------
//関数名	GetIPv4Mask(char* overSize16Buffer, int under32)
//機能		ユーザ定義のバッファ(16バイト以上必要)に 
//			xxx.xxx.xxx.xxx形式の文字列を入れる。
//			under32 は 32以下 0以上 の数字
//---------------------------------------------------
char* UStringWork::GetIPv4MaskString(char* overSize16Buffer, int under32)
{
	DWORD mask = -1;
	if(under32)
		mask <<= 32-under32;
	else
		mask = 0;
	BYTE* b = (BYTE*)&mask;
	_snprintf(overSize16Buffer, 16, "%d.%d.%d.%d", b[3], b[2], b[1], b[0]);
	return overSize16Buffer;
}

//---------------------------------------------------
//関数名	URLEncode(const char *pszSrc,int nSrcLength,char *pszBuf)
//機能		URLエンコードをする
//---------------------------------------------------
int UStringWork::URLEncode(const char *pszSrc,int nSrcLength,char *pszBuf)
{
	int n=0,nOut=0;
	char cData;
	unsigned char byData;
	char cHex[]={
		'0','1','2','3',
		'4','5','6','7',
		'8','9','A','B',
		'C','D','E','F'
	};

	while(pszSrc[n]!='\0' && ( nSrcLength<0 ? !0 : nSrcLength>n )) {
		cData=pszSrc[n];

		if((cData>='0' && cData<='9') ||
			(cData>='A' && cData<='Z') ||
			(cData>='a' && cData<='z') ||
			cData=='_' || cData=='-' || cData=='.' || cData=='*'){
			// そのまま垂れ流しにできる文字の場合
			nOut++;
			if(pszBuf) {
				*pszBuf=cData;
				pszBuf++;
			}
		}
		else {
			if(cData==' ') {
				// 空白文字は'+'に置換する規則
				// '+'は、%XXなどと変換されるので重複はしない
				if(pszBuf) {
					*pszBuf='+';
					pszBuf++;
				}
				nOut++;
			}
			else {
				// %XX形式への変換
				if(pszBuf) {
					*pszBuf='%';pszBuf++;
					byData=(unsigned char)cData;
					*pszBuf=cHex[byData / 0x10];pszBuf++;
					*pszBuf=cHex[byData % 0x10];pszBuf++;
				}
				nOut+=3;
			}
		}
		n++;
	}

	if(pszBuf) {
		*pszBuf='\0';
	}
	nOut++;

	return nOut;
}

//---------------------------------------------------
//関数名	HexToDec(char c)
//機能		16進数変換
//---------------------------------------------------
int UStringWork::HexToDec(char c)
{
	if(c>='0' && c<='9')	return c-'0';
	if(c>='A' && c<='F')	return c-'A'+10;
	if(c>='a' && c<='f')	return c-'a'+10;

	return 0;
}

//---------------------------------------------------
//関数名	URLDecode(const char *pszSrc,char *pszBuf)
//機能		URLデコードをする
//---------------------------------------------------
int UStringWork::URLDecode(const char *pszSrc,char *pszBuf)
{
	int n=0;
	int nOut=0;
	char cData;
	unsigned char byData;
	
	while(pszSrc[n]!='\0') {
		cData=pszSrc[n];
		if(cData=='%') {
			// %12
			n++;byData=(HexToDec(pszSrc[n]))*0x10;	// 1
			n++;byData+=(HexToDec(pszSrc[n]));		// 2

			if(pszBuf) {
				*pszBuf=byData;
				pszBuf++;
			}
			nOut++;
		}
		else {
			if(cData=='+')
				cData=' ';

			byData=cData;
			
			if(pszBuf) {
				*pszBuf=byData;
				pszBuf++;
			}
			nOut++;
		}
		
		n++;
	}

	if(pszBuf) {
		*pszBuf='\0';
	}
	nOut++;

	return nOut;
}
