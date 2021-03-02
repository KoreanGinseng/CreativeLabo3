#pragma once

/**
 * 生成者
 */
template <typename T>
class ICreator {
private:
public:
	/**
	 * @brief	敵の生成をおこなうためのメソッド
	 */
	virtual T* Create() const {
		return new T();
	}
};
template <typename Base, typename Derived>
class Creator : public ICreator< Base >{
private:
public:
	/**
	 * @brief	敵の生成をおこなうためのメソッド
	 */
	virtual Base* Create() const {
		return new Derived();
	}
};