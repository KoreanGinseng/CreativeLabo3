#pragma once

//INCLUDE
#include		<iostream>
#include		<memory>
#include		<conio.h>		//_kbhit�A_getch���g�����߂ɒǉ�

//�V�[����
enum class SceneNo {
	Title,
	Game,
	Ranking,
};
//�O���Q�ƂŎ����Ă����ύX��̃V�[��
extern SceneNo		gChangeScene;

namespace SceneSample {
	/**
	 * @brief		�V�[���p�C���^�[�t�F�C�X
	 */
	class IScene {
	public:
		/**
		 * @brief		�V�[���������p�\����
		 *				�p����̃V�[���Ōp�����Ď��̂��쐬����
		 */
		struct InitializeParam {
			virtual ~InitializeParam() {}
		};
		using InitializeParamPointer = std::shared_ptr<InitializeParam>;

		/**
		 * @brief		������
		 * @param[in]	param		���������
		 * @return		true		����������I��
		 *				false		���������s
		 */
		virtual bool Initialize(const InitializeParamPointer& param) = 0;

		/**
		 * @brief		�X�V
		 */
		virtual void Update() = 0;

		/**
		 * @brief		�`��
		 */
		virtual void Render() = 0;

		/**
		 * @brief		�j��
		 */
		virtual void Destroy() = 0;
	};
	using ScenePointer = std::shared_ptr<IScene>;
}