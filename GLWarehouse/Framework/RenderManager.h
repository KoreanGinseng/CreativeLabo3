#pragma once

#include	"Common.h"
#include	"Camera.h"

namespace Sample {

	class RenderManager
	{
	private:
		/** �E�C���h�E */
		GLFWwindow*									window_;
		/** ���C���J���� */
		CameraPtr									CurrentCamera_;
		/**	��ʕ� */
		int											Width_;
		/**	��ʍ��� */
		int											Height_;
		RenderManager()
		: window_(nullptr)
		, CurrentCamera_()
		, Width_(0)
		, Height_(0) {
		}
		RenderManager(const RenderManager& obj) = delete;
		RenderManager& operator=(const RenderManager&) = delete;
		virtual ~RenderManager() = default;
	public:
		/**
		 * @brief		������
		 */
		void Initialize(GLFWwindow* w) {
			window_ = w;
			glfwGetWindowSize(w, &Width_, &Height_);
		}

		/**
		 * @brief		���p���̃J�����̐ݒ�
		 * @param[in]	cam		���p���̃J����
		 */
		void Camera(CameraPtr& cam) {
			CurrentCamera_ = cam;
		}

		/**
		 * @brief		���p���̃J�����̎擾
		 * @return		���p���̃J����
		 */
		const CameraPtr& Camera() const {
			return CurrentCamera_;
		}

		/**
		 * @brief		���
		 */
		GLFWwindow* Window() const noexcept { return window_; }
		/**
		 * @brief		��ʕ��̎擾
		 */
		const GLint ScreenWidth() const noexcept { return Width_; }
		/**
		 * @brief		��ʍ����̎擾
		 */
		const GLint ScreenHeight() const noexcept { return Height_; }


		static RenderManager& Instance() {
			static RenderManager t;
			return t;
		}
	};

}