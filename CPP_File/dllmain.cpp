// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include <map>
#include <tchar.h>
#include <ctime>
#include <process.h>
#include "InjectBot.h"
#include "Address_Game.h"
#include "GameData.h"
#include "..\\LoLBot\\Resource.h"
#include <math.h>
#include "Utils.h"
#include "Server.h"
#include "SpellData.h"
#include "GameDefine.h"
#include "Dodge.h"
#include "AntiDebugger.h"
#include "Combo/Combo.h"
#include "Farm_Level.h"
#include "LastHitPro.h"
#include "Draw/Drawing.h"
#include "HotKey.h"
#include "Activator.h"
#include "OrbWalker/OrbSafe.h"
#include "OrbWalker/OrbWalker.h"
#include "OrbWalker/OrbT_LHit.h"
#include "OrbWalker/TargetSelector.h"
#include "OrbWalker/Prediction.h"
#include "OrbWalker/Dash.h"
#include "OrbWalker/WayPointAnalysis.h"

#include <GUIMenu/imgui.h>
#include "GUIMenu/imgui_impl_dx9.h"

//#include "AntiBan.h"

//void MOVE_TO(float posX, float posY);

#include "SDK/CORE_API.h"

#include "fucker.h"
#include "ezMenu.h"
#include "DelayAction.h"
#include "VMTHooked.h"

#include "EventFake.h"

extern DelayAction*		_DelayAction;
using namespace std;

//int SoLanRecheck_Login = 0;	// recheck 3 lan neu ko thanh cong

HWND LOL_HWND = 0;
HINSTANCE			hDll;
static TCHAR g_strPath2DLL_Game[MAX_PATH];
const UINT WM_HOOK_WRITE = RegisterWindowMessage(_T("WM_HOOK_WRITE"));
const UINT WM_HOOKEX = RegisterWindowMessage(_T("WM_HOOKEX_RK"));

const UINT WM_HOOKEX_BENTR = RegisterWindowMessage(_T("WM_HOOKEX_BENTR")); //--dang ky



UINT_PTR	nTimerID = 0;
UINT_PTR	nTimerID_222 = 0;

BOOL bHooked = 0;
bool g_bHarass = 0;
bool g_bNormalAttackHero = false;
bool g_bCheckKeyPress = false;
bool g_bPressingKey_S = false;
bool g_bPressingKey_T = false;
bool g_bPressingKey_E = false;
bool g_bPressingKey_A = false;
bool g_bPressingKey_So3 = false;
//bool g_bPressingKey_F8 = false;
bool bPressingSPACE = false;
bool bIs_Chatting = false;
bool IS_BAT_R = true;
bool IS_R_VeBeDa = true;
bool g_isUsedFlash = false;
bool g_isDrawing;
bool g_isDraw_CircleAtMousePos = false;
bool onOff_Detour_Drawing = false;
bool nReset_DanhThuong = false;
float nTimePressingSPACE = 0.0;
bool g_bDodge = 1;
bool g_IsDodging = false;
bool g_bClearLane = false;
bool g_bPressingX = false;
bool g_bMove2MousePos = true;
float g_tDelay = 0.;
float g_DelayNormalAttack = 0;
float tDelayToAttack = 0.;
bool bFlagDelay = false;
bool nIs_DanhVoiMay = false;
bool nIs_EndLua     = false;
bool nIs_PushLane    = false;
bool nIsAlreadyLoadLua= false;


int   myTeamId = 0;
DWORD G_Module = 0;
KNpc* MY_HERO  = 0;

DWORD init_fake_addr = 0;
DWORD initFake_addr = 0;

bool nIs_DrawMenu	= false;
//KBuffer_Spell g_BufferSpell_2[MAX_ARRAY_BUFFER_SPELL];
//KBuffer_Spell g_BufferSpell[MAX_ARRAY_BUFFER_SPELL];
//KBuffer_Spell g_BaseTimeSpell[MAX_ARRAY_BUFFER_SPELL];
//KOptionMenuCustomLua g_OptMenuCustom[MAX_ARRAY_OPTION_MENU];
KLoadScripts g_LoadScript[MAX_SCRIPT_RUN];

int nComboHotKey	 = 0x20;	// default = VK_SPACE
int nLaneClearHotKey = 0x56;	// default = V
int nHarassHotKey    = 0x43;	// default = C
int nLastHitHotKey   = 0x58;	// default = X

bool nCombo_Mode	 = false;
bool nLaneClear_Mode = false;
bool nLastHit_Mode	 = false;
bool nFreeze_Mode	 = false;
bool nMixed_Mode	 = false;//thap cam mode

bool nPressDeleteKey = false;
bool lua_exit		 = false;

KLanguage*	 pLang;
CIniFile* menu_IniFile;
char path_out_ini[MAX_PATH]  = "";

