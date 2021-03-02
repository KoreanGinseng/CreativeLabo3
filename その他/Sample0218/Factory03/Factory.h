#pragma once

#include	<unordered_map>
#include	"Creator.h"

/**
 * ¶¬Hê
 * ¶YHê‚É“o˜^‚Å‚«‚é‚Ì‚ÍT‚ğŒp³‚µ‚½ƒIƒuƒWƒFƒNƒg‚ğ
 * ¶¬‚Å‚«‚éCreator‚Ì‚İ‚Æ‚·‚é
 */
template <typename T>
class Factory {
private:
	/** ¶YÒƒŠƒXƒg */
	std::unordered_map< std::string, std::unique_ptr< ICreator<T> >> products;
public:
	/**
	 * @brief	¶YÒ‚ğ“o˜^‚·‚é
	 *			“n‚³‚ê‚½ptr‚Í“à•”‚Ìmap‚ÉŠ—LŒ ‚ğˆÚ÷‚·‚é
	 * @param[in]	key			“o˜^ƒL[
	 * @param[in]	ptr			“o˜^‚·‚é¶YÒ
	 */
	void Register(const std::string& key, std::unique_ptr< ICreator<T> > ptr)
	{
		products[key] = std::move(ptr);
	}
	/**
	 * @brief	¶YÒ‚ğ“o˜^‚·‚é
	 * @param[in]	key			“o˜^ƒL[
	 */
	template< typename Derived >
	void Register(const std::string& key)
	{
		products[key] = std::make_unique< Creator<T, Derived> >();
	}

	/**
	 * @brief	¶¬‚ğ‚¨‚±‚È‚¤‚½‚ß‚Ìƒƒ\ƒbƒh
	 * @param[in]	key			“o˜^ƒL[
	 */
	T* Create(const std::string& key) {
		return products[key]->Create();
	}
};