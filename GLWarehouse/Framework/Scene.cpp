#pragma once

#include	"Scene.h"

using namespace Sample;

/**
 * @brief		�V�[���̕ۑ�
 */
bool Scene::Save()
{
	nlohmann::json os;
	std::ofstream ofs(Name_ + ".json");
	if (ofs.fail())
	{
		return false;
	}
	os["Name"] = Name_;

	//���\�[�X�ۑ�
	nlohmann::json& osr = os["Resources"];

	//�e�N�X�`���ۑ�
	nlohmann::json& ost = osr["Textures"];
	int tn = 0;
	for (auto& it : Resources_->Textures().Resources())
	{
		nlohmann::json& ostn = ost[tn++];
		ostn[it.first] = it.second->Name();
	}

	//�V�F�[�_�[�ۑ�
	nlohmann::json& ossh = osr["Shaders"];
	int shn = 0;
	for (auto& it : Resources_->Shaders().Resources())
	{
		nlohmann::json& ostn = ossh[shn++];
		ostn[it.first] = it.second->Name();
	}

	//�T�E���h�ۑ�
	nlohmann::json& oss = osr["Sounds"];
	int sn = 0;
	for (auto& it : Resources_->Sounds().Resources())
	{
		nlohmann::json& ostn = oss[sn++];
		ostn[it.first] = it.second->Name();
	}

	//�A�N�^�[�ۑ�
	nlohmann::json& osa = os["Actors"];
	int n = 0;
	for (auto it : *Actors_)
	{
		it->ToJson(osa[n]);
		n++;
	}
	ofs << std::setw(4) << os << std::endl;
	return true;
}

/**
 * @brief		�V�[���̓ǂݍ���
 */
bool Scene::Load()
{
	std::ifstream ifs(Name_ + ".json");
	if (ifs.fail())
	{
		return false;
	}
	nlohmann::json os = nlohmann::json::parse(ifs);
	os["Name"].get_to(Name_);

	//���\�[�X�ǂݍ���
	nlohmann::json& osr = os["Resources"];

	//�e�N�X�`��
	nlohmann::json& ost = osr["Textures"];
	int tsize = ost.size();
	for (int i = 0; i < tsize; i++)
	{
		nlohmann::json& ostn = ost[i];
		std::string name;
		ostn.begin()->get_to(name);
		Resources_->Textures().Add(ostn.begin().key().c_str(), name.c_str());
	}

	//�V�F�[�_�[
	nlohmann::json& ossh = osr["Shaders"];
	int shsize = ossh.size();
	for (int i = 0; i < shsize; i++)
	{
		nlohmann::json& ostn = ossh[i];
		std::string name;
		ostn.begin()->get_to(name);
		Resources_->Shaders().Add(ostn.begin().key().c_str(), name.c_str());
	}

	//�T�E���h
	nlohmann::json& oss = osr["Sounds"];
	int ssize = oss.size();
	for (int i = 0; i < ssize; i++)
	{
		nlohmann::json& ostn = oss[i];
		std::string name;
		ostn.begin()->get_to(name);
		Resources_->Sounds().Add(ostn.begin().key().c_str(), name.c_str());
	}

	//�A�N�^�[�ǂݍ���
	nlohmann::json& osa = os["Actors"];
	int size = osa.size();
	for (int i = 0; i < size; i++)
	{
		ActorPtr pt = ParserManager::Instance().CreateActor(osa[i].begin().key());
		pt->FromJson(osa[i]);
		AddActor(pt);
	}
	//�e�q�֌W�\�z
	for (auto it : *Actors_)
	{
		const auto& ft = std::find_if(Actors_->begin(), Actors_->end(), [&](const ActorPtr& pt) { return pt->Name() == it->ParentName(); });
		if (ft != Actors_->end())
		{
			it->Parent(*ft);
		}
	}
	return true;
}