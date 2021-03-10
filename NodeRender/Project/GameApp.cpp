/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include    "MofImGui.h"

#include	"Actor.h"
#include	"Node/ActionNode.h"
#include	"Node/DecoratorNode.h"
#include	"Node/SelectorNode.h"
#include	"Node/SequencerNode.h"

#include    "ExecView.h"

using namespace Behaviour;

BehaviourActorPtr test = std::make_shared<BehaviourActor>();
BehaviourActorPtr test2 = std::make_shared<BehaviourActor>();
BehaviourActorList enemy;
CompositeNodePtr< BehaviourActorPtr > rootnode = std::make_shared< SelectorNode< BehaviourActorPtr > >();

CTexture g_Target;

namespace Behaviour {

	/**
	 * @brief		一番近い敵ターゲットノード
	 */
	class NearTargetNode : public ActionNodeBase< BehaviourActorPtr >
	{
	protected:
	public:
		NearTargetNode()
			: ActionNodeBase< BehaviourActorPtr >() {
		}
		virtual ~NearTargetNode() = default;


		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		true		実行の成功
		 *				false		実行の失敗
		 */
		virtual bool Exec(BehaviourActorPtr& actor) {
			float min = 0;
			for (auto& en : enemy)
			{
				float d = CVector3Utilities::Distance(actor->GetPosition(), en->GetPosition());
				if (min == 0 || d < min)
				{
					min = d;
					actor->SetTarget(en);
				}
			}
			return true;
		}
	};

	/**
	 * @brief		判定ノード
	 */
	class TargetDistanceNode : public ConditionalNodeBase< BehaviourActorPtr >
	{
	protected:
	public:
		TargetDistanceNode()
			: ConditionalNodeBase< BehaviourActorPtr >() {
		}
		virtual ~TargetDistanceNode() = default;


		/**
		 * @brief		ノードの実行処理
		 * @param[in]	actor		実行アクター
		 * @return		true		実行の成功
		 *				false		実行の失敗
		 */
		virtual bool Exec(BehaviourActorPtr& actor) {
			return actor->GetTargetDistance() <= 20;
		}
	};
}

ExecView exec;
ExecView exec2;

bool g_bRunning    = false;
bool g_bViewRender = false;
bool g_bDemoWindw  = false;

int                      g_HieCurrent = 0;
std::vector<std::string> g_HieName;

void RenderNode() {
	exec.Render();
	exec2.Render();
}

void GameInit() {
	//適当に配置
	test->SetPosition(CVector3(0, 100, 0));
	test2->SetPosition(CVector3(0, 500, 0));
	enemy.clear();
	//ランダムにアクターを１０作成
	for (int i = 0; i < 10; i++) {
		auto add = std::make_shared<BehaviourActor>();
		add->SetPosition(CVector3(200 + CUtilities::RandomFloat() * 824, CUtilities::RandomFloat() * 768, 0));
		enemy.push_back(add);
	}
}

void NodeCreate() {
	//判定ノードを使っての実装
	auto dcnode = std::make_shared < DecoratorNode<BehaviourActorPtr, float> >(ConditionalNodeBase< BehaviourActorPtr >::Operator::Less,
		std::make_shared< GetNode<BehaviourActorPtr, float>>(&BehaviourActor::GetX),
		std::make_shared < ValueNode<BehaviourActorPtr, float> >(200));
	rootnode->AddChild(dcnode);

	//オブジェクトの関数実行ノードを使っての実装
	auto atnode = std::make_shared<FunctionNode<BehaviourActorPtr> >(&BehaviourActor::MoveX);
	dcnode->SetChild(atnode);

	//シーケンス実行
	auto sqnode = std::make_shared<SequencerNode<BehaviourActorPtr>>();
	rootnode->AddChild(sqnode);

	//専用の動作ノードを作っての実装
	auto ntnode = std::make_shared<NearTargetNode>();
	sqnode->AddChild(ntnode);

	auto atnode2 = std::make_shared<FunctionNode<BehaviourActorPtr> >(&BehaviourActor::MoveTarget);
	sqnode->AddChild(atnode2);

	//専用の判定ノードを作っての実装
	auto connode = std::make_shared<TargetDistanceNode >();
	sqnode->AddChild(connode);
}

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
	//IMGUIのセットアップ
	CMofImGui::Setup();

	g_HieName.push_back("root");

	GameInit();

	NodeCreate();

	exec.Init(rootnode, Vector2(0, 0));
	exec2.Init(rootnode, Vector2(0, 300));

	g_Target.CreateTarget(1024, 768, PIXELFORMAT_R8G8B8A8_UNORM, BUFFERACCESS_GPUREADWRITE);

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void) {

	//キーの更新
	g_pInput->RefreshKey();
	//IMGUIの更新
	CMofImGui::Refresh();

	ImGui::Begin("Hello");
	{
		if (ImGui::Button("init")) {
			exec.Init(rootnode);
			exec2.Init(rootnode);
			GameInit();
		}
		ImGui::Checkbox("Running", &g_bRunning);
		ImGui::Checkbox("View", &g_bViewRender);
		ImGui::Checkbox("DemoWindow", &g_bDemoWindw);
	}
	ImGui::End();

	ImGui::Begin("Hierarchy");
	{
		const char* item[10];
		int namecnt = g_HieName.size();
		for (int i= 0; i < namecnt; i++)
		{
			item[i] = g_HieName[i].c_str();
		}
		ImGui::ListBox("roots", &g_HieCurrent, item, namecnt);
	}
	ImGui::End();


	ImGui::Begin("Inspecter");
	{

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::TreeNode("Create"))
			{
				if (ImGui::Selectable("ActionNode"));
				if (ImGui::Selectable("ConditionalNode"));
				if (ImGui::Selectable("DecoratorNode"));
				if (ImGui::Selectable("SelecterNode"));
				if (ImGui::Selectable("SequencerNode"));
				ImGui::TreePop();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();

	if(g_bDemoWindw)
		ImGui::ShowDemoWindow();

	//ビヘイビアの実行
	if (g_bRunning)
	{
		exec.Exec(test);
		exec2.Exec(test2);
	}

	if (g_bViewRender)
	{
		exec.Update();
		exec2.Update();
	}

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	LPRenderTarget pTargetOld = g_pGraphics->GetRenderTarget();
	LPDepthTarget  pDepthTarget = g_pGraphics->GetDepthTarget();

	//アクターのテスト描画
	test->Render(MOF_COLOR_GREEN);
	test2->Render(MOF_COLOR_GREEN);
	for (auto& en : enemy)
	{
		en->Render(MOF_COLOR_RED);
	}
	g_pGraphics->SetRenderTarget(g_Target.GetRenderTarget(), pDepthTarget);
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
	RenderNode();
	g_pGraphics->SetRenderTarget(pTargetOld, pDepthTarget);

	if(g_bViewRender)
		g_Target.Render(0, 0);

	ImGui::Begin("view");
	ImGui::Image((void*)g_Target.GetTexture(), ImVec2(512, 384));
	ImGui::End();
	
	//IMGUIの描画準備
	CMofImGui::RenderSetup();

	//IMGUIの描画
	CMofImGui::RenderGui();

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {

	//IMGUIの解放
	CMofImGui::Cleanup();
	g_Target.Release();

	return TRUE;
}