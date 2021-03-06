#include "Drawing.h"

#pragma comment (lib,"D3dx9.lib")
#pragma comment(lib, "detours.lib")


extern void  Smite_OnDrawMenu();
//==================================================================================//
//==================================================================================//
typedef int (__stdcall* EndScene)(LPDIRECT3DDEVICE9 Device);
EndScene oEndScene = NULL;
EndScene oEndScene_Menu = NULL;

int CircularRangeIndicator_Empty		= 0;		// vong` CircularRangeIndicator_Empty thi chi can  + 80.0 so voi chi so cua game
int CircularRangeIndicator_Dark			= 0;	
int ward_range_indicator				= 0;		
int team_ring_red_02					= 0;		// de ve~ vong tron do Nam - Bay
int team_ring_blue_02					= 0;		// de ve~ vong tron` xanh ThauKinh
int Jayce_AOE_Yellow					= 0;		// de ve~ vong tron danh thuong cua dich
int verythinring						= 0;		// vong` verythinring thi chi can + 80.0 so voi chi so cua game
int ring_highres						= 0;
int SelectionCircleLarge3				= 0;

int rengar_ring							= 0;		// vong` rengar_ring thi chi can  + 170.0 so voi chi so cua game

int template_shield						= 0;		// de ve~ HeroTangHinh (makeItRain_x_red_02.dds cung dep)
int Item_JungleLichbane_ring			= 0;		// de ve~ vong tron Mat-LinhLastHit, Bay, Nam ...

int color_reddisc						= 0;		// de ve~ duong mau do? du doan huong di dich
int color_teleportbeam_reduced			= 0;		// de ve~ duong mau xam' canh bao Gank
int skillshot_Rectangle_base_r01_v01	= 0;		// de ve~ skill-line

int Summoner_teleport				    = 0;
int Summoner_flash						= 0;
int SummonerIgnite						= 0;
int Summoner_heal						= 0;
int Summoner_smite						= 0;
int Summoner_boost						= 0;
int Summoner_Exhaust				    = 0;
int SummonerBarrier					    = 0;
int Summoner_haste						= 0;
int Summoner_Mark				        = 0;
int SummonerMana				        = 0;

int n3115_Nashors_Tooth                  = 0;

int pTexture_TEST	= 0;
//==================================================================================//
//==================================================================================//
void	LoadGameTexture()
{
	if(!pTexture_TEST)
	{
		//pTexture_TEST = GetTexture_Game("skillshot_Rectangle_base_r01_v01.dds");//skillshot_Rectangle_base_r01_v01.dds
	}
	
	if (!n3115_Nashors_Tooth)
	{
		n3115_Nashors_Tooth = GetTexture_Game("3115_Nashors_Tooth.dds");
	}
//-----------------------------
	if (!CircularRangeIndicator_Empty)
	{
		CircularRangeIndicator_Empty = GetTexture_Game( (sz__CircularRangeIndicator_Empty_dds));
	}
	if(!rengar_ring)
	{
		rengar_ring = GetTexture_Game( (sz__rengar_ring_dds));
		//rengar_ring = GetTexture_Game("SchauwareRangeCircle.dds");
	}

	if(!template_shield)
	{
		template_shield = GetTexture_Game( (sz___template_shield_dds));	
	}
	if(!Item_JungleLichbane_ring)
	{
		Item_JungleLichbane_ring = GetTexture_Game( (sz__Item_JungleLichbane_ring_dds));
		//Item_JungleLichbane_ring = GetTexture_Game("makeItRain_x_red_02.dds");
		//__oMsg("%x", Item_JungleLichbane_ring);
	}
	if(!color_reddisc)
	{
		color_reddisc = GetTexture_Game( (sz__color_reddisc_DDS));
	}
	if(!color_teleportbeam_reduced)
	{
		color_teleportbeam_reduced = GetTexture_Game( (sz__color_teleportbeam_reduced_dds));
	}

	if(!skillshot_Rectangle_base_r01_v01)
	{
		skillshot_Rectangle_base_r01_v01 = GetTexture_Game("skillshot_Rectangle_base_r01_v01.dds");//skillshot_Rectangle_base_r01_v01.dds
	}
//-----------------------------
	if(!Summoner_teleport)
	{
		Summoner_teleport = GetTexture_Game( (sz__Summoner_teleport_dds));
	}
	if(!Summoner_flash)
	{
		Summoner_flash = GetTexture_Game( (sz__Summoner_flash_dds));
	}
	if(!SummonerIgnite)
	{
		SummonerIgnite = GetTexture_Game( (sz__SummonerIgnite_dds));
	}
	if(!Summoner_heal)
	{
		Summoner_heal = GetTexture_Game( (sz__Summoner_heal_dds));
	}
	if(!Summoner_smite)
	{
		Summoner_smite = GetTexture_Game( (sz__Summoner_smite_dds));
	}
	if(!Summoner_boost)
	{
		Summoner_boost = GetTexture_Game( (sz__Summoner_boost_dds));
	}
	if(!Summoner_Exhaust)
	{
		Summoner_Exhaust = GetTexture_Game( (sz__Summoner_Exhaust_dds));
	}
	if(!SummonerBarrier)
	{
		SummonerBarrier = GetTexture_Game( (sz__SummonerBarrier_dds));
	}
	if(!Summoner_haste)
	{
		Summoner_haste = GetTexture_Game( (sz__Summoner_haste_dds));
	}
	if(!Summoner_Mark)
	{
		Summoner_Mark = GetTexture_Game( (sz__Summoner_Mark_dds));
	}
	if(!SummonerMana)
	{
		SummonerMana = GetTexture_Game( (sz__SummonerMana_dds));
	}
//-----------------------------
	if(!team_ring_red_02)
	{
		team_ring_red_02 = GetTexture_Game( (sz__team_ring_red_02_DDS));
	}
	if(!ring_highres)
	{
		ring_highres = GetTexture_Game("ring_highres.dds");
	}
	if(!SelectionCircleLarge3)
	{
		SelectionCircleLarge3 = GetTexture_Game("SelectionCircleLarge3.dds");
	}
}

