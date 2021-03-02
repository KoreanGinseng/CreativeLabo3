#pragma once

#include	<vector>
#include	<algorithm>
#include	"Observable.h"

template <class... Args>
class Subject : public IObservable<Args...> {
private:
	/** �ʒm���󂯂�I�u�U�[�o�[���X�g */
	std::vector<Observer<Args...>*> observerList;
public:
	/**
	 * �R���X�g���N�^
	 */
	Subject()
		: observerList()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	virtual ~Subject()
	{
	}

	/**
	 * @brief	�ʒm���󂯂�I�u�U�[�o�[�̓o�^
	 */
	void Subscribe(Observer<Args...>* pobs) override {
		observerList.push_back(pobs);
	}

	/**
	 * @brief	�ʒm���󂯂�I�u�U�[�o�[�̓o�^
	 */
	ObserverFunction<Args...>* Subscribe(std::function<void(Args...)> f) override {
		ObserverFunction<Args...>* func = new ObserverFunction<Args...>(f);
		observerList.push_back(func);
		return func;
	}

	/**
	 * @brief	�ʒm���󂯂�I�u�U�[�o�[�̍폜
	 */
	void Dispose(Observer<Args...>* pobs) override {
		observerList.erase(std::remove_if(
			observerList.begin(), observerList.end(),
			[&](const Observer<Args...>* o) {return o == pobs; }),
			observerList.end());
	}

	/**
	 * @brief	�ʒm���\�b�h
	 */
	void Notify(Args... args) {
		for (auto& obj : observerList) {
			obj->Notify(args...);
		}
	}
};