#pragma once

#include	"Scene.h"

namespace SceneSample {
	/**
	 * @brief		�����L���O�V�[��
	 */
	class RankingScene : public IScene {
	private:
	public:
		/**
		 * @brief		�����L���O�V�[���������p�\����
		 *				�����L���O�V�[���̏������ɕK�v�ȏ�񂪂���ΐݒ肷��
		 */
		struct InitializeParam : public IScene::InitializeParam {
		};

		/**
		 * @brief		�R���X�g���N�^
		 */
		RankingScene() {
		}

		/**
		 * @brief		�R���X�g���N�^
		 */
		virtual ~RankingScene() {
		}

		/**
		 * @brief		������
		 * @param[in]	param		���������
		 * @return		true		����������I��
		 *				false		���������s
		 */
		bool Initialize(const InitializeParamPointer& param) override {
			//�����L���O�̏��������Ƃ��Ď��o��
			auto inparam = std::dynamic_pointer_cast<InitializeParam>(param);
			//�e�X�g�p�\��
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "�����L���O������..." << std::endl;
			return true;
		}

		/**
		 * @brief		�X�V
		 */
		void Update() override {
			//�e�X�g�p�\��
			std::cout << "�����L���O�X�V..." << std::endl;
			std::cout << "1...�^�C�g�����" << std::endl;
			//���̓L�[�̎擾
			int key = _getch();
			if (key == '1')
			{
				gChangeScene = SceneNo::Title;
			}
		}

		/**
		 * @brief		�`��
		 */
		void Render() override {
			//�e�X�g�p�\��
			std::cout << "�����L���O�`��..." << std::endl;
		}

		/**
		 * @brief		�j��
		 */
		void Destroy() override {
			//�e�X�g�p�\��
			std::cout << "�����L���O�j��..." << std::endl;
			std::cout << "----------------------------------------\n" << std::endl;
		}
	};
}