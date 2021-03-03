#pragma once

#include	"../Common.h"

namespace Sample {

	class AudioManager
	{
	private:
		/** AL�f�o�C�X */
		ALCdevice*					ALDevice_;
		/** AL�R���e�L�X�g */
		ALCcontext*					ALContext_;

		AudioManager()
		: ALDevice_(nullptr)
		, ALContext_(nullptr) {
		}
		AudioManager(const AudioManager& obj) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		virtual ~AudioManager() = default;
	public:
		/**
		 * @brief		OpenAL�̏�����
		 * @return		true	��������
		 *				false	�������s
		 */
		bool Initialize();

		/**
		 * @brief		�������
		 */
		void Release();

		void DistanceModel(ALenum en);
		void DopplerFactor(ALfloat v);
		void DopplerVelocity(ALfloat v);
		ALCdevice* Device() { return ALDevice_; }
		ALCcontext* Ccontext() { return ALContext_; }

		static AudioManager& Instance() {
			static AudioManager t;
			return t;
		}

	};
}