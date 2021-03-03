#pragma once

#include	<string>
#include	<list>
#include	<memory>

namespace Sample {

/**
 * @brief		C#スクリプトからアクセスするためのライブラリとのシェア用Actor
 *				C#スクリプトからアクセスしたいパラメーターは
 *				このクラスにインターフェイスとして用意し
 *				SharedWrapperでラップすること
 */
class __declspec(novtable) ISharedActor {
public:
	/**
	 * @brief		nameへのAccessor
	 * @return		名前
	 */
	virtual const std::string& Name() const noexcept = 0;

	/**
	 * @brief		posへのAccessor
	 * @param[in]	x	X位置
	 * @param[in]	y	Y位置
	 */
	virtual void Position(const float x, const float y) = 0;

	/**
	 * @brief		posxへのAccessor
	 * @param[in]	x	X位置
	 */
	virtual void X(const float x) = 0;
	/**
	 * @brief		posxへのAccessor
	 * @return		X位置
	 */
	virtual const float X() const noexcept = 0;

	/**
	 * @brief		posyへのAccessor
	 * @param[in]	y	Y位置
	 */
	virtual void Y(const float y) = 0;
	/**
	 * @brief		posyへのAccessor
	 * @return		Y位置
	 */
	virtual const float Y() const noexcept = 0;
	
	/**
	 * @brief		poszへのAccessor
	 * @param[in]	z	Z位置
	 */
	virtual void Z(const float z) = 0;
	/**
	 * @brief		poszへのAccessor
	 * @return		Z位置
	 */
	virtual const float Z() const noexcept = 0;
};

}