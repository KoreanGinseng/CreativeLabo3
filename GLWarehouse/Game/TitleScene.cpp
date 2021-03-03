#pragma once

#include	"TitleScene.h"
#include	"GameScene.h"
#include	"NetworkScene.h"

using namespace Sample;

class StartButton : public ActionBase<StartButton>, public IClickable, public IMouseOver, public ICollisionEvent {
public:
	CSScript* test;
	/**
	 * @brief		コンストラクタ
	 */
	StartButton()
	: ActionBase() 
	, test(nullptr) {
	}
	/**
	 * @brief		コンストラクタ
	 */
	StartButton(const StartButton& obj)
	: ActionBase(obj) 
	, test(nullptr) {
	}

	/**
	 * @brief		デストラクタ
	 */
	~StartButton() {
	}

	/**
	 * @brief		開始
	 */
	void Start() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
		test = new CSScript(actor);
		test->Load("Sample.TestScript");
		test->Start();
	}

	/**
	 * @brief		更新
	 */
	void Update() override {
		test->Update();
	}

	/**
	 * @brief		クリックイベント
	 */
	void OnClickEvent() override {
		SceneManager::Instance().ChangeNewScene<GameScene>(
			SceneChangeSlide ::Create(0.0f, 0.5f, SceneChangeSlide::Type::LeftOut),
			SceneChangeSlide::Create(0.5f, 1.0f, SceneChangeSlide::Type::RightIn));
	}

	/**
	 * @brief		マウスが載った
	 */
	void OnMouseIn() override {
		SceneManager::Instance().CurrentScene()->CurrentResource()->Sounds().Get("enter")->Play();
		actor->GetRender<TextureRender>()->Alpha(1.0f);
	}

	/**
	 * @brief		マウスが外れた
	 */
	void OnMouseOut() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		接触開始
	 */
	void OnCollisionEnter(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Enter");
	}
	/**
	 * @brief		接触中
	 */
	void OnCollisionStay(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Stay");
	}
	/**
	 * @brief		接触終了
	 */
	void OnCollisionExit(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Exit");
	}
	REGISTER_COMPONENTONLY(StartButton)
};

class NetworkStartButton : public ParameterBase<NetworkStartButton>, public IClickable, public IMouseOver, public ICollisionEvent {
public:
	/**
	 * @brief		コンストラクタ
	 */
	NetworkStartButton()
	: ParameterBase() {
	}
	/**
	 * @brief		コンストラクタ
	 */
	NetworkStartButton(const NetworkStartButton& obj)
	: ParameterBase(obj) {
	}

	/**
	 * @brief		デストラクタ
	 */
	~NetworkStartButton() {
	}

	/**
	 * @brief		開始
	 */
	void Start() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		クリックイベント
	 */
	void OnClickEvent() override {
		SceneManager::Instance().ChangeNewScene<NetworkScene>(
			SceneChangeSlide::Create(0.0f, 0.5f, SceneChangeSlide::Type::LeftOut),
			SceneChangeSlide::Create(0.5f, 1.0f, SceneChangeSlide::Type::RightIn));
	}

	/**
	 * @brief		マウスが載った
	 */
	void OnMouseIn() override {
		SceneManager::Instance().CurrentScene()->CurrentResource()->Sounds().Get("enter")->Play();
		actor->GetRender<TextureRender>()->Alpha(1.0f);
	}

	/**
	 * @brief		マウスが外れた
	 */
	void OnMouseOut() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		接触開始
	 */
	void OnCollisionEnter(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Enter");
	}
	/**
	 * @brief		接触中
	 */
	void OnCollisionStay(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Stay");
	}
	/**
	 * @brief		接触終了
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
	 * @brief		コンストラクタ
	 */
	EndButton()
	: ParameterBase() {
	}
	/**
	 * @brief		コンストラクタ
	 */
	EndButton(const EndButton& obj)
	: ParameterBase(obj) {
	}

	/**
	 * @brief		デストラクタ
	 */
	~EndButton() {
	}

	/**
	 * @brief		開始
	 */
	void Start() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		クリックイベント
	 */
	void OnClickEvent() override {
		glfwSetWindowShouldClose(RenderManager::Instance().Window(), GL_TRUE);
	}

	/**
	 * @brief		マウスが載った
	 */
	void OnMouseIn() override {
		actor->GetRender<TextureRender>()->Alpha(1.0f);
	}

	/**
	 * @brief		マウスが外れた
	 */
	void OnMouseOut() override {
		actor->GetRender<TextureRender>()->Alpha(0.5f);
	}

	/**
	 * @brief		接触開始
	 */
	void OnCollisionEnter(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Enter");
	}
	/**
	 * @brief		接触中
	 */
	void OnCollisionStay(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Stay");
	}
	/**
	 * @brief		接触終了
	 */
	void OnCollisionExit(ActorPtr& other) {
		//INFO_LOG(other->Name() << "Exit");
	}
	REGISTER_COMPONENTONLY(EndButton)
};

/**
 * @brief		コンストラクタ
 */
TitleScene::TitleScene() {
	Name_ = "Title";
}

/**
 * @brief		デストラクタ
 */
TitleScene::~TitleScene() {
}

/**
 * @brief		継承先のシーンの構築をおこなう
 */
void TitleScene::OnCreate() {
	//コンポーネント登録
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

	//テクスチャ追加
	Resources_->Textures().Add("title", "title.png");
	Resources_->Textures().Add("start", "start.png");
	Resources_->Textures().Add("end", "end.png");
	Resources_->Sounds().Add("enter", "se_enter.wav");

	Sample::ScriptCompile("Script", true);

	//アクター登録
	//タイトル
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Logo");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("title"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Anchor(Vector3F(0.5f, 0.0f, 0.0f));
		rd->Pivot(Vector3F(0.5f, 0.0f, 0.5f));
		Resources_->GetActorFactory().SaveProduct("Logo");
	}
	//はじめる
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
	//はじめる
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("NetworkStart");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("start"), ResourceManager::Instance().GetShader("Sprite"));
		pt->AddParameter<ColliderRect>(RectangleF(0, 0, 220, 70));
		pt->AddParameter<NetworkStartButton>();
		Resources_->GetActorFactory().SaveProduct("NetworkStart");
	}
	//おわる
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
 * @brief		継承先のシーンの更新処理をおこなう
 */
void TitleScene::OnUpdate() {
}

/**
 * @brief		継承先のシーンの更新後処理をおこなう
 */
void TitleScene::OnLateUpdate() {
}