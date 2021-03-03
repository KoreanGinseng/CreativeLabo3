#pragma once

#include	"ResourceContainer.h"
#include	"ActorFactory.h"
#include	"Texture.h"
#include	"Shader.h"
#include	"Sound/Sound.h"

namespace Sample {

	/**
	 * @brief		管理リソースパック
	 */
	class ResourcePack
	{
	private:
		using TextureContainer = ResourceContainer<Texture>;
		TextureContainer		Textures_;

		using ShaderContainer = ResourceContainer<Shader>;
		ShaderContainer			Shaders_;

		using SoundContainer = ResourceContainer<Sound>;
		SoundContainer			Sounds_;

		ActorFactory			Factory_;
	public:
		ResourcePack()
		: Textures_()
		, Shaders_() 
		, Sounds_()
		, Factory_(){
		}

		/**
		 * @brief		登録テクスチャの取得
		 */
		TextureContainer& Textures() noexcept { return Textures_; }
		TexturePtr GetTexture(const char* key) { return Textures_.Get(key); }
		TexturePtr GetTexture(const std::string& key) { return Textures_.Get(key.c_str()); }
		TexturePtr GetTextureFromName(const char* key) { return Textures_.GetFromName(key); }
		TexturePtr GetTextureFromName(const std::string& key) { return Textures_.GetFromName(key.c_str()); }

		/**
		 * @brief		登録シェーダーの取得
		 */
		ShaderContainer& Shaders() noexcept { return Shaders_; }
		ShaderPtr GetShader(const char* key) { return Shaders_.Get(key); }
		ShaderPtr GetShader(const std::string& key) { return Shaders_.Get(key.c_str()); }
		ShaderPtr GetShaderFromName(const char* key) { return Shaders_.GetFromName(key); }
		ShaderPtr GetShaderFromName(const std::string& key) { return Shaders_.GetFromName(key.c_str()); }

		/**
		 * @brief		登録サウンドの取得
		 */
		SoundContainer& Sounds() noexcept { return Sounds_; }
		SoundPtr GetSound(const char* key) { return Sounds_.Get(key); }
		SoundPtr GetSound(const std::string& key) { return Sounds_.Get(key.c_str()); }
		SoundPtr GetSoundFromName(const char* key) { return Sounds_.GetFromName(key); }
		SoundPtr GetSoundFromName(const std::string& key) { return Sounds_.GetFromName(key.c_str()); }

		/**
		 * @brief		登録アクターの取得
		 */
		ActorFactory& GetActorFactory() { return Factory_; }
	};
	using ResourcePackPtr = std::shared_ptr< ResourcePack >;

	/**
	 * @brief		リソース管理
	 */
	class ResourceManager
	{
	private:
		std::unordered_map< std::string, ResourcePackPtr > Resources_;

		ResourceManager()
		: Resources_() {
		}
		ResourceManager(const ResourceManager& obj) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		virtual ~ResourceManager() = default;
	public:
		/**
		 * @brief		新規リソースパックの生成
		 * @param[in]	key		キー
		 * @return		true	成功
		 *				false	失敗
		 */
		ResourcePackPtr AddPack(const char* key)
		{
			auto re = std::make_shared<ResourcePack>();
			Resources_[key] = re;
			return re;
		}

		/**
		 * @brief		新規リソースパックの生成
		 * @param[in]	key		キー
		 * @return		true	成功
		 *				false	失敗
		 */
		ResourcePackPtr AddPack(const std::string& key)
		{
			return AddPack(key.c_str());
		}

		/**
		 * @brief		リソースパックの削除
		 * @param[in]	key		キー
		 * @return		true	成功
		 *				false	失敗
		 */
		bool DeletePack(const char* key)
		{
			auto it = Resources_.find(key);
			if (it != Resources_.end())
			{
				Resources_.erase(it);
				return true;
			}
			return false;
		}

		/**
		 * @brief		リソースパックの削除
		 * @param[in]	key		キー
		 * @return		true	成功
		 *				false	失敗
		 */
		bool DeletePack(const std::string& key)
		{
			return DeletePack(key.c_str());
		}

		/**
		 * @brief		全ての登録リソースからの検索
		 * @param[in]	key		キー
		 * @return		見つけたテクスチャ
		 */
		TexturePtr GetTexture(const char* key) {
			for (auto it : Resources_)
			{
				auto t = it.second->GetTexture(key);
				if (t)
				{
					return t;
				}
			}
			return TexturePtr();
		}
		TexturePtr GetTexture(const std::string& key) { return GetTexture(key.c_str()); }

		/**
		 * @brief		全ての登録リソースからの検索
		 * @param[in]	key		キー
		 * @return		見つけたテクスチャ
		 */
		TexturePtr GetTextureFromName(const char* key) {
			for (auto it : Resources_)
			{
				auto t = it.second->GetTextureFromName(key);
				if (t)
				{
					return t;
				}
			}
			return TexturePtr();
		}
		TexturePtr GetTextureFromName(const std::string& key) { return GetTextureFromName(key.c_str()); }

		/**
		 * @brief		全ての登録リソースからの検索
		 * @param[in]	key		キー
		 * @return		見つけたサウンド
		 */
		SoundPtr GetSound(const char* key) {
			for (auto it : Resources_)
			{
				auto t = it.second->GetSound(key);
				if (t)
				{
					return t;
				}
			}
			return SoundPtr();
		}
		SoundPtr GetSound(const std::string& key) { return GetSound(key.c_str()); }

		/**
		 * @brief		全ての登録リソースからの検索
		 * @param[in]	key		キー
		 * @return		見つけたサウンド
		 */
		SoundPtr GetSoundFromName(const char* key) {
			for (auto it : Resources_)
			{
				auto t = it.second->GetSoundFromName(key);
				if (t)
				{
					return t;
				}
			}
			return SoundPtr();
		}
		SoundPtr GetSoundFromName(const std::string& key) { return GetSoundFromName(key.c_str()); }

		/**
		 * @brief		全ての登録リソースからの検索
		 * @param[in]	key		キー
		 * @return		見つけたシェーダー
		 */
		ShaderPtr GetShader(const char* key) {
			for (auto it : Resources_)
			{
				auto t = it.second->GetShader(key);
				if (t)
				{
					return t;
				}
			}
			return ShaderPtr();
		}
		ShaderPtr GetShader(const std::string& key) { return GetShader(key.c_str()); }

		/**
		 * @brief		全ての登録リソースからの検索
		 * @param[in]	key		キー
		 * @return		見つけたシェーダー
		 */
		ShaderPtr GetShaderFromName(const char* key) {
			for (auto it : Resources_)
			{
				auto t = it.second->GetShaderFromName(key);
				if (t)
				{
					return t;
				}
			}
			return ShaderPtr();
		}
		ShaderPtr GetShaderFromName(const std::string& key) { return GetShaderFromName(key.c_str()); }

		/**
		 * @brief		解放処理
		 */
		void Release() {
			std::for_each(Resources_.begin(), Resources_.end(), [](auto& ptr) {ptr.second.reset(); });
			Resources_.clear();
		}

		static ResourceManager& Instance() {
			static ResourceManager t;
			return t;
		}
	};

}