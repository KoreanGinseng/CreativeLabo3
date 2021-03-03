#pragma once

#include	"Common.h"

namespace Sample {

	template < typename T >
	class ResourceContainer {
	public:
		using Resource = std::shared_ptr< T >;
	private:
		std::unordered_map< std::string, Resource > Resources_;

	public:

		/**
		 * @brief		リソースの新規追加
		 * @param[in]	key		登録キー
		 * @param[in]	_Types	追加するリソースの初期化に必要なもの
		 * @return		追加したリソース
		 *
		 */
		template < typename... _Types >
		Resource Add(const char* key, _Types&& ... _Args) {
			auto re = std::make_shared<T>(_Args...);
			Resources_[key] = re;
			return re;
		}

		/**
		 * @brief		既存リソースの追加
		 * @param[in]	key		登録キー
		 * @param[in]	rc		追加するリソース
		 */
		void Add(const char* key, const Resource& rc)
		{
			Resources_[key] = rc;
		}

		/**
		 * @brief		リソースの取得
		 * @param[in]	key		キー
		 * @return		該当するリソース
		 */
		Resource Get(const char* key) {
			auto it = Resources_.find(key);
			if (it != Resources_.end())
			{
				return it->second;
			}
			return std::shared_ptr< T >();
		}
		Resource Get(const std::string& key) { return Get(key.c_str); }

		/**
		 * @brief		リソースの取得
		 * @param[in]	key		キー
		 * @return		該当するリソース
		 */
		Resource GetFromName(const char* key) {
			for (auto& it : Resources_)
			{
				if (it.second->Name() == key)
				{
					return it.second;
				}
			}
			return std::shared_ptr< T >();
		}
		Resource GetFromName(const std::string& key) { return GetFromName(key.c_str()); }

		std::unordered_map< std::string, Resource >& Resources() { return Resources_; }
	};
}