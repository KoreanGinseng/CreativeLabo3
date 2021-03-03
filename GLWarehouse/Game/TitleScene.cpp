#pragma once

#include	"TitleScene.h"
#include	"GameScene.h"
#include	"NetworkScene.h"

using namespace Sample;

class StartButton : public ActionBase<StartButton>, public IClickable, public IMouseOver, public ICollisionEvent {
public:
	CSScript* test;
	/**
	 * @brief		�R���X�g���N�^
	 */
	StartButton()
	: ActionBase() 
	, test(nullptr) {
	}
	/**
	 * @brief		�R���X�g���N�^
	 */
	StartButton(const StartButton& obj)
	: ActionBase(obj) 
	, test(nullptr) {
	}

	/**
	 * @brief		�f�X�g���N�^
	 */
	~StartButton() {
	}

	/**
	 * @brief		�J�n
	 */
	void Start() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
		test = new CSScript(actor);
		test->Load("Sample.TestScript");
		test->Start();
	}

	/**
	 * @brief		�X�V
	 */
	void Update() override {
		test->Update();
	}

	/**
	 * @brief		�N���b�N�C�x���g
	 */
	void OnClickEvent() override {
		SceneManager::Instance().ChangeNewScene<GameScene>(
			SceneChangeSlide ::Create(0.0f, 0.5f, SceneChangeSlide::Type::LeftOut),
			SceneChangeSlide::Create(0.5f, 1.0f, SceneChangeSlide::Type::RightIn));
	}

	/**
	 * @brief		�}�E�X���ڂ���
	 */
	void OnMouseIn() override {
		SceneManager::Instance().CurrentScene()->CurrentResource()->Sounds().Get("enter")->Play();
		actor->GetRender<TextureRender>()->Alpha(1.0f);
	}

	/**
	 * @brief		�}�E�X���O�ꂽ
	 */
	void OnMouseOut() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		�ڐG�J�n
	 */
	void OnCollisionEnter(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Enter");
	}
	/**
	 * @brief		�ڐG��
	 */
	void OnCollisionStay(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Stay");
	}
	/**
	 * @brief		�ڐG�I��
	 */
	void OnCollisionExit(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Exit");
	}
	REGISTER_COMPONENTONLY(StartButton)
};

class NetworkStartButton : public ParameterBase<NetworkStartButton>, public IClickable, public IMouseOver, public ICollisionEvent {
public:
	/**
	 * @brief		�R���X�g���N�^
	 */
	NetworkStartButton()
	: ParameterBase() {
	}
	/**
	 * @brief		�R���X�g���N�^
	 */
	NetworkStartButton(const NetworkStartButton& obj)
	: ParameterBase(obj) {
	}

	/**
	 * @brief		�f�X�g���N�^
	 */
	~NetworkStartButton() {
	}

	/**
	 * @brief		�J�n
	 */
	void Start() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		�N���b�N�C�x���g
	 */
	void OnClickEvent() override {
		SceneManager::Instance().ChangeNewScene<NetworkScene>(
			SceneChangeSlide::Create(0.0f, 0.5f, SceneChangeSlide::Type::LeftOut),
			SceneChangeSlide::Create(0.5f, 1.0f, SceneChangeSlide::Type::RightIn));
	}

	/**
	 * @brief		�}�E�X���ڂ���
	 */
	void OnMouseIn() override {
		SceneManager::Instance().CurrentScene()->CurrentResource()->Sounds().Get("enter")->Play();
		actor->GetRender<TextureRender>()->Alpha(1.0f);
	}

	/**
	 * @brief		�}�E�X���O�ꂽ
	 */
	void OnMouseOut() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		�ڐG�J�n
	 */
	void OnCollisionEnter(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Enter");
	}
	/**
	 * @brief		�ڐG��
	 */
	void OnCollisionStay(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Stay");
	}
	/**
	 * @brief		�ڐG�I��
	 */
	void OnCollisionExit(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Exit");
	}
	REGISTER_COMPONENTONLY(NetworkStartButton)
};

class EndButton : public ParameterBase<EndButton>, public IClickable, public IMouseOver, public ICollisionEvent {
private:
public:

	/**
	 * @brief		�R���X�g���N�^
	 */
	EndButton()
	: ParameterBase() {
	}
	/**
	 * @brief		�R���X�g���N�^
	 */
	EndButton(const EndButton& obj)
	: ParameterBase(obj) {
	}

	/**
	 * @brief		�f�X�g���N�^
	 */
	~EndButton() {
	}

