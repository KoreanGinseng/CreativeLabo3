/**
 * �쐬��
 * �������܂łP��ŏЉ���f�U�C���p�^�[���̍l����S�Ďg���悤�ɂ������ߎ��H�ł͕s�v�ȕ���������
 *   ���H�ł͕K�v�ȃp�^�[�������I�����Ċ��p���Ă�����
 * 
 * ��Flyweight�̓��\�[�X���Ȃ����ߗ��p�����A
 *   �V�[�����Ƃ̉摜�f�ށA���f�ނȂǂ�Flyweight�ŊǗ�����
 *
 * 1.�V�[���̊Ǘ��N���X���쐬���� (Singleton�Ȃ�)
 *   ��SceneManager��Singleton�ō쐬
 *     SceneManager��main.cpp�ɏ����Ă����J�ڂȂǂ̏��������s����
 *
 * 2.�V�[���̐�����switch�ł͂Ȃ����� (Factory���\�b�h�Ȃ�)
 *   ��SceneManager�ɃV�[����o�^�A�o�^���ꂽ�����N���X����V�[���𐶐�
 *     SceneManager������V�[���ԍ��ł̑J�ڂ������A�V�[���ԍ��̗񋓂���V�[�����ł̑J�ڂɕύX
 *     �V�[���̕ύX�����O�ɂȂ������Ƃɂ��A�V�[���̒ǉ��ŗ񋓂̕ύX���s�v�ɁA
 *     �܂��o�^���ꂽ�V�[���֑J�ڂ���悤�ɂ��邱�Ƃ�SceneManager���S�ẴV�[����m��K�v�͂Ȃ��Ȃ���
 *  
 * 3.�e�V�[�����Ǘ��N���X�Ɉˑ����Ȃ��悤�ɂ��� (Observer�Ȃ�)
 *   ��Scene��Subject�ASceneManager��Observer�ɂ��邱�ƂŃV�[������ʒm�𑗐M���邱�ƂőJ��1
 *	   �܂�Scene��Subject�ɂȂ������Ƃɂ��A��ʑJ�ڂ��󂯎�肽���I�u�W�F�N�g��Observer�ɂ���ΑJ�ڎ��ɒʒm���󂯎���
 */

#include	"SceneManager.h"
#include	"TitleScene.h"
#include	"GameScene.h"
#include	"RankingScene.h"

int main()
{
	//�V�[���̓o�^
	SceneManagerInstance
		.Register<SceneSample::TitleScene>("Title")
		.Register<SceneSample::GameScene>("Game")
		.Register<SceneSample::RankingScene>("Ranking");

	//�V�[��������
	SceneManagerInstance.ChangeScene("Title");

	//�Q�[�����[�v
	while (true)
	{
		//�V�[���̍X�V
		SceneManagerInstance.Update();

		//�V�[���̕`��
		SceneManagerInstance.Render();
	}

	//�V�[���̔j��
	SceneSample::SceneManager::Release();
	return 0;
}