//=============================================================================
//
// GAME [game.cpp]
// Author :Takahiro
//
//=============================================================================
#include "game.h"
#include "bg.h"
#include "bg2.h"
#include "bullet.h"
#include "enemy.h"
#include "life.h"
#include "timer.h"
//#include "explosion.h"
//#include "effect.h"
#include "timer.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "fade.h"
#include "block.h"
#include "score.h"
#include "item.h"
#include "particle.h"
#include "sound.h"
#include "pause.h"

//*****************************************************************************
// ÉOÉçÅ[ÉoÉãïœêî
//*****************************************************************************
GAMESTATE g_GameState = GAMESTATE_NONE;
int g_nCounterGameState;

bool g_bPause;

BlockInfo g_aBlockInfo[] = 
{
	{ D3DXVECTOR3(650, 500, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(850, 400, 0.0f),  75.0f, 50.0f},
	{ D3DXVECTOR3(1050, 250, 0.0f),  50.0f, 50.0f },
	{ D3DXVECTOR3(1400, 600, 0.0f), 100.0f, 50.0f },
	{ D3DXVECTOR3(1650, 500, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(1850, 400, 0.0f),  75.0f, 50.0f },
	{ D3DXVECTOR3(2050, 250, 0.0f),  75.0f, 50.0f },
	{ D3DXVECTOR3(2500, 700, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(2800, 700, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(3100, 700, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(3300, 550, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(3500, 400, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(3700, 250, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(4000, 250, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(4300, 250, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(4550, 200, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(5000, 700, 0.0f), 75.0f, 50.0f },
	{ D3DXVECTOR3(5300, 700, 0.0f), 500.0f, 50.0f },


	//{ D3DXVECTOR3(0, 0, 0.0f), 25.0f,SCREEN_HEIGHT+500 },
	//{ D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH + SCREEN_WIDTH, 0, 0.0f), 25.0f,SCREEN_HEIGHT + 500 },
	//{ D3DXVECTOR3(0, 700, 0.0f),  SCREEN_WIDTH + SCREEN_WIDTH + SCREEN_WIDTH, 25.0f},
};
//=============================================================================
// èâä˙âªèàóù
//=============================================================================
void InitGame(void)
{
	int nCntBlock;
	//îwåiÇÃèâä˙âªèàóù

	InitBG();

	InitBG2();

	InitScore();

	InitTimer();

	InitParticle();

	//InitEffect();
	
	InitLife();

	InitBullet();


	//InitExplosion();

	InitEnemy();

	//SetEnemy(D3DXVECTOR3(1050, 500, 0.0f), 0);
	//SetEnemy(D3DXVECTOR3(1950, 600, 0.0f),0);
	//SetEnemy(D3DXVECTOR3(2950, 600, 0.0f), 0);
	

	InitPause();
	//ìGÇÃîzíu


	//É|ÉäÉSÉìÇÃèâä˙âªèàóù
	InitPlayer();

	InitBlock();
	//â°Å@èc
	for(nCntBlock = 0; nCntBlock < sizeof g_aBlockInfo/sizeof(BlockInfo); nCntBlock++)
	{
		SetBlock(g_aBlockInfo[nCntBlock].pos, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),g_aBlockInfo[nCntBlock].fWidth, g_aBlockInfo[nCntBlock].fHeight,BLOCKTYPE_NORMAL);
	}

	SetBlock(D3DXVECTOR3(500, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),500,25, BLOCKTYPE_MOVE);

	InitItem();

	
	SetItem(D3DXVECTOR3(650, 450, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(850, 350, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(1050, 200, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(1450, 550, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(1650, 450, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(1850, 350, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(2050, 200, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(2500, 650, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(2800, 650, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(3100, 650, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(3500, 350, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(3700, 200, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(4000, 200, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(4300, 200, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(4550, 150, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(5000, 650, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);
	SetItem(D3DXVECTOR3(5500, 650, 0.0f), D3DXCOLOR(255, 255, 255, 0), 30.0f, 50.0f);

	g_GameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_bPause = false;
	PlaySound(SOUND_LABEL_BGM001);
}
//=============================================================================
// èIóπèàóù
//=============================================================================
void UninitGame(void)
{
	/*UninitKeyboard();*/
	UninitBG();
	UninitBG2();
	UninitBlock();
	UninitParticle();
	//UninitEffect();
	UninitLife();
	UninitScore();
	UninitTimer();
	UninitBullet();
	//UninitExplosion();
	UninitEnemy();
	UninitItem();
	UninitPlayer();
	UninitPause();
}
//=============================================================================
// çXêVèàóù
//=============================================================================
void UpdateGame(void)
{
	/*UpdateKeyboard();*/
	/*UpdateBG();
	UpdateBullet();
	UpdateEffect();
	UpdateExplosion();
	UpdateEnemy();
	UpdatePlayer();*/

	switch (g_GameState)
	{
	case GAMESTATE_NORMAL:
		
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_GameState = GAMESTATE_NONE;
			StopSound(SOUND_LABEL_BGM002);
			//
			SetFade(MODE_RESULT);
		}
	case GAMESTATE_CLEAR:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_GameState = GAMESTATE_NONE;
			StopSound(SOUND_LABEL_BGM002);
			//
			SetFade(MODE_CLEAR);
		}
		break;
	}

	/*if (GetKeyboardTrigger(DIK_R) == true)
	{
		SetFade(MODE_GAME);
	}*/
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = (g_bPause ? false : true);
	}
	if (g_bPause == false)
	{
		UpdateBG();
		UpdateBG2();
		
		UpdateParticle();
		//UpdateEffect();
		UpdateLife();
		UpdateScore();
		UpdateTimer();
		UpdateItem();
		UpdateBullet();
		//UpdateExplosion();
		UpdateEnemy();
		UpdatePlayer();
		UpdateBlock();
	}
	else
	{
		UpdatePause();
	}
}
//=============================================================================
// ï`âÊèàóù
//=============================================================================
void DrawGame(void)
{
	DrawBG();

	DrawBG2();

	DrawBlock();

	DrawParticle();

	//DrawEffect();

	//DrawLife();

	DrawScore();

	DrawTimer();

	DrawItem();

	DrawBullet();

	DrawEnemy();

	DrawPlayer();

	//DrawExplosion();

	if (g_bPause == true)
	{
		DrawPause();
	}
}

void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 0;
}

GAMESTATE *GetGameState(void)
{
	return &g_GameState;
}