WNDPROC				OldWndProc = NULL;
WNDPROC				OldWndProc_222 = NULL;
void init_fake();
//int UnmapDll_Real(HWND hWnd);
LRESULT CALLBACK	NewWndProc_Real(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK	NewWndProc_Real_222(HWND,UINT,WPARAM,LPARAM);
VOID CALLBACK		TimerProc_Real(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK		TimerProc_Real_222(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
#define pCW ((CWPSTRUCT*)lParam)

#define BUFSIZE 1024


//------ bien global cho Lua -------------------------------------------------------
bool IsEvade_Lua		 = false;
bool IsPrintErrorLog_Lua = false;

std::map<int,std::vector<StoredPath>*> *StoredPaths    = 0; 
std::map<int,std::vector<DashItem>*>   *DetectedDashes = 0;
//==================================================================================//
std::vector<DWORD>				*badPointer_Unit = 0;
GameObjectIssueOrderEventArgs	*ArgsMove		 = 0;

extern bool DodgeSkillShots_menu;

void write_temp_file(char* buffer)
{
	DWORD dwRetVal = 0;

	TCHAR lpTempPathBuffer[MAX_PATH];
	TCHAR szTempFileName[MAX_PATH] = "";  

	HANDLE hTempFile = INVALID_HANDLE_VALUE; 

	BOOL fSuccess  = FALSE;

	DWORD dwBytesWritten = 0;

	dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
		lpTempPathBuffer); // buffer for path 

	//__oMsg("lpTempPathBuffer: %s",lpTempPathBuffer);

	strcat_s(szTempFileName,lpTempPathBuffer);
	strcat_s(szTempFileName,TEMP_FILE_NAME);

	//__oMsg("szTempFileName: %s",szTempFileName);

	hTempFile = CreateFile((LPTSTR) szTempFileName, // file name 
		GENERIC_WRITE,        // open for write 
		0,                    // do not share 
		NULL,                 // default security 
		CREATE_ALWAYS,        // overwrite existing
		FILE_ATTRIBUTE_NORMAL,// normal file 
		NULL);                // no template

	fSuccess = WriteFile(hTempFile, 
		buffer, 
		strlen(buffer),
		&dwBytesWritten, 
		NULL); 

	//__oMsg("fSuccess: %d",fSuccess);

	CloseHandle(hTempFile);
}

//===============================================================================================================//
//===============================================================================================================//
//BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
//{
//	//int iii = 0;
//	//int kkk = 0;
//	//if(iii == 0) kkk = 1;
//	//if(kkk == 1) iii = 2;
////VMProtectBegin("DLL_VM__DllMain_xxx");
//	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
//	{
//		//__oMsg("dllmain: hModule=%x", hModule);
//		hDll = (HINSTANCE) hModule;
//		DisableThreadLibraryCalls(hDll);
//	}
////VMProtectEnd();
//	return TRUE;
//}


//BOOL APIENTRY DllMain( HMODULE hModule,
//					  DWORD  ul_reason_for_call,
//					  LPVOID lpReserved
//					  )
//{
//	VMProtectBegin("DLL_VM__DllMain_xxx");
//	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
//	{
//		hDll = (HINSTANCE) hModule;
//		DisableThreadLibraryCalls(hDll);
//		//OutputDebugString(".");
//
//		__oMsg("FUCKING...: DllMain-1");
//		HWND hLolWnd = ::FindWindow(sz__RiotWindowClass, NULL);
//		OldWndProc = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real);		
//		__oMsg("FUCKING...: DllMain-2");
//
//		GetModuleFileName(hDll, g_strPath2DLL_Game, MAX_PATH);
//
//		__oMsg("(DllMain)[%x], g_strPath2DLL_Game: %s", hDll, g_strPath2DLL_Game);
//		//if (strlen(g_strPath2DLL_Game) > 0)
//		//{
//		//	//
//		//	////__oMsg("g_strPath2DLL_Game(>0) %s",g_strPath2DLL_Game);
//
//		//	//char sz__fdC_Y_yum3ww09[] = { 102, 100, 67, 41, 89, 37, 121, 117, 109, 51, 119, 119, 48, 57, 0 };
//		//	//TCHAR* Data = g_strPath2DLL_Game;
//		//	//TCHAR* Key = sz__fdC_Y_yum3ww09;
//		//	////__oMsg("len1: %d [%s]", _tcslen(Data), Data);
//		//	//Crypt(Data, _tcslen(Data), Key, _tcslen(Key));
//		//	////__oMsg("len2: %d [%s]", _tcslen(Data), Data);
//		//	//write_temp_file(Data);
//		//}
//		//else
//		//{
//		//	//strcat(g_strPath2DLL_Game, "abc");
//		//}
//	}
//	VMProtectEnd();
//	return TRUE;
//}


//===============================================================================================================//
//extern DelayAction* _DelayAction;
void Test_DelayAction()
{
	//__oMsg("-----: %f", (float)GetTickCount());
	//Cast2Pos(&GetPlayer()->currentPos(), _W);
}

//===============================================================================================================//
bool CheckName(char *findSubString, char *inString)
{
	string strInString(inString);
	int found = strInString.find(findSubString);
	if (found >= 0)
		return true;
	return false;
}

//===============================================================================================================//
bool CheckNameEx(char *sz1, char *sz2)
{
	if(CheckName(sz1, sz2) && CheckName(sz2, sz1)) return true;
	return false;
}

//===============================================================================================================//
extern bool b_Vladimir_DoingE;
extern bool Lucian_R_doing;
extern float nCoolDownAA_Chuan;

void WINAPI AUTO_DODGE()
{
	//// ......
}


////----------------------------//-------------------------------------:
//#include <iostream>
//#include <fstream>
//#include <conio.h>
//#include <stdio.h>
//
//bool __oMsg_2(const char * format, ...)
//{
//	AllocConsole();
//	char buffer[1000];
//
//	va_list argptr;
//	va_start(argptr, format);
//	vsprintf_s(buffer, format, argptr);
//	va_end(argptr);
//	strcat_s(buffer, "\n");
//
//	freopen("CONOUT$", "w", stdout);
//	std::cout << buffer ;
//
//	return true;
//}

//----------------------------//-------------------------------------:
DWORD _pDevice = 0;
DWORD beginScene_Org = 0;
void __declspec(naked) BeginScene_Fake()
{
	__asm
	{
		push eax
		mov eax, dword ptr [esp + 8]
		mov _pDevice, eax
		pop eax 
	} 

	__asm pushad 

	if (beginScene_Org == 0)
	{
		DWORD* vtbl = 0;
		DWORD table = FindPattern((DWORD)GetModuleHandle(sz__d3d9_dll), 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", sz__xx____xx____xx);
		if (table != 0)
		{
			memcpy(&vtbl, (void*)(table + 2), 4);
			beginScene_Org = vtbl[41];
		}

		//__oMsg_2(" -------------- table: %x, vtbl: %x, beginScene_Org: %x", table, vtbl, beginScene_Org);
		if (beginScene_Org)
		{
			init_fake();
		}
	}
	//__oMsg_2(" -------------- CLGT");

	__asm popad


	__asm
	{
			push _pDevice
			call beginScene_Org
			retn 0x4
	} 
}

//==================================================================================//
void init_fake()
{
VMProtectBegin("DLL_VM__init_fake");
	//nFake = false;
	//__oMsg("FUCKING...: init_fake-1");
	HWND hLolWnd = ::FindWindow(sz__RiotWindowClass, NULL);
	OldWndProc = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real);		
	//__oMsg("FUCKING...: init_fake-2");
	//GetModuleFileName(hDll, g_strPath2DLL_Game, MAX_PATH);

	//__oMsg("(DllMain)[%x], g_strPath2DLL_Game: %s", hDll, g_strPath2DLL_Game);
	if (strlen(g_strPath2DLL_Game) > 0){

		//__oMsg("g_strPath2DLL_Game(>0) %s",g_strPath2DLL_Game);

		char sz__fdC_Y_yum3ww09[] = { 102, 100, 67, 41, 89, 37, 121, 117, 109, 51, 119, 119, 48, 57, 0 };
		TCHAR* Data = g_strPath2DLL_Game;
		TCHAR* Key = sz__fdC_Y_yum3ww09;
		//__oMsg("len1: %d [%s]", _tcslen(Data), Data);
		//Crypt(Data, _tcslen(Data), Key, _tcslen(Key));
		//__oMsg("len2: %d [%s]", _tcslen(Data), Data);
		//write_temp_file(Data);
	}
	else{
		//strcat(g_strPath2DLL_Game, "abc");
	}
VMProtectEnd();
}

//==================================================================================//
HANDLE AttachToProcess(DWORD procID) 
{
	OSVERSIONINFO osver; 
	// SetDebugPrivileges SE_DEBUG_NAME
	//printf("[+] Setting Debug Privileges [%d]\n", SetDebugPrivileges());
	//SetDebugPrivileges();
	VMProtectBegin("Launcer_VM__AttachToProcess");
	osver.dwOSVersionInfoSize = sizeof(osver);
	if (GetVersionEx(&osver)) {	
		if (osver.dwMajorVersion == 5) {
			//printf("\t[+] Detected Windows XP\n");
			return OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, 0, procID );
		}
		if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0) {
			//printf("\t[+] Detected Windows Vista\n");
			return NULL;
		}
		if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)	{
			//printf("\t[+] Detected Windows 7\n");
			//printf("\t[+] Attaching to Process ID: %d\n", procID);
			return OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, procID );
		}
		return OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, procID);
	} else {
		//printf("\n[!] Could not detect OS version\n");
	}
	VMProtectEnd();
	return NULL;
}

