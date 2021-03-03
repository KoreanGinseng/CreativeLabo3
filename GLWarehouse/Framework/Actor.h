#pragma once

#include	"Interface.h"

namespace Sample {

//--------------------------------------------------------------
#pragma region 基底アクター関連
//--------------------------------------------------------------
class Actor : public IActor {
protected:
	/** アクターの名前 */
	std::string		name_;
	/** アクターの位置 */
	Vector3F		pos_;
	/** アクターの向き */
	Vector3F		rotation_;
	/** アクターの大きさ */
	Vector3F		scale_;

	/** 行列 */
	glm::mat4		LocalMatrix_;
	/** 行列 */
	glm::mat4		WorldMatrix_;

	/** アクティブフラグ */
	bool			active_;
	/** 終了フラグ */
	bool			end_;

	/** アクターパラメーター */
	ParameterArray	parameter_;
	/** アクター動作機能 */
	ActionArray		action_;
	/** アクター描画機能 */
	RenderArray		render_;

	/** 親アクター */
	ActorPtr		parent_;
	/** 親アクターの名前 */
	std::string		parentName_;
	/** 子供 */
	ActorListPtr	children_;
public:
	/**
	 * @brief		コンストラクタ
	 * @param[in]	nm	名前
	 */
	Actor(const char* nm)
	: name_(nm)
	, pos_()
	, rotation_()
	, scale_(1, 1, 1)
	, LocalMatrix_()
	, WorldMatrix_()
	, active_(true)
	, end_(false)
	, parameter_()
	, action_()
	, render_() 
	, parent_()
	, parentName_()
	, children_(std::make_shared<ActorList>()) {
	}
	/**
	 * @brief		コピーコンストラクタ
	 * @param[in]	obj	コピーするアクター
	 */
	Actor(const Actor& obj)
	: name_(obj.name_)
	, pos_(obj.pos_)
	, rotation_(obj.rotation_)
	, scale_(obj.scale_)
	, LocalMatrix_(obj.LocalMatrix_)
	, WorldMatrix_(obj.WorldMatrix_)
	, active_(obj.active_)
	, end_(obj.end_)
	, parameter_()
	, action_()
	, render_()
	, parent_()
	, parentName_(obj.parentName_)
	, children_(std::make_shared<ActorList>()) {
		std::transform(obj.parameter_.begin(), obj.parameter_.end(), back_inserter(parameter_), [&](const ParameterPtr& base)
			{
				ParameterPtr act = base->Clone();
				act->SetActor(this);
				return act;
			}
		);
		std::transform(obj.action_.begin(), obj.action_.end(), back_inserter(action_), [&](const ActionPtr& base)
			{
				ActionPtr act = base->Clone();
				act->SetActor(this);
				return act;
			}
		);
		std::transform(obj.render_.begin(), obj.render_.end(), back_inserter(render_), [&](const RenderPtr& base)
			{
				RenderPtr act = base->Clone();
				act->SetActor(this);
				return act;
			}
		);
		std::transform(obj.children_->begin(), obj.children_->end(), back_inserter(*children_), [&](const ActorPtr& base)
			{
				return std::make_shared<Actor>(*base);
			}
		);
	}

	/**
	 * @brief		デストラクタ
	 */
	virtual ~Actor() {
		children_.reset();
	}

	/**
	 * @brief		開始
	 */
	void Start() override {
		std::for_each(parameter_.begin(), parameter_.end(), [](const ParameterPtr& act) { act->Start(); });
		std::for_each(action_.begin(), action_.end(), [](const ActionPtr& act) { act->Start(); });
		std::for_each(render_.begin(), render_.end(), [](const RenderPtr& act){ act->Start(); } );
	}

	/**
	 * @brief		アクターを動作させる
	 */
	void Update() override {
		//動作更新
		for (auto it : action_)
		{
			if (!it->Enabled())
			{
				continue;
			}
			it->Update();
		}

		//行列更新
		RefreshMatrix();

		//子供の更新
		for (auto it : *children_)
		{
			if (!it->IsActive())
			{
				continue;
			}
			it->Update();
		}
	}

