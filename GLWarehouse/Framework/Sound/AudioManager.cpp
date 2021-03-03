#include	"AudioManager.h"

using namespace Sample;

/**
 * @brief		OpenAL�̏�����
 * @return		true	��������
 *				false	�������s
 */
bool AudioManager::Initialize() {
	ALDevice_ = alcOpenDevice(0);
	ALContext_ = alcCreateContext(ALDevice_, NULL);
	alcMakeContextCurrent(ALContext_);
	INFO_LOG("OpenAL�̏�����...");
	return true;
}

/**
 * @brief		�������
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
	INFO_LOG("OpenAL�̉��...");
}

/**
 * @brief		�������f���ݒ�
 */
void AudioManager::DistanceModel(ALenum en) {
	alDistanceModel(en);
}
/**
 * @brief		�h�b�v���[�ݒ�
 */
void AudioManager::DopplerFactor(ALfloat v) {
	alDopplerFactor(v);
}
/**
 * @brief		�h�b�v���[���x�ݒ�
 */
void AudioManager::DopplerVelocity(ALfloat v) {
	alDopplerVelocity(v);
}