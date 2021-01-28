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
#include "GameDefine.h"
#include "Draw/Drawing.h"
//#include "Activator.h"
//#include "OrbWalker/OrbSafe.h"


#include "Log.h"
#include <GUIMenu/imgui.h>
#include "GUIMenu/imgui_impl_dx9.h"

//#include "AntiBan.h"



using namespace std;

HWND LOL_HWND = 0;
HINSTANCE			hDll;
static TCHAR g_strPath2DLL_Game[MAX_PATH];
const UINT WM_HOOK_WRITE = RegisterWindowMessage(_T("WM_HOOK_WRITE"));
const UINT WM_HOOKEX = RegisterWindowMessage(_T("WM_HOOKEX_RK"));
UINT_PTR	nTimerID = 0;
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
bool nIs_DrawMenu	= false;
KBuffer_Spell g_BufferSpell_2[MAX_ARRAY_BUFFER_SPELL];
KBuffer_Spell g_BufferSpell[MAX_ARRAY_BUFFER_SPELL];
KBuffer_Spell g_BaseTimeSpell[MAX_ARRAY_BUFFER_SPELL];
KOptionMenuCustomLua g_OptMenuCustom[MAX_ARRAY_OPTION_MENU];


int nComboHotKey	 = 0x20;	// default = VK_SPACE
int nLaneClearHotKey = 0x56;	// default = V
int nHarassHotKey    = 0x43;	// default = C
int nLastHitHotKey   = 0x58;	// default = X

bool nCombo_Mode	 = false;
bool nLaneClear_Mode = false;
bool nLastHit_Mode	 = false;
bool nFreeze_Mode	 = false;
bool nMixed_Mode	 = false;//thap cam mode

CIniFile* menu_IniFile;
char path_out_ini[MAX_PATH]  = "";

WNDPROC				OldWndProc = NULL;
void init_fake();
LRESULT CALLBACK	NewWndProc_Real(HWND,UINT,WPARAM,LPARAM);
VOID CALLBACK		TimerProc_Real(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
#define pCW ((CWPSTRUCT*)lParam)

#define BUFSIZE 1024


//------ bien global cho Lua -------------------------------------------------------
bool IsEvade_Lua		 = false;
bool IsPrintErrorLog_Lua = false;


//==================================================================================//
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
BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	//VMProtectBegin("DLL_VM__DllMain_xxx");
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hDll = (HINSTANCE) hModule;
		DisableThreadLibraryCalls(hDll);
		////OutputDebugString(".");
		//HWND hLolWnd = ::FindWindow(sz__RiotWindowClass, NULL);
		//OldWndProc = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real);		

		//GetModuleFileName(hDll, g_strPath2DLL_Game, MAX_PATH);

		////__oMsg("(DllMain)[%x], g_strPath2DLL_Game: %s", hDll, g_strPath2DLL_Game);
		//if (strlen(g_strPath2DLL_Game) > 0){
		//	
		//	//__oMsg("g_strPath2DLL_Game(>0) %s",g_strPath2DLL_Game);

		//	char sz__fdC_Y_yum3ww09[] = { 102, 100, 67, 41, 89, 37, 121, 117, 109, 51, 119, 119, 48, 57, 0 };
		//	TCHAR* Data = g_strPath2DLL_Game;
		//	TCHAR* Key = sz__fdC_Y_yum3ww09;
		//	//__oMsg("len1: %d [%s]", _tcslen(Data), Data);
		//	Crypt(Data, _tcslen(Data), Key, _tcslen(Key));
		//	//__oMsg("len2: %d [%s]", _tcslen(Data), Data);
		//	write_temp_file(Data);
		//}
		//else{
		//	//strcat(g_strPath2DLL_Game, "abc");
		//}
	}
	//VMProtectEnd();
	return TRUE;
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
extern float nCoolDownAA_Chuan;


//==================================================================================//
//==================================================================================//
//bool nFake = true;
void init_fake()
{
	////VMProtectBegin("DLL_VM__init_fake");
	//nFake = false;
	__oMsg("FUCKING...: init_fake-1");
	HWND hLolWnd = ::FindWindow(sz__RiotWindowClass, NULL);
	OldWndProc = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real);		
	__oMsg("FUCKING...: init_fake-2");
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
	////VMProtectEnd();
}

