#pragma once

#include	"Stage.h"

namespace Sample {

	/**
	 * @brief		�^�C�g���V�[���̒�`
	 */
	class TitleScene : public Scene {
	private:
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		TitleScene();

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~TitleScene();

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