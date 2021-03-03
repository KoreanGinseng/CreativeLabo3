#pragma once

#include	"Common.h"
#include	"Vector3.h"
#include	"SharedInterface.h"

namespace Sample {

//アクター前方定義
class Actor;
//ポインタ置き換え
using ActorPtr = std::shared_ptr<Actor>;
//動的配列置き換え
using ActorList = std::list<ActorPtr>;
using ActorListPtr = std::shared_ptr<ActorList>;

//--------------------------------------------------------------
#pragma region 動作用インターフェイス
//--------------------------------------------------------------
class __declspec(novtable) IInterface {
public:
	virtual ~IInterface() = default;
};
//ポインタ置き換え
using InterfacePtr = std::shared_ptr<IInterface>;
//動的配列置き換え
using InterfaceArray = std::vector<InterfacePtr>;

class __declspec(novtable) IComponent : public IInterface {
public:
	virtual void Initialize() = 0;
	virtual void Start() = 0;
	virtual void SetActor(Actor* ac) = 0;
	virtual Actor* GetActor(void) = 0;

};
//ポインタ置き換え
using ComponentPtr = std::shared_ptr<IComponent>;
//動的配列置き換え
using ComponentArray = std::vector<ComponentPtr>;

class __declspec(novtable) IParameter : public IComponent {
public:
	virtual std::shared_ptr<IParameter> Clone() = 0;
};
//ポインタ置き換え
using ParameterPtr = std::shared_ptr<IParameter>;
//動的配列置き換え
using ParameterArray = std::vector<ParameterPtr>;

class __declspec(novtable) IAction : public IComponent {
public:
	virtual void Update() = 0;
	virtual std::shared_ptr<IAction> Clone() = 0;
	virtual void Enabled(const bool p) noexcept = 0;
	virtual const bool Enabled(void) const noexcept = 0;
};
//ポインタ置き換え
using ActionPtr = std::shared_ptr<IAction>;
//動的配列置き換え
using ActionArray = std::vector<ActionPtr>;

class __declspec(novtable) IRender : public IComponent {
public:
	virtual void Render() = 0;
	virtual std::shared_ptr<IRender> Clone() = 0;
	virtual void Enabled(const bool p) noexcept = 0;
	virtual const bool Enabled(void) const noexcept = 0;
	virtual void Priority(const int p) noexcept = 0;
	virtual const int Priority(void) const noexcept = 0;
};
//ポインタ置き換え
using RenderPtr = std::shared_ptr<IRender>;
//動的配列置き換え
using RenderArray = std::vector<RenderPtr>;
#pragma endregion
//--------------------------------------------------------------

//--------------------------------------------------------------
#pragma region 基底アクター関連
//--------------------------------------------------------------
class __declspec(novtable) IActor : public ISharedActor {
public:
	/**
	 * @brief		開始
	 */
	virtual void Start() = 0;

	/**
	 * @brief		アクターを動作させる
	 */
	virtual void Update() = 0;

	/**
	 * @brief		行列の更新
	 */
	virtual void RefreshMatrix() = 0;

	/**
	 * @brief		アクターを表示させる
	 */
	virtual void Render() = 0;

	/**
	 * @brief		nameへのAccessor
	 * @param[in]	nm	名前
	 */
	virtual void Name(const char* nm) = 0;

	/**
	 * @brief		パラメーターの取得
	 * @return		パラメーター
	 */
	virtual const ParameterArray& GetParameters(void) const = 0;

	/**
	 * @brief		更新機能の取得
	 * @return		更新機能
	 */
	virtual const ActionArray& GetActions(void) const = 0;

	/**
	 * @brief		描画機能の取得
	 * @return		描画機能
	 */
	virtual const RenderArray& GetRenders(void) const = 0;

	/**
	 * @brief		アクターの更新有無
	 * @return		true	更新あり
	 *				false	更新なし
	 */
	virtual bool IsAction() const = 0;

	/**
	 * @brief		アクターの描画有無
	 * @return		true	描画あり
	 *				false	描画なし
	 */
	virtual bool IsRender() const = 0;

	/**
	 * @brief		posへのAccessor
	 * @param[in]	p	位置
	 */
	virtual void Position(const Vector3F& p) = 0;
	/**
	 * @brief		posへのAccessor
	 * @return		位置
	 */
	virtual const Vector3F& Position() const noexcept = 0;

	/**
	 * @brief		rotationへのAccessor
	 * @param[in]	p	向き
	 */
	virtual void Rotation(const Vector3F& p) = 0;
	/**
	 * @brief		rotationへのAccessor
	 * @return		向き
	 */
	virtual const Vector3F& Rotation() const noexcept = 0;

	/**
	 * @brief		scaleへのAccessor
	 * @param[in]	p	大きさ
	 */
	virtual void Scale(const Vector3F& p) = 0;
	/**
	 * @brief		scaleへのAccessor
	 * @return		大きさ
	 */
	virtual const Vector3F& Scale() const noexcept = 0;

	/**
	 * @brief		WorldMatrixへのAccessor
	 * @return		行列
	 */
	virtual const glm::mat4& Matrix() const noexcept = 0;

	/**
	 * @brief		parentへのAccessor
	 * @param[in]	p	親ポインタ
	 */
	virtual void Parent(const ActorPtr& p) = 0;
	/**
	 * @brief		parentへのAccessor
	 * @return		親ポインタ
	 */
	virtual const ActorPtr& Parent() const noexcept = 0;
	/**
	 * @brief		nameへのAccessor
	 * @return		名前
	 */
	virtual const std::string& ParentName() const noexcept = 0;

	/**
	 * @brief		終了フラグを立てる
	 */
	virtual void Destroy() = 0;

	/**
	 * @brief		アクティブフラグ操作
	 * @param[in]	f	フラグ
	 */
	virtual void SetActive(const bool f) = 0;

	/**
	 * @brief		動作判定
	 * @return		動作フラグ
	 */
	virtual bool IsActive() const = 0;
	/**
	 * @brief		終了判定
	 * @return		終了フラグ
	 */
	virtual bool IsEnd() const = 0;

	/**
	 * @brief		JSON生成
	 * @param[in]	os	JSONファイル
	 */
	virtual void ToJson(nlohmann::json& os) = 0;
	/**
	 * @brief		JSON生成
	 * @param[in]	os	JSONファイル
	 */
	virtual void ToJsonImpl(nlohmann::json& os) = 0;

	/**
	 * @brief		JSONから生成
	 * @param[in]	os	JSONファイル
	 */
	virtual void FromJson(nlohmann::json& os) = 0;
	/**
	 * @brief		JSONから生成
	 * @param[in]	os	JSONファイル
	 */
	virtual void FromJsonImpl(nlohmann::json& os) = 0;
};
#pragma endregion
//--------------------------------------------------------------

}