//==================================================================================//
LRESULT HookProc_Real (int nCode, WPARAM wParam, LPARAM lParam)
{
	//__oMsg("lib dll path----1");

	//VMProtectBegin("DLL_VM__HookProc_Real");//VM choi choi
	HWND hLolWnd = pCW->hwnd;
	HHOOK hHook = (HHOOK)pCW->wParam;
	if((pCW->message == WM_HOOKEX) && pCW->lParam)
	{
		//MessageBoxA(NULL, "Injection failed !!!", "Failed", MB_OK);
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
		//__oMsg("lib dll path:%s", g_strPath2DLL_Game);
		if(!LoadLibrary(g_strPath2DLL_Game))
			goto END;

		// CODE HIDE DLL DE O DAY

		OldWndProc = (WNDPROC)SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)NewWndProc_Real);
		__oMsg("OldWndProc: %x", OldWndProc);

		if(OldWndProc==NULL) {
			FreeLibrary(hDll);
		}
		else
		{
			extern HWND g_hWinDLL;
			g_hWinDLL = hLolWnd;
			bHooked = TRUE;

			

			//nTimerID = SetTimer(NULL, NULL, SYS_TIMER, (TIMERPROC)TimerProc_Real);
			//BYPASS_SEND_W32();// ByPass-Send() --> dang test voi JP thoi, VN chua can test
			//BYPASS_RECV_W32();// ByPass-Send() --> dang test voi JP thoi, VN chua can test
		}
	}
	else if(pCW->message == WM_HOOKEX) 
	{
		UnhookWindowsHookEx(hHook);
		if (!bHooked)
			goto END;
		if(!SetWindowLong(hLolWnd, GWL_WNDPROC, (LONG)OldWndProc))
			goto END;
		FreeLibrary(hDll);
		if (nTimerID)
			KillTimer(NULL, nTimerID);
		bHooked = FALSE;
		//Retour_EndScene();
	}
	//VMProtectEnd();
END:
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

//==================================================================================//
//==================================================================================//
//==================================================================================//
extern HWND g_hWinDLL;
extern bool g_bCheckConnectDLL;
extern bool g_bDoingE_Camille;

//lua 
HANDLE hThreadLUA = 0;
int LUA_KEY_CODE = 0;
//end lua

int nTime2CheckConfig = 0;
bool s_bIsReadConfig = true;
extern int nScripsLoaded;
bool bIsLuaLoaded = false;
int nTimeFuker = 0;

extern HANDLE vai_logFile;	
extern bool   chi1lan;
bool isInjected = false;

#include <intrin.h>

LRESULT CALLBACK NewWndProc_Real(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!nTimerID)
	{
		if(!G_Module) G_Module     = (DWORD)MyGetModuleHandle("implugin.dll");
		nTimerID = SetTimer(NULL, NULL, 35, (TIMERPROC)TimerProc_Real);
		__oMsg("implugin.dll = %x", G_Module);
	}

	

	//--------------------------
	switch (uMsg)
	{
	case WM_KEYDOWN:
		
		break;

	case WM_KEYUP:
		
		//----------------------------------------------
		if (wParam == VK_HOME) // VK_HOME	0x24	HOME key		phim Home duoc tha ra : goi nuttest
		{		
			//__oMsg("xxxxx");

			//if(!G_Module) G_Module = (DWORD)MyGetModuleHandle("implugin.dll");
			//
			////nTimerID = SetTimer(NULL, NULL, 35, (TIMERPROC)TimerProc_Real);
			//__oMsg("G_Module = %x", G_Module);


			//TestTest();
		}
		if (wParam == VK_END) // VK_END	0x23	End key		phim End duoc tha ra : goi unmapDll
		{
			if (chi1lan == false) chi1lan = true;
			
			if (vai_logFile)
			{
				CloseHandle(vai_logFile);
				vai_logFile = 0;
				//SEND_TEXT_SYSTEM("----Ban Vua CloseHandle FILE-----");
			}

			//ClearScreen();
			//FreeConsole();
		}
		if (wParam == VK_DELETE)
		{
			if (vai_logFile)
				CloseHandle(vai_logFile);


			//FreeConsole();
			//SEND_TEXT_SYSTEM("----Ejected-DLL-----");
			UnmapDll_Real(g_hWinDLL);
		}
		break;
	}



return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);




	if((DWORD)hWnd == 0x121)// doan nay k bgio vao, chi de lay code
	{
		init_fake();
		return 0;
	}