//==================================================================================//
LRESULT HookProc_Real_222 (int nCode, WPARAM wParam, LPARAM lParam)
{
VMProtectBegin("DLL_VM__HookProc_Real_222");//VM choi choi
	
	//__oMsg("-----------bHooked: %d", bHooked);
	HWND hLolWnd = pCW->hwnd;
	HHOOK hHook = (HHOOK)pCW->wParam;
	if((pCW->message == WM_HOOKEX) && pCW->lParam)
	{
		UnhookWindowsHookEx(hHook);
		if (bHooked)
			goto END;

		//__oMsg("----> in [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
		static bool isTriggeredOnce = false;
		if (isTriggeredOnce == false)
		{
			GetModuleFileName(hDll, g_strPath2DLL_Game, MAX_PATH);
			isTriggeredOnce = true;
		}
		//__oMsg("lib dll path:%s , hDllxx=%x", g_strPath2DLL_Game, hDll);
		if(!LoadLibrary(g_strPath2DLL_Game))
			goto END;

		// CODE HIDE DLL DE O DAY
	
		OldWndProc_222 = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real_222);
		if(OldWndProc_222==NULL) {
			//__oMsg("OldWndProc_222==>>>>NULL");
			FreeLibrary(hDll);
		}
		else
		{
			//__oMsg("bHooked ==>> TRUE : hLolWnd=%x", hLolWnd);
			extern HWND g_hWinDLL;
			if(g_hWinDLL == 0) g_hWinDLL = hLolWnd;

			bHooked = TRUE;

		}
	}
	else if(pCW->message == WM_HOOKEX) 
	{
		UnhookWindowsHookEx(hHook);
		if (!bHooked)
			goto END;
		if(!SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)OldWndProc_222))
			goto END;

		if(initFake_addr != 0)
		{
			__asm
			{
				mov eax, initFake_addr
				call eax
				//add esp, 4
			}
			//__oMsg("VKL------VKL-----------");
		}
		//Sleep(200);

		//__oMsg("FreeLibrary---111: %x, OldWndProc_222 = %x", hDll, OldWndProc_222);
		FreeLibrary(hDll);
		//__oMsg("FreeLibrary---222: %x", hDll);
		if (nTimerID_222)
			KillTimer(NULL, nTimerID_222);
		bHooked = FALSE;
		//Retour_EndScene();
	}
	if(pCW->message == WM_HOOKEX_BENTR)
	{
		//__oMsg("WM_HOOKEX_BENTR: %x", pCW->lParam);
		UnhookWindowsHookEx(hHook);
	}
VMProtectEnd();
END:
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

//==================================================================================//
LRESULT HookProc_Real(int nCode, WPARAM wParam, LPARAM lParam)
{
	VMProtectBegin("DLL_VM__HookProc_Real");//VM choi choi

	//__oMsg("-----------bHooked: %d", bHooked);
	HWND hLolWnd = pCW->hwnd;
	HHOOK hHook = (HHOOK)pCW->wParam;
	if((pCW->message == WM_HOOKEX) && pCW->lParam)
	{
		UnhookWindowsHookEx(hHook);
		if (bHooked)
			goto END;

		//__oMsg("----> in [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
		static bool isTriggeredOnce = false;
		if (isTriggeredOnce == false)
		{
			GetModuleFileName(hDll, g_strPath2DLL_Game, MAX_PATH);
			isTriggeredOnce = true;
		}
		//__oMsg("[HookProc_Real]lib dll path:%s , hDllxx=%x", g_strPath2DLL_Game, hDll);
		if(!LoadLibrary(g_strPath2DLL_Game))
			goto END;

		// CODE HIDE DLL DE O DAY

		OldWndProc = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real);
		if(OldWndProc==NULL) {
			//__oMsg("OldWndProc_222==>>>>NULL");
			FreeLibrary(hDll);
		}
		else
		{
			//__oMsg("bHooked ==>> TRUE : hLolWnd=%x", hLolWnd);
			extern HWND g_hWinDLL;
			if(g_hWinDLL == 0) g_hWinDLL = hLolWnd;

			bHooked = TRUE;

		}
	}
	else if(pCW->message == WM_HOOKEX) 
	{
		UnhookWindowsHookEx(hHook);
		if (!bHooked)
			goto END;
		if(!SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)OldWndProc))
			goto END;
		//__oMsg("FreeLibrary---1: %x", hDll);
		FreeLibrary(hDll);
		//__oMsg("FreeLibrary---2: %x", hDll);
		if (nTimerID)
			KillTimer(NULL, nTimerID);
		bHooked = FALSE;
		//Retour_VMT_ALL();
		Retour_EndScene();
	}