//================================================================================================
extern bool g_isDraw_CircleAtMousePos;
extern bool nIs_DrawMenu;
extern bool g_isDrawing;

//#include "OrbWalker/Prediction.h"
//KPos testPoint1(6484, 0,9926);KPos testPoint2(7182, 0,8956);float tetTick1, tetTick2 =0;
extern callback_DrawLogicChamp callbackDrawLogicMenu;
HRESULT WINAPI Hook_EndScene(LPDIRECT3DDEVICE9 pDevice)
{	
	
	

	LoadGameTexture();
	
	
	
//

	return oEndScene(pDevice);
}

//==================================================================================//
//==================================================================================//
KRenderer* GetRenderer()
{
	DWORD hModule = (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempBase  = BASE_RENDERER - TEMP_ENTRY_POINT + hModule;
	DWORD dw 		= *(DWORD*)nTempBase;
	KRenderer *pRenderer = (KRenderer*)dw;
	if (pRenderer == 0)
		return NULL;
	//__oMsg("[%d - %d] - [%f - %f - %f] - [%f - %f - %f]", pRenderer->nWidth, pRenderer->nHeight, pRenderer->nViewMatrix.x, pRenderer->nViewMatrix.y, pRenderer->nViewMatrix.z, pRenderer->nProjectionMatrix.x, pRenderer->nProjectionMatrix.y, pRenderer->nProjectionMatrix.z);
	return pRenderer;
}

//==================================================================================//
//==================================================================================//
LPDIRECT3DDEVICE9 Get_Device() 
{ 
	//DWORD hModule = (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	//DWORD nTempDevice  = OFFSET_D3D9DEVICE - TEMP_ENTRY_POINT + hModule;
	////return *( IDirect3DDevice9** )(nTempDevice);

	//--- su dung doan code duoi nay cung dc, nhung backup se met hon --------------
	//------------------------------------------------------------------------------	

	KRenderer *pRender = GetRenderer();
	if(!pRender) return 0;

	DWORD dw1 = pRender->pDeviceHandler;//0x10
	if(!dw1) return 0;

	DWORD dw2 = *(DWORD*)(dw1 + 0x14);
	if(!dw2) return 0;

	IDirect3DDevice9* nDirect3D = (IDirect3DDevice9*)dw2;
	//__oMsg("%x", (DWORD)nDirect3D);
	//return 0;
	return nDirect3D; 
}

//==================================================================================//
//==================================================================================//
//void World2Screen(D3DXVECTOR4* pPosOutIn)
//{
//	KRenderer *pRender = GetRenderer();
//	if(!pRender) return;
//	
//	D3DXMATRIX		n_m_ViewMatrix = pRender->nViewMatrix;
//	D3DXMATRIX		n_m_ProjectionMatrix = pRender->nProjectionMatrix;
//	
//	D3DXVec4Transform(pPosOutIn, pPosOutIn, &n_m_ViewMatrix);
//	D3DXVec4Transform(pPosOutIn, pPosOutIn, &n_m_ProjectionMatrix);
//
//	pPosOutIn->y *= -1;	
//	//pPosOutIn->y += 250;
//	pPosOutIn->x /= pPosOutIn->w;
//	pPosOutIn->y /= pPosOutIn->w;
//	pPosOutIn->x += 1;
//	pPosOutIn->y += 1;
//	pPosOutIn->x *= (((float)(pRender->nWidth))/2);
//	pPosOutIn->y *= (((float)(pRender->nHeight))/2);
//}

//==================================================================================//
//==================================================================================//
//void World2Screen_ViewPort( D3DXVECTOR3* pPosIn, D3DXVECTOR3* pPosOut ) 
//{ 
	//KRenderer *pRender = GetRenderer();
	//if(!pRender) return;
	//int nWidth  = pRender->nWidth;
	//int nHeight = pRender->nHeight;

	//D3DXMATRIX		n_m_ViewMatrix = pRender->nViewMatrix;
	//D3DXMATRIX		n_m_ProjectionMatrix = pRender->nProjectionMatrix;

	//// Create identity matrix for the world 
	//D3DXMATRIX mWorld;
	//memset( &mWorld, 0, sizeof( mWorld ) );
	//D3DXMatrixIdentity( &mWorld );

	//// Get view port 
	//D3DVIEWPORT9 vp;
	//LPDIRECT3DDEVICE9 nDevice = Get_Device();
	//if(!nDevice) return;

	//nDevice->GetViewport(&vp);
	////__oMsg("			--ViewPort : %d, %d, %d, %d, %f, %f", vp.X, vp.Y, vp.Width, vp.Height, vp.MinZ, vp.MaxZ);
	//// Project 
	//D3DXVec3Project( pPosOut, pPosIn, &vp, &n_m_ProjectionMatrix, &n_m_ViewMatrix, &mWorld ); 

	//pPosOut->x = ( pPosOut->x ) / (nWidth ) * nWidth; 
	//pPosOut->y = ( pPosOut->y ) / (nHeight) * nHeight; 
//}

//==================================================================================//
//==================================================================================//
void WorldPos_to_ScreenPos(KPos* pWorldPos, KPos* pScreenPos)
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = WPOS_2_SPOS_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	DWORD _ECX 		= (DWORD)&pWorldPos->x;
	DWORD _EDX		= (DWORD)&pScreenPos->x;
	__asm{
			push _EDX
			push _ECX
			mov ebx,nFunc
			call ebx
			add esp, 8

			/*mov ecx,_ECX
			mov edx,_EDX
			mov ebx,nFunc
			call ebx*/
	}
}

