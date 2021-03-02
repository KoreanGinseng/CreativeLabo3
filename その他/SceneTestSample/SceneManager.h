#pragma once

#include	<unordered_map>
#include	"Scene.h"
#include	"Singleton.h"
#include	"Observer.h"

namespace SceneSample {
	/**
	 * �V�[���}�l�[�W���[
	 */
	class SceneManager : public Singleton<SceneManager>, public Observer<const KeyType&, const SceneInitializeParamPointer&>
	{
		friend class Singleton<SceneManager>;
	public:
		/**
		 * �V�[�������p�̃C���^�[�t�F�C�X
		 */
		class ISceneCreator {
		public:
			virtual ScenePointer Create() = 0;
			virtual SceneInitializeParamPointer CreateInitializeParam() = 0;
		};
		/**
		 * �V�[�������p�̃N���X
		 */
		template < class T >
		class SceneCreator : public ISceneCreator {
		public:
			ScenePointer Create() override { return std::make_shared<T>(); }
			SceneInitializeParamPointer CreateInitializeParam() override { return std::make_shared<T::InitializeParam>(); }
		};
	private:
		/** ���s���̃V�[�� */
		ScenePointer scene;

		/** �o�^����Ă���V�[�� */
		std::unordered_map<KeyType, std::unique_ptr< ISceneCreator > > creator;

		/**
		 * @brief		�R���X�g���N�^
		 */
		SceneManager() 
		: Singleton<SceneManager> ()
		, scene()
		, creator() { }
	public:
		/**
		 * @brief		�V�[���̓o�^
		 * @param[in]	key			�o�^�L�[
		 */
		template < class T >
		SceneManager& Register(const KeyType& key) {
			creator[key] = std::make_unique<SceneCreator<T>>();
			return *this;
		}

		/**
		 * @brief		�V�[���ύX
		 * @param[in]	key			�o�^�L�[
		 * @return		true		�J�ڐ���
		 *				false		�J�ڎ��s
		 */
		bool ChangeScene(const KeyType& key) {
			return ChangeScene(key, creator[key]->CreateInitializeParam());
		}

		/**
		 * @brief		�V�[���ύX
		 * @param[in]	key			�o�^�L�[
		 * @param[in]	param		���������
		 * @return		true		�J�ڐ���
		 *				false		�J�ڎ��s
		 */
		bool ChangeScene(const KeyType& key, const SceneInitializeParamPointer& param) {
			//�V�����V�[�����쐬
			ScenePointer temp = creator[key]->Create();
			temp->Subscribe(this);
			//�V�����V�[����������
			if (!temp->Initialize(param))
			{
				return false;
			}
			//�Â��V�[����j��
			if (scene) { scene->Destroy(); }
			//�V�����V�[������
			scene = temp;
			return true;
		}

		/**
		 * @brief		�X�V
		 */
		void Update()
		{
			if (scene) { scene->Update(); }
		}

		/**
		 * @brief		�`��
		 */
		void Render()
		{
			if (scene) { scene->Render(); }
		}

		/**
		 * @brief	�ʒm���\�b�h
		 */
		void Notify(const KeyType& key, const SceneInitializeParamPointer& param) override {
			ChangeScene(key, param);
		}
	};
}
//�ȈՃA�N�Z�X�p
#define SceneManagerInstance	SceneSample::SceneManager::GetInstance()