#pragma once

#include	<functional>

template <class... Args>
class Observer {
private:
public:
	/**
	 * @brief	�ʒm���\�b�h
	 */
	virtual void Notify(Args...) = 0;
};

/**
 * �֐��P�̂ł̓o�^�p
 */
template <class... Args>
class ObserverFunction : public Observer<Args...> {
private:
	/** �ʒm���Ɏ��s����֐� */
	std::function<void(Args...)>	func;
public:
	/**
	 * �R���X�g���N�^
	 */
	ObserverFunction(std::function<void(Args...)>& f)
		: func(f) {
	}

	/**
	 * @brief	�ʒm���\�b�h
	 */
	void Notify(Args... args) override {
		func(args...);
	}
};