//==================================================================================//
//==================================================================================//
D3DTLVERTEX CreateD3DTLVERTEX(float X, float Y, float Z, float RHW, D3DCOLOR color, float U, float V)
{
	D3DTLVERTEX v;
	v.fX = X;
	v.fY = Y;
	v.fZ = Z;
	v.fRHW = RHW;
	v.Color = color;
	v.fU = U;
	v.fV = V;
	return v;
}

//==================================================================================//
//==================================================================================//
DWORD nEndSceneFuncion;
DWORD nBackUp_Code1 = 0;
DWORD nBackUp_Code2 = 0;
void Detour_EndScene()
{
	//return;
	if (!IsGameReady())
		return;
	DWORD* vtbl = 0;
	DWORD table = FindPattern((DWORD)GetModuleHandle( (sz__d3d9_dll)), 0x128000,     (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86",  (sz__xx____xx____xx) );
	if (table != 0)
	{
		memcpy(&vtbl, (void*)(table+2), 4);
		nEndSceneFuncion = vtbl[42];
	}
	
	__oMsg("nBackUp_Code1: %x - nBackUp_Code2: %x, nEndSceneFuncion-d3d9 = [%x]", nBackUp_Code1, nBackUp_Code2, nEndSceneFuncion);

	LPDIRECT3DDEVICE9 pDevice =  Get_Device();
	if(!pDevice) return;
	DWORD dw1 = *(DWORD*)pDevice;
	if(!dw1) return;
	nEndSceneFuncion = *(DWORD*)(dw1 + 0xA8);
	__oMsg("nBackUp_Code1: %x - nBackUp_Code2: %x, nEndSceneFuncion-Game = [%x]", nBackUp_Code1, nBackUp_Code2, nEndSceneFuncion);
	if (nEndSceneFuncion)
	{
		if(!nBackUp_Code1 || !nBackUp_Code2)
		{
			nBackUp_Code1 = *(DWORD*) nEndSceneFuncion;
			nBackUp_Code2 = *(DWORD*)(nEndSceneFuncion + 4);
			oEndScene = (EndScene)(DetourFunction((PBYTE)nEndSceneFuncion,(PBYTE)&Hook_EndScene));
		}
	}
}

//==================================================================================//
//==================================================================================//
extern bool  bIs_InitializationSelected;
extern bool  bIs_DrawingSelected;
extern bool  bIs_ActivatorSelected;
extern bool  bIs_CustomLuaSelected;
extern bool  bIs_LoadLuaSelected;
void Retour_EndScene()
{
	//return;
	//bIs_InitializationSelected = false;
	//bIs_DrawingSelected		   = false;
	//bIs_ActivatorSelected	   = false;
	//bIs_CustomLuaSelected	   = false;
	//bIs_LoadLuaSelected        = false;

	if(nBackUp_Code1 && nBackUp_Code2)
	{
		//DWORD* vtbl = 0;     
		//DWORD table = FindPattern((DWORD)GetModuleHandle( (sz__d3d9_dll)), 0x128000,     (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86",  (sz__xx____xx____xx) );
		//if (table != 0) // co van de gi day trong nay
		//{
		//	memcpy(&vtbl, (void*)(table+2), 4);
		//	nEndSceneFuncion = vtbl[42]; 
		//}

		LPDIRECT3DDEVICE9 pDevice =  Get_Device();
		if(!pDevice) return;
		DWORD dw1 = *(DWORD*)pDevice;
		if(!dw1) return;
		nEndSceneFuncion = *(DWORD*)(dw1 + 0xA8);

		HWND  hWnd = FindWindow( (sz__RiotWindowClass), NULL);	
		if(hWnd) 
		{
			DWORD proccess_ID;
			GetWindowThreadProcessId(hWnd, &proccess_ID);
			//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proccess_ID);
			HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, proccess_ID);
			if(hProcess)
			{
				DWORD n_Size = 4;					
				WriteProcessMemory(hProcess, (LPVOID)nEndSceneFuncion, &nBackUp_Code1, n_Size, &n_Size);
				WriteProcessMemory(hProcess, (LPVOID)(nEndSceneFuncion + 4), &nBackUp_Code2, n_Size, &n_Size);
			}
		}
		nBackUp_Code1 = 0;
		nBackUp_Code2 = 0;
	}
}

//==================================================================================//
//==================================================================================//
bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask ) 
			return false;

	return (*szMask) == NULL;
}

