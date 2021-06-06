#include "../pch.h"

#include "Collision.h"
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

	//�����蔻��--------------------------------
	//3D-------------------------------------
	//�l�p���m�̓����蔻��
	bool Collision::HitCheckoxBox3D(const Box3D& box1, const Box3D& box2)
	{
		//�l�p�̒��S�ʒu�̋����ƃT�C�Y�̍��v���r
		//��r����Ƃ��͐�Βl�ōs��

		if (fabsf(box1.center.x - box2.center.x) > (box1.size.x + box2.size.x))
			return false;

		if (fabsf(box1.center.y - box2.center.y) > (box1.size.y + box2.size.y))
			return false;

		if (fabsf(box1.center.z - box2.center.z) > (box1.size.z + box2.size.z))
			return false;

		return true;
	}

	//�l�p�̓����蔻��(plus�����ɂ��炵�Ĕ���)���������ʒu��Ԃ�
	DirectX::SimpleMath::Vector3 Collision::DivideEquallyBox_Plus(const Box3D& splitBox, const DirectX::SimpleMath::Vector3& baseFrontVec, const DirectX::SimpleMath::Matrix& rotation, const Box3D& targetBox, const int splitNum, const int select)
	{
		//�����蔻��p�̎l�p�̍ő�̑傫��
		DirectX::SimpleMath::Vector3 fullSize = DirectX::SimpleMath::Vector3::Transform(splitBox.size,rotation);

		//�����蔻��p�̎l�p����]�������l
		Box3D fullBox =
		{
			splitBox.center + (DirectX::SimpleMath::Vector3::Transform(baseFrontVec,rotation)*(splitNum / 2.0f)),
			DirectX::SimpleMath::Vector3(std::abs(fullSize.x),std::abs(fullSize.y),std::abs(fullSize.z))
		};

		//���������邩����
		if (!HitCheckoxBox3D(targetBox, fullBox))
		{
			return DirectX::SimpleMath::Vector3(100.0f, 100.0f, 100.0f);
		}


		//��������̎l�p�̑傫��(�)
		DirectX::SimpleMath::Vector3 splitBaseSize;
		//��������̎l�p�̑傫��(�����蔻��Ɏg�p)
		DirectX::SimpleMath::Vector3 boxSize;
		//�ŏ��Ɍ����Ă�������ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 baseVec;
		//���ʂɌ������x�N�g��(�l�p�̈ʒu�����炷)
		DirectX::SimpleMath::Vector3 frontVec;
		//�����蔻����s���ϐ��̔��a�̃x�N�g�������邽�߂̕ϐ�
		DirectX::SimpleMath::Vector3 radiusVec = DirectX::SimpleMath::Vector3::Zero;
		//�����鎲��1�ӂ̒���
		float length = 0.0f;

		HWD hwd = { 0,0,0 };

		//�c�ɕ�����
		if (height == select)
		{
			hwd.w++;
		}
		//���ɕ�����
		else if (width == select)
		{
			hwd.h++;
		}
		//���ɕ�����
		else if (depth == select)
		{
			hwd.d++;
		}

		baseVec = baseFrontVec;

		if (baseVec.x == 0.0f)
		{
			baseVec.x++;
		}
		if (baseVec.y == 0.0f)
		{
			baseVec.y++;
		}
		if (baseVec.z == 0.0f)
		{
			baseVec.z++;
		}

		//�c�ɕ����邽�ߎl�p�̉��̒�����splitNum�Ŋ���(splitNum�̐��ɕ�������)
		//��������̒������l�p��x�̊�ɂȂ�
		length = ((splitBox.size.x * hwd.w) / (splitNum)) + ((splitBox.size.y * hwd.h) / (splitNum)) + ((splitBox.size.z * hwd.d) / (splitNum));
		//�����蔻��̊���쐬(�c�ɕ����邽�ߕύX����͉̂�(x)�̂�)
		splitBaseSize = DirectX::SimpleMath::Vector3((-splitBox.size.x* (hwd.w - 1)) + (length* hwd.w), (-splitBox.size.y* (hwd.h - 1)) + (length* hwd.h), (-splitBox.size.z* (hwd.d - 1)) + (length* hwd.d));

		//����(pos)����Plus�����ɂɌ������Ĉʒu�����炵�ē����蔻����s��
		//�l�p�̈ʒu�����炷���߂ɉ�
		for (int i = 0; i < splitNum; i++)
		{
			//����(pos�̃f�[�^�������Ă���N���X)�̐��ʂɎl�p�̈ʒu�����炷����
			//for������邽�тɎl�p�̈ʒu������Ă���

			//�����邽��1�ӂ̒��������߂Ă��ꂩ���]������
			frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3((length * hwd.w*baseVec.x)*i, (length * hwd.h*baseVec.y)*i, (length * hwd.d*baseVec.z)*i), rotation);

			//1�񂾂��������邽��
			if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
			{
				//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
				radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3((length * hwd.w), (length * hwd.h), (length * hwd.d)), rotation);
			}

			//�����蔻��̑傫������]������(�l�p������(pos�̃f�[�^�������Ă���N���X)�Ɠ��������ɂȂ邽��)
			boxSize = DirectX::SimpleMath::Vector3::Transform(splitBaseSize, rotation);

			//�����蔻��Ɏg���l�p
			Box3D box =
			{
				splitBox.center + frontVec,
				boxSize,
			};

			//�����蔻��
			if (HitCheckoxBox3D(targetBox, box))
			{
				//����������ʒu��Ԃ�
				return box.center + radiusVec;
			}
		}

		//������Ȃ������ꍇ
		return DirectX::SimpleMath::Vector3(100.0f, 100.0f, 100.0f);
	}

	//�l�p�̓����蔻��(minus�����ɂ��炵�Ĕ���)���������ʒu��Ԃ�
	DirectX::SimpleMath::Vector3 Collision::DivideEquallyBox_Minus(const Box3D& splitBox, const DirectX::SimpleMath::Vector3& baseFrontVec, const DirectX::SimpleMath::Matrix& rotation, const Box3D& targetBox, const int splitNum, const int select, Box3D& data)
	{
		//��������̎l�p�̑傫��(�)
		DirectX::SimpleMath::Vector3 splitBaseSize;
		//��������̎l�p�̑傫��(�����蔻��Ɏg�p)
		DirectX::SimpleMath::Vector3 boxSize;
		//�ŏ��Ɍ����Ă�������ւ̃x�N�g��
		DirectX::SimpleMath::Vector3 baseVec;
		//���ʂɌ������x�N�g��(�l�p�̈ʒu�����炷)
		DirectX::SimpleMath::Vector3 frontVec;
		//�����蔻����s���ϐ��̔��a�̃x�N�g�������邽�߂̕ϐ�
		DirectX::SimpleMath::Vector3 radiusVec = DirectX::SimpleMath::Vector3::Zero;
		//�����鎲��1�ӂ̒���
		float length = 0.0f;

		HWD hwd = { 0,0,0 };

		//�c�ɕ�����
		if (height == select)
		{
			hwd.w++;
		}
		//���ɕ�����
		else if (width == select)
		{
			hwd.h++;
		}
		//���ɕ�����
		else if (depth == select)
		{
			hwd.d++;
		}

		baseVec = baseFrontVec;

		if (baseVec.x == 0.0f)
		{
			baseVec.x++;
		}
		if (baseVec.y == 0.0f)
		{
			baseVec.y++;
		}
		if (baseVec.z == 0.0f)
		{
			baseVec.z++;
		}

		//�c�ɕ����邽�ߎl�p�̉��̒�����splitNum�Ŋ���(splitNum�̐��ɕ�������)
		//��������̒������l�p��x�̊�ɂȂ�
		length = ((splitBox.size.x * hwd.w) / (splitNum)) + ((splitBox.size.y * hwd.h) / (splitNum)) + ((splitBox.size.z * hwd.d) / (splitNum));
		//�����蔻��̊���쐬(�c�ɕ����邽�ߕύX����͉̂�(x)�̂�)
		splitBaseSize = DirectX::SimpleMath::Vector3((-splitBox.size.x* (hwd.w - 1)) + (length* hwd.w), (-splitBox.size.y* (hwd.h - 1)) + (length* hwd.h), (-splitBox.size.z* (hwd.d - 1)) + (length* hwd.d));

		//����(pos)����Plus�����ɂɌ������Ĉʒu�����炵�ē����蔻����s��
		//�l�p�̈ʒu�����炷���߂ɉ�
		for (int i = 0; i < splitNum; i++)
		{
			//����(pos�̃f�[�^�������Ă���N���X)�̐��ʂɎl�p�̈ʒu�����炷����
			//for������邽�тɎl�p�̈ʒu������Ă���

			//�����邽��1�ӂ̒��������߂Ă��ꂩ���]������
			frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3((length * hwd.w*baseVec.x)*i, (length * hwd.h*baseVec.y)*i, (length * hwd.d*baseVec.z)*i), rotation);

			//1�񂾂��������邽��
			if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
			{
				//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
				radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3((length * hwd.w), (length * hwd.h), (length * hwd.d)), rotation);
			}

			//�����蔻��̑傫������]������(�l�p������(pos�̃f�[�^�������Ă���N���X)�Ɠ��������ɂȂ邽��)
			boxSize = DirectX::SimpleMath::Vector3::Transform(splitBaseSize, rotation);

			//�����蔻��Ɏg���l�p
			Box3D box =
			{
				splitBox.center + frontVec,
				boxSize,
			};

			//�m�F�p(��قǏ���)
			data = box;

			//�����蔻��
			if (HitCheckoxBox3D(targetBox, box))
			{
				//����������ʒu��Ԃ�
				return box.center + radiusVec;
			}
		}

		//������Ȃ������ꍇ
		return DirectX::SimpleMath::Vector3(100.0f, 100.0f, 100.0f);
	}

	//�l�p�̓����蔻��(plus�Eminus���݂Ɉʒu�����炵�Ĕ���)���������ʒu��Ԃ�
	DirectX::SimpleMath::Vector3 Collision::DivideEquallyBox_Alternate(const Box3D& splitBox, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Matrix& rotation, const Box3D& targetBox, const int splitNum, const int select, Box3D& data)
	{
		//��������̎l�p�̑傫��(�)
		DirectX::SimpleMath::Vector3 splitBaseSize;
		//��������̎l�p�̑傫��(�����蔻��Ɏg�p)
		DirectX::SimpleMath::Vector3 boxSize;
		//���ʂɌ������x�N�g��(�l�p�̈ʒu�����炷)
		DirectX::SimpleMath::Vector3 frontVec;
		//�����鎲��1�ӂ̒���
		float length = 0.0f;
		//����(pos)�𒆐S�Ɍ���(�O��������͍��E)�ɔ�����s������
		int count = 0;
		//�����蔻����s���ϐ��̔��a�̃x�N�g�������邽�߂̕ϐ�
		DirectX::SimpleMath::Vector3 radiusVec = DirectX::SimpleMath::Vector3::Zero;

		//�c�ɕ�����
		if (height == select)
		{
			//�c�ɕ����邽�ߎl�p�̉��̒�����splitNum�Ŋ���(����splitNum�̐��ɕ�������)
			//��������̒������l�p��x�̊�ɂȂ�
			length = (splitBox.size.x * 2) / (splitNum * 2);
			//�����蔻��̊���쐬(�c�ɕ����邽�ߕύX����͉̂�(x)�̂�)
			//
			splitBaseSize = DirectX::SimpleMath::Vector3(length, splitBox.size.y, splitBox.size.z);
		}
		//���ɕ�����
		else if (width == select)
		{
			//�c�ɕ����邽�ߎl�p�̉��̒�����splitNum�Ŋ���(����splitNum�̐��ɕ�������)
			//��������̒������l�p��y�̊�ɂȂ�
			length = (splitBox.size.y * 2) / (splitNum * 2);
			//�����蔻��̊���쐬(���ɕ����邽�ߕύX����͉̂�(y)�̂�)
			splitBaseSize = DirectX::SimpleMath::Vector3(splitBox.size.x, length, splitBox.size.z);
		}
		//���ɕ�����
		else if (depth == select)
		{
			//�c�ɕ����邽�ߎl�p�̉��̒�����splitNum�Ŋ���(����splitNum�̐��ɕ�������)
			//��������̒������l�p��z�̊�ɂȂ�
			length = (splitBox.size.z * 2) / (splitNum * 2);
			//�����蔻��̊���쐬(���ɕ����邽�ߕύX����͉̂�(z)�̂�)
			splitBaseSize = DirectX::SimpleMath::Vector3(splitBox.size.x, splitBox.size.y, length);
		}

		//����(pos)���琳�ʂɌ������Ĉʒu�����炵�ē����蔻����s��
		//�l�p�̈ʒu�����炷���߂ɉ�
		for (int i = 0; i < splitNum * 2; i++)
		{
			if (i % 2 == 0)
			{
				//����(pos�̃f�[�^�������Ă���N���X)�̐��ʂɎl�p�̈ʒu�����炷����
				//for������邽�тɎl�p�̈ʒu������Ă���
				if (height == select)
				{
					//�c�ŕ����邽��x�̒��������߂Ă��ꂩ���]������
					frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(length*count, 0.0f, 0.0f), rotation);

					//1�񂾂��������邽��
					if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
					{
						//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
						radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(length * 1, 0.0f, 0.0f), rotation);
					}
				}
				else if (width == select)
				{
					//���ŕ����邽��y�̒��������߂Ă��ꂩ���]������
					frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, length*count, 0.0f), rotation);
					//1�񂾂��������邽��
					if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
					{
						//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
						radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, length * 1, 0.0f), rotation);
					}
				}
				else if (depth == select)
				{
					//���ŕ����邽��z�̒��������߂Ă��ꂩ���]������
					frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -length*count), rotation);
					//1�񂾂��������邽��
					if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
					{
						//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
						radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -length * 1), rotation);
					}
				}
			}
			else
			{
				//����(pos�̃f�[�^�������Ă���N���X)�̐��ʂɎl�p�̈ʒu�����炷����
				//for������邽�тɎl�p�̈ʒu������Ă���
				if (height == select)
				{
					//�c�ŕ����邽��x�̒��������߂Ă��ꂩ���]������
					frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(length*-count, 0.0f, 0.0f), rotation);
					//1�񂾂��������邽��
					if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
					{
						//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
						radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(length*-1, 0.0f, 0.0f), rotation);
					}
				}
				else if (width == select)
				{
					//���ŕ����邽��y�̒��������߂Ă��ꂩ���]������
					frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, length*-count, 0.0f), rotation);
					//1�񂾂��������邽��
					if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
					{
						//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
						radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, length*-1, 0.0f), rotation);
					}
				}
				else if (depth == select)
				{
					//���ŕ����邽��z�̒��������߂Ă��ꂩ���]������
					frontVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -length*-count), rotation);
					//1�񂾂��������邽��
					if (radiusVec == DirectX::SimpleMath::Vector3::Zero)
					{
						//�����蔻��Ɏg���ϐ��̔��a�̃x�N�g��������
						radiusVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -length*-1), rotation);
					}
				}
				count++;
			}

			//�����蔻��̑傫������]������(�l�p������(pos�̃f�[�^�������Ă���N���X)�Ɠ��������ɂȂ邽��)
			boxSize = DirectX::SimpleMath::Vector3::Transform(splitBaseSize, rotation);

			//�����蔻��Ɏg���l�p
			Box3D box =
			{
				pos + frontVec,
				boxSize,
			};

			//�m�F�p(��قǏ���)
			data = box;

			//�����蔻��
			if (HitCheckoxBox3D(targetBox, box))
			{
				//����������ʒu��Ԃ�(�����������̎l�p�̈ʒu + ���a�̃x�N�g��) 
				return box.center + radiusVec;
			}
		}

		//������Ȃ������ꍇ
		return DirectX::SimpleMath::Vector3(100.0f, 100.0f, 100.0f);

	}

	//�����m�̓����蔻��
	bool Collision::HitCheckoxSphere3D(const Sphere3D& sphere1, const Sphere3D& sphere2)
	{
		//�����m�̋��������߂�
		DirectX::SimpleMath::Vector3 d;
		d = sphere1.center - sphere2.center;
		//���߂������̓��ς����߂�(��������)
		float dist2 = d.Dot(d);

		//���a�̍��v
		float radiusSum = sphere1.radius + sphere2.radius;
		//�������������������������a�̍��v�����������ꍇ�ɋ��͌������Ă���
		return dist2 <= radiusSum * radiusSum;
	}
	//---------------------------------------

	//2D-------------------------------------
	//�l�p���m�̂̓����蔻��
	bool Collision::HitCheckoxBox2D(const Box2D& box1, const Box2D& box2)
	{
		if (fabsf(box1.center.x - box2.center.x) > (box1.size.x + box2.size.x))
			return false;

		if (fabsf(box1.center.y - box2.center.y) > (box1.size.y + box2.size.y))
			return false;

		return true;
	}

	//�����m�̓����蔻��
	bool Collision::HitCheckSphere2D(const Sphere2D& sphere1, const Sphere2D& sphere2)
	{
		DirectX::SimpleMath::Vector2 distance = sphere1.center - sphere2.center;
		float dist = distance.Dot(distance);

		if (dist < sphere1.radius + sphere2.radius)
			return false;

		return true;
	}
	//---------------------------------------
	//------------------------------------------


	//�搶���g�񂾂���--------------------------
	//���ƃJ�v�Z���̏Փ˔���֐�
	bool Collision::HitCheck_Sphere2Capsule(const Sphere3D& sphere, const Capsule3D& capsule)
	{
		// ���̒��S�ƃJ�v�Z���̒��S�̐����Ƃ̋����̕������v�Z
		float dist2 = SqDistPointSegment(capsule.startPos, capsule.endPos, sphere.center);
		float radius = sphere.radius + capsule.range;
		return dist2 <= radius * radius;
	}

	// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
	bool Collision::HitCheck_Capsule2Capsule(const Capsule3D& capsule1, const Capsule3D& capsule2)
	{
		float s, t;
		DirectX::SimpleMath::Vector3 c1, c2;
		// �J�v�Z���̒��S�̐����Ԃ̋����̕������v�Z
		float dist2 = ClosestPtSegmentSegment(capsule1.startPos, capsule1.endPos, capsule2.startPos, capsule2.endPos, s, t, c1, c2);
		float radius = capsule1.range + capsule2.range;
		return dist2 <= radius * radius;
	}

	// ���ƃ{�b�N�X�̏Փ˔���֐�
	bool Collision::HitCheck_Sphere2Box(const Sphere3D& sphere, const Box3D& box)
	{
		float sqDist = SqDistPointBox(sphere.center, box);
		return sqDist <= sphere.radius * sphere.radius;
	}
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
		float Collision::SqDistPointSegment(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c)
		{
			DirectX::SimpleMath::Vector3 ab = b - a;
			DirectX::SimpleMath::Vector3 ac = c - a;
			DirectX::SimpleMath::Vector3 bc = c - b;
			float e = ac.Dot(ab);
			if (e <= 0.0f) return ac.Dot(ac);
			float f = ab.Dot(ab);
			if (e >= f) return bc.Dot(bc);
			return ac.Dot(ac) - e * e / f;
		}

		// �N�����v�֐�
		inline float Collision::Clamp(float n, float min, float max)
		{
			if (n < min) return min;
			if (n > max) return max;
			return n;
		}

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
		float Collision::ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 q1
			, DirectX::SimpleMath::Vector3 p2, DirectX::SimpleMath::Vector3 q2
			, float &s, float &t
			, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
		{
			DirectX::SimpleMath::Vector3 d1 = q1 - p1;
			DirectX::SimpleMath::Vector3 d2 = q2 - p2;
			DirectX::SimpleMath::Vector3 r = p1 - p2;
			float a = d1.Dot(d1);
			float e = d2.Dot(d2);
			float f = d2.Dot(r);

			if (a <= FLT_EPSILON && e <= FLT_EPSILON)
			{
				s = t = 0.0f;
				c1 = p1;
				c2 = p2;
				return (c1 - c2).Dot(c1 - c2);
			}
			if (a <= FLT_EPSILON)
			{
				s = 0.0f;
				t = f / e;
				t = Clamp(t, 0.0f, 1.0f);
			}
			else
			{
				float c = d1.Dot(r);
				if (e <= FLT_EPSILON)
				{
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else
				{
					float b = d1.Dot(d2);
					float denom = a * e - b * b;
					if (denom != 0.0f)
					{
						s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
					}
					else
					{
						s = 0.0f;
					}
					float tnom = (b * s + f);
					if (tnom < 0.0f)
					{
						t = 0.0f;
						s = Clamp(-c / a, 0.0f, 1.0f);
					}
					else if (tnom > e)
					{
						t = 1.0f;
						s = Clamp((b - c) / a, 0.0f, 1.0f);
					}
					else
					{
						t = tnom / e;
					}
				}
			}
			c1 = p1 + d1 * s;
			c2 = p2 + d2 * t;
			return (c1 - c2).Dot(c1 - c2);
		}

		// �_�ƃ{�b�N�X�̊Ԃ̍ŒZ�����̕������v�Z����֐�
		float Collision::SqDistPointBox(const DirectX::SimpleMath::Vector3& p, const Box3D& b)
		{
			float point[3] = { p.x, p.y, p.z };
			float min[3] = { b.center.x - b.size.x, b.center.y - b.size.y, b.center.z - b.size.z, };
			float max[3] = { b.center.x + b.size.x, b.center.y + b.size.y, b.center.z + b.size.z, };

			float sqDist = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				float v = point[i];
				if (v < min[i]) sqDist += (min[i] - v) * (min[i] - v);
				if (v > max[i]) sqDist += (v - max[i]) * (v - max[i]);
			}
			return sqDist;
		}

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
		bool Collision::IntersectSegmentTriangle(DirectX::SimpleMath::Vector3 p, DirectX::SimpleMath::Vector3 q, Triangle tri, DirectX::SimpleMath::Vector3* s)
		{
			float distp = p.Dot(tri.p.Normal()) + tri.p.D();
			if (distp < 0.0f) return false;
			float distq = q.Dot(tri.p.Normal()) + tri.p.D();
			if (distq >= 0.0f) return false;
			float denom = distp - distq;
			float t = distp / denom;
			*s = p + t * (q - p);
			float u = s->Dot(tri.edgePlaneBC.Normal()) + tri.edgePlaneBC.D();
			if (fabsf(u) < EPSILON) u = 0.0f;
			if (u < 0.0f || u > 1.0f) return false;
			float v = s->Dot(tri.edgePlaneCA.Normal()) + tri.edgePlaneCA.D();
			if (fabsf(v) < EPSILON) v = 0.0f;
			if (v < 0.0f) return false;
			float w = 1.0f - u - v;
			if (fabsf(w) < EPSILON) w = 0.0f;
			if (w < 0.0f) return false;
			return true;
		}
		//------------------------------------------