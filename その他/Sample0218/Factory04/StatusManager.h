#pragma once

#include	<unordered_map>
#include	"Singleton.h"
#include	"Status.h"

/**
 * ステータス管理
 */
class StatusManager : public Singleton<StatusManager>
{
private:
	/** ステータス */
	std::unordered_map<std::string, StatusPtr> status;
public:
	/**
	 * コンストラクタ
	 */
	StatusManager()
		: status()
	{
	}
	/**
	 * デストラクタ
	 */
	~StatusManager()
	{
	}

	/**
	 * @brief		ステータスを登録する
	 * @param[in]	key			登録キー
	 * @param[in]	st			登録ステータス
	 */
	void Register(const std::string& key, StatusPtr st) {
		status[key] = st;
	}

	/**
	 * @brief		ステータスを登録する
	 * @param[in]	key			登録キー
	 * @param[in]	hp			登録ステータス
	 * @param[in]	mp			登録ステータス
	 * @param[in]	str			登録ステータス
	 * @param[in]	def			登録ステータス
	 * @param[in]	spd			登録ステータス
	 */
	void Register(const std::string& key, int hp, int mp, int str, int def, int spd) {
		status[key] = std::make_shared<Status>(hp, mp, str, def, spd);
	}

	/**
	 * @brief		ステータスを返す
	 * @param[in]	key			検索キー
	 */
	StatusPtr GetStatus(const std::string& key) {
		auto obj = status.find(key);
		if (obj != status.end())
		{
			return obj->second;
		}
		return StatusPtr();
	}
};