//==================================================================================//
//==================================================================================//
DWORD FindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( bCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			return (DWORD)(dwAddress+i);

	return 0;
}

//==================================================================================//
//==================================================================================//
DWORD	GetTexture_Game(char* szTextureName, DWORD nSource)
{
	DWORD nResult = 0;
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GET_TEXTURE_GAME_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	DWORD nTemp1	= BASE_TEXTURE_DDS - TEMP_ENTRY_POINT + hModule;
	DWORD nBaseDDS 	= nTemp1;
	DWORD dw1 = *(DWORD*)nBaseDDS;
	if(!dw1) return 0;
	
	DWORD nTempRender = BASE_RENDERER - TEMP_ENTRY_POINT + hModule;
	DWORD nBaseRender = nTempRender;
	DWORD _ECX = *(DWORD*)nBaseRender;
	if(!_ECX) return 0;

	DWORD nTempDirect = BASE_DIRECTION - TEMP_ENTRY_POINT + hModule;
	DWORD nBaseDirect = nTempDirect;
	

	DWORD nTemp2	= OFFSET_TEXTURE_DDS;
	DWORD nOFF_DDS 	= nTemp2;

	KTextureName* pTextureName = new KTextureName;
	pTextureName->pName = (DWORD*)szTextureName;

	pTextureName->nLen_Name = strlen(szTextureName);
	if		(pTextureName->nLen_Name < 15)	pTextureName->nMaxLen_Name = 15;
	else if	(pTextureName->nLen_Name < 31)  pTextureName->nMaxLen_Name = 31;
	else if	(pTextureName->nLen_Name < 47)  pTextureName->nMaxLen_Name = 47;
	else if	(pTextureName->nLen_Name < 63)  pTextureName->nMaxLen_Name = 63;

	DWORD a1  =  (DWORD)pTextureName;
	DWORD a2  =  dw1 + nOFF_DDS;
	DWORD a3  =  nBaseDirect;
	if(nSource)
	{
		a2 = nSource;
	}

	//__oMsg("[%x] %x - %s - [%s]", &pTextureName->pName, pTextureName->pName, (char*)pTextureName->pName, szTextureName);
	__asm{
			push 1
			push 0
			//push 0
			push a2
			push a1
			mov ecx, _ECX
			mov eax,nFunc
			call eax
			mov nResult, eax
	}
	return nResult;
}

//==================================================================================//
//==================================================================================//
void GameDrawCircle(KPos* pPos, float nRadius, int pTexture, DWORD nColor)// debug = cach chi vao mat so 4 --> ve duong tron mo mo
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GAME_DRAW_CIRCLE_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	DWORD		buff_white[4]	= {nColor , 0x3DCCCCCD,0, 0};
	DWORD   _EAX			= (DWORD)&buff_white[0];
	DWORD	_ECX 			= (DWORD)&pPos->x;
	DWORD	_EDX 			= pTexture;

	float nRad		= nRadius;
	__asm{
			push 0x3F800000
			push 0
			push 0
			push _EAX
			push _EDX
			push nRad
			movss xmm1, nRad
			push _ECX
			mov ebx,nFunc
			call ebx
			add esp, 0x1C

			//mov edx,_EDX
			//push 0x3F800000
			//push 0
			//push 0
			//push _EAX
			//mov ecx,_ECX
			//movss xmm1, n1
			//mov ebx,nFunc
			//call ebx
			//add esp, 0x10
	}
}

//==================================================================================//
//==================================================================================//
void GameDrawCircle_Red(KPos* pPos, float nRadius, int pTexture)
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GAME_DRAW_CIRCLE_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	float  buff_red[4]	= {-1 , 0, 0, 0};
	DWORD _EAX		        = (DWORD)&buff_red[0];
	DWORD _ECX 		= (DWORD)&pPos->x;
	DWORD _EDX 		= pTexture;

	float n1		= nRadius;
	__asm{
			mov edx,_EDX
			push 0x3F800000
			push 0
			push 0
			push _EAX
			mov ecx,_ECX
			movss xmm1, n1
			mov ebx,nFunc
			call ebx
			add esp, 0x10
	}
}

//==================================================================================//
//==================================================================================//
void GameDrawCircle_2ChuaCanDung(KPos* pPos, float nRadius)
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = DrawCircleChuaCanDung_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;
	
	DWORD _ECX 		= (DWORD)&pPos->x;

	float   buff[4]	= {0, 0 , 1, 0};
	DWORD _EAX		= (DWORD)&buff[0];
	DWORD _EDX		= 0x16EF194;// chua lay indicator offset nay
	float n1		= nRadius;
	__asm{
			mov edx,_EDX
			push 0x3F800000
			push 0
			push 0
			push 0
			mov eax,_EAX
			mov ecx,_ECX
			movss xmm1, n1
			mov ebx,nFunc
			call ebx
			add esp, 0x10
	}
}