	/**
	 * @brief		行列の更新
	 */
	void RefreshMatrix() override {
		const glm::mat4 matT = glm::translate(glm::mat4(1), glm::vec3(pos_.x, -pos_.y, pos_.z));
		const glm::mat4 matRY = glm::rotate(matT, glm::radians(rotation_.y), glm::vec3(0, 1, 0));
		const glm::mat4 matRXY = glm::rotate(matRY, glm::radians(rotation_.x), glm::vec3(1, 0, 0));
		const glm::mat4 matRZXY = glm::rotate(matRXY, glm::radians(-rotation_.z), glm::vec3(0, 0, 1));
		const glm::mat4 matS = glm::scale(matRZXY, glm::vec3(scale_.x, scale_.y, scale_.z));
		LocalMatrix_ = matS;
		if (parent_)
		{
			WorldMatrix_ = parent_->Matrix() * LocalMatrix_;
		}
		else
		{
			WorldMatrix_ = LocalMatrix_;
		}
	}

	/**
	 * @brief		アクターを表示させる
	 */
	void Render() override {
		for (auto it : render_)
		{
			if (!it->Enabled())
			{
				continue;
			}
			it->Render();
		}
	}

	/**
	 * @brief		nameへのAccessor
	 * @param[in]	nm	名前
	 */
	void Name(const char* nm) override { name_ = nm; }
	/**
	 * @brief		nameへのAccessor
	 * @return		名前
	 */
	const std::string& Name() const noexcept override { return name_; }

	/**
	 * @brief		パラメーターの追加
	 */
	template < typename T, typename... _Types >
	std::shared_ptr< T > AddParameter(_Types&& ... _Args) {
		auto add = std::make_shared<T>();
		add->SetActor(this);
		add->Initialize(_STD forward<_Types>(_Args)...);
		parameter_.push_back(add);
		return add;
	}

	/**
	 * @brief		動作機能の追加
	 */
	template < typename T, typename... _Types >
	std::shared_ptr< T > AddAction(_Types&& ... _Args) {
		auto add = std::make_shared<T>();
		add->SetActor(this);
		add->Initialize(_STD forward<_Types>(_Args)...);
		action_.push_back(add);
		return add;
	}

	/**
	 * @brief		描画機能の追加
	 */
	template < typename T, typename... _Types >
	std::shared_ptr< T > AddRender(_Types&& ... _Args) {
		auto add = std::make_shared<T>();
		add->SetActor(this);
		add->Initialize(_STD forward<_Types>(_Args)...);
		render_.push_back(add);
		return add;
	}

	/**
	 * @brief		コンポーネントの取得
	 * @return		テンプレートの型のコンポーネントがある場合取得する
	 *				存在しない場合は空のshared_ptrを返す
	 */
	template< typename T >
	std::shared_ptr< T > GetComponent(void) const {
		auto pt = GetParameter< T >();
		if (pt)
		{
			return pt;
		}
		auto at = GetAction< T >();
		if (at)
		{
			return at;
		}
		auto rt = GetRender< T >();
		if (rt)
		{
			return rt;
		}
		return std::shared_ptr< T >();
	}
	
	/**
	 * @brief		コンポーネントの取得
	 * @return		テンプレートの型のコンポーネントがある場合取得する
	 *				存在しない場合は空のshared_ptrを返す
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetComponents(void) {
		std::vector< std::shared_ptr< T > > re;
		auto pt = GetParameters< T >();
		auto at = GetActions< T >();
		auto rt = GetRenders< T >();
		std::copy(pt.begin(), pt.end(), std::back_inserter(re));
		std::copy(at.begin(), at.end(), std::back_inserter(re));
		std::copy(rt.begin(), rt.end(), std::back_inserter(re));
		return re;
	}

	/**
	 * @brief		パラメーターの取得
	 * @return		テンプレートの型のパラメーターがある場合取得する
	 *				存在しない場合は空のshared_ptrを返す
	 */
	template< typename T >
	std::shared_ptr< T > GetParameter(void) const {
		for (auto it : parameter_)
		{
			std::shared_ptr< T > re = std::dynamic_pointer_cast<T>(it);
			if (re)
			{
				return re;
			}
		}
		return std::shared_ptr< T >();
	}