	/**
	 * @brief		�J�n
	 */
	void Start() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		�N���b�N�C�x���g
	 */
	void OnClickEvent() override {
		glfwSetWindowShouldClose(RenderManager::Instance().Window(), GL_TRUE);
	}

	/**
	 * @brief		�}�E�X���ڂ���
	 */
	void OnMouseIn() override {
		actor->GetRender<TextureRender>()->Alpha(1.0f);
	}

	/**
	 * @brief		�}�E�X���O�ꂽ
	 */
	void OnMouseOut() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		�ڐG�J�n
	 */
	void OnCollisionEnter(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Enter");
	}
	/**
	 * @brief		�ڐG��
	 */
	void OnCollisionStay(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Stay");
	}
	/**
	 * @brief		�ڐG�I��
	 */
	void OnCollisionExit(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Exit");
	}
	REGISTER_COMPONENTONLY(EndButton)
};

/**
 * @brief		�R���X�g���N�^
 */
TitleScene::TitleScene() {
	Name_ = "Title";
}

/**
 * @brief		�f�X�g���N�^
 */
TitleScene::~TitleScene() {
}

/**
 * @brief		�p����̃V�[���̍\�z�������Ȃ�
 */
void TitleScene::OnCreate() {
	//�R���|�[�l���g�o�^
	StartButton::RegisterComponent();
	EndButton::RegisterComponent();
	ColliderRect::RegisterComponent();
	ColliderCircle::RegisterComponent();
	TextureRender::RegisterComponent();
	SpriteAnimator::RegisterComponent();
	ParserManager::Instance().AddActorType<Actor>("Actor");
	ParserManager::Instance().AddActorType<Camera>("Camera");
	ParserManager::Instance().Register<ParameterAnimation<Actor, float>>("ActorFloatAnimation");
	ParserManager::Instance().Register<ParameterAnimation<TextureRender, float>>("TextureRenderFloatAnimation");

//	Load();

	//�e�N�X�`���ǉ�
	Resources_->Textures().Add("title", "title.png");
	Resources_->Textures().Add("start", "start.png");
	Resources_->Textures().Add("end", "end.png");
	Resources_->Sounds().Add("enter", "se_enter.wav");

	Sample::ScriptCompile("Script", true);

	//�A�N�^�[�o�^
	//�^�C�g��
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Logo");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("title"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Anchor(Vector3F(0.5f, 0.0f, 0.0f));
		rd->Pivot(Vector3F(0.5f, 0.0f, 0.5f));
		Resources_->GetActorFactory().SaveProduct("Logo");
	}
	//�͂��߂�
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Start");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("start"), ResourceManager::Instance().GetShader("Sprite"));
		pt->AddParameter<ColliderRect>(RectangleF(0, 0, 220, 70));
		pt->AddAction<StartButton>();
		Resources_->GetActorFactory().SaveProduct("Start");
		/*
		Resources_->GetActorFactory().LoadProduct("Start");
		*/
	}
	//�͂��߂�
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("NetworkStart");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("start"), ResourceManager::Instance().GetShader("Sprite"));
		pt->AddParameter<ColliderRect>(RectangleF(0, 0, 220, 70));
		pt->AddParameter<NetworkStartButton>();
		Resources_->GetActorFactory().SaveProduct("NetworkStart");
	}
	//�����
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("End");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("end"), ResourceManager::Instance().GetShader("Sprite"));
		pt->AddParameter<ColliderRect>(RectangleF(0,0,170,86));
		pt->AddParameter<EndButton>();
		auto ad = pt->AddAction<ParameterAnimation<Actor, float>>("Y");
		ad->Register(
			{ true,
					{
						{0.0f, 300},
						{5.0f, 800},
						{10.f, 300}
					}
			},
			std::make_shared< LinearInterpolation <float> >());

		auto ad2 = pt->AddAction<ParameterAnimation<TextureRender, float>>("Alpha");
		ad2->Register(
			{ true,
					{
						{0.0f, 1.0f},
						{1.0f, 0.0f},
						{2.0f, 1.0f}
					}
			},
			std::make_shared< LinearInterpolation <float> >());
		Resources_->GetActorFactory().SaveProduct("End");
	}
	Instaniate("Logo");
	Instaniate("Start", Vector3F(282,484,0));
	Instaniate("NetworkStart", Vector3F(522, 484, 0));
	Instaniate("End", Vector3F(424, 584, 0));
	Save();
}

/**
 * @brief		�p����̃V�[���̍X�V�����������Ȃ�
 */
void TitleScene::OnUpdate() {
}

/**
 * @brief		�p����̃V�[���̍X�V�㏈���������Ȃ�
 */
void TitleScene::OnLateUpdate() {
}