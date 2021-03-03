#pragma once

#include	"AudioManager.h"
#include	"../Vector3.h"

namespace Sample {

	class Listener {
	protected:
		/** ���X�i�[�̈ʒu */
		Vector3F		Position_;
		/** ���X�i�[�̑��x */
		Vector3F		Velocity_;
		/** ���X�i�[�̑O���� */
		Vector3F		Front_;
		/** ���X�i�[�̏���� */
		Vector3F		Up_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		Listener();
		/**
		 * @brief		�R�s�[�R���X�g���N�^
		 * @param[in]	obj	�R�s�[����I�u�W�F�N�g
		 */
		Listener(const Listener& obj);

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Listener();

		/**
		 * @brief		���X�i�[�̍X�V�������Ȃ�
		 */
		virtual void Update();


		/**
		 * @brief		Position�ւ�Accessor
		 * @param[in]	p	�ʒu
		 */
		void Position(const Vector3F& p) { Position_ = p; }
		/**
		 * @brief		Position�ւ�Accessor
		 * @return		�ʒu
		 */
		const Vector3F& Position() const noexcept { return Position_; }

		/**
		 * @brief		Velocity�ւ�Accessor
		 * @param[in]	p	���x
		 */
		void Velocity(const Vector3F& p) { Velocity_ = p; }
		/**
		 * @brief		Velocity�ւ�Accessor
		 * @return		���x
		 */
		const Vector3F& Velocity() const noexcept { return Velocity_; }

		/**
		 * @brief		Front�ւ�Accessor
		 * @param[in]	p	�O
		 */
		void Front(const Vector3F& p) { Front_ = p; }
		/**
		 * @brief		Front�ւ�Accessor
		 * @return		�O
		 */
		const Vector3F& Front() const noexcept { return Front_; }

		/**
		 * @brief		Up�ւ�Accessor
		 * @param[in]	p	��
		 */
		void Up(const Vector3F& p) { Up_ = p; }
		/**
		 * @brief		Up�ւ�Accessor
		 * @return		��
		 */
		const Vector3F& Up() const noexcept { return Up_; }
	};
}