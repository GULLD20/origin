#include "../pch.h"
#include "Map.h"

#include "../MyLib/MyLib.h"

#include "../Scene/GameContext.h"

//�ǂ̓�����͔���̑傫��
const  DirectX::SimpleMath::Vector3 Map::WALL_BOX_SIZE = DirectX::SimpleMath::Vector3(1.5f, 15.0f, 1.5f);

//�ړ����E���̃G�t�F�N�g�̐�
const int Map::LIMIT_EFFECT_NUM = 2;

//�G�t�F�N�g�̊p�x(*0.49�̓G�t�F�N�g�\������0.5���Ɗ��S�Ɍ����Ȃ��Ȃ邽�ߏ������炷)
//�E
const float Map::EFFECT_ANGLE_R = DirectX::XM_PI*0.49f;
//��
const float Map::EFFECT_ANGLE_L = DirectX::XM_PI*-0.49f;

Map::Map()
{
}


Map::~Map()
{
	if (m_pGroundModel != nullptr)
	{
		m_pGroundModel.reset(nullptr);
	}

	if (m_pWallModel != nullptr)
	{
		m_pWallModel.reset();
	}

	if (m_pSpace != nullptr)
	{
		m_pSpace.reset();
	}

	if (m_MapDataList.size() != 0)
	{
		m_MapDataList.clear();
	}

	if (m_wallBoxList.size() != 0)
	{
		m_wallBoxList.clear();
	}

	if (m_pHitWallList.size() != 0)
	{
		for (int i = 0; i < static_cast<int>(m_pHitWallList.size()); i++)
		{
			m_pHitWallList[i]->Lost();
		}

		m_pHitWallList.clear();
	}
}

//�쐬
void Map::Create()
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();

	// �G�t�F�N�g�t�@�N�g���̍쐬
	DirectX::EffectFactory * factory = new DirectX::EffectFactory(deviceResources->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/CMO");

	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	//�n��
	m_pGroundModel = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/mapPart1.cmo",
		*factory
	);

	//��
	m_pWallModel = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/mapPart2.cmo",
		*factory
	);

	//�V��
	m_pSpace = DirectX::Model::CreateFromCMO(
		deviceResources->GetD3DDevice(),
		L"Resources/CMO/space.cmo",
		*factory
	);

	delete factory;

	//�ǂɓ����������̃G�t�F�N�g�̍쐬
	for (int i = 0; i < LIMIT_EFFECT_NUM; i++)
	{
		m_pHitWallList.push_back(std::make_unique<HitWall>());
		m_pHitWallList[i]->Create(deviceResources, L"Resources\\Textures\\LilimitWall.png", 1);
		m_pHitWallList[i]->InitializeNormal(0.0f, DirectX::SimpleMath::Vector3::Zero);
		m_pHitWallList[i]->SetScale(DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f));
	}
}

//����������
void Map::Initialize()
{
	//�n�ʂ̍쐬
	CreatGround();
	//�ǂ̍쐬
	CreatWall();

	//�V���̑傫���̐ݒ�
	m_spaceWorld = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(3.0f, 3.0f, 3.0f));
	//�V���̉�]�p�x�̏�����
	m_spaceAngleY = 0.0f;
}

//�`�揈��
void Map::Draw(const DirectX::SimpleMath::Matrix &viewMatrix)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* CommonState = GameContext<DirectX::CommonStates>().Get();
	Projection* projection = GameContext<Projection>().Get();

	// ���f���`��
	//�V���̉�]
	m_spaceAngleY += 0.01f;
	m_spaceWorld = DirectX::SimpleMath::Matrix::CreateRotationY(m_spaceAngleY);
	m_pSpace->Draw(deviceContext, *CommonState, m_spaceWorld, viewMatrix, projection->GetMatrix());

	//�n�ʂ̕`��
	m_pGroundModel->Draw(deviceContext, *CommonState, m_groundWorld, viewMatrix, projection->GetMatrix());

	//�Ǖ`��p�̃��[���h�s��
	DirectX::SimpleMath::Matrix world;

	//�ǂ̕`��
	for (int i = 0; i < static_cast<int>(m_wallBoxList.size()); i++)
	{
		//�ǂ̈ʒu�Ƀ��[���h�s��𒲐�
		world = DirectX::SimpleMath::Matrix::CreateTranslation(m_wallBoxList[i].center);
		m_pWallModel->Draw(deviceContext, *CommonState, world, viewMatrix, projection->GetMatrix());
	}


	//�ړ����E�̃G�t�F�N�g�̕\��
	for (int i = 0; i < LIMIT_EFFECT_NUM; i++)
	{
		m_pHitWallList[i]->SetRenderState(DirectX::SimpleMath::Vector3::One, viewMatrix, projection->GetMatrix());
		m_pHitWallList[i]->Render();
	}
}

