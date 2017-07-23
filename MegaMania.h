#define SP_FUNDO		0
#define SP_CANHAO		1
#define SP_CANHAOTIRO	2
#define SP_INIMIGO1		3
#define SP_BOMBA1		4
#define SP_TELAINICIO	5
#define SP_TELAFIM		6
#define SP_PLACAR		7
#define SP_EXPLOSAO		8
#define SP_ENERGIA		9

#define T_INIMI			1
#define T_INIMIBOMBA	2
#define T_CANHAOBOMBA	3
#define T_VIDA			4

#define MAX_TIROCANHAO	7
#define MAX_INIMIGOS	30
#define MAX_TIROINIMIGO	10

HRESULT TeleInicial();
HRESULT Fase1();
HRESULT GameOver();
HRESULT GameStart(HWND hWnd);
void GameEnd();
BOOL GamePlay();

void LiveInimi();
void MoveTiroCanhao();
void MoveTiroInimigo();

void AtingeCanhao();
void LoadLife();
void AtingeInimigo();

void Congratulations();