
#include <list>
using namespace std;

//データ種別
#define KIND_ONETIME 0x01	//ワンタイム項目
#define KIND_LOCK    0x02	//ノーマル項目
#define KIND_FOLDER  0x04	//フォルダ項目
#define KIND_RIREKI  0x08	//履歴フォルダ

#define KIND_DATE	1	//日付
#define KIND_EXE    2	//実行
#define KIND_RELATE 3	//関連付け
#define KIND_SELECT 4	//選択
#define KIND_CLIP   5	//クリップボード
#define KIND_PLUG   6   //プラグイン
#define KIND_KEY    7   //キーマクロ

//アイコン番号
#define ICON_FOLDER	0
#define ICON_RIREKI_FOLDER 2

#define ICON_KEY	4
#define ICON_ONETIME 6
#define ICON_DATE	8
#define ICON_EXE	10
#define ICON_RELATE	12
#define ICON_SELECT	14
#define ICON_CLIP	16
#define ICON_PLUG	18
#define ICON_KEYM	20

#define ROOT -1

struct STRING_DATA
{
	char	m_cKIND;		//データ種別
	char	m_cIcon;		//アイコン種別
	int		m_nMyID;		//データのID
	int		m_nParentID;	//親データのID
	time_t	m_timeCreate;	//作成日時
	time_t	m_timeEdit;		//変更日時
	CString  m_strTitle;	//設定データタイトル
	CString  m_strData;		//設定データ文字列
	CString  m_strMacro;		//拡張用文字列データ
};

