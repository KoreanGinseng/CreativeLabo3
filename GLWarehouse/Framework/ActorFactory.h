#pragma once

#include	"ParserManager.h"

namespace Sample {
	
	class ActorFactory {
	private:
		std::unordered_map< std::string, ActorPtr > Products_;
	public:

		/**
		 * @brief		�v���_�N�g�̐���
		 * @param[in]	key		�L�[
		 * @return		���������v���_�N�g
		 */
		ActorPtr CreateProduct(const std::string& key)
		{
			ActorPtr pt = std::make_shared<Actor>(key.c_str());
			Products_[key] = pt;
			return pt;
		}

		/**
		 * @brief		�v���_�N�g�̓o�^
		 * @param[in]	key		�L�[
		 * @param[in]	pt		�o�^����v���_�N�g
		 */
		void RegisterProduct(const std::string& key, const ActorPtr pt)
		{
			Products_[key] = pt;
		}

		/**
		 * @brief		�v���_�N�g�̕ۑ�
		 * @param[in]	key		�L�[
		 */
		bool SaveProduct(const std::string& key)
		{
			auto it = Products_.find(key);
			if (it == Products_.end())
			{
				return false;
			}
			nlohmann::json os;
			std::ofstream ofs(key + ".json");
			if (ofs.fail())
			{
				return false;
			}
			it->second->ToJson(os);
			ofs << std::setw(4) << os << std::endl;
			return true;
		}

		/**
		 * @brief		�v���_�N�g�̓ǂݍ���
		 * @param[in]	key		�L�[
		 * @return		���������v���_�N�g
		 */
		ActorPtr LoadProduct(const std::string& key)
		{
			std::ifstream ifs(key + ".json");
			if (ifs.fail())
			{
				return ActorPtr();
			}
			nlohmann::json os = nlohmann::json::parse(ifs);
			ActorPtr pt = ParserManager::Instance().CreateActor(key.c_str());
			pt->FromJson(os);
			Products_[key] = pt;
			return pt;
		}

		/**
		 * @brief		�v���_�N�g����A�N�^�[�𐶐�
		 * @param[in]	key		�L�[
		 * @return		���������A�N�^�[
		 */
		ActorPtr Create(const std::string& key)
		{
			auto it = Products_.find(key);
			if (it == Products_.end())
			{
				return ActorPtr();
			}
			return std::make_shared<Actor>(*it->second);
		}
	};
}