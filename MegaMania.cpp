/* ======================================================
|| Megamania - 22/11/2002 a 23/11/2002
||    Fábio Ferreira de Souza
||
|| ddLib 1.0 - 20/10/2002 - 23/11/2002
||    Biblioteca de Funções para acessar recursos do DirecDraw do DirecX
||
|| "Fiz, porque nínguem me avisou que era impossível de se fazer"
||    Que Deus que me deu esse dom seja Louvado!
*/

#include "Resource.h"
#include "ddLib.h"
#include "MegaMania.h"

#include <tchar.h>
#include <stdio.h> 

dd Mydd;
WORD nFase=-1;
int nLim=0, iLim=1;
TCHAR cPlacar[100];
BOOL lNewTiro=false;
WORD Inimigos[MAX_INIMIGOS];
int TirosCanhao[MAX_TIROCANHAO];
int TirosInimigo[MAX_TIROINIMIGO];
WORD nLastNave=0;
DWORD ActionTick=0;
BOOL lNewTiroNave=false;
BOOL lDestroi=false;
BOOL lActive=true;
DWORD nLevelTime=300;
DWORD nLevelPass=0;
WORD myVidas[3];
int myVida=3;
int nEnergia=0;

int nPontos=0;

WORD nNave;

// Velocidades dos Tiros e naves
WORD nvTiroCanhao=6;
WORD nvTiroInimigo=4;
WORD nvInimigo=5;

int xPosCanhao=400;

void ValidaPosCanhao(){
	if( lActive ){
		if( xPosCanhao<10 )
			xPosCanhao=10;
		else if( xPosCanhao>590 )
			xPosCanhao=590;
		
		Mydd.MySpriteXY( SP_CANHAO, xPosCanhao, 370 );
		
	}
}

// ----------------------------------------------
// Tela Inicial
//
HRESULT TeleInicial(){
HRESULT hr;
	if( FAILED( hr = Mydd.LoadSprite( SP_FUNDO,
					 MAKEINTRESOURCE( IDB_TELAINICIAL ), 640, 480) ) )
		return hr;

	nFase=0;
	return S_OK;
	}

