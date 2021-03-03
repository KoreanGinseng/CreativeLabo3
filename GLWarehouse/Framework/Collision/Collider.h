#pragma once

#include	"Rectangle.h"
#include	"Circle.h"
#include	"../ParserManager.h"

namespace Sample {

	/**
	 * @brief		コリジョンのタイプ列挙
	 */
	enum class ColliderType {
		Unknown,
		Rectangle,
		Circle,
	};

	/**
	 * @brief		コリジョン駆動用インターフェイス
	 */
	class __declspec(novtable) ICollider {
	public:
		virtual bool OnCollisionPoint(const float x, const float y) = 0;
		virtual const int Layer() const noexcept = 0;
		virtual void Layer(const int l) = 0;
		virtual const ColliderType Type() const noexcept = 0;
	};
	//ポインタ置き換え
	using ColliderPtr = std::shared_ptr<ICollider>;

	/**
	 * @brief		矩形接触判定
	 */
	class ColliderRect : public ParameterBase<ColliderRect>, public ICollider {
	protected:
		/** 判定レイヤー */
		int					Layer_;
		/** 判定矩形 */
		RectangleF			Rect_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		ColliderRect()
		: ParameterBase()
		, Rect_() 
		, Layer_(0) {
		}
		/**
		 * @brief		コンストラクタ
		 */
		ColliderRect(const ColliderRect& obj)
		: ParameterBase(obj)
		, Rect_(obj.Rect_) 
		, Layer_(0) {
		}
		/**
		 * @brief		開始
		 */
		void Initialize() {
		}
		/**
		 * @brief		開始
		 */
		void Initialize(const RectangleF& r) {
			Rect_ = r;
		}
		/**
		 * @brief		開始
		 */
		void Start() override {
		}

		/**
		 * @brief		点との接触判定
		 * @param[in]	x		判定する横位置
		 * @param[in]	y		判定する縦位置
		 * @return		true	接触している
		 *				false	接触していない
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
	 * @brief		矩形接触判定
	 */
	class ColliderCircle : public ParameterBase<ColliderCircle>, public ICollider {
	protected:
		/** 判定レイヤー */
		int					Layer_;
		/** 判定円 */
		CircleF				Circle_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		ColliderCircle()
		: ParameterBase()
		, Circle_() 
		, Layer_(0) {
		}
		/**
		 * @brief		コンストラクタ
		 */
		ColliderCircle(const ColliderCircle& obj)
		: ParameterBase(obj)
		, Circle_(obj.Circle_) 
		, Layer_(0) {
		}
		/**
		 * @brief		開始
		 */
		void Initialize() {
		}
		/**
		 * @brief		開始
		 */
		void Initialize(const CircleF& r) {
			Circle_ = r;
		}
		/**
		 * @brief		開始
		 */
		void Start() override {
		}

		/**
		 * @brief		点との接触判定
		 * @param[in]	x		判定する横位置
		 * @param[in]	y		判定する縦位置
		 * @return		true	接触している
		 *				false	接触していない
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