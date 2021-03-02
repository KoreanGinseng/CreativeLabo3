#pragma once

#include	"Scene.h"

namespace SceneSample {
	/**
	 * @brief		�^�C�g���V�[��
	 */
	class TitleScene : public IScene {
	private:
	public:
		/**
		 * @brief		�^�C�g���V�[���������p�\����
		 *				�^�C�g���V�[���̏������ɕK�v�ȏ�񂪂���ΐݒ肷��
		 */
		struct InitializeParam : public SceneInitializeParam {
		};

		/**
		 * @brief		�R���X�g���N�^
		 */
		TitleScene() {
		}

		/**
		 * @brief		�R���X�g���N�^
		 */
		virtual ~TitleScene() {
		}

		/**
		 * @brief		������
		 * @param[in]	param		���������
		 * @return		true		����������I��
		 *				false		���������s
		 */
		bool Initialize(const SceneInitializeParamPointer& param) override {
			//�^�C�g���̏��������Ƃ��Ď��o��
			auto inparam = std::dynamic_pointer_cast<InitializeParam>(param);
			//�e�X�g�p�\��
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "�^�C�g��������..." << std::endl;
			return true;
		}

		/**
		 * @brief		�X�V
		 */
		void Update() override {
			//�e�X�g�p�\��
			std::cout << "�^�C�g���X�V..." << std::endl;
			std::cout << "1...�Q�[�����" << std::endl;
			std::cout << "2...�����L���O���" << std::endl;
			//���̓L�[�̎擾
			int key = _getch();
			if (key == '1')
			{
				Notify("Game", nullptr);
			}
			else if (key == '2')
			{
				Notify("Ranking", nullptr);
			}
		}

		/**
		 * @brief		�`��
		 */
		void Render() override {
			//�e�X�g�p�\��
			std::cout << "�^�C�g���`��..." << std::endl;
		}

		/**
		 * @brief		�j��
		 */
		void Destroy() override {
			//�e�X�g�p�\��
			std::cout << "�^�C�g���j��..." << std::endl;
			std::cout << "----------------------------------------\n" << std::endl;
		}
	};
}