//==================================================================================//
//==================================================================================//
void GameDrawCircle_MiniMap(KPos* pPos, float nRadius, int pTexture)
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GAMEDRAW_CIRCLE_MiMAP_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;
	//DWORD nFunc 	= 0xDCCE90;	//(v7.5)

	DWORD nTempBASE_DRAW_CIRCLEMINIMAP = BASE_DRAW_CIRCLEMINIMAP - TEMP_ENTRY_POINT + hModule;
	DWORD  dw1 = *(DWORD*)nTempBASE_DRAW_CIRCLEMINIMAP;
	if(!dw1) return;

	DWORD  dw2 = *(DWORD*)(dw1 + DRAWCIRCLEMINIMAP_OFFSET1);
	if(!dw2) return;

	DWORD  dw3 = dw2 + DRAWCIRCLEMINIMAP_OFFSET2;
	DWORD _ECX 		= dw3;

	int   buff[6]	= {0, 0 , 0, 0, 0, 0};
	buff[0]	= *(int*)&pPos->x;
	buff[1]	= *(int*)&pPos->y;
	buff[2]	= *(int*)&pPos->z;
	buff[3]	= *(int*)&nRadius;
	buff[4]	= pTexture;
	buff[5]	= 0xFFFFFFFF;
	DWORD _EAX		= (DWORD)&buff[0];
	__asm{
			push _EAX
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
	}
}

//==================================================================================//
//==================================================================================//
void GameDrawLineMissile(KPos* pSourcePos, KPos* pDestPos, float nRadius, int pTexture)
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GAME_DRAW_LINE_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;
	//DWORD nFunc 	= 0x74C110;			//(v7.5)

	int   buff[2]	= {0xFFFFFFFF, 0x43480000};//200.0
	//float buff[2]	= {-1, 200.0};

	DWORD _EAX		= (DWORD)&buff[0];
	DWORD _ECX 		= (DWORD)&pSourcePos->x;
	DWORD _EDX		= (DWORD)&pDestPos->x;
	float nRad		= nRadius;
	__asm{
			mov eax,_EAX
			push pTexture
			push eax
			movss xmm0, nRad
			push nRad
			push _EDX
			push _ECX
			
			mov ebx,nFunc
			call ebx
			add esp, 0x14

			/*mov eax,_EAX
			push pTexture
			push eax
			mov ecx,_ECX
			mov edx,_EDX
			movss xmm2, n1
			mov ebx,nFunc
			call ebx
			add esp, 8*/
	}
}

//==================================================================================//
//==================================================================================//
KPos	GetWorldPosBar(KNpc* pNpc)
{
	KPos pPos (0,0,0);

	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GET_WPOS_BAR_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	int nBar_Source = pNpc->nBar_Source;
	if(!nBar_Source) return pPos;
	int nThamSo1	= (int)&pPos.x;
	int _ECX 		= nBar_Source;

	__asm{
			//push 0xffffffff
			push nThamSo1
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
	}
	return pPos;

	//----OLD:

	//KPos pPos (0,0,0);
	//
	//DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	//DWORD nTempFunc = GET_WPOS_BAR_FUNC - TEMP_ENTRY_POINT + hModule;
	//DWORD nFunc 	= nTempFunc;

	//int nBar_Source = pNpc->nBar_Source;
	//if(!nBar_Source) return pPos;
	//int nThamSo1	= (int)&pPos.x;
	//int _ECX 		= nBar_Source;
	//
	//__asm{
	//		push 0xffffffff
	//		push nThamSo1
	//		mov ecx,_ECX
	//		mov ebx,nFunc
	//		call ebx
	//}
	//return pPos;
}

//==================================================================================//
//==================================================================================//
int	GetBarDetail(KNpc* pNpc)
{
	return 0;
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GET_BAR_DETAIL_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;
	
	KNpc* pPlayer = GetPlayer();
	if(!pPlayer) return 0;
	int nBar_Source = pNpc->nBar_Source;
	if(!nBar_Source) return 0;
	int dw1 = *(DWORD*)(nBar_Source + 0x14);
	if(!dw1) return 0;
	int _ECX 		= dw1;
	int nBuff[]		= {0,0,0,2,0,0,0};
	int nThamSo3	= (int)&nBuff[0];
	__asm{
			push nThamSo3
			push pPlayer
			push nBar_Source
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
	}
	//__oMsg("nBuff[2] : %x", nBuff[2]);
	return nBuff[2];
}

//==================================================================================//
//==================================================================================//
void	GameDraw2D( float nPosXscreen, float nPosYscreen, float nDai, float nRong, DWORD nColor, int pTexture, int nThamSo1)
{
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = GAME_DRAW_2D_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	int     buff_color[3]	= {nColor, 0x3F800000};
	DWORD _ECX			= (DWORD)&buff_color[0];
	DWORD _EDX			=  pTexture;// ve hinh icon thi cho pTexture vao
	
	__asm{
			push nThamSo1
			push _EDX
			push _ECX

			sub esp, 0x10
			movss xmm0     , nRong
			movss [esp+0xC], xmm0

			movss xmm0    , nDai
			movss [esp+8], xmm0
			
			movss xmm1   , nPosYscreen
			movss [esp+4], xmm1

			movss xmm2 , nPosXscreen
			movss [esp], xmm2
	
			mov ebx,nFunc
			call ebx
			add esp,0x1C

			/*push nThamSo1
			mov ecx,_ECX
			mov edx,_EDX
			movss xmm0, nPosXscreen
			movss xmm1, nPosYscreen
			movss xmm2, nDai
			movss xmm3, nRong
			mov ebx,nFunc
			call ebx
			add esp,0x1C*/
	}
}

