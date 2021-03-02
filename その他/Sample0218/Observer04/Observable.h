#pragma once

#include	"Observer.h"

template <class... Args>
class IObservable {
private:
public:
	/**
	 * @brief	通知を受けるオブザーバーの登録
	 */
	virtual void Subscribe(Observer<Args...>* pobs) = 0;

	/**
	 * @brief	通知を受けるオブザーバーの削除
	 */
	virtual void Dispose(Observer<Args...>* pobs) = 0;
};