#include	"Speaker.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
Speaker::Speaker()
: Position_(Vector3F(0, 0, 0))
, Velocity_(Vector3F(0, 0, 0))
, Direction_(Vector3F(0, 0, 1))
, Gain_(1.0f)
, Pitch_(1.0f) 
, ReferenceDistance_(0)
, MaxDistance_(0)
, Rolloff_(0)
, ConeInnerAngle_(0)
, ConeOuterAngle_(0)
, ConeOuterGain_(0.0f)
, Sounds_() {
}
/**
 * @brief		コピーコンストラクタ
 * @param[in]	obj	コピーするオブジェクト
 */
Speaker::Speaker(const Speaker& obj)
: Position_(obj.Position_)
, Velocity_(obj.Velocity_)
, Direction_(obj.Direction_)
, Gain_(obj.Gain_)
, Pitch_(obj.Pitch_)
, ReferenceDistance_(obj.ReferenceDistance_)
, MaxDistance_(obj.MaxDistance_)
, Rolloff_(obj.Rolloff_)
, ConeInnerAngle_(obj.ConeInnerAngle_)
, ConeOuterAngle_(obj.ConeOuterAngle_)
, ConeOuterGain_(obj.ConeOuterGain_)
, Sounds_(obj.Sounds_) {
}

/**
 * @brief		デストラクタ
 */
Speaker::~Speaker() {
}

/**
 * @brief		ソースの更新をおこなう
 */
void Speaker::SetState(ALuint source) {
	//位置設定
	if (Position_.Enable)
	{
		ALfloat pos[] = {
			Position_.Value.x,	Position_.Value.y,	Position_.Value.z
		};
		alSourcefv(source, AL_POSITION, pos);
	}
	//速度設定
	if (Velocity_.Enable)
	{
		ALfloat vel[] = {
			Velocity_.Value.x,	Velocity_.Value.y,	Velocity_.Value.z
		};
		alSourcefv(source, AL_VELOCITY, vel);
	}
	//向き設定
	if (Direction_.Enable)
	{
		ALfloat direction[] = {
			Direction_.Value.x,	Direction_.Value.y,	Direction_.Value.z
		};
		alSourcefv(source, AL_DIRECTION, direction);
	}
	//ゲイン
	if (Gain_.Enable)
	{
		alSourcef(source, AL_GAIN, Gain_.Value);
	}
	//ピッチ
	if (Pitch_.Enable)
	{
		alSourcef(source, AL_PITCH, Pitch_.Value);
	}
	//参照距離
	if (ReferenceDistance_.Enable)
	{
		alSourcef(source, AL_REFERENCE_DISTANCE, ReferenceDistance_.Value);
	}
	//最大距離
	if (MaxDistance_.Enable)
	{
		alSourcef(source, AL_MAX_DISTANCE, MaxDistance_.Value);
	}
	//ロールオフ
	if (Rolloff_.Enable)
	{
		alSourcef(source, AL_ROLLOFF_FACTOR, Rolloff_.Value);
	}
	//コーンの内角
	if (ConeInnerAngle_.Enable)
	{
		alSourcef(source, AL_CONE_INNER_ANGLE, ConeInnerAngle_.Value);
	}
	//コーンの外角
	if (ConeOuterAngle_.Enable)
	{
		alSourcef(source, AL_CONE_OUTER_ANGLE, ConeOuterAngle_.Value);
	}
	//コーンの外側でのゲイン
	if (ConeOuterGain_.Enable)
	{
		alSourcef(source, AL_CONE_OUTER_GAIN, ConeOuterGain_.Value);
	}
}

/**
 * @brief		スピーカーの更新をおこなう
 */
void Speaker::Update() {
	//設定中の全ての音に対して実行
	for (auto& it : Sounds_)
	{
		SetState(it->Source());
	}
	//再生終了済みの音を削除
	Sounds_.erase(std::remove_if(Sounds_.begin(), Sounds_.end(),
		[](const SoundPtr& pt) {
			ALint state;
			alGetSourcei(pt->Source(), AL_SOURCE_STATE, &state);
			return state != AL_PLAYING;
		}), Sounds_.end());
}

/**
 * @brief		音の再生をおこなう
 */
void Speaker::Play(SoundPtr pt) {
	//リストへのサウンド追加
	Sounds_.push_back(pt);
	//状態設定
	SetState(pt->Source());
	//再生
	pt->Play();
}