VMProtectEnd();
END:
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

//==================================================================================//
extern HWND g_hWinDLL;
extern bool g_bCheckConnectDLL;
extern bool g_bDoingE_Camille;

//lua 
HANDLE hThreadLUA = 0;
int LUA_KEY_CODE = 0;
//end lua


extern int nScripsLoaded;
bool bIsLuaLoaded = false;

//------------------------------
LRESULT CALLBACK NewWndProc_Real(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//-------- CODE 4 LINH TINH --------:
	if(nPressDeleteKey)
	{
		Retour_OnProcessSpell(); Retour_EndScene(); Retour_OnCreateObj(); Retour_OnDeleteObj(); Retour_OnUpdateBuff(); Retour_OnRemoveBuff(); Retour_OnPlayAnimation(); Retour_OnDoCast(); Retour_OnIssueOrder(); Retour_OnNewPath();		//Retour_OnStopCast(); //Retour_OnProcessPacket();
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);	// return 0 la deo ha man` hinh xuong dc
	}
	
VMProtectBegin("DLL_VM__SetValueInit");

	if((DWORD)hWnd == 0x33)// doan nay k bgio vao, chi de lay code
	{
		BeginScene_Fake();
		init_fake();
		return 0;
	}
	//--------//----------------//----------------//--------------------------------|--||


	//-------- INIT LINH TINH --------:

	extern bool g_Exit;
	if(g_Exit == true)
	{
		Retour_EndScene();
		CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
	}

	if (!nTimerID)
	{
		if (ArgsMove == 0)		  ArgsMove = new GameObjectIssueOrderEventArgs;
		if (badPointer_Unit == 0) badPointer_Unit = new std::vector<DWORD>;

		if (!LOL_HWND)		 LOL_HWND		= FindWindow(sz__RiotWindowClass, NULL); // cho may ham isWindowTop ...
		if (!StoredPaths)	 StoredPaths    = new std::map<int,std::vector<StoredPath>*>;
		if (!DetectedDashes) DetectedDashes = new std::map<int,std::vector<DashItem>*>;
		
		G_Module     = (DWORD)MyGetModuleHandle(sz__League_of_Legends_exe);
		GetIniMenuPath(path_out_ini);
		pLang		 = new KLanguage;
		menu_IniFile = new CIniFile(path_out_ini);

		nTimerID = SetTimer(NULL, NULL, SYS_TIMER, (TIMERPROC)TimerProc_Real);	// de SetTimer() o day van Eject duoc(chu y retour_endscene khi Eject)
		//__oMsg("G_Module(dang cach cu~) = %x", G_Module);

		Detour_VMT_ALL();	//detour SendReivPacket
	}

	if (!IsGameReady())
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);

	//------- CHECK LOGIN NOTYET DONE --------:
	if (CHECK_HACK_BB_1())
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);

	if (MY_HERO == 0)
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);

VMProtectEnd();
//--------//----------------//----------------//--------------------------------|--||


	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);


	////-------- LUA RUN ON WND MSG --------:
	////for (int i = 0; i < MAX_SCRIPT_RUN; i++) { /* if(g_LoadScript[i].bIsRunning) { LuaRunOnWndMsg(g_LoadScript[i].LState, g_LoadScript[i].szPathFile, uMsg, wParam); } */ }

	if (uMsg == 513) SetTargetSelected();
	//--------//----------------//----------------//--------------------------------|--||
	

	//-------- ON/OFF BINDING --------:
	func_onOff_binding(hWnd, uMsg, wParam, lParam);
	//--------//----------------//----------------//--------------------------------|--||


	//-------- LAU CAM LON THAP CAM --------:
	LINH_TINH_FUNC(hWnd, uMsg, wParam, lParam);
	//--------//----------------//----------------//--------------------------------|--||


	
	//-------- PRESS SOME SHITs --------:
	switch (uMsg)
	{
	case WM_KEYDOWN:
		LUA_KEY_CODE = (int)wParam;
		break;
	
	case WM_KEYUP:
		LUA_KEY_CODE = 0;
		if (wParam == VK_HOME) // VK_HOME	0x24	HOME key		phim Home duoc tha ra : goi nuttest
		{
			

			//extern float xxx;
			//xxx += 1;
			//__oMsg("xxxxx = %f", xxx);
			//_DelayAction->Add(3000, Test_DelayAction);
			TestTest();

		}
		if (wParam == VK_END) // VK_END	0x23	End key		phim End duoc tha ra : goi unmapDll
		{
			//Retour_OnGetDDSNewFunc();
			//Retour_OnGetDDS__DANG_VE();
			//Retour_OnIssueOrder();

			//FreeConsole();
			//Retour_VMT_ALL();
			//UnmapDll_Real(g_hWinDLL);
			//SEND_TEXT_SYSTEM(" Ejected ");
			////__oMsg("DELETE LUA ---------------------------------------------> EJECTED");

			//LuaScript_LON_1();
			//extern bool switch_key;
			//switch_key = !switch_key;
		}

		if (wParam == VK_DELETE)
		{
			//extern std::vector<std::string> fuckk_str;
			//extern int rengar_ring;

			//if (test_index < fuckk_str.size())
			//{
			//	//rengar_ring = 0;
			//	rengar_ring = GetTexture_Game((char*)fuckk_str[test_index].c_str());
			//	test_index += 1;

			//	__oMsg("-----Testing_Texture: %x - %s", rengar_ring, fuckk_str[test_index].c_str());
			//}
			//else
			//{
			//	__oMsg("------------ENENENEEEEEEEEEE-------------------");
			//}
			
		}

		if (wParam == VK_SHIFT)
		{
			nIs_DrawMenu = false;
		}
		
		break;
	}
	//--------//----------------//----------------//--------------------------------|--||

	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}