// ----------------------------------------------
// Fase 1
//
HRESULT Fase1(){
HRESULT hr;
WORD i,im=0;
int iX;
	
	if( FAILED( hr = Mydd.LoadSprite( SP_FUNDO,
					 MAKEINTRESOURCE( IDB_FUNDO ), 640, 480) ) )
		return hr;

	if( FAILED( hr = Mydd.LoadSprite( SP_CANHAO,
					 MAKEINTRESOURCE( IDB_CANHAO ), 80, 50) ) )
		return hr;

	if( FAILED( hr = Mydd.LoadSprite( SP_CANHAOTIRO,
					 MAKEINTRESOURCE( IDB_CANHAOTIRO ), 6, 16) ) )
		return hr;

	if( FAILED( hr = Mydd.LoadSprite( SP_INIMIGO1,
					 MAKEINTRESOURCE( IDB_INIMIGO1 ), 115, 150) ) )
		return hr;

	if( FAILED( hr = Mydd.LoadSprite( SP_BOMBA1,
					 MAKEINTRESOURCE( IDB_BOMBA1 ), 16, 28) ) )
		return hr;

	if( FAILED( hr = Mydd.LoadSprite( SP_EXPLOSAO,
					 MAKEINTRESOURCE( IDB_EXPLOSAO ), 200, 200) ) )
		return hr;

	if( FAILED( hr = Mydd.LoadSprite( SP_ENERGIA,
					 MAKEINTRESOURCE( IDB_ENERGIA ), 363, 17) ) )
		return hr;

	Mydd.MySpriteXY( SP_ENERGIA, 61, 422 );
	Mydd.SetAnima( SP_ENERGIA, 1, 1, 1000, 0 );
	nEnergia=420;

	Mydd.SetVisible( SP_BOMBA1, false);
	Mydd.SetVisible( SP_INIMIGO1, false);
	Mydd.SetVisible( SP_CANHAOTIRO, false);
	Mydd.SetVisible( SP_EXPLOSAO, false);

	Mydd.SetAnima( SP_EXPLOSAO, 2, 2, 100, 3000 );
	Mydd.SetAnima( SP_CANHAO, 2, 1, 300, 0 );

	Mydd.MySpriteXY( SP_CANHAO, xPosCanhao, 370 );
	Mydd.TextSprite( SP_PLACAR, 9, 442, cPlacar, RGB( 255, 0, 0) );

	for( i=0; i<(MAX_INIMIGOS/3); i++ ){
		iX=i*200;
		
		Inimigos[im]=Mydd.CreateInimi( SP_INIMIGO1, iX, 0, true, T_INIMI, 1 );
		Mydd.SetIteligence( Inimigos[im], 1800, 400, -200, 0, -200, 0, true );
		Mydd.SetAnima( Inimigos[im], 1, 3, 500, 0 );
		im++;

		Inimigos[im]=Mydd.CreateInimi( SP_INIMIGO1, iX, 120, true, T_INIMI, 1 );
		Mydd.SetIteligence( Inimigos[im], 1800, 400, -200, 0, -200, 120, true );
		Mydd.SetAnima( Inimigos[im], 1, 3, 500, 0 );
		im++;

		iX+=100;
		if(iX>1800)
			iX=-100;
		Inimigos[im]=Mydd.CreateInimi( SP_INIMIGO1, iX, 60, true, T_INIMI, 1 );
		Mydd.SetIteligence( Inimigos[im], 1800, 400, -200, 0, -200, 60, true );
		Mydd.SetAnima( Inimigos[im], 1, 3, 500, 0 );
		im++;
		
	}

	nNave=im-2;

	for( i=0; i<MAX_TIROCANHAO; i++ ){
		TirosCanhao[i]=Mydd.CreateInimi( SP_CANHAOTIRO, 0, 0, false, T_CANHAOBOMBA, 2 );
		Mydd.SetIteligence( TirosCanhao[i], 700, 480, 0, 0, 0, 0, false );
		}

	for( i=0; i<MAX_TIROINIMIGO; i++ ){
		TirosInimigo[i]=Mydd.CreateInimi( SP_BOMBA1, 0, 0, false, T_INIMIBOMBA, 3 );
		Mydd.SetIteligence( TirosInimigo[i], 700, 410, 0, 0, 0, 0, false );
		Mydd.SetAnima( TirosInimigo[i], 2, 2, 100, 0 );
		}

	myVidas[0]=Mydd.CreateInimi( SP_CANHAO, 480, 420, true, T_VIDA, 3 );
	Mydd.SetAnima( myVidas[0], 2, 1, 20000, 0 );
	myVidas[1]=Mydd.CreateInimi( SP_CANHAO, 530, 420, true, T_VIDA, 3 );
	Mydd.SetAnima( myVidas[1], 2, 1, 20000, 0 );
	myVidas[2]=Mydd.CreateInimi( SP_CANHAO, 580, 420, true, T_VIDA, 3 );
	Mydd.SetAnima( myVidas[2], 2, 1, 20000, 0 );

	_stprintf( cPlacar, _T( "Pontos: %d" ), nPontos );
	Mydd.DisplayTextSprite(SP_PLACAR, cPlacar );

	nFase=1;

	return S_OK;
}

// ----------------------------------------------
// GameOver
//
HRESULT GameOver(){
HRESULT hr;
int i;

	for( i=0; i<NUM_SPRITES; i++ )
		Mydd.SetVisible( i, false);

	Mydd.TextSprite( SP_PLACAR, 20, 300, cPlacar, RGB( 255, 0, 0) );
	Mydd.SetVisible( SP_PLACAR, true );

	if( FAILED( hr = Mydd.LoadSprite( SP_FUNDO,
					 MAKEINTRESOURCE( IDB_GAMEOVER ), 640, 480) ) )
		return hr;

	nFase=2;
	return S_OK;
	}

// ----------------------------------------------
// Inicializa as variáveis do jogo
//
HRESULT GameStart(HWND hWnd){
	HRESULT hr;
#if _DEBUG
	Mydd.Start(hWnd,true);
#else
	Mydd.Start(hWnd,false);
#endif
	Mydd.GetRectWin();

	if( FAILED( hr = Mydd.LoadSprite( SP_FUNDO,
				 MAKEINTRESOURCE( IDB_FUNDO ), 640, 480) ) )
		return hr;

	TeleInicial();
	return S_OK;
}

