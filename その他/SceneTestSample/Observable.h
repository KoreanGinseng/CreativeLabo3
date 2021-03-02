#pragma once

#include	"Observer.h"

template <class... Args>
class IObservable {
private:
public:
	/**
	 * @brief	�ʒm���󂯂�I�u�U�[�o�[�̓o�^
	 */
	virtual void Subscribe(Observer<Args...>* pobs) = 0;
	/**
	 * @brief	�ʒm���󂯂�I�u�U�[�o�[�̓o�^
	 */
	virtual ObserverFunction<Args...>* Subscribe(std::function<void(Args...)> f) = 0;

	/**
	 * @brief	�ʒm���󂯂�I�u�U�[�o�[�̍폜
	 */
	virtual void Dispose(Observer<Args...>* pobs) = 0;
};