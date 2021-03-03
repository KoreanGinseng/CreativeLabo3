#pragma once

#include	"Common.h"

namespace Sample {

	class FrameTimer
	{
	private:
		double			PrevTime_;
		double			Time_;

		FrameTimer()
		: PrevTime_(0)
		, Time_(0) {
		}
		FrameTimer(const FrameTimer& obj) = delete;
		FrameTimer& operator=(const FrameTimer&) = delete;
		virtual ~FrameTimer() = default;
	public:

		/**
		 * @brief		タイマーのリセット
		 */
		void Initialize() {
			glfwSetTime(0.0);
			PrevTime_ = 0;
			Time_ = 0;
		}

		/**
		 * @brief		フレーム時間の更新
		 *				Framework::Runにて呼び出し
		 */
		void Update() {
			const double NowTime = glfwGetTime();
			Time_ = NowTime - PrevTime_;
			PrevTime_ = NowTime;
		}

		/**
		 * @brief		時間取得
		 * @return		時間
		 */
		const double NowTime() const { return glfwGetTime(); }
		/**
		 * @brief		前のフレームからの経過時間取得
		 * @return		経過時間
		 */
		constexpr double Time() const { return Time_; }
		/**
		 * @brief		前のフレームからの経過時間をfloat型で取得
		 * @return		経過時間
		 */
		constexpr float TimeF() const { return static_cast<float>(Time_); }

		static FrameTimer& Instance() {
			static FrameTimer t;
			return t;
		}
	};

}