//==================================================================================//
//==================================================================================//
KPos	GetScreenPosBar(KNpc* pNpc, bool nIsHP)// dung cho phien ban cu:7.7->7.23
{
	KPos pScreenPosBar;
	KPos pWorldPosBar = GetWorldPosBar(pNpc);
	KPos vScreenPosBar, pPosIn;
	
	KPos result(pWorldPosBar.x, pWorldPosBar.y - 30.0f, pWorldPosBar.z);
	return result;

//---- VKL -------------


	pPosIn.x = pWorldPosBar.x;
	pPosIn.y = pWorldPosBar.y;
	pPosIn.z = pWorldPosBar.z;
	//World2Screen_ViewPort(pPosIn, vScreenPosBar);
	WorldPos_to_ScreenPos(&pPosIn, &vScreenPosBar);

	int nBar_Detail = GetBarDetail(pNpc);

	if(!nBar_Detail) return pScreenPosBar;
	KPos vScreenPosStart_MP, vScreenPosStart_HP;
//----------------------------------------------
	float nDoLech_X_AllBar = 444;
	float nDoLech_Y_AllBar = 666;
	int dw1 = *(int*)(nBar_Detail + 0x10);
	if(dw1)
	{
		nDoLech_X_AllBar = *(float*)(dw1 + 0x64);
		nDoLech_Y_AllBar = *(float*)(dw1 + 0x68);
	}
//----------------------------------------------	
	float nX_1 = -0.5;
	float nX_2 = (float)-0.4;
	int dw4 = *(int*)(pNpc->nBar_Source + 0x34);	// cho nay co the thay doi(lay tu ben ngoai-gan cho Wpos2..)
	if(dw4)
	{
		nX_1 = *(float*) nBar_Detail      + *(float*)(dw4 + 4)     - (float)0.5;
		nX_2 = *(float*)(nBar_Detail + 4) + *(float*)(dw4 + 4 + 4) - (float)0.5;
	}
//----------------------------------------------
	if(nIsHP == false)	// MP
	{
		float nDoLech_X_MP_Bar = 222;
		float nDoLech_Y_MP_Bar = 222;
		int dw2 = *(int*)(nBar_Detail + 0x1C);			// 0x14:HP-Bar , 0x1C:MP-Bar
		if(dw2)
		{
			nDoLech_X_MP_Bar = *(float*)(dw2 + 0x1C);
			nDoLech_Y_MP_Bar = *(float*)(dw2 + 0x20);
		}
		vScreenPosStart_MP.x = (float)(vScreenPosBar.x + nX_1*nDoLech_X_AllBar + nDoLech_X_MP_Bar);
		vScreenPosStart_MP.y = (float)(vScreenPosBar.y + nX_2*nDoLech_Y_AllBar + nDoLech_Y_MP_Bar);

		pScreenPosBar.x = vScreenPosStart_MP.x;
		pScreenPosBar.y = vScreenPosStart_MP.y;
	}
	else		//HP
	{
		float nDoLech_X_HP_Bar = 333;
		float nDoLech_Y_HP_Bar = 333;
		int dw3 = *(int*)(nBar_Detail + 0x14);			// 0x14:HP-Bar , 0x1C:MP-Bar
		if(dw3)
		{
			nDoLech_X_HP_Bar = *(float*)(dw3 + 0x1C);
			nDoLech_Y_HP_Bar = *(float*)(dw3 + 0x20);
		}
		vScreenPosStart_HP.x = (float)(vScreenPosBar.x + nX_1*nDoLech_X_AllBar + nDoLech_X_HP_Bar );
		vScreenPosStart_HP.y = (float)(vScreenPosBar.y + nX_2*nDoLech_Y_AllBar + nDoLech_Y_HP_Bar );

		pScreenPosBar.x = vScreenPosStart_HP.x;
		pScreenPosBar.y = vScreenPosStart_HP.y;
	}
	return pScreenPosBar;
}

//==================================================================================//
KPos	GetScreenPosBarTopCenter(KNpc* pNpc) // dung cho phien ban cu:7.24
{
	KPos pScreenPosBarTopCenter;
	KPos pWorldPosBar = GetWorldPosBar(pNpc);
	KPos vScreenPosBar, pPosIn;

	KPos result(pWorldPosBar.x, pWorldPosBar.y - 30.0f, pWorldPosBar.z);
	return result;

//---- VKL -------------


	pPosIn.x = pWorldPosBar.x;
	pPosIn.y = pWorldPosBar.y;
	pPosIn.z = pWorldPosBar.z;
	//World2Screen_ViewPort(pPosIn, vScreenPosBar);
	WorldPos_to_ScreenPos(&pPosIn, &vScreenPosBar);// neu lay vScreenPosBar thi no la diem trung tam

	int nBar_Detail = GetBarDetail(pNpc);

	if(!nBar_Detail) return pScreenPosBarTopCenter;

	//----------------------------------------------
	float nDoLech_X_AllBar = 444;
	float nDoLech_Y_AllBar = 666;
	int dw1 = *(int*)(nBar_Detail + 0xC);
	if(dw1)
	{
		nDoLech_X_AllBar = *(float*)(dw1 + 0x64);
		nDoLech_Y_AllBar = *(float*)(dw1 + 0x68);
	}
	//----------------------------------------------	
	float nX_1 = -0.5;
	float nX_2 = (float)-0.4;
	int dw4 = *(int*)(pNpc->nBar_Source + 0x34);	// cho nay co the thay doi(lay tu ben ngoai-gan cho Wpos2..)
	if(dw4)
	{
		nX_1 = *(float*) nBar_Detail      + *(float*)(dw4 + 4)     - (float)0.5;
		nX_2 = *(float*)(nBar_Detail + 4) + *(float*)(dw4 + 4 + 4) - (float)0.5;
	}
	//----------------------------------------------
	pScreenPosBarTopCenter.x = vScreenPosBar.x;
	pScreenPosBarTopCenter.y = vScreenPosBar.y + nX_2*nDoLech_Y_AllBar;
	return pScreenPosBarTopCenter;
}

