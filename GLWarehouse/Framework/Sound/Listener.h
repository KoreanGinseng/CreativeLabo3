#pragma once

#include	"AudioManager.h"
#include	"../Vector3.h"

namespace Sample {

	class Listener {
	protected:
		/** リスナーの位置 */
		Vector3F		Position_;
		/** リスナーの速度 */
		Vector3F		Velocity_;
		/** リスナーの前向き */
		Vector3F		Front_;
		/** リスナーの上向き */
		Vector3F		Up_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		Listener();
		/**
		 * @brief		コピーコンストラクタ
		 * @param[in]	obj	コピーするオブジェクト
		 */
		Listener(const Listener& obj);

		/**
		 * @brief		デストラクタ
		 */
		virtual ~Listener();

		/**
		 * @brief		リスナーの更新をおこなう
		 */
		virtual void Update();


		/**
		 * @brief		PositionへのAccessor
		 * @param[in]	p	位置
		 */
		void Position(const Vector3F& p) { Position_ = p; }
		/**
		 * @brief		PositionへのAccessor
		 * @return		位置
		 */
		const Vector3F& Position() const noexcept { return Position_; }

		/**
		 * @brief		VelocityへのAccessor
		 * @param[in]	p	速度
		 */
		void Velocity(const Vector3F& p) { Velocity_ = p; }
		/**
		 * @brief		VelocityへのAccessor
		 * @return		速度
		 */
		const Vector3F& Velocity() const noexcept { return Velocity_; }

		/**
		 * @brief		FrontへのAccessor
		 * @param[in]	p	前
		 */
		void Front(const Vector3F& p) { Front_ = p; }
		/**
		 * @brief		FrontへのAccessor
		 * @return		前
		 */
		const Vector3F& Front() const noexcept { return Front_; }

		/**
		 * @brief		UpへのAccessor
		 * @param[in]	p	上
		 */
		void Up(const Vector3F& p) { Up_ = p; }
		/**
		 * @brief		UpへのAccessor
		 * @return		上
		 */
		const Vector3F& Up() const noexcept { return Up_; }
	};
}