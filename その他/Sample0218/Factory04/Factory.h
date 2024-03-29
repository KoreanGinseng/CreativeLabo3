#pragma once

#include	<unordered_map>
#include	"Creator.h"

/**
 * 生成工場
 * 生産工場に登録できるのはTを継承したオブジェクトを
 * 生成できるCreatorのみとする
 */
template <typename T>
class Factory {
private:
	/** 生産者リスト */
	std::unordered_map< std::string, std::unique_ptr< ICreator<T> >> products;
public:
	/**
	 * @brief	生産者を登録する
	 *			渡されたptrは内部のmapに所有権を移譲する
	 * @param[in]	key			登録キー
	 * @param[in]	ptr			登録する生産者
	 */
	void Register(const std::string& key, std::unique_ptr< ICreator<T> > ptr)
	{
		products[key] = std::move(ptr);
	}
	/**
	 * @brief	生産者を登録する
	 * @param[in]	key			登録キー
	 */
	template< typename Derived >
	void Register(const std::string& key)
	{
		products[key] = std::make_unique< Creator<T, Derived> >();
	}

	/**
	 * @brief	生成をおこなうためのメソッド
	 * @param[in]	key			登録キー
	 */
	T* Create(const std::string& key) {
		return products[key]->Create();
	}
};