#pragma once

#include	"World.h"
#include	"FrameBuffer.h"
#include	"Sprite.h"
#include	"TextureRender.h"
#include	"RenderManager.h"
#include	"ResourceManager.h"

namespace Sample {

	class Scene : public World
	{
	protected:
		/** �V�[���̖��O */
		std::string									Name_;

		/** �V�[�����̃��C���J���� */
		CameraPtr									CurrentCamera_;
		
		/** �V�[���̃����_�����O���� */
		FrameBufferPtr								FrameBuffer_;

		/** �t���[���`��p�X�v���C�g */
		SpritePtr									FrameSprite_;

		/** �V�[�����̗��p���\�[�X */
		ResourcePackPtr								Resources_;

		/** �X�V�t���O */
		bool										bUpdate_;

		/** �I���t���O */
		bool										bEnd_;

		Scene(const Scene& obj) = delete;
		Scene& operator=(const Scene&) = delete;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		Scene()
		: World() 
		, Name_()
		, CurrentCamera_() 
		, FrameBuffer_()
		, FrameSprite_()
		, Resources_()
		, bUpdate_(true) 
		, bEnd_(false) {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Scene() {
			Release();
		}

		/**
		 * @brief		�V�[���̍\�z�������Ȃ�
		 */
		virtual void Create() {
			//�ڐG������
			Collisions_.Initialize({0xFFFFFFFF});

			//���\�[�X�p�b�N����
			Resources_ = ResourceManager::Instance().AddPack(Name_);

			//�t���[���o�b�t�@����
			FrameBuffer_ = std::make_shared<FrameBuffer>();
			FrameBuffer_->Create(RenderManager::Instance().ScreenWidth(),
				RenderManager::Instance().ScreenHeight());
			FrameSprite_ = std::make_shared<Sprite>();
			FrameSprite_->YFlip(true);
			FrameSprite_->Create(FrameBuffer_->Color(), ResourceManager::Instance().GetShader("Sprite"));
			FrameSprite_->RefreshMatrix();

			//�A�N�^�[���X�g����
			Actors_ = std::make_shared<ActorList>();
			ActionActors_ = std::make_shared<ActorList>();

			//�J��������
			CurrentCamera_ = std::make_shared<Camera>();
			CurrentCamera_->Create2D(FrameBuffer_->Width(), FrameBuffer_->Height());
			Actors_->push_back(CurrentCamera_);
			RootActors_.push_back(CurrentCamera_);

			//�p����̃V�[���̍\�z�������Ȃ�
			OnCreate();
		}

		/**
		 * @brief		�p����̃V�[���̍\�z�������Ȃ�
		 */
		virtual void OnCreate() {
		}

		/**
		 * @brief		�V�[���̍X�V�������Ȃ�
		 */
		virtual void Update() {
			//�X�V�Ȃ�
			if (!bUpdate_)
			{
				return;
			}

			//�X�V����
			OnUpdate();

			//�A�N�^�[�J�n
			if (StartActors_.size() > 0)
			{
				std::for_each(StartActors_.begin(), StartActors_.end(), [&](const ActorPtr& ptr) { ptr->Start(); });
				StartActors_.clear();
			}

			//�A�N�^�[�X�V
			for (auto it : RootActors_)
			{
				if (!it->IsActive())
				{
					continue;
				}
				it->Update();
			}

			//�ڐG������s
			Collisions_.Collision();

			//�X�V�㏈��
			OnLateUpdate();

			//�I���A�N�^�[�̏���
			Actors_->erase(std::remove_if(Actors_->begin(), Actors_->end(),
				[](const ActorPtr& act) { return act->IsEnd(); }), Actors_->end());
			RootActors_.erase(std::remove_if(RootActors_.begin(), RootActors_.end(),
				[](const ActorPtr& act) { return act->IsEnd(); }), RootActors_.end());
			ActionActors_->erase(std::remove_if(ActionActors_->begin(), ActionActors_->end(),
				[](const ActorPtr& act) { return act->IsEnd(); }), ActionActors_->end());
		}

		/**
		 * @brief		�p����̃V�[���̍X�V�����������Ȃ�
		 */
		virtual void OnUpdate() {
		}

		/**
		 * @brief		�p����̃V�[���̍X�V�㏈���������Ȃ�
		 */
		virtual void OnLateUpdate() {
		}

		/**
		 * @brief		�V�[���̕`��������Ȃ�
		 */
		virtual void Render() {
			//�V�[�����ɃJ�������ݒ肳��Ă��Ȃ�
			if (!CurrentCamera_)
			{
				CurrentCamera_ = FindActor<Camera>();
				if (!CurrentCamera_)
				{
					ERROR_LOG("Camera Empty!!");
				}
				return;
			}
			//�V�[���̃J������ݒ�
			RenderManager::Instance().Camera(CurrentCamera_);
			//�t���[���o�b�t�@���p
			FrameBuffer_->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//�`��̎��s
			for (auto it : Renders_)
			{
				for (auto ra : it.Renders)
				{
					if (!ra->GetActor()->IsActive())
					{
						continue;
					}
					if (!ra->Enabled())
					{
						continue;
					}
					ra->Render();
				}
				it.Renders.erase(std::remove_if(it.Renders.begin(), it.Renders.end(),
					[](const RenderPtr& act) { return act->GetActor()->IsEnd(); }), it.Renders.end());
			}
			//�f�t�H���g�ɖ߂�
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glViewport(0, 0, FrameBuffer_->Width(), FrameBuffer_->Height());
		}
		/**
		 * @brief		�V�[���̕\��
		 */
		virtual void Present() {
			//�t���[���̕`��
			FrameSprite_->Render();
		}

		/**
		 * @brief		�v���_�N�g����A�N�^�[�𐶐�
		 * @param[in]	key		�L�[
		 * @param[in]	pos		�����ʒu
		 * @return		���������A�N�^�[
		 */
		ActorPtr Instaniate(const std::string& key, const Vector3F& pos = Vector3F())
		{
			ActorPtr re = Resources_->GetActorFactory().Create(key);
			if (re == nullptr)
			{
				return ActorPtr();
			}
			re->Position(pos);
			AddActor(re);
			return re;
		}

		/**
		 * @brief		���̃V�[���̃��\�[�X�̎擾
		 * @return		�V�[���̃��\�[�X
		 */
		ResourcePackPtr& CurrentResource() {
			return Resources_;
		}

		/**
		 * @brief		���p���̃J�����̐ݒ�
		 * @param[in]	cam		���p���̃J����
		 */
		void CurrentCamera(CameraPtr& cam) {
			CurrentCamera_ = cam;
		}

		/**
		 * @brief		���p���̃J�����̎擾
		 * @return		���p���̃J����
		 */
		const CameraPtr& CurrentCamera() const {
			return CurrentCamera_;
		}

		/**
		 * @brief		�V�[���X�v���C�g�̎擾
		 * @return		�V�[���X�v���C�g
		 */
		const SpritePtr& FrameSprite() const {
			return FrameSprite_;
		}

		/**
		 * @brief		�I���t���O�𗧂Ă�
		 */
		void Destroy() { bEnd_ = true; }

		/**
		 * @brief		�I������
		 * @return		�I���t���O
		 */
		bool IsEnd() const { return bEnd_; }

		/**
		 * @brief		���[���h�̎Q�Ƃ��Ȃ���
		 */
		virtual void Release() {
			World::Release();
			ResourceManager::Instance().DeletePack(Name_);
			Resources_.reset();
			CurrentCamera_.reset();
			FrameBuffer_.reset();
			FrameSprite_.reset();
		}

		/**
		 * @brief		�V�[���̕ۑ�
		 */
		bool Save();

		/**
		 * @brief		�V�[���̓ǂݍ���
		 */
		bool Load();
	};
	using ScenePtr = std::shared_ptr< Scene >;
	using SceneList = std::vector<ScenePtr>;

}