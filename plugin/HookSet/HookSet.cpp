
#include "StdAfx.h"

#pragma data_seg(".shared")

static HHOOK hNextHook = NULL;
static HWND  hCallWnd = NULL;
static int   nMessageId = 0;
static int   nPrevKey = 0;

#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

HINSTANCE hInstanceDLL = 0;


int WINAPI DllMain(HINSTANCE hInstance, DWORD dwNotification, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpReserved);

	hInstanceDLL = hInstance;
	return TRUE;
}


LRESULT CALLBACK keyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0 || nCode == HC_NOREMOVE)
		return ::CallNextHookEx(hNextHook, nCode, wParam, lParam);

	if(nCode >= 0 && lParam & 0x80000000/*離された*/ && lParam & 0x40000000/*最初のキー入力*/) {
		if(wParam == VK_SHIFT || wParam == VK_CONTROL || wParam == VK_MENU)
			PostMessage(hCallWnd, nMessageId, wParam, lParam);//メッセージをポスト
	}
	if(nCode >= 0) {
		return CallNextHookEx(hNextHook, nCode, wParam, lParam);
	}
	return 0;
}


extern "C" __declspec(dllexport) bool SetHook(const HWND hWnd, const int msg)
{
	bool isRet = false;

	hNextHook = SetWindowsHookEx(WH_KEYBOARD,keyHookProc,hInstanceDLL,0);//登録
	hCallWnd = hWnd;
	nMessageId = msg;
	if(hNextHook == NULL) {
		return isRet;
	}
	isRet = true;
	return isRet;
}

extern "C" __declspec(dllexport) void  UnHook(void)
{
	if(hNextHook != NULL) {
		UnhookWindowsHookEx(hNextHook);
	}
}
