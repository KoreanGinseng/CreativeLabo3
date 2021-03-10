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

using namespace Behaviour;

BehaviourActorPtr test = std::make_shared<BehaviourActor>();
BehaviourActorPtr test2 = std::make_shared<BehaviourActor>();
BehaviourActorList enemy;
CompositeNodePtr< BehaviourActorPtr > rootnode = std::make_shared< SelectorNode< BehaviourActorPtr > >();

CTexture g_Target;
CTexture g_TargetTexture;


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

class ExecView {
private:
	using Lines = std::pair<int, std::vector<int>>;
	struct view_data {
		CRectangle				            _rect;
		std::string							_name;
		Lines			                    _lines;
		NodeExecutorPtr<BehaviourActorPtr>  _exec;
		view_data() {}
	};
	std::vector<view_data> _view;

	NodeExecutorPtr< BehaviourActorPtr > _exec;

public:

	void Init(const NodePtr< BehaviourActorPtr >& root, Vector2 pos = Vector2()) {
		//適当に動作ノードを生成する
		if (_exec == nullptr) {
			_exec = root->CreateExecutor();
			SetViewData(_exec, pos);
		}
		else {
			_exec->Reset();
		}
	}

	void Exec(BehaviourActorPtr& actor) {
		_exec->Exec(actor);
	}

	void Update() {
		Vector2 mp;
		Vector2 mm;
		g_pInput->GetMousePos(mp);
		g_pInput->GetMouseMove(mm.x, mm.y);
		int rect_cnt = _view.size();
		for (int i = rect_cnt - 1; i >= 0; i--) {
			if (_view[i]._rect.CollisionPoint(mp) && g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
				_view[i]._rect.Translation(mm);
				break;
			}
		}
	}

	void Render() {
		for (const auto& itr : _view) {
			for (const auto& itr2 : itr._lines.second) {
				Vector2 start = _view[itr._lines.first]._rect.GetCenter();
				Vector2 end = _view[itr2]._rect.GetCenter();
				CGraphicsUtilities::RenderLine(start, end, MOF_COLOR_WHITE);
			}
		}
		int rect_cnt = _view.size();
		for (int i = 0; i < rect_cnt; i++) {
			MofU32 color = MOF_COLOR_CWHITE;
			if (_view[i]._exec->GetState() == INodeExecutor<BehaviourActorPtr>::State::Running) {
				color = MOF_COLOR_YELLOW;
			}
			else if (_view[i]._exec->GetState() == INodeExecutor<BehaviourActorPtr>::State::Completed) {
				color = MOF_COLOR_GREEN;
			}
			else if (_view[i]._exec->GetState() == INodeExecutor<BehaviourActorPtr>::State::Incompleted) {
				color = MOF_COLOR_RED;
			}
			CGraphicsUtilities::RenderFillRect(_view[i]._rect, color);
			CGraphicsUtilities::RenderString(_view[i]._rect, _view[i]._name.c_str());
		}
	}

	void SetViewData(const NodeExecutorPtr<BehaviourActorPtr>& exec, Vector2 pos) {
		int i = _view.size();
		_view.push_back(view_data());
		_view[i]._rect = (CRectangle(pos.x, pos.y, pos.x + 100, pos.y + 50));
		_view[i]._name = (exec->GetNode()->GetName());
		_view[i]._lines.first = i;
		_view[i]._exec = exec;
		auto& child = exec->GetChild();
		for (const auto& itr : child) {
			int last = _view.size();
			SetViewData(itr, pos);
			_view[i]._lines.second.push_back(last);
		}
	}
};

ExecView exec;
ExecView exec2;

bool g_bRunning    = false;
bool g_bViewRender = false;

void RenderNode() {
	exec.Render();
	exec2.Render();
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

	//適当に配置
	test->SetPosition(CVector3(0, 100, 0));
	test2->SetPosition(CVector3(0, 500, 0));
	//ランダムにアクターを１０作成
	for (int i = 0; i < 10; i++)
	{
		auto add = std::make_shared<BehaviourActor>();
		add->SetPosition(CVector3(200 + CUtilities::RandomFloat() * 824, CUtilities::RandomFloat() * 768, 0));
		enemy.push_back(add);
	}

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

	exec.Init(rootnode, Vector2(0, 0));
	exec2.Init(rootnode, Vector2(0, 300));

	g_Target.CreateTarget(1024, 768, PIXELFORMAT_R8G8B8A8_UNORM, BUFFERACCESS_GPUREADWRITE);

	g_TargetTexture.Load("test.png");

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
			test->SetPosition(CVector3(0, 100, 0));
			test2->SetPosition(CVector3(0, 500, 0));
		}
		ImGui::Checkbox("Running", &g_bRunning);
		ImGui::Checkbox("View", &g_bViewRender);
	}
	ImGui::End();

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
	g_TargetTexture.Release();

	return TRUE;
}