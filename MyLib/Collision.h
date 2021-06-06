///�l���C�u�����[------------------------------
//�K�{---------
//��
//DirectXTK
//�C���N���[�h
//SimpleMath
//-------------
//���e
//�����蔻��
//3D
//�l�p�Ƌ�
//2D
//�l�p�Ƌ�
///----------------------------------------------

#pragma once

//�C���N���[�h
#include <SimpleMath.h>

class Collision
{
public:
	///�\����-----------------------------------
	//3D-------------------------------------
	//�l�p�`
	struct Box3D
	{
		//���S�ʒu
		DirectX::SimpleMath::Vector3 center;
		//(���S�ʒu�����)����
		DirectX::SimpleMath::Vector3 size;
	};

	//��
	struct Sphere3D
	{
		//���S�ʒu
		DirectX::SimpleMath::Vector3 center;
		//���a
		float radius;
	};

	//---------------------------------------
	//2D-------------------------------------
	//�l�p�`
	struct Box2D
	{
		//���S�ʒu
		DirectX::SimpleMath::Vector2 center;
		//(���S�ʒu�����)����
		DirectX::SimpleMath::Vector2 size;
	};

	//��
	struct Sphere2D
	{
		//���S�ʒu
		DirectX::SimpleMath::Vector2 center;
		//���a
		float radius;
	};

	//�c(Height)�A��(Width)�A���s��(Depth)
	struct HWD
	{
		int h;
		int w;
		int d;
	};
	//---------------------------------------
	///-----------------------------------------

	//------------------------------------------
	enum
	{
		height,
		width,
		depth
	};
	//------------------------------------------

	//�����蔻��--------------------------------
	//3D-------------------------------------
	//�l�p���m�̓����蔻��
	static bool HitCheckoxBox3D(const Box3D& box1, const Box3D& box2);
	//�l�p�̓����蔻��(plus�����ɂ��炵�Ĕ���)���������ʒu��Ԃ�
	static DirectX::SimpleMath::Vector3 DivideEquallyBox_Plus(const Box3D& splitBox, const DirectX::SimpleMath::Vector3& baseFrontVec, const DirectX::SimpleMath::Matrix& rotation, const Box3D& targetBox, const int splitNum, const int select);
	//�l�p�̓����蔻��(minus�����ɂ��炵�Ĕ���)���������ʒu��Ԃ�
	static DirectX::SimpleMath::Vector3 DivideEquallyBox_Minus(const Box3D& splitBox, const DirectX::SimpleMath::Vector3& baseFrontVec, const DirectX::SimpleMath::Matrix& rotation, const Box3D& targetBox, const int splitNum, const int select, Box3D& data);
	//�l�p�̓����蔻��(plus�Eminus���݂Ɉʒu�����炵�Ĕ���)���������ʒu��Ԃ�
	static DirectX::SimpleMath::Vector3 DivideEquallyBox_Alternate(const Box3D& splitBox, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Matrix& rotation, const Box3D& targetBox, const int splitNum, const int select, Box3D& data);

	//�����m�̓����蔻��
	static bool HitCheckoxSphere3D(const Sphere3D& sphere1, const Sphere3D& sphere2);
	//---------------------------------------

	//2D-------------------------------------
	//�l�p���m�̂̓����蔻��
	static bool HitCheckoxBox2D(const Box2D& box1, const Box2D& box2);

	//�����m�̓����蔻��
	static bool HitCheckSphere2D(const Sphere2D& sphere1, const Sphere2D& sphere2);
	//---------------------------------------
	//------------------------------------------



	//�搶���g�񂾂���--------------------------
public:
	// �J�v�Z���̍\����
	struct Capsule3D
	{
		DirectX::SimpleMath::Vector3 startPos; // ���ԕ��̐����̊J�n�_
		DirectX::SimpleMath::Vector3 endPos; // ���ԕ��̐����̏I���_
		float range;   // �J�v�Z���̔��a
	};

	//���ƃJ�v�Z���̏Փ˔���֐�
	static bool HitCheck_Sphere2Capsule(const Sphere3D& sphere, const Capsule3D& capsule);

	// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
	static bool HitCheck_Capsule2Capsule(const Capsule3D& capsule1, const Capsule3D& capsule2);

	// ���ƃ{�b�N�X�̏Փ˔���֐�
	static bool HitCheck_Sphere2Box(const Sphere3D& sphere, const Box3D& box);

