#pragma once

#include	"Scene.h"

using namespace Sample;

/**
 * @brief		シーンの保存
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

	//リソース保存
	nlohmann::json& osr = os["Resources"];

	//テクスチャ保存
	nlohmann::json& ost = osr["Textures"];
	int tn = 0;
	for (auto& it : Resources_->Textures().Resources())
	{
		nlohmann::json& ostn = ost[tn++];
		ostn[it.first] = it.second->Name();
	}

	//シェーダー保存
	nlohmann::json& ossh = osr["Shaders"];
	int shn = 0;
	for (auto& it : Resources_->Shaders().Resources())
	{
		nlohmann::json& ostn = ossh[shn++];
		ostn[it.first] = it.second->Name();
	}

	//サウンド保存
	nlohmann::json& oss = osr["Sounds"];
	int sn = 0;
	for (auto& it : Resources_->Sounds().Resources())
	{
		nlohmann::json& ostn = oss[sn++];
		ostn[it.first] = it.second->Name();
	}

	//アクター保存
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
 * @brief		シーンの読み込み
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

	//リソース読み込み
	nlohmann::json& osr = os["Resources"];

	//テクスチャ
	nlohmann::json& ost = osr["Textures"];
	int tsize = ost.size();
	for (int i = 0; i < tsize; i++)
	{
		nlohmann::json& ostn = ost[i];
		std::string name;
		ostn.begin()->get_to(name);
		Resources_->Textures().Add(ostn.begin().key().c_str(), name.c_str());
	}

	//シェーダー
	nlohmann::json& ossh = osr["Shaders"];
	int shsize = ossh.size();
	for (int i = 0; i < shsize; i++)
	{
		nlohmann::json& ostn = ossh[i];
		std::string name;
		ostn.begin()->get_to(name);
		Resources_->Shaders().Add(ostn.begin().key().c_str(), name.c_str());
	}

	//サウンド
	nlohmann::json& oss = osr["Sounds"];
	int ssize = oss.size();
	for (int i = 0; i < ssize; i++)
	{
		nlohmann::json& ostn = oss[i];
		std::string name;
		ostn.begin()->get_to(name);
		Resources_->Sounds().Add(ostn.begin().key().c_str(), name.c_str());
	}

	//アクター読み込み
	nlohmann::json& osa = os["Actors"];
	int size = osa.size();
	for (int i = 0; i < size; i++)
	{
		ActorPtr pt = ParserManager::Instance().CreateActor(osa[i].begin().key());
		pt->FromJson(osa[i]);
		AddActor(pt);
	}
	//親子関係構築
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