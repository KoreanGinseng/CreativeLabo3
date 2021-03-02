#pragma once

/**
 * ������
 */
template <typename T>
class ICreator {
private:
public:
	/**
	 * @brief	�G�̐����������Ȃ����߂̃��\�b�h
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
	 * @brief	�G�̐����������Ȃ����߂̃��\�b�h
	 */
	virtual Base* Create() const {
		return new Derived();
	}
};