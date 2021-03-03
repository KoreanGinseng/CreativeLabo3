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
		 * @brief		���\�[�X�̐V�K�ǉ�
		 * @param[in]	key		�o�^�L�[
		 * @param[in]	_Types	�ǉ����郊�\�[�X�̏������ɕK�v�Ȃ���
		 * @return		�ǉ��������\�[�X
		 *
		 */
		template < typename... _Types >
		Resource Add(const char* key, _Types&& ... _Args) {
			auto re = std::make_shared<T>(_Args...);
			Resources_[key] = re;
			return re;
		}

		/**
		 * @brief		�������\�[�X�̒ǉ�
		 * @param[in]	key		�o�^�L�[
		 * @param[in]	rc		�ǉ����郊�\�[�X
		 */
		void Add(const char* key, const Resource& rc)
		{
			Resources_[key] = rc;
		}

		/**
		 * @brief		���\�[�X�̎擾
		 * @param[in]	key		�L�[
		 * @return		�Y�����郊�\�[�X
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
		 * @brief		���\�[�X�̎擾
		 * @param[in]	key		�L�[
		 * @return		�Y�����郊�\�[�X
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