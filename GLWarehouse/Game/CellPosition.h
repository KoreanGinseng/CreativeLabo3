#pragma once

#include	"GameDefine.h"

namespace Sample {

	class CellPosition : public ParameterBase<CellPosition> {
	private:
		Vector3I Position_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		CellPosition()
		: ParameterBase()
		, Position_() {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		CellPosition(const CellPosition& obj)
		: ParameterBase(obj)
		, Position_() {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		~CellPosition() {
		}

		/**
		 * @brief		�J�n
		 */
		void Start() override {
		}

		/**
		 * @brief		�ړ�
		 * @param[in]	mx		���ړ���
		 * @param[in]	my		�c�ړ���
		 * @param[in]	mz		���ړ���
		 */
		void Move(const int mx, const int my, const int mz)
		{
			Position_.x += mx;
			Position_.y += my;
			Position_.z += mz;
		}

		/**
		 * @brief		�ʒu�ݒ�
		 * @param[in]	x		���ړ���
		 * @param[in]	y		�c�ړ���
		 * @param[in]	z		���ړ���
		 */
		void Position(const int x, const int y, const int z)
		{
			Position_.x = x;
			Position_.y = y;
			Position_.z = z;
			actor->Position(ScreenPosition());
		}

		/**
		 * @brief		�Z�����W
		 */
		const Vector3I& Position() const {
			return Position_;
		}
		/**
		 * @brief		�X�N���[�����W
		 */
		Vector3F ScreenPosition() const {
			return Vector3F(Position_.x * CellSize, Position_.y * CellSize, Position_.z * CellSize);
		}
	};
	//�|�C���^�u������
	using CellPositionPtr = std::shared_ptr<CellPosition>;

}