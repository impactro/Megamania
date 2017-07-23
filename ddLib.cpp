/******************************************************************************
** DDLIB.CPP
**
** Biblioteca de Funções para acessar recursos do DirecDraw do DirecX
** Fábio Ferreira de Souza - 20/10/2002 - 23/11/2002
**
******************************************************************************/

#include <tchar.h>
#include <stdio.h> 
#include <ddraw.h>
#include <mmsystem.h>
#include <ddutil.h>

#include "ddLib.h"

CSurface*            g_pSpriteSurface[NUM_SURFACE];
CDisplay*            g_pDisplay;
ddSprite			 g_ObjSprite[NUM_SPRITES];
WORD				 dLastObjSprite=NUM_SURFACE+1;

int nMonitor[NUM_SPRITES][5];

dd::dd(){
	lOn=false;
	g_pDisplay=NULL;
	g_bActive=NULL;
	ddDefaltPlan=3;
	lChanged=true;
	}

HRESULT dd::Start( HWND hWindow, BOOL lMode ){
	int i;

	hWnd=hWindow;
	bWindowed=lMode;
	g_dwLastTick = timeGetTime();

	g_pDisplay = new CDisplay();

	for(i=0;i<NUM_SPRITES;i++)g_ObjSprite[i].lVisible=false;


    if( bWindowed )
    {
        if( FAILED( hr = g_pDisplay->CreateWindowedDisplay( hWnd, SCREEN_WIDTH, 
                                                            SCREEN_HEIGHT ) ) )
            return hr;

        // Add the system menu to the window's style
        DWORD dwStyle = GetWindowLong( hWnd, GWL_STYLE );
        SetWindowLong( hWnd, GWL_STYLE, dwStyle );
    }
    else
    {
    	hr = g_pDisplay->CreateFullScreenDisplay( hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP );
		if( FAILED( hr ) ){
			return hr;
		}
    }


	lOn=true;

	SetDisplay( true );
	lChanged=true;

	ShowCursor(false);

	return S_OK;

	}

dd::~dd(){
	int i;
	for( i=0; i<NUM_SURFACE; i++ )
		SAFE_DELETE( g_pSpriteSurface[i] );

    SAFE_DELETE( g_pDisplay );
	ShowCursor(true);
	}


HRESULT dd::Redrall()
{
	WORD i,nPlan, nPosX2;
	int nRecX;
	RECT rp;

	if( !lChanged )
		return S_OK;
	
	lChanged=false;

    // Figure how much time has passed since the last time
    DWORD dwCurrTick = timeGetTime();
    DWORD dwTickDiff = dwCurrTick - g_dwLastTick;

    // Don't update if no time has passed 
    if( dwTickDiff <= 10 )
        return S_OK; 

    g_dwLastTick = dwCurrTick;

    g_pDisplay->Clear( 0 );
   
	g_pDisplay->Blt( 0, 0, g_pSpriteSurface[0], NULL );

	for( nPlan=1; nPlan<MAX_PLANS; nPlan++ ){
		for( i=1; i<NUM_SPRITES; i++ ){
			if( g_ObjSprite[i].nPlan==nPlan ){
				if( g_ObjSprite[i].lVisible )
					if( g_ObjSprite[i].nTickWait==0){
						g_pDisplay->Blt( g_ObjSprite[i].PosX, g_ObjSprite[i].PosY, g_pSpriteSurface[g_ObjSprite[i].nSurface] );
						}
					else {
						WORD nCol = g_ObjSprite[i].nCurFrame % g_ObjSprite[i].nRX;
						WORD nRow = g_ObjSprite[i].nCurFrame / g_ObjSprite[i].nRX;
						
						rp.top=nRow * g_ObjSprite[i].nTamY;
						rp.left=nCol * g_ObjSprite[i].nTamX;
						rp.bottom=rp.top+g_ObjSprite[i].nTamY;
						rp.right=rp.left+g_ObjSprite[i].nTamX;

						nPosX2=g_ObjSprite[i].PosX;

						if( g_ObjSprite[i].PosX+g_ObjSprite[i].nTamX>0 ){
							if( g_ObjSprite[i].PosX<0 ){
								nRecX=-g_ObjSprite[i].PosX;
								rp.left=nRecX;
								nPosX2=0;
								}
							}

						if( (g_ObjSprite[i].PosX+g_ObjSprite[i].nTamX)>g_ObjSprite[i].rectX ){
							nRecX=g_ObjSprite[i].rectX-g_ObjSprite[i].PosX;
							if( nRecX>0 )
								rp.right=nRecX;
							}

						g_pDisplay->Blt( nPosX2, g_ObjSprite[i].PosY, g_pSpriteSurface[g_ObjSprite[i].nSurface], &rp );
						
						g_ObjSprite[i].nTickCurr+=dwTickDiff;
						g_ObjSprite[i].nTickPosEnd+=dwTickDiff;
						
						if( g_ObjSprite[i].nTickEnd>0 && g_ObjSprite[i].nTickPosEnd>g_ObjSprite[i].nTickEnd ){
							g_ObjSprite[i].nTickPosEnd=0;
							g_ObjSprite[i].lVisible=false;
							}

						else if( g_ObjSprite[i].nTickCurr>g_ObjSprite[i].nTickWait ){
							g_ObjSprite[i].nTickCurr=0;
							g_ObjSprite[i].nCurFrame++;
							if( g_ObjSprite[i].nCurFrame>=(g_ObjSprite[i].nRX*g_ObjSprite[i].nRY) )
								g_ObjSprite[i].nCurFrame=0;
							}
						}
			}
		}
	}

    if( FAILED( hr = g_pDisplay->Present() ) )
        return hr;

    return S_OK;
}