//VMProtectBegin("DLL_VM__SetValueInit");	
	if (!nTimerID)
	{
		if(!LOL_HWND) LOL_HWND = FindWindow(sz__RiotWindowClass, NULL);
		
		
		G_Module     = (DWORD)MyGetModuleHandle(sz__League_of_Legends_exe);
		
		nTimerID = SetTimer(NULL, NULL, 35, (TIMERPROC)TimerProc_Real);
		__oMsg("G_Module = %x", G_Module);
	}
	if (!IsGameReady())
		return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);

	//------ checkLogin vai phat nua --:
	nTimeFuker += 30;
	if(nTimeFuker > 880000)
	{
		/*HANDLE hThread;
		DWORD nThread_ID = 0;
		hThread = CreateThread(NULL,NULL,&CheckLogin,NULL,NULL,&nThread_ID);
		typedef NTSTATUS (NTAPI *pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG); 
		NTSTATUS Status; 
		pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandle( TEXT("ntdll.dll")), "NtSetInformationThread");
		if (NtSIT)
		{
			Status = NtSIT(hThread, 0x11, 0, 0); 
		}
		CloseHandle(hThread);

		nTimeFuker = 0;*/
	}
	//__oMsg("nTimeFuker: %d", nTimeFuker);
//VMProtectEnd();	
	switch (uMsg)
	{
		case WM_KEYDOWN:
			LUA_KEY_CODE = (int)wParam;
			break;
		case WM_KEYUP:
			LUA_KEY_CODE = 0;
	}

//---- LuaRunOnWndMsg ------------------------------------------------------------------------------------
	
