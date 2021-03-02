#pragma once

#include	<unordered_map>
#include	"Singleton.h"
#include	"Status.h"

/**
 * �X�e�[�^�X�Ǘ�
 */
class StatusManager : public Singleton<StatusManager>
{
private:
	/** �X�e�[�^�X */
	std::unordered_map<std::string, StatusPtr> status;
public:
	/**
	 * �R���X�g���N�^
	 */
	StatusManager()
		: status()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~StatusManager()
	{
	}

	/**
	 * @brief		�X�e�[�^�X��o�^����
	 * @param[in]	key			�o�^�L�[
	 * @param[in]	st			�o�^�X�e�[�^�X
	 */
	void Register(const std::string& key, StatusPtr st) {
		status[key] = st;
	}

	/**
	 * @brief		�X�e�[�^�X��o�^����
	 * @param[in]	key			�o�^�L�[
	 * @param[in]	hp			�o�^�X�e�[�^�X
	 * @param[in]	mp			�o�^�X�e�[�^�X
	 * @param[in]	str			�o�^�X�e�[�^�X
	 * @param[in]	def			�o�^�X�e�[�^�X
	 * @param[in]	spd			�o�^�X�e�[�^�X
	 */
	void Register(const std::string& key, int hp, int mp, int str, int def, int spd) {
		status[key] = std::make_shared<Status>(hp, mp, str, def, spd);
	}

	/**
	 * @brief		�X�e�[�^�X��Ԃ�
	 * @param[in]	key			�����L�[
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