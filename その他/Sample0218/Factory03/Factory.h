#pragma once

#include	<unordered_map>
#include	"Creator.h"

/**
 * �����H��
 * ���Y�H��ɓo�^�ł���̂�T���p�������I�u�W�F�N�g��
 * �����ł���Creator�݂̂Ƃ���
 */
template <typename T>
class Factory {
private:
	/** ���Y�҃��X�g */
	std::unordered_map< std::string, std::unique_ptr< ICreator<T> >> products;
public:
	/**
	 * @brief	���Y�҂�o�^����
	 *			�n���ꂽptr�͓�����map�ɏ��L�����ڏ�����
	 * @param[in]	key			�o�^�L�[
	 * @param[in]	ptr			�o�^���鐶�Y��
	 */
	void Register(const std::string& key, std::unique_ptr< ICreator<T> > ptr)
	{
		products[key] = std::move(ptr);
	}
	/**
	 * @brief	���Y�҂�o�^����
	 * @param[in]	key			�o�^�L�[
	 */
	template< typename Derived >
	void Register(const std::string& key)
	{
		products[key] = std::make_unique< Creator<T, Derived> >();
	}

	/**
	 * @brief	�����������Ȃ����߂̃��\�b�h
	 * @param[in]	key			�o�^�L�[
	 */
	T* Create(const std::string& key) {
		return products[key]->Create();
	}
};