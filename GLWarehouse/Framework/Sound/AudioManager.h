#pragma once

#include	"../Common.h"

namespace Sample {

	class AudioManager
	{
	private:
		/** ALデバイス */
		ALCdevice*					ALDevice_;
		/** ALコンテキスト */
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
		 * @brief		OpenALの初期化
		 * @return		true	生成成功
		 *				false	生成失敗
		 */
		bool Initialize();

		/**
		 * @brief		解放処理
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