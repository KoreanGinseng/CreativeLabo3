#pragma once

#include	<vector>
#include	"Slime.h"
#include	"Goblin.h"

/**
 * �X�e�[�W�N���X
 */
class Stage {
private:
	/**
	 * �G�̏o�����
	 */
	struct SpawnData {
		/** �o������X�N���[���ʒu */
		int scroll;
		/** �o������X�ʒu */
		int posX;
		/** �o������Y�ʒu */
		int posY;
		/** �o������G�̎�� */
		int type;
	};
	/** �G�̏o���ʒu��� */
	std::vector<SpawnData> spawns;
	/** ���݂܂łɏo�������ԍ� */
	size_t currentSpawn;
public:
	/**
	 * �R���X�g���N�^
	 */
	Stage()
		: spawns()
		, currentSpawn(0)
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	virtual ~Stage()
	{
	}

	/**
	 * @brief	�X�e�[�W�̐���
	 */
	void Create()
	{
		//�G�̏o����K���ɓo�^
		// 0...�X���C��
		// 1...�S�u����
		spawns.push_back({ 10,100,100,0 });
		spawns.push_back({ 15,100,100,0 });
		spawns.push_back({ 15,100,100,0 });
		spawns.push_back({ 20,50,100,1 });
		spawns.push_back({ 20,150,100,1 });
	}

	/**
	 * @brief	�G�̏o��
	 * @param[in]	scroll		�X�N���[���l
	 * @param[in]	enemys		�G���X�g
	 */
	void Spawn(int scroll, std::vector<Enemy*>& enemys)
	{
		while (currentSpawn < spawns.size() && scroll >= spawns[currentSpawn].scroll)
		{
			Enemy* en = nullptr;
			const SpawnData& spd = spawns[currentSpawn];
			//�G�̎�ނɂ�萶������
			switch (spd.type)
			{
			case 0:			//�X���C���̐���
				en = new Slime();
				break;

			case 1:			//�S�u�����̐���
				en = new Goblin();
				break;
			}
			en->SetPosition(spd.posX, spd.posY);
			//�G�Ƃ��ēo�^
			enemys.push_back(en);
			//���̓G��
			currentSpawn++;
		}
	}
};