//�^�C�g����ʂł̃}�b�v�̕\��
void Map::TitleDraw(const DirectX::SimpleMath::Matrix &viewMatrix)
{
	DX::DeviceResources* deviceResources = GameContext<DX::DeviceResources>::Get();
	ID3D11DeviceContext1* deviceContext = deviceResources->GetD3DDeviceContext();
	DirectX::CommonStates* CommonState = GameContext<DirectX::CommonStates>().Get();
	Projection* projection = GameContext<Projection>().Get();

	// ���f���`��
	//�V���̉�]
	m_spaceAngleY += 0.01f;
	m_spaceWorld = DirectX::SimpleMath::Matrix::CreateRotationY(m_spaceAngleY);
	m_pSpace->Draw(deviceContext, *CommonState, m_spaceWorld, viewMatrix, projection->GetMatrix());

	//�n�ʂ̕`��
	m_pGroundModel->Draw(deviceContext, *CommonState, m_groundWorld, viewMatrix, projection->GetMatrix());

	//�Ǖ`��p�̃��[���h�s��
	DirectX::SimpleMath::Matrix world;

	//�ǂ̕`��
	for (int i = 0; i < static_cast<int>(m_wallBoxList.size()); i++)
	{
		//�ǂ̈ʒu�Ƀ��[���h�s��𒲐�
		world = DirectX::SimpleMath::Matrix::CreateTranslation(m_wallBoxList[i].center);
		m_pWallModel->Draw(deviceContext, *CommonState, world, viewMatrix, projection->GetMatrix());
	}
}

//���Z�b�g����
void Map::Reset()
{
	if (m_MapDataList.size() != 0)
	{
		m_MapDataList.clear();
	}

	if (m_wallBoxList.size() != 0)
	{
		m_wallBoxList.clear();
	}
}

//�ǂ̓����蔻��
DirectX::SimpleMath::Vector3 Map::HitWallBox(const Collision::Box3D& target)
{
	DirectX::SimpleMath::Vector3 rePos = target.center;

	for (int i = 0; i < static_cast<int>(m_wallBoxList.size()); i++)
	{
		if (Collision::HitCheckoxBox3D(target, m_wallBoxList[i]))
		{
			if (target.center.x <= (m_wallBoxList[i].center.x + m_wallBoxList[i].size.x)
				&& target.center.x >= (m_wallBoxList[i].center.x - m_wallBoxList[i].size.x))
			{
				if (target.center.z >= (m_wallBoxList[i].center.z + m_wallBoxList[i].size.z))
				{
					rePos = DirectX::SimpleMath::Vector3(
						target.center.x,
						target.center.y,
						(m_wallBoxList[i].center.z + m_wallBoxList[i].size.z + target.size.z)
					);
				}
				else if (target.center.z <= (m_wallBoxList[i].center.z - m_wallBoxList[i].size.z))
				{
					rePos = DirectX::SimpleMath::Vector3(
						target.center.x,
						target.center.y,
						(m_wallBoxList[i].center.z - m_wallBoxList[i].size.z - target.size.z)
					);
				}
			}
			else if (target.center.z <= (m_wallBoxList[i].center.z + m_wallBoxList[i].size.z)
				&& target.center.z >= (m_wallBoxList[i].center.z - m_wallBoxList[i].size.z))
			{
				if (target.center.x >= (m_wallBoxList[i].center.x + m_wallBoxList[i].size.x))
				{
					rePos = DirectX::SimpleMath::Vector3(
						(m_wallBoxList[i].center.x + m_wallBoxList[i].size.x + target.size.x),
						target.center.y,
						target.center.z);
				}
				else if (target.center.x <= (m_wallBoxList[i].center.x - m_wallBoxList[i].size.x))
				{
					rePos = DirectX::SimpleMath::Vector3(
						(m_wallBoxList[i].center.x - m_wallBoxList[i].size.x - target.size.x),
						target.center.y,
						target.center.z);
				}
			}
		}
	}

	return rePos;
}