HRESULT dd::Safe_Delete_Display(){
	SAFE_DELETE( g_pDisplay );
	return S_OK;
}

BOOL dd::gActiveDisplay(){
	return g_bActive;
}

HRESULT dd::SetDisplay( BOOL Active ){
	g_bActive=Active;
	return S_OK;
}


HRESULT dd::TickWait(){
	g_dwLastTick = timeGetTime();
	return S_OK;
}

HRESULT dd::MySpriteXY( WORD i, WORD X, WORD Y ){
	g_ObjSprite[i].PosX=X;
	g_ObjSprite[i].PosY=Y;
	lChanged=true;
	return S_OK;
}

HRESULT dd::MySpriteAddXY( WORD i, int X, int Y ){
	if( !( g_ObjSprite[i].lActive ) )
		return S_OK;
	g_ObjSprite[i].PosX+=X;
	g_ObjSprite[i].PosY+=Y;
	nMonitor[i][0]=g_ObjSprite[i].PosX;
	nMonitor[i][1]=g_ObjSprite[i].MinX;
	if( g_ObjSprite[i].iTeligence ){
		if( g_ObjSprite[i].PosX>g_ObjSprite[i].MaxX || g_ObjSprite[i].PosX<g_ObjSprite[i].MinX ){
			g_ObjSprite[i].PosX=g_ObjSprite[i].ResetX;
			g_ObjSprite[i].lVisible=g_ObjSprite[i].ResetAct;
			}

		if( g_ObjSprite[i].PosY>g_ObjSprite[i].MaxY || g_ObjSprite[i].PosY<g_ObjSprite[i].MinY ){
			g_ObjSprite[i].PosY=g_ObjSprite[i].ResetY;
			g_ObjSprite[i].lVisible=g_ObjSprite[i].ResetAct;
			}
		}

	lChanged=true;
	return S_OK;
}

HRESULT dd::LoadSprite( WORD i, TCHAR *FileBMP, WORD dx, WORD dy ){
HRESULT hr;
DWORD dwColorKey=0;

	SAFE_DELETE( g_pSpriteSurface[i] );

	g_ObjSprite[i].iTeligence=false;
	g_ObjSprite[i].nSurface=i;
	g_ObjSprite[i].lVisible=true;
	g_ObjSprite[i].PosX=0;
	g_ObjSprite[i].PosY=0;
	g_ObjSprite[i].nPlan=ddDefaltPlan;
	g_ObjSprite[i].nOrigX=dx;
	g_ObjSprite[i].nOrigY=dy;
	g_ObjSprite[i].nRX=1;
	g_ObjSprite[i].nRY=1;
	g_ObjSprite[i].nTickWait=0;
	g_ObjSprite[i].lActive=true;
	g_ObjSprite[i].rectX=640;

    hr = g_pDisplay->CreateSurfaceFromBitmap( &g_pSpriteSurface[i], FileBMP, dx, dy );
	if( i>0 )
		g_pSpriteSurface[i]->SetColorKey( dwColorKey );
	lChanged=true;
    return hr;
}

