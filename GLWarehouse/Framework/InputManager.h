#pragma once

#include	"Common.h"
#include	"../Library/include/GLFW/glfw3.h"

namespace Sample {

	class InputManager
	{
	private:
		struct KeyData {
			struct KeyNo {
				int				No;
				bool			bMouse;
			};
			std::vector<KeyNo>	Key;
			bool				bPrev;
			bool				bNow;

			KeyData()
			: Key()
			, bPrev(false)
			, bNow(false){
			}
		};
		using KeyMap = std::unordered_map<std::string, KeyData >;
		std::unique_ptr< KeyMap >					KeyMap_;
		double										MouseX_;
		double										MouseY_;
		GLFWwindow*									window_;

		InputManager()
		: KeyMap_() 
		, MouseX_(0)
		, MouseY_(0)
		, window_(nullptr) {
		}
		InputManager(const InputManager& obj) = delete;
		InputManager& operator=(const InputManager&) = delete;
		virtual ~InputManager() = default;
	public:

		/**
		 * @brief		�L�[�}�b�v�̏�����
		 * @param[in]	w		���̓E�C���h�E
		 */
		void Initialize(GLFWwindow* w) {
			KeyMap_ = std::make_unique< KeyMap >();
			window_ = w;
		}

		/**
		 * @brief		�o�^�L�[�̒ǉ�
		 * @param[in]	kn		�o�^�L�[��
		 * @param[in]	Key		�o�^�L�[
		 */
		void AddKey(const char* kn, int Key) {
			auto km = KeyMap_->find(kn);
			if (km != KeyMap_->end())
			{
				km->second.Key.push_back({ Key, false });
			}
			else
			{
				(*KeyMap_)[kn].Key.push_back({ Key, false });
			}
		}

		/**
		 * @brief		�o�^�L�[�̒ǉ�
		 * @param[in]	kn		�o�^�L�[��
		 * @param[in]	Key		�o�^�L�[
		 */
		void AddMouseKey(const char* kn, int Key) {
			auto km = KeyMap_->find(kn);
			if (km != KeyMap_->end())
			{
				km->second.Key.push_back({ Key, true });
			}
			else
			{
				(*KeyMap_)[kn].Key.push_back({ Key, true });
			}
		}

		/**
		 * @brief		�t���[�����Ԃ̍X�V
		 *				Framework::Run�ɂČĂяo��
		 */
		void Update() {
			for (auto k = KeyMap_->begin(); k != KeyMap_->end(); ++k)
			{
				k->second.bPrev = k->second.bNow;
				k->second.bNow = 0;
				for (auto key : k->second.Key)
				{
					if (key.bMouse)
					{
						k->second.bNow |= (glfwGetMouseButton(window_, key.No) == GLFW_PRESS);
					}
					else
					{
						k->second.bNow |= (glfwGetKey(window_, key.No) == GLFW_PRESS);
					}
				}
			}
			//�}�E�X�ʒu�̎擾
			glfwGetCursorPos(window_, &MouseX_, &MouseY_);
		}

		/**
		 * @brief		�L�[��Ԃ̎擾
		 * @param[in]	keycode	�L�[�R�[�h
		 */
		int GetKeyState(const int keycode) {
			return glfwGetKey(window_, keycode);
		}

		/**
		 * @brief		�w�薼�̂̓o�^�L�[�����̃t���[���ɉ����ꂽ���ǂ���
		 * @param[in]	kn		�o�^�L�[��
		 * @return		true	���̃t���[���ŉ����ꂽ
		 *				false	���̃t���[���ł͉�����Ă��Ȃ�
		 */
		bool IsPush(const char* kn) const {
			const KeyData& kd = (*KeyMap_)[kn];
			return kd.bNow && !kd.bPrev;
		}

		/**
		 * @brief		�w�薼�̂̓o�^�L�[�����̃t���[���ɗ����ꂽ���ǂ���
		 * @param[in]	kn		�o�^�L�[��
		 * @return		true	���̃t���[���ŗ����ꂽ
		 *				false	���̃t���[���ł͗�����Ă��Ȃ�
		 */
		bool IsPull(const char* kn) const {
			const KeyData& kd = (*KeyMap_)[kn];
			return !kd.bNow && kd.bPrev;
		}

		/**
		 * @brief		�w�薼�̂̓o�^�L�[��������Ă��邩�ǂ���
		 * @param[in]	kn		�o�^�L�[��
		 * @return		true	���̃t���[���ŉ�����Ă���
		 *				false	���̃t���[���ŉ�����Ă��Ȃ�
		 */
		bool IsPress(const char* kn) const {
			const KeyData& kd = (*KeyMap_)[kn];
			return kd.bNow;
		}

		/**
		 * @brief		�}�E�X���ʒu
		 * @return		�}�E�X���ʒu
		 */
		constexpr double GetMouseX() const {
			return MouseX_;
		}

		/**
		 * @brief		�}�E�X�c�ʒu
		 * @return		�}�E�X�c�ʒu
		 */
		constexpr double GetMouseY() const {
			return MouseY_;
		}

		/**
		 * @brief		�������
		 */
		void Release() {
			KeyMap_.reset();
			window_ = nullptr;
		}

		static InputManager& Instance() {
			static InputManager t;
			return t;
		}
	};

}