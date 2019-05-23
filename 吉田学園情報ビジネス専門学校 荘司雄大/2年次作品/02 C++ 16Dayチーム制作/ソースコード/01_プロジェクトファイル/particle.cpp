//*****************************************************************************
//
//     �p�[�e�B�N���̏���[particle.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �l�ǂݍ��݂̖ڈ�ƂȂ�p�X��
#define NUM_EMITTER     "NUM_EMITTER"       // �G�~�b�^�[�̎��
#define PARTICLESET     "PARTICLESET"       // �p�[�e�B�N���Z�b�g�ǂݍ��݊J�n�̍��}
#define END_PARTICLESET "END_PARTICLESET"   // �p�[�e�B�N���Z�b�g�ǂݍ��ݏI���̍��}

#define MAXMOVE         "MAXMOVE"           // �ړ��ʂ̍ő�l
#define MINMOVE         "MINMOVE"           // �ړ��ʂ̍ŏ��l
#define INITCOL         "INITCOL"           // �������̐F
#define FLAMECOL        "FLAMECOL"          // 1�t���[���ɉ��Z����F�̕ω���
#define INITSIZE        "INITSIZE"          // �������̃T�C�Y(�����ŏ��l�A�E���ő�l)
#define FLAMESIZE       "FLAMESIZE"         // 1�t���[���ɉ��Z����T�C�Y
#define EMITTERLIFE     "EMITTERLIFE"       // �G�~�b�^�̎���
#define PARLIFE         "PARLIFE"           // �p�[�e�B�N���̎���(�����ŏ��l�A�E���ő�l)
#define APPEAR          "APPEAR"            // �p�[�e�B�N���𐶐�����Ԋu
#define SPREAD          "SPREAD"            // �G�~�b�^�̊g�U�l
#define LAP             "LAP"               // 1��̐����^�C�~���O�Ńp�[�e�B�N���𐶐������
#define GRAVITY         "GRAVITY"           // �p�[�e�B�N���ɂ�����d��
#define ROTPATTERN      "ROTPATTERN"        // ��]�̎��( 0:�Ȃ��@1:���v���@2:�����v��� 3:�����_�� )
#define ROTSPEED        "ROTSPEED"          // �p�[�e�B�N���ɂ�����d��
#define LOOP            "LOOP"              // ���[�v���邩���Ȃ���(0�Ȃ烋�[�v���Ȃ� / 1�Ȃ烋�[�v����)
#define BOUND           "BOUND"             // �o�E���h���邩���Ȃ���(0�Ȃ�o�E���h���Ȃ� / 1�Ȃ�o�E���h����)
#define TEXTURE         "TEXTURE"           // �g�p����e�N�X�`���̔ԍ�
#define DRAW            "DRAW"              // ���Z�����ŕ`�悷�邩���Ȃ���(0�Ȃ���Z�����Ȃ� / 1�Ȃ���Z��������)

//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
//    CParticleData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParticleData::CParticleData()
{
	// �e��l�̃N���A
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ��ʂ̍ő�l
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ��ʂ̍ŏ��l
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);     // �����l�̐F
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // ���t���[�����Z����F�̕ω���
	m_fMinRadius = 0.0f;                               // �������̔��a�ŏ��l
	m_fMaxRadius = 0.0f;                               // �������̔��a�ő�l
	m_fChangeRadius = 0.0f;                            // ���t���[�����Z����|���S���̔��a
	m_fRotSpeed = 0.0f;                                // ���t���[�����Z����|���S���̉�]��
	m_fSpread = 0.0f;                                  // ���t���[�����Z����L����
	m_fGravity = 0.0f;                                 // ���t���[�����Z����d��
    m_nLap = 0;                                        // �P�t���[���ɐ�������|���S����
	m_nLife = 0;			                           // �G�~�b�^�̎���
	m_nAppear = 0;                                     // �p�[�e�B�N���𐶐�����Ԋu
	m_nMinLife = 0;		                               // �\�����Ԃ̍ŏ��l
	m_nMaxLife = 0;		                               // �\�����Ԃ̍ő�l
	m_nTexIdx = 0;                                     // �g�p����e�N�X�`���̔ԍ�
	m_nRotPattern = 0;                                 // ��]�̎�ޕ����p[ 0:�Ȃ��@1:���v���@2:�����v��� 3:�����_�� ]
	m_bLoop = false;                                   // ���[�v���邩���Ȃ���[ 0�Ȃ烋�[�v���Ȃ� / 1�Ȃ烋�[�v���� ]
    m_bBouncing = false;		                       // �o�E���h�����邩[ 0:�Ȃ� 1:���� ]
	m_bDrawAddtive = false;	                           // ���Z�������ǂ���[ 0:�Ȃ� 1:���� ]
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParticleData::~CParticleData()
{

}