void dd::SetAnima( WORD i, WORD nNumX, WORD nNumY, DWORD nTickFrame, DWORD nTickFim ){
	g_ObjSprite[i].nTamX=g_ObjSprite[g_ObjSprite[i].nSurface].nOrigX/nNumX;
	g_ObjSprite[i].nTamY=g_ObjSprite[g_ObjSprite[i].nSurface].nOrigY/nNumY;
	g_ObjSprite[i].nRX=nNumX;
	g_ObjSprite[i].nRY=nNumY;
	g_ObjSprite[i].nTickWait=nTickFrame;
	g_ObjSprite[i].nCurFrame=0;
	g_ObjSprite[i].nTickEnd=nTickFim;
	g_ObjSprite[i].nTickPosEnd=0;
	g_ObjSprite[i].lActive=true;
	g_ObjSprite[i].rectX=640;
}

HRESULT dd::TextSprite( WORD i, WORD X, WORD Y, TCHAR *Message, WORD rgbFore ){
HRESULT hr;
DWORD dwColorKey=0;

	SAFE_DELETE( g_pSpriteSurface[i] );
	g_ObjSprite[i].iTeligence=false;
	g_ObjSprite[i].nSurface=i;
	g_ObjSprite[i].lVisible=false;
	g_ObjSprite[i].PosX=X;
	g_ObjSprite[i].PosY=Y;
	g_ObjSprite[i].ForeColor=rgbFore;
	g_ObjSprite[i].nPlan=ddDefaltPlan;
	g_ObjSprite[i].nRX=1;
	g_ObjSprite[i].nRY=1;
	g_ObjSprite[i].nTickWait=0;
	g_ObjSprite[i].lActive=true;
	g_ObjSprite[i].rectX=640;

    hr = g_pDisplay->CreateSurfaceFromText( &g_pSpriteSurface[i], NULL, Message, 0, rgbFore );
	g_pSpriteSurface[i]->SetColorKey( dwColorKey );
	lChanged=true;
    return hr;
}

void dd::DisplayTextSprite( WORD i, TCHAR *Message ){
	g_ObjSprite[i].lVisible=true;
	g_pSpriteSurface[i]->DrawText( NULL, Message, 0, 0, 0, g_ObjSprite[i].ForeColor );
	lChanged=true;
}

void dd::DrawText( WORD i, WORD X, WORD Y, TCHAR *Message ){
	g_pSpriteSurface[i]->DrawText( NULL, Message, X, Y, 0, RGB(0,255,0) );
	lChanged=true;
}


void dd::SetVisible( WORD i, BOOL lVisible){
	g_ObjSprite[i].lVisible=lVisible;
	lChanged=true;
}

void dd::SetActive( WORD i, BOOL lAct){
	g_ObjSprite[i].lActive=lAct;
	SetVisible( i, lAct );
	lChanged=true;
}

void dd::GetRectWin(){
	if( !lChanged && g_bActive ){
		g_pDisplay->UpdateBounds();
		lChanged=true;
	}
}

WORD dd::CreateInimi( WORD iSurface, int X, int Y, BOOL lAct, WORD nTp, BYTE nPl ){
	WORD i;
	i=dLastObjSprite;
	g_ObjSprite[i].iTeligence=false;
	g_ObjSprite[i].nSurface=iSurface;
	g_ObjSprite[i].PosX=X;
	g_ObjSprite[i].PosY=Y;
	g_ObjSprite[i].lVisible=lAct;
	g_ObjSprite[i].nType=nTp;
	g_ObjSprite[i].nPlan=nPl;
	g_ObjSprite[i].lActive=true;
	g_ObjSprite[i].rectX=640;
	dLastObjSprite++;
	return i;
}

WORD dd::TipoSprite(WORD i){
	return g_ObjSprite[i].nType;
}

BOOL dd::GetVisible(WORD i){
	return g_ObjSprite[i].lVisible;
}

void dd::SetIteligence( WORD i, int maX, int maY, int miX, int miY, int rX, int rY, BOOL lAct2 ){
	g_ObjSprite[i].iTeligence=true;
	g_ObjSprite[i].MaxX=maX;
	g_ObjSprite[i].MaxY=maY;
	g_ObjSprite[i].MinX=miX;
	g_ObjSprite[i].MinY=miY;
	g_ObjSprite[i].ResetX=rX;
	g_ObjSprite[i].ResetY=rY;
	g_ObjSprite[i].ResetAct=lAct2;
	g_ObjSprite[i].lActive=true;
}

int dd::GetMonitor(WORD i, WORD n){
	return nMonitor[i][n];
}


int dd::GetObjX(WORD i){
	return g_ObjSprite[i].PosX;
}

int dd::GetObjY(WORD i){
	return g_ObjSprite[i].PosY;
}

void dd::SetRectX(WORD i, WORD xMax){
	g_ObjSprite[i].rectX=xMax;
}
