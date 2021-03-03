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
		 * @brief		�^�C�}�[�̃��Z�b�g
		 */
		void Initialize() {
			glfwSetTime(0.0);
			PrevTime_ = 0;
			Time_ = 0;
		}

		/**
		 * @brief		�t���[�����Ԃ̍X�V
		 *				Framework::Run�ɂČĂяo��
		 */
		void Update() {
			const double NowTime = glfwGetTime();
			Time_ = NowTime - PrevTime_;
			PrevTime_ = NowTime;
		}

		/**
		 * @brief		���Ԏ擾
		 * @return		����
		 */
		const double NowTime() const { return glfwGetTime(); }
		/**
		 * @brief		�O�̃t���[������̌o�ߎ��Ԏ擾
		 * @return		�o�ߎ���
		 */
		constexpr double Time() const { return Time_; }
		/**
		 * @brief		�O�̃t���[������̌o�ߎ��Ԃ�float�^�Ŏ擾
		 * @return		�o�ߎ���
		 */
		constexpr float TimeF() const { return static_cast<float>(Time_); }

		static FrameTimer& Instance() {
			static FrameTimer t;
			return t;
		}
	};

}