	/**
	 * @brief		パラメーターの取得
	 * @return		テンプレートの型のパラメーターがある場合取得する
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetParameters() const {
		std::vector< std::shared_ptr< T > > re;
		for (auto it : parameter_)
		{
			std::shared_ptr< T > ptr = std::dynamic_pointer_cast<T>(it);
			if (ptr)
			{
				re.push_back(ptr);
			}
		}
		return re;
	}

	/**
	 * @brief		動作機能の取得
	 * @return		テンプレートの型の動作がある場合取得する
	 *				存在しない場合は空のshared_ptrを返す
	 */
	template< typename T >
	std::shared_ptr< T > GetAction(void) const {
		for (auto it : action_)
		{
			std::shared_ptr< T > re = std::dynamic_pointer_cast< T >(it);
			if (re)
			{
				return re;
			}
		}
		return std::shared_ptr< T >();
	}

	/**
	 * @brief		動作機能の取得
	 * @return		テンプレートの型のパラメーターがある場合取得する
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetActions() const {
		std::vector< std::shared_ptr< T > > re;
		for (auto it : action_)
		{
			std::shared_ptr< T > ptr = std::dynamic_pointer_cast<T>(it);
			if (ptr)
			{
				re.push_back(ptr);
			}
		}
		return re;
	}

	/**
	 * @brief		描画機能の取得
	 * @return		テンプレートの型の描画がある場合取得する
	 *				存在しない場合は空のshared_ptrを返す
	 */
	template< typename T >
	std::shared_ptr< T > GetRender(void) const {
		for (auto it : render_)
		{
			std::shared_ptr< T > re = std::dynamic_pointer_cast< T >(it);
			if (re)
			{
				return re;
			}
		}
		return std::shared_ptr< T >();
	}

	/**
	 * @brief		描画機能の取得
	 * @return		テンプレートの型のパラメーターがある場合取得する
	 */
	template< typename T >
	std::vector< std::shared_ptr< T > > GetRenders() const {
		std::vector< std::shared_ptr< T > > re;
		for (auto it : render_)
		{
			std::shared_ptr< T > ptr = std::dynamic_pointer_cast<T>(it);
			if (ptr)
			{
				re.push_back(ptr);
			}
		}
		return re;
	}

	/**
	 * @brief		パラメーターの取得
	 * @return		パラメーター
	 */
	const ParameterArray& GetParameters(void) const override {
		return parameter_;
	}

	/**
	 * @brief		更新機能の取得
	 * @return		更新機能
	 */
	const ActionArray& GetActions(void) const override {
		return action_;
	}

	/**
	 * @brief		描画機能の取得
	 * @return		描画機能
	 */
	const RenderArray& GetRenders(void) const override {
		return render_;
	}

	/**
	 * @brief		アクターの更新有無
	 * @return		true	更新あり
	 *				false	更新なし
	 */
	bool IsAction() const override {
		return action_.size() > 0;
	}

	/**
	 * @brief		アクターの描画有無
	 * @return		true	描画あり
	 *				false	描画なし
	 */
	bool IsRender() const override {
		return render_.size() > 0;
	}

	/**
	 * @brief		posへのAccessor
	 * @param[in]	p	位置
	 */
	void Position(const Vector3F& p) override { pos_ = p; }
	/**
	 * @brief		posへのAccessor
	 * @param[in]	x	X位置
	 * @param[in]	y	Y位置
	 */
	void Position(const float x, const float y) override { pos_.x = x;	 pos_.y = y; }
	/**
	 * @brief		posへのAccessor
	 * @return		位置
	 */
	const Vector3F& Position() const noexcept override { return pos_; }

