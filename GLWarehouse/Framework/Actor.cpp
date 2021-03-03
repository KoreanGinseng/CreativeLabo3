#include	"Actor.h"
#include	"SceneManager.h"
#include	"ParserManager.h"

using namespace Sample;

/**
 * @brief		parentへのAccessor
 * @param[in]	p	親ポインタ
 */
void Actor::Parent(const ActorPtr& p) {
	//アクターリスト
	ActorListPtr& actors = SceneManager::Instance().CurrentScene()->Actors();
	ActorList& rootactors = SceneManager::Instance().CurrentScene()->RootActors();
	//アクターの参照を検索
	const auto& it = std::find_if(actors->begin(), actors->end(), [this](const ActorPtr& pt) { return pt.get() == this; });
	if (it == actors->end())
	{
		ERROR_LOG("Actor Find Error!!");
		return;
	}
	//子ども設定
	if (parent_)
	{
		parent_->children_->remove(*it);
		rootactors.push_back(*it);
	}

	parent_ = p;

	//子ども設定
	if (parent_)
	{
		parentName_ = parent_->Name();
		parent_->children_->push_back(*it);
		rootactors.remove(*it);
	}
	else
	{
		parentName_ = nullptr;
	}
}

/**
 * @brief		JSON生成
 * @param[in]	os	JSONファイル
 */
void Actor::ToJson(nlohmann::json& os) {
	ToJsonImpl(os["Actor"]);
}

/**
 * @brief		JSON生成
 * @param[in]	os	JSONファイル
 */
void Actor::ToJsonImpl(nlohmann::json& os) {
	os["Name"] = name_;
	pos_.ToJson(os["Position"]);
	rotation_.ToJson(os["Rotation"]);
	scale_.ToJson(os["Scale"]);
	os["Active"] = active_;
	os["Parent"] = parentName_;
	for (auto it : parameter_)
	{
		ParserManager::Instance().ToJson(it, os);
	}
	for (auto it : action_)
	{
		ParserManager::Instance().ToJson(it, os);
	}
	for (auto it : render_)
	{
		ParserManager::Instance().ToJson(it, os);
	}
}
/**
 * @brief		JSONから生成
 * @param[in]	os	JSONファイル
 */
void Actor::FromJson(nlohmann::json& os) {
	FromJsonImpl(os["Actor"]);
}
/**
 * @brief		JSONから生成
 * @param[in]	os	JSONファイル
 */
void Actor::FromJsonImpl(nlohmann::json& os) {
	os["Name"].get_to(name_);
	pos_.FromJson(os["Position"]);
	rotation_.FromJson(os["Rotation"]);
	scale_.FromJson(os["Scale"]);
	os["Active"].get_to(active_);
	os["Parent"].get_to(parentName_);
	ParserManager::Instance().Create(this, os);
}