//==================================================================================//
//==================================================================================//
extern bool g_bDoingR_Xerath, g_bDoingE_Pan, g_bDoingR_MissFortune, g_bDoingR_Katarina, g_bDoingR_Jhin, g_bDoingQ_Vayne;
extern bool g_bDoingR_Velkoz, g_bDoingR_Jana, g_bDoingR_Nunu, g_bDoingR_Malzahar, g_bDoingW_Yi;

//======================================================================================================
VOID CALLBACK TimerProc_Real(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	//Test_misile();

	//Detour_OnGetDDSNewFunc();
	//Detour_OnGetDDS__DANG_VE();
	//Detour_OnIssueOrder();

	//-------- CHECK HACK-CheckLogin --------:	
VMProtectBegin("DLL_VM__TimerProc_Real_1");	

	CHECK_HACK_AA_1();
VMProtectEnd();
	//--------//----------------//----------------//--------------------------------|--||
	
    //-------- Pressed DELETE-KEY -----------------:
	if (nPressDeleteKey)
	{
		Retour_OnProcessSpell(); Retour_EndScene(); Retour_OnCreateObj(); Retour_OnDeleteObj(); Retour_OnUpdateBuff(); Retour_OnRemoveBuff(); Retour_OnPlayAnimation(); Retour_OnDoCast(); Retour_OnIssueOrder(); Retour_OnNewPath();	        //Retour_OnStopCast(); //Retour_OnProcessPacket();
		return;
	}
	//--------//----------------//----------------//--------------------------------|--||
	

	//-------- CHECK FAKE-SERVER -----------------:
VMProtectBegin("DLL_VM__TimerProc_Real_2");

	if (!IsGameReady())    return;
	if (CHECK_HACK_AA_2()) return;
VMProtectEnd();
	//--------//----------------//----------------//--------------------------------|--||
		
	MY_HERO = GetPlayer();
	if (MY_HERO == 0) return;

	
	//--------------------: (myTeamId)
	static UINT nUsingTimer = 0;
	nUsingTimer++;
	if (nUsingTimer == 0 || myTeamId == 0)
	{   
		myTeamId = MY_HERO->nTeam_ID;
	}

	//-------- CHECK HACK-CheckLogin(2) ----------:	
//VMProtectBegin("DLL_VM__TimerProc_Real_3");

	if (CHECK_HACK_AA_3()) return;

	//--------------------: (detour EndScene + set Priority)
	if(nUsingTimer == 5)
	{
		//------------ Welcome to Riot  !!! ---------------
		char szTemp[] = {45,45,45,45,45,45,45,45,45,45,45,45,32,87,101,108,99,111,109,101,32,116,111,32,82,105,111,116,32,32,33,33,33,32,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,0};
		SEND_TEXT_SYSTEM(szTemp);
		Detour_EndScene();	
		//InitTrackerClass();
		Set_TS_Prio();
		//Detour_OnProcessSpell(); Detour_OnCreateObj(); Detour_OnDeleteObj(); Detour_OnUpdateBuff(); Detour_OnRemoveBuff(); //Detour_OnPlayAnimation(); Detour_OnDoCast(); Detour_OnIssueOrder(); Detour_OnNewPath(); //--//Detour_OnStopCast();   Detour_OnProcessPacket();
		
	}
VMProtectEnd();
	//--------//----------------//----------------//--------------------------------|--||

	
	//Detour_VMT_ALL();

	//-------- SOME MAIN-FUNCTIONS (1) -----------:
	if(nUsingTimer > 5)	
	{
		OnNewPath_EventFake_InGameUpdate();
		//////OnDeleteObject_EventFake_InGameUpdate();	// *** CACH NAY KO ON? LAM ******

		OnGameUpdate_ezEvade();
		
	}
	
	IsCastingBASafe();				//co RS-AA trong nay roi

	RemoveAttacker_2();
	Set_ActiveAttacks_Safe();
	SetMelee_Processed_Safe();
	SetRanged_Processed_Safe();
	
	if (nUsingTimer >= 15 && bIsLuaLoaded) { /*for(int i = 0; i < MAX_SCRIPT_RUN; i++) { if(g_LoadScript[i].bIsRunning) { LuaRunScript(g_LoadScript[i].LState, g_LoadScript[i].szPathFile, "OnTick"); } }*/ }
	
	//--------------------: (setting hotKey)
	if (nUsingTimer >= 20)
	{
		nComboHotKey     = pLang->comboKeyCode;		
		nLaneClearHotKey = pLang->laneClearKeyCode;		
		nHarassHotKey    = pLang->harassKeyCode;	  
		nLastHitHotKey   = pLang->lastHitKeyCode;
	}
	//--------------------: (Smite + init Evade-Menu)
	if (nUsingTimer == 12)
	{
		Smite_MenuValueDefault ();
		Mikael_MenuValueDefault();

		Init_ezEvade_Menu();		
	}
	
	//--------------------: (Anti-AFK)
	if ( pLang->isAntiAFK ) 
	{
		anti_AFK();
	}
	
	//--------------------: (MOD-SKIN)
	if ( pLang->isModSkin )
	{
		ModSkin();
	}

	
	UpdateConfigIni();
	//--------//----------------//----------------//--------------------------------|--||


	//-------- CHECK-LOGIN-MAIN ------------------:
VMProtectBegin("DLL_VM__PRE_CheckLogin");

	extern int hackChecklogin;  
	extern DWORD g_Address; 
	extern bool g_Exit;

	static bool s_isFirstCheck = true;
	static UINT nTime2CheckLogin = 0;
	nTime2CheckLogin += SYS_TIMER;
	
	if (nTime2CheckLogin > 50 && s_isFirstCheck) 
	{
		hackChecklogin = 1;

		s_isFirstCheck = false;
		CheckLogin(0);
		//HANDLE hThread;
		//DWORD nThread_ID = 0;
		//hThread = CreateThread(NULL,NULL,&CheckLogin,NULL,NULL,&nThread_ID);
		//if (hThread == 0 || nThread_ID == 0)
		//{
		//	//UnmapDll_Real(g_hWinDLL);
		//	char szTemp[] = {67,114,97,99,107,105,110,103,0};	// "Cracking"
		//	SEND_TEXT_SYSTEM(szTemp);
		//	
		//	g_Exit = true;
		//	g_Address = 0x2d2a1; 
		//	return;
		//}

		//typedef NTSTATUS (NTAPI *pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG); 
		//NTSTATUS Status; 
		//pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandle( TEXT("ntdll.dll")), "NtSetInformationThread");
		//if (NtSIT)
		//{
		//	Status = NtSIT(hThread, 0x11, 0, 0); 
		//}
		//CloseHandle(hThread);
		nTime2CheckLogin = 0;
	}

	//----------------
	if (CHECK_HACK_AA_4()) return;
VMProtectEnd();
	//--------//----------------//----------------//--------------------------------|--||


	//-------- SOME MAIN-FUNCTIONS (2) -----------:
	if (bPressingSPACE)
		nTimePressingSPACE += SYS_TIMER;

	
	//__oMsg("0000");
	LogicSmiteJungle(MY_HERO);			// co 1 try_except

	//__oMsg("1111");
	Mikael_update(MY_HERO);

	//__oMsg("2222");
	OnTickComboCore();

	//__oMsg("3333");
	OnTickOrbwalkCore();

	//__oMsg("4444");
	OnAfterAttackCore();
	//__oMsg("5555");

	ActivatorAllItem();					//chua check lai, se check sau
	//--------//----------------//----------------//--------------------------------|--||
	//__oMsg("6666");


	//-------- OLD-EVADING -----------:
	//if (pLang->isEvade && g_bDoingR_Xerath == false && !g_bDoingR_MissFortune && !g_bDoingR_Katarina && !g_bDoingR_Jhin && !g_bDoingR_Jana && !g_bDoingR_Nunu && !g_bDoingW_Yi)
	//{
	//	VERY_OLD_EVADING();
	//}
	//--------//----------------//----------------//--------------------------------|--||
	//-------- XXX-XXXXX -----------:
	//__try
	//{
	//	if(pLang->isTargetSelector_old /*SLMHTClientData::GetInstance()->GetCBAtkCurrentSelected()*/)
	//		DISABLE_SETECTOR();
	//
	//	ActivatorAllItem();
	//	//////AUTO_KETLIEU();
	//}
	//__except(filterException(GetExceptionCode(), GetExceptionInformation())) { SEND_TEXT_SYSTEM("Activator"); }
	//--------//----------------//----------------//--------------------------------|--||


	g_tDelay += SYS_TIMER;
	g_DelayNormalAttack += SYS_TIMER;
}

