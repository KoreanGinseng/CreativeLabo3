#include	"Sound.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
 */
Sound::Sound()
: ResourceBase()
, Buffer_(0)
, Source_(0)
, Wave_() {
}

/**
 * @brief		�R���X�g���N�^
 *				�����Ɠ����ɓǂݍ��݂������Ȃ�
 * @param[in]	name	�ǂݍ��ރe�N�X�`����
 */
Sound::Sound(const char* name)
: ResourceBase()
, Buffer_(0)
, Source_(0)
, Wave_() {
	Load(name);
}

/**
 * @brief		�f�X�g���N�^
 */
Sound::~Sound() {
	Release();
}


/**
 * @brief		�o�b�t�@�̃t�H�[�}�b�g����
 */
unsigned long Sound::BufferFormat()
{
	if (Wave_->Type() == WF_EX)
	{
		if (Wave_->ExHeader().nChannels == 1)
		{
			switch (Wave_->ExHeader().wBitsPerSample)
			{
			case 4: return alGetEnumValue("AL_FORMAT_MONO_IMA4");
			case 8: return alGetEnumValue("AL_FORMAT_MONO8");
			case 16: return alGetEnumValue("AL_FORMAT_MONO16");
			}
		}
		else if (Wave_->ExHeader().nChannels == 2)
		{
			switch (Wave_->ExHeader().wBitsPerSample)
			{
			case 4: return alGetEnumValue("AL_FORMAT_STEREO_IMA4");
			case 8: return alGetEnumValue("AL_FORMAT_STEREO8");
			case 16: return alGetEnumValue("AL_FORMAT_STEREO16");
			}
		}
		else if ((Wave_->ExHeader().nChannels == 4) && (Wave_->ExHeader().wBitsPerSample == 16))
		{
			return alGetEnumValue("AL_FORMAT_QUAD16");
		}
	}
	else if (Wave_->Type() == WF_EXT)
	{
		if ((Wave_->ExHeader().nChannels == 1) &&
			((Wave_->ExtensibleHeader().dwChannelMask == SPEAKER_FRONT_CENTER) ||
			(Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)) ||
				(Wave_->ExtensibleHeader().dwChannelMask == 0)))
		{
			switch (Wave_->ExHeader().wBitsPerSample)
			{
			case 4: return alGetEnumValue("AL_FORMAT_MONO_IMA4");
			case 8: return alGetEnumValue("AL_FORMAT_MONO8");
			case 16: return alGetEnumValue("AL_FORMAT_MONO16");
			}
		}
		else if ((Wave_->ExHeader().nChannels == 2) && (Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)))
		{
			switch (Wave_->ExHeader().wBitsPerSample)
			{
			case 4: return alGetEnumValue("AL_FORMAT_STEREO_IMA4");
			case 8: return alGetEnumValue("AL_FORMAT_STEREO8");
			case 16: return alGetEnumValue("AL_FORMAT_STEREO16");
			}
		}
		else if ((Wave_->ExHeader().nChannels == 2) && (Wave_->ExHeader().wBitsPerSample == 16) && (Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)))
		{
			return alGetEnumValue("AL_FORMAT_REAR16");
		}
		else if ((Wave_->ExHeader().nChannels == 4) && (Wave_->ExHeader().wBitsPerSample == 16) && (Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)))
		{
			return alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if ((Wave_->ExHeader().nChannels == 6) && (Wave_->ExHeader().wBitsPerSample == 16) && (Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)))
		{
			return alGetEnumValue("AL_FORMAT_51CHN16");
		}
		else if ((Wave_->ExHeader().nChannels == 7) && (Wave_->ExHeader().wBitsPerSample == 16) && (Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_BACK_CENTER)))
		{
			return alGetEnumValue("AL_FORMAT_61CHN16");
		}
		else if ((Wave_->ExHeader().nChannels == 8) && (Wave_->ExHeader().wBitsPerSample == 16) && (Wave_->ExtensibleHeader().dwChannelMask == (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT)))
		{
			return alGetEnumValue("AL_FORMAT_71CHN16");
		}
	}
	return 0;
}
/**
 * @brief		�ǂݍ���
 * @param[in]	name	�ǂݍ��ރe�N�X�`����
 * @return		true	�ǂݍ��ݐ���
 *				false	�ǂݍ��ݎ��s
 */
bool Sound::Load(const char* name) {
	//�o�b�t�@�̐���
	alGenBuffers(1, &Buffer_);

	//�t�@�C���ǂݍ���
	Wave_ = std::make_shared<WaveFile>();
	WAVERESULT wr = Wave_->Load(name);
	if (wr != WR_OK)
	{
		ERROR_LOG("�T�E���h" << name << "���ǂݍ��߂܂���[�G���[�R�[�h:" << wr << "]");
		return false;
	}
	// Set XRAM Mode (if application)
	// if (eaxSetBufferMode && eXRAMBufferMode)
	// 	eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode);

	alGetError();
	alBufferData(Buffer_, BufferFormat(), Wave_->Data(), Wave_->Size(), Wave_->Frequency());
	if (alGetError() != AL_NO_ERROR)
	{
		ERROR_LOG("�T�E���h[" << name << "]�o�b�t�@�̐����Ɏ��s");
		return false;
	}
	//�\�[�X�̐���
	alGenSources(1, &Source_);
	//�\�[�X�Ƀo�b�t�@���A�^�b�`
	alSourcei(Source_, AL_BUFFER, Buffer_);
	Name_ = name;
	INFO_LOG("�T�E���h" << name << "��ǂݍ���...");
	return true;
}

/**
 * @brief		�Đ�
 */
void Sound::Play() {
	if (Source_ == 0)
	{
		return;
	}
	alSourcePlay(Source_);
}

/**
 * @brief		��~
 */
void Sound::Stop() {
	if (Source_ == 0)
	{
		return;
	}
	alSourceStop(Source_);
}

/**
 * @brief		�������
 */
void Sound::Release() {
	if (Buffer_ == 0)
	{
		return;
	}
	alSourceStop(Source_);
	alDeleteSources(1, &Source_);
	alDeleteBuffers(1, &Buffer_);
	Wave_.reset();
	INFO_LOG("�T�E���h" << Name_ << "�����...");
	Name_ = "";
	Buffer_ = 0;
	Source_ = 0;
}

/**
 * @brief		���[�v�ݒ�
 * @param[in]	p	���[�v�t���O(AL_TRUE�F���[�v����AAL_FALSE:�F���[�v����)
 */
void Sound::Loop(const ALint& p) {
	alSourcei(Source_, AL_LOOPING, p);
}