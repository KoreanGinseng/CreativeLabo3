#pragma once

#include	<functional>

template <class... Args>
class Observer {
private:
public:
	/**
	 * @brief	通知メソッド
	 */
	virtual void Notify(Args...) = 0;
};

/**
 * 関数単体での登録用
 */
template <class... Args>
class ObserverFunction : public Observer<Args...> {
private:
	/** 通知時に実行する関数 */
	std::function<void(Args...)>	func;
public:
	/**
	 * コンストラクタ
	 */
	ObserverFunction(std::function<void(Args...)>& f)
		: func(f) {
	}

	/**
	 * @brief	通知メソッド
	 */
	void Notify(Args... args) override {
		func(args...);
	}
};