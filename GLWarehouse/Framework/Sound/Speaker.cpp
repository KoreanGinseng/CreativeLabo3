#include	"Speaker.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
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
 * @brief		�R�s�[�R���X�g���N�^
 * @param[in]	obj	�R�s�[����I�u�W�F�N�g
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
 * @brief		�f�X�g���N�^
 */
Speaker::~Speaker() {
}

/**
 * @brief		�\�[�X�̍X�V�������Ȃ�
 */
void Speaker::SetState(ALuint source) {
	//�ʒu�ݒ�
	if (Position_.Enable)
	{
		ALfloat pos[] = {
			Position_.Value.x,	Position_.Value.y,	Position_.Value.z
		};
		alSourcefv(source, AL_POSITION, pos);
	}
	//���x�ݒ�
	if (Velocity_.Enable)
	{
		ALfloat vel[] = {
			Velocity_.Value.x,	Velocity_.Value.y,	Velocity_.Value.z
		};
		alSourcefv(source, AL_VELOCITY, vel);
	}
	//�����ݒ�
	if (Direction_.Enable)
	{
		ALfloat direction[] = {
			Direction_.Value.x,	Direction_.Value.y,	Direction_.Value.z
		};
		alSourcefv(source, AL_DIRECTION, direction);
	}
	//�Q�C��
	if (Gain_.Enable)
	{
		alSourcef(source, AL_GAIN, Gain_.Value);
	}
	//�s�b�`
	if (Pitch_.Enable)
	{
		alSourcef(source, AL_PITCH, Pitch_.Value);
	}
	//�Q�Ƌ���
	if (ReferenceDistance_.Enable)
	{
		alSourcef(source, AL_REFERENCE_DISTANCE, ReferenceDistance_.Value);
	}
	//�ő勗��
	if (MaxDistance_.Enable)
	{
		alSourcef(source, AL_MAX_DISTANCE, MaxDistance_.Value);
	}
	//���[���I�t
	if (Rolloff_.Enable)
	{
		alSourcef(source, AL_ROLLOFF_FACTOR, Rolloff_.Value);
	}
	//�R�[���̓��p
	if (ConeInnerAngle_.Enable)
	{
		alSourcef(source, AL_CONE_INNER_ANGLE, ConeInnerAngle_.Value);
	}
	//�R�[���̊O�p
	if (ConeOuterAngle_.Enable)
	{
		alSourcef(source, AL_CONE_OUTER_ANGLE, ConeOuterAngle_.Value);
	}
	//�R�[���̊O���ł̃Q�C��
	if (ConeOuterGain_.Enable)
	{
		alSourcef(source, AL_CONE_OUTER_GAIN, ConeOuterGain_.Value);
	}
}

/**
 * @brief		�X�s�[�J�[�̍X�V�������Ȃ�
 */
void Speaker::Update() {
	//�ݒ蒆�̑S�Ẳ��ɑ΂��Ď��s
	for (auto& it : Sounds_)
	{
		SetState(it->Source());
	}
	//�Đ��I���ς݂̉����폜
	Sounds_.erase(std::remove_if(Sounds_.begin(), Sounds_.end(),
		[](const SoundPtr& pt) {
			ALint state;
			alGetSourcei(pt->Source(), AL_SOURCE_STATE, &state);
			return state != AL_PLAYING;
		}), Sounds_.end());
}

/**
 * @brief		���̍Đ��������Ȃ�
 */
void Speaker::Play(SoundPtr pt) {
	//���X�g�ւ̃T�E���h�ǉ�
	Sounds_.push_back(pt);
	//��Ԑݒ�
	SetState(pt->Source());
	//�Đ�
	pt->Play();
}