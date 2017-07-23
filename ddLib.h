/* ddLib.h */

#define ddLib_CPP

#include <windows.h>

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480
#define SCREEN_BPP          16

#define NUM_SURFACE			10
#define NUM_SPRITES         80
#define MAX_PLANS	        4

struct ddSprite {
	WORD nSurface;
	BOOL lVisible;
	BOOL lActive;
	BYTE nPlan;
	int PosX;
	int PosY;
	WORD ForeColor;
	WORD nType;
	BOOL iTeligence;
	int MaxY;
	int MaxX;
	int MinY;
	int MinX;
	int ResetX;
	int ResetY;
	BOOL ResetAct;

	WORD nTamX;
	WORD nTamY;
	WORD nOrigX;
	WORD nOrigY;
	WORD nRX;
	WORD nRY;
	DWORD nTickWait;
	DWORD nTickEnd;
	DWORD nTickPosEnd;
	DWORD nTickCurr;
	WORD nCurFrame;

	WORD rectX;
};

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
HRESULT WinInit( HINSTANCE hInst, int nCmdShow, HWND* phWnd );
HRESULT Safe_Delete_Display();
BOOL gActiveDisplay();
HRESULT SetDisplay( BOOL Active );
HRESULT TickWait();
HRESULT MySpriteXY( WORD nNumSprite, WORD X, WORD Y );
HRESULT LoadSprite( WORD i, TCHAR *FileBMP, WORD dx, WORD dy );
int Atcve();

//
// Minha Classe DD
//

class dd{

public:
    dd();
	HRESULT Start( HWND hWindow, BOOL lMode );
    ~dd();
	HRESULT Redrall();
	HRESULT Safe_Delete_Display();
	HRESULT SetDisplay( BOOL Active );
	HRESULT TickWait();
	HRESULT MySpriteXY( WORD i, WORD X, WORD Y );
	HRESULT LoadSprite( WORD i, TCHAR *FileBMP, WORD dx, WORD dy );
	BOOL gActiveDisplay();
	void GetRectWin();
	void SetVisible( WORD i, BOOL lVisible);
	void SetActive( WORD i, BOOL lVisible);
	HRESULT TextSprite( WORD i, WORD X, WORD Y, TCHAR *Message, WORD rgbFore );
	void DisplayTextSprite( WORD i, TCHAR *Message );
	void DrawText( WORD i, WORD X, WORD Y, TCHAR *Message );
	WORD CreateInimi( WORD iSurface, int X, int Y, BOOL lAct, WORD nTp, BYTE nPl );
	WORD TipoSprite(WORD i);
	HRESULT MySpriteAddXY( WORD i, int X, int Y );
	void SetIteligence( WORD i, int maX, int maY, int miX, int miY, int rX, int rY, BOOL lAct2 );
	BOOL GetVisible(WORD i);
	int GetMonitor(WORD i,WORD n);
	int GetObjX(WORD i);
	int GetObjY(WORD i);
	void SetAnima( WORD i, WORD nNumX, WORD nNumY, DWORD nTickFrame, DWORD nTickFim );
	void SetRectX(WORD i, WORD xMax);

	HWND hWnd;
	HRESULT hr;
	int lOn;

	RECT                 g_rcViewport;          
	RECT                 g_rcScreen;            
	BOOL                 g_bActive; 
	DWORD                g_dwLastTick;
	BYTE				 ddDefaltPlan;

	BOOL				lChanged;
	BOOL				bWindowed;

};