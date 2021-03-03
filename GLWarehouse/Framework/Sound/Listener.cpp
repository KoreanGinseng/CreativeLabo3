#include	"Listener.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
Listener::Listener()
: Position_()
, Velocity_()
, Front_(0, 0, 1)
, Up_(0, 1, 0) {
}
/**
 * @brief		コピーコンストラクタ
 * @param[in]	obj	コピーするオブジェクト
 */
Listener::Listener(const Listener& obj)
: Position_(obj.Position_)
, Velocity_(obj.Velocity_)
, Front_(obj.Front_)
, Up_(obj.Up_) {
}

/**
 * @brief		デストラクタ
 */
Listener::~Listener() {
}

/**
 * @brief		リスナーの更新をおこなう
 */
void Listener::Update() {
	//位置設定
	ALfloat pos[] = {
		Position_.x,	Position_.y,	Position_.z
	};
	alListenerfv(AL_POSITION, pos);
	//速度設定
	ALfloat vel[] = {
		Velocity_.x,	Velocity_.y,	Velocity_.z
	};
	alListenerfv(AL_VELOCITY, vel);
	//向き設定
	ALfloat orientation[] = {
		Front_.x,		Front_.y,		Front_.z,
		Up_.x,			Up_.y,			Up_.z,
	};
	alListenerfv(AL_ORIENTATION, orientation);
}