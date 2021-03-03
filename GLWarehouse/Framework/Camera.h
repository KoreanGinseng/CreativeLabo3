#pragma once

#include	"Common.h"
#include	"Actor.h"

namespace Sample {

	class Camera : public Actor
	{
	private:
		glm::mat4			mvp_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		Camera() 
		: Actor("Camera")
		, mvp_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		Camera(const char* nm)
		: Actor(nm)
		, mvp_() {
		}
		/**
		 * @brief		コピーコンストラクタ
		 * @param[in]	obj	コピーするアクター
		 */
		Camera(const Camera& obj)
		: Actor(obj)
		, mvp_(obj.mvp_) {
		}

		/**
		 * @brief		デストラクタ
		 */
		virtual ~Camera() {
		}

		/**
		 * @brief		2D用カメラの生成
		 * @param[in]	w	幅
		 * @param[in]	h	高さ
		 */
		void Create2D(int w, int h) {
			const glm::vec2 hs(w * 0.5f, h * 0.5f);
			const glm::mat4x4 matProj = glm::ortho( -hs.x, hs.x, -hs.y, hs.y, 1.0f, 1000.0f);
			const glm::mat4x4 matView = glm::lookAt(glm::vec3(hs.x + pos_.x, -hs.y - pos_.x, 100), glm::vec3(hs.x + pos_.x, -hs.y - pos_.x, 0), glm::vec3(0, 1, 0));
			mvp_ = matProj * matView;
		}

		/**
		 * @brief		mvpへのAccessor
		 * @return		ビュープロジェクション
		 */
		const glm::mat4& ViewProjection() const { return mvp_; }

		/**
		 * @brief		JSON生成
		 * @param[in]	os	JSONファイル
		 */
		void ToJson(nlohmann::json& os) override {
			ToJsonImpl(os["Camera"]);
		}

		/**
		 * @brief		JSONから生成
		 * @param[in]	os	JSONファイル
		 */
		virtual void FromJson(nlohmann::json& os) override {
			FromJsonImpl(os["Camera"]);
		}
	};
	//ポインタ置き換え
	using CameraPtr = std::shared_ptr<Camera>;

}