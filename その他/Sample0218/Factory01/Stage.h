#pragma once

#include	<vector>
#include	"Slime.h"
#include	"Goblin.h"

/**
 * ステージクラス
 */
class Stage {
private:
	/**
	 * 敵の出現情報
	 */
	struct SpawnData {
		/** 出現するスクロール位置 */
		int scroll;
		/** 出現するX位置 */
		int posX;
		/** 出現するY位置 */
		int posY;
		/** 出現する敵の種類 */
		int type;
	};
	/** 敵の出現位置情報 */
	std::vector<SpawnData> spawns;
	/** 現在までに出現した番号 */
	size_t currentSpawn;
public:
	/**
	 * コンストラクタ
	 */
	Stage()
		: spawns()
		, currentSpawn(0)
	{
	}
	/**
	 * デストラクタ
	 */
	virtual ~Stage()
	{
	}

	/**
	 * @brief	ステージの生成
	 */
	void Create()
	{
		//敵の出現を適当に登録
		// 0...スライム
		// 1...ゴブリン
		spawns.push_back({ 10,100,100,0 });
		spawns.push_back({ 15,100,100,0 });
		spawns.push_back({ 15,100,100,0 });
		spawns.push_back({ 20,50,100,1 });
		spawns.push_back({ 20,150,100,1 });
	}

	/**
	 * @brief	敵の出現
	 * @param[in]	scroll		スクロール値
	 * @param[in]	enemys		敵リスト
	 */
	void Spawn(int scroll, std::vector<Enemy*>& enemys)
	{
		while (currentSpawn < spawns.size() && scroll >= spawns[currentSpawn].scroll)
		{
			Enemy* en = nullptr;
			const SpawnData& spd = spawns[currentSpawn];
			//敵の種類により生成分岐
			switch (spd.type)
			{
			case 0:			//スライムの生成
				en = new Slime();
				break;

			case 1:			//ゴブリンの生成
				en = new Goblin();
				break;
			}
			en->SetPosition(spd.posX, spd.posY);
			//敵として登録
			enemys.push_back(en);
			//次の敵へ
			currentSpawn++;
		}
	}
};