#pragma once

template <class... Args>
class Observer {
private:
public:
	/**
	 * @brief	通知メソッド
	 */
	virtual void Notify(Args...) = 0;
};