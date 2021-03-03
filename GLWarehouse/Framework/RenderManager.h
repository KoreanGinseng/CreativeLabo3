#pragma once

#include	"Common.h"
#include	"Camera.h"

namespace Sample {

	class RenderManager
	{
	private:
		/** ウインドウ */
		GLFWwindow*									window_;
		/** メインカメラ */
		CameraPtr									CurrentCamera_;
		/**	画面幅 */
		int											Width_;
		/**	画面高さ */
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
		 * @brief		初期化
		 */
		void Initialize(GLFWwindow* w) {
			window_ = w;
			glfwGetWindowSize(w, &Width_, &Height_);
		}

		/**
		 * @brief		利用中のカメラの設定
		 * @param[in]	cam		利用中のカメラ
		 */
		void Camera(CameraPtr& cam) {
			CurrentCamera_ = cam;
		}

		/**
		 * @brief		利用中のカメラの取得
		 * @return		利用中のカメラ
		 */
		const CameraPtr& Camera() const {
			return CurrentCamera_;
		}

		/**
		 * @brief		画面
		 */
		GLFWwindow* Window() const noexcept { return window_; }
		/**
		 * @brief		画面幅の取得
		 */
		const GLint ScreenWidth() const noexcept { return Width_; }
		/**
		 * @brief		画面高さの取得
		 */
		const GLint ScreenHeight() const noexcept { return Height_; }


		static RenderManager& Instance() {
			static RenderManager t;
			return t;
		}
	};

}