//*****************************************************************************
//
//     ���b�V���E�H�[���̏���[meshWall.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "3DMesh.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CMeshWall�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMeshWall::CMeshWall(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_nTexIdx = 0;                                // ���b�V���E�H�[���̃e�N�X�`���ԍ�
	m_pVtxBuff = NULL;                            // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;                            // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                          // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // ���b�V���t�B�[���h�̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // ���b�V���t�B�[���h�̌���
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);    // ���b�V���t�B�[���h�̐F
	m_fWidth = 0.0f;                              // 1�u���b�N���̕�
	m_fHeight = 0.0f;                             // 1�u���b�N������
	m_nXBlock = 0;                                // ���̕�����
	m_nYBlock = 0;                                // �c�̕�����
	m_nNumVertex = 0;                             // ���_��
	m_nNumIndex = 0;                              // �C���f�b�N�X��
	m_nNumPolygon = 0;                            // �|���S����
	D3DXMatrixIdentity(&m_MtxWorld);              // ���[���h�}�g���b�N�X
	m_WallNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ǂ̖@���x�N�g��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshWall *CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CMeshWall *pMeshWall = NULL;           // ���b�V���E�H�[���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMeshWall == NULL)
		{// ����������ɂȂ��Ă���
			pMeshWall = new CMeshWall(nPriority);
			if (pMeshWall != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pMeshWall->SetTexIdx(nTexIdx);            // ���b�V���E�H�[���̃e�N�X�`���ԍ�
				pMeshWall->SetPos(pos);                   // ���b�V���E�H�[���̍��W
				pMeshWall->SetRot(rot);                   // ���b�V���E�H�[���̌���
				pMeshWall->SetCol(col);                   // ���b�V���E�H�[���̐F
				pMeshWall->SetWidth(fWidth);              // 1�u���b�N���̕�
				pMeshWall->SetHeight(fHeight);            // 1�u���b�N���̍���
				pMeshWall->SetXBlock(nXBlock);            // ���̕�����
				pMeshWall->SetYBlock(nYBlock);            // �c�̕�����
				pMeshWall->SetTexSplitU(nTexSplitU);      // �e�N�X�`��U���W�̕�����
				pMeshWall->SetTexSplitV(nTexSplitV);      // �e�N�X�`��V���W�̕�����

				if (FAILED(pMeshWall->Init()))
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
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pMeshWall;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshWall::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X����������Ă��Ȃ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMeshWall::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pIdxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshWall::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshWall::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetIndices(m_pIdxBuff);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CMeshWall::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CMeshWall::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �����ɕK�v�ȕϐ���錾
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
	float YPos = (m_fHeight * m_nYBlock) / 2;  // Y���W�����ɐݒ�
	float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
	float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
		    // ���_���W
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

			// �@���x�N�g��
			pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���_�J���[
			pVtx[nCntH].col = col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X���W�����ɐi�߂�
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
		YPos -= m_fHeight;                                                // Y���W�����ɐi�߂�
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
		fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

		// �|�C���^��i�߂�
		pVtx += m_nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();


	// ���[���h�}�g���b�N�X�v�Z����
	SetMtxWorld(pDevice);

	// �@���x�N�g�������
	D3DXVECTOR3 WallPos1, WallPos2, WallPos3;
	// ����̒��_���W
	WallPos1.x = -((m_fWidth * m_nXBlock) / 2);
	WallPos1.y = ((m_fHeight * m_nYBlock) / 2);
	D3DXVec3TransformCoord(&WallPos1, &WallPos1, &m_MtxWorld);

	// �E��̒��_���W
	WallPos2.x = ((m_fWidth * m_nXBlock) / 2);
	WallPos2.y = ((m_fHeight * m_nYBlock) / 2);
	D3DXVec3TransformCoord(&WallPos2, &WallPos2, &m_MtxWorld);

	// �����̒��_���W
	WallPos3.x = -((m_fWidth * m_nXBlock) / 2);
	WallPos3.y = -((m_fHeight * m_nYBlock) / 2);
	D3DXVec3TransformCoord(&WallPos3, &WallPos3, &m_MtxWorld);

	// �O�ς���@���x�N�g�������
	D3DXVECTOR3 WallVec1, WallVec2;
	WallVec1 = WallPos2 - WallPos1;
	WallVec2 = WallPos3 - WallPos1;
	D3DXVECTOR3 WallCross;
	D3DXVec3Cross(&WallCross, &WallVec2, &WallVec1);

	// �O�ς���Z�o�����x�N�g���𐳋K�����Ė@���x�N�g���Ƃ���
	D3DXVec3Normalize(&m_WallNormal, &WallCross);
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CMeshWall::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		 // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_nXBlock + 1; // ����
			pIdx[1] = nCntIdx;                 // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , �������c�̕������̍ŉ��w�ł͂Ȃ�
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;                       // �㑤
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // ���̉���

				pIdx += 2; // 2���i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CMeshWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Offset, D3DXVECTOR3 *pInterPoint)
{
	bool bCol = false;        // �����������ǂ���
	if (pPos != NULL)
	{
		bool bInside = false;     // �����ɂ��邩�ǂ���
		D3DXVECTOR3 VecStart;     // �x�N�g���̎n�_
		D3DXVECTOR3 VecEnd;       // �x�N�g���̏I�_
		D3DXVECTOR3 VerPos[4];    // �����ɂ��邩�ǂ����̔���p
		D3DXVECTOR3 Pos1, Pos2;   // �v�Z�p

		if (pPos->y + Offset.y >= GetPos().y - ((m_fHeight * m_nYBlock) / 2) && pPos->y <= GetPos().y + ((m_fHeight * m_nYBlock) / 2))
		{// �ǂ̍����ȓ��̏ꏊ�ɂ���
		    // �ǂ̓����ɂ��邩����
			VerPos[0] = D3DXVECTOR3(-((m_fWidth * m_nXBlock) / 2) - Offset.x, ((m_fHeight * m_nYBlock) / 2), -Offset.z);
			VerPos[1] = D3DXVECTOR3(((m_fWidth * m_nXBlock) / 2) + Offset.x, ((m_fHeight * m_nYBlock) / 2), -Offset.z);
			VerPos[2] = D3DXVECTOR3(((m_fWidth * m_nXBlock) / 2) + Offset.x, -((m_fHeight * m_nYBlock) / 2), -Offset.z);
			VerPos[3] = D3DXVECTOR3(-((m_fWidth * m_nXBlock) / 2) - Offset.x, -((m_fHeight * m_nYBlock) / 2), -Offset.z);
			for (int nCntVec = 0; nCntVec < 4; nCntVec++)
			{// 4�񕪌J��Ԃ�
				D3DXVec3TransformCoord(&VerPos[nCntVec], &VerPos[nCntVec], &m_MtxWorld);
			}

			int nCntClear = 0;
			for (int nCntVec = 0; nCntVec < 4; nCntVec++)
			{// 4�񕪔���
			    // �x�N�g�����g���ĊO�ςŔ��肷��
				Pos1 = *pPos - VerPos[nCntVec];
				Pos2 = VerPos[(nCntVec + 1) % 4] - VerPos[nCntVec];
				D3DXVECTOR3 Cross;
				D3DXVec3Cross(&Cross, &Pos1, &Pos2);

				if (Cross <= 0 || Cross >= 0)
				{// �ǂɓ�������
					nCntClear++;
					if (nCntClear == 4)
					{// �S�Ă̔����ʂ���
						bInside = true;
					}
				}
			}

			if (bInside == true)
			{// �ǂ̓����ɂ���
			    // �ǂ̒[����(�I�t�Z�b�g)�����߂�
				float fWallStart = -((m_fWidth * m_nXBlock) / 2) - Offset.x;
				float fWallEnd = ((m_fWidth * m_nXBlock) / 2) + Offset.x;
				VecStart = D3DXVECTOR3(fWallStart, 0.0f, -Offset.z);
				VecEnd = D3DXVECTOR3(fWallEnd, 0.0f, -Offset.z);

				// �ǂ̌����ɍ��킹�ăx�N�g�������
				D3DXVec3TransformCoord(&VecStart, &VecStart, &m_MtxWorld);
				D3DXVec3TransformCoord(&VecEnd, &VecEnd, &m_MtxWorld);

				// �x�N�g�����g���ĊO�ςŔ��肷��
				Pos1 = *pPos - VecStart;
				Pos2 = VecEnd - VecStart;

				if ((Pos1.z * Pos2.x) - (Pos1.x * Pos2.z) > 0)
				{// �ǂɓ�������
				    // �O�σx�N�g����p�ӂ��邽�߂̕ϐ���錾
					Pos1 = *pPosOld;                // �O��̃v���C���[�̍��W
					Pos2 = *pPos;                   // ���݂̃v���C���[�̍��W
					D3DXVECTOR3 Pos3 = VecEnd;      // �x�N�g���̏I�_
					D3DXVECTOR3 Pos4 = VecStart;    // �x�N�g���̎n�_

					// XZ���ʏ�ŊO�ς����
					D3DXVECTOR3 Vec1 = Pos4 - Pos1;    // �x�N�g���̎n�_ - �O��̃v���C���[�̍��W
					D3DXVECTOR3 Vec2 = Pos2 - Pos1;    // ���݂̃v���C���[�̍��W - �O��̃v���C���[�̍��W
					D3DXVECTOR3 Vec3 = Pos3 - Pos4;    // �x�N�g���̏I�_ - �x�N�g���̎n�_
					float CrossCalc1 = CFunctionLib::D3DXVec2Cross(&D3DXVECTOR2(Vec1.x, Vec1.z), &D3DXVECTOR2(Vec2.x, Vec2.z));
					float CrossCalc2 = CFunctionLib::D3DXVec2Cross(&D3DXVECTOR2(Vec2.x, Vec2.z), &D3DXVECTOR2(Vec3.x, Vec3.z));
					float CrossCalc3 = CFunctionLib::D3DXVec2Cross(&D3DXVECTOR2(Vec1.x, Vec1.z), &D3DXVECTOR2(Vec3.x, Vec3.z));

					if (CrossCalc2 != 0.0f)
					{// 2�{�ڂ̃x�N�g����3�{�ڂ̃x�N�g���̊O�ς�0�ȊO�Ȃ��(0���ƕ��s�x�N�g���Ȃ̂Ō������邱�Ƃ��Ȃ�)
					    // ��_�܂ł̒����̊������O�ς���Z�o
						float Length1 = CrossCalc3 / CrossCalc2;   // �O��̃v���C���[�̍��W����̋���
						float Length2 = CrossCalc1 / CrossCalc2;   // �x�N�g���̎n�_����̋���

						// �������Ă��邩���肷��(��_�܂ł̒����̊����������̎n�_���� 0�ȏ� 1�ȉ� �ł���Ȃ�������Ă���)
						float ErorrPermission = 0.1f;  // �덷���e�l(���������_�̊ۂߕ��ɂ���Ĕ��肪���蔲����̂ŁE�E�E)
						if (Length1 + ErorrPermission >= 0 && Length1 - ErorrPermission <= 1
							&& Length2 + ErorrPermission >= 0 && Length2 - ErorrPermission <= 1)
						{// �������Ă���Ɣ��肳�ꂽ
						    // ��_�̍��W���Z�o���ǂ���x�N�g�������ߌ�_�����炷
						    // ��_ = �x�N�g���̎n�_ + ��_�܂ł̔䗦 * �x�N�g���̏I�_
						    //D3DXVECTOR3 InterPoint = *pPosOld + t1 * Vec2;      // �����̍��W����
							D3DXVECTOR3 InterPoint = VecStart + Length2 * Vec3;   // �ǂ̃x�N�g������

							// �ǂ���x�N�g�� = �i�s�x�N�g�� - ����(�i�s�x�N�g�� * �ǂ̖@���x�N�g��) * �ǂ̖@���x�N�g��
							D3DXVECTOR3 Move;
							D3DXVECTOR3 ObjMove = *pMove;
							float fDot = D3DXVec3Dot(&ObjMove, &m_WallNormal);
							Move = ObjMove - fDot * m_WallNormal;

							// ��_��ǎC��x�N�g���ł��炵�����̂���
							D3DXVECTOR3 ObjPos;
							ObjPos.x = InterPoint.x + Move.x;
							ObjPos.z = InterPoint.z + Move.z;
							pPos->x = ObjPos.x;
							pPos->z = ObjPos.z;
							if (pInterPoint != NULL)
							{// ���������m�ۂ���Ă���
								*pInterPoint = InterPoint;
							}
							bCol = true;
						}
					}
				}
			}
		}
	}

	return bCol;
}

//=============================================================================
//    �e�N�X�`���ԍ��ݒ菈��
//=============================================================================
void CMeshWall::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMeshWall::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshWall::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshWall::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V���E�H�[���̍��W�ݒ菈��
//=============================================================================
void CMeshWall::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V���E�H�[���̌����ݒ菈��
//=============================================================================
void CMeshWall::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���b�V���E�H�[���̐F�ݒ菈��
//=============================================================================
void CMeshWall::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
			    // ���_�J���[
				pVtx[nCntH].col = m_Col;
			}
			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̕��ݒ菈��
//=============================================================================
void CMeshWall::SetWidth(const float fWidth)
{
	// ����ݒ�
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X���W�����ɐi�߂�
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X���W��i�߂Ă������߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̍����ݒ菈��
//=============================================================================
void CMeshWall::SetHeight(const float fHeight)
{
	// ������ݒ�
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float YPos = (m_fHeight * m_nYBlock) / 2; // Y���W����ɐݒ�

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.y = YPos;
			}
			YPos -= m_fHeight;   // Y���W�����ɐi�߂�

								 // �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshWall::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pIdxBuff);

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �����ɕK�v�ȕϐ���錾
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y���W�����ɐݒ�
			float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
			float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
				    // ���_���W
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// �@���x�N�g��
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// ���_�J���[
					pVtx[nCntH].col = col;

					// �e�N�X�`�����W
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X���W�����ɐi�߂�
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
				YPos -= m_fHeight;                                                // Y���W�����ɐi�߂�
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

				// �|�C���^��i�߂�
				pVtx += m_nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    �c�̕������ݒ菈��
//=============================================================================
void CMeshWall::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �����ɕK�v�ȕϐ���錾
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y���W�����ɐݒ�
			float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
			float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
				 // ���_���W
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// �@���x�N�g��
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// ���_�J���[
					pVtx[nCntH].col = col;

					// �e�N�X�`�����W
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X���W�����ɐi�߂�
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
				YPos -= m_fHeight;                                                // Y���W�����ɐi�߂�
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

				// �|�C���^��i�߂�
				pVtx += m_nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    �e�N�X�`��U���W�̕���������
//=============================================================================
void CMeshWall::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // �e�N�X�`��U���W

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.x = fTexU;
				fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;
			}
			fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1);

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �e�N�X�`��V���W�̕������ݒ菈��
//=============================================================================
void CMeshWall::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexV = 0.0f;   // �e�N�X�`��V���W

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.y = fTexV;
			}
			fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshWall::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �e�N�X�`���ԍ��擾����
//=============================================================================
int CMeshWall::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshWall::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshWall::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshWall::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���b�V���t�B�[���h�̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMeshWall::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V���t�B�[���h�̌����擾����
//=============================================================================
D3DXVECTOR3 CMeshWall::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���b�V���t�B�[���h�̐F�擾����
//=============================================================================
D3DXCOLOR CMeshWall::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1�u���b�N���̕��擾����
//=============================================================================
float CMeshWall::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1�u���b�N���̍����擾����
//=============================================================================
float CMeshWall::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshWall::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshWall::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    �e�N�X�`��U���W�̕������擾����
//=============================================================================
int CMeshWall::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    �e�N�X�`��V���W�̕������擾����
//=============================================================================
int CMeshWall::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshWall::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �C���f�b�N�X���擾����
//=============================================================================
int CMeshWall::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMeshWall::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshWall::GetMtxWorld(void)
{
	return m_MtxWorld;
}