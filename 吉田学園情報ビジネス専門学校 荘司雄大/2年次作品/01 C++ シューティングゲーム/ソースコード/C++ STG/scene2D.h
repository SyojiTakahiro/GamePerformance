//=============================================================================
//
// �V�[��2D���� [scene2D.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority);
	~CScene2D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D *Create(D3DXVECTOR3 pos,float size);
	void SetPosition(D3DXVECTOR3 pos);
	void SetColor(D3DXCOLOR col);
	void SetExplosion(int nPatternAnim);
	void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius);
	void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius);
	void SetPlayer(int nPatternAnim);
	void SetBulletEX(int nPatternAnim);
	void SetBulletSP(int nPatternAnim);
	D3DXVECTOR3 *GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtx(void);

	void SetSize(float size);
	float GetSize(void);

protected:
	D3DXVECTOR3				m_posPolygon;			// �|���S���̈ʒu
private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_move;
	float					m_fSpin;
	float					m_fSize;
	float					m_fScale;
	float					m_fLength;
	float					m_fRadius;
};

#endif