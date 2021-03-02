#pragma once

//INCLUDE
#include		<iostream>
#include		<memory>
#include		<conio.h>		//_kbhit�A_getch���g�����߂ɒǉ�
#include		"Subject.h"

namespace SceneSample {
	//�V�[���L�[(enum class�𗘗p����ꍇ�ύX)
	using KeyType = std::string;

	/**
	 * @brief		�V�[���������p�\����
	 *				�p����̃V�[���Ōp�����Ď��̂��쐬����
	 */
	struct SceneInitializeParam {
		virtual ~SceneInitializeParam() {}
	};
	using SceneInitializeParamPointer = std::shared_ptr<SceneInitializeParam>;

	/**
	 * @brief		�V�[���p�C���^�[�t�F�C�X
	 */
	class IScene : public Subject<const KeyType&, const SceneInitializeParamPointer&> {
	public:

		/**
		 * @brief		������
		 * @param[in]	param		���������
		 * @return		true		����������I��
		 *				false		���������s
		 */
		virtual bool Initialize(const SceneInitializeParamPointer& param) = 0;

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