	/**
	 * @brief		posxへのAccessor
	 * @param[in]	x	X位置
	 */
	void X(const float x) override { pos_.x = x; }
	/**
	 * @brief		posxへのAccessor
	 * @return		X位置
	 */
	const float X() const noexcept override { return pos_.x; }

	/**
	 * @brief		posyへのAccessor
	 * @param[in]	y	Y位置
	 */
	void Y(const float y) override { pos_.y = y; }
	/**
	 * @brief		posyへのAccessor
	 * @return		Y位置
	 */
	const float Y() const noexcept override { return pos_.y; }
	
	/**
	 * @brief		poszへのAccessor
	 * @param[in]	z	Z位置
	 */
	void Z(const float z) override { pos_.z = z; }
	/**
	 * @brief		poszへのAccessor
	 * @return		Z位置
	 */
	const float Z() const noexcept override { return pos_.z; }

	/**
	 * @brief		rotationへのAccessor
	 * @param[in]	p	向き
	 */
	void Rotation(const Vector3F& p) override { rotation_ = p; }
	/**
	 * @brief		rotationへのAccessor
	 * @return		向き
	 */
	const Vector3F& Rotation() const noexcept override { return rotation_; }

	/**
	 * @brief		scaleへのAccessor
	 * @param[in]	p	大きさ
	 */
	void Scale(const Vector3F& p) override { scale_ = p; }
	/**
	 * @brief		scaleへのAccessor
	 * @return		大きさ
	 */
	const Vector3F& Scale() const noexcept override { return scale_; }

	/**
	 * @brief		WorldMatrixへのAccessor
	 * @return		行列
	 */
	const glm::mat4& Matrix() const noexcept override { return WorldMatrix_; }

	/**
	 * @brief		parentへのAccessor
	 * @param[in]	p	親ポインタ
	 */
	void Parent(const ActorPtr& p) override;
	/**
	 * @brief		parentへのAccessor
	 * @return		親ポインタ
	 */
	const ActorPtr& Parent() const noexcept override { return parent_; }
	/**
	 * @brief		nameへのAccessor
	 * @return		名前
	 */
	const std::string& ParentName() const noexcept override { return parentName_; }

	/**
	 * @brief		終了フラグを立てる
	 */
	void Destroy() override { end_ = true; }

	/**
	 * @brief		アクティブフラグ操作
	 * @param[in]	f	フラグ
	 */
	void SetActive(const bool f) override { active_ = f; }

	/**
	 * @brief		動作判定
	 * @return		動作フラグ
	 */
	bool IsActive() const override { return active_; }
	/**
	 * @brief		終了判定
	 * @return		終了フラグ
	 */
	bool IsEnd() const override { return end_; }

	/**
	 * @brief		JSON生成
	 * @param[in]	os	JSONファイル
	 */
	void ToJson(nlohmann::json& os) override;
	/**
	 * @brief		JSON生成
	 * @param[in]	os	JSONファイル
	 */
	void ToJsonImpl(nlohmann::json& os) override;

	/**
	 * @brief		JSONから生成
	 * @param[in]	os	JSONファイル
	 */
	void FromJson(nlohmann::json& os) override;
	/**
	 * @brief		JSONから生成
	 * @param[in]	os	JSONファイル
	 */
	void FromJsonImpl(nlohmann::json& os) override;
};

template <typename T>
class ParameterBase : public IParameter {
protected:
	/** アクター */
	Actor* actor;
public:
	/**
	 * @brief		コンストラクタ
	 */
	ParameterBase()
	: actor(nullptr) {
	}
	/**
	 * @brief		コピーコンストラクタ
	 */
	ParameterBase(const ParameterBase& act)
	: actor(nullptr) {
	}

	/**
	 * @brief		初期化
	 */
	virtual void Initialize() {
	}

	/**
	 * @brief		開始
	 */
	virtual void Start() {
	}

