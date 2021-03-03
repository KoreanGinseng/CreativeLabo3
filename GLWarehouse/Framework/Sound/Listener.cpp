#include	"Listener.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
 */
Listener::Listener()
: Position_()
, Velocity_()
, Front_(0, 0, 1)
, Up_(0, 1, 0) {
}
/**
 * @brief		�R�s�[�R���X�g���N�^
 * @param[in]	obj	�R�s�[����I�u�W�F�N�g
 */
Listener::Listener(const Listener& obj)
: Position_(obj.Position_)
, Velocity_(obj.Velocity_)
, Front_(obj.Front_)
, Up_(obj.Up_) {
}

/**
 * @brief		�f�X�g���N�^
 */
Listener::~Listener() {
}

/**
 * @brief		���X�i�[�̍X�V�������Ȃ�
 */
void Listener::Update() {
	//�ʒu�ݒ�
	ALfloat pos[] = {
		Position_.x,	Position_.y,	Position_.z
	};
	alListenerfv(AL_POSITION, pos);
	//���x�ݒ�
	ALfloat vel[] = {
		Velocity_.x,	Velocity_.y,	Velocity_.z
	};
	alListenerfv(AL_VELOCITY, vel);
	//�����ݒ�
	ALfloat orientation[] = {
		Front_.x,		Front_.y,		Front_.z,
		Up_.x,			Up_.y,			Up_.z,
	};
	alListenerfv(AL_ORIENTATION, orientation);
}