//==================================================================================//
KPos	GetScreenPosBarCenter(KNpc* pNpc) // dung cho phien ban cu:7.24
{
	KPos pWorldPosBar = GetWorldPosBar(pNpc);
	KPos vScreenPosBar, pPosIn;
	
	KPos result(pWorldPosBar.x, pWorldPosBar.y - 30.0f, pWorldPosBar.z);
	return result;

//---- VKL -------------



	pPosIn.x = pWorldPosBar.x;
	pPosIn.y = pWorldPosBar.y;
	pPosIn.z = pWorldPosBar.z;

	WorldPos_to_ScreenPos(&pPosIn, &vScreenPosBar);// neu lay vScreenPosBar thi no la diem trung tam
	return vScreenPosBar;
}

//==================================================================================//
//==================================================================================//
KPos	GetScreenPosMiniMap(KPos* pWorldPos)
{
	KPos pScreenPosMiniMap;
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = WPOS2_SCREENMINI_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;

	DWORD nTempBASE_DRAW_CIRCLEMINIMAP = BASE_DRAW_CIRCLEMINIMAP - TEMP_ENTRY_POINT + hModule;
	DWORD  dw1 = *(DWORD*)nTempBASE_DRAW_CIRCLEMINIMAP;
	if(!dw1) return pScreenPosMiniMap;
	DWORD  dw2 = *(DWORD*)(dw1 + DRAWCIRCLEMINIMAP_OFFSET1);
	if(!dw2) return pScreenPosMiniMap;

	DWORD     a1	= (DWORD)&pWorldPos->x;
	DWORD     a2	= (DWORD)&pScreenPosMiniMap.x;
	DWORD     a3	= (DWORD)&pScreenPosMiniMap.y;
	DWORD     _ECX	= dw2;

	__asm{
			push a3
			push a2
			push a1
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
	}
	//__oMsg("%f - %f", pScreenPosMiniMap.x, pScreenPosMiniMap.y);
	return pScreenPosMiniMap;
}

//==================================================================================//
//==================================================================================//
DWORD	LoadFolderSource_Old(char* szFolderName)
{
	DWORD nResult = 0;
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = LOAD_FOLDER_SOURCE_OLD_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;		//0x96F5F0;			//(v7.7)
	
	DWORD nTempBase = BASE_FOLDER_SOURCE - TEMP_ENTRY_POINT + hModule;
	DWORD _ECX  = nTempBase;
	DWORD* szText = (DWORD*)szFolderName;

	__asm{
		push 0
		push szText
		mov ecx,_ECX
		mov ebx,nFunc
		call ebx
		mov nResult, eax
	}
	//__oMsg("[1] %x",nResult);
	return nResult;
}

//==================================================================================//
//==================================================================================//
DWORD	LoadFolderSource(char* szFolderName)
{
	DWORD nResult = 0;
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempFunc = LOAD_FOLDER_SOURCE_FUNC - TEMP_ENTRY_POINT + hModule;
	DWORD nFunc 	= nTempFunc;		//0x986780;			//(v7.7)

	DWORD nTempBase = BASE_FOLDER_SOURCE - TEMP_ENTRY_POINT + hModule;
	DWORD _ECX		= nTempBase;
	DWORD* szText	= (DWORD*)szFolderName;
	DWORD  Buff[10] = {0};
	DWORD  a3		= (DWORD)&Buff[0];

	__asm{
			push a3
			push 0
			push szText
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
			mov nResult, eax
	}
	//__oMsg("[1] %x",nResult);
	return nResult;
}

//==================================================================================//
KPos GetGameCursorPos()
{
	KPos pCursorPos;
	DWORD hModule	= (DWORD)MyGetModuleHandle( (sz__League_of_Legends_exe));
	DWORD nTempBASE_SKILL  = BASE_SKILL - TEMP_ENTRY_POINT + hModule;
	DWORD dw1 = *(DWORD*)nTempBASE_SKILL;
	if (!dw1) return pCursorPos;
	DWORD dw2 = *(DWORD*)(dw1 + 0x24);
	if (!dw2) return pCursorPos;
	DWORD _ECX 		= dw2;
	pCursorPos.x = (float)(*(DWORD*)(_ECX + 0x68));
	pCursorPos.y = (float)(*(DWORD*)(_ECX + 0x6C));
	return pCursorPos;
}

//==================================================================================//
//==================================================================================//
DWORD	LoadFolderSource_TEST(char* szFolderName)
{
	DWORD nResult = 0;
	DWORD nFunc 	= 0x986780;			//(v7.7)

	DWORD _ECX  = 0x36D1F30;
	DWORD* szText = (DWORD*)szFolderName;
	DWORD Buff[5]  = {0};
	DWORD a3  = (DWORD)&Buff[0];
	__asm{
			push a3
			push 0
			push szText
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
			mov nResult, eax
	}
	//__oMsg("[2] %x",nResult);
	return nResult;
}