//---- End-LuaRunOnWndMsg ---------------------------------------------------------------------------------

	static int nCombinationSkin = 0;
	static int nCombinationSkin_NumLock = 0;
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == 0x43)
		{
			//Detour_EndScene();
		}
		if (wParam == 0x11)
		{
			//Retour_EndScene();
		}
		bIs_Chatting = IS_CHATTING();
		
		if (wParam == VK_SHIFT)
		{
			//Detour_EndScene();
			//Detour_MenuGame();
			nIs_DrawMenu = true;
		}

		if (wParam == VK_SHIFT && bIs_Chatting == false)//VK_SHIFT  0x10  ALT key
		{
			nCombinationSkin = 1;
		}
		if (wParam == VK_CONTROL && bIs_Chatting == false)//VK_CONTROL
		{
			nCombinationSkin_NumLock = 1;
		}
		if (wParam == nHarassHotKey && bIs_Chatting == false) g_bHarass = 1; // phim C dc an'

		if (wParam == nComboHotKey && bIs_Chatting == false) // phim spacebar
		{
			bPressingSPACE = true;
			KNpc *pPlayer = GetPlayer();
			KPos* pMousePos = GetMousePos();
			if(pPlayer && pMousePos)
			{
				float nKC_Player2Mouse = Distance_2D(pPlayer->CurrentPos, *pMousePos);
				//__oMsg("nKC_Player2Mouse: %f  - %d ==>%f", nKC_Player2Mouse, (BYTE)pPlayer->nCanSee, nTimePressingSPACE);
				if((BYTE)pPlayer->nIsDie == 0 && ((BYTE)pPlayer->nCanSee == 1 || nKC_Player2Mouse <= 4200.0))
				{
					g_bNormalAttackHero = true;
				}
				else
				{
					if (nTimePressingSPACE >= 50.0) g_bNormalAttackHero = true;
				}
			}
		}

		if (wParam == 0xC0 && bIs_Chatting == false) // phim ~ duoc bam
		{
			g_bCheckKeyPress = true;
		}
		if (wParam == 0x55 && bIs_Chatting == false) // phim S dang duoc bam -> chuyen sang phim U
		{
			g_bPressingKey_S = true;
		}
		if (wParam == nLaneClearHotKey && bIs_Chatting == false) // phim V
		{
			g_bClearLane = true;
		}
		if (wParam == nLastHitHotKey && bIs_Chatting == false) // phim X
		{
			g_bPressingX = true;
		}
		if (wParam == 0x54 && bIs_Chatting == false) // phim T dang duoc bam
		{
			g_bPressingKey_T = true;
		}
		if (wParam == 0x45 && bIs_Chatting == false) // phim E dang duoc bam-0x45
		{
			g_bPressingKey_E = true;
		}
		if (wParam == 0x41 && bIs_Chatting == false) // phim A dang duoc bam
		{
			g_bPressingKey_A = true;
		}
		if (wParam == 0x33 && bIs_Chatting == false) // phim so 3 dang duoc bam
		{
			g_bPressingKey_So3 = true;
		}
		break;

	
	case WM_KEYUP:
		bIs_Chatting = IS_CHATTING();
		if (wParam == VK_SHIFT && bIs_Chatting == false)//VK_SHIFT  0x10  ALT key
		{
			nCombinationSkin = 0;
		}
		if (wParam == VK_CONTROL && bIs_Chatting == false)//VK_SHIFT  0x10  ALT key
		{
			nCombinationSkin_NumLock = 0;
		}
		KNpc *pPlayer = GetPlayer();
		
		if (wParam == 0x4B  && bIs_Chatting == false) // phim K
		{
			
		}
		//if (wParam == 0x36  && bIs_Chatting == false) // phim So 6
		//if (wParam == 0x4E  && bIs_Chatting == false)   // phim N
		//{
		//	pLang->isMoveFollowCursor = !pLang->isMoveFollowCursor;
		//	SEND_TEXT_SYSTEM(pLang->isMoveFollowCursor ?  (sz__N___ON) :  (sz__N___OFF));
		//	//SEND_TEXT_SYSTEM(g_bMove2MousePos ?  (sz__Di_chuyen_theo_chuot___ON) :  (sz__Di_chuyen_theo_chuot___OFF));
		//}
		if (wParam == nHarassHotKey)						 // phim C
		{
			
			g_bHarass = 0; 
			nMixed_Mode = false;
		}
		if (wParam == 0xC0 || wParam == nComboHotKey  || wParam == 0x55) // phim ~, phim spacebar, phim S(chuyen sang phim U)
		{
			bPressingSPACE = false;
			nTimePressingSPACE = 0.0;
			

			if (wParam == nComboHotKey)
			{
				g_bNormalAttackHero = false;
				nCombo_Mode = false;
			}
			g_DelayNormalAttack = TIME_DELAY_NORMAL_ATTACK; // reset time delay
			if (wParam == 0xC0) // nha phim ~
			{
				g_bCheckKeyPress = false;
				nCombo_Mode = false;
			}
			if (wParam == 0x55) // nha phim S -> chuyen sang phim U
			{
				g_bPressingKey_S = false;
				nCombo_Mode = false;
			}
		}
		if (wParam == nLaneClearHotKey) // phim V
		{
			g_bClearLane = false;
			nLaneClear_Mode = false;
		}
		if (wParam == nLastHitHotKey) // phim X
		{
			g_bPressingX = false;
			nLastHit_Mode = false;
		}
		if (wParam == 0x54  && bIs_Chatting == false) // phim T dang duoc tha ra
		{
			g_bPressingKey_T = false;
		}
		if (wParam == 0x45 && bIs_Chatting == false) // phim E dang duoc tha ra
		{
			g_bPressingKey_E = false;
		}
		if (wParam == 0x41 && bIs_Chatting == false) // phim A dang duoc tha ra
		{
			g_bPressingKey_A = false;
		}
		if (wParam == 0x33 && bIs_Chatting == false) // phim so 3 dang duoc tha ra
		{
			g_bPressingKey_So3 = false;
		}
		if (wParam == 0x77) // phim so F8 da duoc tha ra : dung tat/bat auto-R
		{
			//g_bPressingKey_F8 = false;
			KNpc* pPlayer = GetPlayer();
			if(pPlayer)
			{
				if (CheckName(pPlayer->GetNameHero(),  (sz__Jinx))		|| 
					CheckName(pPlayer->GetNameHero(),  (sz__Varus))	||
					CheckName(pPlayer->GetNameHero(),  (sz__KogMaw))	|| 
					CheckName(pPlayer->GetNameHero(),  (sz__Jhin))		|| 
					CheckName(pPlayer->GetNameHero(),  (sz__Lucian))	|| 
					CheckName(pPlayer->GetNameHero(),  (sz__Ashe))		||
					CheckName(pPlayer->GetNameHero(),  (sz__Ezreal))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Caitlyn))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Twitch))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Karthus))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Velkoz))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Nami))		||
					CheckName(pPlayer->GetNameHero(),  (sz__Orianna))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Malzahar))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Lulu))		||
					CheckName(pPlayer->GetNameHero(),  (sz__Katarina))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Sona))		||
					CheckName(pPlayer->GetNameHero(),  (sz__Nunu))		||
					CheckName(pPlayer->GetNameHero(),  (sz__Janna))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Braum))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Malphite))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Zilean))	||
					CheckName(pPlayer->GetNameHero(),  (sz__MasterYi))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Camille))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Ziggs))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Brand))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Zyra))		||
					CheckName(pPlayer->GetNameHero(),  (sz__Diana))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Cassiopeia))   ||
					CheckName(pPlayer->GetNameHero(),  (sz__MissFortune))	||
					CheckName(pPlayer->GetNameHero(),  (sz__Teemo))
					)
				{
					IS_BAT_R = !IS_BAT_R;	
					if(IS_BAT_R == true)
					{
						SEND_TEXT_SYSTEM( (sz_____Auto_R___On));
					}
					else
					{
						SEND_TEXT_SYSTEM( (sz_____Auto_R___Off));
					}
				}
			}
		}
		if (wParam == 0x76) // phim so F7 da duoc tha ra : dung tat/bat Auto-R-HoTroDongDoi
		{
			//g_bPressingKey_F8 = false;
			KNpc* pPlayer = GetPlayer();
			if(pPlayer)
			{
				if (CheckName(pPlayer->GetNameHero(),  (sz__Lucian))// Lucian thi Tat/Bat auto-E

					)
				{
					IS_R_VeBeDa = !IS_R_VeBeDa;	
					if(IS_R_VeBeDa == true)
					{
						SEND_TEXT_SYSTEM( (sz__Tu_dong_dung_E___ON));
					}
					else
					{
						SEND_TEXT_SYSTEM( (sz__Tu_dong_dung_E___OFF));
					}
				}
				if (CheckName(pPlayer->GetNameHero(),  (sz__Vayne))// Vayne thi Tat/Bat auto-Q

					)
				{
					IS_R_VeBeDa = !IS_R_VeBeDa;	
					if(IS_R_VeBeDa == true)
					{
						SEND_TEXT_SYSTEM( (sz__Tu_dong_dung_Q___ON));
					}
					else
					{
						SEND_TEXT_SYSTEM( (sz__Tu_dong_dung_Q___OFF));
					}
				}

			//-- Jinx va 1 so con khac R ve be da ---------------------------------------
				if (CheckName(pPlayer->GetNameHero(),  (sz__Jinx))
					
				   )
				{
					IS_R_VeBeDa = !IS_R_VeBeDa;	
					if(IS_R_VeBeDa == true)
					{
						//SEND_TEXT_SYSTEM( (sz__Dung_R_Ho_Tro_Dong_Doi___ON));
						SEND_TEXT_SYSTEM( (sz__R_khi_dich_B_bien_ve___ON));
					}
					else
					{
						//SEND_TEXT_SYSTEM( (sz__Dung_R_Ho_Tro_Dong_Doi___OFF));
						SEND_TEXT_SYSTEM( (sz__R_khi_dich_B_bien_ve___OFF));
					}
				}
			}
		}
		if (wParam == 0x75  && bIs_Chatting == false)   // phim F6 : bat/tat ne = toc bien
		{
			
		}
		if (wParam == VK_F5 && bIs_Chatting == false)   // VK_F5	0x74 phim F5 : bat/tat Ve
		{
			
		}
		if (wParam == VK_F11 && bIs_Chatting == false)   // VK_F11	0x7A phim F11 : bat/tat Ve vong tron nho-tai vi tri con tro chuot
		{
			//g_isDraw_CircleAtMousePos = !g_isDraw_CircleAtMousePos;
		}
	//--HackZoom ---------------------------------------------------
		if (wParam == 107 || wParam == 187) // phim + da duoc tha ra : Tang Hack Zoom
		{
			
		}
		if (wParam == 109 || wParam == 189) // phim - da duoc tha ra : Giam Hack Zoom
		{
			
		}
		if (wParam == VK_HOME) // VK_HOME	0x24	HOME key		phim Home duoc tha ra : goi nuttest
		{
			TestTest();

//----- Re-Load Lua--------------------------------------------------------------------
			
		}
		if (wParam == VK_END) // VK_END	0x23	End key		phim End duoc tha ra : goi unmapDll
		{
			ClearScreen();
		}
		if (wParam == VK_DELETE)
		{
			FreeConsole();
			SEND_TEXT_SYSTEM("----Ejected-DLL-----");
			bIsLuaLoaded = false;
			
			//nIs_EndLua = true;
			extern bool g_Exit;
			g_Exit = true;
			UnmapDll_Real(g_hWinDLL);
		}

		if (wParam == VK_SHIFT)
		{
			nIs_DrawMenu = false;
			
//----- WriteConfig --------------------------------------------------------------------
			//DWORD nThread_ID2 = 0;
			//HANDLE hThread2;
			//hThread2 = CreateThread(NULL,NULL,&WriteConfig,NULL,NULL,&nThread_ID2);
			//CloseHandle(hThread2);
			////g_isDrawing ? Detour_EndScene() : Retour_EndScene();
			////--set de 1s sau read lai config -----
			//s_bIsReadConfig   = true;
			//nTime2CheckConfig = 0;
		}
		
		break;
	}
	//__oMsg("----> ENDOF [%s] at [%d] in [%s]", __FUNCTION__, __LINE__, __FILE__);
	
	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}

