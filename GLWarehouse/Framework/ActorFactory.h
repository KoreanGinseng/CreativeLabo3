#pragma once

#include	"ParserManager.h"

namespace Sample {
	
	class ActorFactory {
	private:
		std::unordered_map< std::string, ActorPtr > Products_;
	public:

		/**
		 * @brief		プロダクトの生成
		 * @param[in]	key		キー
		 * @return		生成したプロダクト
		 */
		ActorPtr CreateProduct(const std::string& key)
		{
			ActorPtr pt = std::make_shared<Actor>(key.c_str());
			Products_[key] = pt;
			return pt;
		}

		/**
		 * @brief		プロダクトの登録
		 * @param[in]	key		キー
		 * @param[in]	pt		登録するプロダクト
		 */
		void RegisterProduct(const std::string& key, const ActorPtr pt)
		{
			Products_[key] = pt;
		}

		/**
		 * @brief		プロダクトの保存
		 * @param[in]	key		キー
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
		 * @brief		プロダクトの読み込み
		 * @param[in]	key		キー
		 * @return		生成したプロダクト
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
		 * @brief		プロダクトからアクターを生成
		 * @param[in]	key		キー
		 * @return		生成したアクター
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