//==================================================================================//
//==================================================================================//
void	GameTEST()
{
	return;

	// dang Test ModSkin_HieuUng
	KNpc* pPlayer = GetPlayer();
	if(!pPlayer) return;

	DWORD nFunc 		= 0xB7CE30;
	//DWORD nFunc 		= 0x5AE060;			//(v7.7)
	DWORD nSkinObject	= 0x7d18380;

	DWORD _ECX  = (DWORD)pPlayer;
	DWORD _EDX  = (DWORD)&pPlayer->szPlayerName;
	DWORD a1	= nSkinObject;
	DWORD a2	= (DWORD)&pPlayer->CurrentPos.x;
	DWORD a3	= 0;
	DWORD a4	= 0;
	DWORD a5	= _EDX;
	
	__asm{
			push a1
			push a5
			mov ecx,_ECX
			mov ebx,nFunc
			call ebx
	}

	__asm{
			/*push a4
			push a3
			push a2
			push a1
			mov ecx,_ECX
			mov edx,_EDX
			mov ebx,nFunc
			call ebx
			add esp, 0x10*/
	}
	pPlayer->nTeam_ID = 0x64;
}
//==================================================================================//
//==================================================================================//
void GameDrawMana_Test2()
{
	return;
	//KNpc* pPlayer = GetPlayer();

	//D3DXVECTOR3* vScreen = new D3DXVECTOR3;
	//D3DXVECTOR3* pPosIn  = new D3DXVECTOR3;

	//pPosIn->x = pPlayer->CurrentPos.x;
	//pPosIn->y = pPlayer->CurrentPos.y;
	//pPosIn->z = pPlayer->CurrentPos.z;
	//WorldPos_to_ScreenPos(pPosIn, vScreen);
	////__oMsg("%f - %f", vScreen->x, vScreen->y);
	//DWORD nFunc 	= 0xDAF5E0;			//(v7.7new)

	//float 		buff_1[10]	= {vScreen->x, vScreen->y};

	//DWORD a1			= (DWORD)&buff_1[0];
	//DWORD _ECX			= 0x062E0880 ;
	//float nX			= 1.0;
	//__asm{
	//		push _ECX
	//		push 1
	//		push 0x24bbc944
	//		push a1
	//		mov ecx,_ECX
	//		movss xmm3, nX
	//		mov ebx,nFunc
	//		call ebx
	//}

	//DWORD nFunc 	= 0x5AA430;			//(v7.7new)

	//int 		buff_1[10]	= {0x320031, 0, 0, 0, 4, 7};
	//int			buff_2[10]	= {0xffffffff,0xffffffff, 0xffffffff, 0xffffffff};
	//
	//DWORD a1			= (DWORD)&buff_1[0];
	//DWORD _ECX			= 0x62D44C0 ;
	//float nX			= vScreen->x;
	//float nY			= vScreen->y;
	//__asm{
	//		push 0xff000000
	//		push 0xff000000
	//		push 0xff000000
	//		push 0xffffffff
	//		push 1
	//		push _ECX
	//		push 3
	//		push 1
	//		push 0x7CFE64C
	//		mov ecx,_ECX
	//		movss xmm1, nX
	//		movss xmm2, nY
	//		mov ebx,nFunc
	//		call ebx
	//}
}

//------------------------------	
void    GameDrawMiniMap_Test()
{
	return;
	DWORD nFunc = 0xB0B3A0;
	DWORD color = 0xFFFFFFFF;
	DWORD abc   = (DWORD)&color;
	KPos ppPos = GetScreenPosMiniMap(&GetPlayer()->CurrentPos);
	float nRong = 50;
	float nDai = 50;
	float nPosXscreen = ppPos.x + 25;
	float nPosYscreen = ppPos.y + 25;
	//__oMsg("ppPos: %f, %f", nPosXscreen, nPosYscreen);
	__asm{
			push 0
			push 0x77f4200			//pTexture 13e13500
			push abc
			push 0
			sub esp, 0x10
			movss xmm0     , nRong
			movss [esp+0xC], xmm0

			movss xmm0    , nDai
			movss [esp+8], xmm0

			movss xmm1   , nPosYscreen
			movss [esp+4], xmm1

			movss xmm2 , nPosXscreen
			movss [esp], xmm2
			//push 0x41ADE147
			//push 0x41ADE147
			//push 0x42A9E338
			//push 0x42997940
			mov ebx,nFunc
			call ebx
			add esp,0x20

			push 0
			push 0x77f4200			//pTexture 13e13500
			push abc
			push 0
			sub esp, 0x10
			movss xmm0     , nRong
			movss [esp+0xC], xmm0

			movss xmm0    , nDai
			movss [esp+8], xmm0

			movss xmm1   , nPosYscreen
			movss [esp+4], xmm1

			movss xmm2 , nPosXscreen
			movss [esp], xmm2
			//push 0x41ADE147
			//push 0x41ADE147
			//push 0x42A9E338
			//push 0x42997940
			mov ebx,nFunc
			call ebx
			add esp,0x20
	}
	GameDraw2D( nPosXscreen - 50, nPosYscreen - 50, 50, 50, 0x80FFFFFF, (int)0x13e13500	, 0);
}