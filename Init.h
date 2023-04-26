/*----------------------------------------------------------
	設定管理クラスヘッダ
									2002/11/16 (c)Keizi
----------------------------------------------------------*/

#if !defined(AFX_INIT_H__EA6EFDDA_9745_4690_B6A9_8B0533E4AB56__INCLUDED_)
#define AFX_INIT_H__EA6EFDDA_9745_4690_B6A9_8B0533E4AB56__INCLUDED_

#include "Charu3.h"
#include "MyHotkeyCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;


//設定セクション定義
#define REGKEY_ENV		_T("Environment")
#define REGKEY_WINDOW	_T("Windows")
#define REGKEY_SERCH	_T("Serch")

#define REGKEY_ETC		_T("EtcSetting")
#define REGKEY_POPUP	_T("PouupSetting")
#define REGKEY_FIFO		_T("FifoSetting")
#define REGKEY_VISUAL	_T("VisualSetting")
#define REGKEY_KEY		_T("KeySetting")

#define REGKEY_PLUGIN	_T("PluginSetting")
#define REGKEY_MACRO	_T("MacroTemplate")

//---------------------------------------------------
// 一般設定構造体
//---------------------------------------------------
struct OPTION_ETC
{
	int m_nMulchUser;//マルチユーザーで使うかどうか // No longer used

	int m_nToClip;		//リストから選んだ文字列はクリップボードにも入れる
	int m_nToolTip;	//タスクトレイアイコンのツールチップを固定にする
	int m_nMinimization;	//最小化する

	int m_nIconClick;		//アイコンをクリックしたときの動作
	int m_nWordBugSW;		//Wordのバグに対応する

	int m_nSelfDiagnosisTime;//自己診断タイマー
	int m_nActiveTime;		//監視タイマー

	int m_nDontSave;

	int m_nCopyWaitCnt;
	int m_nPasteWaitCnt;
};

//---------------------------------------------------
// ポップアップ設定構造体
//---------------------------------------------------
struct OPTION_POPUP
{
	int  m_nSelectSW;		//テキストリストの選択位置を保持する
	int  m_nFolderSW;		//テキストリストのフォルダ状態を保持する
	int  m_nCornerPopup;		//マウスカーソルが画面の端に触れたらポップアップする
	int  m_nCornerPopupTime;	//マウスカーソルが画面の端に触れたらポップアップする
	int  m_nCornerPopupPix;	//マウスカーソルが画面の端に触れたらポップアップする

	int  m_nDoubleKeyPOP;
	UINT m_uVK_Pouup;	//ポップアップキー
	UINT m_uMod_Pouup;	//ポップアップ特殊キー
	int  m_nDCKeyPopTime;

	int  m_nDoubleKeyFIFO;
	UINT m_uVK_Fifo;	//履歴FIFO切り替えキー
	UINT m_uMod_Fifo;	//履歴FIFO切り替え特殊キー
	int  m_nDCKeyFifoTime;

	int  m_nPopupPos;		//ポップアップ出現位置
	POINT m_posCaretHosei;	//キャレット位置の補正値
	int  m_nOutRevice;

	int  m_nQuickEnter;		//クイックアクセス終了時の貼り付け
	int  m_nQuickChar;		//クイックアクセスで大文字小文字を区別しない
	int  m_nQuickTime;		//クイックアクセス確定時間
	int  m_nAutoImeOff;

	int  m_nSingleExpand;
};

//---------------------------------------------------
// ストックモード設定構造体
//---------------------------------------------------
struct OPTION_FIFO
{
	int  m_nFifo;			//履歴動作中はCtrl+Vで貼り付ける文字列を先入れ先出しにする
	int  m_nCopySound;		//履歴記録中はコピー時に効果音を鳴らす
	CString m_strWaveFile;	//効果音のファイル名
	int  m_nOffClear;		//履歴OFF時に一時項目をクリア
	int  m_nNoClear;		//履歴モードで最後に空にしないスイッチ
	int  m_nPasteSound;		//貼り付け時にも音を鳴らすスイッチ
	int  m_nAllClearOff;	//一時履歴が空になったら履歴モードをOFFにする
	int  m_nDuplication;	//重複チェック
};

//---------------------------------------------------
// ビジュアル設定構造体
//---------------------------------------------------
struct OPTION_VISUAL
{
	int m_nSemitransparent;
	int m_nScrollbar;

	CString m_strFontName;//フォント名
	int m_nFontSize;		//フォントサイズ

	CString m_strResourceName;//リソース名
	uint32_t m_nBorderColor;		//枠色
	uint32_t m_nBackColor;		//背景色
	uint32_t m_nTextColor;		//テキスト色
	int m_nResetTree;

	int m_nToolTip;			//ツールチップの表示項目
};

struct WINDOWS_MESSAGE
{
	UINT Msg; // message to send
	WPARAM wParam; // first message parameter
	LPARAM lParam; // second message parameter
};