//�Ԃɕǂ����邩
DirectX::SimpleMath::Vector3 Map::LineOfSight(const DirectX::SimpleMath::Vector3 &pos, const DirectX::SimpleMath::Vector3 &baseFront, const DirectX::SimpleMath::Matrix &rotation)
{
	Collision::Box3D splite
	{
		pos,
		DirectX::SimpleMath::Vector3(0.5f,0.5f,10.0f),
	};

	DirectX::SimpleMath::Vector3 boxPos;

	for (int i = 0; i < static_cast<int>(m_wallBoxList.size()); i++)
	{
		boxPos = Collision::DivideEquallyBox_Plus(splite, baseFront, rotation, m_wallBoxList[i], 20, 2);

		if (boxPos != DirectX::SimpleMath::Vector3::One * 100)
		{
			break;
		}
	}

	return boxPos;
}

//�O���̌��E
DirectX::SimpleMath::Vector3 Map::Limit(const DirectX::SimpleMath::Vector3 &pos, float size)
{
	//return�ŕԂ��l
	DirectX::SimpleMath::Vector3 retPos = pos;

	//���̔���
	if (pos.x + size >= m_halfWidthSize)
	{
		retPos.x = m_halfWidthSize - size;
	}
	else if (pos.x- size <= -m_halfWidthSize)
	{
		retPos.x = -m_halfWidthSize + size;
	}

	//�c�̔���
	if (pos.z+ size >= m_halfHightSize)
	{
		retPos.z = m_halfHightSize - size;
	}
	else if (retPos.z- size <= -m_halfHightSize)
	{
		retPos.z = -m_halfHightSize + size;
	}

	return retPos;
}

//�ړ����E�̕ǂ̃G�t�F�N�g(�v���C���[�����Ăяo���Ȃ�)
void Map::LimitEffect(const DirectX::SimpleMath::Vector3 &pos, float size)
{
	//�ǂ̈ʒu
	DirectX::SimpleMath::Vector3 wallPos = pos;
	//�����c(Z��)�̂ǂ���Ƀf�[�^�����邩����
	bool next = false;
	//�O��ƍ��E�ǂ���̈ʒu��ݒ肷�邩����
	CheckLimitEffect check;

	if (std::abs((pos.x - m_halfWidthSize)) <= m_halfWidthSize)
	{
		wallPos.x = m_halfWidthSize;

		check = CheckLimitEffect::CheckLeft;
	}
	else if (std::abs((pos.x + m_halfWidthSize)) <= m_halfWidthSize)
	{
		wallPos.x = -m_halfWidthSize;

		check = CheckLimitEffect::CheckRight;
	}

	//�f�[�^�̐ݒ�(���̃f�[�^�̐ݒ�)
	LimitEffectSetPos(pos, wallPos, check);
	next = true;

	wallPos = pos;
	if (std::abs((pos.z - m_halfHightSize)) <= m_halfHightSize)
	{
		wallPos.z = m_halfHightSize;

		check = CheckLimitEffect::CheckZ;
	}

	else if (std::abs((pos.z + m_halfHightSize)) <= m_halfHightSize)
	{
		wallPos.z = -m_halfHightSize;

		check = CheckLimitEffect::CheckZ;
	}

	//�f�[�^�̐ݒ�(���̃f�[�^�̐ݒ�)
	LimitEffectSetPos(pos, wallPos,check);
}

