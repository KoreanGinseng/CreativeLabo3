#pragma once

#include	<string>
#include	<list>
#include	<memory>

namespace Sample {

/**
 * @brief		C#�X�N���v�g����A�N�Z�X���邽�߂̃��C�u�����Ƃ̃V�F�A�pActor
 *				C#�X�N���v�g����A�N�Z�X�������p�����[�^�[��
 *				���̃N���X�ɃC���^�[�t�F�C�X�Ƃ��ėp�ӂ�
 *				SharedWrapper�Ń��b�v���邱��
 */
class __declspec(novtable) ISharedActor {
public:
	/**
	 * @brief		name�ւ�Accessor
	 * @return		���O
	 */
	virtual const std::string& Name() const noexcept = 0;

	/**
	 * @brief		pos�ւ�Accessor
	 * @param[in]	x	X�ʒu
	 * @param[in]	y	Y�ʒu
	 */
	virtual void Position(const float x, const float y) = 0;

	/**
	 * @brief		posx�ւ�Accessor
	 * @param[in]	x	X�ʒu
	 */
	virtual void X(const float x) = 0;
	/**
	 * @brief		posx�ւ�Accessor
	 * @return		X�ʒu
	 */
	virtual const float X() const noexcept = 0;

	/**
	 * @brief		posy�ւ�Accessor
	 * @param[in]	y	Y�ʒu
	 */
	virtual void Y(const float y) = 0;
	/**
	 * @brief		posy�ւ�Accessor
	 * @return		Y�ʒu
	 */
	virtual const float Y() const noexcept = 0;
	
	/**
	 * @brief		posz�ւ�Accessor
	 * @param[in]	z	Z�ʒu
	 */
	virtual void Z(const float z) = 0;
	/**
	 * @brief		posz�ւ�Accessor
	 * @return		Z�ʒu
	 */
	virtual const float Z() const noexcept = 0;
};

}