/* WinMain Commum Lib */

#include <windows.h>


HRESULT WinInit( HINSTANCE hInst, int nCmdShow, HWND* phWnd );
int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow );
HRESULT MainLoop(HWND hWnd);

#define IDI_MAIN                        102
#define IDB_BITMAP1                     108
#define IDB_BITMAP2                     110
#define IDB_BITMAP3                     111
#define IDB_BITMAP4                     112
#define IDR_MAIN_ACCEL                  103
#define IDM_EXIT                        66573
#define IDM_UP							105537
#define IDM_BUTTOM                      1003