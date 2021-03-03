#pragma once

#include	"ResourceContainer.h"
#include	"ActorFactory.h"
#include	"Texture.h"
#include	"Shader.h"
#include	"Sound/Sound.h"

namespace Sample {

	/**
	 * @brief		�Ǘ����\�[�X�p�b�N
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
		 * @brief		�o�^�e�N�X�`���̎擾
		 */
		TextureContainer& Textures() noexcept { return Textures_; }
		TexturePtr GetTexture(const char* key) { return Textures_.Get(key); }
		TexturePtr GetTexture(const std::string& key) { return Textures_.Get(key.c_str()); }
		TexturePtr GetTextureFromName(const char* key) { return Textures_.GetFromName(key); }
		TexturePtr GetTextureFromName(const std::string& key) { return Textures_.GetFromName(key.c_str()); }

		/**
		 * @brief		�o�^�V�F�[�_�[�̎擾
		 */
		ShaderContainer& Shaders() noexcept { return Shaders_; }
		ShaderPtr GetShader(const char* key) { return Shaders_.Get(key); }
		ShaderPtr GetShader(const std::string& key) { return Shaders_.Get(key.c_str()); }
		ShaderPtr GetShaderFromName(const char* key) { return Shaders_.GetFromName(key); }
		ShaderPtr GetShaderFromName(const std::string& key) { return Shaders_.GetFromName(key.c_str()); }

		/**
		 * @brief		�o�^�T�E���h�̎擾
		 */
		SoundContainer& Sounds() noexcept { return Sounds_; }
		SoundPtr GetSound(const char* key) { return Sounds_.Get(key); }
		SoundPtr GetSound(const std::string& key) { return Sounds_.Get(key.c_str()); }
		SoundPtr GetSoundFromName(const char* key) { return Sounds_.GetFromName(key); }
		SoundPtr GetSoundFromName(const std::string& key) { return Sounds_.GetFromName(key.c_str()); }

		/**
		 * @brief		�o�^�A�N�^�[�̎擾
		 */
		ActorFactory& GetActorFactory() { return Factory_; }
	};
	using ResourcePackPtr = std::shared_ptr< ResourcePack >;

	/**
	 * @brief		���\�[�X�Ǘ�
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
		 * @brief		�V�K���\�[�X�p�b�N�̐���
		 * @param[in]	key		�L�[
		 * @return		true	����
		 *				false	���s
		 */
		ResourcePackPtr AddPack(const char* key)
		{
			auto re = std::make_shared<ResourcePack>();
			Resources_[key] = re;
			return re;
		}

		/**
		 * @brief		�V�K���\�[�X�p�b�N�̐���
		 * @param[in]	key		�L�[
		 * @return		true	����
		 *				false	���s
		 */
		ResourcePackPtr AddPack(const std::string& key)
		{
			return AddPack(key.c_str());
		}

		/**
		 * @brief		���\�[�X�p�b�N�̍폜
		 * @param[in]	key		�L�[
		 * @return		true	����
		 *				false	���s
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
		 * @brief		���\�[�X�p�b�N�̍폜
		 * @param[in]	key		�L�[
		 * @return		true	����
		 *				false	���s
		 */
		bool DeletePack(const std::string& key)
		{
			return DeletePack(key.c_str());
		}

		/**
		 * @brief		�S�Ă̓o�^���\�[�X����̌���
		 * @param[in]	key		�L�[
		 * @return		�������e�N�X�`��
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
		 * @brief		�S�Ă̓o�^���\�[�X����̌���
		 * @param[in]	key		�L�[
		 * @return		�������e�N�X�`��
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
		 * @brief		�S�Ă̓o�^���\�[�X����̌���
		 * @param[in]	key		�L�[
		 * @return		�������T�E���h
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
		 * @brief		�S�Ă̓o�^���\�[�X����̌���
		 * @param[in]	key		�L�[
		 * @return		�������T�E���h
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
		 * @brief		�S�Ă̓o�^���\�[�X����̌���
		 * @param[in]	key		�L�[
		 * @return		�������V�F�[�_�[
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
		 * @brief		�S�Ă̓o�^���\�[�X����̌���
		 * @param[in]	key		�L�[
		 * @return		�������V�F�[�_�[
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
		 * @brief		�������
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