//==================================================================================//
//==================================================================================//
//==================================================================================//
extern bool g_bDoingR_Xerath;
extern bool g_bDoingE_Pan;
extern bool g_bDoingR_MissFortune;
extern bool g_bDoingR_Katarina;
extern bool g_bDoingR_Jhin;
extern bool g_bDoingQ_Vayne;
extern bool g_bDoingR_Velkoz;
extern bool g_bDoingR_Jana;
extern bool g_bDoingR_Nunu;
extern bool g_bDoingR_Malzahar;
extern bool g_bDoingW_Yi;

//==================================================================================//
//DWORD org_b1[0x90] ={0};
//DWORD org_b2[0x10] ={0};

struct buff1
{
	DWORD		temp_1[6];
	DWORD		targetID;
	DWORD		temp_2[9];
	DWORD		textSend;
	DWORD		temp_3[3];
	unsigned	leng;
	unsigned	maxLeng;
	DWORD		temp1[18];
};

struct buff2
{
	DWORD temp[6];
};

buff1* org_b1 = 0;
buff2* org_b2 = 0;

buff1* send_b1 = 0;
buff2* send_b2 = 0;

bool noNeedHook = false;
typedef char (__thiscall* oSendChatGas)(void*, DWORD, DWORD);
oSendChatGas oOnSendChatGas;


