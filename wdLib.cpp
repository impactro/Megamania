//-----------------------------------------------------------------------------
// Name: WinMain()
//-----------------------------------------------------------------------------

#include "ddLib.h"
#include "MegaMania.h"
#include "wdLib.h"

#include <tchar.h>
#include <stdio.h> 


// Main

int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow )
{
    HWND     hWnd;

    srand( GetTickCount() );

    if( FAILED( WinInit( hInst, nCmdShow, &hWnd ) ) )
        return FALSE;

	MainLoop(hWnd);

	return 0;

}

// Main LOOP

HRESULT MainLoop(HWND hWnd){
    MSG		 msg;

	GameStart(hWnd);

	while( TRUE ){

        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            if( 0 == GetMessage(&msg, NULL, 0, 0 ) )
            {
                // WM_QUIT was posted, so exit
				return (int)msg.wParam;
            }

            // Translate and dispatch the message
            else {
                TranslateMessage( &msg ); 
                DispatchMessage( &msg );
            }

        }
        else
        {
            if( !GamePlay() ){
                // Make sure we go to sleep if we have nothing else to do
                WaitMessage();
            }
        }
    }

	GameEnd();

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: WinInit()
// Desc: Init the window
//-----------------------------------------------------------------------------
HRESULT WinInit( HINSTANCE hInst, int nCmdShow, HWND* phWnd )
{
    WNDCLASSEX wc;
    HWND       hWnd;

    // Register the window class
    wc.cbSize        = sizeof(wc);
    wc.lpszClassName = TEXT("MegaMania");
    wc.lpfnWndProc   = MainWndProc;
    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon( hInst, MAKEINTRESOURCE(IDI_MAIN) );
    wc.hIconSm       = LoadIcon( hInst, MAKEINTRESOURCE(IDI_MAIN) );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;

    if( RegisterClassEx( &wc ) == 0 )
        return E_FAIL;

    // Calculate the proper size for the window given a client of 640x480
    DWORD dwFrameWidth    = GetSystemMetrics( SM_CXSIZEFRAME );
    DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );
    DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );
    DWORD dwWindowWidth   = SCREEN_WIDTH  + dwFrameWidth * 2;
    DWORD dwWindowHeight  = SCREEN_HEIGHT + dwFrameHeight * 2 + dwCaptionHeight;

    // Create and show the main window
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
    hWnd = CreateWindowEx( 0, TEXT("MegaMania"), 
                           TEXT("MegaMania"),
                           dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
  	                       dwWindowWidth, dwWindowHeight, NULL, NULL, hInst, NULL );
    if( hWnd == NULL )
    	return E_FAIL;

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    // Save the window size/pos for switching modes

    *phWnd   = hWnd;

    return S_OK;

}


