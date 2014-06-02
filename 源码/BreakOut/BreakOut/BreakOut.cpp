#include <hge.h>
#include <hgegui.h>
#include <hgesprite.h>
#include <hgeparticle.h>
#include <hgeanim.h>
#include <hgevector.h>
#include <hgefont.h>
#include <cstdio>
#include <iostream>
#include <time.h>
#include "menuitem.h"

const int GAME_LOGO = 0;
const int GAME_MENU = 1;
const int GAME_SELECT = 2;
const int GAME_READY = 3;
const int GAME_START = 4;
const int GAME_OVER = 5;

const int BLOCK_BLANK = 0;
const int BLOCK_RED = 1;
const int BLOCK_GREEN = 2;
const int BLOCK_BLUE = 3;
const int BLOCK_YELLOW = 4;
const int BLOCK_ORANGE = 5;
const int BLOCK_PINK = 6;
const int BLOCK_WHITE = 7;


const int scrWidth = 800, scrHeight = 600;

struct flatP
{
	int cx;
	int cy;
	int dx;
}flatPos;

struct ballP
{
	int cx;
	int cy;
	int dx;
	int dy;
}ballPos;

HGE*           pHge     =    0;
hgeGUI*        pGui     =    0;
hgeFont*       Fnt      =    0;
hgeSprite*     pCursor  =    0;
hgeSprite*     pBack    =    0;
hgeSprite*     pRblock  =    0;
hgeSprite*     pGblock  =    0;
hgeSprite*     pWblock  =    0;
hgeSprite*     pBblock  =    0;
hgeSprite*     pYblock  =    0;
hgeSprite*     pOblock  =    0;
hgeSprite*     pPblock  =    0;
hgeSprite*     pFlat    =    0;
hgeSprite*     pBall    =    0;
hgeSprite*     pReady   =    0;
hgeSprite*     pHps     =    0;

hgeParticleSystem* pHpar =    0;

hgeQuad           Qlogo;
HEFFECT			  menuSnd;
HEFFECT           backSnd;
HTEXTURE          mouse;
HTEXTURE          back;
HTEXTURE          rblock;
HTEXTURE          gblock;
HTEXTURE          bblock;
HTEXTURE          wblock;
HTEXTURE          yblock;
HTEXTURE          oblock;
HTEXTURE          pblock;
HTEXTURE          flat;
HTEXTURE          ball;
HTEXTURE          ready;


float dTime;

int gameState;//ÓÎÏ·×´Ì¬
int lastId, curId;
int blocks[40][20];
int ballHeight, ballWidth;
int blankHeight;
int flatWidth, flatHeight;
int blockHeight, blockWidth;
int gameScore, gameLife, gameAgain;
bool sDone;
bool gameEnd;

void InitQuad()
{
	int x=pHge->System_GetState(HGE_SCREENWIDTH)/2;
	int y=pHge->System_GetState(HGE_SCREENHEIGHT)/2;
	Qlogo.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	for(int i = 0; i < 4; i ++)
	{
		Qlogo.v[i].z = 0.5f;
		Qlogo.v[i].col = 0xFFFFFFFF;
	}
	Qlogo.v[0].tx=0.0f; Qlogo.v[0].ty=0.0f;
	Qlogo.v[1].tx=1.0f; Qlogo.v[1].ty=0.0f;
	Qlogo.v[2].tx=1.0f; Qlogo.v[2].ty=1.0f;
	Qlogo.v[3].tx=0.0f; Qlogo.v[3].ty=1.0f;

	Qlogo.v[0].x=x-256.0f; Qlogo.v[0].y=y-256.0f;
	Qlogo.v[1].x=x+256.0f; Qlogo.v[1].y=y-256.0f;
	Qlogo.v[2].x=x+256.0f; Qlogo.v[2].y=y+256.0f;
	Qlogo.v[3].x=x-256.0f; Qlogo.v[3].y=y+256.0f;
	dTime = 0;
}

void LoadSetup()
{
	srand((unsigned)time(NULL));
	flatPos.cx = scrWidth / 2;
	flatPos.cy = 550;
	flatPos.dx = 10;
	ballPos.cx = flatPos.cx;
	ballPos.cy = flatPos.cy - 32;
	ballPos.dx = 0;
	ballPos.dy = -6 - rand() % 2;
	ballHeight = 16;
	ballWidth = 16;
	blankHeight = 40;
	flatWidth = 45;
	flatHeight = 12;
	blockHeight = 20;
	blockWidth = 40;
	gameScore = 0;
	lastId = curId = 0;
}