	/**
	 * @brief		コピーの生成
	 */
	ParameterPtr Clone() override {
		return std::make_shared<T>(*((T*)this));
	}

	/**
	 * @brief		呼び出し元アクターの設定
	 * @param[in]	ac	元アクター
	 */
	void SetActor(Actor* ac) override { actor = ac; }

	/**
	 * @brief		呼び出し元アクターの取得
	 * @return		元アクター
	 */
	Actor* GetActor(void) override { return actor; }
};

template <typename T>
class ActionBase : public IAction {
protected:
	/** アクター */
	Actor* actor;

	/** 有効フラグ */
	bool Enabled_;
public:
	/**
	 * @brief		コンストラクタ
	 */
	ActionBase()
	: actor(nullptr)
	, Enabled_(true) {
	}
	/**
	 * @brief		コピーコンストラクタ
	 */
	ActionBase(const ActionBase& act)
	: actor(nullptr) 
	, Enabled_(act.Enabled_){
	}

	/**
	 * @brief		初期化
	 */
	virtual void Initialize() {
	}

	/**
	 * @brief		開始
	 */
	virtual void Start() {
	}

	/**
	 * @brief		コピーの生成
	 */
	ActionPtr Clone() override {
		return std::make_shared<T>(*((T*)this));
	}

	/**
	 * @brief		呼び出し元アクターの設定
	 * @param[in]	ac	元アクター
	 */
	void SetActor(Actor* ac) override { actor = ac; }

	/**
	 * @brief		呼び出し元アクターの取得
	 * @return		元アクター
	 */
	Actor* GetActor(void) override { return actor; }

	/**
	 * @brief		有効フラグの設定
	 * @param[in]	p	有効フラグ
	 */
	void Enabled(const bool p) noexcept override { Enabled_ = p; }
	/**
	 * @brief		有効フラグの取得
	 * @return		有効フラグ
	 */
	const bool Enabled(void) const noexcept override { return Enabled_; }
};

template <typename T>
class RenderBase : public IRender {
protected:
	/** アクター */
	Actor* actor;

	/** 有効フラグ */
	bool Enabled_;

	/** 描画優先順位 */
	int Priority_;
public:
	/**
	 * @brief		コンストラクタ
	 */
	RenderBase() 
	: actor(nullptr) 
	, Enabled_(true) 
	, Priority_(0) {
	}
	/**
	 * @brief		コピーコンストラクタ
	 */
	RenderBase(const RenderBase& act)
	: actor(nullptr) 
	, Enabled_(act.Enabled_)
	, Priority_(act.Priority_) {
	}

	/**
	 * @brief		初期化
	 */
	virtual void Initialize() {
	}

	/**
	 * @brief		開始
	 */
	virtual void Start() {
	}

	/**
	 * @brief		コピーの生成
	 */
	RenderPtr Clone() override {
		return std::make_shared<T>(*((T*)this));
	}

	/**
	 * @brief		呼び出し元アクターの設定
	 * @param[in]	ac	元アクター
	 */
	void SetActor(Actor* ac) override { actor = ac; }

	/**
	 * @brief		呼び出し元アクターの取得
	 * @return		元アクター
	 */
	Actor* GetActor(void) override { return actor; }

	/**
	 * @brief		有効フラグの設定
	 * @param[in]	p	有効フラグ
	 */
	void Enabled(const bool p) noexcept override { Enabled_ = p; }
	/**
	 * @brief		有効フラグの取得
	 * @return		有効フラグ
	 */
	const bool Enabled(void) const noexcept override { return Enabled_; }

	/**
	 * @brief		描画優先順位の設定
	 * @param[in]	p	描画優先順位
	 */
	void Priority(const int p) noexcept override { Priority_ = p; }
	/**
	 * @brief		描画優先順位の取得
	 * @return		描画優先順位
	 */
	const int Priority(void) const noexcept override{ return Priority_; }
};
#pragma endregion
//--------------------------------------------------------------

}