//---------------------------------------------------
// キー設定構造体
//---------------------------------------------------
struct COPYPASTE_KEY
{
	int  m_nMessage;	//メッセージ方式フラグ
	UINT m_uMod_Paste;	//貼り付け特殊キー
	UINT m_uVK_Paste;	//貼り付けキー
	
	UINT m_uMod_Copy;	//コピー特殊キー
	UINT m_uVK_Copy;	//コピーキー

	WINDOWS_MESSAGE m_copyMessage;
	WINDOWS_MESSAGE m_pasteMessage;

	int  m_nCopyWait;		//コピー待ち時間
	int  m_nPasteWait;		//ペースト待ち時間
};
//---------------------------------------------------
// 自動キー変更構造体
//---------------------------------------------------
struct CHANGE_KEY
{
	CString m_strTitle;	//タイトル文字列
	int		m_nMatch;
	COPYPASTE_KEY m_sCopyPasteKey;
	int  m_nHistoryLimit;	//クリップボード容量制限
};
//---------------------------------------------------
// キー設定構造体
//---------------------------------------------------
struct OPTION_KEYSET
{
	int  m_nHistoryLimit;	//クリップボード容量制限
	COPYPASTE_KEY	m_defKeySet;
	list<CHANGE_KEY> m_KeyList;//キー設定データリスト
};

//---------------------------------------------------
// マクロ設定構造体
//---------------------------------------------------
struct MACRO_STRUCT
{
	int  m_cKind;
	CString m_strName;
	CString m_strMacro;
};

//---------------------------------------------------
// クラス定義
//---------------------------------------------------
class CInit  
{
public:
	CInit();
	virtual ~CInit();
	void initialize();
	void setIniFileName(CString strFileName) {
		m_strIniFile = strFileName;
	}

	void readAllInitData();
	CString setIniFileString(const TCHAR* szSection,const TCHAR* szKey,CString strDefault);
	int setIniFileInt(const TCHAR* szSection,const TCHAR* szKey,int nDefault);

	void writeAllInitData();
	void writeEnvInitData();
	void writeProfileInt(const TCHAR* szSection,const TCHAR* szKey,int nValue);
	void writeProfileString(const TCHAR* szection,const TCHAR* szKey,CString strValue);

	COPYPASTE_KEY getAppendKeyInit(CString strWinName,int nNumber = 0);
	CHANGE_KEY getAppendKeyInit2(CString strWinName);
	void setHotkey(UINT uPopKey,UINT uPopMod,UINT uFifoKey,UINT uFifoMod);
	void getHotKey(UINT *uPopKey,UINT *uPopMod,UINT *uFifoKey,UINT *uFifoMod);
	void setPasteHotkey(UINT uKey,UINT uMod,UINT uCopyKey,UINT uCopyMod);
	void getPasteHotKey(UINT *uKey,UINT *uMod,UINT *uCopyKey,UINT *uCopyMod);

	bool setHookKey(HWND hWnd);
	void unHookKey();

	//OSの種族判別(NT40はfalse)
	bool isNotNt40() {
		// Windows 9x and its variants are no longer supported.
		return false;
	}
	bool isNt40() {
		// Assume running on Windows NT 4.0 or later.
		return true;
	}
	bool isWindows98() {
		// Windows 9x and its variants are no longer supported.
		return false;
	}

	//基本情報
	CString m_strAppPath;	//実行パス
	CString m_strUserName;	//ユーザー名
	CString m_strIniFile;	//iniファイル名
	CString m_strDataFile;	//データファイル名
	CString m_strRwPluginFolder;//プラグインフォルダ
	CString m_strPluginName;//フォーマット名
	CString m_strMacroPluginName;//マクロ形式名
	CString m_strBBS;//サポートBBSURL
	HKL     m_keyLayout;

	int  m_nSelectID;		//選択してるID
	int  m_nTreeID;
	int  m_nOptionPage;
	int  m_nRecNumber;
	int  m_nToolTipTime,m_nToolTipDelay;
	POINT m_DialogSize,m_IconSize;	//ダイアログのサイズ

	int  m_nSerchKind;		//検索種別
	int  m_nSerchLogic;		//検索方法
	CString m_strSerchKey;//検索キー

	int m_nDebug;
	CString m_strDebugLog;

	vector<MACRO_STRUCT>  m_vctMacro;
	vector<MACRO_STRUCT>  m_vctDataMacro;

	//設定情報
	OPTION_ETC		m_etc;
	OPTION_POPUP	m_pop;
	OPTION_FIFO		m_fifo;
	OPTION_VISUAL	m_visual;
	OPTION_KEYSET	m_key;

	HMODULE m_hHookDLL;
};

#endif // !defined(AFX_INIT_H__EA6EFDDA_9745_4690_B6A9_8B0533E4AB56__INCLUDED_)