// ----------------------------------------------
// Finaliza as variáveis do jogo
//
void GameEnd(){
	Mydd.~dd();
}

// ----------------------------------------------
// IA - Aciona a Iteligencia Artificial
// Redral - Executa o jogo
//
BOOL GamePlay(){
	DWORD dwCurrTick, dwDifTick;
	if( Mydd.gActiveDisplay() ){
		if( nFase==1 ){
			if( lActive ){
				if( nEnergia<61 )
					lDestroi=true;
				else
					Mydd.SetRectX( SP_ENERGIA, nEnergia );
			}
			Mydd.SetRectX( SP_ENERGIA, nEnergia );
		    dwCurrTick = timeGetTime();
			dwDifTick = dwCurrTick - Mydd.g_dwLastTick;
		    if( dwDifTick <= 10 )
		        return true; 

			ActionTick += dwDifTick;
			if( ActionTick > nLevelTime ){
				lNewTiroNave=true;
				ActionTick = 0;
				}
			nLevelPass += dwDifTick;
			if( nLevelPass> 800 ){
				nEnergia--;
				nLevelPass=0;
				if( nLevelTime>50 )
					nLevelTime-=5;
				}
			LiveInimi();
			MoveTiroInimigo();
			MoveTiroCanhao();
			AtingeCanhao();
			AtingeInimigo();
			if( lDestroi ){
				lDestroi=false;
				lActive=false;
				Mydd.MySpriteXY( SP_EXPLOSAO, xPosCanhao-30, 330);
				Mydd.SetVisible( SP_EXPLOSAO, true);
				Mydd.SetVisible( SP_CANHAO, false);
			}
			if( !lActive && nPontos<300){
				if( Mydd.GetVisible(SP_EXPLOSAO) )
					xPosCanhao=700;
				else
					LoadLife();
			}
			}
		Mydd.Redrall();
		return true;
		}
	else
		return false;
}

void LoadLife(){
	lActive =true;
	nEnergia=420;
	xPosCanhao=300;
	Mydd.SetVisible( SP_CANHAO, true);
	ValidaPosCanhao();
	myVida--;
	if( myVida<0 )
		GameOver();
	else
		Mydd.SetVisible( myVidas[myVida], false);


}

// ----------------------------------------------
// GU - Game User Interface
//
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg)
    {
		case WM_KEYDOWN:
			switch( nFase ){
			case 0:	// Abertura
				switch( wParam ){
				case VK_ESCAPE:
					PostMessage( hWnd, WM_CLOSE, 0, 0 );
					break;
				case VK_RETURN:
					Fase1();
					break;
				}
				break;
			case 1: // Fase 1
				switch( wParam ){
				case VK_ESCAPE:
					GameOver();
					break;
				case VK_RIGHT:
					xPosCanhao+=5;
					ValidaPosCanhao();
					break;
				case VK_LEFT:
					xPosCanhao-=5;
					ValidaPosCanhao();
					break;
				case VK_SPACE:
					lNewTiro=true;
					break;
				}
				break;
			case 2:	// Final
				switch( wParam ){
				case VK_ESCAPE:
					PostMessage( hWnd, WM_CLOSE, 0, 0 );
					break;
				case VK_RETURN:
					PostMessage( hWnd, WM_CLOSE, 0, 0 );
					break;
				}
				break;
			}
			return TRUE;
			break; // Continue with default processing

		case WM_MOUSEMOVE:
			if( nFase==1 ){
				xPosCanhao=(WORD) ( 0xFFFF & lParam );
				xPosCanhao-=23;
				ValidaPosCanhao();
				}
			return TRUE;

		case WM_LBUTTONDOWN:
			switch( nFase ){
			case 0: // Tela Inicial
				Fase1();
				break;
			case 1: // Durante o Jogo
				lNewTiro=true;
				break;
			}
			return TRUE;
		
		case WM_MOVE:
            // Retrieve the window position after a move.  
			Mydd.GetRectWin();
            return 0L;

        case WM_SIZE:
            // Check to see if we are losing our window...
            if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
                Mydd.SetDisplay( FALSE );
            else
				Mydd.SetDisplay( TRUE );

			Mydd.GetRectWin();

            break;

        case WM_EXITMENULOOP:
            // Ignore time spent in menu
            break;
            
        case WM_DESTROY:
            // Cleanup and close the app
            PostQuitMessage( 0 );
            return 0L;
   }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void LiveInimi(){
	int i;
	for( i=0; i<MAX_INIMIGOS; i++ ){
		Mydd.MySpriteAddXY(Inimigos[i],nvInimigo,iLim);
	}
	nLim+=iLim;
	if(nLim>50 || nLim<0){
		iLim=-iLim;
		nLim+=iLim;
	}
}

