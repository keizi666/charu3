/*----------------------------------------------------------
	設定管理クラス
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#include "stdafx.h"
#include "Charu3.h"
#include "Init.h"
#include "General.h"
#include "StringWork.h"
#include "Color.h"

#include <fstream>
#include <vector>
#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace {

	void ReadPredefined(std::vector<MACRO_STRUCT>& macro, CString filePath)
	{
		nlohmann::json snippets;
		try { snippets = nlohmann::json::parse(std::ifstream(filePath)); }
		catch (...) {}
		for (nlohmann::json::iterator it = snippets.begin(); it != snippets.end(); it++) {
			nlohmann::json jnode = it.value();
			MACRO_STRUCT snippet;
			snippet.m_strName = CGeneral::getSettingCString(jnode, "caption", _T(""));
			snippet.m_strMacro = CGeneral::getSettingCString(jnode, "data", _T(""));
			snippet.m_cKind = static_cast<int>(CGeneral::getSettingNumber(jnode, "mode", 255));
			macro.push_back(snippet);
		}
	}

} // anonymous namespace

//---------------------------------------------------
//関数名	CInit
//機能		コンストラクタ
//---------------------------------------------------
CInit::CInit()
	: m_nSelectID(0)
	, m_nOptionPage(0)
	, m_nRecNumber(0)
	, m_nSearchTarget(0)
	, m_nSearchLogic(0)
	, m_bReadOnly(false)
	, m_strIniFile(_T(""))
	, m_strStateFile(_T(""))
	, m_strSettingsFile(_T(""))
	, m_hHookDLL(nullptr)
{
	m_DialogSize.x = 100;
	m_DialogSize.y = 100;

	m_IconSize.x = ICON_SIZE;
	m_IconSize.y = ICON_SIZE;
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
	TCHAR buf[MAX_PATH];
	TCHAR *cpName;

	// locale
	m_locale.LoadString(APP_LOCALE);

	// user name
	DWORD dwSize = _countof(buf);
	if(::GetUserName(buf, &dwSize)) {
		m_strUserName = CString(buf);
	}
	else {
		m_strUserName = _T("Charu3User");
	}

	// app and plugin path
	GetModuleFileName(NULL, buf, _countof(buf));
	cpName = _tcsrchr(buf, _T('\\'));
	cpName++;
	*cpName = '\0';
	m_strAppPath = CString(buf);
	m_strRwPluginFolder = m_strAppPath + _T("rw_plugin");

	// app data path
	SHGetSpecialFolderPath(NULL, buf, CSIDL_APPDATA, 0);
	m_strAppDataPath = CString(buf) + "\\" + NAME;
	if (!FILEEXIST(m_strAppDataPath)) {
		CreateDirectory(m_strAppDataPath, NULL);
	}

	// file paths for state, settings and log
	m_strIniFile = m_strAppDataPath + "\\" + INI_FILE;
	m_strStateFile = m_strAppDataPath + "\\" + STATE_FILE;
	m_strSettingsFile = m_strAppDataPath + "\\" + SETTINGS_FILE;
	m_strDebugLog = m_strAppDataPath + "\\" + DEBUGLOG_FILE;

	// read state
	try { m_state = nlohmann::json::parse(std::ifstream(m_strStateFile)); }
	catch (...) {}

	// read settings
	try { m_settings = nlohmann::json::parse(std::ifstream(m_strSettingsFile)); }
	catch (...) {}

	//if(m_strIniFile == "") return;

	//int i;
	////int nKetsetCount;
	//CString strKeyBuff,StringBuff;
	//CString strRes;
	//CHANGE_KEY key;
	//TCHAR strBuff[1024],*szName,*szKind,*szMacro;

	//一般設定
	//m_etc.m_nActiveTime = setIniFileInt(REGKEY_ETC,_T("ActiveTime"),400);	//監視タイマー
	//m_etc.m_nCopyRetryTimes = setIniFileInt(REGKEY_ETC,_T("CopyWaitCnt"),5);
	//m_etc.m_nPasteRetryTimes = setIniFileInt(REGKEY_ETC,_T("PasteWaitCnt"),10);

	//m_pop.m_nDCKeyPopTime = setIniFileInt(REGKEY_POPUP,_T("DCPopupKeyTime"),250);
	//m_pop.m_nDCKeyFifoTime = setIniFileInt(REGKEY_POPUP,_T("DCFifoKeyTime"),250);

	//m_nToolTipTime = setIniFileInt(REGKEY_ENV, _T("ToolTipTime"), 30000);
	//m_nToolTipDelay = setIniFileInt(REGKEY_ENV, _T("ToolTipDelay"), 300);

	//履歴FIFO設定
	//m_fifo.m_nFifo = setIniFileInt(REGKEY_FIFO,_T("FifoSW"),1);			//履歴動作中はCtrl+Vで貼り付ける文字列を先入れ先出しにする 0NON 1FIFO 2RIFO
	//m_fifo.m_strCopySound = setIniFileString(REGKEY_FIFO,_T("WaveName"),_T("pu.wav"));
	//m_fifo.m_nOffClear = setIniFileInt(REGKEY_FIFO,_T("OffClear"),0);		//履歴OFF時に一時項目をクリア
	//m_fifo.m_nAllClearOff = setIniFileInt(REGKEY_FIFO,_T("AllClearOff"),1);//一時履歴が空になったら履歴モードをOFFにする
	//m_fifo.m_nDuplication = setIniFileInt(REGKEY_FIFO,_T("Duplication"),1);

	////仮想キー設定
	//WINDOWS_MESSAGE winMessage;
	//// m_key.m_defKeySet.m_nMessage = setIniFileInt(REGKEY_KEY,_T("isMessage"),0); // obsolete
	//m_key.m_defKeySet.m_nMessage = 0; //メッセージ方式
	//m_key.m_defKeySet.m_uMod_Paste = setIniFileInt(REGKEY_KEY,_T("ModPaste"),MOD_CONTROL);	//貼り付け特殊キー
	//m_key.m_defKeySet.m_uVK_Paste  = setIniFileInt(REGKEY_KEY,_T("VKPaste"),'V');			//貼り付けキー
	//m_key.m_defKeySet.m_uMod_Copy  = setIniFileInt(REGKEY_KEY,_T("ModCopy"),MOD_CONTROL);	//コピー特殊キー
	//m_key.m_defKeySet.m_uVK_Copy   = setIniFileInt(REGKEY_KEY,_T("VkCopy"),'C');			//コピーキー
	//m_key.m_defKeySet.m_nCopyWait  = setIniFileInt(REGKEY_KEY,_T("CopyWait"),50);		//コピー待ち時間
	//m_key.m_defKeySet.m_nPasteWait = setIniFileInt(REGKEY_KEY,_T("PasteWait"),50);		//ペースト待ち時間
	////メッセージ
	//strKeyBuff = setIniFileString(REGKEY_KEY,_T("CopyMessage"),_T("301,0,0"));
	//_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
	//m_key.m_defKeySet.m_copyMessage = winMessage;

	//strKeyBuff = setIniFileString(REGKEY_KEY,_T("PasteMessage"),_T("302,0,0"));
	//_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
	//m_key.m_defKeySet.m_pasteMessage = winMessage;
	////クリップボード容量制限
	//m_key.m_nHistoryLimit = setIniFileInt(REGKEY_KEY,_T("HistoryLimit"),-1);	

	////固有キー設定を読む
	//nKetsetCount = setIniFileInt(REGKEY_KEY,_T("KeySetCount"),0);
	//m_key.m_KeyList.clear();
	//for(i = 0; i < nKetsetCount; i++){
	//	StringBuff.Format(_T("KeySetTitle_%d"),i);
	//	//ウィンドウキャプション
	//	key.m_strTitle = setIniFileString(REGKEY_KEY,StringBuff,_T(""));
	//	//マッチ方法
	//	StringBuff.Format(_T("KeySetMatch_%d"),i);
	//	key.m_nMatch = setIniFileInt(REGKEY_KEY,StringBuff,0);
	//	//キーコード
	//	StringBuff.Format(_T("KeySetCode_%d"),i);
	//	strKeyBuff = setIniFileString(REGKEY_KEY,StringBuff,"");
	//	_stscanf_s(LPCTSTR(strKeyBuff),_T("%d,%d,%d,%d")
	//		,&key.m_sCopyPasteKey.m_uMod_Copy,&key.m_sCopyPasteKey.m_uVK_Copy
	//		,&key.m_sCopyPasteKey.m_uMod_Paste,&key.m_sCopyPasteKey.m_uVK_Paste);
	//	StringBuff.Format(_T("KeyCopyWait_%d"),i);
	//	key.m_sCopyPasteKey.m_nCopyWait  = setIniFileInt(REGKEY_KEY,StringBuff,100);		//コピー待ち時間
	//	StringBuff.Format(_T("KeyPasteWait_%d"),i);
	//	key.m_sCopyPasteKey.m_nPasteWait = setIniFileInt(REGKEY_KEY,StringBuff,100);		//ペースト待ち時間

	//	StringBuff.Format(_T("KeyMessage_%d"),i);
	//	key.m_sCopyPasteKey.m_nMessage = setIniFileInt(REGKEY_KEY,StringBuff,0);		//方式

	//	//メッセージ
	//	StringBuff.Format(_T("CopyMessage_%d"),i);
	//	strKeyBuff = setIniFileString(REGKEY_KEY,StringBuff,_T("301,0,0"));
	//	_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
	//	key.m_sCopyPasteKey.m_copyMessage = winMessage;
	//
	//	StringBuff.Format(_T("PasteMessage_%d"),i);
	//	strKeyBuff = setIniFileString(REGKEY_KEY,StringBuff,_T("302,0,0"));
	//	_stscanf_s(LPCTSTR(strKeyBuff),_T("%x,%x,%x"),&winMessage.Msg,&winMessage.wParam,&winMessage.lParam);
	//	key.m_sCopyPasteKey.m_pasteMessage = winMessage;
	//	//クリップボード容量制限
	//	StringBuff.Format(_T("HistoryLimit_%d"),i);
	//	key.m_nHistoryLimit = setIniFileInt(REGKEY_KEY,StringBuff,-1);	
	//	
	//	m_key.m_KeyList.insert(m_key.m_KeyList.end(),key);//設定に追加
	//}	

	//一般環境設定
	//m_strDataPath = setIniFileString(REGKEY_ENV,_T("DataFile"),"");

	//m_strRwPluginFolder = setIniFileString(REGKEY_ENV,_T("RwPluginFolder"),m_strAppPath + _T("RW_Plugin"));
	//m_strPluginName = setIniFileString(REGKEY_ENV,_T("DataFormat"),DAT_FORMAT);
	//m_strMacroPluginName = setIniFileString(REGKEY_ENV,_T("MacroPlugin"),DAT_FORMAT);
	//m_nSelectID     = setIniFileInt(REGKEY_ENV,_T("SelectID"),-1);
	//time_t lTime;
	//m_nTreeID       = setIniFileInt(REGKEY_ENV,_T("TreeID"),time(&lTime));
	//m_nRecNumber    = setIniFileInt(REGKEY_ENV,_T("RecNumber"),0);
	//m_nDebug		= setIniFileInt(REGKEY_ENV,_T("Debug"),0) != 0;
	//m_strDebugLog	= setIniFileString(REGKEY_ENV,_T("LogfileName"),_T("Charu3.log"));

	//ウィンドウ設定
	//m_DialogSize.x = setIniFileInt(REGKEY_WINDOW,_T("PopupSizeX"),250);
	//m_DialogSize.y = setIniFileInt(REGKEY_WINDOW,_T("PopupSizeY"),350);

	//検索設定
	//m_nSearchTarget = setIniFileInt(REGKEY_SERCH,_T("SerchKind"),0);
	//m_nSearchLogic = setIniFileInt(REGKEY_SERCH,_T("SerchLogic"),0);
	//m_strSearchKey = setIniFileString(REGKEY_SERCH,_T("SerchKey"),_T(""));

	//マクロテンプレート
	//for(i = 0; i <= 99; i++) { // TODO: magic number
	//	strRes.LoadString(APP_INF_MACRO_TEMPLATE01+i);
	//	if(strRes == _T("end")) break;
	//}
	//int nStringCnt = i;
	MACRO_STRUCT macroData;
	CString strMacro;
	//m_vctMacro.clear();
	//m_vctMacro.reserve(99); // TODO: magic number
	//for(i = 1; i <= 99; i++) { // TODO: magic number
	//	StringBuff.Format(_T("Macro_%02d"),i);
	//	::GetPrivateProfileString(REGKEY_MACRO,StringBuff,_T(""),strBuff,_countof(strBuff),m_strIniFile);
	//	if (_tcsclen(strBuff) == 0 && i <= nStringCnt) {
	//		strRes.LoadString(APP_INF_MACRO_TEMPLATE01 + i -1);
	//		writeProfileString(REGKEY_MACRO,StringBuff,strRes);
	//		strMacro = strRes;
	//		_tcscpy_s(strBuff,LPCTSTR(strRes));
	//	}
	//	if(_tcsclen(strBuff) == 0) break;
	//	if(UStringWork::splitString(strBuff,_T('@'),&szName,&szKind,&szMacro,NULL) == 3) {
	//		macroData.m_strName = szName;
	//		if(*szKind == _T('D'))		macroData.m_cKind = KIND_DATA_ALL;
	//		else if(*szKind == _T('F'))	macroData.m_cKind = KIND_FOLDER_ALL;
	//		else					macroData.m_cKind = 0xff;
	//		macroData.m_strMacro = szMacro;
	//		m_vctMacro.push_back(macroData);
	//	}
	//}
	
	//拡張マクロテンプレート R履歴フォルダ Fフォルダ A全て Dデータ
	//for(i = 0; i <= 99; i++) { // TODO: magic number
	//	strRes.LoadString(APP_INF_EXMACRO_TEMPLATE01+i);
	//	if(strRes == "end") break;
	//}
	//int nStringCnt = i;
	//m_vctDataMacro.clear();
	//m_vctDataMacro.reserve(99); // TODO: magic number
	//for(i = 1; i <= 99; i++) { // TODO: magic number
	//	StringBuff.Format(_T("DataMacro_%02d"),i);
	//	::GetPrivateProfileString(REGKEY_MACRO,StringBuff,_T(""),strBuff,_countof(strBuff),m_strIniFile);
	//	if (_tcsclen(strBuff) == 0 && i <= nStringCnt) {
	//		strRes.LoadString(APP_INF_EXMACRO_TEMPLATE01 + i -1);
	//		writeProfileString(REGKEY_MACRO,StringBuff,strRes);
	//		strMacro = strRes;
	//		_tcscpy_s(strBuff,LPCTSTR(strRes));
	//	}
	//	if(_tcsclen(strBuff) == 0) break;
	//	if(UStringWork::splitString(strBuff,'@',&szName,&szKind,&szMacro,NULL) == 3) {
	//		macroData.m_strName = szName;
			//if(*szKind == 'D')		macroData.m_cKind = KIND_DATA_ALL;
			//else if(*szKind == 'R')	macroData.m_cKind = KIND_RIREKI;
			//else if(*szKind == 'F')	macroData.m_cKind = KIND_FOLDER_ALL;
			//else					macroData.m_cKind = 0xff;
	//		macroData.m_strMacro = szMacro;
	//		m_vctDataMacro.push_back(macroData);
	//	}
	//}

	// read state

	m_strDataPath = CGeneral::getSettingCString(m_state, "data.path", _T(""));
	m_strDataFormat = CGeneral::getSettingCString(m_state, "data.format", DAT_FORMAT);
	m_bReadOnly = CGeneral::getSettingBool(m_state, "data.readOnly", false);
	m_DialogSize.x = static_cast<LONG>(CGeneral::getSettingNumber(m_state, "treeview.width", 250));
	m_DialogSize.y = static_cast<LONG>(CGeneral::getSettingNumber(m_state, "treeview.height", 350));
	m_nSearchTarget = static_cast<int>(CGeneral::getSettingNumber(m_state, "find.target", SEARCH_TARGET_NAME));
	m_nSearchLogic = static_cast<int>(CGeneral::getSettingNumber(m_state, "find.logic", SEARCH_TARGET_DATA));
	m_strSearchKeywords = CGeneral::getSettingCString(m_state, "find.keywords", _T(""));
	m_nSelectID = static_cast<int>(CGeneral::getSettingNumber(m_state, "internal.selectId", -1));
	{
		time_t lTime;
		m_nTreeID = static_cast<int>(CGeneral::getSettingNumber(m_state, "internal.treeId", time(&lTime)));
	}
	m_nRecNumber = static_cast<int>(CGeneral::getSettingNumber(m_state, "internal.recordNumber", 0));

	// read settings

	m_etc.m_bPutBackClipboard = CGeneral::getSettingBool(m_settings, "clipboard.putBackAfterPasting", false);
	m_nClipboardOpenDelay = static_cast<int>(CGeneral::getSettingNumber(m_settings, "clipboard.openDelay", 0));
	m_nClipboardRetryInterval = static_cast<int>(CGeneral::getSettingNumber(m_settings, "clipboard.retryInterval", 50));
	m_nClipboardRetryTimes = static_cast<int>(CGeneral::getSettingNumber(m_settings, "clipboard.retryTimes", 10));

	m_pop.m_nDoubleKeyPOP = static_cast<int>(CGeneral::getSettingNumber(m_settings, "hotkey.popup", 0));
	m_pop.m_nDCKeyPopTime = static_cast<int>(CGeneral::getSettingNumber(m_settings, "hotkey.popup.hitTime", 250));
	m_pop.m_uVK_Pouup = static_cast<UINT>(CGeneral::getSettingNumber(m_settings, "hotkey.popup.keyCode", 88)); // X
	m_pop.m_uMod_Pouup = static_cast<UINT>(CGeneral::getSettingNumber(m_settings, "hotkey.popup.keyModifier", MOD_ALT));
	m_pop.m_nDoubleKeyFIFO = static_cast<int>(CGeneral::getSettingNumber(m_settings, "hotkey.stockmode", 0));
	m_pop.m_nDCKeyFifoTime = static_cast<int>(CGeneral::getSettingNumber(m_settings, "hotkey.stockmode.hitTime", 250));
	m_pop.m_uVK_Fifo = static_cast<UINT>(CGeneral::getSettingNumber(m_settings, "hotkey.stockmode.keyCode", 83)); // S
	m_pop.m_uMod_Fifo = static_cast<UINT>(CGeneral::getSettingNumber(m_settings, "hotkey.stockmode.keyModifier", MOD_CONTROL|MOD_SHIFT));

	m_etc.m_nIconClick = static_cast<int>(CGeneral::getSettingNumber(m_settings, "notifyIcon.clickedBehavior", 0));
	m_etc.m_bShowClipboardInTooltipOfNofifyIcon = CGeneral::getSettingBool(m_settings, "notifyIcon.showClipboard", true);

	m_fifo.m_nFifo = static_cast<int>(CGeneral::getSettingNumber(m_settings, "stockmode.behavior", 1));
	m_fifo.m_bAutoOff = CGeneral::getSettingBool(m_settings, "stockmode.autoTurnOff", false);
	m_fifo.m_bCleanupAtTurnOff = CGeneral::getSettingBool(m_settings, "stockmode.cleanupAtTurnOff", false);
	m_fifo.m_bDontSaveSameDataAsLast = CGeneral::getSettingBool(m_settings, "stockmode.dontSaveSameDataAsLast", false);
	m_fifo.m_strCopySound = CGeneral::getSettingCString(m_settings, "stockmode.sound.copy", _T("pu.wav"));
	m_fifo.m_strPasteSound = CGeneral::getSettingCString(m_settings, "stockmode.sound.paste", _T(""));

	m_nToolTipTime = static_cast<int>(CGeneral::getSettingNumber(m_settings, "tooltip.autopop", 30000));
	m_nToolTipDelay = static_cast<int>(CGeneral::getSettingNumber(m_settings, "tooltip.delay", 300));

	m_pop.m_nPopupPos = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.popupPosition", 0));
	m_pop.m_posCaretHosei.x = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.popupOffset.x", -20));
	m_pop.m_posCaretHosei.y = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.popupOffset.y", 0));
	m_pop.m_nSelectByTypingFinalizePeriod = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.selectByTyping.finalizePeriod", 350));
	m_pop.m_bSelectByTypingCaseInsensitive = CGeneral::getSettingBool(m_settings, "treeview.selectByTyping.caseInsensitive", true);
	m_pop.m_bSelectByTypingAutoPaste = CGeneral::getSettingBool(m_settings, "treeview.selectByTyping.autoPaste", false);
	m_pop.m_bSelectByTypingAutoExpand = CGeneral::getSettingBool(m_settings, "treeview.selectByTyping.autoExpand", false);
	m_visual.m_nToolTip = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.tooltip", 0));
	m_visual.m_bScrollbarVertical = CGeneral::getSettingBool(m_settings, "treeview.scrollbar.vertical", true);
	m_visual.m_bScrollbarHorizontal = CGeneral::getSettingBool(m_settings, "treeview.scrollbar.horizontal", true);
	m_pop.m_bSingleExpand = CGeneral::getSettingBool(m_settings, "treeview.expandOnSingleClick", false);
	m_pop.m_bKeepSelection = CGeneral::getSettingBool(m_settings, "treeview.keepSelection", true);
	m_pop.m_bKeepFolders = CGeneral::getSettingBool(m_settings, "treeview.keepFolders", true);

	m_visual.m_nBorderColor = Color::Parse(CGeneral::getSettingString(m_settings, "treeview.style.borderColor", "#ff9900"));
	m_visual.m_nBackgroundColor = Color::Parse(CGeneral::getSettingString(m_settings, "treeview.style.backgroundColor", "#ffffee"));
	m_visual.m_nTextColor = Color::Parse(CGeneral::getSettingString(m_settings, "treeview.style.textColor", "#663300"));
	m_visual.m_strFontName = CGeneral::getSettingCString(m_settings, "treeview.style.fontName", m_visual.m_strFontName);
	m_visual.m_nFontSize = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.style.fontSize", 100));
	m_visual.m_strResourceName = CGeneral::getSettingCString(m_settings, "treeview.style.iconFile", m_visual.m_strResourceName);
	m_visual.m_nOpacity = static_cast<int>(CGeneral::getSettingNumber(m_settings, "treeview.opacity", 100));

	m_nWindowCheckInterval = static_cast<int>(CGeneral::getSettingNumber(m_settings, "windowCheckInterval", 400));

	m_bDebug = CGeneral::getSettingBool(m_settings, "debug", false);

	m_key.m_KeyList.clear();
	nlohmann::json keyEventByWindow = m_settings["keyEvent.byWindow"];
	for (nlohmann::json::iterator it = keyEventByWindow.begin(); it != keyEventByWindow.end(); it++) {
		nlohmann::json jnode = it.value();
		CHANGE_KEY node;
		node.m_strTitle = CGeneral::getSettingCString(jnode, "caption", _T(""));
		node.m_nMatch = static_cast<int>(CGeneral::getSettingNumber(jnode, "captionMatchCondition", 0));
		node.m_nHistoryLimit = static_cast<int>(CGeneral::getSettingNumber(jnode, "copyLimit", -1));
		node.m_sCopyPasteKey = COPYPASTE_KEY(jnode["keyEvent"]);
		m_key.m_KeyList.push_back(node);
	}
	m_key.m_defKeySet = COPYPASTE_KEY(m_settings["keyEvent.default"]);
	m_key.m_nHistoryLimit = m_settings["keyEvent.default.copyLimit"];

	SaveSettings();

	// read snippets

	m_vctMacro.clear();
	ReadPredefined(m_vctMacro, m_strAppPath + "_locale\\" + m_locale + "\\snippets.json");
	ReadPredefined(m_vctMacro, m_strAppDataPath + "\\snippets.json");

	// read options

	m_vctDataMacro.clear();
	ReadPredefined(m_vctDataMacro, m_strAppPath + "_locale\\" + m_locale + "\\options.json");
}

//---------------------------------------------------
//関数名	SaveSettings()
//機能		書き込み
//---------------------------------------------------
void CInit::SaveSettings()
{
	m_settings["clipboard.putBackAfterPasting"] = m_etc.m_bPutBackClipboard;
	m_settings["clipboard.openDelay"] = m_nClipboardOpenDelay;
	m_settings["clipboard.retryInterval"] = m_nClipboardRetryInterval;
	m_settings["clipboard.retryTimes"] = m_nClipboardRetryTimes;

	m_settings["hotkey.popup"] = m_pop.m_nDoubleKeyPOP;
	m_settings["hotkey.popup.hitTime"] = m_pop.m_nDCKeyPopTime;
	m_settings["hotkey.popup.keyCode"] = m_pop.m_uVK_Pouup;
	m_settings["hotkey.popup.keyModifier"] = m_pop.m_uMod_Pouup;
	m_settings["hotkey.stockmode"] = m_pop.m_nDoubleKeyFIFO;
	m_settings["hotkey.stockmode.hitTime"] = m_pop.m_nDCKeyFifoTime;
	m_settings["hotkey.stockmode.keyCode"] = m_pop.m_uVK_Fifo;
	m_settings["hotkey.stockmode.keyModifier"] = m_pop.m_uMod_Fifo;

	m_settings["notifyIcon.clickedBehavior"] = m_etc.m_nIconClick;
	m_settings["notifyIcon.showClipboard"] = m_etc.m_bShowClipboardInTooltipOfNofifyIcon;

	m_settings["stockmode.behavior"] = m_fifo.m_nFifo;
	m_settings["stockmode.autoTurnOff"] = m_fifo.m_bAutoOff;
	m_settings["stockmode.cleanupAtTurnOff"] = m_fifo.m_bCleanupAtTurnOff;
	m_settings["stockmode.dontSaveSameDataAsLast"] = m_fifo.m_bDontSaveSameDataAsLast;
	m_settings["stockmode.sound.copy"] = CGeneral::ConvertUnicodeToUTF8(m_fifo.m_strCopySound);
	m_settings["stockmode.sound.paste"] = CGeneral::ConvertUnicodeToUTF8(m_fifo.m_strPasteSound);

	m_settings["tooltip.autopop"] = m_nToolTipTime;
	m_settings["tooltip.delay"] = m_nToolTipDelay;

	m_settings["treeview.popupPosition"] = m_pop.m_nPopupPos;
	m_settings["treeview.popupOffset.x"] = m_pop.m_posCaretHosei.x;
	m_settings["treeview.popupOffset.y"] = m_pop.m_posCaretHosei.y;
	m_settings["treeview.selectByTyping.finalizePeriod"] = m_pop.m_nSelectByTypingFinalizePeriod;
	m_settings["treeview.selectByTyping.caseInsensitive"] = m_pop.m_bSelectByTypingCaseInsensitive;
	m_settings["treeview.selectByTyping.autoPaste"] = m_pop.m_bSelectByTypingAutoPaste;
	m_settings["treeview.selectByTyping.autoExpand"] = m_pop.m_bSelectByTypingAutoExpand;
	m_settings["treeview.tooltip"] = m_visual.m_nToolTip;
	m_settings["treeview.scrollbar.vertical"] = m_visual.m_bScrollbarVertical;
	m_settings["treeview.scrollbar.horizontal"] = m_visual.m_bScrollbarHorizontal;
	m_settings["treeview.expandOnSingleClick"] = m_pop.m_bSingleExpand;
	m_settings["treeview.keepSelection"] = m_pop.m_bKeepSelection;
	m_settings["treeview.keepFolders"] = m_pop.m_bKeepFolders;

	m_settings["treeview.style.backgroundColor"] = Color::String(m_visual.m_nBackgroundColor);
	m_settings["treeview.style.borderColor"] = Color::String(m_visual.m_nBorderColor);
	m_settings["treeview.style.textColor"] = Color::String(m_visual.m_nTextColor);
	m_settings["treeview.style.fontName"] = CGeneral::ConvertUnicodeToUTF8(m_visual.m_strFontName);
	m_settings["treeview.style.fontSize"] = m_visual.m_nFontSize;
	m_settings["treeview.style.iconFile"] = CGeneral::ConvertUnicodeToUTF8(m_visual.m_strResourceName);
	m_settings["treeview.opacity"] = m_visual.m_nOpacity;

	m_settings["windowCheckInterval"] = m_nWindowCheckInterval;

	m_settings["debug"] = m_bDebug;

	nlohmann::json keyEventByWindow;
	std::list<CHANGE_KEY>::iterator it;
	for (it = m_key.m_KeyList.begin(); it != m_key.m_KeyList.end(); it++) {
		nlohmann::json node;
		node["caption"] = CGeneral::ConvertUnicodeToUTF8(it->m_strTitle);
		node["captionMatchCondition"] = it->m_nMatch;
		node["keyEvent"] = it->m_sCopyPasteKey.ToJson();
		node["copyLimit"] = it->m_nHistoryLimit;
		keyEventByWindow.push_back(node);
	}
	m_settings["keyEvent.byWindow"] = keyEventByWindow;
	m_settings["keyEvent.default"] = m_key.m_defKeySet.ToJson();
	m_settings["keyEvent.default.copyLimit"] = m_key.m_nHistoryLimit;

	try { std::ofstream(m_strSettingsFile) << m_settings.dump(1, '\t') << "\n"; }
	catch (...) {}
}

//---------------------------------------------------
//関数名	writeAllInitData()
//機能		書き込み
//---------------------------------------------------
void CInit::writeAllInitData()
{
	SaveSettings();
	writeEnvInitData();
}

//---------------------------------------------------
//関数名	writeEnvInitData()
//機能		環境情報だけ書き込み
//---------------------------------------------------
void CInit::writeEnvInitData()
{
	m_state["data.path"] = CGeneral::ConvertUnicodeToUTF8(m_strDataPath);
	m_state["data.format"] = CGeneral::ConvertUnicodeToUTF8(m_strDataFormat);
	m_state["data.readOnly"] = m_bReadOnly;
	m_state["treeview.width"] = m_DialogSize.x;
	m_state["treeview.height"] = m_DialogSize.y;
	m_state["find.target"] = m_nSearchTarget;
	m_state["find.logic"] = m_nSearchLogic;
	m_state["find.keywords"] = CGeneral::ConvertUnicodeToUTF8(m_strSearchKeywords);
	m_state["internal.selectId"] = m_nSelectID;
	m_state["internal.treeId"] = m_nTreeID;
	m_state["internal.recordNumber"] = m_nRecNumber;
	try { std::ofstream(m_strStateFile) << m_state.dump(1, '\t') << "\n"; } catch (...) {}
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
		// Use default if empty
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
	auto err = GetLastError();
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
	std::list<CHANGE_KEY>::iterator it;
	CHANGE_KEY key;
	COPYPASTE_KEY ret;
	int nFindPoint;

	ret.m_nMessage = -1;
	for(it = m_key.m_KeyList.begin(); it !=  m_key.m_KeyList.end(); it++) {
		key = *it;
		if(key.m_nMatch != MATCH_PHRASE) {
			nFindPoint = strWinName.Find(key.m_strTitle);
		}
		
		// 前方一致
		if(key.m_nMatch == MATCH_FORWARD) {
			if(nFindPoint == 0)	isMatch = true;
		}
		// 後方一致
		else if(key.m_nMatch == MATCH_BACKWARD && nFindPoint >= 0) {
			if(nFindPoint == (strWinName.GetLength() - key.m_strTitle.GetLength()))	isMatch = true;
		}
		// 部分一致
		else if(key.m_nMatch == MATCH_PARTIAL) {
			if(nFindPoint >= 0)	isMatch = true;
		}
		// 完全一致
		else if(key.m_nMatch == MATCH_EXACT) {
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
	std::list<CHANGE_KEY>::iterator it;
	CHANGE_KEY key;
	int nFindPoint;

	for(it = m_key.m_KeyList.begin(); it !=  m_key.m_KeyList.end(); it++) {
		key = *it;
		if(key.m_nMatch != MATCH_EXACT) {
			nFindPoint = strWinName.Find(key.m_strTitle);
		}
		
		// 前方一致
		if(key.m_nMatch == MATCH_FORWARD) {
			if(nFindPoint == 0)	isMatch = true;
		}
		// 後方一致
		else if(key.m_nMatch == MATCH_BACKWARD && nFindPoint >= 0) {
			if(nFindPoint == (strWinName.GetLength() - key.m_strTitle.GetLength()))	isMatch = true;
		}
		// 部分一致
		else if(key.m_nMatch == MATCH_PARTIAL) {
			if(nFindPoint >= 0)	isMatch = true;
		}
		// 完全一致
		else if(key.m_nMatch == MATCH_EXACT) {
			if(strWinName == key.m_strTitle)	isMatch = true;
		}
		
		if(isMatch) {		//見つかった
			return key;
		}
	}

	key.m_nMatch = -1;
	return key;
}