	private:
	//------------------------------//
	// �Փ˔���ɕK�v�ȕ⏕�֐��Q	//
	//------------------------------//

	/// <summary>
	/// �_c�Ɛ���ab�̊Ԃ̋����̕�����Ԃ��֐�
	/// </summary>
	/// <param name="a">�����̎n�_</param>
	/// <param name="b">�����̏I�_</param>
	/// <param name="c">�_</param>
	/// <returns>�_c�Ɛ���ab�̊Ԃ̋����̕���</returns>
	static float SqDistPointSegment(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c);

	// �N�����v�֐�
	static inline float Clamp(float n, float min, float max);

	/// <summary>
	/// �Q�̐����̍ŒZ�����̕�����Ԃ��֐�
	/// </summary>
	/// <param name="p1">�����P�̎n�_</param>
	/// <param name="q1">�����P�̏I�_</param>
	/// <param name="p2">�����Q�̎n�_</param>
	/// <param name="q2">�����Q�̏I�_</param>
	/// <param name="s">�����P��̍ŒZ�ʒu��\���W��</param>
	/// <param name="t">�����Q��̍ŒZ�ʒu��\���W��</param>
	/// <param name="c1">�����P��̍ŒZ�����̈ʒu</param>
	/// <param name="c2">�����Q��̍ŒZ�����̈ʒu</param>
	/// <returns>�Q�̐����̍ŒZ�����̕���</returns>
	static float ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1
		, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2
		, float &s, float &t
		, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2);

	// �_�ƃ{�b�N�X�̊Ԃ̍ŒZ�����̕������v�Z����֐�
	static float SqDistPointBox(const DirectX::SimpleMath::Vector3& p, const Box3D& b);

	//------------------------------//
	// �����ƎO�p�`�̌�������p		//
	//------------------------------//

	// �O�p�`�̍\���́i�����ƎO�p�`�̌�������p�j
	struct Triangle
	{
		// �O�p�`�̕��ʕ�����
		DirectX::SimpleMath::Plane p;
		// ��BC�̕��ʕ������i�d�S���W�̒��_a�ɑ΂���d��u��^����j
		DirectX::SimpleMath::Plane edgePlaneBC;
		// ��CA�̕��ʕ������i�d�S���W�̒��_b�ɑ΂���d��v��^����j
		DirectX::SimpleMath::Plane edgePlaneCA;

		// �R���X�g���N�^���ŏՓ˔�����y�����邽�߂ɑO�����Ōv�Z���Ă���
		Triangle(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c)
		{
			DirectX::SimpleMath::Vector3 n = (c - a).Cross(b - a);
			p = DirectX::SimpleMath::Plane(a, n);
			DirectX::SimpleMath::Plane pp = DirectX::SimpleMath::Plane(b, n);
			edgePlaneBC = DirectX::SimpleMath::Plane(b, n.Cross(b - c));
			edgePlaneCA = DirectX::SimpleMath::Plane(c, n.Cross(c - a));
			p.Normalize(); edgePlaneBC.Normalize(); edgePlaneCA.Normalize();
			float bc_scale = 1.0f / (a.Dot(edgePlaneBC.Normal()) + edgePlaneBC.D());
			float ca_scale = 1.0f / (b.Dot(edgePlaneCA.Normal()) + edgePlaneCA.D());
			edgePlaneBC.x *= bc_scale; edgePlaneBC.y *= bc_scale; edgePlaneBC.z *= bc_scale; edgePlaneBC.w *= bc_scale;
			edgePlaneCA.x *= ca_scale; edgePlaneCA.y *= ca_scale; edgePlaneCA.z *= ca_scale; edgePlaneCA.w *= ca_scale;
		}
	};

	// ���������_�̌덷�œ�����ʂ�����̂ŏ����]�T������
#define EPSILON 1.0e-06F

	/// <summary>
	/// �����ƎO�p�`�̌�������
	/// </summary>
	/// <param name="p">�����̎n�_</param>
	/// <param name="q">�����̏I�_</param>
	/// <param name="tri">�O�p�`</param>
	/// <param name="s">�����ƎO�p�`�̌����_</param>
	/// <returns>true�̏ꍇ�������Ă���</returns>
	static bool IntersectSegmentTriangle(DirectX::SimpleMath::Vector3 p, DirectX::SimpleMath::Vector3 q, Triangle tri, DirectX::SimpleMath::Vector3* s);
	//------------------------------------------
};