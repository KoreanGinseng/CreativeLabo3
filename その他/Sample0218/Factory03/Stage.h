#pragma once

#include	"Enemy.h"
#include	"Factory.h"

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
		std::string key;
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
		spawns.push_back({ 10,100,100,"Slime" });
		spawns.push_back({ 15,100,100,"Slime" });
		spawns.push_back({ 15,100,100,"Slime" });
		spawns.push_back({ 20,50,100,"Goblin" });
		spawns.push_back({ 20,150,100,"Goblin" });
	}

	/**
	 * @brief	�G�̏o��
	 * @param[in]	scroll			�X�N���[���l
	 * @param[in]	enemyFactorys	�G���X�g
	 * @param[in]	enemys			�G���X�g
	 */
	void Spawn(int scroll, Factory<Enemy>& enemyFactorys, std::vector<Enemy*>& enemys)
	{
		while (currentSpawn < spawns.size() && scroll >= spawns[currentSpawn].scroll)
		{
			const SpawnData& spd = spawns[currentSpawn];
			//Factory����G�𐶐�
			Enemy* en = enemyFactorys.Create(spd.key);
			en->SetPosition(spd.posX, spd.posY);
			//�G�Ƃ��ēo�^
			enemys.push_back(en);
			//���̓G��
			currentSpawn++;
		}
	}
};