//-------------------------------------------------------------------------------- :
int shiter(DWORD hWnd, DWORD a1, DWORD a2)//ham Export -> doi lai ten
{
VMProtectBegin("Injecting_dll");

	//if(xxx != 0x209) return 0;
	//HWND appCuaMinh = FindWindowA("#32770", "Risker");
	//__oMsg("[START-INJECT] ----> appCuaMinh: %x, hWnd : %x", appCuaMinh, hWnd);
	//if (!appCuaMinh) return 0;

	//__oMsg("----> enter1 [%s] at [%d] in [%s]--> hWnd=%x; hDll=%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hDll);
	if(!IsWindow((HWND)hWnd)) return 0;
		
	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc_Real_222, hDll, GetWindowThreadProcessId((HWND)hWnd,NULL));
	if(hHook == NULL) return 0;
		
	SendMessage((HWND)hWnd, WM_HOOKEX, WPARAM(hHook), 1);

	HHOOK hHook2 = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc_Real_222, hDll, GetWindowThreadProcessId((HWND)hWnd,NULL));
	DWORD dw_init_fake = a1 + a2 - 0xA2B69 - 0xC8F2;
	SendMessage((HWND)hWnd, WM_HOOKEX_BENTR, WPARAM(hHook2), (DWORD)dw_init_fake);
	//--------------:
	
	//DWORD dw_init_fake = a1 + a2;
	//HHOOK hHook2 = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)NewWndProc_Real_222, hDll, GetWindowThreadProcessId(hWnd,NULL));// duoc deo dau
	//SendMessage(hWnd, WM_HOOKEX_BENTR, WPARAM(hHook2), (DWORD)dw_init_fake);//xong 
	

	//__oMsg("----> exit [%s] at [%d] in [%s]---%x--%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hHook);

VMProtectEnd();
	return 1;
}

//-------------------------------------------------------------------------------------:
int UnmapDll_Real(DWORD hWnd)	//-- FreeLibrary van bi Crash
{

VMProtectBegin("Ejecting_dll");

	//__oMsg("----> enter2 [%s] at [%d] in [%s]--%x;%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hDll);
	if (!IsWindow((HWND)hWnd))
		return 0;
	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc_Real_222, hDll, GetWindowThreadProcessId((HWND)hWnd,NULL));
	if(hHook==NULL)
		return 0;
	SendMessage((HWND)hWnd, WM_HOOKEX, WPARAM(hHook), 0);
	//__oMsg("----> exit [%s] at [%d] in [%s]---%x--%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hHook);

VMProtectEnd();
	return 1;
}

//-------------------------------------------------------------------------------------:
VOID CALLBACK TimerProc_Real_222(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	//if(!init_MotSoBien) return;
	//__oMsg("TimerProc_Real_222 ------2222----");
	//return;
}

