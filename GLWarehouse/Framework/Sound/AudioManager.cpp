#include	"AudioManager.h"

using namespace Sample;

/**
 * @brief		OpenALの初期化
 * @return		true	生成成功
 *				false	生成失敗
 */
bool AudioManager::Initialize() {
	ALDevice_ = alcOpenDevice(0);
	ALContext_ = alcCreateContext(ALDevice_, NULL);
	alcMakeContextCurrent(ALContext_);
	INFO_LOG("OpenALの初期化...");
	return true;
}

/**
 * @brief		解放処理
 */
void AudioManager::Release() {
	if (ALContext_ == nullptr || ALDevice_ == nullptr)
	{
		return;
	}
	alcMakeContextCurrent(NULL);
	alcDestroyContext(ALContext_);
	alcCloseDevice(ALDevice_);
	ALDevice_ = nullptr;
	ALContext_ = nullptr;
	INFO_LOG("OpenALの解放...");
}

/**
 * @brief		距離モデル設定
 */
void AudioManager::DistanceModel(ALenum en) {
	alDistanceModel(en);
}
/**
 * @brief		ドップラー設定
 */
void AudioManager::DopplerFactor(ALfloat v) {
	alDopplerFactor(v);
}
/**
 * @brief		ドップラー速度設定
 */
void AudioManager::DopplerVelocity(ALfloat v) {
	alDopplerVelocity(v);
}