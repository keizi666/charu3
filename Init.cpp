/*----------------------------------------------------------
	設定管理クラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "Init.h"
#include "StringWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------
//関数名	CInit
//機能		コンストラクタ
//---------------------------------------------------
CInit::CInit()
{
	m_nSelectID = 0;		//選択してるID
	m_nOptionPage = 0;
	m_DialogSize.x = 100;	//ダイアログのサイズ
	m_DialogSize.y = 100;	//ダイアログのサイズ

	m_nSerchKind = 0;		//検索種別
	m_nSerchLogic = 0;		//検索方法

	m_strIniFile = "";
	m_nOptionPage = 0;
	m_nRecNumber = 0;

	m_IconSize.x = ICON_SIZE;
	m_IconSize.y = ICON_SIZE;

	m_hHookDLL = NULL;
}

//---------------------------------------------------
//関数名	~CInit
//機能		デストラクタ
//---------------------------------------------------
CInit::~CInit()
{
	if(m_hHookDLL) unHook();
}

//---------------------------------------------------
//関数名	initialize()
//機能		初期化
//---------------------------------------------------
void CInit::initialize()
{
	CString StringBuff,StringBuff2;
	TCHAR strBuff[MAX_PATH];
	TCHAR *cpName;

	//ユーザー名を取得
	DWORD dwSize = _countof(strBuff);
	m_strUserName = _T("Charu3User");
	if(::GetUserName(strBuff,&dwSize)) {
		m_strUserName = strBuff;
	}

	//実行パスを取得、作成
	GetModuleFileName(NULL,strBuff,_countof(strBuff));
	cpName = _tcsrchr(strBuff,_T('\\'));
	cpName++;
	*cpName = NULL;
	m_strAppPath = strBuff;
}

//---------------------------------------------------
//関数名	readAllInitData()
//機能		読み込み
//---------------------------------------------------
void CInit::readAllInitData()
{
	if(m_strIniFile == "") return;

	int i;
	int nKetsetCount;
	CString strKeyBuff,StringBuff;
	CString strRes;
	CHANGE_KEY key;
	TCHAR strBuff[1024],*szName,*szKind,*szMacro;

	//一般設定	
	m_etc.m_nMulchUser    = setIniFileInt(REGKEY_ETC,_T("MulchUser"),1);//マルチユーザーで使うかどうか

	m_etc.m_nToClip    = setIniFileInt(REGKEY_ETC,_T("ToClipBord"),1);//リストから選んだ文字列はクリップボードにも入れる
	m_etc.m_nToolTip   = setIniFileInt(REGKEY_ETC,_T("FixToolTip"),0);//タスクトレイアイコンのツールチップを固定にする
	m_etc.m_nIconClick = setIniFileInt(REGKEY_ETC,_T("IconClickAct"),0);//アイコンをクリックしたときの動作
	m_etc.m_nWordBugSW = setIniFileInt(REGKEY_ETC,_T("WordBug"),0);	//Wordのバグに対応する
	m_etc.m_nSelfDiagnosisTime = setIniFileInt(REGKEY_ETC,_T("SelfDiagnosisTime"),10000);	//自己診断タイマー
	m_etc.m_nActiveTime = setIniFileInt(REGKEY_ETC,_T("ActiveTime"),400);	//監視タイマー
	m_etc.m_nMinimization = setIniFileInt(REGKEY_ETC,_T("Minimization"),1);	//最小化する
	m_etc.m_nDontSave = setIniFileInt(REGKEY_ETC,_T("DontSave"),0);
	m_etc.m_nCopyWaitCnt = setIniFileInt(REGKEY_ETC,_T("CopyWaitCnt"),5);
	m_etc.m_nPasteWaitCnt = setIniFileInt(REGKEY_ETC,_T("PasteWaitCnt"),10);

	

	//ポップアップ設定	
	m_pop.m_nSelectSW  = setIniFileInt(REGKEY_POPUP,_T("HoldSelect"),1);//テキストリストの選択位置を保持する
	m_pop.m_nFolderSW  = setIniFileInt(REGKEY_POPUP,_T("HoldFolder"),1);//テキストリストのフォルダ状態を保持する
	m_pop.m_nCornerPopup = setIniFileInt(REGKEY_POPUP,_T("CornerPopup"),0);//マウスカーソルが画面の端に触れたらポップアップする
	m_pop.m_nCornerPopupTime = setIniFileInt(REGKEY_POPUP,_T("CornerPopupTime"),400);
	m_pop.m_nCornerPopupPix  = setIniFileInt(REGKEY_POPUP,_T("CornerPopupPix"),2);

	m_pop.m_nDoubleKeyPOP = setIniFileInt(REGKEY_POPUP,_T("PopupKeyDC"),0);
	m_pop.m_uVK_Pouup   = setIniFileInt(REGKEY_POPUP,_T("PopupKey"),88);		//ポップアップキー
	m_pop.m_uMod_Pouup  = setIniFileInt(REGKEY_POPUP,_T("PopupKeyM"),MOD_ALT);	//ポップアップ特殊キー
	m_pop.m_nDCKeyPopTime = setIniFileInt(REGKEY_POPUP,_T("DCPopupKeyTime"),250);
	
	m_pop.m_nDoubleKeyFIFO = setIniFileInt(REGKEY_POPUP,_T("FifoKeyDC"),0);
	m_pop.m_uVK_Fifo  = setIniFileInt(REGKEY_POPUP,_T("FifoKey"),29);			//履歴FIFO切り替えキー
	m_pop.m_uMod_Fifo = setIniFileInt(REGKEY_POPUP,_T("FifoKeyM"),MOD_CONTROL);	//履歴FIFO切り替え特殊キー
	m_pop.m_nDCKeyFifoTime = setIniFileInt(REGKEY_POPUP,_T("DCFifoKeyTime"),250);

	m_pop.m_nPopupPos     = setIniFileInt(REGKEY_POPUP,_T("PopupPos"),0);	//ポップアップ出現位置
	m_pop.m_posCaretHosei.x = setIniFileInt(REGKEY_POPUP,_T("CaretHoseiX"),-20);	//キャレット位置の補正値
	m_pop.m_posCaretHosei.y = setIniFileInt(REGKEY_POPUP,_T("CaretHoseiY"),0);	//キャレット位置の補正値
	m_pop.m_nOutRevice      = setIniFileInt(REGKEY_POPUP,_T("ReviseOutOfDesktop"),1);

	m_pop.m_nQuickEnter = setIniFileInt(REGKEY_POPUP,_T("QuickEnter"),0);	//クイックアクセス終了時の貼り付け
	m_pop.m_nQuickTime  = setIniFileInt(REGKEY_POPUP,_T("QuickTime"),400);		//クイックアクセス確定時間
	m_pop.m_nQuickChar  = setIniFileInt(REGKEY_POPUP,_T("QuickChar"),0);
	m_pop.m_nAutoImeOff  = setIniFileInt(REGKEY_POPUP,_T("AutoImeOff"),0);
	m_pop.m_nSingleExpand = setIniFileInt(REGKEY_POPUP,_T("SingleExpand"),0);

	//履歴FIFO設定	
	m_fifo.m_nFifo = setIniFileInt(REGKEY_FIFO,_T("FifoSW"),1);			//履歴動作中はCtrl+Vで貼り付ける文字列を先入れ先出しにする 0NON 1FIFO 2RIFO
	m_fifo.m_nCopySound = setIniFileInt(REGKEY_FIFO,_T("CopySound"),1);	//履歴記録中はコピー時に効果音を鳴らす
	m_fifo.m_nPasteSound = setIniFileInt(REGKEY_FIFO,_T("PasteSound"),0);	//貼り付け時にも音を鳴らすスイッチ
	m_fifo.m_strWaveFile = setIniFileString(REGKEY_FIFO,_T("WaveName"),_T("pu.wav"));//効果音のファイル名
	m_fifo.m_nOffClear = setIniFileInt(REGKEY_FIFO,_T("OffClear"),0);		//履歴OFF時に一時項目をクリア
	m_fifo.m_nNoClear = setIniFileInt(REGKEY_FIFO,_T("NoClear"),1);		//履歴モードで最後に空にしないスイッチ
	m_fifo.m_nAllClearOff = setIniFileInt(REGKEY_FIFO,_T("AllClearOff"),1);//一時履歴が空になったら履歴モードをOFFにする
	m_fifo.m_nDuplication = setIniFileInt(REGKEY_FIFO,_T("Duplication"),1);

	//ビジュアル設定
	m_visual.m_nTransparentSW = setIniFileInt(REGKEY_VISUAL,_T("TransparentSW"),0);	//透明機能を使うかスイッチ
	m_visual.m_nSemitransparent = setIniFileInt(REGKEY_VISUAL,_T("Semitransparent"),50);	//透明度
	strRes.LoadString(APP_INF_DEF_FONT);
	m_visual.m_strFontName = setIniFileString(REGKEY_VISUAL,_T("FontName"),strRes);//フォント名
	m_visual.m_nFontSize = setIniFileInt(REGKEY_VISUAL,_T("FontSize"),100);					//フォントサイズ
	strRes.LoadString(APP_INF_DEF_ICON);
	m_visual.m_strResourceName = setIniFileString(REGKEY_VISUAL,_T("ResourceName"),strRes);//リソース名
	m_visual.m_nBorderColor = setIniFileInt(REGKEY_VISUAL,_T("BorderColor"),0xff9900);	//枠色
	m_visual.m_nBackColor = setIniFileInt(REGKEY_VISUAL,_T("BackColor"),0xffffe0);		//背景色
	m_visual.m_nTextColor = setIniFileInt(REGKEY_VISUAL,_T("TextColor"),0x663300);		//テキスト色
	m_visual.m_nResetTree = setIniFileInt(REGKEY_VISUAL,_T("ResetTree"),1);	//ツリーの自動再構築
	m_visual.m_nScrollbar = setIniFileInt(REGKEY_VISUAL,_T("Scrollbar"),SB_BOTH);
	m_visual.m_nToolTip = setIniFileInt(REGKEY_VISUAL,_T("ToolTip"),0);

	//仮想キー設定
	WINDOWS_MESSAGE winMessage;
	m_key.m_defKeySet.m_nMessage = setIniFileInt(REGKEY_KEY,_T("isMessage"),0);		//メッセージ方式フラグ
	m_key.m_defKeySet.m_uMod_Paste = setIniFileInt(REGKEY_KEY,_T("ModPaste"),MOD_CONTROL);	//貼り付け特殊キー
	m_key.m_defKeySet.m_uVK_Paste  = setIniFileInt(REGKEY_KEY,_T("VKPaste"),'V');			//貼り付けキー
	m_key.m_defKeySet.m_uMod_Copy  = setIniFileInt(REGKEY_KEY,_T("ModCopy"),MOD_CONTROL);	//コピー特殊キー
	m_key.m_defKeySet.m_uVK_Copy   = setIniFileInt(REGKEY_KEY,_T("VkCopy"),'C');			//コピーキー
	m_key.m_defKeySet.m_nCopyWait  = setIniFileInt(REGKEY_KEY,_T("CopyWait"),50);		//コピー待ち時間
	m_key.m_defKeySet.m_nPasteWait = setIniFileInt(REGKEY_KEY,_T("PasteWait"),50);		//ペースト待ち時間
	//メッセージ
	strKeyBuff = setIniFileString(REGKEY_KEY,_T("CopyMessage"),_T("301,0,0"));
	_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
	m_key.m_defKeySet.m_copyMessage = winMessage;

	strKeyBuff = setIniFileString(REGKEY_KEY,_T("PasteMessage"),_T("302,0,0"));
	_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
	m_key.m_defKeySet.m_pasteMessage = winMessage;
	//クリップボード容量制限
	m_key.m_nHistoryLimit = setIniFileInt(REGKEY_KEY,_T("HistoryLimit"),-1);	

	//固有キー設定を読む
	nKetsetCount = setIniFileInt(REGKEY_KEY,_T("KeySetCount"),0);
	m_key.m_KeyList.clear();
	for(i = 0; i < nKetsetCount; i++){
		StringBuff.Format(_T("KeySetTitle_%d"),i);
		//ウィンドウキャプション
		key.m_strTitle = setIniFileString(REGKEY_KEY,StringBuff,_T(""));
		//マッチ方法
		StringBuff.Format(_T("KeySetMatch_%d"),i);
		key.m_nMatch = setIniFileInt(REGKEY_KEY,StringBuff,0);
		//キーコード
		StringBuff.Format(_T("KeySetCode_%d"),i);
		strKeyBuff = setIniFileString(REGKEY_KEY,StringBuff,"");
		_stscanf_s(LPCTSTR(strKeyBuff),_T("%d,%d,%d,%d")
			,&key.m_sCopyPasteKey.m_uMod_Copy,&key.m_sCopyPasteKey.m_uVK_Copy
			,&key.m_sCopyPasteKey.m_uMod_Paste,&key.m_sCopyPasteKey.m_uVK_Paste);
		StringBuff.Format(_T("KeyCopyWait_%d"),i);
		key.m_sCopyPasteKey.m_nCopyWait  = setIniFileInt(REGKEY_KEY,StringBuff,100);		//コピー待ち時間
		StringBuff.Format(_T("KeyPasteWait_%d"),i);
		key.m_sCopyPasteKey.m_nPasteWait = setIniFileInt(REGKEY_KEY,StringBuff,100);		//ペースト待ち時間

		StringBuff.Format(_T("KeyMessage_%d"),i);
		key.m_sCopyPasteKey.m_nMessage = setIniFileInt(REGKEY_KEY,StringBuff,0);		//方式

		//メッセージ
		StringBuff.Format(_T("CopyMessage_%d"),i);
		strKeyBuff = setIniFileString(REGKEY_KEY,StringBuff,_T("301,0,0"));
		_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
		key.m_sCopyPasteKey.m_copyMessage = winMessage;
	
		StringBuff.Format(_T("PasteMessage_%d"),i);
		strKeyBuff = setIniFileString(REGKEY_KEY,StringBuff,_T("302,0,0"));
		_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
		key.m_sCopyPasteKey.m_pasteMessage = winMessage;
		//クリップボード容量制限
		StringBuff.Format(_T("HistoryLimit_%d"),i);
		key.m_nHistoryLimit = setIniFileInt(REGKEY_KEY,StringBuff,-1);	
		
		m_key.m_KeyList.insert(m_key.m_KeyList.end(),key);//設定に追加
	}	

	//一般環境設定
	m_strDataFile = setIniFileString(REGKEY_ENV,_T("DataFile"),m_strAppPath + m_strUserName + _T("\\") + DAT_FILE);

	m_strRwPluginFolder = setIniFileString(REGKEY_ENV,_T("RwPluginFolder"),m_strAppPath + _T("RW_Plugin"));
	m_strPluginName = setIniFileString(REGKEY_ENV,_T("DataFormat"),DAT_FORMAT);
	m_strMacroPluginName = setIniFileString(REGKEY_ENV,_T("MacroPlugin"),DAT_FORMAT);
	m_nSelectID     = setIniFileInt(REGKEY_ENV,_T("SelectID"),-1);
	time_t lTime;
	m_nTreeID       = setIniFileInt(REGKEY_ENV,_T("TreeID"),time(&lTime));
	m_nRecNumber    = setIniFileInt(REGKEY_ENV,_T("RecNumber"),0);
	m_strBBS        = setIniFileString(REGKEY_ENV,_T("SupportBBS"),_T("http://8537.teacup.com/keiziweb/bbs"));
	m_nToolTipTime  = setIniFileInt(REGKEY_ENV,_T("ToolTipTime"),30000);
	m_nToolTipDelay = setIniFileInt(REGKEY_ENV,_T("ToolTipDelay"),300);
	m_nDebug		= setIniFileInt(REGKEY_ENV,_T("Debug"),0);
	m_strDebugLog	= setIniFileString(REGKEY_ENV,_T("LogfileName"),_T("charu3.log"));


	//ウィンドウ設定
	m_DialogSize.x = setIniFileInt(REGKEY_WINDOW,_T("PopupSizeX"),250);
	m_DialogSize.y = setIniFileInt(REGKEY_WINDOW,_T("PopupSizeY"),350);

	//検索設定
	m_nSerchKind  = setIniFileInt(REGKEY_SERCH,_T("SerchKind"),0);		//検索種別
	m_nSerchLogic = setIniFileInt(REGKEY_SERCH,_T("SerchLogic"),0);		//検索方法
	m_strSerchKey  = setIniFileString(REGKEY_SERCH,_T("SerchKey"),_T(""));	//検索キー

	//マクロテンプレート
	for(i = 0; i <= 99; i++) {
		strRes.LoadString(APP_INF_MACRO_TEMPLATE01+i);
		if(strRes == _T("end")) break;
	}
	int nStringCnt = i;
	MACRO_STRUCT macroData;
	CString strMacro;
	m_vctMacro.clear();
	m_vctMacro.reserve(99);
	for(i = 1; i <= 99; i++) {
		StringBuff.Format(_T("Macro_%02d"),i);
		::GetPrivateProfileString(REGKEY_MACRO,StringBuff,_T(""),strBuff,_countof(strBuff),m_strIniFile);
		if (_tcsclen(strBuff) == 0 && i <= nStringCnt) {
			strRes.LoadString(APP_INF_MACRO_TEMPLATE01 + i -1);
			writeProfileString(REGKEY_MACRO,StringBuff,strRes);
			strMacro = strRes;
			_tcscpy_s(strBuff,LPCTSTR(strRes));
		}
		if(_tcsclen(strBuff) == 0) break;
		if(UStringWork::splitString(strBuff,_T('@'),&szName,&szKind,&szMacro,NULL) == 3) {
			macroData.m_strName = szName;
			if(*szKind == _T('D'))		macroData.m_cKind = KIND_DATA_ALL;
			else if(*szKind == _T('F'))	macroData.m_cKind = KIND_FOLDER_ALL;
			else					macroData.m_cKind = 0xff;
			macroData.m_strMacro = szMacro;
			m_vctMacro.push_back(macroData);
		}
	}
	
	//拡張マクロテンプレート R履歴フォルダ Fフォルダ A全て Dデータ
	for(i = 0; i <= 99; i++) {
		strRes.LoadString(APP_INF_EXMACRO_TEMPLATE01+i);
		if(strRes == "end") break;
	}
	nStringCnt = i;
	m_vctDataMacro.clear();
	m_vctDataMacro.reserve(99);
	for(i = 1; i <= 99; i++) {
		StringBuff.Format(_T("DataMacro_%02d"),i);
		::GetPrivateProfileString(REGKEY_MACRO,StringBuff,_T(""),strBuff,_countof(strBuff),m_strIniFile);
		if (_tcsclen(strBuff) == 0 && i <= nStringCnt) {
			strRes.LoadString(APP_INF_EXMACRO_TEMPLATE01 + i -1);
			writeProfileString(REGKEY_MACRO,StringBuff,strRes);
			strMacro = strRes;
			_tcscpy_s(strBuff,LPCTSTR(strRes));
		}
		if(_tcsclen(strBuff) == 0) break;
		if(UStringWork::splitString(strBuff,'@',&szName,&szKind,&szMacro,NULL) == 3) {
			macroData.m_strName = szName;
			if(*szKind == 'D')		macroData.m_cKind = KIND_DATA_ALL;
			else if(*szKind == 'R')	macroData.m_cKind = KIND_RIREKI;
			else if(*szKind == 'F')	macroData.m_cKind = KIND_FOLDER_ALL;
			else					macroData.m_cKind = 0xff;
			macroData.m_strMacro = szMacro;
			m_vctDataMacro.push_back(macroData);
		}
	}
}

//---------------------------------------------------
//関数名	writeAllInitData()
//機能		書き込み
//---------------------------------------------------
void CInit::writeAllInitData()
{
	if(m_strIniFile == "") return;

	//一般設定	
	writeProfileInt(REGKEY_ETC,_T("ToClipBord"),m_etc.m_nToClip);		//リストから選んだ文字列はクリップボードにも入れる
	writeProfileInt(REGKEY_ETC,_T("FixToolTip"),m_etc.m_nToolTip);		//タスクトレイアイコンのツールチップを固定にする
	writeProfileInt(REGKEY_ETC,_T("IconClickAct"),m_etc.m_nIconClick);	//アイコンをクリックしたときの動作
	writeProfileInt(REGKEY_ETC,_T("WordBug"),m_etc.m_nWordBugSW);		//Wordのバグに対応する
	writeProfileInt(REGKEY_ETC,_T("SelfDiagnosisTime"),m_etc.m_nSelfDiagnosisTime);	//自己診断タイマー
	writeProfileInt(REGKEY_ETC,_T("ActiveTime"),m_etc.m_nActiveTime);	//監視タイマー
	writeProfileInt(REGKEY_ETC,_T("Minimization"),m_etc.m_nMinimization);	//最小化する
	writeProfileInt(REGKEY_ETC,_T("DontSave"),m_etc.m_nDontSave);
	
	//ポップアップ設定	
	writeProfileInt(REGKEY_POPUP,_T("HoldSelect"),m_pop.m_nSelectSW);	//テキストリストの選択位置を保持する
	writeProfileInt(REGKEY_POPUP,_T("HoldFolder"),m_pop.m_nFolderSW);	//テキストリストのフォルダ状態を保持する
	writeProfileInt(REGKEY_POPUP,_T("CornerPopup"),m_pop.m_nCornerPopup);	//マウスカーソルが画面の端に触れたらポップアップする

	writeProfileInt(REGKEY_POPUP,_T("PopupKeyDC"),m_pop.m_nDoubleKeyPOP);
	writeProfileInt(REGKEY_POPUP,_T("PopupKey"),m_pop.m_uVK_Pouup);			//ポップアップキー
	writeProfileInt(REGKEY_POPUP,_T("PopupKeyM"),m_pop.m_uMod_Pouup);		//ポップアップ特殊キー
	writeProfileInt(REGKEY_POPUP,_T("FifoKeyDC"),m_pop.m_nDoubleKeyFIFO);
	writeProfileInt(REGKEY_POPUP,_T("FifoKey"),m_pop.m_uVK_Fifo);			//履歴FIFO切り替えキー
	writeProfileInt(REGKEY_POPUP,_T("FifoKeyM"),m_pop.m_uMod_Fifo);		//履歴FIFO切り替え特殊キー

	writeProfileInt(REGKEY_POPUP,_T("PopupPos"),m_pop.m_nPopupPos);	//ポップアップ出現位置
	writeProfileInt(REGKEY_POPUP,_T("CaretHoseiX"),m_pop.m_posCaretHosei.x);	//キャレット位置の補正値
	writeProfileInt(REGKEY_POPUP,_T("CaretHoseiY"),m_pop.m_posCaretHosei.y);	//キャレット位置の補正値
	writeProfileInt(REGKEY_POPUP,_T("ReviseOutOfDesktop"),m_pop.m_nOutRevice);

	writeProfileInt(REGKEY_POPUP,_T("QuickEnter"),m_pop.m_nQuickEnter);//クイックアクセス終了時の貼り付け
	writeProfileInt(REGKEY_POPUP,_T("QuickTime"),m_pop.m_nQuickTime);		//クイックアクセス確定時間
	writeProfileInt(REGKEY_POPUP,_T("QuickChar"),m_pop.m_nQuickChar);
	writeProfileInt(REGKEY_POPUP,_T("AutoImeOff"),m_pop.m_nAutoImeOff);
	writeProfileInt(REGKEY_POPUP,_T("SingleExpand"),m_pop.m_nSingleExpand);

	//履歴FIFO設定	
	writeProfileInt(REGKEY_FIFO,_T("FifoSW"),m_fifo.m_nFifo);			//履歴動作中はCtrl+Vで貼り付ける文字列を先入れ先出しにする 0NON 1FIFO 2RIFO
	writeProfileInt(REGKEY_FIFO,_T("CopySound"),m_fifo.m_nCopySound);	//履歴記録中はコピー時に効果音を鳴らす
	writeProfileInt(REGKEY_FIFO,_T("PasteSound"),m_fifo.m_nPasteSound);//貼り付け時にも音を鳴らすスイッチ
	writeProfileString(REGKEY_FIFO,_T("WaveName"),m_fifo.m_strWaveFile);	//効果音のファイル名
	writeProfileInt(REGKEY_FIFO,_T("OffClear"),m_fifo.m_nOffClear);	//履歴OFF時に一時項目をクリア
	writeProfileInt(REGKEY_FIFO,_T("NoClear"),m_fifo.m_nNoClear);		//履歴モードで最後に空にしないスイッチ
	writeProfileInt(REGKEY_FIFO,_T("AllClearOff"),m_fifo.m_nAllClearOff);//一時履歴が空になったら履歴モードをOFFにする
	writeProfileInt(REGKEY_FIFO,_T("Duplication"),m_fifo.m_nDuplication);

	//ビジュアル設定
	writeProfileInt(REGKEY_VISUAL,_T("TransparentSW"),m_visual.m_nTransparentSW);	//透明機能を使うかスイッチ
	writeProfileInt(REGKEY_VISUAL,_T("Semitransparent"),m_visual.m_nSemitransparent);//透明度
	writeProfileString(REGKEY_VISUAL,_T("FontName"),m_visual.m_strFontName);		//フォント名
	writeProfileInt(REGKEY_VISUAL,_T("FontSize"),m_visual.m_nFontSize);				//フォントサイズ
	writeProfileString(REGKEY_VISUAL,_T("ResourceName"),m_visual.m_strResourceName);//リソース名
	writeProfileInt(REGKEY_VISUAL,_T("BorderColor"),m_visual.m_nBorderColor);		//枠色
	writeProfileInt(REGKEY_VISUAL,_T("BackColor"),m_visual.m_nBackColor);			//背景色
	writeProfileInt(REGKEY_VISUAL,_T("TextColor"),m_visual.m_nTextColor);			//テキスト色
	writeProfileInt(REGKEY_VISUAL,_T("ResetTree"),m_visual.m_nResetTree);	//ツリーの自動再構築
	writeProfileInt(REGKEY_VISUAL,_T("Scrollbar"),m_visual.m_nScrollbar);
	writeProfileInt(REGKEY_VISUAL,_T("ToolTip"),m_visual.m_nToolTip);

	//仮想キー設定
	CString strBuff,strKeyBuff;
	writeProfileInt(REGKEY_KEY,_T("isMessage"),m_key.m_defKeySet.m_nMessage);	//メッセージ方式フラグ
	writeProfileInt(REGKEY_KEY,_T("ModPaste"),m_key.m_defKeySet.m_uMod_Paste);		//貼り付け特殊キー
	writeProfileInt(REGKEY_KEY,_T("VKPaste"),m_key.m_defKeySet.m_uVK_Paste);		//貼り付けキー
	writeProfileInt(REGKEY_KEY,_T("ModCopy"),m_key.m_defKeySet.m_uMod_Copy);		//コピー特殊キー
	writeProfileInt(REGKEY_KEY,_T("VkCopy"),m_key.m_defKeySet.m_uVK_Copy);			//コピーキー
	writeProfileInt(REGKEY_KEY,_T("CopyWait"),m_key.m_defKeySet.m_nCopyWait);		//コピー待ち時間
	writeProfileInt(REGKEY_KEY,_T("PasteWait"),m_key.m_defKeySet.m_nPasteWait);			//ペースト待ち時間
	//メッセージ
	strKeyBuff.Format(_T("%x,%x,%x"),m_key.m_defKeySet.m_copyMessage.Msg,
		m_key.m_defKeySet.m_copyMessage.wParam,
		m_key.m_defKeySet.m_copyMessage.lParam);
	writeProfileString(REGKEY_KEY,_T("CopyMessage"),strKeyBuff);

	strKeyBuff.Format(_T("%x,%x,%x"),m_key.m_defKeySet.m_pasteMessage.Msg,
		m_key.m_defKeySet.m_pasteMessage.wParam,
		m_key.m_defKeySet.m_pasteMessage.lParam);
	writeProfileString(REGKEY_KEY,_T("PasteMessage"),strKeyBuff);
	//クリップボード容量制限
	writeProfileInt(REGKEY_KEY,_T("HistoryLimit"),m_key.m_nHistoryLimit);	

	//固有キー設定を書く
	list<CHANGE_KEY>::iterator it;
	int i,nKetsetCount = m_key.m_KeyList.size();
	writeProfileInt(REGKEY_KEY,_T("KeySetCount"),nKetsetCount);
	for(it = m_key.m_KeyList.begin(),i = 0; it != m_key.m_KeyList.end(); it++,i++) {
		//ウィンドウキャプション
		strBuff.Format(_T("KeySetTitle_%d"),i);
		writeProfileString(REGKEY_KEY,strBuff,it->m_strTitle);

		//マッチ方法
		strBuff.Format(_T("KeySetMatch_%d"),i);
		writeProfileInt(REGKEY_KEY,strBuff,it->m_nMatch);

		//キーコード
		strBuff.Format(_T("KeySetCode_%d"),i);
		strKeyBuff.Format(_T("%d,%d,%d,%d")
			,it->m_sCopyPasteKey.m_uMod_Copy,it->m_sCopyPasteKey.m_uVK_Copy
			,it->m_sCopyPasteKey.m_uMod_Paste,it->m_sCopyPasteKey.m_uVK_Paste);
		writeProfileString(REGKEY_KEY,strBuff,strKeyBuff);

		strBuff.Format(_T("KeyCopyWait_%d"),i);
		writeProfileInt(REGKEY_KEY,strBuff,it->m_sCopyPasteKey.m_nCopyWait);		//コピー待ち時間
		strBuff.Format(_T("KeyPasteWait_%d"),i);
		writeProfileInt(REGKEY_KEY,strBuff,it->m_sCopyPasteKey.m_nPasteWait);		//ペースト待ち時間

		strBuff.Format(_T("KeyMessage_%d"),i);
		writeProfileInt(REGKEY_KEY,strBuff,it->m_sCopyPasteKey.m_nMessage);		//方式

		//メッセージ
		strBuff.Format(_T("CopyMessage_%d"),i);
		strKeyBuff.Format(_T("%x,%x,%x"),it->m_sCopyPasteKey.m_copyMessage.Msg,
			it->m_sCopyPasteKey.m_copyMessage.wParam,
			it->m_sCopyPasteKey.m_copyMessage.lParam);
		writeProfileString(REGKEY_KEY,strBuff,strKeyBuff);
	
		strBuff.Format(_T("PasteMessage_%d"),i);
		strKeyBuff.Format(_T("%x,%x,%x"),it->m_sCopyPasteKey.m_pasteMessage.Msg,
			it->m_sCopyPasteKey.m_pasteMessage.wParam,
			it->m_sCopyPasteKey.m_pasteMessage.lParam);
		writeProfileString(REGKEY_KEY,strBuff,strKeyBuff);
	
		//クリップボード容量制限
		strKeyBuff.Format(_T("HistoryLimit_%d"),i);
		writeProfileInt(REGKEY_KEY,strKeyBuff,it->m_nHistoryLimit);	
	}

	writeEnvInitData();
}


//---------------------------------------------------
//関数名	writeEnvInitData()
//機能		環境情報だけ書き込み
//---------------------------------------------------
void CInit::writeEnvInitData()
{
	if(m_strIniFile == "") return;

	//一般環境設定
	writeProfileString(REGKEY_ENV,_T("DataFile"),m_strDataFile);
	writeProfileString(REGKEY_ENV,_T("RwPluginFolder"),m_strRwPluginFolder);
	writeProfileString(REGKEY_ENV,_T("DataFormat"),m_strPluginName);
	writeProfileString(REGKEY_ENV,_T("MacroPlugin"),m_strMacroPluginName);

	writeProfileInt(REGKEY_ENV,_T("SelectID"),m_nSelectID);
	writeProfileInt(REGKEY_ENV,_T("TreeID"),m_nTreeID);
	writeProfileInt(REGKEY_ENV,_T("RecNumber"),m_nRecNumber);

	//ウィンドウ設定
	writeProfileInt(REGKEY_WINDOW,_T("PopupSizeX"),m_DialogSize.x);
	writeProfileInt(REGKEY_WINDOW,_T("PopupSizeY"),m_DialogSize.y);

	//検索設定
	writeProfileInt(REGKEY_SERCH,_T("SerchKind"),m_nSerchKind);		//検索種別
	writeProfileInt(REGKEY_SERCH,_T("SerchLogic"),m_nSerchLogic);	//検索方法
	writeProfileString(REGKEY_SERCH,_T("SerchKey"),m_strSerchKey);	//検索キー
}


//---------------------------------------------------
//関数名	setIniFileInt()
//機能		設定ファイルを読む
//---------------------------------------------------
int CInit::setIniFileInt(const TCHAR* szSection,const TCHAR* szKey,int nDefault)
{
	int n = (int)::GetPrivateProfileInt(szSection,szKey,-1,m_strIniFile);
	if(n == -1) {
		n = nDefault;
		writeProfileInt(szSection,szKey,n);
	}
	return n;
} 

//---------------------------------------------------
//関数名	setIniFileString()
//機能		設定ファイルを読む
//---------------------------------------------------
CString CInit::setIniFileString(const TCHAR* szSection,const TCHAR* szKey,CString strDefault)
{
	TCHAR strBuff[1024];
	CString strRet;
	::GetPrivateProfileString(szSection,szKey,_T(""),strBuff,_countof(strBuff),m_strIniFile);
	strRet = strBuff;
	if (strRet == "")	{
		// 設定が無い場合はデフォルト(アプリケーションパス)で作成
		writeProfileString(szSection,szKey,strDefault);
		strRet = strDefault;
	}
	return strRet;
} 

//---------------------------------------------------
//関数名	writeProfileInt()
//機能		設定ファイルを書き込み
//---------------------------------------------------
void CInit::writeProfileInt(const TCHAR* szSection,const TCHAR* szKey,int nValue)
{
	TCHAR strBuff[1024];
	_stprintf_s(strBuff,_T("%d"),nValue);	
	WritePrivateProfileString(szSection,szKey,strBuff,m_strIniFile);
}

//---------------------------------------------------
//関数名	writeProfileString()
//機能		設定ファイルを書き込み
//---------------------------------------------------
void CInit::writeProfileString(const TCHAR* szSection,const TCHAR* szKey,CString strValue)
{
	WritePrivateProfileString(szSection,szKey,_T("\"") + strValue + _T("\""),m_strIniFile);
}

//---------------------------------------------------
//関数名	getPasteHotKey
//機能		貼り付け系キー設定を取得
//---------------------------------------------------
void CInit::getPasteHotKey(UINT *uKey,UINT *uMod,UINT *uCopyKey,UINT *uCopyMod)
{
	*uMod = m_key.m_defKeySet.m_uMod_Paste;
	*uKey = m_key.m_defKeySet.m_uVK_Paste;

	*uCopyMod = m_key.m_defKeySet.m_uMod_Copy;
	*uCopyKey = m_key.m_defKeySet.m_uVK_Copy;

}
//---------------------------------------------------
//関数名	setPasteHotkey()
//機能		ホットキー設定をセット
//---------------------------------------------------
void CInit::setPasteHotkey(UINT uKey,UINT uMod,UINT uCopyKey,UINT uCopyMod)
{
	m_key.m_defKeySet.m_uMod_Paste = uMod;
	m_key.m_defKeySet.m_uVK_Paste = uKey;
	m_key.m_defKeySet.m_uMod_Copy = uCopyMod;
	m_key.m_defKeySet.m_uVK_Copy = uCopyKey;
}
//---------------------------------------------------
//関数名	setHotkey()
//機能		ホットキー設定をセット
//---------------------------------------------------
void CInit::setHotkey(UINT uPopKey,UINT uPopMod,UINT uFifoKey,UINT uFifoMod)
{
	m_pop.m_uMod_Pouup = uPopMod;
	m_pop.m_uVK_Pouup = uPopKey;
	m_pop.m_uMod_Fifo = uFifoMod;
	m_pop.m_uVK_Fifo = uFifoKey;
}
//---------------------------------------------------
//関数名	getHotkey()
//機能		ホットキー設定を取得
//---------------------------------------------------
void CInit::getHotKey(UINT *uPopKey,UINT *uPopMod,UINT *uFifoKey,UINT *uFifoMod)
{
	*uPopMod = m_pop.m_uMod_Pouup;
	*uPopKey = m_pop.m_uVK_Pouup;
	*uFifoMod = m_pop.m_uMod_Fifo;
	*uFifoKey = m_pop.m_uVK_Fifo;
}

//---------------------------------------------------
//関数名	setHookKey()
//機能		キーフックを設定
//---------------------------------------------------
bool CInit::setHookKey(HWND hWnd)
{
	bool isRet = false;
	if(!isWindows98()) return true;
	setHook pSetHook;

	if(!m_hHookDLL) {
		m_hHookDLL = LoadLibrary(m_strAppPath + _T("\\HookSet.dll"));
		if(!m_hHookDLL) return isRet;
		pSetHook = (setHook)GetProcAddress(m_hHookDLL,"SetHook");
		if(!pSetHook) return isRet;
		pSetHook(hWnd, WM_KEY_HOOK);

		isRet = true;
	}
	return isRet;
}

//---------------------------------------------------
//関数名	unHookKey()
//機能		フックを解除
//---------------------------------------------------
void CInit::unHookKey()
{
	if(!isWindows98()) return;
	FARPROC pUnHook;
	//フックの解除
	if(m_hHookDLL) {
		pUnHook = ::GetProcAddress(m_hHookDLL, "UnHook");
		if(pUnHook)	pUnHook();
		FreeLibrary(m_hHookDLL);
		m_hHookDLL = NULL;
	}
}


//---------------------------------------------------
//関数名	getAppendKeyInit()
//機能		追加キー設定を取得
//---------------------------------------------------
COPYPASTE_KEY CInit::getAppendKeyInit(CString strWinName,int nNumber)
{
	bool isMatch = false;

	//キー設定リストから適合するものを探す
	list<CHANGE_KEY>::iterator it;
	CHANGE_KEY key;
	COPYPASTE_KEY ret;
	int nFindPoint;

	ret.m_nMessage = -1;
	for(it = m_key.m_KeyList.begin(); it !=  m_key.m_KeyList.end(); it++) {
		key = *it;
		if(key.m_nMatch != MATCH_PHRASE) {
			nFindPoint = strWinName.Find(key.m_strTitle);
		}
		
		//前方一致 頭からあったらOK
		if(key.m_nMatch == MATCH_FORWORD) {
			if(nFindPoint == 0)	isMatch = true;
		}
		//後方一致　最後にあったらOK
		else if(key.m_nMatch == MATCH_BACK && nFindPoint >= 0) {
			if(nFindPoint == (strWinName.GetLength() - key.m_strTitle.GetLength()))	isMatch = true;
		}
		//インクルード　含まれてたらOK
		else if(key.m_nMatch == MATCH_INCLUDE) {
			if(nFindPoint >= 0)	isMatch = true;
		}
		//フレーズ　完全一致
		else if(key.m_nMatch == MATCH_PHRASE) {
			if(strWinName == key.m_strTitle)	isMatch = true;
		}
		
		if(isMatch) {		//見つかった
			if(nNumber > 0) {
				nNumber--;
				isMatch = false;
			}
			else {
				ret = key.m_sCopyPasteKey;
				break;
			}
		}
	}
//	if(!isMatch)		ret = m_key.m_defKeySet;//デフォルトキー設定

	return ret;
}


//---------------------------------------------------
//関数名	getAppendKeyInit()
//機能		追加キー設定を取得
//---------------------------------------------------
CHANGE_KEY CInit::getAppendKeyInit2(CString strWinName)
{
	bool isMatch = false;

	//キー設定リストから適合するものを探す
	list<CHANGE_KEY>::iterator it;
	CHANGE_KEY key;
	int nFindPoint;

	for(it = m_key.m_KeyList.begin(); it !=  m_key.m_KeyList.end(); it++) {
		key = *it;
		if(key.m_nMatch != MATCH_PHRASE) {
			nFindPoint = strWinName.Find(key.m_strTitle);
		}
		
		//前方一致 頭からあったらOK
		if(key.m_nMatch == MATCH_FORWORD) {
			if(nFindPoint == 0)	isMatch = true;
		}
		//後方一致　最後にあったらOK
		else if(key.m_nMatch == MATCH_BACK && nFindPoint >= 0) {
			if(nFindPoint == (strWinName.GetLength() - key.m_strTitle.GetLength()))	isMatch = true;
		}
		//インクルード　含まれてたらOK
		else if(key.m_nMatch == MATCH_INCLUDE) {
			if(nFindPoint >= 0)	isMatch = true;
		}
		//フレーズ　完全一致
		else if(key.m_nMatch == MATCH_PHRASE) {
			if(strWinName == key.m_strTitle)	isMatch = true;
		}
		
		if(isMatch) {		//見つかった
			return key;
		}
	}

	key.m_nMatch = -1;
	return key;
}