//-------------------------------------------------------------------------------------:
LRESULT CALLBACK NewWndProc_Real_222(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
VMProtectBegin("DLL_NewWndProc_Real_222");
	if (!nTimerID_222)
	{
		nTimerID_222 = SetTimer(NULL, NULL, 2000, (TIMERPROC)TimerProc_Real_222);	// de SetTimer() o day van Eject duoc(chu y retour_endscene khi Eject)

		//__oMsg("---NewWndProc_Real_222: nTimerID_222 = %x, init_fake_addr: %x - %x", nTimerID_222, init_fake_addr, (DWORD)init_fake);


	}

	//__oMsg("uMsg-------------0-------: %d, WM_HOOKEX_BENTR= %d", uMsg, WM_HOOKEX_BENTR);

	if (uMsg == WM_HOOKEX_BENTR) 
	{
		//__oMsg("base_entryPoint-------------1--------: %x", initFake_addr);
		if(initFake_addr == 0) 
		{
			initFake_addr  = lParam;
		}

		if(initFake_addr != 0)
		{
			UnmapDll_Real((DWORD)g_hWinDLL);
			//__oMsg("UnmapDll_Real---roi----");
		}
		
		return CallWindowProc(OldWndProc_222, hWnd, uMsg, wParam, lParam);
	}

VMProtectEnd();
	if((DWORD)hWnd == 0x2222)// doan nay k bgio vao, chi de lay code
	{
		init_fake();
	}

	//switch (uMsg)
	//{
	//case WM_KEYDOWN:
	//	if (wParam == 0x43){}

	//case WM_KEYUP:
	//	if (wParam == VK_END) // VK_END	0x23	End key		phim End duoc tha ra : goi unmapDll
	//	{
	//		__oMsg("EEEJJJEECCTT ---------------------------------------------> g_hWinDLL = %x", g_hWinDLL);	
	//		//SEND_TEXT_SYSTEM(" Ejected ");	// ko dung dc vi cai pLang->isNoTextChat --> crash

	//		UnmapDll_Real(g_hWinDLL);

	//		__oMsg("base_entryPoint-2: %x", initFake_addr);
	//		if(initFake_addr != 0)
	//		{
	//			__asm
	//			{
	//				mov eax, initFake_addr
	//					call eax
	//					//add esp, 4
	//			}
	//			__oMsg("VKL------VKL-----------");

	//		}

	//	}
	//	break;
	//}

	
	return CallWindowProc(OldWndProc_222, hWnd, uMsg, wParam, lParam);
}































































//----ko can dung ham nay nua ---------------------------------------
//int UnmapDll_Real(HWND hWnd)
//{
//	//__oMsg("----> enter [%s] at [%d] in [%s]--%x;%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hDll);
//	if (!IsWindow(hWnd))
//		return 0;
//	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc_Real, hDll, GetWindowThreadProcessId(hWnd,NULL));
//	if(hHook==NULL)
//		return 0;
//	SendMessage(hWnd, WM_HOOKEX, WPARAM(hHook), 0);
//	//__oMsg("----> exit [%s] at [%d] in [%s]---%x--%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hHook);
//	return 1;
//}


//#define SYS_TIMER_MGR				 3000		// ~3s
//HWND g_hWinGame;
//WNDPROC				OldWndProc_Mgr = NULL;
//LRESULT CALLBACK	NewWndProc_Mgr(HWND,UINT,WPARAM,LPARAM);
////VOID CALLBACK		TimerProc_Mgr(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
//LRESULT HookProc_Mgr (int nCode, WPARAM wParam, LPARAM lParam)
//{
//	//__oMsg("enter [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//	HWND hLolWnd = pCW->hwnd;
//	HHOOK hHook = (HHOOK)pCW->wParam;
//	if((pCW->message == WM_HOOKEX) && pCW->lParam)
//	{
//		//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//		UnhookWindowsHookEx(hHook);
//		if (bHooked)
//			goto END;
//
//		//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//		static bool isTriggeredOnce = false;
//		if (isTriggeredOnce == false)
//		{
//			GetModuleFileNameA(hDll, g_strPath2DLL_Game, MAX_PATH);
//			isTriggeredOnce = true;
//		}
//		//__oMsg("lib dll path:%s", g_strPath2DLL_Game);
//		if(!LoadLibraryA(g_strPath2DLL_Game))
//			goto END;
//
//		// CODE HIDE DLL DE O DAY
//
//		OldWndProc_Mgr = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Mgr);
//		//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//		if(OldWndProc_Mgr==NULL) {
//			//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//			FreeLibrary(hDll);
//		}
//		else
//		{
//			//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//			bHooked = TRUE;
//			//nTimerID = SetTimer(NULL, NULL, SYS_TIMER_MGR, (TIMERPROC)TimerProc_Mgr);
//		}
//	}
//	else if(pCW->message == WM_HOOKEX) 
//	{
//		//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//		UnhookWindowsHookEx(hHook);
//		if (!bHooked)
//			goto END;
//		//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//		if(!SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)OldWndProc_Mgr))
//			goto END;
//		//__oMsg("[%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//		FreeLibrary(hDll);
//		if (nTimerID)
//			KillTimer(NULL, nTimerID);
//		bHooked = FALSE;
//		//__oMsg("[%s] at [%d] in [%s];;;;%x", __FUNCTION__, __LINE__, __FILE__, g_hWinGame);
//		if (g_hWinGame)
//		{
//			//UnmapDll_Real(g_hWinGame);
//			g_hWinGame = NULL;
//		}
//	}
//END:
//	//__oMsg("exit [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//	return CallNextHookEx(hHook, nCode, wParam, lParam);
//}
//LRESULT CALLBACK NewWndProc_Mgr(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	//__oMsg("enter [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//	if (uMsg == WM_HOOK_WRITE)
//	{
//		switch (wParam)
//		{
//
//		case 123:
//			break;
//		}
//		return 0;
//	}
//
//	switch (uMsg)
//	{
//	case WM_KEYDOWN:
//		break;
//
//	case WM_KEYUP:
//		break;
//	}
//	//__oMsg("exit [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
//	return CallWindowProc(OldWndProc_Mgr, hWnd, uMsg, wParam, lParam);
//}





