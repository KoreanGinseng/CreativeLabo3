/**
 * ���̃v���O�����͉�ʑJ�ڏ������쐬�����T���v���ł��B
 * ���̃T���v�����ߋ��Q�T�Ŋw�񂾃f�U�C���p�^�[���̒m�����g���ď��������Ă݂܂��傤
 *
 * ����Ă݂Ăق������� (���p�\�ȃf�U�C���p�^�[��)
 * 1.�V�[���̊Ǘ��N���X���쐬���� (Singleton�Ȃ�)
 * 2.�V�[���̐�����switch�ł͂Ȃ����� (Factory���\�b�h�Ȃ�)
 * 3.�e�V�[�����Ǘ��N���X�Ɉˑ����Ȃ��悤�ɂ��� (Observer�Ȃ�)
 */

#include	"TitleScene.h"
#include	"GameScene.h"
#include	"RankingScene.h"

//���s���̃V�[��
SceneNo		gNowScene = SceneNo::Title;
//�ύX����V�[��
SceneNo		gChangeScene = SceneNo::Title;

int main()
{
	//�V�[���̐���
	SceneSample::ScenePointer scene = std::make_shared<SceneSample::TitleScene>();

	//�V�[���̏�����
	scene->Initialize(std::make_shared<SceneSample::TitleScene::InitializeParam>());

	//�Q�[�����[�v
	while (true)
	{
		//�V�[���̍X�V
		scene->Update();

		//�V�[���̕`��
		scene->Render();

		//�V�[���̕ύX
		if (gNowScene != gChangeScene)
		{
			//�O�̃V�[����j��
			scene->Destroy();
			//���̃V�[���̔ԍ��ɉ����Đ���
			switch (gChangeScene)
			{
			//�^�C�g����ʂւ̑J��
			case SceneNo::Title:
				//�V�[���̐���
				scene = std::make_shared<SceneSample::TitleScene>();
				//�V�[���̏�����
				scene->Initialize(std::make_shared<SceneSample::TitleScene::InitializeParam>());
				break;

			//�Q�[����ʂւ̑J��
			case SceneNo::Game:
				//�V�[���̐���
				scene = std::make_shared<SceneSample::GameScene>();
				//�V�[���̏�����
				scene->Initialize(std::make_shared<SceneSample::GameScene::InitializeParam>());
				break;

			//�����L���O��ʂւ̑J��
			case SceneNo::Ranking:
				//�V�[���̐���
				scene = std::make_shared<SceneSample::RankingScene>();
				//�V�[���̏�����
				scene->Initialize(std::make_shared<SceneSample::RankingScene::InitializeParam>());
				break;
			}
			gNowScene = gChangeScene;
		}
	}

	//�V�[���̔j��
	scene->Destroy();
	return 0;
}