void MoveTiroCanhao(){
	int i;
	if( !lActive )
		lNewTiro = false;

	for( i=0; i<MAX_TIROCANHAO; i++ ){
		if( Mydd.GetVisible( TirosCanhao[i] ) ){
			Mydd.MySpriteAddXY(TirosCanhao[i],0,-nvTiroCanhao );
			}
		else if( lNewTiro ){
			nEnergia-=5;
			Mydd.MySpriteXY(TirosCanhao[i],xPosCanhao+18,380);
			Mydd.SetVisible(TirosCanhao[i],true);
			lNewTiro=false;
		}
	}
}

void MoveTiroInimigo(){
	int i;
	int nX,nY;
	for( i=0; i<MAX_TIROINIMIGO; i++ ){
		if( Mydd.GetVisible( TirosInimigo[i] ) ){
			Mydd.MySpriteAddXY(TirosInimigo[i],0,nvTiroInimigo);
			}
		else if( lNewTiroNave ){
			while( nLastNave<MAX_INIMIGOS ){
				if( Mydd.GetVisible(Inimigos[nLastNave]) ){
					nX=Mydd.GetObjX(Inimigos[nLastNave]);
					if( nX>(xPosCanhao-80) && nX<(xPosCanhao) ){
						nY=Mydd.GetObjY(Inimigos[nLastNave]);
						Mydd.MySpriteXY(TirosInimigo[i],nX+58,nY+50);
						Mydd.SetVisible(TirosInimigo[i],true);
						break;
					}
				}
				nLastNave++;
			}
			if( nLastNave>=MAX_INIMIGOS )
				nLastNave=0;
			lNewTiroNave=false;
		}
	}
}

void AtingeCanhao(){
int i, bX;
	if( !lActive )
		return;

	for( i=0; i<MAX_TIROINIMIGO; i++ )
		if( Mydd.GetVisible( TirosInimigo[i] ) )
			if( Mydd.GetObjY( TirosInimigo[i] )>370 ){
				bX=Mydd.GetObjX( TirosInimigo[i] );
				if( bX>(xPosCanhao-5) && bX<(xPosCanhao+40) )
					lDestroi=true;
			}
}

void AtingeInimigo(){
int i, n, cX, cY;
int nX, nY;
	for( i=0; i<MAX_TIROCANHAO; i++ ){
		if( Mydd.GetVisible( TirosCanhao[i] ) ){
			cY=Mydd.GetObjY( TirosCanhao[i] );
			if( cY<300 ){
				for( n=0; n<MAX_INIMIGOS; n++ ){
					if( Mydd.GetVisible( Inimigos[n] ) ){
						cX=Mydd.GetObjX( TirosCanhao[i] );
						nX=Mydd.GetObjX( Inimigos[n] );
						nY=Mydd.GetObjY( Inimigos[n] );
						if( (cX>nX && cX<(nX+120)) && (cY>nY && cY<(nY+50)) ){
							Mydd.SetActive( Inimigos[n], false );
							Mydd.SetVisible( TirosCanhao[i], false );
							nPontos+=10;
							_stprintf( cPlacar, _T( "Pontos: %d" ), nPontos );
							Mydd.DisplayTextSprite(SP_PLACAR, cPlacar );
							if( nPontos>=300 ){
								Congratulations();
							}
						}
					}
				}
			}
		}
	}
}

void Congratulations(){
	lActive=false;
	Mydd.SetVisible( SP_CANHAO, false);
	Mydd.DrawText( SP_FUNDO, 150, 200, "PARABENS VOCÊ ELIMINOU TODOS OS INIMIGOS" );
}