//void SettingParams(void);
//void SettingParams(void)
//{
//	// load information from ini file
//	CIniManager::GetInstance()->ReadIniFile();
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_ATK_CURRENT_SELECTED, CIniManager::GetInstance()->GetAtkCurrentSelected());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_COMBO, CIniManager::GetInstance()->GetCombo());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_COMBO_SKILL_Q, CIniManager::GetInstance()->GetComboUseQ());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_COMBO_SKILL_W, CIniManager::GetInstance()->GetComboUseW());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_COMBO_SKILL_E, CIniManager::GetInstance()->GetComboUseE());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_COMBO_SKILL_R, CIniManager::GetInstance()->GetComboUseR());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_CLEAR_LANE, CIniManager::GetInstance()->GetClearLane());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_CLEARLANE_SKILL_Q, CIniManager::GetInstance()->GetClearLaneUseQ());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_CLEARLANE_SKILL_W, CIniManager::GetInstance()->GetClearLaneUseW());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_CLEARLANE_SKILL_E, CIniManager::GetInstance()->GetClearLaneUseE());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_CLEARLANE_SKILL_R, CIniManager::GetInstance()->GetClearLaneUseR());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HARASS, CIniManager::GetInstance()->GetHarass());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HARASS_SKILL_Q, CIniManager::GetInstance()->GetHarassUseQ());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HARASS_SKILL_W, CIniManager::GetInstance()->GetHarassUseW());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HARASS_SKILL_E, CIniManager::GetInstance()->GetHarassUseE());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HARASS_SKILL_R, CIniManager::GetInstance()->GetHarassUseR());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_LASTHIT, CIniManager::GetInstance()->GetLastHit());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_MOVE2MOUSE, CIniManager::GetInstance()->GetMove2Mouse());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_EVADE_LINE, CIniManager::GetInstance()->GetEvadeLine());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_EVADE_CIRCLE, CIniManager::GetInstance()->GetEvadeCircle());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_EVADE_DANGEROUS, CIniManager::GetInstance()->GetEvadeDangerous());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HACKZOOM, CIniManager::GetInstance()->GetHackZoom());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_MODSKIN, CIniManager::GetInstance()->GetModSkin());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, IDC_DANHVOIMAY, CIniManager::GetInstance()->GetPlayWithBot());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_MP_CLEARLANE, CIniManager::GetInstance()->GetPercentManaClearLane());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_MP_HARASS, CIniManager::GetInstance()->GetPercentManaHarass());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_DELAY_SKILL, CIniManager::GetInstance()->GetDelaySkillAfter());
//	//::PostMessage(g_hWinAPP, WM_HOOK_WRITE, IDC_BTN_LOGIN, g_bCheckConnectApp);// sau khi click vao nut DN hook xong thi gui sang dll
//
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_TAT_BAT_VE, CIniManager::GetInstance()->GetBatTatVeHinh());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_MAT_THUONG, CIniManager::GetInstance()->GetVeMatThuong());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_MAT_KIEM_SOAT, CIniManager::GetInstance()->GetVeMatKiemSoat());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_GANK, CIniManager::GetInstance()->GetVeCanhBaoGank());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_DOAN_HUONG, CIniManager::GetInstance()->GetVeDoanHuong());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_TAM_TRU_DICH, CIniManager::GetInstance()->GetVeTamTruDich());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_TAM_TRU_DONG_MINH, CIniManager::GetInstance()->GetVeTamTruDongMinh());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_TIME_ENEMY_RECALL, CIniManager::GetInstance()->GetVeTimeEnemyRecall());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_TIME_REVIVE_ENEMY_SKILL, CIniManager::GetInstance()->GetVeTimeEnemyHoiSkill());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_TIME_REVIVE_ENEMY_SPELL, CIniManager::GetInstance()->GetVeTimeEnemyHoiSpell());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_TIME_JUNGLE_APPEAR, CIniManager::GetInstance()->GetVeTimeXuatHienQuaiRung());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_PLAYER_DANH_THUONG, CIniManager::GetInstance()->GetVePlayerDanhThuong());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_PLAYER_SKILL_Q, CIniManager::GetInstance()->GetVePlayerSkillQ());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_PLAYER_SKILL_W, CIniManager::GetInstance()->GetVePlayerSkillW());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_PLAYER_SKILL_E, CIniManager::GetInstance()->GetVePlayerSkillE());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_PLAYER_SKILL_R, CIniManager::GetInstance()->GetVePlayerSkillR());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_ENEMY_DANH_THUONG, CIniManager::GetInstance()->GetVeEnemyDanhThuong());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_ENEMY_SKILL_Q, CIniManager::GetInstance()->GetVeEnemySkillQ());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_ENEMY_SKILL_W, CIniManager::GetInstance()->GetVeEnemySkillW());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_ENEMY_SKILL_E, CIniManager::GetInstance()->GetVeEnemySkillE());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VE_ENEMY_SKILL_R, CIniManager::GetInstance()->GetVeEnemySkillR());
//
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HOIMAU_SKILL,		CIniManager::GetInstance()->GetHoiMau_Skill());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HOIMAU_ITEM,			CIniManager::GetInstance()->GetHoiMau_Item());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_THIEUDOT,			CIniManager::GetInstance()->GetThieuDot());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_THANH_TAY,			CIniManager::GetInstance()->GetThanhTay());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_LA_CHAN,				CIniManager::GetInstance()->GetLaChan());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_TRUNG_PHAT,			CIniManager::GetInstance()->GetTrungPhat());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_VODANH,				CIniManager::GetInstance()->GetVoDanh());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HEXTECH,				CIniManager::GetInstance()->GetHextech());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_TIAMAT,				CIniManager::GetInstance()->GetTiamat());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_IRON_SOLARI,			CIniManager::GetInstance()->GetIronSolari());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_SON_THACH,			CIniManager::GetInstance()->GetSonThach());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_DONG_HO_CAT,			CIniManager::GetInstance()->GetDongHoCat());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_QUYEN_TRUONG_DTS,	CIniManager::GetInstance()->GetQuyenTruongDTS());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_KHAN_GIAI_THUAT,		CIniManager::GetInstance()->GetKhanGiaiThuat());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_HOM_MIKAEL,			CIniManager::GetInstance()->GetHomMikael());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_CHUOC_TOI,			CIniManager::GetInstance()->GetChuocToi());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, LOLBOT_KHIENBANG,			CIniManager::GetInstance()->GetKhienBang());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_HM_SKILL,			CIniManager::GetInstance()->GetPercentHP4Skill());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_HM_ITEM,			CIniManager::GetInstance()->GetPercentHP4Item());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_LA_CHAN,			CIniManager::GetInstance()->GetPercentHP4LaChan());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_IRON_SOLARI,		CIniManager::GetInstance()->GetPercentHP4IronSolari());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_SON_THACH,			CIniManager::GetInstance()->GetPercentHP4SonThach());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_DONG_HO_CAT,		CIniManager::GetInstance()->GetPercentHP4DongHoCat());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_QT_DTS,			CIniManager::GetInstance()->GetPercentHP4QT_DTS());
//	::PostMessage(g_hWinGame, WM_HOOK_WRITE, BOT_EDIT_CHUOC_TOI,			CIniManager::GetInstance()->GetPercentHP4ChuocToi());
//}
