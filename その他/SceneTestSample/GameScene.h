#pragma once

#include	"Scene.h"

namespace SceneSample {
	/**
	 * @brief		�Q�[���V�[��
	 */
	class GameScene : public IScene {
	private:
	public:
		/**
		 * @brief		�Q�[���V�[���������p�\����
		 *				�Q�[���V�[���̏������ɕK�v�ȏ�񂪂���ΐݒ肷��
		 */
		struct InitializeParam : public SceneInitializeParam {
		};

		/**
		 * @brief		�R���X�g���N�^
		 */
		GameScene() {
		}

		/**
		 * @brief		�R���X�g���N�^
		 */
		virtual ~GameScene() {
		}

		/**
		 * @brief		������
		 * @param[in]	param		���������
		 * @return		true		����������I��
		 *				false		���������s
		 */
		bool Initialize(const SceneInitializeParamPointer& param) override {
			//�Q�[���̏��������Ƃ��Ď��o��
			auto inparam = std::dynamic_pointer_cast<InitializeParam>(param);
			//�e�X�g�p�\��
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "�Q�[��������..." << std::endl;
			return true;
		}

		/**
		 * @brief		�X�V
		 */
		void Update() override {
			//�e�X�g�p�\��
			std::cout << "�Q�[���X�V..." << std::endl;
			std::cout << "1...�^�C�g�����" << std::endl;
			//���̓L�[�̎擾
			int key = _getch();
			if (key == '1')
			{
				Notify("Title", nullptr);
			}
		}

		/**
		 * @brief		�`��
		 */
		void Render() override {
			//�e�X�g�p�\��
			std::cout << "�Q�[���`��..." << std::endl;
		}

		/**
		 * @brief		�j��
		 */
		void Destroy() override {
			//�e�X�g�p�\��
			std::cout << "�Q�[���j��..." << std::endl;
			std::cout << "----------------------------------------\n" << std::endl;
		}
	};
}