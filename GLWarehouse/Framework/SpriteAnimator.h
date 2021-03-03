#pragma once

#include	"Collision/Rectangle.h"
#include	"KeyFrameAnimation.h"
#include	"ParserManager.h"

namespace Sample {

	class SpriteAnimator : public ActionBase<SpriteAnimator> {
	protected:

		/** ���݂̃A�j���[�V�������� */
		float NowTime_;

		/** ���݂̃A�j���[�V������` */
		RectangleF SrcRect_;

		using AnimationPtr = std::shared_ptr< KeyFrameAnimation<RectangleF> >;

		/** ���s���A�j���[�V���� */
		AnimationPtr									NowAnimation_;
		/** �A�j���[�V���� */
		std::unordered_map< std::string, AnimationPtr > Animations_;

		/**
		 * @brief		��`�X�V
		 */
		void RefreshRect() {
			if (!IsAnimation())
			{
				return;
			}
			SrcRect_ = NowAnimation_->GetValue(NowTime_);
		}
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		SpriteAnimator()
		: ActionBase()
		, NowTime_(0)
		, SrcRect_() 
		, NowAnimation_()
		, Animations_() {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		SpriteAnimator(const SpriteAnimator& obj)
		: ActionBase(obj)
		, NowTime_(obj.NowTime_)
		, SrcRect_(obj.SrcRect_) 
		, NowAnimation_(obj.NowAnimation_)
		, Animations_(obj.Animations_) {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		~SpriteAnimator() {
		}

		/**
		 * @brief		�A�j���[�V�����o�^
		 * @param[in]	key		�L�[
		 * @param[in]	anim	�A�j���[�V����
		 */
		void Register(const std::string& key, const KeyFrameAnimation<RectangleF>::Animation& anim) {
			KeyFrameAnimation<RectangleF>::Animation temp = anim;
			float at = 0;
			for (auto& fm : temp.Frames)
			{
				at += fm.Time;
				fm.Time = at;
			}

			auto ap = std::make_shared<KeyFrameAnimation<RectangleF>>(temp, std::make_shared< NearestInterpolation <RectangleF > >());
			Animations_[key] = ap;
		}

		/**
		 * @brief		�A�j���[�V�����ύX
		 * @param[in]	key		�L�[
		 * @param[in]	same	����A�j���[�V�����̏㏑���itrue�ōŏ�����Đ������j
		 * @return		true	�A�j���[�V��������
		 *				false	�A�j���[�V�����Ȃ�
		 */
		bool ChangeAnimation(const std::string& key, bool same = false) {
			auto it = Animations_.find(key);
			if (it == Animations_.end())
			{
				return false;
			}
			//���Ԃ̃��Z�b�g
			if (it->second != NowAnimation_ || same)
			{
				NowTime_ = 0;
			}
			NowAnimation_ = it->second;
			RefreshRect();
			return true;
		}

		/**
		 * @brief		�J�n
		 */
		void Start() override {
		}

		/**
		 * @brief		�X�V
		 */
		void Update() override {
			if (IsAnimation())
			{
				NowTime_ += FrameTimer::Instance().TimeF();
				RefreshRect();
			}
		}

		/**
		 * @brief		��`�擾
		 * @return		��`
		 */
		const RectangleF& SrcRect() const {
			return SrcRect_;
		}

		/**
		 * @brief		��`�ݒ�
		 * @param[in]	��`
		 */
		void SrcRect(const RectangleF& r) {
			SrcRect_ = r;
		}

		/**
		 * @brief		�A�j���[�V���������s���邩�擾
		 * @return		true	�A�j���[�V��������
		 *				false	�A�j���[�V�����Ȃ�
		 */
		bool IsAnimation() const {
			return NowAnimation_ != nullptr;
		}

		/**
		 * @brief		�Ǝ��̂i�r�n�m�o�͎���
		 */
		void ToJson(nlohmann::json& os)
		{
			SrcRect_.ToJson(os["SrcRect"]);
			for (auto it : Animations_)
			{
				it.second->ToJson(os[it.first]);
			}
		}
		/**
		 * @brief		�Ǝ��̂i�r�n�m�ǂݍ��ݎ���
		 */
		void FromJson(nlohmann::json& os)
		{
			SrcRect_.FromJson(os["SrcRect"]);
			for (auto it = os.begin(); it != os.end(); ++it)
			{
				if (it.key() == "SrcRect")
				{
					continue;
				}
				auto p = std::make_shared<KeyFrameAnimation<RectangleF>>();
				p->FromJson(it.value());
				Animations_[it.key()] = p;
			}
		}

		REGISTER_COMPONENTONLY(SpriteAnimator)
	};
	//�|�C���^�u������
	using SpriteAnimatorPtr = std::shared_ptr<SpriteAnimator>;
}