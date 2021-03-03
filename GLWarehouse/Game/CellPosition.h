#pragma once

#include	"GameDefine.h"

namespace Sample {

	class CellPosition : public ParameterBase<CellPosition> {
	private:
		Vector3I Position_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		CellPosition()
		: ParameterBase()
		, Position_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		CellPosition(const CellPosition& obj)
		: ParameterBase(obj)
		, Position_() {
		}

		/**
		 * @brief		デストラクタ
		 */
		~CellPosition() {
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
		}

		/**
		 * @brief		移動
		 * @param[in]	mx		横移動量
		 * @param[in]	my		縦移動量
		 * @param[in]	mz		奥移動量
		 */
		void Move(const int mx, const int my, const int mz)
		{
			Position_.x += mx;
			Position_.y += my;
			Position_.z += mz;
		}

		/**
		 * @brief		位置設定
		 * @param[in]	x		横移動量
		 * @param[in]	y		縦移動量
		 * @param[in]	z		奥移動量
		 */
		void Position(const int x, const int y, const int z)
		{
			Position_.x = x;
			Position_.y = y;
			Position_.z = z;
			actor->Position(ScreenPosition());
		}

		/**
		 * @brief		セル座標
		 */
		const Vector3I& Position() const {
			return Position_;
		}
		/**
		 * @brief		スクリーン座標
		 */
		Vector3F ScreenPosition() const {
			return Vector3F(Position_.x * CellSize, Position_.y * CellSize, Position_.z * CellSize);
		}
	};
	//ポインタ置き換え
	using CellPositionPtr = std::shared_ptr<CellPosition>;

}