void InitMap()
{
	for(int i =0; i < 30; i ++)
			for(int j = 0; j < 20; j ++)
			blocks[i][j] = 0;
}

bool CheckMap()
{
	bool flag = false;
	for(int i =0; i < 30; i ++)
    {
        for(int j = 0; j < 20; j ++)
			if(blocks[i][j])flag = true;
    }
	return flag;
}

void ReadMap()
{
	FILE *file;
	int num,i = 0,j = 0;
	file = fopen("map.txt","r");
	if(!file)MessageBox(NULL, "Can't load map", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	while(!feof(file))
	{
	    for(int i =0; i < 30; i ++)
        fscanf(file,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
		&blocks[i][0], &blocks[i][1], &blocks[i][2], &blocks[i][3], &blocks[i][4], 
		&blocks[i][5], &blocks[i][6], &blocks[i][7], &blocks[i][8], &blocks[i][9],
		&blocks[i][10], &blocks[i][11], &blocks[i][12], &blocks[i][13], &blocks[i][14], 
		&blocks[i][15], &blocks[i][16], &blocks[i][17], &blocks[i][18], &blocks[i][19]);
	}
	fclose(file);
}

void DrawMap()
{
	for(int i =0; i < 30; i ++)
        for(int j = 0; j < 20; j ++)
		{
			switch(blocks[i][j])
			{
			case BLOCK_RED:
				pRblock->Render(j* 40, 40 + i * 20);
				break;
			case BLOCK_GREEN:
				pGblock->Render(j* 40, 40 + i * 20);
				break;
			case BLOCK_BLUE:
				pBblock->Render(j* 40, 40 + i * 20);
				break;
			case BLOCK_YELLOW:
				pYblock->Render(j* 40, 40 + i * 20);
				break;
			case BLOCK_ORANGE:
				pOblock->Render(j* 40, 40 + i * 20);
				break;
			case BLOCK_PINK:
				pPblock->Render(j* 40, 40 + i * 20);
				break;
			case BLOCK_WHITE:
				pWblock->Render(j* 40, 40 + i * 20);
				break;
			default:
				blocks[i][j] = 0;
				break;
			}
		}
}

void Cleanup()
{
	if(mouse)pHge->Texture_Free(mouse);
	if(Qlogo.tex)pHge->Texture_Free(Qlogo.tex);
	if(menuSnd)pHge->Effect_Free(menuSnd);
	if(rblock)pHge->Effect_Free(rblock);
	if(gblock)pHge->Effect_Free(gblock);
	if(bblock)pHge->Effect_Free(bblock);
	if(wblock)pHge->Effect_Free(wblock);
	if(yblock)pHge->Effect_Free(yblock);
	if(oblock)pHge->Effect_Free(oblock);
	if(pblock)pHge->Effect_Free(pblock);
	if(flat)pHge->Effect_Free(flat);
	if(back)pHge->Effect_Free(back);
	if(ball)pHge->Effect_Free(ball);

	if(pGui!=NULL)delete pGui;
	if(Fnt!=NULL)delete Fnt;
	if(pCursor!=NULL)delete pCursor;
	if(pWblock!= NULL)delete pWblock;
	if(pRblock!= NULL)delete pRblock;
	if(pGblock!= NULL)delete pGblock;
	if(pBblock!= NULL)delete pBblock;
	if(pYblock!= NULL)delete pYblock;
	if(pOblock!= NULL)delete pOblock;
	if(pPblock!= NULL)delete pPblock;
	if(pFlat != NULL)delete pFlat;
	if(pBall != NULL)delete pBall;
	if(pHpar != NULL)delete pHpar;
	if(pHps != NULL)delete pHps;
}

bool RenderFunc()
{
	BYTE alpha;
	DWORD col;
	float dt = pHge->Timer_GetDelta();
	if(gameState == GAME_MENU || gameState == GAME_LOGO)
		pGui->SetCursor(NULL);
	else pGui->SetCursor(pCursor);
	switch(gameState)
	{
	case GAME_LOGO:
		dTime += dt;
		if(dTime<=0.25) alpha=(BYTE)((dTime*4)*0xFF);
		else if(dTime<=2.0) alpha=0xFF;
		else if(dTime<=2.25) alpha=(BYTE)((1.0f-(dTime-2.0f)*4)*0xFF);
		else
		{
			gameState = GAME_MENU;
			dTime = 0;
			sDone = true;
			break;
		}
		if(!sDone)
		{
			col=0xFFFFFF | (alpha<<24);
			Qlogo.v[0].col=col;
			Qlogo.v[1].col=col;
			Qlogo.v[2].col=col;
			Qlogo.v[3].col=col;
		}
		break;
	case GAME_MENU:
		if(sDone)
		{
			Qlogo.tex = pHge->Texture_Load("res/logo1.png");
			InitQuad();
			sDone = false;
		}
		dTime += dt;
		if(dTime<=0.25) alpha=(BYTE)((dTime*4)*0xFF);
		else if(dTime<=3.0) alpha=0xFF;
		else if(dTime<=3.25) alpha=(BYTE)((1.0f-(dTime-3.0f)*4)*0xFF);
		else
		{
			gameState = GAME_SELECT;
			dTime = 0;
			sDone = true;
			break;
		}
		if(!sDone)
		{
			col=0xFFFFFF | (alpha<<24);
			Qlogo.v[0].col=col;
			Qlogo.v[1].col=col;
			Qlogo.v[2].col=col;
			Qlogo.v[3].col=col;
		}
		break;
	case GAME_SELECT:
		if(sDone)
		{
			pHge->Effect_PlayEx(backSnd, 100, 0, 1.0f, true);
			pGui->AddCtrl(new hgeGUIMenuItem(1,Fnt,menuSnd,400,300,0.0f,"Play"));
			pGui->AddCtrl(new hgeGUIMenuItem(2,Fnt,menuSnd,400,350,0.1f,"Options"));
			pGui->AddCtrl(new hgeGUIMenuItem(3,Fnt,menuSnd,400,400,0.2f,"Instructions"));
			pGui->AddCtrl(new hgeGUIMenuItem(4,Fnt,menuSnd,400,450,0.3f,"Credits"));
			pGui->AddCtrl(new hgeGUIMenuItem(5,Fnt,menuSnd,400,500,0.4f,"Exit"));
			pGui->SetFocus(1);
			pGui->Enter();
			sDone = false;
			dTime = 0;
		}
		curId=pGui->Update(dt);
		gameAgain = 0;
		if(curId == -1)
		{
			switch(lastId)
			{
			case 1:
				gameState = GAME_READY;
				sDone = true;
				lastId = 1;
				break;
			case 2:
			case 3:
			case 4:
				pGui->SetFocus(1);
				pGui->Enter();
				break;
			case 5:
				gameEnd = true;
				break;
			}
		}
		else if(curId){lastId = curId; pGui->Leave();}
		break;
	case GAME_READY:
		if(sDone)
		{
			dTime = 0.0;
			back = pHge->Texture_Load("res/gameback.png");
			pBack   = new hgeSprite(back, 0, 0, scrWidth, scrHeight);
			//pHge->Effect_Free(backSnd);
			sDone = false;
		}
		dTime += dt;
		if(dTime > 1.5f && dTime < 2.5f)
		{
			ready = pHge->Texture_Load("res/go.png");
			pReady  = new hgeSprite(ready,  0,   0, 256, 80);
		}
		else if(dTime >= 2.5f)
		{
			gameState = GAME_START;
			sDone = true;
		}
		break;
	case GAME_START:
		if(sDone)
		{
			LoadSetup();
			dTime = 0.0f;
			InitMap();
			ReadMap();
			sDone = false;
		}
		if(ballPos.cy <= ballHeight)
		{
			ballPos.dy = -ballPos.dy;
			pHge->Effect_PlayEx(menuSnd);
			//ballPos.cy += ballPos.dy;
		}
		if((ballPos.cy + ballHeight >= flatPos.cy && ballPos.cy + ballHeight<= flatPos.cy + 5)	&&
		 ballPos.cx +ballHeight >= flatPos.cx - flatWidth && ballPos.cx <= flatPos.cx + flatWidth)
		{
			ballPos.dy = -ballPos.dy;
			ballPos.cy += ballPos.dy;
			if(ballPos.cx == flatPos.cx)
				ballPos.dx = 0;
			else if(ballPos.cx >= flatPos.cx)
				ballPos.dx = (ballPos.cx - flatPos.cx) / 8 + 2;
			else if(ballPos.cx < flatPos.cx)
				ballPos.dx = -((flatPos.cx - ballPos.cx) / 8 + 2);
			pHge->Effect_PlayEx(menuSnd);
			//ballPos.cy += ballPos.dy;
		}
		if(ballPos.cy >= scrHeight - ballHeight)
		{
			//ballPos.dy = -ballPos.dy;
			gameState = GAME_READY;
			ready = pHge->Texture_Load("res/ready.png");
			pReady  = new hgeSprite(ready,  0,   0, 256, 80);
			dTime = 0;
			sDone = false;
			gameAgain ++;
			LoadSetup();
			//ballPos.cy += ballPos.dy;
		}
		if(ballPos.cx > scrWidth - ballWidth)
		{
			pHge->Effect_PlayEx(menuSnd);
			ballPos.dx = -ballPos.dx;
		}
		if(ballPos.cx < ballWidth)
		{
			pHge->Effect_PlayEx(menuSnd);
			ballPos.dx = -ballPos.dx;
		}
		for(int i =0; i < 30; i ++)
        {
			for(int j = 0; j < 20; j ++)
			{
				if(blocks[i][j] != BLOCK_BLANK)
				{
					if(ballPos.cx >= j * blockWidth && ballPos.cx <=  (j + 1) * blockWidth &&
						ballPos.cy >= blankHeight + i * blockHeight && 
						ballPos.cy <= blankHeight + (i + 1) * blockHeight)
					{
						pHge->Effect_PlayEx(menuSnd);
						blocks[i][j] = BLOCK_BLANK;
						ballPos.dy = -ballPos.dy;
						ballPos.cy += ballPos.dy;
						gameScore += 5;
						if(!CheckMap())
						{
							gameState = GAME_OVER;
							dTime = 0;
							sDone = true;
							ballPos.dx = ballPos.dy = 0;
							flatPos.dx = 0;
						}
					}
				}
			}
		}
		ballPos.cy += ballPos.dy;
		ballPos.cx += ballPos.dx;
		pHpar->Update(dt);
		pHpar->MoveTo(ballPos.cx, ballPos.cy);
		pHpar->Fire();
		break;
	case GAME_OVER:
		if(sDone)
		{
			dTime = 0;
			ready = pHge->Texture_Load("res/completed.png");
			pReady  = new hgeSprite(ready,  0,   0, 256, 80);
			sDone = false;
		}
		dTime += dt;
		if(dTime > 1.5f)
		{
			back = pHge->Texture_Load("res/menuback.png");
			pBack   = new hgeSprite(back, 0, 0, scrWidth, scrHeight);
			ready = pHge->Texture_Load("res/ready.png");
			pReady  = new hgeSprite(ready,  0,   0, 256, 80);
			pGui->SetFocus(1);
			pGui->Enter();
			gameState = GAME_SELECT;
			dTime = 0;
			sDone = false;
		}
		break;
	}

	pHge->Gfx_BeginScene( );
	pHge->Gfx_Clear( 0 );
	if(gameState <= GAME_MENU)
		pHge->Gfx_RenderQuad( &Qlogo );
	else if(gameState >= GAME_SELECT)
		pBack->Render(0, 0);
	if(gameState == GAME_READY || gameState == GAME_OVER )
		pReady->Render(272, 260);
	if(gameState >= GAME_START)
	{
		DrawMap();
		pFlat->Render(flatPos.cx - 45, flatPos.cy);
		pBall->Render(ballPos.cx, ballPos.cy);
		pHpar->Render();
	}
	pGui->Render();
	Fnt->SetColor(0xFFFFFFFF);
	//Fnt->printf(5, 5, HGETEXT_LEFT, "Delta:%.3f\nFPS:%d", pHge->Timer_GetDelta() * 100, pHge->Timer_GetFPS());
	if(gameState >= GAME_START)
	{
		Fnt->printf(5, 5, HGETEXT_LEFT, "Time(s):%d", gameAgain);
		Fnt->printf(650, 5, HGETEXT_LEFT, "Score:%-4d", gameScore);
	}
	pHge->Gfx_EndScene();
	return false;
}

bool FrameFunc()
{
	int dt = pHge->Timer_GetDelta();
	srand((unsigned)time(NULL));
	if(gameEnd == true) return true;
	if(pHge->Input_GetKeyState(HGEK_ESCAPE))return true;
	if(gameState == GAME_START)
	{
		if(pHge->Input_GetKeyState(HGEK_A))
		{
			if(flatPos.cx < flatWidth + flatPos.dx)
				flatPos.cx = flatWidth;
			else flatPos.cx -= flatPos.dx;
		}
		else if(pHge->Input_GetKeyState(HGEK_D))
		{
			if(flatPos.cx > scrWidth - flatWidth - flatPos.dx)
				flatPos.cx = scrWidth - flatWidth;
			else flatPos.cx += flatPos.dx;
		}
	}
	if(Qlogo.v[0].col != 0xFFFFFFFF)
	{
		for(int i = 0; i < 4; i ++)
			Qlogo.v[i].col += ARGB(1, 0, 0, 0);
	}
	pGui->Update(pHge->Timer_GetDelta());
	return false;
}

void Init()
{
	InitQuad();
	gameState = 0;
	sDone = 0;
	gameEnd = false;
	gameLife = 3;
	gameAgain = 0;
	LoadSetup();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int nCmdShow)
{
	pHge = hgeCreate(HGE_VERSION);
	pHge->System_SetState(HGE_SCREENWIDTH,  scrWidth);
	pHge->System_SetState(HGE_SCREENHEIGHT, scrHeight);
	pHge->System_SetState(HGE_RENDERFUNC,   RenderFunc);
	pHge->System_SetState(HGE_FRAMEFUNC,    FrameFunc);
	pHge->System_SetState(HGE_TITLE,        "BreakOut");
	pHge->System_SetState(HGE_WINDOWED,     true);
	pHge->System_SetState(HGE_USESOUND,     true);
	pHge->System_SetState(HGE_ZBUFFER,      true);
	pHge->System_SetState(HGE_HIDEMOUSE,    true);
	pHge->System_SetState(HGE_SCREENBPP,    32);
	pHge->System_SetState(HGE_LOGFILE, "BreakOut.log");
	
	if(pHge->System_Initiate())
	{
		Qlogo.tex = pHge->Texture_Load("res/logo.png");
		mouse     = pHge->Texture_Load("res/cursor.png");
		back      = pHge->Texture_Load("res/menuback.png");
		rblock    = pHge->Texture_Load("res/block/rblock.png");
		gblock    = pHge->Texture_Load("res/block/gblock.png");
		bblock    = pHge->Texture_Load("res/block/bblock.png");
		wblock    = pHge->Texture_Load("res/block/wblock.png");
		yblock    = pHge->Texture_Load("res/block/yblock.png");
		oblock    = pHge->Texture_Load("res/block/oblock.png");
		pblock    = pHge->Texture_Load("res/block/pblock.png");
		flat      = pHge->Texture_Load("res/flat.png");
		ball      = pHge->Texture_Load("res/particles.png");
		ready     = pHge->Texture_Load("res/ready.png");

		menuSnd = pHge->Effect_Load("res/menu.wav");
		backSnd = pHge->Effect_Load("res/back.mp3");
		if(!Qlogo.tex || !mouse || !rblock || !wblock || !gblock || !bblock || !flat || !ball || !ready)
		{
			MessageBox(NULL, "Can't load texture", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
			pHge->System_Shutdown();
			pHge->Release();
			return 0;
		}
		if(!menuSnd || !backSnd)
		{
			MessageBox(NULL, "Can't load music", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
			pHge->System_Shutdown();
			pHge->Release();
			return 0;
		}
		Init();
		pCursor = new hgeSprite(mouse,  0,  0,  32, 32);
		pBack   = new hgeSprite(back, 0, 0, scrWidth, scrHeight);
		pBblock = new hgeSprite(bblock, 0,  0,  40, 20);
		pRblock = new hgeSprite(rblock, 0,  0,  40, 20);
		pGblock = new hgeSprite(gblock, 0,  0,  40, 20);
		pWblock = new hgeSprite(wblock, 0,  0,  40, 20);
		pYblock = new hgeSprite(yblock, 0,  0,  40, 20);
		pOblock = new hgeSprite(oblock, 0,  0,  40, 20);
		pPblock = new hgeSprite(pblock, 0,  0,  40, 20);
		pFlat   = new hgeSprite(flat,   0,  0,  90, 12);
		pBall   = new hgeSprite(ball,  96, 64,  32, 32);
		pReady  = new hgeSprite(ready,  0,  0, 256, 80);
		pHps    = new hgeSprite(ball,  32, 32,  32, 32);

		pHps->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		pHps->SetHotSpot(16, 16);
		pHps->SetColor(0xFFFFA000);
		pHpar   = new hgeParticleSystem("res/demo.psi", pHps);
		pHpar->info.nEmission = 50;

		pBall->SetColor(0xFFFFFFFF);
		pBall->SetHotSpot(16,16);

		Fnt=new hgeFont("res/font1.fnt");
		pGui=new hgeGUI();
		pGui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		pGui->SetCursor(pCursor);
		pGui->Enter();
		pHge->System_Start();
		Cleanup();
	}
	pHge->System_Shutdown();
	pHge->Release();
	return 0;
}