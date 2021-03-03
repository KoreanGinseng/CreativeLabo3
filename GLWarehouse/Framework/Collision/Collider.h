#pragma once

#include	"Rectangle.h"
#include	"Circle.h"
#include	"../ParserManager.h"

namespace Sample {

	/**
	 * @brief		�R���W�����̃^�C�v��
	 */
	enum class ColliderType {
		Unknown,
		Rectangle,
		Circle,
	};

	/**
	 * @brief		�R���W�����쓮�p�C���^�[�t�F�C�X
	 */
	class __declspec(novtable) ICollider {
	public:
		virtual bool OnCollisionPoint(const float x, const float y) = 0;
		virtual const int Layer() const noexcept = 0;
		virtual void Layer(const int l) = 0;
		virtual const ColliderType Type() const noexcept = 0;
	};
	//�|�C���^�u������
	using ColliderPtr = std::shared_ptr<ICollider>;

	/**
	 * @brief		��`�ڐG����
	 */
	class ColliderRect : public ParameterBase<ColliderRect>, public ICollider {
	protected:
		/** ���背�C���[ */
		int					Layer_;
		/** �����` */
		RectangleF			Rect_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		ColliderRect()
		: ParameterBase()
		, Rect_() 
		, Layer_(0) {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		ColliderRect(const ColliderRect& obj)
		: ParameterBase(obj)
		, Rect_(obj.Rect_) 
		, Layer_(0) {
		}
		/**
		 * @brief		�J�n
		 */
		void Initialize() {
		}
		/**
		 * @brief		�J�n
		 */
		void Initialize(const RectangleF& r) {
			Rect_ = r;
		}
		/**
		 * @brief		�J�n
		 */
		void Start() override {
		}

		/**
		 * @brief		�_�Ƃ̐ڐG����
		 * @param[in]	x		���肷�鉡�ʒu
		 * @param[in]	y		���肷��c�ʒu
		 * @return		true	�ڐG���Ă���
		 *				false	�ڐG���Ă��Ȃ�
		 */
		bool OnCollisionPoint(const float x, const float y) override {
			RectangleF hrec = Rectangle();
			return hrec.Collision(x, y);
		}

		const int Layer() const noexcept override { return Layer_; }
		void Layer(const int l) override { Layer_ = l; }
		const RectangleF Rectangle() const noexcept {
			RectangleF hrec = Rect_;
			const glm::mat4 mat = actor->Matrix();
			hrec += Vector3F(mat[3].x, -mat[3].y, 0);
			return hrec;
		}
		void Rectangle(const RectangleF& l) { Rect_ = l; }
		const ColliderType Type() const noexcept override { return ColliderType::Rectangle; }

		REGISTER_COMPONENT(ColliderRect)
			REGISTER_PARAMETER(Rect_)
			REGISTER_PARAMETER(Layer_)
		REGISTER_END
	};
	
	/**
	 * @brief		��`�ڐG����
	 */
	class ColliderCircle : public ParameterBase<ColliderCircle>, public ICollider {
	protected:
		/** ���背�C���[ */
		int					Layer_;
		/** ����~ */
		CircleF				Circle_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		ColliderCircle()
		: ParameterBase()
		, Circle_() 
		, Layer_(0) {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		ColliderCircle(const ColliderCircle& obj)
		: ParameterBase(obj)
		, Circle_(obj.Circle_) 
		, Layer_(0) {
		}
		/**
		 * @brief		�J�n
		 */
		void Initialize() {
		}
		/**
		 * @brief		�J�n
		 */
		void Initialize(const CircleF& r) {
			Circle_ = r;
		}
		/**
		 * @brief		�J�n
		 */
		void Start() override {
		}

		/**
		 * @brief		�_�Ƃ̐ڐG����
		 * @param[in]	x		���肷�鉡�ʒu
		 * @param[in]	y		���肷��c�ʒu
		 * @return		true	�ڐG���Ă���
		 *				false	�ڐG���Ă��Ȃ�
		 */
		bool OnCollisionPoint(const float x, const float y) override {
			CircleF hrec = Circle();
			return hrec.Collision(x, y);
		}

		const int Layer() const noexcept override { return Layer_; }
		void Layer(const int l) override { Layer_ = l; }
		const CircleF Circle() const noexcept {
			CircleF hrec = Circle_;
			const glm::mat4 mat = actor->Matrix();
			hrec.X += mat[3].x;
			hrec.Y -= mat[3].y;
			return hrec;
		}
		void Circle(const CircleF& l) { Circle_ = l; }
		const ColliderType Type() const noexcept override { return ColliderType::Circle; }

		REGISTER_COMPONENT(ColliderCircle)
			REGISTER_PARAMETER(Circle_)
			REGISTER_PARAMETER(Layer_)
		REGISTER_END
	};

}