//*****************************************************************************
//    CParticleManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParticleManager::CParticleManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pParticleData = NULL;  // �p�[�e�B�N���f�[�^�ւ̃|�C���^
	m_pTexture = NULL;       // �e�N�X�`���ւ̃|�C���^
	m_nNumEmitter = 0;       // �p�[�e�B�N���̎��
	m_nNumTexture = 0;       // �ǂݍ��񂾃e�N�X�`���̐�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParticleManager::~CParticleManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParticleManager *CParticleManager::Create(char *pParticleFileName, char *pTextureFileName)
{
	CParticleManager *pParticleManager = NULL;  // �p�[�e�B�N���Ǘ��N���X�^�̃|�C���^
	if (pParticleManager == NULL)
	{// ����������ɂȂ��Ă���
		pParticleManager = new CParticleManager;
		if (pParticleManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			// �e��l�̐ݒ�
			strcpy(pParticleManager->m_aParticleFileName, pParticleFileName);
			strcpy(pParticleManager->m_aTextureFileName, pTextureFileName);

			if (FAILED(pParticleManager->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pParticleManager;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParticleManager::Init(void)
{
	// �p�[�e�B�N���f�[�^�̓ǂݍ���
	if (FAILED(ParticleDataLoad()))
	{// �ǂݍ��݂Ɏ��s����
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	if (FAILED(TextureLoad()))
	{// �ǂݍ��݂Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CParticleManager::Uninit(void)
{
	// �p�[�e�B�N���f�[�^�̊J��
	if (m_pParticleData != NULL)
	{
		ReleaseParticleData();
	}

	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		ReleaseTexture();
	}

	// ���g�̃|�C���^���J��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParticleManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParticleManager::Draw(void)
{

}

//=============================================================================
//    �p�[�e�B�N���f�[�^��ǂݍ��ޏ���
//=============================================================================
HRESULT CParticleManager::ParticleDataLoad(void)
{
	FILE *pFile = NULL;               // �t�@�C���|�C���^
	int nCntParticleData = 0;  // �ǂݍ��񂾃p�[�e�B�N���f�[�^�̐�
	char aStr[128];            // �e�L�X�g�̕�����i�[�p

	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aParticleFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			while (fscanf(pFile, "%s", aStr) != EOF)
			{// �I�����b�Z�[�W�܂Ń��[�v
				if (strcmp(aStr, NUM_EMITTER) == 0)
				{// �G�~�b�^�̎�ނ̐�
					fscanf(pFile, "%s %d", aStr, &m_nNumEmitter);
					if (m_nNumEmitter >= 1)
					{// �ǂݍ��ރG�~�b�^�[�̐����P�ȏ゠��
						if (m_pParticleData == NULL)
						{// �|�C���^���m�ۂł����Ԃł���
							m_pParticleData = new CParticleData*[m_nNumEmitter];
							if (m_pParticleData != NULL)
							{// �|�C���^���m�ۂł���
								for (int nCntParticle = 0; nCntParticle < m_nNumEmitter; nCntParticle++)
								{// �ǂݍ��ރG�~�b�^�[�̐������J��Ԃ�
									m_pParticleData[nCntParticle] = new CParticleData;
								}
							}
						}
					}
				}
				else if (strcmp(aStr, PARTICLESET) == 0)
				{// �G�~�b�^�̏��
					if (m_pParticleData != NULL)
					{// �|�C���^���m�ۂł��Ă���
						while (strcmp(aStr, END_PARTICLESET) != 0)
						{// �I�����b�Z�[�W�܂Ń��[�v
							fscanf(pFile, "%s", &aStr);

							if (strcmp(aStr, MAXMOVE) == 0)
							{// �ړ��ʂ̍ő�
								D3DXVECTOR3 MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f", aStr, &MaxMove.x, &MaxMove.y, &MaxMove.z) == 4)
								{// �ړ��ʂ̍ő�l��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetMaxMove(MaxMove);
								}
							}
							else if (strcmp(aStr, MINMOVE) == 0)
							{// �ړ��ʂ̍ŏ�
								D3DXVECTOR3 MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f", aStr, &MinMove.x, &MinMove.y, &MinMove.z) == 4)
								{// �ړ��ʂ̍ŏ��l��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetMinMove(MinMove);
								}
							}
							else if (strcmp(aStr, INITCOL) == 0)
							{// �����J���[
								D3DXCOLOR InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f %f", aStr, &InitCol.r, &InitCol.g, &InitCol.b, &InitCol.a) == 5)
								{// �����J���[��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetInitCol(InitCol);
								}
							}
							else if (strcmp(aStr, FLAMECOL) == 0)
							{// �t���[���ɂ��J���[�̕ω�
								D3DXCOLOR ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
								if (fscanf(pFile, "%s %f %f %f %f", aStr, &ChangeCol.r, &ChangeCol.g, &ChangeCol.b, &ChangeCol.a) == 5)
								{// // �t���[���ɂ��J���[�̕ω���ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetChangeCol(ChangeCol);
								}
							}
							else if (strcmp(aStr, INITSIZE) == 0)
							{// �����T�C�Y
								float fMinSize = 0.0f;
								float fMaxSize = 0.0f;
								if (fscanf(pFile, "%s %f %f", aStr, &fMinSize, &fMaxSize) == 3)
								{// �����T�C�Y��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetMinRadius(fMinSize);
									m_pParticleData[nCntParticleData]->SetMaxRadius(fMaxSize);
								}
							}
							else if (strcmp(aStr, FLAMESIZE) == 0)
							{// �t���[���ɂ��T�C�Y�̕ω�
								float fChangeRadius = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fChangeRadius) == 2)
								{// �t���[���ɂ��T�C�Y�̕ω���ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetChangeRadius(fChangeRadius);
								}
							}
							else if (strcmp(aStr, EMITTERLIFE) == 0)
							{// �\������(����)
								int nLife = 0;
								if (fscanf(pFile, "%s %d", aStr, &nLife) == 2)
								{// �\������(����)��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetLife(nLife);
								}
							}
							else if (strcmp(aStr, PARLIFE) == 0)
							{// �p�[�e�B�N���̕\������
								int nMinParLife = 0;
								int nMaxParLife = 0;
								if (fscanf(pFile, "%s %d %d", aStr, &nMinParLife, &nMaxParLife) == 3)
								{// �p�[�e�B�N���̕\�����Ԃ�ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetMinLife(nMinParLife);
									m_pParticleData[nCntParticleData]->SetMaxLife(nMaxParLife);
								}
							}
							else if (strcmp(aStr, APPEAR) == 0)
							{// �o��Ԋu
								int nAppear = 0;
								if (fscanf(pFile, "%s %d", aStr, &nAppear) == 2)
								{// �o��Ԋu��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetAppear(nAppear);
								}
							}
							else if (strcmp(aStr, SPREAD) == 0)
							{// �g�U�l
								float fSpread = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fSpread) == 2)
								{// �g�U�l��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetSpread(fSpread);
								}
							}
							else if (strcmp(aStr, LAP) == 0)
							{// 1���ɏo����
								int nLap = 0;
								if (fscanf(pFile, "%s %d", aStr, &nLap) == 2)
								{// 1���ɏo������ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetLap(nLap);
								}
							}
							else if (strcmp(aStr, GRAVITY) == 0)
							{// �d��
								float fGravity = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fGravity) == 2)
								{// �d�͂�ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetGravity(fGravity);
								}
							}
							else if (strcmp(aStr, ROTPATTERN) == 0)
							{// ��]�̃p�^�[��
								int nRotPattern = 0;
								if (fscanf(pFile, "%s %d", aStr, &nRotPattern) == 2)
								{// 1���ɏo������ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetRotPattern(nRotPattern);
								}
							}
							else if (strcmp(aStr, ROTSPEED) == 0)
							{// ��]�̑��x
								float fRotSpeed = 0.0f;
								if (fscanf(pFile, "%s %f", aStr, &fRotSpeed) == 2)
								{// ��]�̑��x��ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetRotSpeed(fRotSpeed);
								}
							}
							else if (strcmp(aStr, LOOP) == 0)
							{// ���[�v���邩�ǂ���
								int nLoop = 0;
								if (fscanf(pFile, "%s %d", aStr, &nLoop) == 2)
								{// ���[�v���邩�ǂ�����ǂݍ��߂�
									if (nLoop == 0)
									{
										m_pParticleData[nCntParticleData]->SetLoop(false);
									}
									else if(nLoop == 1)
									{
										m_pParticleData[nCntParticleData]->SetRotSpeed(true);
									}
								}
							}
							else if (strcmp(aStr, BOUND) == 0)
							{// �o�E���h���邩�ǂ���
								int nBound = 0;
								if (fscanf(pFile, "%s %d", aStr, &nBound) == 2)
								{// �o�E���h���邩�ǂ�����ǂݍ��߂�
									if (nBound == 0)
									{
										m_pParticleData[nCntParticleData]->SetBouncing(false);
									}
									else if (nBound == 1)
									{
										m_pParticleData[nCntParticleData]->SetBouncing(true);
									}
								}
							}
							else if (strcmp(aStr, TEXTURE) == 0)
							{// �e�N�X�`���̎��
								int nTexIdx = 0;
								if (fscanf(pFile, "%s %d", aStr, &nTexIdx) == 2)
								{// �e�N�X�`���̎�ނ�ǂݍ��߂�
									m_pParticleData[nCntParticleData]->SetTexIdx(nTexIdx);
								}
							}
							else if (strcmp(aStr, DRAW) == 0)
							{// ���Z�����̗L��
								int nDraw = 0;
								if (fscanf(pFile, "%s %d", aStr, &nDraw) == 2)
								{// ���Z�����̗L����ǂݍ��߂�
									if (nDraw == 0)
									{
										m_pParticleData[nCntParticleData]->SetDrawAddtive(false);
									}
									else if (nDraw == 1)
									{
										m_pParticleData[nCntParticleData]->SetDrawAddtive(true);
									}
								}
							}
						}
					}
					nCntParticleData++;	// �G�~�b�^�ԍ��̃J�E���g

					if (nCntParticleData >= m_nNumEmitter)
					{// ����ȏ�ǂݍ��߂Ȃ�
						break;
					}
				}
			}

			fclose(pFile);		// �t�@�C�������
		}
		else
		{// �t�@�C�����J���Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", m_aParticleFileName, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`����ǂݍ��ޏ���
//=============================================================================
HRESULT CParticleManager::TextureLoad(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			FILE *pFile = NULL;  // �t�@�C���|�C���^
			char aFileName[64];		// �e�N�X�`���̖��O

			pFile = fopen(m_aTextureFileName, "r");

			if (NULL != pFile)
			{// �J����
				fscanf(pFile, "%d", &m_nNumTexture);		// �e�N�X�`����

				if (m_pTexture == NULL)
				{// ���������m�ۂł����Ԃɂ���
					m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
					if (m_pTexture != NULL)
					{// ���������m�ۂł���
						for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
						{
							fscanf(pFile, "%s\n", &aFileName[0]);
							D3DXCreateTextureFromFile(pDevice, &aFileName[0], &m_pTexture[nCntTexture]);
						}
					}
				}
				fclose(pFile);
			}
			else
			{// ���s
				MessageBox(0, "�t�@�C�����J���܂���ł���", m_aTextureFileName, MB_OK);
				return E_FAIL;
			}
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X���擾�ł��Ȃ�����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^���J�����鏈��
//=============================================================================
void CParticleManager::ReleaseParticleData(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nNumEmitter; nCntParticle++)
	{// ���������p�[�e�B�N���f�[�^�̐������J��Ԃ�
		if (m_pParticleData[nCntParticle] != NULL)
		{// ���������m�ۂ���Ă���
			delete m_pParticleData[nCntParticle];
			m_pParticleData[nCntParticle] = NULL;
		}
	}
	delete[] m_pParticleData;
	m_pParticleData = NULL;
}

//=============================================================================
//    �e�N�X�`�����J�����鏈��
//=============================================================================
void CParticleManager::ReleaseTexture(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{// ���������e�N�X�`���̐������J��Ԃ�
		if (m_pTexture[nCntTex] != NULL)
		{// ���������m�ۂ���Ă���
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
	delete[] m_pTexture;
	m_pTexture = NULL;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^���擾���鏈��
//=============================================================================
CParticleData *CParticleManager::GetParticleDate(int nIdx)
{
	if (m_pParticleData == NULL)
	{
		return NULL;
	}

	return m_pParticleData[nIdx];
}

//=============================================================================
//    �e�N�X�`�����擾���鏈��
//=============================================================================
LPDIRECT3DTEXTURE9 CParticleManager::GetTexture(int nIdx)
{
	if (m_pTexture == NULL)
	{
		return NULL;
	}

	return m_pTexture[nIdx];
}

//*****************************************************************************
//    CParEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParEmitter::CParEmitter(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTexture = NULL;                      // �p�[�e�B�N���ɐݒ肷��e�N�X�`��
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �G�~�b�^�̍��W
	m_pParticleData = NULL;                 // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	m_nLife = 0;			                // �G�~�b�^�̎���
	m_nTexture = 0;	                        // �e�N�X�`���̎��
	m_nAppear = 0;                          // ���o����Ԋu
	m_nLap = 0;                             // 1��̕��o�ŏo���p�[�e�B�N���̌�
	m_nType = 0;                            // ���o����p�[�e�B�N���̎��
	m_nCntEmitter = 0;                      // ���o����^�C�~���O���Ǘ�����J�E���^�[
	m_bLoop = 0;                            // ���[�v�̗L��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParEmitter::~CParEmitter()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParEmitter *CParEmitter::Create(D3DXVECTOR3 pos, int nType, int nPriority)
{
	CParEmitter *pParEmitter = NULL;  // �p�[�e�B�N���G�~�b�^�N���X�^�̃|�C���^
	if (pParEmitter == NULL)
	{// ����������ɂȂ��Ă���
		pParEmitter = new CParEmitter(nPriority);
		if (pParEmitter != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pParEmitter->SetPos(pos);
			pParEmitter->SetType(nType);

			if (FAILED(pParEmitter->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pParEmitter;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	// �Ǘ��N���X���擾����
	CParticleManager *pParticleManager = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntScene = 0; nCntScene < MAX_PRIORITY_NUM; nCntScene++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		if (pParticleManager == NULL)
		{// �p�[�e�B�N���̊Ǘ��N���X���擾�ł��Ă��Ȃ�
			pScene = CScene::GetScene(nCntScene);
			while (pScene != NULL)
			{// �|�C���^����ɂȂ�܂ŌJ��Ԃ�
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == OBJTYPE_PARTICLEMANAGER)
				{// �p�[�e�B�N���̊Ǘ��N���X��������
					pParticleManager = (CParticleManager*)pScene;
					break;
				}
				pScene = pSceneNext;
			}
		}
	}


	// �e��l���Ǘ��N���X���ۗL����f�[�^���甲�����
	if (pParticleManager != NULL)
	{// �Ǘ��N���X���擾�ł���
		// �p�[�e�B�N���̃f�[�^
		if (m_pParticleData == NULL)
		{// �|�C���^���m�ۂł����Ԃł���
			m_pParticleData = pParticleManager->GetParticleDate(m_nType);
			if (m_pParticleData != NULL)
			{// �|�C���^���m�ۂł���
				m_nAppear = m_pParticleData->GetAppear();
				m_nLife = m_pParticleData->GetLife();
				m_bLoop = m_pParticleData->GetLoop();
				m_nLap = m_pParticleData->GetLap();
			}
		}

		// �e�N�X�`��
		if (m_pTexture == NULL)
		{// �|�C���^���m�ۂł����Ԃł���
			m_pTexture = pParticleManager->GetTexture(m_pParticleData->GetTexIdx());
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CParEmitter::Uninit(void)
{
	// �G�~�b�^�̍폜
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParEmitter::Update(void)
{
	bool bDelete = false;
	m_nLife--;	// �G�~�b�^�̕\�����Ԃ��J�E���g

	if (m_nLife <= 0 && m_bLoop == false)
	{// ��莞�Ԍo�߂ŃG�~�b�^������
		bDelete = true;
	}

	// �o��������Ԋu�̃J�E���g
	m_nCntEmitter = (m_nCntEmitter + 1) % m_nAppear;

	if (m_nCntEmitter == 0)
	{// �o��������^�C�~���O�ɂȂ���
		for (int nCntParticle = 0; nCntParticle < m_nLap; nCntParticle++)
		{// ���̕��o�ŏo�������J��Ԃ�
			SetParticle();
		}
	}

	if (true == bDelete)
	{// ����
		Uninit();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParEmitter::Draw(void)
{

}

//=============================================================================
//    �p�[�e�B�N�����Z�b�g���鏈��
//=============================================================================
void CParEmitter::SetParticle(void)
{
	// �����ɕK�v�ȃf�[�^��錾
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ���
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // ���������̐F
	float fRadius = 0.0f;                                     // ���a
	float fAngle = 0.0f;                                      // ����
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �F�̕ω���
	float fGravity = 0.0f;                                    // �d��
	float fChangeRadisu = 0.0f;                               // ���a�̕ω���
	float fChangeRot = 0.0f;                                  // �����̕ω���
	float fBouncing = 0.0f;                                   // �o�E���h��
	int nLife = 0;                                            // �̗�
	int nRotPattern = 0;                                      // ��]�̃p�^�[��
	bool bBound = false;                                      // �o�E���h���邩���Ȃ���
	bool bDrawAddtive = false;                                // ���Z�����ŕ`�悷�邩���Ȃ���

	// �f�[�^���쐬����
	if (m_pParticleData != NULL)
	{// ���������m�ۂ���Ă���
	     // �f�[�^���������镔���̓f�[�^������������Ă���
		InitCol = m_pParticleData->GetInitCol();
		ChangeCol = m_pParticleData->GetChangeCol();
		fGravity = m_pParticleData->GetGravity();
		fChangeRadisu = m_pParticleData->GetChangeRadius();
		fChangeRot = m_pParticleData->GetRotSpeed();
		nRotPattern = m_pParticleData->GetRotPattern();
		bBound = m_pParticleData->GetBouncing();
		bDrawAddtive = m_pParticleData->GetDrawAddtive();

		// �K�v�ȂƂ��낾���v�Z����
		// �ړ���
		int nLap = m_pParticleData->GetLap();
		float fSpread = m_pParticleData->GetSpread();
		D3DXVECTOR3 MinMove = m_pParticleData->GetMinMove();
		D3DXVECTOR3 MaxMove = m_pParticleData->GetMaxMove();
		fAngle = 1.0f / (nLap / 2);
		if (0 != MaxMove.y && 0 != MinMove.y)
		{// Y���ړ��ʂ������O�ȊO
			Move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (fAngle))) * ((MinMove.x + (rand() % (1 + (int)MaxMove.x))) - ((rand() % 10) * 0.1f)) * fSpread,
				((rand() % (int)MaxMove.y) + (rand() % (int)MinMove.y)) * fSpread,
				cosf(D3DX_PI * (0.0f + (fAngle ))) * ((MinMove.z + (rand() % (1 + (int)MaxMove.z))) - ((rand() % 10) * 0.1f)) * fSpread);
		}
		else
		{// Y���̈ړ��ʂ��O������ꍇ
			Move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (fAngle))) * ((MinMove.x + (rand() % (1 + (int)MaxMove.x))) - ((rand() % 10) * 0.1f)) * fSpread,
				((int)MaxMove.y + (int)MinMove.y) * fSpread,
				cosf(D3DX_PI * (0.0f + (fAngle))) * ((MinMove.z + (rand() % (1 + (int)MaxMove.z))) - ((rand() % 10) * 0.1f)) * fSpread);
		}

		// ����
		int nMinLife = m_pParticleData->GetMinLife();
		int nMaxLife = m_pParticleData->GetMaxLife();
		nLife = nMinLife + (rand() % (1 + (nMaxLife - nMinLife)) - 1);

		// ���a
		int fMinRadius = (int)m_pParticleData->GetMinRadius();
		int fMaxRadius = (int)m_pParticleData->GetMaxRadius();
		fRadius = fMinRadius + (rand() % (1 + (fMaxRadius - fMinRadius)) - 1) + ((rand() % 10) * 0.1f);

		// �p�[�e�B�N���𐶐�����
		CParticle *pParticle = CParticle::Create(m_Pos, Move, InitCol, fRadius, fAngle,
					 							 ChangeCol, fGravity, fChangeRadisu, fChangeRot,
												 fBouncing, nLife, nRotPattern, m_nType,
												 bBound, bDrawAddtive,3);
		if (pParticle != NULL)
		{// ���������m�ۂł���
			pParticle->BindTexture(m_pTexture);
		}
	}
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
void CParEmitter::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ��ނ�ݒ肷�鏈��
//=============================================================================
void CParEmitter::SetType(int nType)
{
	m_nType = nType;
}

//*****************************************************************************
//    CParticle�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// �e��l�̃N���A
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �ǂꂭ�炢�F���ω����邩
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �ړ���
	m_fGravity = 0.0f;                               // �d��
	m_fChangeRadius = 0.0f;                          // �ǂꂭ�炢�ό`���邩
	m_fChangeRot = 0.0f;                             // �ǂꂭ�炢��]���邩
	m_fBouncing = 0.0f;                              // �n�ʂɂ����ۂɃo�E���h����l
	m_nLife = 0;                                     // ����
	m_nRotPattern = 0;                               // ��]�̎��
	m_nType = 0;                                     // ���
	m_bBound = false;                                // �o�E���h���邩�ǂ���
	m_bDrawAddtive = false;                          // ���Z�����ŕ`�悷�邩�ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
							 D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
							 float fChangeRot, float fBouncing, int nLife, int nRotPattern,
							 int nType, bool bBound, bool bDrawAddtive, int nPriority)
{
	CParticle *pParticle = NULL;  // �p�[�e�B�N���N���X�^�̃|�C���^
	if (pParticle == NULL)
	{// ����������ɂȂ��Ă���
		pParticle = new CParticle(nPriority);
		if (pParticle != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pParticle->Init(pos, move, col, fRadius, fAngle, ChangeCol, fGravity, fChangeRadius, fChangeRot, fBouncing, nLife, nRotPattern, nType, bBound, bDrawAddtive)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pParticle;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
						D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
						float fChangeRot, float fBouncing, int nLife, int nRotPattern,
						int nType, bool bBound, bool bDrawAddtive)
{
	// �e��l�̐ݒ�
	SetPos(pos);                       // ���W
	SetCol(col);                       // �F
	SetLength(fRadius);                // ���a
	SetAngle(fAngle);                  // ����
	m_Move = move;                     // �ړ���
	m_ChangeCol = ChangeCol;           // �F
	m_fGravity = fGravity;             // ���a
	m_fChangeRadius = fChangeRadius;   // ���a�̕ω���
	m_fChangeRot = fChangeRot;         // �����̕ω���
	m_fBouncing = fBouncing;           // �o�E���h��
	m_nLife = nLife;                   // ����
	m_nRotPattern = nRotPattern;       // ��]�̃p�^�[��
	m_nType = nType;                   // ���
	m_bBound = bBound;                 // �o�E���h���邩���Ȃ���
	m_bDrawAddtive = bDrawAddtive;     // ���Z�����ŕ`�悷�邩���Ȃ���

	// ���ʂ̏���������
	SetWidth(cosf(fAngle) * fRadius);
	SetHeight(-sinf(fAngle) * fRadius);
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CParticle::Uninit(void)
{
	// ���ʂ̏I������
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParticle::Update(void)
{
	m_nLife--;		// �\�����Ԃ��J�E���g

	if (m_nLife <= 0)
	{// �\�����Ԃ�0�ȉ��ɂȂ���
		Uninit();	// �I������
	}
	else
	{// �܂��\�����Ԃ�����
		// �e��l�̎擾
		D3DXVECTOR3 pos = GetPos();  // ���W
		D3DXCOLOR col = GetCol();    // �F
		float fAngle = GetAngle();   // ����
		float fRadius = GetLength(); // ���a
		m_Move.y -= m_fGravity;		 // �d�͂�������

		m_fBouncing = pos.y;		// �l���L��

		pos += m_Move;				// �ʒu�̍X�V
		col += m_ChangeCol;         // �F�̍X�V
		fRadius += m_fChangeRadius;	// �傫���̍X�V

		if (pos.y <= 0 && m_bBound == true)
		{// �o�E���h
			m_Move.y += (m_fBouncing - pos.y) * 2.0f;
		}

		if (m_nRotPattern == 1)
		{// ���v���
			fAngle -= m_fChangeRot;			// ��]�̃J�E���^
		}
		else if (m_nRotPattern == 2)
		{// �����v���
			fAngle += m_fChangeRot;			// ��]�̃J�E���^
		}
		else if (m_nRotPattern == 3)
		{// �����_��
			if (rand() % 2 == 0)
			{// ���v���
				fAngle += m_fChangeRot;		// ��]�̃J�E���^
			}
			else
			{// �����v���
				fAngle -= m_fChangeRot;		// ��]�̃J�E���^
			}
		}

		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle > D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		// �e��l�̐ݒ�
		SetPos(pos);          // ����
		SetCol(col);          // �F
		SetAngle(fAngle);     // ����
		SetLength(fRadius);   // �傫��
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���Ȃ��悤�ɂ���
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			if (m_bDrawAddtive == true)
			{ // ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			CSceneBillboard::Draw();

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Z�o�b�t�@�����ɖ߂�
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CParticle::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength(), 0.0f);

	// �@��
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(pVtxBuff);
}