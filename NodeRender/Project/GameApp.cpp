/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
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
	 * @brief		��ԋ߂��G�^�[�Q�b�g�m�[�h
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
		 * @brief		�m�[�h�̎��s����
		 * @param[in]	actor		���s�A�N�^�[
		 * @return		true		���s�̐���
		 *				false		���s�̎��s
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
	 * @brief		����m�[�h
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
		 * @brief		�m�[�h�̎��s����
		 * @param[in]	actor		���s�A�N�^�[
		 * @return		true		���s�̐���
		 *				false		���s�̎��s
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
	//�K���ɔz�u
	test->SetPosition(CVector3(0, 100, 0));
	test2->SetPosition(CVector3(0, 500, 0));
	enemy.clear();
	//�����_���ɃA�N�^�[���P�O�쐬
	for (int i = 0; i < 10; i++) {
		auto add = std::make_shared<BehaviourActor>();
		add->SetPosition(CVector3(200 + CUtilities::RandomFloat() * 824, CUtilities::RandomFloat() * 768, 0));
		enemy.push_back(add);
	}
}

void NodeCreate() {
	//����m�[�h���g���Ă̎���
	auto dcnode = std::make_shared < DecoratorNode<BehaviourActorPtr, float> >(ConditionalNodeBase< BehaviourActorPtr >::Operator::Less,
		std::make_shared< GetNode<BehaviourActorPtr, float>>(&BehaviourActor::GetX),
		std::make_shared < ValueNode<BehaviourActorPtr, float> >(200));
	rootnode->AddChild(dcnode);

	//�I�u�W�F�N�g�̊֐����s�m�[�h���g���Ă̎���
	auto atnode = std::make_shared<FunctionNode<BehaviourActorPtr> >(&BehaviourActor::MoveX);
	dcnode->SetChild(atnode);

	//�V�[�P���X���s
	auto sqnode = std::make_shared<SequencerNode<BehaviourActorPtr>>();
	rootnode->AddChild(sqnode);

	//��p�̓���m�[�h������Ă̎���
	auto ntnode = std::make_shared<NearTargetNode>();
	sqnode->AddChild(ntnode);

	auto atnode2 = std::make_shared<FunctionNode<BehaviourActorPtr> >(&BehaviourActor::MoveTarget);
	sqnode->AddChild(atnode2);

	//��p�̔���m�[�h������Ă̎���
	auto connode = std::make_shared<TargetDistanceNode >();
	sqnode->AddChild(connode);
}

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
	//IMGUI�̃Z�b�g�A�b�v
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
		@brief			�A�v���P�[�V�����̍X�V
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void) {

	//�L�[�̍X�V
	g_pInput->RefreshKey();
	//IMGUI�̍X�V
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

	//�r�w�C�r�A�̎��s
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
		@brief			�A�v���P�[�V�����̕`��
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//�`��J�n
	g_pGraphics->RenderStart();
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	LPRenderTarget pTargetOld = g_pGraphics->GetRenderTarget();
	LPDepthTarget  pDepthTarget = g_pGraphics->GetDepthTarget();

	//�A�N�^�[�̃e�X�g�`��
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
	
	//IMGUI�̕`�揀��
	CMofImGui::RenderSetup();

	//IMGUI�̕`��
	CMofImGui::RenderGui();

	//�`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void) {

	//IMGUI�̉��
	CMofImGui::Cleanup();
	g_Target.Release();

	return TRUE;
}