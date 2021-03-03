#pragma once

#include	"Stage.h"

namespace Sample {

	/**
	 * @brief		�Q�[�����C���V�[���̒�`
	 */
	class GameScene : public Scene {
	private:
		StagePtr		Stage_;
		ActorPtr		Clear_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		GameScene();

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~GameScene();

		/**
		 * @brief		�p����̃V�[���̍\�z�������Ȃ�
		 */
		void OnCreate() override;

		/**
		 * @brief		�p����̃V�[���̍X�V�����������Ȃ�
		 */
		void OnUpdate() override;

		/**
		 * @brief		�p����̃V�[���̍X�V�㏈���������Ȃ�
		 */
		void OnLateUpdate() override;

	};
}