//START: 491871350


/*

[6760] ---------- START.[i=0, id = 441500000]
[6760] ---------- END  .[i=499999, id = 441000000]


*/


DWORD currentID =	441000000;
//SENT :			440500000	--> dang den thang nay roi (chua sent)


int __stdcall Hook_SendChatGas(DWORD Buffer_1, DWORD Buffer_2)
{	
	void * pThis = NULL;
	__asm
	{
		mov pThis, ecx
	}

	if (send_b1 == 0) send_b1 = new buff1;
	if (send_b2 == 0) send_b2 = new buff2;

	if (org_b1 == 0) org_b1 = new buff1;
	if (org_b2 == 0) org_b2 = new buff2;

	memcpy(org_b1, (buff1*)Buffer_1, sizeof(buff1));
	memcpy(org_b2, (buff2*)Buffer_2, sizeof(buff2));

	//__oMsg("----2------ org_b1: %x, org_b2: %x - send_b1:%x, send_b2:%x, sizeOf1: %x, sizeOf2: %x, [b1: %x, b2: %x]", org_b1, org_b2, send_b1, send_b2, sizeof(buff1), sizeof(buff2), Buffer_1, Buffer_2);

	if (noNeedHook == false)
	{
		noNeedHook = true;
		
		for (unsigned i = 0; i < 500000; i++)
		{
			DWORD func  = G_Module + 0x37410;
			DWORD _ECX	= (DWORD)pThis;

			memcpy(send_b1, org_b1, sizeof(buff1));
			memcpy(send_b2, org_b2, sizeof(buff2));
			unsigned ID = currentID - i;

			send_b1->targetID = ID;				//0x1D043669;	//(gaatapchoi1.ID)

			DWORD arg1	= (DWORD)send_b1;
			DWORD arg2	= (DWORD)send_b2;
			//__oMsg("---------- [i=%d, id = %d]--funcSendChat: %x, this: %x, arg1: %x, arg2: %x", i, ID, func, _ECX, arg1, arg2);

			if (i == 0)
			{
				__oMsg("\n---------- START.[i=%d, id = %d]", i, ID);
			}

			if (i == 500000 - 1)
			{
				__oMsg("---------- END  .[i=%d, id = %d]\n", i, ID - 1);
			}

			__asm
			{
				push arg2
				push arg1
				mov ecx, _ECX
				mov eax, func
				call eax
			}
		}


		//DWORD func  = G_Module + 0x37410;
		//DWORD _ECX	= (DWORD)pThis;
		//
		//memcpy(send_b1, org_b1, sizeof(buff1));
		//memcpy(send_b2, org_b2, sizeof(buff2));
		//send_b1->targetID = 486815337;				//0x1D043669;	//(gaatapchoi1.ID)

		//DWORD arg1	= (DWORD)send_b1;
		//DWORD arg2	= (DWORD)send_b2;
		//__oMsg("---------- 1--funcSendChat: %x, this: %x, arg1: %x, arg2: %x", func, _ECX, arg1, arg2);
		//
		//__asm
		//{
		//	push arg2
		//	push arg1
		//	mov ecx, _ECX
		//	mov eax, func
		//	call eax
		//}

		////---------------------
		//memcpy(send_b1, org_b1, sizeof(buff1));
		//memcpy(send_b2, org_b2, sizeof(buff2));
		//send_b1->targetID = 490816622;				//0x1D41446E;	//(v101x1.ID)

		//arg1 = (DWORD)send_b1;
		//arg2 = (DWORD)send_b2;
		//__oMsg("---------- 2--funcSendChat: %x, this: %x, arg1: %x, arg2: %x", func, _ECX, arg1, arg2);

		//__asm
		//{
		//	push arg2
		//	push arg1
		//	mov ecx, _ECX
		//	mov eax, func
		//	call eax
		//}

		////---------------------
		//memcpy(send_b1, org_b1, sizeof(buff1));
		//memcpy(send_b2, org_b2, sizeof(buff2));
		//send_b1->targetID = 479925020;				//0x1C9B131C;	//(johnnytran.ID)

		//arg1 = (DWORD)send_b1;
		//arg2 = (DWORD)send_b2;
		//__oMsg("---------- 3--funcSendChat: %x, this: %x, arg1: %x, arg2: %x", func, _ECX, arg1, arg2);

		//__asm
		//{
		//	push arg2
		//	push arg1
		//	mov ecx, _ECX
		//	mov eax, func
		//	call eax
		//}
	}
	
	return oOnSendChatGas(pThis, (DWORD)send_b1, (DWORD)send_b2);
}

