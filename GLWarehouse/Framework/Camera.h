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
		 * @brief		�R���X�g���N�^
		 */
		Camera() 
		: Actor("Camera")
		, mvp_() {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		Camera(const char* nm)
		: Actor(nm)
		, mvp_() {
		}
		/**
		 * @brief		�R�s�[�R���X�g���N�^
		 * @param[in]	obj	�R�s�[����A�N�^�[
		 */
		Camera(const Camera& obj)
		: Actor(obj)
		, mvp_(obj.mvp_) {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Camera() {
		}

		/**
		 * @brief		2D�p�J�����̐���
		 * @param[in]	w	��
		 * @param[in]	h	����
		 */
		void Create2D(int w, int h) {
			const glm::vec2 hs(w * 0.5f, h * 0.5f);
			const glm::mat4x4 matProj = glm::ortho( -hs.x, hs.x, -hs.y, hs.y, 1.0f, 1000.0f);
			const glm::mat4x4 matView = glm::lookAt(glm::vec3(hs.x + pos_.x, -hs.y - pos_.x, 100), glm::vec3(hs.x + pos_.x, -hs.y - pos_.x, 0), glm::vec3(0, 1, 0));
			mvp_ = matProj * matView;
		}

		/**
		 * @brief		mvp�ւ�Accessor
		 * @return		�r���[�v���W�F�N�V����
		 */
		const glm::mat4& ViewProjection() const { return mvp_; }

		/**
		 * @brief		JSON����
		 * @param[in]	os	JSON�t�@�C��
		 */
		void ToJson(nlohmann::json& os) override {
			ToJsonImpl(os["Camera"]);
		}

		/**
		 * @brief		JSON���琶��
		 * @param[in]	os	JSON�t�@�C��
		 */
		virtual void FromJson(nlohmann::json& os) override {
			FromJsonImpl(os["Camera"]);
		}
	};
	//�|�C���^�u������
	using CameraPtr = std::shared_ptr<Camera>;

}