//�G�t�F�N�g�p�̈ʒu��ݒ�
void Map::LimitEffectSetPos(const DirectX::SimpleMath::Vector3 &playerPos, const DirectX::SimpleMath::Vector3 &wallPos, const CheckLimitEffect &check)
{
	//�ړ����E�̃G�t�F�N�g�̕\��
	int i = 0;

	if (check == CheckLimitEffect::CheckZ)
	{
		i++;
	}

	//�ړ����E�̃G�t�F�N�g�̊p�x(��)
	if (check == CheckLimitEffect::CheckRight)
	{
		//�������p�x��ς��Ȃ�����0���w��
		m_pHitWallList[0]->SetRange(EFFECT_ANGLE_R);
	}
	else if(check == CheckLimitEffect::CheckLeft)
	{
		//�������p�x��ς��Ȃ�����0���w��
		m_pHitWallList[0]->SetRange(EFFECT_ANGLE_L);
	}

	//�ǂ̈ʒu
	m_pHitWallList[i]->SetWallPos(wallPos);
	//�v���C���[�̈ʒu
	m_pHitWallList[i]->SetPlayerPos(playerPos);
}

//�������������̕ǂ̓�̊p�̈ʒu��Ԃ�
Map::TwoCornerPos Map::GetHitWallCorner(const DirectX::SimpleMath::Vector3 &pos) const
{
	TwoCornerPos ret =
	{
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
	};

	if (pos.x >= m_halfWidthSize)
	{
		ret.pos1 = DirectX::SimpleMath::Vector3(m_halfWidthSize, 0.0f, -m_halfHightSize);
		ret.pos2 = DirectX::SimpleMath::Vector3(m_halfWidthSize, 0.0f, m_halfHightSize);
	}

	else if (pos.x <= -m_halfWidthSize)
	{
		ret.pos1 = DirectX::SimpleMath::Vector3(-m_halfWidthSize,0.0f, -m_halfHightSize);
		ret.pos2 = DirectX::SimpleMath::Vector3(-m_halfWidthSize, 0.0f, m_halfHightSize);
	}

	if (pos.z >= m_halfHightSize)
	{
		ret.pos1 = DirectX::SimpleMath::Vector3(m_halfWidthSize, 0.0f, m_halfHightSize);
		ret.pos2 = DirectX::SimpleMath::Vector3(-m_halfWidthSize, 0.0f, m_halfHightSize);
	}

	else if (pos.z <= -m_halfHightSize)
	{
		ret.pos1 = DirectX::SimpleMath::Vector3(m_halfWidthSize, 0.0f, -m_halfHightSize);
		ret.pos2 = DirectX::SimpleMath::Vector3(-m_halfWidthSize, 0.0f, -m_halfHightSize);
	}

	return ret;
}

//�}�b�v�f�[�^�̐ݒ�
void Map::SetMapData(std::vector<std::vector<int>> mapDataList)
{
	m_MapDataList = mapDataList;
	//�c�Ɖ��̍L�����i�[����(�����̃f�[�^�����邽�߂�0.5��������)
	m_halfHightSize = (m_MapDataList.size()*0.5f);
	m_halfWidthSize = (m_MapDataList[0].size()*0.5f);
}

//�n�ʂ̍쐬
void Map::CreatGround()
{
	//�����͕ς��Ȃ�����y��1
	//�}�b�v���f���̃T�C�Y��{���̑傫���ɂ��邽�߂Ɂ�2������
	m_groundWorld = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(m_halfWidthSize*2.0f, 1.0f, m_halfHightSize*2.0f));
}

//�ǂ̍쐬
void Map::CreatWall()
{
	//�ǂ̈ʒu
	//X���W
	float x = -m_halfWidthSize;
	//Z���W
	float z = -m_halfHightSize;

	for (int i = 0; i < static_cast<int>(m_MapDataList.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_MapDataList[i].size()); j++)
		{
			//�ǂ̎�
			if (m_MapDataList[i][j] == 1)
			{
				Collision::Box3D box =
				{
					//�ǂ̈ʒu
					DirectX::SimpleMath::Vector3(x, 0.0f, z),
					//�ǂ̓����蔻��̑傫��
					WALL_BOX_SIZE,
				};

				//�ǂ̃f�[�^��ݒ�
				m_wallBoxList.push_back(box);
			}
			x++;
		}
		//�ʒu�̒���
		x = -m_halfWidthSize;
		z++;
	}
}