//==================================================================================//
DWORD nSendChatGas	  = 0;
DWORD nBackUp_Code1xx = 0;
DWORD nBackUp_Code2xx = 0;
void Detour_SendChatGas()
{
	nSendChatGas = G_Module + 0x37410;

	if (nSendChatGas)
	{
		if(!nBackUp_Code1xx || !nBackUp_Code2xx)
		{
			nBackUp_Code1xx = *(DWORD*) nSendChatGas;
			nBackUp_Code2xx = *(DWORD*)(nSendChatGas + 4);
			__oMsg("nBackUp_Code1xx: %x - nBackUp_Code2xx: %x, nSendChatGas = [%x]", nBackUp_Code1xx, nBackUp_Code2xx, nSendChatGas);
			oOnSendChatGas  = (oSendChatGas)(DetourFunction((PBYTE)nSendChatGas,(PBYTE)&Hook_SendChatGas));
		}
	}
}

//==================================================================================//
void Retour_SendChatGas()
{
	nSendChatGas = G_Module + 0x37410;

	if (nBackUp_Code1xx && nBackUp_Code2xx && nSendChatGas && G_Module)
	{
		__oMsg("1.RETOUR -------------: %x - %x - [%x, %x]", *(DWORD*)(nSendChatGas + 0), *(DWORD*)(nSendChatGas + 4), nBackUp_Code1xx, nBackUp_Code2xx);

		//*(DWORD*)(nSendChatGas + 0) = nBackUp_Code1xx;
		//*(DWORD*)(nSendChatGas + 4) = nBackUp_Code2xx;

		HWND  hWnd = FindWindow( "Qt5QWindowToolSaveBits", NULL);	
		if(hWnd) 
		{
			DWORD proccess_ID = 0;
			GetWindowThreadProcessId(hWnd, &proccess_ID);
			HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, proccess_ID);
			if(hProcess)
			{
				__oMsg("RETOUR -------------");
				DWORD n_Size = 4;					
				WriteProcessMemory(hProcess, (LPVOID)(nSendChatGas + 0), &nBackUp_Code1xx, n_Size, &n_Size);
				WriteProcessMemory(hProcess, (LPVOID)(nSendChatGas + 4), &nBackUp_Code2xx, n_Size, &n_Size);
			}
		}

		nBackUp_Code1xx = 0;
		nBackUp_Code2xx = 0;
		__oMsg("2.RETOUR -------------: %x - %x - [%x, %x]", *(DWORD*)(nSendChatGas + 0), *(DWORD*)(nSendChatGas + 4), nBackUp_Code1xx, nBackUp_Code2xx);
	}
}


//==================================================================================//
//extern bool CanMove_LS();

int testTimeGame = 0;

