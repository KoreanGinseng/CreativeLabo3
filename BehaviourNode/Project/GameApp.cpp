/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
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
NodeExecutorPtr< BehaviourActorPtr > exec;
NodeExecutorPtr< BehaviourActorPtr > exec2;

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

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){

	//�K���ɔz�u
	test->SetPosition(CVector3(0, 100, 0));
	test2->SetPosition(CVector3(0, 500, 0));
	//�����_���ɃA�N�^�[���P�O�쐬
	for (int i = 0; i < 10; i++)
	{
		auto add = std::make_shared<BehaviourActor>();
		add->SetPosition(CVector3(200 + CUtilities::RandomFloat() * 824, CUtilities::RandomFloat() * 768, 0));
		enemy.push_back(add);
	}

	//�K���ɓ���m�[�h�𐶐�����

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

	exec = rootnode->CreateExecutor();
	exec2 = rootnode->CreateExecutor();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();

	//�r�w�C�r�A�̎��s
	exec->Exec(test);
	exec2->Exec(test2);
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`��J�n
	g_pGraphics->RenderStart();
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//�A�N�^�[�̃e�X�g�`��
	test->Render(MOF_COLOR_GREEN);
	test2->Render(MOF_COLOR_GREEN);
	for (auto& en : enemy)
	{
		en->Render(MOF_COLOR_RED);
	}

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
MofBool CGameApp::Release(void){
	return TRUE;
}