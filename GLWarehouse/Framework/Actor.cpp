#include	"Actor.h"
#include	"SceneManager.h"
#include	"ParserManager.h"

using namespace Sample;

/**
 * @brief		parent�ւ�Accessor
 * @param[in]	p	�e�|�C���^
 */
void Actor::Parent(const ActorPtr& p) {
	//�A�N�^�[���X�g
	ActorListPtr& actors = SceneManager::Instance().CurrentScene()->Actors();
	ActorList& rootactors = SceneManager::Instance().CurrentScene()->RootActors();
	//�A�N�^�[�̎Q�Ƃ�����
	const auto& it = std::find_if(actors->begin(), actors->end(), [this](const ActorPtr& pt) { return pt.get() == this; });
	if (it == actors->end())
	{
		ERROR_LOG("Actor Find Error!!");
		return;
	}
	//�q�ǂ��ݒ�
	if (parent_)
	{
		parent_->children_->remove(*it);
		rootactors.push_back(*it);
	}

	parent_ = p;

	//�q�ǂ��ݒ�
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
 * @brief		JSON����
 * @param[in]	os	JSON�t�@�C��
 */
void Actor::ToJson(nlohmann::json& os) {
	ToJsonImpl(os["Actor"]);
}

/**
 * @brief		JSON����
 * @param[in]	os	JSON�t�@�C��
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
 * @brief		JSON���琶��
 * @param[in]	os	JSON�t�@�C��
 */
void Actor::FromJson(nlohmann::json& os) {
	FromJsonImpl(os["Actor"]);
}
/**
 * @brief		JSON���琶��
 * @param[in]	os	JSON�t�@�C��
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