VOID CALLBACK TimerProc_Real(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{

	if (testTimeGame == 0)
	{
		__oMsg("------------- %d -------------", testTimeGame);
		Detour_SendChatGas();
		testTimeGame += 1;
	}

	if(GetAsyncKeyState(VK_END) & 0x8000)
	{
		noNeedHook = false;
		__oMsg("------------- set: noNeedHook = %x", noNeedHook);
	}

	if(GetAsyncKeyState(VK_DELETE) & 0x8000)
	{
		Retour_SendChatGas();
		__oMsg("------------- EJECTED ----------------");
		UnmapDll_Real(g_hWinDLL);
	}


	return;
	return;
	return;
	return;
	return;
	return;
	return;
	return;
	return;


	return;

	if (!IsGameReady())
		return;

	
	//return;
	

	//__oMsg("TimerProc_Real-2");
	static UINT nUsingTimer = 0;
	nUsingTimer++;
	if(nUsingTimer == 0 || myTeamId == 0)
	{
		KNpc* myHero = GetPlayer();
		if(myHero) myTeamId = myHero->nTeam_ID;
	}
	if (nUsingTimer == 5)
	{
		//Detour_EndScene();
		//Detour_OnCreateObj();//no
	}
	return;
	

	//-------------------------------------------
	if (nUsingTimer >= 20)
	{
		
	}

	if (nUsingTimer == 10)
	{
		
			
	}
	
	KNpc *pPlayer = GetPlayer();
	if (!pPlayer) return;
	if (nUsingTimer%(10 * 100)==0){
		static float lastPosX = 0, lastPosZ = 0;
		if (pPlayer->CurrentPos.x == lastPosX && pPlayer->CurrentPos.z == lastPosZ)
		{
			float x = pPlayer->CurrentPos.x + 68, z = pPlayer->CurrentPos.z + 68;
			MOVE_TO(x, z);
		}
		lastPosX = pPlayer->CurrentPos.x, lastPosZ = pPlayer->CurrentPos.z;
	}
//------ update config --------------------
	nTime2CheckConfig += SYS_TIMER;
	//__oMsg("nTime2CheckConfig = %d, nTime2CheckLogin=%d", nTime2CheckConfig, nTime2CheckLogin);//test di
	if (nTime2CheckConfig > 400 && s_bIsReadConfig)// chac la tam 10s 1 ti. the thoi
	{
		DWORD nThread_ID2 = 0;
		HANDLE hThread2;
		hThread2 = CreateThread(NULL,NULL,&ReadConfig,NULL,NULL,&nThread_ID2);
		CloseHandle(hThread2);
		nTime2CheckConfig = 0;
		s_bIsReadConfig = false;
	}
//------ end update config ----------------
	if (bPressingSPACE)
		nTimePressingSPACE += SYS_TIMER;

//VMProtectBegin("DLL_VM__PRE_CheckLogin");
	static bool s_isFirstCheck = true;
	static UINT nTime2CheckLogin = 0;
	nTime2CheckLogin += SYS_TIMER;

	if ((nTime2CheckLogin >= 1.5f*159000 /*&& (BYTE)pPlayer->nIsDie*/)|| (nTime2CheckLogin >= 100 && s_isFirstCheck)) //[26500/2 <=> 25s;530<=>1s]
	{
		//__oMsg("checklogin : %d", nTimerID);
		s_isFirstCheck = false;
		HANDLE hThread;
		DWORD nThread_ID = 0;
		hThread = CreateThread(NULL,NULL,&CheckLogin,NULL,NULL,&nThread_ID);
		typedef NTSTATUS (NTAPI *pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG); 
		NTSTATUS Status; 
		pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandle( TEXT("ntdll.dll")), "NtSetInformationThread");
		if (NtSIT)
		{
			
			Status = NtSIT(hThread, 0x11, 0, 0); 
		}
		CloseHandle(hThread);
		nTime2CheckLogin = 0;
	}
//VMProtectEnd();
	
//----- OnTickCore All --------------- :
	

	
//--------------------------------------------------------------------------------

//----- Evading ---------------------- :
	//extern float nMoveFunc_DelayMax;


//----- Danh Voi May ------------------------------------------------------------------
	
//----- Items .. ------------------------------------------------------------------
	
	g_tDelay += SYS_TIMER;
	g_DelayNormalAttack += SYS_TIMER;
}

//-------------------------------------------------------------------------------- :
int glo23523vsasfsabal(HWND hWnd)//ham Export -> doi lai ten
{
	//MessageBoxA(NULL, "Injection failed !!!", "Failed", MB_OK);
	//__oMsg("----> enter [%s] at [%d] in [%s]--%x;%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hDll);
	__oMsg("---: %x", IsWindow(hWnd));
	if(!IsWindow(hWnd)) return 0;
	

	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc_Real, hDll, GetWindowThreadProcessId(hWnd,NULL));
	if(hHook == NULL) return 0;
		
	SendMessage(hWnd, WM_HOOKEX, WPARAM(hHook), 1);
	
	//__oMsg("----> exit [%s] at [%d] in [%s]---%x--%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hHook);
	return 1;
}


































































//----ko can dung ham nay nua ---------------------------------------
int UnmapDll_Real(HWND hWnd)
{
	//__oMsg("----> enter [%s] at [%d] in [%s]--%x;%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hDll);
	if (!IsWindow(hWnd))
		return 0;
	HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC,(HOOKPROC)HookProc_Real, hDll, GetWindowThreadProcessId(hWnd,NULL));
	if(hHook==NULL)
		return 0;
	SendMessage(hWnd, WM_HOOKEX, WPARAM(hHook), 0);
	//__oMsg("----> exit [%s] at [%d] in [%s]---%x--%x", __FUNCTION__, __LINE__, __FILE__, hWnd, hHook);
	return 1;
}


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
