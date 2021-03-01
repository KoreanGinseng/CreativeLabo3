#pragma once
#include "Mof.h"
#include <unordered_map>

namespace sip
{
	// ********************************************************************************
	/// <summary>
	/// アセットのベースクラス
	/// </summary>
	// ********************************************************************************
	template <typename Key, typename T > class CAssetBase
	{
	private:
		// operator= の削除
		void operator= (const CAssetBase<Key,T>&) = delete;

		std::unordered_map<Key, std::shared_ptr<T>> m_AssetMap; //! マップ

		// ********************************************************************************
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/04/26</changed>
		// ********************************************************************************
		CAssetBase(void) :
			m_AssetMap()
		{
		}

		// ********************************************************************************
		/// <summary>
		/// マップの取得
		/// </summary>
		/// <returns>マップ</returns>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/04/26</changed>
		// ********************************************************************************
		static CAssetBase<Key,T>& GetAssetMap(void)
		{
			static CAssetBase<Key,T> assetMap;
			return assetMap;
		}

	public:

		// ********************************************************************************
		/// <summary>
		/// デストラクタ
		/// </summary>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/04/26</changed>
		// ********************************************************************************
		~CAssetBase(void)
		{
			Release();
		}

		// ********************************************************************************
		/// <summary>
		/// アセットの登録。
		/// </summary>
		/// <param name="key">検索用キー</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>登録が成功すれば true それ以外 false</returns>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static bool Load(const Key& key, LPCMofChar fileName)
		{
			// 存在するかの検索。
			if (GetAssetMap().m_AssetMap.find(key) == GetAssetMap().m_AssetMap.end() ||
				GetAssetMap().m_AssetMap[key] == nullptr)
			{
				// 存在しない場合はポインタを作成する。
				GetAssetMap().m_AssetMap[key] = std::make_shared<T>();
			}
			// アセットの読み込みを行う。
			return GetAssetMap().m_AssetMap[key]->Load(fileName);
		}

		// ********************************************************************************
		/// <summary>
		/// 登録したキーからアセットの取得。
		/// </summary>
		/// <param name="key">キー</param>
		/// <returns>データを返す。データがない場合 empty を返す。</returns>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/04/26</changed>
		// ********************************************************************************
		static std::shared_ptr<T> GetAsset(const Key& key)
		{
			return GetAssetMap().m_AssetMap[key];
		}

		// ********************************************************************************
		/// <summary>
		/// マップからキーのデータを削除する。
		/// </summary>
		/// <param name="key">検索用キー</param>
		/// <created>いのうえ,2020/09/10</created>
		/// <changed>いのうえ,2020/09/10</changed>
		// ********************************************************************************
		static void Erase(const Key& key)
		{
			Release(key);
			GetAssetMap().m_AssetMap.erase(key);
		}

		// ********************************************************************************
		/// <summary>
		/// 指定データの解放。
		/// </summary>
		/// <param name="key">開放するデータのキー</param>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/11/11</changed>
		// ********************************************************************************
		static void Release(const Key& key)
		{
			if (GetAssetMap().m_AssetMap[key])
			{
				GetAssetMap().m_AssetMap[key]->Release();
			}
		}

		// ********************************************************************************
		/// <summary>
		/// 全データの解放。
		/// </summary>
		/// <created>いのうえ,2020/04/26</created>
		/// <changed>いのうえ,2020/09/10</changed>
		// ********************************************************************************
		static void Release(void)
		{
			for (auto& itr : GetAssetMap().m_AssetMap)
			{
				if (itr.second)
				{
					itr.second->Release();
				}
			}
		}
	};
}