#include	"Sound.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
Sound::Sound()
: ResourceBase()
, Buffer_(0)
, Source_(0)
, Wave_() {
}

/**
 * @brief		コンストラクタ
 *				生成と同時に読み込みをおこなう
 * @param[in]	name	読み込むテクスチャ名
 */
Sound::Sound(const char* name)
: ResourceBase()
, Buffer_(0)
, Source_(0)
, Wave_() {
	Load(name);
}

/**
 * @brief		デストラクタ
 */
Sound::~Sound() {
	Release();
}


/**
 * @brief		バッファのフォーマット判定
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
 * @brief		読み込み
 * @param[in]	name	読み込むテクスチャ名
 * @return		true	読み込み成功
 *				false	読み込み失敗
 */
bool Sound::Load(const char* name) {
	//バッファの生成
	alGenBuffers(1, &Buffer_);

	//ファイル読み込み
	Wave_ = std::make_shared<WaveFile>();
	WAVERESULT wr = Wave_->Load(name);
	if (wr != WR_OK)
	{
		ERROR_LOG("サウンド" << name << "が読み込めません[エラーコード:" << wr << "]");
		return false;
	}
	// Set XRAM Mode (if application)
	// if (eaxSetBufferMode && eXRAMBufferMode)
	// 	eaxSetBufferMode(1, &uiBufferID, eXRAMBufferMode);

	alGetError();
	alBufferData(Buffer_, BufferFormat(), Wave_->Data(), Wave_->Size(), Wave_->Frequency());
	if (alGetError() != AL_NO_ERROR)
	{
		ERROR_LOG("サウンド[" << name << "]バッファの生成に失敗");
		return false;
	}
	//ソースの生成
	alGenSources(1, &Source_);
	//ソースにバッファをアタッチ
	alSourcei(Source_, AL_BUFFER, Buffer_);
	Name_ = name;
	INFO_LOG("サウンド" << name << "を読み込み...");
	return true;
}

/**
 * @brief		再生
 */
void Sound::Play() {
	if (Source_ == 0)
	{
		return;
	}
	alSourcePlay(Source_);
}

/**
 * @brief		停止
 */
void Sound::Stop() {
	if (Source_ == 0)
	{
		return;
	}
	alSourceStop(Source_);
}

/**
 * @brief		解放処理
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
	INFO_LOG("サウンド" << Name_ << "を解放...");
	Name_ = "";
	Buffer_ = 0;
	Source_ = 0;
}

/**
 * @brief		ループ設定
 * @param[in]	p	ループフラグ(AL_TRUE：ループあり、AL_FALSE:：ループ無し)
 */
void Sound::Loop(const ALint& p) {
	alSourcei(Source_, AL_LOOPING, p);
}